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
 * Description: Thread Safety Header
 *
 */

#ifndef XATHREADSAFETY_H_
#define XATHREADSAFETY_H_

#include "openmaxalwrapper.h"
#include "xaplatform.h"
#include "xadebug.h"

#ifdef _DEBUG
/*parse media object names for debug prints*/

static const char* mediaobjectnames[5] =
    {
            "XATSMediaPlayer",
            "XATSMediaRecorder",
            "XATSEngine",
            "XATSRadio",
            "XATSMetaDataExtractor"
    };
#define MEDIAOBJECTNAME(i) ((i<XATSCount)?mediaobjectnames[i]:"INVALID")
#endif /*_DEBUG*/

/* MACROS */
/*
 * XA_IMPL_THREAD_SAFETY_ENTRY( mediaObject )
 * Description: Tries to lock media object specified by parameter. If object is already locked
 * returns error value.
 * @param mediaObject - Index of media object where check is to be done
 * @return: XA_RESULT_PRECONDITIONS_VIOLATED - If shared data mutex is already in use
 * @return: XA_RESULT_INTERNAL_ERROR - If there is problem when reserving mutex
 * @return: In success cases doesn't return
 */
#define XA_IMPL_THREAD_SAFETY_ENTRY( mediaObject )                                      \
{                                                                                       \
    XAresult  mutexRet = XA_RESULT_SUCCESS;                                             \
    if ( mediaObject > XATSCount )                                                      \
    {                                                                                   \
        DEBUG_INFO("Thread safety - invalid mutex reference index");                    \
        return XA_RESULT_PARAMETER_INVALID;                                             \
    }                                                                                   \
    DEBUG_INFO_A1_STR("Thread safety entry for %s",MEDIAOBJECTNAME(mediaObject));       \
    mutexRet = XAThreadSafety_TryLock( mediaObject );                                   \
    if ( mutexRet == XA_RESULT_PERMISSION_DENIED )                                      \
    {                                                                                   \
        DEBUG_INFO("Media object already locked!");                                     \
        return XA_RESULT_PRECONDITIONS_VIOLATED;                                        \
    }                                                                                   \
    else if ( mutexRet != XA_RESULT_SUCCESS )                                           \
    {                                                                                   \
        DEBUG_INFO("Media object lock error!");                                         \
        return XA_RESULT_INTERNAL_ERROR;                                                \
    }                                                                                   \
    /* Fall through if XA_RESULT_SUCCESS */                                             \
}                                                                                       \

#define XA_IMPL_THREAD_SAFETY_ENTRY_FOR_VOID_FUNCTIONS( mediaObject )                   \
{                                                                                       \
    XAresult  mutexRet = XA_RESULT_SUCCESS;                                             \
    if ( mediaObject > XATSCount )                                                      \
    {                                                                                   \
        DEBUG_INFO("Thread safety - invalid mutex reference index");                    \
        return;                                                                         \
    }                                                                                   \
    DEBUG_INFO_A1_STR("Thread safety entry for %s",MEDIAOBJECTNAME(mediaObject));       \
    mutexRet = XAThreadSafety_TryLock( mediaObject );                                   \
    if ( mutexRet == XA_RESULT_PERMISSION_DENIED )                                      \
    {                                                                                   \
        DEBUG_INFO("Media object already locked!");                                     \
        return;                                                                         \
    }                                                                                   \
    else if ( mutexRet != XA_RESULT_SUCCESS )                                           \
    {                                                                                   \
        DEBUG_INFO("Media object lock error!");                                         \
        return;                                                                         \
    }                                                                                   \
    /* Fall through if XA_RESULT_SUCCESS */                                             \
}                                                                                       \
/*
 * XA_IMPL_THREAD_SAFETY_EXIT( mediaObject )
 * Description: Frees reserved lock for shared data related specified by parameter.
 * @param mediaObject - Index of media object where check is to be done
 * @return: XA_RESULT_PRECONDITIONS_VIOLATED if lock is not reserved
 * @return: XA_RESULT_INTERNAL_ERROR if freeing of lock cannot be done
 * @return: In success cases doesn't return anything
 */
#define XA_IMPL_THREAD_SAFETY_EXIT( mediaObject )                                       \
{                                                                                       \
    XAresult  mutexRet = XA_RESULT_SUCCESS;                                             \
    if ( mediaObject > XATSCount )                                                      \
    {                                                                                   \
        DEBUG_INFO("Thread safety - invalid mutex reference index");                    \
        return XA_RESULT_PARAMETER_INVALID;                                             \
    }                                                                                   \
    DEBUG_INFO_A1_STR("Thread safety exit for %s",MEDIAOBJECTNAME(mediaObject));        \
    mutexRet = XAThreadSafety_Unlock( mediaObject );                                    \
    if ( mutexRet != XA_RESULT_SUCCESS)                                                 \
    {                                                                                   \
        DEBUG_INFO("Unable to unlock media object!");                                   \
        return XA_RESULT_INTERNAL_ERROR;                                                \
    }                                                                                   \
}                                                                                       \

#define XA_IMPL_THREAD_SAFETY_EXIT_FOR_VOID_FUNCTIONS( mediaObject )                    \
{                                                                                       \
    XAresult  mutexRet = XA_RESULT_SUCCESS;                                             \
    if ( mediaObject > XATSCount )                                                      \
    {                                                                                   \
        DEBUG_INFO("Thread safety - invalid mutex reference index");                    \
        return;                                                                         \
    }                                                                                   \
    DEBUG_INFO_A1_STR("Thread safety exit for %s",MEDIAOBJECTNAME(mediaObject));        \
    mutexRet = XAThreadSafety_Unlock( mediaObject );                                    \
    if ( mutexRet != XA_RESULT_SUCCESS)                                                 \
    {                                                                                   \
        DEBUG_INFO("Unable to unlock media object!");                                   \
        return;                                                                         \
    }                                                                                   \
}                                                                                       \
/* ENUMERATIONS */

typedef enum
    {
    XATSMediaPlayer = 0,
    XATSMediaRecorder,
    XATSEngine,
    XATSRadio,
    XATSMetaDataExtractor,
    XATSCount
    } XAThreadSafetyMediaObjects;

/* FUNCTIONS */
XAresult XAThreadSafety_Init(XAboolean tsEnable);
XAresult XAThreadSafety_Destroy(void);
XAresult XAThreadSafety_Unlock(XAThreadSafetyMediaObjects mediaObject);
XAresult XAThreadSafety_TryLock(XAThreadSafetyMediaObjects mediaObject);

#endif /* XATHREADSAFETY_H_ */
