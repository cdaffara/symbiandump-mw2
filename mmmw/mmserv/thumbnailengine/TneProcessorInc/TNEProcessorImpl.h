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
* Description:   Header file for TNE processor implementation
*
*/



#ifndef     __TNEPROCESSORIMPL_H__
#define     __TNEPROCESSORIMPL_H__

#ifndef __E32BASE_H__
#include <e32base.h>
#endif

#include "TNEDecoderWrap.h"
#include "parser.h"
#include "mp4parser.h"
#include "TNEVideoClipInfoImp.h"

#include <TNECommon.h>

//  FORWARD DECLARATIONS
class CMP4Parser;
class CDisplayChain;

// used in image insertion
enum TVideoOperation  
{
    EVideoEncodeFrame = 0,
    EVideoWriteFrameToFile
};
		
// video frame parameters
struct TFrameParameters
{	
	TInt64 iTimeStamp;
	TUint8 iType;	
};
   

// Logging path
_LIT( KTNEModuleLogPath, "\\logs\\testframework\\TNEModule\\" ); 
// Log file
_LIT( KTNEModuleLogFile, "TNEEngine.txt" ); 


//  CLASS DEFINITIONS
class CTNEProcessorImpl : public CActive, public MThumbnailObserver
{
public:

    // Error codes
    enum TErrorCode
    {
        EInvalidProcessorState = -1000,
        EUnsupportedFormat = -1001,
        EVideoTooLarge = -1002,                                
        EStreamCorrupted = -1003,        
        EInvalidStreamData = -1004,
        EInvalidInternalState = -1050
        // Also error codes <EInvalidInternalState may be returned, they can be
        // considered fatal internal errors.
    };

    // Input data format
    enum TFileFormat
    {    
        EData3GP,  // 3GP
        EDataAutoDetect // autodetect type
    };    

public:

    struct TStreamParameters
    {
        TUint iWidth;
        TUint iHeight;
        TInt64 iPicturePeriodNsec;
        TUint iIntraFrequency;
        TUint iReferencePicturesNeeded; // 0 = disable RPS
        TUint iNumScalabilityLayers;
        TUint iLayerFrameRates[8]; // frames per 256 sec
    };
    
public:  // Functions from base classes

    /**
    * From CActive Active object running method
    */
    void RunL();

    /**
    * From CActive Active object cancelling method
    */    
    void DoCancel();

    /**
    * From CActive Active Error handling method
    */    
    TInt RunError(TInt aError);
    
    /**
    * From MThumbnailObserver. 
    * Notifies the client that the thumbnail is available
    * @param none
    * @return error code
    */
    void MNotifyThumbnailReady(TInt aError);

    /**
    * From MThumbnailObserver. 
    * noties the client tha decoded frame is ready
    * @param none
    * @return none
    */
    void MPictureFromDecoder(TVideoPicture* aPicture);
    

    /**
    * From MThumbnailObserver. 
    * Returns media bitstream buffer to the client
    * @param none
    * @return none
    */
    void MReturnCodedBuffer(TVideoBuffer* aBuffer);

    /**
    * From MThumbnailObserver. 
    * Request for more buffers from decoder
    * @param none
    * @return none
    */
    void MSendEncodedBuffer();
    

public:  // New functions
	
    /** 
     * Constructors for instantiating new video processors.
     * Should reserve as little resources as possible at this point.
	 */
	static CTNEProcessorImpl* NewL();
	static CTNEProcessorImpl* NewLC();
	
