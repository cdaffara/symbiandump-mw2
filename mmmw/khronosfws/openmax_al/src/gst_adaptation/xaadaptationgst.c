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
 * Description: Base GStreamer Adaptation Source
 *
 */

#include <string.h>
#include <gst/gst.h>
#include <gst/gstbuffer.h>
#include <stdlib.h>
#ifdef INPUTSRCSELECTION_FEATURE
#include <gst/interfaces/gstaudioinputinterface.h>
#endif
#include "xaobjectitf.h"
#include "xaadaptationgst.h"
/*
 * XAAdaptationGstCtx* XAAdaptationGst_Create()
 * 1st phase initialization function for Adaptation Base context structure.
 * Reserves memory for base context and initializes GStreamer FW.
 */
XAresult XAAdaptationGst_Init(XAAdaptationGstCtx* pSelf, XAuint32 ctxId)
    {
    DEBUG_API("->XAAdaptationGst_Init");

    if (pSelf)
        {
        if (XAAdaptationBase_Init(&(pSelf->baseObj), ctxId)
                != XA_RESULT_SUCCESS)
            {
            DEBUG_ERR("Failed to init base context!!!");
            free(pSelf);
            pSelf = NULL;
            }
        else
            {
            GError* gerror = 0;
            /*         Add default handler for Gst-bus messages */
            //pSelf->busCb = XAAdaptationGst_GstBusCb;
            pSelf->cond_mutx_inited = XA_BOOLEAN_FALSE;
#ifdef PROFILE_HEAP_USAGE
            xa_heap_total_alloc_print("Before GST Init");
#endif
            sem_init(&(pSelf->semAsyncWait), 0, 0);
            if ( !gst_init_check( NULL, NULL, &gerror ) )
                {
                    DEBUG_ERR("Gst Initalization failure.");
                    return XA_RESULT_INTERNAL_ERROR;
                }
#ifdef PROFILE_HEAP_USAGE
            xa_heap_total_alloc_print("After GST Init");
#endif            
            }
        }
    else
        {
        DEBUG_ERR("Invalid Gst Base Context.");
        return XA_RESULT_PARAMETER_INVALID;
        }

    DEBUG_API("<-XAAdaptationGst_Init");
    return XA_RESULT_SUCCESS;
    }

/*
 * XAresult XAAdaptationGst_PostInit()
 * 2nd phase initialization for Adaptation Base.
 */
XAresult XAAdaptationGst_PostInit(XAAdaptationGstCtx* ctx)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAdaptationGst_PostInit");
    XAAdaptationBase_PostInit(&ctx->baseObj);

    ctx->thread_launched = XA_BOOLEAN_FALSE;
    pthread_mutex_init(&(ctx->ds_mutex), NULL);
    pthread_cond_init(&(ctx->ds_condition), NULL);
    ctx->cond_mutx_inited = XA_BOOLEAN_TRUE;

    DEBUG_API("<-XAAdaptationGst_PostInit");
    return ret;
    }

/*
 * void XAAdaptationGst_Free( XAAdaptationGstCtx* ctx )
 * Frees all Base context variables .
 */
void XAAdaptationGst_Free(XAAdaptationGstCtx* ctx)
    {
    GstElement* fakesink = NULL;
    DEBUG_API("->XAAdaptationGst_Free");

    if (ctx->bin)
        {
        fakesink = gst_bin_get_by_name(GST_BIN(ctx->bin), "fakesink");
        if (fakesink)
            {
            gst_element_set_state(GST_ELEMENT(fakesink), GST_STATE_NULL);
            gst_object_unref(fakesink);
            }

        if (gst_element_set_state(GST_ELEMENT(ctx->bin), GST_STATE_NULL)
                != GST_STATE_CHANGE_SUCCESS)
            { /*not much we can do*/
            DEBUG_ERR("WARNING: Failed to change to NULL state before deletion!!");
            }
        }

    if (ctx->asynctimer)
        { /*cancel timer*/
        g_source_remove(ctx->asynctimer);
        }
    sem_post(&(ctx->semAsyncWait));
    sem_destroy(&(ctx->semAsyncWait));

    XAAdaptationGst_StopGstListener(ctx);

    if (ctx->bin)
        {
        gst_object_unref(ctx->bin);
        }

    if (ctx->cond_mutx_inited == XA_BOOLEAN_TRUE)
        {
        ctx->thread_launched = XA_BOOLEAN_FALSE;
        pthread_mutex_destroy(&(ctx->ds_mutex));
        pthread_cond_destroy(&(ctx->ds_condition));
        ctx->cond_mutx_inited = XA_BOOLEAN_FALSE;
        }
    XAAdaptationBase_Free(&ctx->baseObj);

    DEBUG_API("<-XAAdaptationGst_Free");
    }

