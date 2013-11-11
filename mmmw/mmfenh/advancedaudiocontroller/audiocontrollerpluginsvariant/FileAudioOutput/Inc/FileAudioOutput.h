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
* Description:  This file defines the file audio output class responsible for
*                sending output to the file system.
*
*/


#ifndef FILEAUDIOOUTPUT_H
#define FILEAUDIOOUTPUT_H

// INCLUDES
#include "AdvancedAudioOutput.h"
#include "FileOutputAO.h"

// FORWARD DECLARATIONS
class CMMFClip;

// CLASS DECLARATIONS

/**
* Class containing audio output function implementation specific to file output
* used for conversion.
*
* @lib FileAudioOutput.lib
* @since 3.0
*/
class CFileAudioOutput : public CAdvancedAudioOutput,
                         public MFAOEventGeneratorObserver
    {
    public: // New functions

       /**
        * Class instantiation function.
        * @param   aDataSink a data sink where converted data is sent
        * @param   aPrioritySettings  priority settings for devSound and policy
        * @param   aObserver          observer class that will handle all DevSoundAudioOutput
        *                             callbacks
        * @return  pointer to the instance created
        */
        IMPORT_C static CFileAudioOutput* NewL(MDataSink& aDataSink,
                                                const TMMFPrioritySettings& aPrioritySettings,
                                                MAdvancedAudioOutputObserver& aObserver);

        /**
        * Destructor function
        * @param   none
        * @return  none
        */
        IMPORT_C virtual ~CFileAudioOutput();

        /**
        *
        * Prepares DevSound Audio Input for playback (sets state to ready)
        * @since 2.1
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void PrimeL();

        /**
        * Handles request to pause playback. Sends pause command to DevSound.
        * @since 2.1
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void PauseL();

        /**
        * Handles request to start playback. Configures and initializes DevSound and
        * sends first buffer of audio data to DevSound,
        * @since 2.1
        * @param aBuffer buffer to be played
        *        aIndex index of the buffer to be played.
        * @return  none
        */
        IMPORT_C virtual void PlayL(TAny* aBuffer, TInt aIndex);

        /**
        * Handles request to stop playback. Sends stop command to DevSound.
        * @since 2.1
        * @param   none
        * @return  none
        */
        IMPORT_C virtual void StopL(TBool aStopDevsound=ETrue);

        /**
        * Handles request to get the maximum volume. The max volume is obtained from DevSound.
        * @since 2.1
        * @param   none
        * @return  the maximum gain value
        */
        IMPORT_C virtual TInt MaxVolumeL();

        /**
        * Handles request to set the maximum volume. The new value is sent to DevSound.
        * @since 2.1
        * @param   aVolume  new volume value
        * @return  void
        */
        IMPORT_C virtual void SetVolumeL(TInt aVolume);

        /**
        * Handles request to get the current volume. The current volume is obtained from DevSound.
        * @since 2.1
        * @param   void
        * @return  the current volume value
        */
        IMPORT_C virtual TInt VolumeL();

        /**
        * Handles request to set priority. The given priority settings are sent to DevSound.
        * @since 2.1
        * @param   aPrioritySettings  new priority settings
        * @return  none
        */
        IMPORT_C virtual void SetPrioritySettingsL(const TMMFPrioritySettings& aPrioritySettings);

        /**
        * Handles request to calculate current playback position in terms to micro seconds
        * @since 2.1
        * @param   none
        * @return  Current position in microseconds
        */
        IMPORT_C virtual TInt64 CalculateAudioOutputPositionL() const;

        /**
        * Handles request to get all configured sampling rates from DevSound.
        * This is a request for all sampling rates that DevSound is currently
        * configured with.
        * @since 2.1
        * @param   aRates  array to hold the retrieved sampling rates
        * @return  void
        */
        IMPORT_C virtual void ConfigRatesL(RArray<TUint>& aRates);

        /**
        * Handles request to get all configured channels from DevSound.
        * This is a request for all channels that DevSound is currently
        * configured with.
        * @since 2.1
        * @param   aChannels  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void ConfigChannelsL(RArray<TUint>& aChannels);

        /**
        * Handles request to get all configured data types (FourCC) from DevSound.
        * This is a request for all data types that DevSound is currently
        * configured with.
        * @since 2.1
        * @param   aDataTypes  array to hold the retrieved data types
        * @return  void
        */
        IMPORT_C virtual void ConfigDataTypesL(RArray<TFourCC>& aDataTypes);

        /**
        * Handles request to get all capablities sampling rates from DevSound.
        * This contains all sampling rates DevSound is capable of supporting
        * @since 2.1
        * @param   aRates  array to hold the retrieved sampling rates
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesRatesL(RArray<TUint>& aRates);

        /**
        * Handles request to get all capabilities channels from DevSound.
        * This is a request for all channels that DevSound is capable of
        * supporting.
        * @since 2.1
        * @param   aChannels  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesChannelsL(RArray<TUint>& aChannels);

        /**
        * Handles request to get all capabilities data types (FourCC) from DevSound.
        * This is a request for all data types that DevSound is capable of
        * supporting.
        * @since 2.1
        * @param   aDataTypes  array to hold the retrieved channels
        * @return  void
        */
        IMPORT_C virtual void CapabilitiesDataTypesL(RArray<TFourCC>& aDataTypes);

        /**
        * Handles request to set volume ramp to given duration.
        * @since 2.1
        * @param   aRampDuration  ramp duration in microseconds
        * @return  void
        */
        IMPORT_C virtual void SetVolumeRampL(const TTimeIntervalMicroSeconds& aRampDuration);

        /**
        * Handles request to set play balance in DevSound.
        * @since 2.1
        * @param   aLeftPercentage   percentage for left channel
        * @param   aRightPercentage  percentage for right channel
        * @return  void
        */
        IMPORT_C virtual void SetPlayBalanceL(TInt aLeftPercentage, TInt aRightPercentage);

        /**
        * Handles request to get play balance from DevSound.
        * @since 2.1
        * @param   aLeftPercentage   percentage retrieved for left channel
        * @param   aRightPercentage  percentage retrieved for right channel
        * @return  void
        */
        IMPORT_C virtual void GetPlayBalanceL(TInt& aLeftPercentage, TInt& aRightPercentage);

        /**
        * Handles request to configure DevSound with the given parameters.
        * @since 2.1
        * @param   aSampleRate       recording sampling rate
        * @param   aNumChannel       required number of channels (mono/stereo indication)
        * @param   aFourCC           FourCC of required recording format
        * @param   aConfigType       UID for use in the codec ConfigureL function (this UID
        *                            is used to identify the command to the ConfigureL function
        * @param   aCodecConfigData  specific codec configuration data
        *
        * @return  void
        *
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
        
        /**
        * From MAdvancedAudioConverterObserver
        * Called by a CAdvancedAudioDecoder object when the specifed buffer is filled.
        * @since 3.0
        * @param @param   aFilledBuffer  data buffer filled with converted data
        * @return void
        */
        virtual void BufferFilled(CMMFBuffer* aFilledBuffer);

        /**
        * From MAdvancedAudioConverterObserver
        * Callback function from CAdvancedAudioDecoder when the specified buffer
        * needs to be refilled.
        * @since 3.0
        * @param   aEmptyBuffer an empty buffer to be refilled
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
        //virtual void RefillBuffer(CMMFBuffer* aEmptyBuffer, TBool aDoSynchronousRead);

        /**
        * From MAdvancedAudioDecoderObserver
        * Callback function from CAdvancedAudioDecoder to handle specified event
        * @since 3.0
        * @param aEvent The event to be handled
        * @return -
        */
        virtual void SendEvent(const TMMFEvent& aEvent);

        /**
        * From MFAOEventGeneratorObserver
        * Callback function from CFAOEventGenerator when an IO error occurs.
        * @since 3.0
        * @param   aError a system-wide error code.
        * @return  -
        */
        virtual void IOError(TInt aError);

    private:

        /**
        * C++ default constructor.
        */
        CFileAudioOutput();

        /**
        *
        * Phase 2 of two-phase constructor
        *
        * @param   aDataSink a data sink where converted data is sent
        * @param   aPrioritySettings  priority settings for devSound and policy
        * @param   aObserver          observer class that will handle all DevSoundAudioInput
        *                             callbacks
        *
        * @return  void
        *
        */
        void ConstructL(MDataSink& aDataSink,
                        const TMMFPrioritySettings& aPrioritySettings,
                        MAdvancedAudioOutputObserver& aObserver);

       /**
        * Builds configuration data for DevSound and codec
        * @since 3.0
        * @return  void
        */
        void BuildConfigurationParameters();


        /**
        * Issues a user panic with the given panic code
        * @param   aPanicCode  panic code
        * @return  void
        */
        void Panic(TInt aPanicCode) const;

    private:

        // FileAudioOutput states
        enum TState
            {
            EIdle=0,
            EPlaying,
            EPaused,
            EPreIdle
            };

    private:

        // FileAudioOutput internal state
        TState iState;

        MDataSink* iDataSink;

        CMMFClip* iClip;

        // Buffer used when writting to sink
        CMMFDataBuffer* iSinkBuffer;

        // sampling rate from the source audio data
        TUint iSourceSampleRate;

        // number of channel from the source audio data
        TUint iSourceChannels;

        // FourCC from the source audio data
        TFourCC iSourceFourCC;

        // codec configuration data
        RArray<TInt> iCodecConfigData;

        // Write position for file sink
        TInt iSinkWritePosition;

        CFAOEventGenerator* iFAOEventGenerator;

    };

#endif
