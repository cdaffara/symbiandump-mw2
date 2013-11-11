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

#include "xanvolumeextitf.h"
#include "xanokiavolumeextitfadaptationmmf.h"

/**
 * XANokiaVolumeExtItfImpl* GetImpl(XANokiaVolumeExtItf self)
 * Description: Validated interface pointer and cast it to implementations pointer.
 **/
static XANokiaVolumeExtItfImpl* GetImpl(XANokiaVolumeExtItf self)
    {
    if (self)
        {
        XANokiaVolumeExtItfImpl* impl = (XANokiaVolumeExtItfImpl*) (*self);
        if (impl && impl == impl->self)
            {
            return impl;
            }
        }
    return NULL;
    }

/**
 * XAresult XANokiaVolumeExtItfImpl_RegisterVolumeCallback(XANokiaVolumeExtItf self,
 *                                               xaNokiaVolumeExtCallback callback,
 *                                               void * pContext)
 * Description: Sets or clears the xaVolumeCallback.
 **/
XAresult XANokiaVolumeExtItfImpl_RegisterVolumeCallback(
        XANokiaVolumeExtItf self, xaNokiaVolumeExtCallback callback,
        void * pContext)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl* impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_RegisterVolumeCallback");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XANokiaVolumeExtItfImpl_RegisterVolumeCallback");
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->callback = callback;
    impl->context = pContext;
    impl->cbPtrToSelf = self;

    DEBUG_API("<-XANokiaVolumeExtItfImpl_RegisterVolumeCallback");
    return ret;
    }

/**
 * Base interface XANokiaVolumeExtItf implementation
 */

/**
 * XAresult XANokiaVolumeExtItfImpl_SetVolumeLevel(XANokiaVolumeExtItf self, XAmillibel level)
 * Description: Sets the object's volume level.
 **/
XAresult XANokiaVolumeExtItfImpl_SetVolumeLevel(XANokiaVolumeExtItf self,
        XAmillibel level)
    {
    XANokiaVolumeExtItfImpl *impl = GetImpl(self);
    XAresult ret = XA_RESULT_SUCCESS;

    XAmillibel maximumLevel = 0;

    DEBUG_API("->XANokiaVolumeExtItfImpl_SetVolumeLevel");

    /* check maximum volume level */
    if (XANokiaVolumeExtItfImpl_GetMaxVolumeLevel(self, &maximumLevel)
            != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_SetVolumeLevel");
        /* cannot solve maximum volume level */
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (!impl || level > maximumLevel)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_SetVolumeLevel");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->volumeLevel = level;

    DEBUG_API("<-XANokiaVolumeExtItfImpl_SetVolumeLevel");
    return ret;
    }

/**
 * XAresult XANokiaVolumeExtItfImpl_GetVolumeLevel(XANokiaVolumeExtItf self, XAmillibel *pLevel)
 * Description: Gets the object’s volume level.
 **/
XAresult XANokiaVolumeExtItfImpl_GetVolumeLevel(XANokiaVolumeExtItf self,
        XAmillibel *pLevel)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl *impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_GetVolumeLevel");

    if (!impl || !pLevel)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_GetVolumeLevel");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pLevel = impl->volumeLevel;

    DEBUG_API("<-XANokiaVolumeExtItfImpl_GetVolumeLevel");
    return ret;
    }

/**
 * XAresult XANokiaVolumeExtItfImpl_GetMaxVolumeLevel(XANokiaVolumeExtItf  self, XAmillibel *pMaxLevel)
 * Description: Gets the maximum supported level.
 **/
XAresult XANokiaVolumeExtItfImpl_GetMaxVolumeLevel(XANokiaVolumeExtItf self,
        XAmillibel *pMaxLevel)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl *impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_GetVolumeLevel");

    if (!impl || !pMaxLevel)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_GetMaxVolumeLevel");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pMaxLevel = MAX_SUPPORT_VOLUME_LEVEL;

    DEBUG_API("<-XANokiaVolumeExtItfImpl_GetMaxVolumeLevel");
    return ret;
    }

