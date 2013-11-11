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
// This is the header file for CT_MsgUtilsCompareEmailMessages class
// 
//

#ifndef __T_UTILS_COMPARE_EMAIL_MESSAGES_H__
#define __T_UTILS_COMPARE_EMAIL_MESSAGES_H__


/* User includes */
#include "T_UtilsEmailMessage.h"


/* Epoc includes */
#include <imapset.h>
#include <cimapsyncdownloadrules.h>


/* Forward declaration */
class CMsvAttachment;


/* Implements the utility required to compare the email messages */
class CT_MsgUtilsCompareEmailMessages : public CBase
	{
public:
	/* Creates an object of type CT_MsgUtilsCompareEmailMessages s*/
	static CT_MsgUtilsCompareEmailMessages* NewL(CTestStep& aTestStep);

	/* Destructor */
	~CT_MsgUtilsCompareEmailMessages();

public:
	/* 
	 * Compares the Email messages in the sent items folder and the specified remote folder, 
	 * based on the download limts set
	 */
	TBool	CompareEmailL(CMsvSession& aSession, TMsvId aImapServiceId, TMsvId aFolderId,TBool aDownLoadLimts,
						CMsvEntrySelection& aRecvMsgSelectionList,CMsvEntrySelection& aSentMsgSelectionList );
						
	/* 
	 * Compares the Email messages in the sent items folder and the specified remote folder, 
	 * based on the sync download rules set
	 */
	TBool CompareEmailWithDLRulesL(CMsvSession& aSession, TMsvId aImapServiceId, TMsvId aFolderId, TBool aDownLoadLimts,
						CMsvEntrySelection& aRecvMsgSelectionList, CMsvEntrySelection& aSentMsgSelectionList, CImapSyncDownloadRules::TRulesType aFolderType, TUint32 aBearerTypes);

	/* Sets the partial download limts using sync download rules */
	void SetPartialDownloadLimitsUsingDLRulesL(TMsvId aImapServiceId, TMsvId aFolderId, CImapSyncDownloadRules::TRulesType aFolderType, TUint32 aBearerTypes);

	/* Sets the partial/full download limts */
	void SetDownloadLimitsL(TMsvId aImapServiceId,TMsvId aFolderId);

	/* Compares the messages, with the same Subject field, in the Sent Items folder 
	 * with the those in the Remote inbox folder.If the message is found,
	 * sets the sent and received message information*/
	void CompareEmailMessagesL(CMsvSession& aSession, const CMsvEntrySelection& aSentMsgSelection,
							const CMsvEntrySelection& aRecvMsgSelection);

	/* Deletes the sent and received message information stored */
	void DeleteEmailMessageInfo();
	
	/* Sets the sent and received message information */ 
	void SetEmailMessageInfoL(CMsvEntry& aSentMsvEntry,
								CMsvEntry& aRecvMsvEntry);

	/* 
	 * Compares the sent and received messages which have the same Subject field
	 * when partial download limits is set
	 */
	void CompareMessagesWithPartialDownloadLimitsSetL();
	
	 /*
	  * Compares the sent and received messages which have the same Subject field
	  * when full download limits is set
	  */
	void CompareMessagesWithFullDownloadLimitsSetL();

	/* Compares messages when the entire message has to be downloaded */
	void CompareCompleteDownloadOfMessageL();

	/* Compares messages when Download Attachments Only option is set */
	void CompareMessageWithAttachmentsOnlyOptionSetL();

	/* Compares messages when Partial download with Body text and Attachment option is set */
	void CompareMessagePartialDownloadWithBodyTextAndAttachmentsL();

	/* Compares messages when Full download limits with Headers Only option is set */
	void CompareMessageWithGetHeadersOptionSetL();

	/* Compares messages when Full download limits is set and the message size is 
	 * greater than the size limit */
	void CompareMessageFullDownloadWhenMsgSizeGreaterThanLimitL();

	/* Compares messages when Partial download with ECumulative option is set */
	void CompareMessageDownloadWithCumulativeOptionSetL();

	/* Compares the contents and flags of the Plain text part of the message*/
	void ComparePlainBodyTextL(TBool aCompleteDownload);

	/* Checks if the footer message exists in the Body text part*/
	TInt CheckIfFooterMessageExists(TPtrC aBodyText);

	/* Checks if any server message has been added to the Body text*/
	void CheckIfServerMessageExists(TPtrC aBodyText, TInt& aServerMsgLength,
									TInt& aServerMsgPosition);
	
	/* Checks the complete and partial flag settings for the specified message part	*/
	void CheckFlagSettingsL(TMsvId aMsgPartId, TBool aExpectedCompleteFlagVal,
													   TBool aExpectedPartialFlagVal);

	/* Checks the complete flag settings for specified attachment */
	void CheckFlagSettingsL(CMsvAttachment& aMsgPart, TBool aExpectedCompleteFlagVal);

	/* Compares the contents and the flags for the Html part of the message */
	void CompareHtmlL();

	/* Compares the contents of two files, the file paths are provided as inputs*/
	void CompareFilesL(const TPtrC aSentFilePath, const TPtrC aRecvFilePath,
										TInt& aError, TBool& aDiffFlag);

	/* Compares the flag settings and contents of attachments*/
	void CompareAttachmentsL();

	/* Checks if the count of attachments in the received message is correct*/
	void CheckAttachmentCountL(TInt aSentMsgAttachmentCount, TInt aRecvMsgAttachmentCount);

	/* 
	 * Checks if the attachment was supposed to be downloaded when
	 * Full download limit is set 
	 */
	TBool CheckIfAttachmentTobeDownloadedWithCompleteDownloadSet(const TDesC& aAttachmentName);

	/* 
	 * Checks if the attachment was supposed to be downloaded when
	 * Partial download limit is set 
	 */
	TBool CheckIfAttachmentTobeDownloadedWithPartialDownloadSet(const TDesC& aAttachmentName, TInt32 aAttachmentSize);

	TBool CompareSenderL(CMsvSession& aSession, CMsvEntrySelection& aMsgSelection, const TDesC& aExpectedSender);

private:
	
	CT_MsgUtilsCompareEmailMessages(CTestStep& aTestStep);
	
	void ConstructL();

private:
	
	/* 
	 * Flag to indicate whether a full or partial download limit is set
	 * 1 - FullDownLoad  0 - PartialDownLoad
	 */
	TBool iDownloadLimits;
	
	/* To store Full download limits*/
	TImImap4GetMailInfo	iImapCompleteDownloadLimits;

	/* To store Partial download limits */
	TImImap4GetPartialMailInfo	iImapPartialDownloadLimits;

	/* To store cumulative size when TImImap4PartialMailOptions::ECumulative is set*/
	TInt32 iCumulativeSize;

	/* Utility class object for storing the information of the sent email message */
	CT_MsgUtilsEmailMessage* iSentMessageInfo;

	// Utility class object for storing the information of the received email message
	CT_MsgUtilsEmailMessage* iRecvMessageInfo;

	/* Reference to the current Test Step */
	CTestStep& iTestStep;

	/* Flaf to indicate the result of the comparison.
	 * It is initialised to ETrue
	 */
	TBool iResult;
	};
#endif /*__T_UTILS_COMPARE_EMAIL_MESSAGES_H__*/
