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
// The unit test class definitions for the CSmsSchemeHandler class.
// 
//

/**
 @file
 @SYMCreationDate Created : 01/06/01 14:07:22
*/
#ifndef __SMSSCHEMEHANDLERUNITTEST_H__
#define __SMSSCHEMEHANDLERUNITTEST_H__

#include <ecom/test_bed/testbeddefinitions.h>
#include "SmsSchemeHandlerStateAccessor.h"
#include "SmsSchemeHandlerTransitionValidation.h"
#include "SmsSchemeHandlerTransitions.h"

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for CreateAndDestroy on CSmsSchemeHandler, the class under test.
 */
class CSmsSchemeHandler_CreateAndDestroy_UnitTest : public CUnitTest
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
		@return			CSmsSchemeHandler_CreateAndDestroy_UnitTest* The constructed object.
		@pre 			None.
		@post			CSmsSchemeHandler_CreateAndDestroy_UnitTest is fully constructed, and initialised.
	 */
	static CSmsSchemeHandler_CreateAndDestroy_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CSmsSchemeHandler_CreateAndDestroy_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CSmsSchemeHandler_CreateAndDestroy_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CSmsSchemeHandler_CreateAndDestroy_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CSmsSchemeHandler_CreateAndDestroy_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_CreateAndDestroy_UnitTest is fully destroyed.
	*/
	~CSmsSchemeHandler_CreateAndDestroy_UnitTest();

private:
	/**
		@fn				CSmsSchemeHandler_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CSmsSchemeHandler class.
		@pre 			None.
		@post			CSmsSchemeHandler_CreateAndDestroy_UnitTest is fully constructed.
	*/
	inline CSmsSchemeHandler_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CSmsSchemeHandler_CreateAndDestroy_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_CreateAndDestroy_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CSmsSchemeHandler class tested by this UnitTest's transitions.
	 */
	CSmsSchemeHandler_UnitTestContext* iUTContext;
	TSmsSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TSmsSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	//TSmsSchemeHandler_CreateAndDestroy_TransitionValidator*	iCreateAndDestroyValidator;
	TSmsSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CSmsSchemeHandler_CreateAndDestroy_UnitTest


// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest1 on CSmsSchemeHandler, the class under test.
 */
class CSmsSchemeHandler_ParseUrlTest1_UnitTest : public CUnitTest
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
		@return			CSmsSchemeHandler_ParseUrlTest1_UnitTest* The constructed object.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest1_UnitTest is fully constructed, and initialised.
	 */
	static CSmsSchemeHandler_ParseUrlTest1_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CSmsSchemeHandler_ParseUrlTest1_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CSmsSchemeHandler_ParseUrlTest1_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CSmsSchemeHandler_ParseUrlTest1_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CSmsSchemeHandler_ParseUrlTest1_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest1_UnitTest is fully destroyed.
	*/
	~CSmsSchemeHandler_ParseUrlTest1_UnitTest();

private:
	/**
		@fn				CSmsSchemeHandler_ParseUrlTest1_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CSmsSchemeHandler class.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest1_UnitTest is fully constructed.
	*/
	inline CSmsSchemeHandler_ParseUrlTest1_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CSmsSchemeHandler_ParseUrlTest1_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest1_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CSmsSchemeHandler class tested by this UnitTest's transitions.
	 */
	CSmsSchemeHandler_UnitTestContext* iUTContext;
	TSmsSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TSmsSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TSmsSchemeHandler_ParseUrlL_TransitionValidator*	iParseUrlTest1Validator;
	TSmsSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	};	// CSmsSchemeHandler_ParseUrlTest1_UnitTest


// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest2 on CSmsSchemeHandler, the class under test.
 */
class CSmsSchemeHandler_ParseUrlTest2_UnitTest : public CUnitTest
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
		@return			CSmsSchemeHandler_ParseUrlTest2_UnitTest* The constructed object.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest2_UnitTest is fully constructed, and initialised.
	 */
	static CSmsSchemeHandler_ParseUrlTest2_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CSmsSchemeHandler_ParseUrlTest2_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CSmsSchemeHandler_ParseUrlTest2_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CSmsSchemeHandler_ParseUrlTest2_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CSmsSchemeHandler_ParseUrlTest2_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest2_UnitTest is fully destroyed.
	*/
	~CSmsSchemeHandler_ParseUrlTest2_UnitTest();

private:
	/**
		@fn				CSmsSchemeHandler_ParseUrlTest2_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CSmsSchemeHandler class.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest2_UnitTest is fully constructed.
	*/
	inline CSmsSchemeHandler_ParseUrlTest2_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CSmsSchemeHandler_ParseUrlTest2_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest2_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CSmsSchemeHandler class tested by this UnitTest's transitions.
	 */
	CSmsSchemeHandler_UnitTestContext* iUTContext;
	TSmsSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TSmsSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TSmsSchemeHandler_ParseUrlL_TransitionValidator*	iParseUrlTest2Validator;
	TSmsSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;

	};	// CSmsSchemeHandler_ParseUrlTest2_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest3 on CSmsSchemeHandler, the class under test.
 */
class CSmsSchemeHandler_ParseUrlTest3_UnitTest : public CUnitTest
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
		@return			CSmsSchemeHandler_ParseUrlTest3_UnitTest* The constructed object.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest3_UnitTest is fully constructed, and initialised.
	 */
	static CSmsSchemeHandler_ParseUrlTest3_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CSmsSchemeHandler_ParseUrlTest3_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CSmsSchemeHandler_ParseUrlTest3_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CSmsSchemeHandler_ParseUrlTest3_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CSmsSchemeHandler_ParseUrlTest3_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest3_UnitTest is fully destroyed.
	*/
	~CSmsSchemeHandler_ParseUrlTest3_UnitTest();

