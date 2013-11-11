/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * The file provides the actual implementation of the STS API.
 * All of the STS API methods are implemented by passing the
 * calls to the STS Server through the client side STS
 * Session.
 */

//  Include Files  
#include "stsimplementation.h"
#include "rstssession.h"

//  Member Functions
/*static*/CStsImplementation* CStsImplementation::Create()
    {
    CStsImplementation* self = new CStsImplementation();
    if (self != 0)
        {
        bool successful = self->Init();
        if (!successful)
            {
            delete self;
            self = 0;
            }
        }
    return self;
    }

CStsImplementation::CStsImplementation() :
    iSession(NULL)
    {
    }

CStsImplementation::~CStsImplementation()
    {
    iSession->Close();
    delete iSession;
    }

bool CStsImplementation::Init()
    {
    bool result = false;
    iSession = new RStsSession();
    if (iSession)
        {
        TInt err = iSession->Connect();
        result = err == KErrNone;
        }
    return result;
    }

void CStsImplementation::PlayTone(CSystemToneService::TToneType aTone)
    {
    iSession->SendPlayTone(aTone);
    }

void CStsImplementation::PlayAlarm(CSystemToneService::TAlarmType aAlarm,
        unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver)
    {
    //TODO: Add logging and error checking
    iSession->SendPlayAlarm(aAlarm, aAlarmContext, aObserver);
    }

void CStsImplementation::PlayAlarm(CSystemToneService::TToneType aTone,
        unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver)
    {
    iSession->SendPlayToneAlarm(aTone, aAlarmContext, aObserver);
    }

void CStsImplementation::StopAlarm(unsigned int aAlarmContext)
    {
    //TODO: Add logging and error checking
    iSession->SendStopAlarm(aAlarmContext);
    }
