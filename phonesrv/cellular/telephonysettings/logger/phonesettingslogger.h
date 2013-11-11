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
* Description: 
*       Macro definition file for logging.
*
*
*/



#ifndef PHONESETTINGSLOGGER_H
#define PHONESETTINGSLOGGER_H

#include "phonesettingsloggingconf.h" 
#include <e32std.h>
#include <flogger.h>
#include <e32svr.h>

_LIT( KLogsDir,                  "PHS");
_LIT( KLogFileName,              "PHS.log");

/*
 We have 3 possible logging methods:

 set PHS_LOGGING_METHOD to 0 to get no logging at all
 set PHS_LOGGING_METHOD to 1 to get logging via RDebug::Print
 set PHS_LOGGING_METHOD to 2 to get logging to log file

 Switching on/off is done in the Configuration file (PhoneSettingsLoggingConf.h)
 */

#if ( defined (_PHSLOGGING) )
    #if ( defined (_PHS_LOGGING_TO_FILE))
        #define PHS_LOGGING_METHOD 2 
    #else
        #define PHS_LOGGING_METHOD 1
    #endif
#else
    #define PHS_LOGGING_METHOD 0
#endif


#if (PHS_LOGGING_METHOD==0)
    #define __PHSLOGSTRING(C)
    #define __PHSLOGSTRING1(C, X)
    #define __PHSLOGSTRING2(C, X, Y)
    #define __PHSLOGSTRING3(C, X, Y, Z)
#endif
#if (PHS_LOGGING_METHOD==1)
    #define __PHSLOGSTRING(C)            RDebug::Print(_L(C));
    #define __PHSLOGSTRING1(C, X)        RDebug::Print(_L(C),X);
    #define __PHSLOGSTRING2(C, X, Y)     RDebug::Print(_L(C),X, Y);
    #define __PHSLOGSTRING3(C, X, Y, Z)  RDebug::Print(_L(C),X, Y, Z);
#endif
#if (PHS_LOGGING_METHOD==2)
    #define __PHSLOGSTRING(C)            {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, tempLogDes());}
    #define __PHSLOGSTRING1(C, X)        {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X);}
    #define __PHSLOGSTRING2(C, X, Y)     {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X,Y);}
    #define __PHSLOGSTRING3(C, X, Y, Z)  {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X,Y,Z);}
#endif

#endif // PHONESETTINGSLOGGER_H