/**
 * XAresult XANokiaVolumeExtItfImpl_SetMute(XANokiaVolumeExtItf self, XAboolean mute)
 * Description: Mutes or unmutes the object.
 **/
XAresult XANokiaVolumeExtItfImpl_SetMute(XANokiaVolumeExtItf self,
        XAboolean mute)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl *impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_SetMute");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_SetMute");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_SUCCESS)
        {
        /* check is mute state changed */
        if (mute != impl->mute)
            {
            if (impl->adapCtx && impl->adapCtx->fwtype == FWMgrFWMMF)
                {
                ret = XANokiaVolumeExtItfAdapt_SetMute(
                        (XAAdaptationMMFCtx*) impl->adapCtx, mute);
                if (ret == XA_RESULT_SUCCESS)
                    {
                    impl->mute = mute;
                    }
                }
            }

        XAAdaptationBase_ThreadExit(impl->adapCtx);
        }
    DEBUG_API("<-XANokiaVolumeExtItfImpl_SetMute");
    return ret;
    }

/**
 * XAresult XANokiaVolumeExtItfImpl_GetMute(XANokiaVolumeExtItf self, XAboolean *pMute)
 * Description: Retrieves the object's state.
 **/
XAresult XANokiaVolumeExtItfImpl_GetMute(XANokiaVolumeExtItf self,
        XAboolean *pMute)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl *impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_GetMute");

    if (!impl || !pMute)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_GetMute");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pMute = impl->mute;

    DEBUG_API("<-XANokiaVolumeExtItfImpl_GetMute");
    return ret;
    }

/**
 * XAresult XANokiaVolumeExtItfImpl_EnableStereoPosition(XANokiaVolumeExtItf self, XAboolean enable)
 * Description: Enables or disables the stereo positioning effect.
 **/
XAresult XANokiaVolumeExtItfImpl_EnableStereoPosition(
        XANokiaVolumeExtItf self, XAboolean enable)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl *impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_EnableStereoPosition");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_EnableStereoPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_PARAMETER_INVALID || ret
            == XA_RESULT_PRECONDITIONS_VIOLATED)
        {
        DEBUG_API("<-XANokiaVolumeExtItfImpl_EnableStereoPosition");
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
    DEBUG_API("<-XANokiaVolumeExtItfImpl_EnableStereoPosition");
    return ret;
    }

/**
 * XAresult XANokiaVolumeExtItfImpl_IsEnabledStereoPosition(XANokiaVolumeExtItf self,
 *                                                  XAboolean *pEnable)
 * Description: Returns the enabled state of the stereo positioning effect.
 **/
XAresult XANokiaVolumeExtItfImpl_IsEnabledStereoPosition(
        XANokiaVolumeExtItf self, XAboolean *pEnable)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl *impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_IsEnabledStereoPosition");

    if (!impl || !pEnable)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_IsEnabledStereoPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pEnable = impl->enableStereoPos;

    DEBUG_API("<-XANokiaVolumeExtItfImpl_IsEnabledStereoPosition");
    return ret;
    }

/**
 * XAresult XANokiaVolumeExtItfImpl_SetStereoPosition(XANokiaVolumeExtItf self,
 *                                            XApermille stereoPosition)
 * Description: Sets the stereo position of the object.
 **/
XAresult XANokiaVolumeExtItfImpl_SetStereoPosition(XANokiaVolumeExtItf self,
        XApermille stereoPosition)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl *impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_SetStereoPosition");

    if (!impl || (stereoPosition < STEREO_POSITION_LEFT) || (stereoPosition
            > STEREO_POSITION_RIGHT))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_SetStereoPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->stereoPosition = stereoPosition;

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_PARAMETER_INVALID || ret
            == XA_RESULT_PRECONDITIONS_VIOLATED)
        {
        DEBUG_API("<-XANokiaVolumeExtItfImpl_SetStereoPosition");
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
    DEBUG_API("<-XANokiaVolumeExtItfImpl_SetStereoPosition");
    return ret;
    }

