// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// [TestStep Name]
// UnitTestCImap4ClientMtm
// [Paramaters]
// [TestStep Description]
// Invokes the API of the class CImap4ClientMtm to improve the code coverage.
// 
//

#include "t_cimap4clientmtm.h"

#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>
#include <imapset.h>
#include <iapprefs.h>
#include <impcmtm.h>

/* Literals Used */
_LIT(KAccountName,"ImapAccountName");
_LIT8(KConfigFileName, "ImapConfigFileName");
_LIT(KDataComponentFilename, "c:\\system\\mtm\\testsmtm.dat");
_LIT(KDummyRealAddress,"test001@msexchange01.closedtest.intra");
_LIT(KDummyAlliasAddress,"subbutest");
const TInt KTempId = 10;

const TUid KUidTestServerMtmType={0x10001304};
/**
	Function : CT_CImap4ClientMtm
	Description : Constructor
	@param : aSharedDataIMAP		Reference to CT_MsgSharedDataImap
	@return : N/A
*/
CT_CImap4ClientMtm::CT_CImap4ClientMtm(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KUnitTestCImap4ClientMtm);
	}

/**
	Function : ~CT_CImap4ClientMtm
	Description : Destructor
	@return : N/A
*/
CT_CImap4ClientMtm::~CT_CImap4ClientMtm()
	{
	}

void CT_CImap4ClientMtm::TestValidateMessageL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Validate Message -- started"));
	
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CClientMtmRegistry *theClientRegistry=CClientMtmRegistry::NewL(*iSession,KMsvDefaultTimeoutMicroSeconds32);
	TEST(theClientRegistry != NULL);
	TEST(theClientRegistry->IsPresent(KUidMsgTypeIMAP4));
	CleanupStack::PushL(theClientRegistry);

	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CImap4ClientMtm *theClientMtm=(CImap4ClientMtm*)theClientRegistry->NewMtmL(KUidMsgTypeIMAP4);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);
	cEntry->SetEntryL(service.Id());
	selection->AppendL(service.Id());
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::Pop(cEntry);
	
	theClientMtm->SetCurrentEntryL(cEntry);
	theClientMtm->SaveMessageL();
	INFO_PRINTF1(_L("Testing ValidateMessage with KMsvMessagePartRecipient"));
	TInt messagePartValue = theClientMtm->ValidateMessage(KMsvMessagePartRecipient);
	TEST(messagePartValue == 0);
	INFO_PRINTF1(_L("Testing ValidateMessage with KMsvMessagePartOriginator"));
	messagePartValue = 0;
	messagePartValue = theClientMtm->ValidateMessage(KMsvMessagePartOriginator);
	TEST(messagePartValue == 4);
	CleanupStack::Pop(theClientRegistry);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Validate Message -- ended"));
	}
	
