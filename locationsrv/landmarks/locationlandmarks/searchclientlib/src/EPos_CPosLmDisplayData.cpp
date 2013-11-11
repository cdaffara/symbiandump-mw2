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
* Description: Imlpementation of CPosLmDisplayData class
*
*
*/


#include    <EPos_LandmarksErrors.h>
#include    <EPos_CPosLmPartialReadParameters.h>
#include    "EPos_CPosLmDisplayData.h"
#include    "EPos_CPosLmDisplayItem.h"

const TInt KPosLmMatchesGranularity = 32;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmDisplayData::CPosLmDisplayData()
:   CBase(), 
    iDisplayItems( KPosLmMatchesGranularity ),
    iNewDisplayItems( KPosLmMatchesGranularity )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDisplayData* CPosLmDisplayData::NewL()
    {
    CPosLmDisplayData* self = new (ELeave) CPosLmDisplayData;
    return self;
    }


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmDisplayData::~CPosLmDisplayData()
    {
    Reset();

    delete iPartialParameters;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDisplayData::NewItemIndex()
    {
    // Remove index from array and return it.
    // If no more available return KPosLmNoNewItems.

    TInt newItemIndex = KPosLmNoNewItems;

    if (iNewDisplayItems.Count() != 0)
        {
        CPosLmDisplayItem*& newItem = iNewDisplayItems[0];
        newItemIndex = iDisplayItems.Find(newItem);
        iNewDisplayItems.Remove(0);

        __ASSERT_DEBUG(newItemIndex != KErrNotFound,
            Panic(KPosLandmarksClientPanic, EPosInvalidIndex));
        }

    return newItemIndex;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDisplayData::Count() const
    {
    return iDisplayItems.Count();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDisplayData::Reset()
    {
    iDisplayItems.ResetAndDestroy();
    iNewDisplayItems.Reset();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDisplayItem& CPosLmDisplayData::DisplayItem(
    TInt aItemIndex) const
    {
    __ASSERT_ALWAYS(aItemIndex >= 0 && aItemIndex < iDisplayItems.Count(),
        Panic(KPosLandmarksClientPanic, EPosInvalidIndex));

    return *iDisplayItems[aItemIndex];
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDisplayData::SetPartialReadParametersL(
    const CPosLmPartialReadParameters& aPartialSettings)
    {
    CPosLmPartialReadParameters* parameters =
        CPosLmPartialReadParameters::NewLC();

    parameters->SetRequestedAttributes(
        aPartialSettings.RequestedAttributes());

    RArray<TUint> requestedFields;
    CleanupClosePushL(requestedFields);

    User::LeaveIfError(
        aPartialSettings.GetRequestedPositionFields(requestedFields));
    User::LeaveIfError(
        parameters->SetRequestedPositionFields(requestedFields));

    CleanupStack::PopAndDestroy(&requestedFields);

    // Remove old parameters
    UnsetPartialReadParameters();

    iPartialParameters = parameters;
    CleanupStack::Pop(parameters);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDisplayData::UnsetPartialReadParameters()
    {
    delete iPartialParameters;
    iPartialParameters = NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmPartialReadParameters* CPosLmDisplayData::PartialReadParameters() const
    {
    return iPartialParameters;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
RPointerArray<CPosLmDisplayItem>& CPosLmDisplayData::DisplayItems()
    {
    return iDisplayItems;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
RPointerArray<CPosLmDisplayItem>& CPosLmDisplayData::NewDisplayItems()
    {
    return iNewDisplayItems;
    }

