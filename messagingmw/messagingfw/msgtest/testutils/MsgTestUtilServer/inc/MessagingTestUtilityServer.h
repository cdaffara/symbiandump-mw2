// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __MESSAGINGTESTUTILITYSERVER_H__
#define __MESSAGINGTESTUTILITYSERVER_H__

#include <e32base.h>
#include <f32file.h>
#include "messagingtestutility.h"


enum TTestPanic
	{
	ETestPanicBadDescriptor,
	ETestPanicIllegalFunction,
	ETestPanicAlreadyReceiving
	};

void PanicClient(RMessage2& aMessage,TTestPanic TMyPanic);

class CShutdown : public CTimer
	{
	enum {KMyShutdownDelay=0x200000};	// approx 2s
public:
	inline CShutdown();
	inline void ConstructL();
	inline void Start();
private:
	void RunL();
	};


class CMessagingTestUtilityServer : public CServer2
	{
public:
	static CServer2* NewLC();
	void AddSession();
	void DropSession();
private:
	CMessagingTestUtilityServer();
	void ConstructL();
	CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;
private:
	TInt iSessionCount;
	CShutdown iShutdown;
	};


class CMessagingTestUtilitySession : public CSession2
	{
public:
	CMessagingTestUtilitySession();
	void CreateL();
private:
	~CMessagingTestUtilitySession();
	inline CMessagingTestUtilityServer& Server();
	void ServiceL(const RMessage2& aMessage);
	void ServiceError(const RMessage2& aMessage,TInt aError);
	inline TBool ReceivePending() const;
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
private:
	RFs iFs;
	CDir* iDir;
	};

#endif //__MESSAGINGTESTUTILITYSERVER__
