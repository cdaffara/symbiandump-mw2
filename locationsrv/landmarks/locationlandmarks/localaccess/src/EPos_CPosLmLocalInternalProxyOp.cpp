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
* Description: A proxy operation between the client and another operation.
*
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmLocalInternalProxyOp.h"
#include    "EPos_MPosLmLocalInternalProxyOpClient.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPosLmLocalInternalProxyOp::CPosLmLocalInternalProxyOp(
    MPosLmLocalInternalProxyOpClient& aOpClient,
    CPosLmOperation* aOp) :
    CPosLmOperation(),
    iOpClient(&aOpClient),
    iOp(aOp)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalInternalProxyOp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalInternalProxyOp* CPosLmLocalInternalProxyOp::NewL(
    MPosLmLocalInternalProxyOpClient& aOpOwner,
    CPosLmOperation* aOp)
    {
    CPosLmLocalInternalProxyOp* self = new(ELeave) CPosLmLocalInternalProxyOp(
        aOpOwner, aOp);
    return self;
    }

// Destructor
CPosLmLocalInternalProxyOp::~CPosLmLocalInternalProxyOp()
    {
    delete iOp;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalInternalProxyOp::Operation
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalInternalProxyOp::Operation()
    {
    return iOp;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalInternalProxyOp::NextStep
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalInternalProxyOp::NextStep(
    TRequestStatus& aStatus,
    TReal32& aProgress)
    {
    iOpClient->DoNextStep(aStatus, aProgress);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalInternalProxyOp::ExecuteL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalInternalProxyOp::ExecuteL()
    {
    iOpClient->DoExecuteL();
    }

//  End of File
