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
* Description:  This file contains the base class from which specific audio
*                play controllers are derived. This class encapsulates common
*                behavior for all audio play controllers.
*
*/


#ifndef CADVANCEDAUDIOPLAYCONTROLLER_H
#define CADVANCEDAUDIOPLAYCONTROLLER_H

// INCLUDES
#include "AdvancedAudioController.h"
#include "AdvancedAudioOutput.h"
#include "AdvancedAudioUtility.h"   // for CAdvancedAudioUtility, MAdvancedAudioUtilityObserver
#include <mmf/common/mmfdrmcustomcommands.h>
#include <mmffile.h>
#include <MultimediaDataSource.h>
#include <StreamControlCustomCommandsParser.h>
#include <StreamControlEvents.h>
#include "DataSourceAdapter.h"
//#include <s32file.h>

// CONSTANTS
const TUint KAAPSampleRate8000Hz        = 8000;
const TUint KAAPSampleRate11025Hz       = 11025;
const TUint KAAPSampleRate12000Hz       = 12000;
const TUint KAAPSampleRate16000Hz       = 16000;
const TUint KAAPSampleRate22050Hz       = 22050;
const TUint KAAPSampleRate24000Hz       = 24000;
const TUint KAAPSampleRate32000Hz       = 32000;
const TUint KAAPSampleRate44100Hz       = 44100;
const TUint KAAPSampleRate48000Hz       = 48000;
const TUint KAAPSampleRate64000Hz       = 64000;
const TUint KAAPSampleRate88200Hz       = 88200;
const TUint KAAPSampleRate96000Hz       = 96000;


enum TStopCondition
	{
	ECommanded,
	EPlayComplete,
	EErr
	};
			
class MTrailingSilenceObserver
    {
public:
    /**
    * From MTrailingSilenceObserver
    * * Call back function indicating that that trailing silence timer has expired during loop play
    *
    * @since 5.x
    * @param none
    * @return  void
    */
    virtual void TrailingSilenceTimerComplete() = 0;
    };

// FORWARD DECLARATIONS
class CAdvancedAudioResource;
class CMultimediaDataSource;
class CDataSourceAdapter;
class CTrailingSilenceTimer;

