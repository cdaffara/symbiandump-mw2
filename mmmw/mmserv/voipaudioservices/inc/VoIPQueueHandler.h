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
 * Description:  Declares CQueueHandler class which handles VoIP client/server
 *               messaging.
 *
 */

#ifndef VOIPQUEUEHANDLER_H
#define VOIPQUEUEHANDLER_H

// INCLUDES
#include <e32base.h>
#include <e32msgqueue.h>
#include "VoIPAudioClientServer.h"   // for TVoIPMsgBuf
// FORWARD DECLARATIONS
class CVoIPDataBuffer;

/**
 *  MQueueHandlerObserver
 *  Responsible for sending callbacks to the VoIP client app
 */
class MQueueHandlerObserver
    {
public:
    virtual void FillBuffer(CVoIPDataBuffer* aBuffer) = 0;
    virtual void EmptyBuffer(CVoIPDataBuffer* aBuffer) = 0;
    virtual void Event(TInt aEventType, TInt aError) = 0;
    };

/**
 *  CQueueHandler
 *
 */
NONSHARABLE_CLASS(CQueueHandler) : public CActive
    {
public:

    /**
     * NewL()
     * Contructor
     *
     */
    static CQueueHandler* NewL(MQueueHandlerObserver* aObserver,
            RMsgQueue<TVoIPMsgBuf>* aMsgQueue, TInt aBufferLen = 0);

    /**
     * destructor
     */
    virtual ~CQueueHandler();

    /**
     * Start
     * Starts listening for events.
     *
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
    CQueueHandler(RMsgQueue<TVoIPMsgBuf>* aMsgQueue);

    void ConstructL(MQueueHandlerObserver* aObserver, TInt aBufferLen);

    // from CActive
    void DoCancel();
    void RunL();
    TInt RunError(TInt aError);
    void DoFillBuffer(TInt aBufLen, TInt aStatus);
    void DoEmptyBuffer(TInt aBufLen, TInt aStatus);

private:
    // data

    MQueueHandlerObserver* iObserver; // (not owned)
    RMsgQueue<TVoIPMsgBuf>* iMsgQueue;

    RChunk iChunk;
    TPtr8 iChunkDataPtr;
    CVoIPDataBuffer* iBuffer;
    };

#endif //VOIPQUEUEHANDLER_H

// End of File
