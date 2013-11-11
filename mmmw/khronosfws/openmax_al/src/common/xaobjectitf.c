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
 * Description: Object Interface Implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "xaobjectitf.h"

/**
 * Base interface XAObjectItf implementation
 */
XAresult XAObjectItfImpl_Realize(XAObjectItf self, XAboolean async)
    {
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    XAresult retval = XA_RESULT_SUCCESS;

    DEBUG_API("->XAObjectItfImpl_Realize");
    if (!pObjImp || pObjImp != pObjImp->self)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAObjectItfImpl_Realize");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (async)
        {
        /* invoke asynchronously */
        if (!pObjImp->callBack)
            {
            retval = XA_RESULT_PARAMETER_INVALID;
            }
        else
            {
            /* check if another async operation is processed */
            if (pObjImp->asyncOngoing)
                {
                /* abort ongoing operation */
                XAImpl_CancelThread(pObjImp->asyncThr);
                }
            pObjImp->asyncOngoing = XA_BOOLEAN_TRUE;
            if (XAImpl_StartThread(pObjImp->asyncThr, NULL,
                    XAObjectItfImpl_AsyncRealize, (void*) (self)))
                {
                pObjImp->asyncOngoing = XA_BOOLEAN_FALSE;
                retval = XA_RESULT_RESOURCE_ERROR;
                }
            else
                {
                retval = XA_RESULT_SUCCESS;
                }
            }
        }
    else
        {
        /* Check if the object is in unrealized state */
        if (pObjImp->state != XA_OBJECT_STATE_UNREALIZED)
            {
            DEBUG_ERR("XA_RESULT_PRECONDITIONS_VIOLATED");
            DEBUG_API("<-XAObjectItfImpl_Realize");
            return XA_RESULT_PRECONDITIONS_VIOLATED;
            }
        /* invoke synchronously */
        if (pObjImp->DoRealizeImpl)
            {
            retval = pObjImp->DoRealizeImpl(self);
            }
        }
    DEBUG_API("<-XAObjectItfImpl_Realize");
    return retval;
    }

XAresult XAObjectItfImpl_Resume(XAObjectItf self, XAboolean async)
    {
    XAresult retval = XA_RESULT_SUCCESS;
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    DEBUG_API("->XAObjectItfImpl_Resume");

    if (!pObjImp || pObjImp != pObjImp->self)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAObjectItfImpl_Resume");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }
    if (async)
        {
        /* invoke asynchronously */
        if (!pObjImp->callBack)
            {
            retval = XA_RESULT_PARAMETER_INVALID;
            }
        else
            {
            /* check if another async operation is processed */
            if (pObjImp->asyncOngoing)
                {
                /* abort ongoing operation */
                XAImpl_CancelThread(pObjImp->asyncThr);
                }
            pObjImp->asyncOngoing = XA_BOOLEAN_TRUE;
            if (XAImpl_StartThread(pObjImp->asyncThr, NULL,
                    XAObjectItfImpl_AsyncResume, (void*) (self)))
                {
                pObjImp->asyncOngoing = XA_BOOLEAN_FALSE;
                retval = XA_RESULT_RESOURCE_ERROR;
                }
            else
                {
                retval = XA_RESULT_SUCCESS;
                }
            }
        }
    else
        {
        /* invoke synchronously */
        if (pObjImp->DoResumeImpl)
            {
            retval = pObjImp->DoResumeImpl(self);
            }
        }
    DEBUG_API("<-XAObjectItfImpl_Resume");
    return retval;
    }

XAresult XAObjectItfImpl_GetState(XAObjectItf self, XAuint32 *pState)
    {
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    DEBUG_API("->XAObjectItfImpl_GetState");
    if (!pObjImp || pObjImp != pObjImp->self || !pState)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAObjectItfImpl_GetState");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }
    *pState = pObjImp->state;

    DEBUG_API("<-XAObjectItfImpl_GetState");
    return XA_RESULT_SUCCESS;
    }

