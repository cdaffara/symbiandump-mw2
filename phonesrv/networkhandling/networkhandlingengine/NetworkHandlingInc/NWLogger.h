/*
* Copyright (c) 2002 - 2009 Nokia Corporation and/or its subsidiary(-ies). 
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



#ifndef __NWLOGGER_H__
#define __NWLOGGER_H__

//  INCLUDES
#include <e32def.h>

/* 
-----------------------------------------------------------------------------

    USER LOG SELECTION

-----------------------------------------------------------------------------
*/

inline TUint KNWLOGMASK()
    {
    return 0x8fffffff;          // This is default mask value.    
    }                           // User can change mask value to get different king of logging.

#ifndef _DEBUG

/***************************
* Logging method variants:
* 0 = No logging
* 1 = Flogger
* 2 = RDebug
***************************/ 

#define NW_LOGGING_METHOD      0   // UREL BUILD

#else

#ifdef __WINS__
#define NW_LOGGING_METHOD      1   // UDEB BUILD, WINS
#else
#define NW_LOGGING_METHOD      2   // UDEB BUILD, HW
#endif // __WINS__

#endif // _DEBUG


// Next is for backward compatibility
#if NW_LOGGING_METHOD > 0
#define NW_LOGGING_ENABLED     
#endif


/* 
-----------------------------------------------------------------------------

    LOGGING OPTIONS

-----------------------------------------------------------------------------
*/

#define KNWOBJECT  0x00000001   // Construction and destruction

#define KNWERROR   0x00000002   // Error that can be handled. Describe reason for error situation.
#define KNWPANIC   0x00000004   // Critical error: Panic.

#define KNWMESIN   0x00000008   // From client to NW message
#define KNWMESOUT  0x00000010   // From NW to client message
#define KNWMESINT  0x00000020   // TELE internal message

#define KNWREQIN   0x00000040   // Public interface methods
#define KNWREQOUT  0x00000080   // Set outgoing request
#define KNWREQEND  0x00000100   // Complete request

#define KNWINT     0x00000200   // Internal activity

#define KNWMSTATE  0x00000400   // Component state changes

#define KNWGENERAL 0x00000800   // For deprecated NWLOGxxx-macros and for temporary usage


/* 
-----------------------------------------------------------------------------

    LOG SETTINGS

-----------------------------------------------------------------------------
*/

#if NW_LOGGING_METHOD == 1      // Flogger

#include <flogger.h>
_LIT(KNwLogFolder,"NW");
_LIT(KTfLogFile,"NWLOG.TXT");

#elif NW_LOGGING_METHOD == 2    // RDebug

#include <e32svr.h>

#endif


/*
-----------------------------------------------------------------------------

    LOGGING MACROs

    USE THESE MACROS IN YOUR CODE !

-----------------------------------------------------------------------------
*/

#if NW_LOGGING_METHOD == 1      // Flogger

#define NWLOGTEXT(Type,AAA)              { if((KNWLOGMASK()&(Type))==(Type)) { RFileLogger::Write(KNwLogFolder(),KTfLogFile(),EFileLoggingModeAppend, AAA); }}
#define NWLOGSTRING(Type,AAA)            { if((KNWLOGMASK()&(Type))==(Type)) { _LIT(tempLogDes,AAA); RFileLogger::Write(KNwLogFolder(),KTfLogFile(),EFileLoggingModeAppend,tempLogDes()); }}
#define NWLOGSTRING2(Type,AAA,BBB)       { if((KNWLOGMASK()&(Type))==(Type)) { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KNwLogFolder(),KTfLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB); }}
#define NWLOGSTRING3(Type,AAA,BBB,CCC)   { if((KNWLOGMASK()&(Type))==(Type)) { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KNwLogFolder(),KTfLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB,CCC); }}
#define NWLOGSTRING4(Type,AAA,BBB,CCC,DDD)   { if((KNWLOGMASK()&(Type))==(Type)) { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KNwLogFolder(),KTfLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB,CCC,DDD); }}

#elif NW_LOGGING_METHOD == 2    // RDebug

#define NWLOGTEXT(Type,AAA)              { if((KNWLOGMASK()&(Type))==(Type)) RDebug::Print(AAA); }
#define NWLOGSTRING(Type,AAA)            { if((KNWLOGMASK()&(Type))==(Type)) RDebug::Print(_L(AAA)); }
#define NWLOGSTRING2(Type,AAA,BBB)       { if((KNWLOGMASK()&(Type))==(Type)) RDebug::Print(_L(AAA),BBB); }
#define NWLOGSTRING3(Type,AAA,BBB,CCC)   { if((KNWLOGMASK()&(Type))==(Type)) RDebug::Print(_L(AAA),BBB,CCC); }
#define NWLOGSTRING4(Type,AAA,BBB,CCC,DDD)   { if((KNWLOGMASK()&(Type))==(Type)) RDebug::Print(_L(AAA),BBB,CCC,DDD); }

#else   // NW_LOGGING_METHOD == 0 or invalid

#define NWLOGTEXT(Type,AAA)              // Example: NWLOGTEXT(KNWMESOUT, own_desc);
#define NWLOGSTRING(Type,AAA)            // Example: NWLOGSTRING(KNWMESIN, "Test");
#define NWLOGSTRING2(Type,AAA,BBB)       // Example: NWLOGSTRING(KNWMESINT, "Test %i", aValue);
#define NWLOGSTRING3(Type,AAA,BBB,CCC)   // Example: NWLOGSTRING(KNWMESOUT, "Test %i %i", aValue1, aValue2);
#define NWLOGSTRING4(Type,AAA,BBB,CCC,DDD)   // Example: NWLOGSTRING(KNWMESOUT, "Test %i %i %i", aValue1, aValue2, aValue3);

#endif  // NW_LOGGING_METHOD



#endif  // NWLOGGER_H
