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
 * Description: Media Player Adapt Code MMF 
 *
 */

#include <stdlib.h>
#include "xamediaplayeradaptctxmmf.h"
#include "xaadaptationmmf.h"
#include "xaobjectitf.h"
#include "cmmfbackendengine.h"
#include "cmetadatautilityitf.h"

/*
 * XAMediaPlayerAdaptationMMFCtx* XAMediaPlayerAdapt_Create()
 * Allocates memory for Media Player Adaptation Context and makes 1st phase initialization
 * @param XADataSource *pDataSrc - pointer to OMX-AL data source
 * @param XADataSource *pBankSrc - pointer to instrument bank structure in Mobile DLS, if NULL default will be used.
 * @param XADataSink *pAudioSnk - pointer to OMX-AL audio sink definition
 * @param XADataSink *pImageVideoSnk - pointer to OMX-AL image and video sink definition
 * @returns XAMediaPlayerAdaptationMMFCtx* - Pointer to created context, NULL if error occurs.
 */
XAAdaptationBaseCtx* XAMediaPlayerAdaptMMF_Create(XADataSource *pDataSrc,
        XADataSource *pBankSrc, XADataSink *pAudioSnk,
        XADataSink *pImageVideoSnk, XADataSink *pVibra, XADataSink *pLEDArray)
    {
    XAMediaPlayerAdaptationMMFCtx *pSelf = NULL;
    XAuint32 locType = 0;
    XADataLocator_IODevice *ioDevice;
    XAresult res;
    DEBUG_API("->XAMediaPlayerAdaptMMF_Create");

    pSelf = calloc(1, sizeof(XAMediaPlayerAdaptationMMFCtx));
    if (pSelf)
        {
        if (pDataSrc)
            {
            locType = *((XAuint32*) (pDataSrc->pLocator));
            if (locType == XA_DATALOCATOR_IODEVICE)
                {
                ioDevice = (XADataLocator_IODevice*) (pDataSrc->pLocator);
                if (ioDevice->deviceType == XA_IODEVICE_RADIO)
                    {
                    return XAMediaPlayerAdaptMMF_CreateRadio(pSelf, pDataSrc,
                            pBankSrc, pAudioSnk, pImageVideoSnk, pVibra,
                            pLEDArray);
                    }
                }
            }
        if (XAAdaptationBaseMMF_Init(&(pSelf->baseObj),
                XAMediaPlayerAdaptation) != XA_RESULT_SUCCESS)
            {
            DEBUG_ERR("Failed to init base context!!!");
            free(pSelf);
            pSelf = NULL;
            }
        else
            {
            pSelf->isForRadio = XA_BOOLEAN_FALSE;
            pSelf->baseObj.baseObj.fwtype = FWMgrFWMMF;
            pSelf->xaSource = pDataSrc;
            pSelf->xaBankSrc = pBankSrc;
            pSelf->xaAudioSink = pAudioSnk;
            pSelf->xaVideoSink = pImageVideoSnk;
            pSelf->xaLEDArray = pLEDArray;
            pSelf->xaVibra = pVibra;
            pSelf->curMirror = XA_VIDEOMIRROR_NONE;
            pSelf->curRotation = 0;
            pSelf->isobjsrc = XA_BOOLEAN_FALSE;
            pSelf->cameraSinkSynced = XA_BOOLEAN_FALSE;
            pSelf->rateprops = XA_RATEPROP_SMOOTHVIDEO | XA_RATEPROP_NOPITCHCORAUDIO;
            pSelf->playrate = 1000;
            pSelf->mmfContext = NULL;
            }
        }
    else
        {
        DEBUG_ERR("Failed to create XAMediaPlayerAdaptationMMFCtx !!!");
        return NULL;
        }
    
    if (pSelf)
        {
        res = mmf_backend_engine_init(&(pSelf->mmfContext));
        if ((res != XA_RESULT_SUCCESS) || !(pSelf->mmfContext))
            {
            /* TODO Check to make sure there is no undeleted MMF objects here*/
            DEBUG_ERR("Failed to init mmf context!!!");
            free(pSelf);
            pSelf = NULL;
            return NULL;
            }
        res = mmf_set_play_adapt_context(pSelf->mmfContext,
                        &(pSelf->baseObj));
        if (pDataSrc)
            {
            pSelf->mmfMetadataContext
                    = mmf_metadata_utility_init(
                            (char *) (((XADataLocator_URI*) (pDataSrc->pLocator))->URI));
            if (!pSelf->mmfMetadataContext)
                {
                DEBUG_ERR("Failed to init mmf metadata context!!!");
                free(pSelf);
                pSelf = NULL;
                return NULL;
                }
            }
        else
            {
            DEBUG_ERR("Failed to create XAMediaPlayerAdaptationMMFCtx !!!");
            free(pSelf);
            pSelf = NULL;
            return NULL;
            }
        }

    DEBUG_API("<-XAMediaPlayerAdaptMMF_Create");
    return (XAAdaptationBaseCtx*) (&pSelf->baseObj.baseObj);
    }

/*
 * XAMediaPlayerAdaptationMMFCtx* XAMediaPlayerAdapt_CreateRadio()

 * @returns XAMediaPlayerAdaptationMMFCtx* - Pointer to created context, NULL if error occurs.
 */
