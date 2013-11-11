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
 * Description: Volume Interface Implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "xavolumeitf.h"

#include "xanokiavolumeextitfadaptationmmf.h"
/**
 * XAVolumeItfImpl* GetImpl(XAVolumeItf self)
 * Description: Validated interface pointer and cast it to implementations pointer.
 **/
static XAVolumeItfImpl* GetImpl(XAVolumeItf self)
    {
    if (self)
        {
        XAVolumeItfImpl* impl = (XAVolumeItfImpl*) (*self);
        if (impl && impl == impl->self)
            {
            return impl;
            }
        }
    return NULL;
    }

/**
 * Base interface XAVolumeItf implementation
 */

/**
 * XAresult XAVolumeItfImpl_SetVolumeLevel(XAVolumeItf self, XAmillibel level)
 * Description: Sets the object's volume level.
 **/
XAresult XAVolumeItfImpl_SetVolumeLevel(XAVolumeItf self, XAmillibel level)
    {
    XAVolumeItfImpl *impl = GetImpl(self);
    XAresult ret = XA_RESULT_SUCCESS;

    XAmillibel maximumLevel = 0;
    DEBUG_API("->XAVolumeItfImpl_SetVolumeLevel");

    /* check maximum volume level */
    if (XAVolumeItfImpl_GetMaxVolumeLevel(self, &maximumLevel)
            != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVolumeItfImpl_SetVolumeLevel");
        /* cannot solve maximum volume level */
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (!impl || level > maximumLevel || !impl->adapCtx)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVolumeItfImpl_SetVolumeLevel");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_PARAMETER_INVALID || ret
            == XA_RESULT_PRECONDITIONS_VIOLATED)
        {
        DEBUG_API("<-XAVolumeItfImpl_SetVolumeLevel");
        return ret;
        }

    if (ret == XA_RESULT_SUCCESS)
        {
        impl->volumeLevel = level;
        }

    XAAdaptationBase_ThreadExit(impl->adapCtx);

    DEBUG_API("<-XAVolumeItfImpl_SetVolumeLevel");
    return ret;
    }

/**
 * XAresult XAVolumeItfImpl_GetVolumeLevel(XAVolumeItf self, XAmillibel *pLevel)
 * Description: Gets the object’s volume level.
 **/
XAresult XAVolumeItfImpl_GetVolumeLevel(XAVolumeItf self, XAmillibel *pLevel)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAVolumeItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAVolumeItfImpl_GetVolumeLevel");

    if (!impl || !pLevel)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVolumeItfImpl_GetVolumeLevel");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pLevel = impl->volumeLevel;

    DEBUG_API("<-XAVolumeItfImpl_GetVolumeLevel");
    return ret;
    }

/**
 * XAresult XAVolumeItfImpl_GetMaxVolumeLevel(XAVolumeItf  self, XAmillibel *pMaxLevel)
 * Description: Gets the maximum supported level.
 **/
XAresult XAVolumeItfImpl_GetMaxVolumeLevel(XAVolumeItf self,
        XAmillibel *pMaxLevel)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAVolumeItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAVolumeItfImpl_GetVolumeLevel");

    if (!impl || !pMaxLevel || !impl->adapCtx)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVolumeItfImpl_GetMaxVolumeLevel");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_PARAMETER_INVALID || ret
            == XA_RESULT_PRECONDITIONS_VIOLATED)
        {
        DEBUG_API("<-XAVolumeItfImpl_GetMaxVolumeLevel");
        return ret;
        }
    *pMaxLevel = MAX_SUPPORT_VOLUME_LEVEL;

    XAAdaptationBase_ThreadExit(impl->adapCtx);

    DEBUG_API("<-XAVolumeItfImpl_GetMaxVolumeLevel");
    return ret;
    }

/**
 * XAresult XAVolumeItfImpl_SetMute(XAVolumeItf self, XAboolean mute)
 * Description: Mutes or unmutes the object.
 **/
