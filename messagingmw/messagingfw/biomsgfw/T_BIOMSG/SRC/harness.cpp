// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "harness.h"
#include "testframeutils.h"
#include "msvtestutils.h"
#include "biotestutils.h"
#include <cemailaccounts.h>
#include <csmsaccount.h>
#include <iapprefs.h>

CBaseTestHarness::~CBaseTestHarness()
{
	iStateList->ResetAndDestroy();
	delete iStateList;
}

void CBaseTestHarness::StartL(TRequestStatus& aStatus)
{
	iCurrentState = 0;
	iReportStatus = &aStatus;
	aStatus = KRequestPending;

	TInt err = KErrNone;
	if (iCurrentState < iStateList->Count())
	{
		//
		// We have to handle the error handeling ourselfs for the 1st request
		// every other request will be handled by AS
		if(iMarkTestStarts)
			TestStart(iCurrentState);

		TRAP(err, (iStateList->At(iCurrentState))->StartL(iStatus));
		if(err != KErrNone)
			RunError(err);
	}
	else
	{
		//
		// Nothing to do!
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
	}

	if(err == KErrNone)
		SetActive();
}


void CBaseTestHarness::AddStateL(MBaseTestState* aTestState, TTestDebugInfo aDebugInfo)
{
	aTestState->SetDebugInfo(aDebugInfo);
	TRAPD(err, iStateList->AppendL(aTestState));
	if (err != KErrNone)
	{
		//
		// If the state can't be added then delete it.
		// We do this because we have taken ownership of the state.
		delete aTestState;
		User::Leave(err);
	}
}

//
// From CActive
void CBaseTestHarness::RunL()
{
	if(iOldCurrentState != iCurrentState && iMarkTestStarts)
	{
		iOldCurrentState = iCurrentState;
		TestFinish(iCurrentState, iStatus);
	}

	//
	// Was there an error?
	if (iStatus == KErrNone)
	{
		//
		// Any more states?
		iCurrentState++;
		if(iCurrentState < iStateList->Count())
		{
			// If there are any more states, start them.  If StartL leaves, RunErrorL is called
			// and we complete the request, and move to the next state.
			if(iMarkTestStarts)
				TestStart(iCurrentState);

			(*iStateList)[iCurrentState]->StartL(iStatus);

			SetActive();
		}
		else
		{
			//
			// If this is the last state, complete the parents request
			User::RequestComplete(iReportStatus, iStatus.Int());
		}
	}
	else
	{
		//
		// Handle the error
		OnStateError();
	}
}

void CBaseTestHarness::OnStateError()
{
	//
	// Log the error
	LogStateError();

	//
	// Move to the next state
	RunError(KErrNone);
}

TInt CBaseTestHarness::RunError(TInt aError)
{
	// 
	// Called when StartL leaves.  Complete the request.  The next call
	// of RunL will log the error.
	TRequestStatus* i = &iStatus;

	// If the cmd is expected to fail, pass test.
	if ((KErrNone != iExpectedError) && (aError == iExpectedError))
		{
		User::RequestComplete(i, KErrNone);
		iExpectedError = KErrNone;
		}
	else
		User::RequestComplete(i, aError);

	SetActive();
	return KErrNone;
}

TInt CBaseTestHarness::LogStateError()
{
	//
	// Get the error message for the state
	TTestDebugInfo debugInfo = (*iStateList)[iCurrentState]->GetDebugInfo();

	//
	// Get the test failed string (section name etc)
	TInt err;
	HBufC* errorText = NULL;
	TRAP(err, errorText = debugInfo.TestFailedStringL(iStatus.Int()));
	if (err != KErrNone)
		return err;

	//
	// Log it
	TRAP(err, LogCommentL(*errorText));
	if (err != KErrNone)
		return err;
	TestConsole().Printf(*errorText);
	TestConsole().Printf(_L("\r"));
	delete errorText;

	return KErrNone;
}

void CBaseTestHarness::ConstructL()
{
	CActiveScheduler::Add(this);
	iStateList = new (ELeave) CArrayPtrFlat<MBaseTestState>(10);
}

//
//
// CMainTestHarness
//

