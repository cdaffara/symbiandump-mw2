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
// CompareEmailMessages
// [Action Parameters]
// CMsvSession&			session   <input>			: Reference to the session.
// CMsvEntrySelection&	SentMailSelection <input>	: Reference to the
// sent mail selection
// CMsvEntrySelection&	RecvMailSelection <input>	: Reference to the
// received mail selection
// CMtfTestParameter&	Download limits <input>		: The CMTfTestParameter object
// which stores either full download
// limits or partial download limits
// that was set during mail download
// [Action Description]
// Compares the sent and received email messages for size and contents based on
// the download limits set.  The sent and received messages with the same 
// Subject field are compared.  The Complete and Partial download flags set for 
// the message parts are verified.
// Restrictions:
// 1. The Test Action supports comparison of simple emails messages with Body Text,
// Html part and attachments of B64 encoding format.  Needs to be enhanced to 
// support emails with embedded messages, all attachment types and MHTML messages.
// 2. The size of the encoded message parts (Body text, Attachment and Html) are not 
// available as there are no APIs available to get this information
// Workarounds in the Test Action:
// For Body Text part:
// 1. [WorkAround for Body Text Ref1]
// The size of the encoded body text is not available. Hence the size of the 
// body text in the message store will be used.  In the Test Case, care has
// to be taken to have the limits set, such that, the limits considers the 
// size of the encoded body text part and not the size of the body text present
// in the store.
// 2. [WorkAround for Body Text Ref2]
// For the defect "The CRLF characters is NOT interpreted  correctly".
// The body text size is reduced by the tolerance length to overcome
// the message comparison failure
// 3. [WorkAround for Body Text Ref3]
// The Communigate Pro server adds a disclaimer at the beginning
// of the body text in the received message.  This message has to be removed 
// from the body text of the received message before comparison.  The function
// needs to be enhanced to handle different server messages.
// 4. [WorkAround for Body Text Ref4]
// For the defect "The size of the body text in the sent and the received emails
// differ by 2 bytes.".  The size of the sent message's body text is set to
// that of the received message's body text size.
// 5. [WorkAround for Body Text Ref5]
// For the defect "For partially downloaded Body text of a message, 
// the corresponding TMsvEntry.iSize represents the size present in the 
// RFC822 message body text size. It does NOT indicate the partially downloaded
// body text size".  The size of the set message is set to that of partially
// downloaded body text before comparing.  It is required to manually verify
// the size of the partially downloaded message text in the email log files.
// 6. [WorkAround for Body Text Ref6]
// The remaining size of the body text or Html part on the server 
// is not available, hence expected remaining size of the Body text on
// the server cannot be calculated.  Hence verification of the footer message
// information is not possible.  The value of the remaining size
// of the body text size on the server needs to be manually verified 
// by checking the email logs. In the test action, the footer message present
// in the buffer is printed in the log file.
// 7. [WorkAround for Body Text Ref7]
// When a message with only attachment is created, one byte of body text is
// getting added.  To take care of this conditions, the sent body text size
// is checked for being greater than 1 instead of 0.  This means the Body text
// part used for testing should be greater than 1 Byte.
// For Html part:
// 1. [WorkAround for Html Part Ref1]
// Not verifying relative URIs, since message with only one Html part considered
// 2. [WorkAround for Html Part Ref2]
// The size of the encoded html part is not available, hence the size of the Html 
// file in the message store is considered.  Test Case need to ensure that the 
// download limits consider the encoded Html size rather than the size got from the store
// For Attachments:
// 1.	[WorkAround for Attachments Ref1]
// The attachment download limits refers to the encoded attachment 
// size.  This size is not available through any of the APIs.  The
// available size of the attachment is the size of the file that 
// is present in the message store. This size is smaller than the 
// encoded size. Hence adding a correction value to get the approximate
// size of the encoded attachment.
// Note: this correction holds good for only B64 encoding.
// For All Message Parts:
// 1.   [WorkAround for general Ref1]
// Though the TMsvEntry::Complete() and TMsvEmailEntry::PartialDownloaded()
// returns a TBool value, the returned value will not equal to the value of 
// ETrue and EFalse.  Hence the direct comparison of the set flag value 
// and the expected flag value is not possible
// [APIs Used]
// CImEmailMessage::AttachmentSelection()
// CMsvEntry::SetEntry()
// __ACTION_INFO_END__
// 
//

/**
 @file
*/



#include <cemailaccounts.h>
#include <iapprefs.h>
#include <imapset.h>
#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>
#include <miutmsg.h>

// User include
#include "CMtfTestActionCompareEmailMessages.h"
#include "CMtfTestActionParameters.h"
#include "TestFrameworkActionsUtils.h"


//Granularity for CArrayFixFlat arrays
const TInt KArrayGranularity = 8;

// Correction to attachment size
const TReal ATTACHMENT_SIZE_CORRECTION = 1.32;

