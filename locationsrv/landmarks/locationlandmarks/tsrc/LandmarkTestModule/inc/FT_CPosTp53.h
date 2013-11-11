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


#ifndef CPosTp53_H
#define CPosTp53_H

//  INCLUDES

#include "FT_CLandmarkTestProcedureBase.h"

class CActiveLmImportEvent;

// CLASS DECLARATION

/**
*  Test procedure 53
*/
class CPosTp53 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp53() : CLandmarkTestProcedureBase(53) {};
        
        /**
        * Destructor.
        */
        ~CPosTp53() {};

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
        * From CUtfwTestBase
        */
        void CloseTest();

        /**
        * From CPosTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:    // New functions
   
        /**
        * Tests logging of landmark events
        */
        void LandmarkImportEventTestL();

        /**
        * Test Compact during import
        */
        void LandmarkImportCompactTestL();
       
        void ImportLandmarksL(const TDesC& aFileName, CPosLandmarkDatabase::TTransferOptions aTransferOptions);

        void AddCategoriesL();

    private:    // Data
        
        CActiveLmImportEvent*           iActiveLmEvent;
        CPosLandmarkParser*             iParser;
    };

class CActiveLmImportEvent : public CActive
    {
    public:
        
        static CActiveLmImportEvent* NewL(MUtfwLog* aLog);
      
        /**
        * Destructor.
        */
        ~CActiveLmImportEvent();

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
        CActiveLmImportEvent(MUtfwLog* aLog);

        void ConstructL();

        void VerifyEvent();

        void AssertCorrectEvent(TPosLmEventType  aExpectedEventType,
	                            TPosLmEventType  aEventType,
    	                        TPosLmItemId aExpectedLandMarkItemId,
	                            TPosLmItemId aLandMarkItemId);

    private: // data

        friend class CPosTp53;

        MUtfwLog*               iLog; 
        CPosLandmarkDatabase*   iDatabase;
       
        TPosLmEvent             iEvent;
        TInt                    iEventNumber;
       
        TInt                    iGlobalErr;

    };

#endif      // CPosTp53_H
            
// End of File
