// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ctestsearchsortoperation.cpp
//
#include "ctestsearchsortoperation.h"
#include <msvapi.h>
#include <msvsearchsortquery.h>
#include <msvids.h>
#include <msvuids.h>
#include <msvapi.h>
#include <mtclreg.h>
#include <mtclbase.h>
#include <mtmdef.h>
#include <txtrich.h>
#include <cmsvattachment.h>
#include "MSVIPC.H"

#include <emailtestutils.h>
#include <msvtestutils.h>
#include <miuthdr.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);

//Database
_LIT(KTestDbFileName, "c:\\testDB.db");

_LIT(KMsgTo, "channa@symbian.com");
_LIT(KMsgCc, "symbianindia@symbian.com");
_LIT(KMsgBcc, "robert@symbian.com");
_LIT(KSubject, "MessagingTeam");
_LIT(KMsgDescription, "hello");
_LIT(KMsgBody, "mobile");
_LIT(KEmtyDesc, "");


const TUid KUidFindTestPOP3Mtm = {0x10001029};

_LIT(KMiutSubject1, "short mail about something interesting");
_LIT(KMiutSubject2, "Fwd : Why men lie and woman cry");
_LIT(KMiutSubject3, "Fwd : Intere");


_LIT8(KMiutMsgId, "0123456789AB.CdeFGHIj@symbian.com");

_LIT16(KMiutFrom1, "ashok@symbian.commmm");
_LIT16(KMiutFrom2, "yatish@symbian.com");
_LIT(KMiutFrom3, "someone@symbian.com");


_LIT(KMiutReplyTo, "yatish@symbian.com");


_LIT(KMiutToRecipients1, "ashok@symbian.com");
_LIT(KMiutToRecipients2, "yatish@address.com");
_LIT(KMiutToRecipients3, "Ashok@address.com");
_LIT(KMiutToRecipients4, "AshokKumar@address.com"); 
_LIT(KMiutToRecipients5, "ashokkumar@address.com");
_LIT(KMiutToRecipients6, "symbianrocks@address.com");


_LIT(KMiutCcRecipients1, "channa@address.com");
_LIT(KMiutCcRecipients2, "sukanta@address.com");
_LIT(KMiutCcRecipients3, "kavita_khatawate@address.com");
_LIT(KMiutCcRecipients4, "sukantabanik@address.com");
_LIT(KMiutCcRecipients5, "Sukantabanik@address.com");

_LIT(KMiutBccRecipients1, "bcc1@address.com");
_LIT(KMiutBccRecipients2, "bcc2@address.com");
_LIT(KMiutBccRecipients3, "bcc3@address.com");
_LIT(KMiutBccRecipients4, "bcc4@address.com");
_LIT(KMiutBccRecipients5, "bcc5@address.com");
_LIT(KMiutBccRecipients6, "bcc6@address.com");


_LIT8(KMiutResentMsgId, "0123456789AB.CdeFGHIj@symbian.com");
_LIT(KMiutResentFrom, "test@psion6.demon.co.uk");
_LIT(KMiutResentToRecipients1, "recipient1@address.com");
_LIT(KMiutResentToRecipients2, "recipient2@address.com");
_LIT(KMiutResentCcRecipients1, "cc1@address.com");
_LIT(KMiutResentCcRecipients2, "cc2@address.com");
_LIT(KMiutResentBccRecipients, "bcc@address.com");

#define KMiutRemoteSize 					123456
#define KMaxImHeaderStringLengthLong		KMaxImHeaderStringLength+1


LOCAL_D TBuf<KMaxImHeaderStringLengthLong> longValue;
LOCAL_D TBuf8<KMaxImHeaderStringLengthLong> longValue2;
RTest testt(_L("Find Test"));
LOCAL_D CEmailTestUtils* testUtils;


class CSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
public:
	TMsvSessionEvent iEvent;
	};

void CSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
	{
	iEvent = aEvent;
	}

CTestSearchSortOperation::CTestSearchSortOperation()
	{
	}

CTestSearchSortOperation ::~CTestSearchSortOperation()
	{
	}


void CTestSearchSortOperation::SetupL()
	{
	_LIT(KSetupL, "SetupL");
	INFO_PRINTF1(KSetupL);
	
	iActiveWaiter = new (ELeave) CActiveWaiter();

	}
	
void CTestSearchSortOperation::CreateValidQueryOneL()
	{
	_LIT(KFunction, "CreateValidQueryOneL");
	INFO_PRINTF1(KFunction);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(ETrue);
	searchQuery->SetCaseSensitiveOption(ETrue);
	searchQuery->SetWildCardSearch(EFalse);
	
	TInt error = KErrNone;

	TRAP(error, searchQuery->AddSearchOptionL(EMsvTo, KMsgTo, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TInt size = 50;
	TRAP(error, searchQuery->AddSearchOptionL(EMsvSize, size, EMsvGreaterThan));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSortOptionL(EMsvSize, EMsvSortAscending));	
	ASSERT_EQUALS(error, KErrNone);	
	
	CleanupStack::PopAndDestroy();
	}

void CTestSearchSortOperation::CreateValidQueryTwoL()
	{
	_LIT(KFunction, "CreateValidQueryTwoL");
	INFO_PRINTF1(KFunction);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(ETrue);
	
	TInt size = 250;
	TInt error = KErrNone;
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvSize, size, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvTo, KMsgTo, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvCc, KMsgCc, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvSubject, KSubject, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvDescription, KMsgDescription, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending));	
	ASSERT_EQUALS(error, KErrNone);
	
	CleanupStack::PopAndDestroy();
	}

