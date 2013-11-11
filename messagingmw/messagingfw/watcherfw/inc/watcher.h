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
// watcher.h
//
/**
 * @file 
 * @publishedPartner
 * @released
 */

#ifndef __WATCHER_H__
#define __WATCHER_H__

#include <e32base.h>
#include <f32file.h>

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "cwatcher.h"
#endif

/** 
ECom interface UID for non-SMS related critical watchers. 
@publishedPartner
@released
*/
const TUid KUidMsgCriticalWatchers = { 0x1027433D };

/** 
ECom interface UID for non-SMS related non-critical watchers. 
@publishedPartner
@released
*/
const TUid KUidMsgNonCriticalWatchers = { 0x1027433E };

class CWatcherLog;
class RFs;

/** 
ECom interface 
@publishedPartner
@released
*/
struct TWatcherParams
	{
	TWatcherParams(RFs& aFs, CWatcherLog& aLog) : iFs(aFs), iLog(aLog) {}
	//
	RFs&			iFs;
	CWatcherLog&	iLog;			
	};

//**********************************
// CWatcherLog
//**********************************

const TInt KWatcherLogBuffer = 256;
_LIT(KWatcherStringFormat, "%S");

class CWatcherLog : public CBase
/**
@publishedPartner
@released
*/
	{
public:
	static CWatcherLog* NewL(RFs& aFs);
	~CWatcherLog();
	//
	IMPORT_C void Printf(TRefByValue<const TDesC> aFmt,...);
	IMPORT_C void Printf(TRefByValue<const TDesC8> aFmt,...);
	IMPORT_C TBool IsLogging() const;
	//
private:
	CWatcherLog(RFs& aFs);
	void ConstructL();
	TBool IsConsole() const;

	//
private:
	CConsoleBase* iConsole;
	RFs& iFs;
	};

#endif	// __WATCHER_H__
