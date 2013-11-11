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
* Description:    Test module to implement Advanced trigger supervision cases
*
*/





#ifndef ADVANCEDTRIGGERSUPERVISION_H
#define ADVANCEDTRIGGERSUPERVISION_H

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
_LIT( KAdvancedTriggerSupervisionLogPath, "\\logs\\testframework\\AdvancedTriggerSupervision\\" ); 
// Log file
_LIT( KAdvancedTriggerSupervisionLogFile, "AdvancedTriggerSupervision.txt" ); 
_LIT( KAdvancedTriggerSupervisionLogFileWithTitle, "AdvancedTriggerSupervision_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CAdvancedTriggerSupervision;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CAdvancedTriggerSupervision test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CAdvancedTriggerSupervision) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAdvancedTriggerSupervision* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CAdvancedTriggerSupervision();

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
        CAdvancedTriggerSupervision( CTestModuleIf& aTestModuleIf );

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
         * Gets current coordinate
         */
        void GetCurrentCoordinateL( TCoordinate& aCoordinate );
        
        /**
         * Set the phone profile to offline mode
         */
        void SetProfileToOfflineL();
        
        /**
         * Restore original profile
         */
        void RestoreProfileL();
                
            
        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ATSTest1L( CStifItemParser& aItem );
		virtual TInt ATSTest2L( CStifItemParser& aItem );
		virtual TInt ATSTest3L( CStifItemParser& aItem );
		virtual TInt ATSTest4L( CStifItemParser& aItem );
		virtual TInt ATSTest5L( CStifItemParser& aItem );
		virtual TInt ATSTest6L( CStifItemParser& aItem );
		virtual TInt ATSTest7L( CStifItemParser& aItem );
		virtual TInt ATSTest8L( CStifItemParser& aItem );
		virtual TInt ATSTest9L( CStifItemParser& aItem );
		virtual TInt ATSTest10L( CStifItemParser& aItem );
		virtual TInt ATSTest11L( CStifItemParser& aItem );
		virtual TInt ATSTest12L( CStifItemParser& aItem );
		virtual TInt ATSTest13L( CStifItemParser& aItem );
		virtual TInt ATSTest14L( CStifItemParser& aItem );
		virtual TInt ATSTest15L( CStifItemParser& aItem );
		virtual TInt ATSTest16L( CStifItemParser& aItem );
		virtual TInt ATSTest17L( CStifItemParser& aItem );
		virtual TInt ATSTest18L( CStifItemParser& aItem );
		virtual TInt ATSTest19L( CStifItemParser& aItem );
		virtual TInt ATSTest20L( CStifItemParser& aItem );
		virtual TInt ATSTest21L( CStifItemParser& aItem );
        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

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

#endif      // ADVANCEDTRIGGERSUPERVISION_H

// End of File
