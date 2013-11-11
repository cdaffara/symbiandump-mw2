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
// Demonstrates a simple set of derived class implementations to
// test the CMailtoSchemeHandler class using the test bed.
// It may be used as a basis to develop a full test bed dll.
// For support and comment please contact the authors.
// 
//

/**
 @file
 @SYMCreationDate Created : 24/05/01 15:57:45
*/
#include "MailtoSchemeHandlerUnitTest.h"
#include <ecom/test_bed/componenttester.h>

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Test the CMailtoSchemeHandler class.
 */
class CMailtoSchemeHandler_ComponentTest : public CComponentTester
	{
public:
	/**
		@fn				NewLC(CDataLogger& aDataLogger,
							 MComponentTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@leave		KErrNoMemory
		@since			6.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this component test.
		@return			CMailtoSchemeHandler_ComponentTest* The constructed object.
		@pre 			None.
		@post			CMailtoSchemeHandler_ComponentTest is fully constructed.
	*/
	static CMailtoSchemeHandler_ComponentTest* NewLC(CDataLogger& aDataLogger, 
							MComponentTestObserver& aObserver);

private:
	/**
		@fn				CMailtoSchemeHandler_ComponentTest(CDataLogger& aDataLogger,
											MComponentTestObserver& aObserver)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			6.0
		@param			aDataLogger The logging object.
		@param			aObserver The observer of this component test.
		@pre 			None.
		@post			CMailtoSchemeHandler_ComponentTest is fully constructed.
	*/
	inline CMailtoSchemeHandler_ComponentTest(CDataLogger& aDataLogger,
												MComponentTestObserver& aObserver);
	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
						to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory.
		@since			6.0
		@return			None 
		@pre 			CMailtoSchemeHandler_ComponentTest is fully constructed.
		@post			CMailtoSchemeHandler_ComponentTest is fully initialised.
	*/
	inline void ConstructL();

	};	// CMailtoSchemeHandler_ComponentTest

// ______________________________________________________________________________
//
EXPORT_C CComponentTester* NewComponentTestLC(CDataLogger& aDataLogger,
									MComponentTestObserver&	aComponentTestObserver)
	{
	return CMailtoSchemeHandler_ComponentTest::NewLC(aDataLogger, aComponentTestObserver);
	}

/** DLL Entry point */
//GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//	{
//	return(KErrNone);
//	}

// ___________________________________________________________________________
//
// This section of the module simply includes the exported test harness template which 
// makes this a "whole" CPP file with a E32Main entry point below. The test MMP 
// project file can then produce a EXE for the test project instead of a DLL.

#include <ecom/test_bed/testharnesstemplate.h>

GLDEF_C TInt E32Main()
	{
	return E32Main_TestHarness(NewComponentTestLC);
	}
	
// ______________________________________________________________________________
//
inline CMailtoSchemeHandler_ComponentTest* CMailtoSchemeHandler_ComponentTest::NewLC(CDataLogger& aDataLogger, MComponentTestObserver& aObserver)
	{
	CMailtoSchemeHandler_ComponentTest* self = new (ELeave) CMailtoSchemeHandler_ComponentTest(aDataLogger, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

inline CMailtoSchemeHandler_ComponentTest::CMailtoSchemeHandler_ComponentTest(CDataLogger& aDataLogger,
									MComponentTestObserver& aObserver)
: CComponentTester(aDataLogger, aObserver)
	{
	// Do nothing here.
	}

inline void CMailtoSchemeHandler_ComponentTest::ConstructL()
	{
	// Perform base class initialization
	ComponentTesterConstructL();

	AddUnitTestL(CMailtoSchemeHandler_CreateAndDestroy_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest1_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest2_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest3_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest4_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest5_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest6_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest7_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest8_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest9_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest10_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest11_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest12_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest13_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CMailtoSchemeHandler_ParseUrlTest14_UnitTest::NewL(iDataLogger, *this));
	}

