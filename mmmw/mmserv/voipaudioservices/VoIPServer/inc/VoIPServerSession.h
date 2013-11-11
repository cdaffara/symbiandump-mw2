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
 * Description:  Definition of VoIP server session class.
 *
 */

#ifndef VOIPSERVERSESSION_H
#define VOIPSERVERSESSION_H

// INCLUDES
#include <e32msgqueue.h>
#include <mdaaudiosampleplayer.h>
#include <sounddevice.h>
#include "VoIPServerAO.h"
#include "VoIPSharedData.h"
#include "VoIPAudioClientServer.h"
#include "VoIPServer.h"
#include "VoIPAudioSession.h"

// FORWARD DECLARATIONS
class CVoIPAudioServerThread;
class MProEngEngine;
class MProEngProfile;

// -----------------------------------------------------------------------------
//  Class Name:  CVoIPAudioServerSession
//
//  VoIP server session class.
// -----------------------------------------------------------------------------
class CVoIPAudioServerSession : public CSession2,
        public MThreadComObserver,
        public MDevSoundObserver,
        public MMdaAudioPlayerCallback
    {
    // Each session is designated as one of these types
    enum TSessionType
        {
        ENone,
        EQueryDec,
        EQueryEnc,
        EDnlink,
        EUplink,
        ERingTone
        };

public:
    // Constractor
    static CVoIPAudioServerSession* NewL(CVoIPAudioServer &aServer);

    // Destructor
    ~CVoIPAudioServerSession();

    /**
     * ?description
     *
     * @param const RMessage2&
     * @return void
     */
    void SetPriorityL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param const RMessage2&
     * @return void
     */
    void GetCodecsCountL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param const RMessage2&
     * @return void
     */
    void GetSupportedCodecsL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param const RMessage2&
     * @return void
     */
    void OpenDownlinkL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param const RMessage2&
     * @return void
     */
    void OpenUplinkL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetFormatL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void StartDownlink();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void StartUplink();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void StopDownlink();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void StopUplink();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void CloseDownlink();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void CloseUplink();

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetMaxVolumeL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetVolumeL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetVolume(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetMaxGainL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetGainL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetGain(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetAudioDeviceL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetAudioDeviceL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void BufferFilledL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void BufferEmptiedL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RThread&
     * @param TUserCommand
     * @return void
     */
    void SendCmd(RThread& aThread, TUserCommand aCmd);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetIlbcCodecModeL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetIlbcCodecModeL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetG711CodecModeL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetG711CodecModeL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetSupportedBitratesCountL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetSupportedBitratesL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetBitrate(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetBitrateL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void FrameModeRqrdForEcL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetFrameMode(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetFrameModeL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void ConcealErrorForNextBuffer();

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetVad(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetVadL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetCng(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetCngL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void SetPlc(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void GetPlcL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void BadLsfNextBuffer();

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void ConfigureJitterBufferL(const RMessage2& aMessage);

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void ResetJitterBuffer(const RMessage2& aMessage);

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

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void OpenDTMFTonePlayerL();

    /**
     * ?description
     *
     * @param RMessage2&
     * @return void
     */
    void PlayDTMFToneL(const RMessage2& aMessage);

    /**
     * ?description
     *
     *
     * @param none
     * @return void
     */
    void StopDTMFTonePlay();

    /**
     * ?description
     *
     *
     * @param none
     * @return void
     */
    void CloseDTMFTonePlayer();

    /**
     * ?description
     *
     *
     * @param const RMessage2&
     * @return void
     */
    void OpenRingTonePlayerFromFileL(const RMessage2& aMessage);

    /**
     * ?description
     *
     *
     * @param const RMessage2&
     * @return void
     */
    void OpenRingTonePlayerFromRFileL(const RMessage2& aMessage);

    /**
     * ?description
     *
     *
     * @param none
     * @return void
     */
    void OpenRingTonePlayerFromProfileL();

    /**
     * ?description
     *
     *
     * @param const RMessage2&
     * @return void
     */
    void OpenRingTonePlayerFromDesL(const RMessage2& aMessage);

    /**
     * ?description
     *
     *
     * @param none
     * @return void
     */
    void PlayRingTone();

    /**
     * ?description
     *
     *
     * @param none
     * @return void
     */
    void PauseRingTone();

    /**
     * ?description
     *
     *
     * @param none
     * @return void
     */
    void ResumeRingTone();

    /**
     * ?description
     *
     *
     * @param none
     * @return void
     */
    void StopRingTone();

    /**
     * ?description
     *
     *
     * @param none
     * @return void
     */
    void CloseRingTonePlayer();

    /**
     * ?description
     *
     *
     * @param TInt
     * @return void
     */
    void InitRingTonePlayerL();

    // CALLBACKS

    // From MMdaAudioPlayerCallback

    /**
     * ?description
     *
     * @param TInt
     * @param const TTimeIntervalMicroSeconds&
     * @return void
     */
    virtual void MapcInitComplete(TInt aError,
            const TTimeIntervalMicroSeconds& aDuration);

    /**
     * ?description
     *
     * @param TInt
     * @return void
     */
    virtual void MapcPlayComplete(TInt aError);

    // From MThreadComObserver

    /**
     * ?description
     *
     * @param TExcType
     * @return void
     */
    virtual void DoHandleCmd(TExcType aExc, TInt aError);

    /**
     * ?description
     *
     * @param TInt
     * @return void
     */
    virtual void DoHandleError(TInt aError);

    // From MDevSoundObserver
    virtual void InitializeComplete(TInt aError);
    virtual void BufferToBeEmptied(CMMFBuffer* aBuffer);
    virtual void RecordError(TInt aError);
    virtual void BufferToBeFilled(CMMFBuffer* aBuffer);
    virtual void PlayError(TInt aError);
    virtual void ToneFinished(TInt /*aError*/);
    virtual void ConvertError(TInt /*aError*/) {}
    virtual void DeviceMessage(TUid /*aMessageType*/, const TDesC8& /*aMsg*/) {}
    virtual void SendEventToClient(const TMMFEvent& /*aEvent*/) {}

private:

    // Symbian constructors
    void ConstructL();
    CVoIPAudioServerSession(CVoIPAudioServer& aServer);

    // Client-server message handlers
    void ServiceL(const RMessage2& aMessage);
    void DispatchMessageL(const RMessage2& aMessage);

    void SendCmdToThread(const TInt aCommand, const TInt aStatus = KErrNone);
    void NotifyClient(const TInt aCommand, const TInt aStatus = KErrNone,
            const TInt64 aInt64 = TInt64(0));
    void NotifyDtmfClient(const TInt aCommand, const TInt aStatus = KErrNone);

    // Handlers of request complete events from the work threads
    void DownlinkInitComplete(TInt aError);
    void UplinkInitComplete(TInt aError);
    void GetVolumeComplete(TInt aError);
    void GetGainComplete(TInt aError);
    void GetAudioDeviceComplete(TInt aError);
    void GetSupportedBitratesComplete(TInt aError);
    void GetBitrateComplete(TInt aError);
    void GetG711CodecModeComplete(TInt aError);
    void GetIlbcCodecModeComplete(TInt aError);
    void GetFrameModeComplete(TInt aError);
    void GetFrameModeReqForECComplete(TInt aError);
    void GetVadComplete(TInt aError);
    void GetCngComplete(TInt aError);
    void GetPlcComplete(TInt aError);

    //void DetermineG711FrameRateL();
    void ReleaseQueuedMessage(TInt aError, TUint aFrmSize = 0);

private:
    //data members

    CVoIPAudioServer& iVoIPServer;
    RThread iThread;
    TSharedData iShared;

    // Message queue for communication back to the client side
    RMsgQueue<TVoIPMsgBuf> iMsgComQueue;
    RMsgQueue<TVoIPMsgBuf> iMsgComQueueDtmf;

    // Message queue for communication between server threads
    RMsgQueue<TVoIPMsgBuf> iITCMsgComQueue;

    TVoIPMsgBuf iMsgBuffer;
    TMMFPrioritySettings iPriority;

    // Msg queued until called back from the DL/UL thread
    RMessage2 iMessage;
    TBool iMsgQueued;

    TInt iMaxVolume;
    TInt iMaxGain;
    RArray<TFourCC> iCodecsUp;
    RArray<TFourCC> iCodecsDn;
    TInt iCodecsCountUp;
    TInt iCodecsCountDn;
    CVoIPServerAO* iAO; // ITC back with this thread
    TSessionType iSessionType;

    CMMFDevSound* iDevSound; // To get supported codecs
    CMMFDevSound* iDTMFTonePlayer;
    TBool iDTMFTonePlayerInitRequest;
    TBool iDevSoundInitRequest;
    TTimeIntervalMicroSeconds32 iToneLenOn;
    TTimeIntervalMicroSeconds32 iToneLenOff;
    TTimeIntervalMicroSeconds32 iToneLenPause;

    CMdaAudioPlayerUtility* iRingTonePlayer;
    HBufC8* iToneBuf;
    };

#endif //VOIPSERVERSESSION_H

// End of file
