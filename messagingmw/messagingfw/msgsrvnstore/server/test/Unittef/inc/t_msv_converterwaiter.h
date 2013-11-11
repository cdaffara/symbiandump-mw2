// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#include <test\tefunit.h>
#include <msvstd.h>
#include "cmsvconverterwaiter.h"
#include "msvdbadapter.h"
#include "MSVSERV.H"

class CSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
public:
	TMsvSessionEvent iEvent;
	};



class CMyActiveWaiter : public CActive
	{
public:
	CMyActiveWaiter();
	~CMyActiveWaiter();
	
	void WaitActive();
	void SetActiveDontWait();

public:
	// from CActive;
	void RunL();
	void DoCancel();
	
private:
	CActiveSchedulerWait iScheduler;
	CActiveScheduler iWaiter;
	};
	
	
	

/**
@internalTechnology
@prototype
*/
class CTestMessageConverterWaiter : public  CActiveTestFixture, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	CTestMessageConverterWaiter();
	virtual ~CTestMessageConverterWaiter();
	void SetupL();
	void TearDownL();
	void SetupTest();
	
	void TestPerformanceConvertMessageStoreL();
	void TestCheckServerStartUpNotification();
	void TestGetUnsupportedDriveListL();
	void TestStartConverterWaiterL();
	void TestConvertMessageStoreL();
	void TestDeleteMessagingDBL();
	void TestCancelConversionL();
	void TestInstanceL();
	void TestValidateDriveInPreferredDriveListL();
	void TestValidateRequestedDriveL();
	void TestCancelVersion0Version1ConversionL();
	void TestCancelVersion1Version2ConversionL();
	void TestCancelVersion1Version2NewConversionL();
			
	static CTestSuite* CreateSuiteL(const TDesC& aName);

private:
	
	CMyActiveWaiter* iActiveWaiter;
	CMyActiveWaiter* iActiveWaiter2;
	CMyActiveWaiter* iActiveWaiter3;
	CMyActiveWaiter* iActiveWaiter4;
	CMsvServer* iServer;
	TMsvSessionEvent iEvent;

friend  class CMyActiveWaiter;	
	};
	 

