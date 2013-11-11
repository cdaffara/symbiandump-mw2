/**
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
*
*/



/**
 @file CMtfTestActionVerifyReplyToAddress.h
*/

#ifndef __CMTF_TEST_ACTION_VERIFY_REPLY_TO_ADDRESS_H__
#define __CMTF_TEST_ACTION_VERIFY_REPLY_TO_ADDRESS_H__
	
#include <msvapi.h>
#include <msvstd.h>
#include <miutmsg.h>

#include "CExpMailInfo.h"
#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionVerifyReplyToAddress,"VerifyReplyToAddress");
/*@}*/

/**
  Implements the action to compare the Reply-To address set in header of email from txt file to that of the
  email actual sent as stored in the Message store as a TMsvEntry object.
  @internalTechnology
*/

class CMtfTestActionVerifyReplyToAddress : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionVerifyReplyToAddress();
	virtual void ExecuteActionL();
private:
	CMtfTestActionVerifyReplyToAddress(CMtfTestCase& aTestCase);
	TBool LoadParametersL();
	
	TBool ReplyToHeaderFieldCompareL(TMsvEntry& aSentEntry, const TDesC& aFileName, CMsvSession* aSession);
	TBool IsEofL(RReadStream& aReadStream);
	HBufC* CreateFormattedAddressLC(const TDesC& aString, const TDesC& aAlias);

	CMsvSession*						iSession;
	CMsvEntrySelection*					iSentSelectionList;
	CMsvEntrySelection*					iInboxSelectionList;
	RFs									iFs;

	TInt								iReplyToAddress;
	HBufC*								iEmailFilePath;
	};
	
#endif //__CMTF_TEST_ACTION_VERIFY_REPLY_TO_ADDRESS_H__