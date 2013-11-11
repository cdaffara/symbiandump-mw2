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

#include "sms.h"

//
CSmsBaseUtils::CSmsBaseUtils()
	:iSession(NULL), iEntry(NULL), iOrder(KMsvGroupByType|KMsvGroupByStandardFolders,EMsvSortById,ETrue)
	{
	}


CSmsBaseUtils::~CSmsBaseUtils()
	{
	delete iEntry;
	iEntry=NULL;
	delete iSession;
	iSession=NULL;
	}


void CSmsBaseUtils::StartL()
	{
	iSession=CMsvSession::OpenSyncL(*this);
	}


CMsvEntry& CSmsBaseUtils::EntryL()
	{
	if ( iEntry == NULL )
		{
		User::Leave(KErrGeneral);
		}
	return *iEntry;
	}


void CSmsBaseUtils::SetEntryL(TMsvId aId)
	{
	delete iEntry;
	iEntry=NULL;
	iEntry=iSession->GetEntryL(aId);
	}


void CSmsBaseUtils::SetDefaultServiceL(TUid aMtm,TMsvId aService) const
	{
	CMsvEntry* rootEntry=iSession->GetEntryL(KMsvRootIndexEntryIdValue);
	CleanupStack::PushL(rootEntry);
	CMsvStore* store = rootEntry->EditStoreL();
	CleanupStack::PushL(store);
	CMsvDefaultServices* services = new(ELeave)CMsvDefaultServices;
	CleanupStack::PushL(services);
	services->RestoreL(*store);

	TMsvDefaultService	defaultService;
	defaultService.iMtm=aMtm;
	defaultService.iService=aService;
	services->ChangeDefaultServiceL(defaultService);
	services->StoreL(*store);
	store->CommitL();

	CleanupStack::PopAndDestroy(3, rootEntry);
	}


TInt CSmsBaseUtils::DefaultServiceL(TUid aMtm,TMsvId& aService) const
	{
	CMsvEntry*				rootEntry=iSession->GetEntryL(KMsvRootIndexEntryIdValue);
	CleanupStack::PushL(rootEntry);
	CMsvStore*				store=rootEntry->ReadStoreL();
	CleanupStack::PushL(store);
	CMsvDefaultServices*	services=new(ELeave)CMsvDefaultServices;
	CleanupStack::PushL(services);
	services->RestoreL(*store);

	TInt					ret=services->DefaultService(aMtm, aService);

	CleanupStack::PopAndDestroy(3, rootEntry);

	return ret;
	}


//
void CSmsTestUtils::StartL()
	{
	_LIT(KSmsAccountName,	"Short Message");
	
	CSmsBaseUtils::StartL();

	CSmsSettings* smsSettings = CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);
	SetEntryL(KMsvRootIndexEntryId);

	EntryL().SetSortTypeL(SelectionOrdering());
	CMsvEntrySelection*	selection=EntryL().ChildrenWithTypeL(KUidMsvServiceEntry);
	CleanupStack::PushL(selection);
	TInt   count=selection->Count();
	TBool  found=EFalse;
	TPtrC  accountName(KSmsAccountName);
	TMsvId idSms=0;
	for (TInt i=count; i>0 && !found; )
		{
		SetEntryL(selection->At(--i));
		if	(EntryL().Entry().iMtm == KUidMsgTypeSMS && EntryL().Entry().iType == KUidMsvServiceEntry)
			{
			idSms=EntryL().EntryId();
			found=ETrue;
			}
		}

	if (!found)
		{
		TMsvEntry entry;
		entry.iMtm = KUidMsgTypeSMS;
		entry.iType = KUidMsvServiceEntry;
		entry.SetReadOnly(EFalse);
		entry.SetVisible(EFalse);
		entry.SetOperation(ETrue);
		entry.iDetails.Set(accountName);
	
		SetEntryL(KMsvRootIndexEntryId);
		EntryL().CreateL(entry);
		idSms=entry.Id();

		smsSettings->SetValidityPeriod(ESmsVPWeek);
		smsSettings->SetValidityPeriodFormat(TSmsFirstOctet::ESmsVPFInteger);
		smsSettings->SetReplyQuoted(EFalse);
		smsSettings->SetRejectDuplicate(ETrue);
		smsSettings->SetDelivery(ESmsDeliveryImmediately);
		smsSettings->SetDeliveryReport(ETrue);
		smsSettings->SetReplyPath(EFalse);
		smsSettings->SetMessageConversion(ESmsConvPIDNone);
		smsSettings->SetCanConcatenate(ETrue);

		SetEntryL(idSms);
		
		CMsvStore*	msvstore=EntryL().EditStoreL();
		CleanupStack::PushL(msvstore);
		smsSettings->StoreL(*msvstore);
		msvstore->CommitL();
		CleanupStack::PopAndDestroy(msvstore);
		}

		SetEntryL(idSms);
		
 		CMsvStore*	smsStore = EntryL().EditStoreL();
		CleanupStack::PushL(smsStore);

		smsSettings->RestoreL(*smsStore);

		TPtrC	ptrSCName;
		TPtrC	ptrSCNumber;

		smsSettings->AddSCAddressL(ptrSCName, ptrSCNumber);
		
		smsSettings->StoreL(*smsStore);
		smsStore->CommitL();

		CleanupStack::PopAndDestroy(smsStore);

		SetDefaultServiceL(KUidMsgTypeSMS, idSms);
		CleanupStack::PopAndDestroy(2);
	}


