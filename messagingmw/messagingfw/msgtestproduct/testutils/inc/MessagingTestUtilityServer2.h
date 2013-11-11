/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/



/**
 @file
*/

#ifndef __MESSAGINGTESTUTILITYSERVER2_H__
#define __MESSAGINGTESTUTILITYSERVER2_H__

#include <e32base.h>
#include <f32file.h>
#include <messagingtestutility2.h>
#include "cmsgtestpropertymappair.h"
#include "mmsgtestpropertywatcher.h"

class CMsgTestPropertyWatcher;

/**
Enumerations for panic code
*/
enum TTestPanic
	{
	ETestPanicBadDescriptor,
	ETestPanicIllegalFunction,
	ETestPanicAlreadyReceiving
	};

void PanicClient(RMessage2& aMessage,TTestPanic TMyPanic);


/**
Shuts down a server
*/
class CShutdown : public CTimer
	{
	enum {KMyShutdownDelay = 0x200000};	// approx 2s
public:
	inline CShutdown();
	inline void ConstructL();
	inline void Start();

private:
	void RunL();
	};


/**
An utility server that handles the sessions and requests from the client
*/
class CMessagingTestUtilityServer2 : public CServer2
	{
public:
	static CServer2* NewLC();
	void AddSession();
	void DropSession();

private:
	CMessagingTestUtilityServer2();
	void ConstructL();
	CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;

private:
	TInt iSessionCount;
	CShutdown iShutdown;
	};


/**
Perfroms the file system operations
*/
class CMessagingTestUtilitySession2 : public CSession2, public MMsgTestPropertyWatcher
	{
public:
	CMessagingTestUtilitySession2();
	void CreateL();
private:
	~CMessagingTestUtilitySession2();
	inline CMessagingTestUtilityServer2& Server();
	void ServiceL(const RMessage2& aMessage);
	void ServiceError(const RMessage2& aMessage,TInt aError);
	inline TBool ReceivePending() const;
	
	//From MMsgTestPropertyWatcher
	void HandleEventL(RProperty& aProperty, TUint /*aKey*/);
	
private:
	TInt DoDeleteMessageStoreL(const RMessage2& aMessage);
	TInt DoKillProcessL(const RMessage2& aMessage);
	TInt DoCopyFileL(const RMessage2& aMessage);
	TInt DoDeleteFileL(const RMessage2& aMessage);
	TInt DoMkDirL(const RMessage2& aMessage);
	TInt DoRmDirL(const RMessage2& aMessage);
	TInt DoGetDirL(const RMessage2& aMessage);
	TInt DoGetDirCountL(const RMessage2& aMessage);
	TInt DoGetDirEntryL(const RMessage2& aMessage);
	TInt DoFileExistsL(const RMessage2& aMessage);
	
	//EventHandler
	TInt DoEventHandlerInitL();
	TInt DoEventHandlerAddL(const RMessage2& aMessage);
	TInt DoEventHandlerCheck();
	
private:
	RFs iFs;
	CDir* iDir;

	CMsgTestPropertyWatcher* iWatcherNetTestStub;
	RPointerArray<CMsgTestPropertyMapPair> iEventList;
	};

#endif //__MESSAGINGTESTUTILITYSERVER2__
