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
#include "xaplaybackrateitfadaptationmmf.h"
#include "xaplayitfadaptationmmf.h"
#include "cmmfbackendengine.h"

/*XA_RATEPROP_STAGGEREDVIDEO,XA_RATEPROP_SMOOTHVIDEO,XA_RATEPROP_SILENTAUDIO,
  XA_RATEPROP_STAGGEREDAUDIO,XA_RATEPROP_NOPITCHCORAUDIO,XA_RATEPROP_PITCHCORAUDIO*/
#define SUPPORTEDPROPS (XA_RATEPROP_STAGGEREDVIDEO | XA_RATEPROP_SILENTAUDIO)
/* NOTE: currently rewind rates do not seem to work */
#define MINRATE (-1000)
#define MAXRATE 4000
/*
 * XAresult XAPlaybackRateItfAdapt_SetRate(XAAdaptationGstCtx *bCtx, XApermille rate)
 * @param XAAdaptationGstCtx *bCtx - Adaptation context, this will be casted to correct type regarding to contextID value
 * @param XApermille rate - new playback rate in permilles of original speed
 * @return XAresult ret - Success value
 */
XAresult XAPlaybackRateItfAdaptMMF_SetRate(XAAdaptationMMFCtx *bCtx, XApermille rate)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx = (XAMediaPlayerAdaptationMMFCtx*) bCtx;
    DEBUG_API_A1("->XAPlaybackRateItfAdaptMMF_SetRate %d permilles", rate);

    if( !bCtx || bCtx->baseObj.ctxId != XAMediaPlayerAdaptation )
    {
        DEBUG_ERR("Invalid context!");
        return XA_RESULT_PARAMETER_INVALID;
    }
    if( rate<MINRATE || rate>MAXRATE )
    {
        DEBUG_ERR("Invalid rate!");
        return XA_RESULT_PARAMETER_INVALID;
    }

    ret = mmf_playbackrateitf_set_playbackrate(mCtx->mmfContext, rate/10);
    if(ret == XA_RESULT_SUCCESS)
        {
        mCtx->playrate = rate/10;
        if(mCtx->playrate == 0 || mCtx->playrate == 100)
            {
            mCtx->rateprops = XA_RATEPROP_SMOOTHVIDEO | XA_RATEPROP_NOPITCHCORAUDIO;
            }
        else
            {
            mCtx->rateprops = SUPPORTEDPROPS; 
            }
        }

    DEBUG_API("<-XAPlaybackRateItfAdaptMMF_SetRate");
    return ret;
}

/*
 * XAresult XAPlaybackRateItfAdaptMMF_SetPropertyConstraints(XAAdaptationMMFCtx *bCtx,
 *                                                       XAuint32 constraints)
 */
XAresult XAPlaybackRateItfAdaptMMF_SetPropertyConstraints(XAAdaptationMMFCtx *bCtx,
                                                       XAuint32 constraints)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 capabilities;
    XAMediaPlayerAdaptationMMFCtx* mCtx = NULL;
    DEBUG_API("->XAPlaybackRateItfAdaptMMF_SetPropertyConstraints");

    if( !bCtx || bCtx->baseObj.ctxId != XAMediaPlayerAdaptation )
    {
        DEBUG_ERR("Invalid context!");
        return XA_RESULT_PARAMETER_INVALID;
    }
    mCtx = (XAMediaPlayerAdaptationMMFCtx*) bCtx;
    
    ret = XAPlaybackRateItfAdaptMMF_GetCapabilitiesOfRate(bCtx,mCtx->playrate,&capabilities);
    
    if( (constraints & capabilities ) == 0 )
    {
        DEBUG_ERR("constraints cannot be satisfied!!");
        ret = XA_RESULT_FEATURE_UNSUPPORTED;
    }
    else
    {
        mCtx->rateprops = capabilities & constraints;
    }
    DEBUG_API("<-XAPlaybackRateItfAdaptMMF_SetPropertyConstraints");
    return ret;
}

/*
 * XAresult XAPlaybackRateItfAdaptMMF_GetProperties(XAAdaptationMMFCtx *bCtx,
 *                                              XAuint32 *pProperties)
 */