	/** 
	 * Destructor can be called at any time (i.e., also in the middle of a processing operation)
	 * Should release all allocated resources, including releasing all allocated memory and 
	 * *deleting* all output files that are currently being processed but not yet completed.
	 */
	virtual ~CTNEProcessorImpl();
		
	
	/**
	 * Do all initializations necessary to start generating a thumbnail, e.g. open files, 
	 * allocate memory. The video clip file should be opened with EFileShareReadersOnly 
	 * share mode. The thumbnail should be scaled to the specified resolution and 
	 * converted to the specified display mode. If this method leaves, destructor should be called to free 
	 * allocated resources.
	 * 
	 * Possible leave codes:
	 *	- <code>KErrNoMemory</code> if memory allocation fails
	 *	- <code>KErrNotFound</code> if there is no file with the specified name
	 *    in the specified directory (but the directory exists)
	 *	- <code>KErrPathNotFound</code> if the specified directory
	 *    does not exist
	 *	- <code>KErrUnknown</code> if the specified file is of unknown format
	 *  - <code>KErrNotSupported</code> if the specified combination of parameters
	 *                                  is not supported
	 *
	 * @param aFileHandle  file handle to the clip from which to generate the thumbnail 
     * @param aIndex       Frame index for selecting the thumbnail frame
     *                     -1 means the best thumbnail is retrieved
 	 * @param aResolution  resolution of the desired thumbnail bitmap, or
	 *                     <code>NULL</code> if the thumbnail should be
	 *                     in the original resolution
	 * @param aDisplayMode desired display mode; or <code>ENone</code> if 
	 *                     any display mode is acceptable
	 * @param aEnhance	   apply image enhancement algorithms to improve
	 *                     thumbnail quality; note that this may considerably
	 *                     increase the processing time needed to prepare
	 *                     the thumbnail
	 */
	void StartThumbL(RFile& aFileHandle, TInt aIndex, TSize aResolution, 
		TDisplayMode aDisplayMode, TBool aEnhance);		
	
	/**
	 * Starts thumbnail generation. Thumbnail generation is an asynchronous operation,
	 * and its completion is informed to the caller via Active object request completion;
	 * the iStatus member of the caller is passed as a parameter to this method.
	 *
	 * This method may leave if an error occurs in initiating the thumbnail generation.	
	 * If this method leaves, destructor should be called to free allocated resources.	
	 * 
	 * @param aStatus     Reference to caller's iStatus member variable
	 *
	 * @return  
	 *          
	 */
	void ProcessThumbL(TRequestStatus &aStatus);

	/**
	 * Decodes the encoded frame for thumbnail generation. Thumbnail generation is an asynchronous operation,
	 * and its completion is informed to the caller via Active object request completion;
	 * the iStatus member of the caller is passed as a parameter to this method.
	 *
	 * This method may leave if an error occurs in initiating the thumbnail generation.	
	 * If this method leaves, destructor should be called to free allocated resources.	
	 * 
	 * @param aStatus     Reference to caller's iStatus member variable
	 *
	 * @return  
	 *          
	 */
	void DecodeThumb(TBool aFirstFrame);

	 /** 
	 * Method for retrieving the completed thumbnail bitmap.
	 * 
	 * Video processor should not free the CFbsBitmap instance after it has passed it on 
	 * as a return value of this function 
	 *	 
	 */
	void FetchThumb(CFbsBitmap*& aThumb);
	
	/**
	 * Read clip header from the specified file and return its properties.	 
	 * This method should leave if clip is invalid, cannot be opened, etc.
	 *
	 * @param aFileHandle  file handle to the file for reading
	 * @param aVideoFormat  for returning the video format
     * @param aVideoType Type of video data
	 * @param aResolution  for returning the resolution
     * @param aVideoFrameCount Number of video frames in the clip	 
	 */
	void GetClipPropertiesL(RFile& aFileHandle, TTNEVideoFormat& aFormat,
		                    TTNEVideoType& aVideoType, TSize& aResolution,
		                    TInt& aVideoFrameCount);	

    /**
     * Enhances the visual quality of the frame
     *     
     */				
	void EnhanceThumbnailL(const CFbsBitmap* aInBitmap, CFbsBitmap* aTargetBitmap); 


	/**
	* Get the file name of the clip
	*     
	* @return File name of the clip
	*/				
	inline TFileName GetClipFileName() const { return iClipFileName; }

    /**
     * Get number of frames in  clip
     *     
     * @return Number of frames in clip
     */
    inline TInt GetClipNumberOfFrames() const { return iParser->GetNumberOfFramesInClip(); }	
	
