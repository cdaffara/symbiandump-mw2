// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The implementation of the transition validation classes upon the CMailtoSchemeHandler class methods.
// 
//

/**
 @file
 @SYMCreationDate Created : 24/05/01 15:57:47
*/

// ______________________________________________________________________________
//
inline TMailtoSchemeHandler_Ctor_TransitionValidator::TMailtoSchemeHandler_Ctor_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

inline TBool TMailtoSchemeHandler_Ctor_TransitionValidator::ValidatePreConditions()
	{
	return ETrue;
	}

inline TBool TMailtoSchemeHandler_Ctor_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;
	return ETrue;
	}

// ______________________________________________________________________________
//
inline TMailtoSchemeHandler_Dtor_TransitionValidator::TMailtoSchemeHandler_Dtor_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

inline TBool TMailtoSchemeHandler_Dtor_TransitionValidator::ValidatePreConditions()
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;
	return ETrue;
	}

inline TBool TMailtoSchemeHandler_Dtor_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	return ETrue;
	}


// ______________________________________________________________________________
//
inline TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator::TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

inline TBool TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator::ValidatePreConditions()
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;
	return ETrue;
	}

inline TBool TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;
	
	CMailtoSchemeHandler* schemeHandler = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler;
	TMailtoSchemeHandler_StateAccessor* stateAccessor = &REINTERPRET_CAST(TMailtoSchemeHandler_StateAccessor&, iUTContext.StateAccessor());
	RArray<TPtrC> toArray = stateAccessor->GetRecipientArray(*schemeHandler);
	const TInt toCount = toArray.Count();

	RArray<TPtrC> recipientCheck = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iArray;

	for (TInt i=0; i<toCount; ++i)
		{
		//TPtrC a(toArray[i]);
		//TPtrC b(recipientCheck[i]);
		iUTContext.DataLogger().LogInformation(toArray[i]);
		iUTContext.DataLogger().LogInformation(recipientCheck[i]);
		if (toArray[i].CompareF(recipientCheck[i]) != 0)
			{
			iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
			return EFalse;
			}
		}

	TPtrC& subject = stateAccessor->GetSubject(*schemeHandler);
	if (subject.Length() != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		iUTContext.DataLogger().LogInformation(subject);
		return EFalse;
		}

	TPtrC& body = stateAccessor->GetBody(*schemeHandler);
	if (body.Length() != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		iUTContext.DataLogger().LogInformation(body);
		return EFalse;
		}

	return ETrue;
	}

// ______________________________________________________________________________
//
inline TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator::TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

inline TBool TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator::ValidatePreConditions()
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;
	return ETrue;
	}

inline TBool TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;

	CMailtoSchemeHandler* schemeHandler = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler;
	TMailtoSchemeHandler_StateAccessor* stateAccessor = &REINTERPRET_CAST(TMailtoSchemeHandler_StateAccessor&, iUTContext.StateAccessor());
	RArray<TPtrC> toArray = stateAccessor->GetRecipientArray(*schemeHandler);
	const TInt toCount = toArray.Count();

	const TDesC* subjectCheck = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iSubjectCheck;

	
	if (toCount != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		return EFalse;
		}

	TPtrC& subject = stateAccessor->GetSubject(*schemeHandler);
	iUTContext.DataLogger().LogInformation(subject);
	if (subject.CompareF(*subjectCheck) != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		return EFalse;
		}

	TPtrC& body = stateAccessor->GetBody(*schemeHandler);
	if (body.Length() != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		iUTContext.DataLogger().LogInformation(body);
		return EFalse;
		}

	return ETrue;
	}

// ______________________________________________________________________________
//
inline TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator::TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

inline TBool TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator::ValidatePreConditions()
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;
	return ETrue;
	}

inline TBool TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;

	CMailtoSchemeHandler* schemeHandler = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler;
	TMailtoSchemeHandler_StateAccessor* stateAccessor = &REINTERPRET_CAST(TMailtoSchemeHandler_StateAccessor&, iUTContext.StateAccessor());
	RArray<TPtrC> toArray = stateAccessor->GetRecipientArray(*schemeHandler);
	const TInt toCount = toArray.Count();

	const TDesC* bodyCheck = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iBodyCheck;
	
	if (toCount != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		return EFalse;
		}

	TPtrC& subject = stateAccessor->GetSubject(*schemeHandler);
	if (subject.Length() != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		iUTContext.DataLogger().LogInformation(subject);
		return EFalse;
		}

	TPtrC& body = stateAccessor->GetBody(*schemeHandler);
	iUTContext.DataLogger().LogInformation(body);
	if (body.CompareF(*bodyCheck) != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		return EFalse;
		}

	return ETrue;
	}

// ______________________________________________________________________________
//
inline TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator::TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

inline TBool TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator::ValidatePreConditions()
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;
	return ETrue;
	}

