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


#ifndef CPOSTP130_H
#define CPOSTP130_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include "FT_CPosTp119.h"

// FORWARD DECLARATIONS
class CPosLmMultiDbSearch;

// DATA TYPES
class CMatchIteratorTester : public CActive
    {
    public:  // Constructors and destructor
        
        static CMatchIteratorTester* NewLC(
        		CDesCArray* aDbUris, CStifLogger* iLog);        
        ~CMatchIteratorTester();

    public: // Functions from base classes

        void RunL();
        void DoCancel();
        TInt RunError(TInt aError);

    public: // New Functions
    
        void RunTestL(
            CPosLmMultiDbSearch* aMultiSearcher,
            CPosLmOperation* aSearchOperation,
            RPointerArray<RIdArray>& aExpectedResult);        
        TInt Result() const;     
        void GetErrorMsg(TPtrC& aMsg) const;

    private: 
    
        // Constructor
        CMatchIteratorTester( CDesCArray* aDbUris, CStifLogger* aLog );
        
        void VerifyIteratorsL(TInt aResult);        
        void CompareIteratorsL(CPosLmItemIterator& aNewIter, CPosLmItemIterator& aOldIter);
        void SetErrorAndLeaveL(const TDesC& aErrMsg);

    private: // Data

        HBufC*              iErrorMsg;
        TInt                iResult;
        TReal32             iProgress;
        CDesCArray*         iDbUris;
        CPosLmOperation*    iOperation;
        RPointerArray<CPosLmItemIterator> iIterators;
        CPosLmMultiDbSearch* iMultiSearcher;
        RPointerArray<RIdArray>*    iExpectedResult;
        CStifLogger* iLog;
    };    

// CLASS DECLARATION

/**
*  Test procedure 130
*/
class CPosTp130 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        //CPosTp130() : CLandmarkTestProcedureBase(130) {};
        CPosTp130(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp130() {};

    public: // Functions from base classes

       
        /**
        * From CLandmarkTestProcedureBase
        * Initializes the test in the module
        */
        virtual void InitTestL();

        /**
        * From CLandmarkTestProcedureBase
        * Closes the test in the module
        */
        virtual void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
    
        void SearchForLandmarksL();
        void SearchForCategoriesL();        
        void VerifyIteratorsL(RArray<TInt>& aNrOfExpectedItems, TBool aLm);
        void TestErrorCodesL();
        void TestMatchIteratorL(TBool aSearchLm);
        
        void InitExpectedResultsL( const TDesC& aDbName, RIdArray& aArray, TBool aSearchLm );

    private: // Data
    
        CDesCArray*             iDbUris;
        TInt                    iInitialDbCount;
        CPosLmMultiDbSearch*    iDbSearcher;
        RPointerArray<RIdArray>        iExpectedLmResult; // contains expected matches when search patern is *e*
        RPointerArray<RIdArray>        iExpectedCatResult; // contains expected matches when search patern is *e*

    };
    
#endif      // CPOSTP130_H
            
// End of File
