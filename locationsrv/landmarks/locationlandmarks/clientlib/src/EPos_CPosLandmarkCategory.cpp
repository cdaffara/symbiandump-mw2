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
* Description: Implementation of CPosLandmarkCategory class
*
*
*/


// INCLUDE FILES
#include "EPos_CPosLandmarkCategory.h"
#include "EPos_LandmarksErrors.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLandmarkCategory::CPosLandmarkCategory
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLandmarkCategory::CPosLandmarkCategory()
:   iId(KPosLmNullItemId),
    iGlobalCategory(KPosLmNullGlobalCategory),
    iCategoryName(NULL),
    iIconFileName(NULL),
    iIconIndex(KErrNotFound),
    iIconMaskIndex(KPosLmIconMaskNotUsed)
    {
    }

// -----------------------------------------------------------------------------
// CPosLandmarkCategory::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLandmarkCategory::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CPosLandmarkCategory::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLandmarkCategory::ConstructL(const CPosLandmarkCategory& aCategory)
    {
    iId = aCategory.iId;
    iGlobalCategory = aCategory.iGlobalCategory;
    iIconIndex = aCategory.iIconIndex;
    iIconMaskIndex = aCategory.iIconMaskIndex;

    if (aCategory.iCategoryName)
        {
        iCategoryName = aCategory.iCategoryName->AllocL();
        }

    if (aCategory.iIconFileName)
        {
        iIconFileName = aCategory.iIconFileName->AllocL();
        }
    }

// -----------------------------------------------------------------------------
// CPosLandmarkCategory::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkCategory* CPosLandmarkCategory::NewLC()
    {
    CPosLandmarkCategory* self = new (ELeave) CPosLandmarkCategory;
    CleanupStack::PushL(self);
    self->ConstructL();

    return self;
    }

// -----------------------------------------------------------------------------
// CPosLandmarkCategory::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkCategory* CPosLandmarkCategory::NewL()
    {
    CPosLandmarkCategory* self = CPosLandmarkCategory::NewLC();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CPosLandmarkCategory::NewLC
// Two-phased copy constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkCategory* CPosLandmarkCategory::NewLC(
    const CPosLandmarkCategory& aCategory)
    {
    CPosLandmarkCategory* self = new (ELeave) CPosLandmarkCategory;
    CleanupStack::PushL(self);
    self->ConstructL(aCategory);

    return self;
    }

// -----------------------------------------------------------------------------
// CPosLandmarkCategory::NewL
// Two-phased copy constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkCategory* CPosLandmarkCategory::NewL(
    const CPosLandmarkCategory& aCategory)
    {
    CPosLandmarkCategory* self = CPosLandmarkCategory::NewLC(aCategory);
    CleanupStack::Pop();

    return self;
    }

// Destructor
CPosLandmarkCategory::~CPosLandmarkCategory()
    {
    delete iCategoryName;
    delete iIconFileName;
    }

// ---------------------------------------------------------
// CPosLandmarkCategory::CategoryId
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPosLmItemId CPosLandmarkCategory::CategoryId() const
    {
    return iId;
    }

// ---------------------------------------------------------
// CPosLandmarkCategory::GlobalCategory
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPosLmGlobalCategory CPosLandmarkCategory::GlobalCategory() const
    {
    return iGlobalCategory;
    }

// ---------------------------------------------------------
// CPosLandmarkCategory::SetGlobalCategory
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLandmarkCategory::SetGlobalCategory(
    TPosLmGlobalCategory aGlobalCategory)
    {
    iGlobalCategory = aGlobalCategory;
    }

// ---------------------------------------------------------
// CPosLandmarkCategory::GetCategoryName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLandmarkCategory::GetCategoryName(
    TPtrC& aCategoryName) const
    {
    if (!iCategoryName)
        {
        return KErrNotFound;
        }

    aCategoryName.Set(*iCategoryName);
    return KErrNone;
    }

// ---------------------------------------------------------
// CPosLandmarkCategory::SetCategoryNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmarkCategory::SetCategoryNameL(
    const TDesC& aCategoryName)
    {
    if (aCategoryName.Length() > KPosLmMaxCategoryNameLength ||
        aCategoryName.Length() == 0)
        {
        User::Leave(KErrArgument);
        }

    HBufC* previousName = iCategoryName;
    iCategoryName = aCategoryName.AllocL();
    delete previousName;
    }

// ---------------------------------------------------------
// CPosLandmarkCategory::SetIconL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmarkCategory::SetIconL(
    const TDesC& aIconFileName,
    TInt aIconIndex,
    TInt aIconMaskIndex)
    {
    if (aIconFileName.Length() > KMaxFileName)
        {
        User::Leave(KErrArgument);
        }
    if (aIconIndex < 0 || aIconMaskIndex < KPosLmIconMaskNotUsed)
        {
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument);
        }

    HBufC* previousFileName = iIconFileName;
    iIconFileName = aIconFileName.AllocL();
    delete previousFileName;

    iIconIndex = aIconIndex;
    iIconMaskIndex = aIconMaskIndex;
    }

// ---------------------------------------------------------
// CPosLandmarkCategory::GetIcon
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLandmarkCategory::GetIcon(
    TPtrC& aIconFileName,
    TInt& aIconIndex,
    TInt& aIconMaskIndex) const
    {
    if (!iIconFileName)
        {
        return KErrNotFound;
        }

    aIconIndex = iIconIndex;
    aIconMaskIndex = iIconMaskIndex;
    aIconFileName.Set(*iIconFileName);
    return KErrNone;
    }

// ---------------------------------------------------------
// CPosLandmarkCategory::RemoveCategoryAttributes
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmarkCategory::RemoveCategoryAttributes(
    TAttributes aAttributes)
    {
    if (aAttributes & ECategoryName)
        {
        delete iCategoryName;
        iCategoryName = NULL;
        }

    if (aAttributes & EIcon)
        {
        delete iIconFileName;
        iIconFileName = NULL;
        }
    }

// ---------------------------------------------------------
// CPosLandmarkCategory::SetCategoryIdL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLandmarkCategory::SetCategoryIdL(
    TPosLmItemId aId)
    {
    iId = aId;
    }

//  End of File
