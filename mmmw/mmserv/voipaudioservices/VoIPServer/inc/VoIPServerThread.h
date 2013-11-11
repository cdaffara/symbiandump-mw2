/*
 * Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  VoIP Audio Services - Uplink stream API
 *               Defines uplink and downlink thread objects.
 *
 */

#ifndef VOIPAUDIOSERVERTHREAD_H
#define VOIPAUDIOSERVERTHREAD_H

#include <e32msgqueue.h>
#include <sounddevice.h>
#include "JitterBufferObserver.h"
#include "JitterBuffer.h"
#include "VoIPAudioClientServer.h"
#include "VoIPQueueHandlerSrv.h"
#include "VoIPSharedData.h"

// FORWARD DECLARATIONS
class CAudioOutput;
class CErrorConcealmentIntfc;
class CSpeechEncoderConfig;
class CG711DecoderIntfc;
class CG729DecoderIntfc;
class CIlbcDecoderIntfc;
class CG711EncoderIntfc;
class CG729EncoderIntfc;
class CIlbcEncoderIntfc;
class CQueueHandlerSrv;

// -----------------------------------------------------------------------------
//  Class Name:  CVoIPThreadBase
//
//  Base abstract class for creating VoIP uplink and downlink threads.
// -----------------------------------------------------------------------------
class CVoIPThreadBase : public CBase,
        public MDevSoundObserver,
        public MQueueHandlerObserverSrv
    {
public:
    enum TStreamState
        {
        ENotReady,  // not initialized
        EReady,     // initialized and ready for streaming
        EStreaming  // active streaming
        };

public:
    virtual ~CVoIPThreadBase();

    // From MQueueHandlerObserverSrv
    virtual void Event(TInt aEventType, TInt aError) = 0;

protected:
    virtual void SetCodecCiL() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;

    void InitThreadL();
    void InitDevSoundL(const TMMFState aDevSoundState, const TInt aPriority,
            const TInt aPreference);
    void InitMsgQueuesL(const TDesC& aBufferQ, const TDesC& aComQ);
    TInt DoChunk(const TDesC& aChunkName, const TInt aChunkLen,
            const TInt aChunkMaxLen);
    TInt DetermineMaxBufferLen(
            TVoIPCodecSettings::TG711FrameRate aG711FrameRate =
                    TVoIPCodecSettings::E20MS);

    // From MDevSoundObserver
    virtual void InitializeComplete(TInt aError) = 0;
    virtual void BufferToBeEmptied(CMMFBuffer* /*aBuffer*/) {}
    virtual void RecordError(TInt /*aError*/) {}
    virtual void BufferToBeFilled(CMMFBuffer* /*aBuffer*/) {}
    virtual void PlayError(TInt /*aError*/) {}
    virtual void ToneFinished(TInt /*aError*/) {}
    virtual void ConvertError(TInt /*aError*/) {}
    virtual void DeviceMessage(TUid /*aMessageType*/, const TDesC8& /*aMsg*/) {}
    virtual void SendEventToClient(const TMMFEvent& /*aEvent*/) {}

protected:

    CActiveScheduler* iActiveScheduler;
    TStreamState iStatus;
    CMMFDevSound* iDevSound;

    RMsgQueue<TVoIPMsgBuf> iMsgQueue; // ITC back to client
    RMsgQueue<TVoIPMsgBuf> iMsgComQueue; // ITC with this thread
    CQueueHandlerSrv* iMsgQComHandler; // commands from main thread
    TVoIPMsgBuf iMsgBuffer;
    CMMFDataBuffer* iDevSoundBufPtr;
    RChunk iChunk;
    TUint32 iCodecID;
    TInt iBufLen;
    TInt iMaxBufLen;

    CVoIPFormatIntfc::TG711CodecMode iG711CodecMode;
    CVoIPFormatIntfc::TILBCCodecMode iILBCCodecMode;
    };

