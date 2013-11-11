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


#ifndef CPOSTP122_H
#define CPOSTP122_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <EPos_CPosLmNearestCriteria.h>

// FORWARD DECLARATIONS
class CPosLmMultiDbSearch;
class CNearestSearchResult;
// CLASS DECLARATION

/**
*  Test procedure 122
*/
class CPosTp122 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
      
        CPosTp122(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound = KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp122() {};

    public: // Functions from base classes

        
        
        void CloseTest();

       
        void InitTestL();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
    
    	enum TTp122SortOrder
            {
            ESortNone,
            ESortAscending,
            ESortDescending
            };

        void AppendNearestSearchResultsL();
        
        void TestLeaveCodesL();
        
        void CancelTestL(const RPointerArray<CNearestSearchResult>& aSearchResults,
        					TBool aCancelInCallback);
        
        void StartSearchOperationL(
            const RPointerArray<CNearestSearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode = ESynchronous,
            TTp122SortOrder aSortOrder = ESortNone,
            TInt aNrOfSearches = 0);
        
        void SearchL(
            CPosLmNearestCriteria* aNearestSearchCriteria,
            TExecutionMode aExecutionMode,
            TBool aRefinedSearch = EFalse,
            TTp122SortOrder aSortOrder = ESortNone);
        
        void TestMaximumNrOfMatchesL(
            const RPointerArray<CNearestSearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode, TTp122SortOrder aSortOrder);
            
        void ValidateTotalNrOfMatchesL(
            TUint aRetrievedNrOfMatches,
            TUint aExpectedNrOfMatches);
            
        TInt NrOfSearchErrors(CPosLmMultiDbSearch* aMultiDbSearcher);
            
      	void TestOnTheFlySearchL(const RPointerArray<CNearestSearchResult>& aSearchResults);
            
        void ValidateIdL(TPosLmItemId aId, TInt aDbIndex);
        	    
        void PrepareDatabasesL();
            
        void CheckSearchResultL(CNearestSearchResult& aSearchResult, TTp122SortOrder aSortOrder);
        
        void PrintLm(CPosLandmarkDatabase* db);
        
        void DoLastTestPartL(const RPointerArray<CNearestSearchResult>& aSearchResults);
        
        void CheckDbL();
        
        void SetupLimits();
            
    private:

        CDesCArray*             iDatabases;
        CPosLmMultiDbSearch*    iMultiDbSearch;
        TInt                    iNrOfDatabases;
        
        RPointerArray<CNearestSearchResult>    iNearestSearchResults;
        TInt 					iErrorsFound;
        
        CPosLmDisplayData* iDisplayData;
        TBool iPartial;
        
        TUint iIdDb1Min;
        TUint iIdDb1Max;
        TUint iIdDb2Min;
        TUint iIdDb2Max;
        TUint iIdDb3Min;
        TUint iIdDb3Max;
        TUint iIdDb4Min;
        TUint iIdDb4Max;
        TUint iIdDb5Min;
        TUint iIdDb5Max;

    };

// Commented for testing -- Moved these class defines to FT_CPosTp35.h

class TSearchResult 
    {
    public:
        
        TSearchResult();

        TSearchResult(const TPosLmItemId& aId, const TReal32& aDistance);

        TReal64 Distance() const;

        TPosLmItemId Id() const;

    private:
        friend class CPosTp122;

        TReal32         iDistance;
        TPosLmItemId    iId;   

    };



class CNearestSearchResult : public CSearchResultBase
    {

    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CNearestSearchResult* NewL(
            const TDesC& aDes, 
            const TInt& aIndex,
            RFs& aFileSession); 

        /**
        * Destructor.
        */
        ~CNearestSearchResult();
        
    public: 

        TReal64 Latitude() const;

        TReal64 Longitude() const;
        
        TBool UseCoverageRadius() const;
        
        TReal32 MaxDistance() const;

        TInt ErrorCode() const;

        void Print(TDes& aDes);

        void PrintDistances(TDes& aPrint);

    private: 
        
        /**
        * Constructor.
        */
        CNearestSearchResult(RFs& aFileSession);

        void ConstructL(const TDesC& aDes, const TInt& aIndex);
        
        void AppendDistancesL(const TInt& aIndex);

        void ParseDistanceL(const TDesC& aDes, const TInt& aIndex, const TPosLmItemId& aId);

        void SortDistances();

    private:

         // By default, prohibit copy constructor
        CNearestSearchResult(const CNearestSearchResult&);
        // Prohibit assigment operator
        CNearestSearchResult& operator=(const CNearestSearchResult&);

    private: // Data
      
        friend class CPosTp122;

        enum TValueIndex
			{
            ELatitude           =1,	
            ELongitude          =2,	
            EUseCoverageRadius  =3,
            EMaxDistance        =4,
            EErrorCode          =5,
            ERedefined          =6,
            EMaxSearchResult    =7,
            ESearchResult       =8
			};
        
        TInt        iErrorCode;
        TReal64     iLatitude;
        TReal64     iLongitude;
        TBool       iUseCoverageRadius;
        TReal32     iMaxDistance;
        TInt        iMaxSearchResult;

        RArray<TSearchResult>       iDistances;
        
        CPosLandmarkDatabase* iDatabase;
        RFs& iFileSession;
    }; 
   

#endif      // CPOSTP122_H
            
// End of File
