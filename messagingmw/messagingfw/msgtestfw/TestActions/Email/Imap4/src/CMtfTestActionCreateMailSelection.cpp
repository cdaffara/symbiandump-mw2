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
// CreateMailSelection
// [Action Parameters]
// CMsvSession    Session        <input>  : Reference to the session.
// TMsvId         ParentId       <input>  : Value of the parent folder ID
// CMsvSelection  EvenSelection  <output initiation> : Reference to the created 
// even selection.
// CMsvSelection  OddSelection   <output initiation> : Reference to the created
// odd selection.
// [Action Description]
// Creates two selections of IMAP4 messages.  The Test Action first creates a 
// selection of all IMAP4 messages under the parent folder, then copies the 
// Message Ids from the selection into 2 seperate message selections where one
// selection has all the message IDs in the odd position in the main selection
// and the other has the message Ids in the even position.
// These two message selections can be used as inputs to Test Actions related
// to IMAP4 commands which require a mail selection is requried as an input.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::SetSortTypeL
// CMsvEntry::ChildrenWithMtmL
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
#include <miutset.h>


// User include
#include "CMtfTestActionCreateMailSelection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"



/**
  NewL()
  Constructs a CMtfTestActionCreateMailSelection object. Uses two phase 
  construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionCreateMailSelection
  @pre    None
  @post   CMtfTestActionCreateMailSelection object is created
*/
CMtfTestAction* CMtfTestActionCreateMailSelection::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	
	CMtfTestActionCreateMailSelection* self = 
							new (ELeave) CMtfTestActionCreateMailSelection(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
	CMtfTestActionCreateMailSelection constructor
	Calls the base class' constructor
    @internalTechnology  
 	@param  aTestCase  Test Case to which this Test Action belongs
    @pre    None
    @post   None
*/ 
CMtfTestActionCreateMailSelection::CMtfTestActionCreateMailSelection(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ExecuteActionL
  Creates a selection of all IMAP4 mails present in the parent folder, then copies the 
  Message Ids from the selection into 2 seperate message selections where one
  selection has all the message IDs in the odd position in the main selection
  and the other has the message Ids in the even position.  The two message selections
  created are stored as output parameters of this Test Action.
  @internalTechnology 
  @pre    None
  @post   The third and the fourth parameters of the section in the .ini file corresponding 
		  to the instance of this test action should get associated with the even and the 
		  odd message selections objects respectively. These two message selections can be
		  used as inputs to Test Actions related to IMAP4 commands which require a 
		  mail selection is requried as an input.
*/	
void CMtfTestActionCreateMailSelection::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateMailSelection);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),
									ActionParameters().Parameter(0));

	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),
									ActionParameters().Parameter(1));


	/***********************************************************************************
	Create a selection of all IMAP4 messages in the parent folder. Create two new message
	selection objects.  Loop thru all the messages in the IMAP4 message selection, append  
	the Ids of messages present at odd index into one selection and those present at even 
	index into another selection.  The two message selections are stored as ouput paramters
	of this test action.
	***********************************************************************************/


	// Set the context to the parent folder 
	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramParentId,
										TMsvSelectionOrdering(KMsvNoGrouping,
															  EMsvSortByNone,ETrue
															  )
									  );
	
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramParentId);

	// Create a selection of all IMAP4 messages in the parent folder 
	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	entry->SetSortTypeL(order);
	CMsvEntrySelection* selection = entry->ChildrenWithMtmL(KUidMsgTypeIMAP4);
	CleanupStack::PushL(selection);

	// Create a message selection to store message Ids at even index in the main selection
	CMsvEntrySelection* evenSelection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(evenSelection);

	// Create a message selection to store message Ids at odd index in the main selection
	CMsvEntrySelection* oddSelection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(oddSelection);
	
	// Copy Message Ids into seperate selections based on their position (even / odd) 
	for(TInt i=0 ; i<selection->Count(); i++)
		{
		 if((i%2)== 0)
			{
			evenSelection->AppendL(selection->At(i));
			}
		 else
			{
			oddSelection->AppendL(selection->At(i));
			}
		}

	// Pop the mail selection objects before storing them as ouput parameters
	CleanupStack::Pop(oddSelection);
	CleanupStack::Pop(evenSelection);

	// Store the two message selections as output parameters of the Test Action 
	StoreParameterL<CMsvEntrySelection>(TestCase(),*evenSelection,
													ActionParameters().Parameter(2));

	StoreParameterL<CMsvEntrySelection>(TestCase(),*oddSelection,
													ActionParameters().Parameter(3));
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(entry);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateMailSelection);
	TestCase().ActionCompletedL(*this);
	}
