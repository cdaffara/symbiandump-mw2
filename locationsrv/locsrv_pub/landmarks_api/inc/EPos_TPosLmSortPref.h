/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  TPosLmSortPref class
*
*/


#ifndef TPOSLMSORTPREF_H
#define TPOSLMSORTPREF_H

#include <e32base.h>
#include "EPos_CPosLandmark.h"

/**
*  Class for specifying sort preference.
*
*  A sort pref object can be passed to some listing and searching functions in
*  the API, to specify a sorting order for the returned landmark list.
*
*  Landmark sorting is not case sensitive.
*
*  @lib eposlandmarks.lib
*  @since S60 3.0
*/
class TPosLmSortPref
    {
    public:

        /**
        *  Specifies the sort order.
        */
        enum TSortOrder
            {
            EAscending = 0  /**< Ascending sort order */,
            EDescending     /**< Descending sort order */
            };

    public:  // Constructors

        /**
        * C++ constructor.
        *
        * Only one landmark attribute can be specified here, not a bitmap
        * of landmark attributes.
        *
        * Landmark sorting is not case sensitive.
        *
        * @param[in] aLandmarkAttribute The landmark attribute to sort by.
        * @param[in] aSortOrder Ascending or descending sort order.
        *
        * @panic "Landmarks Client"-EPosNoneOrMultipleLandmarkAttributeSet 
        *   More than one attribute is specified or no attributes at all.
        */
        IMPORT_C TPosLmSortPref(
            CPosLandmark::TAttributes aLandmarkAttribute,
            TSortOrder aSortOrder = EAscending
        );

    public:

        /**
        * Gets the preferred sort order.
        *
        * @return The preferred sort order.
        */
        IMPORT_C TSortOrder SortOrder() const;

        /**
        * Gets the landmark attribute to sort by.
        *
        * @return The landmark attribute to sort by, or
        *   @p CPosLandmark::ENoAttribute, if sort is not done by landmark
        *   attribute.
        */
        IMPORT_C CPosLandmark::TAttributes LandmarkAttributeToSortBy() const;

        /**
        * Sets the landmark attribute to sort by.
        *
        * Landmark sorting is not case sensitive.
        *
        * @param[in] aLandmarkAttribute The landmark attribute to sort by.
        * @param[in] aSortOrder The preferred sort order.
        *
        * @panic "Landmarks Client"-EPosNoneOrMultipleLandmarkAttributeSet 
        *   More than one attribute is specified or no attributes at all.
        */
        IMPORT_C void SetSortByLandmarkAttribute(
            CPosLandmark::TAttributes aLandmarkAttribute,
            TSortOrder aSortOrder = EAscending
        );

    private:

        TSortOrder iSortOrder;
        CPosLandmark::TAttributes iLandmarkAttribute;
        TUint8 iUnusedData[16];

    };

#endif      // TPOSLMSORTPREF_H


