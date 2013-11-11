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
// The unit test class implementations for the CSmsSchemeHandler class.
// 
//

/**
 @file
 @SYMCreationDate Created : 01/06/01 14:07:22
*/

#include "SmsSchemeHandlerUnitTest.h"

// ______________________________________________________________________________
//
_LIT(KSmsSchemeHandlerCreateAndDestroyUnitTest,"CSmsSchemeHandler_CreateAndDestroy_UnitTest");

CSmsSchemeHandler_CreateAndDestroy_UnitTest* CSmsSchemeHandler_CreateAndDestroy_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CSmsSchemeHandler_CreateAndDestroy_UnitTest* self = 
					new(ELeave) CSmsSchemeHandler_CreateAndDestroy_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CSmsSchemeHandler_CreateAndDestroy_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	//delete iCreateAndDestroyValidator;
	delete iDtorValidator;
	 
	return error;
	}

inline CSmsSchemeHandler_CreateAndDestroy_UnitTest::~CSmsSchemeHandler_CreateAndDestroy_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	//delete iCreateAndDestroyValidator;
	delete iDtorValidator;
	 
	}

inline CSmsSchemeHandler_CreateAndDestroy_UnitTest::CSmsSchemeHandler_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KSmsSchemeHandlerCreateAndDestroyUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CSmsSchemeHandler_CreateAndDestroy_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TSmsSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CSmsSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CSmsSchemeHandler class.
	//	using ctor and dtor validators
	iCtorValidator = new(ELeave) TSmsSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CSmsSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	iDtorValidator = new(ELeave) TSmsSchemeHandler_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CSmsSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator)); 
	}



// ______________________________________________________________________________
//
_LIT(KSmsSchemeHandlerParseUrlTest1UnitTest,"CSmsSchemeHandler_ParseUrlTest1_UnitTest");

CSmsSchemeHandler_ParseUrlTest1_UnitTest* CSmsSchemeHandler_ParseUrlTest1_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CSmsSchemeHandler_ParseUrlTest1_UnitTest* self = 
					new(ELeave) CSmsSchemeHandler_ParseUrlTest1_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CSmsSchemeHandler_ParseUrlTest1_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest1Validator;
	delete iDtorValidator;

	return error;
	}

inline CSmsSchemeHandler_ParseUrlTest1_UnitTest::~CSmsSchemeHandler_ParseUrlTest1_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest1Validator;
	delete iDtorValidator;
	}

inline CSmsSchemeHandler_ParseUrlTest1_UnitTest::CSmsSchemeHandler_ParseUrlTest1_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KSmsSchemeHandlerParseUrlTest1UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CSmsSchemeHandler_ParseUrlTest1_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TSmsSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CSmsSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CSmsSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl1, "sms: 1234 ");
	//_LIT(KUrl1, "sms: nonsense ");
	iUTContext->iUrl = &KUrl1(); 

	_LIT(KNumber, "1234");

	CSmsSchemeHandler_UnitTestContext::SSmsNumber num;
	num.iNumber.Set(KNumber);

	iUTContext->iPostCheck.Append(num);

	iCtorValidator = new(ELeave) TSmsSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest1Validator = new(ELeave) TSmsSchemeHandler_ParseUrlL_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TSmsSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CSmsSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest1Validator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KSmsSchemeHandlerParseUrlTest2UnitTest,"CSmsSchemeHandler_ParseUrlTest2_UnitTest");

CSmsSchemeHandler_ParseUrlTest2_UnitTest* CSmsSchemeHandler_ParseUrlTest2_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CSmsSchemeHandler_ParseUrlTest2_UnitTest* self = 
					new(ELeave) CSmsSchemeHandler_ParseUrlTest2_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CSmsSchemeHandler_ParseUrlTest2_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest2Validator;
	delete iDtorValidator;
	 
	return error;
	}

inline CSmsSchemeHandler_ParseUrlTest2_UnitTest::~CSmsSchemeHandler_ParseUrlTest2_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest2Validator;
	delete iDtorValidator;
	 
	}

