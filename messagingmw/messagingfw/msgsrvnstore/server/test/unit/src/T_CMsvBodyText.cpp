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
// To build: bldmake bldfiles
// abld test build wins udeb
// To run from windows command prompt:
// cd epoc32\release\wins\udeb 
// T_CMSVBODYTEXT -dtextshell --
// 
//
 

#include <e32cons.h>
#include <e32test.h>
#include <s32file.h>
#include <txtrich.h>	// CRichText
#include <charconv.h>   // CCnvCharacterSetConverter, SCharacterSet

// Message Server headers for linking to msgs.dll. 
#include "msvapi.h"
#include "msvids.h"
#include "msvuids.h"

// Class under test.
#include "CMsvBodyText.h"

// Test utility classes
class CMyScheduler : public CActiveScheduler
	{
private:
	void Error(TInt aError) const;
	};
void CMyScheduler::Error(TInt /*aError*/) const {}


class CObserver : public MMsvSessionObserver
	{
	public:
		void HandleSessionEvent(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
		void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	};
void CObserver::HandleSessionEvent(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/) {}
void CObserver::HandleSessionEventL(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)	{}


// Test Case Prototypes
LOCAL_C TBool TestConstructionL();
LOCAL_C TBool TestSetCharacterSetL();
LOCAL_C TBool TestSetDefaultSetL();
LOCAL_C TBool TestStoreL();
LOCAL_C TBool TestRestoreL();
LOCAL_C TBool TestGetBodyTextL();
LOCAL_C TBool TestRemoveAndIsPresentL();

// Test Harness Prototypes
LOCAL_C void InitL();
LOCAL_C void UninitL();
LOCAL_C void doTest();
LOCAL_C void doMainL();
GLDEF_C TInt E32Main();


// Test harness constants and global variables...
_LIT(KTitle, "T_CMSVBODYTEXT");
_LIT(KTxtPressAnyKey, "Press any key to return.");
CTrapCleanup* pcleanup = NULL;
CMyScheduler* pscheduler = NULL;
RTest test(KTitle);
TMsvId gTestEntryId;
RFs gFs;
CMsvSession* pSession = NULL;
CObserver* pObserver = NULL;
CMsvEntry* pContext = NULL;


// Constants used for testing CMsvBodyText...
_LIT8(KDefChar, "?");
const TUint KCharSetUid = 111;
const TUint KDefCharSetUid = 999;
_LIT8(K10BytesData, "0123456789");  // 10 bytes.

LOCAL_C TBool TestConstructionL()
	{
	// Retrieve the store from the current context... 
	CMsvStore* store = pContext->EditStoreL();
	CleanupStack::PushL(store);
	
	// Test construction via NewL().
	__UHEAP_MARK;
	CMsvBodyText* obj = NULL;
	obj = CMsvBodyText::NewL();
	test(obj != NULL);
	delete obj;
	obj = NULL;
	__UHEAP_MARKEND;
	
	// Test for memory leaks during heap cell failure at specific allocations.
	TInt failureRate = 0;
	while (failureRate < 32)
		{
		__UHEAP_MARK;
		__UHEAP_SETFAIL(RHeap::EDeterministic, failureRate++);
		TRAPD(err, obj = CMsvBodyText::NewL());
		__UHEAP_RESET;
		if (err == KErrNone)
			{
			delete obj;
			obj = NULL;
			}
		else
			{
			test(err == KErrNoMemory);
			}
		__UHEAP_MARKEND;
		}

	// Test construction via NewLC().
	__UHEAP_MARK;
	obj = CMsvBodyText::NewLC();
	test(obj != NULL);
	CleanupStack::PopAndDestroy();
	obj = NULL;
	__UHEAP_MARKEND;
	
	// Test for memory leaks during heap cell failure at specific allocations.
	failureRate = 0;
	while (failureRate < 32)
		{
		__UHEAP_MARK;
		__UHEAP_SETFAIL(RHeap::EDeterministic, failureRate++);
		TRAPD(err, 
			obj = CMsvBodyText::NewLC();
			CleanupStack::Pop());
		__UHEAP_RESET;
		if (err == KErrNone)
			{
			delete obj;
			obj = NULL;
			}
		else
			{
			test(err == KErrNoMemory);
			test(failureRate<32);
			}
		__UHEAP_MARKEND;
		}
	
	CleanupStack::PopAndDestroy(store);
	return ETrue;
	}


LOCAL_C TBool TestSetCharacterSetL()
	{
	// Retrieve the store from the current context... 
	CMsvStore* store = pContext->EditStoreL();
	CleanupStack::PushL(store);
	
	__UHEAP_MARK;
	CMsvBodyText* obj = CMsvBodyText::NewLC();
	
	// Loop in increments of 256 'till the maximum TUint value
	// is exceeded and it wraps back to zero.
	TUint i = 256;
	for(i; i > 0; i+=256)
		{
		obj->SetCharacterSet(i);
		test(obj->CharacterSet() == i);
		}
		
	CleanupStack::PopAndDestroy(obj);
	__UHEAP_MARKEND;

	CleanupStack::PopAndDestroy(store);

	return ETrue;
	}


LOCAL_C TBool TestSetDefaultCharacterSetL()
	{
	// Retrieve the store from the current context... 
	CMsvStore* store = pContext->EditStoreL();
	CleanupStack::PushL(store);

	__UHEAP_MARK;
	CMsvBodyText* obj = CMsvBodyText::NewLC();
	
	// Loop in increments of 256 'till the maximum TUint value
	// is exceeded and it wraps back to zero.
	TUint i = 256;
	for(i; i > 0; i+=256)
		{
		obj->SetDefaultCharacterSet(i);
		test(obj->DefaultCharacterSet() == i);
		}

	CleanupStack::PopAndDestroy(obj);
	__UHEAP_MARKEND;

	CleanupStack::PopAndDestroy(store);

	return ETrue;
	}


LOCAL_C TBool TestStoreL()
	{
	__UHEAP_MARK;
	// Retrieve the store from the current context... 
	CMsvStore* store = pContext->EditStoreL();
	CleanupStack::PushL(store);
	CMsvBodyText* obj = CMsvBodyText::NewLC();
	obj->SetCharacterSet(KCharSetUid);
	obj->SetDefaultCharacterSet(KDefCharSetUid);
	// Add 10 bytes of data...
	CBufFlat *buffer=CBufFlat::NewL(K10BytesData().Size());
	CleanupStack::PushL(buffer);
	buffer->InsertL(0,K10BytesData);
	obj->StoreL(*store, *buffer);
	CleanupStack::PopAndDestroy(buffer);
	store->CommitL();
	CleanupStack::PopAndDestroy(obj);
	obj = CMsvBodyText::NewLC();
	obj->SetCharacterSet(0);
	obj->SetDefaultCharacterSet(0);
	obj->RestoreL(*store);
	test(obj->CharacterSet() == KCharSetUid);
	test(obj->DefaultCharacterSet() == KDefCharSetUid);
	CleanupStack::PopAndDestroy(obj);
	CleanupStack::PopAndDestroy(store);
	__UHEAP_MARKEND;

	__UHEAP_MARK;
	// Retrieve the store from the current context... 
	store = pContext->EditStoreL();
	CleanupStack::PushL(store);
	obj = CMsvBodyText::NewLC();
	obj->SetCharacterSet(KCharSetUid);
	obj->SetDefaultCharacterSet(KDefCharSetUid);
	// Add no data - just store character sets...
	obj->StoreL(*store);
	store->CommitL();
	CleanupStack::PopAndDestroy(obj);
	obj = CMsvBodyText::NewLC();
	obj->SetCharacterSet(0);
	obj->SetDefaultCharacterSet(0);
	obj->RestoreL(*store);
	test(obj->CharacterSet() == KCharSetUid);
	test(obj->DefaultCharacterSet() == KDefCharSetUid);
	CleanupStack::PopAndDestroy(obj);
	CleanupStack::PopAndDestroy(store);
	__UHEAP_MARKEND;

	__UHEAP_MARK;
	// Retrieve the store from the current context in read-only mode... 
	store = pContext->ReadStoreL();
	CleanupStack::PushL(store);
	obj = CMsvBodyText::NewLC();
	obj->SetCharacterSet(KCharSetUid);
	obj->SetDefaultCharacterSet(KDefCharSetUid);
	// Add 10 bytes of data...

	CBufFlat *buffer2=CBufFlat::NewL(K10BytesData().Size());
	CleanupStack::PushL(buffer2);
	buffer2->InsertL(0,K10BytesData);
	TRAPD(err, obj->StoreL(*store, *buffer2));
	test(err == KErrAccessDenied);
	CleanupStack::PopAndDestroy(buffer2);
	CleanupStack::PopAndDestroy(obj);
	CleanupStack::PopAndDestroy(store);
	__UHEAP_MARKEND;

	return ETrue;
	}
	
	
LOCAL_C TBool TestRestoreL()
	{
	// Retrieve the store from the current context... 
	CMsvStore* store = pContext->EditStoreL();
	CleanupStack::PushL(store);
	
	__UHEAP_MARK;
	// Populate CMsvBodyText from the data in the store...
	CMsvBodyText* obj = CMsvBodyText::NewLC();
	obj->RestoreL(*store);

	// Clean up and release resources...
	CleanupStack::PopAndDestroy(obj);
	__UHEAP_MARKEND;

	CleanupStack::PopAndDestroy(store);
		
	return ETrue;
	}


LOCAL_C TBool TestGetBodyTextL()
	{
	__UHEAP_MARK;
	// Retrieve the store from the current context... 
	CMsvStore* store = pContext->EditStoreL();
	CleanupStack::PushL(store);
	
	// Populate CMsvBodyText from the data in the store...
	CMsvBodyText* obj = CMsvBodyText::NewLC();
	obj->RestoreL(*store);
	
	// Create a rich text object...
	CParaFormatLayer* paraLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraLayer);
	CCharFormatLayer* charLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charLayer);
	CRichText* richText = CRichText::NewL(paraLayer, charLayer);
	CleanupStack::PushL(richText);
	
	// Create an array of available character sets...
	CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* charSets;
	CCnvCharacterSetConverter* converter = CCnvCharacterSetConverter::NewL();
	CleanupStack::PushL(converter);
	charSets = converter->CreateArrayOfCharacterSetsAvailableLC(gFs);
	
	// For each available character set, call GetBodyTextL...
	TInt i;
	for (i = 0; i < charSets->Count(); i++)
		{
		// Over-ride the stored character set.
		obj->SetCharacterSet((*charSets)[i].Identifier());
		// Convert the 8 bit data to the specified character set.
		obj->GetBodyTextL(gFs, *store, *richText);
		test(richText->HasChanged());
		richText->Reset();
		}
	
	// Instead of decoding 10 bytes of data, increase the amount of data to 4000 bytes.
	obj->SetCharacterSet(0);
	obj->SetDefaultCharacterSet((*charSets)[0].Identifier());
	const TInt KTextBufferSize=4000;
	CBufBase *buffer=CBufFlat::NewL(KTextBufferSize);
	CleanupStack::PushL(buffer);
	while(buffer->Size()<KTextBufferSize)
		{
		buffer->InsertL(buffer->Size(),K10BytesData);
		}

	obj->StoreL(*store, *buffer);
	CleanupStack::PopAndDestroy(buffer);
	store->CommitL();
	obj->RestoreL(*store);
	obj->GetBodyTextL(gFs, *store, *richText);
	test(richText->HasChanged());
	
	// Clean up and release resources...
	CleanupStack::PopAndDestroy(charSets);
	CleanupStack::PopAndDestroy(converter);
	CleanupStack::PopAndDestroy(richText);
	CleanupStack::PopAndDestroy(charLayer);
	CleanupStack::PopAndDestroy(paraLayer);
	CleanupStack::PopAndDestroy(obj);
	CleanupStack::PopAndDestroy(store);
	__UHEAP_MARKEND;
		
	return ETrue;
	}


