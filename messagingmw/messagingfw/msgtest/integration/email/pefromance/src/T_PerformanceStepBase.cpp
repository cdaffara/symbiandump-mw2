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

#include "T_PerformanceStepBase.h"
#include <hal_data.h>
#include <hal.h>
#include <profiler.h>
#include <cemailaccounts.h>

_LIT(KOutServer,"outserver");
_LIT(KInServer,"inserver");
_LIT(KUserName,"username");
_LIT(KPassword,"password");
_LIT(KPerformanceTest,"t_performance");
_LIT(KProfiler,"profiler");
_LIT(KStart,"start");
_LIT(KTestMessageDir,"");


RTest test(KPerformanceTest);

HBufC8* CPerformanceStepBase::CopyToBuf8LC(const TDesC& aDesC)
    {
    HBufC8* itemBuff8 = HBufC8::NewLC(aDesC.Length());
    itemBuff8->Des().Copy(aDesC);
    return itemBuff8;
    }
	
void CPerformanceStepBase::GetProfileDetailsFromIniFileL()
	{
	if(!GetBoolFromConfig(ConfigSection(),_L("Profile"),iProfile))
		{
		ERR_PRINTF1(_L("Error reading profiline option from ini file"));
		User::Leave(KErrArgument);	
		}
	}
	
void CPerformanceStepBase::LoadProfiler()
	{
	if (Profiler::Start() == KErrNotFound)
		{
		RProcess proc;
		if (proc.Create(KProfiler,KStart) == KErrNone)
			{
			proc.Resume();
			proc.Close();
			}	
		}
	}
	
void CPerformanceStepBase::StartTimer()
	{
	iStartCount = User::FastCounter();		
	}

void CPerformanceStepBase::StopTimer()
	{
	iStopCount = User::FastCounter();
	}
	
CPerformanceStepBase::~CPerformanceStepBase()
	{
	test.Close();
	delete iConsole;
	delete iTestUtils;
	delete iScheduler;
	}

CPerformanceStepBase::CPerformanceStepBase()
	{
	iProfile=EFalse;
	}


void CPerformanceStepBase::TestComplete(TInt aErrorCode)
	{
	//if the spoof server has reached the end of the script file, there may not be a line feed.
	//in this case let RunL complete before stopping the scheduler
	if(aErrorCode==KErrEof || aErrorCode==KErrNotFound)
		{
		return;
		}
	CActiveScheduler::Stop();	
	if(aErrorCode)
		{
		ERR_PRINTF2(_L("Error: %d"),aErrorCode);	
		}
	}
	
	
void CPerformanceStepBase::WriteStoreL()
	{
	TParse parsedFileName;
	TFileName name(KTestMessageDir);
	iTestUtils->ResolveLogFile(name, parsedFileName);
	iTestUtils->FindChildrenL(KMsvRootIndexEntryIdValue, parsedFileName.FullName(), ETrue, EFalse);
	}
	

TInt CPerformanceStepBase::GetEntryCount()
	{
	TMsvSelectionOrdering ordering;	
	ordering.SetShowInvisibleEntries(ETrue);
	//open the imap service entry
	CMsvEntry* imapService = CMsvEntry::NewL(*iSession,iImapAccount.iImapService,ordering);
	CleanupStack::PushL(imapService);
	//get its children
	CMsvEntrySelection* msvEntrySelection;
	msvEntrySelection=imapService->ChildrenL();
	//open its child inbox entry
	CMsvEntry* inboxEntry = CMsvEntry::NewL(*iSession, (*msvEntrySelection)[0],ordering);
    CleanupStack::PushL(inboxEntry);
    //get the children of the inbox
    delete msvEntrySelection;
    msvEntrySelection=NULL;
    msvEntrySelection=inboxEntry->ChildrenL();
    TInt count=msvEntrySelection->Count();
    delete msvEntrySelection;
    msvEntrySelection=NULL;	
	CleanupStack::PopAndDestroy(2,imapService);

	return count;
	}
	
