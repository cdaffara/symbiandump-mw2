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
 * The file provides the implementation of the client side session
 * to the STS Server.
 */

//  Include Files  
#include "rstssession.h"
#include "stsclientservercommon.h"

const TUint KNumSlots = 30;

/*static*/TInt RStsSession::CallBackThreadMain(TAny* aSession)
    {
    TInt err = KErrNoMemory;

    RThread myThread;
    myThread.SetPriority(EPriorityAbsoluteHigh);
    myThread.Close();

    CTrapCleanup* cleanup = CTrapCleanup::New();

    if (cleanup)
        {
        // Run the server and request a thread rendezvous.
        TRAP( err, ((RStsSession*)aSession)->RunThreadL() );
        delete cleanup;
        }

    return err;
    }

void RStsSession::RunThreadL()
    {
    iState = ERunning;
    // Initialisation complete, now signal the client, if requested.
    RThread::Rendezvous(KErrNone);
    TRequestStatus queueStatus = KRequestPending;
    iMsgQueue.NotifyDataAvailable(queueStatus);

    RThread server;
    TInt err = server.Open(iServerThreadId);
    TRequestStatus serverStatus = KRequestPending;
    server.Logon(serverStatus);

    while (iState == ERunning)
        {
        TStsCallBack message;
        // Using ReceiveBlocking here would block forever if the executive thread
        // dies, so instead wait for either a data available notification or a
        // notification that the executive thread has died.
        User::WaitForRequest(queueStatus, serverStatus);

        if (queueStatus != KRequestPending)
            {
            TInt err = iMsgQueue.Receive(message);
            if (err == KErrNone)
                {
                HandleMessage(message);
                }
            else
                {
                //TODO:Log a message
                }
            queueStatus = KRequestPending;
            iMsgQueue.NotifyDataAvailable(queueStatus);
            }
        if (serverStatus != KRequestPending && iState == ERunning)
            {
            //TODO: Log a message
            //Restart the server
            SignalObservers();
            server.Close();
            CreateServerSession();
            TInt err = server.Open(iServerThreadId);
            TRequestStatus serverStatus = KRequestPending;
            server.Logon(serverStatus);
            }
        }

    iMsgQueue.CancelDataAvailable();
    server.LogonCancel(serverStatus);
    server.Close();
    }

void RStsSession::HandleMessage(TStsCallBack& aMessage)
    {
    bool signalObserver = false;
    TStsCallBackType type = aMessage.callBackType;
    if (type == EStsPlayAlarmComplete)
        {
        MStsPlayAlarmObserver* observer = aMessage.observer;
        unsigned int context = aMessage.alarmContext;
        iObserverMutex.Wait();
        if (observer == iObserverMap[context])
            {
            signalObserver = true;
            }
        else
            {
            //TODO: Log a message
            }
        iObserverMap.erase(context);
        iObserverMutex.Signal();
        if (signalObserver)
            {
            observer->PlayAlarmComplete(aMessage.alarmContext);
            }
        }
    else if (type == EStsShutdown)
        {
        iState = EStopping;
        }
    else
        {
        //TODO: Log error message
        }
    }

TInt RStsSession::StartServer()
    {
    TInt err = KErrNone;

    // Launch the server executable (i.e. in it its own process).

    // Create a new server process. Simultaneous launching of two such processes 
    // should be detected when the second one attempts to create the server 
    // object, failing with KErrAlreadyExists.
    RProcess server;
    err = server.Create(KStsServerFile, KNullDesC);

    if (err == KErrNone)
        {
        TRequestStatus rendezvousStatus;
        server.Rendezvous(rendezvousStatus);
        server.Resume();

        // wait for start or death
        User::WaitForRequest(rendezvousStatus);

        // we can't use the 'exit reason' if the server panicked as this
        // is the panic 'reason' and may be '0' which cannot be distinguished
        // from KErrNone  
        if (server.ExitType() == EExitPanic)
            {
            err = KErrGeneral;
            }
        else
            {
            err = rendezvousStatus.Int();
            }
        }
    server.Close();

    return err;
    }

TInt RStsSession::StartThread()
    {
    TInt result = iThread.Create(KNullDesC, RStsSession::CallBackThreadMain,
            KDefaultStackSize, &User::Heap(), (TAny*) this);
    if (result == KErrNone)
        {
        TRequestStatus rendezvousStatus = KRequestPending;

        //  Register for rendezvous notification when thread is started.
        iThread.Rendezvous(rendezvousStatus);

        // Start the thread execution
        iThread.Resume();

        // Wait for thread to start.
        User::WaitForRequest(rendezvousStatus);

        result = rendezvousStatus.Int();

        if (result != KErrNone)
            {
            iThread.Kill(result);
            }
        }
    return result;
    }

