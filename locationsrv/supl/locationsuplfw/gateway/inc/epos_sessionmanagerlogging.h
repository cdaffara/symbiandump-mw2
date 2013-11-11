/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Logging macros
*
*/



#ifndef SUPLGATEWAYLOGGING_H
#define SUPLGATEWAYLOGGING_H

//  INCLUDES
#include <flogger.h>
#include <f32file.h>


// CONSTANTS

// MACROS
#ifdef _DEBUG
_LIT( KLoggingFolder, "epos" );
_LIT( KLoggingFile, "log.txt" );
_LIT( KLoggingFullName, "c:\\logs\\suplgateway\\log.TXT" );

#define CLEARTRACELOG \
    { \
    RFs fs;fs.Connect(); \
    CFileMan* fileMan = CFileMan::NewL( fs ); \
    fileMan->Delete( KLoggingFullName ); \
    delete fileMan; \
    fs.Close();\
    }
    
#define TRACETEXT( S1 ) \
    { \
    RFileLogger::Write( \
        KLoggingFolder(), \
        KLoggingFile(), \
        EFileLoggingModeAppend, \
        S1 ); \
    }

#define TRACESTRING( S1 ) \
    { \
    _LIT( tempLogDes, S1 ); \
    RFileLogger::Write( \
        KLoggingFolder(), \
        KLoggingFile(), \
        EFileLoggingModeAppend, \
        tempLogDes() ); \
    }

#define TRACESTRING2( S1, S2 ) \
    { \
    _LIT( tempLogDes, S1 ); \
    RFileLogger::WriteFormat( \
        KLoggingFolder(), \
        KLoggingFile(), \
        EFileLoggingModeAppend,\
        TRefByValue < const TDesC >( tempLogDes() ), S2 ); \
    }

#define TRACEHEX2( PTR, LEN ) \
    { \
    RFileLogger::HexDump( \
        KLoggingFolder(), \
        KLoggingFile(), \
        EFileLoggingModeAppend, \
        NULL, \
        NULL, \
        PTR, \
        LEN ); \
    }


#else   // _DEBUG
#define CLEARTRACELOG 
#define TRACETEXT( S1 )
#define TRACESTRING( S1 )
#define TRACESTRING2( S1, S2 )
#define TRACEHEX2( PTR, LEN )

#endif  // _DEBUG

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

#endif  // SUPLGATEWAYLOGGING_H
// End of File