	/**
     * Get the timescale of the current video clip
     *     
     * @return Timescale
     */
	inline TInt GetVideoClipTimeScale() const { return iParser->iStreamParameters.iVideoTimeScale; }


    /**
     * Get video frame index based on timestamp
     *
     * @param aTime Frame timestamp in microseconds
     * @return Frame index
     */    
    TInt GetVideoFrameIndex(TTimeIntervalMicroSeconds aTime) const;

    /**
     * Get frame index for the first frame in current clip
     *
     * @param aTime Timestamp in microseconds
     * @return Frame index
     */
    inline TInt GetStartFrameIndex() const { return iStartThumbIndex; }

    /**
     * Get frame type at specified index
     *
     * @param aIndex Frame index
     * @return Frame type
     */
    inline TInt8 GetVideoFrameType(TInt aIndex) const { return iParser->GetVideoFrameType(aIndex); };

	/**
     * Retrieve a video YUV frame from the tmp file
     * @param aBuf Descriptor containing the YUV frame
     * @param aLength Length of the descriptor
     * @param aDuration The frame duration
     * @param aTimeStamp The frame timestamp
     * @return Error code
     */
    TInt GetVideoFrameFromFile(TDes8& aBuf, TInt aLength, TInt& aDuration, TInt64& aTimeStamp);

	/**
     * Get the next frame duration and timestamp
     * @param aDuration The frame duration
     * @param aTimeStamp The frame timestamp
     * @param aIndex Index of timestamp to get
     * @param aTimeStampOffset Offset for timestamp
     * @return Error code
     */
    void GetNextFrameDuration(TInt& aDuration, TInt64& aTimeStamp, TInt aIndex, TInt& aTimeStampOffset);

	/**
     * Get the resolution of the current video clip
     *          
     * @return resolution
     */
	TSize GetVideoClipResolution();
	
    /**
     * Get video timestamp in ms from timestamp in ticks
     *
     * @param aTimeStampInTicks Time in ticks
     * @param aCommonTimeScale ETrue for using the output time scale
     *                         EFalse for using the scale of current clip
     * @return Timestamp in milliseconds
     */        
    TInt64 GetVideoTimeInMsFromTicks(TInt64 aTimeStampInTicks, TBool aCommonTimeScale) const;    
    
    /**
     * Stops processing
     *
     * @return Error code
     */
	TInt Stop();    

    /**
     * Stops processing and closes all submodules except status monitor & video encoder
     *
     * @return Error code
     */
    TInt Close();

    /**
    * Checks if a frame has "good" or "legible" quality
    *
    * @param aYUVDataPtr Pointer to the frame to be checked
    *
    * @return 1 if frame quality is OK, 0 otherwise
    */
    TInt CheckFrameQuality(TUint8* aYUVDataPtr);

    /**
     * Gets the average frame rate of current video clip
     *
     * @return Frame rate
     */
    TReal GetVideoClipFrameRate();
								
private: // Constants
	
	// state	
    enum TProcessorState
    {
        EStateIdle = 0,		
        EStateOpened,   // clip is open at the decoder (?)
        EStatePreparing,
        EStateReadyToProcess,
        EStateProcessing        
    };
	
	
	// Video types
	enum TVideoType
	{
		EVideoNone = 0,
		EVideoH263Profile0Level10,
		EVideoH263Profile0Level45,
		EVideoMPEG4
	};
	
	// Multiplex types
	enum TMuxType
	{
        EMuxNone = 0,
		EMux3GP		
	};     

    enum TDataFormat
    {
        EDataUnknown = 0,
        EDataH263,
        EDataMPEG4,
        EDataAVC
    };
    
private:

    /**
    * By default Symbian OS constructor is private.
	*/
    void ConstructL();

    /**
    * c++ default constructor
	*/
    CTNEProcessorImpl();    
    
    /**
     * Opens a 3gp clip for processing
     *
     * @param aFileName Clip filename
     * @param aDataFormat Clip file format
     *
     * @return Error code
     */
	TInt OpenStream(RFile& aFileHandle, TFileFormat aDataFormat);

