/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  API test automation for thumbnail engine
*
*/


#ifndef TNETEST_H
#define TNETEST_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <e32debug.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <w32std.H>
#include <StifItemParser.h>
#include <StifLogger.h>//logging in STIF
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <Stiftestinterface.h>
#include <tnevideoclipinfo.h> // Thumbnail Main API
// #include "TimeoutController.h"





const TInt KErrTimeoutController = -2007;		// Unexpected notification
_LIT(KTagId, " ");
// FORWARD DECLARATIONS
class CTneTestClass;


// CLASS DECLARATION

/**
*  CTneTestClass test class for STIF Test Framework TestScripter.
*
*  @lib RadioUtilityTestClass.lib
*  @since Series60_3_2
*/
NONSHARABLE_CLASS(CTneTestClass) : public CScriptBase, 
										public MTNEVideoClipInfoObserver,
                    public MTNEVideoClipThumbObserver
                   // public MTimeoutObserver   later
    {
    public:  // Constructors and destructor

     
        static CTneTestClass* NewL( CTestModuleIf& aTestModuleIf );
        
       
       

        /**
        * Destructor.
        */
        virtual ~CTneTestClass();


    

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since 3.2
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

	    
      
      
      // From MTimeoutObserver
	    /**
	    * @since 3.2
	    * @param none
	    * Review if all the expected events have ocurred once the time is over
	    */
	   // void HandleTimeout(TInt error);

	    
        
   

    private:

        /**
        * C++ default constructor.
        */
        CTneTestClass( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
       TInt	FindFileName(CStifItemParser& aItem) ;

        // Prohibit copy constructor if not deriving from CBase.
        // Prohibit assigment operator if not deriving from CBase.

        //Timeout function
        //later
       // TInt SetTimeout( CStifItemParser& aItem );

//logic to open TNE instance 

// NewL File name 
		  TInt TNENewLFileNameL(CStifItemParser& aItem);
// Newl File handle		  
		  TInt TNENewLFileHandleL(CStifItemParser& aItem);
// NewLC File name      
      TInt TNENewLCFileNameL(CStifItemParser& aItem);
// NewLC File handle        
      TInt TNENewLCFileHandleL(CStifItemParser& aItem);

      // Thumbnail Specific from TneVideoClipInfo
      
      TInt CTneTestClass::GetThumb( CStifItemParser& aItem) ;
	   
	    // from TNEVideoClipInfoObserver
	    
	     void NotifyVideoClipInfoReady( CTNEVideoClipInfo& aInfo, TInt aError );
	    
	    void NotifyVideoClipThumbCompleted( CTNEVideoClipInfo& aInfo, TInt aError, CFbsBitmap* aThumb );
/*
		TInt StartMonitor( CStifItemParser& aItem );
		TInt StopMonitor( CStifItemParser& aItem );
        TInt IsRadioOn( CStifItemParser& aItem );
		TInt CreateFactory( CStifItemParser& aItem );
		TInt CreateUtility( CStifItemParser& aItem );
		
	*/


    

    private:    // Data
        
    // reference to TestModuleIf
    CTestModuleIf& iTestModuleIf;
    //CStifLogger* iLog;
    
    // Active object with a timer to timeout the test case   later
  //  CSimpleTimeout * iTimeoutController;

    // Indicates if the test case use a normal exit reason
    TBool iNormalExitReason;

   //Thumbnail Engine
        CTNEVideoClipInfo* iTNEVideoClipInfo;
    /* I think we can depend on user input for this 
    
        TInt iThumbnailFrameIndex;
        TSize iThumbnailSize;
        TDisplayMode iDisplayMode;
        TBool iEnhance;
    */
       TFileName iFileName;
      
       RWsSession 			       iWS;
       RFs                    iFs;       
        

  
    };

#endif

// End of File
