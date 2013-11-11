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
* Description: Iterator for landmark item search matches
*
*
*/


// INCLUDE FILES
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmEmptyIterator.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmEmptyIterator::CPosLmEmptyIterator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmEmptyIterator::CPosLmEmptyIterator()
:   CPosLmItemIterator()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmEmptyIterator::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmEmptyIterator* CPosLmEmptyIterator::NewL()
    {
    CPosLmEmptyIterator* self = new(ELeave) CPosLmEmptyIterator();
    return self;
    }

// Destructor
CPosLmEmptyIterator::~CPosLmEmptyIterator()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmEmptyIterator::NextL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPosLmItemId CPosLmEmptyIterator::NextL()
    {
    return KPosLmNullItemId;
    }

// -----------------------------------------------------------------------------
// CPosLmEmptyIterator::Reset
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEmptyIterator::Reset()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmEmptyIterator::NumOfItemsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint CPosLmEmptyIterator::NumOfItemsL()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CPosLmEmptyIterator::GetItemIdsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEmptyIterator::GetItemIdsL(
    RArray<TPosLmItemId>& /*aIdArray*/,
    TInt /*aStartIndex*/,
    TInt /*aNumOfItems*/)
    {
    Panic(KPosLandmarksClientPanic,
        EPosSpecifiedIntervalLiesOutsideIteratedSet);
    }

//  End of File