CMainTestHarness* CMainTestHarness::NewL(RTest& aTest)
{
	CMainTestHarness* self = new (ELeave) CMainTestHarness(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CMainTestHarness::ConstructL()
{
	CBaseTestHarness::ConstructL();

	iBioUtils = CBioTestUtils::NewL(iTest);
	iAllTestsSuccessful = ETrue;
}

CMainTestHarness::~CMainTestHarness()
{
	if(iAllTestsSuccessful)
		iBioUtils->TestHarnessCompleted();

	else
		iBioUtils->TestHarnessFailed(KErrGeneral);

	delete iBioUtils;
}

void CMainTestHarness::TestStart(TInt& aState)
{
	if(iBioUtils != NULL)
		iBioUtils->TestStart(aState);
}

void CMainTestHarness::TestFinish(TInt& aState, TRequestStatus& aStatus)
{
	if(iBioUtils != NULL)
		iBioUtils->TestFinish(aState, aStatus.Int());

	if (aStatus != KErrNone)
		iAllTestsSuccessful = EFalse;
}

//
//
// CClientTestHarness
//
CClientTestHarness* CClientTestHarness::NewL(CBioTestUtils& aTestUtils, RTest& aTest)
{
	CClientTestHarness* self = new (ELeave) CClientTestHarness(aTestUtils, aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CClientTestHarness::ConstructL()
{
	CBaseTestHarness::ConstructL();
}

void CClientTestHarness::LogCommentL(const TDesC& aComment)
{
	iBioUtils.WriteComment(aComment);
}

//
// Custom error handeling
void CClientTestHarness::OnStateError()
{
	//
	// Get the base class to log the error
	CBaseTestHarness::LogStateError();

	//
	// Complete the parents request - this has the net effect of skipping any
	// remaing tests in this section.  This is because if one test failes, the chances
	// are the remaining tests are dependent on it and fail too - worse still, they normall panic...
	User::RequestComplete(iReportStatus, iStatus.Int());
}

void CClientTestHarness::InstantiateSmsSettingsL()
{
	InstantiateClientMtmL();
	delete iSmsSettings;
	CSmsAccount* account = CSmsAccount::NewLC();
	iSmsSettings = CSmsSettings::NewL();
	account->LoadSettingsL(*iSmsSettings);
	CleanupStack::PopAndDestroy(account);
}

TMsvId CClientTestHarness::GetImapServiceL(CEmailAccounts& aAccounts)
	{
	TMsvId imapService;
	
	RArray<TImapAccount> imapAccounts;
	CleanupClosePushL(imapAccounts);
	
	aAccounts.GetImapAccountsL(imapAccounts);
	if(imapAccounts.Count()>0)
		{
		// if a account exist take the first
		imapService = imapAccounts[0].iImapService;
		}
	else
		{
		CImImap4Settings* imapSettings = new(ELeave) CImImap4Settings();
		CleanupStack::PushL(imapSettings);

		CImIAPPreferences* imapIAP = CImIAPPreferences::NewLC();
				
		aAccounts.PopulateDefaultImapSettingsL(*imapSettings, *imapIAP);

		_LIT(KImapServer, "imap.dial.pipex.com");
		imapSettings->SetServerAddressL(KImapServer);
		_LIT8(KImapLoginName, "abf75");
		imapSettings->SetLoginNameL(KImapLoginName);
		_LIT8(KImapPassword, "oodifama");
		imapSettings->SetPasswordL(KImapPassword);
		imapSettings->SetPort(143);
		
		TImapAccount imapAccount = aAccounts.CreateImapAccountL(KNullDesC, *imapSettings, *imapIAP, EFalse);
		imapService = imapAccount.iImapService;
		
		CleanupStack::PopAndDestroy(2, imapSettings); // imapIAP, imapSettings
		}
	CleanupStack::PopAndDestroy();
	return imapService;
	}

void CClientTestHarness::InstantiateImapSettingsL()
{
	InstantiateClientMtmL();

	if (iImapSettings==NULL)
	{
		TMsvId serviceId(KMsvNullIndexEntryId);
		CEmailAccounts* accounts = CEmailAccounts::NewLC();
		TRAPD(err, serviceId = GetImapServiceL(*accounts));
		//
		// If no default service
		if((err==KErrNotFound) || (serviceId==KMsvUnknownServiceIndexEntryId))
		{
			_LIT(KUhOh,"ERROR Default IMAP4Settings Not Found");
			TestUtils().WriteComment(KUhOh);
			User::Leave(KErrNotFound);
		}
		else
		{
			iImapSettings = new(ELeave)CImImap4Settings();

			TImapAccount imapAccount;
			accounts->GetImapAccountL(serviceId, imapAccount);
 			accounts->LoadImapSettingsL(imapAccount, *iImapSettings);

		}
		
		CleanupStack::PopAndDestroy(accounts);
	}
}

void CClientTestHarness::InstantiateSmtpSettingsL()
{
	InstantiateClientMtmL();

	if (iSmtpSettings==NULL)
	{
		TMsvId serviceId(KMsvNullIndexEntryId);
		CEmailAccounts* accounts = CEmailAccounts::NewLC();
		
		TSmtpAccount smtpAccount;
		TRAPD(err, accounts->DefaultSmtpAccountL(smtpAccount));
		serviceId = smtpAccount.iSmtpService;

		//
		// if no default service
		if((err==KErrNotFound) || (serviceId==KMsvUnknownServiceIndexEntryId))
		{
			_LIT(KUhOh,"ERROR Default SMTPSettings Not Found");
			TestUtils().WriteComment(KUhOh);
			User::Leave(KErrNotFound);
		}
		else
		{
			iSmtpSettings = new(ELeave)CImSmtpSettings();

			
			accounts->GetSmtpAccountL(serviceId, smtpAccount);
			accounts->LoadSmtpSettingsL(smtpAccount, *iSmtpSettings);
		}

		CleanupStack::PopAndDestroy(accounts);
	}
}

void CClientTestHarness::InstantiateNamedPop3SettingsL(const TDesC& aName)
{
	delete iPop3Settings;
	iPop3Settings = NULL;

	InstantiateClientMtmL();

	TMsvId aPop3EntryId=KMsvNullIndexEntryId;

	CMsvEntrySelection *msvSelection = NULL;

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries( ETrue );
	TestUtils().iMsvEntry->SetSortTypeL(order);

	// Get list of children IDs
	TestUtils().iMsvEntry->SetEntryL(KMsvRootIndexEntryId);

	msvSelection = TestUtils().iMsvEntry->ChildrenL();
	CleanupStack::PushL(msvSelection);

	
	TMsvEntry entry;
	TInt count = msvSelection->Count(); 
	for (TInt j = 0; j < count; j++)
	{
		// set context to service entry
		TestUtils().iMsvEntry->SetEntryL((*msvSelection)[j]);
		entry = TestUtils().iMsvEntry->Entry(); 
		
		if (entry.iType == KUidMsvServiceEntry)
		{
			if(entry.iDetails.Length() == aName.Length())
			{
				if(entry.iDetails.CompareF(aName)==0 )
				{
					if(entry.iMtm == KUidMsgTypePOP3)
					{
						aPop3EntryId= entry.Id();						
					}
				}
			}
		}
	}

	CleanupStack::PopAndDestroy(msvSelection);

	if(aPop3EntryId==KMsvNullIndexEntryId)
	{
		_LIT(KUhOh,"ERROR Matching Pop3Settings Not Found");
		TestUtils().WriteComment(KUhOh);
		User::Leave(KErrNotFound);
	}
	else
	{
		iPop3Settings = new(ELeave)CImPop3Settings();
		
		CEmailAccounts* accounts = CEmailAccounts::NewLC();

		TPopAccount popAccount;
		accounts->GetPopAccountL(aPop3EntryId, popAccount);
		accounts->LoadPopSettingsL(popAccount, *iPop3Settings);

		CleanupStack::PopAndDestroy(accounts);
	}
}

void CClientTestHarness::InstantiateNamedSmtpSettingsL(const TDesC& aName)
{
	if (iSmtpSettings)
	{
		delete iSmtpSettings;
		iSmtpSettings = NULL;
	}

	InstantiateClientMtmL();

	//TestUtils().iMsvEntry->SetEntryL(KMsvRootIndexEntryIdValue);

	TMsvId aSmtpEntryId=KMsvNullIndexEntryId;

	CMsvEntrySelection *msvSelection = NULL;

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries( ETrue );
	TestUtils().iMsvEntry->SetSortTypeL(order);

	// Get list of children IDs
	TestUtils().iMsvEntry->SetEntryL(KMsvRootIndexEntryId);

	msvSelection = TestUtils().iMsvEntry->ChildrenL();
	CleanupStack::PushL(msvSelection);

	
	TMsvEntry entry;
	TInt count = msvSelection->Count(); 
	for (TInt j = 0; j < count; j++)
		{
		// set context to service entry
		TestUtils().iMsvEntry->SetEntryL((*msvSelection)[j]);
		entry = TestUtils().iMsvEntry->Entry(); 

		if (entry.iType == KUidMsvServiceEntry)
		    if(entry.iDetails.Length() == aName.Length())
				if(entry.iDetails.CompareF(aName)==0 )
					{
					if(entry.iMtm == KUidMsgTypeSMTP)
						{
						aSmtpEntryId= entry.Id();						
						}
					}

		}

	CleanupStack::PopAndDestroy(msvSelection);

	if(aSmtpEntryId==KMsvNullIndexEntryId)
			{
				_LIT(KUhOh,"ERROR Matching SmtpSettings Not Found");
				TestUtils().WriteComment(KUhOh);
				User::Leave(KErrNotFound);
			}
			else
			{
				iSmtpSettings = new(ELeave)CImSmtpSettings();
				
				CEmailAccounts* accounts = CEmailAccounts::NewLC();

				TSmtpAccount smtpAccount;
				accounts->GetSmtpAccountL(aSmtpEntryId, smtpAccount);
				accounts->LoadSmtpSettingsL(smtpAccount, *iSmtpSettings);

				CleanupStack::PopAndDestroy(accounts);
			}
	
	//Successful
}

void CClientTestHarness::InstantiateNamedImapSettingsL(const TDesC& aName)
{
	if (iImapSettings)
	{
		delete iImapSettings;
		iImapSettings = NULL;
	}

	InstantiateClientMtmL();

	//TestUtils().iMsvEntry->SetEntryL(KMsvRootIndexEntryIdValue);

	TMsvId aImapEntryId=KMsvNullIndexEntryId;

	CMsvEntrySelection *msvSelection = NULL;

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries( ETrue );
	TestUtils().iMsvEntry->SetSortTypeL(order);

	// Get list of children IDs
	TestUtils().iMsvEntry->SetEntryL(KMsvRootIndexEntryId);

	msvSelection = TestUtils().iMsvEntry->ChildrenL();
	CleanupStack::PushL(msvSelection);

	
	TMsvEntry entry;
	TInt count = msvSelection->Count(); 
	for (TInt j = 0; j < count; j++)
		{
		// set context to service entry
		TestUtils().iMsvEntry->SetEntryL((*msvSelection)[j]);
		entry = TestUtils().iMsvEntry->Entry(); 

		if (entry.iType == KUidMsvServiceEntry)
		    if(entry.iDetails.Length() == aName.Length())
				if(entry.iDetails.CompareF(aName)==0 )
					{
					if(entry.iMtm == KUidMsgTypeIMAP4)
						{
						aImapEntryId= entry.Id();						
						}
					}

		}

	CleanupStack::PopAndDestroy(msvSelection);

	if(aImapEntryId==KMsvNullIndexEntryId)
			{
				_LIT(KUhOh,"ERROR Matching ImapSettings Not Found");
				TestUtils().WriteComment(KUhOh);
				User::Leave(KErrNotFound);
			}
			else
			{
				iImapSettings = new(ELeave)CImImap4Settings();
				
				CEmailAccounts* accounts = CEmailAccounts::NewLC();

				TImapAccount imapAccount;
				accounts->GetImapAccountL(aImapEntryId, imapAccount);
				accounts->LoadImapSettingsL(imapAccount, *iImapSettings);

				CleanupStack::PopAndDestroy(accounts);
			}
	
	//Successful
}

void CClientTestHarness::UnInstantiatePop3SettingsL()
{
	if (iPop3Settings)
	{
		delete iPop3Settings;
		iPop3Settings = NULL;
	}
	
}

void CClientTestHarness::UnInstantiateSmtpSettingsL()
{
	if (iSmtpSettings)
	{
		delete iSmtpSettings;
		iSmtpSettings = NULL;
	}
	
}

void CClientTestHarness::UnInstantiateImapSettingsL()
{
	if (iImapSettings)
	{
		delete iImapSettings;
		iImapSettings = NULL;
	}
	
}

TMsvId CClientTestHarness::GetPopServiceL(CEmailAccounts& aAccounts)
	{
	TMsvId popService;
	
	RArray<TPopAccount> popAccounts;
	CleanupClosePushL(popAccounts);
	
	aAccounts.GetPopAccountsL(popAccounts);
	if(popAccounts.Count()>0)
		{
		// if a account exist take the first
		popService = popAccounts[0].iPopService;
		}
	else
		{
		CImPop3Settings* popSettings = new(ELeave) CImPop3Settings();
		CleanupStack::PushL(popSettings);

		CImIAPPreferences* popIAP = CImIAPPreferences::NewLC();
				
		aAccounts.PopulateDefaultPopSettingsL(*popSettings, *popIAP);

		TPopAccount popAccount = aAccounts.CreatePopAccountL(KNullDesC, *popSettings, *popIAP, EFalse);
		popService = popAccount.iPopService;
		
		CleanupStack::PopAndDestroy(2, popSettings); // imapIAP, imapSettings
		}
	CleanupStack::PopAndDestroy();
	return popService;
	}

void CClientTestHarness::InstantiatePop3SettingsL()
{
	
	InstantiateClientMtmL();

	if (iPop3Settings==NULL)
	{
		TestUtils().iMsvEntry->SetEntryL(KMsvRootIndexEntryIdValue);

		TMsvId serviceId(KMsvNullIndexEntryId);
		CEmailAccounts* accounts = CEmailAccounts::NewLC();
		TRAPD(err, serviceId = GetPopServiceL(*accounts));

		// if no default service
			if((err==KErrNotFound) || (serviceId==KMsvUnknownServiceIndexEntryId))
			{
				_LIT(KUhOh,"ERROR Default Pop3Settings Not Found");
				TestUtils().WriteComment(KUhOh);
				User::Leave(KErrNotFound);
			}
			else
			{
				iPop3Settings = new(ELeave)CImPop3Settings();
				
				TPopAccount popAccount;
				accounts->GetPopAccountL(serviceId, popAccount);
				accounts->LoadPopSettingsL(popAccount, *iPop3Settings);
			}
	
		CleanupStack::PopAndDestroy(accounts);
		}
}

void CClientTestHarness::InstantiateClientMtmL()
{
	//
	// To speed up creation, share a static connection to client side MTM
	// set that connection up now
	TInt nRetryCount = 2;
	TInt err = KErrGeneral;

	if(iBioUtils.iBioClientMtm == NULL)
	{
		while(iBioUtils.iBioClientMtm == NULL && nRetryCount > 0)
		{
			//
			// Start up client side stuff
			TRAP(err, iBioUtils.GoClientSideL());
			if(err == KErrNone)
			{
				//
				// Create services
				TRAP(err, iBioUtils.CreateServicesL());
				if(err == KErrNone)
				{
					//
					// Instantiate client MTM
					// (this in its self is a test, but must be done before we can do anything else)
					TRAP(err, iBioUtils.InstantiateClientMtmsL());
				}
			}
			nRetryCount--;
		}
		User::LeaveIfNull(iBioUtils.iBioClientMtm);
		User::LeaveIfError(err);
	}
}

void CClientTestHarness::InstantiateServerMtmL()
{
	

	//
	// To speed up creation, share a static connection to server side MTM
	// set that connection up now
	TInt nRetryCount = 5;
	TInt err = KErrGeneral;

	if(iBioUtils.iBioServerMtm == NULL)
	{
		while(iBioUtils.iBioServerMtm == NULL && nRetryCount > 0)
		{
			//
			// Start up server side stuff
			TRAP(err, iBioUtils.GoServerSideL());
			if(err == KErrNone)
			{			
				//
				// Create services
				TRAP(err, iBioUtils.CreateServicesL());
				if(err == KErrNone)
				{
					//
					// Instantiate server MTM
					// (this in its self is a test, but must be done before we can do anything else)
					TRAP(err, iBioUtils.InstantiateServerMtmsL());
				}
			}
			nRetryCount--;
		}
		User::LeaveIfNull(iBioUtils.iBioServerMtm);	// was out
		User::LeaveIfError(err);
	}
}