XAresult XAObjectItfImpl_GetInterface(XAObjectItf self,
        const XAInterfaceID iid, void *pInterface)
    {
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    XAObjItfMapEntry *mapEntry = NULL;
    DEBUG_API("->XAObjectItfImpl_GetInterface");

    if (!pObjImp || pObjImp != pObjImp->self || !pInterface || !iid)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAObjectItfImpl_GetInterface");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (pObjImp->state != XA_OBJECT_STATE_REALIZED)
        {
        DEBUG_ERR("XA_RESULT_PRECONDITIONS_VIOLATED");
        DEBUG_API("<-XAObjectItfImpl_GetInterface");
        /*state is not correct */
        return XA_RESULT_PRECONDITIONS_VIOLATED;
        }

    mapEntry = XAObjectItfImpl_GetItfEntry(self, iid);
    if (mapEntry && mapEntry->pItf)
        {
        *(void**) pInterface = &(mapEntry->pItf);
        DEBUG_API("<-XAObjectItfImpl_GetInterface");
        return XA_RESULT_SUCCESS;
        }
    else
        {
        DEBUG_ERR("XA_RESULT_FEATURE_UNSUPPORTED");
        DEBUG_API("<-XAObjectItfImpl_GetInterface");
        return XA_RESULT_FEATURE_UNSUPPORTED;
        }
    }

XAresult XAObjectItfImpl_RegisterCallback(XAObjectItf self,
        xaObjectCallback callback, void *pContext)
    {
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    DEBUG_API("->XAObjectItfImpl_RegisterCallback");

    if (!pObjImp || pObjImp != pObjImp->self)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAObjectItfImpl_RegisterCallback");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }
    pObjImp->callBack = callback;
    pObjImp->context = pContext;
    pObjImp->cbPtrToSelf = self;

    DEBUG_API("<-XAObjectItfImpl_RegisterCallback");
    return XA_RESULT_SUCCESS;
    }

void XAObjectItfImpl_AbortAsyncOperation(XAObjectItf self)
    {
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    DEBUG_API("->XAObjectItfImpl_AbortAsyncOperation");

    if (pObjImp && (pObjImp == pObjImp->self) && pObjImp->callBack
            && pObjImp->asyncOngoing)
        {
        /* abort ongoing operation */
        XAImpl_CancelThread(pObjImp->asyncThr);
        }

    DEBUG_API("<-XAObjectItfImpl_AbortAsyncOperation");
    }

void XAObjectItfImpl_Destroy(XAObjectItf self)
    {
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    DEBUG_API("->XAObjectItfImpl_Destroy");

    if (pObjImp && pObjImp == pObjImp->self)
        {
        if (pObjImp->asyncOngoing)
            {
            /* abort ongoing operation */
            XAImpl_CancelThread(pObjImp->asyncThr);
            }
        /* free object specific resources */
        if (pObjImp->FreeResourcesImpl)
            {
            pObjImp->FreeResourcesImpl(self);
            }
        free(pObjImp->interfaceMap);
        XAImpl_DeleteThreadHandle(pObjImp->asyncThr);
        free(pObjImp);
        }

    DEBUG_API("<-XAObjectItfImpl_Destroy");
    }

XAresult XAObjectItfImpl_SetPriority(XAObjectItf self, XAint32 priority,
        XAboolean preemptable)
    {
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    DEBUG_API("->XAObjectItfImpl_SetPriority");

    if (!pObjImp || pObjImp != pObjImp->self)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAObjectItfImpl_SetPriority");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }
    pObjImp->priority = priority;
    pObjImp->preemptable = preemptable;

    DEBUG_API("<-XAObjectItfImpl_SetPriority");
    return XA_RESULT_SUCCESS;
    }

XAresult XAObjectItfImpl_GetPriority(XAObjectItf self, XAint32 *pPriority,
        XAboolean *pPreemptable)
    {
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    DEBUG_API("->XAObjectItfImpl_GetPriority");

    if (!pObjImp || pObjImp != pObjImp->self)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAObjectItfImpl_GetPriority");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }
    *pPriority = pObjImp->priority;
    *pPreemptable = pObjImp->preemptable;

    DEBUG_API("<-XAObjectItfImpl_GetPriority");
    return XA_RESULT_SUCCESS;
    }

