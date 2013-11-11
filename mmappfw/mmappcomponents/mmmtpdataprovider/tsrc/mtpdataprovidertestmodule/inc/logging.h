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
* Description:  Logging definitions for test module
*
*/


#ifndef LOGGING_H
#define LOGGING_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// CONSTANTS
const TUint KError              = 0x1;
const TUint KInit               = 0x2;
const TUint KPrint              = 0x4;
const TUint KMessage            = 0x10;
const TUint KFunction           = 0x20;
const TUint KVerbose            = 0x40;
const TUint KAlways             = 0xFFFFFFFF;
const TUint KDebugLevel         = ( KFunction | KPrint | KError | KMessage | KVerbose );

// Logging path
_LIT( KMtpDataProviderTestModuleLogPath, "\\logs\\testframework\\mtp\\" ); 
// Log file
_LIT( KMtpDataProviderTestModuleLogFile, "MtpDataProviderTestModule.txt" ); 
_LIT( KMtpDataProviderTestModuleLogFileWithTitle, "MtpDataProviderTestModule_[%S].txt" );

_LIT( KTestObserverLogFile, "TestObserver.txt" ); 
_LIT( KTestObserverLogFileWithTitle, "TestObserver_[%S].txt" );

_LIT( KFuncTraceFormat, "%S - %S" );

// MACROS
#define __TRACING_ENABLED

#ifdef __TRACING_ENABLED
    #define PRINTTESTMODULEIF
    #define PRINTTESTMODULE
    // target can be CStifLogger::ERDebug or CStifLogger::EFile
    #define TRACE_TARGET CStifLogger::EFile
     // Disable conditional expression is constant -warning
    #pragma warning( disable : 4127 )

    // General tracing function
    #define __TRACE(level,p) if ( ( (level) & KDebugLevel ) && LOGGER )\
                                          {LOGGER->Log p;}
              
    // Direct RDebug::Print                            
    #define __RDEBUG(p) if( KVerbose & KDebugLevel ){ RDebug::Print p ; } 

     // Log function name
    #define __TRACEFUNC() if( ( KFunction & KDebugLevel ) && LOGGER ){\
                                const char* func = __FUNCTION__;\
                                TPtrC8 F((const unsigned char*)func);\
                                LOGGER->Log(F);}

    #define PRINT0( level, p ) __TRACE( level, ( p ) ); \
        if ( ( level ) & KMessage ) { \
            PRINTTESTMODULEIF( p ); } \
        if ( ( level ) & KError ) { \
            PRINTTESTMODULE( p ); }
    #define PRINTn( level, p ) __TRACE( level, ( p ) );
    #define PRINTM0( p ) PRINT0( KMessage, ( _L( p ) ) )
    #define PRINTM1( p, a ) __TRACE( KMessage, ( _L( p ), a ) )
    #define PRINTM2( p, a, b ) __TRACE( KMessage, ( _L( p ), a, b ) )
    #define PRINTE0( p ) PRINT0( KError, ( _L( p ) ) )
    #define PRINTE1( p, a ) __TRACE( KError, ( _L( p ), a ) )
    #define PRINTE2( p, a, b ) __TRACE( KError, ( _L( p ), a, b ) )
    #define PRINTN0( p ) __TRACE( KPrint, ( _L( p ) ) )
    #define PRINTN1( p, a ) __TRACE( KPrint, ( _L( p ), a ) )
    #define PRINTN2( p, a, b ) __TRACE( KPrint, ( _L( p ), a, b ) )
    #define PRINTV0( p ) __TRACE( KVerbose, ( _L( p ) ) )
    #define PRINTV1( p, a ) __TRACE( KVerbose, ( _L( p ), a ) )
    #define PRINTV2( p, a, b ) __TRACE( KVerbose, ( _L( p ), a, b ) )
    #define PRINTV3( p, a, b, c ) __TRACE( KVerbose, ( _L( p ), a, b, c ) )
    #define PRINTV4( p, a, b, c, d ) __TRACE( KVerbose, ( _L( p ), a, b, c, d ) )
    #define PRINTF0( p ) __TRACE( KFunction, ( _L( p ) ) )
    #define PRINTF1( p, a ) __TRACE( KFunction, ( _L( p ), a ) )
    #define PRINTF2( p, a, b ) __TRACE( KFunction, ( _L( p ), a, b ) )
    #define PRINTF3( p, a, b, c ) __TRACE( KFunction, ( _L( p ), a, b, c ) )

#else // __TRACING_ENABLED
     // No tracing
    #define __TRACE(level,p)
    #define __RDEBUG(p)
    #define __TRACEFUNC()
#endif // __TRACING_ENABLED

#endif // LOGGING_H

