/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: Media Recorder Adaptation for GST
*
*/

#include <string.h>
#include <gst/gst.h>
#include "xacapabilitiesmgr.h"
#include "xamediarecorderadaptctx.h"
#include "xaobjectitf.h"
/*forward declarations*/
GstElement* XAMediaRecorderAdapt_CreateEncodeBin(
        XAMediaRecorderAdaptationCtx* ctx);
XAresult XAMediaRecorderAdapt_CreatePipeline(
        XAMediaRecorderAdaptationCtx* ctx);


/*
 * gboolean XAMediaRecorderAdapt_GstBusCb( GstBus *bus, GstMessage *message, gpointer data )
 * MediaPlayer Gst-bus message handler (Callback)
 */
gboolean XAMediaRecorderAdapt_GstBusCb(GstBus *bus, GstMessage *message,
        gpointer data)
    {
    XAAdaptationGstCtx* bCtx = (XAAdaptationGstCtx*) data;
    XAMediaRecorderAdaptationCtx* mCtx = (XAMediaRecorderAdaptationCtx*)data;

    /* only listen to pipeline messages */
    DEBUG_API_A2_STR("->XAMediaRecorderAdapt_GstBusCb:\"%s\" from object \"%s\"",
                    GST_MESSAGE_TYPE_NAME(message), GST_OBJECT_NAME(GST_MESSAGE_SRC(message)));

    switch( GST_MESSAGE_TYPE(message))
        {
        case GST_MESSAGE_EOS:
            {
            if (GST_MESSAGE_SRC(message)==GST_OBJECT(bCtx->bin))
                {
                /* stop position tracking */
                if(mCtx->runpositiontimer > 0)
                    {
                    g_source_remove(mCtx->runpositiontimer);
                    mCtx->runpositiontimer=0;
                    }

                /* complete any ongoing client async operations */
                XAAdaptationGst_CompleteAsyncWait(bCtx);

                if(mCtx->positionCb)
                    {
                    mCtx->positionCb(bCtx);
                    }
                bCtx->binWantedState = GST_STATE_PAUSED;
                
                if(bCtx->busError)
                    {
                    XAObjItfCBData objdata = {XA_OBJECT_EVENT_RESOURCES_LOST, XA_RESULT_RESOURCE_ERROR, XA_OBJECT_STATE_UNREALIZED, NULL};
                    XAAdaptEvent event = {XA_OBJECTITFEVENTS, 0, sizeof(objdata), NULL };
                    event.data = &objdata;
                    /* send needed events */
                    
                    XAAdaptationBase_SendAdaptEvents(&bCtx->baseObj, &event );
                    }
                }
            break;
            }
        case GST_MESSAGE_STATE_CHANGED:
            {
            if (GST_MESSAGE_SRC(message)==GST_OBJECT(bCtx->bin))
                {
                    GstState oldstate, newstate, pendingstate, gsttargetstate;
                    gst_message_parse_state_changed(message, &oldstate, &newstate, &pendingstate);
                    gsttargetstate = GST_STATE_TARGET(bCtx->bin);
                    DEBUG_INFO_A4("old %d -> new %d (-> pending %d -> gsttarget %d)",
                                   oldstate, newstate, pendingstate, gsttargetstate);
                    if(gsttargetstate!=bCtx->binWantedState)
                    {
                        DEBUG_ERR_A1("WARNING: Gst target is not wanted target [%d]!!!", bCtx->binWantedState);
                    }
                    /* print out some more info */
                    if( pendingstate == GST_STATE_VOID_PENDING )
                    {
                        if( newstate != bCtx->binWantedState )
                        {
                            DEBUG_INFO_A2("Gst in intermediate state transition (curr %d, target %d)",
                                        newstate,bCtx->binWantedState);
                        }
                        else
                        {
                            DEBUG_INFO_A1("Gst in wanted target state (%d)",newstate);
                        }
                    }
                    if( oldstate!=GST_STATE_PLAYING && newstate==GST_STATE_PLAYING )
                    {
                        XAAdaptEvent event = {XA_RECORDITFEVENTS, XA_RECORDEVENT_HEADMOVING, 0, NULL };
                        /* send needed events */
                        XAAdaptationBase_SendAdaptEvents(&bCtx->baseObj, &event );
                        /* enable position tracking if needed */
                        if( mCtx->runpositiontimer==0 && mCtx->trackpositionenabled && mCtx->positionCb )
                        {
                            mCtx->runpositiontimer = g_timeout_add(XA_ADAPT_PU_INTERVAL, mCtx->positionCb, mCtx);
                        }
                    }
                }
                break;
            }

        case GST_MESSAGE_ASYNC_DONE:
            {
                /* some async sequence ended */
                XAAdaptationGst_CompleteAsyncWait(bCtx);
                break;
            }

        case GST_MESSAGE_ERROR:
            {
                gchar* debug;
                gst_message_parse_error(message, &(bCtx->busError), &debug);
                DEBUG_ERR_A1_STR("Gst reports error \"%s\"", debug);
                
                /* stop waiting any ongoing async operations */
                XAAdaptationGst_CompleteAsyncWait(bCtx);
                break;
            }
        case GST_MESSAGE_WARNING:
            {
                GError* error;
                gchar* debug;
                gst_message_parse_warning(message, &error, &debug);
                DEBUG_ERR_A1_STR("Gst reports warning \"%s\"", debug);
                /* stop waiting any ongoing async operations */
                XAAdaptationGst_CompleteAsyncWait(bCtx);
                break;
            }
        default:
            break;
        }
    DEBUG_API("<-XAMediaRecorderAdapt_GstBusCb");
    return TRUE;
    }

