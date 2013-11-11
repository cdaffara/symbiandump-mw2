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
* Description:   Tests senduisingleton.h and CSendingService.h
*
*/




#ifndef SENDUIPLUGINAPITEST_H
#define SENDUIPLUGINAPITEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

// CONSTANTS

// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KSendUiPluginAPITestLogPath, "\\logs\\testframework\\SendUiPluginAPITest\\" ); 
// Log file
_LIT( KSendUiPluginAPITestLogFile, "SendUiPluginAPITest.txt" ); 
_LIT( KSendUiPluginAPITestLogFileWithTitle, "SendUiPluginAPITest_[%S].txt" );

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CSendUiPluginAPITest;

// DATA TYPES

// CLASS DECLARATION

/**
*  CSendUiPluginAPITest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CSendUiPluginAPITest) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSendUiPluginAPITest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CSendUiPluginAPITest();

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
        
    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */

    private:

        /**
        * C++ default constructor.
        */
        CSendUiPluginAPITest( CTestModuleIf& aTestModuleIf );

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
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        /**
         * Tests CSendUiSingleton::NewL
         */
        TInt TestCSendUiSingletonNewL( CStifItemParser& aItem );
        
        /**
         * Tests CSendUiSingleton::MsvSessionL
         */
        TInt TestMsvSessionL(CStifItemParser& aItem );
        
        /**
         * Tests CSendUiSingleton::ClientMtmRegistryL
         */
        TInt TestClientMtmRegistryL(CStifItemParser& aItem );
        
        /**
         * Tests CSendUiSingleton::MtmUiRegistryL
         */
        TInt TestMtmUiRegistryL(CStifItemParser& aItem );
       
        /**
         * Tests CSendUiSingleton::MtmUiDataRegistryL
         */
        TInt TestMtmUiDataRegistryL(CStifItemParser& aItem );        
        
        /**
         * Tests CSendingService::NewL, CSendingService::~CSendingService(), CSendingService::CSendingService
         */        
        TInt TestCSendingServiceL(CStifItemParser& aItem );
        
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

#endif      // SENDUIPLUGINAPITEST_H

// End of File
