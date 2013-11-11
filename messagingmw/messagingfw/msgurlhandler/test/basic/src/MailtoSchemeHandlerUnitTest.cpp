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
// The unit test class implementations for the CMailtoSchemeHandler class.
// 
//

/**
 @file
 @SYMCreationDate Created : 24/05/01 15:57:45
*/

#include "MailtoSchemeHandlerUnitTest.h"

// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerCreateAndDestroyUnitTest,"CMailtoSchemeHandler_CreateAndDestroy_UnitTest");

CMailtoSchemeHandler_CreateAndDestroy_UnitTest* CMailtoSchemeHandler_CreateAndDestroy_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_CreateAndDestroy_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_CreateAndDestroy_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_CreateAndDestroy_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
		delete iCtorValidator;
	//	delete iCreateAndDestroyValidator;
		delete iDtorValidator;
	 
	return error;
	}

inline CMailtoSchemeHandler_CreateAndDestroy_UnitTest::~CMailtoSchemeHandler_CreateAndDestroy_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
		delete iCtorValidator;
	//	delete iCreateAndDestroyValidator;
		delete iDtorValidator;
	
	}

inline CMailtoSchemeHandler_CreateAndDestroy_UnitTest::CMailtoSchemeHandler_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerCreateAndDestroyUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_CreateAndDestroy_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators
		iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
		AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
//		iCreateAndDestroyValidator = new(ELeave) TMailtoSchemeHandler_CreateAndDestroy_TransitionValidator(*iUTContext);
//		AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
		iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);
		AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	
	}

// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest1UnitTest,"CMailtoSchemeHandler_ParseUrlTest1_UnitTest");

CMailtoSchemeHandler_ParseUrlTest1_UnitTest* CMailtoSchemeHandler_ParseUrlTest1_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest1_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest1_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest1_UnitTest::RunError(TInt aError)
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

inline CMailtoSchemeHandler_ParseUrlTest1_UnitTest::~CMailtoSchemeHandler_ParseUrlTest1_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest1Validator;
	delete iDtorValidator;
	
	}

inline CMailtoSchemeHandler_ParseUrlTest1_UnitTest::CMailtoSchemeHandler_ParseUrlTest1_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest1UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest1_UnitTest::ConstructL()
	{

	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators
	
	_LIT(KUrl1, "mailto: shahadat@symbian , david@symbian ");
	//_LIT(KUrl1, "mailto: ");
	iUTContext->iUrl = &KUrl1(); 

	_LIT(KTo1, "shahadat@symbian");
	_LIT(KTo2, "david@symbian");

	iUTContext->iArray.Append(KTo1());
	iUTContext->iArray.Append(KTo2());
	
	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest1Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	//AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlAndSendL_Transition(*iUTContext,*iParseUrlTest1Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest1Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}


// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest2UnitTest,"CMailtoSchemeHandler_ParseUrlTest2_UnitTest");

CMailtoSchemeHandler_ParseUrlTest2_UnitTest* CMailtoSchemeHandler_ParseUrlTest2_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest2_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest2_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest2_UnitTest::RunError(TInt aError)
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

inline CMailtoSchemeHandler_ParseUrlTest2_UnitTest::~CMailtoSchemeHandler_ParseUrlTest2_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest2Validator;
	delete iDtorValidator;
	}

inline CMailtoSchemeHandler_ParseUrlTest2_UnitTest::CMailtoSchemeHandler_ParseUrlTest2_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest2UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest2_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl2, "mailto: shahadat@symbian%2cdavid@symbian");
	iUTContext->iUrl = &KUrl2(); 

	_LIT(KTo1, "shahadat@symbian");
	_LIT(KTo2, "david@symbian");

	iUTContext->iArray.Append(KTo1());
	iUTContext->iArray.Append(KTo2());

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest2Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest2Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest3UnitTest,"CMailtoSchemeHandler_ParseUrlTest3_UnitTest");

CMailtoSchemeHandler_ParseUrlTest3_UnitTest* CMailtoSchemeHandler_ParseUrlTest3_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest3_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest3_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest3_UnitTest::RunError(TInt aError)
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

inline CMailtoSchemeHandler_ParseUrlTest3_UnitTest::~CMailtoSchemeHandler_ParseUrlTest3_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest3Validator;
	delete iDtorValidator;
	}

