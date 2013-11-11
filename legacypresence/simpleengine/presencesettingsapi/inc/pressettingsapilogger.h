/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    This is class is logging purposes.
*
*/





#ifndef _PRESSETTINGSAPILOGGER_H
#define _PRESSETTINGSAPILOGGER_H


// logging is done only in debug
#ifdef _DEBUG

#include <flogger.h>

//Logging constants
_LIT(KLogDir, "pressettingsapi");
_LIT(KLogFile, "pressettingsapi.txt");

#define D_OPENG_LIT(s) _L(s)
#define OPENG_DP OPENGDebugWriteFormat


/**
* Log file printing utility function
* @since Series 60 3.0
*/
inline void OPENGDebugWriteFormat( TRefByValue<const TDesC> aFmt,...) //lint !e960
    {
    //Use RFileLogger to write log - no time and date
    RFileLogger logger;
    logger.Connect();
                         //use date, use time
    logger.SetDateAndTime( EFalse, ETrue );
    logger.CreateLog( KLogDir, 
                      KLogFile, 
                      EFileLoggingModeAppend );

    TBuf< 256 > buffer;

        {
        VA_LIST list;
        VA_START (list, aFmt); //lint !e960
        TBuf< 300 > formatted;
        formatted.FormatList( aFmt, list );
        buffer.Append( formatted.Left( buffer.MaxLength() - buffer.Length() ) ); 
        }

    logger.Write( buffer );
    logger.CloseLog();
    logger.Close();
    }


#else // _DEBUG

struct TOPENGEmptyDebugString { };

#define D_OPENG_LIT(s) TOPENGEmptyDebugString()

/// Empty debug print function for release builds.
inline void OPENG_DP(TOPENGEmptyDebugString)
    {
    }

template<class T1>
inline void OPENG_DP(TOPENGEmptyDebugString,T1)
    {
    }

template<class T1,class T2>
inline void OPENG_DP(TOPENGEmptyDebugString,T1,T2)
    {
    }

template<class T1,class T2,class T3>
inline void OPENG_DP(TOPENGEmptyDebugString,T1,T2,T3)
    {
    }

template<class T1,class T2,class T3,class T4>
inline void OPENG_DP(TOPENGEmptyDebugString,T1,T2,T3,T4)
    {
    }

template<class T1,class T2,class T3,class T4,class T5>
inline void OPENG_DP(TOPENGEmptyDebugString,T1,T2,T3,T4,T5)
    {
    }

template<class T1,class T2,class T3,class T4,class T5,class T6>
inline void OPENG_DP(TOPENGEmptyDebugString,T1,T2,T3,T4,T5,T6)
    {
    }

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7>
inline void OPENG_DP(TOPENGEmptyDebugString,T1,T2,T3,T4,T5,T6,T7)
    {
    }

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
inline void OPENG_DP(TOPENGEmptyDebugString,T1,T2,T3,T4,T5,T6,T7,T8)
    {
    }


#endif // _DEBUG


#endif // _PRESSETTINGSAPILOGGER_H
            
// End of File
