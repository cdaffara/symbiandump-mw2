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


#ifndef CPOSTP39_H
#define CPOSTP39_H

//  INCLUDES

#include <e32base.h>
#include <e32std.h> 


#include "FT_CLandmarkTestProcedureBase.h"


// CLASS DECLARATION


/**
*  Test procedure 39
*/
class CPosTp39 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp39() : CLandmarkTestProcedureBase(39) {};
        
        /**
        * Destructor.
        */
        ~CPosTp39() {};

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

        static void RunEventsListenerL(TAny* aData);
        
        static void RunEventsGeneratorL(TAny* aData);

        void LogError(const TDesC& aError);

        void CheckEvent(const TInt aThreadIndex,
                        const TPosLmEventType  aExpectedEventType,
	                    const TPosLmEventType  aEventType,
	                    const TPosLmItemId aExpectedItemId,
	                    const TPosLmItemId aItemId);
        

    private: // Functions
    
        void SetupGlobalCategoriesL();

        void CreateThreadsL();
        
        TInt GetHeapSizeL(const TDesC& aThreadName, TInt& aHeapSize, TInt& aStackSize);
        
    private: // data   
        
        RArray<RThread>	          iThreads;
        TInt                      iErrors;
        TBuf<4095>                iErrorLog;
        TInt                      iThreadIndex;
    };

class CEventObserverTp39 : public CActive
    {
     public: // Constructors and destructor
        
        static CEventObserverTp39* NewL(CPosTp39* aTp39, TInt aThreadIndex);
       
        /**
        * Destructor.
        */
        ~CEventObserverTp39();

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
        CEventObserverTp39(CPosTp39* aTp39, TInt aThreadIndex); 

        void ConstructL();

        static TInt StopEventNotification(TAny* aSelf);
        
    private: // Data
        
        CPosTp39*               iTp39;
        TInt                    iEventNumber;
        TInt                    iThreadIndex;
        TPosLmEvent             iEvent;
        CPosLandmarkDatabase*   iDatabase;
        CPeriodic*              iPeriodicTimer;
        TCallBack               iCallback;
       

        
    };

#endif      // CPOSTP39_H
            
// End of File
