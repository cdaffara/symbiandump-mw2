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
// CVoIPJBDataBufferImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPJBDataBufferImpl* CVoIPJBDataBufferImpl::NewL(TInt aBufferLen)
    {
    CVoIPJBDataBufferImpl* self = new (ELeave) CVoIPJBDataBufferImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aBufferLen);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPJBDataBufferImpl::~CVoIPJBDataBufferImpl
// ---------------------------------------------------------------------------
//
CVoIPJBDataBufferImpl::~CVoIPJBDataBufferImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPJBDataBufferImpl::CVoIPJBDataBufferImpl
// ---------------------------------------------------------------------------
//
CVoIPJBDataBufferImpl::CVoIPJBDataBufferImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPJBDataBufferImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPJBDataBufferImpl::ConstructL(TInt aBufferLen)
    {
    CVoIPDataBufferImpl::ConstructL(aBufferLen);
    CVoIPJBDataBuffer::ConstructL(this);
    iType = CVoIPJBDataBuffer::EJitterBuffer;
    }

// ---------------------------------------------------------------------------
// CVoIPJBDataBufferImpl::SetBufferSequence
// ---------------------------------------------------------------------------
//
void CVoIPJBDataBufferImpl::SetBufferSequence(const TUint aBufferSeq)
    {
    iBufferSequence = aBufferSeq;
    }

// ---------------------------------------------------------------------------
// CVoIPJBDataBufferImpl::GetBufferSequence
// ---------------------------------------------------------------------------
//
void CVoIPJBDataBufferImpl::GetBufferSequence(TUint& aBufferSeq)
    {
    aBufferSeq = iBufferSequence;
    }

// End of file

