// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <txtrich.h>
#include <mtmdef.h>
#include <msvftext.h>
#include "Testsetup.h"

RTest test(_L("Find Text Testrig"));

const TInt KMaxFindTextLen = 250;

//

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

//

LOCAL_C HBufC* CreateBufferL(TInt aStart, TInt aFinish)
	{
	HBufC* buf = HBufC::NewL(aFinish + 1);
	CleanupStack::PushL(buf);

	TInt count = aStart;
	while(count <= aFinish)
		{
		if (buf->Des().MaxLength() < buf->Des().Length() + count + 1)
			buf = buf->ReAllocL(buf->Des().MaxLength() + aFinish + 1);

		buf->Des().AppendFill('A', count);
		buf->Des().Append(' ');

		count++;
		}
	
	CleanupStack::Pop(); // buf
	return buf;
	}

LOCAL_C void CreateRichBufferL(CRichText& aText, TInt aStart, TInt aFinish)
	{
	HBufC* buf = CreateBufferL(aStart, aFinish);
	CleanupStack::PushL(buf);
	
	aText.Reset();
	aText.InsertL(0, buf->Des());

	CleanupStack::PopAndDestroy(); // buf
	}

LOCAL_C void TestFindSyncL()
	{
	HBufC* text = HBufC::NewL(KMaxFindTextLen);
	CleanupStack::PushL(text);

	CMsvFindText* find = CMsvFindText::NewL();
	CleanupStack::PushL(find);

	HBufC* buf = CreateBufferL(1, KMaxFindTextLen);
	CleanupStack::PushL(buf);

	TInt count = 1;
	while(count <= KMaxFindTextLen)
		{
		text->Des().Fill('A', count);
		test(find->FindTextL(text->Des(), buf->Des(), KMsvFindWholeWord|KMsvFindCaseSensitive));
		test(find->FoundText());

		text->Des().Fill('a', count);
		test(!find->FindTextL(text->Des(), buf->Des(), KMsvFindWholeWord|KMsvFindCaseSensitive));
		test(!find->FoundText());

		count++;
		}

	CleanupStack::PopAndDestroy(3); // buf, find, text
	}

LOCAL_C void TestFindRichSyncL()
	{
	// rich text
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);

	CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(richText);

	HBufC* text = HBufC::NewL(KMaxFindTextLen);
	CleanupStack::PushL(text);

	CMsvFindText* find = CMsvFindText::NewL();
	CleanupStack::PushL(find);

	CreateRichBufferL(*richText, 1, KMaxFindTextLen);

	TInt count = 1;
	while(count <= KMaxFindTextLen)
		{
		text->Des().Fill('A', count);
		test(find->FindRichTextL(text->Des(), *richText, KMsvFindWholeWord|KMsvFindCaseSensitive));
		test(find->FoundText());

		text->Des().Fill('a', count);
		test(!find->FindRichTextL(text->Des(), *richText, KMsvFindWholeWord|KMsvFindCaseSensitive));
		test(!find->FoundText());

		count++;
		}

	CleanupStack::PopAndDestroy(5); // find, text, richText, charFormatLayer, paraFormatLayer
	}

LOCAL_C void TestFindAsyncL()
	{
	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	HBufC* text = HBufC::NewL(KMaxFindTextLen);
	CleanupStack::PushL(text);

	CMsvFindText* find = CMsvFindText::NewL();
	CleanupStack::PushL(find);

	HBufC* buf = CreateBufferL(1, KMaxFindTextLen);
	CleanupStack::PushL(buf);

	TInt count = 1;
	while(count <= KMaxFindTextLen)
		{
		text->Des().Fill('A', count);
		active->StartL();
		find->FindTextL(text->Des(), buf->Des(), KMsvFindWholeWord|KMsvFindCaseSensitive, active->iStatus);
		CActiveScheduler::Start();
		test(active->iStatus == KErrNone);
		test(find->FoundText());

		text->Des().Fill('a', count);
		active->StartL();
		find->FindTextL(text->Des(), buf->Des(), KMsvFindWholeWord|KMsvFindCaseSensitive, active->iStatus);
		CActiveScheduler::Start();
		test(active->iStatus == KErrNone);
		test(!find->FoundText());

		count++;
		}

	CleanupStack::PopAndDestroy(4); // buf, find, text, active
	}

LOCAL_C void TestFindRichAsyncL()
	{
	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	// rich text
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);

	CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(richText);

	HBufC* text = HBufC::NewL(KMaxFindTextLen);
	CleanupStack::PushL(text);

	CMsvFindText* find = CMsvFindText::NewL();
	CleanupStack::PushL(find);

	CreateRichBufferL(*richText, 1, KMaxFindTextLen);

	TInt count = 1;
	while(count <= KMaxFindTextLen)
		{
		text->Des().Fill('A', count);
		active->StartL();
		find->FindRichTextL(text->Des(), *richText, KMsvFindWholeWord|KMsvFindCaseSensitive, active->iStatus);
		CActiveScheduler::Start();
		test(active->iStatus == KErrNone);
		test(find->FoundText());

		text->Des().Fill('a', count);
		active->StartL();
		find->FindRichTextL(text->Des(), *richText, KMsvFindWholeWord|KMsvFindCaseSensitive, active->iStatus);
		CActiveScheduler::Start();
		test(active->iStatus == KErrNone);
		test(!find->FoundText());

		count++;
		}

	CleanupStack::PopAndDestroy(6); // find, text, richText, charFormatLayer, paraFormatLayer, active
	}

