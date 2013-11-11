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
* Description: Implementation of CPosLmNearestCriteria class
*
*
*/


// INCLUDE FILES
#include    <e32math.h>
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmNearestCriteria.h"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmNearestCriteria::CPosLmNearestCriteria
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmNearestCriteria::CPosLmNearestCriteria(
    TBool aUseCoverageRadius)
:   CPosLmSearchCriteria(ECriteriaFindNearest),
    iUseCoverageRadius(aUseCoverageRadius)
    {
    TRealX nan;
    nan.SetNaN();

    iMaxDistance = nan;
    }

// -----------------------------------------------------------------------------
// CPosLmNearestCriteria::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmNearestCriteria::ConstructL(
    const TCoordinate& aCoordinate)
    {
    SetCoordinate(aCoordinate);
    }

// -----------------------------------------------------------------------------
// CPosLmNearestCriteria::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmNearestCriteria* CPosLmNearestCriteria::NewLC(
    const TCoordinate& aCoordinate,
    TBool aUseCoverageRadius)
    {
    CPosLmNearestCriteria* self =
        new (ELeave) CPosLmNearestCriteria(aUseCoverageRadius);
    CleanupStack::PushL(self);
    self->ConstructL(aCoordinate);
    return self;
    }

// Destructor
CPosLmNearestCriteria::~CPosLmNearestCriteria()
    {
    }

// ---------------------------------------------------------
// CPosLmNearestCriteria::GetCoordinate
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmNearestCriteria::GetCoordinate(
    TCoordinate& aCoordinate) const
    {
    aCoordinate = iCenterCoordinate;
    }

// ---------------------------------------------------------
// CPosLmNearestCriteria::SetCoordinate
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmNearestCriteria::SetCoordinate(
    const TCoordinate& aCoordinate)
    {
    if (Math::IsNaN(aCoordinate.Latitude()) ||
        Math::IsNaN(aCoordinate.Longitude()))
        {
        Panic(KPosLandmarksClientPanic, EPosNaNCoordinate);
        }

    iCenterCoordinate = aCoordinate;
    }

// ---------------------------------------------------------
// CPosLmNearestCriteria::UseCoverageRadius
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TBool CPosLmNearestCriteria::UseCoverageRadius() const
    {
    return iUseCoverageRadius;
    }

// ---------------------------------------------------------
// CPosLmNearestCriteria::SetUseCoverageRadius
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmNearestCriteria::SetUseCoverageRadius(
    TBool aUseCoverageRadius)
    {
    iUseCoverageRadius = aUseCoverageRadius;
    }

// ---------------------------------------------------------
// CPosLmNearestCriteria::MaxDistance
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TReal32 CPosLmNearestCriteria::MaxDistance() const
    {
    return iMaxDistance;
    }

// ---------------------------------------------------------
// CPosLmNearestCriteria::SetMaxDistance
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmNearestCriteria::SetMaxDistance(
    TReal32 aMaxDistance)
    {
    if (!Math::IsNaN(aMaxDistance) &&
        aMaxDistance < 0)
        {
        Panic(KPosLandmarksClientPanic, EPosNegativeValue);
        }

    iMaxDistance = aMaxDistance;
    }

//  End of File
