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
// SendAsAddAttachment
// [Action Parameters]
// RSendAsMessage			sendAsMessage	<input>		: Reference of  the RSendAsMessage object
// HBufC						attachmentFile	<input>		: The file to be attached
// HBufC						mimeType		<input>		: MIME type.Default value implies that no MIME type is specified
// [Action Description]
// SendAsAddAttachment Test Action is intended to add non-linked attachment 
// to the created message.  If the
// MIME type is provided as an input, the MIME type for the attachment is set.
// [APIs Used]
// RSendAsMessage::AddAttachment(RFs& aFs, RFile& aAttachmentFile, const TDesC& aMimeType)
// RSendAsMessage::AddAttachment(RFs& aFs, RFile& aAttachmentFile)
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "sendas2.h"
#include "CMtfTestActionSendAsAddAttachment.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "MtfTestActionUtilsUser.h"

/**
  NewL()
  Constructs a CMtfTestActionSendAsAddAttachment object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsAddAttachment
  @pre    None
  @post   CMtfTestActionSendAsAddAttachment object is created
*/
CMtfTestAction* CMtfTestActionSendAsAddAttachment::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsAddAttachment* self = 
							new (ELeave) CMtfTestActionSendAsAddAttachment(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsAddAttachment constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsAddAttachment::CMtfTestActionSendAsAddAttachment(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsAddAttachment
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsAddAttachment::~CMtfTestActionSendAsAddAttachment()
	{
	}
	

/**
  ExecuteActionL
  	Obtain the input parameters
	1.	sendAsMessage 
	2.	attachmentFile
	3.	mimeType (Default value implies empty string)
	Open a File Server session and create a RFile for the attachment File
	IF MIME type is specified 
	RSendAsMessage::AddAttachment (RFs& aFs, RFile& aAttachmentFile, const TDesC& aMimeType) by passing parameters of RFs for file server session, RFile type for attachment file and MIME type
    Else 
	Call RSendAsMessage::AddAttachment (RFs& aFs, RFile& aAttachmentFile) by passing parameters of RFs type for file server session, RFile type for attachment file

	Close session with the file server

  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsAddAttachment::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsAddAttachment);
	if((TestCase().TestStepResult()) == EPass)
		{
		iSendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),
											ActionParameters().Parameter(0));
		HBufC* attachmentFile = ObtainParameterReferenceL<HBufC>(TestCase(),
											ActionParameters().Parameter(1)); 
		HBufC8* mimeType = ObtainParameterReferenceL<HBufC8>(TestCase(),ActionParameters().Parameter(2), NULL);	
		
		
		if(mimeType != NULL)
			{
			iSendAsMessage.AddAttachment(*attachmentFile, *mimeType, iStatus);
			}
		else
			{
			iSendAsMessage.AddAttachment(*attachmentFile, iStatus);
			}
		
		CActiveScheduler::Add(this);		
		SetActive();
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsAddAttachment);
	}

void CMtfTestActionSendAsAddAttachment::DoCancel()
	{
	iSendAsMessage.Cancel();
	}


void CMtfTestActionSendAsAddAttachment::RunL()
	{
	TSendAsProgress		sendAsProgress;
	TInt	errorCode = KErrNone;
		
	if (iStatus == KErrNone)
		{
		iSendAsMessage.ProgressL(sendAsProgress);
		errorCode = sendAsProgress.iError;
		}
	else
		{
		errorCode = iStatus.Int();
		}

	if(ActionParameters().Count() == 4)
		{
		StoreParameterL<TInt>(TestCase(),errorCode,ActionParameters().Parameter(3));
		}
	else
		{
		User::LeaveIfError(errorCode);
		}

	
	TestCase().INFO_PRINTF2(_L("Add Attchment completed with Error code: %d"),errorCode );
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsAddAttachment);
	TestCase().ActionCompletedL(*this);
	}

