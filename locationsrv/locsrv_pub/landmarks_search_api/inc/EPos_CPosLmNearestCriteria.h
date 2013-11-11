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
* Description:  CPosLmNearestCriteria class
*
*/


#ifndef CPOSLMNEARESTCRITERIA_H
#define CPOSLMNEARESTCRITERIA_H

#include <e32base.h>
#include <lbsposition.h>
#include "EPos_CPosLmSearchCriteria.h"


/**
*  Criterion for finding the landmarks which are closest to a certain
*  coordinate, called "center coordinate".
*
*  When using @ref CPosLmNearestCriteria, the matches returned in the search
*  are sorted in ascending distance order if the client does not specify
*  other sort preferences.
*
*  By default, this search returns all landmarks in the database, except
*  those which have no coordinates. It is recommended to specify a maximum
*  distance to narrow down the search. This is done by @ref SetMaxDistance.
*
*  The client can specify if the coverage radius should be used.
*  If coverage radius is used, the distance to the landmark coverage area
*  border is used instead of the distance to the landmark coverage area
*  center. If the source coordinate is inside the landmark coverage area,
*  the distance is considered zero. By default, the coverage radius is not
*  used.
*
*  This criterion is only valid when searching for landmarks, i.e. if it
*  is passed to @p CPosLandmarkSearch::StartCategorySearchL, the function
*  will fail with error code @p KErrArgument.
*
*  @lib eposlmsearchlib.lib
*  @since S60 3.0
*/
class CPosLmNearestCriteria : public CPosLmSearchCriteria
    {
    public:

        /**
        * Two-phased constructor.
        *
        * Latitude and longitude must be set in the coordinate (not NaN).
        * Altitude is ignored.
        *
        * @param[in] aCoordinate The center coordinate of the landmark search.
        * @param[in] aUseCoverageRadius @p ETrue if coverage radius should be
        *   considered in the search.
        *
        * @panic "Landmarks Client"-EPosNaNCoordinate Latitude and/or longitude is NaN.
        */
        IMPORT_C static CPosLmNearestCriteria* NewLC(
            const TCoordinate& aCoordinate,
            TBool aUseCoverageRadius = EFalse
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmNearestCriteria();

    public:

        /**
        * Returns the center coordinate of the search.
        *
        * @param[out] aCoordinate On return, contains the center coordinate.
        */
        IMPORT_C void GetCoordinate( TCoordinate& aCoordinate ) const;

        /**
        * Set the center coordinate of the search.
        *
        * Latitude and longitude must be set in the coordinate (not NaN).
        * Altitude is ignored.
        *
        * @param[in] aCoordinate The center coordinate of the landmarks search.
        *
        * @panic "Landmarks Client"-EPosNaNCoordinate Latitude and/or longitude is NaN.
        */
        IMPORT_C void SetCoordinate( const TCoordinate& aCoordinate );

        /**
        * Returns if coverage radius is considered in the search.
        *
        * @return @p ETrue if coverage radius is considered in the search.
        *   @p EFalse otherwise.
        */
        IMPORT_C TBool UseCoverageRadius() const;

        /**
        * Set if coverage radius should be considered in the search.
        *
        * @param aUseCoverageRadius @p ETrue if coverage radius should be
        *   considered in the search. @p EFalse otherwise.
        */
        IMPORT_C void SetUseCoverageRadius( TBool aUseCoverageRadius );

        /**
        * Returns whether the search should be limited to landmarks which are
        * closer than a maximum distance.
        *
        * By default the maximum distance is set to NaN which means that the
        * distance is unlimited.
        *
        * @return The maximum distance or NaN if the distance is unlimited.
        */
        IMPORT_C TReal32 MaxDistance() const;

        /**
        * Set a maximum distance for the search.
        *
        * Only landmarks, which are closer to the center coordinate than the
        * maximum distance are considered matches.
        *
        * @param aMaxDistance The maximum distance or NaN if the distance
        *   should be unlimited.
        */
        IMPORT_C void SetMaxDistance( TReal32 aMaxDistance );

    private:

        CPosLmNearestCriteria(TBool aUseCoverageRadius);

        void ConstructL(const TCoordinate& aCoordinate);

        // Prohibit copy constructor
        CPosLmNearestCriteria( const CPosLmNearestCriteria& );
        // Prohibit assigment operator
        CPosLmNearestCriteria& operator= ( const CPosLmNearestCriteria& );

    private:

        TCoordinate iCenterCoordinate;
        TBool       iUseCoverageRadius;
        TReal32     iMaxDistance;

    };

#endif      // CPOSLMNEARESTCRITERIA_H


