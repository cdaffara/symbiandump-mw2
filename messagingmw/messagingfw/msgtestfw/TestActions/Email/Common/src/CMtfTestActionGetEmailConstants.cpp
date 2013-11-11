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
// GetEmailConstants
// [Action Parameters]
// Imap4MtmUid <output>: Value of IMAP4 MTM uid.
// Pop3MtmUid  <output>: Value of POP3 MTM uid.
// SmtpMtmUid  <output>: Value of SMTP MTM uid.
// [Action Description]
// Initialises email related constants.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionGetEmailConstants.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <miutset.h>


CMtfTestAction* CMtfTestActionGetEmailConstants::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetEmailConstants* self = new (ELeave) CMtfTestActionGetEmailConstants(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetEmailConstants::CMtfTestActionGetEmailConstants(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetEmailConstants::~CMtfTestActionGetEmailConstants()
	{
	}


void CMtfTestActionGetEmailConstants::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetEmailConstants);
	TUid paramImap4MtmUid = KUidMsgTypeIMAP4;
	StoreParameterL<TUid>(TestCase(),paramImap4MtmUid,ActionParameters().Parameter(0));

	TUid paramPop3MtmUid = KUidMsgTypePOP3;
	StoreParameterL<TUid>(TestCase(),paramPop3MtmUid,ActionParameters().Parameter(1));

	TUid paramSmtpMtmUid = KUidMsgTypeSMTP;
	StoreParameterL<TUid>(TestCase(),paramSmtpMtmUid,ActionParameters().Parameter(2));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetEmailConstants);
	TestCase().ActionCompletedL(*this);
	}

