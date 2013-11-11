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


#ifndef CPosTp28_H
#define CPosTp28_H

//  INCLUDES

#include "FT_CLandmarkTestProcedureBase.h"

class CActiveLmEvent;
// CLASS DECLARATION

/**
*  Test procedure 28
*/
class CPosTp28 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp28(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp28() {};

    public: // Functions from base classes

       
       
        void CloseTest();

        /**
        * From CPosTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:    // New functions
   
        /**
        * Test logging of landmark events
        */
        void LandmarkEventTest();

        void LandmarkEventQueueTest();
		

    private:    // Data
        
        CActiveLmEvent*         iActiveLmEvent;
        CPosLmCategoryManager*  iCategoryManager;
    };

class CActiveLmEvent : public CActive
    {
    public:
        
        static CActiveLmEvent* NewL(CStifLogger* aLog);
      
        /**
        * Destructor.
        */
        ~CActiveLmEvent();

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
        CActiveLmEvent(CStifLogger* aLog);

        void ConstructL();

        void VerifyEvent();

        void VerifyMergedEvent();

        void AssertCorrectEvent(TPosLmEventType  aExpectedEventType,
	                            TPosLmEventType  aEventType,
    	                        TPosLmItemId aExpectedLandMarkItemId,
	                            TPosLmItemId aLandMarkItemId);

    private: // data

        friend class CPosTp28;

        CStifLogger*               iLog; 
        CPosLandmarkDatabase*   iDatabase;
       
       

        TPosLmEvent             iEvent;
        TInt                    iEventNumber;
        TInt                    iMergedEventNumber;
        TInt                    iGlobalErr;

        TBool                   iQueueTest;

        TPosLmItemId            iLandmarkId;
        TPosLmItemId            iLandmarkId2;
        TPosLmItemId            iLandmarkId3;
        TPosLmItemId            iLandmarkId4;

        TPosLmItemId            iCategoryId;
        TPosLmItemId            iCategoryId2;
        TPosLmItemId            iNonExistingCategoryId;
        TPosLmItemId            iDeleteCategoryId;
    };

#endif      // CPosTp28_H
            
// End of File
