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
* Description:  This class is the top base class in the Advanced Audio Controller
*				 inheritance hierarchy.
*
*/



#ifndef CADVANCEDAUDIOCONTROLLER_H
#define CADVANCEDAUDIOCONTROLLER_H

// INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <implementationproxy.h>
#include <mmfcontroller.h>
#include <mmfstandardcustomcommands.h>
#include "MCustomInterfaceCustomCommandImplementor.h"

class CCustomInterfaceBuilder;
class CMMFDevSound;
// CLASS DECLARATION

/**
*  This is the base class CAdvancedAudioController.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class CAdvancedAudioController : public CMMFController,
                                 public MAsyncEventHandler,
                                 public MMMFAudioControllerCustomCommandImplementor,
                                 public MDataSource,
                                 public MDataSink,
                                 public MCustomInterfaceCustomCommandImplementor
    {
    protected:  // Constructors and destructor

		/**
		* Destructor
		*/
		IMPORT_C virtual ~CAdvancedAudioController();

        /**
        * C++ default constructor.
        */
        IMPORT_C CAdvancedAudioController();
        
        virtual void ConstructL();

	protected: // New functions

        /**
        * Creates a buffer of specified size. Buffer is pushed onto the cleanupstack.
        * @since 3.0
        * @param aSize size of the buffer
        * @return pointer to the new buffer
        */
		IMPORT_C CMMFDataBuffer* CreateSourceBufferOfSizeLC( TUint aSize );

        /**
        * Resets all buffers in iSharedBuffers.
        * @since 3.0
        * @param aNumOfBuffer number of buffers to be contained in iSharedBuffers
        * @param aBufferSize size of the buffers
        * @return void
        */
		IMPORT_C void ResetSharedBuffersL( TInt aNumOfBuffer, TInt aBufferSize );

        /**
        * Raises a system panic.
        * @since 3.0
        * @param aPanicCode panic code
        * @return void
        */
		IMPORT_C void Panic( TInt aPanicCode );

    protected: // Functions from base classes (CMMFController)

		/**
        * From CMMFController
		* Handles creation of the data source.
		* @param aSource data source
		* @return void
		*/
		IMPORT_C virtual void AddDataSourceL( MDataSource& aSource );

		/**
        * From CMMFController
		* Handles creation of the data output.
		* @param aSink data output
		* @return void
		*/
		IMPORT_C virtual void AddDataSinkL( MDataSink& aSink );

		/**
        * From CMMFController
		* Deletes the data source.
		* @param aDataSource the address of this source must be same as
		*                    the existing datasource
		* @return void
		*/
		IMPORT_C virtual void RemoveDataSourceL( MDataSource& aDataSource );

		/**
        * From CMMFController
		* Deletes the data output.
		* @param aDataSink the address of this output must be same as
		*                  the existing output
		* @return void
		*/
		IMPORT_C virtual void RemoveDataSinkL( MDataSink& aDataSink );

		/**
        * From CMMFController
		* Resets the controller object and removes datasource/sink.
		* @param none
		* @return void
		*/
		IMPORT_C virtual void ResetL();

		/**
        * From CMMFController
		* Prepares the resources for play. Source and sink must have been added before
		* calling PrimeL().
		* @param none
		* @return void
		*/
		IMPORT_C virtual void PrimeL();

		/**
        * From CMMFController
		* Initiates play. Leaves if PrimeL() has not been called.
		* @param none
		* @return void
		*/
		IMPORT_C virtual void PlayL();

		/**
        * From CMMFController
		* Pauses the play. Leave with if PlayL() has not been called.
		* @param none
		* @return void
		*/
		IMPORT_C virtual void PauseL();

		/**
        * From CMMFController
		* Stops the play. Leaves if controller state is already stopped.
		* @param none
		* @return void
		*/
		IMPORT_C virtual void StopL();

		/**
        * From CMMFController
		* Returns the current position in microseconds.
		* @param none
		* @return current position in microseconds
		*/
		IMPORT_C virtual TTimeIntervalMicroSeconds PositionL() const;

		/**
        * From CMMFController
		* Sets the play position.
		* @param aPosition position in microseconds
		* @return void
		*/
		IMPORT_C virtual void SetPositionL( const TTimeIntervalMicroSeconds& aPosition );

		/**
        * From CMMFController
		* Returns the duration of source (or the sink if the source is not defined)
		* If neither source nor sink are defined, it returns 0.
		* @param none
		* @return duration in microseconds
		*/
		IMPORT_C virtual TTimeIntervalMicroSeconds DurationL() const;

		/**
        * From CMMFController
		* Sets the source priority settings if a priority mechanism is required
		* to arbitrate between multiple clients trying to access the same resource.
		* Sets the priority for the source and the sink (provided they are defined).
		* @param aPrioritySettings priority settings
		* @return void
		*/
		IMPORT_C virtual void SetPrioritySettings( const TMMFPrioritySettings& aPrioritySettings );

		/**
        * From CMMFController
		* Retrieve the number of meta data entries in the clip.
		* @param aNumberOfEntries on return, the number of meta data entries
		* @return void
		*/
		IMPORT_C virtual void GetNumberOfMetaDataEntriesL( TInt& aNumberOfEntries );

		/**
        * From CMMFController
		* Gets the specified meta data entry.
		* @param aIndex the zero based meta data entry index to retrieve
		* @return pointer to the meta data entry
		*/
		IMPORT_C virtual CMMFMetaDataEntry* GetMetaDataEntryL( TInt aIndex );

    protected: // Functions from base classes (MMMFAudioControllerCustomCommandImplementor)

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the sample rate of the data source in samples/second.
		* @param aSampleRate sample rate, in samples/second.
		* @return void
		*/
		IMPORT_C virtual void MacSetSourceSampleRateL( TUint aSampleRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the bit rate of the data source in bits/second.
		* @param aBitRate bit rate, in bits/second.
		* @return void
		*/
		IMPORT_C virtual void MacSetSourceBitRateL( TUint aBitRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the number of channels in the data source.
		* For example, one channel for mono, two channels for stereo, etc.
		* @param aNumChannels number of channels
		* @return void
		*/
		IMPORT_C virtual void MacSetSourceNumChannelsL( TUint aNumChannels );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the format of the data source.
		* The UID corresponds to the UID of the CMMFFormat-derived ECOM plugin to be used.
		* @param aFormatUid format plugin to be used.
		* @return void
		*/
		IMPORT_C virtual void MacSetSourceFormatL( TUid aFormatUid );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the fourCC multimedia data type code of the source.
		* @param aDataType fourCC code.
		* @return void
		*/
		IMPORT_C virtual void MacSetSourceDataTypeL( TFourCC aDataType );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the sample rate of the data sink in samples/second.
		* @param aSampleRate The sample rate, in samples/second.
		* @return void
		*/
		IMPORT_C virtual void MacSetSinkSampleRateL( TUint aSampleRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the bit rate of the data sink in bits/second.
		* @param aRate bit rate, in bits/second.
		* @return void
		*/
		IMPORT_C virtual void MacSetSinkBitRateL( TUint aRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the number of channels in the data sink.
		* For example, one channel for mono, two channels for stereo, etc.
		* @param aNumChannels number of channels
		* @return void
		*/
		IMPORT_C virtual void MacSetSinkNumChannelsL( TUint aNumChannels );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the format of the data sink.
		* The UID corresponds to the UID of the CMMFFormat-derived ECOM plugin to be used.
		* @param aFormatUid format plugin to be used
		* @return void
		*/
		IMPORT_C virtual void MacSetSinkFormatL( TUid aFormatUid );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the fourCC multimedia data type code of the sink.
		* @param aDataType fourCC code
		* @return void
		*/
		IMPORT_C virtual void MacSetSinkDataTypeL( TFourCC aDataType );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the codec to be used. Useful when recording data.
		* @param aSourceDataType source data type
		* @param aSinkDataType sink data type
		* @return void
		*/
		IMPORT_C virtual void MacSetCodecL( TFourCC aSourceDataType, TFourCC aSinkDataType );

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
		* @param aRate sample rate, in samples/second.
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
		* Gets the format of the data source.
		* The UID corresponds to the UID of the CMMFFormat-derived ECOM plugin being used.
		* @param aFormat format plugin being used
		* @return void
		*/
		IMPORT_C virtual void MacGetSourceFormatL( TUid& aFormat );

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
		* @param aRate bit rate, in bits/second.
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkBitRateL( TUint& aRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the number of channels in the data sink.
		* @param aNumChannels number of channels.
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkNumChannelsL( TUint& aNumChannels );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the format of the data sink.
		* The UID corresponds to the uid of the CMMFFormat-derived ECOM plugin being used.
		*
		* @param aFormat format plugin being used.
		* @return void
		*/
		IMPORT_C virtual void MacGetSinkFormatL( TUid& aFormat );

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

    protected: // Functions from base classes (MAsyncEventHandler)

		/**
		* From MAsyncEventHandler
		* Sends an event to the client.
		* @param aEvent event to send
		* @return error code. KErrNone if successful.
		*/
		IMPORT_C virtual TInt SendEventToClient( const TMMFEvent& aEvent );

    protected: // Functions from base classes (MDataSource & MDataSink)

		// From MDataSource
		// The following functions are declared as pure virtual in MDataSource, therefore
		// declared here.  However, they are not implemented.
		IMPORT_C virtual TFourCC SourceDataTypeCode( TMediaId aMediaId );
		IMPORT_C virtual void FillBufferL( CMMFBuffer* aBuffer, MDataSink* aConsumer, TMediaId aMediaId );
		IMPORT_C virtual void BufferEmptiedL( CMMFBuffer* aBuffer );
		IMPORT_C virtual TBool CanCreateSourceBuffer();
		IMPORT_C virtual CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, TBool& aReference );
		IMPORT_C virtual void ConstructSourceL( const TDesC8& aInitData );

		// From MDataSink
		// The following functions are declared as pure virtual in MDataSink, therefore
		// declared here.  However, they are not implemented.
		IMPORT_C virtual TFourCC SinkDataTypeCode( TMediaId aMediaId );
		IMPORT_C virtual void EmptyBufferL( CMMFBuffer* aBuffer, MDataSource* aSupplier, TMediaId aMediaId );
		IMPORT_C virtual void BufferFilledL ( CMMFBuffer* aBuffer );
		IMPORT_C virtual TBool CanCreateSinkBuffer();
		IMPORT_C virtual CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId, TBool& aReference );
		IMPORT_C virtual void ConstructSinkL( const TDesC8& aInitData );

	protected: // Data

		/**
		* From MCustomInterfaceCustomCommandImplementor
		* Creates a Custom Interface Builder
		*/
		IMPORT_C virtual const TMMFMessageDestination& GetCustomInterfaceBuilderL();

	protected: // Data
		// AdvancedAudioController panic codes
		enum TAdvancedAudioControllerPanics
		{
		EBadArgument,
		EBadState,
		EBadInvariant,
		EBadReset,
		EPostConditionViolation
		};

		// Pointer to the data source
		MDataSource* iDataSource;
		// Pointer to the data sink
		MDataSink* iDataSink;

		// Priority settings
		TMMFPrioritySettings iPrioritySettings;
		// Sample rate
		TInt iSampleRate;
		// Bit rate
		TInt iBitRate;
		// Number of channels
		TInt iChannels;
		// Duration of the clip
		TInt64 iDuration;
		// Data type
		TFourCC iDataType;
		// Time position in microseconds
		TInt64 iTimePositionInMicroSecs;

		// Max number of shared buffer
		TInt iSharedBufferMaxNum;
		// Max buffer size
		TInt iSharedBufferMaxSize;
		TInt iSharedBufferMaxSizeForNonSeekableSrc;
		// Array of shared buffers
		RPointerArray<CMMFDataBuffer> iSharedBuffers;
		// Current index within iSharedBuffers
		TInt iSharedBufferIndex;
		CMMFDevSound* iMMFDevSound;
		
		// Pointer to the custom interface builder
		CCustomInterfaceBuilder* iCustomInterfaceBuilder;
    };

#endif		// CADVANCEDAUDIOCONTROLLER_H

// End of File
