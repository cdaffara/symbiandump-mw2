// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "t_plaintextstore.h"
#include "cactivewaiter.h"
#include <cmsvplainbodytext.h>
#include <charconv.h>
#include <txtfmlyr.h>

class CActiveWaiter;

const TInt KChunkLength = 100;
RTest test(KPlainBodyTextTest);

class CMsvPlainBodyText;
	
CTestPlainTextStore::CTestPlainTextStore()
	{
	iPlainBodyTextStore= NULL;
	iMsvServer = NULL;
	iActiveWaiter = NULL;
	}
	

/**
Initialises the tests.
*/	
void CTestPlainTextStore::SetupL()
	{
	//clear the messaging folder
	CEmailTestUtils* testUtils;
	testUtils = CEmailTestUtils::NewL(test);
	CleanupStack::PushL(testUtils);
	testUtils->FileSession().SetSessionPath(KCDrive);
	testUtils->CleanMessageFolderL();
	testUtils->ClearEmailAccountsL();
	CleanupStack::PopAndDestroy(testUtils);
	iActiveWaiter = new (ELeave) CActiveWaiter();
	
	TBuf<KMaxFilePathLength>emailbodyfile(KEmail40kBodyFile);
	RFs fsSession;
	RFile infile; 
		
	User::LeaveIfError(fsSession.Connect()); 
 	User::LeaveIfError(infile.Open(fsSession, emailbodyfile, EFileRead));

	infile.Size(iSize);
 
 	//read the 8 bit data from the file
	iFilebuf1 = HBufC8::NewL(iSize+KTestStringLength);  
	TPtr8 ptr1=iFilebuf1->Des();	  
	infile.Read(ptr1);
	
	//read the 2nd chunk
	iFilebuf2 = HBufC8::NewL(iSize+KTestStringLength);  
	TPtr8 ptr2=iFilebuf2->Des();	  
	ptr2.Copy(ptr1);
	
	//read the 3rd chunk
	iFilebuf3 = HBufC8::NewL(iSize+KTestStringLength);  
	TPtr8 ptr3=iFilebuf3->Des();	  
	ptr3.Copy(ptr1);
	
	//we dont need to read from the file anymore
	infile.Close();
	
	//create a CMsvServer
	TRAPD(err, iMsvServer = CMsvServer::NewL());

	// wait a couple of seconds
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(5000000);
	CActiveScheduler::Start();
	delete timer;
	
	//create a CMsvServerEntry
	iServerEntry = CMsvServerEntry::NewL(*iMsvServer, KMsvRootIndexEntryId);

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsgTypeIMAP4;
	iServerEntry->SetEntry(KMsvGlobalOutBoxIndexEntryIdValue);
	iServerEntry->CreateEntry(entry1);
	iServerEntry->SetEntry(entry1.Id());
	iMsvMessageEntry=entry1.Id();
	
	//folder entry
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	iServerEntry->SetEntry(iMsvMessageEntry);
	iServerEntry->CreateEntry(folder);
	iServerEntry->SetEntry(folder.Id());
	iFolderId=folder.Id();
	
	TMsvEntry bodyentry;
	bodyentry.iType = KUidMsvEmailTextEntry;
	bodyentry.iServiceId = KMsvLocalServiceIndexEntryId;
	bodyentry.iMtm = KUidMsgTypeIMAP4;
	bodyentry.iDetails.Set(KBodyDetails);
	iServerEntry->SetEntry(iFolderId);
	iServerEntry->CreateEntry(bodyentry);
	iServerEntry->SetEntry(bodyentry.Id());
	iBodyMessageEntry=bodyentry.Id();
		
	// Get the default charset
	CCnvCharacterSetConverter* characterConverter = CCnvCharacterSetConverter::NewL();
	CleanupStack::PushL(characterConverter);
	CImConvertCharconv* charConv = CImConvertCharconv::NewL(*characterConverter,fsSession );
	CleanupStack::PushL(charConv);

	iDefaultCharset =	charConv->SystemDefaultCharset();
	CleanupStack::PopAndDestroy(2);
	fsSession.Close();
	}


