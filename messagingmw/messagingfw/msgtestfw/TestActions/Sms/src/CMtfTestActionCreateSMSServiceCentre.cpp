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
// CreateSmsService
// [Action Parameters]
// TDesC	Service centre name				<input>
// TDesC	Service centre number			<input>
// TInt		default service centre index 	<output>
// [Action Description]
// Creates a SMS service and returns its TMsvId
// [APIs Used]
// __ACTION_INFO_END__
// 
//

/**
 @file
*/



#include <csmsaccount.h>


#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "CMtfTestActionCreateSmsServiceCentre.h"



CMtfTestAction* CMtfTestActionCreateSmsServiceCR::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSmsServiceCR* self = new (ELeave) CMtfTestActionCreateSmsServiceCR(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionCreateSmsServiceCR::CMtfTestActionCreateSmsServiceCR(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateSmsServiceCR::~CMtfTestActionCreateSmsServiceCR()
	{
	}

void CMtfTestActionCreateSmsServiceCR::ExecuteActionL()
	{
	
	// TODO - this test action can be removed
	
/*
	TDesC serviceCentreName =  ActionParameters().Parameter(0);
	TDesC serviceCentreNumber =  ActionParameters().Parameter(1);
	
	CSmsSettings *settings = CSmsSettings::NewLC();
	CSmsAccount *account = CSmsAccount::NewLC();
	
	
	
	TInt numSc = settings->S
	account->InitialiseDefaultSettingsL( *settings );
	settings->AddServiceCenterL( serviceCentreName, serviceCentreNumber );
	
	
	
	
	account->SaveSettingsL( *settings );
	
	TInt defaultServiceCentre =  settings->DefaultServiceCenter();
	
	CleanupStack::PopAndDestroy( 2, settings );
	
	
	StoreParameter
	
	
	StoreParameterL<TInt>(TestCase(), defaultServiceCentre, ActionParameters().Parameter(2));
*/
	TestCase().INFO_PRINTF1( _L("CMtfTestActionCreateSmsServiceCR completed" ) );
	TestCase().ActionCompletedL(*this);

	}