    /**
     * Closes the processed clip from parser
     *
     * @return Error code
     */
	TInt CloseStream();

    /**
     * Prepares the processor for processing a movie
     *
     * @return Error code
     */
	TInt Prepare();
    
    /**
     * Deletes objects used in processing
     *     
     */
    void DoCloseVideoL();

	/**
     * Sets output audio & video formats
     *          
     */
	void SetOutputMediaTypesL();

	/**
     * Gets bitstream modes of input clips
     *          
     */
	void GetTranscodeFactorsL();

	/**
     * Sets video transcoding parameters
     *          
     */
	void SetupTranscodingL();

    /**
     * Resets variables used in processing
     *          
     */
    void Reset();

    /**
     * Sets suitable default values for parameters
     *          
     */
    void SetHeaderDefaults();

    /**
     * Parses the clip header
     *
     * @param aStreamParams Destination structure for stream parameters
     * @param Filename of the clip
     *          
     */
	void ParseHeaderOnlyL(CParser::TStreamParameters& aStreamParams, RFile& aFileHandle); 

    /**
     * Copies stream parameters from source to destination structure
     *
     * @param aDestParameters Destination structure
     * @param aSrcParameters Source structure     
     *          
     */
	void UpdateStreamParameters(CParser::TStreamParameters& aDestParameters, 
		CParser::TStreamParameters& aSrcParameters);    
	
    /**
     * Parse the clip header & update internal variables accordingly
     *     
     */
	void ParseHeaderL();	
    
    /**
     * Initializes the processor for the next clip to be processed,
     * called for each clip    
     */
	void InitializeClipL(RFile& aFileHandle);

    
    /**
     * Reports an error in thumbnail generator
     * to the caller
     *
     * @return ETrue if error was reported, EFalse otherwise
     */
    TBool HandleThumbnailError(TInt aError);
    
    /**
     * Create and Initialize the decoder 
     */
	void CreateAndInitializeDecoderL();

    /**
     * Parse the clips mime type 
     */
	void ParseMimeTypeL();

    /**
     * Get the video frame format 
     */
	TInt GetFrameL();

    /**     
     * Writes a buffered frame to output file
     *
     * @return Error code
     */
    TInt ReadVideoFrame();

    /** 
     * CTNEProcessorImpl::ReadAndUpdateFrame
     * Reads a frame from input queue to internal buffer
     * (other items were commented in a header).
     */			
     TInt ReadAndUpdateFrame(); 

private:  // Data

    // current processor state
	TProcessorState iState; 

     // current stream data format	
	TFileFormat iFileFormat;

	// stream start reading buffer
	TUint8 *iReadBuf; 
    // buffer length
	TUint iBufLength; 

	// wait scheduler
    CActiveSchedulerWait *iWaitScheduler;
    TBool iWaitSchedulerStarted;

    // Filename of the current clip
    TFileName iClipFileName;

    // Thumbnail resolution
	TSize iOutputThumbResolution;    
	// Thumbnail index
	TInt iThumbIndex; 
	// Thumbnail display mode
	TDisplayMode iThumbDisplayMode; 
	// Thumbnail enhance
	TBool iThumbEnhance; 

	// Flag for indicating decoder wrapper state
	TBool iDecoderInitPending;	

//******************************************
	
	// data buffer for the current compressed frame
    TUint8 *iDataBuffer;  
    // buffer total length
    TUint iBufferLength; 
    // amount of data in buffer
    TUint iDataLength; 
    // current stream data format	
	TDataFormat iDataFormat;
	// length of the current frame in the buffer
    TUint iCurrentFrameLength;
    
    // for searching a good thumbnail 
    TInt iFramesToSkip;
    
    // number of frame skip cycles done to get a good thumbnail  
    TInt iNumThumbFrameSkips;  
    
    // is decode in progress ?                              
	TBool iDecodePending;  

