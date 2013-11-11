// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CreateExpPop3MailInfo
// [Action Parameters]
// Body Text Lines   <input> : Number of lines of body text to expect
// Footer present    <input> : whether to expect a footer message
// HTML lines        <input> : Number of lines of html to expect
// Description       <input> : Subject line of the email this will be used to check
// Attachments       <input> : Number of attachments to expect
// ExpPop3MailInto   <output>: Reference to the created CExpMailInfo object
// Footer size       <input><optional> : Size of the amount left on the server that should be in the footer message
// [Action Description]
// Creates a object representing the expect results of a donwloaded email
// inlcuding the number of body text lines, number of html lines, number
// of attachments, presence or not of a partial download footer, and optionally
// the size mentioned in the partial download footer.
// This can be passed to the VerifyEmail test action which compares these
// objects with email messages in the mail store
// [APIs Used]
// None
// __ACTION_INFO_END__
// 
//

/**
 @file
*/




#include <msvapi.h>
#include <miutset.h>

#include "CMtfTestActionCreateExpPop3MailInfo.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CExpMailInfo.h"


CMtfTestAction* CMtfTestActionCreateExpPop3MailInfo::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateExpPop3MailInfo* self = new (ELeave) CMtfTestActionCreateExpPop3MailInfo(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateExpPop3MailInfo::CMtfTestActionCreateExpPop3MailInfo(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateExpPop3MailInfo::~CMtfTestActionCreateExpPop3MailInfo()
	{
	}

void CMtfTestActionCreateExpPop3MailInfo::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateExpPop3MailInfo);
	CExpPop3MailInfo* expMailInfo  = new (ELeave) CExpPop3MailInfo;
	CleanupStack::PushL( expMailInfo );
	expMailInfo->SetNumLinesBodyText( ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0)) );
	expMailInfo->SetFooterExpected( ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1)) );
	expMailInfo->SetNumLinesHtml( ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2)) );

	HBufC* paramDescription = ObtainParameterReferenceL<HBufC>(TestCase(), ActionParameters().Parameter(3));	
	TPtr p = paramDescription->Des();
	expMailInfo->SetDescription( p ); 

	TInt paramNAttachmentsExp   = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(4));
	expMailInfo->SetNumAttachments( paramNAttachmentsExp );

	
	if(ActionParameters().Count()==7)
		{
		expMailInfo->SetFooterSize( ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(6)) );
		}
	else
		{
		expMailInfo->SetFooterSize( 0 );
		}

	StoreParameterL<CExpPop3MailInfo>( TestCase(), *expMailInfo, ActionParameters().Parameter( 5 ));

	CleanupStack::Pop(1); // expMailInfo
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateExpPop3MailInfo);
	TestCase().ActionCompletedL(*this);
	}







