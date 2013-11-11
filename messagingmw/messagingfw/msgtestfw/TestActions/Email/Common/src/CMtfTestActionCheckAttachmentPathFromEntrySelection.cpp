// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CheckAttachmentPathFromEntrySelection
// [Action Parameters]
// Session        <input>: Reference to the session.
// EntrySelection <input>: Reference to the entry selection
// [Action Description]
// Checks the attachment paths from an email message in an entry selection.
// [APIs Used]
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionCheckAttachmentPathFromEntrySelection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <miutmsg.h>

CMtfTestAction* CMtfTestActionCheckAttachmentPathFromEntrySelection::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCheckAttachmentPathFromEntrySelection* self = new(ELeave) CMtfTestActionCheckAttachmentPathFromEntrySelection(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
CMtfTestActionCheckAttachmentPathFromEntrySelection::CMtfTestActionCheckAttachmentPathFromEntrySelection(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

CMtfTestActionCheckAttachmentPathFromEntrySelection::~CMtfTestActionCheckAttachmentPathFromEntrySelection()
	{
	}

void CMtfTestActionCheckAttachmentPathFromEntrySelection::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCheckAttachmentPathFromEntrySelection);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	CMsvEntrySelection* paramSelection = ObtainParameterReferenceL<CMsvEntrySelection>(TestCase(),ActionParameters().Parameter(1));
	
	if( paramSelection->Count()==0 )
		{
		User::Leave(KErrArgument);
		}
	
	TMsvId entryId = (*paramSelection)[0];
	CMsvEntry* entry = paramSession->GetEntryL(entryId);
	CleanupStack::PushL(entry);
	
	CImEmailMessage* emailMsg = CImEmailMessage::NewL(*entry);
	CleanupStack::PushL(emailMsg);
	
	MMsvAttachmentManager& attManager = emailMsg->AttachmentManager();
	TInt attachmentCount = attManager.AttachmentCount();
	if( attachmentCount==0 )
		{
		User::Leave(KErrArgument);
		}
	
	for( TInt ii=0; ii<attachmentCount; ++ii )
		{
		// for each attachment, ensure that the physical attachment exists
		// as per the file path given in the attachment info
		CMsvAttachment* attachInfo = attManager.GetAttachmentInfoL(ii);
		CleanupStack::PushL(attachInfo);
		
		// use the Att() method just to check if the file exists
		RFs& fs(paramSession->FileSession());
		TUint fileAttributes;
		TInt err = fs.Att(attachInfo->FilePath(), fileAttributes);
		User::LeaveIfError(err);
		
		CleanupStack::PopAndDestroy(attachInfo);
		}
	
	CleanupStack::PopAndDestroy(2, entry); // emailMsg, entry
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCheckAttachmentPathFromEntrySelection);
	TestCase().ActionCompletedL(*this);
	}


	
