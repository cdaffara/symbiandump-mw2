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
* Description:    Test module to implement Firing of startup trigger and Listing of trigger related test cases
*
*/






#ifndef T_LBTLTAPI_H
#define T_LBTLTAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

// Forward declaration
class TCoordinate;
class MProEngEngine;

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
class CFiringofStartupTriggerAndListTrigger;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CFiringofStartupTriggerAndListTrigger test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CFiringofStartupTriggerAndListTrigger) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CFiringofStartupTriggerAndListTrigger* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CFiringofStartupTriggerAndListTrigger();

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
        CFiringofStartupTriggerAndListTrigger( CTestModuleIf& aTestModuleIf );

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
         * Set the phone profile to offline mode
         */
        void SetProfileToOfflineL();
        
        /**
         * Restore original profile
         */
        void RestoreProfileL();
                
        /**
         * Gets current coordinate
         */
        void GetCurrentCoordinateL( TCoordinate& aCoordinate );
            

        /**
         * Enable simulation PSY
         */
        void EnableSimPSYL();

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
               
        virtual TInt TCLBTFW070_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW071_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW072_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW073_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW074_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW075_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW076_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW077_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW078_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW079_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW080_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW081_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW082_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW083_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW084_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW085_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW086_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW087_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW088_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW089_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW090_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW091_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW092_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW093_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW094_testL( CStifItemParser& /* aItem */ );
        virtual TInt TCLBTFW095_testL( CStifItemParser& /* aItem */ );
        
         void AssertTrueL( TBool aCondition,const TDesC& aMessage);

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
         /**
          * Pointer to profile engine
          * Own.
          */
         MProEngEngine* iProEngine;
         
         /**
          * Current profile id
          */
         TInt iCurrentProfile;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // T_LBTLTAPI_H

// End of File
