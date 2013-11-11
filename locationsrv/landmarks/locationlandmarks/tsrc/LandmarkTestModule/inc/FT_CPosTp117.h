/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*   ?description_line
*
*/


#ifndef CPOSTP117_H
#define CPOSTP117_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <EPos_CPosLmDatabaseManager.h>

#include <StifLogger.h>
// CLASS DECLARATION
class CTP117ActiveLmEvent;

/**
*  Test procedure 117
*/
class CPosTp117 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp117(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound=KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp117() {};

    public: // Functions from base classes

        
        /**
        * 
        * Always called, even if test leaves
        */
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();
        
        // Use this enum to define if the thread should exit with panic or leave
        enum ExitType {ExitPanic, ExitLeave};
        
        static void RunPanicTestL(TAny* aData);
		
    private:
        void LandmarkEventTestL();
        
        void LandmarkQueuedEvenTestL();
        
        void LandmarkQueuedEvenTest2L();
        
        void CreateThreadL();
                
        void ResumeThreadAndVerifyExit();
        
    private:

        CTP117ActiveLmEvent*         iActiveLmEvent;
        TInt iErrorsFound;
        
        RThread     iThread;
        TInt        iExpectedErrorCode;
        ExitType    iExpectedExitType;
        TInt        iTestCase;

    };

class CTP117ActiveLmEvent : public CActive
    {
    public:
        
        static CTP117ActiveLmEvent* NewL(CStifLogger* aLog);
      
        /**
        * Destructor.
        */
        ~CTP117ActiveLmEvent();

    public: 
        
        void Start();    

        /**
        * From CActive
        * Handles request completion event.
        */
        void DoCancel();
    
        /**
        * From CActive.
        *
        * Handles request completion event.
        */
        void RunL();
        
        /**
        * From CActive.
        *
        * Handles request completion event.
        */
        TInt RunError(TInt aError);
        
        /**
		*
		*/
		void CallNotifyDatabaseEvent();
			
		void VerifyEventL();
		
		TInt GetErrors();
		
		void CallDatabaseUriFromLastEventLC();
    
    private:

         /**
        * C++ constructor.
        */
        CTP117ActiveLmEvent(CStifLogger* aLog);

        void ConstructL();

        void SetExpectedDatabase(const TDesC&  aDatabaseUri);


    private: // data

        friend class CPosTp117;

        CStifLogger*               iLog; 
        
        CPosLmDatabaseManager* iManager;
              

        
        TPosLmDatabaseEvent                iEvent;
       
        TInt                    iExpectedEvent;
        
        TBuf<255>           iExpectedDatabase;
        
       
        TInt                    iGlobalErr;

        TBool                   iQueueTest;
        TBool                   iCheckUri;
    };
#endif      // CPOSTP117_H
            
// End of File
