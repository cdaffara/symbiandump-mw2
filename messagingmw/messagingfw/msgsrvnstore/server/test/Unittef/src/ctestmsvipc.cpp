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
//

#include "ctestmsvipc.h"
#include <msvapi.h>
#include <msvsearchsortquery.h>
#include <msvapi.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "msvsearchsortconstants.h"
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

const TInt KMsvBufferLength = 4000;

_LIT(KFormatSrc, "Content of Source String: [%S]");
_LIT(KFormatDst, "Content of Destination String: [%S]");

_LIT(KMsgTo, "David@symbian.com");
_LIT(KMsgCc, "symbianindia@symbian.com");
_LIT(KSubject, "MessagingTeam");
_LIT(KMsgDescription, "hello");
_LIT(KMsgBody, "mobile");


CTestMsvIpc::CTestMsvIpc()
	{
	}

CTestMsvIpc::~CTestMsvIpc()
	{
	}

void CTestMsvIpc::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	}
	

void CTestMsvIpc::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);
	}

int CTestMsvIpc::AllocateBuffer()
	{
	if (iBuffer == NULL)
		{
		iBuffer = HBufC8::New(KMsvBufferLength);
		if (iBuffer == NULL)
			{
			return KErrNoMemory;
			}
		}
	return KErrNone;
	}

//Test for Pack and UnPack query
void CTestMsvIpc::TestPackUnpackQueryL()
	{
	_LIT(KFunction, "TestPackUnpackQueryL");
	INFO_PRINTF1(KFunction);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(ETrue);
	
	searchQuery->AddSearchOptionL(EMsvTo, KMsgTo, EMsvEqual);
	searchQuery->AddSearchOptionL(EMsvCc, KMsgCc, EMsvEqual);
	searchQuery->AddSearchOptionL(EMsvSubject, KSubject, EMsvEqual);
	searchQuery->AddSearchOptionL(EMsvDescription, KMsgDescription, EMsvEqual);
	searchQuery->AddSearchOptionL(EMsvBody, KMsgBody, EMsvEqual);
	searchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending);	
	
	TInt error = AllocateBuffer();
	if(error == KErrNoMemory)
		{
		return;
		}
	CleanupStack::PushL(iBuffer);
		
	TMsvPackQuery packQuery(iBuffer);
	TInt ret = packQuery.PackQuery(searchQuery);
	
	CMsvSearchSortQuery* upQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(upQuery);
	
	TMsvPackQuery unpackQuery(iBuffer);
	unpackQuery.UnpackQuery(upQuery);
	
	RBuf16 printString;
	printString.CleanupClosePushL();
	printString.CreateL(50);
	printString.Copy(searchQuery->iQueryTable[0].iQueryString);
	INFO_PRINTF2(KFormatSrc, &printString);
	
	printString.Copy(searchQuery->iQueryTable[1].iQueryString);
	INFO_PRINTF2(KFormatSrc, &printString);
	
	printString.Copy(searchQuery->iQueryTable[2].iQueryString);
	INFO_PRINTF2(KFormatSrc, &printString);
	
	printString.Copy(searchQuery->iQueryTable[3].iQueryString);
	INFO_PRINTF2(KFormatSrc, &printString);

	printString.Copy(searchQuery->iQueryTable[4].iQueryString);
	INFO_PRINTF2(KFormatSrc, &printString);
	
	printString.Copy(upQuery->iQueryTable[0].iQueryString);
	INFO_PRINTF2(KFormatDst, &printString);
	
	printString.Copy(upQuery->iQueryTable[1].iQueryString);
	INFO_PRINTF2(KFormatDst, &printString);
	
	printString.Copy(upQuery->iQueryTable[2].iQueryString);
	INFO_PRINTF2(KFormatDst, &printString);
	
	printString.Copy(upQuery->iQueryTable[3].iQueryString);
	INFO_PRINTF2(KFormatDst, &printString);
	
	printString.Copy(upQuery->iQueryTable[4].iQueryString);
	INFO_PRINTF2(KFormatDst, &printString);

	//validating query count
	ASSERT_EQUALS(searchQuery->iQueryCount, upQuery->iQueryCount);
	
	//validating query table
	ASSERT_EQUALS(searchQuery->iQueryTable[0].iQueryString, upQuery->iQueryTable[0].iQueryString);
	ASSERT_EQUALS(searchQuery->iQueryTable[1].iQueryString, upQuery->iQueryTable[1].iQueryString);
	ASSERT_EQUALS(searchQuery->iQueryTable[2].iQueryString, upQuery->iQueryTable[2].iQueryString);
	ASSERT_EQUALS(searchQuery->iQueryTable[3].iQueryString, upQuery->iQueryTable[3].iQueryString);
	
	//validating sort field
	ASSERT_EQUALS(searchQuery->iSortOnMessagePart, upQuery->iSortOnMessagePart);
	
	CleanupStack::PopAndDestroy(4);
	}
	
