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
 * Description: Platform Specific Utility Functions
 *
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "xaplatform.h"

/** MACROS **/

/**********************************************************************
 * mutex management
 **********************************************************************/
/* define next if platform supports posix error checking mutex type */
#undef _MUTEXERRORSUPPORT

#ifdef _MUTEXERRORSUPPORT
typedef pthread_mutex_t XA_MTX;
#else
typedef struct
    {
    pthread_mutex_t mutex;
    pthread_t owner;
    } XA_MTX;
#endif

XAresult XAImpl_CreateMutex(XAImplMutexHandle *mtx)
    {
    XA_MTX *pMtx = (XA_MTX *) malloc(sizeof(XA_MTX));
    if (pMtx)
        {
        pthread_mutexattr_t *pAttr = NULL;
#ifdef _MUTEXERRORSUPPORT
        pthread_mutexattr_t attr;
        pAttr = &attr;
        pthread_mutexattr_init(pAttr);
        pthread_mutexattr_settype(pAttr, PTHREAD_MUTEX_ERRORCHECK);
        if(pthread_mutex_init(pMtx, pAttr))
#else
        if (pthread_mutex_init(&(pMtx->mutex), pAttr))
#endif
            {
            free(pMtx);
            *mtx = NULL;
            return XA_RESULT_INTERNAL_ERROR;
            }
        *mtx = (XAImplMutexHandle) pMtx;
        return XA_RESULT_SUCCESS;
        }
    else
        {
        return XA_RESULT_MEMORY_FAILURE;
        }
    }

XAresult XAImpl_LockMutex(XAImplMutexHandle mtx)
    {
    XA_MTX *pMtx = (XA_MTX*) mtx;
#ifdef _MUTEXERRORSUPPORT
    if(pthread_mutex_lock(pMtx))
        {
        return XA_RESULT_PRECONDITIONS_VIOLATED;
        }
#else
    if (pthread_self() == pMtx->owner || pthread_mutex_unlock(&(pMtx->mutex)))
        {
        return XA_RESULT_PRECONDITIONS_VIOLATED;
        }
    pMtx->owner = pthread_self();
#endif
    return XA_RESULT_SUCCESS;
    }

XAresult XAImpl_TryLockMutex(XAImplMutexHandle mtx)
    {
    XA_MTX *pMtx = (XA_MTX*) mtx;
    XAint32 mutexRet;
    XAresult ret = XA_RESULT_SUCCESS;

#ifdef _MUTEXERRORSUPPORT
    mutexRet = pthread_ mutex_trylock(pMtx);
    switch (mutexRet)
        {
        case EBUSY:
        /* if mutex is already locked, return permission denied */
        ret = XA_RESULT_PERMISSION_DENIED;
        break;
        case 0:
        ret = XA_RESULT_SUCCESS;
        break;
        default:
        ret = XA_RESULT_PRECONDITIONS_VIOLATED;
        break;
        }
#else
    if (pthread_self() == pMtx->owner)
        {
        return XA_RESULT_PRECONDITIONS_VIOLATED;
        }

    mutexRet = pthread_mutex_trylock(&(pMtx->mutex));
    switch (mutexRet)
        {
        case EBUSY:
            /* if mutex is already locked, return permission denied */
            ret = XA_RESULT_PERMISSION_DENIED;
            break;
        case 0:
            pMtx->owner = pthread_self();
            ret = XA_RESULT_SUCCESS;
            break;
        default:
            ret = XA_RESULT_PRECONDITIONS_VIOLATED;
            break;
        }
#endif
    return ret;
    }

XAresult XAImpl_UnlockMutex(XAImplMutexHandle mtx)
    {
    XA_MTX *pMtx = (XA_MTX*) mtx;
#ifdef _MUTEXERRORSUPPORT
    if(pthread_mutex_lock(pMtx))
        {
        return XA_RESULT_PRECONDITIONS_VIOLATED;
        }
#else
    if (pthread_self() != pMtx->owner || pthread_mutex_unlock(&(pMtx->mutex)))
        {
        return XA_RESULT_PRECONDITIONS_VIOLATED;
        }
    // Changing the below value to 0 since owner is an unsigned int.
    // pMtx->owner = -1;
    pMtx->owner = 0;
#endif
    return XA_RESULT_SUCCESS;
    }

void XAImpl_DeleteMutex(XAImplMutexHandle mtx)
    {
    XA_MTX *pMtx = (XA_MTX*) mtx;
    if (pMtx)
        {
#ifdef _MUTEXERRORSUPPORT
        pthread_mutex_destroy(pMtx);
#else
        pthread_mutex_destroy(&(pMtx->mutex));
#endif
        free(pMtx);
        }
    }

/**********************************************************************
 * semaphores
 **********************************************************************/

XAresult XAImpl_CreateSemaphore(XAImplSemHandle *sem)
    {
    sem_t *pSem = (sem_t*) malloc(sizeof(sem_t));
    if (pSem)
        {
        if (sem_init(pSem, 0, 0))
            {
            free(pSem);
            *sem = NULL;
            return XA_RESULT_INTERNAL_ERROR;
            }
        *sem = (XAImplSemHandle) pSem;
        return XA_RESULT_SUCCESS;
        }
    else
        {
        return XA_RESULT_MEMORY_FAILURE;
        }
    }

XAresult XAImpl_WaitSemaphore(XAImplSemHandle sem)
    {
    sem_t* pSem = (sem_t*) sem;
    sem_wait(pSem);
    return XA_RESULT_SUCCESS;
    }

XAresult XAImpl_PostSemaphore(XAImplSemHandle sem)
    {
    sem_t *pSem = (sem_t*) sem;
    sem_post(pSem);
    return XA_RESULT_SUCCESS;
    }

void XAImpl_DeleteSemaphore(XAImplSemHandle sem)
    {
    sem_t *pSem = (sem_t*) sem;
    if (pSem)
        {
        sem_destroy(pSem);
        free(pSem);
        }
    }

/**********************************************************************
 * THREADS
 **********************************************************************/

XAresult XAImpl_CreateThreadHandle(XAImplThreadHandle *thd)
    {
    pthread_t *pThd = (pthread_t*) malloc(sizeof(pthread_t));
    if (!pThd)
        {
        return XA_RESULT_MEMORY_FAILURE;
        }
    *thd = (XAImplThreadHandle) pThd;
    return XA_RESULT_SUCCESS;
    }

XAresult XAImpl_StartThread(XAImplThreadHandle thd, void *thdattrib,
        XAImplThreadFunction thdfunc, void* thdfuncargs)
    {
    pthread_t *pThd = (pthread_t*) thd;
    if (pthread_create(pThd, thdattrib, thdfunc, thdfuncargs))
        {
        return XA_RESULT_INTERNAL_ERROR;
        }
    return XA_RESULT_SUCCESS;
    }

void XAImpl_CancelThread(XAImplThreadHandle thd)
    {
    // int res;
    // TL: TODO: There is no pthread_cancel API in S60, need to replace
    //res = pthread_cancel(*pThd);

    }

void XAImpl_ExitThread(XAImplThreadHandle thd)
    {
    pthread_exit(NULL);
    }

void XAImpl_DeleteThreadHandle(XAImplThreadHandle thd)
    {
    pthread_t *pThd = (pthread_t*) thd;
    if (pThd)
        {
        free(pThd);
        }
    }

