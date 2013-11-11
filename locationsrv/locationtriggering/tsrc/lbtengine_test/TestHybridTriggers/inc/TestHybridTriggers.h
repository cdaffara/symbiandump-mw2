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
* Description:    STIF testclass declaration
*
*/




#ifndef TESTHYBRIDTRIGGERS_H
#define TESTHYBRIDTRIGGERS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#include "LbtAppCellInfo.h"
#include <e32cmn.h>
#include <e32des16.h>
#include <e32base.h>
#include <s32mem.h>
#include <e32math.h> 
#include <centralrepository.h>
#include <lbtcommon.h>
#include <lbterrors.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeocell.h>
#include <lbtgeorect.h>
#include <lbtgeohybrid.h>
#include <lbtlisttriggeroptions.h>
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbtserver.h>
#include <lbttriggerchangeevent.h>
#include <lbttriggerchangeeventnotifier.h>
#include <lbttriggerchangeeventobserver.h>
#include <lbttriggerconditionarea.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerdynamicinfo.h>
#include <lbttriggerentry.h>
#include <lbttriggerfilterbase.h>
#include <lbttriggerfilterbyarea.h>
#include <lbttriggerfilterbyattribute.h>
#include <lbttriggerfiltercomposite.h>
#include <lbttriggerfiringeventnotifier.h>
#include <lbttriggerfiringeventobserver.h>
#include <lbttriggerinfo.h>
#include <lbttriggeringsystemsettings.h>
#include <lbttriggeringsystemsettingschangeeventnotifier.h>
#include <lbttriggeringsystemsettingschangeeventobserver.h>
#include "AsynOperationAO.h"