//
CSmsBaseClientTest* CSmsBaseClientTest::NewL(CSmsTestUtils& aSmsUtils,RTest& aTest)
	{
	CSmsBaseClientTest* self = new (ELeave) CSmsBaseClientTest(aSmsUtils,aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CSmsBaseClientTest* CSmsBaseClientTest::NewL(CSmsTestUtils* aSmsUtils, RTest& aTest)
	{
	CSmsBaseClientTest* self = new (ELeave) CSmsBaseClientTest(aSmsUtils, aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CSmsBaseClientTest::~CSmsBaseClientTest()
	{
	Cancel();
	delete iCurrentSelection;
	delete iOperation;
	delete iOwnedSmsUtils;
	}

void CSmsBaseClientTest::DoCancel()
	{
	if (iOperation)
		{
		iOperation->Cancel();
		}
	}

void CSmsBaseClientTest::SetCurrentOperation(CMsvOperation* aOperation)
	{
	delete iOperation;
	iOperation = aOperation;
	}


CMsvOperation& CSmsBaseClientTest::CurrentOperation()
	{
	return *iOperation;
	}

void CSmsBaseClientTest::ConstructL()
	{
	CBaseTestHarness::ConstructL();
	iCurrentSelection = new (ELeave) CMsvEntrySelection;
	}

CSmsBaseClientTest::CSmsBaseClientTest(CSmsTestUtils& aSmsUtils, RTest& aTest) 
: CBaseTestHarness(aTest), 
  iSmsUtils(aSmsUtils)
	{
	iOperation = NULL;
	}

CSmsBaseClientTest::CSmsBaseClientTest(CSmsTestUtils* aSmsUtils, RTest& aTest) 
: CBaseTestHarness(aTest), 
  iSmsUtils(*aSmsUtils),
  iOwnedSmsUtils(aSmsUtils)
	{
	iOperation = NULL;
	}

CSmsTestUtils& CSmsBaseClientTest::SmsTestUtils()
	{
	return iSmsUtils;
	}


//
CSmsBaseClientSectionParser* CSmsBaseClientSectionParser::NewL(CSmsBaseClientTest& aSmsTest, CTestScript& aScript, const TDesC& aNewSectionName)
	{
	CSmsBaseClientSectionParser* self = new (ELeave) CSmsBaseClientSectionParser(aSmsTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
	}

void CSmsBaseClientSectionParser::LogCommentL(const TDesC& /*aComment*/)
	{
//	iEmailTest.EmailTestUtils().WriteComment(aComment);
	}

void CSmsBaseClientSectionParser::ConstructL(const TDesC& aNewSectionName)
	{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iSmsTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iSmsTest.SetDebugInfo(debugInfo);
	iCommandParsers->AppendL(CNewSmsTestCommandParser::NewL(iScript,iSmsTest));
	iCommandParsers->AppendL(CSmsBaseUtilitiesParser::NewL(iSmsTest));
	}

CSmsBaseClientSectionParser::CSmsBaseClientSectionParser(CSmsBaseClientTest& aSmsTest, CTestScript& aScript) : CBaseSectionParser(aScript), iSmsTest(aSmsTest)
	{
	}


//
CNewSmsBaseTestCommandParser* CNewSmsBaseTestCommandParser::NewL(CTestScript& aScript, CMainTestHarness& aMainTest, TBool aKeepLogFile)
	{
	CNewSmsBaseTestCommandParser* self = new (ELeave) CNewSmsBaseTestCommandParser(aScript, aMainTest, aKeepLogFile);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CNewSmsBaseTestCommandParser::ProcessL()
	{
	// Create sms stuff
	CSmsTestUtils* smsUtils = new(ELeave) CSmsTestUtils();
	CleanupStack::PushL(smsUtils);
	
	// Create an Sms test, give it the test utilities
	CSmsBaseClientTest* smsTest = CSmsBaseClientTest::NewL(smsUtils,iMainTest.TestConsole());
	CleanupStack::Pop(smsUtils); // smsTest has taken ownership
	CleanupStack::PushL(smsTest);

	// Attach the Sms test to the main test
	iMainTest.AddStateL(smsTest, iDebugInfo);
	CleanupStack::Pop(smsTest); // SmsTest is now owned by the iMainTest

	// Check that there is one argument, the Sms test section name
	CheckNumberOfArgumentsL(1);

	// Create an Sms section parser
	CSmsBaseClientSectionParser* sectionParser = CSmsBaseClientSectionParser::NewL(*smsTest, iScript, (*iArgumentList)[0]);
	CleanupStack::PushL(sectionParser);
	sectionParser->ParseL();
	CleanupStack::PopAndDestroy(sectionParser);
	}

void CNewSmsBaseTestCommandParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(CCommandSmsBaseClientTest);
	}

CNewSmsBaseTestCommandParser::CNewSmsBaseTestCommandParser(CTestScript& aScript, CMainTestHarness& aMainTest, TBool aKeepLogFile) : iMainTest(aMainTest), iScript(aScript), iKeepLogFile(aKeepLogFile)
	{
	}


//
CSmsClientTest* CSmsClientTest::NewL(CSmsTestUtils& aSmsUtils, RTest& aTest)
	{
	CSmsClientTest* self = new (ELeave) CSmsClientTest(aSmsUtils, aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CSmsClientTest::~CSmsClientTest()
	{
	}

void CSmsClientTest::ConstructL()
	{
	CSmsBaseClientTest::ConstructL();
	}

CSmsClientTest::CSmsClientTest(CSmsTestUtils& aSmsUtils, RTest& aTest) : CSmsBaseClientTest(aSmsUtils, aTest)
	{
	}


//
CSmsClientSectionParser* CSmsClientSectionParser::NewL(CSmsClientTest& aSmsTest, CTestScript& aScript, const TDesC& aNewSectionName)
	{
	CSmsClientSectionParser* self = new (ELeave) CSmsClientSectionParser(aSmsTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
	}

void CSmsClientSectionParser::ConstructL(const TDesC& aNewSectionName)
	{
	CSmsBaseClientSectionParser::ConstructL(aNewSectionName);
	}

CSmsClientSectionParser::CSmsClientSectionParser(CSmsClientTest& aSmsTest, CTestScript& aScript) : CSmsBaseClientSectionParser(aSmsTest, aScript), iSmsTest(aSmsTest)
	{
	}



//
CNewSmsTestCommandParser* CNewSmsTestCommandParser::NewL(CTestScript& aScript, CSmsBaseClientTest& aSmsTest)
	{
	CNewSmsTestCommandParser* self = new (ELeave) CNewSmsTestCommandParser(aScript, aSmsTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	};

void CNewSmsTestCommandParser::ProcessL()
	{
	// Create an sms test, give it the test utilities
	CSmsClientTest* smsTest = CSmsClientTest::NewL(iSmsTest.SmsTestUtils(), iSmsTest.TestConsole());
	CleanupStack::PushL(smsTest);

	// Attach the sms test to the main test
	iSmsTest.AddStateL(smsTest, iDebugInfo);
	CleanupStack::Pop(smsTest); // smsTest is now owned by the iMainTest

	// Check that there is one argument, the sms test section name
	CheckNumberOfArgumentsL(1);

	// Create an sms section parser
	CSmsClientSectionParser* sectionParser = CSmsClientSectionParser::NewL(*smsTest, iScript, (*iArgumentList)[0]);
	CleanupStack::PushL(sectionParser);
	sectionParser->ParseL();
	CleanupStack::PopAndDestroy(sectionParser);
	}

void CNewSmsTestCommandParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(CCommandSmsClientTest);
	}

CNewSmsTestCommandParser::CNewSmsTestCommandParser(CTestScript& aScript, CSmsBaseClientTest& aSmsTest) : iSmsTest(aSmsTest), iScript(aScript)
	{
	}



//
CSmsBaseUtilitiesParser* CSmsBaseUtilitiesParser::NewL(CSmsBaseClientTest& aSmsTest)
	{
	CSmsBaseUtilitiesParser* self = new (ELeave) CSmsBaseUtilitiesParser(aSmsTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CSmsBaseUtilitiesParser::ProcessL()
	{
	TInt howMany;

	if ((*iCurrentCommand) == KCommandSmsBegin)
		{
		iSmsTest.AddStateL(new(ELeave) CSmsBegin(iSmsTest.SmsTestUtils()),iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandSmsCreateMessage)
		{
		if (iArgumentList->Count() == 4) // If optional howmany is specified
			{
			TPtrC param = (*iArgumentList)[3];
			TLex lex(param);
			if (lex.Val(howMany)!=KErrNone)
				howMany=1;
			}
		else
			{
			howMany = 1;
			CheckNumberOfArgumentsL(3);
			}
		RFs fs;
		fs.Connect();
		TParse fileName;
		ResolveFile(fs, (*iArgumentList)[0], (*iArgumentList)[1], fileName);
		fs.Close();
		TMsvId folder;
		TPtrC folderName = (*iArgumentList)[2];
		if (folderName == _L("inbox"))
			folder = KMsvGlobalInBoxIndexEntryId;
		else if (folderName == _L("outbox"))
			folder = KMsvGlobalOutBoxIndexEntryId;
		else if (folderName == _L("sent"))
			folder = KMsvSentEntryId;
		else 
			folder = KMsvDraftEntryId;
		iSmsTest.AddStateL(new (ELeave) CCreateSmsMessage(folder, iSmsTest.iCurrentServiceId, fileName.FullName(), iSmsTest.SmsTestUtils(),howMany), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandSmsEnd)
		{
		iSmsTest.AddStateL(new(ELeave) CSmsEnd(iSmsTest.SmsTestUtils()),iDebugInfo);
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	}

void CSmsBaseUtilitiesParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandSmsBegin);
	AddCommandL(KCommandSmsCreateMessage);
	AddCommandL(KCommandSmsEnd);
	}

CSmsBaseUtilitiesParser::CSmsBaseUtilitiesParser(CSmsBaseClientTest& aSmsTest) : iSmsTest(aSmsTest)
	{
	}


//
void CSmsBegin::StartL(TRequestStatus& aStatus)
	{
	iSmsUtils.StartL();
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

CSmsBegin::CSmsBegin(CSmsTestUtils& aSmsUtils) : iSmsUtils(aSmsUtils)
	{}


//
void CSmsEnd::StartL(TRequestStatus& aStatus)
	{
//	iSmsUtils.StartL();
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

CSmsEnd::CSmsEnd(CSmsTestUtils& aSmsUtils) : iSmsUtils(aSmsUtils)
	{}


//
CCreateSmsMessage::CCreateSmsMessage(TMsvId aFolderId, const TMsvId& aServiceId, const TDesC& aFileName, CSmsTestUtils& aSmsUtils,TInt aHowMany) : CActive(EPriorityNormal), iCurrentFolderId(aFolderId), iServiceId(aServiceId), iSmsUtils(aSmsUtils), iHowMany(aHowMany)
	{
	CActiveScheduler::Add(this);
	iFileName = aFileName;
	}

void CCreateSmsMessage::StartL(TRequestStatus& aStatus)
	{
	delete iTimer;
	iTimer = 0;
	iTimer = CTestTimer::NewL();
	iReportStatus = &aStatus;
	iTimer->AfterReq(5000000, iStatus);
	aStatus = KRequestPending;
	iState = ECreateSmsWait1;
	SetActive();
	}

void CCreateSmsMessage::RunL()
	{
	TInt i;
	switch (iState)
		{
		case ECreateSmsWait1:
			for (i=0; i<iHowMany; i++)
				 CreateMessageL();
			iTimer->AfterReq(5000000, iStatus);

			iState = ECreateSmsWait2;
			SetActive();
			break;

		case ECreateSmsWait2:
			// We should be safely back on the client side so we can continue.
			User::RequestComplete(iReportStatus, iStatus.Int());
			break;
		};
	}

void CCreateSmsMessage::CreateMessageL()
	{
	CParaFormatLayer*	paraFormatLayer=CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer*	charFormatLayer=CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);

	CRichText*			bodyText=CRichText::NewL(paraFormatLayer, charFormatLayer, CEditableText::EFlatStorage, 256);
	CleanupStack::PushL(bodyText);


		CSendAs* sendAs=CSendAs::NewL(*this);
		CleanupStack::PushL(sendAs);

		sendAs->SetMtmL(KUidMsgTypeSMS);

	    if (sendAs->AvailableServices().Count() < 1)
		    {
			User::Leave(KErrNotFound);
			}
		sendAs->SetService(0);

		sendAs->CreateMessageL(iCurrentFolderId);

		TRAPD(err, sendAs->SaveMessageL(ETrue));
		if (err != KErrNone)
			{
			sendAs->AbandonMessage();
			}
		else
			{
			TMsvId	messageId=sendAs->MessageId();
			iSmsUtils.SetEntryL(messageId);

			//Get the new message
			CSmsHeader* smsHeader = CSmsHeader::NewL(CSmsPDU::ESmsSubmit, *bodyText);
			CleanupStack::PushL(smsHeader);
			CMsvStore*	store = iSmsUtils.EntryL().EditStoreL();
			CleanupStack::PushL(store);

			smsHeader->RestoreL(*store);

			TMsvEntry	entry=iSmsUtils.EntryL().Entry();

			entry.SetSendingState(KMsvSendStateSuspended);

			TBuf<KSmsDetailsLength>	details;	
			if ( TSmsUtilities::GetDetails(iSmsUtils.EntryL().Session().FileSession(), smsHeader->Message(), details) == KErrNone )
				{
				entry.iDetails.Set(details);
				}
				
			TBuf<KSmsDescriptionLength>	desc;
			if ( TSmsUtilities::GetDescription(smsHeader->Message(), desc) == KErrNone )
				{
				entry.iDescription.Set(desc);
				}
			entry.SetInPreparation(EFalse);

			//	Update entry with data set from .ini file
			iSmsUtils.EntryL().ChangeL(entry);
			smsHeader->StoreL(*store);

			CleanupStack::PopAndDestroy(2, smsHeader);
		}
		CleanupStack::PopAndDestroy(sendAs);
	CleanupStack::PopAndDestroy(3); // bodyText, paraFormatLayer, charFormatLayer
	}

void CCreateSmsMessage::DoCancel()
	{
	}

CCreateSmsMessage::~CCreateSmsMessage()
	{
	if (iTimer)
		{
		iTimer->Cancel();
		}
	delete iTimer;
	}


