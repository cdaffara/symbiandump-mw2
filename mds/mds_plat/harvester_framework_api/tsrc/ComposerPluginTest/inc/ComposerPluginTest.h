/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#ifndef COMPOSERPLUGINTEST_H
#define COMPOSERPLUGINTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <mdesession.h>
#include <composerplugin.h>

// CONSTANTS

_LIT( KComposerPluginTestLogPath, "\\logs\\testframework\\ComposerPluginTest\\" ); 
// Log file
_LIT( KComposerPluginTestLogFile, "ComposerPluginTest.txt" ); 
_LIT( KComposerPluginTestLogFileWithTitle, "ComposerPluginTest_[%S].txt" );

// FORWARD DECLARATIONS
class CComposerPluginTest;

// CLASS DECLARATION
NONSHARABLE_CLASS(CComposerPluginTest) : public CScriptBase,
									     public MMdESessionObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CComposerPluginTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CComposerPluginTest();

    public: // Functions from base classes

        virtual TInt RunMethodL( CStifItemParser& aItem );

    private: // Functions from base classes
    	
    	void HandleSessionOpened(CMdESession& aSession, TInt aError);
    	void HandleSessionError(CMdESession& aSession, TInt aError);

    private:

        /**
        * C++ default constructor.
        */
        CComposerPluginTest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        */
        void Delete();
        
        /**
        * Test methods are listed below. 
        */
        //[TestMethods] - Do not remove
        virtual TInt ActiveWait( CStifItemParser& aItem );
        virtual TInt BeginComposerSessionL( CStifItemParser& aItem );
        virtual TInt EndComposerSession( CStifItemParser& aItem );
        virtual TInt LoadPluginsL( CStifItemParser& aItem );
        virtual TInt SetSessions( CStifItemParser& aItem );
        virtual TInt RemoveSessions( CStifItemParser& aItem );
        virtual TInt ModifyObjectsL( CStifItemParser& aItem );
        virtual TInt DeleteObjectL( CStifItemParser& aItem );
        virtual TInt DeleteObjectsL( CStifItemParser& aItem );
        virtual TInt WaitComposingComplete( CStifItemParser& aItem );

    private:    // Data
        
        CMdESession* iMdEClient;
        
        RPointerArray<CComposerPlugin> iPluginArray;
    };

#endif      // COMPOSERPLUGINTEST_H

// End of File