TInt RStsSession::CreateServerSession()
    {
    // Try to create a session with the server
    TInt result = CreateSession(KStsServerName, TVersion(
            KStsServerMajorVersion, KStsServerMinorVersion, KStsServerBuild),
            KNumSlots, EIpcSession_Sharable);

    // If the server wasn't found, start the server and try creating a session again
    if (result == KErrNotFound || result == KErrServerTerminated)
        {
        result = StartServer();
        if (result == KErrNone || result == KErrAlreadyExists)
            {
            result = CreateSession(KStsServerName, TVersion(
                    KStsServerMajorVersion, KStsServerMinorVersion,
                    KStsServerBuild), KNumSlots, EIpcSession_Sharable);
            }
        }
    if (result == KErrNone)
        {
        TPckg<TThreadId> idPckg(iServerThreadId);
        result = SendReceive(StsMsg_RegisterMsgQueue, TIpcArgs(iMsgQueue,
                &idPckg));
        }
    return result;
    }

TInt RStsSession::Connect()
    {
    iState = EInitializing;

    // Create a nameless global message queue, then pass the handle to the queue to the server.
    TInt result = iMsgQueue.CreateGlobal(KNullDesC, 30);

    // Create thread for receiving asynch callbacks from the server
    if (result == KErrNone)
        {
        result = CreateServerSession();
        if (result == KErrNone)
            {
            result = StartThread();
            if (result == KErrNone)
                {
                result = iObserverMutex.CreateLocal();
                }
            }
        }
    return result;
    }

void RStsSession::Close()
    {
    TRequestStatus logonStatus = KRequestPending;
    iThread.Logon(logonStatus);
    RSessionBase::Close();
    User::WaitForRequest(logonStatus);
    iThread.Close();
    iMsgQueue.Close();
    CleanUpObservers();
    iObserverMutex.Close();
    }

void RStsSession::SendPlayTone(CSystemToneService::TToneType aTone)
    {
    TInt err = SendReceive(StsMsg_PlayTone, TIpcArgs(aTone));
    if (err != KErrNone)
        {
        //TODO: Log a message
        }
    }

void RStsSession::SendPlayAlarm(CSystemToneService::TAlarmType aAlarm,
        unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver)
    {
    bool signalObserver = false;
    TPckg<unsigned int> alarmContextPckg(aAlarmContext);
    // Need to do the SendReceive and adding the observer to the
    // observer map in a critical section in case the playback
    // completes before this method has a chance to update the
    // observer list.
    iObserverMutex.Wait();
    TInt err = SendReceive(StsMsg_PlayAlarm, TIpcArgs(aAlarm,
            &alarmContextPckg, &aObserver));
    if (err != KErrNone)
        {
        //TODO: Log a message
        signalObserver = true;
        }
    else
        {
        iObserverMap[aAlarmContext] = &aObserver;
        }
    iObserverMutex.Signal();
    if (signalObserver)
        {
        aObserver.PlayAlarmComplete(aAlarmContext);
        }
    }

void RStsSession::SendPlayToneAlarm(CSystemToneService::TToneType aTone,
        unsigned int& aAlarmContext, MStsPlayAlarmObserver& aObserver)
    {
    bool signalObserver = false;
    TPckg<unsigned int> alarmContextPckg(aAlarmContext);
    // Need to do the SendReceive and adding the observer to the
    // observer map in a critical section in case the playback
    // completes before this method has a chance to update the
    // observer list.
    iObserverMutex.Wait();
    TInt err = SendReceive(StsMsg_PlayToneAlarm, TIpcArgs(aTone,
            &alarmContextPckg, &aObserver));
    if (err != KErrNone)
        {
        //TODO: Log a message
        signalObserver = true;
        }
    else
        {
        iObserverMap[aAlarmContext] = &aObserver;
        }
    iObserverMutex.Signal();
    if (signalObserver)
        {
        aObserver.PlayAlarmComplete(aAlarmContext);
        }
    }

void RStsSession::SendStopAlarm(unsigned int aAlarmContext)
    {
    iObserverMutex.Wait();
    iObserverMap.erase(aAlarmContext);
    iObserverMutex.Signal();
    TInt err = SendReceive(StsMsg_StopAlarm, TIpcArgs(aAlarmContext));
    if (err != KErrNone)
        {
        //TODO: Log a message
        }
    }

void RStsSession::CleanUpObservers()
    {
    iObserverMutex.Wait();
    while (!iObserverMap.empty())
        {
        //TODO: Add trace here
        unsigned int context = iObserverMap.begin()->first;
        iObserverMap.erase(context);
        }
    iObserverMutex.Signal();
    }

void RStsSession::SignalObservers()
    {
    iObserverMutex.Wait();
    while (!iObserverMap.empty())
        {
        //TODO: Add trace here
        unsigned int context = iObserverMap.begin()->first;
        iObserverMap[context]->PlayAlarmComplete(context);
        iObserverMap.erase(context);
        }
    iObserverMutex.Signal();
    }