void CTestSearchSortOperation::CreateValidQueryThreeL()
	{
	_LIT(KFunction, "CreateValidQueryOneL");
	INFO_PRINTF1(KFunction);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(ETrue);
	searchQuery->SetCaseSensitiveOption(ETrue);
	searchQuery->SetWildCardSearch(EFalse);
	
	TInt error = KErrNone;

	TRAP(error, searchQuery->AddSearchOptionL(EMsvTo, KMsgTo, EMsvLessThan));
	ASSERT_EQUALS(error, KErrArgument);

	TRAP(error, searchQuery->AddSearchOptionL(EMsvNew, ETrue));
	ASSERT_EQUALS(error, KErrNone);

	TRAP(error, searchQuery->AddSearchOptionL(EMsvCc, ETrue));
	ASSERT_EQUALS(error, KErrArgument);
	
	TRAP(error, searchQuery->AddSortOptionL(EMsvSize, EMsvSortAscending));	
	ASSERT_EQUALS(error, KErrNone);

	//search level should be
	ASSERT_EQUALS(searchQuery->iQueryCount, 1);
	
	CleanupStack::PopAndDestroy();
	}

void CTestSearchSortOperation::CreateQueryWithInvalidDateL()
	{
	_LIT(KFunction, "CreateQueryWithInvalidDateL");
	INFO_PRINTF1(KFunction);

	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(ETrue);
	searchQuery->SetCaseSensitiveOption(ETrue);
	searchQuery->SetWildCardSearch(EFalse);
	
	TInt error = KErrNone;

	TRAP(error, searchQuery->AddSearchOptionL(EMsvBcc, KMsgBcc, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);

	_LIT(KMsgInvalidDate, "ABCD0425:defabc.abcdefxx");	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvDate, KMsgInvalidDate, EMsvEqual));
	ASSERT_EQUALS(error, KErrGeneral );
	
	CleanupStack::PopAndDestroy();
	}

void CTestSearchSortOperation::CreateQueryWithInvalidSizeL()
	{
	_LIT(KFunction, "CreateQueryWithInvalidSizeL");
	INFO_PRINTF1(KFunction);

	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(ETrue);
	searchQuery->SetCaseSensitiveOption(ETrue);
	searchQuery->SetWildCardSearch(EFalse);
	
	TInt error = KErrNone;

	TInt size = -1;
	//Invalid size
	TRAP(error, searchQuery->AddSearchOptionL(EMsvSize, size, EMsvEqual));
	ASSERT_EQUALS(error, KErrArgument);

	CleanupStack::PopAndDestroy();
	}
	
void CTestSearchSortOperation::CreateTooBigQueryL()
	{
	_LIT(KFunction, "CreateTooBigQueryL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KStringIsTooBig, "SearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBigSearchSortStringIsTooBig");
		
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(ETrue);
	searchQuery->SetCaseSensitiveOption(ETrue);
	searchQuery->SetWildCardSearch(EFalse);
	
	//Queey string is too big
	TRAPD(error, searchQuery->AddSearchOptionL(EMsvSubject, KStringIsTooBig, EMsvEqual));
	ASSERT_EQUALS(error, KErrQueryTooBig);
	
	CleanupStack::PopAndDestroy();
	}


void CTestSearchSortOperation::CreateQueryForMultilevelSortL()
	{
	_LIT(KFunction, "CreateQueryForMultilevelSortL");
	INFO_PRINTF1(KFunction);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(ETrue);
	searchQuery->SetWildCardSearch(ETrue);
		
	TInt error = KErrNone;
	
	//sort not supported on body
	TRAP(error, searchQuery->AddSortOptionL(EMsvBody, EMsvSortAscending));	
	ASSERT_EQUALS(error, KErrNotSupported);
	
	//single levele sort
	TRAP(error, searchQuery->AddSortOptionL(EMsvTo, EMsvSortDescending));	
	ASSERT_EQUALS(error, KErrNone);

	//multilevel sort not supported
	TRAP(error, searchQuery->AddSortOptionL(EMsvDate, EMsvSortDescending));	
	ASSERT_EQUALS(error, KErrNotSupported);

	CleanupStack::PopAndDestroy();
	}


void CTestSearchSortOperation::CreateSearchQueryWithMaxLevelsL()
	{
	_LIT(KFunction, "CreateSearchQueryWithMaxLevelsL");
	INFO_PRINTF1(KFunction);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(ETrue);
	
	TInt size = 250;
	TInt error = KErrNone;
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvSize, size, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvTo, KMsgTo, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvCc, KMsgCc, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvSubject, KSubject, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvDescription, KMsgDescription, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
		
	TRAP(error, searchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending));	
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvBody, KMsgBody, EMsvEqual));
	ASSERT_EQUALS(error, KErrMsvQueryMaxLimit);

	CleanupStack::PopAndDestroy();
	}

