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
 * Description: Dynamic Src Itf MMF Adapt Implementation
 *
 */

#include "xamediaplayeradaptctxmmf.h"
#include "xametadataadaptctxmmf.h"
#include "xadynamicsourceitfadaptationmmf.h"
#include "xaplayitfadaptationmmf.h"
#include "cmmfbackendengine.h"
#include "cmetadatautilityitf.h"

/*
 * XAresult XADynamicSourceItfAdaptMMF_SetSource(XAAdaptationGstCtx *bCtx, XADataSource *pDataSource)
 * @param XAAdaptationGstCtx *bCtx - Adaptation context, this will be casted to correct type regarding to contextID value
 * @param XADataSource *pDataSource - new data source
 * @return XAresult ret - Success value
 */
XAresult XADynamicSourceItfAdaptMMF_SetSource(XAAdaptationMMFCtx *bCtx,
        XADataSource *pDataSource)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 origstate;
    DEBUG_API("->XADynamicSourceItfAdaptMMF_SetSource");
    if (!bCtx || !pDataSource || !pDataSource->pLocator)
        {
        DEBUG_ERR("Invalid NULL parameter");
        ret = XA_RESULT_PARAMETER_INVALID;
        }
    else if (bCtx->baseObj.ctxId == XAMediaPlayerAdaptation)
        {
        XADataLocator_URI* tempUri =
                (XADataLocator_URI*) (pDataSource->pLocator);
        XADataFormat_MIME* tempFormat =
                (XADataFormat_MIME*) (pDataSource->pFormat);
        XAMediaPlayerAdaptationMMFCtx* mCtx =
                (XAMediaPlayerAdaptationMMFCtx*) bCtx;

        ret = XAPlayItfAdaptMMF_GetPlayState(&bCtx->baseObj, &origstate);

        ret = XAPlayItfAdaptMMF_SetPlayState(&bCtx->baseObj,
                XA_PLAYSTATE_STOPPED);

        mmf_close(mCtx->mmfContext);
        mCtx->xaSource = pDataSource;
        if (mCtx->xaVideoSink)
            {
            ret = mmf_setup_native_display(mCtx->mmfContext,
                    mCtx->xaVideoSink);
            }
        if (ret == XA_RESULT_SUCCESS)
            {
            ret = mmf_set_player_uri(mCtx->mmfContext,
                    (char *) (tempUri->URI), tempFormat->containerType);
            }
        if (ret == XA_RESULT_SUCCESS)
            {
            ret = XAPlayItfAdaptMMF_SetPlayState(&bCtx->baseObj, origstate);
            }
        else
            {
            DEBUG_ERR("Set Play state failed");
            ret = XA_RESULT_INTERNAL_ERROR;
            }
        if ((ret == XA_RESULT_SUCCESS) && (mCtx->mmfMetadataContext))
            {
            mmf_metadata_utility_parse_source(mCtx->mmfMetadataContext,
                    (char *) (tempUri->URI));
            }
        }
    else if (bCtx->baseObj.ctxId == XAMDAdaptation)
        {

        XAMetadataAdaptationMMFCtx* mCtx = (XAMetadataAdaptationMMFCtx*) bCtx;
        if (mCtx->mmfContext)
            {
            mCtx->xaSource = pDataSource;
            mmf_metadata_utility_parse_source(
                    mCtx->mmfContext,
                    (char *) (((XADataLocator_URI*) (pDataSource->pLocator))->URI));
            }
        }
    DEBUG_API("<-XADynamicSourceItfAdaptMMF_SetSource");
    return ret;
    }

