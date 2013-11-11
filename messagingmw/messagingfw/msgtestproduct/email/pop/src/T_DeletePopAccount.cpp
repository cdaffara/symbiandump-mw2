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
// [TestStep Name]
// DeletePopAccount
// [Paramaters]
// PopAccountName		<input>	: Name of the Pop account to be Deleted
// For deleteing all the Pop accounts "all" can be used 
// and it is case insensitive.
// [Test Step Description]
// Deletes the Pop account(s) in the Central Repository 
// [APIs Used]
// CEmailAccounts::DeletePopAccountL
// 
//

/**
 @file
*/


// User include
#include "T_DeletePopAccount.h"


// epoc inlcudes
#include<cemailaccounts.h>

// Literals used
_LIT(KPopAccountName,"PopAccountName");


/**
CT_MsgDeletePopAccount()
Sets the teststep name to KDeletePopAccount
  
@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session
*/
CT_MsgDeletePopAccount::CT_MsgDeletePopAccount(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgSyncStepPOP(aSharedDataPOP) 
	{
	SetTestStepName(KDeletePopAccount);
	}

/**
~CT_MsgDeletePopAccount()
Destructor
*/
CT_MsgDeletePopAccount::~CT_MsgDeletePopAccount()
	{
	}



/**
doTestStepL()
Reads the POP account name from the ini file. If ALL string is mentioned in the .ini file 
it deletes all the IMAP accounts, Else deletes the IMAP account whose name is 
mentioned in the ini file

@return
Returns the test step result
*/
TVerdict CT_MsgDeletePopAccount::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: Delete Pop Account"));
	TPtrC popAccountName;
	// Read Pop Account Name from ini file
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("Account name not specified"));
		SetTestStepResult(EFail);
		}
	else
		{	
		CEmailAccounts* account = CEmailAccounts::NewLC();
		
		RArray<TPopAccount> arrayPop3Accounts;
		CleanupClosePushL(arrayPop3Accounts);
		account->GetPopAccountsL(arrayPop3Accounts);
		TInt count=arrayPop3Accounts.Count();
		
		TBuf<12> temp(popAccountName);
		temp.UpperCase();	// Making case insensitive
		if(temp.CompareC(_L("ALL")) == 0)
			{
			INFO_PRINTF2(_L("Deleting all accounts. Total = %d"), count);
			for(TInt i=0; i<count; i++)
				{
				TPopAccount id = arrayPop3Accounts[i];
				account->DeletePopAccountL(id);
				}
			}			
		else 
			{
			TBool deleteFlag = EFalse;
			for(TInt i = 0; i < count; i++)
				{
				if(popAccountName.CompareC(arrayPop3Accounts[i].iPopAccountName) == 0)
					{
					account->DeletePopAccountL(arrayPop3Accounts[i]);
					// Just ensure that if we try to delete it again we hopefuly get an error
					// or can see that it has been deleted.
					arrayPop3Accounts.Remove(i);
					deleteFlag = ETrue;
					break;					
					}
				}
			if(deleteFlag)
				{
				INFO_PRINTF2(_L("Pop acount \" %S \" deleted"), &popAccountName);	
				}
			else
				{
				ERR_PRINTF2(_L("Pop acount \" %S \" not found"), &popAccountName);
				SetTestStepResult(EFail);
				User::Leave(KErrNotFound);
				}
			}
		CleanupStack::PopAndDestroy(2, account);	//arrayPop3Accounts,account
		}
	return TestStepResult();
	}