// CLASS DECLARATION
/**
*  This is the base class CAdvancedAudioPlayController.
*  All specific audio play controllers derive from this class.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class CAdvancedAudioPlayController : public CAdvancedAudioController,
									 public MAdvancedAudioOutputObserver,
									 public MMMFAudioPlayControllerCustomCommandImplementor,
									 public MAdvancedAudioUtilityObserver,
 									 public MMultimediaDataSourceObserver,
 									 public MStreamControlCustomCommandImplementor,
 									 public MMMFAudioPlayControllerSetRepeatsCustomCommandImplementor,
#ifndef SYMBIAN_CAF_V2

                                     public MMMFAudioPlayDeviceCustomCommandImplementor
#else
                                     public MMMFAudioPlayDeviceCustomCommandImplementor,
							         public MMMFDRMCustomCommandImplementor,
							         public MTrailingSilenceObserver
#endif
    {
    protected:	// Constructors and destructor

		/**
		* Destructor
		*/
		IMPORT_C virtual ~CAdvancedAudioPlayController();

        /**
        * C++ default constructor.
        */
        IMPORT_C CAdvancedAudioPlayController();

        IMPORT_C virtual void ConstructL();

	protected: // New functions

        /**
        * Format specific initialization for Data Source
        * @since 3.0
        * @return void
        */
        virtual void DoAddDataSourceL() = 0;

        /**
        * Format specific initialization for Data Sink
        * @since 3.0
        * @return void
        */
        virtual void DoAddDataSinkL() = 0;
        
        /**
        * Instantiates codec
        * @since 5.0
        * @return CAdvancedAudioDecoder*
        */
        virtual CAdvancedAudioDecoder* BuildDecoderL() = 0;

        /**
        * Format specific initialization for Data Sink
        * @since 5.0
        * @return void
        */
        IMPORT_C virtual void DoInitializeSinkL();

		/**
        * Stops playback.
        * @since 3.0
        * @return void
        */
        IMPORT_C virtual void DoStopL(TInt aError);
        
        /**
        * Format specific header scanning
        * @since 3.0
        * @return void
        */
        IMPORT_C virtual void DoReadHeaderL(CMMFDataBuffer* aBuffer);
        
        /**
        * support functionality for SetPositionL
        * @since 3.2
        * @param aTimePos Position given by its time interval in micro secs
        * @return void
        */
        IMPORT_C virtual void DoSetPositionL(const TTimeIntervalMicroSeconds& aTimePos);
        
        /**
        * Fills source buffer from clip
        * @since 3.0
        * @return void
        */
        IMPORT_C virtual void InitSharedBuffersL();
        
        /**
        * Stops the source and sink
        * @since 3.0
        * @return void
        */
        IMPORT_C virtual void SourceSinkStopL();
        
        /**
        * Fills shared buffer from source
        * @since 3.0
        * @return void
        */
        IMPORT_C virtual void FillSharedBufferL(CMMFBuffer* aBuffer);
        
        /**
        * Find the index of buffer which contain a given position
        * @since 3.2
        * @param aPos the position to find
        * @return TInt the buffer index which recorded the position
        */
        IMPORT_C virtual TInt FindBufferFromPos(TUint aPos);
        
        /**
        * Prepare shared buffers and its current index
        * @since 3.2
        * @param aStreaming whether SharedBuffers contain streaming source
        * @return void
        */
        IMPORT_C virtual TInt SetPositionInSharedBuffers(TUint aTimeMs, TInt& aFoundPosition, TInt& aFoundTimeMs);
        
        /**
        * Update duration, should be called everytime source size changes
        * @since 3.2
        * @param aLimitInMilliSecond, the limit of difference of the new duration in millisec for a callback
        *        to occur, -1 would mean no callback
        * @return void
        */
        IMPORT_C virtual TInt UpdateDuration(TInt aLimitInMilliSecond = -1);
        IMPORT_C virtual TInt UpdateBitRate();
        
        /**
        * Handle AutoPaused event during SendEvent callback
        * @since 3.2
        * @return void
        */
        IMPORT_C virtual void HandleAutoPauseEvent();
        
        /**
        * Handle Preemption event during SendEvent callback
        * @since 3.2
        * @return void
        */
        IMPORT_C virtual void HandlePreemptionEvent(TInt aError);
        
        /**
        * Handle General event during SendEvent callback
        * @since 3.2
        * @return void
        */
        IMPORT_C virtual void HandleGeneralEvent(const TMMFEvent& aEvent);
        
        /**
		* Set the playback window boundaries
		* @since 3.2
		* @param aStart start of the window, in microseconds
		* @param aEnd end of the window, in microseconds
		* @return void
		*/
		IMPORT_C virtual void SetPlaybackWindowBoundariesL(const TTimeIntervalMicroSeconds& aStart,
			const TTimeIntervalMicroSeconds& aEnd);

        IMPORT_C virtual TInt GetCodecConfigData(RArray<TInt>& aCodecConfigData);
        
		IMPORT_C void RefillPreceedingBuffersL();
		


    protected:	// Functions from base classes (CMMFController)

        /**
        * From CMMFController
        * Handles creation of the data source
        *
        * @since 3.0
        * @param aSource data source
        * @return void
        */
        IMPORT_C virtual void AddDataSourceL(MDataSource& aSource);

        /**
        * From CMMFController
        * Handles creation of the data output
        *
        * @since 3.0
        * @param aSing data output
        * @return void
        */
        IMPORT_C virtual void AddDataSinkL(MDataSink& aSink);

        /**
        * From CMMFController
        * Deletes the data source
        *
        * @since 3.0
        * @param aDataSource the address of this source must be same as
        *                    the existing datasource
        * @return void
        */
        IMPORT_C virtual void RemoveDataSourceL(MDataSource& aDataSource);

        /**
        * From CMMFController
        * Deletes the data output
        *
        * @since 3.0
        * @param aDataSource the address of this output must be same as
        *                    the existing output
        * @return void
        */
        IMPORT_C virtual void RemoveDataSinkL(MDataSink& aDataSink);

        /**
        * From CMMFController
        * Resets the controller object, removes datasource/sink, resets the datapath.
        *
        * @since 3.0
        * @param none
        * @return void
        */
        IMPORT_C virtual void ResetL();

        /**
        * From CMMFController
        * Prepares the resources for play. Source and sink must have been added before
        * calling PrimeL().
        *
        * @since 3.0
        * @param none
        * @return void
        */
        IMPORT_C virtual void PrimeL();

        /**
        * From CMMFController
        * Will leave with if PrimeL() has not been called.
        *
        * @since 3.0
        * @param none
        * @return void
        */
        IMPORT_C virtual void PlayL();

        /**
        * From CMMFController
        * Will leave with if Play() has not been called.
        *
        * @since 3.0
        * @param none
        * @return void
        */
        IMPORT_C virtual void PauseL();

        /**
        * From CMMFController
        * Will leave if controller state is already stopped.
        *
        * @since 3.0
        * @param none
        * @return void
        */
        IMPORT_C virtual void StopL();

        /**
        * From CMMFController
        * Calculates the current position in microseconds.
        *
        * @since 3.0
        * @param none
        * @return TTimeIntervalMicroSeconds The current position in microseconds.
        */
        IMPORT_C virtual TTimeIntervalMicroSeconds PositionL() const;
        
        /**
        * From CMMFController
        * Sets the play position.
        *
        * @since 3.0
        * @param aTimePos Position given by its time interval in micro secs
        * @return void
        */
        IMPORT_C virtual void SetPositionL(const TTimeIntervalMicroSeconds& aTimePos);
        
        /**
        * From CMMFController
        * Method to set the source priority settings if
        * a priority mechanism is required to arbitrate between multiple clients
        * trying to access the same resource. Sets the priority for the source
        * and the sink (provided they are defined).
        *
        * @since 3.0
        * @param aPrioritySettings
        * @return void
        */
        IMPORT_C virtual void SetPrioritySettings(const TMMFPrioritySettings& aPrioritySettings);

		/**
		* From CMMFController
		* Gets the number of meta data entries in the clip.
        *
        * @since 3.0
        * @param aNumberOfEntries the number of meta data entries
		* @return void
		*/
		IMPORT_C virtual void GetNumberOfMetaDataEntriesL(TInt& aNumberOfEntries);

		/**
		* From CMMFController
		* Gets the specified meta data entry.
        *
        * @since 3.0
        * @param aIndex zero based meta data entry index to retrieve.
		* @return pointer to the meta data entry.
		*/
		IMPORT_C virtual CMMFMetaDataEntry* GetMetaDataEntryL(TInt aIndex);

		/**
        * From CMMFController
		* Returns the duration of source (or the sink if the source is not defined)
		* If neither source nor sink are defined, it returns 0.
		* @param none
		* @return duration in microseconds
		*/
		IMPORT_C virtual TTimeIntervalMicroSeconds DurationL() const;

    protected:  // functions from base classes (CAdvancedAudioController)

        /**
        * Send StateChanged vent to client
        * @since 3.2
        * @return error code. KErrNone if successful
        */
        IMPORT_C virtual TInt SendEventToClient(const TMMFEvent& aEvent);

    protected:	// Functions from base classes (MMMFAudioControllerCustomCommandImplementor)

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the sample rate of the data sink in samples/second.
		* @param aSampleRate The sample rate, in samples/second.
		* @return void
		*/
		IMPORT_C virtual void MacSetSinkSampleRateL(TUint aSampleRate);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the bit rate of the data sink in bits/second.
		* @param aRate bit rate, in bits/second.
		* @return void
		*/
		IMPORT_C virtual void MacSetSinkBitRateL(TUint aRate);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the number of channels in the data sink.
		* For example, one channel for mono, two channels for stereo, etc.
		* @param aNumChannels number of channels
		* @return void
		*/
		IMPORT_C virtual void MacSetSinkNumChannelsL(TUint aNumChannels);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the format of the data sink.
		* The UID corresponds to the UID of the CMMFFormat-derived ECOM plugin to be used.
		* @param aFormatUid format plugin to be used
		* @return void
		*/
        IMPORT_C virtual void MacSetSinkFormatL(TUid aFormatUid);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the fourCC multimedia data type code of the sink.
		* @param aDataType fourCC code
		* @return void
		*/
		IMPORT_C virtual void MacSetSinkDataTypeL(TFourCC aDataType);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the sample rate of the data source in samples/second.
		* @param aRate source sample rate, in samples/second
		* @return void
		*/
		IMPORT_C virtual void MacGetSourceSampleRateL(TUint& aRate);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the bit rate of the data source in bits/second.
		* @param aRate source bit rate, in samples/second
		* @return void
		*/
		IMPORT_C virtual void MacGetSourceBitRateL(TUint& aRate);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the number of channels in the data source.
		* @param aNumChannels number of channels
		* @return void
		*/
		IMPORT_C virtual void MacGetSourceNumChannelsL(TUint& aNumChannels);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the format of the data source.
		* The UID corresponds to the UID of the CMMFFormat-derived ECOM plugin being used.
		* @param aFormat format plugin being used
		* @return void
		*/
		IMPORT_C virtual void MacGetSourceFormatL(TUid& aFormat);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the fourCC code of the source.
		* @param aDataType fourCC code
		* @return void
		*/
		IMPORT_C virtual void MacGetSourceDataTypeL(TFourCC& aDataType);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the sample rate of the data sink in samples/second.
		* @param aRate sample rate, in samples/second
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkSampleRateL(TUint& aRate);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the bit rate of the data sink in bits/second.
		* @param aRate bit rate, in bits/second.
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkBitRateL(TUint& aRate);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the number of channels in the data sink.
		* @param aNumChannels number of channels.
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkNumChannelsL(TUint& aNumChannels);

        /**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the format of the data sink.
		* The UID corresponds to the uid of the CMMFFormat-derived ECOM plugin being used.
		*
		* @param aFormat format plugin being used.
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkFormatL(TUid& aFormat);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the fourCC code of the sink.
		* @param aDataType fourCC code.
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkDataTypeL(TFourCC& aDataType);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of sample rates supported by the data source.
		* @param aSupportedRates supported rates.
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSourceSampleRatesL(RArray<TUint>& aSupportedRates);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of bit rates supported by the data source.
		* @param aSupportedRates supported rates.
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSourceBitRatesL(RArray<TUint>& aSupportedRates);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of channels supported by the data source L(ie mono, stereo etc).
		* @param aSupportedChannels supported channels.
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSourceNumChannelsL(RArray<TUint>& aSupportedChannels);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of fourCC codes supported by the data source.
		* @param aSupportedDataTypes supported data types.
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSourceDataTypesL(RArray<TFourCC>& aSupportedDataTypes);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of sample rates supported by the data sink.
		* @param aSupportedRates supported rates.
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSinkSampleRatesL(RArray<TUint>& aSupportedRates);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of bit rates supported by the data sink.
		* @param aSupportedRates supported rates.
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSinkBitRatesL(RArray<TUint>& aSupportedRates);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of channels supported by the data sink L(ie mono, stereo etc).
		* @param aSupportedChannels supported channels.
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSinkNumChannelsL(RArray<TUint>& aSupportedChannels);

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of fourCC codes supported by the data sink.
		* @param aSupportedDataTypes supported data types.
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSinkDataTypesL(RArray<TFourCC>& aSupportedDataTypes);

    protected:	// Functions from base classes (MMMFAudioPlayControllerCustomCommandImplementor)

		/**
		* From MMMFAudioPlayControllerCustomCommandImplementor
		* Sets a playback window. The controller will start playing from the start of the window,
		* and finish playing at the end of the window.
		* @param aStart start of the window, in microseconds
		* @param aEnd end of the window, in microseconds
		* @return void
		*/
		IMPORT_C virtual void MapcSetPlaybackWindowL(const TTimeIntervalMicroSeconds& aStart,
			const TTimeIntervalMicroSeconds& aEnd);

		/**
		* From MMMFAudioPlayControllerCustomCommandImplementor
		* Removes a previously defined playback window.
		* @param none
		* @return void
		*/
		IMPORT_C virtual void MapcDeletePlaybackWindowL();

		/**
		* From MMMFAudioPlayControllerCustomCommandImplementor
		* Gets the completion status of loading/rebuffering the current audio clip
		* @param aPercentageComplete status of loading as a percentage completed
		* @return void
		*/
		IMPORT_C virtual void MapcGetLoadingProgressL(TInt& aPercentageComplete);

    protected:	// Functions from base classes (MMMFAudioPlayDeviceCustomCommandImplementor)

		/**
		* From MMMFAudioPlayDeviceCustomCommandImplementor
		* Sets the volume of the sound device.
		* @param aVolume new volume
		* @return void
		*/
		IMPORT_C virtual void MapdSetVolumeL(TInt aVolume);

		/**
		* From MMMFAudioPlayDeviceCustomCommandImplementor
		* Gets the max volume of the sound device
		* @param aMaxVolume contains the max volume on return
		* @return void
		*/
		IMPORT_C virtual void MapdGetMaxVolumeL(TInt& aMaxVolume);

		/**
		* From MMMFAudioPlayDeviceCustomCommandImplementor
		* Gets the current playback volume
		* @param aVolume contains the current volume on return
		* @return void
		*/
		IMPORT_C virtual void MapdGetVolumeL(TInt& aVolume);

		/**
		* From MMMFAudioPlayDeviceCustomCommandImplementor
		* Sets a volume ramp.
		* This will cause the sound device to start playing with zero volume,
		* increasing the volume over aRampDuration microseconds.
		* The volume ramp can be removed by setting the ramp duration to zero.
		* @param aRampDuration duration over which the volume is to be increased,
		*					in microseconds
		* @return void
		*/
		IMPORT_C virtual void MapdSetVolumeRampL(const TTimeIntervalMicroSeconds& aRampDuration);

		/**
		* From MMMFAudioPlayDeviceCustomCommandImplementor
		* Sets the balance between the left and right stereo audio channels.
		* @param aBalance Use a value between KMMFBalanceMaxLeft and KMMFBalanceMaxRight.
		*                 Center balance can be restored by using KMMFBalanceCenter.
		* @return void
		*/
		IMPORT_C virtual void MapdSetBalanceL(TInt aBalance);

		/**
		* From MMMFAudioPlayDeviceCustomCommandImplementor
		* Retrieves the balance between the left and right stereo audio channels.
		*
		* @param aBalande The current balance, filled in by the controller.
		* @return void
		*/
		IMPORT_C virtual void MapdGetBalanceL(TInt& aBalance);

    protected:  // Functions from base classes (MMMFAudioPlayControllerSetRepeatsCustomCommandImplementor)
        
        /**
        * From MMMFAudioPlayControllerSetRepeatsCustomCommandImplementor
        * Sets the number of times the playback of the audio file is to be repeated and the trailing silence value in ms.
        *
        * @param aRepeatNumberOfTimes Number of times playback is to be repeated .
        * @param aTrailingSilence Period of silence to follow each playback of the audio file
        * @return KErrNone if success, KErrNotSupported and System-wide errors otherwise.
        */
        IMPORT_C virtual TInt MapcSetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds& aTrailingSilence);

    protected: // Functions from base classes (MDataSink)

        /**
        * From MDataSink
        * Callback for ReadBufferL()
        *
        * @since 3.0
        * @param    aBuffer The filled buffer
        * @return    void
        */
        IMPORT_C virtual void BufferFilledL(CMMFBuffer* aBuffer);

    protected: // Functions from base classes (MAdvancedAudioOutputObserver)

        /**
        * From MAdvancedAudioOutputObserver
        * Request to refill the buffer
        *
        * @since 3.0
        * @param    The buffer to be filled
        * @return   TInt
        */
        IMPORT_C virtual TInt RefillBuffer(CMMFBuffer* refillBuffer);

        /**
        * From MAdvancedAudioOutputObserver
        * Handles playback complete
        *
        * @since 3.0
        * @param
        * @return   void
        */
        IMPORT_C virtual void PlaybackComplete();

        /**
        * From MAdvancedAudioOutputObserver
        * Sends event to client;
        *
        * @since 3.0
        * @param aEvent event to propagate to client
        * @return   void
        */
        IMPORT_C virtual void SendEvent(const TMMFEvent& aEvent);

        /**
        * From MAdvancedAudioOutputObserver
        * Callback when lastbuffer is sent to DevSound during loop play by setting the Last Buffer flag to EFalse
        *
        * @since 5.x
        * @param  none
        * @return   void
        */
        IMPORT_C virtual void LastBufferSent();

    protected:  // from MAdvancedAudioUtilityObserver
        
        /**
        * Noticifaction of BitRate Changes, occurs when bitrate changes and EnableBitRateChangedEventL is enabled
        * @since 3.2
        * @return void
        */
        IMPORT_C virtual void BitRateChanged();
        IMPORT_C virtual void SeekPositionReached(TUint aTimeMs);
		IMPORT_C virtual void PlayWindowEndPositionReached();
		
    protected:  // functions from MMultimediaDataSourceObserver

        IMPORT_C virtual void BufferFilled(CMMFBuffer* aBuffer);
        IMPORT_C virtual void Event(TUid aEvent);
        IMPORT_C virtual TInt GetBitRate(TUint& aBitRate);
        
    protected:  // functions from MStreamControlCustomCommandImplementor

        IMPORT_C virtual TInt MscEnableEvents(TBool aEnable);
        IMPORT_C virtual TInt MscGetSeekingSupported(TBool& aSupported);
        IMPORT_C virtual TInt MscGetRandomSeekingSupported(TBool& aSupported);
        
    private:

        /**
        * Calculates the percentage of balance in left and right channels.
        * @since 3.0
        * @param aLeft on return, left balance value
        * @param aRight on return, right balance value
        * @param aBalance current balance value
        * @return void
        */
		void CalculateLeftRightBalanceL(TInt& aLeft, TInt& aRight, TInt aBalance);

        /**
        * Calculates the balance based on the left and right balance values.
        * @since 3.0
        * @param aBalance on return, the balance value
        * @param aLeft left balance value
        * @param aRight right balance value
        * @return void
        */
        void CalculateBalanceL(TInt& aBalance, TInt aLeft, TInt aRight);

        void GoToInitPositionL();
		
		void ResetPositionVariables();
		void DoPlayL();
		void DoInitializeL();
		void DoPauseL(TBool aPreemption = EFalse);
		void DoBufferFilledL(CMMFBuffer* aBuffer);
		void DoResume(TInt aResumePosition = -1);
		TBool AllBuffersFilled();
		TBool AllBuffersEmpty();
        void RefillSharedBuffersL();
        void ClearSharedBuffersL();
        void SeekToTimeMsL(TUint aTimeMs);
        void PlayForDurationL();
        void PlayForInitPositionL();
        void PlayForPauseSeekL();
		void PauseForSetPosL();
		TInt CleanupForStop();
		void ClearRepeatFlag();
		// sets the repeat count and trailing silence timer value supplied by the client
        TInt DoSetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds& aTrailingSilence);
        // handles the loop play of seekable and non-seekable sources
        void DoRepeatL();
        TBool IsLoopPlayEnabled() const;
        
    public: // Functions from base classes (MTrailingSilenceObserver)
        /**
        * From MTrailingSilenceObserver
        * Call back function indicating that that trailing silence timer has expired during loop play
        *
        * @since 5.x
        * @param none
        * @return  void
        */
        IMPORT_C virtual void TrailingSilenceTimerComplete();

    protected:	// Functions from base classes (MMMFDRMCustomCommandImplementor)

