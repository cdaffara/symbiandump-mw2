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
* Description:    Test module to implement Platform security related test cases
*
*/





#ifndef TESTLBTPLATSECURITY_H
#define TESTLBTPLATSECURITY_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( KtestlbtplatsecurityLogPath, "\\logs\\testframework\\testlbtplatsecurity\\" ); 
// Log file
_LIT( KtestlbtplatsecurityLogFile, "testlbtplatsecurity.txt" ); 
_LIT( KtestlbtplatsecurityLogFileWithTitle, "testlbtplatsecurity_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Ctestlbtplatsecurity;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Ctestlbtplatsecurity test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Ctestlbtplatsecurity) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ctestlbtplatsecurity* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ctestlbtplatsecurity();

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
        Ctestlbtplatsecurity( CTestModuleIf& aTestModuleIf );

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
        virtual TInt ExampleL( CStifItemParser& aItem );
        TInt testCreateTriggerL( CStifItemParser& aItem );
        
        TInt testDeleteTriggerL( CStifItemParser& aItem );
        TInt testDeleteTriggers1L( CStifItemParser& aItem );
        TInt testDeleteTriggers2L( CStifItemParser& aItem );
        TInt testDeleteTriggers3L( CStifItemParser& aItem );
        TInt testDeleteTriggers4L( CStifItemParser& aItem );
        
        TInt testGetTriggerLC( CStifItemParser& aItem );
        TInt testUpdateTriggerL( CStifItemParser& aItem );
        TInt testSetTriggerStateL( CStifItemParser& aItem );
        TInt testSetTriggersStateL( CStifItemParser& aItem );
        TInt testSetTriggersState1L( CStifItemParser& aItem );
         
        TInt testListTriggerIdsL( CStifItemParser& aItem );
        
        TInt testListTriggerIds1L( CStifItemParser& aItem );
       
        TInt testGetTriggersL ( CStifItemParser& aItem );
       
        TInt testGetTriggers_test2L( CStifItemParser& aItem );
       
       
        TInt testCreateGetTriggerIteratorL( CStifItemParser& aItem );
        
        TInt testCreateGetTriggerIterator_test2L( CStifItemParser& aItem );
      
        TInt testGetNextTriggerLC( CStifItemParser& aItem );
       
        TInt testNotifyTriggerChangeEventL( CStifItemParser& aItem );
        
        TInt testNotifyTriggerFiredL( CStifItemParser& aItem );
        
        TInt testGetFiredTriggersL( CStifItemParser& aItem );
        
        TInt testNotifyTriggeringSystemSettingChangeL( CStifItemParser& aItem );
        
        TInt testGetTriggeringSystemSettingsL( CStifItemParser& aItem );
        
        TInt testSetTriggeringSystemSettingsL( CStifItemParser& aItem );
       
        TInt testSetTriggeringMechanismStateL( CStifItemParser& aItem );
        TInt testGetTriggeringSystemStatusL( CStifItemParser& aItem );
        TInt testGetTriggerManagerUiInfoL( CStifItemParser& aItem );
        TInt testNotifyTriggeringSystemStatusChangeL( CStifItemParser& aItem );
        
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

#endif      // TESTLBTPLATSECURITY_H

// End of File
