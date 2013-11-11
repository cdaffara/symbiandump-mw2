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
//

/**
 @file
*/

// User include
#include "CMtfTestActionUtilsEmailMessage.h"
#include "CMtfTestCase.h"

const TInt INVALID_MSGPART_ID = -1;



/**
  NewL()
  Constructs a CMtfTestActionUtilsEmailMessage object.
  Uses two phase construction and leaves nothing on the CleanupStack.
  @internalTechnology
  @param  aMsvEntry			Message server entry for the message being accessed
  @param  aTestCase			Reference to the test case. 
  @return Created object of type CMtfTestActionUtilsEmailMessage object
  @pre    None
  @post   
*/
CMtfTestActionUtilsEmailMessage* CMtfTestActionUtilsEmailMessage::NewL(CMsvEntry& aMsvEntry,
																	  CMtfTestCase& aTestCase)
   {
	CMtfTestActionUtilsEmailMessage* self = new (ELeave) 
							CMtfTestActionUtilsEmailMessage(aMsvEntry, aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	

/**
  Constructor
  Constructs a CMtfTestActionUtilsEmailMessage object.
  @internalTechnology
  @pre    None
  @post   None
*/
CMtfTestActionUtilsEmailMessage::CMtfTestActionUtilsEmailMessage(
												 CMsvEntry& aMsvEntry, 
												 CMtfTestCase& aTestCase)
  :iEmailEntry(aMsvEntry), imEmailMessage(NULL),iBodyTextId(INVALID_MSGPART_ID),
   iBodyTextSize(0), iHtmlEntryId(INVALID_MSGPART_ID),iHtmlSize(0), iTestCase(aTestCase)
	{
	}


/**
  Destructor
  Destructor for the class CMtfTestActionUtilsEmailMessage.
  @internalTechnology
  @param  None
  @pre    None
  @post   None
*/
CMtfTestActionUtilsEmailMessage::~CMtfTestActionUtilsEmailMessage()
	{
	delete imEmailMessage;
	}


/**
  ConstructL
  Constructs a CImEmailMessage object and initializes the member variables
  to default values
  @internalTechnology
  @pre    None
  @post   None
*/
void CMtfTestActionUtilsEmailMessage::ConstructL()
	{
	// Create a email message 
	if(&iEmailEntry == NULL)
		{
		iTestCase.ERR_PRINTF1(_L("Error: Email Entry has a NULL Value"));
		User::Leave(KErrBadHandle);
		}
	imEmailMessage = CImEmailMessage::NewLC(iEmailEntry);
	
	ProcessMessageInfoL();

	iTestCase.INFO_PRINTF1(_L("Created CImEmailMessage object"));
	
	CleanupStack::Pop(imEmailMessage);
	}


/**
  ProcessMessageInfoL
  Extracts the information about the Body text, and Html part
  @internalTechnology
  @param  None
  @return KErrNone if successful
  @pre    Successful creation of CImEmailMessage object
  @post   None
*/
TInt CMtfTestActionUtilsEmailMessage::ProcessMessageInfoL()
	{
	TInt retValue = KErrNone;

	if(imEmailMessage != NULL)
		{
		// Set the information for the body text 
		retValue = StoreBodyTextInfoL() ;

		// Set the information for the Html part
		if (retValue == KErrNone)
			{
			retValue = StoreHtmlInfoL();
			}	
		}
	else
		{
		iTestCase.ERR_PRINTF1(_L("Invalid CImEmailMessage object in function ProcessMessageInfoL"));
		retValue = KErrBadHandle;
		}
	return retValue;
	}

/**
  StoreBodyTextInfoL
  Extract the body text information from CImEailMessage object and store it
  in the member variables
  @internalTechnology
  @param  None
  @return TInt KErrNone if successful
  @pre    None
  @post   None
*/		
TInt CMtfTestActionUtilsEmailMessage::StoreBodyTextInfoL()
	{
	// Get message body text in a CRichText object
	CParaFormatLayer*	paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer*	charFormatLayer = CCharFormatLayer::NewL(); 
	CleanupStack::PushL(charFormatLayer);

	CRichText*	bodyText = CRichText::NewL( paraFormatLayer, charFormatLayer, 
										   CEditableText::EFlatStorage, 256);
	CleanupStack::PushL(bodyText);

	TInt retValue = KErrNone;

	// Get the Body text contents into a CRichText object
	if(imEmailMessage != NULL)
		{
		imEmailMessage->GetBodyTextL(iEmailEntry.EntryId(), CImEmailMessage::EThisMessageOnly, *bodyText, *paraFormatLayer, *charFormatLayer);
	
		// Get the selection of body text entries
		TInt count = imEmailMessage->Selection().Count();

		// Check if any body text entry was present
		if( count > 0)
			{
			// Get the Id of the body text
			// The body text part Id is present at index 0.  Not checking for other
			//  Ids, as currently only Plain text part is verified.
			iBodyTextId = imEmailMessage->Selection().At(0);
			
			// Store the size of the message body text
			iBodyTextSize = bodyText->DocumentLength();
			}
		retValue = KErrNone;
		}
	else
		{
		iTestCase.ERR_PRINTF1(_L("Invalid CImEmailMessage object in function StoreBodyTextInfoL"));
		retValue = KErrBadHandle;
		}

	iTestCase.INFO_PRINTF2(_L("Body Text Id: %d"),iBodyTextId);
	iTestCase.INFO_PRINTF2(_L("Body Text Size: %d"),iBodyTextSize);

	// Clean the stack
	CleanupStack::PopAndDestroy(bodyText);
	CleanupStack::PopAndDestroy(charFormatLayer);
	CleanupStack::PopAndDestroy(paraFormatLayer);

	return retValue;
	}


/**
  StoreHtmlInfoL
  Extract the information for Htnl part from CImEailMessage object and store it
  in the member variables
  @internalTechnology
  @param  None
  @return KErrNone if successful
  @pre    None
  @post   None
*/		
TInt CMtfTestActionUtilsEmailMessage::StoreHtmlInfoL()
	{
	TInt retValue = KErrNone;
	
	if(imEmailMessage)
		{
		// Need to make a asynchronous function call to find the first html page
		// Create a active object
		CMsvOperationActiveSchedulerWait* waiter = CMsvOperationActiveSchedulerWait::NewLC();

		// Call asynchronous operation to get the first html
		imEmailMessage->FindFirstHTMLPageL( iEmailEntry.EntryId(),waiter->iStatus);

		// Wait for the asynchronous operation to complete
		waiter->Start();

		retValue = (waiter->iStatus).Int();

		// Destroy the active object
		CleanupStack::PopAndDestroy(waiter);

		if( retValue == KErrNone)
			{
			// Asynchronous operation is completed, get the Id and the name of the Html
			//  The complete path of the Html file will be received.
			TBool found = EFalse;
			HBufC* htmlFileName = imEmailMessage->GetUniversalResourceIdentifierL(iHtmlEntryId, found);

			// If Html part was found, look for the html file in the message store and get its size
			// Assumption: Not verifying relative URIs, since message with only one Html part considered 
			TInt htmlSize = 0;
			if ((found) && (htmlFileName != NULL))
				{
				iHtmlFileName.Set(htmlFileName->Des());	

				RFs fs;
				User::LeaveIfError(fs.Connect());
				
				RFile htmlFile;

				// Open the Html file in the read only mode
				TInt ret = htmlFile.Open(fs, iHtmlFileName, EFileRead|EFileShareReadersOnly);
				if( ret == KErrNone)
					{
					// Get the size of the Html file
					if ( htmlFile.Size(htmlSize) != KErrNone)
						{
						// Html file not present
						htmlSize = 0;
						}
					htmlFile.Close();
					}
				else
					{
					// Failure while opening the html file, set the size to 0
					htmlSize = 0;
					}
				fs.Close();
				}
			else
				{
				iTestCase.INFO_PRINTF1(_L("Html part was not found"));
				}
			// Set html size
			iHtmlSize = htmlSize;

			iTestCase.INFO_PRINTF2(_L("Html Part Entry Id: %d"),iHtmlEntryId);
			iTestCase.INFO_PRINTF2(_L("Html Part Size: %d"),iHtmlSize);
			iTestCase.INFO_PRINTF2(_L("Html Part Path: %S"),&iHtmlFileName);
			}
		}
	else
		{
		iTestCase.ERR_PRINTF1(_L("Invalid CImEmailMessage object in function StoreHtmlInfoL"));
		retValue =  KErrBadHandle;
		}	
	return retValue;
	}

/**
  GetMessageAttachmentListL
  Calls the asynchronous function CImEmailMessage::GetAttachmentsListL() to
  get the list of attachments present in the message.  The function 
  CImEmailMessage::AttachmentSelection() needs to be called after this function
  call to get the list of attachments
  @internalTechnology
  @return  KErrNone if successful
  @pre    None
  @post   CImEmailMessage::AttachmentSelection() needs to be called
*/
TInt CMtfTestActionUtilsEmailMessage::GetMessageAttachmentListL()
	{
	TInt retValue = KErrNone;
	
	if(imEmailMessage != NULL)
		{
		// Need to make a asynchronous function call to get the attachment list
		// Create an active object
		CMsvOperationActiveSchedulerWait* waiter = CMsvOperationActiveSchedulerWait::NewLC();
		
		// Call asynchronous operation to get the list of attachments to this message
		// Embedded messages not considered
		
		// Assumption:  CImEmailMessage::EThisMessageOnly is set as the parameter, because,
		//  currently, messages with out any embedded messages are considered

		imEmailMessage->GetAttachmentsListL( waiter->iStatus, iEmailEntry.Entry().Id(),
										   CImEmailMessage::EAllAttachments, 
										   CImEmailMessage::EThisMessageOnly);
								
		// Wait for the asynchronous operation to complete
		waiter->Start();
			
		retValue = (waiter->iStatus).Int();
			
		// Destroy the active object
		CleanupStack::PopAndDestroy(waiter);
		}
	else
		{
		iTestCase.ERR_PRINTF1(_L("Invalid CImEmailMessage object in function GetMessageAttachmentListL"));
		retValue = KErrBadHandle;
		}
	return retValue;
	}



/**
  GetBodyTextL
  Get body text information by calling the CImEmailMessage::GetBodyTextL() function.
  The calling function is responsible to delete the CRichText, CParaFormatLayer and 
  aCharFormatLayer objects.
  @internalTechnology
  @param  aRichText				CRichText object containing the Body text contents
  @param  aParaFormatLayer		CParaFormatLayer object of the Body text
  @param  aParaFormatLayer		CCharFormatLayer object of the Bodt text
  @pre    None
  @post   None
*/		
void CMtfTestActionUtilsEmailMessage::GetBodyTextL(CRichText& aRichText, 
													CParaFormatLayer& aParaFormatLayer,
													CCharFormatLayer& aCharFormatLayer)
	{
	// Get the Body text contents into a CRichText object
	imEmailMessage->GetBodyTextL(iEmailEntry.EntryId(), CImEmailMessage::EThisMessageOnly,
											aRichText, aParaFormatLayer, aCharFormatLayer);

	}


/**
  GetMessageId
  TMsvId of the message
  @internalTechnology
  @param  aMsvId  - Reference to the Id of the message
  @return TInt KErrNone if successful.
  @pre    None
  @post   None
*/
TInt CMtfTestActionUtilsEmailMessage::GetMessageId(TMsvId& aMsvId)
	{
	TInt retValue = KErrNone;
	if(&iEmailEntry != NULL)
		{
		aMsvId = iEmailEntry.EntryId();
		retValue = KErrNone;
		}
	else
		{
		iTestCase.ERR_PRINTF1(_L("Invalid message entry object in function GetMessageId"));
		retValue = KErrBadHandle;
		}
	return retValue;
	}


/**
  GetMessageSize
  Size of the message
  @internalTechnology
  @param  aMsgSize   Reference to size of the message
  @return TInt KErrNone if successful
  @pre    None
  @post   None
*/
TInt CMtfTestActionUtilsEmailMessage::GetMessageSize(TInt32& aMsgSize)
	{
	if(&iEmailEntry != NULL)
		{
		aMsgSize = iEmailEntry.Entry().iSize;
		return KErrNone;
		}
	else
		{
		iTestCase.ERR_PRINTF1(_L("Invalid message entry object in function GetMessageSize"));
		return KErrBadHandle;
		}
	}

/**
  GetEmailMessageL
  Returns the pointer to CImEmailMessage object
  @internalTechnology
  @param  None
  @return Pointer to CImEmailMessage object
  @pre    None
  @post   None
*/
 CImEmailMessage* CMtfTestActionUtilsEmailMessage::GetEmailMessageL()
	{
	if(imEmailMessage  == NULL)
		{
		iTestCase.ERR_PRINTF1(_L("Invalid email message object in function GetEmailMessageL"));
		User::Leave(KErrBadHandle);
		}
	return imEmailMessage;
	}


/**
  GetBodyTextId
  Returns the TMsvId of the Body Text
  @internalTechnology
  @param  None
  @return TMsvId of the Body Text
  @pre    None
  @post   None
*/
TMsvId CMtfTestActionUtilsEmailMessage::GetBodyTextId()
	{
	return iBodyTextId;
	}


/**
  GetBodyTextSize
  Returns the size of the Body Text
  @internalTechnology
  @param  None
  @return Size of the Body Text
  @pre    None
  @post   None
*/
TInt32 CMtfTestActionUtilsEmailMessage::GetBodyTextSize()
	{
	return iBodyTextSize;
	}



/**
  GetHtmlEntryId
  Returns the TMsvId of the Html part
  @internalTechnology
  @param  None
  @return TMsvId of the Html part
  @pre    None
  @post   None
*/
TInt32 CMtfTestActionUtilsEmailMessage::GetHtmlEntryId()
	{
	return iHtmlEntryId;
	}


/**
  GetHtmlSize
  Returns the size of the Html file
  @internalTechnology
  @param  None
  @return Size of the Html file
  @pre    None
  @post   None
*/
TInt32 CMtfTestActionUtilsEmailMessage::GetHtmlSize()
	{
	return iHtmlSize;
	}
	

/**
  HtmlFileName
  Provides the Html file name
  @internalTechnology
  @param  aHtmlFileName  Reference to the buffer contianing Html file name
  @return TInt Error value
  @pre    None
  @post   None
*/
TInt CMtfTestActionUtilsEmailMessage::HtmlFileName(TPtrC& aHtmlFileName)
	{
	// Set the Html file name if the file path is present, else return error
	TInt error = KErrNotFound;

	if (iHtmlFileName.Ptr())
		{
		aHtmlFileName.Set(iHtmlFileName);
		error = KErrNone;
		}
	return error;
	}



/**
  CheckCompleteFlagL
  Check if the complete flag of the message part is set
  @internalTechnology
  @param  aMsgId		- Id of the message part
  @return ETrue if complete flag is set, EFalse otherwise
  @pre    None
  @post	  None
*/
TInt CMtfTestActionUtilsEmailMessage::CheckCompleteFlagL (
									TMsvId aMsgId, 
								    TBool& aCompleteFlag)
	{
	TInt retValue = KErrNone;

	// Check if the message Id is valid
	if(aMsgId == INVALID_MSGPART_ID)
		{
		// Invalid Id passed to the function
		retValue = KErrBadHandle;
		}
	else
		{
		CMsvEntry* msgEntry = iEmailEntry.Session().GetEntryL(aMsgId);
		CleanupStack::PushL(msgEntry);

		if(msgEntry)
			{
			msgEntry->SetEntryL(aMsgId);
			TMsvEntry msvEntry = msgEntry->Entry();

			aCompleteFlag = msvEntry.Complete();
			}
		else
			{
			retValue = KErrBadHandle;
			}
		CleanupStack::PopAndDestroy(msgEntry);
		}
	return retValue;
	}

/**
  CheckPartialDownloadFlagL
  Check if the partial download flag for the message part is set
  @internalTechnology
  @param  aEntry		- Id of the message part
  @return ETrue if partial download flag is set, EFalse otherwise
  @pre    None
  @post	  None
*/
TInt CMtfTestActionUtilsEmailMessage::CheckPartialDownloadFlagL(
											TMsvId aMsgId,
											TBool& aPartialDownloadFlag)
	{
	TInt retValue = KErrNone;

	// Check if the message Id is valid
	if(aMsgId == INVALID_MSGPART_ID)
		{
		// Invalid Id passed to the function
		retValue = KErrBadHandle;
		}
	else
		{
		CMsvEntry* msgEntry = iEmailEntry.Session().GetEntryL(aMsgId);
		CleanupStack::PushL(msgEntry);

		if(msgEntry)
			{
			msgEntry->SetEntryL(aMsgId);

			TMsvEmailEntry msvEntry = msgEntry->Entry();

			aPartialDownloadFlag = msvEntry.PartialDownloaded();
			}
		else
			{
			retValue = KErrBadHandle;
			}
		CleanupStack::PopAndDestroy(msgEntry);
		}
	return retValue;
	}