#ifdef SYMBIAN_CAF_V2
		/**
		* From MMMFDRMCustomCommandImplementor
		* Returns the result of MdcEvaluateIntent() when DataSourceType is KUidMmfFileSource.
		*
		* @param aIntent The generic DRM intent specified by client.
		* @return TInt
		*/
		IMPORT_C virtual TInt MdcEvaluateIntent(ContentAccess::TIntent aIntent);

		/**
		* From MMMFDRMCustomCommandImplementor
		* Returns the result of ExecuteIntent() when DataSourceType is KUidMmfFileSource.
		*
		* @param aIntent The generic DRM intent specified by client.
		* @return TInt
		*/
		IMPORT_C virtual TInt MdcExecuteIntent(ContentAccess::TIntent aIntent);

		/**
		* From MMMFDRMCustomCommandImplementor
		* Setting the iDisableAutoIntent based on the client request to disable
		* or enable the automatic intent.
		*
		* @param aIntent The generic DRM intent specified by client.
		* @return TInt
		*/
		IMPORT_C virtual TInt MdcDisableAutomaticIntent(TBool aDisableAutoIntent);

		/**
		* From MMMFDRMCustomCommandImplementor
		* Returns the result of SetAgentProperty() when DataSourceType is KUidMmfFileSource.
		*
		* @param aIntent The generic DRM intent specified by client.
		* @return TInt
		*/
		IMPORT_C virtual TInt MdcSetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue);
		
