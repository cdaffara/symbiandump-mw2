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


#ifndef CCOMPOSITESEARCHRESULT_H
#define CCOMPOSITESEARCHRESULT_H

//  INCLUDES
#include <e32base.h>
#include <f32file.h>
#include "Ft_CSearchResultBase.h"
#include <StifLogger.h>


class CPosLmCompositeCriteria;
// CONSTANTS


class CCompositeSearchResult : public CSearchResultBase
    {

    public:  // Constructors and destructor
        
         /**
        * Two-phased constructor.
        */
        static CCompositeSearchResult* NewL(CStifLogger* aLog); 

        /**
        * Destructor.
        */
        ~CCompositeSearchResult();
        
    public: 

        TBool GetCompositeSearchResultsL(CPosLmCompositeCriteria& aComposite);

        //RArray<TPosLmItemId>& GetSearchResult();

    public: // data
        RArray<TPosLmItemId>        iResultIds;

        TReal64     iSouthLat;
        TReal64     iNorthLat;
        TReal64     iWestLong;
        TReal64     iEastLong;

    private: 
        
        CCompositeSearchResult();

        void ConstructL(CStifLogger* aLog);

        TInt ReadLn(RFile& aFile, TDes& aDes);

        void ParseAreaL(const TDesC& aDes);

        void ParseTextL(const TDesC& aDes);

        void ParseCategoryL(const TDesC& aDes);

        void ParseNearestL(const TDesC& aDes);

        void ParseIdListL(const TDesC& aDes);

        void ParseResultL(const TDesC& aDes);
        
        void ExtractIdFields(const TDesC& aDes, RArray<TUint>& aList);

    private:

         // By default, prohibit copy constructor
        CCompositeSearchResult(const CCompositeSearchResult&);
        // Prohibit assigment operator
        CCompositeSearchResult& operator=(const CCompositeSearchResult&);

    private: // Data
        
        enum TAreaValueIndex
			{
            ESouthLat           =1,	
            ENorthLat           =2,	
            EWestLong           =3,
            EEastLong           =4,
			};

        enum TCategoryValueIndex
			{
            ECategoryName       =1,	
            ECategoryId         =2,	
			};

        enum TTextValueIndex
			{
            ETextCriteria       =1,	
            EAttributes         =2,	
            EPositionFields     =3,
			};

        enum TNearestValueIndex
			{
            ELatitude           =1,	
            ELongitude          =2,	
            EUseCoverageRadius  =3,
            EMaxDistance        =4,
			};

        enum TIdListValueIndex
			{
            ELmIdList           =1,
			};

        enum TResultValueIndex
			{
            ELmResultIdList     =1,
            ERedefined          =2
			};        


        // Test log, used for debug text
		CStifLogger* iLog;

        RFs   iFileSession;
        RFile iFile;

        HBufC*                  iTextCriteria;
        HBufC*                  iCategoryName;
        TInt                    iCategoryId;

        CPosLandmark::TAttributes iTextAttributes;

        RArray<TUint>               iIdList;
        RArray<TUint>               iPositionList;

        TReal64     iLatitude;
        TReal64     iLongitude;
        TBool       iUseCoverageRadius;
        TReal32     iMaxDistance;

        TBool iTextEnabled;
        TBool iAreaEnabled;
        TBool iNearestEnabled;
        TBool iCategoryEnabled;
        TBool iIdListEnabled;

        TInt        iTestCounter;

    };

#endif      // CCompositeSearchResult_H
            
// End of File

