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
 * Description: Nokia Linear Volume Interface Implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "xanlinearvolumeitf.h"
#include "xanokialinearvolumeitfadaptationmmf.h"

/**
 * XANokiaLinearVolumeItfImpl* GetImpl(XANokiaLinearVolumeItf self)
 * Description: Validated interface pointer and cast it to implementations pointer.
 **/
static XANokiaLinearVolumeItfImpl* GetImpl(XANokiaLinearVolumeItf self)
    {
    if (self)
        {
        XANokiaLinearVolumeItfImpl* impl =
                (XANokiaLinearVolumeItfImpl*) (*self);
        if (impl && impl == impl->self)
            {
            return impl;
            }
        }
    return NULL;
    }

/**
 * XAresult XANokiaLinearVolumeItfImpl_RegisterVolumeCallback(XANokiaLinearVolumeItf self,
 *                                               xaNokiaLinearVolumeCallback callback,
 *                                               void * pContext)
 * Description: Sets or clears the xaVolumeCallback.
 **/
XAresult XANokiaLinearVolumeItfImpl_RegisterVolumeCallback(
        XANokiaLinearVolumeItf self, xaNokiaLinearVolumeCallback callback,
        void * pContext)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaLinearVolumeItfImpl* impl = GetImpl(self);
    DEBUG_API("->XANokiaLinearVolumeItfImpl_RegisterVolumeCallback");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XANokiaLinearVolumeItfImpl_RegisterVolumeCallback");
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->callback = callback;
    impl->context = pContext;
    impl->cbPtrToSelf = self;

    DEBUG_API("<-XANokiaLinearVolumeItfImpl_RegisterVolumeCallback");
    return ret;
    }

/**
 * Base interface XANokiaLinearVolumeItf implementation
 */

/**
 * XAresult XANokiaLinearVolumeItfImpl_SetVolumeLevel(XANokiaLinearVolumeItf self, XAuint32 percentage)
 * Description: Sets the object's volume level.
 **/
XAresult XANokiaLinearVolumeItfImpl_SetVolumeLevel(
        XANokiaLinearVolumeItf self, XAuint32 *percentage)
    {
    XANokiaLinearVolumeItfImpl *impl = GetImpl(self);
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 vol = *percentage;

    DEBUG_API("->XANokiaLinearVolumeItfImpl_SetVolumeLevel");

    if ((!impl) || (vol > MAX_PERCENTAGE_VOLUME))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaLinearVolumeItfImpl_SetVolumeLevel");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_SUCCESS)
        {
        if (impl->adapCtx && impl->adapCtx->fwtype == FWMgrFWMMF)
            {
            ret = XANokiaLinearVolumeItfAdapt_SetVolumeLevel(
                    (XAAdaptationMMFCtx*) impl->adapCtx, vol);
            if (ret == XA_RESULT_SUCCESS)
                {
                impl->volumeLevel = vol;
                }
            }
        XAAdaptationBase_ThreadExit(impl->adapCtx);
        }
    DEBUG_API("<-XANokiaLinearVolumeItfImpl_SetVolumeLevel");
    return ret;
    }

/**
 * XAresult XANokiaLinearVolumeItfImpl_GetVolumeLevel(XANokiaLinearVolumeItf self, XAmillibel *pLevel)
 * Description: Gets the object’s volume level.
 **/
XAresult XANokiaLinearVolumeItfImpl_GetVolumeLevel(
        XANokiaLinearVolumeItf self, XAuint32 *percentage)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaLinearVolumeItfImpl *impl = GetImpl(self);
    DEBUG_API("->XANokiaLinearVolumeItfImpl_GetVolumeLevel");

    if (!impl || !percentage)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaLinearVolumeItfImpl_GetVolumeLevel");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_SUCCESS)
        {
        if (impl->adapCtx && impl->adapCtx->fwtype == FWMgrFWMMF)
            {
            ret = XANokiaLinearVolumeItfAdapt_GetVolumeLevel(
                    (XAAdaptationMMFCtx*) impl->adapCtx, percentage);
            if (ret == XA_RESULT_SUCCESS)
                {
                impl->volumeLevel = *percentage;
                }
            }
        XAAdaptationBase_ThreadExit(impl->adapCtx);
        }

    DEBUG_API("<-XANokiaLinearVolumeItfImpl_GetVolumeLevel");
    return ret;
    }

/**
 * XAresult XANokiaLinearVolumeItfImpl_GetMaxVolumeLevel(XANokiaLinearVolumeItf  self, XAmillibel *pMaxLevel)
 * Description: Gets the maximum supported level.
 **/
XAresult XANokiaLinearVolumeItfImpl_GetStepCount(XANokiaLinearVolumeItf self,
        XAuint32 *pStepCount)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaLinearVolumeItfImpl *impl = GetImpl(self);
    DEBUG_API("->XANokiaLinearVolumeItfImpl_GetVolumeLevel");

    if (!impl || !pStepCount)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XANokiaLinearVolumeItfImpl_GetMaxVolumeLevel");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if (ret == XA_RESULT_SUCCESS)
        {
        if (impl->adapCtx && impl->adapCtx->fwtype == FWMgrFWMMF)
            {
            ret = XANokiaLinearVolumeItfAdapt_GetStepCount(
                    (XAAdaptationMMFCtx*) impl->adapCtx, pStepCount);
            }
    
        XAAdaptationBase_ThreadExit(impl->adapCtx);
        }
    DEBUG_API("<-XANokiaLinearVolumeItfImpl_GetMaxVolumeLevel");
    return ret;
    }

