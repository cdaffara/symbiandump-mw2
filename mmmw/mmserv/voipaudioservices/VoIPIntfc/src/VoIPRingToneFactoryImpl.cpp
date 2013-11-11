/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: VOIP Audio Service
 *
 */

#include <e32base.h>
#include "VoIPRingTonePlayerImpl.h"
#include "VoIPRingToneFactoryImpl.h"

// ---------------------------------------------------------------------------
// CRingToneFactoryImpl::NewL
// ---------------------------------------------------------------------------
//
CRingToneFactoryImpl* CRingToneFactoryImpl::NewL()
    {
    CRingToneFactoryImpl* self = new (ELeave) CRingToneFactoryImpl();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CRingToneFactoryImpl::~CRingToneFactoryImpl
// ---------------------------------------------------------------------------
//
CRingToneFactoryImpl::~CRingToneFactoryImpl()
    {
    }

// ---------------------------------------------------------------------------
// CRingToneFactoryImpl::CRingToneFactoryImpl
// ---------------------------------------------------------------------------
//
CRingToneFactoryImpl::CRingToneFactoryImpl()
    {
    }

// ---------------------------------------------------------------------------
// CRingToneFactoryImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CRingToneFactoryImpl::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CRingToneFactoryImpl::CreateRingTonePlayer
// ---------------------------------------------------------------------------
//
TInt CRingToneFactoryImpl::CreateRingTonePlayer(
        CRingTonePlayer*& aRingTonePlayer)
    {
    TRAPD(err, aRingTonePlayer = CRingTonePlayerImpl::NewL());
    return err;
    }

// End of file
