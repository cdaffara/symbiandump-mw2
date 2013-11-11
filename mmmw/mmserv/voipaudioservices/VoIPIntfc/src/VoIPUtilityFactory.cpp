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
#include <voipdownlinkstream.h>
#include <voipdtmftoneplayer.h>
#include <voipuplinkstream.h>
#include <voiputilityfactory.h>
#include "VoIPAudioSession.h"
#include "VoIPQueueHandler.h"
#include "VoIPUtilityFactoryImpl.h"

// ---------------------------------------------------------------------------
// CVoIPUtilityFactory::CreateFactory
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPUtilityFactory::CreateFactory(
        CVoIPUtilityFactory*& aFactory)
    {
    TInt err = KErrNone;
    CVoIPUtilityFactory* factory = NULL;
    factory = new CVoIPUtilityFactory;

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
// CVoIPUtilityFactory::~CVoIPUtilityFactory
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPUtilityFactory::~CVoIPUtilityFactory()
    {
    delete iVoIPUtilityFactoryImpl;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactory::CVoIPUtilityFactory
// ---------------------------------------------------------------------------
//
CVoIPUtilityFactory::CVoIPUtilityFactory()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactory::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPUtilityFactory::ConstructL()
    {
    iVoIPUtilityFactoryImpl = NULL;
    iVoIPUtilityFactoryImpl = CVoIPUtilityFactoryImpl::NewL();
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactory::CreateDownlinkStream
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPUtilityFactory::CreateDownlinkStream(
        const TVersion aVersion, const TVoIPCallType aCallType,
        CVoIPAudioDownlinkStream*& aDnLink)
    {
    // Push Over Cellular (PoC) implementation will be added in the future.
    TRAPD(err, iVoIPUtilityFactoryImpl->CreateDownlinkStreamL(aVersion,
            aCallType, aDnLink));
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactory::CreateDownlinkStream
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPUtilityFactory::CreateDownlinkStream(
        const TVersion aVersion, CVoIPJitterBufferIntfc*& aJBIntfc,
        CVoIPAudioDownlinkStream*& aDnLink)
    {
    TRAPD(err, iVoIPUtilityFactoryImpl->CreateDownlinkStreamL(aVersion,
            aJBIntfc, aDnLink));
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactory::CreateUplinkStream
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPUtilityFactory::CreateUplinkStream(
        const TVersion aVersion, const TVoIPCallType aCallType,
        CVoIPAudioUplinkStream*& aUpLink)
    {
    //TODO: What should we do different for PoC call here?
    TRAPD(err, iVoIPUtilityFactoryImpl->CreateUplinkStreamL(aVersion,
            aCallType, aUpLink));
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactory::CreateDTMFTonePlayer
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPUtilityFactory::CreateDTMFTonePlayer(
        CDTMFTonePlayer*& aDtmfPlayer)
    {
    TRAPD(err, iVoIPUtilityFactoryImpl->CreateDTMFTonePlayerL(aDtmfPlayer));
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactory::CreateBuffer
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPUtilityFactory::CreateBuffer(CVoIPDataBuffer*& aBuffer,
        const CVoIPDataBuffer::TVoIPBufferType aType,
        const TVoIPCodecFormat aCodecFormat)
    {
    TRAPD(err, iVoIPUtilityFactoryImpl->CreateBufferL(aBuffer, aType,
            aCodecFormat));
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactory::GetSupportedDownlinkFormats
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPUtilityFactory::GetSupportedDownlinkFormats(
        RArray<TVoIPCodecFormat>& aFormats)
    {
    TInt err = iVoIPUtilityFactoryImpl->GetSupportedDownlinkFormats(aFormats);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactory::GetSupportedUplinkFormats
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPUtilityFactory::GetSupportedUplinkFormats(
        RArray<TVoIPCodecFormat>& aFormats)
    {
    TInt err = iVoIPUtilityFactoryImpl->GetSupportedUplinkFormats(aFormats);
    return err;
    }

// End of file