/**
  NewL()
  Constructs a CMtfTestActionCompareEmailMessages object.
  Uses two phase construction and leaves nothing on the CleanupStack.
  @internalTechnology
  @param  aTestCase          Test Case to which the Test Action belongs
  @param  aActionParameters  Action parameters, must not be NULL
  @return Created object of type CMtfTestAction
  @pre    None
  @post   None
*/
CMtfTestAction* CMtfTestActionCompareEmailMessages::NewL(CMtfTestCase& aTestCase,
													CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareEmailMessages* self = 
					new (ELeave) CMtfTestActionCompareEmailMessages(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Constructor
  Constructs a CMtfTestActionCompareEmailMessages object and initialises the member variables.
  @internalTechnology
  @param  aTestCase  Test Case to which the Test Action belongs
  @pre    None
  @post   None
*/
CMtfTestActionCompareEmailMessages::CMtfTestActionCompareEmailMessages(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase), iParamSession(NULL), iDownloadType(EFullDownload),
  	  iCumulativeSize(0), iSentMessageInfo(NULL), iRecvMessageInfo(NULL)
	{
	}

/**
  Destructor
  Destructor of the class CMtfTestActionCompareEmailMessages.
  Deletes the message information stored in the CMtfTestActionUtilsEmailMessage
  objects
  @internalTechnology
  @param  None
  @pre    None
  @post   None
*/
CMtfTestActionCompareEmailMessages::~CMtfTestActionCompareEmailMessages()
	{
	delete 	iSentMessageInfo;
	delete 	iRecvMessageInfo;
	}


/**
  ExecuteActionL
  Obtains the parameters for the test action and then calls CompareEmailMessagesL()
  function to compare the sent and the received messages, based on the download
  limits set.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareEmailMessages);

	// Get the session object 
	iParamSession				= ObtainParameterReferenceL<CMsvSession>
									(TestCase(),ActionParameters().Parameter(0));
	
		
	// Get the selection object of messages in sent folder - Sent
	CMsvEntrySelection*	sentMsgSelectionList = 
									ObtainParameterReferenceL<CMsvEntrySelection>
									(TestCase(), ActionParameters().Parameter(1)); 

	// Get the selection object of messages in received folder - Inbox
	CMsvEntrySelection*	recvMsgSelectionList = 
									ObtainParameterReferenceL<CMsvEntrySelection>
									(TestCase(), ActionParameters().Parameter(2)); 
	

	if( iParamSession == NULL || sentMsgSelectionList == NULL ||
		 recvMsgSelectionList == NULL)
		{
		// Invalid input parameter value
		TestCase().ERR_PRINTF1(_L("Invalid session or message selection object found"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		if(IsTestCaseStatusPass())
			{
			// Get the download limits provided as the test action input and
			// store the limit values
			SetDownloadLimits();

			if(IsTestCaseStatusPass())
				{
				// Compare sent and received messages
				CompareEmailMessagesL(*sentMsgSelectionList, *recvMsgSelectionList);
				}
			}
		}
	// Test Action is complete
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareEmailMessages);
	TestCase().ActionCompletedL(*this);
	}	


/**
  SetDownloadLimitsL
  Obtains the parameter that contians download limits.  Checks for the type of
  the download limits being set.  Retrieves the values of the limits and stores
  them in member variables.
  @internalTechnology
  @pre    None
  @post   None
*/


void CMtfTestActionCompareEmailMessages::SetDownloadLimits()
	{
	// Get the Partial download limits or  full download limits
	const CMtfTestParameter& paramDownloadLimits = TestCase().ObtainParameterL
										  	 (ActionParameters().Parameter(3));

	// Buffer to store the download limits set
	HBufC8* buf=reinterpret_cast<HBufC8*>(paramDownloadLimits.Parameter());


	// Verify CMtfTestParameter object - Partial/Complete 
	if ( paramDownloadLimits.SpecificTypeId() == EMtfTImImap4GetPartialMailInfo)
		{
		// Partial download limits set
		TImImap4GetPartialMailInfo value;
		TPckgC<TImImap4GetPartialMailInfo> unpack(value);
		unpack.Set(*buf);
		iImapPartialDownloadLimits = unpack();
		
	
		
		
		// Set the download type as partial download
		iDownloadType = EPartialDownload;

		TestCase().INFO_PRINTF1(_L("TImImap4GetPartialMailInfo received as input"));
		TestCase().INFO_PRINTF4(_L("TotalSizeLimit = %d, BodyTextSizeLimit %d, AttachmentSizeLimit %d"),
								iImapPartialDownloadLimits.iTotalSizeLimit,
								iImapPartialDownloadLimits.iBodyTextSizeLimit,
								iImapPartialDownloadLimits.iAttachmentSizeLimit);
		}
	else if(paramDownloadLimits.SpecificTypeId() == EMtfTImImap4GetMailInfo)
		{
		// Full download limits set
		TImImap4GetMailInfo value;
		TPckgC<TImImap4GetMailInfo> unpack(value);
		unpack.Set(*buf);
		iImapCompleteDownloadLimits = unpack();

		// Set the download type as full download
		iDownloadType = EFullDownload;

		TestCase().INFO_PRINTF1(_L("TImImap4GetMailInfo received as input"));
		TestCase().INFO_PRINTF2(_L("iMaxEmailSize = %d"),
									iImapCompleteDownloadLimits.iMaxEmailSize);
		}	
	else
		{
		// No specified download type mentioned 
		TestCase().ERR_PRINTF1(_L("Incorrect Download type provided as input"));
		TestCase().SetTestStepResult(EFail);
		}
	}


/**
  CompareEmailMessagesL
  For the sent messages, searches for the received message with the same 
  Subject field.  If the received message is found, sets the sent and received
  message information and then calls CompareMessagesL() function to compare the 
  contents of the message based on the download limits set.
  @internalTechnology 
  @param  aSentMsgSelection  Selection of sent email messages
  @param  aRecvMsgSelection  Selection of received email messages
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareEmailMessagesL(
										 const CMsvEntrySelection& aSentMsgSelection,
										 const CMsvEntrySelection& aRecvMsgSelection)
	{
	// Verify the input parameters
	if( (!&aSentMsgSelection) || (!&aRecvMsgSelection))	
		{
		TestCase().ERR_PRINTF1(_L("Invalid Message selection"));
		User::LeaveIfError(KErrArgument);
		}

	// Get the count of messages in both the selection object.
	TInt sentMsgCount = aSentMsgSelection.Count();
	TInt recvMsgCount = aRecvMsgSelection.Count();

	TestCase().INFO_PRINTF2(_L("Sent Message count = %d"),sentMsgCount);
	TestCase().INFO_PRINTF2(_L("Received Message count = %d"),recvMsgCount);

	// Check if all the sent messages have to be received.
	if(recvMsgCount != sentMsgCount)
		{
		// Number of received messages is not equal to number of sent messages
		TestCase().ERR_PRINTF1(_L("Sent and Received message count not equal"));
		TestCase().SetTestStepResult(EFail);
		}

	/* Loop through the list of sent messages.  For each of the sent messages,
	   verify if the message has been received.  The comparison is done based
	   on the subject field.  If message is not received - ERROR
			
	   Obtain the message entry with the session object, setting it to root 
	   entry now, will be changed to the sent and received messages entries during 
	   message comparison 
	*/
	
	if(IsTestCaseStatusPass())
		{
		CMsvEntry* sentMsvEntry = iParamSession->GetEntryL(KMsvRootIndexEntryId);
		CleanupStack::PushL(sentMsvEntry);

		CMsvEntry* recvMsvEntry = iParamSession->GetEntryL(KMsvRootIndexEntryId);
		CleanupStack::PushL(recvMsvEntry);

		if((!sentMsvEntry) || (!recvMsvEntry))
			{
			TestCase().ERR_PRINTF1(_L("Invalid Message Entry"));
			User::LeaveIfError(KErrGeneral);
			}
	
		// Loop through the list of sent messages
		for ( TInt sentMsgIndex = 0 ; (sentMsgIndex < sentMsgCount) &&
									  (IsTestCaseStatusPass());
									   sentMsgIndex ++)
			{
			// Get the sent message Id
			TMsvId sentMsgId = aSentMsgSelection.At(sentMsgIndex);
			// Set the context to the sent message entry
			sentMsvEntry->SetEntryL(sentMsgId);

			TBool found = EFalse;

			TestCase().INFO_PRINTF3(_L("Comparison of Message: %d,  Subject: %S"),
							(sentMsgIndex + 1), &(sentMsvEntry->Entry().iDescription));

			// Loop through the list received messages - Inbox folder
			for(TInt recvMsgIndex= 0; recvMsgIndex < recvMsgCount && !found
														    ; recvMsgIndex++)	
				{
				// Get the received message entry
				TMsvId recvMsgId = aRecvMsgSelection.At(recvMsgIndex);

				// Set the context to the received message entry
				recvMsvEntry->SetEntryL(recvMsgId);
				
				// Compare the subject field of the two messages
				if (sentMsvEntry->Entry().iDescription.
								Compare(recvMsvEntry->Entry().iDescription) == 0)
					{
					TestCase().INFO_PRINTF2(_L("Message with Subject field in the received selection: %S found "),
												  &(recvMsvEntry->Entry().iDescription));

					// Sent message has been received
					found = ETrue;

					// Set the email information with the sent and received emails
					// to be compared
					SetEmailMessageInfoL(*sentMsvEntry, *recvMsvEntry);
					}
				}	
	
			if ( found )
				{
				if(IsTestCaseStatusPass())
					{
					// Message is found now compare the two messages
					if( iDownloadType == EPartialDownload )
						{
						CompareMessagesWithPartialDownloadLimitsSetL();
						}
					else 
						{
						CompareMessagesWithFullDownloadLimitsSetL();
						}

					if(IsTestCaseStatusPass())
						{
						// Delete the email information
						DeleteEmailMessageInfo();
						}
					}
				}
			else{
				TestCase().ERR_PRINTF2(_L("Email with subject %S is not received"), &sentMsvEntry->Entry().iDescription);
				TestCase().SetTestStepResult(EFail);
				}
			}
		// Cleanup the stack
		CleanupStack::PopAndDestroy(recvMsvEntry);
		CleanupStack::PopAndDestroy(sentMsvEntry);
		}
	}




/**
  DeleteEmailMessageInfo
  If any message information was stored for sent and received messages,
  delete the infromation.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::DeleteEmailMessageInfo()
	{
	// Delete sent message information
	delete 	iSentMessageInfo;
	iSentMessageInfo = NULL;
	
	// Delete received message information
	delete 	iRecvMessageInfo ;
	iRecvMessageInfo = NULL;
	}




/**
  SetEmailMessageInfoL
  Stores the message informationsent and received messages
  @internalTechnology 
  @param  aSentMsvEntry Entry for the sent message
  @param  aRecvMsvEntry Entry for the received message
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::SetEmailMessageInfoL(
											CMsvEntry& aSentMsvEntry,
											CMsvEntry& aRecvMsvEntry)
	{
	// Store sent message info
	TestCase().INFO_PRINTF1(_L("Setting information for SENT message"));
	iSentMessageInfo = CMtfTestActionUtilsEmailMessage::NewL(aSentMsvEntry, TestCase());
	
	// Store received message info
	TestCase().INFO_PRINTF1(_L("Setting information for RECEIVED message"));
	
	iRecvMessageInfo = CMtfTestActionUtilsEmailMessage::NewL(aRecvMsvEntry, TestCase());
	}



/**
  CompareMessagesWithPartialDownloadLimitsSetL
  Compares the sent and the received message based on the partial download limits set
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareMessagesWithPartialDownloadLimitsSetL()
	{
	// Partial download limits set
	switch( iImapPartialDownloadLimits.iPartialMailOptions )
		{
		case ENoSizeLimits:
			// No limits, the entire message was to be downloaded
			TestCase().INFO_PRINTF1(_L("PartailDownloadLimits ENoSizeLimits option set"));
			
			CompareCompleteDownloadOfMessageL();
			break;

		case EAttachmentsOnly:
			// Only attachments to be downloaded based on the attachment size limit
			// Body text and html parts should not be downloaded
			TestCase().INFO_PRINTF1(_L("PartailDownloadLimits EAttachmentsOnly option set"));
			
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
				downloaded.
			*/
				TestCase().INFO_PRINTF1(_L("PartailDownloadLimits EBodyTextOnly option set"));
			// NO BREAK
		case EBodyAlternativeText:
			// Check for Body text and html based on the body text size limit
			// Check for the dowmload of text attachments
			if(iImapPartialDownloadLimits.iPartialMailOptions  == 
															   EBodyAlternativeText)
				{
				TestCase().INFO_PRINTF1(_L("PartailDownloadLimits EBodyAlternativeText option set"));
				}
			// NO BREAK
		case EBodyTextAndAttachments:
			// Check for body text and html based on the body text size limit
			// Check for the download of attachment based on the attachment size limit
			if(iImapPartialDownloadLimits.iPartialMailOptions  ==  
															   EBodyTextAndAttachments)
				{
				TestCase().INFO_PRINTF1(_L("PartailDownloadLimits EBodyTextAndAttachments option set"));
				}

			CompareMessagePartialDownloadWithBodyTextAndAttachmentsL();
			break;

		case ECumulative:
			/* The message parts are downloaded with the following priority
				1. Body Text 2. Attachments  3.Html part.  The total message size limit
				is used for the deciding the download of message parts.  The download of 
				the message part is stopped when the cumulative size of the downloded parts
				exceed the total size limit
			*/
			TestCase().INFO_PRINTF1(_L("PartailDownloadLimits ECumulative option set"));

			// Set the cumulative size
			iCumulativeSize = iImapPartialDownloadLimits.iTotalSizeLimit;
			CompareMessageDownloadWithCumulativeOptionSetL();
			break;

		default:
			// TImap4GetMailOptions	iGetMailBodyParts set is not a valid value
			TestCase().ERR_PRINTF1(_L("Invalid TImImap4GetPartialMailInfo.iPartialMailOptions set"));
			TestCase().SetTestStepResult(EFail);
		break;
		}
	}

