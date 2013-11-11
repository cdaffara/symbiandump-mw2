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
// CheckDrmReceipt
// [Action Parameters]
// session				input 			CMsvSession
// messageId			input			TMsvId, ID of message to check	
// numberOfReceipts		input			TInt, number of expected DRM receipts
// [Action Description]
// CheckDrmReceipt Test Action is intended to look for DRM receipt attachments
// in an email message.
// [APIs Used]	
// CMsvSession::GetEntry
// CImEmailMessage::AttachmentManager
// MMsvAttachmentManager::AttachmentCount
// MMsvAttachmentManager::GetAttachmentInfoL
// CMsvAttachment::MimeType
// __ACTION_INFO_END__
//
//! @file

#include "CMtfTestActionCheckDrmReceipt.h"

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <e32cmn.h>
#include <msvapi.h>
#include <msvstore.h>
#include <txtrich.h>
#include <miutmsg.h>
#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>
#include <tmsvpackednotifierrequest.h>

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionCheckDrmReceipt object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCheckDrmReceipt::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionCheckDrmReceipt* self = new (ELeave) CMtfTestActionCheckDrmReceipt(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
}

/**
  Function : CMtfTestActionCheckDrmReceipt
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCheckDrmReceipt::CMtfTestActionCheckDrmReceipt(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
{
}
	
/**
  Function : ~CMtfTestActionSendEMsvGetEntry
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCheckDrmReceipt::~CMtfTestActionCheckDrmReceipt()
{
}

/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionCheckDrmReceipt::ExecuteActionL()
{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCheckDrmReceipt);
	CMsvSession* session = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt numberOfReceipts = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
	
	// Get the entry
	CMsvEntry* entry = session->GetEntryL(messageId);
	CleanupStack::PushL(entry);

	CImEmailMessage* msg = CImEmailMessage::NewLC(*entry);

	// loop through the attachments looking for DRM receipt attachments
	TInt foundReceipts = 0;
	TInt count = msg->AttachmentManager().AttachmentCount();
	for (TInt index = 0; index < count; index++)
	{
		CMsvAttachment* att = msg->AttachmentManager().GetAttachmentInfoL(index);
		if (att->MimeType().Length() > 0)
		{
			// mime type is set
			if (att->MimeType().CompareF(KEpocMimeTypeDrm) == 0)
			{
				// this is the receipt attachment, break out
				foundReceipts++;
			}
		}
		delete att;
	}
	
	if (foundReceipts == numberOfReceipts)
	{
		// found it!
		TestCase().INFO_PRINTF1(_L("CMtfTestActionCheckDrmReceipt: SUCCESS! Receipts correct!"));
		TestCase().SetTestStepResult(EPass);
	}
	else
	{
		// ok, no receipt
		TestCase().ERR_PRINTF3(_L("CMtfTestActionCheckDrmReceipt: FAILED! Receipts incorrect! Expected [%d], found [%d]."), numberOfReceipts, foundReceipts);
		TestCase().SetTestStepResult(EFail);
	}

	CleanupStack::PopAndDestroy(2); // entry, msg
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCheckDrmReceipt);
	TestCase().ActionCompletedL(*this);
}