private:
	/**
		@fn				CSmsSchemeHandler_ParseUrlTest3_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CSmsSchemeHandler class.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest3_UnitTest is fully constructed.
	*/
	inline CSmsSchemeHandler_ParseUrlTest3_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CSmsSchemeHandler_ParseUrlTest3_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest3_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CSmsSchemeHandler class tested by this UnitTest's transitions.
	 */
	CSmsSchemeHandler_UnitTestContext* iUTContext;
	TSmsSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TSmsSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TSmsSchemeHandler_ParseUrlL_TransitionValidator*	iParseUrlTest3Validator;
	TSmsSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;

	};	// CSmsSchemeHandler_ParseUrlTest3_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest4 on CSmsSchemeHandler, the class under test.
 */
class CSmsSchemeHandler_ParseUrlTest4_UnitTest : public CUnitTest
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
		@return			CSmsSchemeHandler_ParseUrlTest4_UnitTest* The constructed object.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest4_UnitTest is fully constructed, and initialised.
	 */
	static CSmsSchemeHandler_ParseUrlTest4_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CSmsSchemeHandler_ParseUrlTest4_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CSmsSchemeHandler_ParseUrlTest4_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CSmsSchemeHandler_ParseUrlTest4_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CSmsSchemeHandler_ParseUrlTest4_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest4_UnitTest is fully destroyed.
	*/
	~CSmsSchemeHandler_ParseUrlTest4_UnitTest();

private:
	/**
		@fn				CSmsSchemeHandler_ParseUrlTest4_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CSmsSchemeHandler class.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest4_UnitTest is fully constructed.
	*/
	inline CSmsSchemeHandler_ParseUrlTest4_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CSmsSchemeHandler_ParseUrlTest4_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest4_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CSmsSchemeHandler class tested by this UnitTest's transitions.
	 */
	CSmsSchemeHandler_UnitTestContext* iUTContext;
	TSmsSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TSmsSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TSmsSchemeHandler_ParseUrlL_TransitionValidator*	iParseUrlTest4Validator;
	TSmsSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;

	};	// CSmsSchemeHandler_ParseUrlTest4_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest5 on CSmsSchemeHandler, the class under test.
 */
class CSmsSchemeHandler_ParseUrlTest5_UnitTest : public CUnitTest
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
		@return			CSmsSchemeHandler_ParseUrlTest5_UnitTest* The constructed object.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest5_UnitTest is fully constructed, and initialised.
	 */
	static CSmsSchemeHandler_ParseUrlTest5_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CSmsSchemeHandler_ParseUrlTest5_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CSmsSchemeHandler_ParseUrlTest5_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CSmsSchemeHandler_ParseUrlTest5_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CSmsSchemeHandler_ParseUrlTest5_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest5_UnitTest is fully destroyed.
	*/
	~CSmsSchemeHandler_ParseUrlTest5_UnitTest();

private:
	/**
		@fn				CSmsSchemeHandler_ParseUrlTest5_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CSmsSchemeHandler class.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest5_UnitTest is fully constructed.
	*/
	inline CSmsSchemeHandler_ParseUrlTest5_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CSmsSchemeHandler_ParseUrlTest5_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest5_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CSmsSchemeHandler class tested by this UnitTest's transitions.
	 */
	CSmsSchemeHandler_UnitTestContext* iUTContext;
	TSmsSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TSmsSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TSmsSchemeHandler_ParseUrlL_TransitionValidator*	iParseUrlTest5Validator;
	TSmsSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;
	
	};	// CSmsSchemeHandler_ParseUrlTest5_UnitTest

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Unit Test for ParseUrlTest6 on CSmsSchemeHandler, the class under test.
 */
class CSmsSchemeHandler_ParseUrlTest6_UnitTest : public CUnitTest
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
		@return			CSmsSchemeHandler_ParseUrlTest6_UnitTest* The constructed object.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest6_UnitTest is fully constructed, and initialised.
	 */
	static CSmsSchemeHandler_ParseUrlTest6_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CSmsSchemeHandler_ParseUrlTest6_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			6.0
		@return			TInt KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CSmsSchemeHandler_ParseUrlTest6_UnitTest is fully constructed, and initialised.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CSmsSchemeHandler_ParseUrlTest6_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			6.0
		@pre 			CSmsSchemeHandler_ParseUrlTest6_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest6_UnitTest is fully destroyed.
	*/
	~CSmsSchemeHandler_ParseUrlTest6_UnitTest();

private:
	/**
		@fn				CSmsSchemeHandler_ParseUrlTest6_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CSmsSchemeHandler class.
		@pre 			None.
		@post			CSmsSchemeHandler_ParseUrlTest6_UnitTest is fully constructed.
	*/
	inline CSmsSchemeHandler_ParseUrlTest6_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			6.0
		@return			None 
		@pre 			CSmsSchemeHandler_ParseUrlTest6_UnitTest is fully constructed.
		@post			CSmsSchemeHandler_ParseUrlTest6_UnitTest is fully initialised.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CSmsSchemeHandler class tested by this UnitTest's transitions.
	 */
	CSmsSchemeHandler_UnitTestContext* iUTContext;
	TSmsSchemeHandler_StateAccessor*	iStateAccessor;
	// C'tor, d'tor, and method transition validators
	TSmsSchemeHandler_Ctor_TransitionValidator*	iCtorValidator;
	TSmsSchemeHandler_ParseUrlL_TransitionValidator*	iParseUrlTest6Validator;
	TSmsSchemeHandler_Dtor_TransitionValidator*	iDtorValidator;

	};	// CSmsSchemeHandler_ParseUrlTest6_UnitTest


#endif	// __SMSSCHEMEHANDLERUNITTEST_H__
