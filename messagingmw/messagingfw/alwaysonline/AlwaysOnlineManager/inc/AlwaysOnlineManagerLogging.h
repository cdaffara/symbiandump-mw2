/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: AlwaysOnline server logging macros
*
*/

#ifndef __ALWAYSONLINEMANAGERLOGGING_H__
#define __ALWAYSONLINEMANAGERLOGGING_H__


#ifdef _DEBUG
#define AOMANAGERLOGGING_ON
#endif

//#ifndef __WINS__
// comment this if file logger is wanted
// #define AOMANAGERLOGGING_RDEBUG

//#endif

#ifdef AOMANAGERLOGGING_ON

#include <e32debug.h>
#include <flogger.h>

const TInt KAOEmailMajorVersion=0;
const TInt KAOEmailMinorVersion=0;
const TInt KAOEmailBuildVersion=1;

const TInt KAOManagerLoggingTextBuffer = 255;
    

#ifdef AOMANAGERLOGGING_RDEBUG
_LIT(KAOManagerLogTag,                 "AOMAN: ");
_LIT(KAOManagerLogTimeFormatString,    "%H:%T:%S:%*C2 ");

_LIT(KAOManagerLogMessageAppBanner,"AlwaysOnlineManager %d.%d.%d   (AOMAN)");
_LIT(KAOManagerLogEnterFn,         "AOMAN: -> %S");
_LIT(KAOManagerLogLeaveFn,         "AOMAN: <- %S");
_LIT(KAOManagerLogWrite,           "AOMAN: %S");
_LIT(KAOManagerLogExitBanner,      "AlwaysOnlineEmailPlugin exit");
#define KAOMANAGER_LOGGER_WRITE_FORMAT(a,b)   {_LIT(temp, a); TBuf<KAOManagerLoggingTextBuffer> buf; buf.Format(temp, b); buf.Insert(0, KAOManagerLogTag); RDebug::Print(buf);/*RDebug::Print(a, b);*/}		// CSI: 46 # Macro must be on one line
#define KAOMANAGER_LOGGER_WRITE_FORMAT8(a,b)   {_LIT8(temp, a); TBuf<KAOManagerLoggingTextBuffer> buf; buf.Format(temp, b); buf.Insert(0, KAOManagerLogTag); RDebug::Print(buf);/*RDebug::Print(a, b);*/}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_CREATE              {RDebug::Print(KAOManagerLogMessageAppBanner,KAOEmailMajorVersion, KAOEmailMinorVersion, KAOEmailBuildVersion);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_DELETE              {RDebug::Print(KAOManagerLogExitBanner);}
#define KAOMANAGER_LOGGER_WRITE(a)            {_LIT(temp, a); RDebug::Print(KAOManagerLogWrite, &temp);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_WRITE_TEXT(a)       {RDebug::Print(KAOManagerLogWrite, &a);}
#define KAOMANAGER_LOGGER_FN1(a)          {_LIT(temp, a); RDebug::Print(KAOManagerLogEnterFn, &temp);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_FN2(a)          {_LIT(temp, a); RDebug::Print(KAOManagerLogLeaveFn, &temp);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_WRITE_NUMBER(a)     {TBuf<KAOManagerLoggingTextBuffer> num; num.Num(a); RDebug::Print(KAOManagerLogWrite, &num);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_WRITE_BUFFER(a)     {TBuf<KAOManagerLoggingTextBuffer> temp; temp.Copy(a); RDebug::Print(KAOManagerLogWrite, &temp);}		// CSI: 46 # see comment above

#else
_LIT(KAOManagerLogDir,                 "AOMan");
_LIT(KAOManagerLogFile,                "AOMan.txt");
_LIT8(KAOManagerLogMessageAppBanner,      "AlwaysOnlineManager %d.%d.%d   (AOMAN)");
_LIT8(KAOManagerLogEnterFn,               "-> %S");
_LIT8(KAOManagerLogLeaveFn,               "<- %S");
_LIT8(KAOManagerLogExit,                  "AlwaysOnlineManager exit");

#define KAOMANAGER_LOGGER_CREATE              {RFileLogger::WriteFormat(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeOverwrite, KAOManagerLogMessageAppBanner, KAOEmailMajorVersion, KAOEmailMinorVersion, KAOEmailBuildVersion);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_DELETE              {RFileLogger::WriteFormat(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeOverwrite, KAOManagerLogMessageAppBanner, KAOEmailMajorVersion, KAOEmailMinorVersion, KAOEmailBuildVersion);}		// CSI: 46 # see comment above
    //{RFileLogger::Write(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeAppend, KAOManagerLogExit);}
#define KAOMANAGER_LOGGER_FN1(a)          {_LIT8(temp, a); RFileLogger::WriteFormat(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeAppend, KAOManagerLogEnterFn, &temp);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_FN2(a)          {_LIT8(temp, a); RFileLogger::WriteFormat(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeAppend, KAOManagerLogLeaveFn, &temp);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_WRITE(a)            {_LIT(temp, a); RFileLogger::Write(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeAppend, temp);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_WRITE_FORMAT(a,b)   {_LIT(temp, a); RFileLogger::WriteFormat(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeAppend, temp, b);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_WRITE_FORMAT8(a,b)   {_LIT8(temp, a); RFileLogger::WriteFormat(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeAppend, temp, b);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_WRITE_TIMESTAMP(a)  {_LIT(temp, a); TTime time; time.HomeTime(); TBuf<256> buffer; time.FormatL( buffer, KAOManagerLogTimeFormatString ); buffer.Insert(0, temp); RFileLogger::Write(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeAppend, buffer); }		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_WRITE_TEXT(a)       {RFileLogger::Write(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeAppend, a);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_WRITE_NUMBER(a)     {TBuf<KAOManagerLoggingTextBuffer> num; num.Num(a); RFileLogger::Write(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeAppend, num);}		// CSI: 46 # see comment above
#define KAOMANAGER_LOGGER_WRITE_BUFFER(a)     {TBuf8<KAOManagerLoggingTextBuffer> temp; temp.Copy(a); RFileLogger::Write(KAOManagerLogDir, KAOManagerLogFile, EFileLoggingModeAppend, temp);}		// CSI: 46 # see comment above


#endif

#else

#define KAOMANAGER_LOGGER_DEFINE             
#define KAOMANAGER_LOGGER_CREATE         
#define KAOMANAGER_LOGGER_DELETE         
#define KAOMANAGER_LOGGER_FN1(a)     
#define KAOMANAGER_LOGGER_FN2(a)     
#define KAOMANAGER_LOGGER_WRITE(a)           
#define KAOMANAGER_LOGGER_WRITE_FORMAT(a, b) 
#define KAOMANAGER_LOGGER_WRITE_FORMAT8(a, b) 
#define KAOMANAGER_LOGGER_WRITE_TEXT(a)
#define KAOMANAGER_LOGGER_WRITE_NUMBER(a)
#define KAOMANAGER_LOGGER_WRITE_FORMAT2(a, b)
#define KAOMANAGER_LOGGER_WRITE_BUFFER(a)

#endif // AOMANAGERLOGGING_ON

#endif // __ALWAYSONLINEMANAGERLOGGING_H__

    // End of File