inline CMailtoSchemeHandler_ParseUrlTest3_UnitTest::CMailtoSchemeHandler_ParseUrlTest3_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest3UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest3_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators
	
	_LIT(KUrl3, "mailto: shahadat@symbian ? to = david@symbian ");
	iUTContext->iUrl = &KUrl3(); 

	_LIT(KTo1, "shahadat@symbian");
	_LIT(KTo2, "david@symbian");

	iUTContext->iArray.Append(KTo1());
	iUTContext->iArray.Append(KTo2());

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest3Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest3Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}


// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest4UnitTest,"CMailtoSchemeHandler_ParseUrlTest4_UnitTest");

CMailtoSchemeHandler_ParseUrlTest4_UnitTest* CMailtoSchemeHandler_ParseUrlTest4_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest4_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest4_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest4_UnitTest::RunError(TInt aError)
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

inline CMailtoSchemeHandler_ParseUrlTest4_UnitTest::~CMailtoSchemeHandler_ParseUrlTest4_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest4Validator;
	delete iDtorValidator;
	
	}

inline CMailtoSchemeHandler_ParseUrlTest4_UnitTest::CMailtoSchemeHandler_ParseUrlTest4_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest4UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest4_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators
	
	_LIT(KUrl4, "mailto: shahadat@symbian ? cc = david@symbian ");
	iUTContext->iUrl = &KUrl4(); 

	_LIT(KTo1, "shahadat@symbian");
	_LIT(KTo2, "david@symbian");

	iUTContext->iArray.Append(KTo1());
	iUTContext->iArray.Append(KTo2());

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest4Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest4Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}


// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest5UnitTest,"CMailtoSchemeHandler_ParseUrlTest5_UnitTest");

CMailtoSchemeHandler_ParseUrlTest5_UnitTest* CMailtoSchemeHandler_ParseUrlTest5_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest5_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest5_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest5_UnitTest::RunError(TInt aError)
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

inline CMailtoSchemeHandler_ParseUrlTest5_UnitTest::~CMailtoSchemeHandler_ParseUrlTest5_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest5Validator;
	delete iDtorValidator;
	}

inline CMailtoSchemeHandler_ParseUrlTest5_UnitTest::CMailtoSchemeHandler_ParseUrlTest5_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest5UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest5_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl5, "mailto: shahadat@symbian ? bcc = david@symbian ");
	iUTContext->iUrl = &KUrl5(); 

	_LIT(KTo1, "shahadat@symbian");
	_LIT(KTo2, "david@symbian");

	iUTContext->iArray.Append(KTo1());
	iUTContext->iArray.Append(KTo2());

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest5Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest5Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}


// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest6UnitTest,"CMailtoSchemeHandler_ParseUrlTest6_UnitTest");

CMailtoSchemeHandler_ParseUrlTest6_UnitTest* CMailtoSchemeHandler_ParseUrlTest6_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest6_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest6_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest6_UnitTest::RunError(TInt aError)
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

inline CMailtoSchemeHandler_ParseUrlTest6_UnitTest::~CMailtoSchemeHandler_ParseUrlTest6_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest6Validator;
	delete iDtorValidator;
	 
	}

inline CMailtoSchemeHandler_ParseUrlTest6_UnitTest::CMailtoSchemeHandler_ParseUrlTest6_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest6UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest6_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl6, "mailto: ? to = david@symbian ");
	iUTContext->iUrl = &KUrl6(); 

	_LIT(KTo1, "david@symbian");

	iUTContext->iArray.Append(KTo1());

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest6Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest6Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));

	}

// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest7UnitTest,"CMailtoSchemeHandler_ParseUrlTest7_UnitTest");

CMailtoSchemeHandler_ParseUrlTest7_UnitTest* CMailtoSchemeHandler_ParseUrlTest7_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest7_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest7_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest7_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest7Validator;
	delete iDtorValidator;
	 
	return error;
	}

inline CMailtoSchemeHandler_ParseUrlTest7_UnitTest::~CMailtoSchemeHandler_ParseUrlTest7_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest7Validator;
	delete iDtorValidator;
	 
	}

inline CMailtoSchemeHandler_ParseUrlTest7_UnitTest::CMailtoSchemeHandler_ParseUrlTest7_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest7UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest7_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl7, "mailto: ? cc = david@symbian ");
	iUTContext->iUrl = &KUrl7(); 

	_LIT(KTo1, "david@symbian");

	iUTContext->iArray.Append(KTo1());

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest7Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest7Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}


// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest8UnitTest,"CMailtoSchemeHandler_ParseUrlTest8_UnitTest");

CMailtoSchemeHandler_ParseUrlTest8_UnitTest* CMailtoSchemeHandler_ParseUrlTest8_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest8_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest8_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest8_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest8Validator;
	delete iDtorValidator;

	return error;
	}

inline CMailtoSchemeHandler_ParseUrlTest8_UnitTest::~CMailtoSchemeHandler_ParseUrlTest8_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest8Validator;
	delete iDtorValidator;
	}

inline CMailtoSchemeHandler_ParseUrlTest8_UnitTest::CMailtoSchemeHandler_ParseUrlTest8_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest8UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest8_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl8, "mailto: ? bcc = david@symbian ");
	iUTContext->iUrl = &KUrl8(); 

	_LIT(KTo1, "david@symbian");

	iUTContext->iArray.Append(KTo1());

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest8Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest8Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}


// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest9UnitTest,"CMailtoSchemeHandler_ParseUrlTest9_UnitTest");

CMailtoSchemeHandler_ParseUrlTest9_UnitTest* CMailtoSchemeHandler_ParseUrlTest9_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest9_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest9_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest9_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest9Validator;
	delete iDtorValidator;
	 
	return error;
	}

inline CMailtoSchemeHandler_ParseUrlTest9_UnitTest::~CMailtoSchemeHandler_ParseUrlTest9_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest9Validator;
	delete iDtorValidator;
	}

inline CMailtoSchemeHandler_ParseUrlTest9_UnitTest::CMailtoSchemeHandler_ParseUrlTest9_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest9UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest9_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl9, "mailto: ? subject = hello world ");
	iUTContext->iUrl = &KUrl9(); 

	_LIT(KSubject, "hello world");
	iUTContext->iSubjectCheck = &KSubject(); 	

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest9Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest9Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}


// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest10UnitTest,"CMailtoSchemeHandler_ParseUrlTest10_UnitTest");

CMailtoSchemeHandler_ParseUrlTest10_UnitTest* CMailtoSchemeHandler_ParseUrlTest10_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest10_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest10_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest10_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest10Validator;
	delete iDtorValidator;
	 
	return error;
	}

inline CMailtoSchemeHandler_ParseUrlTest10_UnitTest::~CMailtoSchemeHandler_ParseUrlTest10_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest10Validator;
	delete iDtorValidator;
	}

inline CMailtoSchemeHandler_ParseUrlTest10_UnitTest::CMailtoSchemeHandler_ParseUrlTest10_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest10UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest10_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl10, "mailto: ? subject = something & something else ");
	iUTContext->iUrl = &KUrl10(); 

	_LIT(KSubject, "something");
	iUTContext->iSubjectCheck = &KSubject(); 	

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest10Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest10Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest11UnitTest,"CMailtoSchemeHandler_ParseUrlTest11_UnitTest");

CMailtoSchemeHandler_ParseUrlTest11_UnitTest* CMailtoSchemeHandler_ParseUrlTest11_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest11_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest11_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest11_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest11Validator;
	delete iDtorValidator;
	 
	return error;
	}

inline CMailtoSchemeHandler_ParseUrlTest11_UnitTest::~CMailtoSchemeHandler_ParseUrlTest11_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest11Validator;
	delete iDtorValidator;
	}

inline CMailtoSchemeHandler_ParseUrlTest11_UnitTest::CMailtoSchemeHandler_ParseUrlTest11_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest11UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest11_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl11, "mailto: ? subject = something %26 something else ");
	iUTContext->iUrl = &KUrl11(); 

	_LIT(KSubject, "something");
	iUTContext->iSubjectCheck = &KSubject(); 	

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest11Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest11Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest12UnitTest,"CMailtoSchemeHandler_ParseUrlTest12_UnitTest");

CMailtoSchemeHandler_ParseUrlTest12_UnitTest* CMailtoSchemeHandler_ParseUrlTest12_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest12_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest12_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest12_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest12Validator;
	delete iDtorValidator;
	
	return error;
	}

inline CMailtoSchemeHandler_ParseUrlTest12_UnitTest::~CMailtoSchemeHandler_ParseUrlTest12_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest12Validator;
	delete iDtorValidator;
	}

