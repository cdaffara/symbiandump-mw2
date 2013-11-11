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
* Description: Enhance audio player utility
*
*/



// INCLUDE FILES
#ifdef _DEBUG
#include 	<e32svr.h>
#endif

#include "S60ClientAudioBuffer.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CClientAudioBuffer::CClientAudioBuffer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CClientAudioBuffer::CClientAudioBuffer()
    : iAudioBufferDes(NULL,0),
      iLastBuffer(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// CClientAudioBuffer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CClientAudioBuffer::ConstructL(TInt aBufferSize)
    {
	iAudioBuffer = HBufC8::NewL(aBufferSize);
	iAudioBufferDes.Set(iAudioBuffer->Des());
#ifdef _DEBUG
	RDebug::Print(_L("CClientAudioBuffer::ConstructL: Length=%d, MaxLength=%d"), iAudioBufferDes.Length(), iAudioBufferDes.MaxLength());
#endif
	}

// -----------------------------------------------------------------------------
// CClientAudioBuffer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CClientAudioBuffer* CClientAudioBuffer::NewL(TInt aBufferSize)
    {
    CClientAudioBuffer* self = new(ELeave) CClientAudioBuffer();
    CleanupStack::PushL(self);
    self->ConstructL(aBufferSize);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CClientAudioBuffer::GetBufferPtr
// Gets the Buffer Ptr to the Client Buffer
// -----------------------------------------------------------------------------
//
EXPORT_C TPtr8& CClientAudioBuffer::GetBufferPtr()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CClientAudioBuffer::GetBufferPtr"));
#endif
		return iAudioBufferDes;
	}

// -----------------------------------------------------------------------------
// CClientAudioBuffer::IsLastBuffer
// Gets the Last Buffer Status
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CClientAudioBuffer::IsLastBuffer()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CClientAudioBuffer::IsLastBuffer"));
#endif
		return iLastBuffer;
	}


// -----------------------------------------------------------------------------
// CClientAudioBuffer::SetLastBuffer
// Sets the Last Buffer Status
// -----------------------------------------------------------------------------
//
EXPORT_C void CClientAudioBuffer::SetLastBuffer(TBool aStatus)
	{
#ifdef _DEBUG
    RDebug::Print(_L("CClientAudioBuffer::SetLastBuffer"));
#endif
		iLastBuffer = aStatus;
	}


// -----------------------------------------------------------------------------
// CClientAudioBuffer::~CClientAudioBuffer
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CClientAudioBuffer::~CClientAudioBuffer()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CClientAudioBuffer::~CClientAudioBuffer"));
#endif
    delete iAudioBuffer;
    }




// End of file