void CTestSearchSortOperation::CreateSortQueryWithSubfolderOptionL()
	{
	_LIT(KFunction, "CreateSortQueryWithSubfolderOptionL");
	INFO_PRINTF1(KFunction);
	
	CSessionObserver* observer = new(ELeave)CSessionObserver;
	CleanupStack::PushL(observer);

	CMsvSession* session = CMsvSession::OpenSyncL(*observer);
	CleanupStack::PushL(session);
	
	CMsvSearchSortOperation* search = CMsvSearchSortOperation::NewL(*session);
	CleanupStack::PushL(search);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(EFalse);
	searchQuery->SetSubFolderSearch(ETrue);
	
	TInt error = KErrNone;

	TRAP(error, searchQuery->AddSortOptionL(EMsvSize, EMsvSortAscending));	
	ASSERT_EQUALS(error, KErrNone);
	
	CleanupStack::Pop(searchQuery);
	
	//iActiveWaiter->WaitActive();		
	TRAP(error, search->RequestL(searchQuery, ETrue, iActiveWaiter->iStatus));
	ASSERT_EQUALS(error, KErrNotSupported);
	
	CleanupStack::PopAndDestroy(3); // search, session, observer
	}

void CTestSearchSortOperation::CreateQueryWithAttachmentTypeL()
	{
	_LIT(KFunction, "CreateQueryWithAttachmentTypeL");
	INFO_PRINTF1(KFunction);
	
	//query for CMsvAttachment::EMsvFile
	_LIT(KQuery1,"CMsvAttachment::EMsvFile");
	INFO_PRINTF1(KQuery1);
	CMsvSearchSortQuery* searchQuery1 = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery1);
	
	searchQuery1->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery1->SetResultType(EMsvResultAsTMsvId);
	searchQuery1->SetWholeWord(EFalse);
	searchQuery1->SetCaseSensitiveOption(EFalse);
	searchQuery1->SetWildCardSearch(ETrue);
	
	TInt error = KErrNone;
	
	TRAP(error, searchQuery1->AddSearchOptionL(EMsvAttachment, CMsvAttachment::EMsvFile, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TInt value = 0;
	TLex iLex(searchQuery1->iQueryTable[0].iQueryString);
	iLex.Val(value);
	ASSERT_EQUALS(value, 2);
	CleanupStack::PopAndDestroy(searchQuery1);
	
	//query for CMsvAttachment::EMsvLinkedFile
	_LIT(KQuery2,"CMsvAttachment::EMsvLinkedFile");
	INFO_PRINTF1(KQuery2);

	CMsvSearchSortQuery* searchQuery2 = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery2);
	
	searchQuery2->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery2->SetResultType(EMsvResultAsTMsvId);
	searchQuery2->SetWholeWord(EFalse);
	searchQuery2->SetCaseSensitiveOption(EFalse);
	searchQuery2->SetWildCardSearch(ETrue);
	
	TRAP(error, searchQuery2->AddSearchOptionL(EMsvAttachment, CMsvAttachment::EMsvLinkedFile, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	value = 0;
	TLex iLex2(searchQuery2->iQueryTable[0].iQueryString);
	iLex2.Val(value);
	ASSERT_EQUALS(value, 3);
	CleanupStack::PopAndDestroy(searchQuery2);

	//query for CMsvAttachment::EMsvMessageEntry
	_LIT(KQuery3,"CMsvAttachment::EMsvMessageEntry");
	INFO_PRINTF1(KQuery3);
	CMsvSearchSortQuery* searchQuery3 = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery3);
	
	searchQuery3->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery3->SetResultType(EMsvResultAsTMsvId);
	searchQuery3->SetWholeWord(EFalse);
	searchQuery3->SetCaseSensitiveOption(EFalse);
	searchQuery3->SetWildCardSearch(ETrue);
	
	TRAP(error, searchQuery3->AddSearchOptionL(EMsvAttachment, CMsvAttachment::EMsvMessageEntry, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	value = 0;
	TLex iLex3(searchQuery3->iQueryTable[0].iQueryString);
	iLex3.Val(value);
	ASSERT_EQUALS(value, 4);
	
	//for error condition
	TRAP(error, searchQuery3->AddSearchOptionL(EMsvAttachment, 10, EMsvEqual));
	ASSERT_EQUALS(error, KErrArgument);
	
	CleanupStack::PopAndDestroy(searchQuery3);
	}
	
void CTestSearchSortOperation::CreateQueryUsingNewLcL()
	{
	_LIT(KFunction, "CreateQueryUsing NewLC");
	INFO_PRINTF1(KFunction);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewLC();
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(ETrue);
	searchQuery->SetCaseSensitiveOption(ETrue);
	searchQuery->SetWildCardSearch(EFalse);
	
	TInt error = KErrNone;

	TRAP(error, searchQuery->AddSearchOptionL(EMsvTo, KMsgTo, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	TInt size = 50;
	TRAP(error, searchQuery->AddSearchOptionL(EMsvSize, size, EMsvGreaterThan));
	ASSERT_EQUALS(error, KErrNone);
	
	TRAP(error, searchQuery->AddSortOptionL(EMsvSize, EMsvSortAscending));	
	ASSERT_EQUALS(error, KErrNone);	
	
	CleanupStack::PopAndDestroy();
	}
	
void CTestSearchSortOperation::CreateSearchQueryWithInvalidQueryOptionL()
	{
	_LIT(KFunction, "CreateSearchQueryWithInvalidQueryOptionL");
	INFO_PRINTF1(KFunction);
	
	CSessionObserver* observer = new(ELeave)CSessionObserver;
	CleanupStack::PushL(observer);

	CMsvSession* session = CMsvSession::OpenSyncL(*observer);
	CleanupStack::PushL(session);
	
	CMsvSearchSortOperation* search = CMsvSearchSortOperation::NewL(*session);
	CleanupStack::PushL(search);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(ETrue);
	
	TInt size = 250;
	TInt error = KErrNone;
	
	//Invalid Query Option for AddSearchOptionL (TMsvMessagePart, const TInt, TMsvRelationOp);
	TRAP(error, searchQuery->AddSearchOptionL(TMsvMessagePart(0), size, EMsvEqual));
	ASSERT_EQUALS(error, KErrArgument);
	
	
	TRAP(error, searchQuery->AddSearchOptionL(TMsvMessagePart(20), size, EMsvEqual));
	ASSERT_EQUALS(error, KErrArgument);
	
	//Invalid Query Option for AddSearchOptionL (TMsvMessagePart, const TDesC& , TMsvRelationOp);	
	TRAP(error, searchQuery->AddSearchOptionL(TMsvMessagePart(-1), KMsgDescription, EMsvEqual));
	ASSERT_EQUALS(error, KErrArgument);
	
	TRAP(error, searchQuery->AddSearchOptionL(TMsvMessagePart(25), KMsgDescription, EMsvEqual));
	ASSERT_EQUALS(error, KErrArgument);
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvTo, KEmtyDesc, EMsvEqual));
	ASSERT_EQUALS(error, KErrArgument);
	
	//Invalid Query Option for AddSearchOptionL (TMsvMessagePart, TBool);
	TRAP(error, searchQuery->AddSearchOptionL(TMsvMessagePart(-3), ETrue));
	ASSERT_EQUALS(error, KErrArgument);
	
	TRAP(error, searchQuery->AddSearchOptionL(TMsvMessagePart(30), ETrue));
	ASSERT_EQUALS(error, KErrArgument);
	
	//Invalid Query Option for AddSortOptionL (TMsvMessagePart, TMsvSortOrder );
	TRAP(error, searchQuery->AddSortOptionL(EMsvBody, EMsvSortAscending));	
	ASSERT_EQUALS(error, KErrNotSupported); 
	
	TRAP(error, searchQuery->AddSortOptionL(TMsvMessagePart(-1), EMsvSortAscending));	
	ASSERT_EQUALS(error, KErrArgument);
	
	TRAP(error, searchQuery->AddSortOptionL(TMsvMessagePart(22), EMsvSortAscending));	
	ASSERT_EQUALS(error, KErrArgument); 
	
	TInt queryCount = searchQuery->GetMaxQuerySize();
	
	CleanupStack::Pop(searchQuery);
	
	TRAP(error, search->RequestL(searchQuery, ETrue, iActiveWaiter->iStatus));
	ASSERT_EQUALS(error, KErrArgument);
	
	CleanupStack::PopAndDestroy(3); // search, session, observer
	}

LOCAL_C void InitL()
	{
	testUtils = CEmailTestUtils::NewLC(testt);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	}

LOCAL_C void CreateMessageHeaderL(CImHeader& imHeader)
	{
	// a bunch of data for the header object....
	imHeader.Reset();
	imHeader.SetSubjectL(KMiutSubject1);

	imHeader.SetImMsgIdL(KMiutMsgId);

	imHeader.SetFromL(KMiutFrom1);
	imHeader.SetFromL(KMiutFrom2);
	imHeader.SetReplyToL(KMiutReplyTo);
	imHeader.SetRemoteSize(KMiutRemoteSize);

	imHeader.ToRecipients().AppendL(KMiutToRecipients1);
	imHeader.ToRecipients().AppendL(KMiutToRecipients2);
	imHeader.ToRecipients().AppendL(KMiutToRecipients3);
	imHeader.ToRecipients().AppendL(KMiutToRecipients4);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients1);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients2);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients3);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients4);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients1);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients2);
	}

