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
 * Description: Record Itf Impl
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "xarecorditf.h"

#include "xarecorditfadaptation.h"

#include "xarecorditfadaptationmmf.h"
#include "xathreadsafety.h"
#include <string.h>
/**
 * XARecordItfImpl* GetImpl(XARecordItf self)
 * Description: Validate interface pointer and cast it to implementation pointer.
 **/
static XARecordItfImpl* GetImpl(XARecordItf self)
    {
    if (self)
        {
        XARecordItfImpl* impl = (XARecordItfImpl*) (*self);
        if (impl && (impl == impl->self))
            {
            return impl;
            }
        }
    return NULL;
    }

/*****************************************************************************
 * Base interface XARecordItf implementation
 *****************************************************************************/

/**
 * XAresult XARecordItfImpl_SetRecordState(XARecordItf self,
 *                                         XAuint32 state)
 * Description: Transitions recorder into the given record state.
 **/
XAresult XARecordItfImpl_SetRecordState(XARecordItf self, XAuint32 state)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API("->XARecordItfImpl_SetRecordState");

    if (!impl || state < XA_RECORDSTATE_STOPPED || state
            > XA_RECORDSTATE_RECORDING)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_SetRecordState");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* check is play state changed, if not do nothing */
    if (state != impl->recordState)
        {
        if (impl->adapCtx->fwtype == FWMgrFWMMF)
            {
            ret = XARecordItfAdaptMMF_SetRecordState(
                    (XAAdaptationMMFCtx*) impl->adapCtx, state);
            }
        else
            {
            ret = XARecordItfAdapt_SetRecordState(
                    (XAAdaptationGstCtx*) impl->adapCtx, state);
            }

        if (ret == XA_RESULT_SUCCESS)
            {
            impl->recordState = state;
            }
        }

    DEBUG_API("<-XARecordItfImpl_SetRecordState");
    return ret;
    }

/**
 * XAresult XARecordItfImpl_GetRecordState(XARecordItf self,
 *                                         XAuint32 *pState)
 * Description: Gets the recorder’s current record state.
 **/
XAresult XARecordItfImpl_GetRecordState(XARecordItf self, XAuint32 *pState)
    {
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API("->XARecordItfImpl_GetRecordState");

    if (!impl || !pState)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_GetRecordState");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pState = impl->recordState;

    DEBUG_API("<-XARecordItfImpl_GetRecordState");

    return XA_RESULT_SUCCESS;
    }

/**
 * XAresult XARecordItfImpl_SetDurationLimit(XARecordItf self,
 *                                           XAmillisecond msec)
 * Description: Sets the duration of current content in milliseconds.
 **/
XAresult XARecordItfImpl_SetDurationLimit(XARecordItf self,
        XAmillisecond msec)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API("->XARecordItfImpl_SetDurationLimit");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSMediaRecorder);
    if (!impl || msec <= 0)
        {
        /* invalid parameter */
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaRecorder);
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_SetDurationLimit");
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->durationLimitSetted = 1;
    impl->durationLimit = msec;
    if (impl->adapCtx->fwtype == FWMgrFWGST)
        {
        ret = XARecordItfAdapt_EnablePositionTracking(
                (XAAdaptationGstCtx*) impl->adapCtx, 1);
        }
    else
        {
        ret = XARecordItfAdaptMMF_EnablePositionTracking(
                (XAAdaptationMMFCtx*) impl->adapCtx, 1);
        }

    XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaRecorder);
    DEBUG_API("<-XARecordItfImpl_SetDurationLimit");
    return ret;
    }

/**
 * XAresult XARecordItfImpl_GetPosition(XARecordItf self,
 *                                      XAmillisecond *pMsec)
 * Description: Returns the current position of the recording head relative
 *              to the beginning of content.
 **/
