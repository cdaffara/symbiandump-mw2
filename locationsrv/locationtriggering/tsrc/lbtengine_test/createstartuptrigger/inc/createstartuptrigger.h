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
* Description:    Test module to implement Startup trigger creation related test cases
*
*/





#ifndef CREATESTARTUPTRIGGER_H
#define CREATESTARTUPTRIGGER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( KcreatestartuptriggerLogPath, "\\logs\\testframework\\createstartuptrigger\\" ); 
// Log file
_LIT( KcreatestartuptriggerLogFile, "createstartuptrigger.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Ccreatestartuptrigger;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Ccreatestartuptrigger test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Ccreatestartuptrigger) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ccreatestartuptrigger* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ccreatestartuptrigger();

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
        Ccreatestartuptrigger( CTestModuleIf& aTestModuleIf );

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
        virtual TInt CreatesingleStartupTriggerL( CStifItemParser& aItem );
        virtual TInt CreateexittypeStartupTriggerL( CStifItemParser& aItem );
        virtual TInt CreateDisabledStartupTriggerL( CStifItemParser& aItem );
        virtual TInt CreatemultipleStartupTriggersL( CStifItemParser& aItem );
        virtual TInt CreateStartupTriggerwithoutconnectL( CStifItemParser& aItem );
        virtual TInt CreateStartupTriggerwithoutopenL( CStifItemParser& aItem );
        virtual TInt StartupTriggerNameexceeds256_testL( CStifItemParser& aItem );
        virtual TInt StartupTriggerNameequals0_testL( CStifItemParser& aItem );
        virtual TInt CreateStartupTriggerwithoutNameL( CStifItemParser& aItem );
        virtual TInt CreateStartupTriggerwithoutcondL( CStifItemParser& aItem );
        virtual TInt CreateStartupTriggerManagerUINotFoundL( CStifItemParser& aItem );
        virtual TInt CreateStartupTriggerwithNonUIAppL( CStifItemParser& aItem );
        virtual TInt ErrortriggerAreaToosmallL( CStifItemParser& aItem );
        virtual TInt CreateStartupTriggerwithoutTriggerhandlingprocessL( CStifItemParser& aItem );
        virtual TInt CreateStartupTriggerwithoutRequestorL( CStifItemParser& aItem );
        virtual TInt ErrorRectangletriggerAreaL( CStifItemParser& aItem );

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

#endif      // CREATESTARTUPTRIGGER_H

// End of File
