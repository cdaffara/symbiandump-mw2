/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Offers logging facility through flogger
*
*/


#ifndef SATLOG_H
#define SATLOG_H

// LOGGING LEVELS
const TInt SIMPLE( 1 );
const TInt NORMAL( 2 );
const TInt DETAILED( 3 );

#include <flogger.h>
#include <e32svr.h> // For RDebug

#ifdef ENABLE_SAT_LOGGING

    //
    // DEFINE THE LOGGING LEVEL HERE: SIMPLE, NORMAL, DETAILED
    //
    #define LOGGING_LEVEL NORMAL

    // CONSTANTS
    _LIT( KLogDir, "sat" );
    _LIT( KLogFile, "sat.txt" );

    // This warning is disabled: conditinal expression is constant.

    //
    // USE THIS MACRO FOR LOGGING. THE LEVEL OF LOGGING CAN BE
    // SIMPLE, NORMAL, DETAILED
    //
    #ifdef LOG
        #undef LOG
    #endif

    #define LOG( LEVEL, TEXT ) \
        { \
        if ( LEVEL <= LOGGING_LEVEL ) \
            { \
            _LIT( KText, TEXT ); \
            RFileLogger::Write( \
                KLogDir, \
                KLogFile, \
                EFileLoggingModeAppend, \
                KText ); \
            RDebug::Print( KText ); \
            } \
        }

    #ifdef LOG2
        #undef LOG2
    #endif

    #define LOG2( LEVEL, TEXT, VARIABLE ) \
        { \
        if ( LEVEL <= LOGGING_LEVEL ) \
            { \
            _LIT( KText, TEXT ); \
            RFileLogger::WriteFormat( \
                KLogDir, \
                KLogFile, \
                EFileLoggingModeAppend, \
                TRefByValue<const TDesC>( KText() ), VARIABLE ); \
            RDebug::Print( KText, VARIABLE ); \
            } \
        }

    #ifdef LOG3
        #undef LOG3
    #endif

    #define LOG3( LEVEL, TEXT, VARIABLE1, VARIABLE2 ) \
        { \
        if ( LEVEL <= LOGGING_LEVEL ) \
            { \
            _LIT( KText, TEXT ); \
            RFileLogger::WriteFormat( \
                KLogDir, \
                KLogFile, \
                EFileLoggingModeAppend, \
                TRefByValue<const TDesC>( KText() ), VARIABLE1, VARIABLE2 ); \
            RDebug::Print( KText, VARIABLE1, VARIABLE2 ); \
            } \
        }

#else // ENABLE_SAT_LOGGING

    #define LOG( LEVEL, TEXT )

    #define LOG2( LEVEL, TEXT, VARIABLE1 )

    #define LOG3( LEVEL, TEXT, VARIABLE1, VARIABLE2 )

#endif // ENABLE_SAT_LOGGING

#endif // SATLOG_H
