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
* Description:   Implementation for TNE processor
*
*/



//  Include Files
#include "TNEProcessorImpl.h"
#include "yuv2rgb12.h"
#include "yuv2rgb16.h"
#include "yuv2rgb24.h"
#include "DisplayChain.h"
#include "TNEVideosettings.h"
#include "ctrsettings.h"
#include "mp4parser.h"
#include "TNEDecoderWrap.h"

//  Local Constants
const TUint KReadBufInitSize = 512; // stream start buffer initial size
const TUint KInitialDataBufferSize = 8192; // initial frame data buffer size
const TUint KH263StartCodeLength = 3;  // H.263 picture start code length
const TUint KMPEG4StartCodeLength = 4; // MPEG4 picture start code length

// An assertion macro wrapper to clean up the code a bit
#define VPASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CTNEProcessorImpl"), EInvalidInternalState))

// An assertion macro wrapper to clean up the code a bit
#define VDASSERT(x, n) __ASSERT_DEBUG(x, User::Panic(_L("CTNEProcessorImpl"), EInternalAssertionFailure+n)) 

#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x;
#else
#define PRINT(x)
#endif

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTNEProcessorImpl* CTNEProcessorImpl::NewL() 
{
	CTNEProcessorImpl* self = NewLC();
	CleanupStack::Pop(self);
	return self;
}

CTNEProcessorImpl* CTNEProcessorImpl::NewLC()
{
	CTNEProcessorImpl* self = new (ELeave) CTNEProcessorImpl();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::CTNEProcessorImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTNEProcessorImpl::CTNEProcessorImpl()
: CActive(EPriorityNormal), iReadDes(0, 0)
{
    // Reset state
    iState = EStateIdle;
	iFileFormat = EDataAutoDetect;	
}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::~CTNEProcessorImpl
// Destructor.
// -----------------------------------------------------------------------------
//
CTNEProcessorImpl::~CTNEProcessorImpl()
{
    Cancel();

    TInt error = KErrNone;

	TRAP(error, DoCloseVideoL());	

	if(iParser)
    {
        delete iParser;
        iParser = 0;
    }

	// Deallocate buffers
	// This is allocated in the clip startFrame() method
	if (iFrameBuffer)
	{
		User::Free(iFrameBuffer);
		iFrameBuffer = 0;		
	}

	// Deallocate buffers
	if (iDataBuffer)
	{
		User::Free(iDataBuffer);
		iDataBuffer = 0;		
	}
	
    if (iRgbBuf)
    {
        delete iRgbBuf;
        iRgbBuf = 0;
    }        
    
    if (iOutBitmap)
    {
        delete iOutBitmap;
        iOutBitmap = 0;
    }
    
    if (iReadBuf)
    {
		User::Free(iReadBuf);
		iReadBuf = 0;		
	}
	
	if (iMediaBuffer)
	{
		delete iMediaBuffer;
		iMediaBuffer = 0;
	}

}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::ConstructL()
{
    // Add to active scheduler
    CActiveScheduler::Add(this);
	
	// Allocate buffer
	iDataBuffer = (TUint8*) User::AllocL(KInitialDataBufferSize);
	iBufferLength = KInitialDataBufferSize; 
	
	// Allocate stream reading buffer
	iReadBuf = (TUint8*) User::AllocL(KReadBufInitSize);
	iBufLength = KReadBufInitSize;
	
    iMediaBuffer = new (ELeave)TVideoBuffer;
	
    iDecodePending = EFalse; 
	iDecoding = EFalse; 
	
    // Flag to indicate if the frame has been decoded
	iThumbFrameDecoded = EFalse;
	           
    iState = EStateIdle;
}


// -----------------------------------------------------------------------------
// CTNEProcessorImpl::Reset
// Resets the processor for processing a new movie
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::Reset()
{
    
	iFileFormat = EDataAutoDetect;
	iVideoType = EVideoH263Profile0Level10;
    iFirstFrameOfClip = EFalse;
    iFirstFrameFlagSet = EFalse;

	iProcessingCancelled = EFalse;
    
    iWaitSchedulerStarted = EFalse;
    	
    iVideoFrameNumber = 0;
    iFrameBuffered = EFalse;
    iVideoIntraFrameNumber = 0;
    iStartThumbIndex = 0;
	iVideoClipDuration = 0; 
	iDecoderInitPending = EFalse;   
   
    // @@ YHK HARI AVC frame number flag
    // need to have a more inclusive approach for 
    // all the different encoded streams
    iAVCDecodedFrameNumber = 0;
    
    // We are now properly initialized
    iState = EStateIdle;
}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::GetClipPropertiesL
// Retrieves parameters for a clip
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::GetClipPropertiesL(RFile& aFileHandle, 
                                             TTNEVideoFormat& aFormat,
                                             TTNEVideoType& aVideoType,
                                             TSize& aResolution,
                                             TInt& aVideoFrameCount)
{	
    PRINT((_L("CTNEProcessorImpl::GetClipPropertiesL() begin")))

	// parse clip header
	CParser::TStreamParameters iStreamParams;	

	// parse header 
	TRAPD(error, ParseHeaderOnlyL(iStreamParams, aFileHandle));

    if (error != KErrNone && error != KErrNotSupported)
        User::Leave(error);
	
	/* pass back clip properties */

	// video format (file format actually)
	if (iStreamParams.iFileFormat == CParser::EFileFormat3GP)
		aFormat = ETNEVideoFormat3GPP;
	else if (iStreamParams.iFileFormat == CParser::EFileFormatMP4)
		aFormat = ETNEVideoFormatMP4;
	else
		aFormat = ETNEVideoFormatUnrecognized;

	// video type	
	if(iStreamParams.iVideoFormat == CParser::EVideoFormatNone)
		aVideoType = ETNEVideoTypeNoVideo;
	else if (iStreamParams.iVideoFormat == CParser::EVideoFormatH263Profile0Level10)
		aVideoType = ETNEVideoTypeH263Profile0Level10;
	else if (iStreamParams.iVideoFormat == CParser::EVideoFormatH263Profile0Level45)
		aVideoType = ETNEVideoTypeH263Profile0Level45;
	else if(iStreamParams.iVideoFormat == CParser::EVideoFormatMPEG4)
		aVideoType = ETNEVideoTypeMPEG4SimpleProfile;
	else if(iStreamParams.iVideoFormat == CParser::EVideoFormatAVCProfileBaseline)
		aVideoType = ETNEVideoTypeAVCProfileBaseline;
	else
		aVideoType = ETNEVideoTypeUnrecognized;
	
	// resolution
	aResolution.iWidth = iStreamParams.iVideoWidth; 
	aResolution.iHeight = iStreamParams.iVideoHeight; 

	// get total number of video frames
	aVideoFrameCount = iParser->GetNumberOfVideoFrames();

    PRINT((_L("CTNEProcessorImpl::GetClipPropertiesL() end")))
}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::StartThumbL
// Initiates thumbnail extraction from clip (full resolution raw is reutrned)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::StartThumbL(RFile& aFileHandle, 
									  TInt aIndex, 
									  TSize aResolution,
									  TDisplayMode aDisplayMode, 
									  TBool aEnhance)
{
	PRINT((_L("CTNEProcessorImpl::StartThumbL() begin, aIndex = %d, enhance = %d"), aIndex, aEnhance))

	// Get thumbnail parameters
	iOutputThumbResolution.SetSize(aResolution.iWidth, aResolution.iHeight);
	iThumbIndex = aIndex;
	iThumbDisplayMode = aDisplayMode;
	iThumbEnhance = aEnhance;

    // opens the file & parses header
	InitializeClipL(aFileHandle); 

	// Allocate memory for iFrameBuffer
    TInt length = iVideoParameters.iWidth * iVideoParameters.iHeight;
    length += (length>>1);
    iFrameBuffer = (TUint8*)User::AllocL(length);

	PRINT((_L("CTNEProcessorImpl::StartThumbL() end")))
}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::ProcessThumbL
// Generates thumbnail from clip (actually, full resolution raw is returned)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::ProcessThumbL(TRequestStatus &aStatus)
{				
	PRINT((_L("CTNEProcessorImpl::ProcessThumbL() begin")))

    iState = EStateProcessing;	
    iThumbnailRequestStatus = &aStatus;
    
    // seek to the last intra frame before desired frame 
    TTimeIntervalMicroSeconds startTime(0);
    if ( iThumbIndex > 0 )
    {            
        TInt time = 0;
        TUint inMs = TUint( iParser->GetVideoFrameStartTime(iThumbIndex, &time) );
        TInt64 inMicroS = TInt64( inMs ) * TInt64( 1000 );            
        startTime = TTimeIntervalMicroSeconds( inMicroS );
    }
    
    
    TInt error = iParser->SeekOptimalIntraFrame(startTime, iThumbIndex);
    if (error != KErrNone)
    { 
        iThumbnailRequestStatus = 0;
        User::Leave(KErrGeneral);
    }
    
    iStartThumbIndex = iParser->GetStartFrameIndex();
    
    // @@ YHK: Try to handle this more inclusively
    iAVCDecodedFrameNumber = iStartThumbIndex;
    
    VPASSERT(iStartThumbIndex >= 0);                  
  
    // determine input stream type    
    TRAP(error, GetFrameL());
            
	iDecoding = ETrue;
		
	// Create and Initialize the Decoders
	TRAP(error, CreateAndInitializeDecoderL());
    if (error != KErrNone)
    {
    	// @@ YHK Do we want to use this flag ??
        iThumbnailRequestStatus = 0;
        User::Leave(KErrGeneral);
    }        
}