void CT_CImap4ClientMtm::TestKErrNotSupportedAPI()
{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Validate Message -- started"));
	
	CMsvOperationWait *active = CMsvOperationWait::NewLC();
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CClientMtmRegistry *theClientRegistry=CClientMtmRegistry::NewL(*iSession,KMsvDefaultTimeoutMicroSeconds32);
	TEST(theClientRegistry != NULL);
	TEST(theClientRegistry->IsPresent(KUidMsgTypeIMAP4));
	CleanupStack::PushL(theClientRegistry);

	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CImap4ClientMtm *theClientMtm=(CImap4ClientMtm*)theClientRegistry->NewMtmL(KUidMsgTypeIMAP4);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);
	cEntry->SetEntryL(service.Id());
	selection->AppendL(service.Id());
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::Pop(cEntry);
	
	theClientMtm->SetCurrentEntryL(cEntry);
	theClientMtm->SaveMessageL();
	INFO_PRINTF1(_L("Testing: API - AddAttachmentL : started"));
	TRAP(error, theClientMtm->AddAttachmentL(KAccountName,KConfigFileName, KTempId, active->iStatus));
	TEST(error == KErrNotSupported);
	INFO_PRINTF1(_L("Testing: API - AddAttachmentL : ended"));
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: API - AddAttachmentL with FileSession : started"));
	RFile *tempFile = NULL;
	TRAP(error, theClientMtm->AddAttachmentL(*tempFile,KConfigFileName, KTempId, active->iStatus));
	TEST(error == KErrNotSupported);
	INFO_PRINTF1(_L("Testing: API - AddAttachmentL with FileSession : ended"));
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: API - AddLinkedAttachmentL : started"));
	TRAP(error, theClientMtm->AddLinkedAttachmentL(KAccountName,KConfigFileName, KTempId, active->iStatus));
	TEST(error == KErrNotSupported);
	INFO_PRINTF1(_L("Testing: API - AddLinkedAttachmentL : ended"));
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: API - AddEntryAsAttachmentL : started"));
	TRAP(error, theClientMtm->AddEntryAsAttachmentL(KTempId, active->iStatus));
	TEST(error == KErrNotSupported);
	INFO_PRINTF1(_L("Testing: API - AddEntryAsAttachmentL : ended"));
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: API - CreateAttachmentL : started"));
	TRAP(error, theClientMtm->CreateAttachmentL(KAccountName, *tempFile, KConfigFileName, KTempId, active->iStatus));
	TEST(error == KErrNotSupported);
	INFO_PRINTF1(_L("Testing: API - CreateAttachmentL : ended"));
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: API - CreateMessageL : started"));
	TRAP(error, theClientMtm->CreateMessageL(KTempId));
	TEST(error == KErrNotSupported);
	INFO_PRINTF1(_L("Testing: API - CreateMessageL : ended"));
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: API - DefaultServiceL : started"));
	TRAP(error, theClientMtm->DefaultServiceL());
	TEST(error == KErrNotSupported);
	INFO_PRINTF1(_L("Testing: API - DefaultServiceL : ended"));
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: API - RemoveDefaultServiceL : started"));
	TRAP(error, theClientMtm->RemoveDefaultServiceL());
	TEST(error == KErrNotSupported);
	INFO_PRINTF1(_L("Testing: API - RemoveDefaultServiceL : ended"));
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: API - ChangeDefaultServiceL : started"));
	TRAP(error, theClientMtm->ChangeDefaultServiceL(KTempId));
	TEST(error == KErrNotSupported);
	INFO_PRINTF1(_L("Testing: API - ChangeDefaultServiceL : ended"));

	CleanupStack::Pop(theClientRegistry);
	CleanupStack::PopAndDestroy(active);
	delete iSession;
}

void CT_CImap4ClientMtm::TestReplyL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: ReplyL -- started"));
	
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	
	CClientMtmRegistry *theClientRegistry=CClientMtmRegistry::NewL(*iSession,KMsvDefaultTimeoutMicroSeconds32);
	TEST(theClientRegistry != NULL);
	TEST(theClientRegistry->IsPresent(KUidMsgTypeIMAP4));
	CleanupStack::PushL(theClientRegistry);

	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CImap4ClientMtm *theClientMtm=(CImap4ClientMtm*)theClientRegistry->NewMtmL(KUidMsgTypeIMAP4);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);
	cEntry->SetEntryL(service.Id());
	selection->AppendL(service.Id());
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::Pop(cEntry);
	TMsvPartList partList = KMsvMessagePartBody | KMsvMessagePartAttachments | KMsvMessagePartDescription | KMsvMessagePartOriginator;
	
	theClientMtm->SetCurrentEntryL(cEntry);
	CMsvOperation* operation = NULL;
	active->Start();
	operation = theClientMtm->ReplyL(KMsvDraftEntryId, partList, active->iStatus);
	CActiveScheduler::Start();
	TEST(operation->iStatus.Int()==KErrNone);
	delete operation;
	CleanupStack::Pop(theClientRegistry);
	CleanupStack::Pop(active);
	delete iSession;
	INFO_PRINTF1(_L("Testing: ReplyL -- ended"));
	}

void CT_CImap4ClientMtm::TestAddAddresseeL()
{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: TestAddAddresseeL -- started"));
	
	CMsvOperationWait *active = CMsvOperationWait::NewLC();
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CClientMtmRegistry *theClientRegistry=CClientMtmRegistry::NewL(*iSession,KMsvDefaultTimeoutMicroSeconds32);
	TEST(theClientRegistry != NULL);
	TEST(theClientRegistry->IsPresent(KUidMsgTypeIMAP4));
	CleanupStack::PushL(theClientRegistry);

	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CImap4ClientMtm *theClientMtm=(CImap4ClientMtm*)theClientRegistry->NewMtmL(KUidMsgTypeIMAP4);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);
	cEntry->SetEntryL(service.Id());
	selection->AppendL(service.Id());
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::Pop(cEntry);
	
	theClientMtm->SetCurrentEntryL(cEntry);
	theClientMtm->SaveMessageL();
	INFO_PRINTF1(_L("Testing: API - TestAddAddresseeL with realaddress"));
	theClientMtm->AddAddresseeL(KDummyRealAddress);
	INFO_PRINTF1(_L("Testing: API - TestAddAddresseeL with realaddress"));
	theClientMtm->AddAddresseeL(KDummyRealAddress, KDummyAlliasAddress);
	INFO_PRINTF1(_L("Testing: API - TestAddAddresseeL -- ended"));

	CleanupStack::Pop(theClientRegistry);
	CleanupStack::PopAndDestroy(active);
	delete iSession;
}

