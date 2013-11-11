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

#include "T_smcm.h"

	#include <csmsaccount.h>

CSmcmTest::CSmcmTest(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest)
: CSmsTestBase(aSmsTest, aScriptFile, aNextTest)
	{
	}

void CSmcmTest::RunAutoL()
	{
	iSmsTest.TestStart(++iNextTest, _L("CSmsSettings"));
	TestSettingsL();
	iSmsTest.TestFinish(iNextTest);

	iSmsTest.TestStart(++iNextTest, _L("CSmsClientMtm Capabilities"));
	TestCapabilitiesL();
	iSmsTest.TestFinish(iNextTest);

	iSmsTest.TestStart(++iNextTest, _L("CSmsClientMtm Global Find"));
	TestFindL();
	iSmsTest.TestFinish(iNextTest);

	iSmsTest.TestStart(++iNextTest, _L("CSmsClientMtm Reply"));
	TestReplyL();
	iSmsTest.TestFinish(iNextTest);

	iSmsTest.TestStart(++iNextTest, _L("CSmsClientMtm Forward"));
	TestForwardL();
	iSmsTest.TestFinish(iNextTest);

	iSmsTest.TestStart(++iNextTest, _L("CSmsClientMtm Async Functions"));
// There are no async function in sms client mtm
	iSmsTest.TestFinish(iNextTest);

	iSmsTest.TestStart(++iNextTest, _L("CSmsClientMtm Sync Functions"));
	TestSyncFunctionsL();
	iSmsTest.TestFinish(iNextTest);
	}

CSmcmTest::~CSmcmTest()
	{
	delete iSmsHeader;
	delete iRichText;
	delete iParaLayer;
	delete iCharLayer;
	}

void CSmcmTest::ConstructL()
	{
	SetTestNameL(KSmsClientMtmTestName);
	iSmsTest.SetLogToFile();				// Lets log to a file
	iSmsTest.InstantiateClientMtmsL();

	iSelection = new (ELeave) CMsvEntrySelection();
	iTimer = CTestTimer::NewL();

	iParaLayer =CParaFormatLayer::NewL();	// Rich Text for the body part of the message
	iCharLayer = CCharFormatLayer::NewL();
	iRichText = CRichText::NewL(iParaLayer, iCharLayer, CEditableText::EFlatStorage,256);

	CActiveScheduler::Add(this);
	}

CSmcmTest* CSmcmTest::NewLC(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest)
	{
	CSmcmTest* self = new (ELeave) CSmcmTest(aSmsTest, aScriptFile, aNextTest);
	CleanupStack::PushL(self);

	self->ConstructL();
	return self;
	}

void CSmcmTest::ShowMenuL()
	{
	iSmsTest.ResetMenu();

	iSmsTest.AppendToMenuL(_L("Start"));

	TInt result = iSmsTest.DisplayMenu(_L("SMS Client MTM Test"));

	if (result <= 0)
		return;

	switch (result)
		{
		case 1:
			RunAutoL();
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}

	if (!iSmsTest.RunAuto())
		{
		iSmsTest.Printf(_L("Press any key to continue...\n"));
		iSmsTest.Test().Getch();
		}

	ShowMenuL();
	}

