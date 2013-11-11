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
* Description:    Test module to implement Server startup related test cases
*
*/





#ifndef CLIENTSERVERSETUP_H
#define CLIENTSERVERSETUP_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( KClientServerSetupLogPath, "\\logs\\testframework\\ClientServerSetup\\" ); 
// Log file
_LIT( KClientServerSetupLogFile, "ClientServerSetup.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CClientServerSetup;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CClientServerSetup test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CClientServerSetup) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CClientServerSetup* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CClientServerSetup();

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
        CClientServerSetup( CTestModuleIf& aTestModuleIf );

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
       // virtual TInt ExampleL( CStifItemParser& aItem );
		virtual TInt EstablishConnectionL( CStifItemParser& aItem );
		virtual TInt EstablishAndCloseConnectionL( CStifItemParser& aItem );
		virtual TInt EstablishMultipleConnectionL( CStifItemParser& aItem );
		virtual TInt ErrorAlreadyConnectedL( CStifItemParser& aItem );
		virtual TInt ErrorServerNotRunningL( CStifItemParser& aItem );
		virtual TInt ErrorAlreadyClosedL( CStifItemParser& aItem );
		virtual TInt ErrorCloseWithoutConnectL( CStifItemParser& aItem );
		virtual TInt MultipleServerMultipleConnectionL( CStifItemParser& aItem );
		virtual TInt OpenConnectionL( CStifItemParser& aItem );
		virtual TInt ErrorOpenMultipleConnectionL( CStifItemParser& aItem );	
		virtual TInt MultipleClientMultipleConnectionL( CStifItemParser& aItem );	
		virtual TInt ErrorServerNotConnectedL( CStifItemParser& aItem );	
		virtual TInt ErrorSubsessionNotClosedL( CStifItemParser& aItem );	
		virtual TInt ErrorSubsessionAlreadyClosedL( CStifItemParser& aItem );	
		virtual TInt GetLBTServerVersionL( CStifItemParser& aItem );	
		virtual TInt ErrorGetLBTServerVersionL( CStifItemParser& aItem );
		virtual TInt FindServerL( CStifItemParser& aItem );	
		virtual TInt StatusInformationSupervisionNotActiveL( CStifItemParser& aItem );
		virtual TInt RequestStatusChangeNotificationL( CStifItemParser& aItem );						
    	virtual TInt killLBTServerL( CStifItemParser& aItem );	
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

#endif      // CLIENTSERVERSETUP_H

// End of File