// ---------------------------------------------------------
// CH263Decoder::DecodeThumb
// Decode a thumbnail frame internally
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTNEProcessorImpl::DecodeThumb(TBool aFirstFrame)
{
 
    if (aFirstFrame)
    {
        // frame read in iDataBuffer, decode
        TVideoBuffer::TBufferType bt = 
            (iDataFormat == EDataH263) ? TVideoBuffer::EVideoH263 : TVideoBuffer::EVideoMPEG4;                        

        TInt startTimeInTicks=0;
        TInt startTimeInMs = 0;
        startTimeInMs = iParser->GetVideoFrameStartTime(iStartThumbIndex,&startTimeInTicks);
        
        TTimeIntervalMicroSeconds ts = 
            TTimeIntervalMicroSeconds(startTimeInMs * TInt64(1000) );
                
        iMediaBuffer->Set( TPtrC8(iDataBuffer, iBufferLength), 
                                  bt, 
                                  iCurrentFrameLength, 
                                  ETrue, // keyFrame
                                  ts
                                  );
                                  
        iPreviousTimeStamp = ts;
                                         
        iDecodePending = ETrue;
        
        if (!IsActive())
        {
            SetActive();
            iStatus = KRequestPending;                                
        }

        TRAPD( err, iDecoder->WriteCodedBufferL(iMediaBuffer) );
        if (err != KErrNone)
        {
            // ready
            MNotifyThumbnailReady(err);
            return;            
        }

		TUint freeInputBuffers = iDecoder->GetNumInputFreeBuffers();

        // @@ YHK: AVC hack is there a better way of doing this ??
        if(iDataFormat == EDataAVC)
        {
	        if (freeInputBuffers != 0)
	        {
	            // activate object to end processing
	            TRequestStatus *status = &iStatus;
	            User::RequestComplete(status, KErrNone);                   	
	        }
        }
        return;
    }
    
    if ((iThumbIndex == 0) && iThumbFrameDecoded)
    {        
        MNotifyThumbnailReady(KErrNone);
        return;
    }
 
    iStartThumbIndex++;
    
    if ((iThumbIndex < 0) && iThumbFrameDecoded)
    {       	     
        if (iFramesToSkip == 0)
        {            
             PRINT((_L("CH263Decoder::ProcessThumb() frameskip done %d times"), iNumThumbFrameSkips));

             // limit the number of frame skip cycles to 3, because with
		     // near-black or near-white videos we may never find a good thumb.
		     // => max. 30 frames are decoded to get the thumb
             
             // check quality & frame skip cycles
             if ( CheckFrameQuality(iFrameBuffer) || iNumThumbFrameSkips >= 3 )
             {
                 // quality ok or searched long enough, return
                 MNotifyThumbnailReady(KErrNone);
                 return;              
             }
             iFramesToSkip = 10;
             iNumThumbFrameSkips++;
        }
        else
            iFramesToSkip--;
        
        // read new frame & decode                           
    }
    
    if (iThumbFrameDecoded)
    {
    	iAVCDecodedFrameNumber++;
    	iThumbFrameDecoded = EFalse;
    }
 
    
    if (iThumbIndex > 0)
    {            
    	// HARI AVC IMP THUMB
    	TInt decodedFrameNumber = (iDataFormat == EDataAVC) ? iAVCDecodedFrameNumber : iStartThumbIndex;

        if (decodedFrameNumber > iThumbIndex)        
        {
         	// ready
            MNotifyThumbnailReady(KErrNone);
            return; 
        }
        // read new frame & decode
    }
 
    TInt error;
    
    if(iStartThumbIndex < iParser->GetNumberOfVideoFrames()) // do not read last frame (already read!)
    {   
        error = ReadVideoFrame();
        if (error != KErrNone)
        {
            MNotifyThumbnailReady(error);
            return;
        }
    }
    else
    {
        // no frames left, return
        MNotifyThumbnailReady(KErrNone);
        return;
    }
    
    iCurrentFrameLength = 0;
    iDataFormat = EDataUnknown;
    
    if (ReadAndUpdateFrame())
    {
        // frame read in iDataBuffer, decode
        TVideoBuffer::TBufferType bt = 
            (iDataFormat == EDataH263) ? TVideoBuffer::EVideoH263 : TVideoBuffer::EVideoMPEG4;
        
        TInt startTimeInTicks=0;
        TInt startTimeInMs = 0;
        startTimeInMs = iParser->GetVideoFrameStartTime(iStartThumbIndex,&startTimeInTicks);
        
        TTimeIntervalMicroSeconds ts = 
            TTimeIntervalMicroSeconds(startTimeInMs * TInt64(1000) );
            
        if (ts <= iPreviousTimeStamp)
        {            
            // adjust timestamp so that its bigger than ts of previous frame
            TReal frameRate = GetVideoClipFrameRate();
                        
            TInt64 durationMs =  TInt64( ( 1000.0 / frameRate ) + 0.5 );
            durationMs /= 2;  // add half the duration of one frame
            
            ts = TTimeIntervalMicroSeconds( iPreviousTimeStamp.Int64() + durationMs*1000 );
        }
        
        iPreviousTimeStamp = ts;

        iMediaBuffer->Set( TPtrC8(iDataBuffer, iBufferLength), 
                                  bt, 
                                  iCurrentFrameLength, 
                                  GetVideoFrameType(iStartThumbIndex),
                                  ts );                                                                            

        iDecodePending = ETrue;
        if (!IsActive())
        {
            SetActive();
            iStatus = KRequestPending;                                
        }
               
        TRAPD( err, iDecoder->WriteCodedBufferL(iMediaBuffer) );
        if (err != KErrNone)
        {
            MNotifyThumbnailReady(err);            
        }

		TUint freeInputBuffers = iDecoder->GetNumInputFreeBuffers();
		
        // HARI AVC hack is there a better way of doing this ??
        if(iDataFormat == EDataAVC)
        {
	        if (freeInputBuffers != 0)
	        {
	            // activate object to end processing
	            TRequestStatus *status = &iStatus;
	            User::RequestComplete(status, KErrNone);                   	
	        }
        }
      
        return;        
    }
    else
    {
        MNotifyThumbnailReady(KErrCorrupt);
        return;        
    }
}

void CTNEProcessorImpl::MSendEncodedBuffer()
{
	DecodeThumb(EFalse);
}


