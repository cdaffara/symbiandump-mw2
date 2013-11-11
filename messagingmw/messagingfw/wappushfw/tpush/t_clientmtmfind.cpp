// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <mtclreg.h>
#include <msvids.h>
#include <msvuids.h>
#include <txtrich.h>
#include <mtmdef.h>
#include <pushentry.h>
#include "pushtests.h"
#include "pushclientmtm.h"

/*------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Test Procedure:
 Proper behaviour is that a search for a string returns the parts of the message
 in which it resides. Current parts to search are Body, Originator and 
 Description.

//------------------------------------------------------------------------------
//----------------------------------------------------------------------------*/

const TInt KPushTestBuffer = 128;

_LIT(KTest0, "Test0");
_LIT(KTest1, "Test1");
_LIT(KTest2, "Test2");
_LIT(KTest3, "Test3");
_LIT(KTest4, "Test4");
_LIT(KTest5, "Test5");
_LIT(KTest6, "Test6");
_LIT(KTest7, "Test7");
_LIT(KTest8, "Test8");
_LIT(KLogSucceeded, "OK");
_LIT(KLogFailed, "FAILED");
_LIT(KLogOutput,"WapPushClientMtm %S %S");
_LIT(KSpace, " ");

_LIT(KMTMDataFullName,"z:\\system\\data\\wappushmtm.rsc");