#endif

	protected: // Data

        enum TPlayControllerState
        {
            EStopped,
            EInitializing,
            EInitialized,
            EPlaying,
            EPaused,
            EAutoPaused
        };

        // Holds the controller state
        TPlayControllerState iState;
        // Holds the requested state
        TPlayControllerState iRequestState;
        TPlayControllerState iReturnState;
        // Indicates if the clip is DRM protected
        TBool iIsDRMProtected;

		// Pointer to audio output
		CAdvancedAudioOutput* iAudioOutput;
		// Pointer to audio resource
		CAdvancedAudioResource* iAudioResource;
		// Pointer to audio utility
		CAdvancedAudioUtility* iAudioUtility;

        // Data source type - used by metadata functions
        TUid iSourceType;
		
        // Holds the current byte position in the clip
        TUint iSourceReadPosition;
        // 1st or 2nd header offset
        TInt iHeaderOffset;
        // 1st header offset
        TInt iSyncOffset;
        // Write position for file sink
        TInt iSinkWritePosition;
        // Holds the source format
        TUid iSourceFormat;
        // Sink number of channels
        TInt iSinkNumChannels;
        // Sink sample rate
        TInt iSinkSampleRate;

		// Play window - start position in microseconds
		TTimeIntervalMicroSeconds iPlayWindowStartPosition;
		// Play window - end position in microseconds
		TTimeIntervalMicroSeconds iPlayWindowEndPosition;
		// Indication whether metadata is supported or not
		TBool iMetaDataSupport;
		// Metadata entries, if supported
		RPointerArray<CMMFMetaDataEntry> iMetaDataEntries;
    	// Flag indicating whether metadata has been ready already
    	TBool iMetaDataRead;
	    // Drive number
        TDriveNumber iDriveNumber;
        
