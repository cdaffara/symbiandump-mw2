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
 * Description: Stream Information Interface Implementation
 *
 */

#include "xastreaminformationitf.h"
#include "xastreaminformationitfadaptationmmf.h"

/* XAStreamInformationItfImpl* GetImpl
 * Description: Validate interface pointer and cast it to implementation pointer.
 */
static XAStreamInformationItfImpl* GetImpl(XAStreamInformationItf self)
    {
    if (self)
        {
        XAStreamInformationItfImpl* impl =
                (XAStreamInformationItfImpl*) (*self);
        if (impl && impl == impl->self)
            {
            return impl;
            }
        }
    return NULL;
    }

/**
 * Base interface XAPlayItf implementation
 */

XAresult XAStreamInformationItfImpl_QueryMediaContainerInformation(
        XAStreamInformationItf self, XAMediaContainerInformation * info)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAStreamInformationItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAStreamInformationItfImpl_QueryMediaContainerInformation");
    if (!impl || !info || !impl->adapCtx)
        {
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("-<XAStreamInformationItfImpl_QueryMediaContainerInformation");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->adapCtx->fwtype == FWMgrFWMMF)
        {
        ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
        if (ret == XA_RESULT_PARAMETER_INVALID)
            {
            DEBUG_API("<-XAStreamInformationItfImpl_QueryMediaContainerInformation");
            return ret;
            }
        ret = XAStreamInformationItfAdaptMMF_QueryMediaContainerInformation(
                impl->adapCtx, &(info->containerType),
                &(info->mediaDuration), &(info->numStreams));
        XAAdaptationBase_ThreadExit(impl->adapCtx);
        }

    DEBUG_API("-<XAStreamInformationItfImpl_QueryMediaContainerInformation");
    return ret;
    }

XAresult XAStreamInformationItfImpl_QueryStreamType(
        XAStreamInformationItf self, XAuint32 streamIndex, XAuint32 *domain)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAStreamInformationItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAStreamInformationItfImpl_QueryStreamType");
    if (!impl || !domain || (streamIndex == 0) || !impl->adapCtx)
        {
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("-<XAStreamInformationItfImpl_QueryStreamType");
        return XA_RESULT_PARAMETER_INVALID;
        }
    if (impl->adapCtx->fwtype == FWMgrFWMMF)
        {
        ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
        if (ret == XA_RESULT_PARAMETER_INVALID)
            {
            DEBUG_API("<-XAStreamInformationItfImpl_QueryStreamType");
            return ret;
            }

        ret = XAStreamInformationItfAdaptMMF_QueryStreamType(impl->adapCtx,
                streamIndex, domain);

        XAAdaptationBase_ThreadExit(impl->adapCtx);
        }
    DEBUG_API("-<XAStreamInformationItfImpl_QueryStreamType");
    return ret;
    }

XAresult XAStreamInformationItfImpl_QueryStreamInformation(
        XAStreamInformationItf self, XAuint32 streamIndex, void * info)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAStreamInformationItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAStreamInformationItfImpl_QueryStreamInformation");
    if (!impl || !info || (streamIndex == 0) || !impl->adapCtx)
        {
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("-<XAStreamInformationItfImpl_QueryStreamInformation");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->adapCtx->fwtype == FWMgrFWMMF)
        {
        ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
        if (ret == XA_RESULT_PARAMETER_INVALID)
            {
            DEBUG_API("<-XAStreamInformationItfImpl_QueryStreamInformation");
            return ret;
            }

        ret = XAStreamInformationItfAdaptMMF_QueryStreamInformation(
                impl->adapCtx, streamIndex, info);

        XAAdaptationBase_ThreadExit(impl->adapCtx);
        }

    DEBUG_API("-<XAStreamInformationItfImpl_QueryStreamInformation");
    return ret;
    }

XAresult XAStreamInformationItfImpl_QueryStreamName(
        XAStreamInformationItf self, XAuint32 streamIndex,
        XAuint16 * pNameSize, XAchar * pName)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAStreamInformationItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAStreamInformationItfImpl_QueryStreamName");
    if (!impl || (streamIndex == 0) || !pNameSize || !impl->adapCtx)
        {
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("-<XAStreamInformationItfImpl_QueryStreamName");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->adapCtx->fwtype == FWMgrFWMMF)
        {
        ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
        if (ret == XA_RESULT_PARAMETER_INVALID)
            {
            DEBUG_API("<-XAStreamInformationItfImpl_QueryStreamName");
            return ret;
            }

        ret = XAStreamInformationItfAdaptMMF_QueryStreamName(impl->adapCtx,
                streamIndex, pNameSize, pName);

        XAAdaptationBase_ThreadExit(impl->adapCtx);
        }
    DEBUG_API("-<XAStreamInformationItfImpl_QueryStreamName");
    return ret;
    }

XAresult XAStreamInformationItfImpl_RegisterStreamChangeCallback(
        XAStreamInformationItf self, xaStreamEventChangeCallback callback,
        void * pContext)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAStreamInformationItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAStreamInformationItfImpl_RegisterStreamChangeCallback");
    if (!impl)
        {
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("-<XAStreamInformationItfImpl_RegisterStreamChangeCallback");
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* callback may be NULL (to remove callback) */
    impl->callback = callback;
    impl->cbcontext = pContext;
    impl->cbPtrToSelf = self;

	
    if (impl->adapCtx->fwtype == FWMgrFWMMF)
    {
    	ret = XAStreamInformationItfAdaptMMF_RegisterCallback(impl->adapCtx, callback);
    }

    DEBUG_API("-<XAStreamInformationItfImpl_RegisterStreamChangeCallback");
    return ret;
    }

