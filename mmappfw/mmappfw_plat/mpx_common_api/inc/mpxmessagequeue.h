/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Queue the message
*
*/


#ifndef CMPXMESSAGEQUEUE_H
#define CMPXMESSAGEQUEUE_H

#include <e32base.h>    // For CActive, link against: euser.lib
#include <e32msgqueue.h>
#include <mpxmessage2.h>

class CMPXMessageItem;

/**
*  Message queue.
*
*  @lib mpxcommon.lib
*/
class CMPXMessageQueue : public CBase
    {
public:
    /**
     * Two-phased constructor.
     *
     * @since S60 3.2.3
     * @return new object of the CMPXMessageQueue
     */
    IMPORT_C static CMPXMessageQueue* NewL();

    /**
     * Destructor.
     *
     * @since S60 3.2.3
     */
    IMPORT_C ~CMPXMessageQueue();

public: // New functions
    /**
     * Request to send next message.
     *
     * @since S60 3.2.3
     * @param aMsgRequest message request
     */
    IMPORT_C void SendNext(const RMessage2& aMsgRequest);
    
    /**
     * Adds a message into the queue and sends the message if slot is available.
     *
     * @since S60 3.2.3
     * @param aMessage message object, ownership not transferred.
     * @param aError error code
     */
    IMPORT_C void Add(const CMPXMessage* aMessage, TInt aError);
    
    /**
     * Reset the message queue.
     *
     * @since S60 3.2.3
     */
    IMPORT_C void Reset();

    /**
     * Adds a message into the top of the queue and sends the message if slot is available.
     *
     * @since S60 9.2
     * @param aMessage message object, ownership not transferred.
     * @return system error
     */    
    IMPORT_C TInt AddFirst(const CMPXMessage* aMessage, TInt aError);
private:
    /**
     * C++ constructor.
     *
     * @since S60 3.2.3
     */
    CMPXMessageQueue();
    
    /**
     * Second-phase constructor.
     *
     * @since S60 3.2.3
     */
    void ConstructL();

    /**
     * Send a message to client.
     *
     * @since S60 3.2.3
     */
    void Send();

private:
    RMessage2 iMsgRequest;
    TSglQue<CMPXMessageItem> iMsgs; // Queue for the messages
    CMPXMessageItem* iMsgSent; // Sent message
    TInt iFailure; // Failed to add message to the queue
#ifdef _DEBUG
    TInt iCount; // message count
#endif
    };

#endif // CMPXMESSAGEQUEUE_H

// End of file
