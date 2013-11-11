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


#ifndef CPOSTP119_H
#define CPOSTP119_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <StifLogger.h>

// FORWARD DECLARATIONS
class CPosLmMultiDbSearch;

// DATA TYPES
enum TTp119SortOrder
    {
    ENoSort = 0,
    EAsc,
    EDesc
    };

// CLASS DECLARATION

/**
*  Test procedure 119
*/
class CPosTp119 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp119(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp119() {};

    public: // Functions from base classes

        
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
    
        void SearchAllDbsL(CDesCArray* aDbUris, TTp119SortOrder aSortOrder);
        void SearchAllButOneL(CDesCArray* aDbUris);
        void SearchOneDbL(CDesCArray* aDbUris);
        void SearchNonExistingDbL(CDesCArray* aDbUris);
        
        void SearchInvalidURIDbL(CDesCArray* aDbUris);
        
        void SearchDbMultipleTimesL(CDesCArray* aDbUris);
        void TestMaxNrOfMatchesL(CDesCArray* aDbUris);
        void TestMatchIteratorL(CDesCArray* aDbUris);

        void SearchForLandmarksL(
            CPosLmMultiDbSearch* aSearcher, 
            TTp119SortOrder aSortOrder,
            TBool aRefined);
        
        TUint VerifyResultL(
            CPosLmMultiDbSearch* aSearcher, 
            CDesCArray& aDbUris,
            TTp119SortOrder aSortOrder);

        void RemoveDatabaseL(const TDesC& aFile);
        
        void VerifyDbsToSearchL(
            CPosLmMultiDbSearch* aMultiSearcher, 
            CDesCArray* aDbUris);

    private: // Data

    };
    
#endif      // CPOSTP119_H
            
// End of File
