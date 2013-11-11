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


#ifndef __CCELOGGER_H__
#define __CCELOGGER_H__

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

#define CCE_LOGGING_METHOD      0   // UREL BUILD

#else

#ifdef __WINS__
#define CCE_LOGGING_METHOD      2   // UDEB BUILD, WINS
#else
#define CCE_LOGGING_METHOD      2   // UDEB BUILD, HW
#endif // __WINS__

#endif // _DEBUG


//-----------------------------------------------------------------------------
//  LOGGING SETTINGS (DO NOT EDIT)
//-----------------------------------------------------------------------------

#if CCE_LOGGING_METHOD == 1      // Flogger

#include <flogger.h>
_LIT(KCCELogFolder,"CCE");
_LIT(KCCELogFile,"CCELOG.TXT");

#elif CCE_LOGGING_METHOD == 2    // RDebug

#include <e32svr.h>

#endif


//-----------------------------------------------------------------------------
//  LOGGING MACROs (DO NOT EDIT)
//
//  USE THESE MACROS IN YOUR CODE
//-----------------------------------------------------------------------------

#if CCE_LOGGING_METHOD == 1      // Flogger

#define CCELOGSTRING(AA)           { RFileLogger::Write(KCCELogFolder(),KCCELogFile(),EFileLoggingModeAppend, _L(AA) ); }
#define CCELOGSTRING2(AA,BB)       { RFileLogger::WriteFormat(KCCELogFolder(),KCCELogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB); }
#define CCELOGSTRING3(AA,BB,CC)    { RFileLogger::WriteFormat(KCCELogFolder(),KCCELogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC); }
#define CCELOGSTRING4(AA,BB,CC,DD) { RFileLogger::WriteFormat(KCCELogFolder(),KCCELogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD); }

#elif CCE_LOGGING_METHOD == 2    // RDebug

#define CCELOGSTRING(AA)           { RDebug::Print(_L(AA)); }
#define CCELOGSTRING2(AA,BB)       { RDebug::Print(_L(AA),BB); }
#define CCELOGSTRING3(AA,BB,CC)    { RDebug::Print(_L(AA),BB,CC); }
#define CCELOGSTRING4(AA,BB,CC,DD) { RDebug::Print(_L(AA),BB,CC,DD); }

#else   // CCE_LOGGING_METHOD == 0 or invalid -> Disable loggings

#define CCELOGSTRING(AA)           // Example: CCELOGSTRING("Test");
#define CCELOGSTRING2(AA,BB)       // Example: CCELOGSTRING("Test %d", aValue);
#define CCELOGSTRING3(AA,BB,CC)    // Example: CCELOGSTRING("Test %d %d", aValue1, aValue2);
#define CCELOGSTRING4(AA,BB,CC,DD) // Example: CCELOGSTRING("Test %d %d %d", aValue1, aValue2, aValue3);

#endif  // CCE_LOGGING_METHOD

#endif  // CCELOGGER_H
