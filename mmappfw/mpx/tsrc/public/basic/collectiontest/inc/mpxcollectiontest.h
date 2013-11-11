/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  MpxCollectionTest test module.
*
*/

#ifndef MPXCOLLECTIONTEST_H
#define MPXCOLLECTIONTEST_H

// INCLUDES
#include <e32base.h>
#include <stiftestmodule.h>
#include <mpxcollectionobserver.h>

// MACROS
// Function pointer related internal definitions
// Hack around known GCC bug.
#ifndef __GCC32__
#define GETPTR
#else
#define GETPTR &
#endif  
#define ENTRY(str,func) {(TText*)L##str, GETPTR func}
#define FUNCENTRY(func) {(TText*)L#func, GETPTR func}


// FORWARD DECLARATIONS
class MMPXCollectionUtility;
class MMPXHarvesterUtility;
class CStifLogger;
class CMPXCollectionPath;
class CMpxCollectionTest;
class CConsoleMain;

// DATA TYPES
// A typedef for function that does the actual testing,
typedef TInt (CMpxCollectionTest::* TestFunction)(TTestResult&);    

// CLASS DECLARATION

/**
*  An internal structure containing a test case name and
*  the pointer to function doing the test
*/
class TCaseInfoInternal
    {
    public:
        const TText* iCaseName;
        TestFunction iMethod;
    };

// CLASS DECLARATION

/**
*  A structure containing a test case name and
*  the pointer to function doing the test
*/
class TCaseInfo
    {
    public:
        TPtrC iCaseName;    
        TestFunction iMethod;

    TCaseInfo( const TText* a ) : iCaseName( (TText*) a )
        {        
        };

    };


// CLASS DECLARATION

