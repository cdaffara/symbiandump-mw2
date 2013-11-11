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
* Description:    Test module to implement Delete trigger related test cases
*
*/





#ifndef T_DELETETRIGGERS_H
#define T_DELETETRIGGERS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( Kt_deletetriggersLogPath, "\\logs\\testframework\\t_deletetriggers\\" ); 
// Log file
_LIT( Kt_deletetriggersLogFile, "t_deletetriggers.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Ct_deletetriggers;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Ct_deletetriggers test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Ct_deletetriggers) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ct_deletetriggers* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ct_deletetriggers();

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
        Ct_deletetriggers( CTestModuleIf& aTestModuleIf );

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
        
        virtual TInt TC_LBT013_01_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_02_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_03_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_04_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_05_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_06_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_07_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_08_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_09_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_10_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_11_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_12_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_13_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_14_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_15_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_16_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_17_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_18_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_19_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_20_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_21_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_22_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_23_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_24_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_25_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_26_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_27_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_28_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_29_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_30_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_31_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_32_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_33_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_34_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_35_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_36_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_37_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_38_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT013_39_testL( CStifItemParser& aItem );
        
        void AssertTrueL( TBool aCondition,const TDesC& aMessage);
        //ADD NEW METHOD DEC HERE

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

#endif      // T_DELETETRIGGERS_H

// End of File
