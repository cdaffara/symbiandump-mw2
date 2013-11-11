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
* Description:  Defines the interface to control Call Audio
*
*/


#ifndef CALLAUDIOCONTROL_H
#define CALLAUDIOCONTROL_H

//  INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class CRoutingObserver;
class CDownlinkVolumeObserver;
class CUplinkGainObserver;

// CLASS DECLARATION

/**
* Interface for controlling call audio.
*
* This class provides interface for controlling a) audio routing to various
* connected output devices. b) speaker volume. c) microphone gain.
* This interface can be used to control active Circuit Switching call, Video
* Telephony call and Nokia internal VoIP calls.
*
* Limitations:
* 1. This inteface can only be used on Nokia devices running S60 3.1 OR 3.2
* platform.
* 2. Clients are requried to have MultimediaDD and WriteDeviceData capability to
*    use some of the functions defined in this interface. Check individual
*    function documentation for more information.
* 3. Some of the functions (routing, volume, and gain control) defined in this
*    interface can only be used during an active Circuit Switching OR Video
*    Telephony OR Nokia internal VoIP call.
*    If those functions are used when there is no active call, they return
*    KErrPermissionDenied. Check individual function documentation for more
*    information.
* 4. This API does not control 3rd party VoIP call created via APS or VoIP
*    Audio Services API.
* 5. When connected accessory list changes, initial routing happens
*    automatically.
*
* @lib CallAudioControl.lib
*
* @since Series 60 3.1, 3.2
*
*/

class CCallAudioControlImpl;