// -----------------------------------------------------------------------------
//  Class Name:  CVoIPDownlinkThread
//
//  Class handling VoIP downlink in a separate thread.
// -----------------------------------------------------------------------------
class CVoIPDownlinkThread : public CVoIPThreadBase,
        public MJitterBufferObserver
    {
public:
    static TInt ThreadFunction(TAny* aData);
    virtual ~CVoIPDownlinkThread();

private:

    static CVoIPDownlinkThread* NewL(TSharedData& aData);
    CVoIPDownlinkThread(TSharedData& aData);
    void ConstructL();

    void SetCodecCiL();
    void Start();
    void Stop();

    void BufferFilled();
    void SetVolume();
    void GetVolume();
    void SetAudioDeviceL();
    void GetAudioDeviceL();

    // For CIs
    void SetIlbcCodecMode();
    void GetIlbcCodecMode();
    void SetG711CodecMode();
    void GetG711CodecMode();
    void FrameModeRqrdForEC();
    void SetFrameMode();
    void GetFrameMode();
    void ConcealErrorForNextBuffer();
    void SetCng();
    void GetCng();
    void SetPlc();
    void GetPlc();
    void BadLsfNextBuffer();

    // For Jitter Buffer
    /**
     * ?description
     *
     * @param  none
     * @return void
     */
    void ConfigureJitterBufferL();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void ResetJitterBuffer();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void JBDelayDown();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void JBDelayUp();

    void SendCmd(TUserCommand aCmd, TInt aError = KErrNone);

    // From MDevSoundObserver
    virtual void InitializeComplete(TInt aError);
    virtual void BufferToBeFilled(CMMFBuffer* aBuffer);
    virtual void PlayError(TInt aError);

    // From MQueueHandlerObserverSrv
    virtual void Event(TInt aEventType, TInt aError);

    // From MJitterBufferObserver
    virtual void EventJB(TInt aEventType, TInt aError = KErrNone);

private:

    TSharedData& iShared;
    TBool iFrameMode;
    TBool iPlc;
    CAudioOutput* iAudioOutput;
    CErrorConcealmentIntfc* iErrConcealmentIntfc;
    CG711DecoderIntfc* iG711DecoderIntfc;
    CG729DecoderIntfc* iG729DecoderIntfc;
    CIlbcDecoderIntfc* iIlbcDecoderIntfc;

    CVoIPJitterBuffer* iJitterBuffer;
    CMMFDataBuffer* iAddJBuffer;
    CMMFDataBuffer* iGetJBuffer;

#ifdef _DEBUG
    TInt iSamplesPlayedCount;
#endif
    };

// -----------------------------------------------------------------------------
//  Class Name:  CVoIPUplinkThread
//
//  Class handling VoIP uplink in a separate thread.
// -----------------------------------------------------------------------------
class CVoIPUplinkThread : public CVoIPThreadBase
    {
public:

    static TInt ThreadFunction(TAny* aData);
    virtual ~CVoIPUplinkThread();

private:

    static CVoIPUplinkThread* NewL(TSharedData& aData);
    CVoIPUplinkThread(TSharedData& aData);
    void ConstructL();

    void SetCodecCiL();
    void Start();
    void Stop();

    void BufferEmptied();
    void SetGain();
    void GetGain();

    // For CIs
    void SetIlbcCodecMode();
    void GetIlbcCodecMode();
    void SetG711CodecMode();
    void GetG711CodecMode();
    void GetSupportedBitrates();
    void SetBitrate();
    void GetBitrate();
    void SetVad();
    void GetVad();

    void SendCmd(TUserCommand aCmd, TInt aError = KErrNone);

    // From MDevSoundObserver
    virtual void InitializeComplete(TInt aError);
    virtual void BufferToBeEmptied(CMMFBuffer* /*aBuffer*/);
    virtual void RecordError(TInt /*aError*/);

    // From MQueueHandlerObserverSrv
    virtual void Event(TInt aEventType, TInt aError);

private:

    TSharedData& iShared;
    TBool iVad;
    RArray<TUint> iBitrates;

    CSpeechEncoderConfig* iSpeechEncoderConfig;
    CG711EncoderIntfc* iG711EncoderIntfc;
    CG729EncoderIntfc* iG729EncoderIntfc;
    CIlbcEncoderIntfc* iIlbcEncoderIntfc;

#ifdef _DEBUG
    TInt iSamplesRecCount;
#endif
    };

#endif  // VOIPAUDIOSERVERTHREAD_H

// End of file