XAresult XAObjectItfImpl_SetLossOfControlInterfaces(XAObjectItf self,
        XAint16 numInterfaces, XAInterfaceID *pInterfaceIDs,
        XAboolean enabled)
    {
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    XAint32 itfIndex = 0;
    XAObjItfMapEntry *mapEntry = NULL;
    DEBUG_API("->XAObjectItfImpl_SetLossOfControlInterfaces");

    if (!pObjImp || pObjImp != pObjImp->self || !pInterfaceIDs)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAObjectItfImpl_SetLossOfControlInterfaces");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }
    if (pInterfaceIDs)
        {
        for (itfIndex = 0; itfIndex < numInterfaces; itfIndex++)
            {
            mapEntry = XAObjectItfImpl_GetItfEntry(self,
                    pInterfaceIDs[itfIndex]);
            if (mapEntry)
                {
                mapEntry->locEnabled = enabled;
                }
            }
        }

    DEBUG_API("<-XAObjectItfImpl_SetLossOfControlInterfaces");
    return XA_RESULT_SUCCESS;
    }

/**
 * XAObjectItfImpl -specific methods
 **/

/*
 * Allocate and init new object itf instance
 */
XAresult XAObjectItfImpl_Init(XAObjectItfImpl* self, XAuint32 itfCount,
        const XAInterfaceID** itfIIDs, xaDoRealizeImpl doRealizeImpl,
        xaDoResumeImpl doResumeImpl, xaFreeResourcesImpl freeResourcesImpl)
    {
    XAuint32 i = 0;
    DEBUG_API("->XAObjectItfImpl_Init");

    self->interfaceMap = (XAObjItfMapEntry*) calloc(itfCount,
            sizeof(XAObjItfMapEntry));
    if (!self->interfaceMap)
        {
        free(self);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAObjectItfImpl_Init");
        return XA_RESULT_MEMORY_FAILURE;
        }
    if (XAImpl_CreateThreadHandle(&(self->asyncThr)) != XA_RESULT_SUCCESS)
        {
        free(self);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAObjectItfImpl_Init");
        return XA_RESULT_MEMORY_FAILURE;
        }
    for (i = 0; i < itfCount; i++)
        {
        self->interfaceMap[i].mapIdx = i;
        self->interfaceMap[i].iid = *(itfIIDs[i]);
        self->interfaceMap[i].pItf = NULL;
        self->interfaceMap[i].locEnabled = XA_BOOLEAN_FALSE;
        self->interfaceMap[i].required = XA_BOOLEAN_FALSE;
        self->interfaceMap[i].isDynamic = XA_BOOLEAN_FALSE;
        }
    self->interfaceMap[0].pItf = self;
    self->interfaceMap[0].required = XA_BOOLEAN_TRUE;

    self->interfaceCount = itfCount;
    self->priority = XA_PRIORITY_NORMAL;
    self->preemptable = XA_BOOLEAN_FALSE;
    self->state = XA_OBJECT_STATE_UNREALIZED;
    self->callBack = NULL;
    self->context = NULL;
    self->asyncOngoing = XA_BOOLEAN_FALSE;
    self->cbPtrToSelf = NULL;

    self->DoRealizeImpl = doRealizeImpl;
    self->DoResumeImpl = doResumeImpl;
    self->FreeResourcesImpl = freeResourcesImpl;

    /* init itf default implementation */
    self->itf.Realize = XAObjectItfImpl_Realize;
    self->itf.Resume = XAObjectItfImpl_Resume;
    self->itf.GetState = XAObjectItfImpl_GetState;
    self->itf.GetInterface = XAObjectItfImpl_GetInterface;
    self->itf.RegisterCallback = XAObjectItfImpl_RegisterCallback;
    self->itf.AbortAsyncOperation = XAObjectItfImpl_AbortAsyncOperation;
    self->itf.Destroy = XAObjectItfImpl_Destroy;
    self->itf.SetPriority = XAObjectItfImpl_SetPriority;
    self->itf.GetPriority = XAObjectItfImpl_GetPriority;
    self->itf.SetLossOfControlInterfaces
            = XAObjectItfImpl_SetLossOfControlInterfaces;
    self->self = self;

    DEBUG_API("<-XAObjectItfImpl_Init");
    return XA_RESULT_SUCCESS;
    }

