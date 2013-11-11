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
// GetIRMTMUId
// [Action Parameters]
// TUid IRMtmUid     <output-initiation>: Value of the IR MTM uid.
// [Action Description]
// Returns the value of IR MTM Uid 
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

#include "CMtfTestActionGetIRMTMUId.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <irmsgtypeuid.h>


CMtfTestAction* CMtfTestActionGetIRMTMUId::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetIRMTMUId* self = new (ELeave) CMtfTestActionGetIRMTMUId(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
//  does nothing
CMtfTestActionGetIRMTMUId::CMtfTestActionGetIRMTMUId(CMtfTestCase& aTestCase)
	:CMtfSynchronousTestAction(aTestCase)
	{
	}

// does nothing
CMtfTestActionGetIRMTMUId::~CMtfTestActionGetIRMTMUId()
	{
	}

void CMtfTestActionGetIRMTMUId::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetIrMTMUId);
	TUid irMtmUid = KUidMsgTypeIrUID;
	StoreParameterL<TUid>(TestCase(),irMtmUid,ActionParameters().Parameter(0));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetIrMTMUId);

	TestCase().ActionCompletedL(*this);
	}



