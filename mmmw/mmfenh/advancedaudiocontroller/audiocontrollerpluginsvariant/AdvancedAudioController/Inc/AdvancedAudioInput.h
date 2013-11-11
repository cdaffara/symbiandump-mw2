/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the base class from which specific audio
*                input classes are derived. This class encapsulates common
*                behavior for all audio inputs. An audio input class serves as
*                the interface between the controller and the lower-level sound
*                gathering interface.
*
*/



#ifndef CADVANCEDAUDIOINPUT_H
#define CADVANCEDAUDIOINPUT_H

//  INCLUDES
#include <e32base.h>
//#include <e32std.h>
//#include <MmfControllerFrameworkBase.h>
//#include <MmfControllerFramework.h>
//#include <MmfBase.h>
//#include <mmfpaniccodes.h>
#include "AdvancedAudioEncoder.h"

// CLASS DECLARATION

/**
* Mixin observer class to define an object capable of handling a callback from
* AdvancedAudioInput.
*
* @lib AdvancedAudioInput.lib
* @since 3.0
*/
class MAdvancedAudioInputObserver
    {
    public:
        /**
        * Callback function from CDevSoundAudioInput when the given buffer needs to
        * be emptied (written to external storage)
        * @param   aBuffer  data buffer to be emptied
        * @return  errCode  KErrNone (0)
        */
        virtual TInt EmptyBufferL( CMMFBuffer* aBuffer ) = 0;

        /**
        * Callback function from CDevSoundAudioInput to indicate recording is completed
        * @param   none
        * @return  void
        */
        virtual void RecordComplete() = 0;

        /**
        * Callback function from CDevSoundAudioInput to handle specified event
        * @param   aEvent  the event to be handled
        * @return  void
        */
        virtual void SendEvent( const TMMFEvent& aEvent ) = 0;
    };

/**
*  This is the base class for all audio input classes used for recording.
*  It contains common behaviour for all audio Inputs. The audio Input class
*  provides the interface between the Controller and audio input device.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class CAdvancedAudioInput : public CBase,
							public MAdvancedAudioEncoderObserver
    {
    public: // New functions

        /**
        * Prepares DevSound Audio Input for recording
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void PrimeL() = 0;

        /**
        * Handles request to pause recording. Sends pause command to DevSound.
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void PauseL() = 0;

        /**
        * Handles request to start recording. Configures and initializes DevSound and
        * sends first buffer of audio data to DevSound,
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void RecordL( TAny* aBuffer ) = 0;

        /**
        * Handles request to stop recording. Sends stop command to DevSound.
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void StopL() = 0;

        /**
        * Handles request to set the maximum gain. The new value is sent to DevSound.
        * @since 3.0
        * @param   none
        * @return  the maximum gain value
        */
        IMPORT_C virtual TInt MaxGainL() = 0;

        /**
        * Handles request to set gain. The new value is sent to DevSound.
        * @since 3.0
        * @param   @param   aGain  new gain value
        * @return  void
        */
        IMPORT_C virtual void SetGainL( TInt aGain ) = 0;

        /**
        * Handles request to get the current gain. The current gain is obtained from DevSound.
        * @since 3.0
        * @param   void
        * @return  the current gain value
        */
        IMPORT_C virtual TInt GainL() = 0;

        /**
        * Handles request to get all configured sampling rates from DevSound.
        * This is a request for all sampling rates that DevSound is currently
        * configured with.
        * @since 3.0
        * @param   aRates  array to hold the retrieved sampling rates
        * @return  void
        */
        IMPORT_C virtual void ConfigRatesL( RArray<TUint>& aRates ) = 0;

        /**
        * Handles request to get all configured channels from DevSound.
        * This is a request for all channels that DevSound is currently
        * configured with.
        * @since 3.0
        * @param   aChannels  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void ConfigChannelsL( RArray<TUint>& aChannels ) = 0;

        /**
        * Handles request to get all configured data types (FourCC) from DevSound.
        * This is a request for all data types that DevSound is currently
        * configured with.
        * @since 3.0
        * @param   aDataTypes  array to hold the retrieved data types
        * @return  void
        */
        IMPORT_C virtual void ConfigDataTypesL( RArray<TFourCC>& aDataTypes ) = 0;

        /**
        * Handles request to get all capablities sampling rates from DevSound.
        * This contains all sampling rates DevSound is capable of supporting
        * @since 3.0
        * @param   aRates  array to hold the retrieved sampling rates
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesRatesL( RArray<TUint>& aRates ) = 0;

        /**
        * Handles request to get all capabilities channels from DevSound.
        * This is a request for all channels that DevSound is capable of
        * supporting.
        * @since 3.0
        * @param   aChannels  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesChannelsL( RArray<TUint>& aChannels ) = 0;

        /**
        * Handles request to get all capabilities data types (FourCC) from DevSound.
        * This is a request for all data types that DevSound is capable of
        * supporting.
        * @since 3.0
        * @param   aDataTypes  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesDataTypesL( RArray<TFourCC>& aDataTypes ) = 0;

        /**
        *
        * Handles request to calculate current record position in terms to micro seconds
        * @since 3.0
        * @param   none
        * @return  Current position in microseconds
        */
        IMPORT_C virtual TInt64 CalculateAudioInputPositionL() const = 0;

        /**
        * Handles request to configure DevSound with the given parameters.
        * @since 3.0
        * @param   aSampleRate       recording sampling rate
        * @param   aNumChannel       required number of channels (mono/stereo indication)
        * @param   aFourCC           FourCC of required recording format
        * @param   aCodecConfigData  specific codec configuration data
        * @return  void
        *
        */
        IMPORT_C virtual void ConfigureL( TUint aSampleRate, TUint aNumChannel, TFourCC aFourCC,
                                          const RArray<TInt>& aCodecConfigData ) = 0;

        /**
        * Sets the Encoder instance to be used by the audio input.
        * @since 3.0
        * @param aEncoder Encoder instance
        * @return void
        */
        IMPORT_C virtual void SetEncoder( CAdvancedAudioEncoder* aEncoder ) = 0;

        /**
        * Sets the bit rate to be used by the audio input.
        * @since 3.0
        * @param   aBitRate          recording bit rate
        * @param   aCodecConfigData  specific codec configuration data
        * @return void
        */
        IMPORT_C virtual void SetBitRateL( TUint aBitRate, const RArray<TInt>& aCodecConfigData ) = 0;

	protected: // Data

        // pointer to AdvancedAudioInput observer
        MAdvancedAudioInputObserver* iObserver;

        // pointer to an audio encoder
        CAdvancedAudioEncoder* iAdvancedAudioEncoder;
	};

#endif		// CADVANCEDAUDIOINPUT_H
