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
// DeleteImapAccount
// [Paramaters]
// ImapAccountName		:		  Name of the Imap account to be Deleted
// For deleteing all the IMAP accounts "all" can be used 
// and it is case insensitive.
// [Test Step Description]
// Deletes the IMAP account(s) in the Central Repository 
// [APIs Used]
// CEmailAccounts::DeleteImapAccountL
// 
//



/* User include */
#include "T_DeleteImapAccount.h"


/* epoc inlcudes */
#include <cemailaccounts.h>
#include <miutset.h>

/* Literals used */
_LIT(KImapAccountName,"ImapAccountName");
const TInt KZero={0};


/**
  Function		: CT_MsgDeleteImapAccount
  Description	: Constructor
  @param : aSharedDataIMAP		Reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgDeleteImapAccount::CT_MsgDeleteImapAccount(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KDeleteImapAccount);
	}


/**
  Function		: ~CT_MsgDeleteImapAccount
  Description	: Destructor
  @param :
  @return : N/A
*/
CT_MsgDeleteImapAccount::~CT_MsgDeleteImapAccount()
	{
	}


/**
  Function : doTestStepL
  Description : Reads the IMAP account name from the ini file.
				If ALL string is mentioned in the .ini file it deletes all the 
				IMAP accounts.Else deletes the IMAP account whose name is 
				mentioned in the ini file
  @internalTechnology
  @param :
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgDeleteImapAccount::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: Delete Imap Account"));
	
	/* Read Imap Account Name from ini file */
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Account name not specified"));
		SetTestStepResult(EFail);
		}
	/* Gets the IMAP account created*/
	else
		{	
		CEmailAccounts* account = CEmailAccounts::NewLC();
		
		RArray<TImapAccount> arrayImap4Accounts;
		CleanupClosePushL(arrayImap4Accounts);
		
		account->GetImapAccountsL(arrayImap4Accounts);
		TInt count=arrayImap4Accounts.Count();
		
		TBuf<KMaxSettingStringLength> temp(imapAccountName);
		temp.UpperCase();
		
		/* 
		 * Makes the account name case insensitive and checks if it is ALL
		 * If yes, then deletes all the accounts in the IMAP account array
		 */
		if(temp.CompareC(_L("ALL")) == KZero)
			{
			INFO_PRINTF2(_L("Deleting all accounts. Total = %d"), count );
			for(TInt i=0; i<count; i++)
				{
				TImapAccount id = arrayImap4Accounts[i];
				account->DeleteImapAccountL(id);
				}
			}
		else 
			{
			TBool deleteFlag = EFalse;
			for(TInt i = 0; i < count; i++)
				{
				if(imapAccountName.Compare(arrayImap4Accounts[i].iImapAccountName) == 0)
					{
					account->DeleteImapAccountL(arrayImap4Accounts[i]);
					/* Just ensure that if we try to delete it again we hopefuly get an error
					 * or can see that it has been deleted.
					 */
					arrayImap4Accounts.Remove(i);
					deleteFlag = ETrue;
					break;					
					}
				}
			if(deleteFlag)
				{
				INFO_PRINTF2(_L("IMAP acount \" %S \" deleted"), &imapAccountName);	
				}
			else
				{
				ERR_PRINTF2(_L("IMAP acount \" %S \" not found"), &imapAccountName);
				SetTestStepResult(EFail);
				}
			}
		CleanupStack::PopAndDestroy(2, account);  /* arrayImap4Accounts,account */
		}
	return TestStepResult();
	}
		
	
	

