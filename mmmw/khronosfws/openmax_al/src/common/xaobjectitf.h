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

#ifndef XAOBJECTITF_H
#define XAOBJECTITF_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaplatform.h"

/** MACROS **/

/** TYPES **/
/* object-specific function pointers */
typedef XAresult (*xaDoRealizeImpl)(XAObjectItf self);
typedef XAresult (*xaDoResumeImpl)(XAObjectItf self);
typedef void (*xaFreeResourcesImpl)(XAObjectItf self);

/** ENUMERATIONS **/

/** STRUCTURES **/

typedef struct XAObjItfCBData_
    {
    XAuint32 event;
    XAresult result;
    XAuint32 param;
    void* interface;
    }XAObjItfCBData;

typedef struct XAObjItfMapEntry_
    {
    XAInterfaceID iid;
    XAuint8 mapIdx;
    void *pItf;
    XAboolean locEnabled;
    XAboolean required;
    XAboolean isDynamic;
    } XAObjItfMapEntry;

/* Definition of XAObjectItf implementation */
typedef struct XAObjectItfImpl_
    {
    /* parent interface */
    struct XAObjectItf_ itf;
    /* pointer to self */
    struct XAObjectItfImpl_* self;

    /* variables */
    XAint32 priority;
    XAboolean preemptable;
    XAuint32 state;

    /* array of available interfaces */
    XAObjItfMapEntry *interfaceMap;
    XAuint32 interfaceCount;

    /* async related variables */
    XAImplThreadHandle asyncThr;
    XAboolean asyncOngoing;
    xaObjectCallback callBack;
    void *context;
    XAObjectItf cbPtrToSelf;

    /* object-specific function implementation pointers */
    /* (must be implemented by every object) */
    xaDoRealizeImpl DoRealizeImpl;
    xaDoResumeImpl DoResumeImpl;
    xaFreeResourcesImpl FreeResourcesImpl;
    } XAObjectItfImpl;

/** METHODS **/

/* Base interface XAObjectItf implementation */
XAresult XAObjectItfImpl_Realize(XAObjectItf self, XAboolean async);

XAresult XAObjectItfImpl_Resume(XAObjectItf self, XAboolean async);

XAresult XAObjectItfImpl_GetState(XAObjectItf self, XAuint32 *pState);

XAresult XAObjectItfImpl_GetInterface(XAObjectItf self,
        const XAInterfaceID iid, void *pInterface);

XAresult XAObjectItfImpl_RegisterCallback(XAObjectItf self,
        xaObjectCallback callback, void *pContext);

void XAObjectItfImpl_AbortAsyncOperation(XAObjectItf self);

void XAObjectItfImpl_Destroy(XAObjectItf self);

XAresult XAObjectItfImpl_SetPriority(XAObjectItf self, XAint32 priority,
        XAboolean preemptable);

XAresult XAObjectItfImpl_GetPriority(XAObjectItf self, XAint32 *pPriority,
        XAboolean *pPreemptable);

XAresult XAObjectItfImpl_SetLossOfControlInterfaces(XAObjectItf self,
        XAint16 numInterfaces, XAInterfaceID *pInterfaceIDs,
        XAboolean enabled);

/* XAObjectItfImpl -specific methods */

/* Allocate and initialize base object */
XAresult XAObjectItfImpl_Init(XAObjectItfImpl* self, XAuint32 itfCount,
        const XAInterfaceID** itfIIDs, xaDoRealizeImpl doRealizeImpl,
        xaDoResumeImpl doResumeImpl, xaFreeResourcesImpl freeResourcesImpl);

/* methods for asynchronous service */
void* XAObjectItfImpl_AsyncRealize(void* args);
void* XAObjectItfImpl_AsyncResume(void* args);

XAObjItfMapEntry* XAObjectItfImpl_GetItfEntry(const XAObjectItf self,
        const XAInterfaceID iid);

#endif /* XAOBJECTITF_H */