LOCAL_C void CreateMessageHeader2L(CImHeader& imHeader)
	{
	// a bunch of data for the header object....
	imHeader.Reset();
	imHeader.SetSubjectL(KMiutSubject2);

	imHeader.SetImMsgIdL(KMiutMsgId);

	imHeader.SetFromL(KMiutFrom1);
	imHeader.SetReplyToL(KMiutReplyTo);
	imHeader.SetRemoteSize(KMiutRemoteSize);

	imHeader.ToRecipients().AppendL(KMiutToRecipients3);
	imHeader.ToRecipients().AppendL(KMiutToRecipients4);
	imHeader.ToRecipients().AppendL(KMiutToRecipients5);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients3);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients4);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients3);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients4);
	
	imHeader.SetResentMsgIdL(KMiutResentMsgId);
	imHeader.SetResentFromL(KMiutResentFrom);
	imHeader.ResentToRecipients().AppendL(KMiutResentToRecipients1);
	imHeader.ResentToRecipients().AppendL(KMiutResentToRecipients2);
	imHeader.ResentCcRecipients().AppendL(KMiutResentCcRecipients1);
	imHeader.ResentCcRecipients().AppendL(KMiutResentCcRecipients2);
	imHeader.ResentBccRecipients().AppendL(KMiutResentBccRecipients);
	}