XAresult XARecordItfImpl_GetPosition(XARecordItf self, XAmillisecond *pMsec)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API("->XARecordItfImpl_GetPosition");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSMediaRecorder);
    if (!impl || !pMsec)
        {
        /* invalid parameter */
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaRecorder);
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_GetPosition");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->adapCtx->fwtype == FWMgrFWGST)
        {
        ret = XARecordItfAdapt_GetPosition(
                (XAAdaptationGstCtx*) impl->adapCtx, pMsec);
        }
    else
        {
        ret = XARecordItfAdaptMMF_GetPosition(
                (XAAdaptationMMFCtx*) impl->adapCtx, pMsec);
        }

    XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaRecorder);
    DEBUG_API("<-XARecordItfImpl_GetPosition");
    return ret;
    }

/**
 * XAresult XARecordItfImpl_RegisterCallback(XARecordItf self,
 *                                           xaRecordCallback callback,
 *                                           void *pContext)
 * Description: Registers the record callback function.
 **/
XAresult XARecordItfImpl_RegisterCallback(XARecordItf self,
        xaRecordCallback callback, void *pContext)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API("->XARecordItfImpl_RegisterCallback");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_RegisterCallback");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (callback)
        {
        impl->callback = callback;
        impl->cbcontext = pContext;
        impl->cbPtrToSelf = self;
        }
    else
        {
        /* There is no callback */
        }

    DEBUG_API("<-XARecordItfImpl_RegisterCallback");
    return ret;
    }

/**
 * XAresult XARecordItfImpl_SetCallbackEventsMask(XARecordItf self,
 *                                                XAuint32 eventFlags)
 * Description: Sets the notification state of record events.
 **/
XAresult XARecordItfImpl_SetCallbackEventsMask(XARecordItf self,
        XAuint32 eventFlags)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XARecordItfImpl* impl = GetImpl(self);
    DEBUG_API_A1("->XARecordItfImpl_SetCallbackEventsMask- %lu", eventFlags);
    //XA_IMPL_THREAD_SAFETY_ENTRY(XATSMediaRecorder);

    if (!impl || (eventFlags > (XA_RECORDEVENT_HEADATLIMIT
            | XA_RECORDEVENT_HEADATMARKER | XA_RECORDEVENT_HEADATNEWPOS
            | XA_RECORDEVENT_HEADMOVING | XA_RECORDEVENT_HEADSTALLED
            | XA_RECORDEVENT_BUFFER_FULL)))
        {
        /* invalid parameter */
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaRecorder);
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_SetCallbackEventsMask");
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->eventFlags = eventFlags;

    if (impl->adapCtx->fwtype == FWMgrFWGST)
        {
        /* enable position tracking if client wants so */
        if ((eventFlags & (XA_RECORDEVENT_HEADATMARKER
                | XA_RECORDEVENT_HEADATNEWPOS)) && impl->adapCtx
                && !impl->positionupdateOn)
            {
            ret = XARecordItfAdapt_EnablePositionTracking(
                    (XAAdaptationGstCtx*) impl->adapCtx, XA_BOOLEAN_TRUE);
            if (ret == XA_RESULT_SUCCESS)
                {
                impl->positionupdateOn = XA_BOOLEAN_TRUE;
                }
            }
        else if (!(eventFlags & (XA_RECORDEVENT_HEADATMARKER
                | XA_RECORDEVENT_HEADATNEWPOS)) && impl->adapCtx
                && impl->positionupdateOn)
            {
            ret = XARecordItfAdapt_EnablePositionTracking(
                    (XAAdaptationGstCtx*) impl->adapCtx, XA_BOOLEAN_FALSE);
            if (ret == XA_RESULT_SUCCESS)
                {
                impl->positionupdateOn = XA_BOOLEAN_FALSE;
                }
            }
        }
    else
        {
        /* enable position tracking if client wants so */
        if ((eventFlags & (XA_RECORDEVENT_HEADATMARKER
                | XA_RECORDEVENT_HEADATNEWPOS)) && impl->adapCtx
                && !impl->positionupdateOn)
            {
            ret = XARecordItfAdaptMMF_EnablePositionTracking(
                    (XAAdaptationMMFCtx*) impl->adapCtx, XA_BOOLEAN_TRUE);
            if (ret == XA_RESULT_SUCCESS)
                {
                impl->positionupdateOn = XA_BOOLEAN_TRUE;
                }
            }
        else if (!(eventFlags & (XA_RECORDEVENT_HEADATMARKER
                | XA_RECORDEVENT_HEADATNEWPOS)) && impl->adapCtx
                && impl->positionupdateOn)
            {
            ret = XARecordItfAdaptMMF_EnablePositionTracking(
                    (XAAdaptationMMFCtx*) impl->adapCtx, XA_BOOLEAN_FALSE);
            if (ret == XA_RESULT_SUCCESS)
                {
                impl->positionupdateOn = XA_BOOLEAN_FALSE;
                }
            }
        }
    //XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaRecorder);
    DEBUG_API("<-XARecordItfImpl_SetCallbackEventsMask");
    return ret;
    }

