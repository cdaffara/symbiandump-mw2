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
// CreateMmsMtm
// [Action Parameters]
// Session     <input>: Reference to the session.
// ServiceId   <input>: Value of the the service id.
// MtmRegistry <input>: Reference to the client MTM registry.
// Mtm        <output>: Reference to the created MMS client MTM.
// [Action Description]
// Creates MMS client MTM.
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


#include "CMtfTestActionCreateMmsMtm.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>
#include <mmsclientmtm.h>
#include <mmsutils.h>
#include <mtclreg.h>
#include <mtclbase.h>


CMtfTestAction* CMtfTestActionCreateMmsMtm::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateMmsMtm* self = new (ELeave) CMtfTestActionCreateMmsMtm(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateMmsMtm::CMtfTestActionCreateMmsMtm(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateMmsMtm::~CMtfTestActionCreateMmsMtm()
	{
	}


void CMtfTestActionCreateMmsMtm::ExecuteActionL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	CClientMtmRegistry* paramMtmRegistry = ObtainParameterReferenceL<CClientMtmRegistry>(TestCase(),ActionParameters().Parameter(2));;

	CBaseMtm* paramMtm = paramMtmRegistry->NewMtmL(KUidMsgTypeMMS);
	CleanupStack::PushL(paramMtm);
	CMsvEntry* entry = paramSession->GetEntryL(paramServiceId);
	CleanupStack::PushL(entry);
	paramMtm->SetCurrentEntryL(entry);

	StoreParameterL<CMmsClientMtm>(TestCase(),*(reinterpret_cast<CMmsClientMtm*>(paramMtm)),ActionParameters().Parameter(3));
	CleanupStack::Pop(entry);
	CleanupStack::Pop(paramMtm);
	
	TestCase().ActionCompletedL(*this);
	}

