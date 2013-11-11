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
 * This file defines the interface to the client side session of the
 * STS Server.
 */
#ifndef RSTSSESSION_H_
#define RSTSSESSION_H_

#include <e32msgqueue.h>
#include <map>
#include <systemtoneservice.h>

#include "stsclientservercommon.h"

class RStsSession : public RSessionBase
    {
public:

    TInt Connect();

    void Close();

    void SendPlayTone(CSystemToneService::TToneType aTone);

    void SendPlayAlarm(CSystemToneService::TAlarmType aAlarm,
            unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver);

    void SendPlayToneAlarm(CSystemToneService::TToneType aTone,
            unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver);

    void SendStopAlarm(unsigned int aAlarmContext);

private:

    static TInt CallBackThreadMain(TAny* aSession);
    void RunThreadL();

    TInt CreateServerSession();
    void HandleMessage(TStsCallBack& aMessage);
    TInt StartServer();
    TInt StartThread();
    void CleanUpObservers();
    void SignalObservers();

    RThread iThread;
    RMsgQueue<TStsCallBack> iMsgQueue;
    TThreadId iServerThreadId;

    enum TState
        {
        EInitializing, ERunning, EStopping
        };
    TState iState;
    typedef std::map<unsigned int, MStsPlayAlarmObserver*> TObserverMap;
    TObserverMap iObserverMap;
    RMutex iObserverMutex;
    };

#endif // RSTSSESSION_H_
