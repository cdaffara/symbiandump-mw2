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
* Description:  Definition of the 3GPAudioRecordControllerPlugin interface.
*
*/


#ifndef C3GPAUDIORECORDCONTROLLERPLUGIN_H
#define C3GPAUDIORECORDCONTROLLERPLUGIN_H

// INCLUDES
#include "AdvancedAudioRecordController.h"
#include <mp4lib.h>
#include "MetaDataWrite.h"
#include "3GPAudioRecordControllerMetaData.h"

// FORWARD DECLARATION
class C3GPAudioRecordControllerEncoderBuilder;

// CLASS DECLARATION

/**
*  This class implements 3GPAudioRecordControllerPlugin
*
*  @lib 3GPAudioRecordControllerPlugin.lib
*  @since 3.0
*/
class C3GPAudioRecordControllerPlugin : public CAdvancedAudioRecordController, public MMetaDataWriteCustomCommandImplementor
    {
	public:  // Constructors and destructor

		/**
		* Two-phased constructor.
		*/
		static C3GPAudioRecordControllerPlugin* NewL();

		/**
		* Destructor
		*/
		virtual ~C3GPAudioRecordControllerPlugin();

    private: // Functions from base classes

        /**
		* From CAdvancedAudioRecordController
        * Format specific initialization for Data Source
        * @since 3.0
        * @return void
        */
        void DoAddDataSourceL();

        /**
		* From CAdvancedAudioRecordController
        * Format specific initialization for Data Sink
        * @since 3.0
        * @return void
        */
        void DoAddDataSinkL();

        /**
		* From CAdvancedAudioRecordController
        * Stops playback.
        * @return void
        */
        void DoStopL();

        /**
        * From CAdvancedAudioRecordController
        * Handles event from the DeSoundAudioInput
        *
        * @since 3.0
        * @param    The buffer to be emptied
        * @return   TInt
        */
        TInt EmptyBufferL( CMMFBuffer* aBuffer );

		/**
        * From CMMFController
		* Pauses the play.
		* @param none
		* @return void
		*/
		void PauseL();

		/**
		* From CAdvancedAudioPlayController
        * Prepares the resources for play. Source and sink must have been added before
        * calling PrimeL().
        *
        * @since 3.0
        * @param none
        * @return void
        */
		void PrimeL();

		/**
		* From CAdvancedAudioPlayController
        * Will leave with if PrimeL() has not been called.
        *
        * @since 3.0
        * @param none
        * @return void
        */
		void PlayL();

		/**
		* From CAdvancedAudioPlayController
        * Will leave if controller state is already stopped.
        *
        * @since 3.0
        * @param none
        * @return void
        */
		void StopL();

		/**
		* From CAdvancedAudioRecordController
        * Sets the play position.
        *
        * @since 3.0
        * @param aPosition Position given by its time interval in micro secs
        * @return void
        */
		void SetPositionL( const TTimeIntervalMicroSeconds& aPosition );

		/**
		* From CAdvancedAudioRecordController
        * Sends event to client;
        *
        * @since 3.0
        * @param aEvent event to propagate to client
        * @return   void
        */
		void SendEvent( const TMMFEvent& aEvent );

        /**
        * From CAdvancedAudioRecordController
        * Writes the format header to file
        * @since 3.0
        * @return void
        */
        virtual void WriteHeaderL();

		/**
		* From CAdvancedAudioRecordController
		* Remove a portion from the clip, either from the current position to the beginning
		* or the current position to the end.
		* @param aToEnd crop to the end if ETrue, otherwise crop to the beginning
		* @return void
		*/
		void MarcCropL( TBool aToEnd );

        /**
		* From CAdvancedAudioRecordController
		* Add meta data to the clip.
		* @param aNewEntry details of the new entry to be added
		* @return void
		*/
		void MarcAddMetaDataEntryL( const CMMFMetaDataEntry& aNewEntry );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of bit rates supported by the data sink.
		* @param aSupportedRates supported rates
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		void MacGetSupportedSinkBitRatesL( RArray<TUint>& aSupportedRates );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of sample rates supported by the data sink.
		* @param aSupportedRates supported rates
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		void MacGetSupportedSinkSampleRatesL( RArray<TUint>& aSupportedRates );
		
		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of channels supported by the data sink (ie mono, stereo etc).
		* @param aSupportedChannels supported channels
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		void MacGetSupportedSinkNumChannelsL( RArray<TUint>& aSupportedChannels );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the bit rate of the data sink in bits/second.
		* @param aRate bit rate, in bits/second.
		* @return void
		*/
		void MacSetSinkBitRateL( TUint aRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the sample rate of the data sink in samples/second.
		* @param aSampleRate The sample rate, in samples/second.
		* @return void
		*/
		void MacSetSinkSampleRateL( TUint aSampleRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the number of channels in the data sink.
		* For example, one channel for mono, two channels for stereo, etc.
		* @param aNumChannels number of channels
		* @return void
		*/
		void MacSetSinkNumChannelsL( TUint aNumChannels );

    private:

        /**
        * C++ default constructor.
        */
        C3GPAudioRecordControllerPlugin();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

        /**
        * Initializes the 3GP library with data sink.
        * @return void
        */
		void PrepareMP4ComposerL();

        /**
        * Translates error code from 3GP library to one of System Errors.
        * @param aError error code from 3GP library
        * @return System error
        */
		TInt TranslateMP4Err( MP4Err aError );

        /**
        * Write decoder specific information to 3GP file
        * @return void
        */
		void WriteAudioDecoderSpecificInfoL();

       	/**
        * Get the drive number of the data sink for the given drive name.
        * @return Drive number
        */
		TDriveNumber GetDriveNumber( const TDesC& aDriveName );
		
		/**
        * Determine the supported number of channels base on both sink and source
        * @return void
        */
		void DetermineSupportedNumChannelsL();

        /**
        * Updates the Record Duration after the Recording.
        */
        void UpdateMP4DurationL();

        /**
        * Commit meta data to file
        * @return void
        */
        void DoCommitMetaDataL();

        /**
        * Make up the udta box and write it to a MP4 handler
        * @return void
        */
        void WriteUdtaL();

        /**
        * Close opened MP4Compose Handle
        * @return void
        */
        void CloseMP4ComposerL();

    public:

        /**
		* Add a MetaDataWrite Parser
		* @return void
		*/
        void AddMetaDataWriteParserL();

    public:     // Functions from MMetaDataWriteCustomCommandImplementor

        /**
		* From MMetaDataWriteCustomCommandImplementor
        * Implements request on EnableMetaDataWrite
        * @since 3.0
        * @return TInt standard system error if any
        */
        virtual TInt MmdwcEnableDelayWrite();

        /**
		* From MMetaDataWriteCustomCommandImplementor
        * Implements request on CommitMetaData
        * @since 3.0
        * @return TInt standard system error if any
        */
        virtual TInt MmdwcCommitMetaData();

    private:    // Data

		// 3GP library handle
        MP4Handle iMP4Handle;

		// Builder for decoder object
    	C3GPAudioRecordControllerEncoderBuilder* iEncoderBuilder;
		// Decoder type
		TInt iEncoderType;
		// Data sink type
		TUid iSinkType;
		// buffer for internal use
        CMMFDataBuffer* iBuffer;
        // Array for SupportedChannels
        RArray<TUint> iSupportedChannels;

        // flag on whether MetaDataWrite is enabled
        TBool iEnableDelayWrite;

        // object to isolate meta data handling
        //CMetaDataWrite3GPAudio* iMetaData;
        C3GPAudioRecordControllerMetaData* iMetaData;
        };

#endif // C3GPAUDIORECORDCONTROLLERPLUGIN_H

// End of File
