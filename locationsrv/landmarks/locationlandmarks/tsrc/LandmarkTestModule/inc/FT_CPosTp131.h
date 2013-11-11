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


#ifndef CPOSTP131_H
#define CPOSTP131_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 131
*/
class CPosTp131 : public CLandmarkTestProcedureBase, public MProgressObserver
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp131(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iAscendedSorted = ETrue;iErrorsFound=KErrNone;iPartialFlag=EFalse;};
        /**
        * Destructor.
        */
        ~CPosTp131() {};

    public: // Functions from base classes

        
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();
        
        void CloseTest();
        
        /**
        * From MProgressObserver
        * Notifies the progress of the execution of a CPosLmOperation object.
        *
        * @param sProgress the progress of the operation
        */
        void NotifyProgress(TReal aProgress);

    private:

        void SearchL(
            const RPointerArray<CSearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode);

        void SearchWithSortOrderL(const RPointerArray<CSearchResult>& aSearchResults);
        
        void SearchPartialLandmarksL(const RPointerArray<CSearchResult>& aSearchResults);
        
        void CheckPartialErrorCodeL(const TBool aPartial, const TInt aErr);
        
        void CheckPartialLandmarkL(const CPosLandmark& aLandmark, 
                                     const CPosLandmark::TAttributes& aAttr,
                                     const RArray<TUint>& aReqFields);
        
        void CheckResetL(const RPointerArray<CSearchResult>& aSearchResults);
        
        void CancelTestL(const RPointerArray<CSearchResult>& aSearchResults);
        
        void LandmarksSortL(RArray<TPosLmItemId>& aArray, const TPosLmSortPref& aSortPref);
        
        void ExecuteLmOpL();
        
        private:
        
        	CPosLmDisplayData* iDisplayData;
        	TBool iAscendedSorted;
        	TInt iErrorsFound;
        	TBool iPartialFlag;
    };

#endif      // CPOSTP131_H
            
// End of File
