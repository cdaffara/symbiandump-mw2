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
// To run from release\wins\udeb: T_CMsvChangeBuffer
// 
//

#include <e32cons.h>
#include <e32test.h>
#include <s32file.h>
#include <e32uid.h>
#include "MSVPANIC.H"
#include "CMsvChangeBuffer.h"
#include "GenericTestsetup.h"

// Prototypes
LOCAL_C void TestGeneral();
LOCAL_C void TestMemory();
LOCAL_C void TestDefects();

// Globals
_LIT(KMsvServerPanicString, "MSGS");
_LIT(KTitle,"CMsvChangeBuffer");
RTest test(KTitle);
GLDEF_C void PanicServer(TMsvServerPanic aPanic)
//
// Panic indicating a server fault
//
	{
	test.Printf(KMsvServerPanicString, aPanic);
	test.Panic(KMsvServerPanicString, aPanic);
	}


LOCAL_C void TestGeneral()
//
//	Test General operation
//
	{
	// Startup
	CMsvChangeBuffer* changeBuffer=CMsvChangeBuffer::NewL();
	CleanupStack::PushL(changeBuffer);
	TInt numEntries = KMsvChangeBufferGranularity * 3;
	TUint32 id = 0;
	TInt i;

	// Test 1.
	// Check that the queue is empty
	test(changeBuffer->IsEmpty());

	// Test 2.
	// Add entries and grow array.  
	// Also check that Next() returns the expected entry
	for(i = 0; i < numEntries; i++)
		{
		HBufC8 *entry = HBufC8::NewLC(1);
		changeBuffer->InL(entry->Des());
		test(changeBuffer->Next(id) == entry->Des());
		}

	// Test 3.
	// Test that the number of entries equals those added
	//  This isn't the best test as it relies on the NotifySequence, but
	//  there is no method for getting the actual size.
	test(TInt(changeBuffer->NotifySequence()) == numEntries);

	// Test 4.
	// Delete entries and shrink array
	for(i = 0; i < numEntries; i++)
		{
		changeBuffer->Out();
		}

	// Test 5.
	// Check that the queue is empty
	test(changeBuffer->IsEmpty());


	// Cleanup
	CleanupStack::PopAndDestroy(numEntries);
	CleanupStack::PopAndDestroy(changeBuffer);
	}

LOCAL_C void TestMemory()
//
//	Test Out of Memory errors
//
	{
	// Startup
	CMsvChangeBuffer* changeBuffer=CMsvChangeBuffer::NewL();
	CleanupStack::PushL(changeBuffer);
	TInt numEntries = KMsvChangeBufferGranularity * 3;

	// Test 1.
	// MEMORY TESTS
	// Fail with OOM when growing buffer
	
	TBool finished;

	for(TInt i=0; i<numEntries; i++)
		{
	#if defined(_DEBUG)	
		TInt failCount;
		failCount=0;
	#endif	
		finished = EFalse;
		// Typical pattern for OOM testing - loop until OOM test passes
		while (!finished)
			{
			// Create an entry and attempt to insert it in the buffer
			HBufC8 *entry = HBufC8::NewLC(1);
			__UHEAP_FAILNEXT(failCount++);
			TRAPD(ret, changeBuffer->InL(entry->Des()));
			__UHEAP_RESET;

			if (ret == KErrNone)
				{
				finished = ETrue;
				}
			else
				{
				CleanupStack::PopAndDestroy(entry);
				test(ret == KErrNoMemory);
				}
			}
		}

	// Cleanup
	CleanupStack::PopAndDestroy(numEntries);
	CleanupStack::PopAndDestroy(changeBuffer);
	}

LOCAL_C void TestDefects()
//
//	Test Defect related errors
//
	{
	// DEF035925 - "Message Server can leak memory in low memory conditions"
	//   If an OOM error occurs in CMsvChangeBuffer::GrowArrayL(), then 
	//   a subsequent call to CMsvChangeBuffer::InL will cause the oldest
	//   entry in the queue to be leaked.
	// This can be identified by checking the heap after running TestMemory.
	// Prior to fixing this defect, UHEAP_MARKEND would have panicked.

	__UHEAP_MARK;
	TestMemory();
	__UHEAP_MARKEND;
	}

LOCAL_C void doTestsL()
	{
	CGenericTestUtils::Start(_L("General"));
	TestGeneral();
	CGenericTestUtils::Complete();
	CGenericTestUtils::Next(_L("Out of memory tests"));
	TestMemory();
	CGenericTestUtils::Complete();
	CGenericTestUtils::Next(_L("Defect related tests"));
	TestDefects();
	CGenericTestUtils::Complete();
	}

