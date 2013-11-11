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
* Description:     Test module to implement Cell based triggering test cases
*
*/






#ifndef CELLBASEDTRIGGEROPERATIONS_H
#define CELLBASEDTRIGGEROPERATIONS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <etel3rdparty.h>
#include <etelmm.h>
// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KCellbasedtriggerOperationsLogPath, "\\logs\\testframework\\CellbasedtriggerOperations\\" ); 
// Log file
_LIT( KCellbasedtriggerOperationsLogFile, "CellbasedtriggerOperations.txt" ); 
_LIT( KCellbasedtriggerOperationsLogFileWithTitle, "CellbasedtriggerOperations_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CCellbasedtriggerOperations;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CCellbasedtriggerOperations test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CCellbasedtriggerOperations) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCellbasedtriggerOperations* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCellbasedtriggerOperations();

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
        CCellbasedtriggerOperations( CTestModuleIf& aTestModuleIf );

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
         * Gets current cell information
         */
        void GetCurrentCellInfoL(  
                RMobilePhone::TMobilePhoneNetworkMode& aNetworkMode,
                CTelephony::TNetworkInfoV1& aNwInfo );

        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        void AssertTrueL( TBool aCondition,const TDesC& aMessage);

        virtual TInt TC_LBT_034_01_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT_034_02_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT_034_03_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_04_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_05_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT_034_06_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT_034_07_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_08_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_09_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT_034_10_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT_034_11_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_12_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT_034_13_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_14_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_15_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_16_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_17_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_18_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_19_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_20_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_21_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_22_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_23_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_24_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_25_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_26_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_27_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_28_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_29_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT_034_30_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT_034_31_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_32_testL( CStifItemParser& aItem );
        virtual TInt TC_LBT_034_33_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_34_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_35_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_36_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_37_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_38_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_39_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_40_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_41_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_42_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_43_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_44_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_45_testL( CStifItemParser& aItem );
		#if 0
		virtual TInt TC_LBT_034_46_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_47_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_48_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_49_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_50_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_51_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_52_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_53_testL( CStifItemParser& aItem );
		#endif
		virtual TInt TC_LBT_034_54_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_55_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_56_testL( CStifItemParser& aItem );
		#if 0
		virtual TInt TC_LBT_034_57_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_58_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_59_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_60_testL( CStifItemParser& aItem );
		#endif
		virtual TInt TC_LBT_034_61_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_62_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_65_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_66_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_67_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_68_testL( CStifItemParser& aItem );
		virtual	TInt StartCellChangeNotifier_testL( CStifItemParser&  aItem );
		virtual TInt TC_LBT_034_69_testL( CStifItemParser& aItem );
		virtual TInt TC_LBT_034_70_testL( CStifItemParser& aItem );
        
        
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

#endif      // CELLBASEDTRIGGEROPERATIONS_H

// End of File
