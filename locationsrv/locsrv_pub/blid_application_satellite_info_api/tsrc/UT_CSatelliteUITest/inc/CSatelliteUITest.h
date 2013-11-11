/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: STIF hardcoded module declaration
*
*/

#ifndef CSATELLITEUITEST_H
#define CSATELLITEUITEST_H

// INCLUDES
#include "StifTestModule.h"
#include <StifLogger.h>
#include <NormalHardcodedAssert.h>

#include <CSatelliteInfoUI.h>
#include <EIKENV.h>
#include <StringLoader.h>
#include <aknappui.h>
#include "appui.h"
#include "CExecuteDialog.h"

// included for Huitk impl
#include <alf/alfenv.h>



// CONSTANTS
//const ?type ?constant_var = ?constant;
const TInt32 MAX_EXPONENT1 = 256;
const TInt32 CASE_FIRMAMENT_VIEW = 1;
const TInt32 CASE_COMPASS_VIEW = 2;
const TInt32 CASE_SIGNALSTRENGTH_VIEW = 3;
const TInt32 DEFAULTSTRING = 0;
const TInt32 STRING_LENGTH_256 = 1;
const TInt32 STRING_LENGTH_255 = 2;
const TInt32 STRING_LENGTH_254 = 3;
const TInt32 NULL_STRING = 4;





// MACROS
//#define ?macro ?macro_def
#define TEST_MODULE_VERSION_MAJOR 0
#define TEST_MODULE_VERSION_MINOR 0
#define TEST_MODULE_VERSION_BUILD 0

// Logging path
_LIT( KCSatelliteUITestLogPath, "\\logs\\testframework\\CSatelliteUITest\\" ); 
// Log file
_LIT( KCSatelliteUITestLogFile, "CSatelliteUITest.txt" ); 
_LIT( KCSatelliteUITestLogFileWithTitle, "CSatelliteUITest_[%S].txt" );

#define GETPTR &
#define ENTRY(str,func) {_S(str), GETPTR func,0,0,0}
#define FUNCENTRY(func) {_S(#func), GETPTR func,0,0,0}
#define OOM_ENTRY(str,func,a,b,c) {_S(str), GETPTR func,a,b,c}
#define OOM_FUNCENTRY(func,a,b,c) {_S(#func), GETPTR func,a,b,c}

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CCSatelliteUITest;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;
// A typedef for function that does the actual testing,
// function is a type 
// TInt CCSatelliteUITest::<NameOfFunction> ( TTestResult& aResult )
typedef TInt (CCSatelliteUITest::* TestFunction)(TTestResult&);

// CLASS DECLARATION

/**
*  An internal structure containing a test case name and
*  the pointer to function doing the test
*
*  @lib ?library
*  @since ?Series60_version
*/
class TCaseInfoInternal
    {
    public:
        const TText*    iCaseName;
        TestFunction    iMethod;
        TBool           iIsOOMTest;
        TInt            iFirstMemoryAllocation;
        TInt            iLastMemoryAllocation;
    };

// CLASS DECLARATION

/**
*  A structure containing a test case name and
*  the pointer to function doing the test
*
*  @lib ?library
*  @since ?Series60_version
*/
class TCaseInfo
    {
    public:
        TPtrC iCaseName;
        TestFunction iMethod;
        TBool           iIsOOMTest;
        TInt            iFirstMemoryAllocation;
        TInt            iLastMemoryAllocation;

    TCaseInfo( const TText* a ) : iCaseName( (TText*) a )
        {
        };

    };


// CLASS DECLARATION

