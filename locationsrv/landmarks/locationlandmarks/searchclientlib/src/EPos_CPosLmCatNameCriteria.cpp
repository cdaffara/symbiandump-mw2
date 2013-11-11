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
* Description: Provides searching landmark categories with a certain name
*
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmCatNameCriteria.h"
#include    <EPos_Landmarks.h>

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmCatNameCriteria::CPosLmCatNameCriteria
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmCatNameCriteria::CPosLmCatNameCriteria()
:   CPosLmSearchCriteria(ECriteriaCategoryByName)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmCatNameCriteria::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmCatNameCriteria::ConstructL()
    {
    iSearchPattern = KNullDesC().AllocL();
    }

// -----------------------------------------------------------------------------
// CPosLmCatNameCriteria::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmCatNameCriteria* CPosLmCatNameCriteria::NewLC()
    {
    CPosLmCatNameCriteria* self = new (ELeave) CPosLmCatNameCriteria;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// Destructor
CPosLmCatNameCriteria::~CPosLmCatNameCriteria()
    {
    delete iSearchPattern;
    }

// ---------------------------------------------------------
// CPosLmCatNameCriteria::SearchPattern
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPtrC CPosLmCatNameCriteria::SearchPattern() const
    {
    return iSearchPattern->Des();
    }

// ---------------------------------------------------------
// CPosLmCatNameCriteria::SetSearchPatternL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmCatNameCriteria::SetSearchPatternL(
    const TDesC& aSearchPattern)
    {
    if (aSearchPattern.Length() > KPosLmMaxCategoryNameLength)
        {
        User::Leave(KErrArgument);
        }

    HBufC* previousSearchPattern = iSearchPattern;
    iSearchPattern = aSearchPattern.AllocL();
    delete previousSearchPattern;
    }

//  End of File