/*
 * XAAdaptationGstCtx* XAMediaRecorderAdapt_Create()
 * Allocates memory for Media Recorder Adaptation Context and makes 1st phase initialization
 * @param XADataSource *pAudioSrc - pointer to OMX-AL audio source
 * @param XADataSource *pImageVideoSrc - pointer image/video source
 * @param XADataSink *pDataSnk - pointer to OMX-AL sink
 * @returns XAMediaRecorderAdaptationCtx* - Pointer to created context, NULL if error occurs.
 */
XAAdaptationBaseCtx* XAMediaRecorderAdapt_Create(XADataSource* pAudioSrc,
        XADataSource* pImageVideoSrc, XADataSink* pDataSnk, XAuint8 recModes)
    {
    XAMediaRecorderAdaptationCtx *pSelf = NULL;
    DEBUG_API("->XAMediaRecorderAdapt_Create");

    pSelf = (XAMediaRecorderAdaptationCtx*)calloc(1, sizeof(XAMediaRecorderAdaptationCtx));
    if (pSelf)
        {
        if (XAAdaptationGst_Init(&(pSelf->baseObj),
                XAMediaRecorderAdaptation) != XA_RESULT_SUCCESS)
            {
            DEBUG_ERR("Failed to init base context!!!");
            free(pSelf);
            pSelf = NULL;
            DEBUG_API("<-XAMediaRecorderAdapt_Create");
            return NULL;
            }
        else
            {
            pSelf->baseObj.baseObj.fwtype = FWMgrFWGST;
            pSelf->xaAudioSource = pAudioSrc;
            pSelf->xaVideoSource = pImageVideoSrc;
            pSelf->xaSink = pDataSnk;
            pSelf->xaRecordState = XA_RECORDSTATE_STOPPED;
            pSelf->recModes = recModes;
            pSelf->isRecord = XA_BOOLEAN_FALSE;

            /* defaults from API spec */
            /* no specified defaults for rest, determined later from container type */
            pSelf->audioEncSettings.encoderId = XA_ADAPTID_UNINITED;
            pSelf->audioEncSettings.channelsIn = 0;
            pSelf->audioEncSettings.channelsOut = 0;
            pSelf->audioEncSettings.bitsPerSample = 0;
            pSelf->audioEncSettings.bitRate = 0;
            pSelf->audioEncSettings.sampleRate = 0;
            pSelf->audioEncSettings.rateControl = 0;
            pSelf->audioEncSettings.profileSetting = 0;
            pSelf->audioEncSettings.levelSetting = 0;
            pSelf->audioEncSettings.channelMode = 0;
            pSelf->audioEncSettings.streamFormat = 0;
            pSelf->audioEncSettings.encodeOptions = 0;
            pSelf->audioEncSettings.blockAlignment = 0;
            }
        }

    DEBUG_API("<-XAMediaRecorderAdapt_Create");
    return (XAAdaptationBaseCtx*) &pSelf->baseObj;
    }

/*
 * XAresult XAMediaRecorderAdapt_PostInit()
 * 2nd phase initialization of Media Recorder Adaptation Context
 * @param XAAdaptationGstCtx* ctx - pointer to Media Recorder adaptation context
 * @return XAresult - Success value
 */
