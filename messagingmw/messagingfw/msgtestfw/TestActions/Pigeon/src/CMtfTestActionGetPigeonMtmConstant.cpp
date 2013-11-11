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
// GetPigeonMtmConstant
// [Action Parameters]
// TUid PigeonMtmUid     <output-initiation>: Value of the Pigeon MTM uid.
// [Action Description]
// Provides the value of Pigeon MTM Uid
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

// System Includes
#include "pigeonservermtm.h"

// User Includes
#include "CMtfTestActionGetPigeonMtmConstant.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  Function : NewL
  Description		:Constructs a new CMtfTestActionGetPigeonMtmConstant object
  @internalTechnology
  @param			: aTestCase 		:Reference to the Test case
  @param			: aActionParams 	:Test Action parameters 
  @return			: CMtfTestAction*	:a base class pointer to the newly created object
  @pre		: none
  @post	: none
*/
CMtfTestAction* CMtfTestActionGetPigeonMtmConstant::NewL(CMtfTestCase& aTestCase,
												CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetPigeonMtmConstant* self = new (ELeave) 
												CMtfTestActionGetPigeonMtmConstant(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : CMtfTestActionGetPigeonMtmConstant
  Description	: Constructor
  @internalTechnology
  @param		: aTestCase - CMtfTestCase for the CMtfTestAction base class
  @pre none
  @post: none
*/
CMtfTestActionGetPigeonMtmConstant::CMtfTestActionGetPigeonMtmConstant(CMtfTestCase& aTestCase)
	:CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionGetPigeonMtmConstant
  Description : Destructor
  @internalTechnology
*/
CMtfTestActionGetPigeonMtmConstant::~CMtfTestActionGetPigeonMtmConstant()
	{
	}

/**
  Function : ExecuteActionL
  Description	: Provides the Pigeon MTM UId as the output of the Test Action
  @internalTechnology
  @param		: none
  @return		: void
  @pre 
  @post: none
*/
void CMtfTestActionGetPigeonMtmConstant::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetPigeonMtmConstant);
	TUid mtmUid = KUidMsgTypePigeon;
	StoreParameterL<TUid>(TestCase(),mtmUid,ActionParameters().Parameter(0));
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetPigeonMtmConstant);
	TestCase().ActionCompletedL(*this);
	}
