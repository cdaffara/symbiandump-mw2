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
* Description:  Logger headerfile for CbsServer
*
*/



#ifndef CBSLOGGER_H
#define CBSLOGGER_H

/* 
-----------------------------------------------------------------------------
    LOG SELECTION
-----------------------------------------------------------------------------
*/
#ifndef _DEBUG
    // UREL BUILD:
    #define CBS_LOGGING_METHOD  0   // No logging in UREL builds
#else
    // UDEB BUILD:
    #define CBS_LOGGING_METHOD  1   // 0 = No logging, 
                                        // 1 = Flogger, 
                                        // 2 = RDebug
#endif //_DEBUG


#if CBS_LOGGING_METHOD > 0
    #define CBS_LOGGING_ENABLED     // This is for backward compatibility
#endif //CBS_LOGGING_METHOD 

/* 
-----------------------------------------------------------------------------
    FLOGGER SETTINGS
-----------------------------------------------------------------------------
*/
#if CBS_LOGGING_METHOD == 1      // Flogger

    #include <flogger.h>

    _LIT(KCbsLogFolder, "TF");
    _LIT(KCbsLogFile, "TFLOG.TXT");

#elif CBS_LOGGING_METHOD == 2    // RDebug

    #include <e32svr.h>

#endif //CBS_LOGGING_METHOD


/*
-----------------------------------------------------------------------------
    LOGGING MACROs

    USE THESE MACROS IN YOUR CODE !
-----------------------------------------------------------------------------
*/


#if CBS_LOGGING_METHOD == 1      // Flogger

#define CBSLOGTEXT(AAA) /*lint -save -e960 */              { RFileLogger::Write(KCbsLogFolder(),KCbsLogFile(),EFileLoggingModeAppend,AAA); } /*lint -restore */
#define CBSLOGSTRING(AAA) /*lint -save -e960 */           { _LIT(tempLogDes,AAA); RFileLogger::Write(KCbsLogFolder(),KCbsLogFile(),EFileLoggingModeAppend,tempLogDes()); } /*lint -restore */
#define CBSLOGSTRING2(AAA,BBB) /*lint -save -e960 -e437 */       { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KCbsLogFolder(),KCbsLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB); } /*lint -restore */
#define CBSLOGSTRING3(AAA,BBB,CCC) /*lint -save -e960 */   { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KCbsLogFolder(),KCbsLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB,CCC); } /*lint -restore */

#elif CBS_LOGGING_METHOD == 2    // RDebug

#define CBSLOGTEXT(AAA)              RDebug::Print(AAA);
#define CBSLOGSTRING(AAA)            RDebug::Print(_L(AAA));
#define CBSLOGSTRING2(AAA,BBB)       RDebug::Print(_L(AAA),BBB);
#define CBSLOGSTRING3(AAA,BBB,CCC)   RDebug::Print(_L(AAA),BBB,CCC);

#else    // CBS_LOGGING_METHOD == 0 or invalid

// Example: CBSLOGTEXT(own_desc);
#define CBSLOGTEXT(AAA)              
// Example: CBSLOGSTRING("Test");
#define CBSLOGSTRING(AAA)            
// Example: CBSLOGSTRING("Test %i", aValue);
#define CBSLOGSTRING2(AAA,BBB)       
// Example: CBSLOGSTRING("Test %i %i", aValue1, aValue2);
#define CBSLOGSTRING3(AAA,BBB,CCC)   

#endif  // CBS_LOGGING_METHOD

#endif    // CBSLOGGER_H
            
// End of File
