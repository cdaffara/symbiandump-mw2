/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMS_QUEUEHANDLER_H
#define TMS_QUEUEHANDLER_H

// INCLUDES
#include <e32base.h>
#include <e32msgqueue.h>
#include <glib.h>
#include "tmsclientserver.h"   // for TmsMsgBuf

namespace TMS {

// FORWARD DECLARATIONS
class TMSBuffer;
class TMSGlobalContext;

/**
 *  MQueueHandlerObserver class
 *  Responsible for sending callbacks to the TMS client app
 */
class MQueueHandlerObserver
    {
public:
    virtual void QueueEvent(gint aEventType, gint aError,
	        void* event_data) = 0;
    };

/**
 *  TMSQueueHandler class
 */
NONSHARABLE_CLASS(TMSQueueHandler) : public CActive
    {
public:

    /**
     * NewL()
     * Contructor
     *
     */
    static TMSQueueHandler* NewL(RMsgQueue<TmsMsgBuf>* aMsgQueue,
            TMSGlobalContext* glblCtx);

    /**
     * destructor
     */
    virtual ~TMSQueueHandler();

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

    /*
     * Add and remove observer to buffer events
     */
    gint AddObserver(MQueueHandlerObserver& obsvr, gint aClientId);
    gint RemoveObserver(MQueueHandlerObserver& obsvr);

private:

    /**
     * Constructors
     */
    TMSQueueHandler(RMsgQueue<TmsMsgBuf>* aMsgQueue,
            TMSGlobalContext* glblCtx);

    void ConstructL();

    // From CActive
    void DoCancel();
    void RunL();
    TInt RunError(TInt aError);

    // For buffer processing upon handler events
    void DoFillBuffer(gint aBufLen, gint aStatus, gboolean flag, guint32 key);
    void DoEmptyBuffer(gint aBufLen, gint aStatus, gboolean flag, guint32 key);
    gint FindStreamInList();
    gint FindGainEffectInList();
    gint FindVolEffectInList();
    TMSStreamState ConvertToStreamState(gint request);

private:

    RPointerArray<MQueueHandlerObserver> iObserversList; // (not owned)
    MQueueHandlerObserver* iObserver; //not owned
    RArray<gint> iClientList;
    RMsgQueue<TmsMsgBuf>* iMsgQueue;

    RChunk iChunk;
    TPtr8 iChunkDataPtr;
    TMSBuffer* iBuffer;
    TMSGlobalContext* iTMSGlobalContext;
    };

} //namespace TMS

#endif // TMS_QUEUEHANDLER_H

// End of File
