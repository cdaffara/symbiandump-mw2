/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef CPOSTP31_H
#define CPOSTP31_H

//  INCLUDES

#include <e32base.h>
#include <e32std.h> 


#include "FT_CLandmarkTestProcedureBase.h"



// CLASS DECLARATION


/**
*  Test procedure 31
*/
class CPosTp31 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp31() : CLandmarkTestProcedureBase(31) {};
        
        /**
        * Destructor.
        */
        ~CPosTp31() {};

    public: // Functions from base classes

        /**
        * From CUtfwTestBase
        * Retrieves the test module name
        * @param aName The name of the test module.
        */
        void GetName(
            TDes&   aName
            ) const;

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        /**
        * From CLandmarkTestProcedureBase
        * Always called, even if the test leaves
        */
        void CloseTest();

        
    public:

        static void RunReceiveEventsTestL(TAny* aData);
        
        static void CreateEventsTestL(TAny* aData);

        void LogError(const TDesC& aError);

        void CheckEvent(const TInt aThreadIndex,
                        const TPosLmEventType  aExpectedEventType,
	                    const TPosLmEventType  aEventType,
	                    const TPosLmItemId aExpectedItemId,
	                    const TPosLmItemId aItemId);
        

    private: // Functions

        void CreateThreadsL();
        
    private: // data   
        
        RArray<RThread>	          iThreads;
        TInt                      iErrors;
        TBuf<4095>                iErrorLog;
        TInt                      iThreadIndex;
    };

class CEventObserver : public CActive
    {
     public: // Constructors and destructor
        
        static CEventObserver* NewL(CPosTp31* aTp31, TInt aThreadIndex);
       
        /**
        * Destructor.
        */
        ~CEventObserver();

    public: // Functions from base classes

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

         void StartTimer();

    private:
        
         /**
        * C++ constructor.
        */
        CEventObserver(CPosTp31* aTp31, TInt aThreadIndex); 

        void ConstructL();

       

        static TInt TimeoutCheck(TAny* aSelf);
        
    private: // Data
        
        CPosTp31*               iTp31;
        TInt                    iEventNumber;
        TInt                    iThreadIndex;
        TPosLmEvent             iEvent;
        CPosLandmarkDatabase*   iDatabase;
        CPeriodic*              iPeriodicTimer;
        TCallBack               iCallback;
       

        
    };

#endif      // CPOSTP31_H
            
// End of File
