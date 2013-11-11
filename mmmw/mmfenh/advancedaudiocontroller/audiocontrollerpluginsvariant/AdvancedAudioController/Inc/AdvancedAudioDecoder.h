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
* Description:  Definition of Advance Audio Decoder base class.
*
*/


#ifndef CADVANCEDAUDIODECODER_H
#define CADVANCEDAUDIODECODER_H

// INCLUDES
#include "AdvancedAudioHelper.h"        // for MAdvancedAudioDecoderUtilityObserver
#include <FrameTable.h>                 // for CFrameTable
#include <e32base.h>
#include <sounddevice.h>
#include <mmfcodecimplementationuids.hrh>
#include <mmfcodec.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <mmf/common/mmfhelper.h>
#endif
// CONSTANTS
const TUid KUidConfig = {KUidMmfCodecAudioSettings};
const TInt KErrBufferNotReady = -1000;  // standard error on NextSharedBufferL called and the desire buffer is not EFull

// FORWARD DECLARATIONS
class CMMFBuffer;
class CMMFDataBuffer;
class CMMFCodec;

// CLASS DECLARATION
class TMMFPtr8 : public TPtr8
	{
public:
	TMMFPtr8()
		: TPtr8(0,0,0) {};
	inline void Set(const TDes8& aDes)
		{ TPtr8::Set((TUint8*)(aDes.Ptr()),aDes.Length(),aDes.MaxLength()); };
	inline void SetLengthOnly(const TDes8& aDes)
		{ TPtr8::Set((TUint8*)(aDes.Ptr()),aDes.Length(),aDes.Length()); };
	inline void Set(const TPtrC8& aDes)
		{ TPtr8::Set((TUint8*)(aDes.Ptr()),aDes.Length(),aDes.Length()); };
	inline void Shift(TInt aOffset)
		{ SetLength(Length()-aOffset); iMaxLength-=aOffset; iPtr+=aOffset; };
	};



/**
* Mixin observer class to define an object capable of handling a callback from
* CAdvancedAudioDecoder objects
*
* @lib AdvancedAudioController.lib
* @since 3.0
*/
class MAdvancedAudioDecoderObserver
    {
    public:

        /**
        * Callback function from CAdvancedAudioDecoder when the specified buffer
        * has been filled.
        * @param   aFilledBuffer  data buffer filled with converted data
        * @return  -
        */
        virtual void BufferFilled(CMMFBuffer* aFilledBuffer) = 0;

        /**
        * Callback function from CAdvancedAudioDecoder when the specified buffer
        * needs to be refilled. This indicates a shared buffer is to be refilled.
        * @param   aEmptyBuffer an empty buffer to be refilled
        * @return  -
        */
        virtual void RefillBuffer(CMMFBuffer* aEmptyBuffer) = 0;
        
        /**
        * Callback function from CAdvancedAudioDecoder when the specified buffer
        * needs to be refilled. This indicates a shared buffer is to be refilled.
        * @param   aEmptyBuffer an empty buffer to be refilled
        * @param   aDoSynchronousRead if the operation should be perform Synchronuously
        * @return  -
        */
        //virtual void RefillBuffer(CMMFBuffer* aEmptyBuffer, TBool aDoSynchronousRead) = 0;

        /**
        * Callback function from CAdvancedAudioDecoder to handle specified event
        * @param   aEvent  the event to be handled
        * @return  void
        */
        virtual void SendEvent(const TMMFEvent& aEvent) = 0;
    };
    