//test for BufferOverFlow
void CTestMsvIpc::TestBufferOverflowForPackUnpackQueryL()
	{
	_LIT(KFunction, "TestBufferOverflowForPackUnpackQueryL");
	INFO_PRINTF1(KFunction);

	const TInt KMsvBuffer = 100;

	HBufC8* tempBuffer = NULL;
	tempBuffer = HBufC8::New(KMsvBuffer);
	if (tempBuffer == NULL)
		{
		User::Leave(KErrNoMemory);
		}
	CleanupStack::PushL(tempBuffer);
	
	CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetParentId(KMsvRootIndexEntryIdValue);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(ETrue);
	
	searchQuery->AddSearchOptionL(EMsvTo, KMsgTo, EMsvEqual);
	searchQuery->AddSearchOptionL(EMsvCc, KMsgCc, EMsvEqual);
	searchQuery->AddSearchOptionL(EMsvSubject, KSubject, EMsvEqual);
	searchQuery->AddSearchOptionL(EMsvDescription, KMsgDescription, EMsvEqual);
	searchQuery->AddSearchOptionL(EMsvBody, KMsgBody, EMsvEqual);
	searchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending);	
	
	TMsvPackQuery packQuery(tempBuffer);
	TInt error = packQuery.PackQuery(searchQuery);
	ASSERT_EQUALS(error, KErrOverflow);
	CleanupStack::PopAndDestroy(2);
	}

void CTestMsvIpc::TestPackUnpackTMsvIdArrayL()
{
	_LIT(KFunction, "TestPackUnpackTMsvIdArrayL");
	INFO_PRINTF1(KFunction);

	TMsvId tmsvId = 412850;
	
	RArray<TMsvId> srcIdArray;
	RArray<TMsvId> dstIdArray;

	for(TInt count = 0; count < 10; ++count)
		{
		srcIdArray.AppendL(tmsvId++);
		}
	
	TInt error = AllocateBuffer();
	if(error == KErrNoMemory)
		{
		return;
		}
	CleanupStack::PushL(iBuffer);

	TMsvPackedIdOperation packAtSrc(iBuffer);
	packAtSrc.Pack(srcIdArray);
	
	TMsvPackedIdOperation unpackAtDst(iBuffer);
	unpackAtDst.UnpackL(dstIdArray);

	//check count at both source & destination array
	ASSERT_EQUALS(srcIdArray.Count(), dstIdArray.Count());

	//check the values of both source & destination array
	ASSERT_EQUALS(srcIdArray[0], dstIdArray[0]);
	ASSERT_EQUALS(srcIdArray[3], dstIdArray[3]);
	ASSERT_EQUALS(srcIdArray[6], dstIdArray[6]);
	ASSERT_EQUALS(srcIdArray[7], dstIdArray[7]);
	ASSERT_EQUALS(srcIdArray[9], dstIdArray[9]);
	
	srcIdArray.Close();
	dstIdArray.Close();
	CleanupStack::PopAndDestroy();
}

void CTestMsvIpc::TestBufferOverFlowPackUnpackTMsvIdArrayL()
	{
	_LIT(KFunction, "TestBufferOverFlowPackUnpackTMsvIdArrayL");
	INFO_PRINTF1(KFunction);

	const TInt KMsvBuffer = 40;
	TMsvId tmsvId = 412850;
	RArray<TMsvId> srcIdArray;
	
	for(TInt count = 0; count < 20; ++count)
		{
		srcIdArray.AppendL(tmsvId++);
		}

	iBuffer = HBufC8::New(KMsvBuffer);
	if (iBuffer == NULL)
		{
		User::Leave(KErrNoMemory);
		}
	TInt maxSize = iBuffer->Des().MaxSize();

	TMsvPackedIdOperation packAtSrc(iBuffer);
	TInt error = packAtSrc.Pack(srcIdArray);
	
	//check KErrOverflow condition
	ASSERT_EQUALS(error, KErrOverflow);
	srcIdArray.Close();
	delete iBuffer;
	}
	
CTestSuite* CTestMsvIpc::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestPackUnpackQueryL);
	ADD_ASYNC_TEST_STEP(TestBufferOverflowForPackUnpackQueryL);
	ADD_ASYNC_TEST_STEP(TestPackUnpackTMsvIdArrayL);
	ADD_ASYNC_TEST_STEP(TestBufferOverFlowPackUnpackTMsvIdArrayL);
	END_SUITE;
	}