void CTestPlainTextStore::TearDownL()
	{
	delete iServerEntry;
	delete iMsvServer;
	delete iFilebuf1;
	delete iFilebuf2;
	delete iFilebuf3;
	test.Close();
	}

void CTestPlainTextStore::TestStoreRestore8BitChunkAsyncL()
	{
	iServerEntry->SetEntry(iBodyMessageEntry);
	CMsvStore* store = iServerEntry->EditStoreL();
	iPlainBodyTextStore = store->InitialisePlainBodyTextForWriteL(1,iDefaultCharset,iDefaultCharset);
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf1->Des(),iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf2->Des(),iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf3->Des(),iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	TInt sizeOfText =	iPlainBodyTextStore->Size();
	iPlainBodyTextStore->CommitL();	

	delete iPlainBodyTextStore;
	delete store;
	
	//Restore the data in chunks.	
	HBufC8* filebuf11 = HBufC8::NewL(iSize+KTestStringLength + 2);

	CMsvStore* store1 = iServerEntry->ReadStoreL();
	iPlainBodyTextStore = store1->InitialisePlainBodyTextForReadL(KChunkLength);
	TInt origSize = sizeOfText;
	TInt sizeOfRestoredText = 0;

	TPtr8 bufPtr = filebuf11->Des();
	TInt pos = 0;
	
	iPlainBodyTextStore->NextChunkL(bufPtr , iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	RArray<TPtr8> chunkArray;
	chunkArray.Append(bufPtr);
	
	sizeOfRestoredText += bufPtr.Length();
	
	while(bufPtr.Length() > 0 && bufPtr.Length() == KChunkLength)
		{
		iPlainBodyTextStore->NextChunkL(bufPtr, iActiveWaiter->iStatus);
		iActiveWaiter->WaitActive();
		// put retrived buffer in a array.
		chunkArray.Append(bufPtr);
		sizeOfRestoredText += bufPtr.Length();
		}

	ASSERT_EQUALS(origSize, sizeOfRestoredText);

	for(TInt j=chunkArray.Count() - 1;j >0; --j )
		{
		bufPtr.SetLength(0);
		iPlainBodyTextStore->PreviousChunkL(bufPtr, iActiveWaiter->iStatus);
		iActiveWaiter->WaitActive();
		// Compare chunk retrieved with chunk retrieved by NextChunkL
		TInt comp = bufPtr.Compare(chunkArray[j-1]);
		ASSERT_EQUALS(comp, 0);
		}

	delete filebuf11;
	delete iPlainBodyTextStore;
	delete store1;
	}


void CTestPlainTextStore::TestStoreRestore8BitChunkSynchL()
	{
	iServerEntry->SetEntry(iBodyMessageEntry);
	CMsvStore* store = iServerEntry->EditStoreL();
	iPlainBodyTextStore = store->InitialisePlainBodyTextForWriteL(1,iDefaultCharset,iDefaultCharset);
	
	TRAPD(err, iPlainBodyTextStore->StoreChunkL(iFilebuf1->Des()));
		
	iPlainBodyTextStore->StoreChunkL(iFilebuf2->Des());
		
	iPlainBodyTextStore->StoreChunkL(iFilebuf3->Des());
	
	TInt sizeOfText =	iPlainBodyTextStore->Size();
	iPlainBodyTextStore->CommitL();	

	delete iPlainBodyTextStore;
	delete store;
	
	//Restore the data in chunks.	
	HBufC8* filebuf11 = HBufC8::NewL(iSize+KTestStringLength + 2);

	CMsvStore* store1 = iServerEntry->ReadStoreL();
	iPlainBodyTextStore = store1->InitialisePlainBodyTextForReadL(KChunkLength);
	
	TInt origSize = sizeOfText;
	TInt sizeOfRestoredText = 0;

	TPtr8 bufPtr = filebuf11->Des();
	TInt pos = 0;
	
	iPlainBodyTextStore->NextChunkL(bufPtr);
		
	RArray<TPtr8> chunkArray;
	chunkArray.Append(bufPtr);
	
	sizeOfRestoredText += bufPtr.Length();
	
	while(bufPtr.Length() > 0 && bufPtr.Length() == KChunkLength)
		{
		iPlainBodyTextStore->NextChunkL(bufPtr);
		// put retrived buffer in a array.
		chunkArray.Append(bufPtr);
		sizeOfRestoredText += bufPtr.Length();
		}

	ASSERT_EQUALS(origSize, sizeOfRestoredText);

	for(TInt j=chunkArray.Count() - 1;j >0; --j )
		{
		bufPtr.SetLength(0);
		iPlainBodyTextStore->PreviousChunkL(bufPtr);
		// Compare chunk retrieved with chunk retrieved by NextChunkL
		TInt comp = bufPtr.Compare(chunkArray[j-1]);
		ASSERT_EQUALS(comp, 0);
		}

	delete filebuf11;
	delete iPlainBodyTextStore;
	delete store1;
	}
		
void CTestPlainTextStore::TestStoreRestore16BitChunkAsyncL()
	{
	iServerEntry->SetEntry(iBodyMessageEntry);
	CMsvStore* store = iServerEntry->EditStoreL();
	iPlainBodyTextStore = store->InitialisePlainBodyTextForWriteL(0,iDefaultCharset,iDefaultCharset);
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf1->Des(),iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf2->Des(),iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf3->Des(),iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	TInt sizeOfText =	iPlainBodyTextStore->Size();
	TInt origSize = sizeOfText/2;
	
	iPlainBodyTextStore->CommitL();	

	delete iPlainBodyTextStore;
	delete store;
	
	//Restore the data in chunks.	
	HBufC16* filebuf11 = HBufC::NewL(iSize+KTestStringLength);
	CMsvStore* store1 = iServerEntry->ReadStoreL();
	
	iPlainBodyTextStore = store1->InitialisePlainBodyTextForReadL(KChunkLength);
	TInt sizeOfRestoredText = 0;

	TPtr16 bufPtr = filebuf11->Des();
	TInt pos = 0;
	
	iPlainBodyTextStore->NextChunkL(bufPtr, iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	RArray<TPtr16> chunkArray;
	chunkArray.Append(bufPtr);
	
	sizeOfRestoredText += bufPtr.Length();
	while(bufPtr.Length() > 0 && bufPtr.Length() == KChunkLength)
		{
		iPlainBodyTextStore->NextChunkL(bufPtr, iActiveWaiter->iStatus);
		iActiveWaiter->WaitActive();
		// put retrived buffer in a array.
		chunkArray.Append(bufPtr);
		sizeOfRestoredText += bufPtr.Length();
		}
		
	ASSERT_EQUALS(origSize, sizeOfRestoredText);
	
	for(TInt j=chunkArray.Count() - 1;j >0; --j )
		{
		bufPtr.SetLength(0);
		iPlainBodyTextStore->PreviousChunkL(bufPtr, iActiveWaiter->iStatus);
		iActiveWaiter->WaitActive();
		// Compare chunk retrieved with chunk retrieved by NextChunkL
		TInt comp = bufPtr.Compare(chunkArray[j-1]);
		ASSERT_EQUALS(comp, 0);
		}
	
	delete filebuf11;
	delete iPlainBodyTextStore;
	delete store1;
	}

void CTestPlainTextStore::TestStoreRestore16BitChunkAsynchStoreWith8BitL()
	{
	RFile infile;
	RFs fsSession;
	TBuf<KMaxFilePathLength>emailbodyfile(KEmail40kBodyFile);
	User::LeaveIfError(fsSession.Connect()); 
 	User::LeaveIfError(infile.Open(fsSession, emailbodyfile, EFileRead));

	infile.Size(iSize);
	TPtr8 ptr1=iFilebuf1->Des();	  
	infile.Read(ptr1);
	iFilebuf4 = HBufC16::NewL(iSize+KTestStringLength);  
	TPtr16 ptr4=iFilebuf4->Des();	  
	ptr4.Copy(ptr1);
	infile.Close();
	fsSession.Close();
	iServerEntry->SetEntry(iBodyMessageEntry);
	CMsvStore* store = iServerEntry->EditStoreL();
	CleanupStack::PushL(store);
	iPlainBodyTextStore = store->InitialisePlainBodyTextForWriteL(1,iDefaultCharset,iDefaultCharset);
	TInt error = KErrGeneral;
	TRAP(error, iPlainBodyTextStore->StoreChunkL(iFilebuf4->Des(),iActiveWaiter->iStatus));
	ASSERT_EQUALS(error, KErrNotSupported);
	delete iPlainBodyTextStore;
	CleanupStack::Pop(store);
	delete store;
	}
	
void CTestPlainTextStore::TestStoreRestore16BitChunkSynchL()
	{
	iServerEntry->SetEntry(iBodyMessageEntry);
	CMsvStore* store = iServerEntry->EditStoreL();
	CleanupStack::PushL(store);
	iPlainBodyTextStore = store->InitialisePlainBodyTextForWriteL(0,iDefaultCharset,iDefaultCharset);
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf1->Des());
		
	iPlainBodyTextStore->StoreChunkL(iFilebuf2->Des());
		
	iPlainBodyTextStore->StoreChunkL(iFilebuf3->Des());
		
	TInt sizeOfText =	iPlainBodyTextStore->Size();
	TInt origSize = sizeOfText/2;
	
	iPlainBodyTextStore->CommitL();	

	delete iPlainBodyTextStore;
	CleanupStack::Pop();
	delete store;
	
	//Restore the data in chunks.	
	HBufC16* filebuf11 = HBufC::NewL(iSize+KTestStringLength);
	CMsvStore* store1 = iServerEntry->ReadStoreL();
	CleanupStack::PushL(store1);
	iPlainBodyTextStore = store1->InitialisePlainBodyTextForReadL(KChunkLength);
	TInt sizeOfRestoredText = 0;

	TPtr16 bufPtr = filebuf11->Des();
	TInt pos = 0;
	
	iPlainBodyTextStore->NextChunkL(bufPtr);
		
	RArray<TPtr16> chunkArray;
	chunkArray.Append(bufPtr);
	
	sizeOfRestoredText += bufPtr.Length();
	while(bufPtr.Length() > 0 && bufPtr.Length() == KChunkLength)
		{
		iPlainBodyTextStore->NextChunkL(bufPtr);
		// put retrived buffer in a array.
		chunkArray.Append(bufPtr);
		sizeOfRestoredText += bufPtr.Length();
		}
		
	ASSERT_EQUALS(origSize, sizeOfRestoredText);
	
	for(TInt j=chunkArray.Count() - 1;j >0; --j )
		{
		bufPtr.SetLength(0);
		iPlainBodyTextStore->PreviousChunkL(bufPtr);
		// Compare chunk retrieved with chunk retrieved by NextChunkL
		TInt comp = bufPtr.Compare(chunkArray[j-1]);
		ASSERT_EQUALS(comp, 0);
		}
	
	delete filebuf11;
	delete iPlainBodyTextStore;
	CleanupStack::Pop();
	delete store1;
	}

void CTestPlainTextStore::TestStoreRestore16BitChunkSynchWith8BitL()
	{
	iServerEntry->SetEntry(iBodyMessageEntry);
	CMsvStore* store = iServerEntry->EditStoreL();
	CleanupStack::PushL(store);
	iPlainBodyTextStore = store->InitialisePlainBodyTextForWriteL(1,iDefaultCharset,iDefaultCharset);
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf1->Des());
		
	iPlainBodyTextStore->StoreChunkL(iFilebuf2->Des());
		
	iPlainBodyTextStore->StoreChunkL(iFilebuf3->Des());
		
	TInt sizeOfText =	iPlainBodyTextStore->Size();
	TInt origSize = sizeOfText;
	
	iPlainBodyTextStore->CommitL();	

	delete iPlainBodyTextStore;
	CleanupStack::Pop();
	delete store;
	
	//Restore the data in chunks.	
	HBufC16* filebuf11 = HBufC::NewL(iSize+KTestStringLength);
	CMsvStore* store1 = iServerEntry->ReadStoreL();
	
	iPlainBodyTextStore = store1->InitialisePlainBodyTextForReadL(KChunkLength);
	TInt sizeOfRestoredText = 0;

	TPtr16 bufPtr = filebuf11->Des();
	TInt pos = 0;
	
	iPlainBodyTextStore->NextChunkL(bufPtr);
		
	RArray<TPtr16> chunkArray;
	chunkArray.Append(bufPtr);
	TInt bufferLength = bufPtr.Length();
	sizeOfRestoredText += bufferLength;
	while(bufferLength == KChunkLength)
		{
		iPlainBodyTextStore->NextChunkL(bufPtr);
		// put retrived buffer in a array.
		chunkArray.Append(bufPtr);
		bufferLength = bufPtr.Length();
		sizeOfRestoredText += bufferLength;
		}
		
	ASSERT_EQUALS(origSize, sizeOfRestoredText);
	
	for(TInt j=chunkArray.Count() - 1;j >0; --j )
		{
		bufPtr.SetLength(0);
		iPlainBodyTextStore->PreviousChunkL(bufPtr);
		// Compare chunk retrieved with chunk retrieved by NextChunkL
		TInt comp = bufPtr.Compare(chunkArray[j-1]);
		ASSERT_EQUALS(comp, 0);
		}
	
	delete filebuf11;
	delete iPlainBodyTextStore;
	delete store1;
	}
