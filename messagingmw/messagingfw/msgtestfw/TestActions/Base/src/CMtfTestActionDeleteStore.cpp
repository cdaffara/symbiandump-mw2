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
// DeleteStore
// [Action Parameters]
// CMsvStore	paramStore	 <input>	: Reference to RSendAsMssage object
// [Action Description]
// DeleteStore Test Action is intended to delete the open Store
// [APIs Used]
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/

//system include
#include <msvstore.h>

// User include
#include "CMtfTestActionDeleteStore.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionDeleteStore object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionDeleteStore
  @pre    None
  @post   CMtfTestActionDeleteStore object is created
*/
CMtfTestAction* CMtfTestActionDeleteStore::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteStore* self = 
							new (ELeave) CMtfTestActionDeleteStore(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionDeleteStore constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionDeleteStore::CMtfTestActionDeleteStore(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionDeleteStore
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionDeleteStore::~CMtfTestActionDeleteStore()
	{
	}

/**
  ExecuteActionL
  Obtains the parameters for the test action. Gets the Store and deletes it
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionDeleteStore::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteStore);
	CMsvStore* paramStore = ObtainParameterReferenceL<CMsvStore>(TestCase(),
									ActionParameters().Parameter(0));

	paramStore->DeleteL();
	DeleteParameterL<CMsvStore>(TestCase(),ActionParameters().Parameter(0));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteStore);
	TestCase().ActionCompletedL(*this);
	}

