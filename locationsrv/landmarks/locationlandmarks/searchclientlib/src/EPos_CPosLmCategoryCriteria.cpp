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
* Description: Provides searching of landmarks based on category criteria
*
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmCategoryCriteria.h"
#include <EPos_Landmarks.h>

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmCategoryCriteria::CPosLmCategoryCriteria
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmCategoryCriteria::CPosLmCategoryCriteria()
:   CPosLmSearchCriteria(ECriteriaCategory),
    iItemId(KPosLmNullItemId),
    iGlobalCategory(KPosLmNullGlobalCategory),
    iCategoryName(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmCategoryCriteria::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmCategoryCriteria::ConstructL()
    {
    iCategoryName = KNullDesC().AllocL();
    }

// -----------------------------------------------------------------------------
// CPosLmCategoryCriteria::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmCategoryCriteria* CPosLmCategoryCriteria::NewLC()
    {
    CPosLmCategoryCriteria* self = new (ELeave) CPosLmCategoryCriteria;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// Destructor
CPosLmCategoryCriteria::~CPosLmCategoryCriteria()
    {
    delete iCategoryName;
    }

// ---------------------------------------------------------
// CPosLmCategoryCriteria::CategoryItemId
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPosLmItemId CPosLmCategoryCriteria::CategoryItemId() const
    {
    return iItemId;
    }

// ---------------------------------------------------------
// CPosLmCategoryCriteria::GlobalCategory
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPosLmGlobalCategory CPosLmCategoryCriteria::GlobalCategory() const
    {
    return iGlobalCategory;
    }

// ---------------------------------------------------------
// CPosLmCategoryCriteria::CategoryName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPtrC CPosLmCategoryCriteria::CategoryName() const
    {
    return iCategoryName->Des();
    }

// ---------------------------------------------------------
// CPosLmCategoryCriteria::SetCategoryItemId
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmCategoryCriteria::SetCategoryItemId(
    TPosLmItemId aItemId)
    {
    iItemId = aItemId;

    iGlobalCategory = KPosLmNullGlobalCategory;
    *iCategoryName = KNullDesC;
    }

// ---------------------------------------------------------
// CPosLmCategoryCriteria::SetGlobalCategory
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmCategoryCriteria::SetGlobalCategory(
    TPosLmGlobalCategory aGlobalCategory)
    {
    iGlobalCategory = aGlobalCategory;

    iItemId = KPosLmNullItemId;
    *iCategoryName = KNullDesC;
    }

// ---------------------------------------------------------
// CPosLmCategoryCriteria::SetCategoryNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmCategoryCriteria::SetCategoryNameL(
    const TDesC& aCategoryName)
    {
    if (aCategoryName.Length() > KPosLmMaxCategoryNameLength)
        {
        User::Leave(KErrArgument);
        }

    HBufC* previousName = iCategoryName;
    iCategoryName = aCategoryName.AllocL();
    delete previousName;

    iItemId = KPosLmNullItemId;
    iGlobalCategory = KPosLmNullGlobalCategory;
    }

//  End of File
