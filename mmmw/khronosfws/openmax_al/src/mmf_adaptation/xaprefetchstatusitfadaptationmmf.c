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
 * Description: Play Itf Adapt MMF
 *
 */

//#include "unistd.h"
#include "xamediaplayeradaptctxmmf.h"
#include "xaplayitfadaptationmmf.h"
#include "xaadaptationmmf.h"
#include "cmmfbackendengine.h"
#include "cmmfradiobackendengine.h"
#include <glib.h>


XAresult XAPrefetchStatusItfAdaptMMF_GetPrefetchStatus(XAAdaptationBaseCtx *bCtx, XAuint32 *pStatus)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx;
    if (!bCtx)
    {
        ret = XA_RESULT_PARAMETER_INVALID;
        return ret;
    }

    mCtx = (XAMediaPlayerAdaptationMMFCtx*) bCtx;
    DEBUG_API("->XAPrefetchStatusItfAdaptMMF_GetPrefetchStatus");

	ret = mmf_prefetchstatusitf_get_status(mCtx->mmfContext, pStatus);

    DEBUG_API("<-XAPrefetchStatusItfAdaptMMF_GetPrefetchStatus");
    return ret;

}

XAresult XAPrefetchStatusItfAdaptMMF_GetFillLevel(XAAdaptationBaseCtx *bCtx, XApermille *pLevel)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx;
    if (!bCtx)
    {
        ret = XA_RESULT_PARAMETER_INVALID;
        return ret;
    }

    mCtx = (XAMediaPlayerAdaptationMMFCtx*) bCtx;
    DEBUG_API("->XAPrefetchStatusItfAdaptMMF_GetFillLevel");

	
    ret = mmf_prefetchstatusitf_get_fill_level(mCtx->mmfContext, pLevel);

    DEBUG_API("<-XAPrefetchStatusItfAdaptMMF_GetFillLevel");
    return ret;
}

XAresult XAPrefetchStatusItfAdaptMMF_RegisterCallback(XAAdaptationBaseCtx *bCtx,
                                                    			xaPrefetchCallback callback)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx;
    if (!bCtx)
    {
        ret = XA_RESULT_PARAMETER_INVALID;
        return ret;
    }

    mCtx = (XAMediaPlayerAdaptationMMFCtx*) bCtx;
    DEBUG_API("->XAPrefetchStatusItfAdaptMMF_RegisterCallback");

	
    ret = mmf_prefetchstatusitf_register_callback(mCtx->mmfContext, callback);

    DEBUG_API("<-XAPrefetchStatusItfAdaptMMF_RegisterCallback");
    return ret;
}

XAresult XAPrefetchStatusItfAdaptMMF_SetCallbackEventsMask(XAAdaptationBaseCtx *bCtx,
                                                         XAuint32 eventFlags)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx;
    if (!bCtx)
    {
        ret = XA_RESULT_PARAMETER_INVALID;
        return ret;
    }

    mCtx = (XAMediaPlayerAdaptationMMFCtx*) bCtx;
    DEBUG_API("->XAPrefetchStatusItfAdaptMMF_SetCallbackEventsMask");

	
    ret = mmf_prefetchstatusitf_set_callback_events_mask(mCtx->mmfContext, eventFlags);

    DEBUG_API("<-XAPrefetchStatusItfAdaptMMF_SetCallbackEventsMask");
    return ret;
}

XAresult XAPrefetchStatusItfAdaptMMF_SetFillUpdatePeriod(XAAdaptationBaseCtx *bCtx,
                                                       XApermille period)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx;
    if (!bCtx)
    {
        ret = XA_RESULT_PARAMETER_INVALID;
        return ret;
    }

    mCtx = (XAMediaPlayerAdaptationMMFCtx*) bCtx;
    DEBUG_API("->XAPrefetchStatusItfAdaptMMF_SetFillUpdatePeriod");

    ret = mmf_prefetchstatusitf_set_fill_level_update_period(mCtx->mmfContext, period);

    DEBUG_API("<-XAPrefetchStatusItfAdaptMMF_SetFillUpdatePeriod");
    return ret;
}