void CTNEProcessorImpl::MPictureFromDecoder(TVideoPicture* aPicture)
{
        TInt yuvLength = iVideoParameters.iWidth*iVideoParameters.iHeight;
        yuvLength += (yuvLength >> 1);
        
        // Indicate that the decoded frame has been received
        iThumbFrameDecoded = ETrue;

        // copy to iFrameBuffer	
	    Mem::Copy(iFrameBuffer, aPicture->iData.iRawData->Ptr(), yuvLength);
	    
        // release picture
        TInt error = KErrNone;
        TRAP( error, iDecoder->ReturnPicture(aPicture) );
        if ( error != KErrNone )
        {
            MNotifyThumbnailReady(error);
            return;
        }
        
        //VDASSERT(iDecodePending, 33);
        // complete request    
        TRequestStatus *status = &iStatus;
        User::RequestComplete(status, KErrNone);
        
        return;
}

void CTNEProcessorImpl::MReturnCodedBuffer(TVideoBuffer* /*aBuffer*/)
{
    // Don't have to do anything here
    return;
}

// ---------------------------------------------------------
// CTNEProcessorImpl::CreateAndInitializeDecoderL
// Create and initialize decoder 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTNEProcessorImpl::CreateAndInitializeDecoderL()
{
    PRINT((_L("CTNEProcessorImpl::CreateAndInitializeDecoderL() begin")));    
    TTRVideoFormat videoInputFormat;
    
  	// parse the mime type: Get the codeclevel, max bitrate etc...
	ParseMimeTypeL();
    
    // Create decoder wrapper object 
    iDecoder = CTNEDecoderWrap::NewL(this);

    // Check to see if this mime type is supported
    if ( !(iDecoder->SupportsCodec(iMimeType, iShortMimeType)) )
    {
        User::Leave(KErrNotSupported);
    }            

    videoInputFormat.iSize = TSize(iVideoParameters.iWidth, iVideoParameters.iHeight);
	videoInputFormat.iDataType = ETRDuCodedPicture; 
	   
	// set the codec params
    iDecoder->SetDecoderParametersL(iCodecLevel, videoInputFormat);

    iDecoderInitPending = ETrue;               

	// Activate the processor object
    if (!IsActive())    
    {        
        SetActive();
        
    	TRequestStatus* status = &iStatus;
    	User::RequestComplete(status, KErrNone);
    }
    
    // initialize the decoder
    iDecoder->InitializeL();
		
}

// -----------------------------------------------------------------------------
// CTRTranscoderImp::ParseMimeTypeL
// Parses given MIME type
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::ParseMimeTypeL()
{
    TUint maxBitRate = 0;
    TInt codecType = 0;
    TBuf8<256> shortMimeType;
    TBuf8<256> newMimeType;
    TInt width = iVideoParameters.iWidth;
    TUint codecLevel = 0;

    if ( iDataFormat == EDataH263 )
    {
        // H.263
        codecType = EH263;
        shortMimeType = _L8("video/H263-2000");
        newMimeType = shortMimeType;

		switch( width )
		{
			case KTRSubQCIFWidth:
			case KTRQCIFWidth:
			{
				// Set defaults for level=10;
				maxBitRate = KTRMaxBitRateH263Level10;
				codecLevel = KTRH263CodecLevel10;
				newMimeType += _L8("; level=10");
				break;
			}

			case KTRCIFWidth:
			{
                        // Set defaults for level=30;
                        maxBitRate = KTRMaxBitRateH263Level30;
                        codecLevel = KTRH263CodecLevel30;
                        newMimeType += _L8("; level=30");
                        break;
			}

			case KTRPALWidth:
			{
                        // Set defaults for level=60;
                        maxBitRate = KTRMaxBitRateH263Level60;
                        codecLevel = KTRH263CodecLevel60;
                        newMimeType += _L8("; level=60");
                        break;
			}

			default:
			{
                        // Set defaults for level=10;
                        maxBitRate = KTRMaxBitRateH263Level10;
                        codecLevel = KTRH263CodecLevel10;
                        newMimeType += _L8("; level=10");
                        break;
			}
		}
    }
    else if ( iDataFormat == EDataMPEG4 )
    {
        // MPEG-4 Visual
        codecType = EMpeg4;
        shortMimeType = _L8("video/mp4v-es");   // Set short mime
        newMimeType = shortMimeType;

		switch( width )
		{
			case KTRSubQCIFWidth:
			case KTRQCIFWidth:
			{
				// Set profile-level-id=0
				codecLevel = KTRMPEG4CodecLevel0;
                maxBitRate = KTRMaxBitRateMPEG4Level0;
                newMimeType += _L8("; profile-level-id=8");
                break;
			}

			case KTRQVGAWidth:
			case KTRCIFWidth:
			{
                    // Set profile-level-id=3
                    maxBitRate = KTRMaxBitRateMPEG4Level3;
                    codecLevel = KTRMPEG4CodecLevel3;
                    newMimeType += _L8("; profile-level-id=3");
                    break;
			}
                    
			case KTRVGAWidth:
			{
                    // Set profile-level-id=4 (4a)
                    maxBitRate = KTRMaxBitRateMPEG4Level4a;
                    codecLevel = KTRMPEG4CodecLevel4a;
                    newMimeType += _L8("; profile-level-id=4");
                    break;
			}

			default:
			{
                    // Set profile-level-id=0
                    maxBitRate = KTRMaxBitRateMPEG4Level0;
                    codecLevel = KTRMPEG4CodecLevel0;
                    newMimeType += _L8("; profile-level-id=8");
                    break;
			}
		}
    }
    else if (iDataFormat == EDataAVC)
    {
    	// @@ YHK this is a hack for AVC fix it later....
    	// @@ YHK Imp *****************
    	
        codecType = EH264;
        shortMimeType = _L8("video/H264");   // Set short mime
        newMimeType = shortMimeType;

        codecLevel = KTRMPEG4CodecLevel0;
        maxBitRate = KTRMaxBitRateMPEG4Level0;
        newMimeType += _L8("; profile-level-id=428014");
    	
    }
    else
    {
        PRINT((_L("CTRTranscoderImp::ParseMimeL(), there is curently no support for this type")))
        User::Leave(KErrNotSupported);
    }

	// Mime type was set for Input format
	iCodecLevel = codecLevel;
	iCodec = codecType;
	iMaxBitRate = maxBitRate;
        
	iMimeType = newMimeType;
	iShortMimeType = shortMimeType;

}

// ---------------------------------------------------------
// CTNEProcessorImpl::GetFrameL
// Gets the transcode factor from the current clip
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CTNEProcessorImpl::GetFrameL()
{
	// Read the video frame into buffer
    TInt error = ReadVideoFrame();
	
    // seek to and decode first frame
    if (!ReadAndUpdateFrame())
        User::Leave(KErrCorrupt);
           
    return KErrNone;    
}