//------------------------------------------------------------------------------
void CWapClientMtmFindTest::RunL()
	{
	iDummyObserver = new(ELeave) CDummyObserver;
	iSession = CMsvSession::OpenSyncL(*iDummyObserver);
	// Install the mtm group if necessary
	TInt returnVal = iSession->InstallMtmGroup(KMTMDataFullName);
	if (returnVal != KErrNone && returnVal != KErrAlreadyExists)
		User::Leave(returnVal);

	CBaseMtm* wapPushClientMtm = NULL;
	CClientMtmRegistry* mtmRegistry = CClientMtmRegistry::NewL(*iSession);
	CleanupStack::PushL(mtmRegistry);

	wapPushClientMtm = mtmRegistry->NewMtmL(KUidMtmWapPush);
	CleanupStack::PushL(wapPushClientMtm);

	CMsvEntry* entry = CMsvEntry::NewL(*iSession, KMsvRootIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	SetupFindDataL(entry);

	entry->SetEntryL(iMessageId);
	CleanupStack::Pop(entry);
	wapPushClientMtm->SetCurrentEntryL(entry);

	// restore the message
	entry->SetEntryL(iMessageId);
	CMsvStore* msvStore1 = entry->ReadStoreL();
	CleanupStack::PushL(msvStore1);
	msvStore1->RestoreBodyTextL(wapPushClientMtm->Body());
	CleanupStack::PopAndDestroy(msvStore1);

	PerformTests(wapPushClientMtm);
	
	// Delete the entry
	entry->SetEntryL(iMessageId);
	CMsvStore* msvStore2 = entry->EditStoreL();
	CleanupStack::PushL(msvStore2);
	msvStore2->DeleteBodyTextL();
	msvStore2->CommitL();
	CleanupStack::PopAndDestroy(msvStore2);

	// Install the mtm group if necessary
	returnVal = iSession->DeInstallMtmGroup(KMTMDataFullName);

	CleanupStack::PopAndDestroy(2, mtmRegistry); // wapPushClientMtm, mtmRegistry
	entry = NULL; // Removed by mtm being destroyed
	}

//------------------------------------------------------------------------------
void CWapClientMtmFindTest::SetupFindDataL(CMsvEntry* aEntry)
	{
	/*
	* Test method. Currently 3 areas are searched for when doing a find:
	* Body, details (originator) and descriptor. Find returns a value of OR'd
	* flags representing the areas in which the given text was found. To test
	* this properly we want to test all combinations. 2^3 = 8 tests. A '1'
	* represents the existence of the searched-for text in the repective
	* location.
	* 
	* Test  Body Dtls Desc
	*
	* Test0    0    0    0
	* Test1    0    0    1
	* Test2    0    1    0
	* Test3    0    1    1
	* Test4    1    0    0
	* Test5    1    0    1
	* Test6    1    1    0
	* Test7    1    1    1
	*
	* Test 8 repeats test 7 with a leave
	*/

	TMsvEntry tmsvEntry;

	HBufC* descriptorContents = HBufC::NewLC(KPushTestBuffer);
	HBufC* originatorContents = HBufC::NewLC(KPushTestBuffer);
	HBufC* bodyContents = HBufC::NewLC(KPushTestBuffer);
	TPtr contentPtr(descriptorContents->Des());

	tmsvEntry.SetComplete(EFalse);
	tmsvEntry.SetReadOnly(EFalse);
	tmsvEntry.iMtm       = KUidMtmWapPush;
	tmsvEntry.iType      = KUidMsvMessageEntry;
	tmsvEntry.iServiceId = KMsvLocalServiceIndexEntryId;

	// Set the descriptor for description contents.
	contentPtr.Append(KTest1);
	contentPtr.Append(KSpace);
	contentPtr.Append(KTest3);
	contentPtr.Append(KSpace);
	contentPtr.Append(KTest5);
	contentPtr.Append(KSpace);
	contentPtr.Append(KTest7);
	tmsvEntry.iDescription.Set(contentPtr);

	// Set the details contents for details contents.
	contentPtr.Set(originatorContents->Des());
	contentPtr.Append(KTest2);
	contentPtr.Append(KSpace);
	contentPtr.Append(KTest3);
	contentPtr.Append(KSpace);
	contentPtr.Append(KTest6);
	contentPtr.Append(KSpace);
	contentPtr.Append(KTest7);
	tmsvEntry.iDetails.Set(contentPtr);

	// Set the descriptor contents for body contents.
	contentPtr.Set(bodyContents->Des());
	contentPtr.Append(KTest4);
	contentPtr.Append(KSpace);
	contentPtr.Append(KTest5);
	contentPtr.Append(KSpace);
	contentPtr.Append(KTest6);
	contentPtr.Append(KSpace);
	contentPtr.Append(KTest7);

	// Put an entry in a known location - the outbox
	aEntry->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	aEntry->CreateL(tmsvEntry);
	iSession->CleanupEntryPushL(tmsvEntry.Id());
	iMessageId = tmsvEntry.Id();
	aEntry->SetEntryL(iMessageId);

	CParaFormatLayer* paraFormatLayer=CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer=CCharFormatLayer::NewL(); 
	CleanupStack::PushL(charFormatLayer);

	CRichText* bodyText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(bodyText);
	bodyText->InsertL(0, contentPtr);

	CMsvStore* msvStore = aEntry->EditStoreL();
	CleanupStack::PushL(msvStore);
	msvStore->StoreBodyTextL(*bodyText);
	msvStore->CommitL();
	
	tmsvEntry.SetReadOnly(EFalse);
	tmsvEntry.SetVisible(ETrue);
	tmsvEntry.SetInPreparation(EFalse);
	aEntry->ChangeL(tmsvEntry);
	
	CleanupStack::PopAndDestroy(4, paraFormatLayer); // msvStore, bodyText, charFormatLayer, paraFormatLayer
	iSession->CleanupEntryPop();
	CleanupStack::PopAndDestroy(3, descriptorContents); // bodyContents, originiatorContents, descriptorContents
	}

//------------------------------------------------------------------------------
void CWapClientMtmFindTest::PerformTests(CBaseMtm* aWapPushClientMtm)
	{
	TMsvPartList noWhere = 0;
	TMsvPartList allParts = KMsvMessagePartBody | 
		KMsvMessagePartOriginator | 
		KMsvMessagePartDescription;
	TMsvPartList originatorAndDescription = 
		KMsvMessagePartOriginator | KMsvMessagePartDescription;
	TMsvPartList bodyAndDescription = 
		KMsvMessagePartBody | KMsvMessagePartDescription;
	TMsvPartList bodyAndOriginator =
		KMsvMessagePartBody | KMsvMessagePartOriginator;

	TestAndPrint(aWapPushClientMtm, KTest0, noWhere);
	TestAndPrint(aWapPushClientMtm, KTest1, KMsvMessagePartDescription);
	TestAndPrint(aWapPushClientMtm, KTest2, KMsvMessagePartOriginator);
	TestAndPrint(aWapPushClientMtm, KTest3, originatorAndDescription);
	TestAndPrint(aWapPushClientMtm, KTest4, KMsvMessagePartBody);
	TestAndPrint(aWapPushClientMtm, KTest5, bodyAndDescription);
	TestAndPrint(aWapPushClientMtm, KTest6, bodyAndOriginator);
	TestAndPrint(aWapPushClientMtm, KTest7, allParts);

	// Test8 : perform Test7 again, but with a memory allocation failure.
#if defined(_DEBUG)
	TInt fail=0;
#endif
	TInt err = KErrNone;

	__UHEAP_FAILNEXT(fail);
	__UHEAP_MARK;
	TRAP(err, aWapPushClientMtm->Find(KTest7, allParts));
	__UHEAP_RESET;
	__UHEAP_MARKEND;

	TBuf<KPushLogBuffer> buffer;
	if (err == KErrNone)
		{
		buffer.Format(KLogOutput, &KTest8, &KLogSucceeded);
		}
	else
		{
		buffer.Format(KLogOutput, &KTest8, &KLogFailed);
		}
	WPLPrintf(buffer);

	iEngine->TestCompleted(iStatus.Int());
	}

//------------------------------------------------------------------------------
void CWapClientMtmFindTest::TestAndPrint(CBaseMtm* aWapPushClientMtm, const TDesC& aTestText, TMsvPartList aResultList)
	{
	TBuf<KPushLogBuffer> buffer;
	TMsvPartList allParts = KMsvMessagePartBody | 
		KMsvMessagePartOriginator | 
		KMsvMessagePartDescription;

	if (aWapPushClientMtm->Find(aTestText, allParts) == aResultList)
		{
		buffer.Format(KLogOutput, &aTestText, &KLogSucceeded);
		}
	else
		{
		buffer.Format(KLogOutput, &aTestText, &KLogFailed);
		}
	WPLPrintf(buffer);
	}

//------------------------------------------------------------------------------
void CWapClientMtmFindTest::DoCancel()
	{
	iEngine->TestCompleted(KErrCancel);
	}

//------------------------------------------------------------------------------
TInt CWapClientMtmFindTest::RunError(TInt aError)
	{
	TBuf<80> buf;
	_LIT(KRunError, "CWapClientMtmFindTest::RunError");
	buf.Format(KRunError, aError);
	iEngine->Printf(KRunError,TestName());
	iEngine->TestCompleted(aError);
	return KErrNone;
	}

//------------------------------------------------------------------------------
const TDesC& CWapClientMtmFindTest::TestName()
	{
	_LIT(KTextWapClientMtmFindTest, "WapPushClientMtm Find Test");
	return KTextWapClientMtmFindTest;
	}

//------------------------------------------------------------------------------
CWapClientMtmFindTest::~CWapClientMtmFindTest()
	{
	delete iDummyObserver; 
	delete iSession;
	}
