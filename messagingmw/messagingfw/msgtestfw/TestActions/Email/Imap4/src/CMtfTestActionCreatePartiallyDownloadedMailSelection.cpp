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
// CreatePartiallyDownloadedMailSelection
// [Action Parameters]
// CMsvSession        Session    <input>	:    Reference to the session
// TMsvId             ParentId   <input>	:    Value of the parent folder Id.
// CMsvEntrySelection MailSelection <output-initiation>: Reference to the  
// selection of partially 
// downloded mails.
// [Action Description]
// Creates a selection of all Partially Downloaded Mails under the parent folder.
// [APIs Used]
// CMsvEntry::SetSortTypeL
// CMsvEntry::GetEntry
// CMsvEntrySelection::ChildrenWithMtmL
// CMsvEntrySelection::AppendL
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/

// EPOC include
#include <msvapi.h>


// User include 
#include "CMtfTestActionCreatePartiallyDownloadedMailSelection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"



/**
  NewL()
  Constructs a CMtfTestActionCreatePartiallyDownloadedMailSelection object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionCreatePartiallyDownloadedMailSelection
  @pre    None
  @post   CMtfTestActionCreatePartiallyDownloadedMailSelection object is created
*/
CMtfTestAction* CMtfTestActionCreatePartiallyDownloadedMailSelection::
			NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreatePartiallyDownloadedMailSelection* self = 
			new (ELeave) CMtfTestActionCreatePartiallyDownloadedMailSelection(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

	
/**
  CMtfTestActionCreatePartiallyDownloadedMailSelection constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionCreatePartiallyDownloadedMailSelection::
		CMtfTestActionCreatePartiallyDownloadedMailSelection(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  ExecuteActionL
  Creates a selection of all partially downloaded IMAP4 mails present in the parent folder.
  The created message selection is stored as an output parameter of the Test Action.
  @internalTechnology 
  @pre    None
  @post   None
*/	
void CMtfTestActionCreatePartiallyDownloadedMailSelection::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreatePartiallyDownloadedMailSelection);
	CMsvSession*        paramSession    = ObtainParameterReferenceL<CMsvSession>(TestCase(),
											ActionParameters().Parameter(0));

	TMsvId				paramParentId	= ObtainValueParameterL<TMsvId>(TestCase(),
											ActionParameters().Parameter(1));
	

	/***********************************************************************************
	Create a selection of all IMAP4 messages in the parent folder. Using the message Ids
	present in the selection, check if the message was partially downloaded.  Append the
	partially downloaded messages' Ids to a new message selection.  Store the message 
	selection containig the Ids of partially donwloaded messages as an output parameter 
	of the Test Action.
	***********************************************************************************/

	// Set the context to parent folder 
	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramParentId,
										TMsvSelectionOrdering(KMsvNoGrouping,
															  EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);	

	// Create a selection of all IMAP4 mails in the parent folder 
	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	entry->SetSortTypeL(order);

	CMsvEntrySelection* selection = entry->ChildrenWithMtmL(KUidMsgTypeIMAP4);
	CleanupStack::PushL(selection);

	// Create a new selection to store partially downloaded mails 
	CMsvEntrySelection* paramMailSelection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(paramMailSelection);

	TMsvEmailEntry emailEntry;

	for(TInt i=1;i<selection->Count();i++)
		{
		TMsvId messageId = selection->At(i);
	    entry->Session().GetEntry(messageId,paramParentId,emailEntry);

		// If the mail was partially downloaded, append its Id to the  
		//	paritally downloaded mail selection 
		if(emailEntry.PartialDownloaded())
			{
			paramMailSelection->AppendL(messageId);
			}
		}	
	
	// Pop the mail selection object before storing it as ouput parameter
	CleanupStack::Pop(paramMailSelection);	

	// Store the message selections as output parameters of the Test Action 
	StoreParameterL<CMsvEntrySelection>(TestCase(),*paramMailSelection,
										ActionParameters().Parameter(2));
	
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(entry);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreatePartiallyDownloadedMailSelection);
	TestCase().ActionCompletedL(*this);
	}
