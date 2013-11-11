// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file 
// [Utility Name]
// CT_MsgUtilsEmailMessage
// Stores the information of an email message. Creates a CImEmailMessage object 
// for the message. Any operations related to the email message can be perfored using
// this the CImEmailMessage object.
// The class stores the message part Id and the size information for 
// Body Text and Html and also the Html path.
// If the Body Text part is modified, then StoreBodyTextInfoL() function needs to be 
// called to update the stored Body Text information.  Similarly, StoreHtmlInfoL() function
// needs to be called when Html part is modified by the email message object.
// 
//


/* User includes */
#include "T_UtilsEmailMessage.h"

/* Epoc Include */
#include <miutmsg.h>

const TInt KINVALID_MSGPART_ID = -1;


/**
  Function : NewL
  Description : Constructs a CT_MsgUtilsEmailMessage object.
				Uses two phase construction and leaves nothing on the CleanupStack.
  @param :	aMsvEntry	An object of type CMsvEntry
			aTestStep	A reference to a test step
  @return : Returns an object of type CT_MsgUtilsEmailMessage object.
*/
CT_MsgUtilsEmailMessage* CT_MsgUtilsEmailMessage::NewL( CMsvEntry& aMsvEntry,CTestStep& aTestStep)
   {
	CT_MsgUtilsEmailMessage* self = new (ELeave) CT_MsgUtilsEmailMessage(aMsvEntry , aTestStep);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

	
	
/**
  Function : CT_MsgUtilsEmailMessage
  Description : Constructor
  @param :	aMsvEntry	An object of type CMsvEntry
			aTestStep	A reference to a test step
  @return : none
*/
CT_MsgUtilsEmailMessage::CT_MsgUtilsEmailMessage( CMsvEntry& aMsvEntry , CTestStep& aTestStep )
  : iEmailEntry(aMsvEntry), iEmailMessage(NULL),				iBodyTextId(KINVALID_MSGPART_ID),
    iBodyTextSize(0),       iHtmlEntryId(KINVALID_MSGPART_ID),	iHtmlSize(0),	iTestStep(aTestStep)
	{
	}

	
	
/**
  Function : ConstructL
  Description : Constructs a CImEmailMessage object and initializes the member variables
				to default values
  @return : none
*/
void CT_MsgUtilsEmailMessage::ConstructL()
	{
	/* Create a email message */
	if(&iEmailEntry == NULL)
		{
		iTestStep.ERR_PRINTF1(_L("Error: Email Entry has a NULL Value"));
		User::Leave(KErrBadHandle);
		}
	iEmailMessage = CImEmailMessage::NewLC(iEmailEntry);
	ProcessMessageInfoL();

	iTestStep.INFO_PRINTF1(_L("Created CImEmailMessage object"));
	CleanupStack::Pop(iEmailMessage);
	}



/**
  Function : ~CT_MsgUtilsEmailMessage
  Description : Destructor
  @param :
  @return : N/A
*/
CT_MsgUtilsEmailMessage::~CT_MsgUtilsEmailMessage()
	{
	delete iEmailMessage;
	iEmailMessage=NULL;
	}



/**
  Function : ProcessMessageInfoL
  Description : Extracts the information about the Body text, and Html part
  @return : KErrNone if successful, else the appropriate error code
*/
TInt CT_MsgUtilsEmailMessage::ProcessMessageInfoL()
	{
	TInt retValue = KErrNone;

	if(iEmailMessage != NULL)
		{
		/* Sets the information for the body text */
		retValue = StoreBodyTextInfoL() ;

		/* Sets the information for the Html part */
		if (retValue == KErrNone)
			{
			retValue = StoreHtmlInfoL();
			}	
		}
	else
		{
		iTestStep.ERR_PRINTF1(_L("Invalid CImEmailMessage object in function ProcessMessageInfoL"));
		retValue = KErrBadHandle;
		}
	return retValue;
	}

	
	
/**
  Function : StoreBodyTextInfoL
  Description : Extract the body text information from CImEailMessage object and store it
				in the member variables
  @return : KErrNone if successful, else the appropriate error code
*/
TInt CT_MsgUtilsEmailMessage::StoreBodyTextInfoL()
	{
	/* Get message body text in a CRichText object */
	CParaFormatLayer*	paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer*	charFormatLayer = CCharFormatLayer::NewL(); 
	CleanupStack::PushL(charFormatLayer);

	CRichText*	bodyText = CRichText::NewL( paraFormatLayer, charFormatLayer, 
										   CEditableText::EFlatStorage, 256);//EDefaultTextGranularity bytes (=256)
	CleanupStack::PushL(bodyText);

	TInt retValue = KErrNone;

	/* Gets the Body text contents into a CRichText object */
	if(iEmailMessage != NULL)
		{
		iEmailMessage->GetBodyTextL(iEmailEntry.EntryId(), CImEmailMessage::EThisMessageOnly, *bodyText, *paraFormatLayer, *charFormatLayer);
	
		/* Gets the selection of body text entries */
		TInt count = iEmailMessage->Selection().Count();

		/* Check if any body text entry was present */
		if( count > 0)
			{
			/* Gets the Id of the body text
			The body text part Id is present at index 0.  Not checking for other
			Ids, as currently only Plain text part is verified. */
			iBodyTextId = iEmailMessage->Selection().At(0);
			
			/* Stores the size of the message body text */
			iBodyTextSize = bodyText->DocumentLength();
			}
		retValue = KErrNone;
		}
	else
		{
		iTestStep.ERR_PRINTF1(_L("Invalid CImEmailMessage object in function StoreBodyTextInfoL"));
		retValue = KErrBadHandle;
		}

		iTestStep.INFO_PRINTF2(_L("Body Text Id: %d"),iBodyTextId);
		iTestStep.INFO_PRINTF2(_L("Body Text Size: %d"),iBodyTextSize);
		
		/* Cleans the stack */
		CleanupStack::PopAndDestroy(3,paraFormatLayer); // bodyText,charFormatLayer,paraFormatLayer
		return retValue;
	}



/**
  Function : StoreBodyTextInfoL
  Description : Extract the information for Htnl part from CImEailMessage object and store it
				in the member variables
  @return : KErrNone if successful, else the appropriate error code
*/
TInt CT_MsgUtilsEmailMessage::StoreHtmlInfoL()
	{
	TInt retValue = KErrNone;
	
	if(iEmailMessage)
		{
		/* Need to make a asynchronous function call to find the first html page
		Create a active object */
		CMsvOperationActiveSchedulerWait* waiter = CMsvOperationActiveSchedulerWait::NewLC();

		/* Calls asynchronous operation to get the first html */
		iEmailMessage->FindFirstHTMLPageL( iEmailEntry.EntryId(),waiter->iStatus);

		/* Waits for the asynchronous operation to complete */
		waiter->Start();

		retValue = (waiter->iStatus).Int();

		/* Destroy the active object */
		CleanupStack::PopAndDestroy(waiter);

		if( retValue == KErrNone)
			{
			/* Asynchronous operation is completed, get the Id and the name of the Html
			The complete path of the Html file will be received. */
			TBool found = EFalse;
			HBufC* htmlFileName = iEmailMessage->GetUniversalResourceIdentifierL(iHtmlEntryId, found);
			CleanupStack::PushL(htmlFileName);
			
			/* If Html part was found, look for the html file in the message store and get its size
			Assumption: Not verifying relative URIs, since message with only one Html part considered  */
			TInt htmlSize = 0;
			if ((found) && (htmlFileName != NULL))
				{
				iHtmlFileNameBuf.Copy(htmlFileName->Des());				

				RFs fs;
				User::LeaveIfError(fs.Connect());
				
				RFile htmlFile;

				/* Open the Html file in the read only mode */
				TInt ret = htmlFile.Open(fs, iHtmlFileNameBuf, EFileRead|EFileShareReadersOnly);
				if( ret == KErrNone)
					{
					/* Get the size of the Html file */
					if ( htmlFile.Size(htmlSize) != KErrNone)
						{
						/* Html file not present */
						htmlSize = 0;
						}
					htmlFile.Close();
					}
				else
					{
					/* Failure while opening the html file, set the size to 0 */
					htmlSize = 0;
					}
				fs.Close();
				}
			else
				{
				iTestStep.INFO_PRINTF1(_L("Html part was not found"));
				}
			/* Set html size */
			iHtmlSize = htmlSize;

			iTestStep.INFO_PRINTF2(_L("Html Part Entry Id: %d"),iHtmlEntryId);
			iTestStep.INFO_PRINTF2(_L("Html Part Size: %d"),iHtmlSize);
			iTestStep.INFO_PRINTF2(_L("Html Part Path: %S"),&iHtmlFileNameBuf);
			CleanupStack::PopAndDestroy(htmlFileName);		
			}
		}
	else
		{
		iTestStep.ERR_PRINTF1(_L("Invalid CImEmailMessage object in function StoreHtmlInfoL"));
		retValue =  KErrBadHandle;
		}	
	return retValue;
	}





/**
  Function : GetMessageAttachmentListL
  Description : Calls the asynchronous function CImEmailMessage::GetAttachmentsListL() to
				get the list of attachments present in the message.  The function 
				CImEmailMessage::AttachmentSelection() needs to be called after this function
				call to get the list of attachments
  @return : KErrNone if successful, else the appropriate error code
*/
TInt CT_MsgUtilsEmailMessage::GetMessageAttachmentListL()
	{
	TInt retValue = KErrNone;
	
	if(iEmailMessage != NULL)
		{
		/* Need to make a asynchronous function call to get the attachment list
		Create an active object */
		CMsvOperationActiveSchedulerWait* waiter = CMsvOperationActiveSchedulerWait::NewLC();
		
		/* Call asynchronous operation to get the list of attachments to this message
		Embedded messages not considered */

		iEmailMessage->GetAttachmentsListL( waiter->iStatus, iEmailEntry.Entry().Id(),
										   CImEmailMessage::EAllAttachments, 
										   CImEmailMessage::EThisMessageOnly);
								
		/* Wait for the asynchronous operation to complete */
		waiter->Start();
			
		retValue = (waiter->iStatus).Int();
			
		/* Destroy the active object */
		CleanupStack::PopAndDestroy(waiter);
		}
	else
		{
		iTestStep.ERR_PRINTF1(_L("Invalid CImEmailMessage object in function GetMessageAttachmentListL"));
		retValue = KErrBadHandle;
		}
	return retValue;
	}



/**
  Function : GetBodyTextL
  Description : Get body text information by calling the CImEmailMessage::GetBodyTextL() function.
				The calling function is responsible to delete the CRichText, CParaFormatLayer and 
				aCharFormatLayer objects.
  @param :	aRichText		An object of type CRichText
			aParaFormatLayer	An object of type CParaFormatLayer
			aCharFormatLayer	An object of type CCharFormatLayer
  @return : none
*/
void CT_MsgUtilsEmailMessage::GetBodyTextL(CRichText& aRichText, 
													CParaFormatLayer& aParaFormatLayer,
													CCharFormatLayer& aCharFormatLayer)
	{
	// Get the Body text contents into a CRichText object
	iEmailMessage->GetBodyTextL(iEmailEntry.EntryId(), CImEmailMessage::EThisMessageOnly,
											aRichText, aParaFormatLayer, aCharFormatLayer);

	}

	
	
/**
  Function : GetMessageId
  Description : Gets the TMsvId of the message
  @param :	aMsvId		
  @return : KerrNone if successful
*/
TInt CT_MsgUtilsEmailMessage::GetMessageId(TMsvId& aMsvId)
	{
	TInt retValue = KErrNone;
	if(&iEmailEntry != NULL)
		{
		aMsvId = iEmailEntry.EntryId();
		retValue = KErrNone;
		}
	else
		{
		iTestStep.ERR_PRINTF1(_L("Invalid message entry object in function GetMessageId"));
		retValue = KErrBadHandle;
		}
	return retValue;
	}


/**
  Function : GetMessageSize
  Description : Gets the size of the message
  @param :	aMsgSize		
  @return : KerrNone if successful
*/
TInt CT_MsgUtilsEmailMessage::GetMessageSize(TInt32& aMsgSize)
	{
	if(&iEmailEntry != NULL)
		{
		aMsgSize = iEmailEntry.Entry().iSize;
		return KErrNone;
		}
	else
		{
		iTestStep.ERR_PRINTF1(_L("Invalid message entry object in function GetMessageSize"));
		return KErrBadHandle;
		}
	}



/**
  Function : GetEmailMessageL
  Description : Checks if the email message object is NULL
  @return : Returns the pointer to CImEmailMessage object
*/
 CImEmailMessage* CT_MsgUtilsEmailMessage::GetEmailMessageL()
	{
	if(iEmailMessage  == NULL)
		{
		iTestStep.ERR_PRINTF1(_L("Invalid email message object in function GetEmailMessageL"));
		User::Leave(KErrBadHandle);
		}
	return iEmailMessage;
	}



/**
  Function : GetBodyTextId
  Description : Gets the body text Id of the message
  @return : Returns the TMsvId of the Body Text
*/
TMsvId CT_MsgUtilsEmailMessage::GetBodyTextId()
	{
	return iBodyTextId;
	}



/**
  Function : GetBodyTextSize
  Description : Gets the body text size of the message
  @return : Returns the size of the Body Text
*/
TInt32 CT_MsgUtilsEmailMessage::GetBodyTextSize()
	{
	return iBodyTextSize;
	}
  


/**
  Function : GetHtmlEntryId
  Description : Gets the HTML entry Id of the message
  @return : Returns the TMsvId of the Html part
*/
TInt32 CT_MsgUtilsEmailMessage::GetHtmlEntryId()
	{
	return iHtmlEntryId;
	}


/**
  Function : GetHtmlSize
  Description : Gets the HTML size of the message
  @return : Returns the size of the Html file
*/
TInt32 CT_MsgUtilsEmailMessage::GetHtmlSize()
	{
	return iHtmlSize;
	}
	


/**
  Function : HtmlFileName
  Description : Checks for the HTML file 
  @param :	aHtmlFileName	The name of the HTML file
  @return : Returns the error code
*/
TInt CT_MsgUtilsEmailMessage::HtmlFileName(TPtrC& aHtmlFileName)
	{
	/* Set the Html file name if the file path is present, else return error */
	TInt error = KErrNotFound;
	aHtmlFileName.Set(iHtmlFileNameBuf);
	return error;
	}



/**
  Function : CheckCompleteFlagL
  Description : Check if the complete flag of the message part is set
  @param :	aMsgId		The message Id
			aCompleteFlag	The complete flag, ETrue or EFalse
  @return : Returns the error code
*/
TInt CT_MsgUtilsEmailMessage::CheckCompleteFlagL (
									TMsvId aMsgId, 
								    TBool& aCompleteFlag)
	{
	TInt retValue = KErrNone;

	/* Check if the message Id is valid */
	if(aMsgId == KINVALID_MSGPART_ID)
		{
		/* Invalid Id passed to the function */
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
  Function : CheckPartialDownloadFlagL
  Description : Check if the partial download flag for the message part is set
  @param :	aMsgId		The message Id
			aPartialDownloadFlag	The Partial flag, ETrue or EFalse
  @return : Returns the error code
*/
TInt CT_MsgUtilsEmailMessage::CheckPartialDownloadFlagL(
											TMsvId aMsgId,
											TBool& aPartialDownloadFlag)
	{
	TInt retValue = KErrNone;

	/* Check if the message Id is valid */
	if(aMsgId == KINVALID_MSGPART_ID)
		{
		/* Invalid Id passed to the function */
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

