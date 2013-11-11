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

#include <stdio.h>
#include <stdlib.h>
#include "xaplaybackrateitf.h"

#include "xaplaybackrateitfadaptationmmf.h"

#include "xathreadsafety.h"

static XAPlaybackRateItfImpl* GetImpl(XAPlaybackRateItf self)
{
    if(self)
    {
        XAPlaybackRateItfImpl *impl = (XAPlaybackRateItfImpl*)(*self);
        if(impl && impl == impl->self)
        {
            return impl;
        }
    }
    return NULL;
}

/**
 * Base interface XAPlaybackRateItf implementation
 */
XAresult XAPlaybackRateItfImpl_SetRate(XAPlaybackRateItf self, XApermille rate)
{
    XAresult res = XA_RESULT_SUCCESS;
    XAPlaybackRateItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAPlaybackRateItfImpl_SetRate");
    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaPlayer );

    if( !impl )
    {

        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAPlaybackRateItfImpl_SetRate");
        return XA_RESULT_PARAMETER_INVALID;
    }

    if(impl->adaptCtx->fwtype == FWMgrFWMMF)
        {
        res = XAPlaybackRateItfAdaptMMF_SetRate((XAAdaptationMMFCtx*)impl->adaptCtx, rate);
        }
    else
        {
        DEBUG_ERR("XA_RESULT_FEATURE_UNSUPPORTED");
        res = XA_RESULT_FEATURE_UNSUPPORTED;          
        }
    
    if(res == XA_RESULT_SUCCESS)
    {
        impl->currentRate = rate;
    }

    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
    DEBUG_API("<-XAPlaybackRateItfImpl_SetRate");
    return res;
}

XAresult XAPlaybackRateItfImpl_GetRate(XAPlaybackRateItf self, XApermille *pRate)
{
    XAresult res = XA_RESULT_SUCCESS;
    XAPlaybackRateItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAPlaybackRateItfImpl_GetRate");

    if( !impl || !pRate )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAPlaybackRateItfImpl_GetRate");
        return XA_RESULT_PARAMETER_INVALID;
    }
    
    if(impl->adaptCtx->fwtype == FWMgrFWMMF)
        {
        *pRate = impl->currentRate;
        }
    else
        {
        DEBUG_ERR("XA_RESULT_FEATURE_UNSUPPORTED");
        res = XA_RESULT_FEATURE_UNSUPPORTED;          
        }

    DEBUG_API("<-XAPlaybackRateItfImpl_GetRate");
    return res;
}

XAresult XAPlaybackRateItfImpl_SetPropertyConstraints(XAPlaybackRateItf self,
                                                        XAuint32 constraints)
{
    XAresult res = XA_RESULT_SUCCESS;
    XAPlaybackRateItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAPlaybackRateItfImpl_SetPropertyConstraints");

    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaPlayer );
    if( !impl )
    {
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAPlaybackRateItfImpl_SetPropertyConstraints");
        return XA_RESULT_PARAMETER_INVALID;
    }
    
    if(impl->adaptCtx->fwtype == FWMgrFWMMF)
        {
        /* set to adaptation */
        res = XAPlaybackRateItfAdaptMMF_SetPropertyConstraints((XAAdaptationMMFCtx*)impl->adaptCtx, constraints);
        }
    else
        {
        DEBUG_ERR("XA_RESULT_FEATURE_UNSUPPORTED");
        res = XA_RESULT_FEATURE_UNSUPPORTED;          
        }
    
    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
    DEBUG_API("<-XAPlaybackRateItfImpl_SetPropertyConstraints");
    return res;
}

XAresult XAPlaybackRateItfImpl_GetProperties(XAPlaybackRateItf self,
                                               XAuint32 *pProperties)
{
    XAresult res = XA_RESULT_SUCCESS;
    XAPlaybackRateItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAPlaybackRateItfImpl_GetProperties");

    if( !impl || !pProperties )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAPlaybackRateItfImpl_GetProperties");
        return XA_RESULT_PARAMETER_INVALID;
    }
   
    if(impl->adaptCtx->fwtype == FWMgrFWMMF)
        {
        /* needs to be queried from adaptation */
        res = XAPlaybackRateItfAdaptMMF_GetProperties((XAAdaptationMMFCtx*)impl->adaptCtx, pProperties);
        }
    else
        {
        DEBUG_ERR("XA_RESULT_FEATURE_UNSUPPORTED");
        res = XA_RESULT_FEATURE_UNSUPPORTED;          
        }
    
    DEBUG_API("<-XAPlaybackRateItfImpl_GetProperties");
    return res;
}

