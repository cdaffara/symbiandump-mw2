// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <e32cons.h>
#include <flogger.h>
#include <e32svr.h>
#include <push/pushmessage.h>
#include <push/pushlog.h>
#include "testlog.h"

_LIT(KPushLogDir, "push");
_LIT(KPushLogFile, "WapPushLogging.txt");
const TInt KWatcherLogBuffer = 256;


/**
* CWapPushLog::NewL
* 
*/
CWapPushLog* CWapPushLog::NewL(CConsoleBase& aConsole)
	{
	CWapPushLog* self = new(ELeave)CWapPushLog(aConsole);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CWapPushLog::CWapPushLog(CConsoleBase& aConsole) : iConsole(aConsole)
	{
	}


/**
* Destructor
*/
CWapPushLog::~CWapPushLog()
	{
	}

void CWapPushLog::ConstructL()
	{
	}

/**
* Printf
* 
* @param	aFmt	
*			Printf formatting rules
*/
void CWapPushLog::WPLPrintf(const TDesC& aDescription)
	{
	// Write to log file

	if (iLogFilename.Length())
		RFileLogger::Write(KPushLogDir, iLogFilename, EFileLoggingModeAppend, aDescription);
	else
		RFileLogger::Write(KPushLogDir, KPushLogFile, EFileLoggingModeAppend, aDescription);

	// Write to console
	_LIT(KPushLogFmt,"Push:%S\n");
	TPtrC	buf = aDescription.Left(Min(KWatcherLogBuffer, aDescription.Length()));
	iConsole.Printf(KPushLogFmt,&buf);
	}

/**
 * Takes the data in a Push Message and prints it to console and the logs it to file.
 * Uses the accessor functions of the CPushMessage class and this classes Printf function 
 * to perform this.  Currently 4 headers and the message body are displayed and 
 * logged. The headers are:
 *		PushID, Content-Type, X-Application-ID, Expiry Date, 
 *
 *	@param	CPushMessage& aMessage
 *				in:  a reference to a Push Message.
 */
void CWapPushLog::WPLPrintfL(CPushMessage& aMessage)
	{
	const TInt KWatcherLogBuffer=256;
	TBuf<KWatcherLogBuffer> buf;
	
	// Content type
	_LIT(KLogContentFmt,"Content Type : \t%S");
	TPtrC contentPointer;
	aMessage.GetContentType(contentPointer);
	buf.Format(KLogContentFmt,&contentPointer);
	buf.Append('\n');
	if( buf.Length() > 100 )
		{
		// Flogger has a max of 150 (inc time stamp, and max buf is 256, if data is larger than 100 split onto
		// 2 lines
		WPLPrintf(buf.Left(100)); // Print first 150 chars
		WPLPrintf(buf.Mid(99));  // Print the rest
		}
	else
		{
		WPLPrintf(buf);
		}

	// Date Field
	_LIT(KLogDateTimeFmt," %-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3");
	_LIT(KLogDateFmt,"Date           :\t%S");
	TBool foundField;
	TTime timeValue;
	foundField = aMessage.GetHeaderField(EHttpDate, timeValue);
	if (foundField)
		{
		TRAPD(err, timeValue.FormatL(buf, KLogDateTimeFmt));
		if (err == KErrNone)
			{
			TBuf<KWatcherLogBuffer> dateBuf;
			dateBuf.Format(KLogDateFmt, &buf);
			WPLPrintf(dateBuf);
			}
		}

	//Expires Field
	aMessage.GetHeaderField(EHttpExpires, timeValue);
	if (foundField)
		{
		TRAPD(err, timeValue.FormatL(buf, KLogDateTimeFmt));
		if (err == KErrNone)
			{
			TBuf<KWatcherLogBuffer> dateBuf;
			dateBuf.Format(KLogDateFmt, &buf);
			WPLPrintf(dateBuf);
			}
		}

	// X-Wap-Application-ID
	TBool isInt = EFalse;
	TInt ID = 0;
	TPtrC8 generalPtr;
	_LIT(KLogAppIdIntFmt,"App ID         :\t%X");
	TRAPD( error, foundField = aMessage.GetAppIdL(generalPtr, ID, isInt) );
	if (error == KErrNone && foundField) // App ID is present
		{
		if (isInt)  // Field in integer format
			{
			buf.Format(KLogAppIdIntFmt,ID);
			}
		else  // should be descriptor format
			{
			buf.Copy(generalPtr);
			}
		WPLPrintf(buf);
		}

	// Server Address 
	TPtrC8 serverAddr;
	_LIT(KLogServerAddressIntFmt,"Server Address : ");
	foundField = aMessage.GetServerAddress(serverAddr);
	if (foundField) // Server Address is present
		{
		buf.Copy(serverAddr);
		buf.Insert( 0, KLogServerAddressIntFmt); 
		WPLPrintf(buf);
		}

	// Push ID 
	TUint8 pushID=0;
	_LIT(KLogPushIDIntFmt,"PushID         : %d");
	aMessage.GetPushId(pushID);
	buf.Format(KLogPushIDIntFmt,pushID);
	WPLPrintf(buf);

	//Message Header Binary
	_LIT(KLogMsgHdr,"Header Binary:");
	WPLPrintf(KLogMsgHdr);
	aMessage.GetHeader(generalPtr);
	
	HBufC* tempHdr = HBufC::NewLC(generalPtr.Length());
	tempHdr->Des().Copy(generalPtr);
	WPLLogBinaryAsHex(*tempHdr);
	CleanupStack::PopAndDestroy(); //tempHdr

	//Message Body
	aMessage.GetMessageBody(generalPtr);

	// Dump Body As Text
	_LIT(KLogMsgBody,"Body Text:");
	WPLPrintf(KLogMsgBody);
	HBufC* tempBody = HBufC::NewLC(generalPtr.Length());
	tempBody->Des().Copy(generalPtr);
	WPLPrintf(*tempBody);

	// Dump Body As Hex
	_LIT(KBodyBinary,"\nBody Binary:");
	WPLPrintf(KBodyBinary);
	WPLLogBinaryAsHex(*tempBody);

	CleanupStack::PopAndDestroy(); //tempBody
	}
 

 /**
 * Prints out Buffer data in the format:
 *	%X %X %X %X %X %X %X\n etc 
 * For example
 *	AB CD 01 12 34 A2 
 *
 *	@param	aDescription
 *				in:  the descriptor to be dumped into the log
 */
void CWapPushLog::WPLLogBinaryAsHex(const TDesC& aDescription)
	{
	const TInt KWatcherLogBuffer=256;
	_LIT(KHexSpace,"%02X ");
	TBuf<KWatcherLogBuffer> hexBuf;
	TBuf<KWatcherLogBuffer> buf;

	TInt i = 0, bodyLen = aDescription.Length();

	for (; i < bodyLen; i++)
		{
		hexBuf.Format(KHexSpace,aDescription[i]);
		buf.Append(hexBuf);
		if ( i && ((i+1) % 8) == 0  )
			{
			WPLPrintf(buf);
			buf.Zero();
			}
		}

	if (buf.Length())
		WPLPrintf(buf);
	}
void CWapPushLog::WPLLogError(const TDesC& aDescription,TInt aError)
	{
	_LIT(KErrorLogFmt,"Push: %S, Error\t%d");
	const TInt KWatcherLogBuffer=256;
	TBuf<KWatcherLogBuffer> buf;
	buf.Format(KErrorLogFmt,aDescription,aError);
  	buf.Append('\n');
	WPLPrintf(buf);
	}


void CWapPushLog::SetLogFileName(const TFileName aFilename)
	{
	iLogFilename=aFilename;
	}

const TDesC& CWapPushLog::LogFileName() const
	{
	if (iLogFilename.Length())
		return iLogFilename;
	else
		return KPushLogFile;
	}
