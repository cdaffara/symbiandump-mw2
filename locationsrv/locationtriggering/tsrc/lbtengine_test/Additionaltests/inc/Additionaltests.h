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
* Description:    Test module to implement Test cases found during review
*
*/





#ifndef ADDITIONALTESTS_H
#define ADDITIONALTESTS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KAdditionaltestsLogPath, "\\logs\\testframework\\Additionaltests\\" ); 
// Log file
_LIT( KAdditionaltestsLogFile, "Additionaltests.txt" ); 
_LIT( KAdditionaltestsLogFileWithTitle, "Additionaltests_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CAdditionaltests;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CAdditionaltests test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CAdditionaltests) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAdditionaltests* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CAdditionaltests();

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
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

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
        CAdditionaltests( CTestModuleIf& aTestModuleIf );

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
        void AssertTrueL( TBool aCondition,const TDesC& aMessage );
        virtual TInt ExampleL( CStifItemParser& aItem );
        virtual TInt TestGetTriggers1L( CStifItemParser& aItem );
        virtual TInt TestGetTriggers2L( CStifItemParser& aItem );
        virtual TInt TestGetTriggers3L( CStifItemParser& aItem );
        virtual TInt TestAsynchronousGetTriggers1L( CStifItemParser& aItem );
        virtual TInt TestAsynchronousGetTriggers2L( CStifItemParser& aItem );
        virtual TInt TestAsynchronousGetTriggers3L( CStifItemParser& aItem );
        virtual TInt TestAsynchronousGetTriggers4L( CStifItemParser& aItem );
        virtual TInt TestListTriggerIds1L( CStifItemParser& aItem );
        virtual TInt TestSortingL( CStifItemParser& aItem );
        virtual TInt TestAttributefilterL( CStifItemParser& aItem );
        virtual TInt TestCreateGetTriggerIteratorL( CStifItemParser& aItem );
        virtual TInt TestAttributefilter2L( CStifItemParser& aItem );
        virtual TInt TestUpdateTrigger1L( CStifItemParser& aItem );
        virtual TInt TestSetTriggersState1L( CStifItemParser& aItem );
        virtual TInt TestSetTriggersState2L( CStifItemParser& aItem );
        virtual TInt TestSetTriggersState3L( CStifItemParser& aItem );
        virtual TInt TestSetTriggersState4L( CStifItemParser& aItem );
        virtual TInt TestSetTriggersState5L( CStifItemParser& aItem );
        virtual TInt TestSetTriggersState6L( CStifItemParser& aItem );
        virtual TInt TestdeleteTrigger1L( CStifItemParser& aItem );
        virtual TInt TestdeleteTrigger2L( CStifItemParser& aItem );
        virtual TInt TestdeleteTrigger3L( CStifItemParser& aItem );
        virtual TInt TestdeleteTrigger4L( CStifItemParser& aItem );
        virtual TInt TestdeleteTrigger5L( CStifItemParser& aItem );
        virtual TInt TestFilterbyAttribute1L( CStifItemParser& aItem );
        virtual TInt TestFilterbyAttribute2L( CStifItemParser& aItem );
        virtual TInt TestFilterbyAttribute3L( CStifItemParser& aItem );
        virtual TInt TestClientRequestorL( CStifItemParser& aItem );
        virtual TInt TestNotifyTriggerChangeEventL( CStifItemParser& aItem );
        virtual TInt TestNotifyTriggerChangeEvent2L( CStifItemParser& aItem );
        virtual TInt TestNotifyTriggerChangeEvent3L( CStifItemParser& aItem );
        virtual TInt TestNotifyTriggerChangeEvent4L( CStifItemParser& aItem );
        virtual TInt TestManagerUI1L( CStifItemParser& aItem );
        virtual TInt TestManagerUI2L( CStifItemParser& aItem );
        virtual TInt TestManagerUI3L( CStifItemParser& aItem );
        virtual TInt TestTriggerHandlingproc1L( CStifItemParser& aItem );
        virtual TInt TestOpenL( CStifItemParser& aItem );
        virtual TInt TestUpdateTrigger2L( CStifItemParser& aItem );
        virtual TInt TestUpdateTrigger3L( CStifItemParser& aItem );
        virtual TInt TestUpdateTrigger4L( CStifItemParser& aItem );
        virtual TInt TestUpdateTrigger5L( CStifItemParser& aItem );
        virtual TInt TestUpdateTrigger6L( CStifItemParser& aItem );
        virtual TInt TestListtriggerIds1L( CStifItemParser& aItem );
        virtual TInt TestListtriggerIds2L( CStifItemParser& aItem );
        virtual TInt TestNotifyTriggeringsettingsChangeEvent1L( CStifItemParser& aItem );
        virtual TInt TestTriggeringsettingsChangeEventDestructorL( CStifItemParser& aItem );
        virtual TInt TestCancelAllL( CStifItemParser& aItem );
        virtual TInt TestCLbtGeoCircle1L( CStifItemParser& aItem );
        virtual TInt TestCLbtGeoCircle2L( CStifItemParser& aItem );
        virtual TInt TestCLbtGeoRect1L( CStifItemParser& aItem );
        virtual TInt TestCLbtGeoRect2L( CStifItemParser& aItem );
        virtual TInt TestCLbtGeoRect3L( CStifItemParser& aItem );
        virtual TInt TestCLbtSessionTriggerL( CStifItemParser& aItem );
        virtual TInt TestCLbtStartupTriggerL( CStifItemParser& aItem );
        virtual TInt TestCLbtTriggerChangeEventNotifierL( CStifItemParser& aItem );
        virtual TInt TestCLbtTriggerInfoL( CStifItemParser& aItem );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
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

#endif      // ADDITIONALTESTS_H

// End of File
