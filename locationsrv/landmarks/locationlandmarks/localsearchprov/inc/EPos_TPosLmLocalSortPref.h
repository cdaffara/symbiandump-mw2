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
* Description: Class for specifying a sort preference that can be used both for landmark
*  sorting and landmark category sorting.
*
*/


#ifndef TPOSLMLOCALSORTPREF_H
#define TPOSLMLOCALSORTPREF_H

//  INCLUDES
#include <e32base.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLmCategoryManager.h>

// FORWARD DECLARATIONS
class TPosLmSortPref;

// CLASS DECLARATION

/**
*  Class for specifying a sort preference that can be used both for landmark
*  sorting and landmark category sorting.
*/
class TPosLmLocalSortPref
    {
    public:  // Data types

        /**
        *  Specifies the sort preference type.
        *
        */
        enum TSortType
            {
            ENoSorting = 0,
            ELandmarkSorting,
            ECategorySorting
            };

        /**
        *  Specifies the sort order.
        *
        */
        enum TSortOrder
            {
            ESortAscending = 0,
            ESortDescending
            };

    public:  // Constructors

        /**
        * C++ default constructor to be used for no sorting.
        *
        */
        TPosLmLocalSortPref();

        /**
        * C++ default constructor for landmark sorting.
        *
        * @param aSortPref Sort preference for the search results.
        */
        TPosLmLocalSortPref(
        /* IN  */       const TPosLmSortPref& aSortPref
        );

        /**
        * C++ default constructor for category sorting.
        *
        * @param aSortPref Sort preference for the search results.
        */
        TPosLmLocalSortPref(
        /* IN  */       CPosLmCategoryManager::TCategorySortPref aSortPref
        );

    public: // New functions

        /**
        * Get the preferred sort type.
        *
        * @return The preferred sort type.
        */
        TSortType SortType() const;

        /**
        * Get the preferred sort order.
        *
        * @return The preferred sort order.
        */
        TSortOrder SortOrder() const;

        /**
        * Get the landmark attribute to sort by.
        *
        * @return The landmark attribute to sort by, or
        *   @p CPosLandmark::ENoAttribute, if sort is not done by landmark
        *   attribute.
        */
        CPosLandmark::TAttributes LandmarkAttributeToSortBy() const;

    private:

        TSortType iSortType;

        TSortOrder iSortOrder;

        CPosLandmark::TAttributes iLmSortAttribute;
    };

#endif      // TPOSLMLOCALSORTPREF_H

// End of File