inline CMailtoSchemeHandler_ParseUrlTest12_UnitTest::CMailtoSchemeHandler_ParseUrlTest12_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest12UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest12_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators

	//_LIT(KUrl11, "mailto: ? body = body ");
	_LIT(KUrl12, "mailto: ? body = This is the message body \n It can be long ");
	iUTContext->iUrl = &KUrl12(); 

	//_LIT(KBody, "body");
	_LIT(KBody, "This is the message body \n It can be long");
	iUTContext->iBodyCheck = &KBody(); 	

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest12Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest12Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));		
	}

// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest13UnitTest,"CMailtoSchemeHandler_ParseUrlTest13_UnitTest");

CMailtoSchemeHandler_ParseUrlTest13_UnitTest* CMailtoSchemeHandler_ParseUrlTest13_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest13_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest13_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest13_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest13Validator;
	delete iDtorValidator;

	return error;
	}

inline CMailtoSchemeHandler_ParseUrlTest13_UnitTest::~CMailtoSchemeHandler_ParseUrlTest13_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest13Validator;
	delete iDtorValidator;
	 
	}

inline CMailtoSchemeHandler_ParseUrlTest13_UnitTest::CMailtoSchemeHandler_ParseUrlTest13_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest13UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest13_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators
	
	_LIT(KUrl13, "mailto:?&&&body=silly message&&&&&");
	iUTContext->iUrl = &KUrl13(); 

	_LIT(KBody, "silly message");
	iUTContext->iBodyCheck = &KBody(); 	

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest13Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest13Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));		
	}

// ______________________________________________________________________________
//
_LIT(KMailtoSchemeHandlerParseUrlTest14UnitTest,"CMailtoSchemeHandler_ParseUrlTest14_UnitTest");

CMailtoSchemeHandler_ParseUrlTest14_UnitTest* CMailtoSchemeHandler_ParseUrlTest14_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ParseUrlTest14_UnitTest* self = 
					new(ELeave) CMailtoSchemeHandler_ParseUrlTest14_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self; 
	}

inline TInt CMailtoSchemeHandler_ParseUrlTest14_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest14Validator;
	delete iDtorValidator;
	 
	return error;
	}

inline CMailtoSchemeHandler_ParseUrlTest14_UnitTest::~CMailtoSchemeHandler_ParseUrlTest14_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	// delete any validators used
	delete iCtorValidator;
	delete iParseUrlTest14Validator;
	delete iDtorValidator;
	 
	}

inline CMailtoSchemeHandler_ParseUrlTest14_UnitTest::CMailtoSchemeHandler_ParseUrlTest14_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KMailtoSchemeHandlerParseUrlTest14UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CMailtoSchemeHandler_ParseUrlTest14_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TMailtoSchemeHandler_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CMailtoSchemeHandler_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	//	Examples of C'tor and D'tor transitions on CMailtoSchemeHandler class.
	//	using ctor and dtor validators

	_LIT(KUrl14, "mailto:shahadat@symbian ,david@symbian ?cc= john@symbian&bcc=dave@symbian&subject = mailto URLs&body=they seem to work %3a%2d%29");
	iUTContext->iUrl = &KUrl14(); 

	_LIT(KTo1, "shahadat@symbian");
	_LIT(KTo2, "david@symbian");
	_LIT(KTo3, "john@symbian");
	_LIT(KTo4, "dave@symbian");

	iUTContext->iArray.Append(KTo1());
	iUTContext->iArray.Append(KTo2());
	iUTContext->iArray.Append(KTo3());
	iUTContext->iArray.Append(KTo4());

	_LIT(KSubject, "mailto URLs");
	iUTContext->iSubjectCheck = &KSubject(); 	

	_LIT(KBody, "they seem to work :-)");
	iUTContext->iBodyCheck = &KBody(); 	

	iCtorValidator = new(ELeave) TMailtoSchemeHandler_Ctor_TransitionValidator(*iUTContext);
	iParseUrlTest14Validator = new(ELeave) TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator(*iUTContext);
	iDtorValidator = new(ELeave) TMailtoSchemeHandler_Dtor_TransitionValidator(*iUTContext);

	AddTransitionL(new(ELeave)CMailtoSchemeHandler_NewL_Transition(*iUTContext,*iCtorValidator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_ParseUrlL_Transition(*iUTContext,*iParseUrlTest14Validator));
	AddTransitionL(new(ELeave)CMailtoSchemeHandler_Dtor_Transition(*iUTContext,*iDtorValidator));	
	}

// ______________________________________________________________________________
//