void CT_CImap4ClientMtm::TestRemoveAddresseeL()
{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: TestAddAddresseeL -- started"));
	
	CMsvOperationWait *active = CMsvOperationWait::NewLC();
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CClientMtmRegistry *theClientRegistry=CClientMtmRegistry::NewL(*iSession,KMsvDefaultTimeoutMicroSeconds32);
	TEST(theClientRegistry != NULL);
	TEST(theClientRegistry->IsPresent(KUidMsgTypeIMAP4));
	CleanupStack::PushL(theClientRegistry);

	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CImap4ClientMtm *theClientMtm=(CImap4ClientMtm*)theClientRegistry->NewMtmL(KUidMsgTypeIMAP4);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);
	cEntry->SetEntryL(service.Id());
	selection->AppendL(service.Id());
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::Pop(cEntry);
	
	theClientMtm->SetCurrentEntryL(cEntry);
	theClientMtm->SaveMessageL();
	theClientMtm->AddAddresseeL(KDummyRealAddress);
	INFO_PRINTF1(_L("Testing: API - RemoveAddressee with realaddress"));
	TInt firstAddress = 0;
	theClientMtm->RemoveAddressee(firstAddress);
	INFO_PRINTF1(_L("Testing: API - TestAddAddresseeL -- ended"));

	CleanupStack::Pop(theClientRegistry);
	CleanupStack::PopAndDestroy(active);
	delete iSession;
}

void CT_CImap4ClientMtm::HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
	{
	INFO_PRINTF1(_L("Entered Handlesession EventL"));
	switch(aEvent)
		{
			case EMsvEntriesCreated:
			INFO_PRINTF1(_L("Entries created"));
			break;
			case EMsvEntriesChanged:
			INFO_PRINTF1(_L("Entries Changed"));
			break;
			case EMsvEntriesDeleted:
			INFO_PRINTF1(_L("Entried Deleted"));
			break;
			case EMsvEntriesMoved:
			INFO_PRINTF1(_L("Entried Moved"));
			break;
			case EMsvMtmGroupInstalled:
			INFO_PRINTF1(_L("Mtm Group Installed"));
			break;
			case EMsvMtmGroupDeInstalled:
			INFO_PRINTF1(_L("Mtm Group De-installed"));
			break;
			case EMsvGeneralError:
			INFO_PRINTF1(_L("General Error occured"));
			break;
			case EMsvCloseSession:
			INFO_PRINTF1(_L("Session Closed"));
			break;
			case EMsvServerReady:
			INFO_PRINTF1(_L("Server Ready"));
			break;
			case EMsvServerFailedToStart:
			INFO_PRINTF1(_L("Failed to start"));
			break;
			case EMsvCorruptedIndexRebuilt:
			INFO_PRINTF1(_L("Corrupted Intex rebuilt"));
			break;
			case EMsvServerTerminated:
			INFO_PRINTF1(_L("Server terminated"));
			break;
			case EMsvMediaChanged:
			INFO_PRINTF1(_L("Media Changed"));
			break;
			EMsvMediaUnavailable:
			INFO_PRINTF1(_L("Media Unavailable"));
			break;
			case EMsvMediaAvailable:
			INFO_PRINTF1(_L("Media Available"));
			break;
			case EMsvMediaIncorrect:
			INFO_PRINTF1(_L("Media Incorrect"));
			break;
			case EMsvCorruptedIndexRebuilding:
			INFO_PRINTF1(_L("Corrupted Index Rebuilding"));
			break;
		}
	INFO_PRINTF1(_L("Exited Handlesession EventL"));
	}

