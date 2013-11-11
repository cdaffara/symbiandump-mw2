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


#ifndef PROFILEUTILMACRO_H
#define PROFILEUTILMACRO_H

/*
// MACROS to be defined to use profiling
PROFILE_TIME
PROFILE_RAM_USAGE
PROFILE_HEAP_USAGE
PRINT_TO_CONSOLE_TIME_DIFF
PRINT_TO_CONSOLE_RAM_DIFF
PRINT_TO_CONSOLE_HEAP_DIFF
*/

#if defined(PROFILE_TIME) || defined(PROFILE_RAM_USAGE) || defined(PROFILE_HEAP_USAGE)
#include <e32debug.h>
#include <hal.h>
#endif

#ifdef PROFILE_TIME
#define TAG_TIME_PROFILING_BEGIN \
    TTime beginProfilingTime; \
    beginProfilingTime.HomeTime()

#define TAG_TIME_PROFILING_END \
    TTime endProfilingTime; \
    endProfilingTime.HomeTime(); \
    TTimeIntervalMicroSeconds diffInMicroSecs = endProfilingTime.MicroSecondsFrom(beginProfilingTime)

#define PRINT_TO_CONSOLE_TIME_DIFF \
    RDebug::Printf("VPROFILEDAT: %s : Time taken[%u]microseconds ", __PRETTY_FUNCTION__, diffInMicroSecs.Int64())
#else /* Empty macros */
#define TAG_TIME_PROFILING_BEGIN
#define TAG_TIME_PROFILING_END
#define PRINT_TO_CONSOLE_TIME_DIFF
#endif /*PROFILE_TIME*/

#ifdef PROFILE_RAM_USAGE
#define TAG_RAM_PROFILING_BEGIN \
    TInt beginProfilingRAM; \
    TInt err1 = HAL::Get(HALData::EMemoryRAMFree, beginProfilingRAM)

#define TAG_RAM_PROFILING_END \
    TInt endProfilingRAM; \
    TInt err2 = HAL::Get(HALData::EMemoryRAMFree, endProfilingRAM)

#define PRINT_TO_CONSOLE_RAM_DIFF \
    if ((err1 == KErrNone) && (err2 == KErrNone)) \
        { \
        TInt diffRAM = (beginProfilingRAM - endProfilingRAM); \
        if ( diffRAM > 0 ) \
            { \
            RDebug::Printf("VPROFILEDAT: %s : RAM used[%u]bytes ", __PRETTY_FUNCTION__, diffRAM); \
            } \
        else \
            { \
            RDebug::Printf("VPROFILEDAT: %s : RAM freed[%u]bytes ", __PRETTY_FUNCTION__, (-diffRAM)); \
            } \
        } \
    else \
        { \
        RDebug::Printf("VPROFILEDAT: %s : Error1[%d] Error2[%d] ", __PRETTY_FUNCTION__, err1, err2); \
        }

#else /* Empty macros */
#define TAG_RAM_PROFILING_BEGIN
#define TAG_RAM_PROFILING_END
#define PRINT_TO_CONSOLE_RAM_DIFF
#endif /*PROFILE_RAM_USAGE*/

#ifdef PROFILE_HEAP_USAGE
#define TAG_DEFAULT_HEAP_PROFILING_BEGIN \
    TInt beginProfilingHEAPBiggestBlock; \
    TInt beginProfilingHEAP = User::Available(beginProfilingHEAPBiggestBlock) \

#define TAG_DEFAULT_HEAP_PROFILING_END \
    TInt endProfilingHEAPBiggestBlock; \
    TInt endProfilingHEAP = User::Available(endProfilingHEAPBiggestBlock) \

#define PRINT_TO_CONSOLE_HEAP_DIFF \
    TInt diffHEAP = beginProfilingHEAP - endProfilingHEAP; \
    if ( diffHEAP > 0 ) \
        { \
        RDebug::Printf("VPROFILEDAT: %s : HEAP used[%u]bytes ", __PRETTY_FUNCTION__, diffHEAP); \
        } \
    else \
        { \
        RDebug::Printf("VPROFILEDAT: %s : HEAP freed[%u]bytes ", __PRETTY_FUNCTION__, (-diffHEAP)); \
        }
#else /* Empty macros */
#define TAG_DEFAULT_HEAP_PROFILING_BEGIN
#define TAG_DEFAULT_HEAP_PROFILING_END
#define PRINT_TO_CONSOLE_HEAP_DIFF
#endif /*PROFILE_HEAP_USAGE*/

#endif /* PROFILEUTILMACRO_H */