XAresult XAMediaRecorderAdapt_PostInit(XAAdaptationGstCtx* bCtx)
    {
    GstStateChangeReturn gret;

    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaRecorderAdaptationCtx* ctx = NULL;
    DEBUG_API("->XAMediaRecorderAdapt_PostInit");
    if (bCtx == NULL || bCtx->baseObj.ctxId != XAMediaRecorderAdaptation)
        {
        DEBUG_ERR("Invalid parameter!!");
        DEBUG_API("<-XAMediaRecorderAdapt_PostInit");
        return XA_RESULT_PARAMETER_INVALID;
        }
    ctx = (XAMediaRecorderAdaptationCtx*) bCtx;

    ret = XAAdaptationGst_PostInit(bCtx);
    if (ret != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR("Gst context postinit failed!!");
        return ret;
        }

    /* top level bin for media recorder */
    bCtx->bin = gst_pipeline_new("media_recorder");

    /* Create Gst bus listener. */
    ret = XAAdaptationGst_InitGstListener(bCtx);
    if (ret != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR("Bus listener creation failed!!");
        return ret;
        }
    /* Add Media Recorder specific handler */
    if (bCtx->bus)
        {
        bCtx->busCb = XAMediaRecorderAdapt_GstBusCb;
        gst_bus_add_signal_watch(bCtx->bus);
        g_signal_connect(bCtx->bus, "message::eos", G_CALLBACK(bCtx->busCb), ctx );
        g_signal_connect(bCtx->bus, "message::error", G_CALLBACK(bCtx->busCb), ctx );
        g_signal_connect(bCtx->bus, "message::warning", G_CALLBACK(bCtx->busCb), ctx );
        g_signal_connect(bCtx->bus, "message::state-changed", G_CALLBACK(bCtx->busCb), ctx );
        g_signal_connect(bCtx->bus, "message::segment-done", G_CALLBACK(bCtx->busCb), ctx );
        g_signal_connect(bCtx->bus, "message::async-done", G_CALLBACK(bCtx->busCb), ctx );
        }
    else
        {
        DEBUG_ERR("Failed to create message bus");
        return XA_RESULT_INTERNAL_ERROR;
        }

    /* create pipeline */
    ret = XAMediaRecorderAdapt_CreatePipeline(ctx);
    if (ret != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR("Failed to create recorder pipeline");
        return XA_RESULT_INTERNAL_ERROR;
        }

#ifdef XA_IMPL_MEASURE_GST_DELAY
    bCtx->startTime = clock();
#endif /* XA_IMPL_MEASURE_GST_DELAY */
    
    /* roll up bin */
    bCtx->binWantedState = GST_STATE_READY;
    XAAdaptationGst_PrepareAsyncWait(bCtx);
    gret = gst_element_set_state(GST_ELEMENT(bCtx->bin), bCtx->binWantedState);
    if (gret == GST_STATE_CHANGE_ASYNC)
        {
        DEBUG_INFO("Wait for preroll");
        XAAdaptationGst_StartAsyncWait(bCtx);DEBUG_INFO("Preroll ready");
        }
    else if (gret == GST_STATE_CHANGE_FAILURE)
        {
        DEBUG_ERR("Preroll FAILED");
        /*ret = XA_RESULT_INTERNAL_ERROR;*/
        }
#ifdef XA_IMPL_MEASURE_GST_DELAY
    bCtx->endTime = clock();
    double diff = bCtx->endTime - bCtx->startTime;
    diff = diff / CLOCKS_PER_SEC;
    DEBUG_API_A1( "Starting up bin took %.4lf secs",diff);
#endif /* XA_IMPL_MEASURE_GST_DELAY */


    bCtx->waitingasyncop = XA_BOOLEAN_FALSE;

    DEBUG_API("<-XAMediaRecorderAdapt_PostInit");
    return ret;
    }

/*
 * void XAMediaRecorderAdapt_Destroy( XAAdaptationGstCtx* bCtx )
 * Destroys Media Recorder Adaptation Context
 * @param ctx - Media Recorder Adaptation context to be destroyed
 */
void XAMediaRecorderAdapt_Destroy(XAAdaptationGstCtx* bCtx)
    {
    XAMediaRecorderAdaptationCtx* ctx = NULL;
    char* fname = NULL;
    DEBUG_API("->XAMediaRecorderAdapt_Destroy");

    if (bCtx == NULL || bCtx->baseObj.ctxId != XAMediaRecorderAdaptation)
        {
        DEBUG_ERR("Invalid parameter!!");
        DEBUG_API("<-XAMediaRecorderAdapt_Destroy");
        return;
        }
    ctx = (XAMediaRecorderAdaptationCtx*) bCtx;

    if (ctx->isRecord == XA_BOOLEAN_FALSE)
        {
        DEBUG_INFO("Removing unnecessary file.");

        if (ctx->xaSink && *((XAuint32*) (ctx->xaSink->pLocator))
                == XA_DATALOCATOR_URI)
            {
            if (strncmp(
                    (char *) ((XADataLocator_URI*) (ctx->xaSink->pLocator))->URI,
                    "file:///", 8) == 0)
                {
                fname
                        = (char *) &(((XADataLocator_URI*) (ctx->xaSink->pLocator))->URI[8]);
                }
            else
                {
                fname
                        = (char *) ((XADataLocator_URI*) (ctx->xaSink->pLocator))->URI;
                }

            if (remove(fname) != 0)
                {
                DEBUG_ERR_A1_STR("Cannot remove file %s", (char *)fname);
                }
            }
        }

    if (bCtx->bus)
        {
        gst_bus_remove_signal_watch(bCtx->bus);
        }
    XAAdaptationGst_CancelAsyncWait(bCtx);

    if (ctx->runpositiontimer)
        {
        g_source_remove(ctx->runpositiontimer);
        }
    
    XAAdaptationGst_Free(bCtx);

    free(ctx);
    ctx = NULL;

    DEBUG_API("<-XAMediaRecorderAdapt_Destroy");
    }

/***************** INTERNAL FUNCTIONS *******************************/

    /*
     * void XAMediaRecorderAdapt_CreatePipeline( XAMediaRecorderAdaptationCtx* ctx );
     */