LOCAL_C void CreateMessageHeader3L(CImHeader& imHeader)
	{
	// a bunch of data for the header object....
	imHeader.Reset();
	imHeader.SetSubjectL(KMiutSubject3);

	imHeader.SetImMsgIdL(KMiutMsgId);

	imHeader.SetFromL(KMiutFrom3);
	imHeader.SetReplyToL(KMiutReplyTo);
	imHeader.SetRemoteSize(KMiutRemoteSize);

	imHeader.ToRecipients().AppendL(KMiutToRecipients4);
	imHeader.ToRecipients().AppendL(KMiutToRecipients5);
	imHeader.ToRecipients().AppendL(KMiutToRecipients6);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients4);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients5);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients5);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients6);
	
	imHeader.SetResentMsgIdL(KMiutResentMsgId);
	imHeader.SetResentFromL(KMiutResentFrom);
	imHeader.ResentToRecipients().AppendL(KMiutResentToRecipients1);
	imHeader.ResentToRecipients().AppendL(KMiutResentToRecipients2);
	imHeader.ResentCcRecipients().AppendL(KMiutResentCcRecipients1);
	imHeader.ResentCcRecipients().AppendL(KMiutResentCcRecipients2);
	imHeader.ResentBccRecipients().AppendL(KMiutResentBccRecipients);
	}

