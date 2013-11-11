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
* Description:  Trace macros
*
*/


#ifndef MGTRACEPRINT_H
#define MGTRACEPRINT_H

#include "harvesterlog.h"

#ifdef _DEBUG
#define MG_ENABLE_TRACE_PRINT
#endif

// Debug logging is enabled, you may enable debug printing in release builds also
#ifdef MG_ENABLE_TRACE_PRINT

    // warn if this is a release build!!
    #ifndef __WINSCW__
    #ifndef _DEBUG
        #if defined(__VC32__)
            #pragma message( "Warning: MG trace printing is on in release build!" )     // CSI: 68 #  68:use of #pragma message
        #else // __GCC32__
            #warning "MG trace printing is on in release build!"
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
    #define MG_TRACE_STRING(name,s) _LIT(name,s)

    /**
     * Output to debugger output
     */
    #define MG_TRACE_PRINT RDebug::Print

#else   // MG_ENABLE_TRACE_PRINT

    /**
     * Dummy struct for checking that all MG_TRACE_PRINT's define string
     * literals using space-saving MG_TRACE_STRING.
     */
    struct TMGEmptyTraceString { };     // CSI: 80 #  80:use of struct

    /**
     * Macro for defining debug-only literal strings (empty release version)
     */
    #define MG_TRACE_STRING(name, s) const TMGEmptyTraceString name

    /**
     * Output to debugger output (empty)
     */
    #define MG_TRACE_PRINT MG_EmptyTracePrint


    /// Empty debug print function for release builds.
    inline void MG_EmptyTracePrint(TMGEmptyTraceString)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1>
    inline void MG_EmptyTracePrint(TMGEmptyTraceString,T1)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2>
    inline void MG_EmptyTracePrint(TMGEmptyTraceString,T1,T2)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3>
    inline void MG_EmptyTracePrint(TMGEmptyTraceString,T1,T2,T3)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3,class T4>
    inline void MG_EmptyTracePrint(TMGEmptyTraceString,T1,T2,T3,T4)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3,class T4,class T5>
    inline void MG_EmptyTracePrint(TMGEmptyTraceString,T1,T2,T3,T4,T5)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3,class T4,class T5,class T6>
    inline void MG_EmptyTracePrint(TMGEmptyTraceString,T1,T2,T3,T4,T5,T6)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3,class T4,class T5,class T6,class T7>
    inline void MG_EmptyTracePrint(TMGEmptyTraceString,T1,T2,T3,T4,T5,T6,T7)
        {
        }

    /// Empty debug print function for release builds.
    template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
    inline void MG_EmptyTracePrint(TMGEmptyTraceString,T1,T2,T3,T4,T5,T6,T7,T8)
        {
        }


#endif  // MG_ENABLE_TRACE_PRINT

/**
 * Short variant for easier basic usage.
 */
#define MG_TRACE1( name, s ) MG_TRACE_STRING( name, s ); MG_TRACE_PRINT( name );

#ifdef MG_ENABLE_TRACE_PRINT
#define MG_TRACE2( name, s, i ) MG_TRACE_STRING( name, s ); MG_TRACE_PRINT( name, i );
#define MG_TRACE3( name, s, i, j ) MG_TRACE_STRING( name, s ); MG_TRACE_PRINT( name, i, j );
#define MG_TRACE4( name, s, i, j, k ) MG_TRACE_STRING( name, s ); MG_TRACE_PRINT( name, i, j, k );
#define MG_TRACE5( name, s, i, j, k, l ) MG_TRACE_STRING( name, s ); MG_TRACE_PRINT( name, i, j, k, l );
#else  // MG_ENABLE_DEBUG_PRINT
// If debug printing is not enabled, strip other parameters than the string
// away to prevent unwanted function calls in release build.
#define MG_TRACE2( name, s, i ) MG_TRACE1( name, s )
#define MG_TRACE3( name, s, i, j ) MG_TRACE1( name, s )
#define MG_TRACE4( name, s, i, j, k ) MG_TRACE1( name, s )
#define MG_TRACE5( name, s, i, j, k, l ) MG_TRACE1( name, s )
#endif  // MG_ENABLE_DEBUG_PRINT

#endif //  MGTRACEPRINT_H
