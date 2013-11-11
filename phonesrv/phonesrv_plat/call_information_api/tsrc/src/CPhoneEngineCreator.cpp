/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/
#include "CPhoneEngineCreator.h"

#include <TelephonyAudioRoutingManager.h>
#include <cpephonemodelif.h>
#include "CPEMessageWaiter.h"


CPhoneEngineCreator* CPhoneEngineCreator::NewL()
    {
    CPhoneEngineCreator* self = new( ELeave )CPhoneEngineCreator();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
CPhoneEngineCreator::~CPhoneEngineCreator()
    {
    delete iStubAudioFactory;
    
    }
    
CPhoneEngineCreator::CPhoneEngineCreator() 
    {
    }
    
void CPhoneEngineCreator::ConstructL()
    {
    iStubAudioFactory = CStubAudioFactory::NewL();
    }
    
MPEPhoneModel* CPhoneEngineCreator::CreatePhoneEngineL(
    CPEMessageWaiter& aMsgWaiter,
    TBool aReturnImmedialtely )
    {
    CPEPhoneModelIF* pEif = 
        CPEPhoneModelIF::CreateL( aMsgWaiter, *iStubAudioFactory );
    
    if ( !aReturnImmedialtely )
        {
        aMsgWaiter.WaitForMsg(
                MEngineMonitor::EPEMessagePEConstructionReady,
                40 );
        }    

    return pEif;
    }

CStubAudioFactory* CPhoneEngineCreator::AudioFactory() const
    {
    return iStubAudioFactory;
    }