LOCAL_C TBool TestRemoveAndIsPresentL()
	{
	// Retrieve the store from the current context... 
	CMsvStore* store = pContext->EditStoreL();
	CleanupStack::PushL(store);
	
	__UHEAP_MARK;
	// Populate CMsvBodyText from the data in the store...
	CMsvBodyText* obj = CMsvBodyText::NewLC();
	obj->RestoreL(*store);
	test(obj->IsPresentL(*store));
	obj->RemoveL(*store);
	test(obj->IsPresentL(*store) == EFalse);
	obj->RemoveL(*store); // Should not leave if there is no data to remove.
	// Clean up and release resources...
	CleanupStack::PopAndDestroy(obj);
	__UHEAP_MARKEND;

	CleanupStack::PopAndDestroy(store);

	// Retrieve the store from the current context in read-only mode... 
	store = pContext->ReadStoreL();
	CleanupStack::PushL(store);

	__UHEAP_MARK;
	// Populate CMsvBodyText from the data in the store...
	obj = CMsvBodyText::NewLC();
	TRAPD(err, obj->RemoveL(*store));
	test(err == KErrAccessDenied);
	// Clean up and release resources...
	CleanupStack::PopAndDestroy(obj);
	__UHEAP_MARKEND;

	CleanupStack::PopAndDestroy(store);
	return ETrue;
	}


