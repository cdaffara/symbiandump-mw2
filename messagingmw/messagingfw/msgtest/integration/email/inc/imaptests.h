// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __IMAPTESTS_H
#define __IMAPTESTS_H

#include "emailsmoke.h"
#include "emailsmokecommands.h"
#include <imapconnectionobserver.h>


//
//
// CImapClientTest
//

class CImapClientTest : public CEmailClientTest
	{
public:
	static CImapClientTest* NewL(CEmailTestUtils& aTestUtils, RTest& aTest);
	virtual ~CImapClientTest();

protected:
	virtual void ConstructL();
	CImapClientTest(CEmailTestUtils& aTestUtils, RTest& aTest);
	};


//
//
// CCreateImapClientMtm
//

class CCreateImapClientMtm : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCreateImapClientMtm(CEmailClientTest& aTest);

private:
	CEmailClientTest& iTest;
	};


//
//
// CCreateImapService
//


class CCreateImapService : public CCreateEmailService
	{
public:
	static CCreateImapService* NewL(CEmailClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);
	~CCreateImapService();
	CImImap4Settings& Settings();

private:
	void ConstructL();
	CCreateImapService(CEmailClientTest& aTestHarness);

private:
	CEmailClientTest& iTestHarness;
	CImImap4Settings* iSettings;
	};



//
//
// CImapUseService
//

class CImapUseService : public CBase, public MBaseTestState
	{
public:
	CImapUseService(CImapClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

private:
	CImapClientTest& iTestHarness;
	};



//
//
// CImapConnect
//

class CImapConnect;
class ImapConnectionObserver : public MMsvImapConnectionObserver
	{
public:
	ImapConnectionObserver(CImapConnect* aImapConnect);
	void HandleImapConnectionEvent(TImapConnectionEvent aConnectionState);
private:
	CImapConnect* iImapConnect;
	TInt iCount;
	};

class CImapConnect : public CActive, public MBaseTestState
	{
public:
	enum TSyncStatus
		{
		ENoSync,
		EConnectAndSync,
		EConnectAndPeriodicSync
		};

	CImapConnect(CEmailClientTest& aParentTestHarness, TSyncStatus aSynchronise = ENoSync);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DummyRun();
	void DoCancel();
	~CImapConnect();

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	TSyncStatus iSynchronise;
	ImapConnectionObserver* iObserver;
	TBool iDummyComplete;
	};


//
//
// CImapConnectAndMonitor
//

class CImapConnectAndMonitor : public CActive, public MBaseTestState, public MMsvImapConnectionObserver
	{
public:
	CImapConnectAndMonitor(CEmailClientTest& aParentTestHarness, TBool aSynchronise = EFalse);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CImapConnectAndMonitor();

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);
	void HandleImapConnectionEvent(TImapConnectionEvent aConnectionEvent);
	void MonitorConnectionL();
private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	TBool iSynchronise;
	TBool iConnectionError;
	};


//
//
// CImapConnectAndCancel
//

class CImapConnectAndCancel : public CActive, public MBaseTestState
	{

public:
	CImapConnectAndCancel(CEmailClientTest& aParentTestHarness, TUint aCancelDelay = 500);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CImapConnectAndCancel();

	// What operation is currently in progress
	enum TCommandState
		{
		EFirstConnect,
		EDisconnect,
		ESecondSync,
		ESecondSyncCancelled,
		EThirdSync,
		EComplete
		};

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);
	void CancelOperation();
private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	CMsvEntrySelection* iSelection;
	TUint iCancelDelay;
	TInt iCommandState;
	};


//
//
// CImapDisconnect
//

