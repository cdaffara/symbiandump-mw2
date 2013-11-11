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
 * Description:  Ring Tone Audio playback handler.
 *
 */

#ifndef TMSRTAUDIOHDLR_H
#define TMSRTAUDIOHDLR_H

// INCLUDES

#include <AudioOutput.h>
//#include <c3dringingtoneinterface.h>
#include <mdaaudiosampleplayer.h>
#include <mdaaudiotoneplayer.h>

namespace TMS {

// CLASS DECLARATION
class TMSRtAudioHdlrObsrv;
class TMSRtContainer;

/**
 *  RingTone audio handler.
 *  Handles audio for tone sequence and digital sample playback.
 */
class TMSRtAudioHdlr : public CBase,
                       public MMdaAudioPlayerCallback,
                       public MMdaAudioToneObserver
    {
public:

    enum TAudioPlayerStatus
        {
        EToneLoading,
        EToneReady,
        ETonePlaying
        };

    enum TRingingType
        {
        ETypeRinging,
        ETypeRingingOnce,
        ETypeAscending
        };

public:
    /**
     * Two-phased constructor.
     * @param aRingingTone The ringing tone
     * @param aPriority The priority for the playing
     * @param aPreference The preference value
     * @param aObserver Audio player observer
     * @param aType The id of this audio player
     * @param aExtSecNeeded ETrue if DRM extend security is needed
     * @return The instance of the audio player.
     */
    static TMSRtAudioHdlr* NewL(const TMSRtContainer& aRingingTone,
            TUint aPriority, TUint aPreference,
            TMSRtAudioHdlrObsrv& aObserver, TInt aType = 0,
            TBool aExtSecNeeded = EFalse);

    /**
     * Two-phased constructor.
     * @param aFileName The ringing tone file name.
     * @param aPriority The priority for the playing
     * @param aPreference The preference value
     * @param aObserver Audio player observer
     * @param aType The id of this audio player
     * @param aExtSecNeeded ETrue if DRM extend security is needed
     * @return The instance of the audio player.
     */
    static TMSRtAudioHdlr* NewL(const TDesC& aFileName, TUint aPriority,
            TUint aPreference, TMSRtAudioHdlrObsrv& aObserver,
            TInt aType = 0, TBool aExtSecNeeded = EFalse);

    /**
     * Two-phased constructor, take audio from descriptor.
     *
     * @param aSequence sequence to be played.
     * @param aPriority priority of tone.
     * @param aPreference preference of tone.
     * @param aObserver observer.
     * @param aType identifier, used in observer interface calls.
     * @return The instance of the audio player.
     */
    static TMSRtAudioHdlr* NewSeqL(const TDesC8& aSequence,
            TUint aPriority, TUint aPreference,
            TMSRtAudioHdlrObsrv& aObserver, TInt aType = 0);

    /**
     * Two-phased constructor, takes Text-To-Speech text to be played.
     * @param aText Text to be pronounced.
     * @param aPriority priority of tone.
     * @param aPreference preference of tone.
     * @param aObserver observer.
     * @param aType identifier, used in observer interface calls.
     * @return The instance of the audio player.
     */
    static TMSRtAudioHdlr* NewTtsL(const TDesC& aText, TUint aPriority,
            TUint aPreference, TMSRtAudioHdlrObsrv& aObserver, TInt aType = 0);

    /**
     * Destructor.
     */
    ~TMSRtAudioHdlr();

public:
    // new function

    /**
     * Play the audio file or tone
     * @param aRingingType
     * @param aVolume take value from 1 to 10
     * @param aTTsToBePlayed
     */
    void Play(TRingingType aRingType, TInt aVolume, TBool aTTsToBePlayed);

    /**
     * Stop current file playing
     */
    void StopPlaying();

    /**
     * Sets new volumes and ramptimes.
     * @param aVolume -volume to be set
     * @param aRamptime -ramptime to be set.
     */
    void SetNewVolumeAndRamptime(TInt aVolume, TInt aRamptime);

    /**
     * Re starts playing.
     */
    void ReStartPlaying();
    /**
     * Enable or disable TTs playing.
     * @param aTTsToBePlayed
     */
    void SetTTsToBePlayed(const TBool aTTsToBePlayed);

    /**
     * Sets volume level to zero.
     */
    void MutePlaying();

public:
    /**
     * from MMdaAudioPlayerCallback
     */
    virtual void MapcInitComplete(TInt aError,
            const TTimeIntervalMicroSeconds& aDuration);

    /**
     * from MMdaAudioPlayerCallback
     */
    virtual void MapcPlayComplete(TInt aError);

    /**
     * from MMdaAudioToneObserver
     */
    virtual void MatoPrepareComplete(TInt aError);

    /**
     * from MMdaAudioToneObserver
     */
    virtual void MatoPlayComplete(TInt aError);

private:

    enum TAudioDataFormat
        {
        EFormatTone,
        EFormatSample,
        EFormatTts
        };

    /**
     * C++ constructor.
     */
    TMSRtAudioHdlr(TAudioDataFormat aDataFormat,
            TMSRtAudioHdlrObsrv& aObserver, TUint aPriority,
            TUint aPreference, TInt aType);

    /**
     * Symbian OS constructor.
     */
    void ConstructL(const TDesC& aFileName);

    /**
     * Symbian OS constructor.
     */
    void ConstructSeqL(const TDesC8& aSequence);

    /**
     * Symbian OS constructor for the Text-To-Speech option
     */
    void ConstructTtsL(const TDesC& aTtsText, TUint aPriority,
            TUint aPreference);

    /**
     * Do the playing stuff
     */
    void DoPlay();

    /**
     * Set ringing type properties
     */
    void SetRingingTypeProperties();

    /**
     * Call back function for timer
     */
    static TInt TimerCallBack(TAny* aAny);

    /**
     * Handle timer events
     */
    void HandleTimerEvent();

    /*
     * Convert volume to the scale used by hardware
     * @param aVolume volume to be altered
     * @return TInt   new volume
     */
    TInt ConvertVolume(TInt aVolume);

private:
    // Data

    // The format of audio data, audio sample file or tone file
    TAudioDataFormat iFormat;

    // Observer for error handling
    TMSRtAudioHdlrObsrv& iObserver;

    // Priority. It's only used in playing tone file
    const TInt iPriority;

    // Preference. Only used for tone file
    const TUint iPreference;

    // Identifier number
    TInt iRtType;

    // Player for tone file.
    CMdaAudioToneUtility* iTonePlayer;

    // Player for audio file.
    CMdaAudioPlayerUtility* iSamplePlayer;

    // Player for the Text-To-Speech texts
    CMdaAudioPlayerUtility* iTtsPlayer;

    // The status of the player
    TAudioPlayerStatus iPlayerStatus;

    // How does the file played. It's only used if the player
    // is playing the file
    TRingingType iRingType;

    // Volume
    TInt iVolume;

    // For ascending and descending used for set current volume
    TInt iCurrentVolume;

    // Used by sample player
    TBool iToBePlaying;

    // Store for sequence.
    HBufC8* iSequence;

    // Store for the TTS source string
    HBufC8* iTtsText;

    // 3D Audio ringing tone plugin
    //C3DRingingToneInterface* i3DPlugin;

    // Ringingtone output
    //CAudioOutput* iAudioOutput;

    // Flag to  indicate whether we need to play TTs or not
    TBool iTTsToBePlayed;
    };

} //namespace TMS

#endif //TMSRTAUDIOHDLR_H

// End of File
