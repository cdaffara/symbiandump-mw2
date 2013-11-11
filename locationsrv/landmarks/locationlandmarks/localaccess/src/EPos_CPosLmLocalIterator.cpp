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
* Description: Local iterator base class.
*
*
*/


#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmLocalIterator.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalIterator::CPosLmLocalIterator() :
    CPosLmItemIterator()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalIterator::~CPosLmLocalIterator()
    {
    iIdArray.Close();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosLmItemId CPosLmLocalIterator::NextL()
    {
    if (iCurrent >= iIdArray.Count())
        {
        return KPosLmNullItemId;
        }

    return iIdArray[iCurrent++];
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalIterator::Reset()
    {
    iCurrent = 0;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUint CPosLmLocalIterator::NumOfItemsL()
    {
    return iIdArray.Count();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalIterator::GetItemIdsL(
    RArray<TPosLmItemId>& aIdArray,
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
    aIdArray.ReserveL( aNumOfItems ); // to avoid heap fragmentation
    for (TInt i = aStartIndex; i < aStartIndex + aNumOfItems; i++)
        {
        aIdArray.AppendL( iIdArray[i] );
        }
    }

