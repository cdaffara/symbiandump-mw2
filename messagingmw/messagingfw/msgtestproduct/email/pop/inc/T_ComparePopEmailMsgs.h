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
 @file
*/


#ifndef __T_COMPARE_POP_EMAIL_MSGS_H__
#define __T_COMPARE_POP_EMAIL_MSGS_H__

	
// User include
#include "T_MsgAsyncStepPOP.h"
#include "T_MsgSharedDataPop.h"


// Epoc include
#include <msvstd.h>

// Literal used
_LIT(KComparePopEmailMsgs,"ComparePopEmailMsgs");


// Forward declaration
class CImEmailMessage;
class CExpPop3MailInfo;
class CMsvSession;
class MMsvAttachmentManager;


/**
Retireves the attacments related to a message entry.
*/
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


/**	
Implements the test step to compare email body text with the 
partial/full download limits set.
*/
class CT_MsgComparePopEmailMsgs : public CT_MsgAsyncStepPOP
	{
public:
	static CT_MsgComparePopEmailMsgs* NewL(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_MsgComparePopEmailMsgs();

	// CTestStep implementation
	virtual TVerdict doTestStepL();

private:
	CT_MsgComparePopEmailMsgs(CT_MsgSharedDataPop& aSharedDataPOP);

	//	CT_MsgAsyncStep implementation
	virtual void	ProgressL(TBool bFinal);

	//	MT_MsgActiveCallback implementation
	virtual void	CancelStep();

	TMsvId GetMessagePartIndexL(TMsvEntry& aEntry, TUid aUid);
	TBool CheckIfFooterMessageExistsL(TPtrC& aRecvBodyText, TInt& aFooterSize);
	TBool DoCompareL(TMsvEntry& aRecvEntry, CExpPop3MailInfo& aMailInfo, TMsvEntry& aSentEntry);
	TInt GetMessageLinesL(TMsvEntry& aEntry, TUid aUid, TBool& aFooterExists, TInt &aFooterSize);
	TInt CountLinesOfBodyTextL(CMsvEntry& aEntry, TBool& aFooterExists, TInt& aFooterSize);
	TInt CountLinesOfHtmlL(CMsvEntry& aEntry);	
	TBool LoadParametersL();

	CExpPop3MailInfo* GetExpPop3MailInfoLC(TInt aIndx);
	TBool CheckAttachmentsL(TMsvEntry& aRecvEntry, CExpPop3MailInfo& aMailInfo, TMsvEntry& aSentEntry);
	TBool CompareFilesL(TFileName& aFileName1, TFileName& aFileName2);
 	TInt CT_MsgComparePopEmailMsgs::CheckIfServerMessageExists(TPtrC aBodyText);

private:
	CMsvEntrySelection*					iSentSelectionList;
	CMsvEntrySelection*					iInboxSelectionList;
	RPointerArray<CExpPop3MailInfo>		iExpResults;
	RFs									iFs;
	};
	
#endif //__T_COMPARE_POP_EMAIL_MSGS_H__

