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
* Description: Iterator for landmark item search matches.
*
*
*/


// INCLUDE FILES
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmLocalSearchIterator.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalSearchIterator::CPosLmLocalSearchIterator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalSearchIterator::CPosLmLocalSearchIterator()
:   CPosLmItemIterator(),
    iCurrent(0)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchIterator::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchIterator::ConstructL(
    const RArray<TPosLmItemId>& aMatchArray)
    {
    for (TInt i = 0; i < aMatchArray.Count(); i++)
        {
        User::LeaveIfError(iIdArray.Append(aMatchArray[i]));
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchIterator::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalSearchIterator* CPosLmLocalSearchIterator::NewL(
    const RArray<TPosLmItemId>& aMatchArray)
    {
    CPosLmLocalSearchIterator* self = new(ELeave) CPosLmLocalSearchIterator();
    CleanupStack::PushL(self);
    self->ConstructL(aMatchArray);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchIterator::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalSearchIterator* CPosLmLocalSearchIterator::NewL()
    {
    return new (ELeave) CPosLmLocalSearchIterator();
    }

// Destructor
CPosLmLocalSearchIterator::~CPosLmLocalSearchIterator()
    {
    iIdArray.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchIterator::NextL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPosLmItemId CPosLmLocalSearchIterator::NextL()
    {
    if (iCurrent >= iIdArray.Count())
        {
        return KPosLmNullItemId;
        }

    return iIdArray[iCurrent++];
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchIterator::Reset
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchIterator::Reset()
    {
    iCurrent = 0;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchIterator::NumOfItemsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint CPosLmLocalSearchIterator::NumOfItemsL()
    {
    return iIdArray.Count();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchIterator::GetItemIdsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchIterator::GetItemIdsL(
    RArray<TPosLmItemId>&  aIdArray,
    TInt aStartIndex,
    TInt aNumOfItems)
    {
    if (aStartIndex < 0 || aStartIndex >= iIdArray.Count() ||
        aStartIndex + aNumOfItems > iIdArray.Count())
        {
        Panic(KPosLandmarksClientPanic,
            EPosSpecifiedIntervalLiesOutsideIteratedSet);
        }

    aIdArray.Reset();

    for (TInt i = aStartIndex; i < aStartIndex + aNumOfItems; i++)
        {
        User::LeaveIfError(aIdArray.Append(iIdArray[i]));
        }
    }

//  End of File
