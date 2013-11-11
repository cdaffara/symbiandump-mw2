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
// The unit test class definitions for the CMailtoSchemeHandler class.
// 
//

/**
 @file
 @SYMCreationDate Created : 24/05/01 15:57:45
*/
#ifndef __MAILTOSCHEMEHANDLERUNITTEST_H__
#define __MAILTOSCHEMEHANDLERUNITTEST_H__

#include <ecom/test_bed/testbeddefinitions.h>
#include "MailtoSchemeHandlerStateAccessor.h"
#include "MailtoSchemeHandlerTransitionValidation.h"
#include "MailtoSchemeHandlerTransitions.h"

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for CreateAndDestroy on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_CreateAndDestroy_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_CreateAndDestroy_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_CreateAndDestroy_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_CreateAndDestroy_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_CreateAndDestroy_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_CreateAndDestroy_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_CreateAndDestroy_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_CreateAndDestroy_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_CreateAndDestroy_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_CreateAndDestroy_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_CreateAndDestroy_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_CreateAndDestroy_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_CreateAndDestroy_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	//TMailtoSchemeHandler_CreateAndDestroy_TransitionValidator*	iCreateAndDestroyValidator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	
	};	// CMailtoSchemeHandler_CreateAndDestroy_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest1 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest1_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest1_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest1_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest1_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest1_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest1_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest1_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest1_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest1_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest1_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest1_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest1_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest1_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest1_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest1_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator*	iParseUrlTest1Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest1_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest2 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest2_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest2_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest2_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest2_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest2_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest2_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest2_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest2_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest2_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest2_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest2_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest2_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest2_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest2_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest2_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator*	iParseUrlTest2Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;

	};	// CMailtoSchemeHandler_ParseUrlTest2_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest3 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest3_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest3_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest3_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest3_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest3_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest3_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest3_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest3_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest3_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest3_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest3_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest3_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest3_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest3_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest3_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator*	iParseUrlTest3Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest3_UnitTest



// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest4 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest4_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest4_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest4_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest4_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest4_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest4_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest4_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest4_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest4_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest4_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest4_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest4_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest4_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest4_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest4_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator*	iParseUrlTest4Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest4_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest5 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest5_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest5_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest5_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest5_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest5_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest5_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest5_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest5_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest5_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest5_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest5_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest5_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest5_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest5_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest5_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator*	iParseUrlTest5Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest5_UnitTest


// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest6 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest6_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest6_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest6_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest6_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest6_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest6_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest6_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest6_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest6_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest6_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest6_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest6_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest6_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest6_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest6_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor* iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator*	iParseUrlTest6Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;	
	};	// CMailtoSchemeHandler_ParseUrlTest6_UnitTest


// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest7 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest7_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest7_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest7_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest7_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest7_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest7_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest7_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest7_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest7_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest7_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest7_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest7_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest7_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest7_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest7_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator*	iParseUrlTest7Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest7_UnitTest



// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest8 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest8_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest8_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest8_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest8_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest8_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest8_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest8_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest8_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest8_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest8_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest8_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest8_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest8_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest8_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest8_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator*	iParseUrlTest8Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest8_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest9 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest9_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest9_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest9_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest9_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest9_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest9_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest9_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest9_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest9_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest9_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest9_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest9_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest9_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest9_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest9_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator*	iParseUrlTest9Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest9_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest10 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest10_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest10_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest10_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest10_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest10_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest10_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest10_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest10_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest10_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest10_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest10_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest10_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest10_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest10_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest10_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator*	iParseUrlTest10Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest10_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest11 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest11_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest11_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest11_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest11_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest11_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest11_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest11_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest11_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest11_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest11_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest11_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest11_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest11_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest11_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest11_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator*	iParseUrlTest11Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest11_UnitTest


// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest12 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest12_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest12_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest12_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest12_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest12_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest12_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest12_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest12_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest12_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest12_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest12_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest12_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest12_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest12_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest12_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator*	iParseUrlTest12Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest12_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest13 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest13_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest13_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest13_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest13_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest13_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest13_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest13_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest13_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest13_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest13_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest13_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest13_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest13_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest13_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest13_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator*	iParseUrlTest13Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest13_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest14 on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_ParseUrlTest14_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_ParseUrlTest14_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest14_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_ParseUrlTest14_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_ParseUrlTest14_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_ParseUrlTest14_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_ParseUrlTest14_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_ParseUrlTest14_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest14_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_ParseUrlTest14_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_ParseUrlTest14_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_ParseUrlTest14_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ParseUrlTest14_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ParseUrlTest14_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_ParseUrlTest14_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator*	iParseUrlTest14Validator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_ParseUrlTest14_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class

	Comments : Unit Test for CreateEmailL on CMailtoSchemeHandler, the class under test.
 */
class CMailtoSchemeHandler_CreateEmailL_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CMailtoSchemeHandler_CreateEmailL_UnitTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_CreateEmailL_UnitTest is fully constructed, and initialised.
	 */
	static CMailtoSchemeHandler_CreateEmailL_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CMailtoSchemeHandler_CreateEmailL_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CMailtoSchemeHandler_CreateEmailL_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CMailtoSchemeHandler_CreateEmailL_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CMailtoSchemeHandler_CreateEmailL_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_CreateEmailL_UnitTest is fully destroyed.
	*/
	~CMailtoSchemeHandler_CreateEmailL_UnitTest();

private:
	/**
		@fn				CMailtoSchemeHandler_CreateEmailL_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CMailtoSchemeHandler class.
		@pre 			None.
		@post			CMailtoSchemeHandler_CreateEmailL_UnitTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_CreateEmailL_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_CreateEmailL_UnitTest is fully constructed.
		@post			CMailtoSchemeHandler_CreateEmailL_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CMailtoSchemeHandler class tested by this UnitTest's transitions.
	 */
	CMailtoSchemeHandler_UnitTestContext* iUTContext;
	TMailtoSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TMailtoSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator*	iParseUrlTest14Validator;
	TMailtoSchemeHandler_CreateEmailL_TransitionValidator*	iCreateEmailLValidator;
	TMailtoSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CMailtoSchemeHandler_CreateEmailL_UnitTest


#endif	// __MAILTOSCHEMEHANDLERUNITTEST_H__
