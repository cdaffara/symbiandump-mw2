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


#ifndef __CMTF_TEST_ACTION_UTILS_EMAIL_MESSAGE_H__
#define __CMTF_TEST_ACTION_UTILS_EMAIL_MESSAGE_H__

// System include
#include <e32base.h>
#include <msvapi.h>
#include <msvstd.h>
#include <miutmsg.h>

#include "CMtfTestCase.h"

/**
  Stores the information of an email message. Creates and CImEmailMessage object 
  for the message. Any operations related to the email message can be perfored using
  this the CImEmailMessage object.
  
  The class stores the message part Id and the size information for 
  Body Text and Html and also the Html path.
  
  If the Body Text part is modified, then StoreBodyTextInfoL() function needs to be 
  called to update the stored Body Text information.  Similarly, StoreHtmlInfoL() function
  needs to be called when Html part is modified by the email message object.
    
  @internalTechnology
*/
class CMtfTestActionUtilsEmailMessage
	{
public:
	static CMtfTestActionUtilsEmailMessage* NewL(CMsvEntry& aMsvEntry, CMtfTestCase& aTestCase);

	// Destructor
	~CMtfTestActionUtilsEmailMessage();

	// Sets the information about the Body text, Html and attachment parts
	TInt ProcessMessageInfoL();
	
	// Email message
	CImEmailMessage* GetEmailMessageL();

	// TMsvId of the email message
	TInt GetMessageId(TMsvId& aMsgId);

	// Size of the email message
	TInt GetMessageSize(TInt32& aMsgSize);

	// TMsvId of the Plain text part
	TMsvId GetBodyTextId();

	// Size of the Plain text part
	TInt32 GetBodyTextSize();

	// TMsvId of the Html part
	TMsvId GetHtmlEntryId();

	// Size of the Html part
	TInt32 GetHtmlSize();

	// Body Text
	void GetBodyTextL(CRichText& aRichText, CParaFormatLayer& aParaFormatLayer,
											CCharFormatLayer& aCharFormatLayer);

	// Html file name
	TInt HtmlFileName(TPtrC& aHtmlFileName);

	// Call asynchronous function to get the attachment list for the email message
	TInt GetMessageAttachmentListL();

	// Store the Id, size, and content information for the plain body text
	TInt StoreBodyTextInfoL();

	// Store the Id, size, and File name information for the Html part
	TInt StoreHtmlInfoL();

	// Check for the complete flag of the message part
	TInt CheckCompleteFlagL(TMsvId aMsgId, TBool& aCompleteFlate);

	// Check for the partial download flag of the message part
	TInt CheckPartialDownloadFlagL(TMsvId aMsgId, TBool& aPartialDownloadFlag);

private:
	// Constructor
	CMtfTestActionUtilsEmailMessage(CMsvEntry& aMsvEntry, CMtfTestCase& aTestCase);

	void ConstructL();
	
private:
	/** Id of the email message */
	CMsvEntry& iEmailEntry;
	
	/** Email message */
	CImEmailMessage* imEmailMessage;
	
	/** Id of the Body Text of the email message */
	TMsvId iBodyTextId;
	
	/** Size of the Body Text of the email message */
	TInt32 iBodyTextSize;

	/** Id of  Html part of the email message */
	TMsvId iHtmlEntryId;

	/** Size of the Html part of the email message */
	TInt32 iHtmlSize;
	
	/** Name of Html file */
	TPtrC iHtmlFileName;
	
	/** Reference to the current Test Case */
	CMtfTestCase& iTestCase;
	};

#endif //__CMTF_TEST_ACTION_UTILS_EMAIL_MESSAGE_H__