/**
  CompareMessagesWithFullDownloadLimitsSetL
  Compares the sent and the received message based on the full download limits set
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareMessagesWithFullDownloadLimitsSetL()
	{
	// Full download limits set
	TestCase().INFO_PRINTF1(_L("Full download limits set."));
	
	/* If the size of the message is greater than the toal mail size limit,'
		then the message should not be downloaded
	*/

	// Get the total message size of the sent message
	TInt32 sentMsgSize = 0;
	User::LeaveIfError(iSentMessageInfo->GetMessageSize(sentMsgSize));
	TestCase().INFO_PRINTF2(_L("Sent Message Size = %d"), sentMsgSize);
	
	if(sentMsgSize <= iImapCompleteDownloadLimits.iMaxEmailSize)
		{
		// Sent message size is smaller than the limit, the message should be 
		// downloaded based on the download option set
		switch(iImapCompleteDownloadLimits.iGetMailBodyParts)
			{
			case EGetImap4EmailHeaders:
				// Message headers only, entry should not be created for any message part
				TestCase().INFO_PRINTF1(_L("FullDownloadLimits EGetImap4EmailHeaders option set"));

				CompareMessageWithGetHeadersOptionSetL();
				break;

			case EGetImap4EmailAttachments:
				// Download attachments only.  All attachments should be downloaded
				// Body text and html parts should not be downloaded
				TestCase().INFO_PRINTF1(_L("FullDownloadLimits EGetImap4EmailAttachments option set"));

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
					that the attachments were not downloaded.
				*/
				TestCase().INFO_PRINTF1(_L("FullDownloadLimits EGetImap4EmailBodyText option set"));
				// NO BREAK
			case EGetImap4EmailBodyAlternativeText:
				if(iImapCompleteDownloadLimits.iGetMailBodyParts ==  
											EGetImap4EmailBodyAlternativeText)
					{
					TestCase().INFO_PRINTF1(_L("FullDownloadLimits EGetImap4EmailBodyAlternativeText option set"));
					}
				// NO BREAK
			case EGetImap4EmailBodyTextAndAttachments:
				// The entire message was to be downloaded	
				if(iImapCompleteDownloadLimits.iGetMailBodyParts ==  
											EGetImap4EmailBodyTextAndAttachments)
					{
					TestCase().INFO_PRINTF1(_L("FullDownloadLimits EGetImap4EmailBodyTextAndAttachments option set"));
					}

				CompareCompleteDownloadOfMessageL();
				break;

			default:
				// TImap4GetMailOptions	iGetMailBodyParts set is not a valid value
				TestCase().ERR_PRINTF1(_L("Invalid TImap4GetMailOptions.iGetMailBodyParts set"));
				TestCase().SetTestStepResult(EFail);
			break;
			}
		}
	else
		{
		// The message size was greater than the total mail size limit.
		// Message parts should not be downloaded.
		TestCase().INFO_PRINTF1(_L("Full download limits: Message size greater than mail size limit"));
		CompareMessageFullDownloadWhenMsgSizeGreaterThanLimitL();
		}
	}