inline TBool TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;

	CMailtoSchemeHandler* schemeHandler = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler;
	TMailtoSchemeHandler_StateAccessor* stateAccessor = &REINTERPRET_CAST(TMailtoSchemeHandler_StateAccessor&, iUTContext.StateAccessor());
	RArray<TPtrC> toArray = stateAccessor->GetRecipientArray(*schemeHandler);
	const TInt toCount = toArray.Count();

	RArray<TPtrC> recipientCheck = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iArray;

	for (TInt i=0; i<toCount; ++i)
		{
		iUTContext.DataLogger().LogInformation(toArray[i]);
		//iUTContext.DataLogger().LogInformation(postCheckArray[i]);
		if (toArray[i].CompareF(recipientCheck[i]) != 0)
			{
			iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
			return EFalse;
			}
		}

	const TDesC* subjectCheck = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iSubjectCheck;	
	TPtrC& subject = stateAccessor->GetSubject(*schemeHandler);
	iUTContext.DataLogger().LogInformation(subject);
	if (subject.CompareF(*subjectCheck) != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		return EFalse;
		}

	const TDesC* bodyCheck = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iBodyCheck;
	TPtrC& body = stateAccessor->GetBody(*schemeHandler);
	iUTContext.DataLogger().LogInformation(body);
	if (body.CompareF(*bodyCheck) != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		return EFalse;
		}

	return ETrue;
	}

// ______________________________________________________________________________
//
inline TMailtoSchemeHandler_CreateEmailL_TransitionValidator::TMailtoSchemeHandler_CreateEmailL_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

inline TBool TMailtoSchemeHandler_CreateEmailL_TransitionValidator::ValidatePreConditions()
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;
	return ETrue;
	}

inline TBool TMailtoSchemeHandler_CreateEmailL_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler))
		return EFalse;

	CMailtoSchemeHandler* schemeHandler = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iMailtoSchemeHandler;
	TMailtoSchemeHandler_StateAccessor* stateAccessor = &REINTERPRET_CAST(TMailtoSchemeHandler_StateAccessor&, iUTContext.StateAccessor());
	RArray<TPtrC> recipientCheck = stateAccessor->GetRecipientArray(*schemeHandler);
	
	TPtrC bodyCheck = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iBody;

	TMsvId id = REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext).iId;
	if (id == 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		return EFalse;
		}

	_LIT(KTest, "This is a test");
	RTest test(KTest);
	
	CEmailTestUtils* testUtils = CEmailTestUtils::NewL(test, ETuGoClientSide);
	CleanupStack::PushL(testUtils);

	testUtils->TestStart(1);
	test.Start(_L("Send SMTP message"));

	testUtils->SetEntryL(id);
	TMsvEntry entry = testUtils->Entry();

	// Check Mtm
	//

	TUid mtm = entry.iMtm;
	const TUid mailMtm = {0x10001028};

	if (mtm != mailMtm) 
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		return EFalse;
		}

	// Check Subject
	//

	TPtrC subject = entry.iDescription;
	iUTContext.DataLogger().LogInformation(subject);
	TPtrC& subjectCheck = stateAccessor->GetSubject(*schemeHandler);
	if (subject.CompareF(subjectCheck) != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		return EFalse;
		}

	// Check Recipient
	//

	CMsvStore* store = testUtils->iMsvEntry->ReadStoreL();
	CleanupStack::PushL(store);
	CImHeader* header = CImHeader::NewLC();
	header->RestoreL(*store);
	CDesCArray& toArray = header->ToRecipients();
	const TInt toCount = toArray.Count();
	for (TInt i=0; i<toCount; ++i)
		{
		iUTContext.DataLogger().LogInformation(toArray[i]);
		//iUTContext.DataLogger().LogInformation(receipientCheck[i]);
		if (toArray[i].CompareF(recipientCheck[i]) != 0)
			{
			iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
			return EFalse;
			}
		}
	
	CleanupStack::PopAndDestroy(header);
	CleanupStack::PopAndDestroy(store);

	// Check body
	//

	CParaFormatLayer* paraFormatLayer=CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer=CCharFormatLayer::NewL(); 
	CleanupStack::PushL(charFormatLayer);

	CRichText* validRichText1=CRichText::NewL(paraFormatLayer, charFormatLayer, CEditableText::EFlatStorage, 256);
	CleanupStack::PushL(validRichText1);

	CMsvOperationWait* iWait=CMsvOperationWait::NewLC();

	CImEmailMessage* imEmailMessage = CImEmailMessage::NewLC(*testUtils->iMsvEntry);
	imEmailMessage->GetBodyTextL(iWait->iStatus, id, CImEmailMessage::EThisMessageOnly, *validRichText1, *paraFormatLayer, *charFormatLayer);
	iWait->Start();
	CActiveScheduler::Start();

	TBuf<100> bufa = validRichText1->Read(0 , 21);
	TBuf<100> bufb(bodyCheck);
	iUTContext.DataLogger().LogInformation(bufa);
	//iUTContext.DataLogger().LogInformation(bufb);
	if (bufa.Compare(bufb) != 0)
		{
		iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
		return EFalse;
	}
	
	CleanupStack::PopAndDestroy(2); //imEmailMessage, iWait
	CleanupStack::PopAndDestroy(1);	//validRichText1
	CleanupStack::PopAndDestroy(2); //paraFormatLayer, charFormatLayer

	testUtils->TestFinish(1);
	testUtils->TestHarnessCompleted();
	
	CleanupStack::PopAndDestroy(testUtils);
	
	test.End();
	test.Close();

	return ETrue;
	}
