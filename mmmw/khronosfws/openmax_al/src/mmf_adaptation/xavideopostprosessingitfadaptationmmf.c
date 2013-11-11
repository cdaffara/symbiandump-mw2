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
* Description: 
*
*/

#include "xamediaplayeradaptctxmmf.h"
#include "xavideopostprosessingitfadaptationmmf.h"
#include "cmmfbackendengine.h"
/*
 * XAresult XAVideoPostProcessingItfAdapt_Commit(XAAdaptationMMFCtx *bCtx)
 */
XAresult XAVideoPostProcessingItfAdapt_Commit(XAAdaptationMMFCtx *bCtx,
                                              XAmillidegree rotation,
                                              XAuint32 scaleOptions,
                                              XAuint32 backgroundColor,
                                              XAuint32 renderingHints,
                                              const XARectangle *pSrcRect,
                                              const XARectangle *pDestRect,
                                              XAuint32 mirror,
                                              XAboolean isMirror,
                                              XAboolean isRotate,
                                              XAboolean isDestRect,
                                              XAboolean isSrcRect,
                                              XAboolean isScaleOptions
                                              )
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx = (XAMediaPlayerAdaptationMMFCtx*)bCtx;
    
    DEBUG_API("->XAVideoPostProcessingItfAdapt_Commit");

    if( !bCtx || (bCtx->baseObj.ctxId != XAMediaPlayerAdaptation))
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVideoPostProcessingItfAdapt_Commit");
        return XA_RESULT_PARAMETER_INVALID;
    }

    if(isSrcRect)
        {
        ret = mmf_videoppitf_set_sourcerectangle(mCtx->mmfContext, pSrcRect);
        }
    
    if(ret == XA_RESULT_SUCCESS && isDestRect)
        {
        ret = mmf_videoppitf_set_destinationrectangle(mCtx->mmfContext, pDestRect);
        }
    
    if(ret == XA_RESULT_SUCCESS && isRotate)
        {
        ret = mmf_videoppitf_set_rotation(mCtx->mmfContext, rotation);
        }

    if(ret == XA_RESULT_SUCCESS && isScaleOptions)
        {
        ret = mmf_videoppitf_set_scalingoptions(mCtx->mmfContext, scaleOptions);
        }

    DEBUG_API("<-XAVideoPostProcessingItfAdapt_Commit");
    return ret;
}