XAresult XAPlaybackRateItfImpl_GetCapabilitiesOfRate(XAPlaybackRateItf self,
                                                       XApermille rate,
                                                       XAuint32 *pCapabilities)
{
    XAresult res = XA_RESULT_SUCCESS;
    XAPlaybackRateItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAPlaybackRateItfImpl_GetCapabilitiesOfRate");

    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaPlayer );
    if( !impl || !pCapabilities )
    {

        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAPlaybackRateItfImpl_GetCapabilitiesOfRate");
        return XA_RESULT_PARAMETER_INVALID;
    }
    
    if(impl->adaptCtx->fwtype == FWMgrFWMMF)
        {
        /* needs to be queried from adaptation */
        res = XAPlaybackRateItfAdaptMMF_GetCapabilitiesOfRate((XAAdaptationMMFCtx*)impl->adaptCtx, rate, pCapabilities);
        }
    else
        {
        DEBUG_ERR("XA_RESULT_FEATURE_UNSUPPORTED");
        res = XA_RESULT_FEATURE_UNSUPPORTED;          
        }

    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
    DEBUG_API("<-XAPlaybackRateItfImpl_GetCapabilitiesOfRate");
    return res;
}

XAresult XAPlaybackRateItfImpl_GetRateRange(XAPlaybackRateItf self,
                                              XAuint8 index,
                                              XApermille *pMinRate,
                                              XApermille *pMaxRate,
                                              XApermille *pStepSize,
                                              XAuint32 *pCapabilities)
{
    XAresult res = XA_RESULT_SUCCESS;
    XAPlaybackRateItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAPlaybackRateItfImpl_GetRateRange");

    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaPlayer );
    if( !impl || !pMinRate || !pMaxRate || !pStepSize || !pCapabilities )
    {
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAPlaybackRateItfImpl_GetRateRange");
        return XA_RESULT_PARAMETER_INVALID;
    }
    
    if(impl->adaptCtx->fwtype == FWMgrFWMMF)
        {
        /* needs to be queried from adaptation */
        res = XAPlaybackRateItfAdaptMMF_GetRateRange((XAAdaptationMMFCtx*)impl->adaptCtx, index, pMinRate,
                                                  pMaxRate,pStepSize, pCapabilities);
        }
    else
        {
        DEBUG_ERR("XA_RESULT_FEATURE_UNSUPPORTED");
        res = XA_RESULT_FEATURE_UNSUPPORTED;          
        }
    
    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
    DEBUG_API("<-XAPlaybackRateItfImpl_GetRateRange");
    return res;
}

/**
 * XAPlaybackRateItfImpl -specific methods
 **/
/**
 * XAPlaybackRateItfImpl* XAPlaybackRateItfImpl_Create();
 * @return  XAPlaybackRateItfImpl* - Pointer to  PlaybackRateItf interface implementation
 **/
XAPlaybackRateItfImpl* XAPlaybackRateItfImpl_Create( XAMediaPlayerImpl *impl )
{
    XAPlaybackRateItfImpl *self = (XAPlaybackRateItfImpl*)
        calloc(1,sizeof(XAPlaybackRateItfImpl));
    DEBUG_API("->XAPlaybackRateItfImpl_Create");

    if(self)
    {
        /* init itf default implementation */
        self->itf.GetCapabilitiesOfRate = XAPlaybackRateItfImpl_GetCapabilitiesOfRate;
        self->itf.GetProperties = XAPlaybackRateItfImpl_GetProperties;
        self->itf.GetRate = XAPlaybackRateItfImpl_GetRate;
        self->itf.GetRateRange = XAPlaybackRateItfImpl_GetRateRange;
        self->itf.SetPropertyConstraints = XAPlaybackRateItfImpl_SetPropertyConstraints;
        self->itf.SetRate = XAPlaybackRateItfImpl_SetRate;

        /* init variables */
        self->adaptCtx = impl->curAdaptCtx;
        self->currentRate = 1000;
        self->self = self;
    }

    DEBUG_API("<-XAPlaybackRateItfImpl_Create");
    return self;
}

/**
 * void XAPlaybackRateItfImpl_Free(XAPlaybackRateItfImpl* self);
 * @param  XAPlaybackRateItfImpl* self -
 **/
void XAPlaybackRateItfImpl_Free(XAPlaybackRateItfImpl* self)
{
    DEBUG_API("->XAPlaybackRateItfImpl_Free");
    free(self);
    DEBUG_API("<-XAPlaybackRateItfImpl_Free");
}
