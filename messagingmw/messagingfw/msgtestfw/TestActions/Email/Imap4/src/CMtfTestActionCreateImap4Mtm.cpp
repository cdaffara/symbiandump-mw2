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
// CreateImap4Mtm
// [Action Parameters]
// Session     <input>: Reference to the session.
// ServiceId   <input>: Value of the the service id.
// MtmRegistry <input>: Reference to the client MTM registry.
// Mtm        <output>: Reference to the created Imap4 client MTM.
// [Action Description]
// Creates Imap4 client MTM.
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


#include "CMtfTestActionCreateImap4Mtm.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>
#include <impcmtm.h>
#include <mtclreg.h>


CMtfTestAction* CMtfTestActionCreateImap4Mtm::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateImap4Mtm* self = new (ELeave) CMtfTestActionCreateImap4Mtm(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateImap4Mtm::CMtfTestActionCreateImap4Mtm(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateImap4Mtm::~CMtfTestActionCreateImap4Mtm()
	{
	}


void CMtfTestActionCreateImap4Mtm::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateImap4Mtm);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	CClientMtmRegistry* paramMtmRegistry = ObtainParameterReferenceL<CClientMtmRegistry>(TestCase(),ActionParameters().Parameter(2));;

	CBaseMtm* paramMtm = paramMtmRegistry->NewMtmL(KUidMsgTypeIMAP4);
	CleanupStack::PushL(paramMtm);
	CMsvEntry* entry = paramSession->GetEntryL(paramServiceId);
	CleanupStack::PushL(entry);
	paramMtm->SetCurrentEntryL(entry);

	StoreParameterL<CImap4ClientMtm>(TestCase(),*(reinterpret_cast<CImap4ClientMtm*>(paramMtm)),ActionParameters().Parameter(3));
	CleanupStack::Pop(entry);
	CleanupStack::Pop(paramMtm);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateImap4Mtm);
	TestCase().ActionCompletedL(*this);
	}



