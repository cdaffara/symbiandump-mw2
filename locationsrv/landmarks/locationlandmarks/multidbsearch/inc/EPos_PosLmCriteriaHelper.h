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
* Description: Helper class that validates a search criteria and creates a copy.
*
*/


#ifndef POSLMCRITERIAHELPER_H
#define POSLMCRITERIAHELPER_H

//  INCLUDES

// FORWARD DECLARATION
class CPosLmSearchCriteria;
class CPosLmAreaCriteria;
class CPosLmTextCriteria;
class CPosLmCompositeCriteria;
class CPosLmCategoryCriteria;
class CPosLmNearestCriteria;
class CPosLmCatNameCriteria;

// CLASS DECLARATION

/**
* Helper class that validates a search criteria and creates a copy.
*/
class PosLmCriteriaHelper
    {
    public: // New functions

        /*
        * Validates the specified criteria according to validation rules of the
        * criteria classes and the validation rules of CPosLmMultiDbSearch.
        *
        * @param aCriteria The search criteria.
        * @param aIsLm If ETrue, the criteria will be validated as a landmark
        *   criteria and if EFalse, the criteria will be validated as a
        *   category criteria.
        * @param aUseDistanceSorting Will be set to ETrue if the criteria
        *   should result in distance sorting, EFalse otherwise.
        * @param aIsTopCriteria For internal use, external calls should always
        *   use the default value.
        * @return A copy of the criteria. Caller takes ownership of the
        *   returned object.
        */
        static CPosLmSearchCriteria* ValidateAndCopyL(
        /* IN  */       const CPosLmSearchCriteria& aCriteria,
        /* IN  */       TBool aIsLm,
        /* OUT */       TBool& aUseDistanceSorting,
        /* IN  */       TBool aIsTopCriteria = ETrue
        );

    private:

        static CPosLmSearchCriteria* ValidateAndCopyLC(
        /* IN  */       const CPosLmCompositeCriteria& aCriteria,
        /* IN  */       TBool aIsLm,
        /* OUT */       TBool& aUseDistanceSorting
        );

        static CPosLmSearchCriteria* ValidateAndCopyLC(
        /* IN  */       const CPosLmAreaCriteria& aCriteria
        );

        static CPosLmSearchCriteria* ValidateAndCopyLC(
        /* IN  */       const CPosLmTextCriteria& aCriteria
        );

        static CPosLmSearchCriteria* ValidateAndCopyLC(
        /* IN  */       const CPosLmCategoryCriteria& aCriteria
        );

        static CPosLmSearchCriteria* ValidateAndCopyLC(
        /* IN  */       const CPosLmNearestCriteria& aCriteria
        );

        static CPosLmSearchCriteria* ValidateAndCopyLC(
        /* IN  */       const CPosLmCatNameCriteria& aCriteria
        );

    };

#endif      // POSLMCRITERIAHELPER_H

// End of File
