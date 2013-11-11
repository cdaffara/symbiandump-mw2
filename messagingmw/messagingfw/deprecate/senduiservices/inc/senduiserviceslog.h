/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Logger facility for the SendUiServices.
*
*/




#ifndef SENDUISERVICESLOG_H
#define SENDUISERVICESLOG_H

// MACROS

#include <flogger.h>                    // HW debug logging support

//
#ifdef USE_LOGGER
//

_LIT(KSendUiServicesLogFile,"SENDUISERVICES.TXT");
_LIT(KSendUiServicesLogDir,"SENDUISERVICES");
_LIT(KSendUiServicesTimeFormatString,"%H:%T:%S:%*C2");

#define CREATELOGTEXT(a)    RFileLogger::Write(KSendUiServicesLogDir(),KSendUiServicesLogFile(),EFileLoggingModeOverwrite,a)
#define LOGTEXT(a)          RFileLogger::Write(KSendUiServicesLogDir(),KSendUiServicesLogFile(),EFileLoggingModeAppend,a)
#define LOGTEXT2(a,b)       RFileLogger::WriteFormat(KSendUiServicesLogDir(),KSendUiServicesLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(a),b)
#define LOGTEXT3(a,b,c)     RFileLogger::WriteFormat(KSendUiServicesLogDir(),KSendUiServicesLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(a),b,c)
#define LOGHEXDUMP(a,b,c,d) RFileLogger::HexDump(KSendUiServicesLogDir(),KSendUiServicesLogFile(),EFileLoggingModeAppend,a,b,c,d);
#define LOGTIMESTAMP(a)     {_LIT(temp, a); TTime time; time.HomeTime(); TBuf<256> buffer; time.FormatL( buffer, KSendUiServicesTimeFormatString ); buffer.Insert(0, temp); RFileLogger::Write(KSendUiServicesLogDir, KSendUiServicesLogFile, EFileLoggingModeAppend, buffer); }

#else // no logger

#define CREATELOGTEXT(a)
#define LOGTEXT(a)
#define LOGTEXT2(a,b)
#define LOGTEXT3(a,b,c)
#define LOGHEXDUMP(a,b,c,d)
#define LOGTIMESTAMP(a)

#endif // USE_LOGGER

#endif // SENDUISERVICESLOG_H
            
// End of File
