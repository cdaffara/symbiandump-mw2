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

#include <watcher.h>
#include <wapmessage.h>

#include "pushwatcher.h"
#include <push/pushlog.h>
#include <push/pushmessage.h>
#include "flogger.h"

#ifdef __WATCHER_API_V2__

#include <ecom/implementationproxy.h>

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x10009145, CPushWatcher::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}
#endif	// __WATCHER_API_V2__

#ifdef __WATCHER_API_V2__
CPushWatcher* CPushWatcher::NewL(TAny* aWatcherParams)
#else
/** single exported constructor to allow creation of push watcher
	which in turn creates Connection Manager and waits for a push
	message to arrive. 
	@param RF
	@param CWatcherLog
	@return pointer to created object
*/
EXPORT_C CPushWatcher* CPushWatcher::NewL(RFs&, CWatcherLog& aLog)
#endif	// __WATCHER_API_V2__
	{
#ifdef __WATCHER_API_V2__
	TWatcherParams* params = reinterpret_cast<TWatcherParams*>(aWatcherParams);
	CPushWatcher* self= new (ELeave) CPushWatcher(CActive::EPriorityStandard, params->iLog);
#else
	CPushWatcher* self = new(ELeave) CPushWatcher(CActive::EPriorityStandard, aLog);
#endif	// __WATCHER_API_V2__
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

/** private constructor */
CPushWatcher::CPushWatcher(TInt aPriority, CWatcherLog& aLog)
: CActive(aPriority), iLog(aLog)
	{
	}

/** destructor to cancel any outstanding requests */
CPushWatcher::~CPushWatcher()
	{
	Cancel();
	}

/** start scheduler and create connection manager, then wait */
void CPushWatcher::ConstructL()
	{
	CActiveScheduler::Add(this);
	iLog.Printf(_L("PushWatcher: Constructing..."));
	iConnMan = CConnectionManager::NewL(*this);
	}

/** watcher starts connection manager and just idles, no other states or
	activities so RunL is empty.

	The Connection Manager processes any push messages. */
void CPushWatcher::RunL()
	{
	// nothing to do
	}

/** no active RunL so no Cancel required */
void CPushWatcher::DoCancel()
	{
	// nothing to do
	}






/** log debug information tp c:\logs\watcher directory if present
	after formatting message to print.
	@param list of args
	@return void 
*/
void CPushWatcher::WPLPrintf(const TDesC& aDescription)
	{
	_LIT(KPushLogFmt,"Push:%S");

	// 7 is the length of "Push:" from above + '\n' tagged on in the Printf()
	TPtrC	buf = aDescription.Left(Min(KWatcherLogBuffer-7, aDescription.Length()));
	iLog.Printf(KPushLogFmt, &buf);
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
void CPushWatcher::WPLPrintf(CPushMessage& aMessage)
	{
	TRAPD(error, WPLPrintfL(aMessage));
	if (error != KErrNone)
		{
		_LIT(KErrMessage, "Wap Push Watcher logging failed");
		WPLPrintf(KErrMessage);
		}
	}

/**
 * Private WPLPrint leaving method
 */
void CPushWatcher::WPLPrintfL(CPushMessage& aMessage)
	{
	TBuf<KWatcherLogBuffer> buf;
	
	// Content type
	_LIT(KLogContentFmt,"Content Type : \t%S");
	TPtrC contentPointer;
	aMessage.GetContentType(contentPointer);
	// max length, including the post-append '\n'
	TInt maxSize(KWatcherLogBuffer - (KLogContentFmt().Length()+1));
	TPtrC tmp(contentPointer.Left(maxSize));
	buf.Format(KLogContentFmt,&tmp);
	buf.Append('\n');
	WPLPrintf(buf);

	// Date Field
	_LIT(KLogDateTimeFmt," %-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3");
	_LIT(KLogDateFmt,"Date           :\tS");
	TBool foundField;
	TTime timeValue;
	foundField = aMessage.GetHeaderField(EHttpDate, timeValue);
	if (foundField)
		{
		timeValue.FormatL(buf, KLogDateTimeFmt);
		buf.Format(KLogDateFmt, &buf);
		WPLPrintf(buf);
		}

	//Expires Field
	foundField = aMessage.GetHeaderField(EHttpExpires, timeValue);
	if (foundField)
		{
		timeValue.FormatL(buf, KLogDateTimeFmt);
		buf.Format(KLogDateFmt, &buf);
		WPLPrintf(buf);
		}

	// X-Wap-Application-ID
	TBool isInt;
	TInt ID;
	TPtrC8 generalPtr;
	_LIT(KLogAppIdIntFmt,"App ID         :\t%X");
	_LIT(KLogAppIdStringFmt,"App ID         :\t%S");
	foundField = aMessage.GetAppIdL(generalPtr, ID, isInt);
	if (foundField) // App ID is present
		{
		if (isInt)  // Field in integer format
			{
			buf.Format(KLogAppIdIntFmt,ID);
			}
		else  // should be descriptor format
			{
			buf.Format(KLogAppIdStringFmt,&generalPtr);
			}
		WPLPrintf(buf);
		}

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
	_LIT(KBodyBinary,"Body Binary:");
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
void CPushWatcher::WPLLogBinaryAsHex(const TDesC& aDescription)
	{
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



/** log error description to debug file */
void CPushWatcher::WPLLogError(const TDesC& aDescription,TInt aError)
	{
	_LIT(KErrorLogFmt,"Push:%S, Error\t%d");
	const TInt KWatcherLogBuffer=256;
	TBuf<KWatcherLogBuffer> buf;
	buf.Format(KErrorLogFmt,&aDescription,aError);
  	buf.Append('\n');
	WPLPrintf(buf);
	}
 
