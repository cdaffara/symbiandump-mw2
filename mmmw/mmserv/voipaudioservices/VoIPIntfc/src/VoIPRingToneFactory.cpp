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
 * Description:  VOIP Audio Services
 *
 */

#include <e32base.h>
#include <voipringtoneplayer.h>
#include <voipringtonefactory.h>
#include "VoIPQueueHandler.h"
#include "VoIPRingToneFactoryImpl.h"

// ---------------------------------------------------------------------------
// CRingToneFactory::CreateFactory
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRingToneFactory::CreateFactory(CRingToneFactory*& aFactory)
    {
    TInt err = KErrNone;
    CRingToneFactory* factory = NULL;
    factory = new CRingToneFactory;

    if (factory)
        {
        TRAP(err, factory->ConstructL());
        if (err != KErrNone)
            {
            delete factory;
            factory = NULL;
            }
        }

    aFactory = factory;
    return err;
    }

// ---------------------------------------------------------------------------
// CRingToneFactory::~CRingToneFactory
// ---------------------------------------------------------------------------
//
EXPORT_C CRingToneFactory::~CRingToneFactory()
    {
    delete iRingToneFactoryImpl;
    }

// ---------------------------------------------------------------------------
// CRingToneFactory::CRingToneFactory
// ---------------------------------------------------------------------------
//
CRingToneFactory::CRingToneFactory()
    {
    }

// ---------------------------------------------------------------------------
// CRingToneFactory::ConstructL
// ---------------------------------------------------------------------------
//
void CRingToneFactory::ConstructL()
    {
    iRingToneFactoryImpl = NULL;
    iRingToneFactoryImpl = CRingToneFactoryImpl::NewL();
    }

// ---------------------------------------------------------------------------
// CRingToneFactory::CreateRingTonePlayer
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRingToneFactory::CreateRingTonePlayer(
        CRingTonePlayer*& aRingTonePlayer)
    {
    TInt err = iRingToneFactoryImpl->CreateRingTonePlayer(aRingTonePlayer);
    return err;
    }

// End of file
