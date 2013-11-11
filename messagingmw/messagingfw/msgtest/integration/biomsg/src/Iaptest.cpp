// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Iaptest.cpp
//

#include <biocmtm.h>
#include <bioscmds.h>
#include <bitsids.h>
#include <biouids.h>

#include <bsp.h>
#include "iacp.h"

#include <pop3set.h>
#include <imapset.h>

#include <commsdattypesv1_1.h>
#include <commsdat.h>
using namespace CommsDat;

#include <pop3cmds.h>
#include <iapprefs.h>

#include "iaptest.h"

#include "parsertest.h"
#include "tmsvbioinfo.h"

const TUid KUidMsgTypePop3  = {0x10001029};

CInternetAccessTester::~CInternetAccessTester()
	{
	delete iPop3ClientMtm;
	delete iClientMtmRegistry;

	delete iTimer;

	delete iSelection;
	if(iParsedFieldArray)
		{
		iParsedFieldArray->ResetAndDestroy();
		delete iParsedFieldArray;
		}
	}


CInternetAccessTester* CInternetAccessTester::NewL(CSmsTestUtils& aTestUtils)
	{
	CInternetAccessTester* self = new (ELeave) CInternetAccessTester(aTestUtils);
	CleanupStack::PushL(self);

	self->ConstructL();

	CleanupStack::Pop();
	return self;
	}



CInternetAccessTester::CInternetAccessTester(CSmsTestUtils& aTestUtils)
: iSmsTest(aTestUtils)
	{
	}

void CInternetAccessTester::StartL(TMsvId aEntryId)
	{
	iEntryId = aEntryId;
	MsvEntry().SetEntryL(aEntryId);
	GetParsedFieldStoreL();
	
	GetIspAndMailServiceL();

	TestRefreshL();

	TestQuitL();
	}

void CInternetAccessTester::ConstructL()
	{
	iClientMtmRegistry = CClientMtmRegistry::NewL(Session());
	iPop3ClientMtm = (CPop3ClientMtm*) iClientMtmRegistry->NewMtmL(KUidMsgTypePop3);

	CMsvEntry* cEntry = Session().GetEntryL(KMsvGlobalInBoxIndexEntryId);
	iPop3ClientMtm->SetCurrentEntryL(cEntry); // iClientMtm takes ownership on entry
		
	iTimer = TestUiTimer::NewLC(iSmsTest);
	CleanupStack::Pop();	// iTimer
	}

void CInternetAccessTester::GetParsedFieldStoreL()
	{
	CMsvStore* store = MsvEntry().ReadStoreL();
	CleanupStack::PushL(store);

	// try to get our array of parsed fields
	RMsvReadStream in;
	in.OpenLC( *store, KUidMsvBIODataStream );
	InternalizeL(in);
	CleanupStack::PopAndDestroy(2);
	}

void CInternetAccessTester::InternalizeL(RMsvReadStream& aReadStream)
	{
	if(iParsedFieldArray)
		{
		iParsedFieldArray->ResetAndDestroy();
		delete iParsedFieldArray;
		iParsedFieldArray=NULL;
		}

	iParsedFieldArray = new(ELeave) CArrayPtrSeg<CParsedField>(16);

	CParsedField* parsedField = NULL;
	TInt count = aReadStream.ReadUint8L();
	for (TInt i=0; i < count; i++)
		{
		parsedField = new (ELeave) CParsedField();
		TRAPD(err, parsedField->InternalizeL(aReadStream))
		if(err)
			{
			delete parsedField; //deletes the last allocated object, privious ones will be deleted by iParsedFieldArray->ResetAndDestroy()
			User::Leave(err);
			}
		iParsedFieldArray->AppendL(parsedField);
		}
	}

