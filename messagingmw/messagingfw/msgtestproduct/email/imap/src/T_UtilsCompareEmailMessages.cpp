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
//



/* User includes */
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <mmsvattachmentmanager.h>
#include <t_utilscentralrepository.h>
#include "T_UtilsCompareEmailMessages.h"



/* Epoc includes */
#include <cemailaccounts.h>
#include <miutmsg.h>

/* Granularity for CArrayFixFlat arrays */
const TInt KArrayGranularity = 8;

/* Correction to attachment size */
const TReal ATTACHMENT_SIZE_CORRECTION = 1.32;



/**
  Function		 :	NewL
  Description	 :	Constructs a CT_MsgUtilsCompareEmailMessages object.
					Uses two phase construction.
  @param		 :  aTestStep  A Test step
  @return		 :  Created object of type CT_MsgUtilsCompareEmailMessages
*/
CT_MsgUtilsCompareEmailMessages* CT_MsgUtilsCompareEmailMessages::NewL( CTestStep& aTestStep)
   {
	CT_MsgUtilsCompareEmailMessages* self = new(ELeave) CT_MsgUtilsCompareEmailMessages(aTestStep);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

	
	
/**
  Function		 : CT_MsgUtilsCompareEmailMessages
  Description	 : Constructor
  @param		 : aTestStep  A Test step
  @return		 : N/A
*/
CT_MsgUtilsCompareEmailMessages::CT_MsgUtilsCompareEmailMessages(CTestStep& aTestStep)
:iTestStep(aTestStep),iResult(ETrue)
	{
	}

	
	
/**
  Function		 : ConstructL
  Description	 : 
  @return		 : N/A
*/
void CT_MsgUtilsCompareEmailMessages::ConstructL()
	{
	}

	
	
/**
  Function		 : ~CT_MsgUtilsCompareEmailMessages
  Description	 : Destructor
  @return		 : N/A
*/
CT_MsgUtilsCompareEmailMessages::~CT_MsgUtilsCompareEmailMessages()
	{
	}

	
	
/**
  Function		 : CompareEmailL
  Description	 : Compares the sent and the received messages, based on the download
				   limits set.
  @param		 : aSession			A Session object
				   aImapServiceId	Imap service Id
				   aFolderId		The remote folder Id .
				   aDownLoadLimits  The download limits set,1 - FullDownload, 0 - PartialDownload
				   aRecvMsgSelectionList	The entries under the remote folder
				   aSentMsgSelectionList	The entries under the local sent items folder
  @return		 : ETrue if the messages match
				   EFalse if the dont
*/
TBool CT_MsgUtilsCompareEmailMessages::CompareEmailL(CMsvSession& aSession,TMsvId aImapServiceId, TMsvId aFolderId,TBool aDownLoadLimits,
													  CMsvEntrySelection& aRecvMsgSelectionList,CMsvEntrySelection& aSentMsgSelectionList)
	{
	if( iResult )
		{
		iDownloadLimits = aDownLoadLimits;
				
		/* Get the download limits provided in the ini file and store the limit values */
		SetDownloadLimitsL(aImapServiceId,aFolderId);

		if( iResult )
			{
			/* Compare sent and received messages */
			CompareEmailMessagesL(aSession, aSentMsgSelectionList, aRecvMsgSelectionList);
			}
		}
	return iResult;
	}

/**
  Function		 : CompareEmailWithDLRulesL
  Description	 : Compares the sent and the received messages, based on the sync download
				   rules set.
  @param		 : aSession	A Session object
				   aImapServiceId Imap service Id
				   aFolderId The remote folder Id
				   aDownLoadLimits The download limits set, 1 - FullDownload, 0 - PartialDownload
				   aRecvMsgSelectionList The entries under the remote folder
				   aSentMsgSelectionList The entries under the local sent items folder
				   aFolderType Type of folder the sync download rules are for (Inbox or Other)
				   aBearerTypes The bearer types to search for
  @return		 : ETrue if messages match as required
				   EFalse if messages don't match as required
*/
TBool CT_MsgUtilsCompareEmailMessages::CompareEmailWithDLRulesL(CMsvSession& aSession, TMsvId aImapServiceId, TMsvId aFolderId, TBool aDownLoadLimits,
																CMsvEntrySelection& aRecvMsgSelectionList, CMsvEntrySelection& aSentMsgSelectionList,
																CImapSyncDownloadRules::TRulesType aFolderType, TUint32 aBearerTypes)
	{
	if(iResult)
		{
		iDownloadLimits = aDownLoadLimits;
				
		// Get the partial download limits from the sync download rules and store the limit values
		SetPartialDownloadLimitsUsingDLRulesL(aImapServiceId, aFolderId, aFolderType, aBearerTypes);

		if(iResult)
			{
			// Compare sent and received messages
			CompareEmailMessagesL(aSession, aSentMsgSelectionList, aRecvMsgSelectionList);
			}
		}
	return iResult;
	}

/**
  Function		 : SetPartialDownloadLimitsUsingDLRulesL
  Description	 : Checks for the partial download limits being set. Retrieves 
				   the values of the limits and stores them in member variables.
  @param		 : aImapServiceId Imap service Id
				   aFolderId The remote folder Id
				   aFolderType Type of folder the sync download rules are for (Inbox or Other)
				   aBearerTypes The bearer types to search for
  @return		 : none
*/
void CT_MsgUtilsCompareEmailMessages::SetPartialDownloadLimitsUsingDLRulesL(TMsvId aImapServiceId, TMsvId aFolderId, CImapSyncDownloadRules::TRulesType aFolderType, TUint32 aBearerTypes)
	{
	// Load the Imap account sync download rules and save the partial download limits
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();	
	CImapSyncDownloadRules* dLRules = CImapSyncDownloadRules::NewLC();

	TImapAccount imapAccount;
	emailAccounts->GetImapAccountL(aImapServiceId, imapAccount);
	emailAccounts->LoadImapSyncDownloadRulesL(imapAccount, *dLRules);
	
	// Loaded sync download rules
	TImImap4GetPartialMailInfo loadedMailInfo;
	
	TInt pos = dLRules->GetMailInfoL(aFolderType, aBearerTypes, loadedMailInfo);
	if (pos == KErrNotFound)
		{
		iTestStep.ERR_PRINTF1(_L("No matching bearer type found"));
		iResult = EFalse;
		CleanupStack::PopAndDestroy(2,emailAccounts); // dLRules, emailAccounts
		}
	// If partial download limts are set
	if(!iDownloadLimits)
		{
		// Retrieve the partial download limts set from the loaded partial mail info object
		iImapPartialDownloadLimits.iAttachmentSizeLimit = loadedMailInfo.iAttachmentSizeLimit;
		iImapPartialDownloadLimits.iBodyTextSizeLimit = loadedMailInfo.iBodyTextSizeLimit;
		iImapPartialDownloadLimits.iDestinationFolder = aFolderId;
		iImapPartialDownloadLimits.iPartialMailOptions = loadedMailInfo.iPartialMailOptions;
		iImapPartialDownloadLimits.iTotalSizeLimit 	= loadedMailInfo.iTotalSizeLimit;
		
		iTestStep.INFO_PRINTF1(_L("TImImap4GetPartialMailInfo received as input"));
		iTestStep.INFO_PRINTF4(_L("TotalSizeLimit = %d, BodyTextSizeLimit %d, AttachmentSizeLimit %d"),
								iImapPartialDownloadLimits.iTotalSizeLimit,
								iImapPartialDownloadLimits.iBodyTextSizeLimit,
								iImapPartialDownloadLimits.iAttachmentSizeLimit);
		}
	else
		{
		// Incorrect download type specified
		iTestStep.ERR_PRINTF1(_L("Incorrect download type provided as input"));
		iResult = EFalse;
		}
	CleanupStack::PopAndDestroy(2,emailAccounts); //dLRules, emailAccounts
	}

/**
  Function		 : SetDownloadLimitsL
  Description	 : Checks for the type of the download limits being set.Retrieves 
				   the values of the limits and stores them in member variables.
  @param		 : aImapServiceId	Imap service Id
				   aFolderId		The remote folder Id .				   
  @return		 : none				   
*/
void CT_MsgUtilsCompareEmailMessages::SetDownloadLimitsL(TMsvId aImapServiceId, TMsvId aFolderId)
	{

	/* Load the Imap account settings and save the download limits */
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();	
	CImImap4Settings* imapSettings =  new(ELeave) CImImap4Settings();
	CleanupStack::PushL(imapSettings);

	TImapAccount imapAccount;
	emailAccounts->GetImapAccountL(aImapServiceId, imapAccount);
	emailAccounts->LoadImapSettingsL(imapAccount, *imapSettings);

	/* If full download limts are set */
	if(iDownloadLimits)
		{
		/* Retrieve the full download limts set from the Imap settings object */
		iImapCompleteDownloadLimits.iDestinationFolder = aFolderId;
		iImapCompleteDownloadLimits.iGetMailBodyParts = imapSettings->GetMailOptions();
		iImapCompleteDownloadLimits.iMaxEmailSize = imapSettings->MaxEmailSize();
	
		iTestStep.INFO_PRINTF1(_L("TImImap4GetMailInfo received as input"));
		iTestStep.INFO_PRINTF2(_L("iMaxEmailSize = %d"),iImapCompleteDownloadLimits.iMaxEmailSize);
		}
	
	/* If partial download limts are set */
	else if(!iDownloadLimits)
		{
		/* Retrieve the partial download limts set from the Imap settings object */
		iImapPartialDownloadLimits.iAttachmentSizeLimit = imapSettings->AttachmentSizeLimit();
		iImapPartialDownloadLimits.iBodyTextSizeLimit = imapSettings->BodyTextSizeLimit();
		iImapPartialDownloadLimits.iDestinationFolder = aFolderId;
		iImapPartialDownloadLimits.iPartialMailOptions = imapSettings->PartialMailOptions();
		iImapPartialDownloadLimits.iTotalSizeLimit 	= imapSettings->MaxEmailSize();
	
		iTestStep.INFO_PRINTF1(_L("TImImap4GetPartialMailInfo received as input"));
		iTestStep.INFO_PRINTF4(_L("TotalSizeLimit = %d, BodyTextSizeLimit %d, AttachmentSizeLimit %d"),
								iImapPartialDownloadLimits.iTotalSizeLimit,
								iImapPartialDownloadLimits.iBodyTextSizeLimit,
								iImapPartialDownloadLimits.iAttachmentSizeLimit);
		}
	else
		{
		/* Incorrect download type specified */
		iTestStep.ERR_PRINTF1(_L("Incorrect Download type provided as input"));
		iResult = EFalse;
		}
	CleanupStack::PopAndDestroy(2,emailAccounts);		//imapSettings,emailAccounts
	}


/**
  Function		 : CompareEmailMessagesL
  Description	 : Compares the messages, with the same Subject field, in the Sent Items folder 
				   with the those in the Remote inbox folder.If the message is found,
				   sets the sent and received message information and then calls CompareMessagesL()
				   function to compare the contents of the message based on the download limits set.
  @param		 : aSession				Message server session
				   aSentMsgSelection	The entries under the remote folder
				   aRecvMsgSelection	The entries under the local sent items folder	   
  @return		 : none				   
*/
void CT_MsgUtilsCompareEmailMessages::CompareEmailMessagesL(CMsvSession& aSession, const CMsvEntrySelection& aSentMsgSelection,
																 const CMsvEntrySelection& aRecvMsgSelection)
	{
	/* Verify the input parameters */
	if( (!&aSentMsgSelection) || (!&aRecvMsgSelection))	
		{
		iTestStep.ERR_PRINTF1(_L("Invalid Message selection"));
		User::LeaveIfError(KErrArgument);
		}

	/* Get the count of messages in both the selection object */
	TInt sentMsgCount = aSentMsgSelection.Count();
	TInt recvMsgCount = aRecvMsgSelection.Count();

	iTestStep.INFO_PRINTF2(_L("Sent Message count = %d"),sentMsgCount);
	iTestStep.INFO_PRINTF2(_L("Received Message count = %d"),recvMsgCount);

	/* Check if all the sent messages have been received by comparing the count of the messages */
	if(recvMsgCount != sentMsgCount)
		{
		/* Number of received messages is not equal to number of sent messages */
		iTestStep.ERR_PRINTF1(_L("Sent and Received message count not equal"));
		iResult = EFalse;
		}
	if(iResult)
		{
	
		/* Loop through the list of sent messages.  For each of the sent messages,
		   verify if the message has been received.  The comparison is done based
		   on the subject field.  If message is not received - ERROR */
	
		CMsvEntry* sentMsvEntry = aSession.GetEntryL(KMsvRootIndexEntryId);
		CleanupStack::PushL(sentMsvEntry);

		CMsvEntry* recvMsvEntry = aSession.GetEntryL(KMsvRootIndexEntryId);
		CleanupStack::PushL(recvMsvEntry);

		if((!sentMsvEntry) || (!recvMsvEntry))
			{
			iTestStep.ERR_PRINTF1(_L("Invalid Message Entry"));
			User::LeaveIfError(KErrGeneral);
			}
		if(iResult)
			{
			/* Loop through the list of sent messages */
			for ( TInt sentMsgIndex = 0 ; (sentMsgIndex < sentMsgCount) &&((iResult));
											   sentMsgIndex ++)
				{
				/* Get the sent message Id */
				TMsvId sentMsgId = aSentMsgSelection.At(sentMsgIndex);

				/* Set the context to the sent message entry */
				sentMsvEntry->SetEntryL(sentMsgId);

				TBool found = EFalse;

				iTestStep.INFO_PRINTF3(_L("Comparison of Message: %d,  Subject: %S"),
							(sentMsgIndex + 1), &(sentMsvEntry->Entry().iDescription));

				/* Loop through the list received messages - Inbox folder */
				for(TInt recvMsgIndex= 0; recvMsgIndex < recvMsgCount && !found
														    ; recvMsgIndex++)	
					{
					/* Get the received message entry */
					TMsvId recvMsgId = aRecvMsgSelection.At(recvMsgIndex);

					/* Set the context to the received message entry */
					recvMsvEntry->SetEntryL(recvMsgId);
					
					/* Compare the subject field of the two messages */
					if (sentMsvEntry->Entry().iDescription.
									Compare(recvMsvEntry->Entry().iDescription) == 0)
						{
						iTestStep.INFO_PRINTF2(_L("Message with Subject field in the received selection: %S found "),
													  &(recvMsvEntry->Entry().iDescription));
						/* Sent message has been received */
						found = ETrue;

						/* Set the email information with the sent and received emails
						to be compared */
						SetEmailMessageInfoL(*sentMsvEntry, *recvMsvEntry);
						}
					}
					if ( found )
						{
						if(iResult)
							{
							/* Message is found now compare the two messages */
							if( iDownloadLimits)
								{
								CompareMessagesWithFullDownloadLimitsSetL();
								}
							else 
								{
								CompareMessagesWithPartialDownloadLimitsSetL();
								}
							if(iResult)
								{
								/* Delete the email information */
								DeleteEmailMessageInfo();
								}
							}
						}
						else
							{
							iTestStep.ERR_PRINTF2(_L("Email with subject %S is not received"), &sentMsvEntry->Entry().iDescription);
							iResult = EFalse;
							}
				}
			}
		CleanupStack::PopAndDestroy(2,sentMsvEntry); //recvMsvEntry,sentMsvEntry
		}
	}


		

/**
  Function		 : DeleteEmailMessageInfo
  Description	 : If any message information was stored for sent and received messages,
				   delete the infromation.
  @return		 : none				   
*/
void CT_MsgUtilsCompareEmailMessages::DeleteEmailMessageInfo()
	{
	/* Delete sent message information */
	if(iSentMessageInfo!=NULL)
		{
		delete 	iSentMessageInfo;
		iSentMessageInfo = NULL;
		}	
	
	/* Delete received message information */
	if(iRecvMessageInfo!=NULL)
		{
		delete 	iRecvMessageInfo ;
		iRecvMessageInfo = NULL;
		}
	}

	
	
/**
  Function		 : SetEmailMessageInfoL
  Description	 : Stores the message information sent and received messages				   
  @param		 : aSentMsvEntry	The sent message entry
				   aRecvMsvEntry	The received message entry
  @return		 : none				   
*/
void CT_MsgUtilsCompareEmailMessages::SetEmailMessageInfoL(
											CMsvEntry& aSentMsvEntry,
											CMsvEntry& aRecvMsvEntry)
	{
	/* Store sent message information */
	iTestStep.INFO_PRINTF1(_L("Setting information for SENT message"));
	if(iSentMessageInfo==NULL)
		{
		iSentMessageInfo = CT_MsgUtilsEmailMessage::NewL(aSentMsvEntry,iTestStep);
		}	
	
	/* Store received message information */
	iTestStep.INFO_PRINTF1(_L("Setting information for RECEIVED message"));
	if(iRecvMessageInfo==NULL)
		{
		iRecvMessageInfo = CT_MsgUtilsEmailMessage::NewL(aRecvMsvEntry,iTestStep);
		}
	}

	
	
/**
  Function		 : CompareMessagesWithPartialDownloadLimitsSetL
  Description	 : Compares the sent and the received message based on the partial download limits set
  @return		 : none				  
*/
void CT_MsgUtilsCompareEmailMessages::CompareMessagesWithPartialDownloadLimitsSetL()
	{

	iTestStep.INFO_PRINTF1(_L("Partial download limits set"));

	switch( iImapPartialDownloadLimits.iPartialMailOptions )
		{
		case ENoSizeLimits:
			/* No limits, the entire message was to be downloaded */
			iTestStep.INFO_PRINTF1(_L("PartailDownloadLimits ENoSizeLimits option set"));

			CompareCompleteDownloadOfMessageL();
			break;

		case EAttachmentsOnly:
			/* Only attachments to be downloaded based on the attachment size limit
			Body text and html parts should not be downloaded */
			iTestStep.INFO_PRINTF1(_L("PartailDownloadLimits EAttachmentsOnly option set"));

			CompareMessageWithAttachmentsOnlyOptionSetL();
			break;

		case EBodyTextOnly:
			/*  When EBodyTextOnly option is set, Body text and html part should
			be downloaded based on the body text limit set. The attachments
			should not be downloaded. CompareAttachmentsL() is a generic
			function which compares the attachments for any download option set.
			So this case makes use of the function 
			CompareMessagePartialDownloadWithBodyTextAndAttachmentsL() to compare 
			the body text and html contents based on the body text limit set. The 
			CompareAttachmentsL() function verifies that the attachments were not
			downloaded. */
			iTestStep.INFO_PRINTF1(_L("PartailDownloadLimits EBodyTextOnly option set"));
			/* NO BREAK */
		
		case EBodyAlternativeText:
			/* Check for Body text and html based on the body text size limit
			Check for the dowmload of text attachments */

			if(iImapPartialDownloadLimits.iPartialMailOptions  == 
											   EBodyAlternativeText)
				{
				iTestStep.INFO_PRINTF1(_L("PartailDownloadLimits EBodyAlternativeText option set"));
				}
			/* NO BREAK */
		
		case EBodyTextAndAttachments:
			/* Check for body text and html based on the body text size limit
			Check for the download of attachment based on the attachment size limit */
			if(iImapPartialDownloadLimits.iPartialMailOptions  ==  
															   EBodyTextAndAttachments)
				{
				iTestStep.INFO_PRINTF1(_L("PartailDownloadLimits EBodyTextAndAttachments option set"));
				}

			CompareMessagePartialDownloadWithBodyTextAndAttachmentsL();
			break;

		case ECumulative:
							
			/* The message parts are downloaded with the following priority
				1. Body Text 2. Attachments  3.Html part.  The total message size limit
				is used for the deciding the download of message parts.  The download of 
				the message part is stopped when the cumulative size of the downloded parts
				exceed the total size limit */

			iTestStep.INFO_PRINTF1(_L("PartailDownloadLimits ECumulative option set"));

			/* Set the cumulative size */
			iCumulativeSize = iImapPartialDownloadLimits.iTotalSizeLimit;
			CompareMessageDownloadWithCumulativeOptionSetL();
			break;

		default:
			/* TImap4GetMailOptions	iGetMailBodyParts set is not a valid value */
			iTestStep.ERR_PRINTF1(_L("Invalid TImImap4GetPartialMailInfo.iPartialMailOptions set"));
			iResult = EFalse;
			break;
		}
	}

	
	
/**
  Function		 : CompareMessagesWithFullDownloadLimitsSetL
  Description	 : Compares the sent and the received message based on the full download limits set
				   NOTE : If the size of the message is greater than the total mail size limit,
				   then the message should not be downloaded
  @return		 : none				   
*/
void CT_MsgUtilsCompareEmailMessages::CompareMessagesWithFullDownloadLimitsSetL()
	{
	
	iTestStep.INFO_PRINTF1(_L("Full download limits set."));
	
	/* Get the total message size of the sent message */
	TInt32 sentMsgSize = 0;
	User::LeaveIfError(iSentMessageInfo->GetMessageSize(sentMsgSize));
	iTestStep.INFO_PRINTF2(_L("Sent Message Size = %d"), sentMsgSize);
	
	if(sentMsgSize <= iImapCompleteDownloadLimits.iMaxEmailSize)
		{

		/* Sent message size is smaller than the maximum email size limit,so the message should be 
		downloaded based on the download option set */
		switch(iImapCompleteDownloadLimits.iGetMailBodyParts)
			{
			case EGetImap4EmailHeaders:
				/* Message headers only, entry should not be created for any message part */
				iTestStep.INFO_PRINTF1(_L("FullDownloadLimits EGetImap4EmailHeaders option set"));

				CompareMessageWithGetHeadersOptionSetL();
				break;

			case EGetImap4EmailAttachments:
				/* Download attachments only.  All attachments should be downloaded
				Body text and html parts should not be downloaded */
				iTestStep.INFO_PRINTF1(_L("FullDownloadLimits EGetImap4EmailAttachments option set"));

				CompareMessageWithAttachmentsOnlyOptionSetL();
				break;

			case EGetImap4EmailBodyText:
				/*  When EGetImap4EmailBodyText option is set, Body text and 
					html part should be downloaded, and the attachments should not
					be downloaded. CompareAttachmentsL() is a generic function
					that compares the attachments for any download option set.
					So this case makes use of the function 
					CompareCompleteDownloadOfMessageL() to compare the body text
					and html contents. The CompareAttachmentsL() function verifies
					that the attachments were not downloaded. */
				iTestStep.INFO_PRINTF1(_L("FullDownloadLimits EGetImap4EmailBodyText option set"));
				/* NO BREAK */

			case EGetImap4EmailBodyAlternativeText:
				if(iImapCompleteDownloadLimits.iGetMailBodyParts ==  
										EGetImap4EmailBodyAlternativeText)
				{
				iTestStep.INFO_PRINTF1(_L("FullDownloadLimits EGetImap4EmailBodyAlternativeText option set"));
				}
				/* NO BREAK */

			case EGetImap4EmailBodyTextAndAttachments:
				/* The entire message was to be downloaded	*/
				if(iImapCompleteDownloadLimits.iGetMailBodyParts ==  
											EGetImap4EmailBodyTextAndAttachments)
					{
					iTestStep.INFO_PRINTF1(_L("FullDownloadLimits EGetImap4EmailBodyTextAndAttachments option set"));
					}
				CompareCompleteDownloadOfMessageL();
				break;

			default:
				/* TImap4GetMailOptions	iGetMailBodyParts set is not a valid value */
				iTestStep.ERR_PRINTF1(_L("Invalid TImap4GetMailOptions.iGetMailBodyParts set"));
				iResult = EFalse;
				break;
			}
		}
	else
		{
		/* The message size was greater than the total mail size limit.
		Message parts should not be downloaded. */
		iTestStep.INFO_PRINTF1(_L("Full download limits: Message size greater than mail size limit"));
		CompareMessageFullDownloadWhenMsgSizeGreaterThanLimitL();
		}
	}


/**
  Function		 : CompareCompleteDownloadOfMessageL
  Description	 : Check if complete message was downloaded. The Body text, Html part and all the
				   attachmens should have been downloaded, if they are present in the sent message.
  @return		 : none				   
*/
void CT_MsgUtilsCompareEmailMessages::CompareCompleteDownloadOfMessageL()
	{
	/* The Body text, Html and Attachments of the message should be downloaded. */

	/* Compare Body Text contents */
	if(iResult)
		{
		ComparePlainBodyTextL(ETrue);
		}
		/* Comapre Html contents */
		if(iResult)
			{
			CompareHtmlL();
			}
			/* Compare Attachments */
			if(iResult)
				{
				CompareAttachmentsL();
				}
	}



/**
  Function		 : CompareMessageWithAttachmentsOnlyOptionSetL
  Description	 : Checks whether only attachments are downloaded.  If Body Text or Html part is
				   downloaded, the test fails
				   NOTE :Only attachments have to be downloaded. Body Text and Html part 
				   should not be downloaded
  @return		 : none				   
*/
void CT_MsgUtilsCompareEmailMessages::CompareMessageWithAttachmentsOnlyOptionSetL()
	{
	
	if (iRecvMessageInfo->GetBodyTextSize() > 0) 
		{
		/* Error : body text was downloaded when not expected */
		iTestStep.ERR_PRINTF1(_L("Body Text was downloaded when Attachment only option is set"));
		iResult = EFalse;
		}
	else
		{
		if(iSentMessageInfo->GetBodyTextSize() > 1) 
			{
			/*	When a message with only attachment is created,one byte of body text
				is getting added.  To take care of this conditions, the sent body text
				size is checked to be greater than 1 instead of 0 */

			/* Check for the correctness of Complete and Partial flags set for Body Text entry */
			iTestStep.INFO_PRINTF1(_L("Body Text was not downloaded"));
			iTestStep.INFO_PRINTF1(_L("Expected flag values for Body Text part: Complete flag = EFalse, Partial download flag = EFalse"));
			CheckFlagSettingsL( iRecvMessageInfo->GetBodyTextId(),EFalse,EFalse);
			}
		else
			{
			if(iRecvMessageInfo->GetBodyTextId() > 0)
				{
				/* No Body text part present, compelte flag should be set to ETrue
				to avoid download of Body text part */
				iTestStep.INFO_PRINTF1(_L("Body Text was not downloaded"));
				iTestStep.INFO_PRINTF1(_L("Expected flag values for Body Text part: Complete flag = ETrue, Partial download flag = EFalse"));
				CheckFlagSettingsL( iRecvMessageInfo->GetBodyTextId(),ETrue,EFalse);
				}
			}
		}
	
	/* Html part */
	if(iResult)
		{
		if(iRecvMessageInfo->GetHtmlSize() > 0)
			{
			/* Error : Html part was downloaded when not expected */
			iTestStep.ERR_PRINTF1(_L("Html part was downloaded when Attachments Only option is set"));
			iResult = EFalse;
			}
		else if(iSentMessageInfo->GetHtmlSize() > 0)
			{
			iTestStep.INFO_PRINTF1(_L("Html part was not downloaded"));
			iTestStep.INFO_PRINTF1(_L("Expected flag values for Html part: Complete flag = EFalse, Partial download flag = EFalse"));
			CheckFlagSettingsL( iRecvMessageInfo->GetHtmlEntryId(), EFalse, EFalse);
			}
		else 
			{
			if(iRecvMessageInfo->GetHtmlEntryId() > 0)
				{
				/* No Html part present, compelte flag should be set to ETrue
				to avoid download of Html part */
				iTestStep.INFO_PRINTF1(_L("Html part was not downloaded"));
				iTestStep.INFO_PRINTF1(_L("Expected flag values for Html part: Complete flag = ETrue, Partial download flag = EFalse"));
				CheckFlagSettingsL( iRecvMessageInfo->GetHtmlEntryId(), ETrue, EFalse);
				}
			}
		}

	/* Attachments */
	if(iResult)
		{
		CompareAttachmentsL();
		}
	}

	
	
/**
  Function		 : CompareMessagePartialDownloadWithBodyTextAndAttachmentsL
  Description	 : Checks for the download of Body text and Html part based on the Body text size limit
				   Calls CompareAttachment() to check the attachment download based on the attachment 
				   size limit.
  @return		 : none				   
*/
void CT_MsgUtilsCompareEmailMessages::CompareMessagePartialDownloadWithBodyTextAndAttachmentsL()
	{
	if(iResult)
		{
		/* Get the size of Body Text part of the sent and the received messages */
		TInt32 sentBodyTextSize = iSentMessageInfo->GetBodyTextSize();
		TInt32 sentHtmlSize		= iSentMessageInfo->GetHtmlSize();
		
		/* If both Body text and html part fit into the Body text limit, it is a complete
		download of Body text. Html should also be downloaded */
		if((sentBodyTextSize + sentHtmlSize) <= iImapPartialDownloadLimits.iBodyTextSizeLimit)
			{
			iTestStep.INFO_PRINTF1(_L("Both Body text and html part fit into the Body text limit"));
			
			/* Compare Body Text contents */
			ComparePlainBodyTextL(ETrue);

			if(iResult)
				{
				/* Compare Html contents */
				CompareHtmlL();
				}
			}
		else
			{
			/* If both Body text and Html does not fit in, then it is a partial download of 
		   Body text, Html part should not be downloaded */
			iTestStep.INFO_PRINTF1(_L("Partial Download of Body text"));
		
			ComparePlainBodyTextL(EFalse);

			if(iResult)
				{
				if(iRecvMessageInfo->GetHtmlSize() > 0)
					{
					/* Error : Html was downloaded when not expected */
					iTestStep.ERR_PRINTF1(_L("Html part was downloaded when Body Text+ Html size > Body text limit"));
					iResult = EFalse;
					}

				if(iResult)
					{
					if(iSentMessageInfo->GetHtmlSize() > 0)
						{
						/* When Html is not downloaded, check the correctness of the 
						complete and the partial download flags set for the Html entry */
						iTestStep.INFO_PRINTF1(_L("Html part was not downloaded"));

						iTestStep.INFO_PRINTF1(_L("Expected flag values for Html part:  Complete flag = EFalse, Partial download flag = EFalse"));
    					CheckFlagSettingsL( iRecvMessageInfo->GetHtmlEntryId(), EFalse, EFalse);
						}
					}
				}

			}
		}
	
	if(iResult)
		{
		/* Check if the attachments are downloaded if the size of the attachment
		is less than or equal to the attachment size limit */
		CompareAttachmentsL();
		}
	}

	
	
/**
  Function		 : CompareMessageWithGetHeadersOptionSetL
  Description	 : Checks if any message part was downloaded when Full download limits
				   with EGetImap4EmailHeaders option is set. If any message part is downloaded,
				   the test fails.
  @return		 : none				   
*/
void CT_MsgUtilsCompareEmailMessages::CompareMessageWithGetHeadersOptionSetL()
	{
	/* Download headers only.  The Body text, Html or Attachments of the message
	should not be downloaded.  The entries for the message parts should not be created. */
	if(iResult)
		{
		TMsvId recvBodyTextId = iRecvMessageInfo->GetBodyTextId();

		if(recvBodyTextId > 0)
			{
			/* Error : Entry created for Body Text */
			iTestStep.ERR_PRINTF1(_L("Entry for body part is created when Headers Only option is set"));
			iResult = EFalse;
			}
		else
			{
			iTestStep.INFO_PRINTF1(_L("Body Text was not downloaded"));
			}

		if(iResult)
			{
			TMsvId recvHtmlId = iRecvMessageInfo->GetHtmlEntryId();
			if( recvHtmlId > 0 )
				{
				/* Error : Entry created for Html part */
				iTestStep.ERR_PRINTF1(_L("Entry for Html part is created when Headers Only option is set"));
				iResult = EFalse;
				}
			else
				{
				iTestStep.INFO_PRINTF1(_L("Html part was not downloaded"));
				}
			}
		if(iResult)
			{
			/* Compare attachments */
			CompareAttachmentsL();
			}
		}
	}

	
	
/**
  Function		 : CompareMessageFullDownloadWhenMsgSizeGreaterThanLimitL
  Description	 : The message size is greater than the message size limit.No message parts should be
				   downloaded.  Fails if any message part was downloaded.
  @return		 : none				   
*/
 void CT_MsgUtilsCompareEmailMessages::CompareMessageFullDownloadWhenMsgSizeGreaterThanLimitL()
	{
	/* Body Text */
	if(iRecvMessageInfo->GetBodyTextSize() > 0) 
		{
		iTestStep.ERR_PRINTF1(_L("Body text downloaded when mail size is greater than the iMaxEmailSize"));
		iResult = EFalse;
		}
	else
		{
		iTestStep.INFO_PRINTF1(_L("Body text not was downloaded"));
		}	

	/* Html */
	if(iResult)
		{
		if(iRecvMessageInfo->GetHtmlSize() > 0) 
			{
			iTestStep.ERR_PRINTF1(_L("Html part downloaded when mail size is greater than the iMaxEmailSize"));
			iResult = EFalse;
			}
		else
			{
			iTestStep.INFO_PRINTF1(_L("Html part not was downloaded"));
			}
		}

	/* Attachments */
	if(iResult)
		{
		CompareAttachmentsL();
		}
	}


/**
  Function		 : CompareMessageDownloadWithCumulativeOptionSetL
  Description	 : Checks for the download of message parts when Partial download ECumulative options is set
  @return		 : none				  
*/
void CT_MsgUtilsCompareEmailMessages::CompareMessageDownloadWithCumulativeOptionSetL()
	{

	/*	When Partial download with Cumulative option is set, 
		
	The message body text part will be downloaded first. If the body text
	size is greater than the total size limit, the body text will be 
	downloaded partially and none of the other parts are downloaded.
	If the body text size is lesser than the total size limit,
	the attachment that fits into the remaining size will be downloaded.

	After download of attachments, if Html part can fit in, the Html part 
	should be downloaded */

	/* Get the size of the sent message */
	TInt32 sentMsgSize = 0;
	User::LeaveIfError(iSentMessageInfo->GetMessageSize(sentMsgSize));
	
	iTestStep.INFO_PRINTF2(_L("Sent message size = %d"), sentMsgSize);

	/* If the message is lesser than the limit, the complete message has to be downloaded */
	if(sentMsgSize  <= iImapPartialDownloadLimits.iTotalSizeLimit )
		{
		CompareCompleteDownloadOfMessageL();
		}
	else
		{
		/* Set the cumulative size */
		iCumulativeSize = iCumulativeSize - iSentMessageInfo->GetBodyTextSize();

		/* Compare the attachments first. This will help in knowing the remaining cumulative
		size after attachment download.  To decide whether the Body text was to be completely
		downloaded or not, we need to know if Html part fits in the cumulative size.
		Based on the remaining cumulative size after attachment comparison, decide whether
		Body text was to be completely downloaded or pariatlly downlaoded */

		if(iResult)
			{
			CompareAttachmentsL();
			}

		if(iResult)
			{
			/* Check if Html part fits in */
			if( (iSentMessageInfo->GetHtmlSize()) <= iCumulativeSize )
				{
				/* Compare the complete download of Body Text */
				ComparePlainBodyTextL(ETrue);

				if(iResult)
					{
					/* Compare Htmp contents */
					CompareHtmlL();
					}
				}
			else
				{
				/* Partial download of body text */
				iTestStep.INFO_PRINTF1(_L("Partial download of Body Text"));
				ComparePlainBodyTextL(EFalse);

				if(iResult)
					{
					/* Check if Html part was received. If received, fail the Test Case */
					if(iRecvMessageInfo->GetHtmlSize() > 0)
						{
						/* Error : Html was downloaded when not expected */
						iTestStep.ERR_PRINTF1(_L("Html part was downloaded when cumulative size is lesser than Html size"));
						iResult = EFalse;
						}
					else
						{
						iTestStep.INFO_PRINTF1(_L("Html part was not downloaded"));
						}

					if(iResult)
						{
						/* If Html part is not received, verify the complete and partial download flag
						set for the Html part */
						if(iSentMessageInfo->GetHtmlSize() > 0)
							{
							iTestStep.INFO_PRINTF1(_L("Expected flag values for Html part:  Complete flag = EFalse, Partial download flag = EFalse"));
							CheckFlagSettingsL( iRecvMessageInfo->GetHtmlEntryId(), EFalse, EFalse);
							}
						}
					}
				}
			}
		}
	}

	
	
/**
  Function		 : ComparePlainBodyTextL
  Description	 : Compares the contents of the received body text with that of the sent message.
				   Checks if the footer message is present and verifies the complete and partial 
				   download flags set for the Body text entry.
  @param		 : aCompleteDownload	
  @return		 : none				   
*/
void CT_MsgUtilsCompareEmailMessages::ComparePlainBodyTextL(TBool aCompleteDownload)
	{
	const TInt KPartialBodyTextCompareTolerance = 5;

	iTestStep.INFO_PRINTF1(_L("Comparing Plain text part of the message"));
		
	/* Comapre the contents when sent message had Body Text */
	if( iSentMessageInfo->GetBodyTextSize() > 0)
		{
		if( iRecvMessageInfo->GetBodyTextSize() > 0)
			{
			/* Get message body text for sent message */
			CParaFormatLayer*	sentParaFormatLayer = CParaFormatLayer::NewL();
			CleanupStack::PushL(sentParaFormatLayer);

			CCharFormatLayer*	sentCharFormatLayer = CCharFormatLayer::NewL(); 
			CleanupStack::PushL(sentCharFormatLayer);

			CRichText*	sentPlainBodyText  = CRichText::NewL( sentParaFormatLayer,
											  sentCharFormatLayer,   
											  CEditableText::EFlatStorage);
			CleanupStack::PushL(sentPlainBodyText);
			
			iSentMessageInfo->GetBodyTextL(*sentPlainBodyText, *sentParaFormatLayer,
															*sentCharFormatLayer);	

			/* Get message body text for received message */
			CParaFormatLayer*	recvParaFormatLayer = CParaFormatLayer::NewL();
			CleanupStack::PushL(recvParaFormatLayer);

			CCharFormatLayer*	recvCharFormatLayer = CCharFormatLayer::NewL(); 
			CleanupStack::PushL(recvCharFormatLayer);

			CRichText*	recvPlainBodyText = CRichText::NewL( recvParaFormatLayer, 
															 recvCharFormatLayer, 
										     CEditableText::EFlatStorage, 256);

			CleanupStack::PushL(recvPlainBodyText);

			
			iRecvMessageInfo->GetBodyTextL(*recvPlainBodyText, *recvParaFormatLayer,
															*recvCharFormatLayer);
			
			
			/* If the body text buffer is not available, fail the test case */
			if(sentPlainBodyText == NULL || recvPlainBodyText == NULL)
				{
				/* Error : Body text buffer not available */
				iTestStep.ERR_PRINTF1(_L("Body text buffer not available"));
				iResult = EFalse;
				}			

			if(iResult)
				{
				TPtrC sentBodyText = sentPlainBodyText->Read(0);
				TPtrC recvBodyText = recvPlainBodyText->Read(0);

				iTestStep.INFO_PRINTF3(_L("Sent body text size: %d, Received body text size = %d"),
											sentBodyText.Length(), recvBodyText.Length());
						
				TInt serverMsgPos = -1;
				TInt serverMsgLength = 0;

				/* Check if server has added any message to the beginning of the
				Body Text Part */

				CheckIfServerMessageExists(recvBodyText, serverMsgLength, serverMsgPos);
				
				/* Get the Body text size of the received messagge */
				TInt totalBodyTextSize = recvBodyText.Length();

				/* If server message is present, set the buffer to ignore the server message */
				if ( serverMsgPos >= 0 )
					{
					totalBodyTextSize = totalBodyTextSize - serverMsgLength;
					}
				TPtrC recvBodyTextPtr = recvBodyText.Right(totalBodyTextSize);
				
				/* Check if Footer message is present */
				TInt footerMsgPos = CheckIfFooterMessageExists(recvBodyText);
				
				if( aCompleteDownload)
					{
					/* Complete download of body text expected.  Both Body Text and Html part
					downloaded.  The Body text should not contian the footer message */

					if (footerMsgPos != KErrNotFound)
						{
						/* Error : Footer message exists when complete download of body text expected */
						iTestStep.ERR_PRINTF1(_L("Footer message exists when complete download of body text expected"));
						iResult = EFalse;
						}

					if(iResult)
						{
						TPtrC recvBodyText1 = recvBodyTextPtr.Left(recvBodyTextPtr.Length()
														- KPartialBodyTextCompareTolerance);

						TPtrC sentBodyText1 = sentBodyText.Left(recvBodyText1.Length());
						
						/* Compare the sent and received message's body text contents */
						if( (recvBodyText1.Compare(sentBodyText1)) != 0)
							{
							/* Error : Comparison of body text failed */
							iTestStep.ERR_PRINTF1(_L("Comparison of body text failed"));
							iResult = EFalse;
							}

						if(iResult)
							{
							iTestStep.INFO_PRINTF1(_L("Comparison of Body text was successful"));
							/* Verify the complete and partial download flags set */
							iTestStep.INFO_PRINTF1(_L("Expected flag values for Body Text part:  Complete flag = ETrue, Partial download flag = EFalse"));
							CheckFlagSettingsL( iRecvMessageInfo->GetBodyTextId(),
																 ETrue, EFalse );
							}
						}
					}
				else
					{
					//Partial download of body text
					/* The Body Text may be downloaded partially or completely 
					based on the body text size limit (iCumulative size in  
					case of ECumulative option).  In this case, the Html part
					should not be downloaded, hence the footer message should be
					present in the received message's body text. */

					if(iResult)
						{
						if (footerMsgPos == KErrNotFound)
							{
							/* Error : Footer message expected, but does not exists */
							iTestStep.ERR_PRINTF1(_L("Footer message expected, but does not exists"));
							iResult = EFalse;
							}
	
						if(iResult)
							{
							TPtrC recvBodyText1 = recvBodyTextPtr.Left(
												  recvBodyTextPtr.Length() - (footerMsgPos));

							
							TPtrC sentBodyText1 = sentBodyText.Left(recvBodyText1.Length());
							
							if( (recvBodyText1.Compare(sentBodyText1)) != 0)
								{
								/* Error : Comparison of body text failed */
								iTestStep.ERR_PRINTF1(_L("Comparison of body text failed"));
								iResult = EFalse;
								}

							if(iResult)
								{
								iTestStep.INFO_PRINTF1(_L("Comparison of Body text was successful"));

								/* Verify the complete and the partial download flags set */
								iTestStep.INFO_PRINTF1(_L("Expected flag values for Body Text part: Complete flag = ETrue, Partial download flag = ETrue"));

								CheckFlagSettingsL( iRecvMessageInfo->GetBodyTextId(),
																	  ETrue, ETrue);
								}
							}
						}
					}
				}
			CleanupStack::PopAndDestroy(6, sentParaFormatLayer);
			}
		else
			{
			/* Error : Sent message contains Body text. But Body text was not received */
			iTestStep.ERR_PRINTF1(_L("Sent message contains Body text. But Body text was not received"));
			iResult = EFalse;
			}
		}
	else
		{
		iTestStep.INFO_PRINTF1(_L("Sent message does not contain Body text part"));
		if(iRecvMessageInfo->GetBodyTextSize() > 0)
			{
			/* Error : Sent message does not contains Body text. But Body text was received */
			iTestStep.ERR_PRINTF1(_L("Sent message does not contains Body text. But Body text was received"));
			iResult = EFalse;
			}
		}
	}

	
	
/**
  Function		 : CheckIfFooterMessageExists
  Description	 : Checks for the footer message in the specified buffer.If footer message
				   is found, returns the position of the footer message in the buffer.
				   If not found, returns KErrNotFound
  @param		 : aBodyText	The body text of the message
  @return		 : found value, If the footer message is found in the body text				   
*/
TInt CT_MsgUtilsCompareEmailMessages::CheckIfFooterMessageExists(TPtrC aBodyText)
	{
	/* Look for the footer message in the received email message. */
	_LIT(KFooterMsg1,"This message has not been fully downloaded. There is still ");
	
	/* Find for the footer message in the received message */
	TInt found = aBodyText.Find(KFooterMsg1);

	if(found != KErrNotFound)
		{
		TPtrC footerMsg = aBodyText.Right(aBodyText.Length() - found);
		iTestStep.INFO_PRINTF2(_L("Footer message: %S"), &footerMsg);
		}
	return found;
	}


/**
  Function		 : CheckIfServerMessageExists
  Description	 : Checks if any server message has been added to the Body text returns the position of 
				   the server message in the buffer and the length of the server message.This function can 
				   be enhanced to check for any other server message encountered
				   If not found, returns KErrNotFound
  @param		 : aBodyText			The body text of the message
				   aServerMsgLength		The server message length
				   aServerMsgPosition	The position of the server message
  @return		 : none
*/
 void CT_MsgUtilsCompareEmailMessages::CheckIfServerMessageExists(
										TPtrC aBodyText, TInt& aServerMsgLength,
										TInt& aServerMsgPosition)
	{
	_LIT(KServerMessage,"*This message was transferred with a trial version of CommuniGate(r) Pro*");
	
	aServerMsgLength = KServerMessage().Length()+1;

	aServerMsgPosition = aBodyText.Find(KServerMessage);
	}

	
	
/**
  Function		 : CheckIfServerMessageExists
  Description	 : Checks if the complete and the partial download flags for the specified message part
				   is same as the expected value. Fails the test if the flags set for the message
				   part does not match with the expected value
  @param		 : aMsgPartId					The Id of the Message part
				   aExpectedCompleteFlagVal		The value of the ExpectedCompleteFlag
				   aExpectedPartialFlagVal		The value of the ExpectedPartialFlag
  @return		 : none
*/
void CT_MsgUtilsCompareEmailMessages::CheckFlagSettingsL(TMsvId aMsgPartId, 
														   TBool aExpectedCompleteFlagVal,
														   TBool aExpectedPartialFlagVal)
	{
	if( aMsgPartId > 0)
		{
		/* Check the complete and partial flag settings */
		TBool completeFlag	= EFalse;
		User::LeaveIfError(iRecvMessageInfo->CheckCompleteFlagL( aMsgPartId,completeFlag));
		
		/* Compare the set and the expected complete flag value */
		if(aExpectedCompleteFlagVal && completeFlag)
			{
			iTestStep.INFO_PRINTF1(_L("Complete Flag is set to ETrue"));
			}
		else if((aExpectedCompleteFlagVal == 0) && (completeFlag == 0))
			{
			iTestStep.INFO_PRINTF1(_L("Complete Flag is set to EFalse"));
			}
		else
			{
			iTestStep.ERR_PRINTF1(_L("Error: Incorrect Complete flag setting"));
			iTestStep.INFO_PRINTF2(_L("Complete Flag is set to %d "), completeFlag);
			iResult = EFalse;
			}

		if(iResult)
			{
			TBool partialDownloadFlag = EFalse;
			User::LeaveIfError(	iRecvMessageInfo->CheckPartialDownloadFlagL
												   (aMsgPartId, partialDownloadFlag)); 
			
			/* Compare the set and the expected partial download flag value */
			if(aExpectedPartialFlagVal && partialDownloadFlag)
				{
				iTestStep.INFO_PRINTF1(_L("Partial download flag is set to ETrue"));
				}
			else if((aExpectedPartialFlagVal == 0)  && (partialDownloadFlag == 0))
				{
				iTestStep.INFO_PRINTF1(_L("Partial download flag is set to EFalse"));
				}
			else
				{	
				iTestStep.ERR_PRINTF1(_L("Error: Incorrect Partial download flag setting"));
				iTestStep.INFO_PRINTF2(_L("Partial download is set to %d "), partialDownloadFlag);
				iResult = EFalse;
				}
			}
		} 
	else
		{
		iTestStep.ERR_PRINTF1(_L("Invalid TMsvId provided for checking complete and partial download flags"));
		iResult = EFalse;
		}
	
	}


/**
  Function		 : CheckFlagSettingsL
  Description	 :  Checks if the complete download flag for the specified message attachment is same as
					the expected value. Fails the test if the flags set for the message part does not
					match with the expected value. 
  @param		 : aMsgPart						The Message part
				   aExpectedCompleteFlagVal		The value of the ExpectedCompleteFlag
  @return		 : none
*/
void CT_MsgUtilsCompareEmailMessages::CheckFlagSettingsL(CMsvAttachment& aMsgPart, 
														   TBool aExpectedCompleteFlagVal)
	{
	/* Check the complete flag settings */
	TBool completeFlag	= aMsgPart.Complete();
	
	/* Compare the set and the expected complete flag value */
	if(aExpectedCompleteFlagVal && completeFlag)
		{
		iTestStep.INFO_PRINTF1(_L("Complete Flag is set to ETrue"));
		}
	else if((aExpectedCompleteFlagVal == 0) && (completeFlag == 0))
		{
		iTestStep.INFO_PRINTF1(_L("Complete Flag is set to EFalse"));
		}
	else
		{
		iTestStep.ERR_PRINTF1(_L("Error: Incorrect Complete flag setting"));
		iTestStep.INFO_PRINTF2(_L("Complete Flag is set to %d "), completeFlag);
		iResult = EFalse;
		}
	}


/**
  Function		 : CheckFlagSettingsL
  Description	 : Compares the contents of the sent and received html parts. Verifies the complete
				   and the partial download flags set for the received message's html part.
  @param		 : none
  @return		 : none
*/
void CT_MsgUtilsCompareEmailMessages::CompareHtmlL()
	{
	/* Get the Html file names for the sent and the received messages */
	TPtrC sentHtmlFileName;
	TInt err1 = iSentMessageInfo->HtmlFileName(sentHtmlFileName);
	TPtrC recvHtmlFileName;
	TInt err2 = iRecvMessageInfo->HtmlFileName(recvHtmlFileName);
	
	/* If the sent message has Html part, compare the received message's html part */
	if((iSentMessageInfo->GetHtmlSize() > 0) && (err1 == KErrNone))
		{
		if( err2 == KErrNone)
			{
			TInt errorVal;
			TBool diffFlag = EFalse;

			/* Compare the html files */
			CompareFilesL(sentHtmlFileName, recvHtmlFileName, errorVal, diffFlag);
			
			if (errorVal == KErrNone )
				{
				if ( diffFlag == EFalse )
					{
					iTestStep.INFO_PRINTF1(_L("Comparison of Html files was successful"));
					iTestStep.INFO_PRINTF2(_L("Sent Html file : %S"),&sentHtmlFileName );
					iTestStep.INFO_PRINTF2(_L("Received Html file : %S"),&recvHtmlFileName );
					
					/* Verify the complete and the partial download flags set */
					iTestStep.INFO_PRINTF1(_L("Expected flag values for Html part:  Complete flag = ETrue, Partial download flag = EFalse"));
					CheckFlagSettingsL( iRecvMessageInfo->GetBodyTextId(), ETrue, EFalse);
					}
				else
					{
					/* Comparison of Html files failed */
					iTestStep.ERR_PRINTF1(_L("Comparison of Html files failed"));
					iResult = EFalse;
					}
				}
			else
				{
				/* An error occured while comparing the html files */
				iTestStep.ERR_PRINTF1(_L("Error occurred while comparing Html files"));
				iResult = EFalse;
				}
			}
		else
			{
			/* Received message's html path was not found */
			iTestStep.ERR_PRINTF1(_L("Html file path not found in the received message"));
			iResult = EFalse;
			}
		}
	else
		{
		iTestStep.INFO_PRINTF1(_L("Html file path not found for the sent message"));
		}
	}


/**
  Function		 : CompareFilesL
  Description	 : Compare the contents of two files where the complete path of the two file are provided				   
  @param		 : aSentFilePath	The path of the sent file to be compared
				   aRecvFilePath    The path of the received file to be compared
				   aError			The error code
				   aDiffFlag		The difference flag
  @return		 : none
*/
 void CT_MsgUtilsCompareEmailMessages::CompareFilesL(const TPtrC aSentFilePath,
													   const TPtrC aRecvFilePath,
													   TInt& aError, TBool& aDiffFlag)
	{
	aDiffFlag = EFalse;
	CArrayFixFlat<TUint16> *ignoreCharList = new (ELeave)
											CArrayFixFlat<TUint16>(KArrayGranularity);
	CleanupStack::PushL(ignoreCharList);

	/* Compare the content of the attachment files */
	aError = CT_MsgUtils::CompareFilesL(aSentFilePath, aRecvFilePath,
														 EFalse, ignoreCharList, aDiffFlag);
	iTestStep.INFO_PRINTF2(_L("The error value is %d"),aError);
						
	CleanupStack::PopAndDestroy(ignoreCharList);
	}

	
	
/**
  Function		 : CompareAttachmentsL
  Description	 : Checks if the attachments download for all options of download limits set.
				   Verifies the Complete and Partial download flag settings for the attachments
				   in the received message based on the download option and download limits set.			   
  @param		 : 
  @return		 : none
*/
void CT_MsgUtilsCompareEmailMessages::CompareAttachmentsL()
	{
	iTestStep.INFO_PRINTF1(_L("Comparing Attachments"));

	if(iResult)
		{
		/* Get attachment list for Sent and Received message */
		CImEmailMessage* sentEmailMessage = iSentMessageInfo->GetEmailMessageL();
		
		CImEmailMessage* recvEmailMessage = iRecvMessageInfo->GetEmailMessageL();
				
		/* Call the asynchronous function to get the attachment list */
		User::LeaveIfError(iSentMessageInfo->GetMessageAttachmentListL());
		
		/* Get the list of attachments present in sent message */
		MMsvAttachmentManager& sentManager = sentEmailMessage->AttachmentManager();
		/* Get attachment list for Received message */
		
		
		/* Call the asynchronous function to get the attachment list */
		User::LeaveIfError(iRecvMessageInfo->GetMessageAttachmentListL());

		/* Get the list of attachments present in received message */
		MMsvAttachmentManager& recvManager = recvEmailMessage->AttachmentManager();
		
		TInt sentMsgAttachmentCount = sentManager.AttachmentCount();
		TInt recvMsgAttachmentCount = recvManager.AttachmentCount();

		iTestStep.INFO_PRINTF3(_L("Sent Attachments: %d, Received Attachments: %d"),
		sentMsgAttachmentCount,recvMsgAttachmentCount); 

		/* Check for the attachment count */
		CheckAttachmentCountL(sentMsgAttachmentCount, recvMsgAttachmentCount);
		if((iResult ) && recvMsgAttachmentCount)
			{
			TBool toBeDownloaded ;
			TBool downloaded	 ;
			TBool attachmentFound;

			/* Loop through Sent attachment list */
			for ( TInt index1 = 0, index2 = 0 ; (iResult ) && 
						(index1 < sentMsgAttachmentCount) ; index1++)
				{

				/* Get the name of the attachment */
				CMsvAttachment* sentAttInfo1 = sentManager.GetAttachmentInfoL(index1);
				CleanupStack::PushL(sentAttInfo1);
				TFileName fileName1 = sentAttInfo1->AttachmentName();
				
				toBeDownloaded	= EFalse;	
				downloaded		= EFalse;
				attachmentFound = EFalse;

				/* Based on the download type set, check if the attachment 
				was supposed to be downloaded or not */
				if(!iDownloadLimits)
					{
					/* Partial download limits set */
					toBeDownloaded = CheckIfAttachmentTobeDownloadedWithPartialDownloadSet
													(sentAttInfo1->AttachmentName(), sentAttInfo1->Size());
													
					}
				else
					{
					/* Full download limits set */
					toBeDownloaded = CheckIfAttachmentTobeDownloadedWithCompleteDownloadSet(sentAttInfo1->AttachmentName());	
					}

				/* Loop through the received messages to search for the attachment
				with the same name */
				for ( index2 = 0; index2 < recvMsgAttachmentCount; index2++ )
					{
					/* Get the attachment name */
					CMsvAttachment* recvAttInfo2 = recvManager.GetAttachmentInfoL(index2);
					CleanupStack::PushL(recvAttInfo2);
					TFileName fileName2 = recvAttInfo2->AttachmentName();

					/* Compare the name of the attachment in the sent message with the 
					received message's attachment name */
					if ( fileName2.Compare(fileName1) == 0 )
						{
						/* Comparison was success, the attachment found in the
						received message */
						attachmentFound = ETrue;
						
						/* Check if the attachment was downloaded */
						if ( recvAttInfo2->Complete() )
							{
							/* Complete flag set to True ==> attachment is downloaded */
							downloaded	 = ETrue;
							iTestStep.INFO_PRINTF2(_L("Attachment %S is downloaded"),&fileName2);
							}
						CleanupStack::PopAndDestroy(recvAttInfo2);
						break;
						}						
					CleanupStack::PopAndDestroy(recvAttInfo2);	
					}
				if(attachmentFound == EFalse)
					{
					/* Expected attachment is not downloaded */
					iTestStep.ERR_PRINTF2(_L("Attachment %S was not received"), &fileName1);
					/* Test step iResult is set to EFail */
					iResult = EFalse;
					}
				else
					{
					CMsvAttachment* recvAttInfo2 = recvManager.GetAttachmentInfoL(index2);
					CleanupStack::PushL(recvAttInfo2);
					if ( toBeDownloaded && downloaded )
						{
						/* Compare sent and received attachment contents */
						TInt error = 0;
						TBool diffFlag = EFalse;

						CompareFilesL(sentAttInfo1->FilePath(),
									  recvAttInfo2->FilePath(), error, diffFlag);
						
						if ((error != KErrNone) || (diffFlag) )
							{
							/* Comparison of sent and received attachment fails */
							iTestStep.ERR_PRINTF2(_L("Comparison of %S attachment failed"), 
														&fileName1);
							/* Test step iResult is set to EFail */
							iResult = EFalse;
							}

						if(iResult )
							{
							/* The attachments contents are same */
							iTestStep.INFO_PRINTF2(_L("Comparison of attachment %S was success"),
														&fileName1);
							}

						/* Verify the complete and the partial download flags set */
						iTestStep.INFO_PRINTF1(_L("Expected flag values for Attachment:  Complete flag = ETrue, Partial download flag = EFalse"));
						CheckFlagSettingsL(*recvAttInfo2 , ETrue);
						}
					else if(toBeDownloaded  && (downloaded == EFalse))
						{
						/* Expected attachment is not downloaded */
						iTestStep.ERR_PRINTF2(_L("Attachment %S was not downloaded"), 
																		&fileName1);
						/* Test step iResult is set to EFail */
						iResult = EFalse;

						}
					else if( (toBeDownloaded == EFalse)  && (downloaded)) 
						{
						
						/* Unexpected attachment has been downloaded */
						iTestStep.ERR_PRINTF2(_L("Attachment %S was received, but not expected"), 
																					&fileName1);
						/* Test step iResult is set to EFail */
						iResult = EFalse;
						}	
					else if((toBeDownloaded == EFalse)  && (downloaded == EFalse)) 
						{
						/* Verify the flags */
						iTestStep.INFO_PRINTF2(_L("The attachment %S was not downloaded"),&fileName1);	
						iTestStep.INFO_PRINTF1(_L("Expected flag values for Attachment:  Complete flag = EFalse, Partial download flag = EFalse"));
						CheckFlagSettingsL(*recvAttInfo2, EFalse);
						}
					CleanupStack::PopAndDestroy(recvAttInfo2);	
					}
				CleanupStack::PopAndDestroy(sentAttInfo1);	
				}
			}
		}
	}

	
	
/**
  Function		 : CheckAttachmentCountL
  Description	 : Checks if the number of attachments in the received message is correct. 
				   Otherwise, fails the test			   
  @param		 : aSentMsgAttachmentCount	The count of the sent messages
				   aRecvMsgAttachmentCount	The count of the received messages
  @return		 : none
*/
void CT_MsgUtilsCompareEmailMessages::CheckAttachmentCountL(TInt aSentMsgAttachmentCount, TInt aRecvMsgAttachmentCount)
	{
	TInt32 sentMsgSize;
	
	User::LeaveIfError( iSentMessageInfo->GetMessageSize(sentMsgSize) );	
	if( iDownloadLimits)
		{	
		/* If attachment download allowed */
		if( iImapCompleteDownloadLimits.iGetMailBodyParts != EGetImap4EmailHeaders && 
			iImapCompleteDownloadLimits.iGetMailBodyParts != EGetImap4EmailBodyText ) 
			{
			if( sentMsgSize <= iImapCompleteDownloadLimits.iMaxEmailSize && aSentMsgAttachmentCount != aRecvMsgAttachmentCount )
				{
				iTestStep.ERR_PRINTF1(_L("Full Download Number of attachments are not equal"));
				iResult = EFalse;
				}			
			}
		else if( aRecvMsgAttachmentCount > 0 )  // no attachment downloads allowed -should not recieve any attachments
			{
			iTestStep.ERR_PRINTF1(_L("Download with EGetImap4EmailHeaders or EGetImap4EmailBodyText Attachment count is not equal to 0"));
			iResult = EFalse;			
			}
		}
		else if(!iDownloadLimits)
		{
		/* If attatchment download allowed */
		if( iImapPartialDownloadLimits.iPartialMailOptions != EBodyTextOnly )
			{			
			/* If message size is below the max then attchments downloaded should match attachments sent */
			if( sentMsgSize <= iImapPartialDownloadLimits.iTotalSizeLimit && 
			    aSentMsgAttachmentCount != aRecvMsgAttachmentCount )
				{
				iTestStep.ERR_PRINTF1(_L("Partial Download Number of attachments are not equal"));
				iResult = EFalse;
				}
			}
		else if( aRecvMsgAttachmentCount > 0 )  // body text only - should not recieve any attachments
			{
			iTestStep.ERR_PRINTF1(_L("Download with EBodyTextOnly, Attachments will not be downloaded"));
			}		
		}
		else
		{
		iTestStep.ERR_PRINTF1(_L("Unknown value for iDownloadLimits"));
		iResult = EFalse;
		}	
	}


/**
  Function		 :  CheckIfAttachmentTobeDownloadedWithCompleteDownloadSet
  Description	 :  Checks if the attachment download when Full download options is set.
  @param		 : aAttachmentName		The name of the attachment				   
  @return		 : ETrue if attachment is to be downloaded, EFalse otherwise
*/
TBool CT_MsgUtilsCompareEmailMessages::CheckIfAttachmentTobeDownloadedWithCompleteDownloadSet
														(const TDesC& aAttachmentName )
	{
	TBool toBeCompletelyDownloadedFlag = EFalse;

	if(iDownloadLimits)
		{		
		switch(iImapCompleteDownloadLimits.iGetMailBodyParts)
			{
			case EGetImap4EmailHeaders:
				toBeCompletelyDownloadedFlag = EFalse;
			break;

			case EGetImap4EmailBodyText:
				toBeCompletelyDownloadedFlag = EFalse;
			break;

			case EGetImap4EmailAttachments:
				toBeCompletelyDownloadedFlag = ETrue;
			break;
			
			case EGetImap4EmailBodyTextAndAttachments:
				toBeCompletelyDownloadedFlag = ETrue;
			break;
			
			case EGetImap4EmailBodyAlternativeText:
				/* Check is done against filename because Mime_type field isn't filled out */
				if ( aAttachmentName.CompareF(_L("calendar.ics")) == 0 )
					{
					toBeCompletelyDownloadedFlag = ETrue;
					}
			break;
			}
		}
	return 	toBeCompletelyDownloadedFlag;
	}

/**
  Function		 :  CheckIfAttachmentTobeDownloadedWithCompleteDownloadSet
  Description	 :  Checks if the attachment download when Partial download options is set.

  @param		 : aAttachmentName		The name of the attachment
				   aAttachmentSize		The size of the attachment to be downloaded
  @return		 : ETrue if attachment is to be downloaded, EFalse otherwise
*/
TBool CT_MsgUtilsCompareEmailMessages::CheckIfAttachmentTobeDownloadedWithPartialDownloadSet
										  (const TDesC& aAttachmentName, TInt32 aAttachmentSize)
	{
	TInt32 recvAttachmentSize = (TInt32)(aAttachmentSize * ATTACHMENT_SIZE_CORRECTION);
	TBool toBePartialyDownloadedFlag = EFalse;

	if(!iDownloadLimits)
		{		
		switch(iImapPartialDownloadLimits.iPartialMailOptions)
			{
			case ENoSizeLimits:
				toBePartialyDownloadedFlag = ETrue;
			break;
			
			case EBodyTextOnly:
				toBePartialyDownloadedFlag = EFalse;
			break;
			
			case EAttachmentsOnly:
				if( recvAttachmentSize <= iImapPartialDownloadLimits.iAttachmentSizeLimit)
					{
					toBePartialyDownloadedFlag = ETrue;
					}
			break;
			
			case EBodyTextAndAttachments:
				if( recvAttachmentSize <= iImapPartialDownloadLimits.iAttachmentSizeLimit)
					{
					toBePartialyDownloadedFlag = ETrue;
					}
			break;

			case ECumulative:
				if( recvAttachmentSize <= iCumulativeSize)
					{
					toBePartialyDownloadedFlag = ETrue;
					iCumulativeSize = iCumulativeSize - recvAttachmentSize;
					}
			break;

			case EBodyAlternativeText:
				/* Check is done against filename because Mime_type field isn't filled out */
				if( recvAttachmentSize <= iImapPartialDownloadLimits.iAttachmentSizeLimit &&
					 aAttachmentName.CompareF(_L("calendar.ics")) == 0 )
					{
					toBePartialyDownloadedFlag = ETrue;
					}
			break;
			} 
		}
	return toBePartialyDownloadedFlag;
	}

/**
Function : CompareSenderL
Description : Checks that the 'From' address for a set of emails matches a
              given value.
@param aSession A Session object
@param aMsgSelection The entries under the remote folder
@param aExpectedSender Expected sender

@return ETrue if the addresses match, EFalse if not
*/
TBool CT_MsgUtilsCompareEmailMessages::CompareSenderL(CMsvSession& aSession, CMsvEntrySelection& aMsgSelection, const TDesC& aExpectedSender)
	{
	TInt msgCount = aMsgSelection.Count();

	iTestStep.INFO_PRINTF2(_L("Message count = %d"), msgCount);

	TBool matched = ETrue;
	TInt index = 0;

	while (index < msgCount && matched)
		{
		CMsvEntry* entry = aSession.GetEntryL(aMsgSelection.At(index));

		if (entry->Entry().iDetails.Compare(aExpectedSender) != 0)
			{
			iTestStep.ERR_PRINTF2(_L("Failed comparison: INdex %d"), index);
			matched = EFalse;
			}
		else
			{
			++index;
			}

		delete entry;
		}

	return matched;
	}