XAresult XAMediaRecorderAdapt_CreatePipeline(
        XAMediaRecorderAdaptationCtx* ctx)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMediaRecorderAdapt_CreatePipeline");

    /* Create the audio src */
    if ( ctx->xaAudioSource )
        {
        /* create audio pipe source */
        ctx->audiosource = XAAdaptationGst_CreateGstSource( ctx->xaAudioSource, "audiosource",
                NULL );
    
        if( ctx->audiosource )
            {
            if (gst_bin_add(GST_BIN(ctx->baseObj.bin), ctx->audiosource))
                {
                DEBUG_API("Added audiosource to bin");
                }
            else
                {
                DEBUG_API("Could not add audiosource to bin");
                return XA_RESULT_INTERNAL_ERROR;
                }
            }
        else
            {
            DEBUG_ERR("Could not create audio source!!!!");
            return XA_RESULT_INTERNAL_ERROR;
            }
        }

    /* create and add video pipeline */
    ctx->codecbin = XAMediaRecorderAdapt_CreateEncodeBin(ctx);
    if (ctx->codecbin)
        {
        if (gst_bin_add(GST_BIN(ctx->baseObj.bin), ctx->codecbin))
            {
            DEBUG_API("->XAMediaRecorderAdapt_CreatePipeline: gst_bin_add success");
            }
        else
            {
            DEBUG_ERR("Could not add codec bin");
            return XA_RESULT_INTERNAL_ERROR;
            }
        }
    else
        {
        DEBUG_ERR("Could not create encoding bin!!!");
        return XA_RESULT_INTERNAL_ERROR;
        }

    /* create and add audio pipeline */
    if ( ctx->audiosource )
        {    
        //if(!gst_element_link(ctx->audiofilter, ctx->codecbin ))
        if(!gst_element_link(ctx->audiosource, ctx->codecbin ))
            {
            DEBUG_ERR("Could not link audiosource to codecbin!!");
            return XA_RESULT_INTERNAL_ERROR;
            }
        else
            {
            DEBUG_INFO("Warning: link audiosource to codecbin is successfull with muxcaps!!");
            }
        }
    else
        {
        DEBUG_INFO("No audio input");
        }

    /* create and add data sink */
    ctx->datasink = XAAdaptationGst_CreateGstSink( ctx->xaSink, "datasink" );
    if( ctx->datasink )
        {
        if (gst_bin_add(GST_BIN(ctx->baseObj.bin), ctx->datasink))
            {
            DEBUG_INFO("Added datasink to bin");
            }
        else
            {
            DEBUG_ERR("Could not add datasink to bin");
            return XA_RESULT_INTERNAL_ERROR;
            }
        if(!gst_element_link(ctx->codecbin, ctx->datasink))
            {
            DEBUG_ERR("Could not link codec to sink!!");
            return XA_RESULT_INTERNAL_ERROR;
            }
        }
    else
        {
        DEBUG_ERR("Could not create data sink!!!");
        return XA_RESULT_INTERNAL_ERROR;
        }
    DEBUG_API("<-XAMediaRecorderAdapt_CreatePipeline");
    return ret;
    }

/*
 * XAresult XAMediaRecorderAdapt_CheckCodec( XAMediaRecorderAdaptationCtx_* mCtx );
 * Check codec compatibility and support with initiated datasink content
 */
XAresult XAMediaRecorderAdapt_CheckCodec( XAMediaRecorderAdaptationCtx_* mCtx, XACapsType encType, XAuint32 encoderId )
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 format = 0;
    XACapabilities temp;

    DEBUG_API("->XAMediaRecorderAdapt_CheckCodec");

    /*first, check if codec supported at all*/
    ret = XACapabilitiesMgr_GetCapsById(mCtx->baseObj.baseObj.capslist,(XACapsType)(XACAP_ENCODER|encType), encoderId, &temp);

    if( ret==XA_RESULT_SUCCESS )
        {
        if(encType & (XACAP_VIDEO|XACAP_AUDIO))
            {
            if(mCtx->xaSink && mCtx->xaSink->pFormat)
                {
                format = *(XAuint32*)(mCtx->xaSink->pFormat);
                }
            else
                {
                ret=XA_RESULT_FEATURE_UNSUPPORTED;
                }
            }
        
        if(ret==XA_RESULT_SUCCESS)
            { 
            switch ( format )
                {
                case XA_DATAFORMAT_PCM:
                    if ( (encType == XACAP_AUDIO) && (encoderId == XA_AUDIOCODEC_PCM) )
                        {
                        ret=XA_RESULT_SUCCESS;
                        }
                    else
                        {
                        ret=XA_RESULT_FEATURE_UNSUPPORTED;
                        }
                    break;

                case XA_DATAFORMAT_MIME:
                    DEBUG_INFO("XA_DATAFORMAT_MIME ");
                        {
                        XADataFormat_MIME* mime = ((XADataFormat_MIME*)mCtx->xaSink->pFormat);
                        DEBUG_INFO_A1("mime->containerType:%u",(int)mime->containerType);
                    DEBUG_INFO_A1_STR("mime->mimeType:%s", (char *)mime->mimeType);
                        switch ( mime->containerType )
                            {
                            case XA_CONTAINERTYPE_RAW:
                                if((encType == XACAP_AUDIO) && (encoderId == XA_AUDIOCODEC_PCM))
                                    {
                                    ret=XA_RESULT_SUCCESS;
                                    }
                                else
                                    {
                                    ret=XA_RESULT_FEATURE_UNSUPPORTED;
                                    }
                                break;

                            case XA_CONTAINERTYPE_WAV:
                                if(encType == XACAP_AUDIO)
                                    {
                                    switch(encoderId)
                                        {
                                        case XA_AUDIOCODEC_PCM:
                                            ret=XA_RESULT_SUCCESS;
                                            break;
                                        default:
                                            ret=XA_RESULT_FEATURE_UNSUPPORTED;
                                            break;
                                        }
                                    }
                                else
                                    {
                                    ret=XA_RESULT_FEATURE_UNSUPPORTED;
                                    }
                                break;

                            case XA_CONTAINERTYPE_UNSPECIFIED:
                                    {
                                    ret=XA_RESULT_FEATURE_UNSUPPORTED;
                                    }
                                break;
                            case XA_CONTAINERTYPE_AMR:
                                if(encType == XACAP_AUDIO)
                                    {
                                    switch(encoderId)
                                        {
                                        case XA_AUDIOCODEC_AMR:
                                            ret=XA_RESULT_SUCCESS;
                                            break;
                                        default:
                                            ret=XA_RESULT_FEATURE_UNSUPPORTED;
                                        break;
                                        }
                                    }
                                else
                                    {
                                    ret=XA_RESULT_FEATURE_UNSUPPORTED;
                                    }
                                break;
                            case XA_CONTAINERTYPE_MP4:
                                if(encType == XACAP_AUDIO )
                                    {
                                    switch(encoderId)
                                        {
                                        case XA_AUDIOCODEC_AAC:
                                            ret=XA_RESULT_SUCCESS;
                                            break;
                                        default:
                                            ret=XA_RESULT_FEATURE_UNSUPPORTED;
                                            break;
                                        }
                                    }
                                else
                                    {
                                    ret=XA_RESULT_FEATURE_UNSUPPORTED;
                                    }
                                break;
                            default: /*switch (containertype)*/
                                ret = XA_RESULT_FEATURE_UNSUPPORTED;
                                break;
                            }
                        break;
                        }
                default: /*switch (format)*/
                    ret = XA_RESULT_FEATURE_UNSUPPORTED;
                    break;
                }
            }
        }
    if( ret!=XA_RESULT_SUCCESS )
        {
        DEBUG_ERR("cannot accommodate given codec & datasink pair!!!");
        }
    DEBUG_API("<-XAMediaRecorderAdapt_CheckCodec");
    return ret;
    }
