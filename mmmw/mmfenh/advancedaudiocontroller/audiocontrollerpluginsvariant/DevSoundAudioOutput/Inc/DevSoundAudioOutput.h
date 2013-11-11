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
* Description:  This file defines the DevSound audio output class responsible for
*                sending output to the speaker.
*
*/


#ifndef CDEVSOUNDAUDIOOUTPUT_H
#define CDEVSOUNDAUDIOOUTPUT_H

// INCLUDES
#include "AdvancedAudioOutput.h"
#include <sounddevice.h>

// CLASS DECLARATIONS
class CConfigurationComponentsFactory;
class CAudioOutputControlUtility;  
/**
* This class defines audio output functions specific to DevSound used for playback.
*
* @lib DevSoundAudioOutput.lib
* @since 3.0
*/
class CDevSoundAudioOutput : public CAdvancedAudioOutput,
                             public MDevSoundObserver
    {
    public:  // Constructors and destructor

       /**
        * Class instantiation function.
        * @param   aPrioritySettings  priority settings for devSound and policy
        * @param   aObserver          observer class that will handle all DevSoundAudioOutput
        *                             callbacks
        * @return  pointer to the instance created
        */
        IMPORT_C static CDevSoundAudioOutput* NewL(const TMMFPrioritySettings& aPrioritySettings,
                                                    MAdvancedAudioOutputObserver& aObserver,
                                                    CMMFDevSound& aMMFDevSound);

        /**
        * Destructor function
        * @param   none
        * @return  none
        */
        IMPORT_C virtual ~CDevSoundAudioOutput();

    public: // Functions from base classes (CAdvancedAudioOutput)

        /**
        * From CAdvancedAudioOutput
        * Prepares DevSound Audio Output for playback.
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void PrimeL();

        /**
        * From CAdvancedAudioOutput
        * Handles request to pause playback. Sends pause command to DevSound.
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void PauseL();

        /**
        * From CAdvancedAudioOutput
        * Handles request to start playback. Configures and initializes DevSound.
        * @since 3.1
        * @param aBuffer buffer to be played
        *        aIndex index of the buffer to be played.
        * @return  none
        */
        IMPORT_C virtual void PlayL(TAny* aBuffer, TInt aIndex);

        /**
        * From CAdvancedAudioOutput
        * Handles request to stop playback. Sends stop command to DevSound.
        * @since 3.0
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void StopL(TBool aStopDevsound=ETrue);

        /**
        * From CAdvancedAudioOutput
        * Handles request to get the maximum volume. The max volume is obtained from DevSound.
        * @since 3.0
        * @param   none
        * @return  the maximum gain value
        */
        IMPORT_C virtual TInt MaxVolumeL();

        /**
        * From CAdvancedAudioOutput
        * Handles request to set the volume. The new value is sent to DevSound.
        * @since 3.0
        * @param   aVolume  new volume value
        * @return  void
        */
        IMPORT_C virtual void SetVolumeL(TInt aVolume);

        /**
        * From CAdvancedAudioOutput
        * Handles request to get the current volume. The current volume is obtained from DevSound.
        * @since 3.0
        * @param   void
        * @return  the current volume value
        */
        IMPORT_C virtual TInt VolumeL();

        /**
        * From CAdvancedAudioOutput
        * Handles request to set priority. The given priority settings are sent to DevSound.
        * @since 3.0
        * @param   aPrioritySettings  new priority settings
        * @return  none
        */
		IMPORT_C virtual void SetPrioritySettingsL(const TMMFPrioritySettings& aPrioritySettings);

        /**
        * From CAdvancedAudioOutput
        * Handles request to calculate current playback position in terms to micro seconds
        * @since 3.0
        * @param   none
        * @return  Current position in microseconds
        */
        IMPORT_C virtual TInt64 CalculateAudioOutputPositionL() const;

        /**
        * From CAdvancedAudioOutput
        * Handles request to get all configured sampling rates from DevSound.
        * This is a request for all sampling rates that DevSound is currently
        * configured with.
        * @since 3.0
        * @param   aRates  array to hold the retrieved sampling rates
        * @return  void
        */
        IMPORT_C virtual void ConfigRatesL(RArray<TUint>& aRates);

        /**
        * From CAdvancedAudioOutput
        * Handles request to get all configured channels from DevSound.
        * This is a request for all channels that DevSound is currently
        * configured with.
        * @since 3.0
        * @param   aChannels  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void ConfigChannelsL(RArray<TUint>& aChannels);

        /**
        * From CAdvancedAudioOutput
        * Handles request to get all configured data types (FourCC) from DevSound.
        * This is a request for all data types that DevSound is currently
        * configured with.
        * @since 3.0
        * @param   aDataTypes  array to hold the retrieved data types
        * @return  void
        */
        IMPORT_C virtual void ConfigDataTypesL(RArray<TFourCC>& aDataTypes);

        /**
        * From CAdvancedAudioOutput
        * Handles request to get all capablities sampling rates from DevSound.
        * This contains all sampling rates DevSound is capable of supporting
        * @since 3.0
        * @param   aRates  array to hold the retrieved sampling rates
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesRatesL(RArray<TUint>& aRates);

        /**
        * Handles request to get all capabilities channels from DevSound.
        * This is a request for all channels that DevSound is capable of
        * supporting.
        * @since 3.0
        * @param   aChannels  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesChannelsL(RArray<TUint>& aChannels);

        /**
        * From CAdvancedAudioOutput
        * Handles request to get all capabilities data types (FourCC) from DevSound.
        * This is a request for all data types that DevSound is capable of
        * supporting.
        * @since 3.0
        * @param   aDataTypes  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesDataTypesL(RArray<TFourCC>& aDataTypes);

        /**
        * From CAdvancedAudioOutput
        * Handles request to set volume ramp to given duration.
        * @since 3.0
        * @param   aRampDuration  ramp duration in microseconds
        * @return  void
        */
        IMPORT_C virtual void SetVolumeRampL(const TTimeIntervalMicroSeconds& aRampDuration);

        /**
        * From CAdvancedAudioOutput
        * Handles request to set play balance in DevSound.
        * @since 3.0
        * @param   aLeftPercentage   percentage for left channel
        * @param   aRightPercentage  percentage for right channel
        * @return  void
        */
        IMPORT_C virtual void SetPlayBalanceL(TInt aLeftPercentage, TInt aRightPercentage);

        /**
        * From CAdvancedAudioOutput
        * Handles request to get play balance from DevSound.
        * @since 3.0
        * @param   aLeftPercentage   percentage retrieved for left channel
        * @param   aRightPercentage  percentage retrieved for right channel
        * @return  void
        */
        IMPORT_C virtual void GetPlayBalanceL(TInt& aLeftPercentage, TInt& aRightPercentage);

        /**
        * Handles request to configure DevSound with the given parameters.
        * @since 3.0
        * @param   aSampleRate       sampling rate
        * @param   aNumChannel       required number of channels (mono/stereo indication)
        * @param   aFourCC           FourCC of required recording format
        * @param   aCodecConfigData  specific codec configuration data
        * @return  void
        */
        IMPORT_C virtual void ConfigureL(TUint aSampleRate, TUint aNumChannel, TFourCC aFourCC,
                                          const RArray<TInt>& aCodecConfigData);

        /**
        * Sets the Decoder instance to be used by the audio output.
        * @since 3.0
        * @param aDecoder Decoder instance
        * @return void
        */
        IMPORT_C virtual void SetDecoder(CAdvancedAudioDecoder* aDecoder);

        IMPORT_C virtual void Resume(TInt aBufferIndex = -1);
        
        IMPORT_C virtual void IsLoopPlayEnabled(const TBool aLoopPlayEnabled);
        
        IMPORT_C virtual void UnSetLastBuffer(const TBool aUnSetLastBuffer);
        
        IMPORT_C TBool IsDSStopped();
        
        IMPORT_C virtual void SetDataSourceAdapter(CDataSourceAdapter* aDataSourceAdapter);

    public: // Functions from base classes (MDevSoundObserver)

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate initialization has completed.
        * @since 2.0
        * @param   aError  error code returned by DevSound (KErrNone if no error)
        * @return  void
        */
        virtual void InitializeComplete(TInt aError);

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate tone playback has completed
        * since 2.0
        * @param   aError  error code returned by DevSound (KErrNone if no error)
        * @return  void
        */
        virtual void ToneFinished(TInt aError);

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate the spcecified buffer needs to be filled.
        * (Applicable to playback only)
        * @since 2.0
        * @param   aBuffer  buffer to place playback data
        * @return  void
        */
        virtual void BufferToBeFilled(CMMFBuffer* aBuffer);

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate playback has completed successfully
        * or otherwise.
        * @since 2.0
        * @param   aError  error code returned by DevSound (KErrNone if no error)
        * @return  void
        */
        virtual void PlayError(TInt aError);

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate the specified buffer has been filled and
        * needs to be emptied. (Applicable for Recording only)
        * @since 2.0
        * @param   aBuffer  buffer with recorded data that needs to be emptied
        * @return  void
        */
        virtual void BufferToBeEmptied(CMMFBuffer* aBuffer);

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate error has occurred during recording
        * (for record only)
        * @since 3.0
        * @param   aError  error code returned by DevSound (KErrNone if no error)
        * @return  void
        */
        virtual void RecordError(TInt aError);

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to indicate conversion from one format to another
        * has completed successfully or otherwise.
        * (during conversion only)
        * @since 3.0
        * @param   aError  error code returned by DevSound (KErrNone if no error)
        * @return  void
        */
        virtual void ConvertError(TInt aError);

        /**
        * From MDevSoundObserver
        * Callback function from hardware device passed through via DevSound
        * @since 3.0
        * @param   aMessageType  type of message
        * @param   aMsg          message contents
        * @return  void
        */
        virtual void DeviceMessage(TUid aMessageType, const TDesC8& aMsg);

        /**
        * From MDevSoundObserver
        * Callback function from DevSound to request given event be sent to client
        * @since 3.0
        * @param   aEvent  event to send
        * @return  void
        */
        virtual void SendEventToClient(const TMMFEvent& aEvent);

        /**
        * From MAdvancedAudioDecoderObserver
        * Called by a CAdvancedAudioDecoder object when the specifed buffer is filled.
        * @since 3.0
        * @param @param   aFilledBuffer  data buffer filled with decoded data
        * @return void
        */
        virtual void BufferFilled(CMMFBuffer* aFilledBuffer);

        /**
        * From MAdvancedAudioDecoderObserver
        * Callback function from CAdvancedAudioDecoder when the specified source buffer
        * needs to be refilled.
        * @since 3.0
        * @param   aEmptyBuffer an empty buffer to be refilled
        * @param   aDoSynchronousRead whether read should be Sync
        * @return  -
        */
        virtual void RefillBuffer(CMMFBuffer* aEmptyBuffer);
        
        /**
        * From MAdvancedAudioDecoderObserver
        * Callback function from CAdvancedAudioDecoder when the specified buffer
        * needs to be refilled. This indicates a shared buffer is to be refilled.
        * @since 3.2
        * @param   aEmptyBuffer an empty buffer to be refilled
        * @param   aDoSynchronousRead if the operation should be perform Synchronuously
        * @return  -
        */
//        virtual void RefillBuffer(CMMFBuffer* aEmptyBuffer, TBool aDoSynchronousRead);

        /**
        * From MAdvancedAudioDecoderObserver
        * Callback function from CAdvancedAudioDecoder to handle specified event
        * @since 3.0
        * @param aEvent The event to be handled
        * @return -
        */
		virtual void SendEvent(const TMMFEvent& aEvent);

    private:

        /**
        * C++ default constructor.
        */
        CDevSoundAudioOutput(CMMFDevSound& aMMFDevSound);

        /**
        *
        * Phase 2 of two-phase constructor
        *
        * @param   aPrioritySettings  priority settings for devSound and policy
        * @param   aObserver          observer class that will handle all DevSoundAudioOutput
        *                             callbacks
        *
        * @return  void
        *
        */
        void ConstructL(const TMMFPrioritySettings& aPrioritySettings,
						 MAdvancedAudioOutputObserver& aObserver);

       /**
        * Builds configuration data for DevSound and codec.
        * @since 3.0
        * @return  void
        */
        void BuildConfigurationParametersL();

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
		void ConvertFromDevSoundCapsToSampleRatesL(const TMMFCapabilities& aDevSoundCaps,
													RArray<TUint>& aSampleRates);

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
		void ConvertFromDevSoundCapsToDataTypesL(const TMMFCapabilities& aDevSoundCaps,
												  RArray<TFourCC>& aDataTypes);

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
		void ConvertFromDevSoundCapsToNumChannelsL(const TMMFCapabilities& aDevSoundCaps,
													RArray<TUint>& aNumChannels);

        /**
        *
        * Issues a user panic with the given panic code.
        * @param   aPanicCode  panic code
        * @return  void
        *
        */
		void Panic(TInt aPanicCode) const;

		/**
        * Handles actual start of playback. Configures and initializes DevSound.
        * @since 3.0
        * @param   none
        * @return  none
        */
        void DoPlayL(TAny* aBuffer, TInt aIndex);
    private:

        // DevSoundAudioOutput states
        enum TState
            {
            EIdle=0,
            EDevSoundReady,
            EMMFDevSoundFirstBufferSent,
            EPlaying,
            EPaused,
            EBufferEmpty,
			EPreIdle,
			EPlayPending
            };

        // DevSoundAudioOutput error code
        enum TError
            {
            ECantCreateBuffer,
            EEmptyBuffer,
            EDevSoundNotLoaded,
            ERecordNotSupported,
            EDevSoundError,
            EUnknown
            };

    private:

        // DevSoundAudioOutput internal state
        TState iState;

        // True if sampling rate conversion is required
        TBool iNeedsSWConversion;

        // Sample rate to convert to
        TUint iSWConvertSampleRate;

        // Number of channels to convert to
        TUint iSWConvertChannels;

        // priority settings for DevSound
        TMMFPrioritySettings iPrioritySettings;

        // Structure for DevSound configuration parameters
        TMMFCapabilities iDevSoundConfig;

        // sampling rate from the source audio data
        TUint iSourceSampleRate;

        // number of channel from the source audio data
        TUint iSourceChannels;

        // FourCC from the source audio data
        TFourCC iSourceFourCC;

		// Codec configuration data
        RArray<TInt> iCodecConfigData;

       // DevSound pointer
        CMMFDevSound& iMMFDevSound;
		TAny* iPlayPendingBuffer;
		TBool iDevSoundInitialized;

		TInt iBufferIndex;
		TUint32 iPlayDevSound;
		
		TBool iLoopPlayEnabled; // Indicates if LoopPlay is enabled or not
		TBool iIgnoreBTBFDuringLoopPlay; // Ignores any BTBF callbacks from DevSound during the TrailingSilence in LoopPlay
		TBool iUnSetLastBuffer; // Indicates if LastBuffer needs to be unset during LoopPlay
		TBool iDSStopped; // Indicates if DevSound is Stopped or not
		TBool iLastBufferSentToDevSound; // Indicates the end of the playback when last buffer is sent to DevSound at (in normal playback or in loopplay)

		CConfigurationComponentsFactory*    iFactory;
        CAudioOutputControlUtility*         iAudioOutputControlUtility;  
	};

#endif 		// CDEVSOUNDAUDIOOUTPUT_H
