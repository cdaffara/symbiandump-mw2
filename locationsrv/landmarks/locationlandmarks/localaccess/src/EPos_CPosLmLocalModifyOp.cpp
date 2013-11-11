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
* Description: A local representation of the operation class assigned for operations that
* modifies the database.
*
*
*/


#include    <epos_cposlmlocaldbaccess.h>
#include    <epos_cposlmresourcereader.h>

#include    "epos_cposlmlocaldatabase.h"
#include    "EPos_RPosLmLocalAccessSubsession.h"
#include    "EPos_LocalLandmarks.h"
#include    "EPos_CPosLmLocalModifyOp.h"

const TReal32 KPosLmLocalSubOpInitialProgress = 0.05f;
const TReal32 KPosLmLocalSubOpProgressChange = 0.9f;
const TReal32 KPosLmLocalSubOpAlmost100Percent = 0.99f;


// ========================== MEMBER FUNCTIONS =================================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalModifyOp::CPosLmLocalModifyOp(
    CPosLmLocalDatabase& aDb) :
    CPosLmLocalOperation(aDb),
    iStatusFlag(KPosLmOperationNotComplete),
    iCompactState(ECompactSubOperationOff)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalModifyOp::BaseConstructL()
    {
    CPosLmLocalOperation::BaseConstructL();

    AquireLockL(CPosLmLocalDatabase::EWriteLock);
    User::LeaveIfError(iDb->DatabaseAccess()->BeginTransaction());
    if ( iUsesServerData )
        {
        iDb->BeginServerTransactionL();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalModifyOp::~CPosLmLocalModifyOp()
    {
    delete iCompactSubOp;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalModifyOp::NextStepL(
    TReal32& aProgress)
    {
    if (iCompactState == ECompactSubOperationOn)
        {
        // Compact state. Continue compacting sub operation.
        TReal32 progress;
        if (iCompactSubOp->NextL(progress))
            {
            delete iCompactSubOp;
            iCompactSubOp = NULL;
            iCompactState = ECompactSubOperationOff;
            }

        aProgress = iSavedProgress +
            CalculateSubOperationProgress(Step(), progress);
        }
    else
        {
        // Normal operation state
        DoNextStepPreL(aProgress);

        User::LeaveIfError(iDb->DatabaseAccess()->CommitTransaction());
        if ( iUsesServerData )
            {
            iDb->CommitServerTransactionL();
            }

        if (iDb->DatabaseAccess()->IsCompactNeededL())
            {
            RecalculateProgressDueToCompact(aProgress, Step());

            iCompactState = ECompactSubOperationOn;
            iSavedProgress = aProgress;
            iCompactSubOp = CPosLmCompactDatabase::NewL(*iDb->DatabaseAccess());
            }
        }

    if (iCompactState == ECompactSubOperationOff)
        {
        if (DoNextStepPostL(aProgress))
            {
            User::LeaveIfError(iDb->DatabaseAccess()->BeginTransaction());
            if ( iUsesServerData )
                {
                iDb->BeginServerTransactionL();
                }
            }
        else
            {
            ReleaseLock();
            }
        }

    return iStatusFlag;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalModifyOp::ReadInfoFromResourceFileL(
    TInt aResourceId)
    {
    CPosLmResourceReader* resources = CPosLmResourceReader::NewLC(
        KPosLocalSuboperationResourceFile);
    iNoOfSubOperations = resources->ReadInt32L(aResourceId);
    CleanupStack::PopAndDestroy(resources);

    CheckResourceValue(aResourceId, iNoOfSubOperations);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalModifyOp::RollbackIfNeeded()
    {
    if ( iStatusFlag != KErrNone )
        {
        if (iDb->DatabaseAccess()->InTransaction())
            {
            iDb->DatabaseAccess()->RollbackTransaction();
            if ( iUsesServerData )
                {
                iDb->RollbackServerTransaction();
                }
            }
        ReleaseLock();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalModifyOp::IsInCompactingState()
    {
    return (iCompactState == ECompactSubOperationOn);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmLocalModifyOp::CalculateSubOperationProgress(
    TReal32 aProgressStep,
    TReal32 aCompactProgress)
    {
    return aProgressStep * (KPosLmLocalSubOpInitialProgress +
        KPosLmLocalSubOpProgressChange * aCompactProgress);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalModifyOp::RecalculateProgressDueToCompact(
    TReal32& aProgress,
    TReal32 aProgressStep)
    {
    aProgress = aProgress - aProgressStep * KPosLmLocalSubOpAlmost100Percent;
    }

