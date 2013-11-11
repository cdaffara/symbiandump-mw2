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
* Description: Implementation of Active object wrapping an async operation.
*
*
*/


// INCLUDE FILES
#include    <EPos_CPosLmOperation.h>
#include    "EPos_CPosLmActiveOperation.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmActiveOperation::CPosLmActiveOperation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmActiveOperation::CPosLmActiveOperation(
    CPosLmOperation* aOperation,
    TUint aDatabaseIndex,
    TAny* aOwner)
:   CActive(EPriorityNormal),
    iOperation(aOperation),
    iDatabaseIndex(aDatabaseIndex),
    iOwner(aOwner)
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CPosLmActiveOperation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmActiveOperation* CPosLmActiveOperation::NewL(
    CPosLmOperation* aOperation,
    TUint aDatabaseIndex,
    TAny* aOwner)
    {
    CPosLmActiveOperation* self = new (ELeave) CPosLmActiveOperation(
        aOperation, aDatabaseIndex, aOwner);
    return self;
    }

// Destructor
CPosLmActiveOperation::~CPosLmActiveOperation()
    {
    Cancel();
    delete iOperation;
    }

// -----------------------------------------------------------------------------
// CPosLmActiveOperation::DoCancel
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmActiveOperation::DoCancel()
    {
    delete iOperation;
    iOperation = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmActiveOperation::RunL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmActiveOperation::RunL()
    {
    iCallBack.CallBack();
    }

// -----------------------------------------------------------------------------
// CPosLmActiveOperation::ExecuteStep
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmActiveOperation::ExecuteStep(TCallBack callBack)
    {
    iCallBack = callBack;
    iOperation->NextStep(iStatus, iProgress);
    SetActive();
    }

// -----------------------------------------------------------------------------
// CPosLmActiveOperation::DatabaseIndex
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint CPosLmActiveOperation::DatabaseIndex()
    {
    return iDatabaseIndex;
    }

// -----------------------------------------------------------------------------
// CPosLmActiveOperation::Owner
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TAny* CPosLmActiveOperation::Owner()
    {
    return iOwner;
    }

// -----------------------------------------------------------------------------
// CPosLmActiveOperation::Status
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmActiveOperation::Status()
    {
    return iStatus.Int();
    }

// -----------------------------------------------------------------------------
// CPosLmActiveOperation::Progress
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 CPosLmActiveOperation::Progress()
    {
    return iProgress;
    }

//  End of File
