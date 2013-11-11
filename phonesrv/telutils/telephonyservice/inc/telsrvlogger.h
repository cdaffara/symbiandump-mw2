/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef TELSRVLOGGER_H_
#define TELSRVLOGGER_H_

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

#define TS_LOGGING_METHOD      0   // UREL BUILD

#else

#ifdef __WINS__
#define TS_LOGGING_METHOD      2   // UDEB BUILD, WINS
#else
#define TS_LOGGING_METHOD      2   // UDEB BUILD, HW
#endif // __WINS__

#endif // _DEBUG


//-----------------------------------------------------------------------------
//  LOGGING SETTINGS (DO NOT EDIT)
//-----------------------------------------------------------------------------

#if TS_LOGGING_METHOD == 1      // Flogger

#include <flogger.h>
_LIT(KTSLogFolder,"TS");
_LIT(KTSLogFile,"telsrvlog.txt");

#elif TS_LOGGING_METHOD == 2    // RDebug

#include <e32svr.h>

#endif


//-----------------------------------------------------------------------------
//  LOGGING MACROs (DO NOT EDIT)
//
//  USE THESE MACROS IN YOUR CODE
//-----------------------------------------------------------------------------

#if TS_LOGGING_METHOD == 1      // Flogger

#define TSLOGSTRING(AA)           { RFileLogger::Write(KTSLogFolder(),KTSLogFile(),EFileLoggingModeAppend, _L(AA) ); }
#define TSLOGSTRING2(AA,BB)       { RFileLogger::WriteFormat(KTSLogFolder(),KTSLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB); }


#elif TS_LOGGING_METHOD == 2    // RDebug

#define TSLOGSTRING(AA)           { RDebug::Print(_L(AA)); }
#define TSLOGSTRING2(AA,BB)       { RDebug::Print(_L(AA),BB); }


#else   // TS_LOGGING_METHOD == 0 or invalid -> Disable loggings

#define TSLOGSTRING(AA)           // Example: TSLOGSTRING("Test");
#define TSLOGSTRING2(AA,BB)       // Example: TSLOGSTRING("Test %d", aValue);

#endif  // TS_LOGGING_METHOD


#endif /* TELSRVLOGGER_H_ */
