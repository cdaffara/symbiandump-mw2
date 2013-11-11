/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides macros for logging
*
*/



#ifndef CENREPDATABASELOGGER_H
#define CENREPDATABASELOGGER_H

//  INCLUDES
#include <e32def.h>


//-----------------------------------------------------------------------------
//  USER LOGGING SETTINGS
//-----------------------------------------------------------------------------

#ifndef _DEBUG

/***************************
* Logging method variants:
*   0 = No logging
*   1 = Flogger
*   2 = RDebug
***************************/ 

#define CENREPDATABASE_LOGGING_METHOD  0   // UREL BUILD

#else

#ifdef __WINS__
#define CENREPDATABASE_LOGGING_METHOD  2   // UDEB BUILD, WINS
#else
#define CENREPDATABASE_LOGGING_METHOD  2   // UDEB BUILD, HW
#endif // __WINS__

#endif // _DEBUG


//-----------------------------------------------------------------------------
//  LOGGING SETTINGS (DO NOT EDIT)
//-----------------------------------------------------------------------------

#if CENREPDATABASE_LOGGING_METHOD == 1      // Flogger

#include <flogger.h>
_LIT(KCenRepDatabaseLogFolder,"cenrepdatabase");
_LIT(KCenRepDatabaseLogFile,"cenrepdatabase.log");

#elif CENREPDATABASE_LOGGING_METHOD == 2    // RDebug

#include <e32debug.h>

#endif


//-----------------------------------------------------------------------------
//  LOGGING MACROs (DO NOT EDIT)
//
//  USE THESE MACROS IN YOUR CODE
//-----------------------------------------------------------------------------

#if CENREPDATABASE_LOGGING_METHOD == 1      // Flogger

#define CENREPDATABASELOG(AA)           { RFileLogger::Write(KCenRepDatabaseLogFolder(),KCenRepDatabaseLogFile(),EFileLoggingModeAppend, _L(AA) ); }
#define CENREPDATABASELOG2(AA,BB)       { RFileLogger::WriteFormat(KCenRepDatabaseLogFolder(),KCenRepDatabaseLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB); }
#define CENREPDATABASELOG3(AA,BB,CC)    { RFileLogger::WriteFormat(KCenRepDatabaseLogFolder(),KCenRepDatabaseLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC); }
#define CENREPDATABASELOG4(AA,BB,CC,DD) { RFileLogger::WriteFormat(KCenRepDatabaseLogFolder(),KCenRepDatabaseLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD); }

#elif CENREPDATABASE_LOGGING_METHOD == 2    // RDebug

#define CENREPDATABASELOG(AA)           { RDebug::Print(_L(AA)); }
#define CENREPDATABASELOG2(AA,BB)       { RDebug::Print(_L(AA),BB); }
#define CENREPDATABASELOG3(AA,BB,CC)    { RDebug::Print(_L(AA),BB,CC); }
#define CENREPDATABASELOG4(AA,BB,CC,DD) { RDebug::Print(_L(AA),BB,CC,DD); }

#else   // CENREPDATABASE_LOGGING_METHOD == 0 or invalid -> Disable loggings

#define CENREPDATABASELOG(AA)           
#define CENREPDATABASELOG2(AA,BB)       
#define CENREPDATABASELOG3(AA,BB,CC)    
#define CENREPDATABASELOG4(AA,BB,CC,DD) 

#endif  // CENREPDATABASE_LOGGING_METHOD

#endif  // CENREPDATABASELOGGER_H
