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
* Description:  CPosLmAreaCriteria class
*
*/


#ifndef CPOSLMAREACRITERIA_H
#define CPOSLMAREACRITERIA_H

#include <e32base.h>
#include "EPos_CPosLmSearchCriteria.h"

/**
*  Criterion for searching for landmarks which reside in a certain area.
*
*  The search area is defined by providing two latitude and two longitude
*  values which specify the borders of the area. Note that this search
*  does not consider landmark coverage radius, see
*  @p CPosLandmark::GetCoverageRadius.
*
*  The search area is defined as a spherical rectangle, limited by two
*  longitude borders (WestLongitude and EastLongitude) and two latitude
*  borders (SouthLatitude and NorthLatitude).
*
*  It is required that -90 =< SouthLatitude =< NorthLatitude =< 90.
*  WestLongitude must be in the interval [-180, 180), i.e. not including +180.
*  EastLongitude must be in the interval [-180, 180], i.e. including +180.
*  It is allowed that EastLongitude < WestLongitude. This defines an area
*  which crosses the 180 meridian. The area definition is inclusive, i.e.
*  landmarks which lie on the border of the area will be considered as matches.
*  If WestLongitude = EastLongitude and NorthLatitude = SouthLatitude then
*  only landmarks which match the longitude and latitude respectively, are
*  considered to be matches. If WestLongitude = -180 and EastLongitude = +180,
*  all longitudes are included in the search.
*
*  This criterion is only valid when searching for landmarks, i.e. if it
*  is passed to @p CPosLandmarkSearch::StartCategorySearchL, the function
*  will fail with error code @p KErrArgument.
*
*  @lib eposlmsearchlib.lib
*  @since S60 3.0
*/
class CPosLmAreaCriteria : public CPosLmSearchCriteria
    {
    public:

        /**
        * Two-phased constructor.
        *
        * The input parameters specify the area to search as described
        * in the class description.
        *
        * If one of the parameters is out of range, this function fails with
        * error code @p KErrArgument. The correct ranges are:
        *   -# -90 =< aSouthLatitude =< aNorthLatitude =< 90
        *   -# -180 <= aWestLongitude < 180
        *   -# -180 <= aEastLongitude <= 180
        *
        * @param[in] aSouthLatitude The southern latitude border of the search area.
        * @param[in] aNorthLatitude The northern latitude border of the search area.
        * @param[in] aWestLongitude The western longitude border of the search area.
        * @param[in] aEastLongitude The eastern longitude border of the search area.
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmAreaCriteria* NewLC(
            const TReal64& aSouthLatitude,
            const TReal64& aNorthLatitude,
            const TReal64& aWestLongitude,
            const TReal64& aEastLongitude
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmAreaCriteria();

    public:

        /**
        * Returns the search area.
        *
        * @param[out] aSouthLatitude The southern latitude border of the search area.
        * @param[out] aNorthLatitude The northern latitude border of the search area.
        * @param[out] aWestLongitude The western longitude border of the search area.
        * @param[out] aEastLongitude The eastern longitude border of the search area.
        */
        IMPORT_C void GetSearchArea(
            TReal64& aSouthLatitude,
            TReal64& aNorthLatitude,
            TReal64& aWestLongitude,
            TReal64& aEastLongitude
        ) const;

        /**
        * Define the search area.
        *
        * The input parameters specify the area to search as described in the
        * class description.
        *
        * If one of the parameters is out of range, this function fails with
        * error code @p KErrArgument. The correct ranges are:
        *   -# -90 =< aSouthLatitude =< aNorthLatitude =< 90
        *   -# -180 <= aWestLongitude < 180
        *   -# -180 <= aEastLongitude <= 180
        *
        * @param[in] aSouthLatitude The southern latitude border of the search area.
        * @param[in] aNorthLatitude The northern latitude border of the search area.
        * @param[in] aWestLongitude The western longitude border of the search area.
        * @param[in] aEastLongitude The eastern longitude border of the search area.
        * @returns @p KErrNone if successful, @p KErrArgument if the input
        *   parameters are invalid, otherwise a system wide error code.
        */
        IMPORT_C TInt SetSearchArea(
            const TReal64& aSouthLatitude,
            const TReal64& aNorthLatitude,
            const TReal64& aWestLongitude,
            const TReal64& aEastLongitude
        );

    private:

        // C++ constructor
        CPosLmAreaCriteria();

        void ConstructL(
            const TReal64&  aSouthLatitude,
            const TReal64&  aNorthLatitude,
            const TReal64&  aWestLongitude,
            const TReal64&  aEastLongitude
        );

        // Prohibit copy constructor
        CPosLmAreaCriteria( const CPosLmAreaCriteria& );
        // Prohibit assigment operator
        CPosLmAreaCriteria& operator= ( const CPosLmAreaCriteria& );

    private:

        TReal64 iSouthLatitude;
        TReal64 iNorthLatitude;
        TReal64 iWestLongitude;
        TReal64 iEastLongitude;

    };

#endif      // CPOSLMAREACRITERIA_H


