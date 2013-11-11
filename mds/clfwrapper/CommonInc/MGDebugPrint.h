/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Debug macros
*
*/


#ifndef __MGDEBUGPRINT_H__
#define __MGDEBUGPRINT_H__

#include "harvesterlog.h"

#ifdef _DEBUG
#define MG_ENABLE_DEBUG_PRINT
#endif

// Debug logging is enabled, you may enable debug printing in release builds also
#ifdef MG_ENABLE_DEBUG_PRINT

    // warn if this is a release build!!
    #ifndef __WINSCW__
    #ifndef _DEBUG
        #if defined(__VC32__)
            #pragma message( "Warning: MG debug printing is on in release build!" )     // CSI: 68 #  68:use of #pragma message
        #else // __GCC32__
            #warning "MG debug printing is on in release build!"
        #endif // __VC32__
    #endif
    #endif // __WINSCW__

// no include files if no debug printing --> faster compile time
// INCLUDES
#include <e32std.h>
#include <e32svr.h>

    /**
     * Helper macro for defining debug strings with just the debug text.
     */
    #define MG_DEBUG_STRING(name,s) _LIT(name,s)

    /**
     * Output to debugger output
     */
    #define MG_DEBUG_PRINT RDebug::Print

#else   // MG_ENABLE_DEBUG_PRINT

    /**
     * Dummy struct for checking that all MG_DEBUG_PRINT's define string
     * literals using space-saving MG_DEBUG_STRING.
     */
    struct TMGEmptyDebugString { };     // CSI: 80 #  80:use of struct

    /**
     * Macro for defining debug-only literal strings (empty release version)
     */
    #define MG_DEBUG_STRING(name, s) const TMGEmptyDebugString name

    /**
     * Output to debugger output (empty)
     */
    #define MG_DEBUG_PRINT MG_EmptyPrint

    /// Empty debug print function for release builds.
    inline void MG_EmptyPrint(TMGEmptyDebugString)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1>
    inline void MG_EmptyPrint(TMGEmptyDebugString,T1)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2>
    inline void MG_EmptyPrint(TMGEmptyDebugString,T1,T2)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3>
    inline void MG_EmptyPrint(TMGEmptyDebugString,T1,T2,T3)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3,class T4>
    inline void MG_EmptyPrint(TMGEmptyDebugString,T1,T2,T3,T4)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3,class T4,class T5>
    inline void MG_EmptyPrint(TMGEmptyDebugString,T1,T2,T3,T4,T5)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3,class T4,class T5,class T6>
    inline void MG_EmptyPrint(TMGEmptyDebugString,T1,T2,T3,T4,T5,T6)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3,class T4,class T5,class T6,class T7>
    inline void MG_EmptyPrint(TMGEmptyDebugString,T1,T2,T3,T4,T5,T6,T7)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
    inline void MG_EmptyPrint(TMGEmptyDebugString,T1,T2,T3,T4,T5,T6,T7,T8)
        {
        }


#endif  // MG_ENABLE_DEBUG_PRINT

/**
 * Short variant for easier basic usage.
 */
#define MG_DEBUG1( name, s ) MG_DEBUG_STRING( name, s ); MG_DEBUG_PRINT( name );

#ifdef MG_ENABLE_DEBUG_PRINT
#define MG_DEBUG2( name, s, i ) MG_DEBUG_STRING( name, s ); MG_DEBUG_PRINT( name, i );
#define MG_DEBUG3( name, s, i, j ) MG_DEBUG_STRING( name, s ); MG_DEBUG_PRINT( name, i, j );
#define MG_DEBUG4( name, s, i, j, k ) MG_DEBUG_STRING( name, s ); MG_DEBUG_PRINT( name, i, j, k );
#define MG_DEBUG5( name, s, i, j, k, l ) MG_DEBUG_STRING( name, s ); MG_DEBUG_PRINT( name, i, j, k, l );
#else
// If debug printing is not enabled, strip other parameters than the string
// away to prevent unwanted function calls in release build.
#define MG_DEBUG2( name, s, i ) MG_DEBUG1( name, s )
#define MG_DEBUG3( name, s, i, j ) MG_DEBUG1( name, s )
#define MG_DEBUG4( name, s, i, j, k ) MG_DEBUG1( name, s )
#define MG_DEBUG5( name, s, i, j, k, l ) MG_DEBUG1( name, s )
#endif

#endif //  __MGDEBUGPRINT_H__