/**
  CompareCompleteDownloadOfMessageL
  Check if complete message was downloaded.  The Body text, Html part and all the
  attachmens should have been downloaded, if they are present in the sent message.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareCompleteDownloadOfMessageL()
	{
	// The Body text, Html and Attachments of the message should be downloaded.

	// Compare Body Text contents
	if(IsTestCaseStatusPass())
		{
		ComparePlainBodyTextL(ETrue);
		}
	// Comapre Html contents
	if(IsTestCaseStatusPass())
		{
		CompareHtmlL();
		}
	// Compare Attachments
	if(IsTestCaseStatusPass())
		{
		CompareAttachmentsL();
		}
	}


/**
  CompareMessageWithAttachmentsOnlyOptionSetL
  Checks whether only attachments are downloaded.  If Body Text or Html part is
  downloaded, the test fails.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareMessageWithAttachmentsOnlyOptionSetL()
	{
	/* Only attachments have to be downloaded.  
	   Body Text and Html part should not be downloaded
	*/

	// Body Text
	if (iRecvMessageInfo->GetBodyTextSize() > 0) 
		{
		// Error : body text was downloaded when not expected
		TestCase().ERR_PRINTF1(_L("Body Text was downloaded when Attachment only option is set"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		if(iSentMessageInfo->GetBodyTextSize() > 1) 
			{
			/* [WorkAround for Body Text Ref7]
				When a message with only attachment is created,one byte of body text
				is getting added.  To take care of this conditions, the sent body text
				size is checked to be greater than 1 instead of 0
			*/

			// Check for the correctness of Complete and Partial flags set for Body Text entry
			TestCase().INFO_PRINTF1(_L("Body Text was not downloaded"));
			TestCase().INFO_PRINTF1(_L("Expected flag values for Body Text part: Complete flag = EFalse, Partial download flag = EFalse"));
			CheckFlagSettingsL( iRecvMessageInfo->GetBodyTextId(),EFalse,EFalse);
			}
		else
			{
			if(iRecvMessageInfo->GetBodyTextId() > 0)
				{
				// No Body text part present, compelte flag should be set to ETrue
				// to avoid download of Body text part
				TestCase().INFO_PRINTF1(_L("Body Text was not downloaded"));
				TestCase().INFO_PRINTF1(_L("Expected flag values for Body Text part: Complete flag = ETrue, Partial download flag = EFalse"));
				CheckFlagSettingsL( iRecvMessageInfo->GetBodyTextId(),ETrue,EFalse);
				}
			}
		}
	
	// Html part
	if(IsTestCaseStatusPass())
		{
		if(iRecvMessageInfo->GetHtmlSize() > 0)
			{
			// Error : Html part was downloaded when not expected
			TestCase().ERR_PRINTF1(_L("Html part was downloaded when Attachments Only option is set"));
			TestCase().SetTestStepResult(EFail);
			}
		else if(iSentMessageInfo->GetHtmlSize() > 0)
			{
			TestCase().INFO_PRINTF1(_L("Html part was not downloaded"));
			TestCase().INFO_PRINTF1(_L("Expected flag values for Html part: Complete flag = EFalse, Partial download flag = EFalse"));
			CheckFlagSettingsL( iRecvMessageInfo->GetHtmlEntryId(), EFalse, EFalse);
			}
		else 
			{
			if(iRecvMessageInfo->GetHtmlEntryId() > 0)
				{
				// No Html part present, compelte flag should be set to ETrue
				// to avoid download of Html part
				TestCase().INFO_PRINTF1(_L("Html part was not downloaded"));
				TestCase().INFO_PRINTF1(_L("Expected flag values for Html part: Complete flag = ETrue, Partial download flag = EFalse"));
				CheckFlagSettingsL( iRecvMessageInfo->GetHtmlEntryId(), ETrue, EFalse);
				}
			}
		}
	// Attachments
	if(IsTestCaseStatusPass())
		{
		CompareAttachmentsL();
		}
	}


/**
  CompareMessagePartialDownloadWithBodyTextAndAttachmentsL
  Checks for the download of Body text and Html part based on the Body text size limit
  Calls CompareAttachment() to check the attachment download based on the attachment 
  size limit.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareMessagePartialDownloadWithBodyTextAndAttachmentsL()
	{
	if(IsTestCaseStatusPass())
		{
		// [WorkAround for Body Text Ref1] and [WorkAround for Html Part Ref2] Begin
		
		// Get the size of Body Text part of the sent and the received messages
		TInt32 sentBodyTextSize = iSentMessageInfo->GetBodyTextSize();
		TInt32 sentHtmlSize		= iSentMessageInfo->GetHtmlSize();

		// [WorkAround for Body Text Ref1] and [WorkAround for Html Part Ref2] End
		
		// If both Body text and html part fit into the Body text limit, it is a complete
		// download of Body text. Html should also be downloaded
		if((sentBodyTextSize + sentHtmlSize) <= iImapPartialDownloadLimits.iBodyTextSizeLimit)
			{
			TestCase().INFO_PRINTF1(_L("Both Body text and html part fit into the Body text limit"));
			
			// Compare Body Text contents
			ComparePlainBodyTextL(ETrue);

			if(IsTestCaseStatusPass())
				{
				// Compare Html contents
				CompareHtmlL();
				}
			}
		else
			{
			// If both Body text and Html does not fit in, then it is a partial download of 
			// Body text, Html part should not be downloaded
			TestCase().INFO_PRINTF1(_L("Partial Download of Body text"));

			ComparePlainBodyTextL(EFalse);

			if(IsTestCaseStatusPass())
				{
				if(iRecvMessageInfo->GetHtmlSize() > 0)
					{
					// Error : Html was downloaded when not expected
					TestCase().ERR_PRINTF1(_L("Html part was downloaded when Body Text+ Html size > Body text limit"));
					TestCase().SetTestStepResult(EFail);
					}

				if(IsTestCaseStatusPass())
					{
					if(iSentMessageInfo->GetHtmlSize() > 0)
						{
						// When Html is not downloaded, check the correctness of the 
						// complete and the partial download flags set for the Html entry
						TestCase().INFO_PRINTF1(_L("Html part was not downloaded"));

						TestCase().INFO_PRINTF1(_L("Expected flag values for Html part:  Complete flag = EFalse, Partial download flag = EFalse"));
						CheckFlagSettingsL( iRecvMessageInfo->GetHtmlEntryId(), EFalse, EFalse);
						}
					}
				}
			}
		}
	
	if(IsTestCaseStatusPass())
		{
		// Check if the attachments are downloaded if the size of the attachment
		// is less than or equal to the attachment size limit
		CompareAttachmentsL();
		}
	}


/**
  CompareMessageWithGetHeadersOptionSetL
  Checks if any message part was downloaded when Full download limits
  with EGetImap4EmailHeaders option is set.  If any message part is downloaded,
  the test fails.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareMessageWithGetHeadersOptionSetL()
	{
	/* Download headers only.  The Body text, Html or Attachments of the message
	  should not be downloaded.  The entries for the message parts should not be created.
	*/
	if(IsTestCaseStatusPass())
		{
		TMsvId recvBodyTextId = iRecvMessageInfo->GetBodyTextId();

		if(recvBodyTextId > 0)
			{
			// Error : Entry created for Body Text
			TestCase().ERR_PRINTF1(_L("Entry for Html part is created when Headers Only option is set"));
			TestCase().SetTestStepResult(EFail);
			}
		else
			{
			TestCase().INFO_PRINTF1(_L("Body Text was not downloaded"));
			}

		if(IsTestCaseStatusPass())
			{
			TMsvId recvHtmlId = iRecvMessageInfo->GetHtmlEntryId();
			if( recvHtmlId > 0 )
				{
				// Error : Entry created for Html part
				TestCase().ERR_PRINTF1(_L("Entry for Html part is created when Headers Only option is set"));
				TestCase().SetTestStepResult(EFail);
				}
			else
				{
				TestCase().INFO_PRINTF1(_L("Html part was not downloaded"));
				}
			}
		if(IsTestCaseStatusPass())
			{
			// Compare attachments
			CompareAttachmentsL();
			}
		}
	}	