/**
 * XAresult XARecordItfImpl_GetCallbackEventsMask(XARecordItf self,
 *                                                XAuint32 *pEventFlags)
 * Description: Queries the notification state of record events.
 **/
XAresult XARecordItfImpl_GetCallbackEventsMask(XARecordItf self,
        XAuint32 *pEventFlags)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API("->XARecordItfImpl_GetCallbackEventsMask");

    DEBUG_INFO_A1("pEventFlags - %u", (int) pEventFlags);

    if (!impl || !pEventFlags)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_GetCallbackEventsMask");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pEventFlags = impl->eventFlags;
    DEBUG_API("<-XARecordItfImpl_GetCallbackEventsMask");
    return ret;
    }

/**
 * XAresult XARecordItfImpl_SetMarkerPosition(XARecordItf self,
 *                                            XAmillisecond mSec)
 * Description: Sets the position of the recording marker.
 **/
XAresult XARecordItfImpl_SetMarkerPosition(XARecordItf self,
        XAmillisecond mSec)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API_A1("->XARecordItfImpl_SetMarkerPosition, mSec-%lu",mSec);

    if (!impl
            || ((impl->durationLimitSetted) && (mSec > impl->durationLimit)))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_SetMarkerPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->markerPosition = mSec;

    DEBUG_API("<-XARecordItfImpl_SetMarkerPosition");
    return ret;
    }

/**
 * XAresult XARecordItfImpl_ClearMarkerPosition(XARecordItf self)
 * Description: Clears marker.
 **/
XAresult XARecordItfImpl_ClearMarkerPosition(XARecordItf self)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API("->XARecordItfImpl_ClearMarkerPosition");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_ClearMarkerPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->markerPosition = NO_POSITION;

    DEBUG_API("<-XARecordItfImpl_ClearMarkerPosition");
    return ret;
    }

/**
 * XAresult XARecordItfImpl_GetMarkerPosition(XARecordItf self,
 *                                            XAmillisecond *pMsec)
 * Description: Queries the position of the recording marker.
 **/
XAresult XARecordItfImpl_GetMarkerPosition(XARecordItf self,
        XAmillisecond *pMsec)
    {
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API("->XARecordItfImpl_GetMarkerPosition");

    if (!impl || !pMsec)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_GetMarkerPosition");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }
    
    if (impl->markerPosition == NO_POSITION)
        {
        DEBUG_ERR("No marker position set.");
        return XA_RESULT_PRECONDITIONS_VIOLATED;
        }

    *pMsec = impl->markerPosition;

    DEBUG_API("<-XARecordItfImpl_GetMarkerPosition");
    return XA_RESULT_SUCCESS;
    }

/**
 * XAresult XARecordItfImpl_SetPositionUpdatePeriod(XARecordItf self,
 *                                                  XAmillisecond mSec)
 * Description: Sets the interval between periodic position notifications.
 **/
XAresult XARecordItfImpl_SetPositionUpdatePeriod(XARecordItf self,
        XAmillisecond mSec)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API_A1("->XARecordItfImpl_SetPositionUpdatePeriod, mSec-%lu",mSec);

    if (!impl
            || ((impl->durationLimitSetted) && (mSec > impl->durationLimit)))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_SetPositionUpdatePeriod");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    impl->positionUpdatePeriod = mSec;

    DEBUG_API("<-XARecordItfImpl_SetPositionUpdatePeriod");
    return ret;
    }

