// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @SYMTestCaseID MSG-FRAMEWORK-0243
 @SYMTestType UT
 @SYMTestPriority High
 @SYMPREQ 234
 @SYMTestCaseDesc Creates a number of emails in different locales, sorts the messages and check there order.
 @SYMTestActions  Checks the sorting of the messages.
 @SYMTestExpectedResults The messages should be in the order they were created regardless of locale.
*/


#include <test/testexecutelog.h>
#include <cemailaccounts.h>
#include <iapprefs.h>
#include "T_SORT_UTCStep.h"
 
_LIT(KTestMessageDir,"");
const TInt KNumEmailMessages=10;

RTest test(_L("UTC Sort Test"));

void DummyObserver::HandleSessionEventL(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/){}
 
LOCAL_C void FormatDateTime(TDes& aBuffer, const TTime& aTime)
	{ 
	aBuffer.Zero();
	_LIT(KFormat4,"%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%:3%+B");
	aTime.FormatL(aBuffer,KFormat4); 
	}
		

CTestUTCSort::~CTestUTCSort()
	{
	iTMsvIdList->Reset();
	delete iTMsvIdList;
	delete iTestActive;
	delete iScheduler;
	delete iTestUtils;
	delete iObserver;
	delete iSession;
	
//	__UHEAP_MARKEND;
	}

CTestUTCSort::CTestUTCSort()
	{
	
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestUTCSort);
	}
		
 
void CTestUTCSort::SetLocale(TInt aCount)
	{
	const TInt KOneHour 	= 3600;
	const TInt KThreeHours 	= 3*KOneHour;
	const TInt KFourHours 	= 4*KOneHour;
	const TInt KFiveHours 	= 5*KOneHour;
	const TInt KSevenHours 	= 7*KOneHour;
	const TInt KNineHours 	= 9*KOneHour;
	const TInt KTwelveHours = 12*KOneHour;
	
	switch(aCount)
		{
		case 0:
		//ahead 1 hour DST off
		User::SetUTCOffset(KOneHour);
		break;
		case 1:
		//behind 4 hours DST EDstEuropean
		User::SetUTCOffset(-KFourHours);

		break;
		case 2:
		//ahead 5 hour DST off
		User::SetUTCOffset(KFiveHours);

		break;
		case 3:
		//behind 9 hours DST EDstSouthern
		User::SetUTCOffset(-KNineHours);

		break;
		case 4:
		//behind 12 hours DST EDstSouthern
		User::SetUTCOffset(-KTwelveHours);

		break;
		case 5:
		//ahead 4 hour DST EDstNorthern
		User::SetUTCOffset(KFourHours);

		break;
		case 6:
		//ahead 7 hour DST EDstNorthern
		User::SetUTCOffset(KSevenHours);

		break;
		case 7:
		//ahead 3 1/2 hours DST EDstEuropean
		User::SetUTCOffset(KThreeHours+KOneHour/2);

		break;
		case 8:
		//behind 7 1/2 hours DST EDstNorthern
		User::SetUTCOffset(-(KSevenHours+KOneHour/2));

		break;
		case 9:
		//ahead 1/2 hour DST EDstEuropean
		User::SetUTCOffset(KOneHour/2);

		break;			
		
		default:
		//setset the locale to UTC
		User::SetUTCOffset(0);
		break;
		}	
	}
	 
void CTestUTCSort::WriteStoreL()
	{
	//create the entry_structure file
	TParse parsedFileName;
	TFileName name(KTestMessageDir);
	iTestUtils->ResolveLogFile(name, parsedFileName);
	iTestUtils->FindChildrenL(KMsvRootIndexEntryIdValue, parsedFileName.FullName(), ETrue, EFalse);
	}
	
	
void CTestUTCSort::CreateServiceAndAccountL()
	{	
	CEmailAccounts* accounts = CEmailAccounts::NewL();
	CleanupStack::PushL(accounts);
	CImSmtpSettings* settings=new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(settings);
	CImIAPPreferences* smtpIAP = CImIAPPreferences::NewLC();
	TSmtpAccount smtpAccountId=accounts->CreateSmtpAccountL(_L("SMTP"),*settings, *smtpIAP, EFalse);	
	CleanupStack::PopAndDestroy(smtpIAP);
	CleanupStack::PopAndDestroy(settings);
	CleanupStack::PopAndDestroy(accounts);
	}
 
