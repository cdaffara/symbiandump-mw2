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
// CheckMessageStore
// [Action Parameters]
// session			input 			CMsvSession
// [Action Description]
// CheckMessageStore Test Action is intended to traverse the message store to
// ensure its integrity.
// [APIs Used]	
// CMsvSession::GetEntryL
// CMsvEntrySelection::Count
// CMsvEntry::ChildrenL
// CMsvEntry::Entry
// CMsvEntry::SetEntryL
// CMsvEntry::HasStore
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// CImEmailMessage::GetAttachmentListL
// MMsvAttachmentManager::AttachmentCount
// __ACTION_INFO_END__
//

//! @file

#include "CMtfTestActionCheckMessageStore.h"

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvstore.h>
#include <miutset.h>
#include <miutmsg.h>
#include <mmsvattachmentmanager.h>

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionCheckMessageStore object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCheckMessageStore::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionCheckMessageStore* self = new (ELeave) CMtfTestActionCheckMessageStore(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
}

/**
  Function : CMtfTestActionCheckMessageStore
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCheckMessageStore::CMtfTestActionCheckMessageStore(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
{
}
	
/**
  Function : ~CMtfTestActionCheckMessageStore
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCheckMessageStore::~CMtfTestActionCheckMessageStore()
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
void CMtfTestActionCheckMessageStore::ExecuteActionL()
{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCheckMessageStore);
	CMsvSession* session = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	
	// Get the root entry
	CMsvEntry* entry = session->GetEntryL(KMsvRootIndexEntryIdValue);
	CleanupStack::PushL(entry);
	
	// Traverse the message store and check the entries
	TRAPD(err, WalkMessageStoreL(*entry));
	if (err == KErrNone)
	{
		TestCase().INFO_PRINTF1(_L("CMtfTestActionCheckMessageStore: Message Store integrity OK!"));
		TestCase().SetTestStepResult(EPass);
	}
	else
	{
		TestCase().ERR_PRINTF2(_L("CMtfTestActionCheckMessageStore: FAILED! Error traversing message store: [%d]!"), err);
		TestCase().SetTestStepResult(EFail);
	}
	CleanupStack::PopAndDestroy(entry);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCheckMessageStore);
	TestCase().ActionCompletedL(*this);
}

/**
  Function : WalkMessageStore
  Description : Recurse down the message store
  @internalTechnology
  @param : aEntry - CMsvEntry representing the starting point for traversal
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionCheckMessageStore::WalkMessageStoreL(CMsvEntry& aEntry)
{
	// Get all the children of the current entry into a selection
	CMsvEntrySelection* sel = aEntry.ChildrenL();
	CleanupStack::PushL(sel);
	
	// Loop through the sub-entries
	for(TInt i=0;i<sel->Count();i++)
	{
		// Reset the entry to the child[i]
		aEntry.SetEntryL(sel->At(i));
		TMsvEntry entry = aEntry.Entry();
		
		// Parse the message. We're only interested in email messages.
		if(entry.iType == KUidMsvMessageEntry && (entry.iMtm == KUidMsgTypeIMAP4 || entry.iMtm == KUidMsgTypePOP3))
		{
			ProcessMessageL(aEntry);
		}
		
		// aEntry now used so safe to go recursive again
		WalkMessageStoreL(aEntry);
	}
	CleanupStack::PopAndDestroy(sel);
}

/**
  Function : ProcessMessageL
  Description : Checks that the entry is valid
  @internalTechnology
  @param : aEntry - CMsvEntry to check
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionCheckMessageStore::ProcessMessageL(CMsvEntry& aEntry)
{
	CImEmailMessage* message = CImEmailMessage::NewLC(aEntry);
	message->GetAttachmentsListL(aEntry.EntryId(),CImEmailMessage::EAllAttachments,CImEmailMessage::EThisMessageAndEmbeddedMessages);
	TInt count = message->AttachmentManager().AttachmentCount();
	{
		if(aEntry.HasStoreL())
		{
			CMsvStore* store = aEntry.ReadStoreL();
			MMsvAttachmentManager& attManager = store->AttachmentManagerL();																			 
			TInt count = attManager.AttachmentCount();
			
			// we might want to loop through the attachments and check that their files exist
			
			delete store;
		}
	}
	CleanupStack::PopAndDestroy(message);
}
