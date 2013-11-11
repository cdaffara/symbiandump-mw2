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
// GetSMTPMTMUId
// [Action Parameters]
// TUid SmtpMtmUid     <output-initiation>: Value of the SMTP MTM uid.
// [Action Description]
// Returns the value of SMTP MTM Uid 
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

#include "CMtfTestActionGetSMTPMTMUId.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <miutset.h>


CMtfTestAction* CMtfTestActionGetSMTPMTMUId::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetSMTPMTMUId* self = new (ELeave) CMtfTestActionGetSMTPMTMUId(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
//  does nothing
CMtfTestActionGetSMTPMTMUId::CMtfTestActionGetSMTPMTMUId(CMtfTestCase& aTestCase)
	:CMtfSynchronousTestAction(aTestCase)
	{
	}

// does nothing
CMtfTestActionGetSMTPMTMUId::~CMtfTestActionGetSMTPMTMUId()
	{
	}

void CMtfTestActionGetSMTPMTMUId::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetSMTPMTMUId);
	TUid smtpMtmUid = KUidMsgTypeSMTP;
	StoreParameterL<TUid>(TestCase(),smtpMtmUid,ActionParameters().Parameter(0));
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetSMTPMTMUId);
	TestCase().ActionCompletedL(*this);
	}



