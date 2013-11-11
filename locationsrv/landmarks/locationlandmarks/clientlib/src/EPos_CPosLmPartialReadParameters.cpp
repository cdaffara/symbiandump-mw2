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
* Description: Container class for partial landmark settings.
*
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmPartialReadParameters.h"
#include    "EPos_LandmarksErrors.h"

// ==================== LOCAL FUNCTIONS ====================

// ---------------------------------------------------------
// CheckParameters Panics if invalid attribute is passed.
// ---------------------------------------------------------
//
void CheckParameters(CPosLandmark::TAttributes aAttributes)
    {
    CPosLandmark::TAttributes findValues =
                                    CPosLandmark::ELandmarkName|
                                    CPosLandmark::EPosition|
                                    CPosLandmark::ECoverageRadius|
                                    CPosLandmark::ECategoryInfo|
                                    CPosLandmark::EIcon|
                                    CPosLandmark::EDescription;

    __ASSERT_ALWAYS(
        aAttributes == CPosLandmark::ENoAttribute ||
        aAttributes == CPosLandmark::EAllAttributes ||
        aAttributes <= findValues,
        Panic(KPosLandmarksClientPanic, EPosInvalidPartialReadParameters));
    }

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPosLmPartialReadParameters::CPosLmPartialReadParameters()
:   iRequestedFields(),
    iAttributes(CPosLandmark::ENoAttribute)
    {
    }

// EPOC default constructor can leave.
void CPosLmPartialReadParameters::ConstructL()
    {
    }

// Two-phased constructor.
EXPORT_C CPosLmPartialReadParameters* CPosLmPartialReadParameters::NewLC()
    {
    CPosLmPartialReadParameters* self =
        new (ELeave) CPosLmPartialReadParameters;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// Destructor
CPosLmPartialReadParameters::~CPosLmPartialReadParameters()
    {
    iRequestedFields.Close();
    }

// ---------------------------------------------------------
// CPosLmPartialReadParameters::RequestedAttributes
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CPosLandmark::TAttributes
    CPosLmPartialReadParameters::RequestedAttributes() const
    {
    return iAttributes;
    }

// ---------------------------------------------------------
// CPosLmPartialReadParameters::SetRequestedAttributes
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmPartialReadParameters::SetRequestedAttributes(
    CPosLandmark::TAttributes aAttributes)
    {
    CheckParameters(aAttributes);
    iAttributes = aAttributes;
    }

// ---------------------------------------------------------
// CPosLmPartialReadParameters::GetRequestedPositionFields
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLmPartialReadParameters::GetRequestedPositionFields(
    RArray<TUint>& aRequestedFields) const
    {
    aRequestedFields.Reset();
    for (TInt i = 0; i < iRequestedFields.Count(); i++)
        {
        TInt result = aRequestedFields.Append(iRequestedFields[i]);
        if (result != KErrNone)
            {
            return result;
            }
        }

    return KErrNone;
    }

// ---------------------------------------------------------
// CPosLmPartialReadParameters::SetRequestedPositionFields
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLmPartialReadParameters::SetRequestedPositionFields(
    const RArray<TUint>& aRequestedFields)
    {
    for (TInt i = 0; i < aRequestedFields.Count(); i++)
        {
        __ASSERT_ALWAYS(aRequestedFields[i] <= KMaxTUint16,
            Panic(KPosLandmarksClientPanic,
                EPosInvalidRequestedPositionFields));
        }

    iRequestedFields.Reset();

    for (TInt j = 0; j < aRequestedFields.Count(); j++)
        {
        TInt result = iRequestedFields.Append(aRequestedFields[j]);
        if (result != KErrNone)
            {
            return result;
            }
        }

    return KErrNone;
    }

//  End of File
