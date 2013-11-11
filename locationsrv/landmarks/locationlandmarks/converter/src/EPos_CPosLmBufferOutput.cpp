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
* Description: Buffer output for encoder.
*
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmBufferOutput.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmBufferOutput::CPosLmBufferOutput
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmBufferOutput::CPosLmBufferOutput()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmBufferOutput::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmBufferOutput::ConstructL(TInt anExpandSize)
    {
    BaseConstructL(iBufWriteStream);

    iBuffer = CBufSeg::NewL(anExpandSize);
    iOwnsBuffer = ETrue;

    iBufWriteStream.Open(*iBuffer);
    }

// -----------------------------------------------------------------------------
// CPosLmBufferOutput::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmBufferOutput* CPosLmBufferOutput::NewL(TInt anExpandSize)
    {
    CPosLmBufferOutput* self = new (ELeave) CPosLmBufferOutput;
    CleanupStack::PushL(self);
    self->ConstructL(anExpandSize);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmBufferOutput::~CPosLmBufferOutput()
    {
    iBufWriteStream.Release();

    if (iOwnsBuffer)
        {
        delete iBuffer;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmBufferOutput::CommitL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmBufferOutput::CommitL()
    {
    CPosLmOutput::CommitL();
    iBuffer->Compress();
    }

// -----------------------------------------------------------------------------
// CPosLmBufferOutput::CheckLowDiskL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmBufferOutput::CheckLowDiskL(
    const TDesC16& /*aDesc*/)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmBufferOutput::BufferLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CBufBase* CPosLmBufferOutput::BufferLC()
    {
    CBufBase* buf = iBuffer;
    CleanupStack::PushL(buf);
    iOwnsBuffer = EFalse;
    return buf;
    }

//  End of File