XAresult XAStreamInformationItfImpl_QueryActiveStreams(
        XAStreamInformationItf self, XAuint32 *numStreams,
        XAboolean *activeStreams)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAStreamInformationItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAStreamInformationItfImpl_QueryActiveStreams");
    if (!impl || !numStreams || !impl->adapCtx)
        {
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("-<XAStreamInformationItfImpl_QueryActiveStreams");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->adapCtx->fwtype == FWMgrFWMMF)
        {
        ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
        if (ret == XA_RESULT_PARAMETER_INVALID)
            {
            DEBUG_API("<-XAStreamInformationItfImpl_QueryActiveStreams");
            return ret;
            }

        ret = XAStreamInformationItfAdaptMMF_QueryActiveStreams(
                impl->adapCtx, numStreams, activeStreams);

        XAAdaptationBase_ThreadExit(impl->adapCtx);
        }

    DEBUG_API("-<XAStreamInformationItfImpl_QueryActiveStreams");
    return ret;
    }

XAresult XAStreamInformationItfImpl_SetActiveStream(
        XAStreamInformationItf self, XAuint32 streamNum, XAboolean active,
        XAboolean commitNow)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAStreamInformationItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAStreamInformationItfImpl_SetActiveStream");
    if (!impl || !impl->adapCtx)
        {
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("-<XAStreamInformationItfImpl_SetActiveStream");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->adapCtx->fwtype == FWMgrFWMMF)
        {
        ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
        if (ret == XA_RESULT_PARAMETER_INVALID)
            {
            DEBUG_API("<-XAStreamInformationItfImpl_SetActiveStream");
            return ret;
            }

        ret = XAStreamInformationItfAdaptMMF_SetActiveStream(impl->adapCtx,
                streamNum, active, commitNow);

        XAAdaptationBase_ThreadExit(impl->adapCtx);
        }

    DEBUG_API("-<XAStreamInformationItfImpl_SetActiveStream");
    return ret;
    }

/**
 * XAStreamInformationItfImpl -specific methods
 **/

/**
 * XAStreamInformationItfImpl* XAStreamInformationItfImpl_Create()
 * Description: Allocate and initialize PlayItfImpl
 **/
XAStreamInformationItfImpl* XAStreamInformationItfImpl_Create(
        XAMediaPlayerImpl* impl)
    {
    XAStreamInformationItfImpl *self;

    DEBUG_API("->XAStreamInformationItfImpl_Create");
    self = (XAStreamInformationItfImpl*) calloc(1,
            sizeof(XAStreamInformationItfImpl));
    if (self)
        {
        /* init itf default implementation */
        self->itf.QueryMediaContainerInformation
                = XAStreamInformationItfImpl_QueryMediaContainerInformation;
        self->itf.QueryStreamType
                = XAStreamInformationItfImpl_QueryStreamType;
        self->itf.QueryStreamInformation
                = XAStreamInformationItfImpl_QueryStreamInformation;
        self->itf.QueryStreamName
                = XAStreamInformationItfImpl_QueryStreamName;
        self->itf.RegisterStreamChangeCallback
                = XAStreamInformationItfImpl_RegisterStreamChangeCallback;
        self->itf.QueryActiveStreams
                = XAStreamInformationItfImpl_QueryActiveStreams;
        self->itf.SetActiveStream
                = XAStreamInformationItfImpl_SetActiveStream;

        /* init variables */
        self->cbPtrToSelf = NULL;
        self->callback = NULL;
        self->cbcontext = NULL;
        self->adapCtx = impl->curAdaptCtx;

        self->self = self;
		
        XAAdaptationBase_AddEventHandler( 	self->adapCtx, 
											&XAStreamInformationItfImpl_AdaptCb, XA_STREAMINFOEVENTS, self );
        }

    DEBUG_API("<-XAStreamInformationItfImpl_Create");
    return self;
    }

/* void XAStreamInformationItfImpl_Free(XAStreamInformationItfImpl* self)
 * Description: Free all resources reserved at XAStreamInformationItfImpl_Create
 */
void XAStreamInformationItfImpl_Free(XAStreamInformationItfImpl* self)
    {
    DEBUG_API("->XAStreamInformationItfImpl_Free");
    if(self)
    {
        XAAdaptationBase_RemoveEventHandler( (XAAdaptationBaseCtx*)self->adapCtx, &XAStreamInformationItfImpl_AdaptCb );
        free(self);
    }
    DEBUG_API("<-XAStreamInformationItfImpl_Free");
    }

void XAStreamInformationItfImpl_AdaptCb( void *pHandlerCtx, XAAdaptEvent *event )
{
	XAStreamInformationItfImpl* impl = (XAStreamInformationItfImpl*)pHandlerCtx;
	DEBUG_API("->XAPrefetchStatusItfImpl_AdaptCb");
	if(!impl)
	{
		DEBUG_ERR("XAPrefetchStatusItfImpl_AdaptCb, invalid context pointer!");
		DEBUG_API("<-XAPrefetchStatusItfImpl_AdaptCb");
		return;
	}
	
	if (impl->adapCtx->fwtype == FWMgrFWMMF)
	{
		impl->callback(impl->cbPtrToSelf, XA_STREAMCBEVENT_PROPERTYCHANGE, event->eventid, NULL, impl->cbcontext);
	}
	
	DEBUG_API("<-XAPrefetchStatusItfImpl_AdaptCb");
	return;
}

