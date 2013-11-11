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

#ifndef __TESTLOG_H__
#define __TESTLOG_H__


#include <push/pushlog.h>

/**
 * CWapPushLog
 *	A logging utility, primarily for debugging.  
 *  If  c:\logs\push\ is on the device, a log file will be created.
 *  The log will also be displayed to the console.  
 *	This is useful for debugging UI less CActive objects.
 *		
 */
class CWapPushLog : public CBase, public MWapPushLog
	{
public:
	static CWapPushLog* NewL(CConsoleBase& aConsole);
	~CWapPushLog();

	// Set the filename, default is KPushLogFile
	void SetLogFileName(const TFileName aFilename);
	const TDesC& LogFileName() const;

public:
	//from MWapPushLog
	virtual void WPLPrintf(const TDesC& aDescription);
	virtual void WPLPrintfL(CPushMessage& aMessage);
	virtual void WPLLogBinaryAsHex(const TDesC& aDescription);
	virtual void WPLLogError(const TDesC& aDescription,TInt aError);

protected:
	void ConstructL();
	CWapPushLog(CConsoleBase& aConsole);

private:
	TFileName	iLogFilename;
	CConsoleBase& iConsole;
	};
#endif
