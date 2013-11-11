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


#ifndef __CCPTESTPLUGINLOGGER_H__
#define __CCPTESTPLUGINLOGGER_H__

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

#define CCPTESTPLUGIN_LOGGING_METHOD      0   // UREL BUILD

#else

#ifdef __WINS__
#define CCPTESTPLUGIN_LOGGING_METHOD      2   // UDEB BUILD, WINS
#else
#define CCPTESTPLUGIN_LOGGING_METHOD      2   // UDEB BUILD, HW
#endif // __WINS__

#endif // _DEBUG


//-----------------------------------------------------------------------------
//  LOGGING SETTINGS (DO NOT EDIT)
//-----------------------------------------------------------------------------

#if CCPTESTPLUGIN_LOGGING_METHOD == 1      // Flogger

#include <flogger.h>
_LIT(KCCPTESTPLUGINLOGFolder,"ccetester");
_LIT(KCCPTESTPLUGINLOGFile,"ccptestplugin.txt");

#elif CCPTESTPLUGIN_LOGGING_METHOD == 2    // RDebug

#include <e32svr.h>

#endif


//-----------------------------------------------------------------------------
//  LOGGING MACROs (DO NOT EDIT)
//
//  USE THESE MACROS IN YOUR CODE
//-----------------------------------------------------------------------------

#if CCPTESTPLUGIN_LOGGING_METHOD == 1      // Flogger

// plain
#define CCPTESTPLUGINLOG(AA)                   { RFileLogger::Write(KCCPTESTPLUGINLOGFolder(),KCCPTESTPLUGINLOGFile(),EFileLoggingModeAppend, _L(AA) ); }
// with parameters
#define CCPTESTPLUGINLOG1(AA,BB)                { RFileLogger::WriteFormat(KCCPTESTPLUGINLOGFolder(),KCCPTESTPLUGINLOGFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB); }
#define CCPTESTPLUGINLOG2(AA,BB,CC)             { RFileLogger::WriteFormat(KCCPTESTPLUGINLOGFolder(),KCCPTESTPLUGINLOGFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC); }
#define CCPTESTPLUGINLOG3(AA,BB,CC,DD)          { RFileLogger::WriteFormat(KCCPTESTPLUGINLOGFolder(),KCCPTESTPLUGINLOGFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD); }
#define CCPTESTPLUGINLOG4(AA,BB,CC,DD,EE)       { RFileLogger::WriteFormat(KCCPTESTPLUGINLOGFolder(),KCCPTESTPLUGINLOGFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD,EE); }
#define CCPTESTPLUGINLOG5(AA,BB,CC,DD,EE,FF)    { RFileLogger::WriteFormat(KCCPTESTPLUGINLOGFolder(),KCCPTESTPLUGINLOGFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD,EE,FF); }
#define CCPTESTPLUGINLOG6(AA,BB,CC,DD,EE,FF,GG) { RFileLogger::WriteFormat(KCCPTESTPLUGINLOGFolder(),KCCPTESTPLUGINLOGFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(AA)),BB,CC,DD,EE,FF,GG); }

#elif CCPTESTPLUGIN_LOGGING_METHOD == 2    // RDebug

// plain
#define CCPTESTPLUGINLOG(AA)                   { RDebug::Print(_L(AA)); }
// with parameters
#define CCPTESTPLUGINLOG1(AA,BB)                { RDebug::Print(_L(AA),BB); }
#define CCPTESTPLUGINLOG2(AA,BB,CC)             { RDebug::Print(_L(AA),BB,CC); }
#define CCPTESTPLUGINLOG3(AA,BB,CC,DD)          { RDebug::Print(_L(AA),BB,CC,DD); }
#define CCPTESTPLUGINLOG4(AA,BB,CC,DD,EE)       { RDebug::Print(_L(AA),BB,CC,DD,EE); }
#define CCPTESTPLUGINLOG5(AA,BB,CC,DD,EE,FF)    { RDebug::Print(_L(AA),BB,CC,DD,EE,FF); }
#define CCPTESTPLUGINLOG6(AA,BB,CC,DD,EE,FF,GG) { RDebug::Print(_L(AA),BB,CC,DD,EE,FF,GG); }

#else   // CCPTESTPLUGIN_LOGGING_METHOD == 0 or invalid -> Disable loggings

#define CCPTESTPLUGINLOG(AA)                    // Example: CCPTESTPLUGINLOG("Test");
#define CCPTESTPLUGINLOG1(AA,BB)                // Example: CCPTESTPLUGINLOG("Test %d", aValue);
#define CCPTESTPLUGINLOG2(AA,BB,CC)             // Example: CCPTESTPLUGINLOG("Test %d %d", aValue1, aValue2);
#define CCPTESTPLUGINLOG3(AA,BB,CC,DD)
#define CCPTESTPLUGINLOG4(AA,BB,CC,DD,EE)
#define CCPTESTPLUGINLOG5(AA,BB,CC,DD,EE,FF)
#define CCPTESTPLUGINLOG6(AA,BB,CC,DD,EE,FF,GG)

#endif  // CCETESTER_LOGGING_METHOD

#endif  // __CCPTESTPLUGINLOGGER_H__