/**
 * XAresult XANokiaVolumeExtItfImpl_GetStereoPosition(XANokiaVolumeExtItf self,
 *                                            XApermille *pStereoPosition)
 * Description: Gets the object’s stereo position setting.
 **/
XAresult XANokiaVolumeExtItfImpl_GetStereoPosition(XANokiaVolumeExtItf self,
        XApermille *pStereoPosition)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl *impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_GetStereoPosition");

    if (!impl || !pStereoPosition)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_GetStereoPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pStereoPosition = impl->stereoPosition;

    DEBUG_API("<-XANokiaVolumeExtItfImpl_GetStereoPosition");
    return ret;
    }

XAresult XANokiaVolumeExtItfImpl_SetCallbackEventsMask(
        XANokiaVolumeExtItf self, XAuint32 eventFlags)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl* impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_SetCallbackEventsMask");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XANokiaVolumeExtItfImpl_SetCallbackEventsMask");
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->eventFlags = eventFlags;

    DEBUG_API("<-XANokiaVolumeExtItfImpl_SetCallbackEventsMask");
    return ret;
    }

XAresult XANokiaVolumeExtItfImpl_GetCallbackEventsMask(
        XANokiaVolumeExtItf self, XAuint32 * pEventFlags)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaVolumeExtItfImpl* impl = GetImpl(self);
    DEBUG_API("->XANokiaVolumeExtItfImpl_GetCallbackEventsMask");

    if (!impl || !pEventFlags)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XANokiaVolumeExtItfImpl_GetCallbackEventsMask");
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pEventFlags = impl->eventFlags;

    DEBUG_API("<-XANokiaVolumeExtItfImpl_GetCallbackEventsMask");
    return ret;
    }

/**
 * XANokiaVolumeExtItfImpl -specific methods
 **/

/**
 * XANokiaVolumeExtItfImpl* XANokiaVolumeExtItfImpl_Create()
 * Description: Allocate and initialize VolumeItfImpl
 **/
XANokiaVolumeExtItfImpl* XANokiaVolumeExtItfImpl_Create(
        XAAdaptationBaseCtx *adapCtx)
    {
    XANokiaVolumeExtItfImpl *self = (XANokiaVolumeExtItfImpl*) calloc(1,
            sizeof(XANokiaVolumeExtItfImpl));
    DEBUG_API("->XANokiaVolumeExtItfImpl_Create");

    if (self)
        {
        /* init itf default implementation */
        self->itf.EnableStereoPosition
                = XANokiaVolumeExtItfImpl_EnableStereoPosition;
        self->itf.GetMaxVolumeLevel
                = XANokiaVolumeExtItfImpl_GetMaxVolumeLevel;
        self->itf.GetMute = XANokiaVolumeExtItfImpl_GetMute;
        self->itf.GetStereoPosition
                = XANokiaVolumeExtItfImpl_GetStereoPosition;
        self->itf.GetVolumeLevel = XANokiaVolumeExtItfImpl_GetVolumeLevel;
        self->itf.IsEnabledStereoPosition
                = XANokiaVolumeExtItfImpl_IsEnabledStereoPosition;
        self->itf.SetMute = XANokiaVolumeExtItfImpl_SetMute;
        self->itf.SetStereoPosition
                = XANokiaVolumeExtItfImpl_SetStereoPosition;
        self->itf.SetVolumeLevel = XANokiaVolumeExtItfImpl_SetVolumeLevel;
        self->itf.RegisterVolumeCallback
                = XANokiaVolumeExtItfImpl_RegisterVolumeCallback;
        self->itf.SetCallbackEventsMask
                = XANokiaVolumeExtItfImpl_SetCallbackEventsMask;
        self->itf.GetCallbackEventsMask
                = XANokiaVolumeExtItfImpl_GetCallbackEventsMask;

        /* init variables */
        self->volumeLevel = 0;
        self->mute = XA_BOOLEAN_FALSE;
        self->enableStereoPos = XA_BOOLEAN_FALSE;
        self->stereoPosition = STEREO_POSITION_CENTER;

        self->adapCtx = adapCtx;

        XAAdaptationBase_AddEventHandler(adapCtx,
                &XANokiaVolumeExtItfImpl_AdaptCb, XA_NOKIAEXTVOLITFEVENTS,
                self);

        self->self = self;
        }

    DEBUG_API("<-XANokiaVolumeExtItfImpl_Create");
    return self;
    }

