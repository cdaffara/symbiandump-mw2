/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Test Module DLL to Location Settings Launch API
*
*/



#ifndef TESTMNCLIENTLIB_H
#define TESTMNCLIENTLIB_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <swinstapi.h>

// CONSTANTS
_LIT( KtestmnclientlibLogPath, "\\logs\\testframework\\testmnclientlib\\" ); 
_LIT( KtestmnclientlibLogFile, "testmnclientlib.txt" ); 
_LIT( KProviderSisxPath, "e:\\testing\\data\\mnrefprovider.sisx" ); 
_LIT( KMnPanicInternal, "MnInternal" );

// FORWARD DECLARATIONS
class CTestMNClientLib;
class CTestMnGeocoder;

// CLASS DECLARATION

/**
*  CTestMNClientLib test class for STIF Test Framework TestScripter.
*  This is the testclass for Geocoding API
*
*  @lib testmnclientlib.lib
*  @since S60 v3.2
*/
NONSHARABLE_CLASS(CTestMNClientLib) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTestMNClientLib* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTestMNClientLib();

    public: // New functions
    
	    /**
	     * Test Case for Creating the new instance of CMnGeocoder 
	     * using NewL
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	  	  TInt TestNewL( CStifItemParser& aItem );
	      
	    /**
	     * Test Case for Creating the new instance of CMnGeocoder 
	     * using NewChainedL - Expected to Fail with KErrNotsupported
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	      TInt TestNewChainedL( CStifItemParser& aItem );
    
	    /**
	     * Test Case for Setting and Getting options 
	     * of CMnGeocoder
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	  	  TInt TestGetSetOptionsL( CStifItemParser& aItem );
	      
	    /**
	     * Test Case for converting coordinate to Address 
	     * Asynchronous Call
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	  	  TInt TestFindAddressByCoordinateL( CStifItemParser& aItem );
    
	    /**
	     * Test Case for converting Address(CPosLandmark) to coordinate
	     * Asynchronous Call
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	  	  TInt TestFindCoordinateByAddressL( CStifItemParser& aItem );
    
	    /**
	     * Test Case for converting Address(Desc) to coordinate
	     * Asynchronous Call
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	  	  TInt TestFindCoordinateByAddressDescL( CStifItemParser& aItem );
    
	    /**
	     * Test Case for Setting the Exit Observer 
	     * for CMnGeocoder
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	  	  TInt TestSetExitObserverL( CStifItemParser& aItem );
	    
	    /**
	     * Test Case for Removing the Exit Observer 
	     * for CMnGeocoder
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	  	  TInt TestRemoveExitObserverL( CStifItemParser& aItem );
    
	    /**
	     * Test Case for Cancelling a the request given to 
	     * CMnGeocoder
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	  	  TInt TestCancelRequestL( CStifItemParser& aItem );
	      
	    /**
	     * Test Case for allowing a panic(KMnPanicDuplicateRequest) on  
	     * issuing duplicate requests on CMnGeocoder
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	  	  TInt TestRequestPanicL( CStifItemParser& aItem );


    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since S60 v3.2
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    private:

        /**
        * C++ default constructor.
        */
          CTestMNClientLib( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
          void ConstructL();
          
        /**
        * Install Dependent sisx files.
        */
		  TInt Install( const TDesC& aPackagePath );
		  
        /**
        * Frees all resources allocated from test methods.
        * @since S60 v3.2
        */
          void Delete();

    private:    // Data
        CTestMnGeocoder* iTestMnGeocoder;
    };

#endif      // TESTMNCLIENTLIB_H

// End of File