XAresult XAVolumeItfImpl_SetMute(XAVolumeItf self, XAboolean mute)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAVolumeItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAVolumeItfImpl_SetMute");

    if (!impl || !impl->adapCtx)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVolumeItfImpl_SetMute");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_PARAMETER_INVALID || ret
            == XA_RESULT_PRECONDITIONS_VIOLATED)
        {
        DEBUG_API("<-XAVolumeItfImpl_SetMute");
        return ret;
        }
    /* check is mute state changed */
    if (mute != impl->mute)
        {
        if (impl->adapCtx->fwtype == FWMgrFWMMF)
            {
            ret = XANokiaVolumeExtItfAdapt_SetMute(
                    (XAAdaptationMMFCtx*) impl->adapCtx, mute);
            }

        if (ret == XA_RESULT_SUCCESS)
            {
            impl->mute = mute;
            }
        }

    XAAdaptationBase_ThreadExit(impl->adapCtx);

    DEBUG_API("<-XAVolumeItfImpl_SetMute");
    return ret;
    }

/**
 * XAresult XAVolumeItfImpl_GetMute(XAVolumeItf self, XAboolean *pMute)
 * Description: Retrieves the object's state.
 **/
XAresult XAVolumeItfImpl_GetMute(XAVolumeItf self, XAboolean *pMute)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAVolumeItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAVolumeItfImpl_GetMute");

    if (!impl || !pMute)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVolumeItfImpl_GetMute");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pMute = impl->mute;

    DEBUG_API("<-XAVolumeItfImpl_GetMute");
    return ret;
    }

/**
 * XAresult XAVolumeItfImpl_EnableStereoPosition(XAVolumeItf self, XAboolean enable)
 * Description: Enables or disables the stereo positioning effect.
 **/
XAresult XAVolumeItfImpl_EnableStereoPosition(XAVolumeItf self,
        XAboolean enable)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAVolumeItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAVolumeItfImpl_EnableStereoPosition");

    if (!impl || !impl->adapCtx)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVolumeItfImpl_EnableStereoPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_PARAMETER_INVALID || ret
            == XA_RESULT_PRECONDITIONS_VIOLATED)
        {
        DEBUG_API("<-XAVolumeItfImpl_EnableStereoPosition");
        return ret;
        }
    /* Check is stereo position state changed */
    if (enable != impl->enableStereoPos)
        {
        if (impl->adapCtx->fwtype == FWMgrFWMMF)
            {
            ret = XANokiaVolumeExtItfAdapt_EnableStereoPosition(
                    (XAAdaptationMMFCtx*) impl->adapCtx, enable);
            }
        if (ret == XA_RESULT_SUCCESS)
            {
            impl->enableStereoPos = enable;
            }
        }

    XAAdaptationBase_ThreadExit(impl->adapCtx);

    DEBUG_API("<-XAVolumeItfImpl_EnableStereoPosition");
    return ret;
    }

/**
 * XAresult XAVolumeItfImpl_IsEnabledStereoPosition(XAVolumeItf self,
 *                                                  XAboolean *pEnable)
 * Description: Returns the enabled state of the stereo positioning effect.
 **/
XAresult XAVolumeItfImpl_IsEnabledStereoPosition(XAVolumeItf self,
        XAboolean *pEnable)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAVolumeItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAVolumeItfImpl_IsEnabledStereoPosition");

    if (!impl || !pEnable)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVolumeItfImpl_IsEnabledStereoPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pEnable = impl->enableStereoPos;

    DEBUG_API("<-XAVolumeItfImpl_IsEnabledStereoPosition");
    return ret;
    }

/**
 * XAresult XAVolumeItfImpl_SetStereoPosition(XAVolumeItf self,
 *                                            XApermille stereoPosition)
 * Description: Sets the stereo position of the object.
 **/