void CInternetAccessTester::GetIspAndMailServiceL()
	{
	HBufC* ispName = NULL;
	HBufC* mailName = NULL;

	TBool mailNameFound = EFalse;
	TBool ispNameFound = EFalse;

	TInt count = iParsedFieldArray->Count();
	for(TInt loop = 0;loop<count && !(ispNameFound && mailNameFound);loop++)
		{
		CParsedField& field = *(*iParsedFieldArray)[loop];
		if(field.FieldName().Compare(KMAIL)==0)
			{
			mailName = field.FieldValue().AllocLC();
			mailNameFound = ETrue;
			}
		else if(field.FieldName().Compare(KINTERNET)==0)
			{
			ispName = field.FieldValue().AllocLC();
			ispNameFound = ETrue;
			}
		}

	if(mailNameFound && ispNameFound)
		{
		GetMailServiceL(*mailName);
		GetIAPL(*ispName);
		}
	else
		User::Leave(KErrNotFound);
	CleanupStack::PopAndDestroy(2);
	}

void CInternetAccessTester::GetMailServiceL(const TDesC& aServiceName)
	{		
	TBool serviceFound = EFalse;

	// Get list of children IDs
	MsvEntry().SetEntryL(KMsvRootIndexEntryId);
	CMsvEntrySelection* msvSelection = MsvEntry().ChildrenWithTypeL(KUidMsvServiceEntry);
	CleanupStack::PushL(msvSelection);
	
	TMsvEntry entry;
	TInt count = msvSelection->Count(); 
	for (TInt j = 0; j < count; j++)
		{
		MsvEntry().SetEntryL((*msvSelection)[j]);
		// set context to service entry
		entry = MsvEntry().Entry(); 
		if (entry.iType == KUidMsvServiceEntry)
		    if(entry.iDetails.Compare(aServiceName)==0)
				if((entry.iMtm == KUidMsgTypePOP3 || entry.iMtm == KUidMsgTypeIMAP4) && entry.iDetails.CompareF(aServiceName)==0 )
					{
					iServiceId = entry.Id();
					serviceFound = ETrue;
					break;
					}

		}

	CleanupStack::PopAndDestroy();//msvSelection
	if(serviceFound==EFalse)
		User::Leave(KErrNotFound);
	}

void CInternetAccessTester::GetIAPL(const TDesC& aIspName)
	{
	CMDBSession* iDBSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	CCDIAPRecord *iapRecord = (CCDIAPRecord*)CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord);
	CleanupStack::PushL(iapRecord);
	iapRecord->iRecordName.SetMaxLengthL(aIspName.Length());
	iapRecord->iRecordName = aIspName;
	
	if(iapRecord->FindL(*iDBSession))
		iIap = iapRecord->iRecordTag.RecordId();	
	else 
		User::Leave(KErrNotFound);
	
	CleanupStack::PopAndDestroy(iapRecord);
	delete iDBSession;
	iDBSession= NULL;
	}

void CInternetAccessTester::TestRefreshL()
	{
	
	// try and refresh
	delete iSelection;
	iSelection = NULL;
	iSelection = new(ELeave)CMsvEntrySelection;
	iSelection->AppendL(iServiceId);

	TBuf8<1> dummyParam;

	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;

	CMsvOperation* operation = iPop3ClientMtm->InvokeAsyncFunctionL( KPOP3MTMConnect, *iSelection, dummyParam, wait->iStatus);
	
	iTimer->SetOperation(operation);
	iTimer->IssueRequest();
	
	wait->Start();
	CActiveScheduler::Start();

	TInt error = operation->iStatus.Int();
	iTimer->Cancel();
	
	CleanupStack::PopAndDestroy();	// wait
	if(error !=KErrNone)
		User::Leave(error);
	}

void CInternetAccessTester::TestQuitL()
	{
	TBuf8<1> dummyParam;

	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;

	CMsvOperation* operation = iPop3ClientMtm->InvokeAsyncFunctionL( KPOP3MTMDisconnect, *iSelection, dummyParam, wait->iStatus);
	
	iTimer->SetOperation(operation);
	iTimer->IssueRequest();
	
	wait->Start();
	CActiveScheduler::Start();

	TInt error = operation->iStatus.Int();
	iTimer->Cancel();
	
	CleanupStack::PopAndDestroy();	// wait
	if(error !=KErrNone)
		User::Leave(error);
	}
