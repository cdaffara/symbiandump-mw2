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
 * Description: Nokia Volume Ext Impl
 *
 */

#include "xanokiavolumeextitfadaptationmmf.h"
#include "xamediaplayeradaptctxmmf.h"
#include "xamediarecorderadaptctxmmf.h"
#include "cmmfbackendengine.h"
#include "cmmfradiobackendengine.h"

/*
 * XAresult XANokiaVolumeExtItfAdapt_SetMute(void *ctx, AdaptationContextIDS ctx->ctxId, XAboolean mute)
 * @param void *ctx - Adaptation context, this will be casted to correct type regarding to contextID value given as 2nd parameter
 * @param AdaptationContextIDS ctx->ctxId - Type specifier for context, this will be used to cast ctx pointer to correct type.
 * @param XAboolean mute - status of mute value
 * @return XAresult ret - Success value
 */
XAresult XANokiaVolumeExtItfAdapt_SetMute(XAAdaptationMMFCtx *ctx,
        XAboolean mute)
    {
    XAuint32 volume;
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XANokiaVolumeExtItfAdapt_SetMute");
    if (!ctx || (ctx->baseObj.ctxId != XAMediaPlayerAdaptation
            && ctx->baseObj.ctxId != XAMediaRecorderAdaptation
            && ctx->baseObj.ctxId != XARadioAdaptation))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfAdapt_SetMute");
        /* invalid parameter */
        res = XA_RESULT_PARAMETER_INVALID;
        return res;
        }

    if (ctx->baseObj.ctxId == XAMediaPlayerAdaptation)
        {
        if (mute)
            {

            res = mmf_volumeitf_get_volume(
                    ((XAMediaPlayerAdaptationMMFCtx*) ctx)->mmfContext,
                    &volume);
            if(res == XA_RESULT_SUCCESS)
                {
                ((XAMediaPlayerAdaptationMMFCtx*) ctx)->premutevol = volume;
                res = mmf_volumeitf_set_volume(
                        ((XAMediaPlayerAdaptationMMFCtx*) ctx)->mmfContext, 0);
                }
            }
        else
            {
            res = mmf_volumeitf_set_volume(
                    ((XAMediaPlayerAdaptationMMFCtx*) ctx)->mmfContext,
                    ((XAMediaPlayerAdaptationMMFCtx*) ctx)->premutevol);
            }
        }
    else if (ctx->baseObj.ctxId == XAMediaRecorderAdaptation)
        {
        if (mute)
            {

            res = mmf_volumeitf_get_volume(
                    ((XAMediaRecorderAdaptationMMFCtx*) ctx)->mmfContext,
                    &volume);
            if(res == XA_RESULT_SUCCESS)
                {
                ((XAMediaRecorderAdaptationMMFCtx*) ctx)->premutevol = volume;
                res = mmf_volumeitf_set_volume(
                        ((XAMediaRecorderAdaptationMMFCtx*) ctx)->mmfContext, 0);
                }
            }
        else
            {
            res = mmf_volumeitf_set_volume(
                    ((XAMediaRecorderAdaptationMMFCtx*) ctx)->mmfContext,
                    ((XAMediaRecorderAdaptationMMFCtx*) ctx)->premutevol);
            }
        }
    else if (ctx->baseObj.ctxId == XARadioAdaptation)
        {
        res = set_mute(cmmfradiobackendengine_init(), ctx, mute);
        }

    DEBUG_API("<-XANokiaVolumeExtItfAdapt_SetMute");
    return res;
    }

/*
 * XAresult XANokiaVolumeExtItfAdapt_EnableStereoPosition(XAAdaptationBaseCtx *ctx, XAboolean enable)
 * @param XAAdaptationBaseCtx *ctx - Adaptation context
 * @param XAboolean enable - Enable Stereo Position
 * @return XAresult - Success value
 */
XAresult XANokiaVolumeExtItfAdapt_EnableStereoPosition(
        XAAdaptationMMFCtx *ctx, XAboolean enable)
    {
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XANokiaVolumeExtItfAdapt_EnableStereoPosition");

    if (!ctx || (ctx->baseObj.ctxId != XAMediaPlayerAdaptation
            && ctx->baseObj.ctxId != XAMediaRecorderAdaptation))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfAdapt_EnableStereoPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (ctx->baseObj.ctxId == XAMediaPlayerAdaptation)
        {
        res =  XA_RESULT_FEATURE_UNSUPPORTED;
        }
    else if (ctx->baseObj.ctxId == XAMediaRecorderAdaptation)
        {
        res =  XA_RESULT_FEATURE_UNSUPPORTED;
        }
    else if (ctx->baseObj.ctxId == XARadioAdaptation)
        {
        res =  XA_RESULT_FEATURE_UNSUPPORTED;
        }

    DEBUG_API("<-XANokiaVolumeExtItfAdapt_EnableStereoPosition");
    return res;
    }

/*
 * XAresult XANokiaVolumeExtItfAdapt_SetStereoPosition(XAAdaptationMMFCtx *ctx, XApermille stereoPosition)
 * @param XAAdaptationBaseCtx *ctx - Adaptation context
 * @param XApermille stereoPosition - Stereo Position to be set
 * @return XAresult - Success value
 */
XAresult XANokiaVolumeExtItfAdapt_SetStereoPosition(XAAdaptationMMFCtx *ctx,
        XApermille stereoPosition)
    {
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XANokiaVolumeExtItfAdapt_SetStereoPosition");

    if (!ctx || (ctx->baseObj.ctxId != XAMediaPlayerAdaptation
            && ctx->baseObj.ctxId != XAMediaRecorderAdaptation))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfAdapt_SetStereoPosition");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (ctx->baseObj.ctxId == XAMediaPlayerAdaptation)
        {
        res = XA_RESULT_FEATURE_UNSUPPORTED;
        }
    else if (ctx->baseObj.ctxId == XAMediaRecorderAdaptation)
        {
        res = XA_RESULT_FEATURE_UNSUPPORTED;
        }
    else if (ctx->baseObj.ctxId == XARadioAdaptation)
        {
        res = XA_RESULT_FEATURE_UNSUPPORTED;
        }

    DEBUG_API("<-XANokiaVolumeExtItfAdapt_SetStereoPosition");
    return res;
    }

