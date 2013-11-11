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
* Description:    Test module to implement Cancellation of asynchronous requests related test cases
*
*/





#ifndef TESTLBTCANCELLATION_H
#define TESTLBTCANCELLATION_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <EPos_MPosModulesObserver.h> 
#include <EPos_CPosModuleIdList.h>
#include <EPos_CPosModules.h>
#include <EPos_CPosModuleUpdate.h> // CPosModuleUpdate

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( KtestlbtcancellationLogPath, "\\logs\\testframework\\testlbtcancellation\\" ); 
// Log file
_LIT( KtestlbtcancellationLogFile, "testlbtcancellation.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Ctestlbtcancellation;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Ctestlbtcancellation test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Ctestlbtcancellation) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ctestlbtcancellation* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ctestlbtcancellation();

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
        Ctestlbtcancellation( CTestModuleIf& aTestModuleIf );

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
        void EnableSimPSYL();
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
        virtual TInt testCancelCreateTrigger1L(CStifItemParser& aItem);
        virtual TInt testCancelCreateTrigger2L(CStifItemParser& aItem);
        virtual TInt testCancelDeleteTriggers1L(CStifItemParser& aItem);
        virtual TInt testCancelDeleteTriggers2L(CStifItemParser& aItem);
        virtual TInt testCancelDeleteTriggers3L(CStifItemParser& aItem);
        virtual TInt testCancelDeleteTriggers4L(CStifItemParser& aItem);
        virtual TInt testCancelSetTriggersState1L(CStifItemParser& aItem);
        virtual TInt testCancelSetTriggersState2L(CStifItemParser& aItem);
        virtual TInt testCancelListTriggerIds1L(CStifItemParser& aItem);
        virtual TInt testCancelListTriggerIds2L(CStifItemParser& aItem);
        virtual TInt testCancelGetTriggers1L(CStifItemParser& aItem);
        virtual TInt testCancelGetTriggers2L(CStifItemParser& aItem);
        virtual TInt testCancelCreatetTriggerIteratorL(CStifItemParser& aItem);
        virtual TInt testCancelCreateTriggerFromDifferentSubsessionL(CStifItemParser& aItem);
        virtual TInt testCancelDeleteTriggersFromDifferentSubsessionL(CStifItemParser& aItem);
        virtual TInt testCancelSetTriggersStateFromDifferentSubsessionL(CStifItemParser& aItem);
        virtual TInt testCancelListTriggerIdsFromDifferentSubsessionL(CStifItemParser& aItem);
        virtual TInt testCancelGetTriggersFromDifferentSubsessionL(CStifItemParser& aItem);
        virtual TInt testCancelCreatetTriggerIteratorFromDifferentSubsessionL(CStifItemParser& aItem);
        virtual TInt testCancelCreateTriggerFromDifferentProcessL(CStifItemParser& aItem);
        virtual TInt testCancelDeleteTriggersFromDifferentProcessL(CStifItemParser& aItem);
        virtual TInt testCancelSetTriggersStateFromDifferentProcessL( CStifItemParser& aItem );
        virtual TInt testCancelListTriggerIdsFromDifferentProcessL( CStifItemParser& aItem );
        virtual TInt testCancelGetTriggersFromDifferentProcessL( CStifItemParser& aItem );
        virtual TInt testCancelCreatetTriggerIteratorFromDifferentProcessL(CStifItemParser& aItem);
        virtual TInt testCancelUpdateTrigger1L(CStifItemParser& aItem);
        virtual TInt testCancelUpdateTrigger2L(CStifItemParser& aItem);
        virtual TInt testCancelNotifyTriggerChangeEvent1L(CStifItemParser& aItem);
        virtual TInt testCancelNotifyTriggerChangeEvent2L(CStifItemParser& aItem);
        virtual TInt testCanceltNotifyTriggerFiredL(CStifItemParser& aItem);
        virtual TInt testCancelNotifyTriggeringSettingsChangeL(CStifItemParser& aItem);
        virtual TInt testlbtmgrCancelNotifyTriggeringSettingsChangeL(CStifItemParser& aItem);
        virtual TInt testCancelNotifyTriggeringSystemStatusChangeL(CStifItemParser& aItem);
        virtual TInt testPartialDeleteTriggers1L(CStifItemParser& aItem);
	/*	TInt testCancelTwoAsyncOperationFromDifferentSubsession1(CStifItemParser& aItem);
		TInt testCancelTwoAsyncOperationFromDifferentSubsession2(CStifItemParser& aItem);*/
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

#endif      // TESTLBTCANCELLATION_H

// End of File
