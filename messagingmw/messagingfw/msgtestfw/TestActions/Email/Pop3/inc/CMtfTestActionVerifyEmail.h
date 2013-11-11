/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file 
 @internalTechnology 
*/

#ifndef __CMTF_TEST_ACTION_COMPARE_POP3_EMAIL_MESSAGES_H__
#define __CMTF_TEST_ACTION_COMPARE_POP3_EMAIL_MESSAGES_H__
	
#include <msvapi.h>
#include <e32def.h>
#include <e32des8.h>
#include <msvstd.h>
#include <miutmsg.h>
#include <imapset.h>

#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>



#include "CExpMailInfo.h"
#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionVerifyEmail,"VerifyEmail");
/*@}*/


/**
  Implements the action to compare email body text
  the partial/full download limits set.
  @internalTechnology
*/


class CPop3MailInfo;






class CMsvEntry;
class CImEmailMessage;


class CAttachmentItem : CBase
	{
public:	
	static CAttachmentItem* NewL(CMsvSession& aSession, TMsvEntry aEntry);
	static CAttachmentItem* NewLC(CMsvSession& aSession, TMsvEntry aEntry);
	~CAttachmentItem();
	
	void ConstructL(CMsvSession& aSession, TMsvEntry aEntry);
	MMsvAttachmentManager& MMsvAttachmentManager() const;
	
private:	
	CMsvEntry*	iMsvEntry;
	CImEmailMessage* iEmailMsg;
	
	};
	



class CMtfTestActionVerifyEmail : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase, CMtfTestActionParameters* ActionParameters );
	virtual ~CMtfTestActionVerifyEmail(); 
	virtual void ExecuteActionL();
private:
	CMtfTestActionVerifyEmail(CMtfTestCase& aTestCase);
	TMsvId GetMessagePartIndexL( TMsvEntry& aEntry, TUid aUid );
	TBool CheckIfFooterMessageExistsL( TPtrC& aRecvBodyText, TInt& aFooterSize );
	TBool DoCompareL( TMsvEntry& aRecvEntry, CExpPop3MailInfo& aMailInfo, TMsvEntry& aSentEntry );
	TInt GetMessageLinesL(TMsvEntry& aEntry, TUid aUid, TBool& aFooterExists, TInt &aFooterSize );
	TInt CountLinesOfBodyTextL( CMsvEntry& aEntry, TBool& aFooterExists, TInt& aFooterSize );
	TInt CountLinesOfHtmlL( CMsvEntry& aEntry );	
	TBool LoadParametersL();
	void CMtfTestActionVerifyEmail::CheckPartialDownloadedFlag();

	TBool CheckAttachmentsL( TMsvEntry& aRecvEntry, CExpPop3MailInfo& aMailInfo, TMsvEntry& aSentEntry );
	TBool CompareFilesL( TFileName& aFileName1, TFileName& aFileName2 );
	CMsvSession*						iSession;
	CMsvEntrySelection*					iSentSelectionList;
	CMsvEntrySelection*					iInboxSelectionList;
	RPointerArray<CExpPop3MailInfo>		iExpResults;
	RFs									iFs;
	};
	
#endif //__CMTF_TEST_ACTION_COMPARE_POP3_EMAIL_MESSAGES_H__