void CSmcmTest::TestSettingsL()
	{
	iSmsTest.Printf(_L("Testing Create Settings...\n"));
	MsvEntry().SetEntryL(iSmsTest.iSmsServiceId);

// Lets create sms settings (just in case it does not exist yet)

	CSmsSettings* settings = CSmsSettings::NewL();
	CleanupStack::PushL(settings);
	settings->AddServiceCenterL(_L("Radiolinja0"),_L("+358508771010"));
	settings->AddServiceCenterL(_L("Radiolinja1"),_L("+358508771010"));
	settings->AddServiceCenterL(_L("Nokia"),_L("+358454400050"));
	settings->SetDefaultServiceCenter(2);
	iSmsTest(settings->DefaultServiceCenter()==2);
	settings->SetValidityPeriod(ESmsVPWeek);
	settings->SetReplyQuoted(ETrue);
	settings->SetRejectDuplicate(ETrue);
	settings->SetDelivery(ESmsDeliveryImmediately);
	settings->SetDeliveryReport(ETrue);
	settings->SetReplyPath(EFalse);
	settings->SetMessageConversion(ESmsConvPIDNone);
	settings->SetCanConcatenate(ETrue);
	settings->SetCharacterSet(TSmsDataCodingScheme::ESmsAlphabet8Bit);
	settings->SetValidityPeriodFormat(TSmsFirstOctet::ESmsVPFSemiOctet);
	settings->SetStatusReportHandling(CSmsSettings::EMoveReportToInboxVisible);
	settings->SetSpecialMessageHandling(CSmsSettings::EMoveReportToInboxVisible);


	CSmsAccount* smsAccount = CSmsAccount::NewLC();
	smsAccount->SaveSettingsL(*settings);
	CleanupStack::PopAndDestroy(smsAccount);


	iSmsTest.iServiceSettings->CopyL(*settings);
	CleanupStack::PopAndDestroy();  //settings
	}

// Now we shall test the recipient handling, find and validate features

void CSmcmTest::TestFindL()
	{
	iSmsTest.Test().Next(_L("Testing CSmsClientMtm Global Find"));

// Before testing the find and validate we need a dummy message to play with

	TMsvPartList partList;
	TMsvId id = iSmsTest.CreateDummyMessageToSendL();
	Client().SwitchCurrentEntryL(id);

// Lets load the message we just created
	Client().LoadMessageL();

// And add some recipients
	Client().AddAddresseeL(_L("+358503367709"), _L("Mico3"));
	Client().AddAddresseeL(_L("+358503367709"), _L("Mico4"));
	Client().AddAddresseeL(_L("//\\"), _L("\n"));
	Client().AddAddresseeL(_L("\n0"), _L("^"));
	Client().AddAddresseeL(_L("1234"), _L("~¨"));
	Client().AddAddresseeL(_L("äöåÖÅÖÖ¤"), _L("fdsa4324521{[]}"));
	Client().AddAddresseeL(_L("äöåÖÅÖÖ¤"), _L(""));
	Client().AddAddresseeL(_L("äöåÖÅÖÖ¤"));
	
// Remove Recipient
	Client().RemoveAddressee(2);

// Validating Message
	partList=KMsvMessagePartRecipient;
	iSmsTest(Client().ValidateMessage(partList));   // All the addresses are not valid

	Client().RemoveAddressee(Client().AddresseeList().Count()-1);
	Client().RemoveAddressee(Client().AddresseeList().Count()-1);
	Client().RemoveAddressee(Client().AddresseeList().Count()-1);
	Client().RemoveAddressee(Client().AddresseeList().Count()-2);
	Client().RemoveAddressee(Client().AddresseeList().Count()-2);

	partList=KMsvMessagePartRecipient;			// supported
	iSmsTest(!Client().ValidateMessage(partList));   
	partList=KMsvMessagePartBody;				// not supported, returns 0
	iSmsTest(!Client().ValidateMessage(partList));	
	partList=KMsvMessagePartOriginator;			// not supported, returns 0
	iSmsTest(!Client().ValidateMessage(partList));
	partList=KMsvMessagePartDescription;		// not supported, returns 0
	iSmsTest(!Client().ValidateMessage(partList));
	partList=KMsvMessagePartDate;				// not supported, returns 0
	iSmsTest(!Client().ValidateMessage(partList));
	partList=KMsvMessagePartAttachments;		// not supported, returns 0
	iSmsTest(!Client().ValidateMessage(partList));
	partList=KMsvMessagePartBody|KMsvMessagePartRecipient;
	iSmsTest(!Client().ValidateMessage(partList));
	iSmsTest.Printf(_L("Test Validate Passed\n"));

// Find in body and recipients
	Client().AddAddresseeL(_L("+358503367709"), _L("Mr. Tester"));
	partList=KMsvMessagePartRecipient;
	iSmsTest(Client().Find(_L("est"),partList)==KMsvMessagePartRecipient);
	partList=KMsvMessagePartBody;				
	iSmsTest(Client().Find(_L("end"),partList)==KMsvMessagePartBody);	
	partList=KMsvMessagePartOriginator;			
	iSmsTest(!Client().Find(_L("end"),partList));
	partList=KMsvMessagePartDescription;		// not supported, returns 0
	iSmsTest(!Client().Find(_L("end"),partList));
	partList=KMsvMessagePartDate;				// not supported, returns 0
	iSmsTest(!Client().Find(_L("end"),partList));
	partList=KMsvMessagePartAttachments;		// not supported, returns 0
	iSmsTest(!Client().Find(_L("end"),partList));
	partList=KMsvMessagePartBody|KMsvMessagePartRecipient;
//	resultList=Client().Find(_L(""),partList);		// crashes with empty string
	iSmsTest(Client().Find(_L("\n"),partList));
	iSmsTest(Client().Find(_L("äöå"),partList));
	iSmsTest(!Client().Find(_L("{[}]"),partList));
	iSmsTest(!Client().Find(_L("\\"),partList));
	iSmsTest(!Client().Find(_L("4352316542631753dfgfdgs4t6543w563vsd4"),partList));
	iSmsTest(Client().Find(_L("~"),partList));
	iSmsTest(Client().Find(_L(" "),partList));
	iSmsTest(!Client().Find(_L("^"),partList));
	iSmsTest(!Client().Find(_L("   \"dsasaf¤;¨"),partList));
	iSmsTest(!Client().Find(_L("\\"),partList));
	iSmsTest(!Client().Find(_L(" "),partList)); // alt+255
	iSmsTest(Client().Find(_L("¨"),partList));
	iSmsTest(!Client().Find(_L("/\\"),partList));
	iSmsTest(!Client().Find(_L("\\ "),partList));
	iSmsTest.Printf(_L("Test Find Passed\n"));
	}

