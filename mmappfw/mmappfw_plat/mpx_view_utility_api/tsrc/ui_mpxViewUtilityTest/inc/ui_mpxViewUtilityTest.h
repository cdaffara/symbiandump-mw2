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
* Description:  ui_mpxViewUtilityTest header for STIF Test Framework TestScripter
*
*/



#ifndef ui_mpxViewUtilityTest_H
#define ui_mpxViewUtilityTest_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <mpxviewutility.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( Kui_mpxViewUtilityTestLogPath, "\\logs\\testframework\\ui_mpxViewUtilityTest\\" ); 
// Log file
_LIT( Kui_mpxViewUtilityTestLogFile, "ui_mpxViewUtilityTest.txt" ); 
_LIT( Kui_mpxViewUtilityTestLogFileWithTitle, "ui_mpxViewUtilityTest_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Cui_mpxViewUtilityTest;
class MMPXViewUtility;

// CLASS DECLARATION

/**
*  Cui_mpxViewUtilityTest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cui_mpxViewUtilityTest) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Cui_mpxViewUtilityTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cui_mpxViewUtilityTest();

    

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

   

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        Cui_mpxViewUtilityTest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt TestMPXViewUtility( CStifItemParser& aItem );
                
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

      

  
    private:    // Friend classes
        //From mpxviewutility.h
    	MMPXViewUtility* iMPXViewUtility;
    };

#endif      // ui_mpxViewUtilityTest_H

// End of File