XAresult XAPlaybackRateItfAdaptMMF_GetProperties(XAAdaptationMMFCtx *bCtx,
                                               XAuint32 *pProperties)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx = NULL;
    DEBUG_API("->XAPlaybackRateItfAdaptMMF_GetProperties");

    if( !bCtx || bCtx->baseObj.ctxId != XAMediaPlayerAdaptation )
    {
        DEBUG_ERR("Invalid context!");
        return XA_RESULT_PARAMETER_INVALID;
    }
    mCtx = (XAMediaPlayerAdaptationMMFCtx*) bCtx;
    *pProperties = mCtx->rateprops;

    DEBUG_API("<-XAPlaybackRateItfAdaptMMF_GetProperties");
    return ret;
}


XAresult XAPlaybackRateItfAdaptMMF_GetCapabilitiesOfRate(XAAdaptationMMFCtx *bCtx,
                                                       XApermille rate,
                                                       XAuint32 *pCapabilities)
{
    XAresult ret = XA_RESULT_SUCCESS;
    
    DEBUG_API("->XAPlaybackRateItfAdaptMMF_GetCapabilitiesOfRate");
    if( !bCtx || bCtx->baseObj.ctxId != XAMediaPlayerAdaptation || !pCapabilities)
    {
        DEBUG_ERR("Invalid context!");
        return XA_RESULT_PARAMETER_INVALID;
    }

    if( rate < MINRATE || rate > MAXRATE )
    {
        DEBUG_ERR("Invalid rate!");
        *pCapabilities = 0;
        ret = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {
        if(rate == 0 || rate == 1000)
            {
            *pCapabilities = XA_RATEPROP_SMOOTHVIDEO | XA_RATEPROP_NOPITCHCORAUDIO;
            }
        else
            {
            *pCapabilities = XA_RATEPROP_STAGGEREDVIDEO | XA_RATEPROP_SILENTAUDIO;
            }
    }

    DEBUG_API("<-XAPlaybackRateItfAdaptMMF_GetCapabilitiesOfRate");
    return ret;
}


XAresult XAPlaybackRateItfAdaptMMF_GetRateRange(XAAdaptationMMFCtx *bCtx,
                                             XAuint8 index,
                                             XApermille *pMinRate,
                                             XApermille *pMaxRate,
                                             XApermille *pStepSize,
                                             XAuint32 *pCapabilities)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx;
    XAboolean forward = XA_BOOLEAN_FALSE;
    XAboolean backward = XA_BOOLEAN_FALSE;
    XAuint8 allowedindex = 0;
    
    DEBUG_API("->XAPlaybackRateItfAdaptMMF_GetRateRange");

    if( !bCtx || bCtx->baseObj.ctxId != XAMediaPlayerAdaptation )
    {
        DEBUG_ERR("Invalid context!");
        return XA_RESULT_PARAMETER_INVALID;
    }
    mCtx = (XAMediaPlayerAdaptationMMFCtx*) bCtx;
    ret = mmf_playbackrateitf_get_playbackratecaps(mCtx->mmfContext, &forward, &backward);
    if(ret == XA_RESULT_SUCCESS)
        {
        if(forward == XA_BOOLEAN_TRUE)
            {
            allowedindex++;
            }
        if(backward == XA_BOOLEAN_TRUE)
            {
            allowedindex++;
            }
    
        if( index > allowedindex )
            {
            ret = XA_RESULT_PARAMETER_INVALID;
            }
        else
            {
            switch(index)
                {
                case 0:
                    {
                    *pMinRate = 0;
                    *pMaxRate = 1000;
                    *pStepSize = 0; /* continuous range */
                    *pCapabilities = XA_RATEPROP_SMOOTHVIDEO | XA_RATEPROP_NOPITCHCORAUDIO;
                    break;
                    }
                case 1:
                    {
                    *pMinRate = 2000;
                    *pMaxRate = 4000;
                    *pStepSize = 0; /* continuous range */
                    *pCapabilities = SUPPORTEDPROPS;
                    break;
                    }
                case 2:
                    {
                    *pMinRate = MINRATE;
                    *pMaxRate = MINRATE;
                    *pStepSize = 0; /* continuous range */
                    *pCapabilities = SUPPORTEDPROPS;
                    break;
                    }
                }
            }
        }

    DEBUG_API("<-XAPlaybackRateItfAdaptMMF_GetRateRange");
    return ret;
}