/**
  CompareMessageFullDownloadWhenMsgSizeGreaterThanLimitL
  The message size is greater than the message size limit. No message parts should be
  downloaded.  Fails if any message part was downloaded.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareMessageFullDownloadWhenMsgSizeGreaterThanLimitL()
	{
	// Body Text
	if(iRecvMessageInfo->GetBodyTextSize() > 0) 
		{
		TestCase().ERR_PRINTF1(_L("Body text downloaded when mail size is greater than the iMaxEmailSize"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("Body text not was downloaded"));
		}	

	// Html
	if(IsTestCaseStatusPass())
		{
		if(iRecvMessageInfo->GetHtmlSize() > 0) 
			{
			TestCase().ERR_PRINTF1(_L("Html part downloaded when mail size is greater than the iMaxEmailSize"));
			TestCase().SetTestStepResult(EFail);
			}
		else
			{
			TestCase().INFO_PRINTF1(_L("Html part not was downloaded"));
			}
		}

	// Attachments
	if(IsTestCaseStatusPass())
		{
		CompareAttachmentsL();
		}
	}




/**
  CompareMessageDownloadWithCumulativeOptionSetL
  Checks for the download of message parts when Partial download ECumulative options is set
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareMessageDownloadWithCumulativeOptionSetL()
	{

	/*	When Partial download with Cumulative option is set, 
		
		The message body text part will be downloaded first. If the body text
		size is greater than the total size limit, the body text will be 
		downloaded partially and none of the other parts are downloaded.
		If the body text size is lesser than the total size limit,
		the attachment that fits into the remaining size will be downloaded.

		After download of attachments, if Html part can fit in, the Html part 
		should be downloaded
	*/

	// Get the size of the sent message
	TInt32 sentMsgSize = 0;
	User::LeaveIfError(iSentMessageInfo->GetMessageSize(sentMsgSize));
	
	TestCase().INFO_PRINTF2(_L("Sent message size = %d"), sentMsgSize);

	// If the message is lesser than the limit, the complete message has to be downloaded
	if(sentMsgSize  <= iImapPartialDownloadLimits.iTotalSizeLimit )
		{
		CompareCompleteDownloadOfMessageL();
		}
	else
		{
		// Set the cumulative size
		iCumulativeSize = iCumulativeSize - iSentMessageInfo->GetBodyTextSize();

		/* Compare the attachments first. This will help in knowing the remaining cumulative
		   size after attachment download.  To decide whether the Body text was to be completely
		   downloaded or not, we need to know if Html part fits in the cumulative size.
		   Based on the remaining cumulative size after attachment comparison, decide whether
		   Body text was to be completely downloaded or pariatlly downlaoded
		*/

		if(IsTestCaseStatusPass())
			{
			CompareAttachmentsL();
			}

		if(IsTestCaseStatusPass())
			{
			// Check if Html part fits in
			if( (iSentMessageInfo->GetHtmlSize()) <= iCumulativeSize )
				{
				// Compare the complete download of Body Text
				ComparePlainBodyTextL(ETrue);

				if(IsTestCaseStatusPass())
					{
					// Compare Htmp contents
					CompareHtmlL();
					}
				}
			else
				{
				// Partial download of body text
				TestCase().INFO_PRINTF1(_L("Partial download of Body Text"));
				ComparePlainBodyTextL(EFalse);

				if(IsTestCaseStatusPass())
					{
					// Check if Html part was received. If received, fail the Test Case
					if(iRecvMessageInfo->GetHtmlSize() > 0)
						{
						// Error : Html was downloaded when not expected
						TestCase().ERR_PRINTF1(_L("Html part was downloaded when cumulative size is lesser than Html size"));
						TestCase().SetTestStepResult(EFail);
						}
					else
						{
						TestCase().INFO_PRINTF1(_L("Html part was not downloaded"));
						}

					if(IsTestCaseStatusPass())
						{
						// If Html part is not received, verify the complete and partial download flag
						// set for the Html part
						if(iSentMessageInfo->GetHtmlSize() > 0)
							{
							TestCase().INFO_PRINTF1(_L("Expected flag values for Html part:  Complete flag = EFalse, Partial download flag = EFalse"));
							CheckFlagSettingsL( iRecvMessageInfo->GetHtmlEntryId(), EFalse, EFalse);
							}
						}
					}
				}
			}
		}
	}

/**
  ComparePlainBodyTextL
  Compares the contents of the received body text with that of the sent message.
  Checks if the footer message is present and verifies the complete and partial 
  download flags set for the Body text entry.
  @internalTechnology 
  @param  TBool ETrue for complete download of Body text, EFalse when partail download
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::ComparePlainBodyTextL(TBool aCompleteDownload)
	{
	//[WorkAround for Body Text Ref2]
	const TInt KPartialBodyTextCompareTolerance = 5;

	TestCase().INFO_PRINTF1(_L("Comparing Plain text part of the message"));
		
	// Comapre the contents when sent message had Body Text
	if( iSentMessageInfo->GetBodyTextSize() > 0)
		{
		if( iRecvMessageInfo->GetBodyTextSize() > 0)
			{
			// Get the Body Text contents of sent and received messages

			// Get message body text for sent message
			CParaFormatLayer*	sentParaFormatLayer = CParaFormatLayer::NewL();
			CleanupStack::PushL(sentParaFormatLayer);

			CCharFormatLayer*	sentCharFormatLayer = CCharFormatLayer::NewL(); 
			CleanupStack::PushL(sentCharFormatLayer);

			CRichText*	sentPlainBodyText  = CRichText::NewL( sentParaFormatLayer,
											  sentCharFormatLayer,   
											  CEditableText::EFlatStorage, 256);
			CleanupStack::PushL(sentPlainBodyText);
			
			iSentMessageInfo->GetBodyTextL(*sentPlainBodyText, *sentParaFormatLayer,
															*sentCharFormatLayer);	

			// Get message body text for received message
			CParaFormatLayer*	recvParaFormatLayer = CParaFormatLayer::NewL();
			CleanupStack::PushL(recvParaFormatLayer);

			CCharFormatLayer*	recvCharFormatLayer = CCharFormatLayer::NewL(); 
			CleanupStack::PushL(recvCharFormatLayer);

			CRichText*	recvPlainBodyText = CRichText::NewL( recvParaFormatLayer, 
															 recvCharFormatLayer, 
										     CEditableText::EFlatStorage, 256);

			CleanupStack::PushL(recvPlainBodyText);

			//[WorkAround Ref1 Start]
			iRecvMessageInfo->GetBodyTextL(*recvPlainBodyText, *recvParaFormatLayer,
															*recvCharFormatLayer);
			//[WorkAround Ref1 End]
			
			// If the body text buffer is not available, fail the test case
			if(sentPlainBodyText == NULL || recvPlainBodyText == NULL)
				{
				// Error : Body text buffer not available
				TestCase().ERR_PRINTF1(_L("Body text buffer not available"));
				TestCase().SetTestStepResult(EFail);
				}			

			if(IsTestCaseStatusPass())
				{
				TPtrC sentBodyText = sentPlainBodyText->Read(0);
				TPtrC recvBodyText = recvPlainBodyText->Read(0);

				TestCase().INFO_PRINTF3(_L("Sent body text size: %d, Received body text size = %d"),
											sentBodyText.Length(), recvBodyText.Length());
						
				TInt serverMsgPos = -1;
				TInt serverMsgLength = 0;

				// Check if server has added any message to the beginning of the
				// Body Text Part

				// [WorkAround for Body Text Ref3] Begin
				CheckIfServerMessageExists(recvBodyText, serverMsgLength, serverMsgPos);
				
				// Get the Body text size of the received messagge
				TInt totalBodyTextSize = recvBodyText.Length();

				// If server message is present, set the buffer to ignore the server message
				if ( serverMsgPos >= 0 )
					{
					totalBodyTextSize = totalBodyTextSize - serverMsgLength;
					}
				TPtrC recvBodyTextPtr = recvBodyText.Right(totalBodyTextSize);
				
				// [WorkAround for Body Text Ref3] End

				// Check if Footer message is present
				TInt footerMsgPos = CheckIfFooterMessageExists(recvBodyText);
				
				if( aCompleteDownload)
					{
					// Complete download of body text expected.  Both Body Text and Html part
					// downloaded.  The Body text should not contian the footer message

					if (footerMsgPos != KErrNotFound)
						{
						// Error : Footer message exists when complete download of body text expected
						TestCase().ERR_PRINTF1(_L("Footer message exists when complete download of body text expected"));
						TestCase().SetTestStepResult(EFail);
						}

					if(IsTestCaseStatusPass())
						{
						// [WorkAround for Body Text Ref2] Begin
						TPtrC recvBodyText1 = recvBodyTextPtr.Left(recvBodyTextPtr.Length()
														- KPartialBodyTextCompareTolerance);
						// [WorkAround for Body Text Ref2] End

						// [WorkAround for Body Text Ref3] Begin
						TPtrC sentBodyText1 = sentBodyText.Left(recvBodyText1.Length());
						// [WorkAround for Body Text Ref3] End

						// Compare the sent and received message's body text contents
						if( (recvBodyText1.Compare(sentBodyText1)) != 0)
							{
							// Error : Comparison of body text failed
							TestCase().ERR_PRINTF1(_L("Comparison of body text failed"));
							TestCase().SetTestStepResult(EFail);
							}

						if(IsTestCaseStatusPass())
							{
							TestCase().INFO_PRINTF1(_L("Comparison of Body text was successful"));
							// Verify the complete and partial download flags set
							TestCase().INFO_PRINTF1(_L("Expected flag values for Body Text part:  Complete flag = ETrue, Partial download flag = EFalse"));
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
					   present in the received message's body text.
					*/
					if(IsTestCaseStatusPass())
						{
						if (footerMsgPos == KErrNotFound)
							{
							// Error : Footer message expected, but does not exists
							TestCase().ERR_PRINTF1(_L("Footer message expected, but does not exists"));
							TestCase().SetTestStepResult(EFail);
							}
	
						if(IsTestCaseStatusPass())
							{
							TPtrC recvBodyText1 = recvBodyTextPtr.Left(
												  recvBodyTextPtr.Length() - (footerMsgPos));

							// [WorkAround for Body Text Ref5] Begin
							TPtrC sentBodyText1 = sentBodyText.Left(recvBodyText1.Length());
							// [WorkAround for Body Text Ref5] End
							
							if( (recvBodyText1.Compare(sentBodyText1)) != 0)
								{
								// Error : Comparison of body text failed
								TestCase().ERR_PRINTF1(_L("Comparison of body text failed"));
								TestCase().SetTestStepResult(EFail);
								}

							if(IsTestCaseStatusPass())
								{
								TestCase().INFO_PRINTF1(_L("Comparison of Body text was successful"));

								// Verify the complete and the partial download flags set
								TestCase().INFO_PRINTF1(_L("Expected flag values for Body Text part: Complete flag = ETrue, Partial download flag = ETrue"));

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
			// Error : Sent message contains Body text. But Body text was not received
			TestCase().ERR_PRINTF1(_L("Sent message contains Body text. But Body text was not received"));
			TestCase().SetTestStepResult(EFail);
			}
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("Sent message does not contain Body text part"));
		if(iRecvMessageInfo->GetBodyTextSize() > 0)
			{
			// Error : Sent message does not contains Body text. But Body text was received
			TestCase().ERR_PRINTF1(_L("Sent message does not contains Body text. But Body text was received"));
			TestCase().SetTestStepResult(EFail);
			}
		}
	}



