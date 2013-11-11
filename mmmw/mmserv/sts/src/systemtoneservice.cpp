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
 * This file is the direct implementation of the System Tone Service
 * API.  This acts as a facade to the actual implementation logic
 * which is in the CStsImplementation class.
 */

//  Include Files  
#include <systemtoneservice.h>	// CSystemToneService
#include "stsimplementation.h"
#include "stsdebug.h"

//  Member Functions
EXPORT_C /*static*/CSystemToneService* CSystemToneService::Create()
    {
    TRACE_FUNCTION_ENTRY;
    CSystemToneService* self = 0;
    CStsImplementation* implementation = CStsImplementation::Create();
    if (implementation != 0)
        {
        self = new CSystemToneService(*implementation);
        if (self == 0)
            {
            TRACE_LOG(_L(" Unable to instantiate CSystemToneService"));
            delete implementation;
            }
        }
    TRACE_FUNCTION_EXIT;
    return self;
    }

EXPORT_C /*static*/void CSystemToneService::Delete(
        CSystemToneService* aSystemToneService)
    {
    TRACE_FUNCTION_ENTRY;
    delete &(aSystemToneService->iImplementation);
    delete aSystemToneService;
    TRACE_FUNCTION_EXIT;
    }

CSystemToneService::CSystemToneService(CStsImplementation& aImplementation) :
    iImplementation(aImplementation)
    {
    TRACE_FUNCTION_ENTRY_EXIT;
    }

CSystemToneService::~CSystemToneService()
    {
    TRACE_FUNCTION_ENTRY_EXIT;
    }

EXPORT_C void CSystemToneService::PlayTone(TToneType aTone)
    {
    TRACE_FUNCTION_ENTRY;
    TRACE_LOG((_L("TToneType[0x%x]"), aTone));
    iImplementation.PlayTone(aTone);
    TRACE_FUNCTION_EXIT;
    }

EXPORT_C void CSystemToneService::PlayAlarm(TAlarmType aAlarm,
        unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver)
    {
    TRACE_FUNCTION_ENTRY;
    TRACE_LOG((_L("TAlarmType[0x%x]"), aAlarm));
    iImplementation.PlayAlarm(aAlarm, aAlarmContext, aObserver);
    TRACE_LOG((_L("AlarmContext[%u]"), aAlarmContext));
    TRACE_FUNCTION_EXIT;
    }

EXPORT_C void CSystemToneService::PlayAlarm(TToneType aTone,
        unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver)
    {
    TRACE_FUNCTION_ENTRY;
    TRACE_LOG((_L("TToneType[0x%x]"), aTone));
    iImplementation.PlayAlarm(aTone, aAlarmContext, aObserver);
    TRACE_LOG((_L("AlarmContext[%u]"), aAlarmContext));
    TRACE_FUNCTION_EXIT;
    }

EXPORT_C void CSystemToneService::StopAlarm(unsigned int aAlarmContext)
    {
    TRACE_FUNCTION_ENTRY;
    TRACE_LOG((_L("AlarmContext[%u]"), aAlarmContext));
    iImplementation.StopAlarm(aAlarmContext);
    TRACE_FUNCTION_EXIT;
    }