XAresult XAAdaptationGst_InitGstListener(XAAdaptationGstCtx* ctx)
    {
    int ret;
    DEBUG_API("->XAAdaptationGst_InitGstListener");
    if (ctx->bin)
        {
        ctx->bus = gst_pipeline_get_bus(GST_PIPELINE( ctx->bin ));
        }
    if (!ctx->bus)
        {
        DEBUG_ERR("could not get gst bus!");
        return XA_RESULT_INTERNAL_ERROR;
        }
    ret = pthread_create(&(ctx->busloopThr), NULL,
            (XAAdaptationGst_LaunchGstListener), (void*) ctx);
    if (ctx->thread_launched == XA_BOOLEAN_FALSE)
        {
        // Wait until the thread is created
        pthread_mutex_lock(&(ctx->ds_mutex));
        pthread_cond_wait(&(ctx->ds_condition), &(ctx->ds_mutex));
        pthread_mutex_unlock(&(ctx->ds_mutex));
        }
    if (ret)
        {
        DEBUG_ERR_A1("could not create thread!! (%d)",ret);
        return XA_RESULT_INTERNAL_ERROR;
        }
    DEBUG_API("<-XAAdaptationGst_InitGstListener");
    return XA_RESULT_SUCCESS;
    }

void * XAAdaptationGst_LaunchGstListener(void* args)
    {
    XAAdaptationGstCtx* ctx = (XAAdaptationGstCtx*) args;
    DEBUG_API("->XAAdaptationGst_LaunchGstListener");
    // Signal calling thread that this thread creation is completed
    ctx->thread_launched = XA_BOOLEAN_TRUE;
    pthread_mutex_lock(&(ctx->ds_mutex));
    pthread_cond_signal(&(ctx->ds_condition));
    pthread_mutex_unlock(&(ctx->ds_mutex));

    ctx->busloop = g_main_loop_new(NULL, FALSE);
    if (!ctx->busloop)
        {
        DEBUG_ERR("Glib main loop failure.");
        DEBUG_API("<-XAAdaptationGst_LaunchGstListener");
        }
    else
        {
        DEBUG_INFO("Start Glib main loop");
        g_main_loop_run(ctx->busloop);
        DEBUG_INFO("Glib main loop stopped - exiting thread");
        DEBUG_API("<-XAAdaptationGst_LaunchGstListener");
        pthread_exit(NULL);
        }
    return NULL;
    }

void XAAdaptationGst_StopGstListener(XAAdaptationGstCtx* ctx)
    {
    DEBUG_API("->XAAdaptationGst_StopGstListener");
    if (ctx->busloop)
        {
        g_main_loop_quit(ctx->busloop);
        g_main_loop_unref(ctx->busloop);
        }
    if (ctx->bus)
        {
        gst_object_unref(ctx->bus);
        ctx->bus = NULL;
        }
    DEBUG_API("<-XAAdaptationGst_StopGstListener");
    }

/*
 * ASynchronous operation managing
 **/

/* NOTE: This should NOT be called from gst callbacks - danger of deadlock!!
 */
void XAAdaptationGst_PrepareAsyncWait(XAAdaptationGstCtx* ctx)
    {
    DEBUG_API("->XAAdaptationGst_PrepareAsyncWait");

    if (ctx->waitingasyncop)
        { /*wait previous async op*/
        DEBUG_INFO("::WARNING:: previous asynch still ongoing!!!");
        DEBUG_INFO(">>>>  WAIT PREVIOUS");
        sem_wait(&(ctx->semAsyncWait));
        DEBUG_INFO("<<<<  PREVIOUS COMPLETED");
        }
    sem_init(&(ctx->semAsyncWait), 0, 0);

    ctx->waitingasyncop = XA_BOOLEAN_TRUE;
    DEBUG_API("<-XAAdaptationGst_PrepareAsyncWait");
    }

