/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Tests Reading of message file.
*
*/

#include <ecom/ecom.h>
#include <ecom/implementationinformation.h>
#include <epos_comasuplpospayload.h>

#include "epos_comasuplmsgfilereadertest.h"
#include "epos_comasuplmessagefilereader.h"
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplpossessioncreationtest.h"

#include "epos_comasuplposhandlerbase.h"
#include "epos_comasuplpossessionbase.h"

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplMsgFileReaderTest* COMASuplMsgFileReaderTest::NewL(
							TFileName& aMsgFile
							, COMASuplPosTesterCategory* aCallBack)
	{
	COMASuplMsgFileReaderTest* self =
						 new(ELeave)COMASuplMsgFileReaderTest(aCallBack);
	CleanupStack::PushL(self);
	self->ConstructL(aMsgFile);
	CleanupStack::Pop(self);
	return self;
	}

COMASuplMsgFileReaderTest::COMASuplMsgFileReaderTest(
										COMASuplPosTesterCategory* aCallBack)
										:COMASuplPosTesterCategory(aCallBack)
										
	{
	iNextInSeq = EPosSessionTestComplete;
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplMsgFileReaderTest::ConstructL(TFileName& aMsgFile)
	{
	CActiveScheduler::Add(this);
	iMsgFileName.Copy(aMsgFile);
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplMsgFileReaderTest::~COMASuplMsgFileReaderTest()
	{
	Cancel();
	if(iMsgInfoList)
		{
		iMsgInfoList->ResetAndDestroy();
		delete iMsgInfoList;
		}
	
	delete iMsgFileReader;
	}

void COMASuplMsgFileReaderTest::InitializeL(COMASuplTestHandler* aTestHandler
								, COMASuplPosTesterLogger& aLogger
								, TInt aTestNo)
	{
	iTestHandler = aTestHandler;
	iLogger = &aLogger;
	iTestNo = aTestNo;
	}
// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplMsgFileReaderTest::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KMessageReaderTest, iTestNo);
		}
	iNextInSeq = EStartTest;
	__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
	DummyRequest(iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Handles an active object's request completion event.
// ---------------------------------------------------------------------------
//
void COMASuplMsgFileReaderTest::RunL()
	{
	switch(iNextInSeq)
		{
		case EStartTest:
			{
			iMsgFileReader = COMASuplMessageFileReader::NewL(iMsgFileName, iLogger);
			iInfo++; //info about message file name
			iMsgInfoList = NULL;
			iMsgInfoList = &(iMsgFileReader->GetMessageListL());
			TestingComplete(KMessageReaderTestComplete);
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplMsgFileReaderTest::DoCancel()
	{
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt COMASuplMsgFileReaderTest::RunError(TInt /*aError*/)
	{
	iError++;//Leave in file reader is to be counted.
	TestingAborted(KMessageReaderTestAborted);
	return KErrNone;
	}