class CImapDisconnect : public CActive, public MBaseTestState
	{
public:
	CImapDisconnect(CEmailClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CImapDisconnect();

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};



//
//
// CCheckImapOperation
//

class CCheckImapOperation : public CBase, public MBaseTestState
	{
public:
	CCheckImapOperation(TInt aExpectedStatus, CImapClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

private:
	CImapClientTest& iTestHarness;
	TInt iExpectedStatus;
	};

//
//
// CImapSyncTree
//

class CImapSyncTree : public CActive, public MBaseTestState
	{
public:
	CImapSyncTree(CEmailClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CImapSyncTree();

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};


//
//
// CImapInboxNewSync
//

class CImapInboxNewSync : public CActive, public MBaseTestState
	{
public:
	CImapInboxNewSync(CEmailClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CImapInboxNewSync();

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};

//
//
// CImapFolderSync
//

class CImapFolderSync : public CActive, public MBaseTestState
	{
public:
	CImapFolderSync(CEmailClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CImapFolderSync();

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};

//
//
// CImapPerformFullSync
//

class CImapPerformFullSync : public CActive, public MBaseTestState
	{
public:
	CImapPerformFullSync(CEmailClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CImapPerformFullSync();

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};

//
//
// CImapSetSyncLimits
//

class CImapSetSyncLimits : public CBase, public MBaseTestState
	{
public:
	CImapSetSyncLimits(TInt aInboxSyncLimit, TInt aFolderSyncLimit, CImapClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CImapClientTest& iTestHarness;
	TInt iInboxSyncLimit;
	TInt iFolderSyncLimit;
	};

//
//
// CImapSubscribeFolder
//

class CImapSubscribeFolder : public CActive, public MBaseTestState
	{
public:
	CImapSubscribeFolder(TBool aSubscribe, CEmailClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CImapSubscribeFolder();

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	TBool iSubscribe;
	};



//
//
// CImapPopulateFolder
//

class CImapPopulateFolder : public CActive, public MBaseTestState
	{
public:
	CImapPopulateFolder(CEmailClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CImapPopulateFolder();

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};


//
//
// CImapMarkSelectionUnread
//

class CImapMarkSelection : public CBase, public MBaseTestState
	{
public:
	CImapMarkSelection(TBool Unreadflag, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils);
	void StartL(TRequestStatus& aStatus);
	~CImapMarkSelection();

protected:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	
	TBool iUnreadflag;
	
	};

//


//
//
// CImapCheckSelectionUnread
//

class CImapCheckSelectionUnread : public CBase, public MBaseTestState
	{
public:
	CImapCheckSelectionUnread(TInt aTestType, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils);
	void StartL(TRequestStatus& aStatus);
	~CImapCheckSelectionUnread();

protected:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	
	TInt iTestType;
	};


//
//
// CImapCheckSelectionSeen
//

class CImapCheckSelectionSeen : public CBase, public MBaseTestState
	{
public:
	CImapCheckSelectionSeen(TInt aTestType, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils);
	void StartL(TRequestStatus& aStatus);
	~CImapCheckSelectionSeen();

protected:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	
	TInt iSeenflag;
	};

//
//
// CImapMarkSelectedMail
//

class CImapMarkSelectedMail : public CBase, public MBaseTestState
	{
public:
	CImapMarkSelectedMail(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils);
	void StartL(TRequestStatus& aStatus);
	~CImapMarkSelectedMail();

protected:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	
	
	};

//
//
// CImapCheckSelectedMail
//

class CImapCheckSelectedMail : public CBase, public MBaseTestState
	{
public:
	CImapCheckSelectedMail(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils);
	void StartL(TRequestStatus& aStatus);
	~CImapCheckSelectedMail();

protected:
void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	
	CMsvEntrySelection& iSelection;	
	};

//
//
// CImapSetSyncRate 
//

class CImapSetSyncRate : public CBase, public MBaseTestState
	{
public:
	CImapSetSyncRate(TInt aSyncRate, CImapClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

private:
	CImapClientTest& iTestHarness;
	TInt iSyncRate;
	};

//
//
// CImapSetSyncRate 
//

class CImapGetSyncRate : public CBase, public MBaseTestState
	{
public:
	CImapGetSyncRate(TInt aSyncRate, CImapClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

private:
	CImapClientTest& iTestHarness;
	TInt iSyncRate;
	};




#endif
