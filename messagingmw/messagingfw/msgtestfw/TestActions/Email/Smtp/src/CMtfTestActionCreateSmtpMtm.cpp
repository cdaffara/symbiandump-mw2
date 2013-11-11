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
// CreateSmtpMtm
// [Action Parameters]
// Session     <input>: Reference to the session.
// ServiceId   <input>: Value of the the service id.
// MtmRegistry <input>: Reference to the client MTM registry.
// Mtm        <output>: Reference to the created Smtp client MTM.
// [Action Description]
// Creates Smtp client MTM.
// [APIs Used]
// CClientMtmRegistry::NewMtmL
// CMsvSession::GetEntryL
// CBaseMtm::SetCurrentEntryL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include <msvapi.h>
#include <smtcmtm.h>
#include <mtclreg.h>

#include <cemailaccounts.h>

#include "CMtfTestActionCreateSmtpMtm.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

CMtfTestAction* CMtfTestActionCreateSmtpMtm::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSmtpMtm* self = new (ELeave) CMtfTestActionCreateSmtpMtm(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateSmtpMtm::CMtfTestActionCreateSmtpMtm(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateSmtpMtm::~CMtfTestActionCreateSmtpMtm()
	{
	}


void CMtfTestActionCreateSmtpMtm::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateSmtpMtm);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));

	CEmailAccounts* accounts = CEmailAccounts::NewLC();
	TSmtpAccount smtpAccount;
	User::LeaveIfError(accounts->DefaultSmtpAccountL(smtpAccount));
	TMsvId paramServiceId = smtpAccount.iSmtpService;	
	CleanupStack::PopAndDestroy( accounts );
	
	CClientMtmRegistry* paramMtmRegistry = ObtainParameterReferenceL<CClientMtmRegistry>(TestCase(),ActionParameters().Parameter(2));;

	CBaseMtm* paramMtm = paramMtmRegistry->NewMtmL(KUidMsgTypeSMTP);
	CleanupStack::PushL(paramMtm);
	CMsvEntry* entry = paramSession->GetEntryL(paramServiceId);
	CleanupStack::PushL(entry);
	paramMtm->SetCurrentEntryL(entry);

	StoreParameterL<CBaseMtm>(TestCase(),*(reinterpret_cast<CBaseMtm*>(paramMtm)),ActionParameters().Parameter(3));
	CleanupStack::Pop(entry);
	CleanupStack::Pop(paramMtm);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateSmtpMtm);
	TestCase().ActionCompletedL(*this);
	}

