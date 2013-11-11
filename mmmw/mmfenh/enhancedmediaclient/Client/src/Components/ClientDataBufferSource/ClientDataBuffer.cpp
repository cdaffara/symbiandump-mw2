/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of the ClientDataBuffer class.
*
*/


#include "ClientDataBuffer.h"

using namespace multimedia;

// ---------------------------------------------------------------------------
// Contructor
// ---------------------------------------------------------------------------
CClientDataBuffer::CClientDataBuffer()
: iDataBufferDes(NULL,0),
iLastBuffer(EFalse)
    {
    }

// ---------------------------------------------------------------------------
// Second Phase Constructor
// ---------------------------------------------------------------------------

void CClientDataBuffer::ConstructL(TUint aBufferSize)
    {
    if ( aBufferSize == 0 )
        {
        User::Leave(KErrGeneral);
        }
    
    iDataBuffer = HBufC8::NewL(aBufferSize);
    iDataBufferDes.Set(iDataBuffer->Des());
    }

// ---------------------------------------------------------------------------
// NewL method
// ---------------------------------------------------------------------------
CClientDataBuffer* CClientDataBuffer::NewL(TUint aBufferSize)
    {
    CClientDataBuffer* self = new(ELeave) CClientDataBuffer();
    CleanupStack::PushL(self);
    self->ConstructL(aBufferSize);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
CClientDataBuffer::~CClientDataBuffer()
    {
    delete iDataBuffer;
    }

// ---------------------------------------------------------------------------
// Returns the Pointer Descriptor for the Buffer
// ---------------------------------------------------------------------------
TPtr8& CClientDataBuffer::GetBufferPtr()
    {
    return iDataBufferDes;
    }

// ---------------------------------------------------------------------------
// Returns the Last Buffer Flag
// ---------------------------------------------------------------------------
TBool CClientDataBuffer::IsLastBuffer()
    {
    return iLastBuffer;
    }

// ---------------------------------------------------------------------------
// Sets the Last Buffer Flag
// ---------------------------------------------------------------------------
void CClientDataBuffer::SetLastBuffer(TBool aStatus)
    {
    iLastBuffer = aStatus;
    }

// End of file
