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


#include <push/pushlog.h>
#include "ConnMan.h"


// Classes referenced
class RFs;
class CWatcherLog;


/** 
 * Real watcher
 * A watcher interface conformant class which has one NewL export called by 
 * the watcher framework to create the watcher. It creates the connection
 * manager and provides an adapter for our logging interface (MWapPushLog) 
 * to the log provided by the watcher framework
 */

//##ModelId=3B659E6F0105
class CPushWatcher : public CActive, public MWapPushLog
/**
@internalTechnology
@released
*/
	{
public:
#ifdef __WATCHER_API_V2__
	static CPushWatcher* NewL(TAny* aWatcherParams);
#else
	// Watcher Plugin Signature
	//##ModelId=3B659E6F0156
	IMPORT_C static CPushWatcher* NewL(RFs& aFs, CWatcherLog& aLog);
#endif	// __WATCHER_API_V2__
	//##ModelId=3B659E6F0155
	~CPushWatcher();

	// from MWapPushLog
	//##ModelId=3B659E6F014B
	virtual void WPLPrintf(const TDesC& aDescription);
	//##ModelId=3B659E6F014D
	virtual void WPLPrintf(CPushMessage& aMessage);
	//##ModelId=3B659E6F0142
	virtual void WPLLogBinaryAsHex(const TDesC& aDescription);
	//##ModelId=3B659E6F0138
	virtual void WPLLogError(const TDesC& aDescription,TInt aError);
private:
	//##ModelId=3B659E6F012F
	CPushWatcher(TInt aPriority, CWatcherLog& aLog);
	//##ModelId=3B659E6F012E
	void ConstructL();
	
	//from CActive
	//##ModelId=3B659E6F012D
	void RunL();
	//##ModelId=3B659E6F0126
	void DoCancel();
	void WPLPrintfL(CPushMessage& aMessage);

private:
	//##ModelId=3B659E6F0123
	CWatcherLog& iLog;
	//##ModelId=3B659E6F0113
	CConnectionManager* iConnMan;
	}; 

