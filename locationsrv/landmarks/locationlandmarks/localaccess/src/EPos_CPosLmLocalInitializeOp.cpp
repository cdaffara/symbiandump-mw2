/*
* Copyright (c) 2002-2005,2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Operation for initializing a landmark database.
*
*
*/


#include    <EPos_LandmarksErrors.h>
#include    <epos_poslmservercommon.h>
#include    "EPos_CPosLmLocalInitializeOp.h"
#include    "epos_cposlmlocaldatabase.h"
#include    "epos_cposlmlocaldbaccess.h"
#include    "EPos_RPosLmLocalAccessSubsession.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalInitializeOp::CPosLmLocalInitializeOp(
    CPosLmLocalDatabase& aDb) :
    CPosLmOperation(),
    iDb(&aDb),
    iStatusPckg(KPosLmOperationNotComplete)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalInitializeOp::ConstructL()
    {
    iInitFlag = iDb->DatabaseAccess()->InitFlagPtr();
    iInitFlagPckg = new (ELeave) TPckg<TUint>(*iInitFlag);

    if ( !iDb->IsInitializingNeeded() )
        {
        // Database does not need to be initialized.
        iInitNotNeeded = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalInitializeOp* CPosLmLocalInitializeOp::NewL(
    CPosLmLocalDatabase& aDb)
    {
    CPosLmLocalInitializeOp* self = new(ELeave) CPosLmLocalInitializeOp(aDb);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalInitializeOp::~CPosLmLocalInitializeOp()
    {
    if (iDb)
        {
        iDb->Session().CancelInitializeDbAsync(iInitType);
        }

    delete iProgressPckg;
    delete iInitFlagPckg;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalInitializeOp::NextStep(
    TRequestStatus& aStatus,
    TReal32& aProgress)
    {
    if (iOperationStatus == ESyncStarted || iStatusPckg() == KRequestPending)
        {
        Panic(KPosLandmarksClientPanic, EPosInvalidOperationMode);
        }

    iOperationStatus = EAsyncStarted;

    if (iInitNotNeeded)
        {
        aProgress = 1; // Operation done
        CompleteRequest(aStatus, KErrNone);
        }
    else if (iStatusPckg() != KPosLmOperationNotComplete)
        {
        CompleteRequest(aStatus, iStatusPckg());
        }
    else
        {
        iStatusPckg = KRequestPending;

        if (iProgressPckg)
            {
            aProgress = (*iProgressPckg)();
            delete iProgressPckg;
            iProgressPckg = NULL;
            }

        iProgressPckg = new TPckg<TReal32>(aProgress);
        if (!iProgressPckg)
            {
            CompleteRequest(aStatus, KErrNoMemory);
            }
        else
            {
            iDb->Session().InitializeDbAsync(iInitType,
                &iStatusPckg, iProgressPckg, iInitFlagPckg, aStatus);
            }
        }
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
void CPosLmLocalInitializeOp::ExecuteL()
    {
    if (iOperationStatus != EOperationNotStarted)
        {
        Panic(KPosLandmarksClientPanic, EPosInvalidOperationMode);
        }

    iOperationStatus = ESyncStarted;

    if (!iInitNotNeeded)
        {
        User::LeaveIfError(iDb->Session().InitializeDbSync(iInitType));
        }

    // If the initialization has completed the init flag must be resetted.
    iDb->DatabaseAccess()->ResetInitFlag();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalInitializeOp::CompleteRequest(
    TRequestStatus& aStatus,
    TInt aReason)
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete(status, aReason);
    }

