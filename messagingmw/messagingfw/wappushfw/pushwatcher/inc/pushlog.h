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

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __PUSHLOG_H__
#define __PUSHLOG_H__


/** 
Logs a literal string specified in a, assuming iLog is an MWapPushLog object. 
*/
#define __LOG_ALWAYS(a) {_LIT(name,a); iLog.WPLPrintf(name);} 

/** 
Logs a literal string specified in a, assuming iLog is a pointer to a MWapPushLog 
object. 
*/
#define __LOG_PTR_ALWAYS(a) {_LIT(name,a); if(iLog) iLog->WPLPrintf(name);} 

/** 
Logs a literal string specified in a, assuming aLog is a MWapPushLog object. 
*/
#define __LOG_PAR_ALWAYS(a) {_LIT(name,a); aLog.WPLPrintf(name);}

/** 
Logs an error descriptor and integer error code specified in name and a respectively, 
assuming iLog is a MWapPushLog object. 
*/
#define __LOG_ERROR_ALWAYS(a,b) {_LIT(name,a); iLog.WPLLogError(name,b);}


#if defined(_DEBUG)
/** 
Logs, in debug builds only, a literal string specified in a, assuming iLog 
is an MWapPushLog object. 
*/
#define __LOG_DEBUG(a) __LOG_ALWAYS(a) 

/** 
Logs, in debug builds only, a literal string specified in a, assuming iLog 
is a pointer to a MWapPushLog object. 
*/
#define __LOG_PTR_DEBUG(a) __LOG_PTR_ALWAYS(a) 

/** 
Logs, in debug builds only, a literal string specified in a, assuming aLog 
is a MWapPushLog object. 
*/
#define __LOG_PAR_DEBUG(a) __LOG_PAR_ALWAYS(a)

/** 
Logs, in debug builds only, a CPushMessage specified in a, assuming iLog is 
an MWapPushLog object. 
*/
#define __LOG_MSG_DEBUG(a) iLog.WPLPrintfL(a)

/** 
Logs, in debug builds only, an error descriptor and integer error code specified 
in name and a respectively, assuming iLog is a MWapPushLog object. 
*/
#define __LOG_ERROR_DEBUG(a,b) __LOG_ERROR_ALWAYS(a,b)
#else
/** Logs always. iLog is an MWapPushLog object*/
#define __LOG_DEBUG(a) 
/** Logs always. iLog is an MWapPushLog pointer*/
#define __LOG_PTR_DEBUG(a) 
/** Logs always. iLog is an MWapPushLog object*/
#define __LOG_PAR_DEBUG(a) 
/** Logs always. iLog is CPushMessage specified in MWapPushLog object*/
#define __LOG_MSG_DEBUG(a)
/** Logs error in debug mode */
#define __LOG_ERROR_DEBUG(a,b) 


#endif


class CPushMessage;


/** 
Abstract WAP Push log access interface: logging is primarily for debugging.

The standard Symbian OS WAP Push watcher component writes to a log at c:\\logs\\watcher\\watcher.txt 
if the c:\\logs\\watcher\\ directory exists. It supplies this interface to push 
plug-ins through CPushHandlerBase::SetLogger(). The plug-in can then call 
the interface's functions to add its own messages to the log. 

@publishedPartner
@released
*/
class MWapPushLog
	{
public:
	/** 
	Writes a string to the log.
	
	@param aDescription 
	String to log 
	*/
	virtual void WPLPrintf(const TDesC& aDescription)=0;
	
	/** 
	Writes a push message to the log.
	
	It writes the message's content-type, date, expiry, and application-ID headers 
	as text, and the complete headers and message body in binary form.
	
	@param aMessage 
	Push message to log 
	*/
	virtual void WPLPrintfL(CPushMessage& aMessage)=0;
	
	/** 
	Writes a binary buffer to the log.
	
	The buffer is written as six hexadecimal bytes per line: e.g.
	
	@code
	AB CD 01 12 34 A2
	FF 00 AB CD 12 DE
	@endcode
	
	@param aDescription 
	Binary buffer to log 
	*/
	virtual void WPLLogBinaryAsHex(const TDesC& aDescription)=0;
	
	/** 
	Writes an error message and code to the log.
	
	@param aDescription 
	Error message
	
	@param aError 
	Error code 
	*/
	virtual void WPLLogError(const TDesC& aDescription,TInt aError)=0;
	};


#endif
