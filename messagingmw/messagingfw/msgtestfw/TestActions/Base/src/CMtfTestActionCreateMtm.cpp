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
// CreateMtm
// [Action Parameters]
// Session     <input>: Reference to the session.
// ServiceId   <input>: Value of the service id.
// MtmRegistry <input>: Reference to the client MTM registry.
// Mtm        <output>: Reference to the created MTM.
// [Action Description]
// Creates the specified client MTM.
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


#include "CMtfTestActionCreateMtm.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>
#include <mtclbase.h>
#include <mtclreg.h>


CMtfTestAction* CMtfTestActionCreateMtm::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateMtm* self = new (ELeave) CMtfTestActionCreateMtm(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateMtm::CMtfTestActionCreateMtm(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateMtm::~CMtfTestActionCreateMtm()
	{
	}


void CMtfTestActionCreateMtm::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateMtm);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	CClientMtmRegistry* paramMtmRegistry = ObtainParameterReferenceL<CClientMtmRegistry>(TestCase(),ActionParameters().Parameter(2));;

	CMsvEntry* entry = paramSession->GetEntryL(paramServiceId);
	TUid mtmUid = entry->Entry().iMtm;
	CleanupStack::PushL(entry);
	CBaseMtm* paramMtm = paramMtmRegistry->NewMtmL(mtmUid);
	CleanupStack::PushL(paramMtm);
	paramMtm->SetCurrentEntryL(entry);
	StoreParameterL<CBaseMtm>(TestCase(),*paramMtm,ActionParameters().Parameter(3));
	CleanupStack::Pop(paramMtm);
	CleanupStack::Pop(entry);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateMtm);
	TestCase().ActionCompletedL(*this);
	}



