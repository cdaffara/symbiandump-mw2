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
* Description:  This file contains the definition of the DevSound Audio Input
*				 class.
*
*/



#ifndef CDEVSOUNDAUDIOINPUT_H
#define CDEVSOUNDAUDIOINPUT_H

// INCLUDES
#include "AdvancedAudioInput.h"
#include <sounddevice.h>

// CLASS DECLARATIONS

/**
* This class defines audio input functions specific to DevSound used for recording.
*
* @lib DevSoundAudioInput.lib
* @since 3.0
*/
class CDevSoundAudioInput : public CAdvancedAudioInput,
                        	public MDevSoundObserver
    {
    public:  // Constructors and destructor

       /**
        * Class instantiation function
        * @param   aPrioritySettings  priority settings for devSound and policy
        * @param   aObserver          observer class that will handle all DevSoundAudioInput
        *                             callbacks
        * @return  pointer to the instance created
        */
        IMPORT_C static CDevSoundAudioInput* NewL( const TMMFPrioritySettings& aPrioritySettings,
                                                   MAdvancedAudioInputObserver& aObserver,
                                                   CMMFDevSound& aMMFDevSound );
        /**
        * Destructor function
        * @param   none
        * @return  none
        */
        IMPORT_C virtual ~CDevSoundAudioInput();

    public: // Functions from base classes (CAdvancedAudioInput)

        /**
        * From CAdvancedAudioInput
        * Prepares DevSound Audio Input for recording
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void PrimeL();

        /**
        * From CAdvancedAudioInput
        * Handles request to pause recording. Sends pause command to DevSound.
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void PauseL();

        /**
        * From CAdvancedAudioInput
        * Handles request to start recording. Configures and initializes DevSound
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void RecordL( TAny* aBuffer );

        /**
        * From CAdvancedAudioInput
        * Handles request to stop recording. Sends stop command to DevSound.
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void StopL();

        /**
        * From CAdvancedAudioInput
        * Handles request to set priority. The given priority settings are sent to DevSound.
        * @since 3.0
        * @param   aPrioritySettings  new priority settings
        * @return  none
        */
		IMPORT_C virtual void SetPrioritySettingsL( const TMMFPrioritySettings& aPrioritySettings );

        /**
        * From CAdvancedAudioInput
        * Handles request to set the maximum gain. The new value is sent to DevSound.
        * @since 3.0
        * @param   none
        * @return  the maximum gain value
        */
        IMPORT_C virtual TInt MaxGainL();

        /**
        * Handles request to set gain. The new value is sent to DevSound.
        * @since 3.0
        * @param   @param   aGain  new gain value
        * @return  void
        */
        IMPORT_C virtual void SetGainL( TInt aGain );

        /**
        * From CAdvancedAudioInput
        * Handles request to get the current gain. The current gain is obtained from DevSound.
        * @since 3.0
        * @param   void
        * @return  the current gain value
        */
        IMPORT_C virtual TInt GainL();

        /**
        * From CAdvancedAudioInput
        * Handles request to get all configured sampling rates from DevSound.
        * This is a request for all sampling rates that DevSound is currently
        * configured with.
        * @since 3.0
        * @param   aRates  array to hold the retrieved sampling rates
        * @return  void
        */
        IMPORT_C virtual void ConfigRatesL( RArray<TUint>& aRates );

        /**
        * From CAdvancedAudioInput
        * Handles request to get all configured channels from DevSound.
        * This is a request for all channels that DevSound is currently
        * configured with.
        * @since 3.0
        * @param   aChannels  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void ConfigChannelsL( RArray<TUint>& aChannels );

        /**
        * From CAdvancedAudioInput
        * Handles request to get all configured data types (FourCC) from DevSound.
        * This is a request for all data types that DevSound is currently
        * configured with.
        * @since 3.0
        * @param   aDataTypes  array to hold the retrieved data types
        * @return  void
        */
        IMPORT_C virtual void ConfigDataTypesL( RArray<TFourCC>& aDataTypes );

        /**
        * From CAdvancedAudioInput
        * Handles request to get all capablities sampling rates from DevSound.
        * This contains all sampling rates DevSound is capable of supporting
        * @since 3.0
        * @param   aRates  array to hold the retrieved sampling rates
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesRatesL( RArray<TUint>& aRates );

        /**
        * From CAdvancedAudioInput
        * Handles request to get all capabilities channels from DevSound.
        * This is a request for all channels that DevSound is capable of
        * supporting.
        * @since 3.0
        * @param   aChannels  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesChannelsL( RArray<TUint>& aChannels );

        /**
        * Handles request to get all capabilities data types (FourCC) from DevSound.
        * This is a request for all data types that DevSound is capable of
        * supporting.
        * @since 3.0
        * @param   aDataTypes  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesDataTypesL( RArray<TFourCC>& aDataTypes );

        /**
        * From CAdvancedAudioInput
        * Handles request to calculate current record position in terms to micro seconds
        * @since 3.0
        * @param   none
        * @return  Current position in microseconds
        */
        IMPORT_C virtual TInt64 CalculateAudioInputPositionL() const;

        /**
        * From CAdvancedAudioInput
        * Handles request to configure DevSound with the given parameters.
        * @since 3.0
        * @param   aSampleRate       recording sampling rate
        * @param   aNumChannel       required number of channels (mono/stereo indication)
        * @param   aFourCC           FourCC of required recording format
        * @param   aCodecConfigData  specific codec configuration data
        *
        * @return  void
        *
        */
        IMPORT_C virtual void ConfigureL( TUint aSampleRate, TUint aNumChannel, TFourCC aFourCC,
                                          const RArray<TInt>& aCodecConfigData );

        /**
        * Sets the Encoder instance to be used by the audio output.
        * @since 3.0
        * @param aEncoder Encoder instance
        * @return void
        */
        IMPORT_C virtual void SetEncoder( CAdvancedAudioEncoder* aEncoder );

        /**
        * From CAdvancedAudioInput
        * Sets the bit rate to be used by the audio input.
        * @since 3.0
        * @param   aBitRate          recording bit rate
        * @param   aCodecConfigData  specific codec configuration data
        * @return void
        */
        IMPORT_C virtual void SetBitRateL( TUint aBitRate, const RArray<TInt>& aCodecConfigData );

    public: // Functions from base classes (MDevSoundObserver)

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate initialization has completed
        * @since 2.0
        * @param   aError  error code returned by DevSound (KErrNone if no error)
        * @return  void
        */
        virtual void InitializeComplete( TInt aError );

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate tone playback has completed
        * since 2.0
        * @param   aError  error code returned by DevSound (KErrNone if no error)
        * @return  void
        */
        virtual void ToneFinished( TInt aError );

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate the spcecified buffer needs to be filled.
        * (Applicable to playback only)
        * @since 2.0
        * @param   aBuffer  buffer to place playback data
        * @return  void
        */
        virtual void BufferToBeFilled( CMMFBuffer* aBuffer );

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate playback has completed successfully
        * or otherwise.
        * @since 2.0
        * @param   aError  error code returned by DevSound (KErrNone if no error)
        * @return  void
        */
        virtual void PlayError( TInt aError );

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate the specified buffer has been filled and
        * needs to be emptied. (Applicable for Recording only)
        * @since 2.0
        * @param   aBuffer  buffer with recorded data that needs to be emptied
        * @return  void
        */
        virtual void BufferToBeEmptied( CMMFBuffer* aBuffer );

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate error has occurred during recording
        * (for record only)
        * @since 3.0
        * @param   aError  error code returned by DevSound (KErrNone if no error)
        * @return  void
        */
        virtual void RecordError( TInt aError );

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate conversion from one format to another
        * has completed successfully or otherwise.
        * (during conversion only)
        * @since 3.0
        * @param   aError  error code returned by DevSound (KErrNone if no error)
        * @return  void
        */
        virtual void ConvertError( TInt aError );

        /**
        * From MDevSoundObserver
        * Callback function from hardware device passed through via DevSound
        * @since 3.0
        * @param   aMessageType  type of message
        * @param   aMsg          message contents
        * @return  void
        */
        virtual void DeviceMessage( TUid aMessageType, const TDesC8& aMsg );

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to request given event be sent to client
        * @since 3.0
        * @param   aEvent  event to send
        * @return  void
        */
        virtual void SendEventToClient( const TMMFEvent& aEvent );

        /**
        * From MAdvancedAudioEncoderObserver
        * Called by a aAdvancedAudioEncoder object when the specifed buffer has been
        * emptied (consumed).
        * @since 3.0
        * @param @param   aEmptyBuffer an empty data buffer
        * @return void
        */
        virtual void BufferEmptied();

        /**
        * From MAdvancedAudioEncoderObserver
        * Callback function from aAdvancedAudioEncoder when the specified buffer
        * needs to be emptied. This indicates a shared buffer is to be emptied.
        * @since 3.0
        * @param   aEmptyBuffer a buffer to be emptied
        * @return  -
        *
        */
        virtual void EmptyBuffer( CMMFBuffer* aEmptyBuffer );

        /**
        * From MAdvancedAudioEncoderObserver
        * Callback function from aAdvancedAudioEncoder to handle specified event
        * @since 3.0
        * @param aEvent The event to be handled
        * @return -
        */
		virtual void SendEvent( const TMMFEvent& aEvent );

    private:

        /**
        * C++ default constructor.
        */
        CDevSoundAudioInput(CMMFDevSound& aMMFDevSound);

        /**
        *
        * Phase 2 of two-phase constructor
        *
        * @param   aPrioritySettings  priority settings for devSound and policy
        * @param   aObserver          observer class that will handle all DevSoundAudioInput
        *                             callbacks
        *
        * @return  void
        *
        */
        void ConstructL( const TMMFPrioritySettings& aPrioritySettings,
						 MAdvancedAudioInputObserver& aObserver );

		/**
        * Builds configuration data for DevSound and codec
        * @since 3.0
        * @return  void
        */
        void BuildConfigurationParameters();

        /**
        *
        * Converts DevSound bit representation of sampling rates to integer values.
        *
        * @param   aDevSoundCaps  DevSound capabilities structure
        * @param   aSampleRates   array to hold the retrieved sampling rates
        *
        * @return  void
        *
        */
		void ConvertFromDevSoundCapsToSampleRatesL( const TMMFCapabilities& aDevSoundCaps,
													RArray<TUint>& aSampleRates );

        /**
        *
        * Converts DevSound representation of data types to FourCC values.
        *
        * @param   aDevSoundCaps  DevSound capabilities structure
        * @param   aDataTypes     array to hold the retrieved data types
        *
        * @return  void
        *
        */
		void ConvertFromDevSoundCapsToDataTypesL( const TMMFCapabilities& aDevSoundCaps,
												  RArray<TFourCC>& aDataTypes );

        /**
        *
        * Converts DevSound representation of channels to integer values.
        *
        * @param   aDevSoundCaps  DevSound capabilities structure
        * @param   aNumChannels   array to hold the retrieved number of channels
        *
        * @return  void
        *
        */
		void ConvertFromDevSoundCapsToNumChannelsL( const TMMFCapabilities& aDevSoundCaps,
													RArray<TUint>& aNumChannels );

        /**
        *
        * Issues a user panic with the given panic code
        *
        * @param   aPanicCode  panic code
        *
        * @return  void
        *
        */
		void Panic( TInt aPanicCode ) const;

    private:

        // DevSoundAudioInput states
        enum TState
            {
            EIdle=0,
            EDevSoundReady,
            ERecord,
            EPaused,
            EBufferEmpty
            };

        // DevSoundAudioInput error code
        enum TError
            {
            ECantCreateBuffer,
            EEmptyBuffer,
            EDevSoundNotLoaded,
            EUnknown
            };

    private:

        // current DevSoundAudioInput state
        TState iState;

        // priority settings for DevSound
        TMMFPrioritySettings iPrioritySettings;

        // Structure for DevSound configuration parameters
        TMMFCapabilities iDevSoundConfig;

        // sampling rate
        TUint iSinkSampleRate;
        // number of channels
        TUint iSinkChannels;
        // FourCC code
        TFourCC iSinkFourCC;
        // data type
        TFourCC iDataTypeCode;

        // Codec configuration data
        RArray<TInt> iCodecConfigData;
        RArray<TInt> iPendingCodecConfigData;

        // DevSound pointer
        CMMFDevSound& iMMFDevSound;
        TBool iDevSoundInitialized;
        TBool iEncoderConfigPending; 
        TUint  iBitRate;
        
        CActiveSchedulerWait* iBlockInitialize;

	};

#endif 		// CDEVSOUNDAUDIOINPUT_H
