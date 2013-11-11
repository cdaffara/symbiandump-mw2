/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*     See class description below
*
*/


#ifndef TPOSLMNEARESTLANDMARK_H
#define TPOSLMNEARESTLANDMARK_H

#include <e32base.h>
#include <EPos_Landmarks.h>

/**
*  This is a helper class for sorting landmarks in ascending distance order.
*/
class TPosLmNearestLandmark
    {
    public:

        /**
        * C++ default constructor.
        */
        TPosLmNearestLandmark();

        /**
        * Function to be used as a @ref TLinearOrder.
        * It compares the distance to the two landmarks.
        *
        * @param[in] aFirst The first landmark to compare.
        * @param[in] aSecond The second landmark to compare.
        * @return 0 if the two distances are equal, -1 if the distance to the
        *   first landmark is less than the distance to the second landmark,
        *   1 if the distance to the first landmark is greater than the
        *   distance to the second landmark.
        */
        static TInt Compare(
        	const TPosLmNearestLandmark& aFirst,
        	const TPosLmNearestLandmark& aSecond );

        /**
        * Function to be used as a @ref TLinearOrder.
        * It compares the ID to the two landmarks.
        *
        * @param[in] aFirst The first landmark to compare.
        * @param[in] aSecond The second landmark to compare.
        * @return 0 if the two IDs are equal, -1 if the ID of the
        *   first landmark is less than the ID of the second landmark,
        *   1 if the ID of the first landmark is greater than the
        *   ID of the second landmark.
        */
        static TInt CompareByID(
            const TPosLmNearestLandmark& aFirst,
            const TPosLmNearestLandmark& aSecond );

    public:

        /** The id of the landmark. */
        TPosLmItemId    iLmId;

        /** The distance to the landmark. */
        TReal32         iDistance;
    };

#endif  // TPOSLMNEARESTLANDMARK_H

// End of File