void XAAdaptationGst_StartAsyncWait(XAAdaptationGstCtx* ctx)
    {
    DEBUG_API("->XAAdaptationGst_StartAsyncWait");

    /* timeout to try to avoid gst freeze in rollup */
    ctx->asynctimer = g_timeout_add(XA_ADAPT_ASYNC_TIMEOUT,
            XAAdaptationGst_CancelAsyncWait, ctx);
    /* check flag once again if callback already happened before wait */
    if (ctx->waitingasyncop)
        {
        DEBUG_INFO(">>>>  ASYNC STARTS");
        sem_wait(&(ctx->semAsyncWait));
        DEBUG_INFO("<<<<  ASYNC COMPLETED");
        }
    else
        {
        DEBUG_INFO("<> async completed already");
        }
    /*cancel timer*/
    if (ctx->asynctimer)
        {
        g_source_remove(ctx->asynctimer);
        }
    ctx->waitingasyncop = XA_BOOLEAN_FALSE;

    DEBUG_API("<-XAAdaptationGst_StartAsyncWait");
    }

/* async operation timeout callback*/
gboolean XAAdaptationGst_CancelAsyncWait(gpointer ctx)
    {
    XAAdaptationGstCtx* bCtx = (XAAdaptationGstCtx*) ctx;
    DEBUG_API("->XAAdaptationGst_CancelAsyncWait");
    if (bCtx->waitingasyncop)
        {
        DEBUG_ERR_A3("ASYNC TIMED OUT : current %d, gsttarget %d, wanted %d",
                GST_STATE(bCtx->bin), GST_STATE_TARGET(bCtx->bin), bCtx->binWantedState);
        bCtx->waitingasyncop = XA_BOOLEAN_FALSE;
        sem_post(&(bCtx->semAsyncWait));
        }
    DEBUG_API("<-XAAdaptationGst_CancelAsyncWait");
    /* return false to remove timer */
    return FALSE;
    }

void XAAdaptationGst_CompleteAsyncWait(XAAdaptationGstCtx* ctx)
    {
    DEBUG_API("->XAAdaptationGst_CompleteAsyncWait");
    if (ctx->waitingasyncop)
        {
        int i;
        ctx->waitingasyncop = XA_BOOLEAN_FALSE;
        sem_getvalue(&(ctx->semAsyncWait), &i);
        DEBUG_INFO_A1("Asynch operation succeeded, sem value %d",i);
        if (i <= 0)
            { /* only post if locked */
            sem_post(&(ctx->semAsyncWait));
            }
        else if (i > 0)
            { /* should not be, reset semaphore */
            sem_init(&(ctx->semAsyncWait), 0, 0);
            }
        }
    DEBUG_API("<-XAAdaptationGst_CompleteAsyncWait");
    }

/**
 * GstElement* XAAdaptationGst_CreateGstSource( XADataSource* xaSrc, const XAchar *name )
 * @param XADataSource* xaSnk - XADataSource defining gst source to create
 * @param const XAchar *name - string for naming the gst element
 * @param XAboolean *isobj - (out param) is source another XA object?
 * @return GstElement* - return newly created gst source element
 * Description: Create gst source element corresponding to XA source structure
 */
