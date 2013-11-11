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
 * Description: Dynamic Mgmt Interface Header
 *
 */

#ifndef XADYNINTMGMTITF_H
#define XADYNINTMGMTITF_H

#include "openmaxalwrapper.h"
#include "xaobjectitf.h"
#include "xaplatform.h"

/** MACROS **/

/** TYPES **/
/* object implementation-specific DIM methods */
typedef XAresult (*xaDoAddItfImpl)(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);
typedef XAresult (*xaDoResumeItfImpl)(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);
typedef XAresult (*xaDoRemoveItfImpl)(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XADynamicInterfaceManagementItf implementation */
typedef struct XADIMItfImpl_
    {
    /* parent interface */
    struct XADynamicInterfaceManagementItf_ itf;
    /* pointer to self */
    struct XADIMItfImpl_* self;

    /* variables */
    /* pointer to parent object */
    XAObjectItf parent;
    /* pointers to implementation-specific methods */
    xaDoAddItfImpl DoAddItfImpl;
    xaDoResumeItfImpl DoResumeItfImpl;
    xaDoRemoveItfImpl DoRemoveItfImpl;

    /* client callback */
    XADynamicInterfaceManagementItf dimCbPtrToSelf;
    xaDynamicInterfaceManagementCallback dimCb;
    void* dimCbContext;
    XAImplThreadHandle asyncThr;
    XAboolean asyncOngoing;
    XAInterfaceID asyncID;

    } XADIMItfImpl;

/* Base interface XADynamicInterfaceManagementItf implementation */
XAresult XADIMItfImpl_AddInterface(XADynamicInterfaceManagementItf self,
        const XAInterfaceID iid, XAboolean async);

XAresult XADIMItfImpl_RemoveInterface(XADynamicInterfaceManagementItf self,
        const XAInterfaceID iid);

XAresult XADIMItfImpl_ResumeInterface(XADynamicInterfaceManagementItf self,
        const XAInterfaceID iid, XAboolean async);

XAresult XADIMItfImpl_RegisterCallback(XADynamicInterfaceManagementItf self,
        xaDynamicInterfaceManagementCallback callback, void * pContext);

/* XADIMItfImpl -specific methods */
XADIMItfImpl* XADIMItfImpl_Create(void);
void XADIMItfImpl_Free(XADIMItfImpl* self);

void XADIMItfImpl_Init(XADIMItfImpl* self, XAObjectItf parent,
        xaDoAddItfImpl doAdd, xaDoResumeItfImpl doResume,
        xaDoRemoveItfImpl doRemove);

void XADIMItfImpl_SendCbEvent(XADIMItfImpl* self, XAuint32 event,
        XAresult result, const XAInterfaceID iid);

/* methods for asynchronous service */
void* XADIMItfImpl_AsyncAddItf(void* args);
XAresult XADIMItfImpl_DoAddItf(XADIMItfImpl* impl, const XAInterfaceID iid);
void* XADIMItfImpl_AsyncResumeItf(void* args);
XAresult
        XADIMItfImpl_DoResumeItf(XADIMItfImpl* impl, const XAInterfaceID iid);

#endif /* XADYNINTMGMTITF_H */
