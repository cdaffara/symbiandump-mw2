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


#ifndef CPOSTP35_H
#define CPOSTP35_H

//  INCLUDES
#include <EPos_CPosLmNearestCriteria.h>
#include "FT_CLandmarkTestProcedureBase.h"

class CNearestSearchResultTp35;


// CLASS DECLARATION

/**
*  Test procedure 35
*/
class CPosTp35 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp35(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp35() {};

    public: // Functions from base classes

       
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        void CloseTest();

        
        void CheckSearchResultL(CNearestSearchResultTp35& aSearchResult);

    private:
 
        void SearchL(const TBool& aSync=EFalse);

        void SearchAndCancelL(const TBool& aInCallback=EFalse);
    
        void AppendNearestSearchResultsL();
        
        CPosLandmarkDatabase* UseNearestSearchGeneratedDbFileL();
        
        void SearchWithSortOrderL();

    private:
        
        
        RPointerArray<CNearestSearchResultTp35>    iNearestSearchResults;
       
    };


class TSearchResultTp35 
    {
    public:
        
        TSearchResultTp35();		
        
        TSearchResultTp35(const TPosLmItemId& aId, const TReal32& aDistance);

        TReal64 Distance() const;

        TPosLmItemId Id() const;

    private:
        friend class CPosTp35;

        
        TReal32         iDistance;
        TPosLmItemId    iId;   

    };

class CNearestSearchResultTp35 : public CSearchResultBase
    {

    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CNearestSearchResultTp35* NewL(
            const TDesC& aDes, 
            const TInt& aIndex,
            RFs& aFileSession); 

        /**
        * Destructor.
        */
         CNearestSearchResultTp35::~CNearestSearchResultTp35();
        
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
         CNearestSearchResultTp35(RFs& aFileSession);

        void ConstructL(const TDesC& aDes, const TInt& aIndex);
        
        void AppendDistancesL(const TInt& aIndex);

        void ParseDistanceL(const TDesC& aDes, const TInt& aIndex, const TPosLmItemId& aId);

        void SortDistances();

    private:

         // By default, prohibit copy constructor
         CNearestSearchResultTp35(const  CNearestSearchResultTp35&);
        // Prohibit assigment operator
         CNearestSearchResultTp35& operator=(const  CNearestSearchResultTp35&);

    private: // Data
      
        friend class CPosTp35;       

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

        RArray<TSearchResultTp35>       iDistances;
        
        CPosLandmarkDatabase* iDatabase;
        RFs& iFileSession;
    };

#endif      // CPOSTP35_H
            
// End of File
