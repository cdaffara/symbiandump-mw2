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
* Description:  Definition of Advance Audio Encoder base class.
*
*/



#ifndef CADVANCEDAUDIOENCODER_H
#define CADVANCEDAUDIOENCODER_H

// INCLUDES
#include <e32base.h>
#include <sounddevice.h>
#include <mmfcodecimplementationuids.hrh>

// CONSTANTS
const TUid KUidConfig = {KUidMmfCodecAudioSettings};

// FORWARD DECLARATIONS
class CMMFBuffer;
class CMMFDataBuffer;

// CLASS DECLARATION

/**
* Mixin observer class to define an object capable of handling a callback from
* CAdvancedAudioEncoder objects
*
* @lib AdvancedAudioController.lib
* @since 3.0
*/
class MAdvancedAudioEncoderObserver
    {
    public:

        /**
        * Callback function from CAdvancedAudioEncoder when the specified buffer
        * has been emptied.
        * @return  -
        */
        virtual void BufferEmptied() = 0;

        /**
        * Callback function from CAdvancedAudioEncoder when the specified buffer
        * needs to be emptied. This indicates a shared buffer is to be written to
        * external storage.
        * @param   aBuffer an empty buffer to be refilled
        * @return  -
        */
        virtual void EmptyBuffer( CMMFBuffer* aBuffer ) = 0;

        /**
        * Callback function from CAdvancedAudioDecoder to handle specified event
        * @param   aEvent  the event to be handled
        * @return  void
        */
        virtual void SendEvent( const TMMFEvent& aEvent ) = 0;
    };

/**
*  This is the base class for all audio encoder classes.
*  It contains common behaviour for all audio encoders. The audio encoder class
*  provides the interface to audio codec.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class CAdvancedAudioEncoder : public CActive
    {
    public:

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CAdvancedAudioEncoder();

    protected:

        /**
        * C++ default constructor.
        */
    	IMPORT_C CAdvancedAudioEncoder( TInt aPriority );

    public: // New functions

        /**
        * Sets the DevSound instance to be used by the audio decoder.
        * @since 3.0
        * @param aDevSound Devsound instance
        * @return void
        */
        IMPORT_C virtual void SetDevSound( CMMFDevSound& aDevSound );

        /**
        * Sets the observer object that will handle callbacks
        * @since 3.0
        * @param aObserver Observer object that will handle callbacks
        * @return void
        */
		IMPORT_C virtual void SetObserver( MAdvancedAudioEncoderObserver& aObserver );

        /**
        * Sets the sink buffer for encoding operations.
        * @since 3.0
        * @param aBuffers Pointer to shared buffers.
        * @return void
        */
        IMPORT_C virtual void SetSinkBuffers( RPointerArray<CMMFDataBuffer>* aBuffers );

        /**
        * Provides the audio encoder a buffer to process.
        * @since 3.0
        * @param   aBuffer a data buffer with data to encode.
        * @return  -
        */
        IMPORT_C virtual void EmptyBuffer( CMMFBuffer* aBuffer );

        /**
		* Sets audio decoder settings and attributes.
		* @since 3.0
        * @param   aSampleRate 			a sink sampling rate
        * @param   aNumChannel 			required number of channels (mono/stereo indication)
        * @param   aSWConvertSampleRate a sampling rate after conversion
        * @param   aSWConvertChannels   number of channels after conversion
        * @param   aCodecConfigData  	specific codec configuration data
        *
        * @return  void
        */
        IMPORT_C virtual void SetConfigL( TUint aSampleRate, TUint aChannels,
        						 TUint aSWConvertSampleRate, TUint aSWConvertChannels,
        						 const RArray<TInt>& aCodecConfigData ) = 0;

        /**
		* Sets the bit rate.
		* @since 3.0
        * @param   aBitRate 			bit rate
        * @param   aCodecConfigData  	specific codec configuration data
        *
        * @return  void
        */
        IMPORT_C virtual void SetBitRateL( TUint aBitRate, const RArray<TInt>& aCodecConfigData ) = 0;

        /**
		* Resets audio encoder settings and attributes.
		* @since 3.0
        * @return void
        */
		IMPORT_C virtual void ResetL() = 0;

        /**
	    * Check if the audio encoder is hardware accelerated.
	    * @since 3.0
	    * @return ETrue if hardare accelerated, otherwise, EFalse.
	    */
        IMPORT_C virtual TBool IsHwAccelerated() = 0;

        /**
        * Stops encoding process.
        * @since 3.0
        * @param none
        * @return void
        */
        IMPORT_C virtual void Stop();

    protected: // New functions

       /**
        * Handle emptying of the buffer received in EmptyBuffer().
        * @param   none
        * @return  void
        */
		IMPORT_C virtual void HandleEmptyBufferL() = 0;

        /**
        * Completes own request.
        */
        IMPORT_C void Ready( TInt aRequest );

       /**
        * Gets the next available shared buffer from the pool of shared buffers.
        * @param   none
        * @return  void
        */
        IMPORT_C void NextSharedBufferL();

    protected:	// Functions from base classes

        /**
        * From CActive
        */
        IMPORT_C virtual void RunL();

        /**
        * From CActive
        */
        IMPORT_C virtual void DoCancel();

    protected: // Data

		enum TAudioEncoderState
			{
			EIdle,
			EEncoding
			};

        enum TNextEvent
            {
            EDoNothing,
            EFlushBuffer,
            EPlayEnd
            };

		// Encoder state
		TAudioEncoderState iState;

		// The request being handled asynchronously
		TInt iRequest;

		// array of shared buffers
		RPointerArray<CMMFDataBuffer>* iSharedBuffers;
       	// next available buffer index
	    TInt iSharedBufferIndex;

        // audio buffer to empty pointer
        CMMFBuffer* iBufferToEmpty;
        // next available buffer pointer
        CMMFBuffer* iNextBuffer;

        // DevSound pointer
        CMMFDevSound* iMMFDevSound;

        // Audio Converter observer
        MAdvancedAudioEncoderObserver* iObserver;
	};

#endif		// CADVANCEDAUDIOENCODER_H