inline CSmsSchemeHandler_ParseUrlTest2_UnitTest::CSmsSchemeHandler_ParseUrlTest2_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KSmsSchemeHandlerParseUrlTest2UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CSmsSchemeHandler_ParseUrlTest2_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TSmsSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CSmsSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CSmsSchemeHandler class.
	//	using ctor and dtor validators
	 
	_LIT(KUrl2, "sms: 3452 ; via = 5687 ");
	iUTContext->iUrl = &KUrl2(); 

	_LIT(KNumber, "3452");
	_LIT(KServiceCentre, "5687");

	CSmsSchemeHandler_UnitTestContext::SSmsNumber num;
	num.iNumber.Set(KNumber);
	num.iServiceCentre.Set(KServiceCentre);

	iUTContext->iPostCheck.Append(num);

	iCtorValidator = new(ELeave) TSmsSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest2Validator = new(ELeave) TSmsSchemeHandler_ParseUrlL_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TSmsSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CSmsSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest2Validator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KSmsSchemeHandlerParseUrlTest3UnitTest,"CSmsSchemeHandler_ParseUrlTest3_UnitTest");

CSmsSchemeHandler_ParseUrlTest3_UnitTest* CSmsSchemeHandler_ParseUrlTest3_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CSmsSchemeHandler_ParseUrlTest3_UnitTest* self = 
					new(ELeave) CSmsSchemeHandler_ParseUrlTest3_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CSmsSchemeHandler_ParseUrlTest3_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest3Validator;
	delete iDtorValidator;
	 
	return error;
	}

inline CSmsSchemeHandler_ParseUrlTest3_UnitTest::~CSmsSchemeHandler_ParseUrlTest3_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest3Validator;
	delete iDtorValidator;
	 
	}

inline CSmsSchemeHandler_ParseUrlTest3_UnitTest::CSmsSchemeHandler_ParseUrlTest3_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KSmsSchemeHandlerParseUrlTest3UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CSmsSchemeHandler_ParseUrlTest3_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TSmsSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CSmsSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CSmsSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl3, "sms: ; via = 5687 ");
	iUTContext->iUrl = &KUrl3(); 

	_LIT(KServiceCentre, "5687");

	CSmsSchemeHandler_UnitTestContext::SSmsNumber num;

	num.iServiceCentre.Set(KServiceCentre);

	iUTContext->iPostCheck.Append(num);

	iCtorValidator = new(ELeave) TSmsSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest3Validator = new(ELeave) TSmsSchemeHandler_ParseUrlL_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TSmsSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CSmsSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest3Validator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KSmsSchemeHandlerParseUrlTest4UnitTest,"CSmsSchemeHandler_ParseUrlTest4_UnitTest");

CSmsSchemeHandler_ParseUrlTest4_UnitTest* CSmsSchemeHandler_ParseUrlTest4_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CSmsSchemeHandler_ParseUrlTest4_UnitTest* self = 
					new(ELeave) CSmsSchemeHandler_ParseUrlTest4_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CSmsSchemeHandler_ParseUrlTest4_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest4Validator;
	delete iDtorValidator;
	 
	return error;
	}

inline CSmsSchemeHandler_ParseUrlTest4_UnitTest::~CSmsSchemeHandler_ParseUrlTest4_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest4Validator;
	delete iDtorValidator;
	 
	}

inline CSmsSchemeHandler_ParseUrlTest4_UnitTest::CSmsSchemeHandler_ParseUrlTest4_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KSmsSchemeHandlerParseUrlTest4UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CSmsSchemeHandler_ParseUrlTest4_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TSmsSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CSmsSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CSmsSchemeHandler class.
	//	using ctor and dtor validators
	
	_LIT(KUrl4, "sms: 4 , +441712081834 , +1800000000 ");
	iUTContext->iUrl = &KUrl4(); 

	_LIT(KNumber1, "4");
	_LIT(KNumber2, "+441712081834");
	_LIT(KNumber3, "+1800000000");

	CSmsSchemeHandler_UnitTestContext::SSmsNumber num1;
	CSmsSchemeHandler_UnitTestContext::SSmsNumber num2;
	CSmsSchemeHandler_UnitTestContext::SSmsNumber num3;

	num1.iNumber.Set(KNumber1);
	num2.iNumber.Set(KNumber2);
	num3.iNumber.Set(KNumber3);

	iUTContext->iPostCheck.Append(num1);
	iUTContext->iPostCheck.Append(num2);
	iUTContext->iPostCheck.Append(num3);

	iCtorValidator = new(ELeave) TSmsSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest4Validator = new(ELeave) TSmsSchemeHandler_ParseUrlL_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TSmsSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CSmsSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest4Validator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KSmsSchemeHandlerParseUrlTest5UnitTest,"CSmsSchemeHandler_ParseUrlTest5_UnitTest");

