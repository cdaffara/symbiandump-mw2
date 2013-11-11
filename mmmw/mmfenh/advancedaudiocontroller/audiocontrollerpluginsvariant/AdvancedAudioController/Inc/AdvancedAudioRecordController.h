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
*                record controllers are derived. This class encapsulates common
*                behavior for all audio record controllers.
*
*/



#ifndef CADVANCEDAUDIORECORDCONTROLLER_H
#define CADVANCEDAUDIORECORDCONTROLLER_H

// INCLUDES
#include "AdvancedAudioController.h"
#include "AdvancedAudioInput.h"

// FORWARD DECLARATIONS
class CAdvancedAudioResource;
class CAdvancedAudioUtility;

// CLASS DECLARATION

/**
*  This is the base class CAdvancedAudioRecordController
*  All specific audio record controllers derive from this class.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class CAdvancedAudioRecordController : public CAdvancedAudioController,
									   public MAdvancedAudioInputObserver,
                                       public MMMFAudioRecordControllerCustomCommandImplementor,
									   public MMMFAudioRecordDeviceCustomCommandImplementor
	{
    public:	// Constructors and destructor

		/**
		* Destructor
		*/
		IMPORT_C virtual ~CAdvancedAudioRecordController();

        /**
        * C++ default constructor.
        */
        IMPORT_C CAdvancedAudioRecordController();
        
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
        * Stops recording
        * @since 3.0
        * @return void
        */
        IMPORT_C virtual void DoStopL();

        /**
        * Writes the format header to file
        * @since 3.0
        * @return void
        */
        virtual void WriteHeaderL() = 0;

        /**
        * Writes buffer to file
        * @since 3.0
        * @param aBuffer Contains the data
        * @param aWritePosition Write position in the file
        * @param aSupplier Callback pointer if asynchronous write
        * @param aWriteLen Length of data to write
        * @return void
        */
        IMPORT_C void WriteIfEnoughSpaceL( CMMFBuffer* aBuffer, TInt aWritePosition, MDataSource* aSupplier = NULL, TInt aWriteLen = 0 );

        /**
        * Writes to file if maxfile size has been reached
        * @since 3.0
        * @param aBuffer Contains the data
        * @return void
        */
        IMPORT_C void WriteIfMaxFileSizeReachedL( CMMFBuffer* aBuffer );

    protected:	// Functions from base classes (CMMFController)

        /**
        * From CMMFController
        * Handles creation of the data source
        *
        * @since 3.0
        * @param aSource data source
        * @return void
        */
        IMPORT_C virtual void AddDataSourceL( MDataSource& aSource );

        /**
        * From CMMFController
        * Handles creation of the data output
        *
        * @since 3.0
        * @param aSing data output
        * @return void
        */
        IMPORT_C virtual void AddDataSinkL( MDataSink& aSink );

        /**
        * From CMMFController
        * Deletes the data source
        *
        * @since 3.0
        * @param aDataSource the address of this source must be same as
        *                    the existing datasource
        * @return void
        */
        IMPORT_C virtual void RemoveDataSourceL( MDataSource& aDataSource );

        /**
        * From CMMFController
        * Deletes the data output
        *
        * @since 3.0
        * @param aDataSource the address of this output must be same as
        *                    the existing output
        * @return void
        */
        IMPORT_C virtual void RemoveDataSinkL( MDataSink& aDataSink );

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
        * @param aPosition Position given by its time interval in micro secs
        * @return void
        */
        IMPORT_C virtual void SetPositionL( const TTimeIntervalMicroSeconds& aPosition );

    protected:	// Functions from base classes (MMMFAudioControllerCustomCommandImplementor)

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the sample rate of the data source in samples/second.
		* @param aRate sample rate, in samples/second
		* @return void
		*/
		IMPORT_C virtual void MacGetSourceSampleRateL( TUint& aRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the bit rate of the data source in bits/second.
		* @param aRate sample rate, in bits/second
		* @return void
		*/
		IMPORT_C virtual void MacGetSourceBitRateL( TUint& aRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the number of channels in the data source.
		* @param aNumChannels number of channels
		* @return void
		*/
		IMPORT_C virtual void MacGetSourceNumChannelsL( TUint& aNumChannels );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the fourCC code of the source.
		* @param aDataType fourCC code
		* @return void
		*/
		IMPORT_C virtual void MacGetSourceDataTypeL( TFourCC& aDataType );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the sample rate of the data sink in samples/second.
		* @param aRate sample rate, in samples/second
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkSampleRateL( TUint& aRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the bit rate of the data sink in bits/second.
		* @param aRate bit rate, in bits/second
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkBitRateL( TUint& aRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the number of channels in the data sink.
		* @param aNumChannels number of channels
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkNumChannelsL( TUint& aNumChannels );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the fourCC code of the sink.
		* @param aDataType fourCC code
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkDataTypeL( TFourCC& aDataType );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of sample rates supported by the data source.
		* @param aSupportedRates supported rates
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSourceSampleRatesL( RArray<TUint>& aSupportedRates );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of bit rates supported by the data source.
		* @param aSupportedRates supported rates
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSourceBitRatesL( RArray<TUint>& aSupportedRates );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of channels supported by the data source (ie mono, stereo etc).
		* @param aSupportedChannels supported channels
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSourceNumChannelsL( RArray<TUint>& aSupportedChannels );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of fourCC codes supported by the data source.
		* @param aSupportedDataTypes supported data types
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSourceDataTypesL( RArray<TFourCC>& aSupportedDataTypes );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of sample rates supported by the data sink.
		* @param aSupportedRates supported rates
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSinkSampleRatesL( RArray<TUint>& aSupportedRates );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of bit rates supported by the data sink.
		* @param aSupportedRates supported rates
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSinkBitRatesL( RArray<TUint>& aSupportedRates );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of channels supported by the data sink (ie mono, stereo etc).
		* @param aSupportedChannels supported channels
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSinkNumChannelsL( RArray<TUint>& aSupportedChannels );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of fourCC codes supported by the data sink.
		* @param aSupportedDataTypes supported data types
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		IMPORT_C virtual void MacGetSupportedSinkDataTypesL( RArray<TFourCC>& aSupportedDataTypes );

    protected:	// Functions from base classes (MMMFAudioRecordControllerCustomCommandImplementor)

		/**
		* From MMMFAudioRecordControllerCustomCommandImplementor
		* Gets the (possibly estimated) record time left in the clip.
		*
		* @param aTime The record time available, in microseconds
		* @return void
		*/
		IMPORT_C virtual void MarcGetRecordTimeAvailableL( TTimeIntervalMicroSeconds& aTime );

		/**
		* From MMMFAudioRecordControllerCustomCommandImplementor
		* Sets the maximum duration of the recorded clip, in microseconds.
		* Not currently supported. Leaves with KErrNotSupported.
		* @param aMaxDuration maximum duration of the recorded clip, in microseconds
		* @return void
		*/
		IMPORT_C virtual void MarcSetMaxDurationL( const TTimeIntervalMicroSeconds& aMaxDuration );

		/**
		* From MMMFAudioRecordControllerCustomCommandImplementor
		* Sets the maximum size of the recorded clip, in bytes.
		* Not currently supported. Leaves with KErrNotSupported.
		* @param aFileSize maximum size of the recorded clip, in bytes
		* @return void
		*/
		IMPORT_C virtual void MarcSetMaxFileSizeL( TInt aFileSize );

		/**
		* From MMMFAudioRecordControllerCustomCommandImplementor
		* Remove a portion from the clip, either from the current position to the beginning
		* or the current position to the end.
		* @param aToEnd crop to the end if ETrue, otherwise crop to the beginning
		* @return void
		*/
		IMPORT_C virtual void MarcCropL( TBool aToEnd );

		/**
		* From MMMFAudioRecordControllerCustomCommandImplementor
		* Add meta data to the clip.
		* Not currently supported. Leaves with KErrNotSupported.
		* @param aNewEntry details of the new entry to be added
		* @return void
		*/
		IMPORT_C virtual void MarcAddMetaDataEntryL( const CMMFMetaDataEntry& aNewEntry );

		/**
		* From MMMFAudioRecordControllerCustomCommandImplementor
		* Remove meta data from the clip.
		* Not currently supported. Leaves with KErrNotSupported.
		*
		* @param aIndex The id of the meta data entry to delete.
		* @return void
		*/
		IMPORT_C virtual void MarcRemoveMetaDataEntryL( TInt aIndex );

		/**
		* From MMMFAudioRecordControllerCustomCommandImplementor
		* Replace a meta data in the clip.
		* Not currently supported. Leaves with KErrNotSupported.
		*
		* @param aIndex The id of the meta data entry to replace.
		* @param aNewEntry The details of the new entry to be added.
		* @return void
		*/
		IMPORT_C virtual void MarcReplaceMetaDataEntryL( TInt aIndex, const CMMFMetaDataEntry& aNewEntry );

    protected:	// Functions from base classes (MMMFAudioRecordDeviceCustomCommandImplementor)

		/**
		* From MMMFAudioRecordDeviceCustomCommandImplementor
		* Sets the gain of the sound device.
		* @param aGain new gain
		* @return void
		*/
		IMPORT_C virtual void MardSetGainL( TInt aGain );

		/**
		* From MMMFAudioRecordDeviceCustomCommandImplementor
		* Gets the maximum gain supported by the sound device.
		* @param aMaxGain maximum gain filled in by the controller
		* @return void
		*/
		IMPORT_C virtual void MardGetMaxGainL( TInt& aMaxGain );

		/**
		* From MMMFAudioRecordDeviceCustomCommandImplementor
		* Gets the current recording gain.
		* @param aGain current gain filled in by the controller
		* @return void
		*/
		IMPORT_C virtual void MardGetGainL( TInt& aGain );

		/**
		* From MMMFAudioRecordDeviceCustomCommandImplementor
		* Sets the balance between the left and right stereo microphone channels.
		* @param aBalance Use a value between KMMFBalanceMaxLeft and KMMFBalanceMaxRight
		*                 Centre balance can be restored by using KMMFBalanceCenter.
		* @return void
		*/
		IMPORT_C virtual void MardSetBalanceL( TInt aBalance );

		/**
		* From MMMFAudioRecordDeviceCustomCommandImplementor
		* Retrieve the balance between the left and right stereo microphone channels.
		* @param aBalance current balance, filled in by the controller
		* @return void
		*/
		IMPORT_C virtual void MardGetBalanceL( TInt& aBalance );

    protected: // Functions from base classes (MDataSource)

        /**
        * From MDataSource
        * Callback for WriteBufferL()
        *
        * @since 3.0
        * @param    aBuffer The emptied buffer
        * @return    void
        */
        IMPORT_C virtual void BufferEmptiedL( CMMFBuffer* aBuffer );

    protected: // Functions from base classes (MAdvancedAudioInputObserver)

        /**
        * From MAdvancedAudioInputObserver
        * Handles event from the devsoundaudiooutput
        *
        * @since 3.0
        * @param    The buffer to be emptied
        * @return   TInt
        */
        IMPORT_C virtual TInt EmptyBufferL( CMMFBuffer* aBuffer );

        /**
        * From MAdvancedAudioInputObserver
        * Handles record complete
        *
        * @since 3.0
        * @param
        * @return   void
        */
        IMPORT_C virtual void RecordComplete();

        /**
        * From MAdvancedAudioInputObserver
        * Sends event to client;
        *
        * @since 3.0
        * @param
        * @return   void
        */
        IMPORT_C virtual void SendEvent( const TMMFEvent& aEvent );

    private:

        /**
        * Handles event from the devsoundaudiooutput
        *
        * @param    The buffer to be emptied
        * @return   TInt
        */
        TInt DoEmptySharedBufferL( CMMFBuffer* aBuffer );

	protected: // Data

        enum TRecordControllerState
        {
            EStopped,
            EStopping,
            EWritingStopped,
            ERecording,
            EPaused
        };

        // Holds the controller state
        TRecordControllerState iState;

		// Pointer to audio output
		CAdvancedAudioInput* iAudioInput;
		// Pointer to audio resource
		CAdvancedAudioResource* iAudioResource;
		// Pointer to audio utility
		CAdvancedAudioUtility* iAudioUtility;

        // The write position
        TInt iSinkWritePosition;
		// max file size
		TInt iMaxFileSize;
        // File Clip exists
        TBool iClipAlreadyExists ;
        // Drive number
        TDriveNumber iDriveNumber;
        // For maintaining the current write position for any outstanding write request(s)
        TInt iOutstandingPos;
    };

#endif		// CADVANCEDAUDIORECORDCONTROLLER_H

// End of File