/**
 * XAresult XARecordItfImpl_GetPositionUpdatePeriod(XARecordItf self,
 *                                                  XAmillisecond *pMsec)
 * Description: Queries the interval between periodic position notifications.
 **/
XAresult XARecordItfImpl_GetPositionUpdatePeriod(XARecordItf self,
        XAmillisecond *pMsec)
    {
    XARecordItfImpl *impl = GetImpl(self);
    DEBUG_API("->XARecordItfImpl_GetPositionUpdatePeriod");

    if (!impl || !pMsec)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfImpl_GetPositionUpdatePeriod");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pMsec = impl->positionUpdatePeriod;

    DEBUG_API("<-XARecordItfImpl_GetPositionUpdatePeriod");
    return XA_RESULT_SUCCESS;
    }

/*****************************************************************************
 * XARecordItfImpl -specific methods
 *****************************************************************************/

/**
 * XARecordItfImpl* XARecordItfImpl_Create()
 * Description: Allocate and initialize XARecordItfImpl
 **/
XARecordItfImpl* XARecordItfImpl_Create(XAMediaRecorderImpl* impl)
    {
    XARecordItfImpl* self = (XARecordItfImpl*) calloc(1,
            sizeof(XARecordItfImpl));

    DEBUG_API("->XARecordItfImpl_Create");
    if (self)
        {
        /* init itf default implementation */
        self->itf.ClearMarkerPosition = XARecordItfImpl_ClearMarkerPosition;
        self->itf.GetCallbackEventsMask
                = XARecordItfImpl_GetCallbackEventsMask;
        self->itf.GetMarkerPosition = XARecordItfImpl_GetMarkerPosition;
        self->itf.GetPosition = XARecordItfImpl_GetPosition;
        self->itf.GetPositionUpdatePeriod
                = XARecordItfImpl_GetPositionUpdatePeriod;
        self->itf.GetRecordState = XARecordItfImpl_GetRecordState;
        self->itf.RegisterCallback = XARecordItfImpl_RegisterCallback;
        self->itf.SetCallbackEventsMask
                = XARecordItfImpl_SetCallbackEventsMask;
        self->itf.SetDurationLimit = XARecordItfImpl_SetDurationLimit;
        self->itf.SetMarkerPosition = XARecordItfImpl_SetMarkerPosition;
        self->itf.SetPositionUpdatePeriod
                = XARecordItfImpl_SetPositionUpdatePeriod;
        self->itf.SetRecordState = XARecordItfImpl_SetRecordState;

        /* init variables */
        self->recordState = XA_RECORDSTATE_STOPPED;
        self->markerPosition = NO_POSITION;
        self->durationLimit = DURATION_LIMIT_NOT_SETTED;
        self->durationLimitSetted = 0;
        self->positionUpdatePeriod = RECORDITF_DEFAULT_UPDATE_PERIOD;
        self->lastPosition = START_POSITION;
        self->callback = NULL;
        self->cbcontext = NULL;
        self->eventFlags = 0;
        self->cbPtrToSelf = NULL;
        self->pObjImpl = impl;
        self->adapCtx = impl->adaptationCtx;
        XAAdaptationBase_AddEventHandler(impl->adaptationCtx,
                &XARecordItfImpl_AdaptCb, XA_RECORDITFEVENTS, self);
        self->self = self;
        }
    DEBUG_API("<-XARecordItfImpl_Create");
    return self;
    }

/**
 * void XARecordItfImpl_Free(XARecordItfImpl* self)
 * Description: Free all resources reserved at XARecordItfImpl_Create
 **/
void XARecordItfImpl_Free(XARecordItfImpl* self)
    {
    DEBUG_API("->XARecordItfImpl_Free");
    XAAdaptationBase_RemoveEventHandler(self->adapCtx,
            &XARecordItfImpl_AdaptCb);
    free(self);
    DEBUG_API("<-XARecordItfImpl_Free");
    }

