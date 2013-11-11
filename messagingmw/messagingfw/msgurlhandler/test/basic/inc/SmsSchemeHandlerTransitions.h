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
// The definitions of the transition classes upon the CSmsSchemeHandler class methods.
// 
//

/**
 @file
 @SYMCreationDate Created : 01/06/01 14:07:23
*/

#ifndef __SMSSCHEMEHANDLERTRANSITIONS_H__
#define __SMSSCHEMEHANDLERTRANSITIONS_H__

#include <ecom/test_bed/testutilities.h>
#include <ecom/test_bed/stateaccessor.h>
#include <ecom/test_bed/leaktesttransition.h>
#include <ecom/test_bed/datalogger.h>
#include "SMSSCH.H"
#include "SmsSchemeHandlerStateAccessor.h"
#include <ecom/test_bed/unittest.h>

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Provide all the CSmsSchemeHandler specific
	parameters and behaviour on the CSmsSchemeHandler
	test class for a transition.
 */

class CSmsSchemeHandler_UnitTestContext : public CUnitTestContext
	{
public:
	/**
		@fn				CSmsSchemeHandler_UnitTestContext(CDataLogger& aDataLogger,
														MStateAccessor& aStateAccessor,
														MTransitionObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest's Transitions.
		@param			aStateAccessor WhiteBox state access to the CSmsSchemeHandler class under test.
		@pre 			None.
		@post			CSmsSchemeHandler_UnitTestContext is fully constructed, and initialised.
	*/
	inline CSmsSchemeHandler_UnitTestContext(CDataLogger& aDataLogger,
										MStateAccessor& aStateAccessor,
										MTransitionObserver& aObserver);

	/**
		@fn				~CSmsSchemeHandler_UnitTestContext()
		Intended Usage	: Default Destructor
		@since			6.0
		@pre 			CSmsSchemeHandler_UnitTestContext is fully constructed.
		@post			CSmsSchemeHandler_UnitTestContext is fully destroyed
		*/
		virtual inline ~CSmsSchemeHandler_UnitTestContext();

	/** The instance of the class under test */
	CSmsSchemeHandler* iSmsSchemeHandler;

	const TDesC* iUrl;

	struct SSmsNumber
		{
		SSmsNumber() : iNumber(KNullDesC), iServiceCentre(KNullDesC) {}
		TPtrC iNumber;
		TPtrC iServiceCentre;
		};

	RArray<SSmsNumber> iPostCheck;

	}; // CSmsSchemeHandler_UnitTestContext

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Transition test of the CSmsSchemeHandler::NewL method.
 */
class CSmsSchemeHandler_NewL_Transition : public CTransition
	{
public:
	/**
		@fn				CSmsSchemeHandler_NewL_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			6.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CSmsSchemeHandler_NewL_Transition is fully constructed.
	*/
	CSmsSchemeHandler_NewL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CSmsSchemeHandler::NewL method for the test harness.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, (@see CSmsSchemeHandler::NewL)
		@since			6.0
		@return			None
		@pre 			CSmsSchemeHandler_NewL_Transition is fully constructed.
		@post			No change in the CSmsSchemeHandler_NewL_Transition apart
						from iSmsSchemeHandler, which may have changed state.
						(@see CSmsSchemeHandler::NewL post-condition) for iSmsSchemeHandler's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			6.0
		@return			The unit test context cast to a CSmsSchemeHandler_UnitTestContext
		@pre 			iUTContext is a valid CSmsSchemeHandler_UnitTestContext.
		@post			No change in the CSmsSchemeHandler_NewL_Transition
	*/
	inline CSmsSchemeHandler_UnitTestContext& Context() const;
	};	// CSmsSchemeHandler_NewL_Transition

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Transition test of the CSmsSchemeHandler::Dtor method.
 */
class CSmsSchemeHandler_Dtor_Transition : public CTransition
	{
public:
	/**
		@fn				CSmsSchemeHandler_Dtor_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			6.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CSmsSchemeHandler_Dtor_Transition is fully constructed.
	*/
	CSmsSchemeHandler_Dtor_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CSmsSchemeHandler::Dtor method for the test harness.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, (@see CSmsSchemeHandler::Dtor)
		@since			6.0
		@return			None
		@pre 			CSmsSchemeHandler_Dtor_Transition is fully constructed.
		@post			No change in the CSmsSchemeHandler_Dtor_Transition apart
						from iSmsSchemeHandler, which may have changed state.
						(@see CSmsSchemeHandler::Dtor post-condition) for iSmsSchemeHandler's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			6.0
		@return			The unit test context cast to a CSmsSchemeHandler_UnitTestContext
		@pre 			iUTContext is a valid CSmsSchemeHandler_UnitTestContext.
		@post			No change in the CSmsSchemeHandler_Dtor_Transition
	*/
	inline CSmsSchemeHandler_UnitTestContext& Context() const;
	};	// CSmsSchemeHandler_Dtor_Transition


// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Transition test of the CSmsSchemeHandler::ParseUrlL method.
 */
class CSmsSchemeHandler_ParseUrlL_Transition : public CTransition
	{
public:
	/**
		@fn				CSmsSchemeHandler_ParseUrlL_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			6.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlL_Transition is fully constructed.
	*/
	CSmsSchemeHandler_ParseUrlL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CSmsSchemeHandler::ParseUrlL method for the test harness.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, (@see CSmsSchemeHandler::ParseUrlL)
		@since			6.0
		@return			None
		@pre 			CSmsSchemeHandler_ParseUrlL_Transition is fully constructed.
		@post			No change in the CSmsSchemeHandler_ParseUrlL_Transition apart
						from iSmsSchemeHandler, which may have changed state.
						(@see CSmsSchemeHandler::ParseUrlL post-condition) for iSmsSchemeHandler's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			6.0
		@return			The unit test context cast to a CSmsSchemeHandler_UnitTestContext
		@pre 			iUTContext is a valid CSmsSchemeHandler_UnitTestContext.
		@post			No change in the CSmsSchemeHandler_ParseUrlL_Transition
	*/
	inline CSmsSchemeHandler_UnitTestContext& Context() const;
	};	// CSmsSchemeHandler_ParseUrlL_Transition


// Add additional Transition class definitions here...

#include "SmsSchemeHandlerTransitions.inl"

#include "SmsSchemeHandlerUnitTestContext.inl"

#endif // __SMSSCHEMEHANDLERTRANSITIONS_H__

