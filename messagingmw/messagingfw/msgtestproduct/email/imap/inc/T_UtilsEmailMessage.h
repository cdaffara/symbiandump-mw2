/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* @file 
* This is the header file for Email utilities
* 
*
*/




#ifndef __T_EMAIL_UTILS_H__
#define __T_EMAIL_UTILS_H__



/* User defined classes */
#include <test/testexecutestepbase.h>


/* Epoc includes */
#include <msvstd.h>


/* Forward class declaration */
class CMsvEntry;
class CImEmailMessage;
class CRichText;
class CParaFormatLayer;
class CCharFormatLayer;


/* Implements a class which contains the utilities required for comparing email messages */
class CT_MsgUtilsEmailMessage
	{
public:
	/* Creates an object of CT_MsgUtilsEmailMessage */
	static CT_MsgUtilsEmailMessage* NewL(CMsvEntry& aMsvEntry,CTestStep& aTestStep);

	
	~CT_MsgUtilsEmailMessage();

	/* Sets the information about the Body text, Html and attachment parts */
	TInt ProcessMessageInfoL();

	/* Used to mnipulate an Email message */
	CImEmailMessage* GetEmailMessageL();

	/* Gets the TMsvId of the Email message */
	TInt GetMessageId(TMsvId& aMsgId);

	/* Gets the Size of the Email message */
	TInt GetMessageSize(TInt32& aMsgSize);

	/* Gets the TMsvId of the Plain text part */
	TMsvId GetBodyTextId();

	/* Gets the Size of the Plain text part */
	TInt32 GetBodyTextSize();

	/* Gets the TMsvId of the Html part */
	TMsvId GetHtmlEntryId();

	/* Gets the Size of the Html part */
	TInt32 GetHtmlSize();

	/* Gets the Body Text of the message*/
	void GetBodyTextL(CRichText& aRichText, CParaFormatLayer& aParaFormatLayer,
										CCharFormatLayer& aCharFormatLayer);

	/* Gets the Html file name */
	TInt HtmlFileName(TPtrC& aHtmlFileName);

	/* Calls an asynchronous function to get the attachment list for the Email message */
	TInt GetMessageAttachmentListL();

	/* Stores the Id, size, and content information for the plain body text */
	TInt StoreBodyTextInfoL();

	/* Stores the Id, size, and File name information for the Html part*/
	TInt StoreHtmlInfoL();

	/* Check for the complete flag of the message part */
	TInt CheckCompleteFlagL(TMsvId aMsgId, TBool& aCompleteFlate);

	/* Check for the partial download flag of the message part*/
	TInt CheckPartialDownloadFlagL(TMsvId aMsgId, TBool& aPartialDownloadFlag);
private:
	
	CT_MsgUtilsEmailMessage(CMsvEntry& aMsvEntry,CTestStep& aTestStep);

	void ConstructL();
private:
	/* Id of the email message*/
	CMsvEntry& iEmailEntry;
	
	/* A pointer to the Email message */
	CImEmailMessage* iEmailMessage;
	
	/* Id of the Body Text part of the Email message */
	TMsvId iBodyTextId;
	
	/* Size of the Body Text part of the email message */
	TInt32 iBodyTextSize;

	/* Id of  Html part of the email message */
	TMsvId iHtmlEntryId;

	/* Size of the Html part of the email message */
	TInt32 iHtmlSize;
	
	/* A buffer for the Html file name */
	TBuf<256> iHtmlFileNameBuf;

	/* Reference to the current Test Step */
	CTestStep& iTestStep;
	};
#endif /* __T_EMAIL_UTILS_H__ */
		



