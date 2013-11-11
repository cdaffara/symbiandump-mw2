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


#ifndef CPOSTP133_H
#define CPOSTP133_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION
class CPosLmMultiDbSearch;
/**
*  Test procedure 133
*/
class CPosTp133 : public CLandmarkTestProcedureBase, public MProgressObserver
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
         CPosTp133(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iLastNrOfDisplayData=0;iAscendedSorted = ETrue;iErrorsFound=KErrNone;iPartialFlag=EFalse;};
        /**
        * Destructor.
        */
        ~CPosTp133() {};

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
            
       void DoSearchL(
            const RPointerArray<CSearchResult>& aSearchResults, CDesCArray* aDbUris, 
            					TExecutionMode aExecutionMode);

        void SearchPartialLandmarksL(TInt aWhatIndexToSerrachFor, 
        							const RPointerArray<CSearchResult>& aSearchResults,
        							CDesCArray* aDbUris);
        
        void CheckPartialErrorCodeL(const TBool aPartial, const TInt aErr);
        
        void CheckPartialLandmarkL(const CPosLandmark& aLandmark, 
                                     const CPosLandmark::TAttributes& aAttr,
                                     const RArray<TUint>& aReqFields);
                                     
        void SearchPartialWithoutNameL(const RPointerArray<CSearchResult>& aSearchResults,
        							CDesCArray* aDbUris);
        
        void CheckResetL(const RPointerArray<CSearchResult>& aSearchResults, CDesCArray* aDbUris);
        
        void CancelTestL(const RPointerArray<CSearchResult>& aSearchResults, 
        				CDesCArray* aDbUris, TBool aCancelInCallback);
        
        void LandmarksSortL(RArray<TPosLmItemId>& aArray, const TPosLmSortPref& aSortPref);
        
        /**
        * Execute search operation, add testclass as observer
        */
        void ExecuteLmOpL();
        
        /**
        * Execute search operation but cancel in callback, add testclass as observer
        */
        void ExecuteLmOpInCallbackL();
        
        private:
        
        	CPosLmDisplayData* iDisplayData;
        	TBool iAscendedSorted;
        	TInt iErrorsFound;
        	TBool iPartialFlag;
        	CPosLmMultiDbSearch* iMultiLandmarkSearch;
        	TInt iLastNrOfDisplayData;
        	RArray<TPosLmItemId>* iJustNowSearchResults;
        	
    };

#endif      // CPOSTP133_H
            
// End of File
