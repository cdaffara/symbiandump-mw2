/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Logging macros
*
*/


#ifndef NPPPSYLOGGING_H
#define NPPPSYLOGGING_H

//  INCLUDES
#include <flogger.h>
#include <f32file.h>


// CONSTANTS

// MACROS
#ifdef _DEBUG
_LIT( KLoggingFolder, "NppPsy" );
_LIT( KLoggingFile, "log.txt" );
_LIT( KLoggingFullName, "c:\\logs\\NppPsy\\log.TXT" );

#define CLEARTRACELOG \
    {RFs fs;fs.Connect();CFileMan* fileMan = CFileMan::NewL( fs ); \
    fileMan->Delete( KLoggingFullName ); delete fileMan; fs.Close();\
    }
    
#define TRACETEXT( S1 )         { RFileLogger::Write( KLoggingFolder(),\
KLoggingFile(), EFileLoggingModeAppend, S1 ); }

#define TRACESTRING( S1 )       { \
    _LIT( tempLogDes, S1 ); \
    RFileLogger::Write( KLoggingFolder(), KLoggingFile(), \
    EFileLoggingModeAppend, tempLogDes() ); \
    }

#define TRACESTRING2( S1, S2 )  { \
    _LIT( tempLogDes, S1 ); \
    RFileLogger::WriteFormat( KLoggingFolder(), KLoggingFile(), \
    EFileLoggingModeAppend, TRefByValue < const TDesC>( tempLogDes() ), S2 ); \
    }


#else   // _DEBUG
#define CLEARTRACELOG 
#define TRACETEXT( S1 )
#define TRACESTRING( S1 )
#define TRACESTRING2( S1, S2 )
#endif  // _DEBUG

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

#endif  // NPPPSYLOGGING_H
// End of File