CSmsSchemeHandler_ParseUrlTest5_UnitTest* CSmsSchemeHandler_ParseUrlTest5_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CSmsSchemeHandler_ParseUrlTest5_UnitTest* self = 
					new(ELeave) CSmsSchemeHandler_ParseUrlTest5_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CSmsSchemeHandler_ParseUrlTest5_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest5Validator;
	delete iDtorValidator;
	 
	return error;
	}

inline CSmsSchemeHandler_ParseUrlTest5_UnitTest::~CSmsSchemeHandler_ParseUrlTest5_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest5Validator;
	delete iDtorValidator;
	 
	}

inline CSmsSchemeHandler_ParseUrlTest5_UnitTest::CSmsSchemeHandler_ParseUrlTest5_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KSmsSchemeHandlerParseUrlTest5UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CSmsSchemeHandler_ParseUrlTest5_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TSmsSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CSmsSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CSmsSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl5, "sms: 4 ; via = +44468700700 , +441712081834 , +1800000000 ; via = 909 ");
	iUTContext->iUrl = &KUrl5(); 

	_LIT(KNumber1, "4");
	_LIT(KServiceCentre1, "+44468700700");
	_LIT(KNumber2, "+441712081834");
	_LIT(KNumber3, "+1800000000");
	_LIT(KServiceCentre3, "909");

	CSmsSchemeHandler_UnitTestContext::SSmsNumber num1;
	CSmsSchemeHandler_UnitTestContext::SSmsNumber num2;
	CSmsSchemeHandler_UnitTestContext::SSmsNumber num3;

	num1.iNumber.Set(KNumber1);
	num1.iServiceCentre.Set(KServiceCentre1);
	num2.iNumber.Set(KNumber2);
	num3.iNumber.Set(KNumber3);
	num3.iServiceCentre.Set(KServiceCentre3);

	iUTContext->iPostCheck.Append(num1);
	iUTContext->iPostCheck.Append(num2);
	iUTContext->iPostCheck.Append(num3);

	iCtorValidator = new(ELeave) TSmsSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest5Validator = new(ELeave) TSmsSchemeHandler_ParseUrlL_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TSmsSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CSmsSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest5Validator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));

	}

// ______________________________________________________________________________
//
_LIT(KSmsSchemeHandlerParseUrlTest6UnitTest,"CSmsSchemeHandler_ParseUrlTest6_UnitTest");

CSmsSchemeHandler_ParseUrlTest6_UnitTest* CSmsSchemeHandler_ParseUrlTest6_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CSmsSchemeHandler_ParseUrlTest6_UnitTest* self = 
					new(ELeave) CSmsSchemeHandler_ParseUrlTest6_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CSmsSchemeHandler_ParseUrlTest6_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest6Validator;
	delete iDtorValidator;
	 
	return error;
	}

inline CSmsSchemeHandler_ParseUrlTest6_UnitTest::~CSmsSchemeHandler_ParseUrlTest6_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest6Validator;
	delete iDtorValidator;
	 
	}

inline CSmsSchemeHandler_ParseUrlTest6_UnitTest::CSmsSchemeHandler_ParseUrlTest6_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KSmsSchemeHandlerParseUrlTest6UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CSmsSchemeHandler_ParseUrlTest6_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TSmsSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CSmsSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...

	_LIT(KUrl6, "sms: 4 ; via = +44468700700 , +441712081834 ; via = , +1800000000 ; via = 909 ");
	iUTContext->iUrl = &KUrl6(); 

	_LIT(KNumber1, "4");
	_LIT(KServiceCentre1, "+44468700700");
	_LIT(KNumber2, "+441712081834");
	_LIT(KNumber3, "+1800000000");
	_LIT(KServiceCentre3, "909");

	CSmsSchemeHandler_UnitTestContext::SSmsNumber num1;
	CSmsSchemeHandler_UnitTestContext::SSmsNumber num2;
	CSmsSchemeHandler_UnitTestContext::SSmsNumber num3;

	num1.iNumber.Set(KNumber1);
	num1.iServiceCentre.Set(KServiceCentre1);
	num2.iNumber.Set(KNumber2);
	num3.iNumber.Set(KNumber3);
	num3.iServiceCentre.Set(KServiceCentre3);

	iUTContext->iPostCheck.Append(num1);
	iUTContext->iPostCheck.Append(num2);
	iUTContext->iPostCheck.Append(num3);

	iCtorValidator = new(ELeave) TSmsSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest6Validator = new(ELeave) TSmsSchemeHandler_ParseUrlL_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TSmsSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CSmsSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest6Validator));
	AddTransitionL(new(ELeave)CSmsSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));

	}

