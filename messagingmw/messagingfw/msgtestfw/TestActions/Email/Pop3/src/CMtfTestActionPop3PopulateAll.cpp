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
// Pop3TopPopulateAll
// [Action Parameters]
// CPop3ClientMtm		paramMtm			<input>		: Reference to Pop3 client MTM
// CMsvEntrySelection   paramSelection		<input>		: Reference to the message selection
// TMsvId				paramServiceId		<input>		: Value of POP3 Service Id
// TTfImPop3GetMailInfo	paramMailInfo		<input>		: Reference to  param pack containing 
// : a TImPop3GetMailInfo containing POP3
// : TOP size limit info
// CMsvEntrySelection   paramSelection		<input>		: Reference to the selection.
// [Action Description]
// [APIs Used]
// __ACTION_INFO_END__
// 
//

/**
 @file
*/



#include "CMtfTestCase.h"
#include "CMtfTestActionPop3PopulateAll.h"


/**
  Function : CMtfTestActionPop3PopulateAll
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionPop3PopulateAll::CMtfTestActionPop3PopulateAll(CMtfTestCase& aTestCase)
	: CMtfTestActionPop3TopBase(aTestCase)
	{
	}
/**
  Function : ~CMtfTestActionPop3PopulateAll
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionPop3PopulateAll::~CMtfTestActionPop3PopulateAll()
	{
	}

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionPop3PopulateAll object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionPop3PopulateAll::NewL( CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionParams )
	{
	CMtfTestActionPop3PopulateAll *self = new (ELeave) CMtfTestActionPop3PopulateAll(aTestCase);
	CleanupStack::PushL( self );
	self->ConstructL( aActionParams );
	CleanupStack::Pop( self );
	return self;
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
void CMtfTestActionPop3PopulateAll::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionPop3PopulateAll);
	DoInvokeAsyncFunctionL( KPOP3MTMPopulateAll );
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionPop3PopulateAll);
	}


