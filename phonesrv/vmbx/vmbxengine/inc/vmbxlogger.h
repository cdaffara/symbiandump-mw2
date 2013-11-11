/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
*  Interface   : Private
*
*/

#ifndef __VMBXLOGGER_H__
#define __VMBXLOGGER_H__

//  INCLUDES
#include <e32def.h>

/*
-----------------------------------------------------------------------------

    USER LOG SELECTION

-----------------------------------------------------------------------------
*/

#ifndef _DEBUG

/***************************
* Logging method variants:
* 0 = No logging
* 1 = Flogger
* 2 = RDebug
***************************/

#define VMB_LOGGING_METHOD      0   // UREL BUILD

#else

#ifdef __WINS__
#define VMB_LOGGING_METHOD      1   // UDEB BUILD, WINS
#else
#define VMB_LOGGING_METHOD      2   // UDEB BUILD, HW
#endif // __WINS__

#endif // _DEBUG


// Next is for backward compatibility
#if VMB_LOGGING_METHOD > 0
    #define VMB_LOGGING_ENABLED
#endif //VMB_LOGGING_METHOD


/**
*
* FLOGGER SETTINGS
*
*/
#if VMB_LOGGING_METHOD == 1      // Flogger

    #include <flogger.h>

    _LIT(KVMBLogFolder, "VMBX");
    _LIT(KVMBLogFile, "VMBX.TXT");

#elif VMB_LOGGING_METHOD == 2    // RDebug

    #include <e32svr.h>

#endif //VMB_LOGGING_METHOD


/**
*
* LOGGING MACROs
*
* USE THESE MACROS IN YOUR CODE !
*
*/

#if VMB_LOGGING_METHOD == 1      // Flogger

#define VMBLOGTEXT(AAA) /*lint -save -e960 */              { RFileLogger::Write(KVMBLogFolder(),KVMBLogFile(),EFileLoggingModeAppend,AAA); } /*lint -restore */
#define VMBLOGSTRING(AAA) /*lint -save -e960 */            { _LIT(tempLogDes,AAA); RFileLogger::Write(KVMBLogFolder(),KVMBLogFile(),EFileLoggingModeAppend,tempLogDes()); } /*lint -restore */
#define VMBLOGSTRING2(AAA,BBB) /*lint -save -e960 -e437 */ { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KVMBLogFolder(),KVMBLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB); } /*lint -restore */
#define VMBLOGSTRING3(AAA,BBB,CCC) /*lint -save -e960 */   { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KVMBLogFolder(),KVMBLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB,CCC); } /*lint -restore */

#elif VMB_LOGGING_METHOD == 2    // RDebug

#define VMBLOGTEXT(AAA)              RDebug::Print(AAA);
#define VMBLOGSTRING(AAA)            RDebug::Print(_L(AAA));
#define VMBLOGSTRING2(AAA,BBB)       RDebug::Print(_L(AAA),BBB);
#define VMBLOGSTRING3(AAA,BBB,CCC)   RDebug::Print(_L(AAA),BBB,CCC);

#else    // VMB_LOGGING_METHOD == 0 or invalid

// Example: VMBLOGTEXT(own_desc);
#define VMBLOGTEXT(AAA)
// Example: VMBLOGSTRING("Test");
#define VMBLOGSTRING(AAA)
// Example: VMBLOGSTRING("Test %i", aValue);
#define VMBLOGSTRING2(AAA,BBB)
// Example: VMBLOGSTRING("Test %i %i", aValue1, aValue2);
#define VMBLOGSTRING3(AAA,BBB,CCC)

#endif  // VMB_LOGGING_METHOD

#endif    // VMBXLOGGER_H
