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
// CreatePigeonMessage
// [Action Parameters]
// CMsvSession  Session        	<input>:  Reference to the session.
// TMsvId       ParentId       	<input>:  Value of the parent id.
// TMsvId		ServiceId      	<input>:  Value of the the service id.
// TInt			Time		   	<input>:  Time interval in minutes 
// for scheduling the message
// TMsvId 		MessageId     	<output>: Value of the created message id.
// [Action Description]
// Creates a Pigeon message on the specified parent folder.
// [APIs Used]	
// TMsvEntry::iMtm	
// TMsvEntry::iServiceId
// TMsvEntry::iDate
// TMsvEntry::Id
// CMsvEntry::SetEntryL
// CMsvEntry::CreateL
// __ACTION_INFO_END__
// 
//

// System Includes
#include <msvapi.h>
#include <msvuids.h>
#include <txtrich.h>
#include <smuthdr.h>
#include "pigeonservermtm.h"

// User Includes
#include "CMtfTestActionCreatePigeonMessage.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  Function : NewL
  Description 	: Constructs a new CMtfTestActionCreatePigeonMessage object
  @internalTechnology
  @param 		: aTestCase 		:Reference to the Test case
  @param 		: aActionParams 	:Test Action parameters 
  @return 		: CMtfTestAction* 	:a base class pointer to the newly created object
  @pre none
  @post: none
*/
CMtfTestAction* CMtfTestActionCreatePigeonMessage::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreatePigeonMessage* self = new (ELeave) CMtfTestActionCreatePigeonMessage(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  Function 		: CMtfTestActionCreatePigeonMessage
  Description 	: Constructor
  @internalTechnology
  @param 		: aTestCase - CMtfTestCase for the CMtfTestAction base class
  @pre none
  @post: none
*/
CMtfTestActionCreatePigeonMessage::CMtfTestActionCreatePigeonMessage(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ExecuteActionL
  Description :Creates a Pigeon Message in the specified folder.
  @internalTechnology
  @param  : none
  @return : void
  @pre  : none
  @post none
*/
void CMtfTestActionCreatePigeonMessage::ExecuteActionL()
	{
	TestCase().Logger().Write(_L("CMtfTestActionCreatePigeonMessage::ExecuteActionL IN"));
			
	// Obtain Input parameters
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramParentId  	= ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvId paramServiceId 	= ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));
	TInt paramTime  		= ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3));
	
	if(TestCase().TestStepResult() == EPass)
		{
		// Create a message entry
		TMsvEntry indexEntry; 
		indexEntry.iType = KUidMsvMessageEntry;
		indexEntry.iMtm = KUidMsgTypePigeon;	
		indexEntry.iServiceId = paramServiceId;
		indexEntry.SetVisible(ETrue);	
		indexEntry.SetInPreparation(EFalse); 
		indexEntry.iDescription.Set(KNullDesC); 
		indexEntry.iDetails.Set(KNullDesC); 
		
		// Set schedule time
		TTime date ;
		date.HomeTime();
		// If creating two or more messages for the same time (ie MSG_SCH_SEND_0009.script), they have
		// to have exactly the same time, otherwise they will cause an assert further on. Fractions of
		// a second are not acceptable.
		RoundUpToSecond(date);
		date += TTimeIntervalMinutes(paramTime);
		indexEntry.iDate = date;
		
		// Set message scheduling info
		indexEntry.SetOffPeak(EFalse);
		indexEntry.SetScheduled(EFalse);
		indexEntry.SetSendingState(KMsvSendStateWaiting);
			
		// Create Entry	
		CMsvEntry* entry = paramSession->GetEntryL(paramParentId);
		CleanupStack::PushL(entry);
		
		//Creates a new child entry owned by the context synchronously
		entry->CreateL(indexEntry);
		TMsvId paramMessageId = indexEntry.Id();
		
		paramSession->CleanupEntryPushL(paramMessageId);
		entry->SetEntryL(paramMessageId);
		
		paramSession->CleanupEntryPop();
		CleanupStack::PopAndDestroy(entry);

		// Provide the Message Id as output of the Test Action 
		StoreParameterL<TMsvId>(TestCase(),paramMessageId,ActionParameters().Parameter(4));
		
		TestCase().Logger().Write(_L("CMtfTestActionCreatePigeonMessage::ExecuteActionL OUT"));
		TestCase().ActionCompletedL(*this);
		}
	}


/**
  Function : RoundUpToSecond
  Description : On return, the passed value rounded up to the nearest second.
  @param aTime 
  @return : void
*/
void CMtfTestActionCreatePigeonMessage::RoundUpToSecond(TTime& aTime)
	{
	TDateTime dt(aTime.DateTime());
	dt.SetMicroSecond(0);
	aTime = dt;
	aTime += (TTimeIntervalSeconds) 1;
	}
