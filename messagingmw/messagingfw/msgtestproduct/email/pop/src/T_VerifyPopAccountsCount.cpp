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
// VerifyPopAccountsCount
// [Paramaters]
// ExpectedCount		<input>	: Expected Number of the POP Accouts present
// [Test Step Description]
// If number of POP accounts present equals with the expected number of accounts then test step result is pass.
// [APIs Used]
// CEmailAccounts::GetPopAccountsL
// 
//

/**
 @file
*/


// User include
#include "T_VerifyPopAccountsCount.h"


// epoc inlcudes
#include<cemailaccounts.h>


// Literals used
_LIT(KExpectedCount,"ExpectedCount");


/**
CT_MsgVerifyPopAccountsCount()
Sets the teststep name to KCheckChildrenCountPop

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Sessoin
*/
CT_MsgVerifyPopAccountsCount::CT_MsgVerifyPopAccountsCount(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgSyncStepPOP(aSharedDataPOP) 
	{
	SetTestStepName(KVerifyPopAccountsCount);
	}


/**
~CT_MsgVerifyPopAccountsCount()
Destructor
*/
CT_MsgVerifyPopAccountsCount::~CT_MsgVerifyPopAccountsCount()
	{
	}


/**
doTestStepL()
Reads the expected count from the ini file.
Obtains the count of the POP accounts created and compares against the expected number.

@return
Returns the test step result.
*/
TVerdict CT_MsgVerifyPopAccountsCount::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: Verify Pop Accounts Count"));
	TInt expectedCount = -1;
	if(!GetIntFromConfig(ConfigSection(), KExpectedCount, expectedCount) && expectedCount < 0)
		{
		ERR_PRINTF1(_L("ExpectedCount is not Specified or < 0"));
		SetTestStepResult(EFail);
		}
	else
		{
		CEmailAccounts *account = CEmailAccounts::NewLC();	
		RArray<TPopAccount> arrayPopAccounts;
		account->GetPopAccountsL(arrayPopAccounts);
		TInt count = arrayPopAccounts.Count();
		
		CleanupStack::PopAndDestroy(account);
		arrayPopAccounts.Reset();
		
		if (count != expectedCount)
			{
			ERR_PRINTF3(_L("Number of POP accounts do not match ! expected = %d actual = %d"), 
									  expectedCount, count);
			SetTestStepResult(EFail);
			}
		else
			{
			INFO_PRINTF2(_L("Number of POP accounts matched value = %d !"), expectedCount);	
			}			
		}
	return TestStepResult();
	}