/*
 * Asynchronous invocation of Realize
 */
void* XAObjectItfImpl_AsyncRealize(void* args)
    {
    XAresult retval = XA_RESULT_SUCCESS;
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*(XAObjectItf) (args));
    DEBUG_API("->XAObjectItfImpl_AsyncRealize");

    if (pObjImp && pObjImp == pObjImp->self)
        {
        /* Check if the object is in unrealized state */
        if (pObjImp->state != XA_OBJECT_STATE_UNREALIZED)
            {
            retval = XA_RESULT_PRECONDITIONS_VIOLATED;
            }
        else if (pObjImp->DoRealizeImpl)
            {
            retval = pObjImp->DoRealizeImpl((XAObjectItf) (args));
            }
        if (pObjImp->callBack)
            {
            pObjImp->callBack(pObjImp->cbPtrToSelf, pObjImp->context,
                    XA_OBJECT_EVENT_ASYNC_TERMINATION, retval,
                    pObjImp->state, NULL);
            }
        pObjImp->asyncOngoing = XA_BOOLEAN_FALSE;
        XAImpl_ExitThread(pObjImp->asyncThr);
        }
    else
        {
        DEBUG_API("XAObjectItfImpl_AsyncRealize: INVALID args");
        }

    DEBUG_API("<-XAObjectItfImpl_AsyncRealize");
    return NULL;
    }

/*
 * Asynchronous invocation of Resume
 */
void* XAObjectItfImpl_AsyncResume(void* args)
    {
    XAresult retval = XA_RESULT_SUCCESS;
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*(XAObjectItf) (args));
    DEBUG_API("->XAObjectItfImpl_AsyncResume");

    if (pObjImp && pObjImp == pObjImp->self)
        {
        if (pObjImp->DoResumeImpl)
            {
            retval = pObjImp->DoResumeImpl((XAObjectItf) (args));
            }
        if (pObjImp->callBack)
            {
            pObjImp->callBack(pObjImp->cbPtrToSelf, pObjImp->context,
                    XA_OBJECT_EVENT_ASYNC_TERMINATION, retval,
                    pObjImp->state, NULL);
            }
        pObjImp->asyncOngoing = XA_BOOLEAN_FALSE;
        XAImpl_ExitThread(pObjImp->asyncThr);
        }
    else
        {
        DEBUG_API("XAObjectItfImpl_AsyncResume: INVALID args");
        }

    DEBUG_API("<-XAObjectItfImpl_AsyncResume");
    return NULL;
    }

XAObjItfMapEntry* XAObjectItfImpl_GetItfEntry(const XAObjectItf self,
        const XAInterfaceID iid)
    {
    XAuint32 mapIndex = 0;
    XAObjectItfImpl* pObjImp = (XAObjectItfImpl*) (*self);
    DEBUG_API("->XAObjectItfImpl_GetItfEntry");

    if (pObjImp && (pObjImp == pObjImp->self) && pObjImp->interfaceMap && iid)
        {
        /* Check interface */
        for (mapIndex = 0; mapIndex < pObjImp->interfaceCount; mapIndex++)
            {
            if (XACommon_EqualIIds(pObjImp->interfaceMap[mapIndex].iid, iid))
                {
                DEBUG_API("<-XAObjectItfImpl_GetItfEntry");
                return &(pObjImp->interfaceMap[mapIndex]);
                }
            }
        }

    DEBUG_API("<-XAObjectItfImpl_GetItfEntry");
    return NULL;
    }