#include <flogger.h>
_LIT(KLbtTraceDir,"epos");
 _LIT(KLbtTraceFile,"TriggerFireInfo.log");

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KTestHybridTriggersLogPath, "\\logs\\testframework\\TestHybridTriggers\\" ); 
// Log file
_LIT( KTestHybridTriggersLogFile, "TestHybridTriggers.txt" ); 
_LIT( KTestHybridTriggersLogFileWithTitle, "TestHybridTriggers_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CTestHybridTriggers;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CTestHybridTriggers test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CTestHybridTriggers) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTestHybridTriggers* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTestHybridTriggers();

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
        CTestHybridTriggers( CTestModuleIf& aTestModuleIf );

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
        void AssertTrueL( TBool aCondition,const TDesC& aMessage);
        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TC_TestNewL1L( CStifItemParser& aItem );
        virtual TInt TC_TestNewL2L( CStifItemParser& aItem );
        virtual TInt TC_TestNewL3L( CStifItemParser& aItem );
        virtual TInt TC_TestNewL4L( CStifItemParser& aItem );
        virtual TInt TC_TestNewL5L( CStifItemParser& aItem );
        virtual TInt TC_TestNewLC1L( CStifItemParser& aItem );
        virtual TInt TC_TestNewLC2L( CStifItemParser& aItem );
        virtual TInt TC_TestNewLC3L( CStifItemParser& aItem );
        virtual TInt TC_TestNewLC4L( CStifItemParser& aItem );
        virtual TInt TC_TestNewLC5L( CStifItemParser& aItem );
        virtual TInt TC_TestSetHybridArea1L( CStifItemParser& aItem );
        virtual TInt TC_TestSetHybridArea2L( CStifItemParser& aItem );
        virtual TInt TC_TestSetHybridArea3L( CStifItemParser& aItem );
        virtual TInt TC_TestInternalizeAndExternalizeL( CStifItemParser& aItem );
        virtual TInt TC_TestCLbtTriggerConditionArea1L( CStifItemParser& aItem );
        virtual TInt TC_TestCLbtTriggerConditionArea2L( CStifItemParser& aItem );
        virtual TInt TC_TestCLbtTriggerConditionArea3L( CStifItemParser& aItem );
        virtual TInt TC_TestCLbtTriggerConditionArea4L( CStifItemParser& aItem );
        virtual TInt TC_TestCLbtTriggerFilterByArea1L( CStifItemParser& aItem );
        virtual TInt TC_TestCLbtTriggerFilterByArea2L( CStifItemParser& aItem );
        virtual TInt TC_TestCLbtTriggerFilterByArea3L( CStifItemParser& aItem );
        virtual TInt TC_TestCLbtTriggerFilterByArea4L( CStifItemParser& aItem );
        virtual TInt TC_TestCreatetrigger1L( CStifItemParser& aItem );
        virtual TInt TC_TestCreatetrigger2L( CStifItemParser& aItem );
        virtual TInt TC_TestCreatetrigger3L( CStifItemParser& aItem );
        virtual TInt TC_TestCreatetrigger4L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTrigger1L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTrigger2L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers1L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers2L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers3L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers4L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers5L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers6L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers7L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers8L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers9L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers10L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers11L( CStifItemParser& aItem );
        virtual TInt TC_TestDeleteTriggers12L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers1L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers2L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers3L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers4L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers5L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers6L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers7L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers8L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers9L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers10L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers11L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers12L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator1L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator2L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator3L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator4L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator5L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator6L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator7L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator8L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator9L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator10L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator11L( CStifItemParser& aItem );
        virtual TInt TC_TestGetTriggers_Iterator12L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState1L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState2L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState3L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState4L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState5L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState6L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState7L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState8L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState9L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState10L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState11L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggersState12L( CStifItemParser& aItem );
        virtual TInt TC_TestSetTriggerState1L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds1L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds2L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds3L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds4L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds5L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds6L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds7L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds8L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds9L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds10L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds11L( CStifItemParser& aItem );
        virtual TInt TC_TestListtriggerIds12L( CStifItemParser& aItem );
        virtual TInt TC_TestGettriggerLC1L( CStifItemParser& aItem );
        virtual TInt TC_TestGettriggerLC2L( CStifItemParser& aItem );
        virtual TInt TC_TestUpdateTrigger1L( CStifItemParser& aItem );
        virtual TInt TC_TestUpdateTrigger2L( CStifItemParser& aItem );
        virtual TInt TC_TestUpdateTrigger3L( CStifItemParser& aItem );
        virtual TInt TC_TestUpdateTrigger4L( CStifItemParser& aItem );
        virtual TInt TC_TestUpdateTrigger5L( CStifItemParser& aItem );
        virtual TInt TC_TestUpdateTrigger6L( CStifItemParser& aItem );
        virtual TInt TC_TestHybridTriggerFiring1L( CStifItemParser& aItem );
        virtual TInt TC_TestHybridTriggerFiring2L( CStifItemParser& aItem );
        virtual TInt StartCellChangeNotifierL( CStifItemParser& aItem );
        
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
        TInt TC_TestDeleteTriggers3L( );
        TInt TC_TestDeleteTriggers4L();
        TInt TC_TestDeleteTriggers5L();
        TInt TC_TestDeleteTriggers6L();
        TInt TC_TestDeleteTriggers7L();
        TInt TC_TestDeleteTriggers8L();
        TInt TC_TestDeleteTriggers9L();
        TInt TC_TestDeleteTriggers10L();
        TInt TC_TestDeleteTriggers11L();
        TInt TC_TestDeleteTriggers12L();
        TInt TC_TestGetTriggers3L();
        TInt TC_TestGetTriggers4L();
        TInt TC_TestGetTriggers5L();
        TInt TC_TestGetTriggers6L();
        TInt TC_TestGetTriggers7L();
        TInt TC_TestGetTriggers8L();
        TInt TC_TestGetTriggers9L();
        TInt TC_TestGetTriggers10L();
        TInt TC_TestGetTriggers11L();
        TInt TC_TestGetTriggers12L();
        TInt TC_TestGetTriggers_Iterator3L();
        TInt TC_TestGetTriggers_Iterator4L();
        TInt TC_TestGetTriggers_Iterator5L();
        TInt TC_TestGetTriggers_Iterator6L();
        TInt TC_TestGetTriggers_Iterator7L();
        TInt TC_TestGetTriggers_Iterator8L();
        TInt TC_TestGetTriggers_Iterator9L();
        TInt TC_TestGetTriggers_Iterator10L();
        TInt TC_TestGetTriggers_Iterator11L();
        TInt TC_TestGetTriggers_Iterator12L();
        TInt TC_TestSetTriggersState3L();
        TInt TC_TestSetTriggersState4L();
        TInt TC_TestSetTriggersState5L();
        TInt TC_TestSetTriggersState6L();
        TInt TC_TestSetTriggersState7L();
        TInt TC_TestSetTriggersState8L();
        TInt TC_TestSetTriggersState9L();
        TInt TC_TestSetTriggersState10L();
        TInt TC_TestSetTriggersState11L();
        TInt TC_TestSetTriggersState12L();
        TInt TC_TestListtriggerIds3L();
        TInt TC_TestListtriggerIds4L();
        TInt TC_TestListtriggerIds5L();
        TInt TC_TestListtriggerIds6L();
        TInt TC_TestListtriggerIds7L();
        TInt TC_TestListtriggerIds8L();
        TInt TC_TestListtriggerIds9L();
        TInt TC_TestListtriggerIds10L();
        TInt TC_TestListtriggerIds11L();
        TInt TC_TestListtriggerIds12L();

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

#endif      // TESTHYBRIDTRIGGERS_H

// End of File
