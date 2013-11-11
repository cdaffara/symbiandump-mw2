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
* Description:    Test module to implement Test cases for LBT Management settings
*
*/





#ifndef T_MGMTSETTINGS_H
#define T_MGMTSETTINGS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( Kt_mgmtsettingsLogPath, "\\logs\\testframework\\t_mgmtsettings\\" ); 
// Log file
_LIT( Kt_mgmtsettingsLogFile, "t_mgmtsettings.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Ct_mgmtsettings;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Ct_mgmtsettings test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Ct_mgmtsettings) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ct_mgmtsettings* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ct_mgmtsettings();

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
        Ct_mgmtsettings( CTestModuleIf& aTestModuleIf );

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
        //ADD NEW METHOD DEC HERE
        
        void AssertTrueL( TBool aCondition,const TDesC& aMessage );
        
        virtual TInt GetDefaultSettingsL( CStifItemParser& /* aItem */ );
        
        virtual TInt SetFewSystemSettingsL( CStifItemParser& /* aItem */ );
        
        virtual TInt SetAllSystemSettingsL( CStifItemParser& /* aItem */ );
        
        virtual TInt GetAllSystemSettingsL( CStifItemParser& /* aItem */ );
        
        virtual TInt NotifyTriggeringSettingsChangeL( CStifItemParser& /* aItem */ );
        
        virtual TInt GetTrigger1L( CStifItemParser& /* aItem */ );
        
        virtual TInt UpdateTriggerL( CStifItemParser& /* aItem */ );
        
        virtual TInt SetTriggerState1L( CStifItemParser& /* aItem */ );
        
        virtual TInt SetTriggerState2L( CStifItemParser& /* aItem */ );
        
        virtual TInt SetTriggerState3L( CStifItemParser& /* aItem */ );
        
        virtual TInt SetTriggerState4L( CStifItemParser& /* aItem */ );
        
        virtual TInt SetTriggerState5L( CStifItemParser& /* aItem */ );
        
        virtual TInt ListTriggerId1L( CStifItemParser& /* aItem */ );
        
        virtual TInt ListTriggerId2L( CStifItemParser& /* aItem */ );
        
        virtual TInt ListTriggerId3L( CStifItemParser& /* aItem */ );
        
        virtual TInt ListTriggerId4L( CStifItemParser& /* aItem */ );
        
        virtual TInt GetTriggers1L( CStifItemParser& /* aItem */ );
        
        virtual TInt GetTriggers2L( CStifItemParser& /* aItem */ );
        
        virtual TInt GetTriggers3L( CStifItemParser& /* aItem */ );
        
        virtual TInt GetTriggers4L( CStifItemParser& /* aItem */ );
        
        virtual TInt GetNextTriggerL( CStifItemParser& /* aItem */ );
        
        virtual TInt TestDeleteTriggerL( CStifItemParser& /* aItem */ );
        
        virtual TInt TestDeleteTriggers1L( CStifItemParser& /* aItem */ );
        virtual TInt TestDeleteTriggers2L( CStifItemParser& /* aItem */ );
        virtual TInt TestDeleteTriggers3L( CStifItemParser& /* aItem */ );
        virtual TInt testGetNextTrigger1L( CStifItemParser& /* aItem */ );
        virtual TInt testGetNextTrigger2L( CStifItemParser& /* aItem */ );
        virtual TInt TestCreatetriggerL( CStifItemParser& /* aItem */ );
        virtual TInt Cell_GetTrigger1L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_UpdateTriggerL( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_SetTriggerState1L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_SetTriggerState2L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_SetTriggerState3L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_SetTriggerState4L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_SetTriggerState5L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_ListTriggerId1L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_ListTriggerId2L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_ListTriggerId3L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_ListTriggerId4L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_GetTriggers1L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_GetTriggers2L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_GetTriggers3L( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_GetTriggers4L( CStifItemParser& /* aItem */ );
        
      //  virtual TInt Cell_GetNextTriggerL( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_TestDeleteTriggerL( CStifItemParser& /* aItem */ );
        
        virtual TInt Cell_TestDeleteTriggers1L( CStifItemParser& /* aItem */ );
        virtual TInt Cell_TestDeleteTriggers2L( CStifItemParser& /* aItem */ );
        virtual TInt Cell_testGetNextTrigger1L( CStifItemParser& /* aItem */ );
        virtual TInt Cell_testGetNextTrigger2L( CStifItemParser& /* aItem */ );
        virtual TInt Cell_TestCreatetriggerL( CStifItemParser& /* aItem */ );
        
        virtual TInt TC_LBT015_01_testL( CStifItemParser& /* aItem */ );
        virtual TInt TC_LBT015_02_testL( CStifItemParser& /* aItem */ );
        virtual TInt TC_LBT015_03_testL( CStifItemParser& /* aItem */ );
        virtual TInt TC_LBT015_04_testL( CStifItemParser& /* aItem */ );
        virtual TInt TC_LBT015_05_testL( CStifItemParser& /* aItem */ );
        
        virtual TInt TestdeleteTrigger4L( CStifItemParser& aItem );
        virtual TInt TestdeleteTrigger5L( CStifItemParser& aItem );
         virtual TInt TestNotifyTriggerChangeEventL( CStifItemParser& aItem );

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

#endif      // T_MGMTSETTINGS_H

// End of File
