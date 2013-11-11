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
// ComparePopSettings
// [Action Parameters]
// PopSettings1			input		CImPop4Settings
// PopIAP1				input		CImIAPPreferences
// SMTPSettings1		input		CImSMTPSettings
// SmtpIAP1				input		CImIAPPreferences
// PopSettings2			input		CImPop4Settings
// PopIAP2				input		CImIAPPreferences
// SMTPSettings2		input		CImSMTPSettings
// SmtpIAP2				input		CImIAPPreferences
// [Action Description]
// ComparePopSettings Test Action is intended to compare the POP account settings.
// [APIs Used]	
// CImPop3Settings::ServerAddress
// CImPop3Settings::LoginName
// CImPop3Settings::Password
// CImPop3Settings::MaxEmailSize
// CImPop3Settings::Port
// CImPop3Settings::InboxSynchronisationLimit
// CImPop3Settings::UserAddress
// CImPop3Settings::AutoSendOnConnect
// CImPop3Settings::Apop
// CImPop3Settings::DisconnectedUserMode
// CImPop3Settings::DeleteEmailWhenDisconnecting
// CImPop3Settings::AcknowledgeReceipts
// CImPop3Settings::GetMailOptions
// CImPop3Settings::PopulationLimit
// CImPop3Settings::SecureSockets
// CImPop3Settings::Version
// CImSmtpSettings::ServerAddressL
// CImSmtpSettings::EmailAddressL
// CImSmtpSettings::Version
// CImSmtpSettings::UserAddress
// CImSmtpSettings::Port
// CImSmtpSettings::BodyEncoding
// CImSmtpSettings::DefaultMsgCharSet
// CImSmtpSettings::ReceiptAddress
// CImSmtpSettings::RequestReceipts
// CImSmtpSettings::SendMessageOption
// CImSmtpSettings::EmailAlias
// CImIAPPreferences::Version
// __ACTION_INFO_END__
// 
//


#include <pop3set.h>
#include <smtpset.h>
#include <iapprefs.h>

#include "CMtfTestActionComparePopSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsPopScripts.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionComparePopSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionComparePopSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionComparePopSettings* self = new (ELeave) CMtfTestActionComparePopSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionComparePopSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionComparePopSettings::CMtfTestActionComparePopSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionComparePopSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionComparePopSettings::~CMtfTestActionComparePopSettings()
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
void CMtfTestActionComparePopSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionComparePopSettings);
	CImPop3Settings* popSettings1 = ObtainParameterReferenceL<CImPop3Settings>(TestCase(), ActionParameters().Parameter(0));
	CImPop3Settings* popSettings2 = ObtainParameterReferenceL<CImPop3Settings>(TestCase(), ActionParameters().Parameter(1));
	CImIAPPreferences* popPrefs1 = ObtainParameterReferenceL<CImIAPPreferences>(TestCase(), ActionParameters().Parameter(2));
	CImIAPPreferences* popPrefs2 = ObtainParameterReferenceL<CImIAPPreferences>(TestCase(), ActionParameters().Parameter(3));
	CImSmtpSettings* smtpSettings1 = ObtainParameterReferenceL<CImSmtpSettings>(TestCase(), ActionParameters().Parameter(4));
	CImSmtpSettings* smtpSettings2 = ObtainParameterReferenceL<CImSmtpSettings>(TestCase(), ActionParameters().Parameter(5));
	CImIAPPreferences* smtpPrefs1 = ObtainParameterReferenceL<CImIAPPreferences>(TestCase(), ActionParameters().Parameter(6));
	CImIAPPreferences* smtpPrefs2 = ObtainParameterReferenceL<CImIAPPreferences>(TestCase(), ActionParameters().Parameter(7));
	
	// final param indicates whether we expect the input params to match
	// a non-zero value indicates we expect a match
	TInt matchExpected = ObtainValueParameterL<TInt>(TestCase(), ActionParameters().Parameter(8));
	
	TVerdict result = EPass;
	if ( ! ( *popSettings1 == *popSettings2 ) )
		{
		// failed
		TestCase().ERR_PRINTF1(_L("POP3 Settings objects do not match !"));
		result = EFail;
		}
	else if(!(*smtpSettings1 == *smtpSettings2))
		{
		// failed
		TestCase().ERR_PRINTF1(_L("SMTP Settings objects do not match !"));
		result = EFail;
		}
	else if (!CompareIapPrefs(*popPrefs1, *popPrefs2))
		{
		// failed
		TestCase().ERR_PRINTF1(_L("POP3 IAP Preference objects do not match !"));
		result = EFail;
		}
	else if (!CompareIapPrefs(*smtpPrefs1, *smtpPrefs2))
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
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionComparePopSettings);	
	TestCase().ActionCompletedL(*this);
	}

TBool CMtfTestActionComparePopSettings::CompareIapPrefs(CImIAPPreferences& prefs1, CImIAPPreferences& prefs2) const
	{
	// Compare version
	if (prefs1.Version() != prefs2.Version())
		{
		return EFalse;
		}
	
	// Compare number of IAPs stored
	TInt numIAPs1 = prefs1.NumberOfIAPs();
	
	if (numIAPs1 != prefs2.NumberOfIAPs())
		{
		return EFalse;
		}
	
	// Compare individual IAPs
	for (TInt i = 0; i < numIAPs1; i++)
		{
		if (prefs1.IAPPreference(i).iIAP != prefs2.IAPPreference(i).iIAP ||
			prefs1.IAPPreference(i).iDialogPref != prefs2.IAPPreference(i).iDialogPref)
			{
			return EFalse;
			}
		}
	
	return ETrue;
	}
	
