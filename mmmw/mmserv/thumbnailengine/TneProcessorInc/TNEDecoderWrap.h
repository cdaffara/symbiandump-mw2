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
* Description:   TNE Decoder wrapper
*
*/




#ifndef TNEDECODERWRAP_H
#define TNEDECODERWRAP_H

// INCLUDES
#include <e32base.h>
#include <devvideoplay.h>
#include <devvideobase.h>
#include "ctrcommon.h" // @@ YHK Check do we need this


// FORWARD DECLARATIONS
class MTRDevVideoClientObserver;
class TVideoBuffer;

// Observer class for thumbnail generation
class MThumbnailObserver
{
public:
    virtual void MNotifyThumbnailReady(TInt aError) = 0;
    virtual void MPictureFromDecoder (TVideoPicture* aPicture) = 0;
    virtual void MReturnCodedBuffer(TVideoBuffer* aBuffer) = 0;
    virtual void MSendEncodedBuffer() = 0;
};


/**
*  TNE decoder wrap
*  @lib TNE.LIB
*/
NONSHARABLE_CLASS(CTNEDecoderWrap) : public CBase, public MMMFDevVideoPlayObserver
    {
    public: // Constuctor / destructor

        /**
        * Two-phased constructor.
        */
        static CTNEDecoderWrap* NewL(MThumbnailObserver* aObserver);

        /**
        * Destructor.
        */
        ~CTNEDecoderWrap();

        // Information methods
        /**
        * Checks codec support by MIME type
        * @param aFormat MIME type
        * @param aShortFormat MIME type (short version)
        * @return TBool value: ETrue - givemn MIME type is supported; EFalse - no support; 
        */
        TBool SupportsCodec(const TDesC8& aFormat, const TDesC8& aShortFormat);

        /**
        * Gets codec info
        * @param none
        * @return none
        */
        void GetCodecInfoL();

        /**
        * Sets codec parameters
        * @param aCodecLevel Codec level, aFormat video format, aOutputFormat Output video format
        * @return none
        */
        void SetDecoderParametersL(TInt aCodecLevel, const TTRVideoFormat& aFormat);

        /**
        * From MMMFDevVideoPlayObserver. Notifies the client that one or more new empty input buffers are available
        * @param none
        * @return none
        */
        void MdvpoNewBuffers();

        /**
        * From MMMFDevVideoPlayObserver. Returns a used input video picture back to the caller
        * @param aPicture Video picture
        * @return none
        */
        void MdvpoReturnPicture(TVideoPicture *aPicture);

        /**
        * From MMMFDevVideoPlayObserver. Delivers supplemental information from a coded data unit
        * @param aData Data, aTimestamp TimeStamp info, aPictureId Picture Id
        * @return none
        */
        void MdvpoSupplementalInformation(const TDesC8 &aData, const TTimeIntervalMicroSeconds &aTimestamp, 
                                          const TPictureId &aPictureId);

        /**
        * From MMMFDevVideoPlayObserver. Back channel information from the decoder, indicating a picture loss 
        * without specifying the lost picture
        * @param none
        * @return none
        */
        void MdvpoPictureLoss();

        /**
        * From MMMFDevVideoPlayObserver. Back channel information from the decoder, indicating the pictures that have been lost
        * @param aPictures Array with picture Ids
        * @return none
        */
        void MdvpoPictureLoss(const TArray< TPictureId > &aPictures);

        /**
        * From MMMFDevVideoPlayObserver. Back channel information from the decoder, indicating the loss of consecutive 
        * macroblocks in raster scan order
        * @param aFirstMacroblock First macroblock, aNumMacroblocks, Number of macroblocks, aPicture PictureId
        * @return none
        */
        void MdvpoSliceLoss(TUint aFirstMacroblock, TUint aNumMacroblocks, const TPictureId &aPicture);

        /**
        * From MMMFDevVideoPlayObserver. Back channel information from the decoder, indicating a reference picture 
        * selection request
        * @param aSelectionData Selection data
        * @return none
        */
        void MdvpoReferencePictureSelection(const TDesC8 &aSelectionData);

        /**
        * From MMMFDevVideoPlayObserver. Called when a timed snapshot request has been completed
        * @param aError Error, aPictureData Picture data, aPresentationTimestamp Presentation timestamp, aPictureId picture Id
        * @return
        */
        void MdvpoTimedSnapshotComplete(TInt aError, TPictureData *aPictureData, 
                                        const TTimeIntervalMicroSeconds &aPresentationTimestamp, const TPictureId &aPictureId);

        /**
        * From MMMFDevVideoPlayObserver. Notifies the client that one or more new output pictures are available
        * @param none
        * @return none
        */
        void MdvpoNewPictures();

        /**
        * From MMMFDevVideoPlayObserver. Reports a fatal decoding or playback error
        * @param aError Rin-time error
        * @return none
        */
        void MdvpoFatalError(TInt aError);

        /**
        * From MMMFDevVideoPlayObserver. Reports that DevVideoPlay initialization has completed
        * @param aError Init error status
        * @return none
        */
        void MdvpoInitComplete(TInt aError);

        /**
        * From MMMFDevVideoPlayObserver. Reports that the input video stream end has been reached and all pictures 
        * have been processed
        * @param none
        * @return none
        */
        void MdvpoStreamEnd();

        /**
        * Returns used videopicture
        * @param aPicture Video picture
        * @return none
        */
        void ReturnPicture(TVideoPicture* aPicture);

        /**
        * Starts decoding
        * @param none
        * @return none
        */
        void StartL();

        /**
        * Stops decoding synchronously
        * @param none
        * @return none
        */
        void StopL();

        /**
        * Stops decoding asynchronously
        * @param none
        * @return none
        */
        void AsyncStopL();

        /**
        * Checks decoder info
        * @param aUid Decoder Uid
        * @return TBool ETrue - accelerated codec; EFalse - non-accelerated codec
        */
        TBool CheckDecoderInfoL(TUid aUid);

        /**
        * Initialize decoder client
        * @param none
        * @return none
        */
        void InitializeL();

        /**
        * Select decoder
        * @param none
        * @return none
        */
        void SelectDecoderL();

        /**
        * Send buffer
        * @param aBuffer Media buffer
        * @return none
        */
        void SendBufferL(TVideoBuffer* aBuffer);

        /**
        * Write coded buffer
        * @param aBuffer Media buffer
        * @return none
        */
        void WriteCodedBufferL(TVideoBuffer* aBuffer);

        /**
        * Write coded buffer
        * @param aBuffer Media buffer
        * @return none
        */

		TUint GetNumInputFreeBuffers();
		
    private:
        /**
        * C++ default constructor.
        */
        CTNEDecoderWrap(MThumbnailObserver* aObserver);

        /**
        * Symbian 2nd phase constructor, can leave
        */
        void ConstructL();

    private:
        // Observer
        MThumbnailObserver* iObserver;

        // DevVideoPlay
        CMMFDevVideoPlay* iDevVideoPlay;

        // Decoder mime type
        TBuf8<256> iMimeType;
        
        // Short version mime type
        TBuf8<256> iShortMimeType;

        // Uncompressed format
        TUncompressedVideoFormat iUncompressedFormat;

        // Compressed format
        CCompressedVideoFormat* iCompresedFormat;

        // HwDevice Uid
        THwDeviceId iHwDeviceId;

        // Video coded input buffer
        TVideoInputBuffer* iInputBuffer;

        // Decoded picture
        TVideoPicture* iDecodedPicture;

        // Uid
        TUid iUid;

        // Coded buffer
        TVideoBuffer* iCodedBuffer;

        // Codec level
        TInt iCodecLevel;

        // Decoders to check
        RArray<TUid> iCheckDecoders;

        // array of Accelerated Decoders
        RArray<TUid> iAcceleratedDecoders;

        // Buffer options
        CMMFDevVideoPlay::TBufferOptions iBufferOptions;

        // Fatal error code
        TInt iFatalError;
        
        // decoder format
        TTRVideoFormat iFormat;
        
        // Data unit type
        TVideoDataUnitType iDataUnitType;
        
        // Stop
        TBool iStop;
        
        TInt iState;
        
        // Last ts
        TTimeIntervalMicroSeconds iLastTimestamp;
                
        // support for AVC
        
        //clock source
       	CSystemClockSource*				iClockSource;
       	
       	// keep track of frame sequence number
       	TInt iInputFrameNum;

    };