// ---------------------------------------------------------
// CTNEProcessorImpl::ReadVideoFrame
// Gets the transcode factor from the current clip
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CTNEProcessorImpl::ReadVideoFrame()
{
    TUint frameLen;
	TFrameType frameType = EFrameTypeVideo;
	TBool frameAvailable = 0; 
    TPtr8 readDes(0,0);        
	TUint32 numReadFrames = 0;
	TUint32 timeStamp;

	// Get the next frame information
	TInt error = iParser->GetNextFrameInformation(frameType, 
												  frameLen, 
												  frameAvailable);

    if (error !=KErrNone)
      return error;

    VPASSERT(frameAvailable);
                
   	while (iBufferLength < frameLen)
	{
		// New size is 3/2ths of the old size, rounded up to the next
		// full kilobyte
		TUint newSize = (3 * iBufferLength) / 2;
		newSize = (newSize + 1023) & (~1023);
					
        TUint8* tmp = (TUint8*) User::ReAlloc(iDataBuffer, newSize);
		if (!tmp)
		{
			return EFalse;
		}
		
        iDataBuffer = tmp;
		iBufferLength = newSize;
	}

    iDataLength = frameLen;

    
    if (iBufferLength < iDataLength)
    {
    	// need to allocate a bigger buffer
 		User::Panic(_L("CVideoPlayer"), EInvalidInternalState);
    }
                

	// @@ YHK clean up replace *p with the iDataBuffer directly
	// make space for timestamp 
	TUint8 *p = iDataBuffer;
    readDes.Set(p, 0, TInt(frameLen));
                

	// @@ YHK check frameType do we need to send it ??
	// is the parser smart enopugh to get the video frame 
	// type or do we need to skip the audio frame ???
 
	// @@ YHK test this scenario wiht Nth frame 
	// and how do we read the frame from the stream

	// read frame(s) from parser
	error = iParser->ReadFrames(readDes, frameType, 
                                numReadFrames, timeStamp);
   
	if ( error != KErrNone )
		return error;

	VPASSERT( numReadFrames > 0 );   
        
    // @@ YHK We dont need the TS, its not used anywhere
	// put timestamp in the output block before the actual frame data                       
	// Mem::Copy(iDataBuffer, &timeStamp, 4);
	
	// set the frame length back to zero
	//	frameLen = 0;
        
    return KErrNone;
}


// ---------------------------------------------------------
// CTNEProcessorImpl::ReadAndUpdateFrame
// Read the encoded frame and Update information 
// (other items were commented in a header).
// ---------------------------------------------------------
//				
TInt CTNEProcessorImpl::ReadAndUpdateFrame()
{		
	// Determine data format if needed
	if ( iDataFormat == EDataUnknown )
	{
		// OK, we have 4 bytes of data. Check if the buffer starts with a
		// H.263 PSC:
		
		if(iParser->iStreamParameters.iVideoFormat == CParser::EVideoFormatAVCProfileBaseline)
		{
			iDataFormat = EDataAVC;	
		}
		else if ( (iDataBuffer[0] == 0) && (iDataBuffer[1] == 0) &&
				  ((iDataBuffer[2] & 0xfc) == 0x80) )
		{
			// Yes, this is a H.263 stream
			iDataFormat = EDataH263;
		}
		
		// It should be MPEG-4, check if it starts with MPEG 4 Visual
		// Object Sequence start code, Visual Object start code, Video
		// Object start code, or Video Object Layer start code
		else if ( ((iDataBuffer[0] == 0) && (iDataBuffer[1] == 0) && (iDataBuffer[2] == 1) && (iDataBuffer[3] == 0xb0)) ||
			((iDataBuffer[0] == 0) && (iDataBuffer[1] == 0) && (iDataBuffer[2] == 1) && (iDataBuffer[3] == 0xb6)) ||
			((iDataBuffer[0] == 0) && (iDataBuffer[1] == 0) && (iDataBuffer[2] == 1) && (iDataBuffer[3] == 0xb3)) ||
			((iDataBuffer[0] == 0) && (iDataBuffer[1] == 0) && (iDataBuffer[2] == 1) && (iDataBuffer[3] == 0xb5)) ||
			((iDataBuffer[0] == 0) && (iDataBuffer[1] == 0) && (iDataBuffer[2] == 1) && ((iDataBuffer[3] >> 5) == 0)) ||
			((iDataBuffer[0] == 0) && (iDataBuffer[1] == 0) && (iDataBuffer[2] == 1) && ((iDataBuffer[3] >> 4) == 2)))
		{
			iDataFormat = EDataMPEG4;                
		}
		else
		{
            PRINT((_L("CH263Decoder::ReadFrame() - no PSC or MPEG-4 start code in the start of the buffer")));
			return EFalse;
		}
	}
	
	// Determine the start code length
	TUint startCodeLength = 0;
	switch (iDataFormat)
	{
	case EDataH263:
		startCodeLength = KH263StartCodeLength;
		break;
	case EDataMPEG4:
		startCodeLength = KMPEG4StartCodeLength ;
		break;
	case EDataAVC:
		break;

	default:
		User::Panic(_L("CVideoPlayer"), EInvalidInternalState);
	}

	// If the stream has ended, we have no blocks and no data for even a
	// picture start code, we can't get a frame
	if( iDataFormat == EDataH263 )
	{
		if ((iCurrentFrameLength <= startCodeLength) &&	(iDataLength <= startCodeLength) )
			return EFalse;
	}
	else
	{
		if ( (iCurrentFrameLength <= startCodeLength) &&	(iDataLength < startCodeLength) )
			return EFalse;
	}
	
	// When reading H.263, the buffer always starts with the PSC of the
	// current frame
	if (iDataFormat == EDataH263)
	{

		// There should be one PSC at the buffer start, and no other PSCs up to
		// iDataLength
		if ( (iDataLength >= KH263StartCodeLength) && 
            ((iDataBuffer[0] != 0) || (iDataBuffer[1] != 0) || ((iDataBuffer[2] & 0xfc) != 0x80)) )
            {
    		PRINT((_L("CH263Decoder::ReadFrame() - no PSC in the start of the buffer")))
    		return EFalse;
            }
		if (iCurrentFrameLength < KH263StartCodeLength )
			iCurrentFrameLength = KH263StartCodeLength;

        TBool gotPSC = EFalse;
		while (!gotPSC)
		{
			// If we don't have a block at the moment, get one and check if it
			// has a new PSC
			
			// If we are at the start of a block, check if it begins with a PSC
			if ( (iDataLength > 2) &&
				( (iDataBuffer[0] == 0) && (iDataBuffer[1] == 0) && ((iDataBuffer[2] & 0xfc) == 0x80) ) )
			{
				gotPSC = ETrue;
				iCurrentFrameLength = iDataLength;					
			}
            else
            {
                PRINT((_L("CH263Decoder::ReadFrame() - no PSC in the start of the buffer")))
                return EFalse;
            }			
		}
		return ETrue;
    }
    else if (iDataFormat == EDataMPEG4)
    {
        // MPEG-4
		        
        // check for VOS end code
        if ( (iDataBuffer[0] == 0 ) && (iDataBuffer[1] == 0 ) && 
            (iDataBuffer[2] == 0x01) && (iDataBuffer[3] == 0xb1) )
            return EFalse;
        
        // insert VOP start code at the end, the decoder needs it
        iDataBuffer[iDataLength++] = 0;
        iDataBuffer[iDataLength++] = 0;
        iDataBuffer[iDataLength++] = 0x01;
        iDataBuffer[iDataLength++] = 0xb6;
        iCurrentFrameLength = iDataLength;
        
        // we have a complete frame
        return ETrue;
    }
    else
    {
    	// Allocate buffer
    	// @@ YHK need to come up with some decent value other than 100
	    TUint8* tmpPtr = (TUint8*) User::AllocL(iDataLength + 100);
	    TInt dLen = 0;
	    TInt skip = 0;
	    TUint32 nalSize = 0;

		if(iFirstFrameOfClip)
		{
			// Set the flag to false.
			iFirstFrameOfClip = EFalse;
			
///////////////////////////////////////////////////////////////////
/*
AVC Decoder Configuration
-------------------------
aligned(8) class AVCDecoderConfigurationRecord {
unsigned int(8) configurationVersion = 1;
unsigned int(8) AVCProfileIndication;
unsigned int(8) profile_compatibility;
unsigned int(8) AVCLevelIndication;
bit(6) reserved = ‘111111’b;
unsigned int(2) lengthSizeMinusOne;
bit(3) reserved = ‘111’b;
unsigned int(5) numOfSequenceParameterSets;
for (i=0; i< numOfSequenceParameterSets; i++) {
  unsigned int(16) sequenceParameterSetLength ;
  bit(8*sequenceParameterSetLength) sequenceParameterSetNALUnit;
}
unsigned int(8) numOfPictureParameterSets;
for (i=0; i< numOfPictureParameterSets; i++) {
   unsigned int(16) pictureParameterSetLength;
   bit(8*pictureParameterSetLength) pictureParameterSetNALUnit;
}
*/
//////////////////////////////////////////////////////////////////////   
		
			// Copy the first 4 bytes for config version, profile indication
			// profile compatibility and AVC level indication
       		Mem::Copy(&tmpPtr[dLen], iDataBuffer, 4);
       		dLen += 4;
			skip += 4;				
			
			// copy 1 byte for bit(6) reserved = ‘111111’b;
            // unsigned int(2) lengthSizeMinusOne;
         	Mem::Copy(&tmpPtr[dLen], iDataBuffer + skip, 1);
         	dLen += 1;
         	
            iNalUnitBytes = (0x3 & iDataBuffer[skip]) + 1;
            skip += 1;
            
            // SSP packets
         	Mem::Copy(&tmpPtr[dLen], iDataBuffer + skip, 1);
         	dLen += 1;
            
            TInt numOfSSP = 0x1F & iDataBuffer[skip];
            skip += 1; 
            
            for (TInt i = 0; i < numOfSSP; i++)
            {
            	TInt sspSize = iDataBuffer[skip]*256 + iDataBuffer[skip+1];
            	skip += 2;
            	
            	tmpPtr[dLen++] = 0;
        		tmpPtr[dLen++] = 0;
        		tmpPtr[dLen++] = 0;
        		tmpPtr[dLen++] = 0x01;
        		
        		Mem::Copy(&tmpPtr[dLen], iDataBuffer + skip, sspSize);

				skip += sspSize;
				dLen += sspSize;
            }
			
			// PSP packets
         	Mem::Copy(&tmpPtr[dLen], iDataBuffer + skip, 1);
         	dLen += 1;

			TInt numOfPSP = iDataBuffer[skip];
			skip += 1;
			
			for (TInt i = 0; i < numOfPSP; i++)
            {
            	TInt pspSize = iDataBuffer[skip]*256 + iDataBuffer[skip+1];
            	skip += 2;
            	
            	tmpPtr[dLen++] = 0;
        		tmpPtr[dLen++] = 0;
        		tmpPtr[dLen++] = 0;
        		tmpPtr[dLen++] = 0x01;
        		
        		Mem::Copy(&tmpPtr[dLen], iDataBuffer + skip, pspSize);

				skip += pspSize;
				dLen += pspSize;
            }
		}
		
		while(skip < iDataLength)
		{
			switch (iNalUnitBytes)
			{
				case 1:
					nalSize = iDataBuffer[skip];
					break;
				case 2:
					nalSize = iDataBuffer[skip]*256 + iDataBuffer[skip+1];
					break;
				case 4:
					nalSize = iDataBuffer[skip]*14336 + iDataBuffer[skip+1]*4096 + iDataBuffer[skip+2]*256 + iDataBuffer[skip+3];
					break;
			}
			
			skip += iNalUnitBytes;
			
	        tmpPtr[dLen++] = 0;
    	    tmpPtr[dLen++] = 0;
        	tmpPtr[dLen++] = 0;
	        tmpPtr[dLen++] = 0x01;
        
    	    Mem::Copy(&tmpPtr[dLen], iDataBuffer + skip, nalSize);
			
			skip += nalSize;
			dLen += nalSize;
		}

        Mem::Copy(iDataBuffer, tmpPtr, dLen);
        iDataLength = dLen;
        iCurrentFrameLength = iDataLength;
        
        if(tmpPtr)
        delete tmpPtr;
        
        // we have a complete frame
        return ETrue;
        	
    }

}
			
