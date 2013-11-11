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
 * This file defines the interface for the STS server side session.
 */
#ifndef STSSERVERSESSION_H_
#define STSSERVERSESSION_H_

#include <e32base.h>
#include <e32msgqueue.h>
#include <map>

#include "stsclientservercommon.h"

class CSts;
class CStsServer;
class CStsPlayAlarmObserver;

class CStsServerSession : public CSession2, private MStsPlayAlarmObserver
    {
public:

    CStsServerSession(CStsServer& aServer, CSts& aSts);

    virtual ~CStsServerSession();

    // inherited from CSession2
    void CreateL();
    void ServiceL(const RMessage2& aMessage);
    void ServiceError(const RMessage2& aMessage, TInt aError);

private:

    void DoRegisterMsgQueueL(const RMessage2& aMessage);
    void DoPlayToneL(const RMessage2& aMessage);
    void DoPlayAlarmL(const RMessage2& aMessage);
    void DoPlayToneAlarmL(const RMessage2& aMessage);
    void DoStopAlarmL(const RMessage2& aMessage);

    void CleanUpObservers();

    // inherited from MPlayAlarmObserver
    virtual void PlayAlarmComplete(unsigned int aAlarmContext);

    CStsServer& iServer;
    CSts& iSts;

    typedef std::map<unsigned int, MStsPlayAlarmObserver*> TObserverMap;
    TObserverMap iObserverMap;
    RMsgQueue<TStsCallBack> iMsgQueue;
    };

#endif // STSSERVERSESSION_H_
