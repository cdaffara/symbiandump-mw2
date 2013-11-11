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
// DeleteImapAccounts
// [Action Parameters]
// accountIds	input 	TImapAccounts
// Index	TInt
// Note : TImapAccounts is defined as typedef RArray<TIMAP4Account> TImapAccounts;
// see TMtfTestParameterType.h for details
// [Action Description]
// DeleteImapAccounts Test Action is intended to delete IMAP accounts from the 
// Central Repository. This Test Action takes TIMAP4Account array and an index 
// value as input parameters.  If an integer value is provided for the index, 
// the IMAP account at the specified index will be deleted.  In case of default 
// value provided as index, all the IMAP accounts will be deleted.
// [APIs Used]	
// CEmailAccounts::DeleteImapAccountL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionDeleteImapAccounts.h"

#include <cemailaccounts.h>


#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionDeleteImapAccounts object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionDeleteImapAccounts::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteImapAccounts* self = new (ELeave) CMtfTestActionDeleteImapAccounts(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionDeleteImapAccounts
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionDeleteImapAccounts::CMtfTestActionDeleteImapAccounts(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionDeleteImapAccounts
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionDeleteImapAccounts::~CMtfTestActionDeleteImapAccounts()
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

// Note if want no accounts you must call "DeleteImapAccounts accountsIdList" to ensure that 
// all accounts are cleared at the start. 
// You can not gaurantee that some other Test has left it clean.
// A test might leave, leaveing all the accounts in the central repository. 


void CMtfTestActionDeleteImapAccounts::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteImapAccounts);
	TInt paramDefaultIndex = ObtainValueParameterL<TInt>( TestCase(),ActionParameters().Parameter(0) ,-1 );

	CEmailAccounts* account = CEmailAccounts::NewLC();
	
	RArray<TImapAccount> arrayImap4Accounts;
	CleanupClosePushL(arrayImap4Accounts);
	account->GetImapAccountsL(arrayImap4Accounts);
	
	if ( paramDefaultIndex >= 0 ) 
		{ // Delete the specified id.
		account->DeleteImapAccountL(arrayImap4Accounts[paramDefaultIndex]);
			// Just ensure that if we try to delete it again we hopefuly get an error
			// or can see that it has been deleted.
		arrayImap4Accounts.Remove(paramDefaultIndex);
		}
	else if (paramDefaultIndex == -1 )
		{
		// Assume delete all the indexs		
		TInt count=arrayImap4Accounts.Count();
		TestCase().INFO_PRINTF2(_L("CMtfTestActionDeletePopAccounts deleting all accounts count = %d"), count );
		
		for(TInt i=0; i<count; i++)
			{
			TImapAccount id = arrayImap4Accounts[i];
			account->DeleteImapAccountL(id);
			}
		
		}
	else
		{	// Bad value.
		TestCase().ERR_PRINTF1(_L("DeleteImapAccounts : Index value must be -1 or greater than 0\n"));
		TestCase().SetTestStepResult(EFail);
		}		

	CleanupStack::PopAndDestroy(2, account);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteImapAccounts);
	TestCase().ActionCompletedL(*this);	
		
	}


