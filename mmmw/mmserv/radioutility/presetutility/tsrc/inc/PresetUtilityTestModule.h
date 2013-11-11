/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: STIF testclass declaration
*
*/

#ifndef PRESETUTILITYTESTMODULE_H
#define PRESETUTILITYTESTMODULE_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
//INTERNAL INCLUDES
#include <presetutility.h>
#include <presetutilityobserver.h>
#include <RadioFmPresetUtility.h>
#include <preset.h>
#include "trace.h"


// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file

const TInt KErrEventPending =-2000;             // Timeout and not callback 
const TInt KErrCallbackErrorCode = -2001;       // Callback return error code
const TInt KErrUnexpectedValue =-2002;          // Unexpected value at setting    
const TInt KExpectedFrequencyChange = -2003;        // Is not finding more than one station
const TInt KErrTimeoutController = -2007;       // Unexpected notification

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 1
#define TEST_CLASS_VERSION_MINOR 1
#define TEST_CLASS_VERSION_BUILD 1

// Logging path
_LIT( KPresetUtilityTestModuleLogPath, "\\logs\\testframework\\PresetUtilityTestModule\\" ); 
// Log file
_LIT( KPresetUtilityTestModuleLogFile, "PresetUtilityTestModule.txt" ); 
_LIT( KPresetUtilityTestModuleLogFileWithTitle, "PresetUtilityTestModule_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
//class CPresetUtilityTestModule;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CPresetUtilityTestModule test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CPresetUtilityTestModule) : public CScriptBase, public MRadioPresetObserver, public MPresetUtilityObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPresetUtilityTestModule* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CPresetUtilityTestModule();

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

        /**
        * From MPresetUtilityObserver
        * Called when a preset changes.
        *
        * NOTE: EPresetDeleted with aIndex == 0, indicates that all presets have been deleted.
        *
        * @since S60 3.2
        * @param aChange Change event type
        * @param aIndex Index to the preset that has changed. Zero means all presets.
        */
        virtual void PresetChanged( MPresetUtilityObserver::TPresetChangeEvent aChange, TInt aIndex );

        /**
        * From MRadioPresetObserver
        * Called when a preset changes.
        *
        * NOTE: EPresetDeleted with aIndex == 0, indicates that all presets have been deleted.
        *
        * @since S60 3.2
        * @param aChange Change event type
        * @param aIndex Index to the preset that has changed. Zero means all presets.
        */
        virtual void MrpeoPresetChanged( MRadioPresetObserver::TPresetChangeEvent aChange, TInt aIndex );

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );
        // Test case functions
        TInt CreateRadioPresetL( CStifItemParser& aItem );
        TInt CreatePresetUtilityL( CStifItemParser& aItem );
        TInt CreateFmPresetUtilityL( CStifItemParser& aItem );
        TInt DestroyRadioPresetL( CStifItemParser& aItem );
        TInt DestroyPresetUtilityL( CStifItemParser& aItem );
        TInt DestroyFmPresetUtilityL( CStifItemParser& aItem );
        
        TInt SavePresetL( CStifItemParser& aItem );
        
        TInt ReadPresetL( CStifItemParser& aItem );
                
        TInt MaxNumberOfPresets( CStifItemParser& aItem );
        
        TInt PresetCount( CStifItemParser& aItem );
        
        TInt FirstPreset( CStifItemParser& aItem );
        
        TInt NextPreset( CStifItemParser& aItem );
        
        TInt DeletePreset( CStifItemParser& aItem );
        
        // Old API
        TInt SetPresetL( CStifItemParser& aItem );
        TInt GetPresetL( CStifItemParser& aItem );
        TInt GetPresetNameL( CStifItemParser& aItem );
        TInt GetPresetFrequencyL( CStifItemParser& aItem );      
        TInt GetMaxNumberOfPresets( CStifItemParser& aItem );
        TInt GetNumberOfPresets( CStifItemParser& aItem );
        TInt GetFirstPresetL( CStifItemParser& aItem );
        TInt GetNextPresetL( CStifItemParser& aItem );
        TInt DeletePresetL( CStifItemParser& aItem );

        TInt SetPresetUrlL( CStifItemParser& aItem );
        TInt SetPresetPtyL( CStifItemParser& aItem );
        TInt SetPresetPICodeL( CStifItemParser& aItem );
        TInt SetPresetFavouriteInfoL( CStifItemParser& aItem );
        TInt SetPresetUserRenameInfoL( CStifItemParser& aItem );
        TInt GetPresetUrlL( CStifItemParser& aItem );
        TInt GetPresetPtyL( CStifItemParser& aItem );
        TInt GetPresetPICodeL( CStifItemParser& aItem );
        TInt GetPresetFavouriteInfoL( CStifItemParser& aItem );
        TInt GetPresetUserRenameInfoL( CStifItemParser& aItem );

        TInt SetFavorite( CStifItemParser& aItem );
        TInt Favorite( CStifItemParser& aItem );
        TInt SetLocalStation( CStifItemParser& aItem );
        TInt LocalStation( CStifItemParser& aItem );
        TInt SetRenamedByUser( CStifItemParser& aItem );
        TInt RenamedByUser( CStifItemParser& aItem );
        TInt SetGenre( CStifItemParser& aItem );
        TInt Genre( CStifItemParser& aItem );
        TInt SetPiCode( CStifItemParser& aItem );
        TInt PiCode( CStifItemParser& aItem );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CPresetUtilityTestModule( CTestModuleIf& aTestModuleIf );

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

        /** FM Preset Utility */
        CPresetUtility* iPresetUtility;

        /** Old interface FM Preset Utility */
        CRadioFmPresetUtility* iRadioFmPresetUtility;

        /** Preset class instance */
        TPreset* iPreset;
        
    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // PRESETUTILITYTESTMODULE_H

// End of File
