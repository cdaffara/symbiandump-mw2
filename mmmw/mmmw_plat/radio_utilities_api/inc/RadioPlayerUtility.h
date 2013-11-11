/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class provides an interface for controlling radio playback,
*                audio settings such as volume, balance, etc. In addition, this
*                class implements Custom Command API so clients can utilize it
*                with Audio Enhancement features provides by MultiMedia Framework (MMF).
*
*
*/



#ifndef C_RADIOPLAYERUTILITY_H
#define C_RADIOPLAYERUTILITY_H

#include <RadioUtility.h>
#include <MCustomInterface.h>

class RRadioSession;

/**  Player state */
enum TPlayerState
    {
	/**  Radio is idle (i.e. not playing) */
    ERadioPlayerIdle,
    /**  Radio is playing */
    ERadioPlayerPlaying
    };


/**
 *  This class provides the interface class to be implemented by object receiving request
 *  complete notification from radio player utility. These functions are called once the
 *  asynchronous request completes successfully or with an error.
 *
 *  In addition, this class provides the interface for receiving events about radio playback.
 *
 *  @since S60 3.2
 */
class MRadioPlayerObserver
    {
public:

    /**
     * Radio state changed. This event is asynchronous and is received after a call to
	 * CRadioPlayerUtility::Play and CRadioPlayerUtility::Stop. This event is also generated when
	 * the radio is disrupted due to an error.
	 *
     * If the radio is disrupted due to an error, appropriate error code will be sent.
     * The following are the most common error codes: KErrDied when radio is pre-empted by another
     * application with higher priority, KErrHardwareNotAvailable when tuner is forced off
     * (e.g. when FM transmitter is turned on).
     *
     * @since S60 3.2
     * @param aState Radio player state
     * @param aError A standard system error code, only used when aState is ERadioPlayerIdle
     */
    virtual void MrpoStateChange( TPlayerState aState, TInt aError ) = 0;

    /**
     * Called when volume changes. This may be caused by other applications.
     *
     * @since S60 3.2
     * @param aVolume Current volume.
     */
    virtual void MrpoVolumeChange( TInt aVolume ) = 0;

    /**
     * Called when mute setting changes. This may be caused by other applications.
     *
     * @since S60 3.2
     * @param aMute ETrue indicates audio is muted.
     */
    virtual void MrpoMuteChange( TBool aMute ) = 0;

    /**
     * Called when balance setting changes. This may be caused by other applications.
     *
     * @since S60 3.2
     *        Left speaker volume percentage. This can be any value from zero to 100.
     *        Zero value means left speaker is muted.
     * @param aRightPercentage
     *        Right speaker volume percentage. This can be any value from zero to 100.
     *        Zero value means right speaker is muted.
     */
    virtual void MrpoBalanceChange( TInt aLeftPercentage, TInt aRightPercentage ) = 0;
    };


/**
 *  This class provides an interface to initiate and control playback of audio from the tuner.
 *
 *  @lib RadioUtility.lib
 *  @since S60 3.2
 */
