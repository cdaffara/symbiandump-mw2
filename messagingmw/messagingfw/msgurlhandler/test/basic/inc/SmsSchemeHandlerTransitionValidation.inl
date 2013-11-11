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
// The implementation of the transition validation classes upon the CSmsSchemeHandler class methods.
// 
//

/**
 @file
 @SYMCreationDate Created : 01/06/01 14:07:24
*/

// ______________________________________________________________________________
//
inline TSmsSchemeHandler_Ctor_TransitionValidator::TSmsSchemeHandler_Ctor_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

inline TBool TSmsSchemeHandler_Ctor_TransitionValidator::ValidatePreConditions()
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext).iSmsSchemeHandler))
		return EFalse;
	return ETrue;
	}

inline TBool TSmsSchemeHandler_Ctor_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext).iSmsSchemeHandler))
		return EFalse;
	return ETrue;
	}

// ______________________________________________________________________________
//
inline TSmsSchemeHandler_Dtor_TransitionValidator::TSmsSchemeHandler_Dtor_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

inline TBool TSmsSchemeHandler_Dtor_TransitionValidator::ValidatePreConditions()
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext).iSmsSchemeHandler))
		return EFalse;
	return ETrue;
	}

inline TBool TSmsSchemeHandler_Dtor_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext).iSmsSchemeHandler))
		return EFalse;
	return ETrue;
	}


// ______________________________________________________________________________
//
inline TSmsSchemeHandler_ParseUrlL_TransitionValidator::TSmsSchemeHandler_ParseUrlL_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

inline TBool TSmsSchemeHandler_ParseUrlL_TransitionValidator::ValidatePreConditions()
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext).iSmsSchemeHandler))
		return EFalse;
	return ETrue;
	}

inline TBool TSmsSchemeHandler_ParseUrlL_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext).iSmsSchemeHandler))
		return EFalse;

	CSmsSchemeHandler* schemeHandler = REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext).iSmsSchemeHandler;
	TSmsSchemeHandler_StateAccessor* stateAccessor = &REINTERPRET_CAST(TSmsSchemeHandler_StateAccessor&, iUTContext.StateAccessor());
	RArray<SSmsNumber> smsArray = stateAccessor->GetSmsArray(*schemeHandler);
	const TInt smsCount = smsArray.Count();

	RArray<CSmsSchemeHandler_UnitTestContext::SSmsNumber> postCheck = REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext).iPostCheck;

	for (TInt i=0; i<smsCount; ++i)
		{
		iUTContext.DataLogger().LogInformation(smsArray[i].iNumber);
		//iUTContext.DataLogger().LogInformation(postCheck[i].iNumber);
		if ((smsArray[i].iNumber).CompareF(postCheck[i].iNumber) != 0)
			{
			iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
			return EFalse;
			}

		iUTContext.DataLogger().LogInformation(smsArray[i].iServiceCentre);
		//iUTContext.DataLogger().LogInformation(postCheck[i].iServiceCentre);
		if ((smsArray[i].iServiceCentre).CompareF(postCheck[i].iServiceCentre) != 0)
			{
			iUTContext.DataLogger().LogInformation(_L("***** TEST FAILED *****"));
			return EFalse;
			}
		}

	return ETrue;
	}