// -----------------------------------------------------------------------------
// CTNEProcessorImpl::MNotifyThumbnailReady
// Called by thumbnail generator when thumbnail is ready
// for retrieval
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::MNotifyThumbnailReady(TInt aError)
{
	// Handle if any error is returned
    if (HandleThumbnailError(aError))
        return;    
    TInt bytesPerPixel = 0;           
    TInt error;
    
    if ( !iRgbBuf )
    {
        TSize inputFrameResolution(iParser->iStreamParameters.iVideoWidth,iParser->iStreamParameters.iVideoHeight);
                       
        // rgb specs
        TUint thumbLength = inputFrameResolution.iWidth * inputFrameResolution.iHeight; 
        TUint thumbUVLength = thumbLength>>2;	
        
       // VPASSERT(iYuvBuf);
        // assign yuv pointers 
        TUint8* yBuf = iFrameBuffer;
        TUint8* uBuf = yBuf + thumbLength;
        TUint8* vBuf = uBuf + thumbUVLength;	
        
        // check validity of thumbnail and associated operation
        if(iThumbEnhance)		// for saving to file
        {
            if(iThumbDisplayMode == ENone)					// if no preference
                iThumbDisplayMode = EColor16M;				// 24-bit color image for enhancement
            else if(iThumbDisplayMode != EColor16M)	// invalid combination
            {     
                HandleThumbnailError(KErrNotSupported);
                return;                
            }
        }
        else								// for screen display
        {
            if(iThumbDisplayMode == ENone)					// if no preference
                iThumbDisplayMode = EColor64K;				// 16-bit image
        }
        
        // determine proper bit depth for the bitmap
        if(iThumbDisplayMode == EColor16M)
            bytesPerPixel = 3; // 24-bit rgb takes 3 bytes, stored as bbbbbbbb gggggggg rrrrrrrr
        else if(iThumbDisplayMode == EColor64K || iThumbDisplayMode == EColor4K)
            bytesPerPixel = 2; // 12-bit rgb takes 2 bytes, stored as ggggbbbb xxxxrrrr                            
        else
        {
            HandleThumbnailError(KErrNotSupported);
            return;					// support for 12-, 16- and 24-bit color images only
        }
        
        // create output rgb buffer
        TRAP(error, iRgbBuf = (TUint8*) User::AllocL(thumbLength * bytesPerPixel));
        if (HandleThumbnailError(error))
            return;                                
        
        TInt scanLineLength;
        
        // convert yuv to rgb
        switch (iThumbDisplayMode)
        {
            
        case EColor4K:
            {
                TInt error;
                CYuv2Rgb12* yuvConverter; 
                TRAP(error, yuvConverter = new(ELeave) CYuv2Rgb12);
                if (HandleThumbnailError(error))
                    return;
                scanLineLength = inputFrameResolution.iWidth * bytesPerPixel; 
                VPASSERT(yuvConverter);
                TRAP(error, yuvConverter->ConstructL(inputFrameResolution.iWidth, inputFrameResolution.iHeight, inputFrameResolution.iWidth, inputFrameResolution.iHeight));
                if (HandleThumbnailError(error))
                    return;
                yuvConverter->Convert(yBuf, uBuf, vBuf, inputFrameResolution.iWidth, inputFrameResolution.iHeight, iRgbBuf, scanLineLength);                
                delete yuvConverter;		
                yuvConverter=0;         
            }
            break;
            
        default:
        case EColor64K:
            {
                TInt error;
                CYuv2Rgb16* yuvConverter; 
                TRAP(error, yuvConverter = new(ELeave) CYuv2Rgb16);
                if (HandleThumbnailError(error))
                    return;                
                scanLineLength = inputFrameResolution.iWidth * bytesPerPixel; 
                VPASSERT(yuvConverter);
                TRAP(error, yuvConverter->ConstructL(inputFrameResolution.iWidth, inputFrameResolution.iHeight, inputFrameResolution.iWidth, inputFrameResolution.iHeight);)
                if (HandleThumbnailError(error))
                    return;
                yuvConverter->Convert(yBuf, uBuf, vBuf, inputFrameResolution.iWidth, inputFrameResolution.iHeight, iRgbBuf, scanLineLength);                
                delete yuvConverter;		
                yuvConverter=0; 
            }
            break;
            
        case EColor16M:
            {
                TInt error;
                CYuv2Rgb24* yuvConverter; 
                TRAP(error, yuvConverter = new(ELeave) CYuv2Rgb24);
                if (HandleThumbnailError(error))
                    return;                                
                scanLineLength = inputFrameResolution.iWidth * bytesPerPixel; 
                VPASSERT(yuvConverter);
                TRAP(error, yuvConverter->ConstructL(inputFrameResolution.iWidth, inputFrameResolution.iHeight, inputFrameResolution.iWidth, inputFrameResolution.iHeight))
                if (HandleThumbnailError(error))
                    return;                
                yuvConverter->Convert(yBuf, uBuf, vBuf, inputFrameResolution.iWidth, inputFrameResolution.iHeight, iRgbBuf, scanLineLength);                
                delete yuvConverter;		
                yuvConverter=0; 
            }
            break;
        }        
    }

	if(!iThumbEnhance)
	{
        TSize inputFrameResolution(iParser->iStreamParameters.iVideoWidth,iParser->iStreamParameters.iVideoHeight);
		    
		/* Pre-calculate pixel indices for horizontal scaling. */
		// inputFrameResolution is the resolution of the image read from video clip.
		// iOutputThumbResolution is the final resolution desired by the caller.
		
		TInt xIncrement = inputFrameResolution.iWidth * iOutputThumbResolution.iWidth;
		TInt xBoundary = iOutputThumbResolution.iWidth * iOutputThumbResolution.iWidth;
		
		TInt* xIndices = 0;
		TRAPD(xIndicesErr, xIndices = new (ELeave) TInt[iOutputThumbResolution.iWidth]);
		if (xIndicesErr == KErrNone)
		{
			TInt xDecision = xIncrement / bytesPerPixel; // looks like they changed here - orig was /2
			TInt sourceIndex = 0;
			for (TInt x = 0; x < iOutputThumbResolution.iWidth; x++)
			{
				while (xDecision > xBoundary)
				{
					xDecision -= xBoundary;
					sourceIndex += bytesPerPixel;
				}
				
				xIndices[x] = sourceIndex;
				xDecision += xIncrement;
			}
		}
		else
		{		    
		    HandleThumbnailError(xIndicesErr);
		    return;
		}
		
		/* Initialize bitmap. */
		TRAPD(bitmapErr, iOutBitmap = new (ELeave) CFbsBitmap);
		if ((xIndicesErr == KErrNone) && (bitmapErr == KErrNone))
		{
			bitmapErr = iOutBitmap->Create(iOutputThumbResolution, iThumbDisplayMode/*EColor64K*/);
			if (bitmapErr == KErrNone)
			{
                // Lock the heap to prevent the FBS server from invalidating the address
                iOutBitmap->LockHeap();

				/* Scale to desired iOutputThumbResolution and copy to bitmap. */
				TUint8* dataAddress = (TUint8*)iOutBitmap->DataAddress(); // fix
				
				TInt yIncrement = inputFrameResolution.iHeight * iOutputThumbResolution.iHeight;
				TInt yBoundary = iOutputThumbResolution.iHeight * iOutputThumbResolution.iHeight;
				
				TInt targetIndex = 0;
				TInt sourceRowIndex = 0;
				TInt yDecision = yIncrement / 2; 
				for (TInt y = 0; y < iOutputThumbResolution.iHeight; y++)
				{
					while (yDecision > yBoundary)
					{
						yDecision -= yBoundary;
						sourceRowIndex += (inputFrameResolution.iWidth * bytesPerPixel); 
					}
					yDecision += yIncrement;


					for (TInt x = 0; x < iOutputThumbResolution.iWidth; x++)					
					{
                        for (TInt i = 0; i < bytesPerPixel; ++i)
                        {
                            const TInt firstPixelSourceIndex = sourceRowIndex + xIndices[x] + i;
                            dataAddress[targetIndex] = iRgbBuf[firstPixelSourceIndex];
                            targetIndex++;
                        }
                    }
				}
                iOutBitmap->UnlockHeap();
			} 
			
			else
			{			    
				delete iOutBitmap; iOutBitmap = 0;
				HandleThumbnailError(bitmapErr);
				return;
			}
		}
		else
		{
		    HandleThumbnailError(bitmapErr);
		    delete[] xIndices; xIndices = 0;
		    return;
		}
		
		delete[] xIndices;
		xIndices = 0;
	}
	else		// enhance
	{
		TInt i,j;
		// create input bitmap and buffer
		CFbsBitmap* inBitmap = 0;
		TRAPD(inBitmapErr, inBitmap = new (ELeave) CFbsBitmap);
		if( inBitmapErr == KErrNone )
        {
		    // create bitmaps
		    TSize originalResolution(iParser->iStreamParameters.iVideoWidth, iParser->iStreamParameters.iVideoHeight);
		    inBitmapErr = inBitmap->Create(originalResolution, iThumbDisplayMode/*EColor16M*/); 
		
            if( inBitmapErr == KErrNone )
            {
		        // fill image from rgb buffer to input bitmap buffer 
		        TPtr8 linePtr(0,0); 
        		TInt lineLength = inBitmap->ScanLineLength(originalResolution.iWidth, iThumbDisplayMode); 
		        for(j=0, i=0; j<originalResolution.iHeight; j++, i+=lineLength)
		        {
        			linePtr.Set(iRgbBuf+i, lineLength, lineLength);
		        	inBitmap->SetScanLine((TDes8&)linePtr,j); 
		        }
		
        		// create output bitmap 
		        TRAPD(outBitmapErr, iOutBitmap = new (ELeave) CFbsBitmap);
                if( outBitmapErr == KErrNone )
                {
		            outBitmapErr = iOutBitmap->Create(iOutputThumbResolution, iThumbDisplayMode/*EColor16M*/); // same size as input frame
		
                    if( outBitmapErr == KErrNone )
                    {
		                // post-processing enhancement 
		                TRAP(outBitmapErr, EnhanceThumbnailL((const CFbsBitmap*)inBitmap, (CFbsBitmap*)iOutBitmap));

                    }
                    else
                    {
                        delete inBitmap; inBitmap = 0;   
                        delete iOutBitmap; iOutBitmap = 0;
                        HandleThumbnailError(outBitmapErr);
                        return;
                    }
                }
                else
                {
                     delete inBitmap; inBitmap = 0;
                     HandleThumbnailError(outBitmapErr);
                     return;
                }
            }
            else
            {
                delete inBitmap; inBitmap = 0;
                HandleThumbnailError(inBitmapErr);
                return;                
            }
		
		    // delete input bitmap 
		    delete inBitmap;
		    inBitmap = 0;
        }
        else
        {
            HandleThumbnailError(inBitmapErr);
            return;
        }
	}

    delete iRgbBuf;
    iRgbBuf = 0;
     
	// Handle video decoder deletion. If the decoder has been used,
	// it has to be reset before deletion
	if (iDecoder)
	{
		iDecoder->StopL();
	
		delete iDecoder;
		iDecoder = 0;					
	}    
    
    VPASSERT(iThumbnailRequestStatus);
    User::RequestComplete(iThumbnailRequestStatus, KErrNone);
	iThumbnailRequestStatus = 0;	

	PRINT((_L("CTNEProcessorImpl::MMNotifyThumbnailReady() end")))        
}


