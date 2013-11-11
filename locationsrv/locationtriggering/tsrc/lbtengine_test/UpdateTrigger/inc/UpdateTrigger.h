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
* Description:     Test module to implement Update trigger related test cases
*
*/






#ifndef UPDATETRIGGER_H
#define UPDATETRIGGER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( KUpdateTriggerLogPath, "\\logs\\testframework\\UpdateTrigger\\" ); 
// Log file
_LIT( KUpdateTriggerLogFile, "UpdateTrigger.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CUpdateTrigger;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CUpdateTrigger test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CUpdateTrigger) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CUpdateTrigger* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CUpdateTrigger();

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
        CUpdateTrigger( CTestModuleIf& aTestModuleIf );

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
        virtual TInt TCLBTFW001_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW002_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW003_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW004_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW005_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW006_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW007_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW008_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW009_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW010_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW011_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW012_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW013_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW014_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW015_testL( CStifItemParser& aItem );
		virtual TInt TCLBTFW016_testL( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate1L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate2L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate3L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate4L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate5L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate6L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate7L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate8L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate9L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate10L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate11L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate12L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate13L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate14L( CStifItemParser& aItem );
		virtual TInt TestFireOnUpdate15L( CStifItemParser& aItem );
		virtual TInt TestAsyncUpdate1L( CStifItemParser& aItem );
		virtual TInt TestAsyncUpdate2L( CStifItemParser& aItem );
		virtual TInt TestAsyncUpdate3L( CStifItemParser& aItem );
		virtual TInt TestAsyncUpdate4L( CStifItemParser& aItem );
		virtual TInt TestAsyncUpdate5L( CStifItemParser& aItem );
		virtual TInt TestAsyncUpdate6L( CStifItemParser& aItem );
		virtual TInt TestAsyncUpdate7L( CStifItemParser& aItem );
		virtual TInt TestAsyncUpdate8L( CStifItemParser& aItem );
		virtual TInt TestAsyncUpdate9L( CStifItemParser& aItem );
		virtual TInt TestAsyncUpdate10L( CStifItemParser& aItem );
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

#endif      // UPDATETRIGGER_H

// End of File