XAAdaptationBaseCtx* XAMediaPlayerAdaptMMF_CreateRadio(
        XAMediaPlayerAdaptationMMFCtx *pSelf, XADataSource *pDataSrc,
        XADataSource *pBankSrc, XADataSink *pAudioSnk,
        XADataSink *pImageVideoSnk, XADataSink *pVibra, XADataSink *pLEDArray)
    {
    XAresult res;
    DEBUG_API("->XAMediaPlayerAdaptMMF_CreateRadio");

    res = XAAdaptationBaseMMF_Init(&(pSelf->baseObj), XARadioAdaptation);
    if (res != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR("Failed to init base context!!!");
        free(pSelf);
        pSelf = NULL;
        return NULL;
        }

    pSelf->isForRadio = XA_BOOLEAN_TRUE;
    pSelf->baseObj.baseObj.fwtype = FWMgrFWMMF;
    pSelf->xaSource = pDataSrc;
    pSelf->xaBankSrc = pBankSrc;
    pSelf->xaAudioSink = pAudioSnk;
    pSelf->xaVideoSink = pImageVideoSnk;
    pSelf->xaLEDArray = pLEDArray;
    pSelf->xaVibra = pVibra;
    pSelf->curMirror = XA_VIDEOMIRROR_NONE;
    pSelf->curRotation = 0;
    pSelf->isobjsrc = XA_BOOLEAN_FALSE;
    pSelf->cameraSinkSynced = XA_BOOLEAN_FALSE;
    /*pSelf->waitData = XA_BOOLEAN_FALSE;*/

    res = mmf_backend_engine_init(&(pSelf->mmfContext));
    if (!(pSelf->mmfContext) || (res != XA_RESULT_SUCCESS))
        {
        /* TODO Check to make sure there is no undeleted MMF objects here*/
        DEBUG_ERR("Failed to init mmf context!!!");
        free(pSelf);
        pSelf = NULL;
        return NULL;
        }
    DEBUG_API("<-XAMediaPlayerAdaptMMF_Create");
    return (XAAdaptationBaseCtx*) (&pSelf->baseObj.baseObj);
    }

/*
 * XAresult XAMediaPlayerAdaptMMF_PostInit()
 * 2nd phase initialization of Media Player Adaptation Context
 * @param XAMediaPlayerAdaptationMMFCtx* ctx - pointer to Media Player adaptation context
 * @return XAresult - Success value
 */
XAresult XAMediaPlayerAdaptMMF_PostInit(XAAdaptationMMFCtx* bCtx)
    {
    XAresult ret = XA_RESULT_PRECONDITIONS_VIOLATED;
    XAMediaPlayerAdaptationMMFCtx *pSelf = NULL;
    if(!bCtx)
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        return ret;
        }
    
    pSelf = (XAMediaPlayerAdaptationMMFCtx*) bCtx;

    DEBUG_API("->XAMediaPlayerAdaptMMF_PostInit");

    ret = XAAdaptationBaseMMF_PostInit(bCtx);
    if(ret == XA_RESULT_SUCCESS)
        {
        if (pSelf->isForRadio)
            {
            DEBUG_API("<-XAMediaPlayerAdaptMMF_PostInit");
            return XA_RESULT_SUCCESS;
            }
    
        if (pSelf->mmfContext)
            {
            XADataLocator_URI* tempUri =
                    (XADataLocator_URI*) (pSelf->xaSource->pLocator);
            XADataFormat_MIME* tempFormat =
                    (XADataFormat_MIME*) (pSelf->xaSource->pFormat);
            ret = XA_RESULT_SUCCESS;
            if (pSelf->xaVideoSink)
                {
                ret = mmf_setup_native_display(pSelf->mmfContext,
                        pSelf->xaVideoSink);
                }
            if (ret == XA_RESULT_SUCCESS)
                {
                ret = mmf_set_player_uri(pSelf->mmfContext,
                        (char *) (tempUri->URI), tempFormat->containerType);
                }
            }
        }
    DEBUG_API("<-XAMediaPlayerAdaptMMF_PostInit");
    return ret;
    }

/*
 * void XAMediaPlayerAdaptMMF_Destroy( XAMediaPlayerAdaptationMMFCtx* ctx )
 * Destroys Media Player Adaptation Context
 * @param ctx - Media Player Adaptation context to be destroyed
 */
void XAMediaPlayerAdaptMMF_Destroy(XAAdaptationMMFCtx* bCtx)
    {
    XAMediaPlayerAdaptationMMFCtx* ctx = NULL;

    DEBUG_API("->XAMediaPlayerAdaptMMF_Destroy");
    if (bCtx == NULL)
        {
        DEBUG_ERR("Invalid parameter!!");
        DEBUG_API("<-XAMediaPlayerAdaptMMF_Destroy");
        return;
        }
    ctx = (XAMediaPlayerAdaptationMMFCtx*) bCtx;

    if (ctx->mmfContext)
        {
        mmf_backend_engine_deinit(ctx->mmfContext);
        }

    if (ctx->mmfMetadataContext)
        {
        mmf_metadata_utility_destroy(ctx->mmfMetadataContext);
        }

    XAAdaptationBaseMMF_Free(bCtx);

    free(ctx);
    ctx = NULL;

    DEBUG_API("<-XAMediaPlayerAdaptMMF_Destroy");
    }