// -----------------------------------------------------------------------------
// CTNEProcessorImpl::HandleThumbnailError
// Handle error in thumbnail generation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CTNEProcessorImpl::HandleThumbnailError(TInt aError)
{
    if (aError != KErrNone)
    {
        VPASSERT(iThumbnailRequestStatus);
        User::RequestComplete(iThumbnailRequestStatus, aError);
		iThumbnailRequestStatus = 0;
		return ETrue;		
    }                
    return EFalse;
}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::FetchThumb
// Returns a pointer to completed thumbnail bitmap
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::FetchThumb(CFbsBitmap*& aThumb)
{
    aThumb = iOutBitmap;
    iOutBitmap = 0;
    
    iState = EStateReadyToProcess;
}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::InitializeClipL
// Initializes the processor for processing a clip
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::InitializeClipL(RFile& aFileHandle)
{
	
    PRINT((_L("CTNEProcessorImpl::InitializeClipL() begin")));

    iFirstFrameOfClip = ETrue;
    iFirstFrameFlagSet = EFalse;

    if (!iParser) 
    {            
		iParser = (CMP4Parser*) CMP4Parser::NewL(this, aFileHandle);
    }
    
    iParser->iFirstTimeClipParsing = ETrue;
    iState = EStateIdle;
 
    // open file & parse header
    CTNEProcessorImpl::TFileFormat format = CTNEProcessorImpl::EDataAutoDetect;        
    
    User::LeaveIfError(OpenStream(aFileHandle, format));

	if (iHaveVideo == EFalse)
		User::Leave(KErrNotFound);

    VPASSERT(iState == EStateOpened);
    
    iState = EStatePreparing;

    // open demux & decoder
    User::LeaveIfError(Prepare());

    VPASSERT(iState == EStateReadyToProcess);        

	PRINT((_L("CTNEProcessorImpl::InitializeClipL() end")))
}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::ParseHeaderOnlyL
// Parses the header for a given clip
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::ParseHeaderOnlyL(CParser::TStreamParameters& aStreamParams, RFile& aFileHandle)
{
	
	if (!iParser)
    {        
		// create an instance of the parser 
		iParser = (CMP4Parser*) CMP4Parser::NewL(this, aFileHandle);
	}
	iParser->ParseHeaderL(aStreamParams);
	
    // update output parameters. 
	UpdateStreamParameters(iParser->iStreamParameters, aStreamParams); 
}



