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
// HBufC						attachmentFile	<input>		: The file to be attached as a linked attachment
// HBufC						mimeType		<input>		: MIME type.Default value implies that no MIME type is specified
// [Action Description]
// SendAsAddLinkedAttachment Test Action is intended to add linked attachment to 
// the created message.  The attachment file provided to be added as a linked attachment 
// should be present in the public data area.  If MIME type of the attachment is provided 
// as an input to the Test Action, the MIME type will be set for the attachment.
// [APIs Used]
// RSendAsMessage::AddLinkedAttachment(const TDesC& aLinkedAttachmentFile,
// const TDesC& aMimeType)
// RSendAsMessage::AddLinkedAttachment(const TDesC& aLinkedAttachmentFile)
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "sendas2.h"
#include "CMtfTestActionSendAsAddLinkedAttachment.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

/**
  NewL()
  Constructs a CMtfTestActionSendAsAddLinkedAttachment object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsAddLinkedAttachment
  @pre    None
  @post   CMtfTestActionSendAsAddLinkedAttachment object is created
*/
CMtfTestAction* CMtfTestActionSendAsAddLinkedAttachment::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsAddLinkedAttachment* self = 
							new (ELeave) CMtfTestActionSendAsAddLinkedAttachment(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsAddLinkedAttachment constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsAddLinkedAttachment::CMtfTestActionSendAsAddLinkedAttachment(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsAddLinkedAttachment
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsAddLinkedAttachment::~CMtfTestActionSendAsAddLinkedAttachment()
	{
	}
	
/**
  ExecuteActionL
	Obtain the input parameters
	1.	sendAsMessage 
	2.	attachmentFile
	3.	mimeType (Default value implies empty string)
	Create a File Server session and create a RFile for the attachment File
	IF MIME type is specified 
	Call RSendAsMessage:: AddLinkedAttachment(const TDesC& aLinkedAttachmentFile,  const TDesC& aMimeType) providing attachment file and the mimeType as input parameters
	Else 
	Call RSendAsMessage:: AddLinkedAttachment(const TDesC& aLinkedAttachmentFile), providing attachment file as input parameter


  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsAddLinkedAttachment::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsAddLinkedAttachment);
	if((TestCase().TestStepResult()) == EPass)
		{

		iSendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),
											ActionParameters().Parameter(0));
											
		HBufC* linkedAttachmentFile = ObtainParameterReferenceL<HBufC>(TestCase(),
											ActionParameters().Parameter(1));
											
		HBufC8* mimeType = ObtainParameterReferenceL<HBufC8>(TestCase(),
											ActionParameters().Parameter(2), NULL);
		CActiveScheduler::Add(this);
		
		if(mimeType != NULL)
			{		
			iSendAsMessage.AddLinkedAttachment(*linkedAttachmentFile, *mimeType, iStatus);
			}
		else 
			{
			iSendAsMessage.AddLinkedAttachment(*linkedAttachmentFile, iStatus);
			}	
		SetActive();
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsAddLinkedAttachment);
	}

void CMtfTestActionSendAsAddLinkedAttachment::DoCancel()
	{
	iSendAsMessage.Cancel();
	}


void CMtfTestActionSendAsAddLinkedAttachment::RunL()
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

	TestCase().INFO_PRINTF2(_L("Add Linked Attachment completed with Error code: %d"),errorCode );
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsAddLinkedAttachment);
	TestCase().ActionCompletedL(*this);
	}

