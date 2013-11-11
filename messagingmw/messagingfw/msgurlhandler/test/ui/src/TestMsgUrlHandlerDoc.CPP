// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains the implementation for the class defined in 
// TestMsgUrlHandlerApp.h
// 
//

/**
 @file
 @see	TestMsgUrlHandlerApp.h
*/
#include <e32test.h>
#include "smstestutils.h"
#include <cemailaccounts.h>
#include <csmsaccount.h>
#include <pop3set.h>
#include <smtpset.h>
#include <iapprefs.h>

#include "TestMsgUrlHandlerApp.H"

//             The constructor of the document class just passes the
//             supplied reference to the constructor initialisation list.
//             The document has no real work to do in this application.
//

CTestMsgUrlHandlerDocument* CTestMsgUrlHandlerDocument::NewL(CEikApplication& aApp)
    {
    CTestMsgUrlHandlerDocument* self = new (ELeave) CTestMsgUrlHandlerDocument(aApp);
	self->ConstructL();
    return self;
    }

void CTestMsgUrlHandlerDocument::ConstructL()
    {
	// Ensure there are default service settings.
	CreateSMSServiceL();
	CreateEmailServiceL();

    }

void CTestMsgUrlHandlerDocument::CreateSMSServiceL()
	{
	RTest aTest(_L("URL Handler UI Test"));
	CSmsTestUtils* testUtils = CSmsTestUtils::NewL(aTest);
	CleanupStack::PushL(testUtils);
	testUtils->SetEntryL(testUtils->iSmsServiceId);
	CSmsAccount* account = CSmsAccount::NewLC();
	account->LoadSettingsL(*testUtils->iServiceSettings);
	// Remove the default service centres from the service settings
	TInt count = testUtils->iServiceSettings->ServiceCenterCount();
	while( count-- )
		{
		testUtils->iServiceSettings->RemoveServiceCenter(count);
		}
	// Add the Vodafone service centre - store the settings.
	testUtils->iServiceSettings->AddServiceCenterL(_L("Vodafone"), _L("+447785016005"));
	// Set delivery options - do matching and make reports visible.	
	testUtils->iServiceSettings->SetDeliveryReport(ETrue);
	testUtils->iServiceSettings->SetStatusReportHandling(CSmsSettings::EMoveReportToInboxVisibleAndMatch);
	account->SaveSettingsL(*testUtils->iServiceSettings);
	CleanupStack::PopAndDestroy(2, testUtils); // account, testUtils
	aTest.Close();
	}

void CTestMsgUrlHandlerDocument::CreateEmailServiceL()
	{	
	TMsvId popServiceId = 0;
	TRAPD(err, ServiceIdL(KUidMsgTypePOP3, popServiceId));

	if (err)
		{
		CEmailAccounts* account = CEmailAccounts::NewLC();

		CImPop3Settings* popSettings = new(ELeave) CImPop3Settings();
		CleanupStack::PushL(popSettings);
		CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings();
		CleanupStack::PushL(smtpSettings);
		
		CImIAPPreferences* popIAP = CImIAPPreferences::NewLC();
		CImIAPPreferences* smtpIAP = CImIAPPreferences::NewLC();
				
		account->PopulateDefaultPopSettingsL(*popSettings, *popIAP); 
		account->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpIAP);

		_LIT(KSmtpServer, "smtp.dial.pipex.com");
		smtpSettings->SetServerAddressL(KSmtpServer);
		smtpSettings->SetEmailAliasL(_L("Messaging Test1"));
		_LIT(KSmtpEmailAddress, "msgtest1@internet-test.plc.psion.com");
		smtpSettings->SetEmailAddressL(KSmtpEmailAddress);
		smtpSettings->SetReplyToAddressL(KSmtpEmailAddress);
		smtpSettings->SetBodyEncoding(EMsgOutboxMIME);
		smtpSettings->SetReceiptAddressL(KSmtpEmailAddress);
		smtpSettings->SetPort(25);
	
		TPopAccount popAccount= account->CreatePopAccountL(KNullDesC, *popSettings, *popIAP, EFalse);
		
		account->CreateSmtpAccountL(popAccount, *smtpSettings, *smtpIAP, EFalse);
		
		CleanupStack::PopAndDestroy(5, account); // smtpIAP, popIAP, smtpSettings, popSettings, account
		}
	}

void CTestMsgUrlHandlerDocument::ServiceIdL(TUid aMtm, TMsvId& rFirstId)
	{
	//Returns the Service IDs of MTM aMtm
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = session->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(cEntry);

	rFirstId = KMsvNullIndexEntryId;

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	cEntry->SetSortTypeL(order);

	//Get the children on the Root Index Entry
	CMsvEntrySelection* selection = cEntry->ChildrenWithTypeL(KUidMsvServiceEntry);
	CleanupStack::PushL(selection);

	TInt count = selection->Count();

	//Find an entry for MTM aMtm
	for (TInt curChild = 0; curChild < count && (rFirstId == KMsvNullIndexEntryId); curChild++)
		{
		cEntry->SetEntryL(selection->At(curChild));

		if (cEntry->Entry().iMtm == aMtm)
			{
			TMsvId id = cEntry->Entry().Id();

			if (rFirstId == KMsvNullIndexEntryId)
				rFirstId = id;
			}
		}

	//Leave if no Service Entry found for MTM aMtm
	if (rFirstId == KMsvNullIndexEntryId)
		{
		CleanupStack::PopAndDestroy(); //selection
		User::Leave(KErrNotFound);
		}

	CleanupStack::PopAndDestroy(4, ob1); //selection, cMsvEntry, session, ob1
	}

CTestMsgUrlHandlerDocument::CTestMsgUrlHandlerDocument(CEikApplication& aApp)
		: CEikDocument(aApp)
	{
	}


//             This is called by the UI framework as soon as the 
//             document has been created. It creates an instance
//             of the ApplicationUI. The Application UI class is
//             an instance of a CEikAppUi derived class.
//
CEikAppUi* CTestMsgUrlHandlerDocument::CreateAppUiL()
	{
	return new (ELeave) CTestMsgUrlHandlerAppUi;
	}
