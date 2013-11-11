/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Test module to implement Test cases for BC
*
*/





#ifndef LBTBCTEST_H
#define LBTBCTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KlbtbctestLogPath, "\\logs\\testframework\\lbtbctest\\" ); 
// Log file
_LIT( KlbtbctestLogFile, "lbtbctest.txt" ); 
_LIT( KlbtbctestLogFileWithTitle, "lbtbctest_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Clbtbctest;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Clbtbctest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Clbtbctest) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Clbtbctest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Clbtbctest();

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
        Clbtbctest( CTestModuleIf& aTestModuleIf );

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
        void AssertTrueL( TBool aCondition,const TDesC& aMessage);

       
        virtual TInt TestRlbtOpen1L( CStifItemParser& aItem );
        virtual TInt TestRlbtOpen2L( CStifItemParser& aItem );
        virtual TInt TestRlbtConnect1L( CStifItemParser& aItem );
        virtual TInt TestRlbtCreateTriggerL( CStifItemParser& aItem );
        virtual TInt TestRlbtCreateTrigger2L( CStifItemParser& aItem );
        virtual TInt TestRlbtCreateTrigger3L( CStifItemParser& aItem );
        virtual TInt TestRlbtCreateTrigger4L( CStifItemParser& aItem );
        virtual TInt TestRlbtCreateTrigger5L( CStifItemParser& aItem );
        virtual TInt TestRlbtCancelCreateTriggerL( CStifItemParser& aItem );
        virtual TInt TestRlbtDeleteTriggerL( CStifItemParser& aItem );
        virtual TInt TestRlbtDeleteTriggers1L( CStifItemParser& aItem );
        virtual TInt TestRlbtDeleteTriggers2L( CStifItemParser& aItem );
        virtual TInt TestRlbtDeleteTriggers3L( CStifItemParser& aItem );
        virtual TInt TestRlbtDeleteTriggers4L( CStifItemParser& aItem );
        virtual TInt TestRlbtDeleteTriggers5L( CStifItemParser& aItem );
        virtual TInt TestRlbtDeleteTriggers6L( CStifItemParser& aItem );
        virtual TInt TestRlbtDeleteTriggers7L( CStifItemParser& aItem );
        virtual TInt TestRlbtDeleteTriggers8L( CStifItemParser& aItem );
        virtual TInt TestRlbtCancelDeleteTriggersL( CStifItemParser& aItem );
        virtual TInt TestRlbtGetTriggerLCL( CStifItemParser& aItem );
        virtual TInt TestRlbtUpdateTriggerL( CStifItemParser& aItem );
        virtual TInt TestRlbtUpdateTrigger2L( CStifItemParser& aItem );
        virtual TInt TestRlbtUpdateTrigger3L( CStifItemParser& aItem );
        virtual TInt TestRlbtUpdateTrigger4L( CStifItemParser& aItem );
        virtual TInt TestRlbtUpdateTrigger5L( CStifItemParser& aItem );
        virtual TInt TestRlbtUpdateTrigger6L( CStifItemParser& aItem );
        virtual TInt TestRlbtUpdateTrigger7L( CStifItemParser& aItem );
        virtual TInt TestRlbtUpdateTrigger8L( CStifItemParser& aItem );
        virtual TInt TestRlbtSetTriggerStateL( CStifItemParser& aItem );
        virtual TInt TestRlbtSetTriggersState1L( CStifItemParser& aItem );
        virtual TInt TestRlbtSetTriggersState2L( CStifItemParser& aItem );
        virtual TInt TestRlbtSetTriggersState3L( CStifItemParser& aItem );
        virtual TInt TestRlbtSetTriggersState4L( CStifItemParser& aItem );
        virtual TInt TestRlbtSetTriggersState5L( CStifItemParser& aItem );
        virtual TInt TestRlbtSetTriggersState6L( CStifItemParser& aItem );
        virtual TInt TestRlbtSetTriggersState7L( CStifItemParser& aItem );
        virtual TInt TestRlbtSetTriggersState8L( CStifItemParser& aItem );
        virtual TInt TestRlbtCancelSetTriggersStateL( CStifItemParser& aItem );
        virtual TInt TestRlbtListTriggerIds1L( CStifItemParser& aItem );
        virtual TInt TestRlbtListTriggerIds2L( CStifItemParser& aItem );
        virtual TInt TestRlbtListTriggerIds3L( CStifItemParser& aItem );
        virtual TInt TestRlbtListTriggerIds4L( CStifItemParser& aItem );
        virtual TInt TestRlbtListTriggerIds5L( CStifItemParser& aItem );
        virtual TInt TestRlbtListTriggerIds6L( CStifItemParser& aItem );
        virtual TInt TestRlbtListTriggerIds7L( CStifItemParser& aItem );
        virtual TInt TestRlbtListTriggerIds8L( CStifItemParser& aItem );
        virtual TInt TestRlbtCancelListTriggerIdsL( CStifItemParser& aItem );
        virtual TInt TestRlbtGetTriggers1L( CStifItemParser& aItem );
        virtual TInt TestRlbtGetTriggers2L( CStifItemParser& aItem );
        virtual TInt TestRlbtGetTriggers3L( CStifItemParser& aItem );
        virtual TInt TestRlbtGetTriggers4L( CStifItemParser& aItem );
        virtual TInt TestRlbtGetTriggers5L( CStifItemParser& aItem );
        virtual TInt TestRlbtGetTriggers6L( CStifItemParser& aItem );
        virtual TInt TestRlbtGetTriggers7L( CStifItemParser& aItem );
        virtual TInt TestRlbtCancelGetTriggersL( CStifItemParser& aItem );
        virtual TInt TestRlbtCreateGetTriggerIterator1L( CStifItemParser& aItem );
        virtual TInt TestRlbtCreateGetTriggerIterator2L( CStifItemParser& aItem );
        virtual TInt TestRlbtCreateGetTriggerIterator3L( CStifItemParser& aItem );
        virtual TInt TestRlbtCancelCreateTriggerIteratorL( CStifItemParser& aItem );
        virtual TInt TestRlbtNotifyTriggerChangeEventL( CStifItemParser& aItem );
        virtual TInt TestRlbtNotifyTriggerChangeEvent2L( CStifItemParser& aItem );
        virtual TInt TestRlbtNotifyTriggerFiredL( CStifItemParser& aItem );
        virtual TInt TestRlbtNotifyTriggerFired2L( CStifItemParser& aItem );
        virtual TInt TestRlbtCancelNotifyTriggerFiredL( CStifItemParser& aItem );
        virtual TInt TestRlbtGetFiredTriggersL( CStifItemParser& aItem );
        virtual TInt TestRlbtNotifyTriggeringSystemSettingChangeL( CStifItemParser& aItem );
        virtual TInt TestRlbtCancelNotifyTriggeringSystemSettingChangeL( CStifItemParser& aItem );
        virtual TInt TestRlbtGetTriggeringSystemSettingsL( CStifItemParser& aItem );
        virtual TInt TestRlbtCancelAllL( CStifItemParser& aItem );
        virtual TInt TestCLbtGeoCell1L(CStifItemParser& aItem);
		    virtual TInt TestCLbtGeoCell2L(CStifItemParser& aItem);
		    virtual TInt TestCLbtGeoCell3L(CStifItemParser& aItem);
		    virtual TInt TestCLbtGeoCell4L(CStifItemParser& aItem);
		    virtual TInt TestCLbtGeoCell5L(CStifItemParser& aItem);
		    virtual TInt TestCLbtGeoCell6L(CStifItemParser& aItem);
		    virtual TInt TestCLbtGeoCell7L(CStifItemParser& aItem);
		    virtual TInt TestCLbtGeoCircle1L(CStifItemParser& aItem);
				virtual TInt TestCLbtGeoCircle2L(CStifItemParser& aItem);
				virtual TInt TestCLbtGeoCircle3L(CStifItemParser& aItem);
				virtual TInt TestCLbtGeoCircle4L(CStifItemParser& aItem);
				virtual TInt TestCLbtGeoCircle5L(CStifItemParser& aItem);
				virtual TInt TestCLbtGeoCircle6L(CStifItemParser& aItem);
				virtual TInt TestCLbtGeoRect1L(CStifItemParser& aItem);   
				virtual TInt TestCLbtGeoRect2L(CStifItemParser& aItem);  
				virtual TInt TestCLbtGeoRect3L(CStifItemParser& aItem);  
				virtual TInt TestCLbtGeoRect4L(CStifItemParser& aItem);  
				virtual TInt TestCLbtGeoRect5L(CStifItemParser& aItem);  
				virtual TInt TestCLbtListTriggerOptions1L(CStifItemParser& aItem);  
				virtual TInt TestCLbtListTriggerOptions2L(CStifItemParser& aItem);  
				virtual TInt TestCLbtListTriggerOptions3L(CStifItemParser& aItem);  
				virtual TInt TestCLbtListTriggerOptions4L(CStifItemParser& aItem);  
				virtual TInt TestCLbtListTriggerOptions5L(CStifItemParser& aItem);  
				virtual TInt TestCLbtListTriggerOptions6L(CStifItemParser& aItem);  
				virtual TInt TestRLbtServerL(CStifItemParser& aItem);  
				virtual TInt TestCLbtSessionTrigger1L(CStifItemParser& aItem);  		  
				virtual TInt TestCLbtSessionTrigger2L(CStifItemParser& aItem);  		  
				virtual TInt TestCLbtSessionTrigger3L(CStifItemParser& aItem);  		  
				virtual TInt TestCLbtSessionTrigger4L(CStifItemParser& aItem);  
				virtual TInt TestCLbtSessionTrigger5L(CStifItemParser& aItem);  
				virtual TInt TestCLbtStartupTrigger1L(CStifItemParser& aItem); 
				virtual TInt TestCLbtStartupTrigger2L(CStifItemParser& aItem);
				virtual TInt TestCLbtStartupTrigger3L(CStifItemParser& aItem);
				virtual TInt TestCLbtStartupTrigger4L(CStifItemParser& aItem); 
				virtual TInt TestCLbtStartupTrigger5L(CStifItemParser& aItem); 	
				virtual TInt TestCLbtTriggerChangeEventNotifierL(CStifItemParser& aItem);	  
				virtual TInt TestCLbtTriggerConditionArea1L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerConditionArea2L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerConditionArea3L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerConditionArea4L(CStifItemParser& aItem);	
				virtual TInt TestCLbtTriggerConditionArea5L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerConditionArea6L(CStifItemParser& aItem);	
				virtual TInt TestCLbtTriggerConditionArea7L(CStifItemParser& aItem);	
				virtual TInt TestCLbtTriggerConditionArea8L(CStifItemParser& aItem);	
				virtual TInt TestCLbtTriggerEntry1L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerEntry2L(CStifItemParser& aItem); 	
				virtual TInt TestCLbtTriggerFilterByArea1L(CStifItemParser& aItem);	  
				virtual TInt TestCLbtTriggerFilterByArea2L(CStifItemParser& aItem);	  
				virtual TInt TestCLbtTriggerFilterByArea3L(CStifItemParser& aItem);	  
				virtual TInt TestCLbtTriggerFilterByArea4L(CStifItemParser& aItem);	  
				virtual TInt TestCLbtTriggerFilterByArea5L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerFilterByArea6L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerFilterByArea7L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerFilterByArea8L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerFilterByAttribute1L(CStifItemParser& aItem);	  
				virtual TInt TestCLbtTriggerFilterByAttribute2L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerFilterByAttribute3L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerFilterComposite1L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerFilterComposite2L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerFilterComposite3L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerFilterComposite4L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerFiringEventNotifierL(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerInfo1L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerInfo2L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerInfo3L(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggerInfo4L(CStifItemParser& aItem);
				virtual TInt TestTLbtTriggeringSystemSettingsL(CStifItemParser& aItem);
				virtual TInt TestCLbtTriggeringSystemSettingsChangeEventNotifier1L(CStifItemParser& aItem);
				virtual TInt TestRLbtAsyncUpdate1L(CStifItemParser& aItem);
				virtual TInt TestRLbtAsyncUpdate2L(CStifItemParser& aItem);
				virtual TInt TestRLbtAsyncUpdate3L(CStifItemParser& aItem);
				virtual TInt TestRLbtAsyncUpdate4L(CStifItemParser& aItem);
				virtual TInt TestRLbtAsyncUpdate5L(CStifItemParser& aItem);
				virtual TInt TestRLbtAsyncUpdate6L(CStifItemParser& aItem);
				virtual TInt TestRLbtAsyncUpdate7L(CStifItemParser& aItem);
				virtual TInt TestRLbtCancelUpdateTriggerL(CStifItemParser& aItem);
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

#endif      // LBTBCTEST_H

// End of File
