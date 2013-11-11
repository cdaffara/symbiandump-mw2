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

#include "VoIPJBDataBufferImpl.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CVoIPJBDataBuffer::CVoIPJBDataBuffer()
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPJBDataBuffer::~CVoIPJBDataBuffer()
    {
    }

// ---------------------------------------------------------------------------
// 2-nd phase constructor
// ---------------------------------------------------------------------------
//
void CVoIPJBDataBuffer::ConstructL(CVoIPJBDataBufferImpl* aJBDataBufferImpl)
    {
    iVoIPJBDataBufferImpl = aJBDataBufferImpl;
    }

// ---------------------------------------------------------------------------
// CVoIPJBDataBuffer::SetBufferSequence
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoIPJBDataBuffer::SetBufferSequence(const TUint aBufferSeq)
    {
    iVoIPJBDataBufferImpl->SetBufferSequence(aBufferSeq);
    }

// ---------------------------------------------------------------------------
// CVoIPJBDataBuffer::GetBufferSequence
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoIPJBDataBuffer::GetBufferSequence(TUint& aBufferSeq)
    {
    iVoIPJBDataBufferImpl->GetBufferSequence(aBufferSeq);
    }

// End of file
