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
// DeletePopAccounts
// [Action Parameters]
// Index		input			Tint	
// Note :  TPopAccounts is defined as typedef RArray<TPOP3Account> TPopAccounts 
// see TMtfTestParameterType.h for definition and parameter implementation
// [Action Description]
// DeletePopAccounts Test Action is intended to delete the POP accounts from the 
// Central Repository.  This Test Action takes TPOP3Account array and an index 
// value as input parameters.  If an integer value is provided for the index, 
// the POP account at the specified index will be deleted.  In case of default 
// value (-1) provided as index, all the POP accounts will be deleted.
// [APIs Used]	
// CEmailAccounts::DeletePopAccountL
// __ACTION_INFO_END__
// 
//

// system #includes
#include <cemailaccounts.h>

// user #includes
#include "CMtfTestActionDeletePopAccounts.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionDeletePopAccounts object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionDeletePopAccounts::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeletePopAccounts* self = new (ELeave) CMtfTestActionDeletePopAccounts(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionDeletePopAccounts
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionDeletePopAccounts::CMtfTestActionDeletePopAccounts(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionDeletePopAccounts
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionDeletePopAccounts::~CMtfTestActionDeletePopAccounts()
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
void CMtfTestActionDeletePopAccounts::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeletePopAccounts);
	TInt paramDefaultIndex = ObtainValueParameterL<TInt>( TestCase(),ActionParameters().Parameter(0), -1 );
	CEmailAccounts* accounts = CEmailAccounts::NewLC();	
	RArray<TPopAccount> arrayPopAccounts;
	CleanupClosePushL(arrayPopAccounts);
	accounts->GetPopAccountsL(arrayPopAccounts);
	
	if ( paramDefaultIndex >= 0 ) 
		{	 
		// Delete the specified id.
		accounts->DeletePopAccountL(arrayPopAccounts[paramDefaultIndex]);
		arrayPopAccounts.Remove(paramDefaultIndex);
		}
	else if ( paramDefaultIndex == -1 )
		{	
		// Assume delete all the indexs		
		TInt count = arrayPopAccounts.Count();
		TestCase().INFO_PRINTF2(_L("CMtfTestActionDeletePopAccounts deleting all accounts count = %d"), count );
		
		for( TInt i = 0; i < count; i++ )
			{
			TPopAccount id = arrayPopAccounts[i];
			accounts->DeletePopAccountL(id);						
			}		
				
		}
	else
		{	// Bad value.
		TestCase().ERR_PRINTF1(_L("DeletePopAccounts : Index value must be -1 or greater than 0\n"));
		TestCase().SetTestStepResult(EFail);
		}		

	CleanupStack::PopAndDestroy( 2, accounts );
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeletePopAccounts);
	TestCase().ActionCompletedL(*this);
}


