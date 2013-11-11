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
* Description:    OMA Presence Provisioning Adapter, for presence settings
*
*/





#ifndef _PRESENCE_PROV_LOGGER_H
#define _PRESENCE_PROV_LOGGER_H


// logging is done only in debug
#ifdef _DEBUG

#include <flogger.h>

//Logging constants
_LIT(KLogDir, "presenceota");
_LIT(KLogFile, "presenceprovlog.txt");

#define LOG_LIT(s) _L(s)
#define LOG_LIT8(s) _L8(s)
#define PRES_PROV_LOG DebugWriteFormat


/**
* Log file printing utility function
* @since Series 60 3.0
*/
inline void DebugWriteFormat( TRefByValue<const TDesC> aFmt,...) //lint !e960
    {
    VA_LIST list;
    VA_START (list, aFmt); //lint !e960
    TBuf< 300 > buffer;
    buffer.FormatList( aFmt, list );
    RFileLogger::Write( KLogDir, KLogFile, EFileLoggingModeAppend, buffer );
    }
    
/**
* Log file printing utility function
* @since Series 60 3.0
*/
inline void DebugWriteFormat( TRefByValue<const TDesC8> aFmt,...) //lint !e960
    {
    VA_LIST list;
    VA_START (list, aFmt); //lint !e960
    TBuf8<300> buffer;
    buffer.FormatList( aFmt, list );
    RFileLogger::Write( KLogDir, KLogFile, EFileLoggingModeAppend, buffer );
    }
    

#else // _DEBUG

struct LogEmptyDebugString { };

#define LOG_LIT(s) LogEmptyDebugString()
#define LOG_LIT8(s) LogEmptyDebugString()

/// Empty debug print function for release builds.
inline void PRES_PROV_LOG(LogEmptyDebugString)
    {
    }

template<class T1>
inline void PRES_PROV_LOG(LogEmptyDebugString,T1)
    {
    }

template<class T1,class T2>
inline void PRES_PROV_LOG(LogEmptyDebugString,T1,T2)
    {
    }

template<class T1,class T2,class T3>
inline void PRES_PROV_LOG(LogEmptyDebugString,T1,T2,T3)
    {
    }

template<class T1,class T2,class T3,class T4>
inline void PRES_PROV_LOG(LogEmptyDebugString,T1,T2,T3,T4)
    {
    }

template<class T1,class T2,class T3,class T4,class T5>
inline void PRES_PROV_LOG(LogEmptyDebugString,T1,T2,T3,T4,T5)
    {
    }

template<class T1,class T2,class T3,class T4,class T5,class T6>
inline void PRES_PROV_LOG(LogEmptyDebugString,T1,T2,T3,T4,T5,T6)
    {
    }

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7>
inline void PRES_PROV_LOG(LogEmptyDebugString,T1,T2,T3,T4,T5,T6,T7)
    {
    }

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
inline void PRES_PROV_LOG(LogEmptyDebugString,T1,T2,T3,T4,T5,T6,T7,T8)
    {
    }


#endif // _DEBUG


#endif // _PRESENCE_PROV_LOGGER_H
            
// End of File
