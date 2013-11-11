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
* Description:  Ctestsuplinitapi class
*
*/



#ifndef TESTSUPLINITAPI_H
#define TESTSUPLINITAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// MACROS
// Logging path
_LIT( KtestsuplinitapiLogPath, "\\logs\\testframework\\testsuplinitapi\\" ); 
// Log file
_LIT( KtestsuplinitapiLogFile, "testsuplinitapi.txt" ); 

// FORWARD DECLARATIONS
class Ctestsuplinitapi;


/**
*  Ctestsuplinitapi test class for STIF Test Framework TestScripter.
*  @lib testsuplinitapi.lib
*  @since S60 v3.2
*/
NONSHARABLE_CLASS(Ctestsuplinitapi) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ctestsuplinitapi* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ctestsuplinitapi();

   
    public: 

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );
        
        

       private:

        /**
        * C++ default constructor.
        */
        Ctestsuplinitapi( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        
        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * Test case to test Connect method of RSuplNetworkServer class. 
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt RSuplNetworkServer_ConnectL(CStifItemParser& aItem);
        
        /**
        * Test case to test ForwardMessageL method of RSuplNetworkServer class. 
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
		TInt RSuplNetworkServer_ForwardMessageL(CStifItemParser& aItem);
        
        /**
        * Test case to test Version method of RSuplNetworkServer class. 
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt RSuplNetworkServer_VersionL(CStifItemParser& aItem);
        
    
    };

#endif      // TESTSUPLINITAPI_H

// End of File