/**
  CheckIfFooterMessageExists
  Checks for the footer message in the specified buffer.  If footer message
  is found, returns the position of the footer message in the buffer.
  If not found, returns KErrNotFound
  @internalTechnology 
  @param  aBodyText Pointer descriptor of the  buffer
  @return TInt Position of the start of footer message in the buffer, KErrNotFound
				if the footer message is not found
  @pre    None
  @post   None
*/
TInt CMtfTestActionCompareEmailMessages::CheckIfFooterMessageExists(TPtrC aBodyText)
	{
		// Look for the footer message in the received email message. 
	_LIT(KFooterMsg1,"This message has not been fully downloaded. There is still ");
	
	//Find for the footer message in the received message
	TInt found = aBodyText.Find(KFooterMsg1);

	if(found != KErrNotFound)
		{
		// [WorkAround for Body Text Ref6] Begin
		TPtrC footerMsg = aBodyText.Right(aBodyText.Length() - found);
		TestCase().INFO_PRINTF2(_L("Footer message: %S"), &footerMsg);
		// [WorkAround for Body Text Ref6] End
		}
	return found;
	}


/**
  CheckIfServerMessageExists
  Checks if any server message has been added to the Body text,
  returns the position of the server message in the buffer and the 
  length of the server message.  This function can be enhanced 
  to check for any other server message encountered
  If not found, returns KErrNotFound
  @internalTechnology 
  @param  aBodyText			- Pointer descriptor of the  buffer
  @param  aServerMsgLength	- Length of the server message
  @param  aServerMsgPosition - Position of the server message in the buffer
  @pre    None		
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CheckIfServerMessageExists(
										TPtrC aBodyText, TInt& aServerMsgLength,
										TInt& aServerMsgPosition)
	{
	_LIT(KServerMessage,"*This message was transferred with a trial version of CommuniGate(r) Pro*");
	
	aServerMsgLength = KServerMessage().Length()+1;

	aServerMsgPosition = aBodyText.Find(KServerMessage);
	}



/**
  CheckFlagSettingsL
  Checks if the complete and the partial download flags for the specified message part
  is same as the expected value. Fails the test case if the flags set for the message
  part does not match with the expected value
  @internalTechnology 
  @param  aMsgPartId  TMsvId of the message part
  @param  aExpectedCompleteFlagVal  Expected value of the Complete flag
  @param  aExpectedPartialFlagVal   Expected value of the Parital download flag
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CheckFlagSettingsL(TMsvId aMsgPartId, 
														   TBool aExpectedCompleteFlagVal,
														   TBool aExpectedPartialFlagVal)
	{
	// [WorkAround for general Ref1] Begin
	if( aMsgPartId > 0)
		{
		// Check the complete and partial flag settings
		TBool completeFlag	= EFalse;
		User::LeaveIfError(iRecvMessageInfo->CheckCompleteFlagL( aMsgPartId,completeFlag));
		
		// Compare the set and the expected complete flag value
		if(aExpectedCompleteFlagVal && completeFlag)
			{
			TestCase().INFO_PRINTF1(_L("Complete Flag is set to ETrue"));
			}
		else if((aExpectedCompleteFlagVal == 0) && (completeFlag == 0))
			{
			TestCase().INFO_PRINTF1(_L("Complete Flag is set to EFalse"));
			}
		else
			{
			TestCase().ERR_PRINTF1(_L("Error: Incorrect Compelte flag setting"));
			TestCase().INFO_PRINTF2(_L("Complete Flag is set to %d "), completeFlag);
			TestCase().SetTestStepResult(EFail);
			}

		if(IsTestCaseStatusPass())
			{
			TBool partialDownloadFlag = EFalse;
			User::LeaveIfError(	iRecvMessageInfo->CheckPartialDownloadFlagL
												   (aMsgPartId, partialDownloadFlag)); 
			
			// Compare the set and the expected partial download flag value
			if(aExpectedPartialFlagVal && partialDownloadFlag)
				{
				TestCase().INFO_PRINTF1(_L("Partial download flag is set to ETrue"));
				}
			else if((aExpectedPartialFlagVal == 0)  && (partialDownloadFlag == 0))
				{
				TestCase().INFO_PRINTF1(_L("Partial download flag is set to EFalse"));
				}
			else
				{	
				TestCase().ERR_PRINTF1(_L("Error: Incorrect Partial download flag setting"));
				TestCase().INFO_PRINTF2(_L("Partial download is set to %d "), partialDownloadFlag);
				TestCase().SetTestStepResult(EFail);
				}
			}
		} 
	else
		{
		TestCase().ERR_PRINTF1(_L("Invalid TMsvId provided for checking complete and partial download flags"));
		TestCase().SetTestStepResult(EFail);
		}
	// [WorkAround for general Ref1] End
	}


/**
  CheckFlagSettingsL
  Checks if the complete download flag for the specified message attachment is same as
  the expected value. Fails the test case if the flags set for the message part does not
  match with the expected value. 
  @internalTechnology 
  @param  aMsgPart  CMsvAttachment of the message part
  @param  aExpectedCompleteFlagVal  Expected value of the Complete flag
  @pre    None
  @post   None
*/


