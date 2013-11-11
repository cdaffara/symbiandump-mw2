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
// test the CSmsSchemeHandler class using the test bed.
// It may be used as a basis to develop a full test bed dll.
// For support and comment please contact the authors.
// 
//

/**
 @file
 @SYMCreationDate Created : 01/06/01 14:07:22
*/
#include "SmsSchemeHandlerUnitTest.h"
#include <ecom/test_bed/componenttester.h>

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Test the CSmsSchemeHandler class.
 */
class CSmsSchemeHandler_ComponentTest : public CComponentTester
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
		@return			CSmsSchemeHandler_ComponentTest* The constructed object.
		@pre 			None.
		@post			CSmsSchemeHandler_ComponentTest is fully constructed.
	*/
	static CSmsSchemeHandler_ComponentTest* NewLC(CDataLogger& aDataLogger, 
							MComponentTestObserver& aObserver);

private:
	/**
		@fn				CSmsSchemeHandler_ComponentTest(CDataLogger& aDataLogger,
											MComponentTestObserver& aObserver)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			6.0
		@param			aDataLogger The logging object.
		@param			aObserver The observer of this component test.
		@pre 			None.
		@post			CSmsSchemeHandler_ComponentTest is fully constructed.
	*/
	inline CSmsSchemeHandler_ComponentTest(CDataLogger& aDataLogger,
												MComponentTestObserver& aObserver);
	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
						to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@leave		KErrNoMemory.
		@since			6.0
		@return			None 
		@pre 			CSmsSchemeHandler_ComponentTest is fully constructed.
		@post			CSmsSchemeHandler_ComponentTest is fully initialised.
	*/
	inline void ConstructL();

	};	// CSmsSchemeHandler_ComponentTest

// ______________________________________________________________________________
//
EXPORT_C CComponentTester* NewComponentTestLC(CDataLogger& aDataLogger,
									MComponentTestObserver&	aComponentTestObserver)
	{
	return CSmsSchemeHandler_ComponentTest::NewLC(aDataLogger, aComponentTestObserver);
	}

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
inline CSmsSchemeHandler_ComponentTest* CSmsSchemeHandler_ComponentTest::NewLC(CDataLogger& aDataLogger, MComponentTestObserver& aObserver)
	{
	CSmsSchemeHandler_ComponentTest* self = new (ELeave) CSmsSchemeHandler_ComponentTest(aDataLogger, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

inline CSmsSchemeHandler_ComponentTest::CSmsSchemeHandler_ComponentTest(CDataLogger& aDataLogger,
									MComponentTestObserver& aObserver)
: CComponentTester(aDataLogger, aObserver)
	{
	// Do nothing here.
	}

inline void CSmsSchemeHandler_ComponentTest::ConstructL()
	{
	// Perform base class initialization
	ComponentTesterConstructL();

	AddUnitTestL(CSmsSchemeHandler_CreateAndDestroy_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CSmsSchemeHandler_ParseUrlTest1_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CSmsSchemeHandler_ParseUrlTest2_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CSmsSchemeHandler_ParseUrlTest3_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CSmsSchemeHandler_ParseUrlTest4_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CSmsSchemeHandler_ParseUrlTest5_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CSmsSchemeHandler_ParseUrlTest6_UnitTest::NewL(iDataLogger, *this));
	}

