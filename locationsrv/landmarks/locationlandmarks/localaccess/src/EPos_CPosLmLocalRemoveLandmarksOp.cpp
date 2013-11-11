/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Operation for removing landmarks from a database.
*
*
*/


#include    <epos_cposlmresourcereader.h>
#include    <eposlmasyncops.rsg>
#include    <EPos_LandmarksErrors.h>
#include    <epos_cposlmdiskutilities.h>
#include    <epos_cposlmlocaldbaccess.h>
#include    <epos_poslmlandmarkhandler.h>

#include    "epos_cposlmlocaldatabase.h"
#include    "EPos_LocalLandmarks.h"
#include    "EPos_CPosLmLocalLandmarkIterator.h"
#include    "epos_rposlmlocalnameindex.h"
#include    "EPos_RPosLmLocalAccessSubsession.h"
#include    "EPos_CPosLmLocalRemoveLandmarksOp.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalRemoveLandmarksOp::CPosLmLocalRemoveLandmarksOp( CPosLmLocalDatabase& aDb ) :
    CPosLmLocalModifyOp(aDb)
    {
    iUsesServerData = ETrue;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveLandmarksOp::ConstructL(
    const RArray<TPosLmItemId>& aLandmarkIdArray,
    TInt aResourceId )
    {
    if ( aLandmarkIdArray.Count() == 0 )
        {
        User::Leave( KErrArgument );
        }

    BaseConstructL();

    ReadInfoFromResourceFileL( aResourceId );

    iLandmarkIdArray.ReserveL( aLandmarkIdArray.Count() ); // to avoid heap fragmentation
    for ( TInt i = 0; i < aLandmarkIdArray.Count(); i++ )
        {
        iLandmarkIdArray.AppendL( aLandmarkIdArray[i] );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveLandmarksOp::ConstructL()
    {
    BaseConstructL();

    CPosLmItemIterator* iterator =
        CPosLmLocalLandmarkIterator::NewL( *iDb, ETrue );
    CleanupStack::PushL( iterator );

    if ( iterator->NumOfItemsL() > 0 )
        {
        iterator->GetItemIdsL( iLandmarkIdArray, 0, iterator->NumOfItemsL() );
        }

    CleanupStack::PopAndDestroy( iterator );

    if ( iLandmarkIdArray.Count() > 0 )
        {
        // Check disk space
        CPosLmDiskUtilities* diskutil = CPosLmDiskUtilities::NewL();
        CleanupStack::PushL(diskutil);
        diskutil->DiskSpaceBelowCriticalLevelL(
            diskutil->EstimatedDiskSizeOfLmOperation(
                CPosLmDiskUtilities::ERemoveLandmarksOp,
                iLandmarkIdArray), iDb->DatabaseDrive());
        CleanupStack::PopAndDestroy(diskutil);

        ReadInfoFromResourceFileL( R_POS_LM_LOCAL_REMOVE_ALL_LMS_OP );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalRemoveLandmarksOp* CPosLmLocalRemoveLandmarksOp::NewL(
    CPosLmLocalDatabase& aDb,
    const RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    CPosLmLocalRemoveLandmarksOp* self = new (ELeave)
        CPosLmLocalRemoveLandmarksOp(aDb);
    CleanupStack::PushL(self);
    self->ConstructL(aLandmarkIdArray, R_POS_LM_LOCAL_REMOVE_LMS_OP);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalRemoveLandmarksOp* CPosLmLocalRemoveLandmarksOp::NewL(
    CPosLmLocalDatabase& aDb)
    {
    CPosLmLocalRemoveLandmarksOp* self = new (ELeave)
        CPosLmLocalRemoveLandmarksOp(aDb);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalRemoveLandmarksOp::~CPosLmLocalRemoveLandmarksOp()
    {
    RollbackAndGenerateEventIfNeeded(Progress());

    iLandmarkIdArray.Close();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveLandmarksOp::DoNextStepPreL(
    TReal32& aProgress)
    {
    if ( iLandmarkIdArray.Count() == 0 )
        {
        aProgress = 1;
        }
    else
        {
        RArray<TPosLmItemId> removed;
        CleanupClosePushL( removed );
        for (TInt i = 0; i < iNoOfSubOperations &&
            iCurrent < iLandmarkIdArray.Count(); i++)
            {
            PosLmLandmarkHandler::RemoveLandmarkL(
                *iDb->DatabaseAccess(), iLandmarkIdArray[iCurrent] );
            removed.AppendL( iLandmarkIdArray[iCurrent] );

            iCurrent++;
            }
        iDb->NameIndex().RemoveL( removed );
        CleanupStack::PopAndDestroy( &removed );

        // Set progress
        aProgress = TReal32(iCurrent)/iLandmarkIdArray.Count();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalRemoveLandmarksOp::DoNextStepPostL(
    TReal32& aProgress)
    {
    if (iCurrent < iLandmarkIdArray.Count())
        {
        return ETrue;
        }

    User::LeaveIfError(GenerateEventIfNeeded(aProgress));
    iStatusFlag = KErrNone;
    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmLocalRemoveLandmarksOp::Step()
    {
    if (iLandmarkIdArray.Count() == 0)
        {
        return 1.0f;
        }
    return 1.0f/iLandmarkIdArray.Count();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveLandmarksOp::CheckResourceValue(
    TInt /*aResourceId*/,
    TInt aNoOfSubOperations)
    {
    __ASSERT_ALWAYS(aNoOfSubOperations > 0, Panic(KPosLandmarksClientPanic,
        EPosInvalidValueSpecifiedInResourceFile));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveLandmarksOp::HandleError(
    TInt& aError)
    {
    iStatusFlag = aError;
    RollbackAndGenerateEventIfNeeded(Progress());
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveLandmarksOp::RollbackAndGenerateEventIfNeeded(
    TReal32 aProgress)
    {
    RollbackIfNeeded();

    GenerateEventIfNeeded(aProgress);

    // Reset flag: rollback and event generation should not be done twice.
    iStatusFlag = KErrNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalRemoveLandmarksOp::GenerateEventIfNeeded(
    TReal32 aProgress)
    {
    if (iStatusFlag != KErrNone && aProgress > 0)
        {
        // If only one landmark has been removed, a more specific event can be
        // reported.
        if (iCurrent == 1)
            {
            return iDb->RegisterEvent(EPosLmEventLandmarkDeleted,
                iLandmarkIdArray[0]);
            }

        // If more than one landmark have been removed, a general event is
        // reported.
        if (iCurrent > 0)
            {
            return iDb->RegisterEvent(EPosLmEventLandmarkUnknownChanges);
            }

        }
    // Nothing has happend, don't report event.
    return KErrNone;
    }

