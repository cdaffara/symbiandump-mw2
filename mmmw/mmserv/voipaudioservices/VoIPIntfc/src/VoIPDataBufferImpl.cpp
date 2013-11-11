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

#include "VoIPSharedData.h"
#include "VoIPDataBufferImpl.h"

// -----------------------------------------------------------------------------
// CVoIPDataBufferImpl::NewL
// -----------------------------------------------------------------------------
//
CVoIPDataBufferImpl* CVoIPDataBufferImpl::NewL(TInt aBufferLen)
    {
    CVoIPDataBufferImpl* self = new (ELeave) CVoIPDataBufferImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aBufferLen);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CVoIPDataBufferImpl::CVoIPDataBufferImpl
// -----------------------------------------------------------------------------
//
CVoIPDataBufferImpl::CVoIPDataBufferImpl() :
    iPayloadPtr(0, 0, 0)
    {
    iType = CVoIPDataBuffer::EStandard;
    }

// -----------------------------------------------------------------------------
// CVoIPDataBufferImpl::~CVoIPDataBufferImpl
// -----------------------------------------------------------------------------
//
CVoIPDataBufferImpl::~CVoIPDataBufferImpl()
    {
    delete iBuf;
    }

// -----------------------------------------------------------------------------
// CVoIPDataBufferImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CVoIPDataBufferImpl::ConstructL(TInt aBufferLen)
    {
    CVoIPDataBuffer::ConstructL(this);
    iBuf = HBufC8::New(aBufferLen); //non-leaving?
    iPayloadPtr.Set(iBuf->Des());
    }

// -----------------------------------------------------------------------------
// CVoIPDataBufferImpl::GetPayloadPtr
// -----------------------------------------------------------------------------
//
void CVoIPDataBufferImpl::GetPayloadPtr(TPtr8& aPayloadPtr)
    {
    aPayloadPtr.Set(iPayloadPtr);
    }

// -----------------------------------------------------------------------------
// CVoIPDataBufferImpl::SetBufferType
// -----------------------------------------------------------------------------
//
void CVoIPDataBufferImpl::SetPayloadPtr(TPtr8 aPayloadPtr)
    {
    iPayloadPtr.Set(aPayloadPtr);
    }

// -----------------------------------------------------------------------------
// CVoIPDataBufferImpl::GetBufferType
// -----------------------------------------------------------------------------
//
void CVoIPDataBufferImpl::GetBufferType(
        CVoIPDataBuffer::TVoIPBufferType& aType)
    {
    aType = iType;
    }

// End of file
