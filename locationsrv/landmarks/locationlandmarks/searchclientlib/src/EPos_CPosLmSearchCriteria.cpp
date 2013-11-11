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
* Description: Implementation of CPosLmSearchCriteria class
*
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmSearchCriteria.h"


// CONSTANTS


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmSearchCriteria::CPosLmSearchCriteria
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmSearchCriteria::CPosLmSearchCriteria(
    TCriteriaType aCriteriaType)
:   iCriteriaType(aCriteriaType)
    {
    }

// Destructor
CPosLmSearchCriteria::~CPosLmSearchCriteria()
    {
    }

// ---------------------------------------------------------
// CPosLmSearchCriteria::CriteriaType
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CPosLmSearchCriteria::TCriteriaType
    CPosLmSearchCriteria::CriteriaType() const
    {
    return iCriteriaType;
    }

//  End of File
