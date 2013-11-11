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
* Description: Provides combine multiple search criterion.
*
*
*/


// INCLUDE FILES
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmCompositeCriteria.h"

// CONSTANTS


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmCompositeCriteria::CPosLmCompositeCriteria
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmCompositeCriteria::CPosLmCompositeCriteria(TCompositionType aType)
:   CPosLmSearchCriteria(ECriteriaComposite)
    {
    SetCompositionType(aType);
    }

// -----------------------------------------------------------------------------
// CPosLmCompositeCriteria::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmCompositeCriteria::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmCompositeCriteria::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmCompositeCriteria* CPosLmCompositeCriteria::NewLC(
    TCompositionType aType)
    {
    CPosLmCompositeCriteria* self = new (ELeave) CPosLmCompositeCriteria(aType);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// Destructor
CPosLmCompositeCriteria::~CPosLmCompositeCriteria()
    {
    iArguments.ResetAndDestroy();
    }

// ---------------------------------------------------------
// CPosLmCompositeCriteria::CompositionType
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CPosLmCompositeCriteria::TCompositionType
    CPosLmCompositeCriteria::CompositionType() const
    {
    return iCompositionType;
    }

// ---------------------------------------------------------
// CPosLmCompositeCriteria::SetCompositionType
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmCompositeCriteria::SetCompositionType(
    TCompositionType aType)
    {
    __ASSERT_ALWAYS(aType == ECompositionAND || aType == ECompositionOR,
        Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));
    iCompositionType = aType;
    }

// ---------------------------------------------------------
// CPosLmCompositeCriteria::NumOfArguments
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TUint CPosLmCompositeCriteria::NumOfArguments() const
    {
    return iArguments.Count();
    }

// ---------------------------------------------------------
// CPosLmCompositeCriteria::Argument
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C const CPosLmSearchCriteria& CPosLmCompositeCriteria::Argument(
    TUint aIndex) const
    {
    return *iArguments[aIndex];
    }

// ---------------------------------------------------------
// CPosLmCompositeCriteria::Argument
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CPosLmSearchCriteria& CPosLmCompositeCriteria::Argument(
    TUint aIndex)
    {
    return *iArguments[aIndex];
    }

// ---------------------------------------------------------
// CPosLmCompositeCriteria::AddArgument
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLmCompositeCriteria::AddArgument(
    CPosLmSearchCriteria* aCriteria)
    {
    if(iCompositionType == ECompositionAND)
    {
    	
	    switch (aCriteria->CriteriaType())
	        {
	        case ECriteriaArea:
	        case ECriteriaText:
	        case ECriteriaCategory:
	        case ECriteriaFindNearest:
	        case ECriteriaIdList:
	            return iArguments.Append(aCriteria);

	        default:
	            return KErrNotSupported;
	        }
    }
    else
    {
    	//For 'OR' Composition only text criteria supported.
    	switch (aCriteria->CriteriaType())
	        {
	        case ECriteriaText:
	            return iArguments.Append(aCriteria);
	        default:
	            return KErrNotSupported;
	        }	
    }
    }

// ---------------------------------------------------------
// CPosLmCompositeCriteria::RemoveArgument
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CPosLmSearchCriteria* CPosLmCompositeCriteria::RemoveArgument(
    TUint aIndex)
    {
    CPosLmSearchCriteria* criteria = iArguments[aIndex];
    iArguments.Remove(aIndex);
    return criteria;
    }

// ---------------------------------------------------------
// CPosLmCompositeCriteria::ClearArguments
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmCompositeCriteria::ClearArguments()
    {
    iArguments.ResetAndDestroy();
    }

//  End of File
