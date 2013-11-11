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
* Description: A converter representation of the operation class. It provides an interface
*  for operations that are performed in small synchronous steps.
*
*
*/


// INCLUDE FILES
#include    <EPos_Landmarks.h>
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmConverterOperation.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmConverterOperation::CPosLmConverterOperation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmConverterOperation::CPosLmConverterOperation() :
    CPosLmOperation(),
    iOperationStatus(EOperationNotStarted)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmConverterOperation::BaseConstructL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmConverterOperation::BaseConstructL()
    {
    }

// Destructor
CPosLmConverterOperation::~CPosLmConverterOperation()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmConverterOperation::NextStep
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmConverterOperation::NextStep(
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
// CPosLmConverterOperation::ExecuteL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmConverterOperation::ExecuteL()
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
            User::Leave(err);
            }

        }

    iOperationStatus = EOperationCompleted;
    HandleOperationCompleted();
    }

// -----------------------------------------------------------------------------
// CPosLmConverterOperation::HandleOperationCompleted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmConverterOperation::HandleOperationCompleted()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmConverterOperation::Progress
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 CPosLmConverterOperation::Progress()
    {
    return iProgress;
    }

//  End of File
