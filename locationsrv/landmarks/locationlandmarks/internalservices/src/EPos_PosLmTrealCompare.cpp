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
* Description: Provides functions to compare two TReal values.
*
*
*/


// INCLUDE FILES
#include    "EPos_PosLmTrealCompare.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// PosLmTrealCompare::IsEqual
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool PosLmTrealCompare::IsEqual(
    TReal32 aReal1,
    TReal32 aReal2)
    {
    return IsEqual(TRealX(aReal1), TRealX(aReal2));
    }

// -----------------------------------------------------------------------------
// PosLmTrealCompare::IsEqual
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool PosLmTrealCompare::IsEqual(
    TReal64 aReal1,
    TReal64 aReal2)
    {
    return IsEqual(TRealX(aReal1), TRealX(aReal2));
    }

// -----------------------------------------------------------------------------
// PosLmTrealCompare::IsEqual
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool PosLmTrealCompare::IsEqual(
    TRealX aReal1,
    TRealX aReal2)
    {
    return (aReal1.IsNaN() && aReal2.IsNaN()) || aReal1 == aReal2;
    }

//  End of File