/*
 * XAresult XAMediaRecorderAdapt_ChangeEncoders( XAMediaRecorderAdaptationCtx* mCtx );
 * re-create encodebin based on new encoder settings
 */
XAresult XAMediaRecorderAdapt_ChangeEncoders( XAMediaRecorderAdaptationCtx* mCtx )
    {
    XAresult ret = XA_RESULT_SUCCESS;
    GstElement  *encoderelement = NULL;
    XAAdaptationGstCtx* bCtx = &(mCtx->baseObj);

    DEBUG_API("->XAMediaRecorderAdapt_ChangeEncoders");
    /* check state */
    if(GST_STATE(mCtx->baseObj.bin)<GST_STATE_PLAYING)
        {
        GstElement* newBin = XAMediaRecorderAdapt_CreateEncodeBin(mCtx);
        if(newBin)
            { /* replace old bin with new */
            GstStateChangeReturn gret;
            GstPad *asink=NULL, *linkedasrc=NULL;
            GstPad *src=NULL, *linkedsink=NULL;
            GstCaps* encSrcCaps = NULL;
            GstPad *afiltsrc=NULL, *afiltsink=NULL;

            /* pipeline must be unrolled for renegotiation */
            DEBUG_INFO("Unroll pipeline");
            bCtx->binWantedState = GST_STATE_READY;
            gret = gst_element_set_state( GST_ELEMENT(bCtx->bin), bCtx->binWantedState);
            if(gret == GST_STATE_CHANGE_FAILURE)
                {
                DEBUG_ERR("Failed to Unroll Pipeline!!");
                return XA_RESULT_INTERNAL_ERROR;
                }
            
            gret = gst_element_get_state( GST_ELEMENT(bCtx->bin), NULL, NULL, XA_ADAPT_ASYNC_TIMEOUT_SHORT_NSEC);

            if(!mCtx ->audiofilter)
                {
                   //creating caps filter 
                   mCtx->audiofilter = gst_element_factory_make("capsfilter", "audiofilter");
                   if ( mCtx->audiofilter )
                    {
                       gst_bin_add(GST_BIN(bCtx->bin), mCtx->audiofilter);
                       encSrcCaps = gst_caps_new_full(
                       gst_structure_new("audio/x-raw-int",
                               "channels", G_TYPE_INT, mCtx->audioEncSettings.channelsOut,
                               "rate", G_TYPE_INT, (mCtx->audioEncSettings.sampleRate / 1000),
                               "depth", G_TYPE_INT, mCtx->audioEncSettings.bitsPerSample,
                               "signed",G_TYPE_BOOLEAN, TRUE,
                               "endianness",G_TYPE_INT, mCtx->audioEncSettings.blockAlignment,
                               NULL),
                       gst_structure_new("audio/amr",
                              "signed",G_TYPE_BOOLEAN, TRUE,
                              "endianness",G_TYPE_INT, mCtx->audioEncSettings.blockAlignment,
                              "depth", G_TYPE_INT, mCtx->audioEncSettings.bitsPerSample,
                              "rate", G_TYPE_INT, (mCtx->audioEncSettings.sampleRate / 1000),
                              "channels", G_TYPE_INT, mCtx->audioEncSettings.channelsOut,
                              NULL),
                        gst_structure_new("audio/x-raw-float",
                               "channels", G_TYPE_INT, mCtx->audioEncSettings.channelsOut,
                               "width", G_TYPE_INT, mCtx->audioEncSettings.bitsPerSample,
                               "rate", G_TYPE_INT , mCtx->audioEncSettings.sampleRate / 1000,
                               NULL),
                               NULL);
                       DEBUG_INFO_A1_STR("new audio encoder config from settings: %s",gst_caps_to_string(encSrcCaps));
                       g_object_set( G_OBJECT(mCtx->audiofilter), "caps",encSrcCaps,NULL);
                                                             
                       gst_caps_unref(encSrcCaps);

                    }else
                        {
                        DEBUG_ERR("cannot create caps filter");
                        return XA_RESULT_INTERNAL_ERROR;
                        }
                    
                }

            DEBUG_INFO("Unlink and remove old encodebin");
            asink = gst_element_get_static_pad(mCtx->codecbin,"sink");
            if(asink)
                {
                linkedasrc = gst_pad_get_peer(asink);
                if(linkedasrc)
                    {
                    gst_pad_unlink(linkedasrc,asink);
                    }
                }

            src = gst_element_get_static_pad(mCtx->codecbin,"src");
            if(src)
                {
                linkedsink = gst_pad_get_peer(src);
                if(linkedsink)
                    {
                    gst_pad_unlink(src,linkedsink);
                    }
                }

            gst_element_set_state( GST_ELEMENT(mCtx->codecbin), GST_STATE_NULL );
            gst_element_get_state( GST_ELEMENT(mCtx->codecbin),NULL,NULL,1000 );
            gst_bin_remove( GST_BIN(bCtx->bin), mCtx->codecbin );
            /*reset filesink too*/
            gst_element_set_state(GST_ELEMENT(mCtx->datasink),GST_STATE_NULL);
            gst_element_sync_state_with_parent(mCtx->datasink);
            gst_element_get_state(mCtx->datasink,NULL,NULL,XA_ADAPT_ASYNC_TIMEOUT_SHORT_NSEC);

            DEBUG_INFO("Link new encodebin");
            mCtx->codecbin = newBin;
            gst_bin_add(GST_BIN(bCtx->bin), mCtx->codecbin);
            asink = gst_element_get_static_pad(mCtx->codecbin,"sink");
            afiltsink = gst_element_get_static_pad(mCtx->audiofilter,"sink");
            afiltsrc = gst_element_get_static_pad(mCtx->audiofilter,"src");
            if(linkedasrc && afiltsink)
                {
                    if(gst_pad_link(linkedasrc , afiltsink ) != GST_PAD_LINK_OK)
                        {
                        DEBUG_ERR("Could not link audiosource to audiofilter!!");
                        return XA_RESULT_INTERNAL_ERROR;
                        }
                }
            if(asink && afiltsrc)
                {
                    if(gst_pad_link(afiltsrc , asink) != GST_PAD_LINK_OK)
                        {
                        DEBUG_ERR("Could not link audiosource to audiofilter!!");
                        return XA_RESULT_INTERNAL_ERROR;
                        }
                }
            src = gst_element_get_static_pad(mCtx->codecbin,"src");
            if(src && linkedsink)
                {
                        if(gst_pad_link(src,linkedsink) != GST_PAD_LINK_OK)
                            {
                            DEBUG_ERR("Could not link codecbin src pad  to linkedsink!!");
                            return XA_RESULT_INTERNAL_ERROR;
                            }
                }

            //setting the Bitrate and other properties for elements
                if(mCtx->audioEncSettings.encoderId == XA_AUDIOCODEC_AAC)
                    {   
                    encoderelement = gst_bin_get_by_name((GstBin*)mCtx->codecbin, "audioenc") ;
                    if(encoderelement)
                        {
                        g_object_set(G_OBJECT(encoderelement),"bitrate" , mCtx->audioEncSettings.bitRate , NULL );
                        g_object_set(G_OBJECT(encoderelement),"profile", mCtx->audioEncSettings.levelSetting , NULL );
                        g_object_set(G_OBJECT(encoderelement),"output-format", mCtx->audioEncSettings.streamFormat , NULL );
                        gst_object_unref (encoderelement);
                        }
                    else
                        {
                         DEBUG_ERR("Encoder Element not found for AAC");
                        }
                    }
                else if(mCtx->audioEncSettings.encoderId == XA_AUDIOCODEC_AMR)
                    {
                    g_object_set(G_OBJECT(mCtx->audiosource),"speechbitrate", mCtx->audioEncSettings.bitRate , NULL );
                    }
                else
                    {
                    DEBUG_INFO("No properties for PCM or Wav");
                    }       
                
            DEBUG_INFO_A1_STR("Pipeline in state %s",gst_element_state_get_name(GST_STATE(bCtx->bin)));
            }
        else
            {
            /* could not support wanted encoders */
            DEBUG_ERR("Failed to create encodebin with new settings, using old one!");
            ret = XA_RESULT_FEATURE_UNSUPPORTED;
            }
        }
    else
        { /* n/a while playing */
        DEBUG_ERR("Cannot change encoder when recording ongoing!");
        ret = XA_RESULT_PRECONDITIONS_VIOLATED;
        }
    DEBUG_API("<-XAMediaRecorderAdapt_ChangeEncoders");
    return ret;
    }

