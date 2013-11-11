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
// The definitions of the transition classes upon the CMailtoSchemeHandler class methods.
// 
//

/**
 @file
 @SYMCreationDate Created : 24/05/01 15:57:46
*/

#ifndef __MAILTOSCHEMEHANDLERTRANSITIONS_H__
#define __MAILTOSCHEMEHANDLERTRANSITIONS_H__


#include "emailtestutils.h"

#include <ecom/test_bed/testutilities.h>
#include <ecom/test_bed/stateaccessor.h>
#include <ecom/test_bed/leaktesttransition.h>
#include <ecom/test_bed/datalogger.h>
#include "MTSCHEME.H"
#include "MailtoSchemeHandlerStateAccessor.h"
#include <ecom/test_bed/unittest.h>

#define CTransitionType CLeakTestTransition
//#define CTransitionType CTransition

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Provide all the CMailtoSchemeHandler specific
	parameters and behaviour on the CMailtoSchemeHandler
	test class for a transition.
 */
class CMailtoSchemeHandler_UnitTestContext : public CUnitTestContext
	{
public:
	/**
		@fn				CMailtoSchemeHandler_UnitTestContext(CDataLogger& aDataLogger,
														MStateAccessor& aStateAccessor,
														MTransitionObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest's Transitions.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class under test.
		@pre 			None.
		@post			CMailtoSchemeHandler_UnitTestContext is fully constructed, and initialised.
	*/
	inline CMailtoSchemeHandler_UnitTestContext(CDataLogger& aDataLogger,
										MStateAccessor& aStateAccessor,
										MTransitionObserver& aObserver);

	/**
		@fn				~CMailtoSchemeHandler_UnitTestContext()
		Intended Usage	: Default Destructor
		@since			6.0
		@pre 			CMailtoSchemeHandler_UnitTestContext is fully constructed.
		@post			CMailtoSchemeHandler_UnitTestContext is fully destroyed
		*/
		virtual inline ~CMailtoSchemeHandler_UnitTestContext();

	/** The instance of the class under test */
	CMailtoSchemeHandler* iMailtoSchemeHandler;
	const TDesC* iUrl;
	TPtrC iBody;
	TBool iVisible;
	TMsvId iId;

	RArray<TPtrC> iArray;
	const TDesC* iSubjectCheck;
	const TDesC* iBodyCheck;

	}; // CMailtoSchemeHandler_UnitTestContext

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Transition test of the CMailtoSchemeHandler::NewL method.
 */
class CMailtoSchemeHandler_NewL_Transition : public CTransitionType
	{
public:
	/**
		@fn				CMailtoSchemeHandler_NewL_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			6.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CMailtoSchemeHandler_NewL_Transition is fully constructed.
	*/
	CMailtoSchemeHandler_NewL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CMailtoSchemeHandler::NewL method for the test harness.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, (@see CMailtoSchemeHandler::NewL)
		@since			6.0
		@return			None
		@pre 			CMailtoSchemeHandler_NewL_Transition is fully constructed.
		@post			No change in the CMailtoSchemeHandler_NewL_Transition apart
						from iMailtoSchemeHandler, which may have changed state.
						(@see CMailtoSchemeHandler::NewL post-condition) for iMailtoSchemeHandler's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			6.0
		@return			The unit test context cast to a CMailtoSchemeHandler_UnitTestContext
		@pre 			iUTContext is a valid CMailtoSchemeHandler_UnitTestContext.
		@post			No change in the CMailtoSchemeHandler_NewL_Transition
	*/
	inline CMailtoSchemeHandler_UnitTestContext& Context() const;
	};	// CMailtoSchemeHandler_NewL_Transition

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Transition test of the CMailtoSchemeHandler::Dtor method.
 */
