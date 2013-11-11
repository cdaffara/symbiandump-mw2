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

#ifndef CALLCSADPT_H
#define CALLCSADPT_H

// INCLUDES
#include <TelephonyAudioRouting.h>
#include <MTelephonyAudioRoutingObserver.h>
#include <e32msgqueue.h>
#include <etelmm.h>
#include <rmmcustomapi.h>
#include "tmsclientserver.h"
#include "tmscalladpt.h"
#include "tmscsdevsoundobserver.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSCSUplink;
class TMSCSDownlink;
class TMSTarSettings;
class TMSSyncVol;

/*
 * TMSCallCSAdpt class
 */
class TMSCallCSAdpt : public TMSCallAdpt,
                      public TMSCSDevSoundObserver,
                      public MTelephonyAudioRoutingObserver
    {
public:
	static TMSCallCSAdpt* NewL();
    virtual ~TMSCallCSAdpt();
    virtual gint PostConstruct();

    // From TMSStream
    virtual gint CreateStream(const TMSCallType callType,
            const TMSStreamType strmType, gint& outStrmId);
    virtual gint InitStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId,
            const TMSFormatType frmtType, const gint retryTime,
            const RMessage2& message);
    virtual gint StartStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId,
            const gint retrytime);
    virtual gint PauseStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId);
    virtual gint StopStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId);
    virtual gint DeinitStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId);
    virtual gint DeleteStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId);

    // From TMS effects
    gint GetGlobalMaxVolume(guint& volume);
    gint SetGlobalVolume(const guint volume);
    gint GetGlobalVolume(guint& volume);
    gint GetGlobalMaxGain(guint& gain);
    gint SetGlobalGain(const guint gain);
    gint GetGlobalGain(guint& gain);

    // From TMS audio routing
    virtual gint SetOutput(const TMSAudioOutput output);
    virtual gint GetOutput(TMSAudioOutput& output);
    virtual gint GetPreviousOutput(TMSAudioOutput& output);
    virtual gint GetAvailableOutputsL(gint& count, CBufFlat*& outputsbuf);

    //From TMSCSDevSoundObserver
    void DownlinkInitCompleted(gint status);
    void UplinkInitCompleted(gint status);
    void DownlinkActivationCompleted(gint status);
    void UplinkActivationCompleted(gint status);

private:
    TMSCallCSAdpt();
    void ConstructL();

    gint InitUplink(const gint retrytime);
    gint InitDownlink(const gint retrytime);
    void AvailableOutputsChanged(
            CTelephonyAudioRouting& aTelephonyAudioRouting);
    void OutputChanged(CTelephonyAudioRouting& aTelephonyAudioRouting);
    void SetOutputComplete(CTelephonyAudioRouting& aTelephonyAudioRouting,
            gint status);
    void NotifyClient(const gint strmId, const gint command,
            const gint status = KErrNone, const gint64 int64 = TInt64(0));

private:
    gint iNextStreamId;
    TMSCSUplink* iCSUplink;
    TMSCSDownlink* iCSDownlink;
    CTelephonyAudioRouting* iRouting;
    TMSTarSettings* iTarSettings;

    // Message queues for communication and data transfer back to the client
    RMsgQueue<TmsMsgBuf> iMsgQueueUp;
    RMsgQueue<TmsMsgBuf> iMsgQueueDn;
    TmsMsgBuf iMsgBuffer;

    gint iUplinkStreamId;
    gint iDnlinkStreamId;
    TMSSyncVol* iResetVolNotifier;
    };

} //namespace TMS

#endif // CALLCSADPT_H

