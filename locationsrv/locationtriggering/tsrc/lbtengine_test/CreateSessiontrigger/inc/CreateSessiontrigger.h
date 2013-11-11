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
* Description:    Test module to implement Session trigger creation related test cases
*
*/





#ifndef CREATESESSIONTRIGGER_H
#define CREATESESSIONTRIGGER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( KCreateSessiontriggerLogPath, "\\logs\\testframework\\CreateSessiontrigger\\" ); 
// Log file
_LIT( KCreateSessiontriggerLogFile, "CreateSessiontrigger.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CCreateSessiontrigger;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CCreateSessiontrigger test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CCreateSessiontrigger) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCreateSessiontrigger* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCreateSessiontrigger();

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
        CCreateSessiontrigger( CTestModuleIf& aTestModuleIf );

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
        virtual TInt CreatesingleSessiontriggerL( CStifItemParser& aItem );
        virtual TInt CreateExittypeSessiontriggerL( CStifItemParser& aItem );
        virtual TInt CreateDisabledSessiontriggerL( CStifItemParser& aItem );
        virtual TInt CreatemultipleSessiontriggersL( CStifItemParser& aItem );
        virtual TInt CreateSessiontriggerwithoutConnectL( CStifItemParser& aItem );
        virtual TInt CreateSessiontriggerwithoutOpenL( CStifItemParser& aItem );
        virtual TInt SessiontriggerNameexceeds256L( CStifItemParser& aItem );
        virtual TInt SessiontriggerNameequals0L( CStifItemParser& aItem );
        virtual TInt CreateSessiontriggerwithoutNameL( CStifItemParser& aItem );
        virtual TInt CreateSessiontriggerwithoutcondL( CStifItemParser& aItem );
        virtual TInt CreateSessiontriggermanagerUINotFoundL( CStifItemParser& aItem );
        virtual TInt CreateSessiontriggerwithNonUIApplicationL( CStifItemParser& aItem );
        virtual TInt ErrortriggerAreaToosmallL( CStifItemParser& aItem );
        virtual TInt ErrortriggerAreaRectangleL( CStifItemParser& aItem );
        virtual TInt CreateSessiontriggerwithoutRequestorL( CStifItemParser& aItem );
                

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

#endif      // CREATESESSIONTRIGGER_H

// End of File
