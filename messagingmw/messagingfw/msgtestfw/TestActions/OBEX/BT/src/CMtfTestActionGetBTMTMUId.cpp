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
// GetBTMTMUId
// [Action Parameters]
// TUid BTMtmUid     <output-initiation>: Value of the BT MTM uid.
// [Action Description]
// Returns the value of BT MTM Uid 
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

#include "CMtfTestActionGetBTMTMUId.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <btmsgtypeuid.h>


CMtfTestAction* CMtfTestActionGetBTMTMUId::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetBTMTMUId* self = new (ELeave) CMtfTestActionGetBTMTMUId(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
//  does nothing
CMtfTestActionGetBTMTMUId::CMtfTestActionGetBTMTMUId(CMtfTestCase& aTestCase)
	:CMtfSynchronousTestAction(aTestCase)
	{
	}

// does nothing
CMtfTestActionGetBTMTMUId::~CMtfTestActionGetBTMTMUId()
	{
	}

void CMtfTestActionGetBTMTMUId::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetBtMTMUId);
	TUid btMtmUid = KUidMsgTypeBt;
	StoreParameterL<TUid>(TestCase(),btMtmUid,ActionParameters().Parameter(0));

	TestCase().INFO_PRINTF1(_L("CMtfTestActionGetBTMTMUId completed....."));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetBtMTMUId);
	TestCase().ActionCompletedL(*this);
	}



