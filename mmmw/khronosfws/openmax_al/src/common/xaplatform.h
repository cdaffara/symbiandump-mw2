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
 * Description: Platform Specfic Header File
 *
 */

#ifndef XAPLATFORM_H
#define XAPLATFORM_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"

/** MACROS **/

/** TYPES **/

typedef void* XAImplMutexHandle;
typedef void* XAImplSemHandle;
typedef void* XAImplThreadHandle;
typedef void* (* XAImplThreadFunction)(void *xaThdFunAargs);

/** METHODS **/

/* mutex management */
XAresult XAImpl_CreateMutex(XAImplMutexHandle *mtx);
XAresult XAImpl_TryLockMutex(XAImplMutexHandle mtx);
XAresult XAImpl_LockMutex(XAImplMutexHandle mtx);
XAresult XAImpl_UnlockMutex(XAImplMutexHandle mtx);
void XAImpl_DeleteMutex(XAImplMutexHandle mtx);

/* semaphores */
XAresult XAImpl_CreateSemaphore(XAImplSemHandle *sem);
XAresult XAImpl_WaitSemaphore(XAImplSemHandle sem);
XAresult XAImpl_PostSemaphore(XAImplSemHandle sem);
void XAImpl_DeleteSemaphore(XAImplSemHandle sem);

/* thread management */

XAresult XAImpl_CreateThreadHandle(XAImplThreadHandle *thd);
XAresult XAImpl_StartThread(XAImplThreadHandle thd, void *thdattrib,
        XAImplThreadFunction thdfunc, void *thdfuncargs);
void XAImpl_CancelThread(XAImplThreadHandle thd);
void XAImpl_ExitThread(XAImplThreadHandle thd);
void XAImpl_DeleteThreadHandle(XAImplThreadHandle thd);

#endif /* XAPLATFORM_H */