/**
*  This a MpxCollectionTest class.
*/
class CMpxCollectionTest: public CTestModuleBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMpxCollectionTest* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMpxCollectionTest();
        
    public: // Functions from base classes


        /**
        * From CTestModuleBase InitL is used to initialize the 
        *       MpxCollectionTest. It is called once for every instance of 
        *       TestModuleMpxCollectionTest after its creation.
        * @param aIniFile Initialization file for the test module (optional)
        * @param aFirstTime Flag is true when InitL is executed for first 
        *               created instance of MpxCollectionTest.
        * @return Symbian OS error code
        */
        TInt InitL( TFileName& aIniFile, TBool aFirstTime );

        /**
        * From CTestModuleBase GetTestCasesL is used to inquiry test cases 
        *   from MpxCollectionTest. 
        * @param aTestCaseFile Test case file (optional)
        * @param aTestCases  Array of TestCases returned to test framework
        * @return Symbian OS error code
        */
        TInt GetTestCasesL( const TFileName& aTestCaseFile, 
                            RPointerArray<TTestCaseInfo>& aTestCases );
    
        /**
        * From CTestModuleBase RunTestCaseL is used to run an individual 
        *   test case. 
        * @param aCaseNumber Test case number
        * @param aTestCaseFile Test case file (optional)
        * @param aResult Test case result returned to test framework (PASS/FAIL)
        * @return Symbian OS error code (test case execution error, which is 
        *           not reported in aResult parameter as test case failure).
        */   
        TInt RunTestCaseL( const TInt aCaseNumber, 
                           const TFileName& aTestCaseFile,
                           TTestResult& aResult );
        
    private:

        /**
        * C++ default constructor.
        */
        CMpxCollectionTest();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Function returning test case name and pointer to test case function.
        * @param aCaseNumber test case number
        * @return TCaseInfo 
        */
        const TCaseInfo Case ( const TInt aCaseNumber ) const;
        
        /**
        * Initialize environment before test
        */
        void InitializeL( CActiveScheduler* scheduler );
        
        /**
        * Cleanup environment after test
        */
        void Cleanup();
        
        /**
        * Actual Hardcoded test case functions are listed below.
        */
        
        /**
        * Common test case driver for all Collection tests.
        * @param aResult Test case result (PASS/FAIL)
        * @return Symbian OS error code (test case execution error
        *   that is not returned as test case result in aResult)
        */
        TInt CommonTestL( TInt testIndex, TTestResult& aResult );
		
    private:    // Help functions

  	    TInt Test0001L(TTestResult& aResult) { return CommonTestL(  1, aResult); }
	    TInt Test0002L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(  2, aResult); }
	    TInt Test0003L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(  3, aResult); }
	    TInt Test0004L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(  4, aResult); }
	    TInt Test0005L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(  5, aResult); }
	    TInt Test0006L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(  6, aResult); }
	    TInt Test0007L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(  7, aResult); }
	    TInt Test0008L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(  8, aResult); }
	    TInt Test0009L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(  9, aResult); }
	    TInt Test0010L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 10, aResult); }
  	    TInt Test0011L(TTestResult& aResult) { return CommonTestL( 11, aResult); }
	    TInt Test0012L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 12, aResult); }
	    TInt Test0013L(TTestResult& aResult) { return CommonTestL( 13, aResult); }
	    TInt Test0014L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 14, aResult); }
	    TInt Test0015L(TTestResult& aResult) { return CommonTestL( 15, aResult); }
	    TInt Test0016L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 16, aResult); }
	    TInt Test0017L(TTestResult& aResult) { return CommonTestL( 17, aResult); }
	    TInt Test0018L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 18, aResult); }
	    TInt Test0019L(TTestResult& aResult) { return CommonTestL( 19, aResult); }
	    TInt Test0020L(TTestResult& aResult) { return CommonTestL( 20, aResult); }
  	    TInt Test0021L(TTestResult& aResult) { return CommonTestL( 21, aResult); }
	    TInt Test0022L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 22, aResult); }
	    TInt Test0023L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 23, aResult); }
	    TInt Test0024L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 24, aResult); }
	    TInt Test0025L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 25, aResult); }
	    TInt Test0026L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 26, aResult); }
	    TInt Test0027L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 27, aResult); }
	    TInt Test0028L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 28, aResult); }
	    TInt Test0029L(TTestResult& aResult)    { return 0; } //return CommonTestL( 29, aResult); }
	    TInt Test0030L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 30, aResult); }
  	    TInt Test0031L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 31, aResult); }
	    TInt Test0032L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 32, aResult); }
	    TInt Test0033L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 33, aResult); }
	    TInt Test0034L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 34, aResult); }
	    TInt Test0035L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 35, aResult); }
	    TInt Test0036L(TTestResult& aResult) { return CommonTestL( 36, aResult); }
	    TInt Test0037L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 37, aResult); }
	    TInt Test0038L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 38, aResult); }
	    TInt Test0039L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 39, aResult); }
	    TInt Test0040L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 40, aResult); }
  	    TInt Test0041L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 41, aResult); }
	    TInt Test0042L(TTestResult& aResult) { return CommonTestL( 42, aResult); }
	    TInt Test0043L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 43, aResult); }
	    TInt Test0044L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 44, aResult); }
	    TInt Test0045L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 45, aResult); }
	    TInt Test0046L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 46, aResult); }
	    TInt Test0047L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 47, aResult); }
	    TInt Test0048L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 48, aResult); }
	    TInt Test0049L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 49, aResult); }
	    TInt Test0050L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 50, aResult); }
  	    TInt Test0051L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 51, aResult); }
	    TInt Test0052L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 52, aResult); }
	    TInt Test0053L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 53, aResult); }
	    TInt Test0054L(TTestResult& aResult) { return CommonTestL( 54, aResult); }
	    TInt Test0055L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 55, aResult); }
	    TInt Test0056L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 56, aResult); }
	    TInt Test0057L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 57, aResult); }
	    TInt Test0058L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 58, aResult); }
	    TInt Test0059L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 59, aResult); }
	    TInt Test0060L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 60, aResult); }
  	    TInt Test0061L(TTestResult& aResult) { return 0; } //return CommonTestL( 61, aResult); }
	    TInt Test0062L(TTestResult& aResult) { return 0; } //return CommonTestL( 62, aResult); }
	    TInt Test0063L(TTestResult& aResult) { return 0; } //return CommonTestL( 63, aResult); }
	    TInt Test0064L(TTestResult& aResult) { return 0; } //return CommonTestL( 64, aResult); }
	    TInt Test0065L(TTestResult& aResult) { return 0; } //return CommonTestL( 65, aResult); }
	    TInt Test0066L(TTestResult& aResult) { return 0; } //return CommonTestL( 66, aResult); }
	    TInt Test0067L(TTestResult& aResult) { return 0; } //return CommonTestL( 67, aResult); }
	    TInt Test0068L(TTestResult& aResult) { return 0; } //return CommonTestL( 68, aResult); }
	    TInt Test0069L(TTestResult& aResult) { return 0; } //return CommonTestL( 69, aResult); }
	    TInt Test0070L(TTestResult& aResult) { return CommonTestL( 70, aResult); }
  	    TInt Test0071L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 71, aResult); }
	    TInt Test0072L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 72, aResult); }
	    TInt Test0073L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 73, aResult); }
	    TInt Test0074L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 74, aResult); }
	    TInt Test0075L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 75, aResult); }
	    TInt Test0076L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 76, aResult); }
	    TInt Test0077L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 77, aResult); }
	    TInt Test0078L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 78, aResult); }
	    TInt Test0079L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 79, aResult); }
	    TInt Test0080L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 80, aResult); }
  	    TInt Test0081L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 81, aResult); }
	    TInt Test0082L(TTestResult& aResult) { return CommonTestL( 82, aResult); }
	    TInt Test0083L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 83, aResult); }
	    TInt Test0084L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 84, aResult); }
	    TInt Test0085L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 85, aResult); }
	    TInt Test0086L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 86, aResult); }
	    TInt Test0087L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 87, aResult); }
	    TInt Test0088L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 88, aResult); }
	    TInt Test0089L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 89, aResult); }
	    TInt Test0090L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 90, aResult); }
  	    TInt Test0091L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 91, aResult); }
	    TInt Test0092L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 92, aResult); }
	    TInt Test0093L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 93, aResult); }
	    TInt Test0094L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 94, aResult); }
	    TInt Test0095L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 95, aResult); }
	    TInt Test0096L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 96, aResult); }
	    TInt Test0097L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 97, aResult); }
	    TInt Test0098L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 98, aResult); }
	    TInt Test0099L(TTestResult& aResult) 	{ return 0; } //return CommonTestL( 99, aResult); }
  	    TInt Test0100L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(100, aResult); }	    
  	    TInt Test0101L(TTestResult& aResult) { return CommonTestL(101, aResult); }
	    TInt Test0102L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(102, aResult); }
	    TInt Test0103L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(103, aResult); }
	    TInt Test0104L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(104, aResult); }
	    TInt Test0105L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(105, aResult); }
	    TInt Test0106L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(106, aResult); }
	    TInt Test0107L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(107, aResult); }
	    TInt Test0108L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(108, aResult); }
	    TInt Test0109L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(109, aResult); }
	    TInt Test0110L(TTestResult& aResult) { return CommonTestL(110, aResult); }
  	    TInt Test0111L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(111, aResult); }
	    TInt Test0112L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(112, aResult); }
	    TInt Test0113L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(113, aResult); }
	    TInt Test0114L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(114, aResult); }
	    TInt Test0115L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(115, aResult); }
	    TInt Test0116L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(116, aResult); }
	    TInt Test0117L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(117, aResult); }
	    TInt Test0118L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(118, aResult); }
	    TInt Test0119L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(119, aResult); }
	    TInt Test0120L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(120, aResult); }
  	    TInt Test0121L(TTestResult& aResult) { return CommonTestL(121, aResult); }
	    TInt Test0122L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(122, aResult); }
	    TInt Test0123L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(123, aResult); }
	    TInt Test0124L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(124, aResult); }
	    TInt Test0125L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(125, aResult); }
	    TInt Test0126L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(126, aResult); }
	    TInt Test0127L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(127, aResult); }
	    TInt Test0128L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(128, aResult); }
	    TInt Test0129L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(129, aResult); }
	    TInt Test0130L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(130, aResult); }
  	    TInt Test0131L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(131, aResult); }
	    TInt Test0132L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(132, aResult); }
	    TInt Test0133L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(133, aResult); }
	    TInt Test0134L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(134, aResult); }
	    TInt Test0135L(TTestResult& aResult)	{ return 0; } //return CommonTestL(135, aResult); }
	    TInt Test0136L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(136, aResult); }
	    TInt Test0137L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(137, aResult); }
	    TInt Test0138L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(138, aResult); }
	    TInt Test0139L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(139, aResult); }
	    TInt Test0140L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(140, aResult); }
  	    TInt Test0141L(TTestResult& aResult) { return CommonTestL(141, aResult); }
	    TInt Test0142L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(142, aResult); }
	    TInt Test0143L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(143, aResult); }
	    TInt Test0144L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(144, aResult); }
	    TInt Test0145L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(145, aResult); }
	    TInt Test0146L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(146, aResult); }
	    TInt Test0147L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(147, aResult); }
	    TInt Test0148L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(148, aResult); }
	    TInt Test0149L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(149, aResult); }
	    TInt Test0150L(TTestResult& aResult) { return CommonTestL(150, aResult); }
  	    TInt Test0151L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(151, aResult); }
	    TInt Test0152L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(152, aResult); }
	    TInt Test0153L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(153, aResult); }
	    TInt Test0154L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(154, aResult); }
	    TInt Test0155L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(155, aResult); }
	    TInt Test0156L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(156, aResult); }
	    TInt Test0157L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(157, aResult); }
	    TInt Test0158L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(158, aResult); }
	    TInt Test0159L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(159, aResult); }
	    TInt Test0160L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(160, aResult); }
  	    TInt Test0161L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(161, aResult); }
	    TInt Test0162L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(162, aResult); }
	    TInt Test0163L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(163, aResult); }
	    TInt Test0164L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(164, aResult); }
	    TInt Test0165L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(165, aResult); }
	    TInt Test0166L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(166, aResult); }
	    TInt Test0167L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(167, aResult); }
	    TInt Test0168L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(168, aResult); }
	    TInt Test0169L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(169, aResult); }
	    TInt Test0170L(TTestResult& aResult) { return CommonTestL(170, aResult); }
  	    TInt Test0171L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(171, aResult); }
	    TInt Test0172L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(172, aResult); }
	    TInt Test0173L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(173, aResult); }
	    TInt Test0174L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(174, aResult); }
	    TInt Test0175L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(175, aResult); }
	    TInt Test0176L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(176, aResult); }
	    TInt Test0177L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(177, aResult); }
	    TInt Test0178L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(178, aResult); }
	    TInt Test0179L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(179, aResult); }
	    TInt Test0180L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(180, aResult); }
  	    TInt Test0181L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(181, aResult); }
	    TInt Test0182L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(182, aResult); }
	    TInt Test0183L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(183, aResult); }
	    TInt Test0184L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(184, aResult); }
	    TInt Test0185L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(185, aResult); }
	    TInt Test0186L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(186, aResult); }
	    TInt Test0187L(TTestResult& aResult) { return 0; } //return CommonTestL(187, aResult); }
	    TInt Test0188L(TTestResult& aResult) { return 0; } //return CommonTestL(188, aResult); }
	    TInt Test0189L(TTestResult& aResult) { return 0; } //return CommonTestL(189, aResult); }
	    TInt Test0190L(TTestResult& aResult) { return CommonTestL(190, aResult); }
  	    TInt Test0191L(TTestResult& aResult) { return CommonTestL(191, aResult); }
	    TInt Test0192L(TTestResult& aResult) { return CommonTestL(192, aResult); }
	    TInt Test0193L(TTestResult& aResult) { return CommonTestL(193, aResult); }
	    TInt Test0194L(TTestResult& aResult) { return CommonTestL(194, aResult); }
	    TInt Test0195L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(195, aResult); }
	    TInt Test0196L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(196, aResult); }
	    TInt Test0197L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(197, aResult); }
	    TInt Test0198L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(198, aResult); }
	    TInt Test0199L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(199, aResult); }
	    TInt Test0200L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(200, aResult); }
  	    TInt Test0201L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(201, aResult); }
	    TInt Test0202L(TTestResult& aResult) 	{ return 0; } //return CommonTestL(202, aResult); }

    private:    // Data
        
        // Pointer to test (function) to be executed
        TestFunction 		iMethod;
        
        // Pointer to an active scheduler. Needed for MCModel
        CActiveScheduler* 	iScheduler;
        
        // Pointer to a Logger. Helps to write log information
        CStifLogger* 		iLogger;

        CConsoleMain* 		iMainConsole;
    };

#endif      // MpxCOLLECTIONTEST_H
            
// End of File
