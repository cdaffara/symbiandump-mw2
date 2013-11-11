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
 * This file defines the interface to the class that provides the
 * implementation logic of the STS API.
 */
#ifndef STSIMPLEMENTATION_H_
#define STSIMPLEMENTATION_H_

#include <systemtoneservice.h>

class RStsSession;

NONSHARABLE_CLASS(CStsImplementation) : public CBase
    {
public:
    static CStsImplementation* Create();
    virtual ~CStsImplementation();
public:
    void PlayTone(CSystemToneService::TToneType aTone);
    void PlayAlarm(CSystemToneService::TAlarmType aAlarm,
            unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver);
    void PlayAlarm(CSystemToneService::TToneType aTone,
            unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver);
    void StopAlarm(unsigned int aAlarmContext);
private:
    CStsImplementation();
    bool Init();

    RStsSession* iSession;
    };

#endif /* STSIMPLEMENTATION_H_ */