GstElement* XAAdaptationGst_CreateGstSource(XADataSource* xaSrc,
        const char *name, XAboolean *isPCM )
    {
    XAuint32 locType = 0;
    GstElement* gstSrc = NULL;
#ifdef INPUTSRCSELECTION_FEATURE    
    GstAudioInputConfigIntfc *audioInputIntfc = NULL;
#endif
    char* fname = NULL;
    XADataLocator_URI* uri = NULL;
    XADataLocator_IODevice* ioDevice = NULL;

    DEBUG_API("->XAAdaptationGst_CreateGstSource");
    if (!xaSrc || !xaSrc->pLocator )
        {
        return NULL;
        }

    if (xaSrc && xaSrc->pFormat && *((XAuint32*) (xaSrc->pFormat))
            ==XA_DATAFORMAT_PCM && isPCM)
        {
        *isPCM = XA_BOOLEAN_TRUE;
        }

    locType = *((XAuint32*) (xaSrc->pLocator));
    switch (locType)
        {
        case XA_DATALOCATOR_URI:
            {
            DEBUG_INFO("XA_DATALOCATOR_URI");
            uri = (XADataLocator_URI*) xaSrc->pLocator;
            gstSrc = gst_element_factory_make("filesrc", name);
            if (uri->URI != NULL)
                {
                DEBUG_INFO_A1_STR("URI: %s", (char *)uri->URI);
                if (strncmp((char *) uri->URI, "file:///", 8) == 0)
                    {
                    fname = (char *) &((uri->URI)[8]);
                    }
                else
                    {
                    fname = (char *) uri->URI;
                    }
                DEBUG_INFO_A1_STR("->filesystem path %s", (char *)fname);
                g_object_set(G_OBJECT(gstSrc), "location", fname, NULL);
                /*check for pcm - decodebin does not know how to handle raw PCM files */
                if (isPCM && strstr(fname, ".pcm"))
                    {
                    DEBUG_INFO("PCM file detected");
                    *isPCM = XA_BOOLEAN_TRUE;
                    }
                }
            else
                {
                DEBUG_ERR("No uri specified.");
                return NULL;
                }
            break; /* XA_DATALOCATOR_URI */
            }
        case XA_DATALOCATOR_IODEVICE:
            {
            DEBUG_INFO("XA_DATALOCATOR_IODEVICE");
            ioDevice = (XADataLocator_IODevice*) (xaSrc->pLocator);
            switch (ioDevice->deviceType)
                {
                case XA_IODEVICE_AUDIOINPUT:
                    {
                    DEBUG_INFO("XA_IODEVICE_AUDIOINPUT");
                    DEBUG_INFO_A1("ioDevice->deviceID: %x", ioDevice->deviceID);
                    switch (ioDevice->deviceID)
                        {
                        case XA_ADAPTID_DEVSOUNDSRC_MIC:
                            DEBUG_INFO("devsoundsrc");
                            gstSrc = gst_element_factory_make("devsoundsrc",
                                    name);
#ifdef INPUTSRCSELECTION_FEATURE
                            if(gstSrc)
                                {
                                audioInputIntfc = GST_AUDIO_INPUT_CONFIG_GET_IFACE(gstSrc);
                                if(audioInputIntfc)
                                    {
                                    audioInputIntfc->SetAudioInputs(EDefaultMicrophone);
                                    }
                                }
#endif
                            break;
                        case XA_ADAPTID_DEVSOUNDSRC_FMRX:
                            DEBUG_INFO("devsoundsrc");
                            gstSrc = gst_element_factory_make("devsoundsrc",
                                    name);
#ifdef INPUTSRCSELECTION_FEATURE
                            if(gstSrc)
                                {
                                audioInputIntfc = GST_AUDIO_INPUT_CONFIG_GET_IFACE(gstSrc);
                                if(audioInputIntfc)
                                    {
                                    audioInputIntfc->SetAudioInputs(EFmRadio);
                                    }
                                }
#endif
                            break;
                        case XA_ADAPTID_DEVSOUNDSRC_CALL:
                            DEBUG_INFO("devsoundsrc");
                            gstSrc = gst_element_factory_make("devsoundsrc",
                                    name);
#ifdef INPUTSRCSELECTION_FEATURE
                            if(gstSrc)
                                {
                                audioInputIntfc = GST_AUDIO_INPUT_CONFIG_GET_IFACE(gstSrc);
                                if(audioInputIntfc)
                                    {
                                    audioInputIntfc->SetAudioInputs(EVoiceCall);
                                    }
                                }
#endif
                            break;

                        case XA_ADAPTID_AUDIOTESTSRC:
                            /*fall through*/
                        default:
                            DEBUG_INFO("audiotestsrc");
                            gstSrc = gst_element_factory_make("audiotestsrc",
                                    name);
                            break;
                        }
                    break;
                    }

                default:
                    {
                    DEBUG_ERR("Unsupported IODevice.");
                    break;
                    }
                }
            break; /* XA_DATALOCATOR_IODEVICE */
            }
        case XA_DATALOCATOR_ADDRESS:
        default:
            DEBUG_ERR("Incorrect data locator for source.");
            break;
        }

    if (gstSrc)
        {
        DEBUG_INFO_A1("Created gstreamer source element at %x", (int)gstSrc);
        }

    DEBUG_API("<-XAAdaptationGst_CreateGstSource");
    return gstSrc;
    }