void CSmcmTest::TestCapabilitiesL()
	{
// Query MTM's capabilities
	TInt response;
	iSmsTest(Client().QueryCapability(KUidMtmQueryMaxBodySize,response)==KErrNone);
//	iSmsTest(response==KSmscMaxTotalMsgSize);
	iSmsTest(Client().QueryCapability(KUidMtmQueryMaxTotalMsgSize,response)==KErrNone);
//	iSmsTest(response==KSmcmMaxMessageNumber*KSmcmMaxCharsInMessageConcatenated7Bit);
	iSmsTest(Client().QueryCapability(KUidMtmQuerySupportedBody,response)==KErrNone);
	iSmsTest(response==KMtm7BitBody + KMtm8BitBody + KMtm16BitBody);
	iSmsTest(Client().QueryCapability(KUidMtmQuerySupportAttachments,response)==KErrNotSupported);
	iSmsTest(response);
	iSmsTest(Client().QueryCapability(KUidMtmQuerySupportSubject,response)==KErrNotSupported);
	iSmsTest(response);
	iSmsTest(Client().QueryCapability(KUidMtmQuerySupportsFolder,response)==KErrNotSupported);
	iSmsTest(response);
	iSmsTest(Client().QueryCapability(KUidMtmQueryOffLineAllowed,response)==KErrNotSupported);
	iSmsTest(response);
	iSmsTest(Client().QueryCapability(KUidMtmQueryCanSendMsg,response)==KErrNone);
	iSmsTest(response);
	iSmsTest(Client().QueryCapability(KUidMtmQueryCanReceiveMsg,response)==KErrNone);
	iSmsTest(response);
	iSmsTest(Client().QueryCapability(KUidMtmQueryMaxRecipientCount,response)==KErrNone);
	iSmsTest(response==-1);
	iSmsTest(Client().QueryCapability(KUidMtmQuerySendAsRequiresRenderedImage,response)==KErrNotSupported);
	iSmsTest(response);
	iSmsTest(Client().QueryCapability(KUidMtmQuerySendAsRenderingUid,response)==KErrNotSupported);
	iSmsTest(response);
	iSmsTest(Client().QueryCapability(KUidMsvMtmQueryEditorUid,response)==KErrNone);
	iSmsTest(response==268441151); // 0x1000163f in hex form
	iSmsTest(Client().QueryCapability(KUidMsvQuerySupportsBioMsg,response)==KErrNone);
	iSmsTest(response);
	iSmsTest(Client().QueryCapability(KUidMsvQuerySupportsScheduling,response)==KErrNone);
	iSmsTest(response);
	iSmsTest(Client().QueryCapability(KUidSmcmTestNonSense,response)==KErrNotSupported);
	iSmsTest(response);
	iSmsTest.Printf(_L("Test Query Capabilities Passed\n"));
	}

