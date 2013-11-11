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
// CreatePop3MailInfo
// [Action Parameters]
// MaxEmailSize		 <input> : Value of the maximum email size.
// DestinationFolder <input> : Value for the folder id
// PopulationLimit   <input> : Value for the number of lines to fetch
// Pop3MailInfo		 <output>: Reference to the created TImPop3GetMailInfo2 object
// [Action Description]
// Creates a POP3 mail info object TImPop3GetMailInfo and stores it
// [APIs Used]
// TImPop3GetMailInfo::iMaxEmailSize
// TImPop3GetMailInfo::iDestinationFolder
// TImPop3GetMailInfo::iPopulationLimit
// __ACTION_INFO_END__
// 
//

/**
 @file
*/




#include <msvapi.h>
#include <miutset.h>

#include "CMtfTestActionCreatePop3MailInfo.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CPop3MailInfo.h"


CMtfTestAction* CMtfTestActionCreatePop3MailInfo::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreatePop3MailInfo* self = new (ELeave) CMtfTestActionCreatePop3MailInfo(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreatePop3MailInfo::CMtfTestActionCreatePop3MailInfo(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreatePop3MailInfo::~CMtfTestActionCreatePop3MailInfo()
	{
	}

void CMtfTestActionCreatePop3MailInfo::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreatePop3MailInfo);
	TInt  paramMaxEmailSize = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));
	TInt	paramPopLimit = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));

	CPop3MailInfo* pop3MailInfo  = new (ELeave) CPop3MailInfo;
	CleanupStack::PushL( pop3MailInfo );
	pop3MailInfo->MailInfo().SetMaxEmailSize( static_cast< TInt32 >(paramMaxEmailSize) );
	pop3MailInfo->MailInfo().SetPopulationLimit( static_cast< TInt32 >(paramPopLimit) );
	StoreParameterL<CPop3MailInfo>( TestCase(), *pop3MailInfo, ActionParameters().Parameter(2));
	CleanupStack::Pop();  // pop3MailInfo
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreatePop3MailInfo);
	TestCase().ActionCompletedL(*this);
	}