void CPerformanceStepBase::CreateAccountsL()
	{
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();	
	
	//get the imap server address from the ini file
	TPtrC ptrInServer;
	if(!GetStringFromConfig(ConfigSection(), KInServer, ptrInServer))
		{
		ERR_PRINTF1(_L("Error reading imap server address from ini file"));
		User::Leave(KErrArgument);	
		}
		
	//get the smtp server address from the ini file
	TPtrC ptrOutServer;
	if(!GetStringFromConfig(ConfigSection(), KOutServer, ptrOutServer))
		{
		ERR_PRINTF1(_L("Error reading smtp server address from ini file"));
		User::Leave(KErrArgument);	
		}
			
	//get the username from the ini file
	TPtrC ptrUserName;
	if(!GetStringFromConfig(ConfigSection(), KUserName, ptrUserName))
		{
		ERR_PRINTF1(_L("Error reading username from ini file"));
		User::Leave(KErrArgument);	
		}
	
	//get password from the ini file
	TPtrC ptrPassword;
	if(!GetStringFromConfig(ConfigSection(), KPassword, ptrPassword))
		{
		ERR_PRINTF1(_L("Error reading password from ini file"));
		User::Leave(KErrArgument);	
		}
	
	//get email address
	TPtrC ptrEmailaddress;
	if(!GetStringFromConfig(ConfigSection(), KEmailAddress, ptrEmailaddress))
		{
		ERR_PRINTF1(_L("Error reading email address from ini file"));
		User::Leave(KErrArgument);	
		}
		
	//create objects and initialise with the defaults
	CImImap4Settings* imap4Settings=new(ELeave)CImImap4Settings;
	CleanupStack::PushL(imap4Settings);
	CImIAPPreferences* imapIap = CImIAPPreferences::NewLC();
		
	//create the imap account
	iImapAccount = emailAccounts->CreateImapAccountL(_L("TestAccount"), *imap4Settings, *imapIap, EFalse);	
	emailAccounts->PopulateDefaultImapSettingsL(*imap4Settings, *imapIap);
	
//	TImIAPChoice iap;
  //  iap.iIAP = 14;       // COMMDB_ID 1
 //   iap.iDialogPref = ECommDbDialogPrefDoNotPrompt;
 //   imapIap->AddIAPL(iap);
    
	//override some of the defaults
	HBufC8*	userName=CopyToBuf8LC(ptrUserName);
	imap4Settings->SetLoginNameL(userName->Des());
	HBufC8*	password=CopyToBuf8LC(ptrPassword);
	imap4Settings->SetPasswordL(password->Des());
	imap4Settings->SetServerAddressL(ptrInServer);
	imap4Settings->SetFolderPathL(_L8(""));
	imap4Settings->SetSynchronise(EUseLocal);
	emailAccounts->SaveImapSettingsL(iImapAccount, *imap4Settings);
//	emailAccounts->SaveImapIapSettingsL(iImapAccount, *imapIap);


	//create the smtp account	
	CImSmtpSettings* smtpSettings=new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSettings);
	CImIAPPreferences* smtpIap = CImIAPPreferences::NewLC();
	iSmtpAccount=emailAccounts->CreateSmtpAccountL(_L("SMTP"),*smtpSettings, *smtpIap, EFalse);
	emailAccounts->SetDefaultSmtpAccountL(iSmtpAccount);
	//override default settings
	smtpSettings->SetServerAddressL(ptrOutServer);
	smtpSettings->SetEmailAddressL(ptrEmailaddress);
	smtpSettings->SetLoginNameL(userName->Des());
	smtpSettings->SetPasswordL(password->Des());
	smtpSettings->SetSMTPAuth(ETrue);
	emailAccounts->SaveSmtpSettingsL(iSmtpAccount, *smtpSettings);
	CleanupStack::PopAndDestroy(7,emailAccounts); 
	}

TVerdict CPerformanceStepBase::doTestStepPreambleL()
	{
	SetTestStepResult(EPass);
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);
	iConsole=Console::NewL(KPerformanceTest,TSize(KConsFullScreen,KConsFullScreen));
	
	iTestUtils = CEmailTestUtils::NewL(test);
	iTestUtils->FileSession().SetSessionPath(_L("C:\\"));
	iTestUtils->CleanMessageFolderL();
	iTestUtils->ClearEmailAccountsL();
	iTestUtils->GoClientSideL();	

	//find out if we are profiling or not
	GetProfileDetailsFromIniFileL();
	CreateAccountsL();

	iSessionObserver = new (ELeave) TDummySessionObserver;
	iSession=CMsvSession::OpenSyncL(*iSessionObserver);
	
	//first body file
	TPtrC emailBodyFile1;
	if(!GetStringFromConfig(ConfigSection(), KEmailBodyFile1, emailBodyFile1))
		{
		ERR_PRINTF1(_L("Error reading email body file from ini file"));
		User::Leave(KErrArgument);	
		}
	iEmailBodyFile1.Append(emailBodyFile1);
	
	//second body file
	TPtrC emailBodyFile2;	
	if(!GetStringFromConfig(ConfigSection(), KEmailBodyFile2, emailBodyFile2))
		{
		ERR_PRINTF1(_L("Error reading email body file from ini file"));
		User::Leave(KErrArgument);	
		}
	iEmailBodyFile2.Append(emailBodyFile2);
	
	//first attachment file
	TPtrC attachmentFile1;	
	if(!GetStringFromConfig(ConfigSection(), KAttachmentFile1, attachmentFile1))
		{
		ERR_PRINTF1(_L("Error reading attachment file from ini file"));
		User::Leave(KErrArgument);	
		}
	iAttachmentFile1.Append(attachmentFile1);
	
	//second attachment file
	TPtrC attachmentFile2;	
	if(!GetStringFromConfig(ConfigSection(), KAttachmentFile2, attachmentFile2))
		{
		ERR_PRINTF1(_L("Error reading attachment file from ini file"));
		User::Leave(KErrArgument);	
		}
	iAttachmentFile2.Append(attachmentFile2);
	
	//email address
	if(!GetStringFromConfig(ConfigSection(), KEmailAddress, iPtrEmailaddress))
		{
		ERR_PRINTF1(_L("Error reading email address from ini file"));
		User::Leave(KErrArgument);	
		}
	
	//subject
	TPtrC subject;	
	if(!GetStringFromConfig(ConfigSection(), KSubject, subject))
		{
		ERR_PRINTF1(_L("Error reading subject from ini file"));
		User::Leave(KErrArgument);	
		}
	iSubject.Append(subject);
	
	return TestStepResult();
	}

void CPerformanceStepBase::InitialisationComplete(TInt aErrorCode)
	{
	if(aErrorCode)
		{
		ERR_PRINTF2(_L("InitialisationComplete Error: %d"),aErrorCode);	
		}
	CActiveScheduler::Stop();	
	}