class CCallAudioControl : public CBase
    {
    public:  // DATA TYPES

        //Output types
        enum TAudioOutput
            {
            ENotActive,             /**< NotActive */
            ENone,                  /**< None */
            EHandset,               /**< Handset */
            ELoudspeaker,           /**< Loudspeaker */
            EWiredAudioAccessory,   /**< Wired Audio Accessory */
            EBTAudioAccessory,      /**< BT Audio Accessory */
            ETTY                    /**< TTY */
            };

    public:  // Constructors and destructor

        /**
        * Factory interface returning a new instance of CCallAudioControl.
        *
        * Leaves on failure.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @return CCallAudioControl*. A pointer to newly created object.
        *
        */
        IMPORT_C static CCallAudioControl* NewL();

        /**
        * Destrurctor
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        */
        IMPORT_C ~CCallAudioControl();

        /**
        * Appends routing observer to list.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param CRoutingObserver& aObserver. Reference to an object implementing
        *       CRoutingObserver interface.
        * @return TInt. KErrNone if observer is successfully appended to list.
        *       KErrAlready exists if aObserver is already in the list.
        *
        */
        IMPORT_C TInt AppendRoutingObserver( CRoutingObserver& aObserver );

        /**
        * Remove routing observer from list.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param CRoutingObserver& aObserver. Reference to an object implementing
        *       CRoutingObserver interface.
        * @return TInt. KErrNone if observer is successfully removed from list.
        *       KErrNotFound if aObserver is not in the list.
        *
        */
        IMPORT_C TInt RemoveRoutingObserver( CRoutingObserver& aObserver );

        /**
        * Appends downlink volume observer to list.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param CDownlinkVolumeObserver& aObserver. Reference to an object
        *       implementing CDownlinkVolumeObserver interface.
        * @return TInt. KErrNone if observer is successfully appended to list.
        *       KErrAlready exists if aObserver is already in the list.
        *
        */
        IMPORT_C TInt AppendDownlinkVolumeObserver( CDownlinkVolumeObserver& aObserver );

        /**
        * Remove downlink observer from list.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param CDownlinkVolumeObserver& aObserver. Reference to an object implementing
        *       CDownlinkVolumeObserver interface.
        * @return TInt. KErrNone if observer is successfully removed from list.
        *       KErrNotFound if aObserver is not in the list.
        *
        */
        IMPORT_C TInt RemoveDownlinkVolumeObserver( CDownlinkVolumeObserver& aObserver );

        /**
        * Appends uplink gain observer to list.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param CUplinkGainObserver& aObserver. Reference to an object
        *       implementing CUplinkVolumeObserver interface.
        * @return TInt. KErrNone if observer is successfully appended to list.
        *       KErrAlready exists if aObserver is already in the list.
        *
        */
        IMPORT_C TInt AppendUplinkGainObserver( CUplinkGainObserver& aObserver );

        /**
        * Remove uplink gain observer from list.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param CUplinkGainObserver& aObserver. Reference to an object implementing
        *       CUplinkGainObserver interface.
        * @return TInt. KErrNone if observer is successfully removed from list.
        *       KErrNotFound if aObserver is not in the list.
        *
        */
        IMPORT_C TInt RemoveUplinkGainObserver( CUplinkGainObserver& aObserver );

        /**
        * Returns array of currently available outputs.
        *
        * The array contains list of following outputs depending on whether the
        * output is available or not at the time of query:
        *  - EHandset
        *  - ELoudspeaker
        *  - EWiredAudioAccessory,
        *  - EWiredAccessory,
        *  - EBTAudioAccessory
        *  - ETTY
        * Audio can be routed to any of the available outputs in the list.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param TArray<TAudioOutput>& aAvailableOutputs. A reference to array
        *        of available outputs. On successful return will contain list of
        *        available outputs.
        * @return TInt - Error code. KErrNone if output list is returned
        *        succesfully. KErrPermissionDenied if call is not active.
        *
        */
        IMPORT_C TInt GetAvailableOutputs( RArray<CCallAudioControl::TAudioOutput>& aAvailableOutputs );


        /**
        * Returns the current output.
        *
        * The value returned represents output enumeration where call audio is
        * currently routed.
        *
        * Possible values are:
        *  - ENotActive : If output has not been set by any application or if
        *    output is disabled.
        *  - ENone : If user has destroyed audio link by long key press on the
        *    BT device.
        *  - EHandset or ELoudspeaker or EWiredAudioAccessory or
        *    EBTAudioAccessory or ETTY depending on the last successful
        *    SetOutput() call. EBTAudioAccessory is also returned when system
        *    routes audio from device to BT accessory automatically when user
        *    initiated long key press event occurs.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param TAudioOutput&. A reference to TAudioOutput type. On successful
        *        return will contain current output.
        * @return TInt - Error code. KErrNone if current output is returned
        *        succesfully. KErrPermissionDenied if call is not active.
        *
        */
        IMPORT_C TInt GetOutput( CCallAudioControl::TAudioOutput& aOutput );


        /**
        * Request to route call audio to a specific device.
        *
        * Sends an asynchronous request to set the output where call audio will
        * be routed. This request is completed only when the application receives
        * CCallAudioControlObserver::SetOutputComplete callback.
        *
        * If CCallAudioControlObserver::SetOutputComplete returns KErrNone,
        * aOutput requested by this application is configured by system.
        * Otherwise there was error configuring aOutput and application should
        * call CCallAudioControl::Output to get the active output.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability MultimediaDD
        *
        * @param aOutput : output requested by application. This can only be any
        *           of the outputs from AvailableOutputs() list or ENotActive.
        * @return TInt - Error code. KErrNone if output list is returned
        *        succesfully. KErrPermissionDenied if call is not active.
        *        KErrArgument when parameter is ENone or ENotActive.
        *        KErrHardwareNotAvailable if aOutput is not in the list of
        *        available outputs.
        *
        */
        IMPORT_C TInt SetOutput( CCallAudioControl::TAudioOutput aOutput );

        /**
        * Returns the previous output.
        *
        * Returns the second last output that was successfully configured to
        * play call audio by this client.
        * Possible values are:
        *  - ENotActive : If output had not been set or one of the call
        *                 apps had previously called SetOutput(ENotActive)
        *                 successfully.
        *  - EHandset or ELoudspeaker or EWiredAudioAccessory or
        *    EBTAudioAccessory or ETTY depending on the second last successful
        *    SetOutput() call.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param TAudioOutput&. A reference to TAudioOutput. On successful
        *       return will contain previous output that was configured for call
        *       audio. ENone will never be returned since call apps cannot call
        *       SetOutput(ENone).
        * @return TInt - Error code. KErrNone if output list is returned
        *        succesfully. KErrPermissionDenied if call is not active.
        *
        */
        IMPORT_C TInt PreviousOutput( CCallAudioControl::TAudioOutput& aPrevOutput );

        /**
        * Returns the minumum volume level that can be set on speaker for call
        * audio.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param TUint&. A reference to TUint. On successful return will contain
        *        minimum speaker volume level.
        * @return TInt - Error code. KErrNone if minimum volume is returned
        *        succesfully.
        *
        */
        IMPORT_C TInt GetMinDownlinkVolume( TUint& aVolume );

        /**
        * Returns the call audio speaker volume level that is used for call
        * audio.
        *
        * When a call is active, for devices on which volume can be set viz
        * EHandset, ELoudspeaker, EWiredAudioAccessory, this function returns the
        * volume level of the current active output device. For devices on which
        * volume cannot be set viz EBTAudioAccessory, ETTY, or ENone this function
        * returns KErrUnknown.
        *
        * When there is no active call, this function returns the volume level of
        * the earpiece that will be used for next active call.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param TUint&. A reference to TUint. On successful return will contain
        *        call audio volume level.
        * @return TInt - Error code. KErrNone if current volume is returned
        *        succesfully. KErrUnknown if current active device does not
        *        support volume control.
        *
        */
        IMPORT_C TInt GetDownlinkVolume( TUint& aVolume );

        /**
        * Returns the maximum volume level that can be set on speaker for call
        * audio.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param TUint&. A reference to TUint. On successful return will contain
        *        maximum speaker volume.
        * @return TInt - Error code. KErrNone if maximum volume is returned
        *        succesfully.
        *
        */
        IMPORT_C TInt GetMaxDownlinkVolume( TUint& aVolume );

        /**
        * Sets the speaker volume level for current call audio.
        *
        * The volume level specified should be in the range of minimum and
        * maximum volume level (both inclusive).
        *
        * Setting downlink volume to minimum will mute the downlink stream.
        *
        * When a call is active, for devices on which volume can be set viz
        * EHandset, ELoudspeaker, EWiredAudioAccessory, this function sets the
        * volume level of the current active output device. For devices on which
        * volume cannot be set viz EBTAudioAccessory, ETTY, or ENone this function
        * returns KErrNotSupported.

        * @since Series 60 3.1, 3.2
        *
        * @capability WriteDeviceData
        *
        * @param TUint. On successful return will set call audio volume level a
        *        aVolume.
        * @return TInt - Error code. KErrNone if current volume is returned
        *        succesfully. KErrPermissionDenied if call is not active.
        *
        */
        IMPORT_C TInt SetDownlinkVolume( TUint aVolume );

        /**
        * Returns call audio uplink (microphone) mute attribute.
        *
        * If returned value is ETrue, call audio uplink is muted. If
        * returned value is EFalse, call audio uplink is un-muted.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability None
        *
        * @param TBool&. A reference to TBool. On successful return will
        *        contain call audio mute attribute.
        * @return TInt - Error code. KErrNone if call audio is muted value is
        *       returned succesfully.
        *
        */
        IMPORT_C TInt GetUplinkMute( TBool& aMute );

        /**
        * Sets the call audio uplink (microphone) mute to on or off.
        *
        * Setting attribute to ETrue will mute call audio uplink. Calling this
        * function with EFalse will unmutes call audio uplink.
        *
        * @since Series 60 3.1, 3.2
        *
        * @capability WriteDeviceData
        *
        * @param TBool. On successful return will mute call audio of aMute is
        *        ETrue, else will unmute call audio.
        * @return TInt - Error code. KErrNone if call audio is muted or unmuted
        *       succesfully. KErrPermissionDenied if call is not active.
        *
        */
        IMPORT_C TInt SetUplinkMute( TBool aMute );

    private:

        void ConstructL();
        CCallAudioControl();

        //Data
        CCallAudioControlImpl* iImpl;

    };

#endif      // CALLAUDIOCONTROL_H

// End of File