/**
*  This a CSatelliteUITest class.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CCSatelliteUITest) : public CTestModuleBase
    {
    public:  // Constructors and destructor


        /**
        * Two-phased constructor.
        */
        static CCSatelliteUITest* NewL();

        /**
        * Destructor.
        */
        virtual ~CCSatelliteUITest();

    public: // New functions


        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CTestModuleBase InitL is used to initialize the 
        *       CSatelliteUITest. It is called once for every instance of 
        *       TestModuleCSatelliteUITest after its creation.
        * @since ?Series60_version
        * @param aIniFile Initialization file for the test module (optional)
        * @param aFirstTime Flag is true when InitL is executed for first 
        *               created instance of CSatelliteUITest.
        * @return Symbian OS error code
        */
        TInt InitL( TFileName& aIniFile, TBool aFirstTime );

        /**
        * From CTestModuleBase GetTestCasesL is used to inquiry test cases 
        *   from CSatelliteUITest. 
        * @since ?Series60_version
        * @param aTestCaseFile Test case file (optional)
        * @param aTestCases  Array of TestCases returned to test framework
        * @return Symbian OS error code
        */
        TInt GetTestCasesL( const TFileName& aTestCaseFile, 
                            RPointerArray<TTestCaseInfo>& aTestCases );

        /**
        * From CTestModuleBase RunTestCaseL is used to run an individual 
        *   test case. 
        * @since ?Series60_version
        * @param aCaseNumber Test case number
        * @param aTestCaseFile Test case file (optional)
        * @param aResult Test case result returned to test framework (PASS/FAIL)
        * @return Symbian OS error code (test case execution error, which is 
        *           not reported in aResult parameter as test case failure).
        */   
        TInt RunTestCaseL( const TInt aCaseNumber, 
                           const TFileName& aTestCaseFile,
                           TTestResult& aResult );

        /**
        * From CTestModuleBase; OOMTestQueryL is used to specify is particular
        * test case going to be executed using OOM conditions
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @param aFailureType OOM failure type (optional)
        * @param aFirstMemFailure The first heap memory allocation failure value (optional)
        * @param aLastMemFailure The last heap memory allocation failure value (optional)
        * @return TBool
        */
        virtual TBool OOMTestQueryL( const TFileName& /* aTestCaseFile */, 
                                     const TInt /* aCaseNumber */, 
                                     TOOMFailureType& aFailureType, 
                                     TInt& /* aFirstMemFailure */, 
                                     TInt& /* aLastMemFailure */ );

        /**
        * From CTestModuleBase; OOMTestInitializeL may be used to initialize OOM
        * test environment
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @return None
        */
        virtual void OOMTestInitializeL( const TFileName& /* aTestCaseFile */, 
                                    const TInt /* aCaseNumber */ ); 

        /**
        * From CTestModuleBase; OOMHandleWarningL
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @param aFailNextValue FailNextValue for OOM test execution (optional)
        * @return None
        *
        * User may add implementation for OOM test warning handling. Usually no
        * implementation is required.           
        */
        virtual void OOMHandleWarningL( const TFileName& /* aTestCaseFile */,
                                        const TInt /* aCaseNumber */, 
                                        TInt& /* aFailNextValue */); 

        /**
        * From CTestModuleBase; OOMTestFinalizeL may be used to finalize OOM
        * test environment
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @return None
        *
        */
        virtual void OOMTestFinalizeL( const TFileName& /* aTestCaseFile */, 
                                       const TInt /* aCaseNumber */ );
        
        /**
         * Method used to log version of test module
         */
        void SendTestModuleVersion();

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CCSatelliteUITest();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Function returning test case name and pointer to test case function.
        * @since ?Series60_version
        * @param aCaseNumber test case number
        * @return TCaseInfo 
        */
        const TCaseInfo Case ( const TInt aCaseNumber ) const;

        /**
        * [test case declarations] - do not remove
        */
        //ADD NEW METHOD DEC HERE"

       
        TInt CreateInstance(TTestResult& aResult);
        TInt SetandExecuteFirmament(TTestResult& aResult);
        TInt SetandExecuteCompass(TTestResult& aResult);
        TInt SetandExecuteSignalStrength(TTestResult& aResult);
//        TInt CreateInstance_OOM(TTestResult& aResult);
        TInt HandleFGEvent(TTestResult& aResult);
        TInt HandleBGEvent(TTestResult& aResult);

        TInt ExecuteBVA1(TTestResult& aResult);
        TInt ExecuteBVA2(TTestResult& aResult);
        TInt ExecuteBVA3(TTestResult& aResult);
        TInt ExecuteBVA4(TTestResult& aResult);


       



               
        TInt Initialize(TInt aCase = 1);
        void Finalize();
        void SetView(TInt aViewCase);
        TInt ExecuteViewL(TInt aCase = 0);
        void DrainMemory();
        void RestoreMemory();
        void SetResult( TTestResult& aResult );
        void EnableSimPSYL();
        void HandleFGEventL(TBool aForeground);



    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
    	CSatelliteInfoUI* mSatelliteInfo;


    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        // Pointer to test (function) to be executed
        TestFunction iMethod;

        // Pointer to logger
        CStifLogger * iLog; 

        // Normal logger
        CStifLogger* iStdLog;

        // Test case logger
        CStifLogger* iTCLog;

        // Flag saying if test case title should be added to log file name
        TBool iAddTestCaseTitleToLogName;

        // Flag saying if version of test module was already sent
        TBool iVersionLogged;
        
    	
    	CEikonEnv* mEnv;
    	
    	CAppUi* appUi;
    	
    	CExecuteDialog* mExecuteDialog;
    	
		/*! @var HUIToolkit environment. */
    	CAlfEnv* mAlfEnv;
    	
    	bool mTCPassed;
    	
		TAny* cells[MAX_EXPONENT1];
    	

        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // CSATELLITEUITEST_H

// End of File
