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
*    This is a helper class for sorting landmarks in distance order.
*
*/


#include "EPos_TPosLmNearestLandmark.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosLmNearestLandmark::TPosLmNearestLandmark()
:   iLmId(0),
    iDistance(0)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt TPosLmNearestLandmark::Compare(
    const TPosLmNearestLandmark& aFirst,
    const TPosLmNearestLandmark& aSecond)
    {
    if (aFirst.iDistance < aSecond.iDistance)
        {
        return -1;
        }

    if (aFirst.iDistance > aSecond.iDistance)
        {
        return 1;
        }

    return 0;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt TPosLmNearestLandmark::CompareByID(
    const TPosLmNearestLandmark& aFirst,
    const TPosLmNearestLandmark& aSecond)
    {
    if ( aFirst.iLmId < aSecond.iLmId )
        {
        return -1;
        }

    if ( aFirst.iLmId > aSecond.iLmId )
        {
        return 1;
        }

    return 0;
    }
