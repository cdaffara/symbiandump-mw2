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
 * Description: Dynamic Management Interface Implementation
 *
 */

#include <stdlib.h>
#include "openmaxalwrapper.h"
#include "xadynintmgmtitf.h"
#include "xadebug.h"

/* XADIMItfImpl* GetImpl(XADIMItf self)
 * Description: Validate interface pointer and cast it to implementation pointer.
 */
static XADIMItfImpl* GetImpl(XADynamicInterfaceManagementItf self)
    {
    if (self)
        {
        XADIMItfImpl* impl = (XADIMItfImpl*) (*self);
        if (impl && (impl == impl->self))
            {
            return impl;
            }
        }
    return NULL;
    }

/**
 * Base Interface XADynamicInterfaceManagementItf implementation
 **/

/* XAresult XADIMItfImpl_AddInterface
 * Optionally asynchronous method for exposing an interface on an object.
 */
XAresult XADIMItfImpl_AddInterface(XADynamicInterfaceManagementItf self,
        const XAInterfaceID iid, XAboolean async)
    {
    XADIMItfImpl* impl = GetImpl(self);
    XAresult retval = XA_RESULT_SUCCESS;
    DEBUG_API("->XADIMItfImpl_AddInterface");
    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XADIMItfImpl_AddInterface");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (async)
        {
        /* invoke asynchronously */
        /* check if another async operation is processed */
        if (impl->asyncOngoing)
            {
            /* abort ongoing operation */
            XAImpl_CancelThread(impl->asyncThr);
            }
        impl->asyncOngoing = XA_BOOLEAN_TRUE;
        impl->asyncID = iid;
        if (XAImpl_StartThread(impl->asyncThr, NULL,
                &XADIMItfImpl_AsyncAddItf, (void*) (self)))
            {
            impl->asyncOngoing = XA_BOOLEAN_FALSE;
            retval = XA_RESULT_RESOURCE_ERROR;
            }
        else
            {
            retval = XA_RESULT_SUCCESS;
            }
        }
    else
        {
        /* invoke synchronously */
        retval = XADIMItfImpl_DoAddItf(impl, iid);
        }

    DEBUG_API_A1("<-XADIMItfImpl_AddInterface %lu", retval);
    return retval;
    }

/* XAresult XADIMItfImpl_RemoveInterface
 * Synchronous method for removing a dynamically exposed interface on an object.
 */
XAresult XADIMItfImpl_RemoveInterface(XADynamicInterfaceManagementItf self,
        const XAInterfaceID iid)
    {
    XADIMItfImpl* impl = GetImpl(self);
    XAresult retval = XA_RESULT_SUCCESS;
    DEBUG_API("->XADIMItfImpl_RemoveInterface");

    if (impl)
        {
        XAObjItfMapEntry *mapEntry = NULL;
        if (impl->parent)
            {
            mapEntry = XAObjectItfImpl_GetItfEntry(impl->parent, iid);
            }
        if (mapEntry && mapEntry->isDynamic)
            {
            if ((mapEntry->pItf))
                {
                if (impl->DoRemoveItfImpl)
                    {
                    retval = impl->DoRemoveItfImpl(impl->parent, mapEntry);
                    }
                else
                    {
                    DEBUG_ERR("XADIMItfImpl_RemoveInterface: Object has specified dynamic itfs but not implemented DoRemoveItfImpl!!!");
                    retval = XA_RESULT_INTERNAL_ERROR;
                    }
                }
            else
                {
                DEBUG_ERR("XADIMItfImpl_RemoveInterface interface not exposed");
                retval = XA_RESULT_PRECONDITIONS_VIOLATED;
                }
            }
        else /*( mapEntry && mapEntry->isDynamic )*/
            { /* interface is not supported for dynamic management */
            DEBUG_ERR("XADIMItfImpl_RemoveInterface interface not supported");
            retval = XA_RESULT_FEATURE_UNSUPPORTED;
            }
        }
    else
        {
        retval = XA_RESULT_PARAMETER_INVALID;
        DEBUG_ERR("XADIMItfImpl_RemoveInterface: INVALID args");
        }

    DEBUG_API_A1("<-XADIMItfImpl_RemoveInterface %lu", retval);
    return retval;
    }

/* XAresult XADIMItfImpl_ResumeInterface
 * Optionally asynchronous method for resuming a dynamically exposed interface on the object.
 */
XAresult XADIMItfImpl_ResumeInterface(XADynamicInterfaceManagementItf self,
        const XAInterfaceID iid, XAboolean async)
    {
    XADIMItfImpl* impl = GetImpl(self);
    XAresult retval = XA_RESULT_SUCCESS;
    DEBUG_API("->XADIMItfImpl_ResumeInterface");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XADIMItfImpl_ResumeInterface");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (async)
        {
        /* invoke asynchronously */
        /* check if another async operation is processed */
        if (impl->asyncOngoing)
            {
            /* abort ongoing operation */
            XAImpl_CancelThread(impl->asyncThr);
            }
        impl->asyncOngoing = XA_BOOLEAN_TRUE;
        impl->asyncID = iid;
        if (XAImpl_StartThread(impl->asyncThr, NULL,
                &XADIMItfImpl_AsyncResumeItf, (void*) (self)))
            {
            impl->asyncOngoing = XA_BOOLEAN_FALSE;
            retval = XA_RESULT_RESOURCE_ERROR;
            }
        else
            {
            retval = XA_RESULT_SUCCESS;
            }
        }
    else
        {
        /* invoke synchronously */
        retval = XADIMItfImpl_DoResumeItf(impl, iid);
        }

    DEBUG_API_A1("<-XADIMItfImpl_ResumeInterface %lu", retval);
    return retval;
    }

