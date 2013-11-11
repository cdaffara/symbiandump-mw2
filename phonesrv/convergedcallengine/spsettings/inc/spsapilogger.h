/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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



#ifndef XSPSAPILOGGER_H
#define XSPSAPILOGGER_H

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

#define XSPSAPI_LOGGING_METHOD  0   // UREL BUILD

#else

#ifdef __WINS__
#define XSPSAPI_LOGGING_METHOD  2   // UDEB BUILD, WINS
#else
#define XSPSAPI_LOGGING_METHOD  2   // UDEB BUILD, HW
#endif // __WINS__

#endif // _DEBUG


//-----------------------------------------------------------------------------
//  LOGGING SETTINGS (DO NOT EDIT)
//-----------------------------------------------------------------------------

#if XSPSAPI_LOGGING_METHOD == 1      // Flogger

#include <flogger.h>
_LIT(KXSPSAPILogFolder,"XSPSAPI");
_LIT(KXSPSAPILogFile,"XSPSAPI.TXT");

#elif XSPSAPI_LOGGING_METHOD == 2    // RDebug

#include <e32svr.h>

#endif


//-----------------------------------------------------------------------------
//  LOGGING MACROs (DO NOT EDIT)
//
//  USE THESE MACROS IN YOUR CODE
//-----------------------------------------------------------------------------

#if XSPSAPI_LOGGING_METHOD == 1      // Flogger

#define XSPSLOGSTRING(AA)           { RFileLogger::Write(KXSPSAPILogFolder(),KXSPSAPILogFile(),EFileLoggingModeAppend, _L(AA) ); }
#define XSPSLOGSTRING2(AA,BB)       { RFileLogger::WriteFormat(KXSPSAPILogFolder(),KXSPSAPILogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB); }
#define XSPSLOGSTRING3(AA,BB,CC)    { RFileLogger::WriteFormat(KXSPSAPILogFolder(),KXSPSAPILogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC); }
#define XSPSLOGSTRING4(AA,BB,CC,DD) { RFileLogger::WriteFormat(KXSPSAPILogFolder(),KXSPSAPILogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD); }

#elif XSPSAPI_LOGGING_METHOD == 2    // RDebug

#define XSPSLOGSTRING(AA)           { RDebug::Print(_L(AA)); }
#define XSPSLOGSTRING2(AA,BB)       { RDebug::Print(_L(AA),BB); }
#define XSPSLOGSTRING3(AA,BB,CC)    { RDebug::Print(_L(AA),BB,CC); }
#define XSPSLOGSTRING4(AA,BB,CC,DD) { RDebug::Print(_L(AA),BB,CC,DD); }

#else   // XSPSAPI_LOGGING_METHOD == 0 or invalid -> Disable loggings

#define XSPSLOGSTRING(AA)           // Example: XSPSLOGSTRING("Test");
#define XSPSLOGSTRING2(AA,BB)       // Example: XSPSLOGSTRING("Test %d", aValue);
#define XSPSLOGSTRING3(AA,BB,CC)    // Example: XSPSLOGSTRING("Test %d %d", aValue1, aValue2);
#define XSPSLOGSTRING4(AA,BB,CC,DD) // Example: XSPSLOGSTRING("Test %d %d %d", aValue1, aValue2, aValue3);

#endif  // XSPSAPI_LOGGING_METHOD

#endif  // XSPSAPILOGGER_H
