/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Landmark iterator.
*
*
*/


#include    <e32base.h>
#include    <epos_cposlmlocaldbaccess.h>
#include    <epos_poslmlandmarkhandler.h>

#include    "epos_cposlmlocaldatabase.h"
#include    "EPos_CPosLmLocalGetSortedIdsOp.h"
#include    "EPos_CPosLmLocalLandmarkIterator.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalLandmarkIterator::CPosLmLocalLandmarkIterator() :
    CPosLmLocalIterator(),
    iSortPref(CPosLandmark::ELandmarkName),
    iUseSort(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalLandmarkIterator::CPosLmLocalLandmarkIterator(
    TPosLmSortPref aSortPref) :
    CPosLmLocalIterator(),
    iSortPref(aSortPref),
    iUseSort(ETrue)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalLandmarkIterator::ConstructL(
    CPosLmLocalDatabase& aDb, TBool aAlreadyHasLock )
    {
    if (iUseSort && iSortPref.LandmarkAttributeToSortBy() !=
        CPosLandmark::ELandmarkName)
        {
        User::Leave(KErrNotSupported);
        }

    if ( iUseSort )
        {
        CPosLmLocalGetSortedIdsOp* op = CPosLmLocalGetSortedIdsOp::NewL( aDb, iSortPref );
        CleanupStack::PushL( op );
        op->ExecuteL();
        op->GetResultL( iIdArray );
        CleanupStack::PopAndDestroy( op );
        }
    else
        {
        if (!aAlreadyHasLock)
            {
            aDb.AquireLockLC(CPosLmLocalDatabase::EReadLock);
            }
    
        PosLmLandmarkHandler::GetAllLandmarkIdsL(
            *aDb.DatabaseAccess(), 
            EFalse, EFalse, 
            iIdArray );
    
        if (!aAlreadyHasLock)
            {
            CleanupStack::PopAndDestroy(); // DbLock
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalLandmarkIterator* CPosLmLocalLandmarkIterator::NewL(
    CPosLmLocalDatabase& aDb, TBool aAlreadyHasLock)
    {
    CPosLmLocalLandmarkIterator* self =
        new(ELeave) CPosLmLocalLandmarkIterator();
    CleanupStack::PushL(self);
    self->ConstructL(aDb, aAlreadyHasLock);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalLandmarkIterator* CPosLmLocalLandmarkIterator::NewL(
    CPosLmLocalDatabase& aDb,
    TPosLmSortPref aSortPref)
    {
    CPosLmLocalLandmarkIterator* self = new(ELeave) CPosLmLocalLandmarkIterator(
        aSortPref);
    CleanupStack::PushL(self);
    self->ConstructL(aDb);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalLandmarkIterator::~CPosLmLocalLandmarkIterator()
    {
    }