XAresult XANokiaLinearVolumeItfImpl_SetCallbackEventsMask(
        XANokiaLinearVolumeItf self, XAuint32 eventFlags)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaLinearVolumeItfImpl* impl = GetImpl(self);
    DEBUG_API("->XANokiaLinearVolumeItfImpl_SetCallbackEventsMask");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XANokiaLinearVolumeItfImpl_SetCallbackEventsMask");
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->eventFlags = eventFlags;

    DEBUG_API("<-XANokiaLinearVolumeItfImpl_SetCallbackEventsMask");
    return ret;
    }

XAresult XANokiaLinearVolumeItfImpl_GetCallbackEventsMask(
        XANokiaLinearVolumeItf self, XAuint32 * pEventFlags)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XANokiaLinearVolumeItfImpl* impl = GetImpl(self);
    DEBUG_API("->XANokiaLinearVolumeItfImpl_GetCallbackEventsMask");

    if (!impl || !pEventFlags)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XANokiaLinearVolumeItfImpl_GetCallbackEventsMask");
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pEventFlags = impl->eventFlags;

    DEBUG_API("<-XANokiaLinearVolumeItfImpl_GetCallbackEventsMask");
    return ret;
    }

/**
 * XANokiaLinearVolumeItfImpl -specific methods
 **/

/**
 * XANokiaLinearVolumeItfImpl* XANokiaLinearVolumeItfImpl_Create()
 * Description: Allocate and initialize VolumeItfImpl
 **/
XANokiaLinearVolumeItfImpl* XANokiaLinearVolumeItfImpl_Create(
        XAAdaptationBaseCtx *adapCtx)
    {
    XANokiaLinearVolumeItfImpl *self = (XANokiaLinearVolumeItfImpl*) calloc(
            1, sizeof(XANokiaLinearVolumeItfImpl));
    DEBUG_API("->XANokiaLinearVolumeItfImpl_Create");

    if (self)
        {
        /* init itf default implementation */
        self->itf.GetStepCount = XANokiaLinearVolumeItfImpl_GetStepCount;
        self->itf.GetVolumeLevel = XANokiaLinearVolumeItfImpl_GetVolumeLevel;
        self->itf.SetVolumeLevel = XANokiaLinearVolumeItfImpl_SetVolumeLevel;
        self->itf.RegisterVolumeCallback
                = XANokiaLinearVolumeItfImpl_RegisterVolumeCallback;
        self->itf.SetCallbackEventsMask
                = XANokiaLinearVolumeItfImpl_SetCallbackEventsMask;
        self->itf.GetCallbackEventsMask
                = XANokiaLinearVolumeItfImpl_GetCallbackEventsMask;
        /* init variables */
        self->volumeLevel = 0;

        self->adapCtx = adapCtx;

        XAAdaptationBase_AddEventHandler(adapCtx,
                &XANokiaLinearVolumeItfImpl_AdaptCb,
                XA_NOKIALINEARVOLITFEVENTS, self);

        self->self = self;
        }

    DEBUG_API("<-XANokiaLinearVolumeItfImpl_Create");
    return self;
    }

/**
 * void XANokiaLinearVolumeItfImpl_Free(XANokiaLinearVolumeItfImpl* self)
 * Description: Free all resources reserved at XANokiaLinearVolumeItfImpl_Create
 **/
void XANokiaLinearVolumeItfImpl_Free(XANokiaLinearVolumeItfImpl* self)
    {
    DEBUG_API("->XANokiaLinearVolumeItfImpl_Free");
    free(self);
    DEBUG_API("<-XANokiaLinearVolumeItfImpl_Free");
    }

/* void XANokiaLinearVolumeItfimpl_AdaptCb( void *pHandlerCtx, XAAdaptEvent *event )
 * Description: Event handler for adaptation events
 */
void XANokiaLinearVolumeItfImpl_AdaptCb(void *pHandlerCtx,
        XAAdaptEvent *event)
    {
    XANokiaLinearVolumeItfImpl* impl =
            (XANokiaLinearVolumeItfImpl*) pHandlerCtx;
    XAboolean eventBoolean = XA_BOOLEAN_FALSE;

    DEBUG_API("->XANokiaLinearVolumeItfimpl_AdaptCb");

    if (!impl || !event)
        {
        DEBUG_ERR("XANokiaLinearVolumeItfImpl_AdaptCb, invalid context pointer!");
        DEBUG_API("<-XANokiaLinearVolumeItfImpl_AdaptCb");
        return;
        }
    
    if (event->eventid == XA_ADAPT_VOLUME_VOLUME_CHANGED && impl->callback)
        {
        if (XA_NOKIALINEARVOLUME_EVENT_VOLUME_CHANGED & impl->eventFlags)
            {
            DEBUG_API("Volume level changed in adaptation");
            impl->callback(impl->cbPtrToSelf, impl->context,
                    XA_NOKIALINEARVOLUME_EVENT_VOLUME_CHANGED, eventBoolean);
            }
        }
    else
        {
        /* do nothing */
        }
    DEBUG_API("<-XANokiaLinearVolumeItfimpl_AdaptCb");
    }
