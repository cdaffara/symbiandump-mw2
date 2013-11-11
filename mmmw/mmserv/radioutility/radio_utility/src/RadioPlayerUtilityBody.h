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
* Description:  Implementation header of the Radio Player Utility API
*
*
*/


#ifndef C_RADIOPLAYERUTILITYBODY_H
#define C_RADIOPLAYERUTILITYBODY_H

#include <RadioPlayerUtility.h>
#include <MCustomInterface.h>

class RRadioSession;

/**
 *  This class encapsulates the implementation body of CRadioPlayerUtility interface.
 *
 *  @lib Radio_Utility.lib
 *  @since S60 3.2
 */
class CRadioPlayerUtility::CBody : public CBase,
                                   public MCustomInterface
    {

public:

    static CBody*  NewL( RRadioSession& aRadioSession, MRadioPlayerObserver& aObserver );

    ~CBody();

    /**
     * Retrieve the current state of the player.
     * If the radio is already playing, client should simply retrieve current settings such
     * as volume, etc.
     *
     * @since S60 3.2
     * @return Radio player state.
     */
     void PlayerState( TPlayerState& aPlayerState ) const;

    /**
     * Close the radio player and free all memory allocated. Any ongoing playing activity will
     * be stopped if no other clients need it.
     *
     * @since S60 3.2
     */
     void Close();

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
     void Play();

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
     void Stop();

    /**
     * Get the maximum volume supported by the device. Volume range from 0 to max.
     * Setting volume to zero will mute the speakers.
     *
     * @since S60 3.2
     * @param aMaxVolume On return contains the maximum volume supported.
     * @return A standard system error code.
     */
     TInt GetMaxVolume( TInt& aMaxVolume ) const;

    /**
     * Set the volume to the specified level. This can be any value from zero to max returned
     * by GetMaxVolume.
     *
     * @since S60 3.2
     * @param aVolume The volume level to set.
     * @return A standard system error code.
     */
     TInt SetVolume( TInt aVolume );

    /**
     * Get the current volume.
     *
     * @since S60 3.2
     * @param aVolume On return contains the current volume.
     * @return A standard system error code.
     */
     TInt GetVolume( TInt& aVolume ) const;

    /**
     * Set a volume ramp. Must be called before Play.
     * @param aRampInterval
     *        The time interval over which the volume should be increased from zero to
     *        the current volume setting.
     *
     * @since S60 3.2
     * @return A standard system error code.
     */
     TInt SetVolumeRamp( const TTimeIntervalMicroSeconds& aRampInterval );

    /**
     * Mutes or unmutes radio playback.
     *
     * @param aMute ETrue to mute the audio, EFalse to unmute it.
     * @since S60 3.2
     * @return A standard system error code.
     */
     TInt Mute( TBool aMute );

    /**
     * Find out if the audio is muted or not.
     *
     * @since S60 3.2
     * @return ETrue if audio is muted.
     */
     TBool IsMute();

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
     TInt SetBalance( TInt aLeftPercentage, TInt aRightPercentage );

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
     TInt GetBalance( TInt& aLeftPercentage, TInt& aRightPercentage ) const;

    /**
     * Retrieves a custom interface to the device. (Used by Audio Enhancement)
     *
     * @since S60 3.2
     * @param aInterfaceId The interface UID, defined with the custom interface.
     * @return A pointer to the interface implementation, or NULL if the device does not
     * implement the interface requested. The return value must be cast to the correct
     * type by the user.
     */
     TAny* CustomInterface( TUid InterfaceId );


private:

    void ConstructL();
    CBody( RRadioSession& aRadioSession, MRadioPlayerObserver& aObserver );

private: // data

    // The radio server session
    RRadioSession& iRadioSession;
    // Player utility observer
    MRadioPlayerObserver& iRadioPlayerUtilityClient;
    };

#endif  // C_RADIOPLAYERUTILITY_H

// End of File
