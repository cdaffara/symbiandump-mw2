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
 * This file defines an interface to an System Tone Service used by the
 * STS Server for playing and stopping tones.  Only one instance of this
 * class is created by the server, and it exists for the lifetime of the
 * server.  This class manages all of the active players, and guarantees
 * that the context values are unique.
 */
#ifndef STS_H_
#define STS_H_

#include <systemtoneservice.h>
#include <map>

#include "stsplayer.h"

class CSts : private MStsPlayerObserver
    {
public:
    static CSts* Create();
    static void Delete(CSts* aSts);
    void PlayTone(CSystemToneService::TToneType aTone);
    void PlayAlarm(CSystemToneService::TAlarmType aAlarm,
            unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver);
    void PlayAlarm(CSystemToneService::TToneType aTone,
            unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver);
    void StopAlarm(unsigned int aAlarmContext);

protected:
    CSts();
    virtual ~CSts();
    bool Init();

private:
    void CleanUpPlayers();

    // inherited from MPlayerObserver
    virtual void PlayComplete(unsigned int aContext);

private:
    unsigned int iNextContext;
    CStsSettingsManager* iSettingsManager;
    class CPlayerNode;
    typedef std::map<unsigned int, CPlayerNode*> TPlayerMap;
    TPlayerMap iPlayerMap;
    };

#endif //STS_H