void CTestUTCSort::CreateMessagesL()
	{
	
	TMsvId emailMessageID;
	CImEmailOperation* emailOp;
	TMsvPartList partList;
	partList = (KMsvMessagePartBody | KMsvMessagePartAttachments);
		
	for(TInt count=0;count<KNumEmailMessages;++count)
		{
		//change the UTC offset for each message
		SetLocale(count);
		//wait for a bit
		User::After(1000000);
		
		emailOp = CImEmailOperation::CreateNewL(iTestActive->iStatus, *iSession, KMsvDraftEntryIdValue, KMsvMessagePartBody, partList, KUidMsgTypeSMTP); 
		CleanupStack::PushL(emailOp);
		// Active object which stops the active scheduler
		iTestActive->StartL(); 
		CActiveScheduler::Start();
		
		// Get the new message id.
		TPckg<TMsvId> param(emailMessageID);
		param.Copy(emailOp->FinalProgress());
		
		iTMsvIdList->AppendL(emailMessageID);		
		CleanupStack::PopAndDestroy(emailOp); 
		}	
	
	}

 	
TVerdict CTestUTCSort::TestSortingL()
	{
	
	TVerdict result=EPass;
	//create ordering objects
	TMsvSelectionOrdering ordering;	
	ordering.SetSorting(EMsvSortByDate);
	
	//sort the gloabl inbox by date
	CMsvEntry* inboxEntry = CMsvEntry::NewL(*iSession, KMsvDraftEntryIdValue,ordering);
	CleanupStack::PushL(inboxEntry);
	
	//retrieve the children of the inbox
	CMsvEntrySelection* msvEntrySelection;
	msvEntrySelection=inboxEntry->ChildrenL();
	CleanupStack::PushL(msvEntrySelection);
	
	CMsvEntry* child;
	TMsvEntry childDetails;
	TBuf<45> dateTimeString;
	
	//look at the id of each child and test its id, these should
	//be in the order the entries were created.
	for(TInt i=0;i<KNumEmailMessages;++i)
		{
		child=iSession->GetEntryL((*msvEntrySelection)[i]);
		childDetails=child->Entry();
		delete child;

		//print the time of the entry to the log file		
		dateTimeString.Zero();
		FormatDateTime(dateTimeString,childDetails.iDate); 
		INFO_PRINTF2(_L("%S"),&dateTimeString);
				
		if(childDetails.Id()!=iTMsvIdList->At(i))
			{
			ERR_PRINTF1(_L("Entries in wrong order"));	
			result=EFail;
			break;
			}
		}

	CleanupStack::PopAndDestroy(msvEntrySelection);
	CleanupStack::PopAndDestroy(inboxEntry);

	return result;
	
	}
 

TVerdict CTestUTCSort::doTestStepPreambleL()
	{
	SetTestStepResult(EPass);
	
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);
	//prepare for the test
	iTestUtils = CEmailTestUtils::NewL(test);
	iTestUtils->CreateAllTestDirectories();
	iTestUtils->FileSession().SetSessionPath(_L("C:\\"));
	iTestUtils->CleanMessageFolderL();
	iTestUtils->ClearEmailAccountsL();
	iTestUtils->GoClientSideL();
	iTMsvIdList=new(ELeave)CArrayFixFlat<TMsvId>(KNumEmailMessages);
	// Connect to the Message Server...
	iObserver = new (ELeave) DummyObserver();
	iSession = CMsvSession::OpenSyncL(*iObserver);
	iTestActive = new (ELeave) CTestActive();
	
	//wait 2 seconds for mailinit.exe to create a default entry in the message store
	User::After(2000000);
	//Remove the entry if it exists
	RemoveDefaultEntryL();
			
	return TestStepResult();
	}
		
void CTestUTCSort::RemoveDefaultEntryL()
	{
	
	TMsvSelectionOrdering ordering;	
	
		//sort the gloabl inbox by date
	CMsvEntry* inboxEntry = CMsvEntry::NewL(*iSession, KMsvDraftEntryIdValue,ordering);
	CleanupStack::PushL(inboxEntry);
	
	if(inboxEntry->Count()==1)
		{
		CMsvEntrySelection* mvEntrySelection;
		mvEntrySelection=inboxEntry->ChildrenL();
		CleanupStack::PushL(mvEntrySelection);
		CMsvEntry* child;		
		child=iSession->GetEntryL((*mvEntrySelection)[0]);
		iSession->RemoveEntry(child->EntryId());
		delete child;
		CleanupStack::PopAndDestroy(mvEntrySelection);
		}
	
	CleanupStack::PopAndDestroy(inboxEntry);
		
	}
	
TVerdict CTestUTCSort::doTestStepL()
	{
	TVerdict result=EPass;
	
	CreateServiceAndAccountL();
	//create the messages
	CreateMessagesL();	
	//test the message store
	result=TestSortingL();	
	SetTestStepResult(result);
	
	return TestStepResult();
	}
	

TVerdict CTestUTCSort::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
	
	


