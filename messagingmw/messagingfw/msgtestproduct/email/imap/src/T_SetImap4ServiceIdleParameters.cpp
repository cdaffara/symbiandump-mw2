// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file 
// [TestStep Name]
// SetImap4ServiceIdleParameters
// [Paramaters]
// ImapAccountName		:	Name of the IMAP account 
// IsImapIdleSelected	:	Flag to check if IMAP Idle is selected
// ImapIdleTimeout		:	Value of the IMAP idle timeout
// ImapSyncRate			:	Value of the IMAP sync rate
// [TestStep Description]
// Sets parameters of Imap4 service
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::EditStoreL
// CImImap4Settings::Reset
// CImImap4Settings::SetServerAddressL
// CImImap4Settings::SetLoginNameL
// CImImap4Settings::SetPasswordL
// CImImap4Settings::StoreL
// CMsvStore::CommitL
// 
//



/* User includes */
#include "T_SetImap4ServiceIdleParameters.h"
#include <t_utilscentralrepository.h>


/* Epoc includes */
#include <cemailaccounts.h>
#include <imapset.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KIsImapIdleSelected,"IsImapIdleSelected");
_LIT(KImapIdleTimeout,"ImapIdleTimeout");
_LIT(KImapSyncRate,"ImapSyncRate");


/**
  Function : CT_MsgSetImap4SetImap4ServiceIdleParamters
  Description : Constructor
  @param	:	aSharedDataIMAP		Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgSetImap4SetImap4ServiceIdleParamters::CT_MsgSetImap4SetImap4ServiceIdleParamters(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KSetImap4ServiceIdleParamters);
	}


/**
  Function : ~CT_MsgSetImap4SetImap4ServiceIdleParamters
  Description : Destructor
  @return : N/A
*/
CT_MsgSetImap4SetImap4ServiceIdleParamters::~CT_MsgSetImap4SetImap4ServiceIdleParamters()
	{
	}


	
/**
  Function : doTestStepL
  Description : Sets the IMAP service parameters like Imap Idle,Imap sync rate, Imap timeout
  @return : TVerdict - Test step result
  @leave :	KMsvNullIndexEntryId	Invalid IMAP account name specified
*/
TVerdict CT_MsgSetImap4SetImap4ServiceIdleParamters::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : SetImap4ServiceIdleParameters"));
	
	/* Reads the IMAP account name from the ini file */
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("ImapAccount Name is not specified"));
		SetTestStepResult(EFail);
		}		
	else
		{
		/* Retrieves the Imap service Id for the given Imap account */
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );

		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name specified"));
			SetTestStepResult(EFail);
			}
		/* Reads if Imap Idle is Selected */
		else
			{
			TBool isImapIdleSelected = ETrue;
			GetBoolFromConfig(ConfigSection(), KIsImapIdleSelected, isImapIdleSelected);
			
			/* Default values */
			TInt imapIdleTimeout = 1680;
			GetIntFromConfig(ConfigSection(),KImapIdleTimeout,imapIdleTimeout);

			/* Default values */
			TInt imapSyncRate = 300;
			GetIntFromConfig(ConfigSection(),KImapSyncRate,imapSyncRate);

			CImImap4Settings* settings = new(ELeave) CImImap4Settings();
			CleanupStack::PushL(settings);

 			CEmailAccounts *accounts = CEmailAccounts::NewLC();

 			TImapAccount imapAccount;
 			accounts->GetImapAccountL(imapServiceId, imapAccount);
			accounts->LoadImapSettingsL(imapAccount, *settings);

			/* Sets the options like timeout perios, sync rate etc */
			settings->SetImapIdle(isImapIdleSelected);
			settings->SetImapIdleTimeoutL(imapIdleTimeout);
			settings->SetSyncRateL(imapSyncRate);

			/* Saves these settings */
			accounts->SaveImapSettingsL(imapAccount, *settings);

			CleanupStack::PopAndDestroy(2, settings);    /* settings,accounts */
			}
		}
	return TestStepResult();
	}
		
		
					