/* XAresult XADIMItfImpl_RegisterCallback
 * Registers a callback on the object
 */
XAresult XADIMItfImpl_RegisterCallback(XADynamicInterfaceManagementItf self,
        xaDynamicInterfaceManagementCallback callback, void * pContext)
    {
    XADIMItfImpl* impl = GetImpl(self);
    DEBUG_API("->XADIMItfImpl_RegisterCallback");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XADIMItfImpl_RegisterCallback");
        return XA_RESULT_PARAMETER_INVALID;
        }
    impl->dimCb = callback;
    impl->dimCbContext = pContext;
    impl->dimCbPtrToSelf = self;

    DEBUG_API("<-XADIMItfImpl_RegisterCallback");
    return XA_RESULT_SUCCESS;
    }

/**
 * XADIMItfImpl -specific methods
 **/

/* XADIMItfImpl* XADIMItfImpl_Create()
 * Description: Allocate and initialize DIMItfImpl
 */
XADIMItfImpl* XADIMItfImpl_Create()
    {
    XADIMItfImpl *self = (XADIMItfImpl*) calloc(1, sizeof(XADIMItfImpl));
    DEBUG_API("->XADIMItfImpl_Create");

    if (self)
        {
        /* init itf default implementation */
        self->itf.AddInterface = XADIMItfImpl_AddInterface;
        self->itf.RemoveInterface = XADIMItfImpl_RemoveInterface;
        self->itf.ResumeInterface = XADIMItfImpl_ResumeInterface;
        self->itf.RegisterCallback = XADIMItfImpl_RegisterCallback;

        /* init variables */
        self->asyncOngoing = XA_BOOLEAN_FALSE;
        self->dimCbPtrToSelf = NULL;

        self->self = self;
        if (XAImpl_CreateThreadHandle(&(self->asyncThr)) != XA_RESULT_SUCCESS)
            {
            free(self);
            self = NULL;
            }
        }
    DEBUG_API("<-XADIMItfImpl_Create");
    return self;
    }

/* void XADIMItfImpl_Free(XADIMItfImpl* self)
 * Description: Free all resources reserved at XADIMItfImpl_Create
 */
void XADIMItfImpl_Free(XADIMItfImpl* self)
    {
    DEBUG_API("->XADIMItfImpl_Free");
    XAImpl_DeleteThreadHandle(self->asyncThr);
    free(self);
    DEBUG_API("<-XADIMItfImpl_Free");
    }

/* void XADIMItfImpl_Init
 * Description: Initialize DIM interface. Mandatory to call if dynamic
 * interfaces are supported by object.
 */
void XADIMItfImpl_Init(XADIMItfImpl* self, XAObjectItf parent,
        xaDoAddItfImpl doAdd, xaDoResumeItfImpl doResume,
        xaDoRemoveItfImpl doRemove)
    {
    DEBUG_API("->XADIMItfImpl_Init");
    self->parent = parent;
    self->DoAddItfImpl = doAdd;
    self->DoResumeItfImpl = doResume;
    self->DoRemoveItfImpl = doRemove;

    DEBUG_API("<-XADIMItfImpl_Init");
    }

/* void XADIMItfImpl_SendCbEvent
 * Description: Object can use this method to send resource events
 */
void XADIMItfImpl_SendCbEvent(XADIMItfImpl* self, XAuint32 event,
        XAresult result, const XAInterfaceID iid)
    {
    DEBUG_API("->XADIMItfImpl_SendCbEvent");
    if (self->dimCb)
        {
        self->dimCb((XADynamicInterfaceManagementItf) (self),
                self->dimCbContext, event, result, iid);
        }
    DEBUG_API("<-XADIMItfImpl_SendCbEvent");
    }

/* void* XADIMItfImpl_AsyncAddItf
 * Description: Launch AddItf from async thread.
 */
void* XADIMItfImpl_AsyncAddItf(void* args)
    {
    XAresult retval = XA_RESULT_SUCCESS;
    XADIMItfImpl* impl = GetImpl(args);
    DEBUG_API("->XADIMItfImpl_AsyncAddItf");

    if (impl)
        {
        retval = XADIMItfImpl_DoAddItf(impl, impl->asyncID);
        if (impl->dimCb)
            {
            impl->dimCb((XADynamicInterfaceManagementItf) (args),
                    impl->dimCbContext,
                    XA_DYNAMIC_ITF_EVENT_ASYNC_TERMINATION, retval,
                    impl->asyncID);
            }
        impl->asyncOngoing = XA_BOOLEAN_FALSE;
        XAImpl_ExitThread(impl->asyncThr);
        }
    else
        {
        DEBUG_ERR("XADIMItfImpl_AsyncAddItf: INVALID args");
        }

    DEBUG_API("<-XADIMItfImpl_AsyncAddItf");
    return NULL;
    }

