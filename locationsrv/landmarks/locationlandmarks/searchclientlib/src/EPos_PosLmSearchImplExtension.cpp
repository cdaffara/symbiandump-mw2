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
* Description: Provides static exported functions needed by the implementation
*  of components in the Landmarks subsystem.
*
*
*/


// INCLUDE FILES
#include    <EPos_CPosLmPartialReadParameters.h>
#include    "EPos_PosLmSearchImplExtension.h"
#include    "EPos_CPosLmDisplayData.h"
#include    "EPos_CPosLmDisplayItem.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// PosLmSearchImplExtension::PartialReadParameters
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmPartialReadParameters*
PosLmSearchImplExtension::PartialReadParameters(
    const CPosLmDisplayData& aDisplayData)
    {
    return aDisplayData.PartialReadParameters();
    }

// -----------------------------------------------------------------------------
// PosLmSearchImplExtension::SetDatabaseIndex
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmSearchImplExtension::SetDatabaseIndex(
    CPosLmDisplayItem& aDisplayItem,
    TUint aDatabaseIndex)
    {
    aDisplayItem.SetDatabaseIndex(aDatabaseIndex);
    }

// -----------------------------------------------------------------------------
// PosLmSearchImplExtension::SetDistance
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmSearchImplExtension::SetDistance(
    CPosLmDisplayItem& aDisplayItem,
    TReal32 aDistance)
    {
    aDisplayItem.SetDistance(aDistance);
    }

//  End of File
