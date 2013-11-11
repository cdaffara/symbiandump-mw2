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
* Description:  Test Module DLL to Geocoding API
*
*/


#ifndef CTESTMNGEOCODER_H
#define CTESTMNGEOCODER_H

// System Includes
#include <StifLogger.h>
#include <StifTestModule.h>
#include <StifParser.h>
#include <e32base.h>
#include <e32std.h>
#include <AknServerApp.h>


// Constants
const TInt KProviderUid = 0x03132014;

//Forward Declarations
class CMnGeocoder;
class CMnProvider;
class CPosLandmark;

// CLASS DECLARATION
/**
 *  Test Class to invoke the CTestMNClientLib methods
 *
 *  This class is a helper class. The status variable of this active obect is 
 *  used to provide the asynchronous method Calls.
 *
 *  @lib testmnclientlib.lib
 *  @since S60 v3.2
 */

class CTestMnGeocoder :public CActive, public MAknServerAppExitObserver
    {
    public:
    
    	enum TFunction
    		{
    		EFindAddressByCoordinateL = 0,
    		EFindCoordinateByAddressL,
    		EFindCoordinateByAddressDescL
    		};
    
    public:  // Constructors and destructor

        /**
        * Destructor. 
        */
        static CTestMnGeocoder* NewL(CStifLogger* aLog);
        
        /**
        * Destructor. 
        */
        ~CTestMnGeocoder();

    public:         
        
        //From CActive
          void RunL();
          void DoCancel();
		  TInt RunError(TInt aError);   

		//
	    /**
	     * Test Case for Creating the new instance of CMnGeocoder 
	     * using NewL
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
		  TInt TestNewL();
		
	    /**
	     * Test Case for Creating the new instance of CMnGeocoder 
	     * using NewChainedL - Expected to Fail with KErrNotsupported
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */
	 	  TInt TestNewChainedL();
		  		  	 		
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
  		  TInt TestSetExitObserverL( );
			    
	    /**
	     * Test Case for Removing the Exit Observer 
	     * for CMnGeocoder
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
		  TInt TestRemoveExitObserverL( );
		    
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
		
		// From MAknServerAppExitObserver
		  void HandleServerAppExit(TInt aReason);	

	private:
	
		void ConstructL();
	
        /**
        * C++ default constructor.
        */
        CTestMnGeocoder(CStifLogger* aLog);
	 	    
    private:  //data
	    /**
	     * CMnGeocoder Client instance
	     */     		 
    	CMnGeocoder*         iMnGeocoder;
    	
   		CMnProvider* iMnProvider;

		RPointerArray<CMnProvider> iProviderArray;

		CPosLandmark* iLmk;

		TUid iUid;

		TFunction iFunction;
	    /**
	     * iLog instance - Not owned
	     */         
        CStifLogger* 			iLog;
	        
     };     

#endif      // CTESTMNGEOCODER_H

// End of File
