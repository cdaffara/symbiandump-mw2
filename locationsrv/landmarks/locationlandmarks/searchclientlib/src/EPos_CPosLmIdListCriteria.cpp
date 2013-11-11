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
* Description: Implementation of CPosLmIdListCriteria class
*
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmIdListCriteria.h"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmIdListCriteria::CPosLmIdListCriteria
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmIdListCriteria::CPosLmIdListCriteria()
:   CPosLmSearchCriteria(ECriteriaIdList)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmIdListCriteria::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmIdListCriteria* CPosLmIdListCriteria::NewLC()
    {
    CPosLmIdListCriteria* self = new (ELeave) CPosLmIdListCriteria;
    CleanupStack::PushL(self);
    return self;
    }

// Destructor
EXPORT_C CPosLmIdListCriteria::~CPosLmIdListCriteria()
    {
    iIdArray.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmIdListCriteria::SetLandmarkIdsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmIdListCriteria::SetLandmarkIdsL(
    const RArray<TPosLmItemId>& aIdArray)
    {
    iIdArray.Reset();
    for (TInt i = 0; i < aIdArray.Count(); i++)
        {
        User::LeaveIfError(iIdArray.Append(aIdArray[i]));
        }
    }

// -----------------------------------------------------------------------------
// CPosLmIdListCriteria::GetLandmarkIdsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmIdListCriteria::GetLandmarkIdsL(
    RArray<TPosLmItemId>& aIdArray) const
    {
    aIdArray.Reset();
    for (TInt i = 0; i < iIdArray.Count(); i++)
        {
        User::LeaveIfError(aIdArray.Append(iIdArray[i]));
        }
    }

//  End of File
