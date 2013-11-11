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
 * Description:  CQueueHandler class declaration.
 *
 */

#ifndef VOIPSERVERCALLBACKHANDLERSRV_H
#define VOIPSERVERCALLBACKHANDLERSRV_H

// INCLUDES
#include <e32base.h>
#include <e32msgqueue.h>
#include "VoIPAudioClientServer.h"   // for TVoIPMsgBuf

/**
 *  MQueueHandlerObserver
 *  Responsible for sending callbacks to the VoIP client app
 */
class MQueueHandlerObserverSrv
    {
public:
    virtual void Event(TInt aEventType, TInt aError) = 0;
    };

/**
 *  CQueueHandler
 */
class CQueueHandlerSrv : public CActive
    {
public:

    /**
     * NewL()
     * Contructor that takes single queue pointer
     */
    static CQueueHandlerSrv* NewL(MQueueHandlerObserverSrv* aObserver,
            RMsgQueue<TVoIPMsgBuf>* aMsgQueue);

    /**
     * destructor
     */
    ~CQueueHandlerSrv();

    /**
     * Start
     * Starts listening for events.
     */
    void Start();

    /**
     * Returns request status.
     */
    TRequestStatus* Status();

private:

    /**
     * constructor
     */
    CQueueHandlerSrv(RMsgQueue<TVoIPMsgBuf>* aMsgQueue);

    void ConstructL(MQueueHandlerObserverSrv* aObserver);

    // from CActive
    void DoCancel();
    void RunL();
    TInt RunError(TInt aError);

private:
    // data

    MQueueHandlerObserverSrv* iObserver; // (not owned)
    RMsgQueue<TVoIPMsgBuf>* iMsgQueue;

    };

#endif // VOIPSERVERCALLBACKHANDLERSRV_H

// End of File