LOCAL_C void InitL()
	{
	// Connect to the file system...
	gFs.Connect();

	// Connect to the Message Server...
	pObserver = new (ELeave) CObserver();
	CleanupStack::PushL(pObserver);
	pSession = CMsvSession::OpenSyncL(*pObserver);
	CleanupStack::PushL(pSession);
	
	// Create a test entry with some dummy values so the Message Server
	// will not panic when the entry is created in the message store...
	pContext = CMsvEntry::NewL(*pSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(pContext);
	TMsvEntry testEntry;
	TUid entryMtm;
	entryMtm.iUid = 11;
	testEntry.iMtm = entryMtm;
	testEntry.iServiceId = pContext->OwningService();
	testEntry.iType = KUidMsvMessageEntry;
	
	// Create the entry in the message store and set the current context to it...
	pContext->CreateL(testEntry);
	gTestEntryId = testEntry.Id();
	pContext->SetEntryL(gTestEntryId);
	}


LOCAL_C void UninitL()
	{
	pContext->DeleteL(gTestEntryId);
	CleanupStack::PopAndDestroy(pContext);
	CleanupStack::PopAndDestroy(pSession);
	CleanupStack::PopAndDestroy(pObserver);
	gFs.Close();
	}


LOCAL_C void doTest()
	{
	test.Start(_L("Testing construction/destruction"));
	test(TestConstructionL());
	test.Next(_L("Testing SetCharacterSet method"));
	test(TestSetCharacterSetL());
	test.Next(_L("Testing SetDefaultCharacterSet method"));
	test(TestSetDefaultCharacterSetL());
	test.Next(_L("Testing StoreL method"));
	test(TestStoreL());
	test.Next(_L("Testing RestoreL method"));
	test(TestRestoreL());
	test.Next(_L("Testing GetBodyTextL method"));
	test(TestGetBodyTextL());
	test.Next(_L("Testing IsPresentL method when stream is present and removed"));
	test(TestRemoveAndIsPresentL());
	test.End();
	}


LOCAL_C void doMainL()
	{
	InitL();
	doTest();
	test.Getch();
	UninitL();
	}


GLDEF_C TInt E32Main()
	{
	pcleanup = CTrapCleanup::New();
	pscheduler = new (ELeave) CMyScheduler();
	CActiveScheduler::Install(pscheduler);
	TRAPD(ret,doMainL());		
	delete pcleanup;
	return(KErrNone);
	}