#ifdef SYMBIAN_CAF_V2
 		TBool iDisableAutoIntent;
#endif

 		TBool iSourceUnreadable;
 		TBool iSinkInitDataReady;
		TBool iReadHeader;                  // indicate whether the header is waited to be read
		TUint iCurrentPosition;             // to keep track with current position related to source on streaming situation

		TInt64 iPDTimePositionInMicroSecs;
		
    	TBool iEventsEnabled;               // indicate whether StreamControlEvent is enabled
        TBool iDecoderExists;
    	
    	TBool iSourceIsPosSeekable;
    	TBool iSourceIsTimeSeekable;
    	TBool iBitRateFrozen;
    	TBool iDurationFrozen;
    	TBool iPlayingForDuration;
    	TBool iPlayingForInitPos;
        TBool iPausingForSetPos;
    	TBool iPlayingForPauseSeek;
    	TBool iPlaySeeking;
    	TTimeIntervalMicroSeconds iInitPosition;
    	TTimeIntervalMicroSeconds iPreSeekTime;
    	
        CDataSourceAdapter* iDataSourceAdapter;
        
        mutable CActiveSchedulerWait* iWait;
        mutable CActiveSchedulerWait* iBlockSetPos;
        mutable CActiveSchedulerWait* iBlockPrime;

        TUint32 iPlayStartTime;
        TUint32 iZeroTime;
        TUint32 iAfterTime;
        
		mutable TBool iBlockDuration;
		TBool iEnablePrimedStateChangedEvent;
		
		// Decoder type
		TInt iDecoderType;
		
		TInt iSharedBufferCnt;

		TInt iResumePosition;

		TInt iRepeatCount;    // Repeat count set by the client
		TInt iCurrentRepeatCount;  // index to track the current repeat count during loop play
		TBool iLoopPlayEnabled;   // flag indicating if loop play is enabled or not
		TBool iRepeatForever;     // flag indicating that loop play forever is enabled
		TTimeIntervalMicroSeconds32 iTrailingSilenceMs;   // Trailing silence value set by the client
		CTrailingSilenceTimer* iTrailingSilenceTimer;     // Trailing silence timer class
		TBool iIntentStopped;     // flag indicating that rights are consumed during the end of each repeat in loop play
		TUint iSavedTimePositionInMicroSecs;      // saved position to adjust the position during loop play
		TTimeIntervalMicroSeconds iSavedSetPosition;
    };

