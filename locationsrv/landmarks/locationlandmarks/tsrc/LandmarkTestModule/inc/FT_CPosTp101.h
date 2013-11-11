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


#ifndef CPOSTP101_H
#define CPOSTP101_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"



/**
*  Test procedure 101
*/
class CPosTp101 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp101(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp101() {};

    public: // Functions from base classes

       
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();
        
        void CloseTest();

    private:
    
        void VerifySetOfDatabasesL();

        void VerifyDatabaseL(CDesCArray* aLmUris, CDesCArray* aNewLmUris = NULL);

        void CheckDatabasesL(
            CDesCArray* aExpectedLmUris,
            TUint aExpectedMaxMatches = KPosLmMaxNumOfMatchesUnlimited
        );


        void SearchL(
            const RPointerArray<CSearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode);

        void SearchWithSortOrderL(const RPointerArray<CSearchResult>& aSearchResults);

        void GetLmIdsL(
            TInt aDbId, 
            RArray<TPosLmItemId> aSearchResults,
            RArray<TPosLmItemId>& aLmIdForADb
        );
        
        void CheckEqualsLmsL(
            TPosLmItemId aSource, 
            TPosLmItemId aTarget
        );
        
        void OnTheFlySearchL();
        
        void SearchAndDeleteL(
            const RPointerArray<CSearchResult>& aSearchResults
        );

        void SearchAndRenameL(
            const RPointerArray<CSearchResult>& aSearchResults
        );

        void SearchAndCancelL(const TBool& aInCallback = EFalse);
        
        void SearchWithMaximumL(
            const RPointerArray<CSearchResult>& aSearchResults
        );


        CDesCArray* CreateDatabasesL(
            TInt aNrOfLmDbs, 
            TInt aLmPerDb, 
            TInt aLastId, 
            TBool aDebug = EFalse
        );
        
    private:

        CDesCArray* iDbArray;

    };

#endif      // CPOSTP56_H
            
// End of File