LOCAL_C void TestFindWordSyncL()
	{
	HBufC* text = HBufC::NewL(3 * KMsvMaxFindTextLength);
	CleanupStack::PushL(text);

	CMsvFindText* find = CMsvFindText::NewL();
	CleanupStack::PushL(find);

	HBufC* buf = HBufC::NewL(KMsvMaxFindTextLength);
	CleanupStack::PushL(buf);

	TInt start;
	for(start = 0; start <= 2 * KMsvMaxFindTextLength; start++)
		{
		text->Des().Fill(' ', start);		
		text->Des().AppendFill('A', KMsvMaxFindTextLength);
		text->Des().AppendFill(' ', 2 * KMsvMaxFindTextLength - start);		

		buf->Des().Fill('A', KMsvMaxFindTextLength);
		test(find->FindTextL(buf->Des(), text->Des(), KMsvFindWholeWord));
		test(find->FoundText());
		}

	CleanupStack::PopAndDestroy(3); // buf, find, text
	}

LOCAL_C void TestFindRichWordSyncL()
	{
	// rich text
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);

	CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(richText);

	CMsvFindText* find = CMsvFindText::NewL();
	CleanupStack::PushL(find);

	HBufC* text = HBufC::NewL(3 * KMsvMaxFindTextLength);
	CleanupStack::PushL(text);

	HBufC* buf = HBufC::NewL(KMsvMaxFindTextLength);
	CleanupStack::PushL(buf);

	TInt start;
	for(start = 0; start <= 2 * KMsvMaxFindTextLength; start++)
		{
		text->Des().Fill(' ', start);		
		text->Des().AppendFill('A', KMsvMaxFindTextLength);
		text->Des().AppendFill(' ', 2 * KMsvMaxFindTextLength - start);		
	
		richText->Reset();
		richText->InsertL(0, text->Des());

		buf->Des().Fill('A', KMsvMaxFindTextLength);
		test(find->FindRichTextL(buf->Des(), *richText, KMsvFindWholeWord));
		test(find->FoundText());
		}

	CleanupStack::PopAndDestroy(6); // buf, text, find, richText, charFormatLayer, paraFormatLayer
	}

LOCAL_C void TestFindWordAsyncL()
	{
	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	HBufC* text = HBufC::NewL(3 * KMsvMaxFindTextLength);
	CleanupStack::PushL(text);

	CMsvFindText* find = CMsvFindText::NewL();
	CleanupStack::PushL(find);

	HBufC* buf = HBufC::NewL(KMsvMaxFindTextLength);
	CleanupStack::PushL(buf);

	TInt start;
	for(start = 0; start <= 2 * KMsvMaxFindTextLength; start++)
		{
		text->Des().Fill(' ', start);		
		text->Des().AppendFill('A', KMsvMaxFindTextLength);
		text->Des().AppendFill(' ', 2 * KMsvMaxFindTextLength - start);		

		buf->Des().Fill('A', KMsvMaxFindTextLength);

		active->StartL();
		find->FindTextL(buf->Des(), text->Des(), KMsvFindWholeWord, active->iStatus);
		CActiveScheduler::Start();
		test(active->iStatus == KErrNone);
		test(find->FoundText());
		}

	CleanupStack::PopAndDestroy(4); // buf, find, text, async
	}

LOCAL_C void TestFindRichWordAsyncL()
	{
	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	// rich text
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);

	CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(richText);

	CMsvFindText* find = CMsvFindText::NewL();
	CleanupStack::PushL(find);

	HBufC* text = HBufC::NewL(3 * KMsvMaxFindTextLength);
	CleanupStack::PushL(text);

	HBufC* buf = HBufC::NewL(KMsvMaxFindTextLength);
	CleanupStack::PushL(buf);

	TInt start;
	for(start = 0; start <= 2 * KMsvMaxFindTextLength; start++)
		{
		text->Des().Fill(' ', start);		
		text->Des().AppendFill('A', KMsvMaxFindTextLength);
		text->Des().AppendFill(' ', 2 * KMsvMaxFindTextLength - start);		
	
		richText->Reset();
		richText->InsertL(0, text->Des());

		buf->Des().Fill('A', KMsvMaxFindTextLength);

		active->StartL();
		find->FindRichTextL(buf->Des(), *richText, KMsvFindWholeWord, active->iStatus);
		CActiveScheduler::Start();
		test(active->iStatus == KErrNone);
		test(find->FoundText());
		}

	CleanupStack::PopAndDestroy(7); // buf, text, find, richText, charFormatLayer, paraFormatLayer, active
	}

LOCAL_C void TestFindConstructionWithHeapFailL()
	{
	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	
	TInt error;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		CMsvFindText* find = NULL;
		TRAP(error, find = CMsvFindText::NewL());

		__UHEAP_RESET;

		if (error == KErrNone)
			{
			finished = ETrue;
			delete find;
			}
		else
			test(error == KErrNoMemory);
		}
	}

LOCAL_C void doTestsL()
	{
	CMsgsTestUtils::Start(_L("Find Construction With Heap Failure"));
	TestFindConstructionWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Find Plain Text Synchronous"));
	TestFindSyncL();
	TestFindWordSyncL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Find Plain Text Asynchronous"));
	TestFindAsyncL();
	TestFindWordAsyncL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Find Rich Text Synchronous"));
	TestFindRichSyncL();
	TestFindRichWordSyncL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Find Rich Text Asynchronous"));
	TestFindRichAsyncL();
	TestFindRichWordAsyncL();
	CMsgsTestUtils::Complete();
	}
