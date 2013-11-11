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
*                output classes are derived. This class encapsulates common
*                behavior for all audio outputs. An audio output class serves as
*                the interface between the controller and the lower-level sound
*                producing interface.
*
*/



#ifndef CADVANCEDAUDIOOUTPUT_H
#define CADVANCEDAUDIOOUTPUT_H

//  INCLUDES
#include <e32base.h>
//#include <e32std.h>
//#include <MmfControllerFrameworkBase.h>
//#include <MmfControllerFramework.h>
//#include <MmfBase.h>
//#include <mmfpaniccodes.h>
#include "AdvancedAudioDecoder.h"
#include "DataSourceAdapter.h"

// CLASS DECLARATION

/**
* Mixin observer class to define an object capable of handling a callback from
* AdvancedAudioOutput.
*
* @lib AdvancedAudioOutput.lib
* @since 3.0
*/
class MAdvancedAudioOutputObserver
    {
    public:
        /**
        *
        * Callback function from CAdvaincedAudioOuput when the given buffer needs to
        * be refilled (read to external storage)
        * @param   refillBuffer  data buffer to be filled with new data
        * @return  errCode  KErrNone (0)
        *
        */
        virtual TInt RefillBuffer( CMMFBuffer* aRefillBuffer/*, TBool aDoSynchronousRead = EFalse */) = 0;

        /**
        *
        * Callback function from CAdvancedAudioOuput to indicate playback is completed
        * @param   none
        * @return  void
        *
        */
        virtual void PlaybackComplete() = 0;

        /**
        *
        * Callback function from CDevSoundAudioInput to handle specified event
        * @param   aEvent  the event to be handled
        * @return  void
        *
        */
        virtual void SendEvent( const TMMFEvent& aEvent ) = 0;

        /**
        *
        * Callback function from CAdvancedAudioOuput when lastbuffer is sent 
        * during a loopplay by setting the Last Buffer flag to EFalse
        * @param   none
        * @return  void
        *
        */
        virtual void LastBufferSent() = 0;
    };


