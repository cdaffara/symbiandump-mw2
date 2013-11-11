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

#ifndef TMS_STREAM_BODY_IMPL_H
#define TMS_STREAM_BODY_IMPL_H

#include <e32base.h>
#include <tms.h>
#include "tmsqueuehandler.h"
#include "tmsstreambody.h"
#include "tmsglobalcontext.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSStreamObserver;
class TMSSource;
class TMSSink;
class TMSForamt;
class TMSEffect;
class TMSCallProxy;
class TMSStream;
class TMSBuffer;
class TMSFormat;

// TMSStreamBodyImpl class
class TMSStreamBodyImpl : public TMSStreamBody,
                          public MQueueHandlerObserver
    {
    enum TQueueType
        {
        EMsgBufQueue,
        EMsgComQueue
        };

public:
    static gint Create(TMSCallType callType, TMSStreamType stype,
            TMSCallProxy* proxy, TMSStream& parent, TMSStreamBody*& bodyimpl);
    virtual ~TMSStreamBodyImpl();

    // From TMSStreamBody
    virtual gint AddObserver(TMSStreamObserver& obsrvr, gpointer user_data);
    virtual gint RemoveObserver(TMSStreamObserver& obsrvr);
    virtual gint AddSource(TMSSource* source);
    virtual gint RemoveSource(TMSSource* source);
    virtual gint AddSink(TMSSink* sink);
    virtual gint RemoveSink(TMSSink* sink);
    virtual gint SetFormat(TMSFormat* format);
    virtual gint ResetFormat(TMSFormat* format);
    virtual gint AddEffect(TMSEffect* effect);
    virtual gint RemoveEffect(TMSEffect* effect);
    virtual TMSStreamState GetState();
    virtual TMSStreamType GetStreamType();
    virtual gint GetStreamId();
    virtual gint Init(gint retrytime);
    virtual gint Pause();
    virtual gint Start(gint retrytime);
    virtual gint Stop();
    virtual void Deinit();

    // From MQueueHandlerObserver
    virtual void QueueEvent(TInt aEventType, TInt aError, void* user_data);

private:
    TMSStreamBodyImpl();
    gint PostConstruct(TMSCallType callType, TMSStreamType stype,
            TMSCallProxy* proxy, TMSStream& parent);

    gint CreateQueue(const gint aNumSlots);
    void ReceiveMsgQHandlerEventsL();

    gint ValidateStream();
    gint ValidateSource(TMSSource* source);
    gint ValidateSink(TMSSink* sink);
    gint ValidateFormat(TMSFormat* format);

private:
    TMSCallProxy* iProxy;
    TMSStream* iParent;
    TMSCallType iCallType;
    TMSStreamType iStreamType;
    gint iStreamId;

    // Source
    TMSSource* iSource;
    TMSSink* iSink;

    // Message queue and the handler
    RMsgQueue<TmsMsgBuf> iMsgQueue;
    TMSQueueHandler* iMsgQHandler;

    // Need to make this a list
    TMSStreamObserver* iObserver;

    gpointer iUserData;
    RPointerArray<TMSEffect> iEffectsList;
    TMSFormat* iFormat;
    TMSGlobalContext iContext;
    TMSStreamState iStreamState;
    TMSStreamState iPrevState;
    };

} //namespace TMS

#endif // TMS_STREAM_BODY_IMPL_H