XAresult XAVolumeItfImpl_SetStereoPosition(XAVolumeItf self,
        XApermille stereoPosition)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAVolumeItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAVolumeItfImpl_SetStereoPosition");

    if (!impl || (stereoPosition < STEREO_POSITION_LEFT) || (stereoPosition
            > STEREO_POSITION_RIGHT) || !impl->adapCtx)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVolumeItfImpl_SetStereoPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->stereoPosition = stereoPosition;

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_PARAMETER_INVALID || ret
            == XA_RESULT_PRECONDITIONS_VIOLATED)
        {
        DEBUG_API("<-XAVolumeItfImpl_SetStereoPosition");
        return ret;
        }
    /* check is stereo position effect enabled if is then handle effect */
    if (impl->enableStereoPos)
        {
        if (impl->adapCtx->fwtype == FWMgrFWMMF)
            {
            ret = XANokiaVolumeExtItfAdapt_SetStereoPosition(
                    (XAAdaptationMMFCtx*) impl->adapCtx, stereoPosition);
            }
        }

    XAAdaptationBase_ThreadExit(impl->adapCtx);

    DEBUG_API("<-XAVolumeItfImpl_SetStereoPosition");
    return ret;
    }

/**
 * XAresult XAVolumeItfImpl_GetStereoPosition(XAVolumeItf self,
 *                                            XApermille *pStereoPosition)
 * Description: Gets the object’s stereo position setting.
 **/
XAresult XAVolumeItfImpl_GetStereoPosition(XAVolumeItf self,
        XApermille *pStereoPosition)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAVolumeItfImpl *impl = GetImpl(self);
    DEBUG_API("->XAVolumeItfImpl_GetStereoPosition");

    if (!impl || !pStereoPosition)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVolumeItfImpl_GetStereoPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pStereoPosition = impl->stereoPosition;

    DEBUG_API("<-XAVolumeItfImpl_GetStereoPosition");
    return ret;
    }

/**
 * XAVolumeItfImpl -specific methods
 **/

/**
 * XAVolumeItfImpl* XAVolumeItfImpl_Create()
 * Description: Allocate and initialize VolumeItfImpl
 **/
XAVolumeItfImpl* XAVolumeItfImpl_Create(XAAdaptationBaseCtx *adapCtx)
    {
    XAVolumeItfImpl *self = (XAVolumeItfImpl*) calloc(1,
            sizeof(XAVolumeItfImpl));
    DEBUG_API("->XAVolumeItfImpl_Create");

    if (self)
        {
        /* init itf default implementation */
        self->itf.EnableStereoPosition = XAVolumeItfImpl_EnableStereoPosition;
        self->itf.GetMaxVolumeLevel = XAVolumeItfImpl_GetMaxVolumeLevel;
        self->itf.GetMute = XAVolumeItfImpl_GetMute;
        self->itf.GetStereoPosition = XAVolumeItfImpl_GetStereoPosition;
        self->itf.GetVolumeLevel = XAVolumeItfImpl_GetVolumeLevel;
        self->itf.IsEnabledStereoPosition
                = XAVolumeItfImpl_IsEnabledStereoPosition;
        self->itf.SetMute = XAVolumeItfImpl_SetMute;
        self->itf.SetStereoPosition = XAVolumeItfImpl_SetStereoPosition;
        self->itf.SetVolumeLevel = XAVolumeItfImpl_SetVolumeLevel;

        /* init variables */
        self->volumeLevel = 0;
        self->mute = XA_BOOLEAN_FALSE;
        self->enableStereoPos = XA_BOOLEAN_FALSE;
        self->stereoPosition = STEREO_POSITION_CENTER;

        self->adapCtx = adapCtx;

        self->self = self;
        }

    DEBUG_API("<-XAVolumeItfImpl_Create");
    return self;
    }

/**
 * void XAVolumeItfImpl_Free(XAVolumeItfImpl* self)
 * Description: Free all resources reserved at XAVolumeItfImpl_Create
 **/
void XAVolumeItfImpl_Free(XAVolumeItfImpl* self)
    {
    DEBUG_API("->XAVolumeItfImpl_Free");
    free(self);
    DEBUG_API("<-XAVolumeItfImpl_Free");
    }
