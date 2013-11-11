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
* Description:    Test module to implement Test cases for C,T Classes
*
*/





#ifndef T_LBTLTAPI_H
#define T_LBTLTAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( Kt_lbtltapiLogPath, "\\logs\\testframework\\t_lbtltapi\\" ); 
// Log file
_LIT( Kt_lbtltapiLogFile, "t_lbtltapi.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Ct_lbtltapi;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Ct_lbtltapi test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Ct_lbtltapi) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ct_lbtltapi* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ct_lbtltapi();

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
        virtual TInt RunMethodL( CStifItemParser&  aItem );

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
        Ct_lbtltapi( CTestModuleIf& aTestModuleIf );

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
        virtual TInt TCLBTFW001_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW002_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW003_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW004_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW005_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW006_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW007_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW008_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW009_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW010_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW011_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW012_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW013_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW014_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW015_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW016_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW017_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW018_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW019_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW020_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW021_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW022_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW023_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW024_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW025_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW026_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW027_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW028_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW029_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW030_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW031_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW032_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW033_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW034_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW035_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW036_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW037_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW038_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW039_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW040_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW041_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW042_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW043_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW044_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW045_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW046_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW047_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW048_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW049_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW050_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW051_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW052_testL( CStifItemParser& /* aItem */ );
        virtual TInt TC_LBT015_01_testL( CStifItemParser& /* aItem */ );
        virtual TInt TC_LBT015_02_testL( CStifItemParser& /* aItem */ );
        virtual TInt TC_LBT015_03_testL( CStifItemParser& /* aItem */ );
        virtual TInt TC_LBT015_04_testL( CStifItemParser& /* aItem */ );
        virtual TInt TC_LBT015_05_testL( CStifItemParser& /* aItem */ );
         
        
         void AssertTrueL( TBool aCondition,const TDesC& aMessage);

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

#endif      // T_LBTLTAPI_H

// End of File
