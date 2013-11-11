/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Operation for finalizing URL encoder.
*
*
*/


// INCLUDE FILES
#include    "epos_cposlmurlencoderoperation.h"
#include    "EPos_CPosLmOutput.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmUrlEncoderOperation::CPosLmUrlEncoderOperation
// C++ default constructor 
// -----------------------------------------------------------------------------
//
CPosLmUrlEncoderOperation::CPosLmUrlEncoderOperation(
    CPosLmOutput* aEncoderOutput)
    : CPosLmConverterOperation(),
    iEncoderOutput(aEncoderOutput)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoderOperation::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoderOperation::ConstructL()
    {
    BaseConstructL();
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoderOperation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmUrlEncoderOperation* CPosLmUrlEncoderOperation::NewL(
    CPosLmOutput* aEncoderOutput)
    {
    CPosLmUrlEncoderOperation* self =
        new (ELeave) CPosLmUrlEncoderOperation(aEncoderOutput);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoderOperation::~CPosLmUrlEncoderOperation
// Destructor
// -----------------------------------------------------------------------------

CPosLmUrlEncoderOperation::~CPosLmUrlEncoderOperation()
    {
    delete iEncoderOutput;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoderOperation::NextStepL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmUrlEncoderOperation::NextStepL(
    TReal32& /*aProgress*/)
    {
    iEncoderOutput->CommitL();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoderOperation::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoderOperation::HandleError(
    TInt& /*aError*/)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoderOperation::HandleOperationCompleted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoderOperation::HandleOperationCompleted()
    {
    delete iEncoderOutput;
    iEncoderOutput = NULL;
    }

//  End of File