void CTestSearchSortOperation::CreateMessagesL(TUid aMtm)
	{
	_LIT(KName, "TEST");
	HBufC* data = HBufC::NewL(15); 
	*data = KName;
		
	CParaFormatLayer* paraLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraLayer);
	CCharFormatLayer* charLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charLayer);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iDescription.Set(data->Des());
	folder.iDetails.Set(data->Des());
	testUtils->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	testUtils->iMsvEntry->CreateL(folder);
	testUtils->iMsvEntry->SetEntryL(folder.Id());
	//folderId=folder.Id();

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = aMtm;
	entry1.iDescription.Set(data->Des());
	entry1.iDetails.Set(data->Des());
	testUtils->iMsvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	testUtils->iMsvEntry->CreateL(entry1);
	TRAPD(error,testUtils->iMsvEntry->SetEntryL(entry1.Id()));
	//entry1Id=entry1.Id();
	aSelectionArray.AppendL(entry1.Id());


	CMsvStore* fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);

	CImHeader* header = CImHeader::NewLC(); // Cleaned up when exiting this menu selection
	CreateMessageHeaderL(*header);
	header->StoreL(*fileStore);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(2); // filestore, header

	TMsvEmailEntry ementry1;
	ementry1.iMtm = aMtm;
	ementry1.iServiceId = testUtils->iMsvEntry->Entry().iServiceId;
	ementry1.iType = KUidMsvEmailTextEntry;
	ementry1.iDescription.Set(data->Des());
	ementry1.iDetails.Set(data->Des());
	testUtils->iMsvEntry->CreateL(ementry1);
	TRAP(error,testUtils->iMsvEntry->SetEntryL(ementry1.Id()));

	fileStore=NULL;
	fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);
	
	CRichText* text1=CRichText::NewL(paraLayer, charLayer);
	CleanupStack::PushL(text1);
	text1->InsertL(0,_L("This is the body of first mail which has something"));
	fileStore->StoreBodyTextL(*text1);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(2); // filestore


	fileStore=NULL;
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDescription.Set(data->Des());
	entry2.iDetails.Set(data->Des());
	testUtils->iMsvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	testUtils->iMsvEntry->CreateL(entry2);
	TRAP(error,testUtils->iMsvEntry->SetEntryL(entry2.Id()));
	//entry2Id=entry2.Id();
	aSelectionArray.AppendL(entry2.Id());
	
	fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);
	CImHeader* header1 = CImHeader::NewLC(); 
	CreateMessageHeader2L(*header1);
	header1->StoreL(*fileStore);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(2); // filestore,header


	TMsvEmailEntry ementry2;
	ementry2.iMtm = aMtm;
	ementry2.iServiceId = testUtils->iMsvEntry->Entry().iServiceId;
	ementry2.iType = KUidMsvEmailTextEntry;
	ementry2.iDescription.Set(data->Des());
	ementry2.iDetails.Set(data->Des());
	testUtils->iMsvEntry->CreateL(ementry2);
	TRAP(error,testUtils->iMsvEntry->SetEntryL(ementry2.Id()));

	fileStore=NULL;
	fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);
	CRichText* text2=CRichText::NewL(paraLayer, charLayer);
	CleanupStack::PushL(text2);
	text2->InsertL(0,_L("Be slow to promise and quick to perform.Be swift to hear, slow to speak. thats love..."));
	fileStore->StoreBodyTextL(*text2);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(2); // filestore,text


	fileStore=NULL;
	TMsvEntry entry3;
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDescription.Set(data->Des());
	entry3.iDetails.Set(data->Des());
	testUtils->iMsvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	testUtils->iMsvEntry->CreateL(entry3);
	TRAP(error,testUtils->iMsvEntry->SetEntryL(entry3.Id()));
	//entry3Id=entry3.Id();
	aSelectionArray.AppendL(entry3.Id());
	
	fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);
	CImHeader* header2 = CImHeader::NewLC(); 
	CreateMessageHeader3L(*header2);
	header2->StoreL(*fileStore);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(2); // filestore,header
	
	
	TMsvEmailEntry ementry3;
	ementry3.iMtm = aMtm;
	ementry3.iServiceId = testUtils->iMsvEntry->Entry().iServiceId;
	ementry3.iType = KUidMsvEmailTextEntry;
	ementry3.iDescription.Set(data->Des());
	ementry3.iDetails.Set(data->Des());
	testUtils->iMsvEntry->CreateL(ementry3);
	TRAP(error,testUtils->iMsvEntry->SetEntryL(ementry3.Id()));

	fileStore=NULL;
	fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);
	CRichText* text3=CRichText::NewL(paraLayer, charLayer);
	CleanupStack::PushL(text3);
	text3->InsertL(0,_L("You can understand the facts of life. But facts of love are much different. To love you love someone or someone loves you.. But when you both love each other you're in love with one another. First become friends and let it go. Then get serious and get together. Just make yourself known as a person not as someone you don't want to be known as..."));
	fileStore->StoreBodyTextL(*text3);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(4); // filestore,header,text,layers
	}