/**
*  This is the base class for all audio converter classes.
*  It contains common behaviour for all audio decoders. The audio decoder class
*  provides the interface to audio decoders.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class CAdvancedAudioDecoder : public CActive, public MFrameTableEventObserver
    {
	protected:
		enum TCodecCmd
			{
			ERenderDisable,
			ERenderEnable,
			EDisable,
			EEnable,
			EMarkPlayEnd,
			EUnMarkPlayEnd
			};
			
    public:
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CAdvancedAudioDecoder();

    protected:

        /**
        * C++ default constructor.
        */
    	IMPORT_C CAdvancedAudioDecoder(TInt aPriority);

    public: // New functions

        /**
        * Sets the DevSound instance to be used by the audio decoder.
        * @since 3.0
        * @param aDevSound Devsound instance
        * @return void
        */
        IMPORT_C virtual void SetDevSound(CMMFDevSound& aDevSound);

        /**
        * Sets the observer object that will handle callbacks
        * @since 3.0
        * @param aObserver Observer object that will handle callbacks
        * @return void
        */
		IMPORT_C virtual void SetObserver(MAdvancedAudioDecoderObserver& aObserver);
		
		/**
        * Sets the DecoderUtilityObserver object that will handle callbacks
        * @since 3.0
        * @param aDecoderUtilityObserver DecoderUtilityObserver object that will handle callbacks
        * @return void
        */
		IMPORT_C virtual void SetDecoderUtilityObserver(MAdvancedAudioDecoderUtilityObserver& aDecoderUtilityObserver);
		
        /**
        * Sets the source buffer for decoding operations.
        * @since 3.0
        * @param aBuffers Pointer to shared buffers.
        * @return void
        */
        IMPORT_C virtual void SetSourceBuffers(RPointerArray<CMMFDataBuffer>* aBuffers);
        
        /**
		* Sets audio decoder settings and attributes.
		* @since 3.0
        * @param   aSampleRate 			a source sampling rate
        * @param   aNumChannel 			required number of channels (mono/stereo indication)
        * @param   aSWConvertSampleRate a sampling rate after conversion
        * @param   aSWConvertChannels   number of channels after conversion
        * @param   aCodecConfigData  	specific codec configuration data
        * @param   aIndex               specify the index in iSharedBuffers
        *
        * @return  void
        */
        IMPORT_C virtual void SetConfigL(TUint aSampleRate, TUint aChannels,
        						 TUint aSWConvertSampleRate, TUint aSWConvertChannels,
        						 const RArray<TInt>& aCodecConfigData, TInt aIndex);


        /**
        * Provides the audio converter a buffer to fill with decoded data.
        * @since 3.0
        * @param   aBuffer a data buffer to be filled with decoded data
        * @return  -
        */
        IMPORT_C virtual void FillBufferL(CMMFBuffer* aBuffer);

        /**
		* Resets audio decoder settings and attributes.
		* @since 3.0
        * @return void
        */
		IMPORT_C virtual void ResetL();

        /**
	    * Check if the audio converter is hardware accelerated.
	    * @since 3.0
	    * @return ETrue if hardare accelerated, otherwise, EFalse.
	    */
        IMPORT_C virtual TBool IsHwAccelerated();

        /**
        * Stops decoding process.
        * @since 3.0
        * @param none
        * @return void
        */
        IMPORT_C virtual void Stop();
        
        /**
        * Set source reference offset in frametable
        * added to support AAC optimization
        * @since 3.2
        * @param aOffset    offset of the source position
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt SetSourceReference(TUint aTimeMs, TUint aPos);
        
        /**
        * Find the closest Frame's position from a given time (millisec) in frametable
        * added to support AAC optimization
        * @since 3.2
        * @param aTimeMs    Time to search from in millisec
        * @param aPos       Bytes Position found in frametable if return is KErrNone
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt FindFramePosFromTime(TUint& aTimeMs, TUint& aPos);
		
		IMPORT_C virtual TInt FindFrameTimeFromPos(TUint& aTimeMs, TUint& aPos);

		/**
        * Find the last record Frame's position in frametable
        * added to support AAC optimization
        * @since 3.2
        * @param aPos       Bytes Position found in frametable if return is KErrNone
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt LastFramePos(TUint& aPos);
		
		/*
        * Find the last Frame's position relative time (millisec) in frametable
        * added to support AAC optimization
        * @since 3.2
        * @param aTimeMs    Time of the last Frame in millisec if return is KErrNone
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt LastFrameTime(TUint& aTimeMs);
		
		/*
        * Reset the frame table
        * added to support AAC optimization
        * @since 3.2
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt ResetTable();
		
		/**
        * Cause FrameTable to generate an event when the desire Position is recorded in FrameTable
        * added to support AAC optimization
        * @since 3.2
        * @param aTimeMs    Time to be Seek to when callback occurs in millisec
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt SeekToTimeMs(TUint aTimeMs);
		IMPORT_C virtual TInt SetPlayWindowEndTimeMs(TUint aTimeMs);
		/**
        * Get the average bitrate inside the FrameTable
        * added to support AAC optimization
        * @since 3.2
        * return TInt       averate bitrate at the moment
        */
		IMPORT_C virtual TInt BitRate();
		
        /**
        * Cause FrameTable to generate an event when desire interval of data has been decoded
        * added to support AAC optimization
        * @since 3.2
        * @param aEnable    Enable/Disable the event
        * @param aTimeMs    Time of the desire interval in millisec
        * return TInt       standard system error
        */
        IMPORT_C virtual TInt EnableDecodeIntervalEvent(TBool aEnable, TUint aTimeMs = 0);
        
        IMPORT_C virtual void RenderDisable();
    
        IMPORT_C virtual void RenderEnable();

        IMPORT_C virtual void QueueThisBuffer(TUint aBufferIndex);

    public: // from MFrameTableEventObserver
        /**
        * Callback from FrameTable when event occur
        * added to support AAC optimization
        * @since 3.2
        * @param CFrameTable::TFrameTableEvent
        *                   event from FrameTable
        * return TInt       standard system error
        */
        IMPORT_C virtual TInt HandleFrameTableEvent(CFrameTable::TFrameTableEvent aEvent);