/**
*  Buffer class used in TNEProcessor
*
*/
class TVideoBuffer : public CBase
    {

    public:  // Constants

        // Buffer type
        enum TBufferType 
            {
            EAudioAMRNB = 0,
            EAudioAMRWB,            
            EVideoH263,
            EVideoMPEG4,
            EVideoMPEG4DecSpecInfo,
            EAudioQCELP13,
            EAudioDecSpecInfo,
            EAudioMPEG4AAC
            };        

    public:  // Constructors and destructor

        /**
        * C++ constructors. The 1st creates a buffer to be initialized later with Set, 
        * and the 2nd one initializes it already here
        */

        inline TVideoBuffer() {}

        inline TVideoBuffer(const TPtrC8& aBuffer, 
                        const TBufferType aType, 
                        const TInt aSize, 
                        const TBool aRandomAccessPoint,
                        const TTimeIntervalMicroSeconds& aTimeStamp) 
                        : iType(aType), iSize(aSize), iRandomAccessPoint(aRandomAccessPoint), iTimeStamp(aTimeStamp), iPtr(aBuffer) {}

        /**
        * Destructor.
        */
        inline ~TVideoBuffer() {}

    public:  // New functions

        /**                     
        * Set the buffer parameters. This enables reusing the buffer
        * @since 3.2
        * @param const TPtrC8& aBuffer                          Data buffer
        * @param const TBufferType aType                        Buffer type
        * @param const TInt aSize                               Buffer size (in bytes)
        * @param const TBool aRandomAccessPoint                 ETrue if buffer has a video random access point
        * @param const TTimeIntervalMicroSeconds& aTimeStamp    Timestamp of the buffer
        * @return void
        */
        inline void Set(const TPtrC8& aBuffer, 
                        const TBufferType aType, 
                        const TInt aSize, 
                        const TBool aRandomAccessPoint,
                        const TTimeIntervalMicroSeconds& aTimeStamp)
            {
            iType = aType;
            iSize = aSize;
            iRandomAccessPoint = aRandomAccessPoint;
            iTimeStamp = aTimeStamp;
            iPtr.Set(aBuffer);
            }

        /**                     
        * Return the data pointer
        * @since 3.2
        * @return TPtrC8 pointer
        */
        inline TPtrC8 Data() const { return iPtr; }

        /**
        * Return buffer type
        * @since 3.2
        * @return TInt Error code
        */
        inline TBufferType Type() const { return iType; } 

        /**
        * Write Return buffer size
        * @since 3.2
        * @return TInt Error code
        */
        inline TInt BufferSize() const { return iSize; }

        /**
        * Query whether this buffer contains a frame that can be used as a random-access point
        * @since 3.2
        * @return TBool ETrue if this is a random access point
        */
        inline TBool RandomAccessPoint() const { return iRandomAccessPoint; } 

        /**
        * Write a buffer to sink
        * @since 3.2
        * @return TTimeIntervalMicroSeconds Timestamp
        */
        inline TTimeIntervalMicroSeconds TimeStamp() const { return iTimeStamp; }

    private:  // Data

        // media type
        TBufferType iType;         

        // media buffer size in bytes
        TInt iSize;                

        // ETrue if the media contained in this buffer
        // can be used as random access point in playback        
        TBool iRandomAccessPoint;  
                                   
        // time stamp in microseconds counting from the beginning of rec.
        TTimeIntervalMicroSeconds iTimeStamp;           

        // descriptor for returning the data pointer
        TPtrC8 iPtr;               
    };



#endif  // TNEDECODERWRAP_H
