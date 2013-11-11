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
// WAP Push - ConnectionManager Test Harness
// 
//

#ifndef __T_FAILURE_H__
#define __T_FAILURE_H__

//
//          CFailureTester: INTERFACE          //
//

class CPushMessageTester : public CBase
	{
public:
	static CPushMessageTester* NewL();
	~CPushMessageTester();
	
	void FailureTestL();
	void PluginFailureTestL();
private:
	CPushMessageTester();
private:
	HBufC8*	iBody;
	HBufC8* iHeader;
	CPushMessage* iMessage;
	};

class CFailureTester : public CBase
	{
public:
	static CFailureTester* NewL(CWapPushLog& aWapLog);
	~CFailureTester();
	
	void HeapFailureTest();
	void InitSchedulerL();
	void PluginHeapFailureTest();
private:
	CFailureTester(CWapPushLog& aWapLog);
	CPushMessageTester* DoPushMessageTestL();
	CPushMessage* DoPushMessageTest2L();
	CPushMessageTester* CreatePushHandlerBaseL();
	
	//Logging functions
	void LogStr(const TDesC& aDes);
	void LogFormatInt(const TDesC& aFormat, TInt aInt);
	void LogFormatInt(const TDesC& aFormat, TInt aInt1, TInt aInt2);
	void LogFormatStr(const TDesC& aFormat, const TDesC& aDes);

private:
	CWapPushLog& iLog;  //Don't own this
	TBuf<256> iFormatBuffer;
	CActiveScheduler	*iScheduler;
	};

#endif
