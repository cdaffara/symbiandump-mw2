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

#ifndef __SMS_H
#define __SMS_H

#include "emailsmoke.h"
#include "sendas.h"
#include <smuthdr.h>
#include <smutset.h>
#include "testframeparse.h"

// sms commands
_LIT(CCommandSmsBaseClientTest,	"sms_base_client_test");	// 1 param, name of sms base section
_LIT(CCommandSmsClientTest,		"sms_client_test");			// 1 param, name of sms section
_LIT(KCommandSmsBegin,			"sms_begin");				// no params
_LIT(KCommandSmsCreateMessage,	"create_sms_message");		// 3+1 params: 
															// param1, subdirectory of text file to be attached
															// param2, file name to be attached
															// param3, what folder to create
															// param4, OPTIONAL how many messages to create
_LIT(KCommandSmsEnd,			"sms_end");					// no params


//
class CSmsBaseUtils : public MMsvSessionObserver
	{
public:
	CSmsBaseUtils();
	virtual ~CSmsBaseUtils();

	virtual void StartL();

	void							SetEntryL(TMsvId	aId);
	CMsvEntry&						EntryL();
	const TMsvSelectionOrdering&	SelectionOrdering() const { return iOrder; }

	//	MMsvSessionObserver
	void	HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};

protected:
	void SetDefaultServiceL(TUid aMtm, TMsvId aService) const;
	TInt DefaultServiceL(TUid aMtm, TMsvId& aService) const;

protected:
	CMsvSession* iSession;
	
private:
	CMsvEntry*				iEntry;
	TMsvSelectionOrdering	iOrder;	
	};


//
class CSmsTestUtils : public CSmsBaseUtils
	{
public:
	CSmsTestUtils() {};
   ~CSmsTestUtils() {};

	void StartL();
	};


//
class CSmsBaseClientTest : public CBaseTestHarness
	{
public:
	static CSmsBaseClientTest* NewL(CSmsTestUtils& aSmsUtils,RTest& aTest);
	static CSmsBaseClientTest* NewL(CSmsTestUtils* aSmsUtils,RTest& aTest);
	virtual ~CSmsBaseClientTest();

	CSmsTestUtils& SmsTestUtils();
	void DoCancel();
	void SetCurrentOperation(CMsvOperation* aOperation);
	CMsvOperation& CurrentOperation();

protected:
	virtual void ConstructL();
	CSmsBaseClientTest(CSmsTestUtils& aSmsUtils,RTest& aTest);
	CSmsBaseClientTest(CSmsTestUtils* aSmsUtils,RTest& aTest);

public:
	CMsvEntrySelection* iCurrentSelection;
	TMsvId iSelectedFolder;
	TMsvId iDestinationFolder;
	TMsvId iCurrentServiceId;

protected:
	CSmsTestUtils& iSmsUtils;
private:
	CSmsTestUtils* iOwnedSmsUtils;
	CMsvOperation* iOperation;
	};


//
class CSmsBaseClientSectionParser : public CBaseSectionParser
	{
public:
	static CSmsBaseClientSectionParser* NewL(CSmsBaseClientTest& aSmsTest, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void LogCommentL(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName);
	CSmsBaseClientSectionParser(CSmsBaseClientTest& aSmsTest, CTestScript& aScript);

private:
	CSmsBaseClientTest& iSmsTest;
	};


//
class CNewSmsBaseTestCommandParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CNewSmsBaseTestCommandParser* NewL(CTestScript& aScript, CMainTestHarness& aMainTest, TBool aKeepLogFile);
private:
	void ConstructL();
	CNewSmsBaseTestCommandParser(CTestScript& aScript, CMainTestHarness& aMainTest, TBool aKeepLogFile);
	CMainTestHarness& iMainTest;
	CTestScript& iScript;
	TBool iKeepLogFile;
	};


//
class CSmsClientTest : public CSmsBaseClientTest
	{
public:
	static CSmsClientTest* NewL(CSmsTestUtils& aSmsUtils, RTest& aTest);
	virtual ~CSmsClientTest();

protected:
	virtual void ConstructL();
	CSmsClientTest(CSmsTestUtils& aSmsUtils, RTest& aTest);
	};


//
class CSmsClientSectionParser : public CSmsBaseClientSectionParser
	{
public:
	static CSmsClientSectionParser* NewL(CSmsClientTest& aSmsTest, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void ConstructL(const TDesC& aNewSectionName);
	CSmsClientSectionParser(CSmsClientTest& aSmsTest, CTestScript& aScript);

private:
	CSmsClientTest& iSmsTest;
	};


//
class CNewSmsTestCommandParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CNewSmsTestCommandParser* NewL(CTestScript& aScript, CSmsBaseClientTest& aSmsTest);
private:
	void ConstructL();
	CNewSmsTestCommandParser(CTestScript& aScript, CSmsBaseClientTest& aSmsTest);
	CSmsBaseClientTest& iSmsTest;
	CTestScript& iScript;
	};


//
class CSmsBaseUtilitiesParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CSmsBaseUtilitiesParser* NewL(CSmsBaseClientTest& aSmsTest);
private:
	void ConstructL();
	CSmsBaseUtilitiesParser(CSmsBaseClientTest& aSmsTest);
	CSmsBaseClientTest& iSmsTest;
	};


//
class CSmsBegin : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CSmsBegin(CSmsTestUtils& aSmsUtils);

protected:
	CSmsTestUtils& iSmsUtils;
	};


//
class CSmsEnd : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CSmsEnd(CSmsTestUtils& aSmsUtils);

protected:
	CSmsTestUtils& iSmsUtils;
	};




//
class CCreateSmsMessage : public CActive, public MBaseTestState, public MSendAsObserver
	{
public:
	CCreateSmsMessage(TMsvId iFolderId, const TMsvId& iServiceId, const TDesC& aFileName, CSmsTestUtils& aSmsUtils,TInt aHowMany=1);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CCreateSmsMessage();
	TBool CapabilityOK(TUid /*aCapability*/, TInt /*aResponse*/)	{ return ETrue; }
	void CreateMessageL();

private:
	enum TCreateSmsState
		{
		ECreateSmsWait1,
		ECreateSmsWait2
		};

	TCreateSmsState iState;
	TMsvId iCurrentFolderId;
	const TMsvId& iServiceId;
	TFileName iFileName;
	CSmsTestUtils& iSmsUtils;
	CTestTimer* iTimer;
	TRequestStatus* iReportStatus;
	TInt iHowMany;
	};


#endif
