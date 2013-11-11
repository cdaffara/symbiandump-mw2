// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// CompareImapSettings
// [Action Parameters]
// Input 	Input Type
// ImapSettings1		input		CImImap4Settings
// ImapIAP1				input		CImIAPPreferences
// SMTPSettings1		input		CImSMTPSettings
// SmtpIAP1				input		CImIAPPreferences
// ImapSettings2		input		CImImap4Settings
// ImapIAP2				input		CImIAPPreferences
// SMTPSettings2		input		CImSMTPSettings
// SmtpIAP2				input		CImIAPPreferences
// [Action Description]
// CompareImapSettings Test Action is intended to compare the IMAP account settings.
// [APIs Used]	
// CImImap4Settings::ServerAddress
// CImImap4Settings::UserAddress
// CImImap4Settings::LoginName
// CImImap4Settings::Password
// CImImap4Settings::Port
// CImImap4Settings::SecureSockets
// CImImap4Settings::Version
// CImImap4Settings::FolderPath
// CImImap4Settings::PathSeparator
// CImImap4Settings::MaxEmailSize
// CImImap4Settings::GetMailOptions
// CImImap4Settings::InboxSynchronisationLimit
// CImImap4Settings::MailboxSynchronisationLimit
// CImSmtpSettings::ServerAddress
// CImSmtpSettings::EmailAddress
// CImSmtpSettings::Version
// CImSmtpSettings::UserAddress
// CImSmtpSettings::Port
// CImSmtpSettings::BodyEncoding
// CImSmtpSettings::DefaultMsgCharSet
// CImSmtpSettings::ReceiptAddress
// CImIAPPreferences::Version
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionCompareImapSettings.h"


#include <imapset.h>
#include <smtpset.h>
#include <iapprefs.h>

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "CMtfTestActionUtilsImapScripts.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestAction<> object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCompareImapSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareImapSettings* self = new (ELeave) CMtfTestActionCompareImapSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestAction<>
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCompareImapSettings::CMtfTestActionCompareImapSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestAction<>
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCompareImapSettings::~CMtfTestActionCompareImapSettings()
	{
	}	
	
/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionCompareImapSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareImapSettings);
	CImImap4Settings* imap4Settings1 = ObtainParameterReferenceL<CImImap4Settings>(TestCase(), ActionParameters().Parameter(0));
	CImImap4Settings* imap4Settings2 = ObtainParameterReferenceL<CImImap4Settings>(TestCase(), ActionParameters().Parameter(1));
	CImIAPPreferences* imap4Prefs1 = ObtainParameterReferenceL<CImIAPPreferences>(TestCase(), ActionParameters().Parameter(2));
	CImIAPPreferences* imap4Prefs2 = ObtainParameterReferenceL<CImIAPPreferences>(TestCase(), ActionParameters().Parameter(3));
	CImSmtpSettings* smtpSettings1 = ObtainParameterReferenceL<CImSmtpSettings>(TestCase(), ActionParameters().Parameter(4));
	CImSmtpSettings* smtpSettings2 = ObtainParameterReferenceL<CImSmtpSettings>(TestCase(), ActionParameters().Parameter(5));
	CImIAPPreferences* smtpPrefs1 = ObtainParameterReferenceL<CImIAPPreferences>(TestCase(), ActionParameters().Parameter(6));
	CImIAPPreferences* smtpPrefs2 = ObtainParameterReferenceL<CImIAPPreferences>(TestCase(), ActionParameters().Parameter(7));

	// final param indicates whether we expect the input params to match
	// a non-zero value indicates we expect a match
	TInt matchExpected = ObtainValueParameterL<TInt>(TestCase(), ActionParameters().Parameter(8));
	
	TVerdict result = EPass;
	
	if (!(*imap4Settings1 == *imap4Settings2))   
		{
		// failed
		TestCase().ERR_PRINTF1(_L("IMAP4 Settings objects do not match !"));
		result = EFail;
		}
	else if (!(*smtpSettings1 == *smtpSettings2))
		{
		// failed
		TestCase().ERR_PRINTF1(_L("SMTP Settings objects do not match !"));
		result = EFail;
		}
	else if (!CMtfTestActionUtilsImapScripts::CompareIapPrefs(*imap4Prefs1, *imap4Prefs2))
		{
		// failed
		TestCase().ERR_PRINTF1(_L("IMAP4 IAP Preference objects do not match !"));
		result = EFail;
		}
	else if (!CMtfTestActionUtilsImapScripts::CompareIapPrefs(*smtpPrefs1, *smtpPrefs2))
		{
		// failed
		TestCase().ERR_PRINTF1(_L("SMTP IAP Preference objects do not match !"));
		result = EFail;
		}
	
	// if the parameters did not match and we expected them to then fail the test			
	if( result == EFail && matchExpected != 0 )
		{
		TestCase().ERR_PRINTF1(_L("Expected to match but failed comparison"));
		TestCase().SetTestStepResult(EFail);	
		}
	
	// if the parameters matched and we didn't expected them to then fail the test
	if( result == EPass && matchExpected == 0)
		{
		TestCase().ERR_PRINTF1(_L("Expected to fail comparison but matched"));
		TestCase().SetTestStepResult(EFail);	
		}
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareImapSettings);
	TestCase().ActionCompletedL(*this);
	}



