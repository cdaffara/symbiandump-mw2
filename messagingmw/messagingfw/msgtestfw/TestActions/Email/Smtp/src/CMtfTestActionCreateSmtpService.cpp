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
// CreateSmtpService
// [Action Parameters]
// Session        <input>: Reference to the session.
// (Priority)     <input>: Value of the entry priority. Default is medium priority.
// (ReadOnlyFlag) <input>: Value of the read only flag. Default is false.
// (VisibleFlag)  <input>: Value of the visible flag. Default is true.
// (Description)  <input>: Name of the description.	Default is blank.
// (Details)      <input>: Name of the details. Default is blank.
// ServiceId     <output>: Value of the created service id.
// [Action Description]
// Creates a SMTP service.
// [APIs Used]
// TMsvEntry::iType
// TMsvEntry::SetPriority	
// TMsvEntry::iMtm	
// TMsvEntry::SetReadOnly	
// TMsvEntry::SetVisible	
// TMsvEntry::iDescription
// TMsvEntry::iDetails
// TMsvEntry::iDate
// TMsvEntry::Id
// CMsvEntry::SetEntryL
// CMsvEntry::CreateL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include <msvapi.h>
#include <miutset.h>

#include <cemailaccounts.h>



#include "CMtfTestActionCreateSmtpService.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"



CMtfTestAction* CMtfTestActionCreateSmtpService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSmtpService* self = new (ELeave) CMtfTestActionCreateSmtpService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateSmtpService::CMtfTestActionCreateSmtpService(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateSmtpService::~CMtfTestActionCreateSmtpService()
	{
	delete iBlank;
	}


void CMtfTestActionCreateSmtpService::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateSmtpService);
	// This method requires that a related IMAP4 or POP3 account has been previously set up
	// in the central repository - this is because SMTP accounts do not exist independently
	// but only as part of an IMAP4 or POP3 account
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();
	TSmtpAccount smtpAccount;
	User::LeaveIfError(emailAccounts->DefaultSmtpAccountL(smtpAccount));
		
	TMsvId paramServiceId = smtpAccount.iSmtpService;
	CleanupStack::PopAndDestroy(emailAccounts);

	StoreParameterL<TMsvId>(TestCase(),paramServiceId,ActionParameters().Parameter(6));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateSmtpService );
	TestCase().ActionCompletedL(*this);
	}

