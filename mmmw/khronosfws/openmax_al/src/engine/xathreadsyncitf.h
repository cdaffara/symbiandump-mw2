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
 * Description: ThreadSync Itf Header
 *
 */

#ifndef XATHREADSYNCITF_H
#define XATHREADSYNCITF_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaplatform.h"

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XA%ExampleItf% implementation */
typedef struct XAThreadSyncItfImpl_
    {
    /* parent interface */
    struct XAThreadSyncItf_ itf;
    /* pointer to self */
    struct XAThreadSyncItfImpl_* self;

    /* variables */
    XAImplMutexHandle engCriticalSection;
    XAboolean engInCritical;
    } XAThreadSyncItfImpl;

/** METHODS **/

/* Base interface XAThreadSyncItf implementation */
XAresult XAThreadSyncItfImpl_EnterCriticalSection(XAThreadSyncItf self);
XAresult XAThreadSyncItfImpl_ExitCriticalSection(XAThreadSyncItf self);

/* XAThreadSyncItfImpl -specific methods */
XAThreadSyncItfImpl* XAThreadSyncItfImpl_Create(void);
void XAThreadSyncItfImpl_Free(XAThreadSyncItfImpl* self);

#endif /* XATHREADSYNCITF_H */
