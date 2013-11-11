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
// The implementation of the transition classes upon the CMailtoSchemeHandler class methods.
// 
//

/**
 @file
 @SYMCreationDate Created : 24/05/01 15:57:46
*/

// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerNewLTransition,"CMailtoSchemeHandler_NewL_Transition");

inline CMailtoSchemeHandler_NewL_Transition::CMailtoSchemeHandler_NewL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KMailtoSchemeHandlerNewLTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CMailtoSchemeHandler_NewL_Transition::TransitMethodL()
	{
	_LIT(KMailtoSchemeHandlerNewLTransitMethod, "CMailtoSchemeHandler::NewL transition");
	Context().DataLogger().LogInformation(KMailtoSchemeHandlerNewLTransitMethod);
	Context().iMailtoSchemeHandler = CMailtoSchemeHandler::NewL();
	}

inline CMailtoSchemeHandler_UnitTestContext& CMailtoSchemeHandler_NewL_Transition::Context() const
	{
	return REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerDtorTransition,"CMailtoSchemeHandler_Dtor_Transition");

inline CMailtoSchemeHandler_Dtor_Transition::CMailtoSchemeHandler_Dtor_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KMailtoSchemeHandlerDtorTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CMailtoSchemeHandler_Dtor_Transition::TransitMethodL()
	{
	_LIT(KMailtoSchemeHandlerDtorTransitMethod, "CMailtoSchemeHandler::Dtor transition");
	Context().DataLogger().LogInformation(KMailtoSchemeHandlerDtorTransitMethod);
	delete Context().iMailtoSchemeHandler;
	Context().iMailtoSchemeHandler = NULL;
	}

inline CMailtoSchemeHandler_UnitTestContext& CMailtoSchemeHandler_Dtor_Transition::Context() const
	{
	return REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext);
	}



// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlLTransition,"CMailtoSchemeHandler_ParseUrlL_Transition");

inline CMailtoSchemeHandler_ParseUrlL_Transition::CMailtoSchemeHandler_ParseUrlL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KMailtoSchemeHandlerParseUrlLTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CMailtoSchemeHandler_ParseUrlL_Transition::TransitMethodL()
	{
	_LIT(KMailtoSchemeHandlerParseUrlLTransitMethod, "CMailtoSchemeHandler::ParseUrlL transition");
	Context().DataLogger().LogInformation(KMailtoSchemeHandlerParseUrlLTransitMethod);
	/**
		Set up any asynchronous calls by setting iStatus = KRequestPending,
				calling SetActive() etc...,
				Handle method parameter passing, if any.
				N.B. Do the right thing on construction/destruction
				E.g. For heap based classes:
				If this is a d'tor, don't forget to NULL the iMailtoSchemeHandler pointer.
				and conversely if this is a c'tor assign the
				created class to the iMailtoSchemeHandler pointer.

		Note:	To transit a private method you will almost certainly have to do
				something like this:
		REINTERPRET_CAST(TMailtoSchemeHandler_StateAccessor&, Context().StateAccessor()).ParseUrlL(Context().iMailtoSchemeHandler, Context().iParameter1, etc...);
	 */
	TMailtoSchemeHandler_StateAccessor& accessor = REINTERPRET_CAST(TMailtoSchemeHandler_StateAccessor&, Context().StateAccessor());
	accessor.ParseUrlL(*(Context().iMailtoSchemeHandler), *Context().iUrl);
	}

inline CMailtoSchemeHandler_UnitTestContext& CMailtoSchemeHandler_ParseUrlL_Transition::Context() const
	{
	return REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext);
	}


// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlAndSendLTransition,"CMailtoSchemeHandler_ParseUrlAndSendL_Transition");

inline CMailtoSchemeHandler_ParseUrlAndSendL_Transition::CMailtoSchemeHandler_ParseUrlAndSendL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransition(KMailtoSchemeHandlerParseUrlAndSendLTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CMailtoSchemeHandler_ParseUrlAndSendL_Transition::TransitMethodL()
	{
	_LIT(KMailtoSchemeHandlerParseUrlAndSendLTransitMethod, "CMailtoSchemeHandler::ParseUrlAndSendL transition");
	Context().DataLogger().LogInformation(KMailtoSchemeHandlerParseUrlAndSendLTransitMethod);
	/**
		Set up any asynchronous calls by setting iStatus = KRequestPending,
				calling SetActive() etc...,
				Handle method parameter passing, if any.
				N.B. Do the right thing on construction/destruction
				E.g. For heap based classes:
				If this is a d'tor, don't forget to NULL the iMailtoSchemeHandler pointer.
				and conversely if this is a c'tor assign the
				created class to the iMailtoSchemeHandler pointer.

		Note:	To transit a private method you will almost certainly have to do
				something like this:
		REINTERPRET_CAST(TMailtoSchemeHandler_StateAccessor&, Context().StateAccessor()).ParseUrlAndSendL(Context().iMailtoSchemeHandler, Context().iParameter1, etc...);
	 */
	Context().iMailtoSchemeHandler->ParseUrlAndSendL(*Context().iUrl);
	}

inline CMailtoSchemeHandler_UnitTestContext& CMailtoSchemeHandler_ParseUrlAndSendL_Transition::Context() const
	{
	return REINTERPRET_CAST(CMailtoSchemeHandler_UnitTestContext&,iUTContext);
	}


// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerCreateEmailLTransition,"CMailtoSchemeHandler_CreateEmailL_Transition");