void CMtfTestActionCompareEmailMessages::CheckFlagSettingsL(CMsvAttachment& aMsgPart, 
														   TBool aExpectedCompleteFlagVal)
	{
	// Check the complete flag settings
	TBool completeFlag	= aMsgPart.Complete();
	
	// Compare the set and the expected complete flag value
	if(aExpectedCompleteFlagVal && completeFlag)
		{
		TestCase().INFO_PRINTF1(_L("Complete Flag is set to ETrue"));
		}
	else if((aExpectedCompleteFlagVal == 0) && (completeFlag == 0))
		{
		TestCase().INFO_PRINTF1(_L("Complete Flag is set to EFalse"));
		}
	else
		{
		TestCase().ERR_PRINTF1(_L("Error: Incorrect Complete flag setting"));
		TestCase().INFO_PRINTF2(_L("Complete Flag is set to %d "), completeFlag);
		TestCase().SetTestStepResult(EFail);
		}
	}


/**
  CompareHtmlL
  Compares the contents of the sent and received html parts. Verifies the complete
  and the partial download flags set for the received message's html part.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareHtmlL()
	{
	// [WorkAround for Html Part Ref1] Begin

	// Get the Html file names for the sent and the received messages
	TPtrC sentHtmlFileName;
	TInt err1 = iSentMessageInfo->HtmlFileName(sentHtmlFileName);
	TPtrC recvHtmlFileName;
	TInt err2 = iRecvMessageInfo->HtmlFileName(recvHtmlFileName);
	
	// [WorkAround for Html Part Ref1] End

	// If the sent message has Html part, compare the received message's html part
	if((iSentMessageInfo->GetHtmlSize() > 0) && (err1 == KErrNone))
		{
		if( err2 == KErrNone)
			{
			TInt errorVal;
			TBool diffFlag = EFalse;

			// Compare the html files
			CompareFilesL(sentHtmlFileName, recvHtmlFileName, errorVal, diffFlag);
			
			if (errorVal == KErrNone )
				{
				if ( diffFlag == EFalse )
					{
					TestCase().INFO_PRINTF1(_L("Comparison of Html files was successful"));
					TestCase().INFO_PRINTF2(_L("Sent Html file : %S"),&sentHtmlFileName );
					TestCase().INFO_PRINTF2(_L("Received Html file : %S"),&recvHtmlFileName );
					
					// Verify the complete and the partial download flags set
					TestCase().INFO_PRINTF1(_L("Expected flag values for Html part:  Complete flag = ETrue, Partial download flag = EFalse"));
					CheckFlagSettingsL( iRecvMessageInfo->GetBodyTextId(), ETrue, EFalse);
					}
				else
					{
					// Comparison of Html files failed
					TestCase().ERR_PRINTF1(_L("Comparison of Html files failed"));
					TestCase().SetTestStepResult(EFail);
					}
				}
			else
				{
				// An error occured while comparing the html files
				TestCase().ERR_PRINTF1(_L("Error occurred while comparing Html files"));
				TestCase().SetTestStepResult(EFail);
				}
			}
		else
			{
			// Received message's html path was not found
			TestCase().ERR_PRINTF1(_L("Html file path not found in the received message"));
			TestCase().SetTestStepResult(EFail);
			}
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("Html file path not found for the sent message"));
		}
	}

			
/**
  CompareFilesL
  Compare the contents of two files where the complete path of the two file are provided
  @internalTechnology
  @param  aSentFilePath		- Pointer buffer contianing file path of the sent message 
  @param  aRecvFilePath		- Pointer buffer contianing file path of the received message 
  @param  aError			- Any error value set during comparison of files
  @param  aDiffFlag			- Set to ETrue if the files differ, else set ot EFalse
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareFilesL(const TPtrC aSentFilePath,
													   const TPtrC aRecvFilePath,
													   TInt& aError, TBool& aDiffFlag)
	{
	aDiffFlag = EFalse;
	CArrayFixFlat<TUint16> *ignoreCharList = new (ELeave)
											CArrayFixFlat<TUint16>(KArrayGranularity);
	CleanupStack::PushL(ignoreCharList);

	// Compare the content of the attachment files
	aError = TestFrameworkActionsUtils::CompareFilesL(aSentFilePath, aRecvFilePath,
														 EFalse, ignoreCharList, aDiffFlag);
						
	CleanupStack::PopAndDestroy(ignoreCharList);
	}


/**
  CompareAttachmentsL
  Checks if the attachments download for all options of download limits set.
  Verifies the Complete and Partial download flag settings for the attachments
  in the received message based on the download option and download limits set.
  @internalTechnology
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmailMessages::CompareAttachmentsL()
	{
	TestCase().INFO_PRINTF1(_L("Comparing Attachments"));

	if(IsTestCaseStatusPass())
		{
		// Get attachment list for Sent and Received message
		CImEmailMessage* sentEmailMessage = iSentMessageInfo->GetEmailMessageL();
		
		CImEmailMessage* recvEmailMessage = iRecvMessageInfo->GetEmailMessageL();
				
		// Call the asynchronous function to get the attachment list
		User::LeaveIfError(iSentMessageInfo->GetMessageAttachmentListL());
		
		//Get the list of attachments present in sent message
		MMsvAttachmentManager& sentManager = sentEmailMessage->AttachmentManager();
		// Get attachment list for Received message
		
		
		// Call the asynchronous function to get the attachment list
		User::LeaveIfError(iRecvMessageInfo->GetMessageAttachmentListL());

		//Get the list of attachments present in received message
		MMsvAttachmentManager& recvManager = recvEmailMessage->AttachmentManager();
		
		TInt sentMsgAttachmentCount = sentManager.AttachmentCount();
		TInt recvMsgAttachmentCount = recvManager.AttachmentCount();

		TestCase().INFO_PRINTF3(_L("Sent Attachments: %d, Received Attachments: %d"),
		sentMsgAttachmentCount,recvMsgAttachmentCount); 

		// Check for the attachment count
		CheckAttachmentCountL(sentMsgAttachmentCount, recvMsgAttachmentCount);
		if(IsTestCaseStatusPass() && recvMsgAttachmentCount)
			{
			TBool toBeDownloaded ;
			TBool downloaded	 ;
			TBool attachmentFound;

			// Loop through Sent attachment list
			for ( TInt index1 = 0, index2 = 0 ; ( IsTestCaseStatusPass()) && 
						(index1 < sentMsgAttachmentCount) ; index1++)
				{

				// Get the name of the attachment
				CMsvAttachment* sentAttInfo1 = sentManager.GetAttachmentInfoL(index1);
				CleanupStack::PushL(sentAttInfo1);
				TFileName fileName1 = sentAttInfo1->AttachmentName();
				
				toBeDownloaded	= EFalse;	
				downloaded		= EFalse;
				attachmentFound = EFalse;

				// Based on the download type set, check if the attachment 
				// was supposed to be downloaded or not
				if(iDownloadType == EPartialDownload)
					{
					// Partial download limits set
					toBeDownloaded = CheckIfAttachmentTobeDownloadedWithPartialDownloadSet
													(sentAttInfo1->AttachmentName(), sentAttInfo1->Size());
													
					}
				else
					{
					// Full download limits set
					toBeDownloaded = CheckIfAttachmentTobeDownloadedWithCompleteDownloadSet(sentAttInfo1->AttachmentName());	
					}

				// Loop through the received messages to search for the attachment
				// with the same name
				for ( index2 = 0; index2 < recvMsgAttachmentCount; index2++ )
					{
					// Get the attachment name
					CMsvAttachment* recvAttInfo2 = recvManager.GetAttachmentInfoL(index2);
					CleanupStack::PushL(recvAttInfo2);
					TFileName fileName2 = recvAttInfo2->AttachmentName();

					// Compare the name of the attachment in the sent message with the
					// received message's attachment name
					if ( fileName2.Compare(fileName1) == 0 )
						{
						// Comparison was success, the attachment found in the
						// received message
						attachmentFound = ETrue;
						
						// Check if the attachment was downloaded
						if ( recvAttInfo2->Complete() )
							{
							// Complete flag set to True ==> attachment is downloaded
							downloaded	 = ETrue;
							TestCase().INFO_PRINTF2(_L("Attachment %S is downloaded"),
																		   &fileName2);
							}
						CleanupStack::PopAndDestroy(recvAttInfo2);
						break;
						}						
					CleanupStack::PopAndDestroy(recvAttInfo2);	
					}
				if(attachmentFound == EFalse)
					{
					// Expected attachment is not downloaded
					TestCase().ERR_PRINTF2(_L("Attachment %S was not received"), &fileName1);
					// Test step result is set to EFail
					TestCase().SetTestStepResult(EFail);
					}
				else
					{
					CMsvAttachment* recvAttInfo2 = recvManager.GetAttachmentInfoL(index2);
					CleanupStack::PushL(recvAttInfo2);
					if ( toBeDownloaded && downloaded )
						{
						//compare sent and received attachment contents
						TInt error = 0;
						TBool diffFlag = EFalse;

						CompareFilesL(sentAttInfo1->FilePath(),
									  recvAttInfo2->FilePath(), error, diffFlag);
						
						if ((error != KErrNone) || (diffFlag) )
							{
							// Comparison of sent and received attachment fails
							TestCase().ERR_PRINTF2(_L("Comparison of %S attachment failed"), 
														&fileName1);
							// Test step result is set to EFail
							TestCase().SetTestStepResult(EFail);
							}

						if(IsTestCaseStatusPass())
							{
							// The attachments contents are same
							TestCase().INFO_PRINTF2(_L("Comparison of attachment %S was success"),
														&fileName1);
							}

						// Verify the complete and the partial download flags set
						TestCase().INFO_PRINTF1(_L("Expected flag values for Attachment:  Complete flag = ETrue, Partial download flag = EFalse"));
						CheckFlagSettingsL(*recvAttInfo2 , ETrue);
						}
					else if(toBeDownloaded  && (downloaded == EFalse))
						{
						// Expected attachment is not downloaded
						TestCase().ERR_PRINTF2(_L("Attachment %S was not downloaded"), 
																		&fileName1);
						// Test step result is set to EFail
						TestCase().SetTestStepResult(EFail);

						}
					else if( (toBeDownloaded == EFalse)  && (downloaded)) 
						{
						
						// Unexpected attachment has been downloaded
						TestCase().ERR_PRINTF2(_L("Attachment %S was received, but not expected"), 
																					&fileName1);
						// Test step result is set to EFail
						TestCase().SetTestStepResult(EFail);
						}	
					else if((toBeDownloaded == EFalse)  && (downloaded == EFalse)) 
						{
						// Verify the flags
						TestCase().INFO_PRINTF2(_L("The attachment %S was not downloaded"),&fileName1);	
						TestCase().INFO_PRINTF1(_L("Expected flag values for Attachment:  Complete flag = EFalse, Partial download flag = EFalse"));
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
  CheckAttachmentCountL
  Checks if the number of attachments in the received message is correct. 
  Otherwise, fails the Test Case.
  @internalTechnology
  @param  aSentMsgAttachmentCount - Number of attachments in the sent message 
  @param  aRecvMsgAttachmentCount - Number of attachments in the sent message
  @pre    None
  @post   None
*/

	
void CMtfTestActionCompareEmailMessages::CheckAttachmentCountL(TInt aSentMsgAttachmentCount, TInt aRecvMsgAttachmentCount)
	{
	TInt32 sentMsgSize;
	
	User::LeaveIfError( iSentMessageInfo->GetMessageSize(sentMsgSize) );	
	if( iDownloadType == EFullDownload )
		{	
		// if attachment download allowed
		if( iImapCompleteDownloadLimits.iGetMailBodyParts != EGetImap4EmailHeaders && 
			iImapCompleteDownloadLimits.iGetMailBodyParts != EGetImap4EmailBodyText ) 
			{
			if( sentMsgSize <= iImapCompleteDownloadLimits.iMaxEmailSize && aSentMsgAttachmentCount != aRecvMsgAttachmentCount )
				{
				TestCase().ERR_PRINTF1(_L("Full Download Number of attachments are not equal"));
				TestCase().SetTestStepResult(EFail);
				}			
			}
		else if( aRecvMsgAttachmentCount > 0 )  // no attachment downloads allowed -should not recieve any attachments
			{
			TestCase().ERR_PRINTF1(_L("Download with EGetImap4EmailHeaders or EGetImap4EmailBodyText Attachment count is not equal to 0"));
			TestCase().SetTestStepResult(EFail);			
			}
		
		
		/*
		// if only downloading headers or body text just check we havent recieved any attachments				
	    if( ( iImapCompleteDownloadLimits.iGetMailBodyParts == EGetImap4EmailHeaders || 
	    	iImapCompleteDownloadLimits.iGetMailBodyParts == EGetImap4EmailBodyText )  &&
			aRecvMsgAttachmentCount > 0 )
			{		
			TestCase().ERR_PRINTF1(_L("Download with EGetImap4EmailHeaders, Attachment count is not equal to 0"));
			TestCase().SetTestStepResult(EFail);
			}
		// check that the total size limit should allow all attachments to be 
		// downloaded and that the attachment counts are equal
		else if( sentMsgSize <= iImapCompleteDownloadLimits.iMaxEmailSize && aSentMsgAttachmentCount != aRecvMsgAttachmentCount )
			{
			TestCase().ERR_PRINTF1(_L("Full Download Number of attachments are not equal"));
			TestCase().SetTestStepResult(EFail);
			}			
			*/
		}
	else if( iDownloadType == EPartialDownload )
		{
		// if attatchment download allowed
		if( iImapPartialDownloadLimits.iPartialMailOptions != EBodyTextOnly )
			{			
			// if message size is below the max then attchments downloaded should match attachments sent
			if( sentMsgSize <= iImapPartialDownloadLimits.iTotalSizeLimit && 
			    aSentMsgAttachmentCount != aRecvMsgAttachmentCount )
				{
				TestCase().ERR_PRINTF1(_L("Partial Download Number of attachments are not equal"));
				TestCase().SetTestStepResult(EFail);
				}
			}
		else if( aRecvMsgAttachmentCount > 0 )  // body text only - should not recieve any attachments
			{
			TestCase().ERR_PRINTF1(_L("Download with EBodyTextOnly, Attachments will not be downloaded"));
			}		
		}
	else
		{
		TestCase().ERR_PRINTF1(_L("Unknown value for iDownloadType"));
		TestCase().SetTestStepResult(EFail);
		}					
	}


