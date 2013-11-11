// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Internet Mail Settings Parser
// 
//

#include <msvids.h>   // KMsvRootIndexEntryId
#include <msventry.h>
#include <msvruids.h>
#include <smtpset.h>
#include <pop3set.h>
#include <imapset.h>
#include <cemailaccounts.h>
//
#include "BSP.H"
#include "IACP.H"
#include "IMP.H"
#include "IACPDEF.H"
#include "IACPERR.H"
#include <iapprefs.h>

_LIT(KIacpOldImapName, "-Old-IMAP");
_LIT(KIacpOldPop3Name, "-Old-POP");

#include <commdb.h>
#include <ipaddr.h>

//
// Constructor
//
CMailParser::CMailParser()
	{
	}

//
// Factory fns
//
CMailParser* CMailParser::NewLC()
	{
	CMailParser* self=new (ELeave) CMailParser();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CMailParser* CMailParser::NewL()
	{
	CMailParser* self=CMailParser::NewLC();
	CleanupStack::Pop();
	return self;
	}

//
// 2nd stage of construction
//
void CMailParser::ConstructL()
	{
	// --- Create the blank settings objects ---
	iSmtpSettings=new(ELeave) CImSmtpSettings();
	iPop3Settings=new(ELeave) CImPop3Settings();
	iImap4Settings=new(ELeave) CImImap4Settings();
	
	// email account creator
	iEmailAccounts = CEmailAccounts::NewL();
	
	// IAP preferences
	iEmailPreferences = CImIAPPreferences::NewLC();
	CleanupStack::Pop(iEmailPreferences);
	iSmtpPreferences = CImIAPPreferences::NewLC();
	CleanupStack::Pop(iSmtpPreferences);

	// set defaults	
	iEmailAccounts->PopulateDefaultPopSettingsL(*iPop3Settings, *iEmailPreferences);
	iEmailAccounts->PopulateDefaultSmtpSettingsL(*iSmtpSettings, *iSmtpPreferences); 											
	iEmailAccounts->PopulateDefaultImapSettingsL(*iImap4Settings, *iEmailPreferences);
	}

//
// Destruction
//
CMailParser::~CMailParser()
	{
	delete iSmtpSettings;
	delete iPop3Settings;
	delete iImap4Settings;
	delete iEmailAccounts;
	delete iEmailPreferences;
	delete iSmtpPreferences;
	}

void CMailParser::CheckMandatoryFieldsL(CParsedFieldCollection& aIacpFields)
	{
	//Defect fix for Def021855- IACP - Not all fields should be mandatory 
	
	//according to the Smart Messaging Spec v3.0.0 (18/12/00) the only mandatory field is the mail-iap-name
	//all other parameters are optional and so this function will leave only if the mail-iap-name is empty.
	//However this may change and so the spec needs to be watched just in case.
	if(aIacpFields.GetFieldValue(SMS_ISP_M_NAME).Length()==0)		
			User::Leave(KIacpMandatoryDataNotSet);
	}

//
// Parse/Set data members of CImSmtpSettings, CImPop3Settings, CImImap4Settings
//
void CMailParser::ParseL(CParsedFieldCollection& aIacpFields)
	{
	TPtrC aFieldValueBuf;
	TBuf8<KMaxSettingStringLength>	tempBuf; //LoginName, Password, FolderPath

	SetMailProtocolL(aIacpFields);

	//set service name
	if (aIacpFields.GetFieldValueAndLength(SMS_ISP_M_NAME, aFieldValueBuf) != 0)
		iIspName= aFieldValueBuf;

	//smtp settings
	if (aIacpFields.GetFieldValueAndLength(SMS_SENDING_HOST, aFieldValueBuf) != 0)
		iSmtpSettings->SetServerAddressL(aFieldValueBuf);
	
	if (aIacpFields.GetFieldValueAndLength(SMS_USER_EMAIL_ADDR, aFieldValueBuf) != 0)
		iSmtpSettings->SetEmailAddressL(aFieldValueBuf);
	
	if(iProtocolName == EPop3)
		{
		//pop3 settings
		if (aIacpFields.GetFieldValueAndLength(SMS_MAILBOX_NAME, aFieldValueBuf) != 0)
			{
			tempBuf.Copy(aFieldValueBuf); //copy from 16 to 8 bit buffer
			iPop3Settings->SetLoginNameL(tempBuf);
			}

		if (aIacpFields.GetFieldValueAndLength(SMS_MAILBOX_PASS, aFieldValueBuf) != 0)
			{
			tempBuf.Copy(aFieldValueBuf); //copy from 16 to 8 bit buffer
			iPop3Settings->SetPasswordL(tempBuf);
			}
			

		if (aIacpFields.GetFieldValueAndLength(SMS_RECEIVING_HOST, aFieldValueBuf) != 0)
			iPop3Settings->SetServerAddressL(aFieldValueBuf);

		// Set DisconnectedUserMode flag so that messages are not made invisible
		iPop3Settings->SetDisconnectedUserMode(ETrue);	
		}
	else if(iProtocolName == EImap4)
		{
		//imap4 settings
		if (aIacpFields.GetFieldValueAndLength(SMS_MAILBOX_NAME, aFieldValueBuf) != 0)
			{
			tempBuf.Copy(aFieldValueBuf); //copy from 16 to 8 bit buffer
			iImap4Settings->SetLoginNameL(tempBuf);
			}

		if (aIacpFields.GetFieldValueAndLength(SMS_MAILBOX_PASS, aFieldValueBuf) != 0)
			{
			tempBuf.Copy(aFieldValueBuf); //copy from 16 to 8 bit buffer
			iImap4Settings->SetPasswordL(tempBuf);
			}
		
		
		if (aIacpFields.GetFieldValueAndLength(SMS_RECEIVING_HOST, aFieldValueBuf) != 0)
			iImap4Settings->SetServerAddressL(aFieldValueBuf);

		if (aIacpFields.GetFieldValueAndLength(SMS_FOLDER_PATH, aFieldValueBuf) != 0)
			{
			tempBuf.Copy(aFieldValueBuf); //copy from 16 to 8 bit buffer
			iImap4Settings->SetFolderPathL(tempBuf);
			}

		// Set DisconnectedUserMode flag so that messages are not made invisible
		iImap4Settings->SetDisconnectedUserMode(ETrue);
		}
	}

//
// Create smtp, pop3, imap4 services
//
void CMailParser::ProcessL(CMSVENTRY& aEntry)
	{
	
	// check for existing services with the same name
	GetServiceIdsL(iPop3AccountId, iImap4AccountId);

	// check if this account has switched from IMAP to POP or vise-versa
	TBool protChanged = EFalse;
	TBuf<KMaxSettingStringLength> oldIspName;
	oldIspName.Copy(iIspName);
	if (iProtocolName == EPop3 && iImap4AccountId.iImapAccountId != KErrNotFound && 
								  iPop3AccountId.iPopAccountId == KErrNotFound)
		{
		protChanged = ETrue;
		oldIspName.Append(KIacpOldImapName);
		}
	else if (iProtocolName == EImap4 && iPop3AccountId.iPopAccountId != KErrNotFound && 
										iImap4AccountId.iImapAccountId == KErrNotFound)
		{
		protChanged = ETrue;
		oldIspName.Append(KIacpOldPop3Name);
		}

	// create a new account if there is no existing account or the account has been
	// switched from POP to IMAP or IMAP to POP
	if (protChanged || iPop3AccountId.iPopAccountId == KErrNotFound && 
					   iImap4AccountId.iImapAccountId == KErrNotFound)
		{
		// if service swapped rename old service
		if (protChanged)
			{
			CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings;
			CleanupStack::PushL(smtpSettings);
			
			if (iProtocolName == EPop3)
				{
				CImPop3Settings* popSettings = new(ELeave) CImPop3Settings;
				CleanupStack::PushL(popSettings);
				
				// load settings
				iEmailAccounts->LoadPopSettingsL(iPop3AccountId, *popSettings);
				TSmtpAccount smtpAccountId;
				iEmailAccounts->GetSmtpAccountL(iPop3AccountId.iSmtpService, smtpAccountId);
				iEmailAccounts->LoadSmtpSettingsL(smtpAccountId, *smtpSettings);
				
				// rename old service
				iPop3AccountId.iPopAccountName = oldIspName;
				
				// save settings
				iEmailAccounts->SavePopSettingsL(iPop3AccountId, *popSettings); 
				iEmailAccounts->SaveSmtpSettingsL(smtpAccountId, *smtpSettings);
				CleanupStack::PopAndDestroy(popSettings);
				}
			else if (iProtocolName == EImap4)
				{
				CImImap4Settings* imapSettings = new(ELeave) CImImap4Settings;
				CleanupStack::PushL(imapSettings);
				
				// load settings
				iEmailAccounts->LoadImapSettingsL(iImap4AccountId, *imapSettings);
				TSmtpAccount smtpAccountId;
				iEmailAccounts->GetSmtpAccountL(iImap4AccountId.iSmtpService, smtpAccountId);				
				iEmailAccounts->LoadSmtpSettingsL(smtpAccountId, *smtpSettings);

				// rename old service
				iImap4AccountId.iImapAccountName = oldIspName;
				
				// save settings
				iEmailAccounts->SaveImapSettingsL(iImap4AccountId, *imapSettings);
				iEmailAccounts->SaveSmtpSettingsL(smtpAccountId, *smtpSettings);
				CleanupStack::PopAndDestroy(imapSettings);
				}
			CleanupStack::PopAndDestroy(smtpSettings);
			}

		// create new services
		if (iProtocolName == EPop3)
			{
			iPop3AccountId = iEmailAccounts->CreatePopAccountL(iIspName, *iPop3Settings, *iEmailPreferences, EFalse);
			TSmtpAccount smtpAccountId = iEmailAccounts->CreateSmtpAccountL(iPop3AccountId, *iSmtpSettings, *iSmtpPreferences, EFalse);

			aEntry.SetEntryL(iPop3AccountId.iPopService);
			iEmailAccounts->SetDefaultSmtpAccountL(smtpAccountId);
			iImap4AccountId.iImapAccountId = KErrNotFound;
			}
		else if (iProtocolName == EImap4)
			{
			iImap4AccountId = iEmailAccounts->CreateImapAccountL(iIspName, *iImap4Settings, *iEmailPreferences, EFalse);
			TSmtpAccount smtpAccountId = iEmailAccounts->CreateSmtpAccountL(iImap4AccountId, *iSmtpSettings, *iSmtpPreferences, EFalse);
			
			aEntry.SetEntryL(iImap4AccountId.iImapService);
			iEmailAccounts->SetDefaultSmtpAccountL(smtpAccountId);
			iPop3AccountId.iPopAccountId = KErrNotFound;
			}
		}
	else
		{
		// overwrite existing account settings
		if (iProtocolName == EPop3)
			{
			iEmailAccounts->SavePopSettingsL(iPop3AccountId, *iPop3Settings);
			iEmailAccounts->SavePopIapSettingsL(iPop3AccountId, *iEmailPreferences);
			TSmtpAccount smtpAccountId;
			iEmailAccounts->GetSmtpAccountL(iPop3AccountId.iSmtpService, smtpAccountId);							
			iEmailAccounts->SaveSmtpSettingsL(smtpAccountId, *iSmtpSettings);
			iEmailAccounts->SaveSmtpIapSettingsL(smtpAccountId, *iSmtpPreferences);

			aEntry.SetEntryL(iPop3AccountId.iPopService);
			iEmailAccounts->SetDefaultSmtpAccountL(smtpAccountId);
			}
		else if (iProtocolName == EImap4)
			{
			iEmailAccounts->SaveImapSettingsL(iImap4AccountId, *iImap4Settings);
			iEmailAccounts->SaveImapIapSettingsL(iImap4AccountId, *iEmailPreferences);
			TSmtpAccount smtpAccountId;
			iEmailAccounts->GetSmtpAccountL(iImap4AccountId.iSmtpService, smtpAccountId);			
			iEmailAccounts->SaveSmtpSettingsL(smtpAccountId, *iSmtpSettings);
			iEmailAccounts->SaveSmtpIapSettingsL(smtpAccountId, *iSmtpPreferences);

			aEntry.SetEntryL(iImap4AccountId.iImapService);
			iEmailAccounts->SetDefaultSmtpAccountL(smtpAccountId);
			}
		}
	}
	
//
// Get the email account ids for any matching POP/IMAP accounts
//
void CMailParser::GetServiceIdsL(TPopAccount& aPop3AccountId, TImapAccount& aImap4AccountId)
	{
	aPop3AccountId.iPopAccountId = KErrNotFound;
	aImap4AccountId.iImapAccountId = KErrNotFound;
	
	// get list of pop3 accounts
	RArray<TPopAccount> pop3Accounts;
	CleanupClosePushL(pop3Accounts);
	iEmailAccounts->GetPopAccountsL(pop3Accounts);
	for (TInt i=0; i<pop3Accounts.Count(); ++i)
		{
		if (pop3Accounts[i].iPopAccountName.Length() == iIspName.Length())
			{
			if (pop3Accounts[i].iPopAccountName.CompareF(iIspName) == 0)
				{
				aPop3AccountId = pop3Accounts[i];
				break;
				}
			}
		}

	// get list of imap4 accounts	
	RArray<TImapAccount> imap4Accounts;
	CleanupClosePushL(imap4Accounts);
	iEmailAccounts->GetImapAccountsL(imap4Accounts);
	for (TInt j=0; j<imap4Accounts.Count(); ++j)
		{
		if (imap4Accounts[j].iImapAccountName.Length() == iIspName.Length())
			{
			if (imap4Accounts[j].iImapAccountName.CompareF(iIspName) == 0)
				{
				aImap4AccountId = imap4Accounts[j];
				break;
				}
			}
		}

	CleanupStack::PopAndDestroy(2, &pop3Accounts); // imap4Accounts, pop3Accounts
	}

	
	
//
// Gets Types of folder list sync
//
void CMailParser::GetFolderSyncTypeL(const TDesC& aDes,TFolderSyncType& aFolderSyncType) const
	{
	__ASSERT_DEBUG(aDes.Length() > 0 ,User::Panic(KIACP,EIacpEmptyBuffer));
	TInt8 extractInt=0;

	TLex lex(aDes);
    if (lex.Val(extractInt) == KErrNone)
		{
		if(extractInt==0)
			aFolderSyncType = EUseCombination;
		else if(extractInt==1)
			aFolderSyncType = EUseLocal;
		else if(extractInt==2)
			aFolderSyncType = EUseRemote;
		else
			User::Leave(KIacpErrRightToken);
		}
	else
		User::Leave(KIacpErrRightToken);

	}
//
//  Subscription data
//
void CMailParser::GetFolderSubscribeTypeL(const TDesC& aDes,TFolderSubscribeType& aFolderSubscribeType) const
	{
	__ASSERT_DEBUG(aDes.Length() > 0 ,User::Panic(KIACP,EIacpEmptyBuffer));
	TInt8 extractInt=0;

	TLex lex(aDes);
    if (lex.Val(extractInt) == KErrNone)
		{
		if(extractInt==0)
			aFolderSubscribeType = EUpdateNeither;
		else if(extractInt==1)
			aFolderSubscribeType = EUpdateLocal;
		else if(extractInt==2)
			aFolderSubscribeType = EUpdateRemote;
		else if(extractInt==3)
			aFolderSubscribeType = EUpdateBoth;
		else
			User::Leave(KIacpErrRightToken);
		}
	else
		User::Leave(KIacpErrRightToken);
	}

//
// set iMailProtocolName to Pop3 or Imap4
//
void CMailParser::SetMailProtocolL(CParsedFieldCollection& aIacpFields)
	{
	iProtocolName =0;
	TInt count = aIacpFields.Count();

	for ( TInt i=0; i < count; i++ )
		{
		CParsedField& field = *(aIacpFields[i]);
		if(field.FieldName().CompareF(SMS_MAIL_PROTOCOL)==0)
			{	
			if(field.FieldValue().CompareF(SMS_POP)==0)
				{
				iProtocolName = EPop3;
				return;
				}
			else if (field.FieldValue().CompareF(SMS_IMAP)==0)
				{
				iProtocolName = EImap4;
				return;
				}
			}
		}
	User::Leave(KIacpUnknownMailProtocol);
	}

void CMailParser::AssociateIAPWithMailL(CMSVENTRY& /*aEntry*/, TUint32 aId)
	{
	// load settings
	TSmtpAccount smtpAccountId;
	if (iPop3AccountId.iPopAccountId != KErrNotFound)
		{
		iEmailAccounts->LoadPopSettingsL(iPop3AccountId, *iPop3Settings);
		iEmailAccounts->LoadPopIapSettingsL(iPop3AccountId, *iEmailPreferences);

		iEmailAccounts->GetSmtpAccountL(iPop3AccountId.iSmtpService, smtpAccountId);
		iEmailAccounts->LoadSmtpSettingsL(smtpAccountId, *iSmtpSettings);
		iEmailAccounts->LoadSmtpIapSettingsL(smtpAccountId, *iSmtpPreferences);
		}
	else if (iImap4AccountId.iImapAccountId != KErrNotFound)
		{
		iEmailAccounts->LoadImapSettingsL(iImap4AccountId, *iImap4Settings);
		iEmailAccounts->LoadImapIapSettingsL(iImap4AccountId, *iEmailPreferences);

		iEmailAccounts->GetSmtpAccountL(iImap4AccountId.iSmtpService, smtpAccountId);			
		iEmailAccounts->LoadSmtpSettingsL(smtpAccountId, *iSmtpSettings);
		iEmailAccounts->LoadSmtpIapSettingsL(smtpAccountId, *iSmtpPreferences);
		}

	// update IAP preferences
	TBool updated(EFalse);
	TImIAPChoice iap;
	iap.iIAP = aId;
	iap.iDialogPref = ECommDbDialogPrefDoNotPrompt;
	TInt location;
	if (iEmailPreferences->FindIAPL(iap.iIAP, location) == KErrNotFound)
		{
		iEmailPreferences->AddIAPL(iap);
		updated = ETrue;
		}
	if (iSmtpPreferences->FindIAPL(iap.iIAP, location) == KErrNotFound)
		{
		iSmtpPreferences->AddIAPL(iap);
		updated = ETrue;
		}

	// save settings
	if (updated && iPop3AccountId.iPopAccountId != KErrNotFound)
		{
		iEmailAccounts->SavePopIapSettingsL(iPop3AccountId, *iEmailPreferences);
		iEmailAccounts->SaveSmtpIapSettingsL(smtpAccountId, *iSmtpPreferences);
		}
	else if (updated && iImap4AccountId.iImapAccountId != KErrNotFound)
		{
		iEmailAccounts->SaveImapIapSettingsL(iImap4AccountId, *iEmailPreferences);
		iEmailAccounts->SaveSmtpIapSettingsL(smtpAccountId, *iSmtpPreferences);
		}
	}

