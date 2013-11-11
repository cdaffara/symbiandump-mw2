/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  VOIP Audio Services
 *
 */

#include "VoIPDataBufferImpl.h"

// ---------------------------------------------------------------------------
// Contructor
// ---------------------------------------------------------------------------
//
CVoIPDataBuffer::CVoIPDataBuffer()
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPDataBuffer::~CVoIPDataBuffer()
    {
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CVoIPDataBuffer::ConstructL(CVoIPDataBufferImpl* aDataBufferImpl)
    {
    iVoIPDataBufferImpl = aDataBufferImpl;
    }

// ---------------------------------------------------------------------------
// CVoIPDataBuffer::GetPayloadPtr
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoIPDataBuffer::GetPayloadPtr(TPtr8& aPayloadPtr)
    {
    iVoIPDataBufferImpl->GetPayloadPtr(aPayloadPtr);
    }

// ---------------------------------------------------------------------------
// CVoIPDataBuffer::SetPayloadPtr
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoIPDataBuffer::SetPayloadPtr(TPtr8 aPayloadPtr)
    {
    iVoIPDataBufferImpl->SetPayloadPtr(aPayloadPtr);
    }

// ---------------------------------------------------------------------------
// CVoIPDataBuffer::GetBufferType
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoIPDataBuffer::GetBufferType(
        CVoIPDataBuffer::TVoIPBufferType& aType)
    {
    iVoIPDataBufferImpl->GetBufferType(aType);
    }

// End of file