/**
 * GstElement* XAAdaptationGst_CreateGstSink( XADataSink* xaSnk, const XAchar *name )
 * @param XADataSink* xaSnk - XADataSink defining gst sink to create
 * @param const XAchar *name - string for naming the gst element
 * @return GstElement* - return newly created gst sink element
 * Description: Create gst sink element corresponding to XA sink structure
 */
GstElement* XAAdaptationGst_CreateGstSink(XADataSink* xaSnk,
        const char *name)
    {
    XAuint32 locType = 0;
    GstElement* gstSnk = NULL;
    XADataLocator_URI* uri = NULL;
    DEBUG_API("->XAAdaptationGst_CreateGstSink");
    if (!xaSnk || !xaSnk->pLocator)
        {
        DEBUG_INFO("Warning! No sink specified, use fakesink");
        gstSnk = gst_element_factory_make("fakesink", name);
        if (!gstSnk)
            {
            DEBUG_ERR("Cannot create sink!");
            return NULL;
            }
        g_object_set(G_OBJECT(gstSnk), "async", FALSE, NULL);
        }
    else
        {
        locType = *((XAuint32*) (xaSnk->pLocator));
        switch (locType)
            {
            case XA_DATALOCATOR_URI:
                {
                DEBUG_INFO("XA_DATALOCATOR_URI");
                uri = (XADataLocator_URI*) xaSnk->pLocator;
                gstSnk = gst_element_factory_make("filesink", name);
                if (!gstSnk)
                    {
                    DEBUG_ERR("Cannot create sink!");
                    return NULL;
                    }
                if (uri->URI != NULL)
                    {
                    XAchar *fname;
                    DEBUG_INFO_A1_STR("URI: %s", (char *)uri->URI);
                    if (strncmp((char *) uri->URI, "file:///", 8) == 0)
                        {
                        fname = &((uri->URI)[8]);
                        }
                    else
                        {
                        fname = uri->URI;
                        }
                    DEBUG_INFO_A1_STR("->filesystem path %s", (char *)fname);
                    g_object_set(G_OBJECT(gstSnk), "location", fname,
                            "async", FALSE, "qos", FALSE, "max-lateness",
                            (gint64) (-1), NULL);
                    }
                else
                    {
                    DEBUG_ERR("No recording output uri specified.");
                    return NULL;
                    }
                break;
                }
/*            case XA_DATALOCATOR_ADDRESS:
            case XA_DATALOCATOR_NATIVEDISPLAY:
            case XA_DATALOCATOR_OUTPUTMIX:
            case XA_DATALOCATOR_IODEVICE:*/
                /* when only valid IOdevice sinks vibra and LED sinks implemented
                 * at adaptation level, add handling here (in this implementation,
                 * no handling needed as only dummy implementations for those)
                 **/
            default:
                DEBUG_ERR("Incorrect data locator for sink.");
                break;
            }
        }
    if (gstSnk)
        {
        DEBUG_INFO_A1("Created gstreamer sink element at %x", (int)gstSnk);
        }
    DEBUG_API("<-XAAdaptationGst_CreateGstSink");
    return gstSnk;
    }

/* called when pad is actually blocking/ gets unblocked*/
void XAAdaptationGst_PadBlockCb(GstPad *pad, gboolean blocked,
        gpointer user_data)
    {
    DEBUG_API_A2_STR("->XAAdaptationGst_PadBlockCb   pad \"%s\" of \"%s\" ",
            GST_OBJECT_NAME(pad),
            GST_OBJECT_NAME(gst_pad_get_parent_element(pad)) );
    DEBUG_API_A1("<-XAAdaptationGst_PadBlockCb   blocked:%d",blocked);
    }