// -----------------------------------------------------------------------------
// CTNEProcessorImpl::OpenStream
// Opens a clip for processing
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTNEProcessorImpl::OpenStream(RFile& aFileHandle, TFileFormat aFileFormat)
{
    // We can only streams in idle state
    if (iState != EStateIdle)
        return EInvalidProcessorState;

	TInt error = KErrNone;
	iFileFormat = aFileFormat;

	// set descriptor to read buffer
	TPtr8 readDes(0,0);    
	readDes.Set(iReadBuf, 0, KReadBufInitSize);
	
	// read data from the file 		
	if ( (error = aFileHandle.Read(readDes)) != KErrNone )
		return error;
	
	if ( readDes.Length() < 8 )
		return KErrGeneral;
	
	
    // detect if format is 3GP, 5-8 == "ftyp"
    // This method is not 100 % proof, but good enough
    if ( (iReadBuf[4] == 0x66) && (iReadBuf[5] == 0x74) &&
        (iReadBuf[6] == 0x79) && (iReadBuf[7] == 0x70) )
    {
        iFileFormat = EData3GP;
        iMuxType = EMux3GP;        
    }
    else
        return KErrNotSupported;

    // parse 3GP header
    CMP4Parser *parser = 0;
    if ( !iParser ) 
    {        
        TRAP(error, (parser = CMP4Parser::NewL(this, aFileHandle)) );
        if (error != KErrNone)
            return error;
        iParser = parser;
    }
    else
        parser = (CMP4Parser*)iParser;        

    TRAP(error, ParseHeaderL());

    if (error != KErrNone)
        return error;

    iState = EStateOpened;

    return KErrNone;
}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::CloseStream
// Closes the processed stream from parser
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTNEProcessorImpl::CloseStream()
{
 
	PRINT((_L("CTNEProcessorImpl::CloseStream() begin - iState = %d"), iState))

    if ( (iState != EStateOpened) && (iState != EStateProcessing) ) 
		return EInvalidProcessorState;
	
	TInt error=0;

	// delete parser
	if (iParser)
	{
		TRAP(error,
				{
			delete iParser;
			iParser=0;
				}
		);
		if (error != KErrNone)
			return error;
	}
			
	// We are idle again
	iState = EStateIdle;

	PRINT((_L("CTNEProcessorImpl::CloseStream() end ")))
    
    return KErrNone;
}


// -----------------------------------------------------------------------------
// CTNEProcessorImpl::Prepare
// Prepares the processor for processing, opens demux & decoder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTNEProcessorImpl::Prepare()
{
    // We can only prepare from preparing state
    if (iState != EStatePreparing)
        return EInvalidProcessorState;
	
	// Make sure we now know the stream format
	if (iFileFormat == EDataAutoDetect)
		return EUnsupportedFormat;	
	
    // Check whether the stream has audio, video or both, and whether it is
    // muxed
    switch (iFileFormat)
        {
        case EData3GP:
            // the video and audio flags are set when
            // the header is parsed.
            iIsMuxed = ETrue;
            break;
        default:
            User::Panic(_L("CTNEProcessorImpl"), EInvalidInternalState);
        }

	iState = EStateReadyToProcess;
    
	return KErrNone;
}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::DoCloseVideoL
// Closes & deletes the structures used in processing
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//				
void CTNEProcessorImpl::DoCloseVideoL()
{
	if ((iState == EStateProcessing) || (iState == EStateReadyToProcess)|| 
        (iState == EStatePreparing) )
	{
		PRINT((_L("CTNEProcessorImpl::DoCloseVideoL() - stopping")))
		User::LeaveIfError(Stop());
		iState = EStateOpened;
	}
	
	// If we are buffering or opening at the moment or clip is open then close it 
	if ( (iState == EStateOpened) || (iState == EStateReadyToProcess)) 
	{
		PRINT((_L("CTNEProcessorImpl::DoCloseVideoL() - closing stream")))
		User::LeaveIfError(CloseStream());
		iState = EStateIdle;
	}
}
				
				

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::Stop
// Stops processing & closes modules used in processing
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//				
TInt CTNEProcessorImpl::Stop()
{
	iDecoding = EFalse;
	
	// Check state
	if ( (iState != EStateProcessing) && (iState != EStateReadyToProcess) && (iState != EStatePreparing) )
		return EInvalidProcessorState;
	// We may also get here from the middle of a Prepare() attempt.

	PRINT((_L("CTNEProcessorImpl::Stop() begin")))
		
	// Handle video encoder deletion. If the encoder has been used,
	// it has to be reseted before deleting
	if (iDecoder)
	{
		iDecoder->StopL();
	
		delete iDecoder;
		iDecoder = 0;					
	}
		
	iState = EStateOpened;
	
	PRINT((_L("CTNEProcessorImpl::Stop() end")))

	return KErrNone;
}


// -----------------------------------------------------------------------------
// CTNEProcessorImpl::Close
// Stops processing and closes all submodules except status monitor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TInt CTNEProcessorImpl::Close()
{
 
    // delete all objects except status monitor
    TRAPD(error, DoCloseVideoL());
    if (error != KErrNone)
        return error;

    iState = EStateIdle;

    return KErrNone;

}


// -----------------------------------------------------------------------------
// CTNEProcessorImpl::RunL
// Called by the active scheduler when the video encoder initialization is done
// or an ending black frame has been encoded
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CTNEProcessorImpl::RunL()
{

    PRINT((_L("CancelProcessingL begin, iDecoding %d, iDecoderInitPending %d, iDecodePending %d"), 
				iDecoding, iDecoderInitPending, iDecodePending ));
						
 	// @@ YHK we probably dont need this flag ???
 	// Don't decode if we aren't decoding
    if (!iDecoding)
    {
        if (!IsActive())
        {
            SetActive();
            iStatus = KRequestPending;
        }        
    	PRINT((_L("CH263Decoder::RunL() out from !iDecoding branch")))
        return;		
    }            
    
    if (iDecoderInitPending)
    {        
        iDecoderInitPending = EFalse;        
        if (iStatus != KErrNone)
        {
            MNotifyThumbnailReady(iStatus.Int());                
            return;
        }
        // at this point we have already read a frame, 
        // so now start processing                
        iDecoder->StartL();
                
        // stop if a fatal error has occurred in starting 
        // the transcoder (decoding stopped in MtroFatalError)
        if (!iDecoding)
            return;
        
        DecodeThumb(ETrue);
        return;
    }

    if (iDecodePending)
    {
        iDecodePending = EFalse;
        
        DecodeThumb(EFalse);
        return;
    }    
}


