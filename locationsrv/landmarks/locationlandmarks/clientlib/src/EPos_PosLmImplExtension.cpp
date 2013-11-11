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
* Description: Class containing static exported functions needed by the implementation
*  of components in the Landmarks subsystem.
*
*
*/


// INCLUDE FILES
#include    "EPos_PosLmImplExtension.h"
#include    "EPos_CPosLandmarkDatabase.h"
#include    "EPos_CPosLandmark.h"
#include    "EPos_CPosLandmarkCategory.h"


// CONSTANTS


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// PosLmImplExtension::ImplementationId
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TUid PosLmImplExtension::ImplementationId(
    const CPosLandmarkDatabase& aDatabase)
    {
    return aDatabase.ImplementationId();
    }

// ---------------------------------------------------------
// PosLmImplExtension::SetPartialL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void PosLmImplExtension::SetPartialL(
    CPosLandmark &aLandmark,
    TBool aPartial)
    {
    aLandmark.SetPartialL(aPartial);
    }

// ---------------------------------------------------------
// PosLmImplExtension::SetLandmarkIdL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void PosLmImplExtension::SetLandmarkIdL(
    CPosLandmark &aLandmark,
    TPosLmItemId aId)
    {
    aLandmark.SetLandmarkIdL(aId);
    }

// ---------------------------------------------------------
// PosLmImplExtension::SetCategoryIdL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void PosLmImplExtension::SetCategoryIdL(
    CPosLandmarkCategory &aCategory,
    TPosLmItemId aId)
    {
    aCategory.SetCategoryIdL(aId);
    }

// ---------------------------------------------------------
// PosLmImplExtension::SetGlobalCategory
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void PosLmImplExtension::SetGlobalCategory(
    CPosLandmarkCategory& aCategory,
    TPosLmGlobalCategory aGlobalCategory)
    {
    aCategory.SetGlobalCategory(aGlobalCategory);
    }

//  End of File
