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
* Description: Operation for finalizing an encoder.
*
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmXmlEncoderOperation.h"
#include    "EPos_CPosLmOutput.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmXmlEncoderOperation::CPosLmXmlEncoderOperation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmXmlEncoderOperation::CPosLmXmlEncoderOperation(
    CPosLmOutput* aEncoderOutput)
    : CPosLmConverterOperation(),
    iEncoderOutput(aEncoderOutput)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmXmlEncoderOperation::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoderOperation::ConstructL()
    {
    BaseConstructL();
    }

// -----------------------------------------------------------------------------
// CPosLmXmlEncoderOperation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmXmlEncoderOperation* CPosLmXmlEncoderOperation::NewL(
    CPosLmOutput* aEncoderOutput)
    {
    CPosLmXmlEncoderOperation* self =
        new (ELeave) CPosLmXmlEncoderOperation(aEncoderOutput);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmXmlEncoderOperation::~CPosLmXmlEncoderOperation()
    {
    delete iEncoderOutput;
    }

// -----------------------------------------------------------------------------
// CPosLmXmlEncoderOperation::NextStepL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmXmlEncoderOperation::NextStepL(
    TReal32& /*aProgress*/)
    {
    iEncoderOutput->CommitL();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPosLmXmlEncoderOperation::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoderOperation::HandleError(
    TInt& /*aError*/)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmXmlEncoderOperation::HandleOperationCompleted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoderOperation::HandleOperationCompleted()
    {
    delete iEncoderOutput;
    iEncoderOutput = NULL;
    }

//  End of File
