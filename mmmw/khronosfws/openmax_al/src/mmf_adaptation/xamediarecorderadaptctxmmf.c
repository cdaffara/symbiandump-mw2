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
 * Description: MediaRecorder MMF Adaptation
 *
 */

#include <string.h>
#include "xamediarecorderadaptctxmmf.h"
#include "xaadaptationmmf.h"
#include "cmmfbackendengine.h"
#include "xaadptbasectx.h"

/*
 * XAAdaptationBaseCtx* XAMediaRecorderAdaptMMF_Create()
 * Allocates memory for Media Recorder Adaptation Context and makes 1st phase initialization
 * @param XADataSource *pAudioSrc - pointer to OMX-AL audio source
 * @param XADataSource *pImageVideoSrc - pointer image/video source
 * @param XADataSink *pDataSnk - pointer to OMX-AL sink
 * @returns XAMediaRecorderAdaptationMMFCtx* - Pointer to created context, NULL if error occurs.
 */
XAAdaptationBaseCtx* XAMediaRecorderAdaptMMF_Create(XADataSource* pAudioSrc,
        XADataSource* pImageVideoSrc, XADataSink* pDataSnk, XAuint8 recModes)
    {
    XAMediaRecorderAdaptationMMFCtx *pSelf = NULL;
    XAresult res;
    DEBUG_API("->XAMediaRecorderAdaptMMF_Create");

    pSelf = (XAMediaRecorderAdaptationMMFCtx*) calloc(1,
            sizeof(XAMediaRecorderAdaptationMMFCtx));
    if (!pSelf)
        {
        DEBUG_ERR("Failed to create XAMediaRecorderAdaptationMMFCtx !!!");
        DEBUG_API("<-XAMediaRecorderAdaptMMF_Create");
        return NULL;
        }

    if (XAAdaptationBaseMMF_Init(&(pSelf->baseObj),
            XAMediaRecorderAdaptation) != XA_RESULT_SUCCESS)
        {
        free(pSelf);
        DEBUG_ERR("Failed to init base context!!!");
        DEBUG_API("<-XAMediaRecorderAdaptMMF_Create");
        return NULL;
        }

    pSelf->xaAudioSource = pAudioSrc;
    pSelf->xaVideoSource = pImageVideoSrc;
    pSelf->xaSink = pDataSnk;
    pSelf->xaRecordState = XA_RECORDSTATE_STOPPED;
    pSelf->curMirror = XA_VIDEOMIRROR_NONE;
    pSelf->curRotation = 0;
    pSelf->recModes = recModes;
    pSelf->isRecord = XA_BOOLEAN_FALSE;

    /* defaults from API spec */
    pSelf->imageEncSettings.width = 640;
    pSelf->imageEncSettings.height = 480;
    pSelf->imageEncSettings.compressionLevel = 0;
    pSelf->imageEncSettings.encoderId = XA_IMAGECODEC_JPEG;
    pSelf->imageEncSettings.colorFormat = XA_COLORFORMAT_UNUSED;
    /* no specified defaults for rest, determined later from container type */
    pSelf->videoEncSettings.encoderId = XA_ADAPTID_UNINITED;
    pSelf->videoEncSettings.width = 640;
    pSelf->videoEncSettings.height = 480;
    pSelf->videoEncSettings.frameRate = 15;
    pSelf->audioEncSettings.encoderId = XA_ADAPTID_UNINITED;
    pSelf->audioEncSettings.channelsIn = 2;
    pSelf->audioEncSettings.channelsOut = 2;
    pSelf->audioEncSettings.bitsPerSample = 8;
    pSelf->audioEncSettings.bitRate = 128;
    pSelf->audioEncSettings.sampleRate = 44100;

    res = mmf_backend_engine_init(&(pSelf->mmfContext));
    if (!(pSelf->mmfContext) || (res != XA_RESULT_SUCCESS))
        {
        free(pSelf);
        DEBUG_ERR("Failed to init mmf context!!!");
        DEBUG_API("<-XAMediaRecorderAdaptMMF_Create");
        return NULL;
        }

    DEBUG_API("<-XAMediaRecorderAdaptMMF_Create");
    return (XAAdaptationBaseCtx*) (&pSelf->baseObj.baseObj);
    }

/*
 * XAresult XAMediaRecorderAdaptMMF_PostInit()
 * 2nd phase initialization of Media Recorder Adaptation Context
 * @param XAAdaptationBaseCtx* ctx - pointer to Media Recorder adaptation context
 * @return XAresult - Success value
 */
XAresult XAMediaRecorderAdaptMMF_PostInit(XAAdaptationMMFCtx* bCtx)
    {

    XAresult ret = XA_RESULT_SUCCESS;
    XADataLocator_URI* tempUri;
    XADataFormat_MIME* tempFormat;
    XAMediaRecorderAdaptationMMFCtx *pSelf;
    DEBUG_API("->XAMediaRecorderAdapt_PostInit");
    if(!bCtx)
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAMediaRecorderAdapt_PostInit");
        return ret;
        }
    
    pSelf = (XAMediaRecorderAdaptationMMFCtx*) bCtx;
    if (pSelf->mmfContext)
        {
        tempUri = (XADataLocator_URI*) (pSelf->xaSink->pLocator);
        tempFormat = (XADataFormat_MIME*) (pSelf->xaSink->pFormat);
        ret = mmf_set_recorder_uri(pSelf->mmfContext, (char *) (tempUri->URI),
                tempFormat->containerType);
        if (ret != XA_RESULT_SUCCESS)
            {
            DEBUG_ERR("Failed to set recorder uri on mmf!!!");
            DEBUG_API("<-XAMediaRecorderAdapt_PostInit");
            return ret;
            }
        ret = mmf_set_adapt_context(pSelf->mmfContext, &(pSelf->baseObj));
        if (ret != XA_RESULT_SUCCESS)
            {
            DEBUG_ERR("Failed to set adapt context on mmf!!!");
            DEBUG_API("<-XAMediaRecorderAdapt_PostInit");
            return ret;
            }
        }
    DEBUG_API("<-XAMediaRecorderAdapt_PostInit");
    return ret;
    }

/*
 * void XAMediaRecorderAdaptMMF_Destroy( XAAdaptationMMFCtx* bCtx )
 * Destroys Media Recorder Adaptation Context
 * @param ctx - Media Recorder Adaptation context to be destroyed
 */
void XAMediaRecorderAdaptMMF_Destroy(XAAdaptationMMFCtx* bCtx)
    {
    XAMediaRecorderAdaptationMMFCtx* ctx = NULL;

    DEBUG_API("->XAMediaRecorderAdaptMMF_Destroy");

    if (bCtx == NULL)
        {
        DEBUG_ERR("Invalid parameter!!");
        DEBUG_API("<-XAMediaRecorderAdaptMMF_Destroy");
        return;
        }
    ctx = (XAMediaRecorderAdaptationMMFCtx*) bCtx;

    if (ctx->mmfContext)
        {
        mmf_backend_engine_deinit(ctx->mmfContext);
        }
    XAAdaptationBaseMMF_Free(bCtx);

    free(ctx);
    ctx = NULL;

    DEBUG_API("<-XAMediaRecorderAdaptMMF_Destroy");
    }

