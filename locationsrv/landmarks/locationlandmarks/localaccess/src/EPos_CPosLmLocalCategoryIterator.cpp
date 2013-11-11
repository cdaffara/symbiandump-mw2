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
* Description: Provides Category iterator.
*
*
*/


// INCLUDE FILES
#include    <epos_cposlmlocaldbaccess.h>
#include    <epos_poslmcategoryhandler.h>
#include    "EPos_CPosLmLocalCategoryIterator.h"
#include    "epos_cposlmlocaldatabase.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalCategoryIterator::CPosLmLocalCategoryIterator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalCategoryIterator::CPosLmLocalCategoryIterator() :
    CPosLmLocalIterator()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalCategoryIterator::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalCategoryIterator::ConstructL(
    CPosLmLocalDatabase& aDb,
    CPosLmCategoryManager::TCategorySortPref aSortPref,
    TBool aReferencedCategories)
    {
    aDb.AquireLockLC(CPosLmLocalDatabase::EReadLock);

    if (!aReferencedCategories)
        {
        PosLmCategoryHandler::GetAllCategoryIdsL(*aDb.DatabaseAccess(),
            aSortPref, iIdArray);
        }
    else
        {
        PosLmCategoryHandler::GetReferencedCategoryIdsL(*aDb.DatabaseAccess(),
            aSortPref, iIdArray);
        }

    CleanupStack::PopAndDestroy(); // DbLock
    }

// -----------------------------------------------------------------------------
// CPosLmLocalCategoryIterator::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalCategoryIterator* CPosLmLocalCategoryIterator::NewL(
    CPosLmLocalDatabase& aDb,
    CPosLmCategoryManager::TCategorySortPref aSortPref,
    TBool aReferencedCategories)
    {
    CPosLmLocalCategoryIterator* self =
        new(ELeave) CPosLmLocalCategoryIterator();
    CleanupStack::PushL(self);
    self->ConstructL(aDb, aSortPref, aReferencedCategories);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmLocalCategoryIterator::~CPosLmLocalCategoryIterator()
    {
    }

//  End of File
