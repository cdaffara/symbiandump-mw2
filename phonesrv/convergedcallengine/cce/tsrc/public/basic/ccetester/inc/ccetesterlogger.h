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


#ifndef __CCETESTERLOGGER_H__
#define __CCETESTERLOGGER_H__

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

#define CCETESTER_LOGGING_METHOD      0   // UREL BUILD

#else

#ifdef __WINS__
#define CCETESTER_LOGGING_METHOD      2   // UDEB BUILD, WINS
#else
#define CCETESTER_LOGGING_METHOD      2   // UDEB BUILD, HW
#endif // __WINS__

#endif // _DEBUG


//-----------------------------------------------------------------------------
//  LOGGING SETTINGS (DO NOT EDIT)
//-----------------------------------------------------------------------------

#if CCETESTER_LOGGING_METHOD == 1      // Flogger

#include <flogger.h>
_LIT(KCCETesterLogFolder,"ccetester");
_LIT(KCCETesterLogFile,"ccetesterlog.txt");

#elif CCETESTER_LOGGING_METHOD == 2    // RDebug

#include <e32svr.h>

#endif


//-----------------------------------------------------------------------------
//  LOGGING MACROs (DO NOT EDIT)
//
//  USE THESE MACROS IN YOUR CODE
//-----------------------------------------------------------------------------

#if CCETESTER_LOGGING_METHOD == 1      // Flogger

#define CCETESTERLOG(AA)                    { RFileLogger::Write(KCCETesterLogFolder(),KCCETesterLogFile(),EFileLoggingModeAppend, _L(AA) ); }
#define CCETESTERLOG1(AA,BB)                { RFileLogger::WriteFormat(KCCETesterLogFolder(),KCCETesterLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB); }
#define CCETESTERLOG2(AA,BB,CC)             { RFileLogger::WriteFormat(KCCETesterLogFolder(),KCCETesterLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC); }
#define CCETESTERLOG3(AA,BB,CC,DD)          { RFileLogger::WriteFormat(KCCETesterLogFolder(),KCCETesterLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD); }
#define CCETESTERLOG4(AA,BB,CC,DD,EE)       { RFileLogger::WriteFormat(KCCETesterLogFolder(),KCCETesterLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD,EE); }
#define CCETESTERLOG5(AA,BB,CC,DD,EE,FF)    { RFileLogger::WriteFormat(KCCETesterLogFolder(),KCCETesterLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD,EE,FF); }
#define CCETESTERLOG6(AA,BB,CC,DD,EE,FF,GG) { RFileLogger::WriteFormat(KCCETesterLogFolder(),KCCETesterLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD,EE,FF,GG); }

#elif CCETESTER_LOGGING_METHOD == 2    // RDebug

#define CCETESTERLOG(AA)                    { RDebug::Print(_L(AA)); }
#define CCETESTERLOG1(AA,BB)                { RDebug::Print(_L(AA),BB); }
#define CCETESTERLOG2(AA,BB,CC)             { RDebug::Print(_L(AA),BB,CC); }
#define CCETESTERLOG3(AA,BB,CC,DD)          { RDebug::Print(_L(AA),BB,CC,DD); }
#define CCETESTERLOG4(AA,BB,CC,DD,EE)       { RDebug::Print(_L(AA),BB,CC,DD,EE); }
#define CCETESTERLOG5(AA,BB,CC,DD,EE,FF)    { RDebug::Print(_L(AA),BB,CC,DD,EE,FF); }
#define CCETESTERLOG6(AA,BB,CC,DD,EE,FF,GG) { RDebug::Print(_L(AA),BB,CC,DD,EE,FF,GG); }

#else   // CCETESTER_LOGGING_METHOD == 0 or invalid -> Disable loggings

#define CCETESTERLOG(AA)           // Example: CCELOGSTRING("Test");
#define CCETESTERLOG1(AA,BB)       // Example: CCELOGSTRING("Test %d", aValue);
#define CCETESTERLOG2(AA,BB,CC)    // Example: CCELOGSTRING("Test %d %d", aValue1, aValue2);
#define CCETESTERLOG3(AA,BB,CC,DD)
#define CCETESTERLOG4(AA,BB,CC,DD,EE)
#define CCETESTERLOG5(AA,BB,CC,DD,EE,FF)
#define CCETESTERLOG6(AA,BB,CC,DD,EE,FF,GG)

#endif  // CCETESTER_LOGGING_METHOD

#endif  // __CCETESTERLOGGER_H__
