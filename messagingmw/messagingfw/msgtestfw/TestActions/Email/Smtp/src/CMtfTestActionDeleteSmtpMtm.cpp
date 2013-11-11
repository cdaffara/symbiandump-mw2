// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// DeleteSmtpMtm
// [Action Parameters]
// Mtm <input>: Reference to Smtp client MTM.
// [Action Description]
// Deletes Smtp client MTM.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteSmtpMtm.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <smtcmtm.h>


CMtfTestAction* CMtfTestActionDeleteSmtpMtm::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteSmtpMtm* self = new (ELeave) CMtfTestActionDeleteSmtpMtm(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteSmtpMtm::CMtfTestActionDeleteSmtpMtm(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteSmtpMtm::~CMtfTestActionDeleteSmtpMtm()
	{
	}


void CMtfTestActionDeleteSmtpMtm::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteSmtpMtm);
	DeleteParameterL<CSmtpClientMtm>(TestCase(),ActionParameters().Parameter(0));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteSmtpMtm);
	TestCase().ActionCompletedL(*this);
	}

