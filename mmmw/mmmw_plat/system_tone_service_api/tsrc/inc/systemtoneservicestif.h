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
* Description: STIF testclass declaration
*
*/

#ifndef SYSTEMTONESERVICESTIF_H
#define SYSTEMTONESERVICESTIF_H

//  INCLUDES
#include <e32base.h>
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <systemtoneservice.h>
#include "systemtoneservicesactiveobj.h"


// CONSTANTS


// MACROS

#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KsystemtoneservicestifLogPath, "f:\\testing\\Log\\" ); 
// Log file
_LIT( KsystemtoneservicestifLogFile, "systemtoneservicestif.txt" ); 
_LIT( KsystemtoneservicestifLogFileWithTitle, "systemtoneservicestif_[%S].txt" );




// FORWARD DECLARATIONS

class CSystemToneServiceStif;


enum TPlayState
    {
    EStopped,
    EPlaying
    };

_LIT( KTrue, "True");
_LIT( KFalse, "False");

// CLASS DECLARATION

/**
*  CSystemToneServiceStif test class for STIF Test Framework TestScripter.

*/
NONSHARABLE_CLASS(CSystemToneServiceStif) : public CScriptBase, MStsPlayAlarmObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSystemToneServiceStif* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CSystemToneServiceStif();


    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );



    private:

        /**
        * C++ default constructor.
        */
        CSystemToneServiceStif( CTestModuleIf& aTestModuleIf );

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

        /**
        * Example test method.
        * @return Symbian OS error code.
        */
        virtual TInt ExampleL( CStifItemParser& aItem );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD 
        
        virtual TInt  CreateSystemToneService(  );
      
        virtual TInt  DeleteSystemToneService(  );
        
        virtual TInt  PlayTone( CStifItemParser& aItem);
        
        virtual TInt  PlayAlarm( CStifItemParser& aItem );
        
        virtual TInt  StopAlarm( CStifItemParser& aItem );
        
        virtual TInt  PlayToneWithStop( CStifItemParser& aItem);



    private:  
    	 // inherited from MPlayAlarmObserver
       virtual void PlayAlarmComplete(unsigned int aAlarmContext);
       virtual void HandleEvent();
    	
    	
    	
    	  
    	  // Data
        TPlayState            iPlayState;
        CSystemToneService*   iSts;
          // Active object with a timer to timeout the test case
        CStsActiveObj*        iCallbackController;   
        TUint                 iCurrentContext;



    };

#endif      // SYSTEMTONESERVICESTIF_H

// End of File
