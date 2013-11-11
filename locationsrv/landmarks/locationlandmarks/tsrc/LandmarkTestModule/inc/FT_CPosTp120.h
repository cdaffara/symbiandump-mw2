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


#ifndef CPOSTP120_H
#define CPOSTP120_H

//  INCLUDES
#include <EPos_CPosLmAreaCriteria.h>
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 23
*/
class CPosTp120 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp120(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp120() {};

    public: // Functions from base classes

       
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        void CloseTest();
        
    private:

        CPosLmAreaCriteria* AreaCriteriaL(const TReal64 &aSouthLat, 
                                          const TReal64 &aNorthLat,
                                          const TReal64 &aWestLong, 
                                          const TReal64 &aEastLong);


        void CheckDatabasesL(
            CDesCArray* aExpectedLmUris,
            TUint aExpectedMaxMatches = KPosLmMaxNumOfMatchesUnlimited
        );

        void GetLmIdsL(
            TInt aDbId, 
            RArray<TPosLmItemId> aSearchResults,
            RArray<TPosLmItemId>& aLmIdForADb
        );
        
        void CheckEqualsLmsL(
            TPosLmItemId aSource, 
            TPosLmItemId aTarget
        );
        
        void SearchL(
            const RPointerArray<CAreaSearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode);

        void SearchWithSortOrderL(const RPointerArray<CAreaSearchResult>& aSearchResults);
        
        void OnTheFlySearchL();

        void SearchAndDeleteL();

        void SearchAndRenameL();

        void SearchAndCancelL(const TBool& aInCallback=EFalse);

        void SearchWithMaximumL();

        CDesCArray* CreateDatabasesL(
            TInt aNrOfLmDbs, 
            TInt aLmPerDb, 
            TInt aLastId, 
            TBool aDebug = EFalse
        );

    private:
    
        CDesCArray* iDbArray;

    };

#endif      // CPOSTP120_H
            
// End of File
