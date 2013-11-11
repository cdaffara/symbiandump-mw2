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
* Description: Active help object for running an internal operation.
*
*
*/


// INCLUDE FILES
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmOperation.h"
#include    "EPos_CPosLmLocalInternalOpActive.h"
#include    "EPos_MPosLmLocalInternalOpExecutor.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalInternalOpActive::CPosLmLocalInternalOpActive
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalInternalOpActive::CPosLmLocalInternalOpActive(
    MPosLmLocalInternalOpExecutor& aExecutor,
    CPosLmOperation* aOp) :
    CActive(EPriorityNormal),
    iExecutor(&aExecutor),
    iOp(aOp)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalInternalOpActive::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalInternalOpActive* CPosLmLocalInternalOpActive::NewL(
    MPosLmLocalInternalOpExecutor& aExecutor,
    CPosLmOperation* aOp)
    {
    CPosLmLocalInternalOpActive* self = new(ELeave) CPosLmLocalInternalOpActive(
        aExecutor, aOp);
    return self;
    }

// Destructor
CPosLmLocalInternalOpActive::~CPosLmLocalInternalOpActive()
    {
    Cancel();

    delete iOp;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalInternalOpActive::DoNext
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalInternalOpActive::DoNext(
    TRequestStatus& aClientStatus,
    TReal32& aClientProgress)
    {
    __ASSERT_ALWAYS(!IsActive(),
        Panic(KPosLandmarksClientPanic, EPosInvalidOperationMode));

    iClientStatus = &aClientStatus;
    iClientProgress = &aClientProgress;

    if (!IsAdded())
        {
        CActiveScheduler::Add(this);
        }

    InternalOpNextStep(iStatus, iProgress);
    SetActive();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalInternalOpActive::InternalOpNextStep
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalInternalOpActive::InternalOpNextStep(
    TRequestStatus& aStatus,
    TReal32& aProgress)
    {
    iOp->NextStep(aStatus, aProgress);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalInternalOpActive::DoCancel
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalInternalOpActive::DoCancel()
    {
    delete iOp; // Cancels outstanding request
    iOp = NULL;

    if (iClientStatus)
        {
        User::RequestComplete(iClientStatus, KErrCancel);
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalInternalOpActive::RunL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalInternalOpActive::RunL()
    {
    iExecutor->InternalOpCompleted(iStatus.Int(), iProgress, *iClientStatus,
        *iClientProgress);
    }

//  End of File
