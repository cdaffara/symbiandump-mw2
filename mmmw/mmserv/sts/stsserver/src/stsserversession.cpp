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
 * This method provides the generic Symbian specific message handling
 * for the server session.  The actual STS specific processing occurs
 * in the STS class.
 */

//  Include Files  
#include "stsserversession.h"
#include "stsserver.h"
#include "sts.h"

// CStsServerSession IMPLEMENTATION
CStsServerSession::CStsServerSession(CStsServer& aServer, CSts& aSts) :
    iServer(aServer), iSts(aSts)
    {
    }

CStsServerSession::~CStsServerSession()
    {
    CleanUpObservers();
    TStsCallBack callBack;
    callBack.callBackType = EStsShutdown;
    iMsgQueue.SendBlocking(callBack);
    iServer.DropSession(this);
    iMsgQueue.Close();
    }

void CStsServerSession::CreateL()
    {
    iServer.AddSession(this);
    }

void CStsServerSession::ServiceL(const RMessage2& aMessage)
    {
    switch (aMessage.Function())
        {
        case StsMsg_RegisterMsgQueue:
            DoRegisterMsgQueueL(aMessage);
            break;
        case StsMsg_PlayTone:
            DoPlayToneL(aMessage);
            break;
        case StsMsg_PlayAlarm:
            DoPlayAlarmL(aMessage);
            break;
        case StsMsg_PlayToneAlarm:
            DoPlayToneAlarmL(aMessage);
            break;
        case StsMsg_StopAlarm:
            DoStopAlarmL(aMessage);
            break;
        default:
            break;
        }
    }

void CStsServerSession::ServiceError(const RMessage2& aMessage, TInt aError)
    {
    CSession2::ServiceError(aMessage, aError);
    }

void CStsServerSession::DoRegisterMsgQueueL(const RMessage2& aMessage)
    {
    TInt result = iMsgQueue.Open(aMessage, 0);
    if (result == KErrNone)
        {
        TThreadId id = RThread().Id();
        TPckg<TThreadId> idPckg(id);
        TRAP(result,aMessage.Write(1, idPckg));
        }
    aMessage.Complete(result);
    }

void CStsServerSession::DoPlayToneL(const RMessage2& aMessage)
    {
    CSystemToneService::TToneType tone =
            (CSystemToneService::TToneType) aMessage.Int0();
    aMessage.Complete(KErrNone);
    iSts.PlayTone(tone);
    }

void CStsServerSession::DoPlayAlarmL(const RMessage2& aMessage)
    {
    CSystemToneService::TAlarmType alarm =
            (CSystemToneService::TAlarmType) aMessage.Int0();
    unsigned int context = 0;
    iSts.PlayAlarm(alarm, context, *this);
    iObserverMap[context] = (MStsPlayAlarmObserver*) aMessage.Ptr2();
    TPckg<unsigned int> contextPckg(context);
    TRAPD(err,aMessage.WriteL(1,contextPckg));
    aMessage.Complete(err);
    // if the context is 0 there was a problem with the PlayAlarm,
    // so trigger the PlayAlarmComplete callback.
    if (context == 0)
        {
        PlayAlarmComplete(0);
        }
    }

void CStsServerSession::DoPlayToneAlarmL(const RMessage2& aMessage)
    {
    CSystemToneService::TToneType tone =
            (CSystemToneService::TToneType) aMessage.Int0();
    unsigned int context = 0;
    iSts.PlayAlarm(tone, context, *this);
    iObserverMap[context] = (MStsPlayAlarmObserver*) aMessage.Ptr2();
    TPckg<unsigned int> contextPckg(context);
    TRAPD(err,aMessage.WriteL(1,contextPckg));
    aMessage.Complete(err);
    // if the context is 0 there was a problem with the PlayAlarm,
    // so trigger the PlayAlarmComplete callback.
    if (context == 0)
        {
        PlayAlarmComplete(0);
        }
    }

void CStsServerSession::DoStopAlarmL(const RMessage2& aMessage)
    {
    unsigned int context = aMessage.Int0();
    aMessage.Complete(KErrNone);
    iObserverMap.erase(context);
    iSts.StopAlarm(context);
    }

void CStsServerSession::CleanUpObservers()
    {
    while (!iObserverMap.empty())
        {
        //TODO: Add trace here
        unsigned int context = iObserverMap.begin()->first;
        iObserverMap.erase(context);
        iSts.StopAlarm(context);
        }
    }

void CStsServerSession::PlayAlarmComplete(unsigned int aAlarmContext)
    {
    TStsCallBack callBack =
        {
        EStsPlayAlarmComplete, iObserverMap[aAlarmContext], aAlarmContext
        };
    iMsgQueue.SendBlocking(callBack);
    iObserverMap.erase(aAlarmContext);
    }