/* XAresult XADIMItfImpl_DoAddItf
 * Description: Handle Itf dynamic addition and call object-specific parts.
 *              Launched either from async thread or directly.
 */
XAresult XADIMItfImpl_DoAddItf(XADIMItfImpl* impl, const XAInterfaceID iid)
    {
    XAresult retval = XA_RESULT_SUCCESS;
    DEBUG_API("->XADIMItfImpl_DoAddItf");

    if (impl)
        {
        XAObjItfMapEntry *mapEntry = NULL;
        if (impl->parent)
            {
            mapEntry = XAObjectItfImpl_GetItfEntry(impl->parent, iid);
            }
        if (mapEntry && mapEntry->isDynamic)
            {
            if (!(mapEntry->pItf))
                {
                if (impl->DoAddItfImpl)
                    {
                    retval = impl->DoAddItfImpl(impl->parent, mapEntry);
                    }
                else
                    {
                    DEBUG_ERR("XADIMItfImpl_DoAddItf: Object has specified dynamic itfs but not implemented DoAddItfImpl!!!");
                    retval = XA_RESULT_INTERNAL_ERROR;
                    }
                }
            else
                {
                DEBUG_ERR("XADIMItfImpl_DoAddItf interface already exposed");
                retval = XA_RESULT_PRECONDITIONS_VIOLATED;
                }
            }
        else /*( mapEntry && mapEntry->isDynamic )*/
            { /* interface is not supported for dynamic management */
            DEBUG_ERR("XADIMItfImpl_DoAddItf interface not supported");
            retval = XA_RESULT_FEATURE_UNSUPPORTED;
            }
        }
    else
        {
        retval = XA_RESULT_INTERNAL_ERROR;
        DEBUG_ERR("XADIMItfImpl_DoAddItf: INVALID args");
        }

    DEBUG_API("<-XADIMItfImpl_DoAddItf");
    return retval;
    }

/* void* XADIMItfImpl_AsyncResumeItf
 * Description: Launch ResumeItf from async thread.
 */
void* XADIMItfImpl_AsyncResumeItf(void* args)
    {
    XAresult retval = XA_RESULT_SUCCESS;
    XADIMItfImpl* impl = GetImpl(args);
    DEBUG_API("->XADIMItfImpl_AsyncResumeItf");

    if (impl)
        {
        retval = XADIMItfImpl_DoResumeItf(impl, impl->asyncID);
        if (impl->dimCb)
            {
            impl->dimCb((XADynamicInterfaceManagementItf) (args),
                    impl->dimCbContext,
                    XA_DYNAMIC_ITF_EVENT_ASYNC_TERMINATION, retval,
                    impl->asyncID);
            }
        impl->asyncOngoing = XA_BOOLEAN_FALSE;
        XAImpl_ExitThread(impl->asyncThr);
        }
    else
        {
        DEBUG_ERR("XADIMItfImpl_AsyncResumeItf: INVALID args");
        }

    DEBUG_API("<-XADIMItfImpl_AsyncResumeItf");
    return NULL;
    }

/* void XADIMItfImpl_DoResumeItf
 * Description: Handle Itf dynamic resuming and call object-specific parts.
 *              Launched either from async thread or directly.
 */
XAresult XADIMItfImpl_DoResumeItf(XADIMItfImpl* impl, const XAInterfaceID iid)
    {
    XAresult retval = XA_RESULT_SUCCESS;
    DEBUG_API("->XADIMItfImpl_DoResumeItf");

    if (impl)
        {
        XAObjItfMapEntry *mapEntry = XAObjectItfImpl_GetItfEntry(
                impl->parent, iid);
        if (mapEntry && mapEntry->isDynamic)
            {
            if ((!mapEntry->pItf))
                {
                if (impl->DoResumeItfImpl)
                    {
                    retval = impl->DoResumeItfImpl(impl->parent, mapEntry);
                    }
                else
                    {
                    DEBUG_ERR("XADIMItfImpl_DoResumeItf: Object has specified dynamic itfs but not implemented DoResumeItfImpl!!!");
                    retval = XA_RESULT_INTERNAL_ERROR;
                    }
                }
            else
                {
                DEBUG_ERR("XADIMItfImpl_DoResumeItf interface already exposed");
                retval = XA_RESULT_PRECONDITIONS_VIOLATED;
                }
            }
        else /*( mapEntry && mapEntry->isDynamic )*/
            { /* interface is not supported for dynamic management */
            DEBUG_ERR("XADIMItfImpl_DoResumeItf interface not supported");
            retval = XA_RESULT_FEATURE_UNSUPPORTED;
            }
        }
    else
        {
        retval = XA_RESULT_INTERNAL_ERROR;
        DEBUG_ERR("XADIMItfImpl_DoResumeItf: INVALID args");
        }

    DEBUG_API("<-XADIMItfImpl_DoResumeItf");
    return retval;
    }

