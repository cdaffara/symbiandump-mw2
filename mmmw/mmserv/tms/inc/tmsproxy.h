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

#ifndef TMS_PROXY_H
#define TMS_PROXY_H

// INCLUDES
#include <mmfbase.h>
#include <mmfutilities.h> //for TFourCC
#include <tms.h>
#include "tmsqueuehandler.h"
#include "tmsclientserver.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSEffect;
class TMSEffectObserver;
class TMSGlobalRouting;
class TMSGlobalRoutingObserver;
class TMSDTMF;
class TMSDTMFObserver;
class TMSRingTone;
class TMSRingToneObserver;
class TMSInbandTone;
class TMSInbandToneObserver;

// TMSProxy class
class TMSProxy : public RSessionBase,
                 public MQueueHandlerObserver
    {
public:
    IMPORT_C TMSProxy();
    IMPORT_C virtual ~TMSProxy();
    /**
     * Opens a connection to the server.
     * @return KErrNone if successful connection created
     */
    IMPORT_C gint Connect();

    /**
     * @return The server version this session can connect to
     */
    IMPORT_C TVersion Version() const;

    /**
     * Close the session to the server
     */
    IMPORT_C void Close();

    IMPORT_C gint GetTMSCallSessionHandle();

    IMPORT_C gint GetSupportedDecoders(RArray<TUint32>& aDecoders,
            gint& aFrameSize);
    IMPORT_C gint GetSupportedEncoders(RArray<TUint32>& aEncoders,
            gint& aFrameSize);

    // TMSGlobalRouting
    IMPORT_C gint SetOutput(TMSAudioOutput output);
    IMPORT_C gint GetOutput(TMSAudioOutput& output);
    IMPORT_C gint GetPreviousOutput(TMSAudioOutput& output);
    IMPORT_C gint GetAvailableOutputs(OutputVector& outputs);

    // TMSGlobal Volume & Gain
    IMPORT_C gint GetLevel(guint& level);
    IMPORT_C gint GetMaxLevel(guint& level);
    IMPORT_C gint SetLevel(guint level);
    IMPORT_C gint GetGain(guint& level);
    IMPORT_C gint GetMaxGain(guint& level);
    IMPORT_C gint SetGain(guint level);

    // Ringtone
    IMPORT_C gint InitRT(const TMSRingToneType type, GString* str,
            GString* tts);
    IMPORT_C gint DeinitRT();
    IMPORT_C gint PlayRT();
    IMPORT_C gint StopRT();
    IMPORT_C gint PauseRT();
    IMPORT_C gint MuteRT();

    // TMSDTMF
    IMPORT_C gint InitDTMFPlayer(TMSStreamType streamtype);
    IMPORT_C gint StartDTMF(TMSStreamType streamtype, GString* tone);
    IMPORT_C gint StopDTMF(TMSStreamType streamtype);
    IMPORT_C gint ContinueDTMFStringSending(TBool continuesending);

    // TMSInbandTone
    IMPORT_C gint StartInbandTone(TMSInbandToneType inbandtonetype);
    IMPORT_C gint StopInbandTone();

    // Global notifiers
    IMPORT_C gint StartGlobalEffectNotifier();
    IMPORT_C gint CancelGlobalEffectNotifier();
    IMPORT_C gint StartRoutingNotifier();
    IMPORT_C gint CancelRoutingNotifier();

    // Message queue
    IMPORT_C gint SetMsgQueueNotifier(TMSMsgQueueNotifierType type,
            gpointer obsrvr, gpointer parent, gint clientid);
    IMPORT_C gint RemoveMsgQueueNotifier(TMSMsgQueueNotifierType type,
            gpointer obsrvr);

    IMPORT_C void TermSrv();

    // From MQueueHandlerObserver
    virtual void QueueEvent(gint aEventType, gint aError, void* event_data);

private:
    gint StartServer();

    void PopulateArrayL(TMSClientServerRequest aRequest,
            RArray<TUint32>& aDecoders, gint aCount);
    void GetAvailableOutputsL(OutputVector& outputs);

    gint AddGlobalEffectObserver(TMSEffectObserver& obsrvr,
            TMSEffect& parent, gint clientid);
    gint RemoveGlobalEffectObserver(TMSEffectObserver& obsrvr);
    gint AddRoutingObserver(TMSGlobalRoutingObserver& obsrvr,
            TMSGlobalRouting& parent, gint clientid);
    gint RemoveRoutingObserver(TMSGlobalRoutingObserver& obsrvr);
    gint AddRingToneObserver(TMSRingToneObserver& obsrvr, TMSRingTone& parent,
            gint clientid);
    gint RemoveRingToneObserver(TMSRingToneObserver& obsrvr);
    gint AddDTMFObserver(TMSDTMFObserver& obsrvr, TMSDTMF& parent,
            gint clientid);
    gint RemoveDTMFObserver(TMSDTMFObserver& obsrvr);
    gint AddInbandToneObserver(TMSInbandToneObserver& obsrvr,
            TMSInbandTone& parent, gint clientid);
    gint RemoveInbandToneObserver(TMSInbandToneObserver& obsrvr);

    gint CreateQueue(const gint aNumSlots);
    void ReceiveMsgQHandlerEventsL();
    void ResetObjectLists();

private:
    TRoutingMsgBufPckg ipckg;

    RPointerArray<TMSEffectObserver> iEffectsObsrvrList;
    RPointerArray<TMSGlobalRoutingObserver> iRoutingObsrvrList;
    RPointerArray<TMSRingToneObserver> iRingToneObsrvrList;
    RPointerArray<TMSDTMFObserver> iDTMFObsrvrList;
    RPointerArray<TMSInbandToneObserver> iInbandToneObsrvrList;

    RPointerArray<TMSEffect> iEffectsParentList;
    RPointerArray<TMSGlobalRouting> iRoutingParentList;
    RPointerArray<TMSRingTone> iRingToneParentList;
    RPointerArray<TMSDTMF> iDTMFParentList;
    RPointerArray<TMSInbandTone> iInbandToneParentList;

    // Message queue and the handler
    RMsgQueue<TmsMsgBuf> iMsgQueue;
    TMSQueueHandler* iMsgQHandler;
    };

} //namespace TMS

#endif // TMS_PROXY_H

// End of file
