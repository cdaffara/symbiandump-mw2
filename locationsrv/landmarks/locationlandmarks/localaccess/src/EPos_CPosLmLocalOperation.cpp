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
* Description: A local representation of the operation class.
*
*
*/


// INCLUDE FILES
#include    <EPos_Landmarks.h>
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmLocalOperation.h"
#include    "EPos_TPosOperationStatus.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPosLmLocalOperation::CPosLmLocalOperation(CPosLmLocalDatabase& aDb) :
    CPosLmOperation(),
    iDb(&aDb),
    iOperationStatus(EOperationNotStarted)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalOperation::BaseConstructL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalOperation::BaseConstructL()
    {
    }

// Destructor
EXPORT_C CPosLmLocalOperation::~CPosLmLocalOperation()
    {
    if (iHasDbLock)
        {
        iDb->ReleaseLock(iLockType);
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalOperation::NextStep
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalOperation::NextStep(
    TRequestStatus& aStatus,
    TReal32& aProgress)
    {
    if (iOperationStatus == ESyncStarted ||
        iOperationStatus == EOperationCompleted)
        {
        Panic(KPosLandmarksClientPanic, EPosInvalidOperationMode);
        }

    iOperationStatus = EAsyncStarted;
    aStatus = KRequestPending;

    TInt status = KPosLmOperationNotComplete;

    aProgress = iProgress;
    TRAPD(err, status = NextStepL(aProgress));
    iProgress = aProgress;
    if (err != KErrNone)
        {
        status = err;
        iOperationStatus = EOperationCompleted;
        HandleError(err);
        }
    if (status == KErrNone)
        {
        iProgress = 1.0;
        aProgress = iProgress;
        iOperationStatus = EOperationCompleted;
        HandleOperationCompleted();
        }

    TRequestStatus* clientStatus = &aStatus;
    User::RequestComplete(clientStatus, status);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalOperation::ExecuteL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalOperation::ExecuteL()
    {
    if (iOperationStatus != EOperationNotStarted)
        {
        Panic(KPosLandmarksClientPanic, EPosInvalidOperationMode);
        }

    iOperationStatus = ESyncStarted;

    TInt status = KPosLmOperationNotComplete;

    while (status != KErrNone)
        {
        TRAPD(err, status = NextStepL(iProgress));

        __ASSERT_DEBUG(err != KErrNone || err == KErrNone &&
            (status == KErrNone || status == KPosLmOperationNotComplete),
            Panic(KPosLandmarksClientPanic, EPosInvalidOperationMode));

        if (err != KErrNone)
            {
            HandleError(err);
            User::LeaveIfError(err);
            }

        }

    iOperationStatus = EOperationCompleted;
    HandleOperationCompleted();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalOperation::HandleOperationCompleted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalOperation::HandleOperationCompleted()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalOperation::Progress
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 CPosLmLocalOperation::Progress()
    {
    return iProgress;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalOperation::AquireLockL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalOperation::AquireLockL(
    CPosLmLocalDatabase::TLockType aLockType)
    {
    iDb->AquireLockL(aLockType);
    iHasDbLock = ETrue;
    iLockType = aLockType;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalOperation::ReleaseLock
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmLocalOperation::ReleaseLock()
    {
    if (iHasDbLock)
        {
        TInt status = iDb->ReleaseLock(iLockType);
        if (status == KErrNone)
            {
            iHasDbLock = EFalse;
            }
        return status;
        }
    return KErrNone;
    }

//  End of File