void CTestPlainTextStore::TestStoreRestore16BitChunkAsyncWith8BitL()
	{
	iServerEntry->SetEntry(iBodyMessageEntry);
	CMsvStore* store = iServerEntry->EditStoreL();
	iPlainBodyTextStore = store->InitialisePlainBodyTextForWriteL(1,iDefaultCharset,iDefaultCharset);
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf1->Des(),iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf2->Des(),iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	iPlainBodyTextStore->StoreChunkL(iFilebuf3->Des(),iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	TInt sizeOfText =	iPlainBodyTextStore->Size();
	TInt origSize = sizeOfText;
	
	iPlainBodyTextStore->CommitL();	

	delete iPlainBodyTextStore;
	delete store;
	
	//Restore the data in chunks.	
	HBufC16* filebuf11 = HBufC::NewL(iSize+KTestStringLength);
	CMsvStore* store1 = iServerEntry->ReadStoreL();
	
	iPlainBodyTextStore = store1->InitialisePlainBodyTextForReadL(KChunkLength);
	TInt sizeOfRestoredText = 0;

	TPtr16 bufPtr = filebuf11->Des();
	TInt pos = 0;
	
	iPlainBodyTextStore->NextChunkL(bufPtr, iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	RArray<TPtr16> chunkArray;
	chunkArray.Append(bufPtr);
	TInt bufferLength = bufPtr.Length();
	sizeOfRestoredText += bufferLength;
	while(bufferLength == KChunkLength)
		{
		iPlainBodyTextStore->NextChunkL(bufPtr, iActiveWaiter->iStatus);
		iActiveWaiter->WaitActive();
		// put retrived buffer in a array.
		chunkArray.Append(bufPtr);
		bufferLength = bufPtr.Length();
		sizeOfRestoredText += bufferLength;
		}
		
	ASSERT_EQUALS(origSize, sizeOfRestoredText);
	
	for(TInt j=chunkArray.Count() - 1;j >0; --j )
		{
		bufPtr.SetLength(0);
		iPlainBodyTextStore->PreviousChunkL(bufPtr, iActiveWaiter->iStatus);
		iActiveWaiter->WaitActive();
		// Compare chunk retrieved with chunk retrieved by NextChunkL
		TInt comp = bufPtr.Compare(chunkArray[j-1]);
		ASSERT_EQUALS(comp, 0);
		}
	
	delete filebuf11;
	delete iPlainBodyTextStore;
	delete store1;
	}
			
void CTestPlainTextStore::TestStoreRichTextAsPlainTextL()
	{	
	iServerEntry->SetEntry(iBodyMessageEntry);
	CMsvStore* store = iServerEntry->EditStoreL();
	
	iPlainBodyTextStore = store->InitialisePlainBodyTextForWriteL(0,iDefaultCharset,iDefaultCharset);

	CParaFormatLayer* paraLayer = CParaFormatLayer::NewL();
	CCharFormatLayer* charLayer = CCharFormatLayer::NewL();
	CRichText* richText=CRichText::NewL(paraLayer, charLayer, CEditableText::EFlatStorage, KBufferGranularity);

	
	TBuf<50> theMessageContents = _L("Test message contents");
	richText->InsertL(0, theMessageContents);
	TInt cc = richText->DocumentLength();
	iPlainBodyTextStore->StoreRichTextAsPlainTextL(*richText);
	iPlainBodyTextStore->CommitL();
	delete iPlainBodyTextStore;
	delete richText;
	
	richText=CRichText::NewL(paraLayer, charLayer, CEditableText::EFlatStorage, KBufferGranularity);
	store->RestoreBodyTextL(*richText);
	richText->DocumentLength();
	
	ASSERT_EQUALS(richText->DocumentLength(), cc);
	delete store;
	delete richText;
	delete charLayer;
	delete paraLayer;
	}

void CTestPlainTextStore::TestStoreRestoreRichTextL()
	{	
	iServerEntry->SetEntry(iBodyMessageEntry);
	CMsvStore* store = iServerEntry->EditStoreL();

	CParaFormatLayer* paraLayer = CParaFormatLayer::NewL();
	CCharFormatLayer* charLayer = CCharFormatLayer::NewL();
	CRichText* richText=CRichText::NewL(paraLayer, charLayer, CEditableText::EFlatStorage, KBufferGranularity);

	
	TBuf<50> theMessageContents = _L("Test message contents");
	richText->InsertL(0, theMessageContents);
	TInt cc = richText->DocumentLength();
	store->StoreBodyTextL(*richText);
	store->CommitL();

	delete richText;
	delete store;
	store = NULL;
		
	HBufC16* filebuf11 = HBufC::NewL(iSize+KTestStringLength);
	
	CMsvStore* store1 = iServerEntry->ReadStoreL();
	iPlainBodyTextStore = store1->InitialisePlainBodyTextForReadL(10);
	TInt sizeOfRestoredText = 0;

	TPtr16 bufPtr = filebuf11->Des();
	TInt pos = 0;
	iPlainBodyTextStore->NextChunkL(bufPtr, iActiveWaiter->iStatus);
	iActiveWaiter->WaitActive();
	
	TInt len = bufPtr.Length();
	while(bufPtr.Length() > 0)
		{
		iPlainBodyTextStore->NextChunkL(bufPtr, iActiveWaiter->iStatus);
		iActiveWaiter->WaitActive();
		len += bufPtr.Length();
		}
	ASSERT_EQUALS(len, cc);

	delete store1;
	}
	
CTestSuite* CTestPlainTextStore::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestStoreRestore8BitChunkAsyncL);
	ADD_ASYNC_TEST_STEP(TestStoreRestore8BitChunkSynchL);
	ADD_ASYNC_TEST_STEP(TestStoreRestore16BitChunkAsyncL);
	ADD_ASYNC_TEST_STEP(TestStoreRestore16BitChunkSynchL);
	ADD_ASYNC_TEST_STEP(TestStoreRestore16BitChunkAsyncWith8BitL);
	ADD_ASYNC_TEST_STEP(TestStoreRestore16BitChunkSynchWith8BitL);
	ADD_ASYNC_TEST_STEP(TestStoreRestore16BitChunkAsynchStoreWith8BitL);
	ADD_ASYNC_TEST_STEP(TestStoreRichTextAsPlainTextL);
	ADD_ASYNC_TEST_STEP(TestStoreRestoreRichTextL);
	END_SUITE;
	}