/**
 * GstElement* XAMediaRecorderAdapt_CreateEncodeBin( XAMediaRecorderAdaptationCtx* ctx )
 * @return GstElement* - pointer to created bin element
 * Decription: Create encoder/muxer element based on given format and encoder settings
 */
GstElement* XAMediaRecorderAdapt_CreateEncodeBin( XAMediaRecorderAdaptationCtx* ctx )
    {
    XAresult ret = XA_RESULT_SUCCESS;
    GstElement *audioenc = NULL, *mux=NULL;
    GstElement *codecbin = gst_bin_new( "mrcodecbin" );
    GstPad *ghostsrc = NULL, *ghostaudiosink = NULL;
    XAuint32 format;
    XACapabilities temp;

    DEBUG_API("->XAMediaRecorderAdapt_CreateEncodeBin");
    if(ctx->recModes & XA_RECMODE_STREAM)
        {
        if(ctx->xaSink && ctx->xaSink->pFormat)
            {
            format = *(XAuint32*)(ctx->xaSink->pFormat);
            switch ( format )
                {
                case XA_DATAFORMAT_PCM:
                    DEBUG_INFO("XA_DATAFORMAT_PCM");
                        {
                        XADataFormat_PCM* pcm = ((XADataFormat_PCM*)ctx->xaSink->pFormat);
                        if(!ctx->xaAudioSource)
                            {
                            DEBUG_ERR("Unsupported dataformat for given data sources");
                            return NULL;
                            }
                        /* no need for codec, just pass data on */
                        mux = gst_element_factory_make("identity", "mux");
                        gst_bin_add(GST_BIN(codecbin), mux);
                        ghostsrc = gst_element_get_static_pad(mux,"src");
                        ghostaudiosink = gst_element_get_static_pad(mux,"sink");
                        /*set usable audio settings from the sink structure*/
                        ctx->audioEncSettings.encoderId = XA_AUDIOCODEC_PCM;
                        ctx->audioEncSettings.channelsOut = pcm->numChannels;
                        ctx->audioEncSettings.bitsPerSample = pcm->bitsPerSample;
                        }
                    break;

                case XA_DATAFORMAT_MIME:
                    DEBUG_INFO("XA_DATAFORMAT_MIME ");
                    {
                    XADataFormat_MIME* mime = ((XADataFormat_MIME*)ctx->xaSink->pFormat);
                    DEBUG_INFO_A1("mime->containerType:%u",(int)mime->containerType);
                    DEBUG_INFO_A1_STR("mime->mimeType:%s", (char *)mime->mimeType);
                    switch ( mime->containerType )
                        {
                        case XA_CONTAINERTYPE_WAV:
                            {
                            DEBUG_INFO("XA_CONTAINERTYPE_WAV");
                            audioenc = gst_element_factory_make("wavenc", "audioenc");
                            if(audioenc)
                                {
                                if (gst_bin_add(GST_BIN(codecbin), audioenc))
                                    {
                                    DEBUG_INFO("added audioenc to codecbin");
                                    }
                                else
                                    {
                                    DEBUG_ERR("Could not add audioenc to codecbin");
                                    return NULL;
                                    }
                                /*use this as linkpoint*/
                                ghostsrc = gst_element_get_static_pad(audioenc,"src");
                                ghostaudiosink = gst_element_get_static_pad(audioenc,"sink");
                                if ( ghostsrc == NULL || ghostaudiosink == NULL)
                                    {
                                    DEBUG_ERR("Could not get src or sink ghoset element(s)");
                                    return NULL;
                                    }
                                }
                            /* no other codecs needed */
                            break;
                            }

                        case XA_CONTAINERTYPE_UNSPECIFIED:
                            {
                            DEBUG_INFO("No support for requested encoder...try to select encoder from mime string");
                            DEBUG_INFO("No support for requested mime/container type.");
                            ret = XA_RESULT_CONTENT_UNSUPPORTED;
                            break;
                            }
                        case XA_CONTAINERTYPE_MP4:
                            {
                            DEBUG_INFO("XA_CONTAINERTYPE_MP4");
                            mux = gst_element_factory_make("mp4mux", "mp4mux");
                            if(mux)
                                {
                                if (gst_bin_add(GST_BIN(codecbin), mux))
                                    {
                                    DEBUG_INFO("Added mux to codecbin");
                                    }
                                else
                                    {
                                    DEBUG_ERR("Could not add mux to codecbin");
                                    return NULL;
                                    }
                                /*use this as linkpoint*/
                                ghostsrc = gst_element_get_static_pad(mux,"src");
                                }
                            /* Add and link audio/video codecs */
    
                            /*set audio defaults*/
                            if(ctx->audioEncSettings.encoderId == XA_ADAPTID_UNINITED)
                                ctx->audioEncSettings.encoderId = XA_AUDIOCODEC_AAC;
                            if(ctx->xaAudioSource)
                                {
                                if(XACapabilitiesMgr_GetCapsById(ctx->baseObj.baseObj.capslist, (XACapsType)(XACAP_ENCODER|XACAP_AUDIO), ctx->audioEncSettings.encoderId, &temp) == XA_RESULT_SUCCESS)
                                    {
                                    if(temp.adaptId)
                                        {
                                        audioenc = gst_element_factory_make((char*)temp.adaptId, "audioenc");
                                        }
                                    }
                                if(audioenc)
                                    {
                                    gst_bin_add(GST_BIN(codecbin), audioenc);
                                    if(!gst_element_link(audioenc, mux))
                                        {
                                        DEBUG_ERR("Could not link audioenc to mux!!");
                                        DEBUG_API("<-XAMediaRecorderAdapt_CreateEncodeBin");
                                        return NULL;
                                        }
                                    ghostaudiosink = gst_element_get_static_pad(audioenc,"sink");
                                    }
                                else
                                    {
                                    /*no audio codec but audio source = PCM case, explicity request audio pad*/
                                    ghostaudiosink = gst_element_get_request_pad(mux,"audio_%d");
                                    }
                                }
                            break;	
                            }						
                        case XA_CONTAINERTYPE_AMR:
                            {
                            DEBUG_INFO("XA_CONTAINERTYPE_AMR");
                            if(ctx->audioEncSettings.encoderId == XA_ADAPTID_UNINITED)
                                ctx->audioEncSettings.encoderId = XA_AUDIOCODEC_AMR;
                             mux = gst_element_factory_make("amrmux", "mux");
                             if(mux)
                                 {
                                 if (gst_bin_add(GST_BIN(codecbin), mux))
                                     {
                                     DEBUG_INFO("Added mux to codecbin");
                                     /*Setting the buffer size on src since amr generates 
                                      * small amounts of data */
                                     g_object_set (G_OBJECT (ctx->audiosource),
                                               "blocksize", 1280,
                                                NULL);                                         
                                     }
                                 else
                                     {
                                     DEBUG_ERR("Could not add mux to codecbin");
                                     return NULL;
                                     }
                                 /*use this as linkpoint*/
                                 ghostsrc = gst_element_get_static_pad(mux,"src");
                                 }
                             /*set audio defaults*/
                             if(ctx->xaAudioSource)
                                 {
                                 /*no audio codec but audio source = PCM case, explicity request audio pad*/
                                 ghostaudiosink = gst_element_get_static_pad(mux,"sink");
                                 }
                             break;     
                            }                           
                        
/*                        case XA_CONTAINERTYPE_3GPP:
                        case XA_CONTAINERTYPE_MOBILE_DLS:
                        case XA_CONTAINERTYPE_BMP:
                        case XA_CONTAINERTYPE_ASF:
                        case XA_CONTAINERTYPE_M4A:
                        case XA_CONTAINERTYPE_MP3:
                        case XA_CONTAINERTYPE_JPG2000:
                        case XA_CONTAINERTYPE_MPEG_ES:
                        case XA_CONTAINERTYPE_MPEG_PS:
                        case XA_CONTAINERTYPE_MPEG_TS:
                        case XA_CONTAINERTYPE_QT:
                        case XA_CONTAINERTYPE_XMF_0:
                        case XA_CONTAINERTYPE_XMF_1:
                        case XA_CONTAINERTYPE_XMF_2:
                        case XA_CONTAINERTYPE_XMF_3:
                        case XA_CONTAINERTYPE_XMF_GENERIC:
                        case XA_CONTAINERTYPE_AAC:
                        case XA_CONTAINERTYPE_3GA:
                        case XA_CONTAINERTYPE_RM:
                        case XA_CONTAINERTYPE_DMF:*/
                            
                        default:
                            DEBUG_INFO("No support for requested container type.");
                            ret = XA_RESULT_CONTENT_UNSUPPORTED;
                            break;
                        }
                    break;
                    }
                default:
                    DEBUG_ERR("Incorrect data format type.");
                    ret = XA_RESULT_PARAMETER_INVALID;
                break;
                }
            }
        else
            {
            DEBUG_ERR("Invalid data sink for stream recording!!");
            ret = XA_RESULT_PARAMETER_INVALID;
            }
        }
    else
        {/* stream recording not requested, datasink ignored, use uncoded recordstream*/
        mux = gst_element_factory_make("identity", "mux");
        gst_bin_add(GST_BIN(codecbin), mux);
        ghostsrc = gst_element_get_static_pad(mux,"src");
        }

    /*set default codecs for unrecognized*/
    if(ctx->audioEncSettings.encoderId == XA_ADAPTID_UNINITED)
    ctx->audioEncSettings.encoderId = XA_AUDIOCODEC_PCM;

    if ( ret != XA_RESULT_SUCCESS )
        {
        gst_object_unref(codecbin);
        codecbin=NULL;
        }
    else
        {
        /*add ghost pad(s) to link to*/
        if(ghostsrc)
            {
            gst_element_add_pad(codecbin, gst_ghost_pad_new("src",ghostsrc));
            gst_object_unref(GST_OBJECT(ghostsrc));
            }
        if(ghostaudiosink)
            {
            gst_element_add_pad(codecbin, gst_ghost_pad_new("sink",ghostaudiosink));
            gst_object_unref(GST_OBJECT(ghostaudiosink));
            }

        DEBUG_INFO_A1("Created encoder bin at %x", (int)codecbin);
        }

    DEBUG_API("<-XAMediaRecorderAdapt_CreateEncodeBin");
    return codecbin;

    }


