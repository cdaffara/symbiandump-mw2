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
 * Description:  Active object which implements ringing tone functionality
 *
 */

#ifndef TMSRINGTONEPLAYER_H
#define TMSRINGTONEPLAYER_H

// INCLUDES
#include <MProEngEngine.h>
#include <MProEngProfile.h>
#include "tmsrtaudiohdlrobsrv.h"
#include "tmsrtplayerobsrv.h"
#include "tmsrtaudiohdlr.h"
#include "tmstimer.h"
#include "tmsasyncreqao.h"
#include "tmsrtparam.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSRtAudioHdlr;
class TMSTimer;
class CMdaServer;
class TMSRtContainer;
class TMSRtParam;

// CLASS DECLARATION

/**
 * It implements ringing tone functionality.
 * Note that when issuing Play-command to audio side, ALWAYS set the tone type
 * before the Play() is called. This is to ensure that correct tone is tried to
 * be played.
 */
class TMSRingTonePlayer : public CBase,
                          public TMSRtAudioHdlrObsrv,
                          public TMSTimerObserver,
                          public TMSAsyncReqObsrv
    {
public:

    /**
     * Two-phased constructor.
     * @return new instance.
     */
    static TMSRingTonePlayer* NewL(TMSRtPlayerObsrv& aObserver);

    /**
     * Destructor.
     */
    ~TMSRingTonePlayer();

    /**
     * Construct default tone player.
     */
    void CreateDefaultRtPlayerL();

    /**
     * Do construct tone.
     * @param aRtParam
     * @param aFile
     */
    void CreateRingTonePlayerL(const TRtPlayerType aPlayer,
            const TDesC& aFile = KNullDesC());

    /**
     * Construct audio player.
     */
    void CreateAudioPlayerL();

    /**
     * Construct sequence player.
     * @param aPlayer
     */
    void CreateSequencePlayerL(const TRtPlayerType aPlayer);

    /**
     * Construct custom sequence players.
     * @param aSequence
     */
    void CreateCustomSequencePlayerL(const TDesC8& aSequence);

    /**
     * Construct TTS player.
     * @param aTextToSay
     */
    void CreateTTSPlayerL(const TDesC& aTextToSay);

    /**
     * Delete TTS player.
     */
    void RemoveTTSPlayer();

    /**
     * Play ring tone
     */
    void PlayRingToneL();

    /**
     * Play audio ring tone
     */
    void PlayAudioRingToneL();

    /**
     * Playing the Beep once.
     */
    void BeepOnceL();

    /**
     * Play silent tone. No_sound.wav will be played.
     * If vibration alert is ON, phone will vibrate.
     */
    void PlaySilentToneL();

    /**
     * Play unsecure VoIP tone.
     * Play tone if secure call fails and call is changed to unsecure call
     */
    void PlayUnsecureVoIPToneL();

    /**
    * Play custom sequence tone.
    */
    void PlaySequenceToneL();

    /**
     * Stops playing the ringing tone.
     */
    void StopPlaying();

    /**
     * Continues video and audio RT playback with volume 0
     */
    void MuteRingTone();

    /**
     * Deinitialize Ringtone player.
     */
    void DeinitRingTonePlayer();

    /**
     * Play default tone.
     */
    void PlayDefaultToneL();

    /**
     * Play TTS tone along with audio RT. Only supported with Default and
     * custom sample tones. TTS does not start playing immediately.
     * This method does not command phone to play TTS immediately but after
     * pre-determined delay.
     */
    void PlayTtsTone();

    /**
     * Checks if extended security is required.
     * @return ETrue if extended security is required.
     */
    TBool ExtendedSecurity() const;

    /*
     * Return RT player type
     */
    TRtPlayerType PlayerType() const;

    /*
     * From TMSAsyncReqObsrv
     */
    virtual void HandleError(TInt aError);
    virtual void HandleAsyncReq();

private:

    /**
     * Creates single beep tone player.
     */
    void CreateBeepOncePlayerL();

    /**
     * Creates silent tone player.
     */
    void CreateSilentPlayerL();

    /**
     * Creates unsecure voip tone player.
     */
    void CreateUnsecureVoipPlayerL();

    /**
     * Initializes ring tone configuration parameters.
     */
    void InitRtParams(TRtPlayerType aPlayerType);

    /*
     * from TMSRtAudioHdlrObsrv
     */
    void RtAudioHdlrEvent(TInt aEvent, TInt aError, TInt aPlayer);

    /**
     * Handle RT player error event.
     */
    void HandleRtAudioError(TInt aEvent, TInt aError, TInt aPlayer);

    /**
     * Handle RT player initialize complete event.
     */
    virtual void HandleRtAudioInitComplete(TInt aEvent, TInt aError,
            TInt aPlayer);

    /**
     * Handle RT play complete event.
     */
    virtual void HandleRtAudioPlayComplete(TInt aEvent, TInt aError,
            TInt aPlayer);

    /**
     * From TMSTimerObserver
     */
    virtual void TimerEvent();

    /**
     * Enumerates how many times TTS has played
     * ESaidOnce
     * ESaidTwice
     */
    enum TTtsStatus
        {
        ESaidOnce,
        ESaidTwice
        };

    /**
     * C++ default constructor.
     */
    TMSRingTonePlayer(TMSRtPlayerObsrv& aObserver);

    /**
     * Default Symbian OS constructor.
     */
    void ConstructL();

    /**
     * Play backup tone.
     */
    void PlayBackupToneL();

    /**
     * Handle audio player error.
     * @param aDelete ETrue if deletion of the player allowed.
     * @param aSync ETrue if synchronous deletion of players is allowed.
     */
    void DoHandlePlayerError(TBool aDelete, TBool aSync);

    /**
     * Convert ringing type from TProfileRingingType to local type.
     * @param aRingingType ringing type form PhoneAppEngine.
     * @return local ringing type mapping.
     */
    TMSRtAudioHdlr::TRingingType ConvertRingingType(
            TProfileRingingType aRingingType);

    /**
     * Construct default ring tone container
     */
    void CreateDefaultRingToneL();

    /**
     * Load ring tone settings from curent Profile
     */
    void LoadActiveProfileSettingsL();

    /**
     * Load ring tone settings from curent Profile
     */
    void LoadProfileRingToneL();

    /**
     * Sets TTS volume ramp and schedules next TTS playback
     */
    void ResumeTTS();

    /**
     * Construct backup tone player.
     */
    void CreateBkpRtPlayerL();

    /**
     * Delete all players.
     */
    void RemovePlayers();

    /**
     * Delete single player based on index.
     */
    void DeletePlayer();

    /**
     * Deletes player asynchronoysly.
     * @param aPlayer player to be deleted.
     */
    void DeletePlayerAsync(TRtPlayerType aPlayer);

    /**
     * Wrapper of DoHandleTtsDelayTimeout, that can be used as TCallback.
     * Immediately passes control to DoHandleTtsDelayTimeout method.
     * @param object Object to call DoHandleTtsDelayTimeout() on.
     * @return status.
     */
    static TInt HandleTtsDelayTimeout(TAny* object);

    /**
     * It is called after TTS delay elapsed. Starts playing TTS.
     */
    void DoHandleTtsDelayTimeout();

    /**
     * Returns currently active player which is used with TTS player.
     * @return pointer to active player.
     */
    TMSRtAudioHdlr* GetCurrentlyActiveAudioPlayerWithTTS();

    /**
     * Calculates new volume and ramptimes for Ascending TTS and normal TTS
     * ringing.
     * @param aStatus - TTS player status.
     */
    void SolveNewVolumeAndRamptime(TTtsStatus aStatus);

    /**
     * Restarts currently active ringingtone player.
     */
    void ReStartRingTonePlayer();

    /**
     * Checks if ring tone file size is within allowed limit. If size limit
     * is exceeded, default tone is played instead.
     * @param aRingingTone RT file to be checked.
     * @return EFalse if the limit was exceeded.
     *         ETrue  if tone file's size was valid.
     */
    TBool CheckAndHandleToneSizeLimit(TMSRtContainer* aRingingTone);

    /**
     * Checks if ring tone file size is larger than the allowed limit.
     * @param aFile   File to check.
     *        aSizeLimitKB  the size limit in kB.
     * @return KErrNone          if the file can be used as a ring tone.
     *         KErrTooBig        if the file size limit is exceeded.
     *         Other error value if error happened during size checking.
     */
    TInt CheckToneFileSize(const TDesC& aFile, TInt aSizeLimitKB);

    /**
     * Reads from central repository the file size limit of following tones:
     * - contact's personal ringing tone
     * - voice call line1
     * - voice call line2
     * Value 0 means all sizes are allowed.
     * @param aSizeLimitKB  the size limit in kB is read to this variable.
     * @return leaves on error.
     */
    void GetMaxToneFileSizeL(TInt& aSizeLimitKB) const;

    /**
     * Returns currently active audio player.
     * @return Currently active audio player or NULL if none is active.
     */
    TMSRtAudioHdlr* ActiveAudioPlayer();

private:

    TMSRtAudioHdlr* iAudioPlayer;
    TMSRtAudioHdlr* iBeepOncePlayer;
    TMSRtAudioHdlr* iSilentPlayer;
    TMSRtAudioHdlr* iUnsecureVoIPTonePlayer;
    TMSRtAudioHdlr* iDefaultPlayer;
    TMSRtAudioHdlr* iSequencePlayer;
    TMSRtAudioHdlr* iTtsPlayer;
    TMSRtAudioHdlr* iBackupPlayer;

    // Indicate which tone is playing
    enum TTonePlayingStatus
        {
        EIdle,
        EAudioTonePlaying,
        EBeepOnce,
        ESilentTonePlaying,
        EUnsecureVoIPTonePlaying,
        EDefaultTonePlaying,
        EBackupTonePlaying,
        EVideoTonePlaying,
        ESilentVideoTonePlaying,
        ESequenceTonePlaying,
        EPersonalVideoTonePlaying
        };

    // Current playing status.
    TTonePlayingStatus iTonePlayingStatus;

    // True if TTS is currently playing. Different from iTonePlayingStatus,
    // because TTS is played in parallel with the other tone types.
    TBool iTtsTonePlaying;

    // Zero -based index of the TTS delay that is "executed" currently,
    // or to be "executed" after the current playing of the TTS tone.
    TInt iTtsDelayIndex;

    // True if TTS tone should be played when the next playing of the
    // default or personal tone is fired.
    TBool iTtsToneToBePlayed;

    // Audio/Video RT file container
    TMSRtContainer* iRtContainer;

    // Volume of the TTS
    TInt iTtsVolume;

    // RT player type
    TInt iRtType;

    //
    TMSRtPlayerObsrv& iRtObserver;
    TMSAsyncReqAO* iAsyncReqAO;
    TMSRtParam iRtParam;

    // 3 sec timer that fires up fallback logic.
    TMSTimer* iTimer;

    // Timer that tracks delays before playing TTS.
    TMSTimer* iTtsDelayTimer;

    // Number of times TTS tone should be played for the current ringtone.
    // If ringing tone is to be played just once, iTtsDelayCount == 1,
    // otherwise iTtsDelayCount == KTtsDelayCount.
    TInt iTtsDelaysCount;

    // Array of players to be deleted asynchronously.
    RPointerArray<TMSRtAudioHdlr> iAsyncDeletePlayers;

    // DRM extend security flag.
    TBool iExtSecNeeded;

    // Async delete request is active.
    TBool iAsyncReq;

    // Ringing Type of TTS Player.
    TProfileRingingType iTtsRingingType;

    // Counter for TTS timeouts.
    TInt iTTsTimeOutCounter;

    // Voice call RT file size max.
    TInt iToneFileSizeLimitKB;

    MProEngEngine* iEngine;
    MProEngProfile* iProfile;
    };

} //namespace TMS

#endif // TMSRINGTONEPLAYER_H

// End of File