/* void XARecordItfImpl_AdaptCb
 * Description: Listen changes in adaptation
 */
void XARecordItfImpl_AdaptCb(void *pHandlerCtx, XAAdaptEvent *event)
    {
    XARecordItfImpl* impl = (XARecordItfImpl*) pHandlerCtx;
    DEBUG_API("->XARecordItfImpl_AdaptCb");

    if (!impl)
        {
        DEBUG_ERR("XARecordItfImpl_AdaptCb, invalid context pointer!");
        DEBUG_API("<-XARecordItfImpl_AdaptCb");
        return;
        }
    /* check position update events */
    if (event->eventid == XA_ADAPT_POSITION_UPDATE_EVT)
        {
        XAuint32 newpos = 0;
        newpos = *((XAuint32*) (event->data));
        DEBUG_INFO_A1("new position %u",newpos);
        /* check if marker passed and callback needed */
        if ((impl->markerPosition != NO_POSITION) && (impl->eventFlags
                & XA_RECORDEVENT_HEADATMARKER))
            {
            if ((impl->lastPosition < impl->markerPosition) && (newpos
                    >= impl->markerPosition) && impl->callback)
                {
                impl->callback(impl->cbPtrToSelf, impl->cbcontext,
                        XA_RECORDEVENT_HEADATMARKER);
                }
            }
        /* check if update period passed and callback needed */
        if ((impl->positionUpdatePeriod > 0) && (impl->eventFlags
                & XA_RECORDEVENT_HEADATNEWPOS) && impl->callback)
            {
            if ((XAuint32) ((impl->lastPosition)
                    / (impl->positionUpdatePeriod)) < (XAuint32) (newpos
                    / (impl->positionUpdatePeriod)))
                {
                impl->callback(impl->cbPtrToSelf, impl->cbcontext,
                        XA_RECORDEVENT_HEADATNEWPOS);
                }
            }
        /* store position */
        impl->lastPosition = newpos;

        /* Check have we reached record duration limit */
        if (impl->durationLimitSetted)
            {
            if (impl->callback && (impl->lastPosition >= impl->durationLimit))
                {
                impl->itf.SetRecordState(impl->cbPtrToSelf,
                        XA_RECORDSTATE_STOPPED);
                impl->callback(impl->cbPtrToSelf, impl->cbcontext,
                        XA_RECORDEVENT_HEADATLIMIT);
                }
            }
        }
    else if (event->eventid == XA_RECORDEVENT_HEADSTALLED)
        {
        impl->recordState = XA_RECORDSTATE_PAUSED;
        /* send callback if needed */
        if ((XA_RECORDEVENT_HEADSTALLED & impl->eventFlags) && impl->callback)
            {
            impl->callback(impl->cbPtrToSelf, impl->cbcontext,
                    XA_RECORDEVENT_HEADSTALLED);
            }
        }
    else if (event->eventid == XA_RECORDEVENT_BUFFER_FULL)
        {
        /* Adaptation is set to pause, need to sync state with AL-layer*/
        impl->recordState = XA_RECORDSTATE_STOPPED;
        /* send callback if needed */
        if ((XA_RECORDEVENT_BUFFER_FULL & impl->eventFlags) && impl->callback)
            {
            impl->callback(impl->cbPtrToSelf, impl->cbcontext,
                    XA_RECORDEVENT_BUFFER_FULL);
            }
        }
    else if (event->eventid == XA_RECORDEVENT_HEADMOVING)
        {
        /* send callback if needed */
        if ((XA_RECORDEVENT_HEADMOVING & impl->eventFlags) && impl->callback)
            {
            impl->callback(impl->cbPtrToSelf, impl->cbcontext,
                    XA_RECORDEVENT_HEADMOVING);
            }
        }
    else
        {
        /* do nothing */
        }

    DEBUG_API("<-XARecordItfImpl_AdaptCb");
    }

