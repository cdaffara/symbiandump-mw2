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

#ifndef __CMTF_TEST_ACTION_COMPARE_EMAIL_MESSAGES_H__
#define __CMTF_TEST_ACTION_COMPARE_EMAIL_MESSAGES_H__

// System include
#include <msvapi.h>


// User include
#include "CMtfSynchronousTestAction.h"
#include "CMtfTestActionUtilsEmailMessage.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionCompareEmailMessages,"CompareEmailMessages");
/*@}*/


// class forward declaration
class CMtfTestParameter;


/**
  Implements the test action to compare the sent and received email messages
  based on the partial/full download limits set.
  @internalTechnology
*/
class CMtfTestActionCompareEmailMessages : public CMtfSynchronousTestAction
	{
public:

	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters
							   );
	// Destructor
	virtual ~CMtfTestActionCompareEmailMessages(); 

	virtual void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionCompareEmailMessages( CMtfTestCase& aTestCase );

	// Store the partial or full download limits received as input
	void SetDownloadLimits();

	// Delete the sent and received message information stored
	void DeleteEmailMessageInfo();

	// Set the sent and received message information
	void SetEmailMessageInfoL( CMsvEntry& sentMsvEntry, CMsvEntry& recvMsvEntry );
	
	// Main function for comparing the sent and received messages
	void CompareEmailMessagesL( const CMsvEntrySelection& aSentMsgSelection,
							  	const CMsvEntrySelection& aRecvMsgSelection );


	// Compare the sent and received messages which have the same Subject field
	// when partial download limits is set
	void CompareMessagesWithPartialDownloadLimitsSetL();

	// Compare the sent and received messages which have the same Subject field
	// when full download limits is set
	void CompareMessagesWithFullDownloadLimitsSetL();
	
	// Compare messages when Full download limits with Headers Only option is set
	void CompareMessageWithGetHeadersOptionSetL();

	// Compare messages when the whole message has to be downloaded
	void CompareCompleteDownloadOfMessageL();

	// Compare messages when Download Attachments Only option is set
	void CompareMessageWithAttachmentsOnlyOptionSetL();

	// Compare messages when Full download limits is set and the message size is 
	// greater than the size limit
	void CompareMessageFullDownloadWhenMsgSizeGreaterThanLimitL();

	// Compare messages when Partial download with Body text and Attachment option is set
	void CompareMessagePartialDownloadWithBodyTextAndAttachmentsL();

	// Compare messages when Partial download with ECumulative option is set
	void CompareMessageDownloadWithCumulativeOptionSetL();

	// Compares the contents and flags of the Plain text part of the message.
	void ComparePlainBodyTextL(TBool aCompleteDownload);

	// Check if the footer message exists in the Body text part
	TInt CheckIfFooterMessageExists(TPtrC aBodyText);

	// Checks if any server message has been added to the Body text
	void CheckIfServerMessageExists(TPtrC aBodyText, TInt& aServerMsgLentght,
													 TInt& aServerMsgPosition);

	// Compares the contents and the flags for the Html part of the message
	void CompareHtmlL();

	// Compares the flag settings and contents of attachments
	void CompareAttachmentsL();
	
	// Check if the count of attachments in the received message is correct
	void CheckAttachmentCountL(TInt aSentMsgAttachmentCount, TInt aRecvMsgAttachmentCount);
	
	// Checks if the attachment was supposed to be downloaded when
	//  Partial download limit is set 
	TBool CheckIfAttachmentTobeDownloadedWithPartialDownloadSet(const TDesC& aAttachmentName, TInt32 aAttachmentSize);

	// Checks if the attachment was supposed to be downloaded when
	//  Full download limit is set 
	TBool CheckIfAttachmentTobeDownloadedWithCompleteDownloadSet(const TDesC& aAttachmentName);


	// Checks the complete and partial flag settings for the specified message part	
	void CheckFlagSettingsL(TMsvId aMsgPartId, TBool expectedCompleteFlagVal,
											  TBool expectedPartialFlagVal);

	// Checks the complete flag setting for specified attachment
	void CheckFlagSettingsL(CMsvAttachment& aMsgPart, TBool aExpectedCompleteFlagVal);
	
	// Compares the contents of two files, the file path are provided as inputs
	void CompareFilesL(const TPtrC aSentFilePath, const TPtrC aRecvFilePath,
											TInt& aError, TBool& aDiffFlag);

	// Check if the Test Case result is EPass
	inline TBool IsTestCaseStatusPass();

private:
	enum TMtfDownloadType 
		{ 
		/**	 Partial download limit is set */
		EPartialDownload,
		/**  Full download limit is */
		EFullDownload
		};

	/**  Session object */
	CMsvSession* iParamSession;

	/**  To know whether a partial for download limit is set */
	TMtfDownloadType iDownloadType;
	
	/**  To store Full download limits */
	TImImap4GetMailInfo	iImapCompleteDownloadLimits;

	/** To store Partial download limits */
	TImImap4GetPartialMailInfo	iImapPartialDownloadLimits;

	/**	To store cumulative size when TImImap4PartialMailOptions::ECumulative is set */
	TInt32 iCumulativeSize;

	/**	Utility class object storing the information of the sent email message*/ 
	CMtfTestActionUtilsEmailMessage* iSentMessageInfo;

	/**	Utility class object storing the information of the received email message*/ 
	CMtfTestActionUtilsEmailMessage* iRecvMessageInfo;
	};

#include "CMtfTestActionCompareEmailMessages.inl"

#endif //__CMTF_TEST_ACTION_COMPARE_EMAIL_MESSAGES_H__