void CTestSearchSortOperation::TestHeaderSearchBodyL()
	{
	_LIT(KFunction, "TestHeaderSearchBodyL");
	INFO_PRINTF1(KFunction);

	InitL();
	testUtils->GoClientSideL();
	
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	_LIT(KSearchBody, "love");//Body String
	TBufC<20> searchStringBody(KSearchBody);
	
	CSessionObserver* observer = new(ELeave)CSessionObserver;
	CleanupStack::PushL(observer);

	CMsvSession* session = CMsvSession::OpenSyncL(*observer);
	CleanupStack::PushL(session); 
	
	CMsvSearchSortOperation* search = CMsvSearchSortOperation::NewL(*session);
	CleanupStack::PushL(search);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(ETrue);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(EFalse);
	searchQuery->SetSubFolderSearch(ETrue);
	
	TInt error = KErrNone;

	TRAP(error, searchQuery->AddSearchOptionL(EMsvBody, searchStringBody, EMsvEqual));	
	ASSERT_EQUALS(error, KErrNone);
		
	CleanupStack::Pop(searchQuery);
	
	search->RequestL(searchQuery, ETrue, iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();		
	
	RArray<TMsvEntry> resultArray;
	search->GetResultsL(resultArray);
	
	_LIT(KFormatId, "TMsvId %D");
	
	for(TInt i=0;i<resultArray.Count();i++)
		{
		INFO_PRINTF2(KFormatId,resultArray[i].Id());
		}
	ASSERT_EQUALS(resultArray.Count(), 2);
	
	CleanupStack::PopAndDestroy(4); // search, session, observer
	resultArray.Close();
	}


void CTestSearchSortOperation::CreateEntryL(TMsvId aId, TMsvId aParentId, TUid aUid, TMsvId& aVisibleFolder, TBool aIsVisible /* (Default = EFalse)*/, TBool isPreparationFlag /*(Default=EFalse)*/, TBool iTestValue/*(Default=EFalse)*/,TBool aService)
	{
	TMsvEntry sampleTMsvEntry;
	TMsvId temp = aVisibleFolder;
	
	sampleTMsvEntry.SetId(aId);
	sampleTMsvEntry.SetParent(aParentId);
	
	sampleTMsvEntry.SetVisible(aIsVisible);
	sampleTMsvEntry.SetVisibleFolderFlag(aIsVisible);
	
	// Extra Information (Needed only in TestGetEntry()
	if(iTestValue)
		{	
		sampleTMsvEntry.SetUnread(ETrue);
		sampleTMsvEntry.SetAttachment(ETrue);
		sampleTMsvEntry.SetNew(ETrue);
		sampleTMsvEntry.SetPriority(EMsvLowPriority);
		sampleTMsvEntry.iSize = 64;
		sampleTMsvEntry.iDate = 1234;
		sampleTMsvEntry.iType = KUidMsvServiceEntry;
		sampleTMsvEntry.iMtm = KUidMsvServiceEntry;
		}	
	else
		{
		sampleTMsvEntry.SetUnread(EFalse);
		sampleTMsvEntry.SetAttachment(EFalse);
		sampleTMsvEntry.SetNew(EFalse);
		sampleTMsvEntry.SetPriority(EMsvMediumPriority);
		sampleTMsvEntry.iSize = 32;
		sampleTMsvEntry.iDate.UniversalTime();
		sampleTMsvEntry.iType = aUid;
		sampleTMsvEntry.iMtm = aUid;
		}
		
	if (aService)
		{
		sampleTMsvEntry.iType = KUidMsvServiceEntry;
		sampleTMsvEntry.iServiceId = 4097;		// Some Random Id.
		}
	else
		{
		sampleTMsvEntry.iRelatedId = aId+20;		// Some Random Id.
		sampleTMsvEntry.iServiceId = aId+10;		// Some Random Id.		
		}

	sampleTMsvEntry.iError = NULL;		
	sampleTMsvEntry.iBioType = 64;		
	sampleTMsvEntry.iMtmData1 = 64;		
	sampleTMsvEntry.iMtmData2 = 64;		
	sampleTMsvEntry.iMtmData3 = 64;	
	sampleTMsvEntry.SetInPreparation(isPreparationFlag);
	
	if(iTestValue)
		{
		_LIT(KName, "TEST");
		HBufC* data = HBufC::NewL(15); 
		*data = KName;
		sampleTMsvEntry.iDescription.Set(data->Des());
		sampleTMsvEntry.iDetails.Set(data->Des());
	
		testUtils->SetEntryL(aParentId);
		testUtils->CreateEntryL(sampleTMsvEntry);
		
		delete data;
		data = NULL;
		}
	else
		{
		_LIT(KName, "Hello World");
		HBufC* data = HBufC::NewL(15); 
		*data = KName;
		sampleTMsvEntry.iDescription.Set(data->Des());
		sampleTMsvEntry.iDetails.Set(data->Des());
	
		testUtils->SetEntryL(aParentId);
		testUtils->CreateEntryL(sampleTMsvEntry);
		
		delete data;
		data = NULL;
		}
	}


void CTestSearchSortOperation::SeachSortRequestL()
	{
	_LIT(KFunction, "SeachSortRequestL");
	INFO_PRINTF1(KFunction);
	
	InitL();
	testUtils->GoClientSideL();
			
	TMsvId visibleFolder;
	RArray<TMsvId> entrySelection;
	TInt err;	

	visibleFolder = KMsvRootIndexEntryIdValue;
	for(TInt index=1; index<=100; ++index)
		{
		if((index%10) == 0)
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId + index, KMsvGlobalInBoxIndexEntryIdValue, KUidMsvMessageEntry, visibleFolder));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
			}
		else
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvGlobalInBoxIndexEntryIdValue, KUidMsvMessageEntry, visibleFolder));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
			}
		}
	
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvRootIndexEntryIdValue, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CMsvSearchSortOperation* search = CMsvSearchSortOperation::NewL(*session);
	CleanupStack::PushL(search);
		
	_LIT(KSizeString, "64");//Size Search String
	TBufC<2> sizeSearch(KSizeString);
	
	_LIT(KSizeString2, "32");//Size Search String
	TBufC<2> sizeSearch2(KSizeString2);
	
	_LIT(KDateString, "1234");//Date Search String
	TBufC<4> dateSearch(KDateString);
	

	//TBool cancelOperation=EFalse;
	
	
	_LIT(KSearchString, "Test");
	TBufC<300> searchString(KSearchString);
	
	_LIT(KFormatId, "QueryId %D");
	_LIT(KResultCount, "Result count: %D");

	//searchQuery will be deleted by messaging client
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(ETrue);
	
	TInt error = KErrNone;
	
	TRAP(error, searchQuery->AddSearchOptionL(EMsvDescription, searchString, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	CleanupStack::Pop(searchQuery);
		
	RArray<TMsvId> result;
	search->RequestL(searchQuery, ETrue, iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	search->GetResultCountL();
	
	RArray<TMsvEntry> entryResults;
	TMsvEntry entry;
	TInt count=0;
	TRAPD(ret1,search->GetResultsL(entryResults));
	if(ret1 == KErrNone)
		{
		count = entryResults.Count();
		}
	TInt queryId1 = search->GetQueryIdL();
	
	INFO_PRINTF2(KFormatId,queryId1);
	INFO_PRINTF2(KResultCount,count);
	
	result.Reset();
	_LIT(KSearchString2, "TEST");
	TBufC<300> searchString2(KSearchString2);

	//searchQuery will be deleted by messaging client
	CMsvSearchSortQuery* searchQuery2 = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery2);
	
	searchQuery2->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery2->SetResultType(EMsvResultAsTMsvId);
	searchQuery2->SetWholeWord(EFalse);
	searchQuery2->SetCaseSensitiveOption(ETrue);
	searchQuery2->SetWildCardSearch(ETrue);
	
	
	TRAP(error, searchQuery2->AddSearchOptionL(EMsvDescription, searchString2, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	CleanupStack::Pop(searchQuery2);
		
	//RArray<TMsvId> result;
	search->RequestL(searchQuery2, ETrue, iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();

	TRAPD(ret2, search->GetResultsL(result));
	if(ret2 == KErrNone)
		{
		count = result.Count();	
		}
	TInt queryId2 = search->GetQueryIdL();
	INFO_PRINTF2(KFormatId,queryId2);
	
	result.Reset();
	_LIT(KSearchString3, "Hello");
	TBufC<300> searchString3(KSearchString3);

	//searchQuery will be deleted by messaging client
	CMsvSearchSortQuery* searchQuery3 = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery3);
	
	searchQuery3->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery3->SetResultType(EMsvResultAsTMsvId);
	searchQuery3->SetWholeWord(EFalse);
	searchQuery3->SetCaseSensitiveOption(EFalse);
	searchQuery3->SetWildCardSearch(EFalse);
	
	
	TRAP(error, searchQuery3->AddSearchOptionL(EMsvDescription, searchString3, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	CleanupStack::Pop(searchQuery3);
		
	//RArray<TMsvId> result;
	search->RequestL(searchQuery3, ETrue, iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();

	TRAPD(ret3, search->GetResultsL(result));
	if(ret3 == KErrNone)
		{
		count = result.Count();	
		}
	TInt queryId3 = search->GetQueryIdL();
	INFO_PRINTF2(KFormatId,queryId3);
	
	result.Reset();
	_LIT(KSearchString4, "world");
	TBufC<300> searchString4(KSearchString4);

	//searchQuery will be deleted by messaging client
	CMsvSearchSortQuery* searchQuery4 = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery4);
	
	searchQuery4->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery4->SetResultType(EMsvResultAsTMsvId);
	searchQuery4->SetWholeWord(EFalse);
	searchQuery4->SetCaseSensitiveOption(EFalse);
	searchQuery4->SetWildCardSearch(EFalse);
	
	
	TRAP(error, searchQuery4->AddSearchOptionL(EMsvDescription, searchString4, EMsvEqual));
	ASSERT_EQUALS(error, KErrNone);
	
	CleanupStack::Pop(searchQuery4);
		
	search->RequestL(searchQuery4, ETrue, iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();

	TRAPD(ret4, search->GetResultsL(result));
	if(ret4 == KErrNone)
		{
		count = result.Count();	
		}

	result.Close();
	entryResults.Close();
	testt.Close();
	CleanupStack::PopAndDestroy(5); // search, cEntry, session, ob
	}

void CTestSearchSortOperation::TearDownL()
	{
	_LIT(KTearDownL, "TearDownL");
	INFO_PRINTF1(KTearDownL);
	delete iActiveWaiter;
	}

CTestSuite* CTestSearchSortOperation::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(SeachSortRequestL);
	ADD_ASYNC_TEST_STEP(CreateValidQueryOneL);
	ADD_ASYNC_TEST_STEP(CreateValidQueryTwoL);
	ADD_ASYNC_TEST_STEP(CreateValidQueryThreeL);
	ADD_ASYNC_TEST_STEP(CreateSearchQueryWithMaxLevelsL);
	ADD_ASYNC_TEST_STEP(CreateQueryWithInvalidDateL);
	ADD_ASYNC_TEST_STEP(CreateQueryWithInvalidSizeL);
	ADD_ASYNC_TEST_STEP(CreateTooBigQueryL);
	ADD_ASYNC_TEST_STEP(CreateQueryForMultilevelSortL);
	ADD_ASYNC_TEST_STEP(CreateSortQueryWithSubfolderOptionL);
	ADD_ASYNC_TEST_STEP(CreateQueryWithAttachmentTypeL);
	ADD_ASYNC_TEST_STEP(CreateQueryUsingNewLcL);
	ADD_ASYNC_TEST_STEP(CreateSearchQueryWithInvalidQueryOptionL);
	END_SUITE;
	}