class CRadioPlayerUtility : public CBase,
                            public MCustomInterface
    {
    friend class CRadioUtility::CBody;

public:

    /**
     * Retrieve the current state of the player.
     * If the radio is already playing, client should simply retrieve current settings such
     * as volume, etc.
     *
     * @since S60 3.2
     * @return Radio player state.
     */
    IMPORT_C TPlayerState PlayerState() const;

    /**
     * Close the radio player and free all memory allocated. Any ongoing playing activity will
     * be stopped if no other clients need it.
     *
     * @since S60 3.2
     */
	IMPORT_C void Close();

    /**
     * Asynchronous request to start playing the radio.
     *
     * User must be ready to receive callback method MrpoStateChange.
     * If radio plays successfully, the user will receive callback method MrpoStateChange with
     * ERadioPlayerPlaying and KErrNone. If a problem is encountered, user will receive
     * callback MrpoStateChange with ERadioPlayerIdle with an error code.
     *
     * @since S60 3.2
     * @see MRadioPlayerObserver::MrpoStateChange
     */
    IMPORT_C void Play();

    /**
     * Asynchronous request to stop the radio.
     *
     * This will stop radio for all client applications playing the radio. If the application
     * wishes to terminate without disrupting the radio playback (in case of multiple
     * active applications running), the application should terminate without issuing Stop.
     * User must be ready to receive callback method MrpoStateChange.
     *
     * @since S60 3.2
     * @see MRadioPlayerObserver::MrpoStateChange
     */
    IMPORT_C void Stop();

    /**
     * Get the maximum volume supported by the device. Volume range from 0 to max.
     * Setting volume to zero will mute the speakers.
     *
     * @since S60 3.2
     * @param aMaxVolume On return contains the maximum volume supported.
     * @return A standard system error code.
     */
    IMPORT_C TInt GetMaxVolume( TInt& aMaxVolume ) const;

    /**
     * Set the volume to the specified level. This can be any value from zero to max returned
     * by GetMaxVolume.
     *
     * @since S60 3.2
     * @param aVolume The volume level to set.
     * @return A standard system error code.
     */
    IMPORT_C TInt SetVolume( TInt aVolume );

    /**
     * Get the current volume.
     *
     * @since S60 3.2
     * @param aVolume On return contains the current volume.
     * @return A standard system error code.
     */
    IMPORT_C TInt GetVolume( TInt& aVolume ) const;

    /**
     * Set a volume ramp. Must be called before Play.
     * @param aRampInterval
     *        The time interval over which the volume should be increased from zero to
     *        the current volume setting.
     *
     * @since S60 3.2
     * @return A standard system error code.
     */
    IMPORT_C TInt SetVolumeRamp( const TTimeIntervalMicroSeconds& aRampInterval );

    /**
     * Mutes or unmutes radio playback.
     *
     * @param aMute ETrue to mute the audio, EFalse to unmute it.
     * @since S60 3.2
     * @return A standard system error code.
     */
    IMPORT_C TInt Mute( TBool aMute );

    /**
     * Find out if the audio is muted or not.
     *
     * @since S60 3.2
     * @return ETrue if audio is muted.
     */
    IMPORT_C TBool IsMute();

    /**
     * Set the speaker balance for playing.
     *
     * @since S60 3.2
     * @param aLeftPercentage
     *        Left speaker volume percentage. This can be any value from zero to 100.
     *        Setting a zero value mutes the sound on left speaker.
     * @param aRightPercentage
     *        Right speaker volume percentage. This can be any value from zero to 100.
     *        Setting a zero value mutes the sound on right speaker.
     * @return A standard system error code.
     */
    IMPORT_C TInt SetBalance( TInt aLeftPercentage, TInt aRightPercentage );

    /**
     * Get the speaker balance set for playing.
     *
     * @since S60 3.2
     * @param aLeftPercentage
     *        On return contains left speaker volume perecentage. This can be any value
     *        from zero to 100. Zero value means left speaker is muted.
     * @param aRightPercentage
     *        On return contains right speaker volume perecentage. This can be any value
     *        from zero to 100. Zero value means right speaker is muted.
     * @return A standard system error code.
     */
    IMPORT_C TInt GetBalance( TInt& aLeftPercentage, TInt& aRightPercentage ) const;

    /**
     * Retrieves a custom interface to the device. (Used by Audio Enhancement)
     *
     * @since S60 3.2
     * @param aInterfaceId The interface UID, defined with the custom interface.
     * @return A pointer to the interface implementation, or NULL if the device does not
     * implement the interface requested. The return value must be cast to the correct
     * type by the user.
     */
    IMPORT_C TAny* CustomInterface( TUid aInterfaceId );

private:

    /**
     * Factory function to create a new instance of the radio player utility.
     *
     * @since S60 3.2
     * @param aRadioSession Reference to radio session
     * @param aObserver The observer object for receiving async completion callbacks
     * @return A new radio player utility object.
     */
    static CRadioPlayerUtility* NewL( RRadioSession& aRadioSession, MRadioPlayerObserver& aObserver );

    ~CRadioPlayerUtility();

    CRadioPlayerUtility();

    void ConstructL();

private: // data

    class CBody;

    /**
     * Radio player utility implementation body
     * Own.
     */
    CBody* iBody;
	};

#endif	// C_RADIOPLAYERUTILITY_H

// End of File