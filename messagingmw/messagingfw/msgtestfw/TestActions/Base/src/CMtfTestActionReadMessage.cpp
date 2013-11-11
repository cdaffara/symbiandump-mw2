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
// ReadMessage
// [Action Parameters]
// Session			<input>: Reference to the session.
// Entry			<input>: Reference to the entry.
// [Action Description]
// Reads the contents of a message and prints it in the log.
// [APIs Used]
// CMsvEntry::Entry
// CMtfTestActionUtilsEmailMessage::GetBodyTextL
// CRichText::Read
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionReadMessage.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsEmailMessage.h"

#include <miutset.h>



CMtfTestAction* CMtfTestActionReadMessage::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionReadMessage* self = new(ELeave) CMtfTestActionReadMessage(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionReadMessage::CMtfTestActionReadMessage(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionReadMessage::~CMtfTestActionReadMessage()
	{
	}

void CMtfTestActionReadMessage::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionAddReadMessage);

	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	CMsvEntry* messageEntry = ObtainParameterReferenceL<CMsvEntry>(TestCase(),ActionParameters().Parameter(1));

	// print message subject in log
	TestCase().INFO_PRINTF1(_L("Message subject:"));
	TPtrC subject = messageEntry->Entry().iDescription;
	TestCase().INFO_PRINTF1(subject);

	// get and read message body
	CMtfTestActionUtilsEmailMessage* messageInfo = CMtfTestActionUtilsEmailMessage::NewL(*messageEntry, TestCase());
	CleanupStack::PushL(messageInfo);

	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL(); 
	CleanupStack::PushL(charFormatLayer);

	CRichText* plainBodyText = CRichText::NewL(paraFormatLayer, charFormatLayer, CEditableText::EFlatStorage, 256);
	CleanupStack::PushL(plainBodyText);

	messageInfo->GetBodyTextL(*plainBodyText, *paraFormatLayer, *charFormatLayer);

	TPtrC bodyText = plainBodyText->Read(0);

	// print message body in log
	TestCase().INFO_PRINTF1(_L("Message body:"));
	TestCase().INFO_PRINTF1(bodyText);

	CleanupStack::PopAndDestroy(4);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionAddReadMessage);
	TestCase().ActionCompletedL(*this);
	}



