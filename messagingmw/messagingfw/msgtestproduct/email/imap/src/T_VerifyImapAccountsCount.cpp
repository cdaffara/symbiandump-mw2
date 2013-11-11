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
// VerifyImapAccountsCount
// [Paramaters]
// ExpectedCount		<input>	: Expected Number of the IMAP Accouts present
// [Test Step Description]
// If number of IMAP accounts present equals with the expected number of accounts then test step result is pass.
// [APIs Used]
// CEmailAccounts::GetImapAccountsL
// 
//



/* User include */
#include "T_VerifyImapAccountsCount.h"


/* Epoc inlcudes */
#include<cemailaccounts.h>


/* Literals used */
_LIT(KExpectedCount,"ExpectedCount");


/**
  Function : CT_MsgVerifyImapAccountsCount
  Description : Constructor
  @return : N/A
*/
CT_MsgVerifyImapAccountsCount::CT_MsgVerifyImapAccountsCount(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KVerifyImapAccountsCount);
	}


/**
  Function : ~CT_MsgVerifyImapAccountsCount
  Description : Destructor
  @return : N/A
*/
CT_MsgVerifyImapAccountsCount::~CT_MsgVerifyImapAccountsCount()
	{
	}


/**
  Function : doTestStepL
  Description : This test step reads the expected count from the ini file
  				Obtains the count of the IMAP accounts created and compares
  				against the expected number.
  @return : TVerdict TestStep result
			EPass ,if the number of account created macth the expected count
			EFail ,if the number of account created do macth the expected count
*/
TVerdict CT_MsgVerifyImapAccountsCount::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: Verify Imap Accounts Count"));
	
	/* Reads the expected number of IMAP account from the ini file */
	TInt expectedCount = -1;
	if(!GetIntFromConfig(ConfigSection(), KExpectedCount, expectedCount) && expectedCount < 0)
		{
		ERR_PRINTF1(_L("ExpectedCount is not Specified or < 0"));
		SetTestStepResult(EFail);
		}			
	/* Gets the IMAP account creates and obtains their count */
	else
		{
		CEmailAccounts *account = CEmailAccounts::NewLC();	
		RArray<TImapAccount> arrayImapAccounts;
		account->GetImapAccountsL(arrayImapAccounts);
		TInt count = arrayImapAccounts.Count();
		
		/* Resets the array of IMAP accounts */
		CleanupStack::PopAndDestroy(account);
		arrayImapAccounts.Reset();
		
		/* 
		 *  Matches the expected count with the actual count 
		 *  If the count does not match, fails the test
		 */
		if (count != expectedCount)
			{
			ERR_PRINTF3(_L("Number of IMAP accounts do not match ! expected = %d actual = %d"), 
									  expectedCount, count );
			SetTestStepResult(EFail);
			}
		/* If the count matches the expected count then the test step passes */
		else
			{
			INFO_PRINTF2(_L("Number of IMAP accounts matched value = %d !"), expectedCount);	
			}
		}	
	return TestStepResult();
	}
	
	
