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
 * Description: ThreadSync Itf Implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "xathreadsyncitf.h"

/**
 * Base interface XAThreadSyncItf Implementation
 */
XAresult XAThreadSyncItfImpl_EnterCriticalSection(XAThreadSyncItf self)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAThreadSyncItfImpl* impl = NULL;

    DEBUG_API("->XAThreadSyncItfImpl_EnterCriticalSection");

    impl = (XAThreadSyncItfImpl*) (*self);
    if (!impl || impl != impl->self)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAThreadSyncItfImpl_EnterCriticalSection");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->engInCritical)
        {
        /* The calling context must not already be in
         critical section state. */
        DEBUG_ERR("XA_RESULT_PRECONDITIONS_VIOLATED");
        ret = XA_RESULT_PRECONDITIONS_VIOLATED;
        }
    else
        {
        ret = XAImpl_LockMutex(impl->engCriticalSection);
        if (ret == XA_RESULT_SUCCESS)
            {
            impl->engInCritical = XA_BOOLEAN_TRUE;
            }
        }
    DEBUG_API("<-XAThreadSyncItfImpl_EnterCriticalSection");
    return ret;
    }

XAresult XAThreadSyncItfImpl_ExitCriticalSection(XAThreadSyncItf self)
    {
    XAThreadSyncItfImpl* impl = NULL;
    XAresult ret = XA_RESULT_SUCCESS;

    impl = (XAThreadSyncItfImpl*) (*self);
    DEBUG_API("->XAThreadSyncItfImpl_ExitCriticalSection");

    if (!impl || impl != impl->self)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAThreadSyncItfImpl_ExitCriticalSection");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->engInCritical)
        {
        ret = XAImpl_UnlockMutex(impl->engCriticalSection);
        if (ret == XA_RESULT_SUCCESS)
            {
            impl->engInCritical = XA_BOOLEAN_FALSE;
            }
        }
    else
        {
        /* The engine must be in critical section state */
        DEBUG_ERR("XA_RESULT_PRECONDITIONS_VIOLATED");
        ret = XA_RESULT_PRECONDITIONS_VIOLATED;
        }
    DEBUG_API("<-XAThreadSyncItfImpl_ExitCriticalSection");
    return ret;
    }

/**
 * XAThreadSyncItfImpl -specific methods
 **/

XAThreadSyncItfImpl* XAThreadSyncItfImpl_Create()
    {
    XAThreadSyncItfImpl *self = NULL;
    DEBUG_API("->XAThreadSyncItfImpl_Create");

    self = (XAThreadSyncItfImpl*) calloc(1, sizeof(XAThreadSyncItfImpl));

    if (self)
        {
        /* init itf default implementation */
        self->itf.EnterCriticalSection
                = XAThreadSyncItfImpl_EnterCriticalSection;
        self->itf.ExitCriticalSection
                = XAThreadSyncItfImpl_ExitCriticalSection;

        if (XAImpl_CreateMutex(&(self->engCriticalSection))
                != XA_RESULT_SUCCESS)
            {
            DEBUG_ERR("Mutex creation failed, abort");
            free(self);
            DEBUG_API("<-XAThreadSyncItfImpl_Create");
            return NULL;
            }

        self->self = self;
        }
    DEBUG_API("<-XAThreadSyncItfImpl_Create");
    return self;
    }

void XAThreadSyncItfImpl_Free(XAThreadSyncItfImpl* self)
    {
    DEBUG_API("->XAThreadSyncItfImpl_Free");
    if(self)
        {
        XAImpl_DeleteMutex(self->engCriticalSection);
        free(self);
        }
    DEBUG_API("<-XAThreadSyncItfImpl_Free");
    }