// Now testing replying to received messages
void CSmcmTest::TestReplyL()
	{
	iSmsTest.Test().Next(_L("CSmsClientMtm Reply"));
	iSmsTest.Printf(_L("This test is INCOMPLETE\n"));

// First we need a received message that we will reply soon
	TMsvId id = iSmsTest.CreateDummyMessageToReceiveL();
	iSmsTest.SetEntryL(id);

	Client().SwitchCurrentEntryL(id); // Lets point at the received message
	Client().LoadMessageL();

	TMsvPartList part=0;
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	CMsvOperation* op=Client().ReplyL(KMsvDraftEntryId, part, wait->iStatus);
	CleanupStack::PushL(op);
	wait->Start();
	CActiveScheduler::Start();

	TPckgBuf<TMsvId> pkg;
	pkg.Copy(op->ProgressL());
	TMsvId progress = pkg();

	CleanupStack::PopAndDestroy(2); //op, wait
	
	Client().SwitchCurrentEntryL(progress);
	Client().LoadMessageL();
	Client().Body().InsertL(0,_L("This is reply"));
	Client().SaveMessageL();
	}

void CSmcmTest::TestForwardL()
	{
	iSmsTest.Test().Next(_L("CSmsClientMtm Forward"));
	iSmsTest.Printf(_L("This test is INCOMPLETE\n"));

	TMsvId id = iSmsTest.CreateDummyMessageToReceiveL();
	Client().SwitchCurrentEntryL(id);
	Client().LoadMessageL();
	TMsvPartList part=0;
	TRequestStatus status(0);
	CMsvOperation* op = Client().ForwardL(KMsvDraftEntryId, part, status);
	CleanupStack::PushL(op);

	TMsvId temp;	
	TPckgC<TMsvId> paramPack(temp);
	const TDesC8& progBuf2 = op->ProgressL();	
	paramPack.Set(progBuf2);
	TMsvId progress=paramPack();
	
	CleanupStack::PopAndDestroy(); //op
	op = NULL;

	Client().SwitchCurrentEntryL(progress);

	Client().LoadMessageL();
	Client().Body().InsertL(0,_L("This is forward"));
	Client().AddAddresseeL(_L("+358503367709"), _L("MicoForward"));

	CSmsSettings* smssett = CSmsSettings::NewL();
	CleanupStack::PushL(smssett);
	smssett->CopyL(Client().ServiceSettings());
	CleanupStack::PopAndDestroy(); //smssett
	
	Client().SaveMessageL();
	}

void CSmcmTest::TestSyncFunctionsL()
	{
	iSmsTest.Test().Next(_L("Test CSmsClientMtm Sync Functions"));
	iSmsTest.Printf(_L("This test is INCOMPLETE\n"));
	}
