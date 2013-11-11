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


#ifndef CPOSTP116_H
#define CPOSTP116_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <EPos_CPosLmDatabaseManager.h>
#include <StifLogger.h>

// CLASS DECLARATION
class CTP116ActiveLmEvent;

/**
*  Test procedure 116
*/


class CPosTp116 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp116(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp116() {};

    public: // Functions from base classes

        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
        void LandmarkEventTestL();
        
    private:

        CTP116ActiveLmEvent*         iActiveLmEvent;

    };

class CTP116ActiveLmEvent : public CActive
    {
    public:
        
        static CTP116ActiveLmEvent* NewL(CStifLogger* aLog);
      
        /**
        * Destructor.
        */
        ~CTP116ActiveLmEvent();

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
    
    private:

         /**
        * C++ constructor.
        */
        CTP116ActiveLmEvent(CStifLogger* aLog);

        void ConstructL();

        void VerifyEvent(TPosLmDatabaseEvent event);
        
        TInt GetErrors();

        void SetExpectedDatabase(const TDesC&  aDatabaseUri);

    private: // data

        friend class CPosTp116;

        CStifLogger*               iLog; 
        CPosLmDatabaseManager*  iManager;
        TPosLmDatabaseEvent     iEvent;

        TInt                    iExpectedEvent;
        TBuf<255>           	iExpectedDatabase;
        
        
        TInt                    iGlobalErr;

        TBool                   iQueueTest;

    };

#endif      // CPOSTP116_H
            
// End of File