/**
  CheckIfAttachmentTobeDownloadedWithCompleteDownloadSet
  Checks if the attachment download when Full download options is set.
  @internalTechnology
  @return TBool  ETrue if attachment is to be downloaded, EFalse otherwise
  @pre    None
  @post   None
*/
TBool CMtfTestActionCompareEmailMessages::
							CheckIfAttachmentTobeDownloadedWithCompleteDownloadSet(const TDesC& aAttachmentName /*const TDesC8& aMimeType*/)
	{
	TBool toBeCompletelyDownloadedFlag = EFalse;

	if(iDownloadType == EFullDownload)
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
				// Check is done against filename because Mime_type field isn't filled out
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
  CheckIfAttachmentTobeDownloadedWithPartialDownloadSet
  Checks if the attachment download when Partial download options is set.
  @internalTechnology
  @param  aAttachmentSize  Size of the attachment
  @return TBool  ETrue if attachment is to be downloaded, EFalse otherwise
  @pre    None
  @post   None
*/
TBool CMtfTestActionCompareEmailMessages::
			CheckIfAttachmentTobeDownloadedWithPartialDownloadSet
										  (const TDesC& aAttachmentName, TInt32 aAttachmentSize)
	{
	// [WorkAround for Attachments Ref1] Begin
	TInt32 recvAttachmentSize = (TInt32)(aAttachmentSize * ATTACHMENT_SIZE_CORRECTION);
	// [WorkAround for Attachments Ref1] End

	TBool toBePartialyDownloadedFlag = EFalse;

	if(iDownloadType == EPartialDownload)
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
				// Check is done against filename because Mime_type field isn't filled out
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