class CMailtoSchemeHandler_Dtor_Transition : public CTransitionType
	{
public:
	/**
		@fn				CMailtoSchemeHandler_Dtor_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			6.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CMailtoSchemeHandler_Dtor_Transition is fully constructed.
	*/
	CMailtoSchemeHandler_Dtor_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CMailtoSchemeHandler::Dtor method for the test harness.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, (@see CMailtoSchemeHandler::Dtor)
		@since			6.0
		@return			None
		@pre 			CMailtoSchemeHandler_Dtor_Transition is fully constructed.
		@post			No change in the CMailtoSchemeHandler_Dtor_Transition apart
						from iMailtoSchemeHandler, which may have changed state.
						(@see CMailtoSchemeHandler::Dtor post-condition) for iMailtoSchemeHandler's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			6.0
		@return			The unit test context cast to a CMailtoSchemeHandler_UnitTestContext
		@pre 			iUTContext is a valid CMailtoSchemeHandler_UnitTestContext.
		@post			No change in the CMailtoSchemeHandler_Dtor_Transition
	*/
	inline CMailtoSchemeHandler_UnitTestContext& Context() const;
	};	// CMailtoSchemeHandler_Dtor_Transition


// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Transition test of the CMailtoSchemeHandler::ParseUrlL method.
 */
class CMailtoSchemeHandler_ParseUrlL_Transition : public CTransitionType
	{
public:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlL_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			6.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlL_Transition is fully constructed.
	*/
	CMailtoSchemeHandler_ParseUrlL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CMailtoSchemeHandler::ParseUrlL method for the test harness.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, (@see CMailtoSchemeHandler::ParseUrlL)
		@since			6.0
		@return			None
		@pre 			CMailtoSchemeHandler_ParseUrlL_Transition is fully constructed.
		@post			No change in the CMailtoSchemeHandler_ParseUrlL_Transition apart
						from iMailtoSchemeHandler, which may have changed state.
						(@see CMailtoSchemeHandler::ParseUrlL post-condition) for iMailtoSchemeHandler's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			6.0
		@return			The unit test context cast to a CMailtoSchemeHandler_UnitTestContext
		@pre 			iUTContext is a valid CMailtoSchemeHandler_UnitTestContext.
		@post			No change in the CMailtoSchemeHandler_ParseUrlL_Transition
	*/
	inline CMailtoSchemeHandler_UnitTestContext& Context() const;
	};	// CMailtoSchemeHandler_ParseUrlL_Transition




// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Transition test of the CMailtoSchemeHandler::ParseUrlAndSendL method.
 */
class CMailtoSchemeHandler_ParseUrlAndSendL_Transition : public CTransition
	{
public:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlAndSendL_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			6.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlAndSendL_Transition is fully constructed.
	*/
	CMailtoSchemeHandler_ParseUrlAndSendL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CMailtoSchemeHandler::ParseUrlAndSendL method for the test harness.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, (@see CMailtoSchemeHandler::ParseUrlAndSendL)
		@since			6.0
		@return			None
		@pre 			CMailtoSchemeHandler_ParseUrlAndSendL_Transition is fully constructed.
		@post			No change in the CMailtoSchemeHandler_ParseUrlAndSendL_Transition apart
						from iMailtoSchemeHandler, which may have changed state.
						(@see CMailtoSchemeHandler::ParseUrlAndSendL post-condition) for iMailtoSchemeHandler's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			6.0
		@return			The unit test context cast to a CMailtoSchemeHandler_UnitTestContext
		@pre 			iUTContext is a valid CMailtoSchemeHandler_UnitTestContext.
		@post			No change in the CMailtoSchemeHandler_ParseUrlAndSendL_Transition
	*/
	inline CMailtoSchemeHandler_UnitTestContext& Context() const;
	};	// CMailtoSchemeHandler_ParseUrlAndSendL_Transition


// Add additional Transition class definitions here...

#include "MailtoSchemeHandlerTransitions.inl"

#include "MailtoSchemeHandlerUnitTestContext.inl"

#endif // __MAILTOSCHEMEHANDLERTRANSITIONS_H__

