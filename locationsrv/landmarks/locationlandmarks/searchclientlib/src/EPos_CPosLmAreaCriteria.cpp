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
* Description: Provides searching of landmarks based on area criteria
*
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmAreaCriteria.h"


// CONSTANTS


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmAreaCriteria::CPosLmAreaCriteria
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmAreaCriteria::CPosLmAreaCriteria()
:   CPosLmSearchCriteria(ECriteriaArea)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmAreaCriteria::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmAreaCriteria::ConstructL(
    const TReal64& aSouthLatitude,
    const TReal64& aNorthLatitude,
    const TReal64& aWestLongitude,
    const TReal64& aEastLongitude)
    {
    User::LeaveIfError(SetSearchArea(aSouthLatitude, aNorthLatitude,
                                     aWestLongitude, aEastLongitude));
    }

// -----------------------------------------------------------------------------
// CPosLmAreaCriteria::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmAreaCriteria* CPosLmAreaCriteria::NewLC(
    const TReal64&  aSouthLatitude,
    const TReal64&  aNorthLatitude,
    const TReal64&  aWestLongitude,
    const TReal64&  aEastLongitude)
    {
    CPosLmAreaCriteria* self = new (ELeave) CPosLmAreaCriteria;
    CleanupStack::PushL(self);
    self->ConstructL(aSouthLatitude, aNorthLatitude,
                     aWestLongitude, aEastLongitude);
    return self;
    }

// Destructor
CPosLmAreaCriteria::~CPosLmAreaCriteria()
    {
    }

// ---------------------------------------------------------
// CPosLmAreaCriteria::GetSearchArea
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmAreaCriteria::GetSearchArea(
    TReal64&  aSouthLatitude,
    TReal64&  aNorthLatitude,
    TReal64&  aWestLongitude,
    TReal64&  aEastLongitude) const
    {
    aSouthLatitude = iSouthLatitude;
    aNorthLatitude = iNorthLatitude;
    aWestLongitude = iWestLongitude;
    aEastLongitude = iEastLongitude;
    }

// ---------------------------------------------------------
// CPosLmAreaCriteria::SetSearchArea
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLmAreaCriteria::SetSearchArea(
    const TReal64&  aSouthLatitude,
    const TReal64&  aNorthLatitude,
    const TReal64&  aWestLongitude,
    const TReal64&  aEastLongitude)
    {
    if (aSouthLatitude < -90 ||
        aNorthLatitude < aSouthLatitude ||
        aNorthLatitude > 90 ||
        aWestLongitude < -180 ||
        aEastLongitude < -180 ||
        aWestLongitude >= 180 ||
        aEastLongitude > 180)
        {
        return KErrArgument;
        }

    iSouthLatitude = aSouthLatitude;
    iNorthLatitude = aNorthLatitude;
    iWestLongitude = aWestLongitude;
    iEastLongitude = aEastLongitude;

    return KErrNone;
    }

//  End of File