// -----------------------------------------------------------------------------
// CTNEProcessorImpl::RunError
// Called by the AO framework when RunL method has leaved
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TInt CTNEProcessorImpl::RunError(TInt aError)
{
    return aError;
}


// -----------------------------------------------------------------------------
// CTNEProcessorImpl::DoCancel
// Cancels any pending asynchronous requests
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::DoCancel()
{

	PRINT((_L("CTNEProcessorImpl::DoCancel() begin")))

    // Cancel our internal request
    if ( iStatus == KRequestPending )
    {
		PRINT((_L("CTNEProcessorImpl::DoCancel() cancel request")))
        TRequestStatus *status = &iStatus;
        User::RequestComplete(status, KErrCancel);
    }

	PRINT((_L("CTNEProcessorImpl::DoCancel() end")))
}


// -----------------------------------------------------------------------------
// CTNEProcessorImpl::SetHeaderDefaults
// Sets appropriate default values for processing parameters
// in audio-only case
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::SetHeaderDefaults()
{

    // set suitable default values
	iHaveVideo = ETrue;
	iVideoType = EVideoH263Profile0Level10;
		
	iVideoParameters.iWidth = 0;
	iVideoParameters.iHeight = 0;
	iVideoParameters.iIntraFrequency = 0;
	iVideoParameters.iNumScalabilityLayers = 0;
	iVideoParameters.iReferencePicturesNeeded = 0;
    // picture period in nanoseconds
    iVideoParameters.iPicturePeriodNsec = TInt64(33366667);

    iStreamLength = 0;
    iStreamSize = 0;
    iStreamBitrate = 10000;

}


// -----------------------------------------------------------------------------
// CTNEProcessorImpl::ParseHeaderL
// Parses the clip header & sets internal variables accordingly
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::ParseHeaderL()
{	
	VPASSERT(iParser);
	
	if ( iMuxType != EMux3GP )
        User::Leave(EUnsupportedFormat);
	
	CParser::TStreamParameters streamParams;
	
	// parse
	iParser->ParseHeaderL(streamParams);
	
	// copy input stream info into parser
	UpdateStreamParameters(iParser->iStreamParameters, streamParams); 
	
	// copy parameters
	iHaveVideo = streamParams.iHaveVideo;
	iVideoType = (TVideoType)streamParams.iVideoFormat;
	iCanSeek = streamParams.iCanSeek;
	iVideoParameters.iWidth = streamParams.iVideoWidth;
	iVideoParameters.iHeight = streamParams.iVideoHeight;
	iVideoParameters.iIntraFrequency = streamParams.iVideoIntraFrequency;
	iVideoParameters.iNumScalabilityLayers = streamParams.iNumScalabilityLayers;
	iVideoParameters.iReferencePicturesNeeded = streamParams.iReferencePicturesNeeded;
	iVideoParameters.iPicturePeriodNsec = streamParams.iVideoPicturePeriodNsec;
    
	iStreamLength = streamParams.iStreamLength;
	iStreamBitrate = streamParams.iStreamBitrate;
	iStreamSize = streamParams.iStreamSize;		

	// Ensure that the video isn't too large
	if ( (iVideoParameters.iWidth > KTNEMaxVideoWidth) ||
		(iVideoParameters.iHeight > KTNEMaxVideoHeight) )
		User::Leave(EVideoTooLarge);

}

// -----------------------------------------------------------------------------
// CTNEProcessorImpl::UpdateStreamParameters
// Copies stream parameters to destination structure
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::UpdateStreamParameters(CParser::TStreamParameters& aDestParameters, 
																								 CParser::TStreamParameters& aSrcParameters)
{
		aDestParameters.iHaveVideo = aSrcParameters.iHaveVideo; 
		aDestParameters.iVideoFormat = aSrcParameters.iVideoFormat; 
		aDestParameters.iVideoWidth = aSrcParameters.iVideoWidth; 
		aDestParameters.iVideoHeight = aSrcParameters.iVideoHeight; 
		aDestParameters.iVideoPicturePeriodNsec = aSrcParameters.iVideoPicturePeriodNsec; 
		aDestParameters.iVideoIntraFrequency = aSrcParameters.iVideoIntraFrequency; 
		aDestParameters.iStreamLength = aSrcParameters.iStreamLength; 
		aDestParameters.iVideoLength = aSrcParameters.iVideoLength; 
		aDestParameters.iCanSeek = aSrcParameters.iCanSeek; 
		aDestParameters.iStreamSize = aSrcParameters.iStreamSize; 
		aDestParameters.iStreamBitrate = aSrcParameters.iStreamBitrate; 
		aDestParameters.iMaxPacketSize = aSrcParameters.iMaxPacketSize; 
		aDestParameters.iLogicalChannelNumberVideo = aSrcParameters.iLogicalChannelNumberVideo; 
		aDestParameters.iReferencePicturesNeeded = aSrcParameters.iReferencePicturesNeeded; 

		aDestParameters.iFrameRate = aSrcParameters.iFrameRate; 

}


// -----------------------------------------------------------------------------
// CTNEProcessorImpl::EnhanceThumbnailL
// Enhances the visual quality of the frame
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessorImpl::EnhanceThumbnailL(const CFbsBitmap* aInBitmap, 
																						CFbsBitmap* aTargetBitmap) 
{

  // create enhancement object
	if(!iEnhancer)
    iEnhancer = (CDisplayChain*) CDisplayChain::NewL();

	// enhance image
	iEnhancer->ProcessL(aInBitmap, aTargetBitmap); 

	// clear enhancement object
	delete iEnhancer;
	iEnhancer=0;

}

// ---------------------------------------------------------
// CH263Decoder::CheckFrameQuality
// Checks if a frame has "good" or "legible" quality
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CTNEProcessorImpl::CheckFrameQuality(TUint8* aYUVDataPtr)
{
    TInt i;
    TInt minValue = 255;
    TInt maxValue = 0;
    TInt goodFrame = 1;
    TInt runningSum=0;
    TInt averageValue=0;
    TInt pixelSkips = 4;
    TInt numberOfSamples=0;
    TInt minMaxDeltaThreshold = 20; 
    TInt extremeRegionThreshold = 20; 
    TInt ySize = iVideoParameters.iWidth*iVideoParameters.iHeight; 
    
    // gather image statistics
    for(i=0, numberOfSamples=0; i<ySize; i+=pixelSkips, aYUVDataPtr+=pixelSkips, numberOfSamples++)
    {
        runningSum += *aYUVDataPtr;
        if(*aYUVDataPtr > maxValue)
            maxValue = *aYUVDataPtr;
        if(*aYUVDataPtr < minValue)
            minValue = *aYUVDataPtr;
    }
    //VDASSERT(numberOfSamples,10);
    averageValue = runningSum/numberOfSamples;
    
    // make decision based statistics
    if((maxValue - minValue) < minMaxDeltaThreshold)
        goodFrame = 0;
    else 
    {
        if(averageValue < (minValue + extremeRegionThreshold) || 
            averageValue > (maxValue - extremeRegionThreshold))
            goodFrame = 0;
    }
    return goodFrame;
}

// -----------------------------------------------------------------------------
// CVideoProcessorImpl::GetVideoClipFrameRate
// Gets video frame rate of current clip
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal CTNEProcessorImpl::GetVideoClipFrameRate()
    {

    TReal rate;
    iParser->GetVideoFrameRate(rate);

    return rate;

    }

// -----------------------------------------------------------------------------
// CVideoProcessorImpl::GetVideoTimeInMsFromTicks
// Converts a video timestamp from ticks to milliseconds
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt64 CTNEProcessorImpl::GetVideoTimeInMsFromTicks(TInt64 aTimeStampInTicks, TBool /*aCommonTimeScale*/) const
{
	// @@ YHK code modified check this if this is necessary
    TUint timeScale = iParser->iStreamParameters.iVideoTimeScale; 
	VPASSERT(timeScale > 0);
    return TInt64( I64REAL(aTimeStampInTicks) / (TReal)timeScale * 1000 + 0.5 );
}

//=============================================================================


//  End of File

