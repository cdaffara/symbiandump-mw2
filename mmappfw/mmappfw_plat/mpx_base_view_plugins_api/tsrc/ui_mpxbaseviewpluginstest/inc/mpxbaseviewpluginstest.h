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
* Description:  Cmpxbaseviewpluginstest header for STIF Test Framework TestScripter
*
*/



#ifndef MPXBASEVIEWPLUGINSTEST_H
#define MPXBASEVIEWPLUGINSTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <mpxviewplugin.h>
#include <mpxcustomcommandobserver.h>
#include <e32def.h>
#include "dummyMPXViewPlugin.h"
// CONSTANTS
//const ?type ?constant_var = ?constant;
const TUid KMPXMetadataEditorDialogImplementationId  = { 0x101FFC83 };
const TUid KMPXAudioEffectsViewImplementationId = { 0x101FFC74 };

// Logging path
_LIT( KmpxbaseviewpluginstestLogPath, "\\logs\\testframework\\mpxbaseviewpluginstest\\" ); 
// Log file
_LIT( KmpxbaseviewpluginstestLogFile, "mpxbaseviewpluginstest.txt" ); 
_LIT( KmpxbaseviewpluginstestLogFileWithTitle, "mpxbaseviewpluginstest_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;


class CMPXViewPlugin;
class Cmpxbaseviewpluginstest;
class Cmpxcustomcommandobservertestimp;
// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Cmpxbaseviewpluginstest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cmpxbaseviewpluginstest) : public CScriptBase,
											 public MMPXCustomCommandObserver 
											 
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Cmpxbaseviewpluginstest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cmpxbaseviewpluginstest();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );
    	virtual void HandleCustomCommand( 
    	                const TUid& aUid,
    	                const TInt aCommand, 
    	                const TDesC8& aData );
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
        Cmpxbaseviewpluginstest( CTestModuleIf& aTestModuleIf );

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
       
        TInt CMPXViewPluginNewL( CStifItemParser& /*aItem*/ );
        TInt CMPXViewPluginDestruct( CStifItemParser& /*aItem*/ );

        TInt CMPXAknViewPluginDestruct( CStifItemParser& /*aItem*/);
        TInt CMPXAknViewPluginCreateViewL( CStifItemParser& /*aItem*/);
        TInt CMPXAknViewPluginDestroyView( CStifItemParser& /*aItem*/);
        TInt CMPXAknViewPluginActivateViewL( CStifItemParser& /*aItem*/);
        TInt CMPXAknViewPluginSetAsDefaultViewL( CStifItemParser& /*aItem*/);
        TInt CMPXAknViewPluginDeactivateView( CStifItemParser& /*aItem*/);                     
        
        TInt CMPXAknDialogPluginDestruct( CStifItemParser& aItem);
        TInt CMPXAknDialogPluginCreateViewL( CStifItemParser& /*aItem*/);
        TInt CMPXAknDialogPluginDestroyView( CStifItemParser& /*aItem*/);
        TInt CMPXAknDialogPluginActivateViewL( CStifItemParser& /*aItem*/);
        TInt CMPXAknDialogPluginSetAsDefaultViewL( CStifItemParser& /*aItem*/);
        TInt CMPXAknDialogPluginDeactivateView( CStifItemParser& /*aItem*/);                     
        TInt CDummyMPXAknViewPluginConstruct( CStifItemParser& /*aItem*/);
        TInt CDummyMPXAknDialogPluginConstruct( CStifItemParser& /*aItem*/);
        TInt CDummyMPXViewPluginDesConstruct( CStifItemParser& /*aItem*/);
        /**
         * Method used to log version of test class
         */
        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

        private:    // Friend classes
        //?friend_class_declaration;s
    	CMPXViewPlugin* iViewPlugin;
    	CMPXViewPlugin* idummyMPXViewPlugin;
    };

#endif      // MPXBASEVIEWPLUGINSTEST_H

// End of File
