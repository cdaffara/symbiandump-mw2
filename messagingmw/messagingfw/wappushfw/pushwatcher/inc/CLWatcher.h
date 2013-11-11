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
// WAP Push Message Header File - 
// This file contains the classes for Concrete Secure/Unsecure CL Watcher and connectionless Watcher Base Class
// 
//

/**
 @file CLWatcher.H
*/

#ifndef __CLWATCHER_H__
#define __CLWATCHER_H__

// System Include
#include <e32base.h>
#include <push/pushlog.h>
#include "cmanobserver.h"
#include <push/murilistlookup.h>

// User Include
// User Include
#include "errorcodes.h"
// Forward Declares
class CWapPushMessageFilter;
class CWapBoundCLPushService;

/**
 * KRecieveBufferSize
 *
 *	Pre-allocated buffer size for recieving a Push Message.  
 */
const TInt KRecieveBufferSize=256;  


/**
 * PushPorts
 */
const Wap::TPort KPushPortUnsecure = 2948; 
const Wap::TPort KPushPortSecure = 2949; 


/**
 *		Connectionless Watcher Base Class
 *		Connects to the WapStack, waits for a Push Message, dispatches it, waits
 */
//##ModelId=3B659EEA027E
class CCLWatcherBase : public CActive
/**
@internalComponent
@released
*/
	{
public:
	//##ModelId=3B659EEB0009
	virtual ~CCLWatcherBase();


	enum TCLWatcherBasePanic 
		{
		ENoMessageExists,
		EUnknownState
		};

protected:
	//##ModelId=3B659EEA03CB
	CCLWatcherBase(MWapPushLog& aLog, MConnManObserver& aManager); 
	
	//##ModelId=3B659EEA03CA
	void ConstructL();

	// Called by ConstructL()
	// Override and use for initialization
	//##ModelId=3B659EEA03C9
	virtual void SetupCLWatcherL() = 0;

	// CActive 
	//##ModelId=3B659EEA03BF
	void RunL();
	//##ModelId=3B659EEA03AD
	TInt RunError(TInt aError);
	//##ModelId=3B659EEA03AC
	void DoCancel();
	
	//##ModelId=3B659EEA03AB
	void IdleComplete();

	//##ModelId=3B659EEA0398
	void Panic(TCLWatcherBasePanic aPanicCode);

private:

	//##ModelId=3B659EEA0397
	void WaitForPushL();
	//##ModelId=3B659EEA038F
	void ReceivePushL();
	//##ModelId=3B659EEA038E
	void DispatchMessageL();
	//##ModelId=3B659EEA038D
	void FilterMessageL();

protected:	

	enum TCLWatcherState 
		{ 
		EWaiting, 
		EReceiving,
		EFiltering, 
		EDispatching
		} iState;
	

	//##ModelId=3B659EEA037B
	MWapPushLog&				iLog;
	//##ModelId=3B659EEA0365
	MConnManObserver&			iManager;

	//##ModelId=3B659EEA033F
	CWapBoundCLPushService		*iWapCLServ;

	//##ModelId=3B659EEA030D
	TBuf8<KRecieveBufferSize>	iBodyBuf;
	//##ModelId=3B659EEA02F9
	TBuf8<KRecieveBufferSize>	iHeadersBuf;
	//##ModelId=3B659EEA02D8
	HBufC8*						iBody;
	//##ModelId=3B659EEA02CE
	HBufC8*						iHeaders;
	//##ModelId=3B659EEA02BC
	TPckgBuf<TUint8>			iPushID;
	//##ModelId=3B659EEA02B2
	TBool						iCachedFilter;
	//##ModelId=3B659E71016E
	CWapPushMessageFilter*		iPushMsgFilter;
	//##ModelId=3B659E710164
	TBool						iPassedFilter;

	//##ModelId=3B659EEA0292
	CPushMessage*				iCurrentMessage;

	HBufC8* iServerAddress;
	
private: 
	MUriListLookup* iUriListLookup;
	};

/**
 * Concrete Unsecure CL Watcher
 */
//##ModelId=3B659EE401D5
class CCLUnsecureWatcher : public CCLWatcherBase
/**
@internalComponent
@released
*/
	{
public:
	//##ModelId=3B659EE401EF
	static CCLUnsecureWatcher* NewL(MWapPushLog& aLog, MConnManObserver& aManager);
	//##ModelId=3B659EE401EE
	~CCLUnsecureWatcher();

protected:
	//##ModelId=3B659EE401ED
	virtual void SetupCLWatcherL();

private:
	//##ModelId=3B659EE401EA
	CCLUnsecureWatcher(MWapPushLog& aLog,MConnManObserver& aManager) : CCLWatcherBase(aLog,aManager){};
	};

/**
 * Concrete Secure CL Watcher
 */
//##ModelId=3B659EE00388
class CCLSecureWatcher : public CCLWatcherBase
/**
@internalComponent
@released
*/
	{
public:
	//##ModelId=3B659EE003A1
	static CCLSecureWatcher* NewL(MWapPushLog& aLog, MConnManObserver& aManager);
	//##ModelId=3B659EE003A0
	~CCLSecureWatcher();

protected:
	//##ModelId=3B659EE0039F
	virtual void SetupCLWatcherL();

private:
	//##ModelId=3B659EE0039C
	CCLSecureWatcher(MWapPushLog& aLog,MConnManObserver& aManager) : CCLWatcherBase(aLog,aManager){};
	};


#endif