class CTrailingSilenceTimer: public CTimer
    {
    public:

         /**
          * First phase constructor in the Two Phase Construction.
          */  
         // static CTrailingSilenceTimer* NewL(CAdvancedAudioPlayController &aAdvancedAudioPlayController);
        static CTrailingSilenceTimer* NewL(MTrailingSilenceObserver &aobserver);

         /**
          * Default Destructor.
          */
         virtual ~CTrailingSilenceTimer();
        
    private:
        
         /**
          * Default Constructor.
          */
        // CTrailingSilenceTimer(CAdvancedAudioPlayController &aAdvancedAudioPlayController);
        CTrailingSilenceTimer(MTrailingSilenceObserver &aobserver);

         /**
          * Second phase constructor in the Two Phase Construction.
          */
         void ConstructL();

         /**
          * Default RunL() from CActive 
          */
         void RunL();

         /**
          * Implements cancellation of an outstanding request. 
          */
          void DoCancel();

          /**
           * Handles a leave occurring in the request completion event handler RunL()
           */
          void RunError();
          
    private:
          // CAdvancedAudioPlayController* iAdvancedAudioPlayController;
        MTrailingSilenceObserver* iObserver;
    };
	
#endif 		// CADVANCEDAUDIOPLAYCONTROLLER_H

// End of File