/**
*  This is the base class for all audio output classes, used for playback.
*  It contains common behaviour for all audio outputs. The audio output class
*  provides the interface between the Controller and the sound output device.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class CAdvancedAudioOutput : public CBase,
							 public MAdvancedAudioDecoderObserver
    {
    public: // New functions

        /**
        * Prepares the resources for playback.
        * @since 3.0
        * @param none
        * @return void
        */
        IMPORT_C virtual void PrimeL() = 0;

        /**
        * Pauses the playback.
        * @since 3.0
        * @param none
        * @return void
        */
        IMPORT_C virtual void PauseL() = 0;

        /**
        * Initiates playback.
        * @since 3.0
        * @param aBuffer buffer to be played
        *        aIndex index of the buffer to be played
        * @return void
        */
        IMPORT_C virtual void PlayL( TAny* aBuffer, TInt aIndex ) = 0;

        /**
        * Stops playback.
        * @since 3.0
        * @param none
        * @return void
        */
        IMPORT_C virtual void StopL(TBool aStopDevsound=ETrue) = 0;

        /**
        * Returns the max volume.
        * @since 3.0
        * @param none
        * @return max volume
        */
		IMPORT_C virtual TInt MaxVolumeL() = 0;

        /**
        * Sets the volume.
        * @since 3.0
        * @param aVolume volume to be set
        * @return void
        */
        IMPORT_C virtual void SetVolumeL( TInt /*aVolume*/ ) = 0;

        /**
        * Returns the current volume.
        * @since 3.0
        * @param none
        * @return current volume
        */
        IMPORT_C virtual TInt VolumeL() = 0;

        /**
		* Sets the source priority settings if a priority mechanism is required
		* to arbitrate between multiple clients trying to access the same resource.
		* Sets the priority for the source and the sink (provided they are defined).
		* @since 3.0
        * @param aPrioritySettings priority settings
        * @return void
        */
        IMPORT_C virtual void SetPrioritySettingsL( const TMMFPrioritySettings& /*aPrioritySettings*/ ) = 0;

        /**
        * Returns the audio output position in microseconds.
        * @since 3.0
        * @param none
        * @return input output
        */
        IMPORT_C virtual TInt64 CalculateAudioOutputPositionL() const = 0;

        /**
        * Returns the currently configured rates.
        * @since 3.0
        * @param aRates on return, configured rates
        * @return void
        */
        IMPORT_C virtual void ConfigRatesL( RArray<TUint>& aRates ) = 0;

        /**
        * Returns the currently configured channels.
        * @since 3.0
        * @param aChannels on return, configured channels
        * @return void
        */
        IMPORT_C virtual void ConfigChannelsL( RArray<TUint>& aChannels ) = 0;

        /**
        * Returns the currently configured data types.
        * @since 3.0
        * @param aDataTypes on return, configured data types
        * @return void
        */
        IMPORT_C virtual void ConfigDataTypesL( RArray<TFourCC>& aDataTypes ) = 0;

        /**
        * Returns the supported rates.
        * @since 3.0
        * @param aRates on return, supported rates
        * @return void
        */
        IMPORT_C virtual void CapabilitiesRatesL( RArray<TUint>& aRates ) = 0;

        /**
        * Returns the supported channels.
        * @since 3.0
        * @param aChannels on return, supported channels
        * @return void
        */
        IMPORT_C virtual void CapabilitiesChannelsL( RArray<TUint>& aChannels ) = 0;

        /**
        * Returns the supported data types.
        * @since 3.0
        * @param aDataTypes on return, supported data types
        * @return void
        */
        IMPORT_C virtual void CapabilitiesDataTypesL( RArray<TFourCC>& aDataTypes ) = 0;

        /**
        * Sets the volume ramp.
        * @since 3.0
        * @param aRampDuration duration over which the volume is to be increased in microseconds
        * @return void
        */
        IMPORT_C virtual void SetVolumeRampL( const TTimeIntervalMicroSeconds& aRampDuration ) = 0;

        /**
        * Sets the balance.
        * @since 3.0
        * @param aLeftPercentage percentage of balance to be played on left
        * @param aRightPercentage percentage of balance to be played on right
        * @return void
        */
        IMPORT_C virtual void SetPlayBalanceL( TInt aLeftPercentage, TInt aRightPercentage ) = 0;

        /**
        * Returns the current balance percentages.
        * @since 3.0
        * @param aLeftPercentage on return, percentage of balance to be played on left
        * @param aRightPercentage on return, percentage of balance to be played on right
        * @return void
        */
        IMPORT_C virtual void GetPlayBalanceL( TInt& aLeftPercentage, TInt& aRightPercentage ) = 0;

        /**
        * Sets the configuration parameters for recording.
        * @since 3.0
        * @param aSampleRate sample rate
        * @param aNumChannel number of channels
        * @param aFourCC data type
        * @param aCodecConfigData codec configuration data
        * @return void
        */
        IMPORT_C virtual void ConfigureL( TUint aSampleRate, TUint aNumChannel, TFourCC aFourCC,
                                          const RArray<TInt>& aCodecConfigData ) = 0;

        /**
        * Sets the Decoder instance to be used by the audio output.
        * @since 3.0
        * @param aDecoder Decoder instance
        * @return void
        */
        IMPORT_C virtual void SetDecoder( CAdvancedAudioDecoder* aDecoder ) = 0;
        IMPORT_C virtual void Resume(TInt aBufferIndex = -1) = 0;
        IMPORT_C virtual void IsLoopPlayEnabled(const TBool aLoopPlayEnabled) = 0;
        IMPORT_C virtual void UnSetLastBuffer(const TBool aUnSetLastBuffer) = 0;
        IMPORT_C virtual TBool IsDSStopped() = 0;
        
        /**
        * Sets the DataAdapter instance to be used by the audio output.
        * @param aDataSourceAdapter DataSourceAdapter instance
        * @return void
        */
        IMPORT_C virtual void SetDataSourceAdapter(CDataSourceAdapter* aDataSourceAdapter) = 0;


	protected: // Data

        // pointer to AdvancedAudioOutput observer
        MAdvancedAudioOutputObserver* iObserver;

        // pointer to an audio decoder
        CAdvancedAudioDecoder* iAdvancedAudioDecoder;
        CDataSourceAdapter* iDataSourceAdapter;
        CMMFBuffer* iSavedBufferPtr;
	};

#endif		// CADVANCEDAUDIOOUTPUT_H
