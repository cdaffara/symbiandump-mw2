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
// The implementation of the transition classes upon the CSmsSchemeHandler class methods.
// 
//

/**
 @file
 @SYMCreationDate Created : 01/06/01 14:07:23
*/

// ______________________________________________________________________________
//
_LIT(KSmsSchemeHandlerNewLTransition,"CSmsSchemeHandler_NewL_Transition");

inline CSmsSchemeHandler_NewL_Transition::CSmsSchemeHandler_NewL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransition(KSmsSchemeHandlerNewLTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CSmsSchemeHandler_NewL_Transition::TransitMethodL()
	{
	_LIT(KSmsSchemeHandlerNewLTransitMethod, "CSmsSchemeHandler::NewL transition");
	Context().DataLogger().LogInformation(KSmsSchemeHandlerNewLTransitMethod);
	Context().iSmsSchemeHandler = CSmsSchemeHandler::NewL();
	}

inline CSmsSchemeHandler_UnitTestContext& CSmsSchemeHandler_NewL_Transition::Context() const
	{
	return REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KSmsSchemeHandlerDtorTransition,"CSmsSchemeHandler_Dtor_Transition");

inline CSmsSchemeHandler_Dtor_Transition::CSmsSchemeHandler_Dtor_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransition(KSmsSchemeHandlerDtorTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CSmsSchemeHandler_Dtor_Transition::TransitMethodL()
	{
	_LIT(KSmsSchemeHandlerDtorTransitMethod, "CSmsSchemeHandler::Dtor transition");
	Context().DataLogger().LogInformation(KSmsSchemeHandlerDtorTransitMethod);
	delete Context().iSmsSchemeHandler;
	Context().iSmsSchemeHandler = NULL;
	}

inline CSmsSchemeHandler_UnitTestContext& CSmsSchemeHandler_Dtor_Transition::Context() const
	{
	return REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext);
	}


// ______________________________________________________________________________
//
_LIT(KSmsSchemeHandlerParseUrlLTransition,"CSmsSchemeHandler_ParseUrlL_Transition");

inline CSmsSchemeHandler_ParseUrlL_Transition::CSmsSchemeHandler_ParseUrlL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransition(KSmsSchemeHandlerParseUrlLTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CSmsSchemeHandler_ParseUrlL_Transition::TransitMethodL()
	{
	_LIT(KSmsSchemeHandlerParseUrlLTransitMethod, "CSmsSchemeHandler::ParseUrlL transition");
	Context().DataLogger().LogInformation(KSmsSchemeHandlerParseUrlLTransitMethod);
	/**
		Set up any asynchronous calls by setting iStatus = KRequestPending,
				calling SetActive() etc...,
				Handle method parameter passing, if any.
				N.B. Do the right thing on construction/destruction
				E.g. For heap based classes:
				If this is a d'tor, don't forget to NULL the iSmsSchemeHandler pointer.
				and conversely if this is a c'tor assign the
				created class to the iSmsSchemeHandler pointer.

		Note:	To transit a private method you will almost certainly have to do
				something like this:
		REINTERPRET_CAST(TSmsSchemeHandler_StateAccessor&, Context().StateAccessor()).ParseUrlL(Context().iSmsSchemeHandler, Context().iParameter1, etc...);
	 */

	TSmsSchemeHandler_StateAccessor& accessor = REINTERPRET_CAST(TSmsSchemeHandler_StateAccessor&, Context().StateAccessor());
	accessor.ParseUrlL(*(Context().iSmsSchemeHandler), *Context().iUrl);
	}

inline CSmsSchemeHandler_UnitTestContext& CSmsSchemeHandler_ParseUrlL_Transition::Context() const
	{
	return REINTERPRET_CAST(CSmsSchemeHandler_UnitTestContext&,iUTContext);
	}
