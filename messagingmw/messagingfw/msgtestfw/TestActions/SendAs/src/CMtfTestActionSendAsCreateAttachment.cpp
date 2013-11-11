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
// SendAsCreateAttachment
// [Action Parameters]
// RSendAsMessage				sendAsMessage	<input>		: Reference of  the RSendAsMessage object
// HBufC						attachmentFile	<input>		: The file to be attached as a linked attachment
// HBufC						mimeType		<input>		: MIME type.Default value implies that no MIME type is specified
// [Action Description]
// SendAsCreateAttachment Test Action is intended to create an attachment to the message.
// If the MIME  type is provided as an input to the Test Action, the MIME type will be set.
// [APIs Used]
// RSendAsMessage::CreateAttachmentL(RFs& aFs, RFile& aAttachmentFile, const TDesC& aMimeType)
// RSendAsMessage::CreateAttachmentL(RFs& aFs, RFile& aAttachmentFile)
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "sendas2.h"
#include "CMtfTestActionSendAsCreateAttachment.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsCreateAttachment object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsCreateAttachment
  @pre    None
  @post   CMtfTestActionSendAsCreateAttachment object is created
*/
CMtfTestAction* CMtfTestActionSendAsCreateAttachment::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsCreateAttachment* self = 
							new (ELeave) CMtfTestActionSendAsCreateAttachment(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsCreateAttachment constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsCreateAttachment::CMtfTestActionSendAsCreateAttachment(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ~CMtfTestActionSendAsCreateAttachment
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsCreateAttachment::~CMtfTestActionSendAsCreateAttachment()
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
	Call RSendAsMessage::CreateAttachmentL (RFs& aFs, RFile& aAttachmentFile, const TDesC& aMimeType) providing file session, RFile and the mimeType as input parameters
    ELSE
	Call RSendAsMessage::CreateAttachmentL (RFs& aFs, RFile& aAttachmentFile) providing file session, RFile as input parameter
	END IF
	Open the attachmentFile in Read-Only mode
	Read the contents of the attachmentFile and write it to the attachment 
	file using the File handle provided by RSendAsMessage::CreateAttachmentL() function
	Close the file handles
	Close session with the file server

  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsCreateAttachment::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		RSendAsMessage sendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),
											ActionParameters().Parameter(0));
		HBufC* attachmentFileName = ObtainParameterReferenceL<HBufC>(TestCase(),
											ActionParameters().Parameter(1)); 
		HBufC* attachmentFile = ObtainParameterReferenceL<HBufC>(TestCase(),
											ActionParameters().Parameter(2)); 
		HBufC8* mimeType = ObtainParameterReferenceL<HBufC8>(TestCase(),
											ActionParameters().Parameter(3), NULL);	

		RFs fs;
		TInt err = KErrNone;
		if(fs.Connect() == KErrNone)
			{
			CleanupClosePushL(fs);
			
			RFile createdAttachment;
			if(mimeType == NULL)
				{
				TRAP(err,sendAsMessage.CreateAttachmentL(*attachmentFileName, createdAttachment));
				}
			else
				{
				TRAP(err,sendAsMessage.CreateAttachmentL(*attachmentFileName, createdAttachment, *mimeType));
				}
			CleanupClosePushL(createdAttachment);	
					
			RFile attachFile;	
			
			User::LeaveIfError(attachFile.Open(fs,*attachmentFile,EFileShareReadersOnly));
			CleanupClosePushL(attachFile);
			
			
			TInt fileSize = 0;
			User::LeaveIfError(attachFile.Size(fileSize));
			HBufC8* fileContents = HBufC8::NewLC(fileSize);
			TPtr8 fileContentsptr  = fileContents->Des();
			
			User::LeaveIfError(attachFile.Read(fileContentsptr));
					
			createdAttachment.Write(0, fileContentsptr);
			
			TInt fileSize1 = 0;
			User::LeaveIfError(createdAttachment.Size(fileSize1));
			CleanupStack::PopAndDestroy(4); // fileContents, attachFile, createdAttachment,fs
			}
		else
			{
			TestCase().ERR_PRINTF1(_L("Failed to Connect to the File Server"));
			}
		
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsCreateAttachment);
		}
	TestCase().ActionCompletedL(*this);
	}