	// timestamp of previous frame
    TTimeIntervalMicroSeconds iPreviousTimeStamp; 

     // the video decoder
    CTNEDecoderWrap *iDecoder;

	// CCMR media buffer sent to decoder
	TVideoBuffer* iMediaBuffer;	

	// Concatenated YUV data for decoded frame
    TUint8 *iFrameBuffer;  
    
	// are we decoding?
    TBool iDecoding; 

    // Input mimetype
    TBufC8<256> iMimeType;
    // Short mimetype
    TBufC8<256> iShortMimeType;

	// Codec level
	TInt iCodecLevel;
	// Input codec
	TInt iCodec;
	// Input Max Bit Rate
	TUint iMaxBitRate;
	
	// AVC Stuff
	// number of bytes representing the NalUnitLength
	TInt iNalUnitBytes;
	
//***********************************************	


	// True if processing has been cancelled
	TBool iProcessingCancelled;

	// timestamp for encoding ending black frames
	TTimeIntervalMicroSeconds iTimeStamp;

    // stream duration: max. of audio,video
	TInt64 iVideoClipDuration;

    // output video buffer for one frame
    HBufC8 *iOutVideoBuffer;

    // Is there a frame in buffer ?
    TBool iFrameBuffered;

    // Timestamp of buffered frame
    TInt64 iBufferedTimeStamp;

    // key frame info of buffered frame
    TBool iBufferedKeyFrame;

    TBool iFirstFrameBuffered;

    // Are we processing a thumbnail ?
	// TBool iThumbnailInProgress; 
		
    // index of the first frame in clip
    TInt iStartThumbIndex;	

	// @@ YHK HARI 
	TInt iAVCDecodedFrameNumber;
	TBool iThumbFrameDecoded;

    // buffer pointers for thumbnail generation
    TUint8* iYuvBuf;
    TUint8* iRgbBuf;

	// for storing the converted YUV image in generation case
	TUint8 *iYuvImageBuf; 

	// flag for reseting the encoder to encode an intra frame
	TBool iIntraResetRequest;
	
	TInt iMP4SpecificSize; 

	// Is it the first frame of a clip
    TBool iFirstFrameOfClip;

    // for setting the first frame flag
    // when doing wipe/crossfade transitions
    TBool iFirstFrameFlagSet;

    TInt iVideoFrameNumber;
    TInt iVideoIntraFrameNumber;


    // MPEG-4 VOS size 
    TInt iOutputMpeg4VosSize;
    
// @@ YHK demux
    // the demultiplexer    
//    CDemultiplexer *iDemux; 

    // the video decoder input queue
//    CActiveQueue *iVideoQueue; 
    
     // file format parser
    CMP4Parser *iParser;   
    
    // frame enhancement
    CDisplayChain *iEnhancer;  

    // does the stream have video?
    TBool iHaveVideo; 
    // video stream type
	TVideoType iVideoType; 
	
    // is the stream multiplexed?
	TBool iIsMuxed; 
    // multiplex type					
	TMuxType iMuxType; 
		

    // amount of data in buffer
	TUint iBufData; 
    // reading descriptor	
	TPtr8 iReadDes; 
			   
    // demux parameters read
//	CMP4Demux::TStreamParameters iMP4Parameters; 
//	CMP4Demux::TOutputChannel iMP4Channels[2];
		
    // video stream parameters
	CTNEProcessorImpl::TStreamParameters iVideoParameters; 

    // stream length in milliseconds
	TUint iStreamLength; 
    // stream size in bytes
	TUint iStreamSize; 
    // stream average bitrate
	TUint iStreamBitrate; 
    // is it possible to seek in the file	
	TBool iCanSeek; 


	RFile iCurClip;
    RFs   iFs;
    TBool iFsConnected;	

    TInt           iCurClipIndex;


	TInt iCurrentVideoSize;
    
    TRequestStatus *iThumbnailRequestStatus;
    CFbsBitmap* iOutBitmap;
    
    friend class CMP4Parser;
};


#endif      //  __TNEPROCESSORIMPL_H__

// End of File