//		IMPORT_C virtual TInt IsSeeking(TBool& aIsSeeking);

    protected: // New functions

      	/**
        * Handle filling the requested buffer received in FillBufferL()
        * @param   none
        * @return  void
        */
		IMPORT_C virtual void HandleFillBufferL();
		IMPORT_C virtual TCodecProcessResult ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst);
		IMPORT_C virtual TInt CodecCmd(TCodecCmd aCmd);
		IMPORT_C virtual TInt CodecConfig(RArray<TInt>& aCodecConfigData);
//		IMPORT_C void RenderDisable();
//		IMPORT_C void RenderEnable();
		IMPORT_C void MarkPlayEnd();
		IMPORT_C void UnMarkPlayEnd();
		IMPORT_C void Disable();
		IMPORT_C void Enable();

		IMPORT_C virtual TCodecProcessResult ProcessHwL(CMMFBuffer& aSrc, CMMFBuffer& aDst);
		IMPORT_C virtual void ProcessParseL(TMMFPtr8* aSrc, TMMFPtr8* aDst);
		IMPORT_C virtual TUint8* PrepareInFrame(TUint8* aSrc, TInt aSrcLen, TInt& aSrcUsed);
		IMPORT_C virtual void ParseL(TUint8* aSrc, TInt& aSrcUsed, TUint8* aDst, TInt& aDstLen);
		virtual TInt SeekSync(TUint8* aBuf, TInt aBufLen);
		virtual TInt FrameLength(const TUint8* aBuf, TInt aBufLen, TInt& aFrameLength);

	private:	
        /**
        * Completes own request.
        */
        void Ready(TInt aRequest);
        void NextSharedBufferL();

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

		enum TAudioDecoderState
			{
			EIdle,
			EDecoding
			};

        enum TNextEvent
            {
            EDoNothing,
            ERefillBuffer,
            EPlayEnd
            };

		// Decoder state
		TAudioDecoderState iState;

		// array of shared buffers
		RPointerArray<CMMFDataBuffer>* iSharedBuffers;
       	// next available buffer index
	    TInt iSharedBufferIndex;

		// audio buffer to fill pointer
		CMMFBuffer* iBufferToFill;
        // audio buffer to empty pointer
        CMMFBuffer* iBufferToEmpty;
        // next available buffer pointer
        CMMFBuffer* iNextBuffer;

        // DevSound pointer
        CMMFDevSound* iMMFDevSound;

        // Audio Converter observer
        MAdvancedAudioDecoderObserver* iObserver;
        MAdvancedAudioDecoderUtilityObserver* iDecoderUtilityObserver;  // the Utility object as Observer to Decoder event

        CFrameTable* iFrameTable;   // FrameTable that record decoded frame position, added for AAC optimization
        
        TBool iRenderEnabled; // indicate whether audio buffer should be given to DevSound, added for AAC optimization
        TBool iEnabled;       // indicates if decoding in codec is enabled
        TBool iMarkPlayEnd;
        TUint iSeekToTimeMs;

        // soft codec pointer if soft codec is used
		CMMFCodec* iSoftCodec;

        // True if sampling rate conversion is required
        TBool iNeedsSWConversion;
        // Sample rate to convert to
        TUint iSWConvertSampleRate;
        // Number of channels to convert to
        TUint iSWConvertChannels;

        // sample rate converter factory pointer
        CMMFChannelAndSampleRateConverterFactory* iChannelAndSampleRateConverterFactory;
        // sample rate converter pointer
        CMMFChannelAndSampleRateConverter* iChannelAndSampleRateConverter;

        // converter buffer pointer
        CMMFDataBuffer* iConvertBuffer;
        TUint iEventPos;
        
        
       	RArray<TInt> iRenderEnableConfig;
       	RArray<TInt> iRenderDisableConfig;
       	RArray<TInt> iMarkPlayEndConfig;
       	RArray<TInt> iUnMarkPlayEndConfig;
       	RArray<TInt> iEnableConfig;
       	RArray<TInt> iDisableConfig;

		// below are for the hw decoding and could be moved to another header file
        	// local input buffer
        TUint8* iInBuffer;
        TInt iInBufferCount;
        TInt iInBufferIndx;
		TInt iSavedBytesCopiedFromSrc;
		TBool iInBufferFilled;
		TUint iAccLen;
		TInt iUsedInBuffer;
        
        // local output buffer
        TUint8* iOutBuffer;
        TInt iOutBufferCount;
        TInt iOutBufferLength;
        
        TUint8* iOutBufferPtr;
        TInt iOutFrameLength;

		const CMMFDataBuffer*	iSrc;
		CMMFDataBuffer*			iDst;

        TInt iMaxFrameSize;
        TInt iSizeOfInBuffer;

	};
	
#endif 		// CADVANCEDAUDIODECODER_H