/**
 * void XANokiaVolumeExtItfImpl_Free(XANokiaVolumeExtItfImpl* self)
 * Description: Free all resources reserved at XANokiaVolumeExtItfImpl_Create
 **/
void XANokiaVolumeExtItfImpl_Free(XANokiaVolumeExtItfImpl* self)
    {
    DEBUG_API("->XANokiaVolumeExtItfImpl_Free");
    free(self);
    DEBUG_API("<-XANokiaVolumeExtItfImpl_Free");
    }

/* void XANokiaVolumeExtItfimpl_AdaptCb( void *pHandlerCtx, XAAdaptEvent *event )
 * Description: Event handler for adaptation events
 */
void XANokiaVolumeExtItfImpl_AdaptCb(void *pHandlerCtx, XAAdaptEvent *event)
    {
    XANokiaVolumeExtItfImpl* impl = (XANokiaVolumeExtItfImpl*) pHandlerCtx;
    XAboolean eventBoolean = XA_BOOLEAN_FALSE;

    DEBUG_API("->XANokiaVolumeExtItfimpl_AdaptCb");

    if (!impl)
        {
        DEBUG_ERR("XANokiaVolumeExtItfImpl_AdaptCb, invalid context pointer!");
        DEBUG_API("<-XANokiaVolumeExtItfImpl_AdaptCb");
        return;
        }

    if (event->eventid == XA_ADAPT_VOLUME_MUTE_CHANGED && impl->callback)
        {
        if (impl->eventFlags & XA_NOKIAVOLUMEEXT_EVENT_MUTE_CHANGED)
            {
            DEBUG_API("Mute Status  changed in adaptation");
            eventBoolean = *(XAboolean*) event->data;
            impl->callback(impl->cbPtrToSelf, impl->context,
                    XA_NOKIAVOLUMEEXT_EVENT_MUTE_CHANGED, eventBoolean);
            }
        }

    else if (event->eventid == XA_ADAPT_VOLUME_VOLUME_CHANGED)
        {
        if (impl->eventFlags & XA_NOKIAVOLUMEEXT_EVENT_VOLUME_CHANGED)
            {
            DEBUG_API("Volume Status  changed in adaptation");
            eventBoolean = *(XAboolean*) event->data;
            impl->callback(impl->cbPtrToSelf, impl->context,
                    XA_NOKIAVOLUMEEXT_EVENT_VOLUME_CHANGED, eventBoolean);
            }
        }
    else if (event->eventid == XA_ADAPT_VULOME_STEREOPOSITION_CHANGED)
        {
        if (impl->eventFlags
                & XA_NOKIAVOLUMEEXT_EVENT_STEREO_POSITION_CHANGED)
            {
            DEBUG_API("StereoPosituin Status  changed in adaptation");
            eventBoolean = *(XAboolean*) event->data;
            impl->callback(impl->cbPtrToSelf, impl->context,
                    XA_NOKIAVOLUMEEXT_EVENT_STEREO_POSITION_CHANGED,
                    eventBoolean);
            }
        }

    DEBUG_API("<-XANokiaVolumeExtItfimpl_AdaptCb");
    }
