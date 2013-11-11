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
* Description: Implementation of CPosLmTextCriteria class 
*
*
*/


// INCLUDE FILES
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmTextCriteria.h"


// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmTextCriteria::CPosLmTextCriteria
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmTextCriteria::CPosLmTextCriteria()
:   CPosLmSearchCriteria(ECriteriaText),
    iAttributes(CPosLandmark::ENoAttribute)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmTextCriteria::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmTextCriteria::ConstructL()
    {
    iText = KNullDesC().AllocL();
    }

// -----------------------------------------------------------------------------
// CPosLmTextCriteria::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmTextCriteria* CPosLmTextCriteria::NewLC()
    {
    CPosLmTextCriteria* self = new (ELeave) CPosLmTextCriteria;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// Destructor
CPosLmTextCriteria::~CPosLmTextCriteria()
    {
    delete iText;
    iPositionFieldArray.Close();
    }

// ---------------------------------------------------------
// CPosLmTextCriteria::Text
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPtrC CPosLmTextCriteria::Text() const
    {
    return iText->Des();
    }

// ---------------------------------------------------------
// CPosLmTextCriteria::SetTextL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmTextCriteria::SetTextL(
    const TDesC& aText)
    {
    if (aText.Length() > KPosLmMaxSearchStringLength)
        {
        User::Leave(KErrArgument);
        }

    HBufC* previousText = iText;
    iText = aText.AllocL();
    delete previousText;
    }

// ---------------------------------------------------------
// CPosLmTextCriteria::AttributesToSearch
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CPosLandmark::TAttributes
    CPosLmTextCriteria::AttributesToSearch() const
    {
    return iAttributes;
    }

// ---------------------------------------------------------
// CPosLmTextCriteria::SetAttributesToSearch
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmTextCriteria::SetAttributesToSearch(
    CPosLandmark::TAttributes aAttributes)
    {
    // Check if any other attributes are set apart from the valid ones.
    CPosLandmark::TAttributes allButName =
        CPosLandmark::EAllAttributes - CPosLandmark::ELandmarkName;

    CPosLandmark::TAttributes allButDescription =
        CPosLandmark::EAllAttributes - CPosLandmark::EDescription;

    CPosLandmark::TAttributes otherAttributes =
        aAttributes & allButName & allButDescription;

    if (otherAttributes != CPosLandmark::ENoAttribute)
        {
        Panic(KPosLandmarksClientPanic, EPosInvalidLandmarkAttribute);
        }

    iAttributes =  aAttributes;
    }

// ---------------------------------------------------------
// CPosLmTextCriteria::GetPositionFieldsToSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmTextCriteria::GetPositionFieldsToSearchL(
    RArray<TUint>& aFieldArray) const
    {
    aFieldArray.Reset();
    for (TInt i = 0; i < iPositionFieldArray.Count(); i++)
        {
        User::LeaveIfError(aFieldArray.Append(iPositionFieldArray[i]));
        }
    }

// ---------------------------------------------------------
// CPosLmTextCriteria::SetPositionFieldsToSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmTextCriteria::SetPositionFieldsToSearchL(
    const RArray<TUint>& aFieldArray)
    {
    iPositionFieldArray.Reset();
    for (TInt i = 0; i < aFieldArray.Count(); i++)
        {
        User::LeaveIfError(iPositionFieldArray.Append(aFieldArray[i]));
        }
    }

// ---------------------------------------------------------
// CPosLmTextCriteria::ClearPositionFieldsToSearch
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmTextCriteria::ClearPositionFieldsToSearch()
    {
    iPositionFieldArray.Reset();
    }

//  End of File
