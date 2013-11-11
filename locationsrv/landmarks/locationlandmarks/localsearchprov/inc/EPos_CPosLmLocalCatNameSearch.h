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
* Description: Class for handling searching for landmark categories with a certain name.
*
*/


#ifndef CPOSLMLOCALCATNAMESEARCH_H
#define CPOSLMLOCALCATNAMESEARCH_H

//  INCLUDES
#include <e32base.h>
#include "EPos_CPosLmLocalSearchHandler.h"

// CONSTANTS

// FORWARD DECLARATIONS
class CPosLmCatNameCriteria;

// CLASS DECLARATION

/**
*  Class for handling searching for landmark categories with a certain name.
*/
class CPosLmLocalCatNameSearch : public CPosLmLocalSearchHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param[in] aDbAccess The access handle to the database to search in.
        * @param[in] aCriteria The search criteria.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aDisplayData The displayable data.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        static CPosLmLocalCatNameSearch* NewL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches );

        /**
        * Destructor.
        */
        ~CPosLmLocalCatNameSearch();

    public: // New functions

        /**
        * Validates that the criteria is correct according to the search API.
        * Leaves with KErrArgument if the criteria is not correctly set.
        *
        * @param aCriteria The criteria to validate.
        */
        static void ValidateCriteriaL(
        /* IN  */       const CPosLmCatNameCriteria& aCriteria
        );

    public: // From base classes

        /**
        * From CPosLmLocalSearchHandler.
        *
        * @return ETrue.
        */
        TBool IsValidForCategorySearch() const;

        /**
        * From CPosLmLocalSearchHandler.
        *
        * @param aProgress Will be set to the progress of the search when
        * the step has finished.
        * @return @p KPosLmOperationNotComplete if the step has completed but
        *   more steps are needed before the search has finished,
        *   @p KErrNone if the search has finished successfully.
        */
        TInt NextStepL(
        /* OUT */   TReal32& aProgress
        );

    protected:

    private:

        void PrepareSearchL();
        void PrepareQueryL();
        void SearchL();

        /**
        * C++ default constructor.
        * @param[in] aDbAccess The access handle to the database to search in.
        * @param aCriteria The search criteria.
        * @param aSortPref A sort preference object.
        * @param aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        CPosLmLocalCatNameSearch(
            CPosLmLocalDatabase& aDatabase,
        /* IN  */       const CPosLmSearchCriteria& aCriteria,
        /* IN  */       const TPosLmLocalSortPref& aSortPref,
        /* IN  */       TInt aMaxNumOfMatches
        );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aCriteria The search criteria.
        * @param aDisplayData The displayable data.
        */
        void ConstructL(
        /* IN  */       const CPosLmSearchCriteria& aCriteria,
        /* IN  */       CPosLmDisplayData* aDisplayData
        );

        // By default, prohibit copy constructor
        CPosLmLocalCatNameSearch( const CPosLmLocalCatNameSearch& );
        // Prohibit assigment operator
        CPosLmLocalCatNameSearch& operator= ( const CPosLmLocalCatNameSearch& );

    private:

        enum TPosCategoryNameSearchStatus
            {
            EPosSearchNotStarted = 0,
            EPosCatNameSearch,
            EPosSearchCompleted
            };

        TPosCategoryNameSearchStatus iSearchStatus;

        CPosLmCatNameCriteria* iCriteria;

    };

#endif      // CPOSLMLOCALCATNAMESEARCH_H

// End of File