/*TMsvId CT_CImap4ClientMtm::CreateMessageL()
	{
	// Handle command line arguments
	//CCommandLineArguments* cmdLineArg=CCommandLineArguments::NewLC();
	//TInt count = cmdLineArg->Count();
	//TBool isCmdLine=FALSE;
	//if (count>2)	// Command line arguments?
//		{
//		if ((!(cmdLineArg->Arg(EArgTestParams).Compare(KTestParams))) && count==EArgEnd)
//			isCmdLine=TRUE;
//		}	

//	testUtils->TestStart(testNo++);
//	CTestActive* testActive = new (ELeave) CTestActive();
//	CleanupStack::PushL(testActive);

	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	TMsvPartList partList = KMsvMessagePartBody | KMsvMessagePartAttachments | KMsvMessagePartDescription | KMsvMessagePartOriginator;
	TMsvEmailTypeList msvEmailTypeList = 0;
	
	
	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	active->Start();
	CImEmailOperation* emailOperation = CImEmailOperation::CreateNewL(active->iStatus, *iSession, KMsvGlobalOutBoxIndexEntryId, partList, msvEmailTypeList, KUidMsgTypeSMTP);
	CleanupStack::PushL(emailOperation);

//	TestUiTimer* testUiTimer = TestUiTimer::NewLC(test.Console(), emailOperation);
//	testActive->StartL();
//	testUiTimer->IssueRequest();
	CActiveScheduler::Start();

	TMsvId temp;	
	TPckgC<TMsvId> paramPack(temp);
	const TDesC8& progBuf = emailOperation->ProgressL();
	paramPack.Set(progBuf);
	TMsvId messageId=paramPack();
	TInt error = emailOperation->iStatus.Int();
	CleanupStack::PopAndDestroy(emailOperation);
	CleanupStack::PopAndDestroy(active);
	//CleanupStack::PopAndDestroy(3, testActive); // testActive, emailOperation, testUiTimer
	//testUtils->TestFinish(testNo-1, error);
	//log->AppendComment(_L8(" Messages created in the outbox"));

	// fill in details for email message
	testUtils->iMsvEntry->SetEntryL(messageId);
	CMsvStore* store = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	CImHeader* header = CImHeader::NewLC();
	header->RestoreL(*store);

	if (isCmdLine)
		{
		TBuf<100> buf;
		TLex lex(cmdLineArg->Arg(EArgEmail));
		buf=lex.NextToken();
		header->ToRecipients().AppendL(buf);
		}
	else
		header->ToRecipients().AppendL(_L("t_smtc@lon-msgtest06.intra"));

	header->SetSubjectL(_L("Message from T_SMTC Typhoon!!"));
	header->StoreL(*store);
	store->CommitL();
 	CleanupStack::PopAndDestroy(3, cmdLineArg); // cmdLineArg, header, store

	//store some body text!!
	CParaFormatLayer* paraFormatLayer=CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer=CCharFormatLayer::NewL(); 
	CleanupStack::PushL(charFormatLayer);

	CRichText* bodyText=CRichText::NewL(paraFormatLayer, charFormatLayer, CEditableText::EFlatStorage, 256);
	CleanupStack::PushL(bodyText);

	TBuf<100> bodyContents = _L("Test body contents so that we can test the store body text code.");
	bodyText->InsertL(0, bodyContents);

	// store the body text
	testActive = new (ELeave) CTestActive();
	CleanupStack::PushL(testActive);

	CImEmailMessage* imEmailMessage = CImEmailMessage::NewLC(*(testUtils->iMsvEntry));
	imEmailMessage->StoreBodyTextL(messageId, *bodyText, testActive->iStatus);
	testActive->StartL();
	CActiveScheduler::Start();	// wait for the asynch operation to complete
	CleanupStack::PopAndDestroy(5, paraFormatLayer); // imEmailMessage, testActive, bodyText, charFormatLayer, paraFormatLayer

	// Add an attachment
	testActive = new (ELeave) CTestActive();
	CleanupStack::PushL(testActive);

	imEmailMessage = CImEmailMessage::NewLC(*(testUtils->iMsvEntry));
	CMsvAttachment* attachmentInfo = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachmentInfo);
	imEmailMessage->AttachmentManager().AddAttachmentL(KAttachment1,attachmentInfo,testActive->iStatus);
	CleanupStack::Pop(attachmentInfo);
	testActive->StartL();
	CActiveScheduler::Start();	// wait for the asynch operation to complete
	CleanupStack::PopAndDestroy(2, testActive);

	return messageId;
	}*/

/**
	Function : doTestStepL
	Description : 
	@return : TVerdict - Test step result
	@leave : KMsvNullIndexEntryId	Invalid IMAP account name specified
*/
TVerdict CT_CImap4ClientMtm::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CImap4ClientMtm Started"));
	TestValidateMessageL();
	//TestReplyL();
	TestKErrNotSupportedAPI();
	TestAddAddresseeL();
	TestRemoveAddresseeL();
	INFO_PRINTF1(_L("Test Step : CImap4ClientMtm Ended"));

	return TestStepResult();
	}
