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
// CreatePop3Mtm
// [Action Parameters]
// Session     <input>: Reference to the session.
// ServiceId   <input>: Value of the the service id.
// MtmRegistry <input>: Reference to the client MTM registry.
// Mtm        <output>: Reference to the created POP3 client MTM.
// [Action Description]
// Creates POP3 client MTM.
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
#include <popcmtm.h>
#include <mtclreg.h>

#include <cemailaccounts.h>


#include "CMtfTestActionCreatePop3Mtm.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

CMtfTestAction* CMtfTestActionCreatePop3Mtm::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreatePop3Mtm* self = new (ELeave) CMtfTestActionCreatePop3Mtm(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreatePop3Mtm::CMtfTestActionCreatePop3Mtm(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreatePop3Mtm::~CMtfTestActionCreatePop3Mtm()
	{
	}


void CMtfTestActionCreatePop3Mtm::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreatePop3Mtm);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>( TestCase(),ActionParameters().Parameter(1));
	CClientMtmRegistry* paramMtmRegistry = ObtainParameterReferenceL<CClientMtmRegistry>(TestCase(),ActionParameters().Parameter(2));;

	CBaseMtm* paramMtm = paramMtmRegistry->NewMtmL(KUidMsgTypePOP3);
	CleanupStack::PushL(paramMtm);
	CMsvEntry* entry = paramSession->GetEntryL(paramServiceId);
	CleanupStack::PushL(entry);
	paramMtm->SetCurrentEntryL(entry);

	StoreParameterL<CPop3ClientMtm>(TestCase(),*(reinterpret_cast<CPop3ClientMtm*>(paramMtm)),ActionParameters().Parameter(3));
	CleanupStack::Pop(entry);
	CleanupStack::Pop(paramMtm);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreatePop3Mtm);
	
	TestCase().ActionCompletedL(*this);
	}
