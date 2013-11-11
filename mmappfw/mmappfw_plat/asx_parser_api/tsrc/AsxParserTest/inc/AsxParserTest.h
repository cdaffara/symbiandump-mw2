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
* Description:  AsxParserTest.h
*
*/

// Version : %version: 3.1.2 %




#ifndef ASXPARSERTEST_H
#define ASXPARSERTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include "AsxParser.h"
// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
#ifdef __WINSCW__
_LIT( KAsxParserTestLogPath, "C:\\logs\\testframework\\AsxParserTest\\" ); 
// Log file
_LIT( KAsxParserTestLogFile, "AsxParserTest.txt" ); 
_LIT( KAsxParserTestLogFileWithTitle, "AsxParserTest_[%S].txt" );
_LIT( KAsxParserTest_testPath, "C:\\testing\\data\\" );
#else
_LIT( KAsxParserTestLogPath, "E:\\logs\\testframework\\AsxParserTest\\" ); 
// Log file
_LIT( KAsxParserTestLogFile, "AsxParserTest.txt" ); 
_LIT( KAsxParserTestLogFileWithTitle, "AsxParserTest_[%S].txt" );
_LIT( KAsxParserTest_testPath, "E:\\testing\\data\\" );
#endif
// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CAsxParserTest;
class CAsxParser;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CAsxParserTest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CAsxParserTest) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAsxParserTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CAsxParserTest();

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
        CAsxParserTest( CTestModuleIf& aTestModuleIf );

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
        TInt CreateNew( CStifItemParser& aItem );
        TInt CreateNewRFile( CStifItemParser& aItem );
        TInt FileVersion( CStifItemParser& aItem );
        TInt GetUrlCount( CStifItemParser& aItem );
        TInt GetUrl( CStifItemParser& aItem );
        TInt PrintUrl( CStifItemParser& aItem );
        TInt FileVersionRFile( CStifItemParser& aItem );
        TInt GetUrlCountRFile( CStifItemParser& aItem );
        TInt GetUrlRFile( CStifItemParser& aItem );
        TInt PrintUrlRFile( CStifItemParser& aItem );
        TInt CAsxParserDestructor(CStifItemParser& aItem );
        TInt GetAsxStruct( CStifItemParser& aItem );
        
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
    	CAsxParser* iAsxParser; 
    };

#endif      // ASXPARSERTEST_H

// End of File
