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
* Description:   TNE Parser
*
*/



// INCLUDE FILES
#include    "mp4parser.h"
#include    "TNEProcessorImpl.h"
#include    "TNEVideosettings.h"

// ASSERTIONS
#define MPASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CTNEProcessorImpl"), EInternalAssertionFailure))

#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x;
#else
#define PRINT(x)
#endif

// LOCAL CONSTANTS AND MACROS
#ifdef _DEBUG
const TInt KErrorCode = CParser::EParserFailure;
#else
const TInt KErrorCode = KErrGeneral;
#endif

const TUint KNumFramesInOneRun = 10;

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//

CMP4Parser::CMP4Parser()
{       
	iMP4Handle = 0;
	iVideoType = 0;
	iBytesRead = 0;
	iFirstRead = ETrue;		// added for Mp4
	iFirstFrameInfo = ETrue;  // added for Mp4
	iOutputNumberOfFrames = 0; 
    iStreamSource = ESourceFile;
	iFrameNumber=0;
	iFirstTimeClipParsing=ETrue;
    iStartFrameIndex=0;		
}

// Two-phased constructor.

CMP4Parser* CMP4Parser::NewL(CTNEProcessorImpl* aProcessor, RFile& aFileHandle)
{
	CMP4Parser *self = new (ELeave) CMP4Parser;
	CleanupStack::PushL(self);    
	
	self->ConstructL(aProcessor, aFileHandle);

	CleanupStack::Pop();  // self
	
	return self;
}

// Symbian OS default constructor can leave.

void CMP4Parser::ConstructL(CTNEProcessorImpl* aProcessor, RFile& aFileHandle)
{
	MP4Err error;
	iProcessor = aProcessor; 

	// open MP4 library
    if ( iStreamSource == ESourceFile )
		{
			iFileHandle.Duplicate(aFileHandle);
			error = MP4ParseOpenFileHandle(&iMP4Handle, &iFileHandle);
		}
    else
		{
			error = MP4ParseOpen(&iMP4Handle, 0);        
		}
		
    if (error != MP4_OK)
		{
            if (error == MP4_OUT_OF_MEMORY)
                User::Leave(KErrNoMemory);
            else
                User::Leave(KErrorCode);
		}
}

// Destructor
CMP4Parser::~CMP4Parser()
{
	// Close the Duplicate file handle
	iFileHandle.Close();
	
    if (iMP4Handle)
        MP4ParseClose(iMP4Handle);

    iMP4Handle = 0;
}


// ---------------------------------------------------------
// CMP4Parser::GetNextFrameInformation
// Get type (streaming-case), length and availability of next frame to be fetched
// using MP4 library API functions
// (other items were commented in a header).
// ---------------------------------------------------------
//

TInt CMP4Parser::GetNextFrameInformation(TFrameType& aType, TUint& aLength, TBool& aIsAvailable)
{   
	
	// If aType == EFrameTypeNone, the type of next frame is retrieved 
	// (valid only in streaming case)
	// Otherwise, only the length of next specified type of frame is retrieved
	MPASSERT(iStreamSource != ESourceNone);            
	
	mp4_u32 type = MP4_TYPE_NONE;
	MP4Err error;

	aIsAvailable = 0;    
	if ( iNextFrameType == EFrameTypeNone )
		// if the mp4 library is reading a file, a frame has always been read when 
		// we come here
		// otherwise it might be that a complete frame was not available yet
		// and we come here to ask again        
	{
		if ( aType == EFrameTypeNone ) 
		{
			MPASSERT(iStreamSource == ESourceUser);
			
			// get next frame type
			error = MP4ParseNextFrameType(iMP4Handle, &type);

			if ( error == MP4_NOT_AVAILABLE )
				return KErrNone;                
			else if ( error == MP4_NO_FRAME )                
				return EParserEndOfStream; // no video or audio frames left, stream ended
			else if ( error == MP4_INVALID_INPUT_STREAM )
				return KErrCorrupt;
			else if ( error != MP4_OK )
				return KErrorCode;
            else
            {
                MPASSERT(error == MP4_OK);
            }
			
			switch ( type ) 
			{
			case MP4_TYPE_H263_PROFILE_0:
		    case MP4_TYPE_MPEG4_VIDEO:
		    case MP4_TYPE_AVC_PROFILE_BASELINE:
		    // @@ YHK HARI IMP Do we need to support other AVC profiles ??
          
				MPASSERT( type == iVideoType );
				iNextFrameType = EFrameTypeVideo;         
				break;

			default:
				return KErrNotSupported;
			}
		}
		else 
		{
			// library reads the file            
			//MPASSERT(iStreamSource == ESourceFile);
			type = iVideoType;
			iNextFrameType = aType;
		}
		
		// get length for the frame
		mp4_u32 length = 0, mp4Specific = 0;
		MPASSERT( type != MP4_TYPE_NONE );
		if ( (iFirstFrameInfo) &&
		     ((iVideoType == MP4_TYPE_MPEG4_VIDEO) || (iVideoType == MP4_TYPE_AVC_PROFILE_BASELINE)) ) 		   
		{
			error = MP4ParseReadVideoDecoderSpecificInfo( iMP4Handle, 0, 0, &mp4Specific );
			iFirstFrameInfo = EFalse;
		}
		error = MP4ParseNextFrameSize(iMP4Handle, type, &length);
		MPASSERT( error != MP4_NOT_AVAILABLE );
		
		if ( length == 0 || error == MP4_NO_REQUESTED_FRAME )
		{            
			// file-reading case: all frames of this type have been read
			//   and the caller should try with the other type
			MPASSERT( length == 0 );
			iNextFrameType = EFrameTypeNone;
			aLength = 0;            
			return KErrNone;            
		}
		else if ( error == MP4_INVALID_INPUT_STREAM )
			return KErrCorrupt;
		else if ( error != MP4_OK )
			return KErrorCode;
       else if ( length > iMaxVideoFrameLength )
            {
            PRINT((_L("CMP4Parser::GetNextFrameInformation() too large video frame size %d, return KErrCorrupt"),length));
			return KErrCorrupt;
            }
        else
            iNextFrameLength = TUint(length + mp4Specific);

	}
	MPASSERT(iNextFrameType != EFrameTypeNone);
	MPASSERT(iNextFrameLength != 0);
	
	// check if frame is available
	if ( iStreamSource == ESourceUser )
	{
		error = MP4ParseIsFrameAvailable(iMP4Handle, type);
		if ( error != MP4_OK && error != MP4_NOT_AVAILABLE )
			return KErrorCode;
		aIsAvailable = ( error == MP4_NOT_AVAILABLE ) ? EFalse : ETrue;
	}
	else 
		aIsAvailable = ETrue;
	
	aType = iNextFrameType;
	aLength = iNextFrameLength;	
	
	return KErrNone;
}   

// ---------------------------------------------------------
// CMP4Parser::ReadFrames
// Read the next frame(s) from file / stream
// (other items were commented in a header).
// ---------------------------------------------------------
//

TInt CMP4Parser::ReadFrames(TDes8& aDstBuffer, TFrameType aType, TUint32& aNumRead, 
														TUint32& aTimeStamp)
{

// @@ YHK if the frame typ ei s audio do we need to skip 
// it or can we hard code it to video type the 3GP lib 
// will return the appropriate type of the frame information.


	MP4Err error;
	mp4_u32 returnedSize = 0;    
	mp4_bool keyFrame = 0;
		
	MPASSERT( iNextFrameType != EFrameTypeNone && aType == iNextFrameType ); 
	MPASSERT( iNextFrameLength != 0 );

#ifdef _DEBUG
	mp4_u32 type = MP4_TYPE_NONE; // buffer support
	type = iVideoType; // buffer support
	if (iStreamSource == ESourceUser)
		MPASSERT( MP4ParseIsFrameAvailable(iMP4Handle, type) == MP4_OK );
#endif

	
	if (aType == EFrameTypeVideo) 
	{	
		TUint32 iTimeStampInTicks=0;
		mp4_u32 mp4Specific = 0;

		if ( (iFirstRead) &&
		     ((iVideoType == MP4_TYPE_MPEG4_VIDEO) || (iVideoType == MP4_TYPE_AVC_PROFILE_BASELINE)) ) 		   
		{
			error = MP4ParseReadVideoDecoderSpecificInfo( iMP4Handle, (mp4_u8*)(aDstBuffer.Ptr() + aDstBuffer.Length()), 
				mp4_u32( aDstBuffer.MaxLength() ), &mp4Specific );
			iFirstRead = EFalse;
		}
		
		error = MP4ParseReadVideoFrame(iMP4Handle, (mp4_u8*)(aDstBuffer.Ptr() + aDstBuffer.Length()+ mp4Specific), 
			mp4_u32( aDstBuffer.MaxLength() ), &returnedSize, (mp4_u32*)&aTimeStamp,
			&keyFrame, &iTimeStampInTicks);	

		returnedSize += mp4Specific;
		iFrameNumber++;
		aNumRead = 1;
	}
	else 
	{       
		error = MP4ParseReadAudioFrames(iMP4Handle, (mp4_u8*)(aDstBuffer.Ptr()), 
			mp4_u32(aDstBuffer.MaxLength()), &returnedSize, (mp4_u32*)&aTimeStamp, 
			(mp4_u32*)&aNumRead, NULL);               
	
			
		//PRINT((_L("audio TS:%d, "), aTimeStamp));
	}
	
	MPASSERT(error != MP4_BUFFER_TOO_SMALL);    
	aDstBuffer.SetLength(aDstBuffer.Length() + TInt(returnedSize));
	iBytesRead += returnedSize;
	iNextFrameType = EFrameTypeNone;
	iNextFrameLength = 0;
		
	//PRINT((_L("error=%d, numReturned=%d, returnedSize=%d, bufferSize=%d\n"), 
	//	error, aNumRead, returnedSize, aDstBuffer.MaxLength()));
		
	if (error == MP4_NOT_AVAILABLE)
		return EParserNotEnoughData;    
	else if ( error == MP4_INVALID_INPUT_STREAM )
		return KErrCorrupt;
	else if ( error != MP4_OK )
		return KErrorCode;
    else
        return KErrNone;
}


// ---------------------------------------------------------
// CMP4Parser::Reset
// Resets the parser to its initial state
// (other items were commented in a header).
// ---------------------------------------------------------
//

TInt CMP4Parser::Reset()
{
	MP4Err error;
	
	if ( iStreamSource == ESourceFile )
	{
		mp4_u32 audioPos, videoPos;
		
		// seek to very beginning
		error = MP4ParseSeek(iMP4Handle, 0, &audioPos, &videoPos, EFalse);
		if ( error != MP4_OK )
			return KErrorCode;
		
		MPASSERT( videoPos == 0 && audioPos == 0 );		
		
	}
	else 
	{
		// close & open library to make sure old data is flushed
		error = MP4ParseClose(iMP4Handle);
		
		if ( error != MP4_OK )
			return KErrorCode;
		error = MP4ParseOpen(&iMP4Handle, 0);
		if ( error != MP4_OK )
			return KErrorCode;                    
	}
	
	iBytesRead = 0;
	iNextFrameType = EFrameTypeNone;
	iNextFrameLength = 0;
	
	return KErrNone;
}


// ---------------------------------------------------------
// CMP4Parser::ParseHeaderL
// Get relevant stream parameters by calling mp4 library functions
// (other items were commented in a header).
// ---------------------------------------------------------
//

TInt CMP4Parser::ParseHeaderL(CParser::TStreamParameters& aStreamParameters)
{

    PRINT((_L("CMP4Parser::ParseHeaderL() begin")));

	MP4Err error;
	mp4_double frameRate = 0;    
	TBool hasVideo = ETrue;
	
	// Reset channel info
	aStreamParameters.iHaveVideo = EFalse;
	aStreamParameters.iFileFormat = EFileFormatUnrecognized;
	aStreamParameters.iVideoFormat = EVideoFormatNone;
	aStreamParameters.iVideoLength = 0;
	aStreamParameters.iStreamLength = 0;
	aStreamParameters.iVideoPicturePeriodNsec = 0;
	aStreamParameters.iCanSeek = EFalse;
	aStreamParameters.iFrameRate=0;
	aStreamParameters.iVideoTimeScale=0; 
	iVideoType = 0;
	iNumberOfFrames=0;
	
	// get video description
	error = MP4ParseRequestVideoDescription(iMP4Handle, (mp4_u32 *)&aStreamParameters.iVideoLength,
		&frameRate, &iVideoType, (mp4_u32 *)&aStreamParameters.iVideoWidth, 
		(mp4_u32 *)&aStreamParameters.iVideoHeight, (mp4_u32 *)&aStreamParameters.iVideoTimeScale);

	if ( error == MP4_NOT_AVAILABLE )
		User::Leave(KErrorCode);
	else if ( error == MP4_INVALID_INPUT_STREAM )
		User::Leave(KErrCorrupt);
	else if ( error == MP4_NO_VIDEO )  
	{
		hasVideo = EFalse;
		aStreamParameters.iVideoWidth = aStreamParameters.iVideoHeight = 0;
	}
	else if ( error != MP4_OK )
		User::Leave(KErrorCode);
    else
    {
        MPASSERT(error == MP4_OK);
    }


	if (aStreamParameters.iVideoLength > 0)
		aStreamParameters.iStreamLength = aStreamParameters.iVideoLength;
	
	aStreamParameters.iFrameRate = frameRate;

	if(hasVideo)
	{
		iNumberOfFrames = GetNumberOfVideoFrames();
		MPASSERT(iNumberOfFrames);
    		
		MPASSERT(frameRate > 0);
		if (frameRate > 0)
			aStreamParameters.iVideoPicturePeriodNsec = TInt64( TReal(1000000000) / TReal(frameRate) );
		else
			aStreamParameters.iVideoPicturePeriodNsec = TInt64(33366667);
		
		if ( iVideoType == MP4_TYPE_H263_PROFILE_0 || iVideoType == MP4_TYPE_H263_PROFILE_3 )
		{
			TVideoClipProperties prop;
			prop.iH263Level = 0;
			MP4ParseGetVideoClipProperties(iMP4Handle, prop);

            iMaxVideoFrameLength = KMaxCodedPictureSizeQCIF;
			if (prop.iH263Level == 45)
                {
				aStreamParameters.iVideoFormat = EVideoFormatH263Profile0Level45;
                }
			else 
                {
				aStreamParameters.iVideoFormat = EVideoFormatH263Profile0Level10;
                }
		}
		else if ( iVideoType == MP4_TYPE_MPEG4_VIDEO )
            {
			aStreamParameters.iVideoFormat = EVideoFormatMPEG4;		
            if ( aStreamParameters.iVideoWidth <= KTNEResolutionQCIF.iWidth )
                {
                iMaxVideoFrameLength = KMaxCodedPictureSizeMPEG4L0BQCIF;//distinction between L0 and L0B not possible here
                }
            else if (aStreamParameters.iVideoWidth <= KTNEResolutionCIF.iWidth )
                {
                iMaxVideoFrameLength = KMaxCodedPictureSizeMPEG4CIF;
                }
            else
                {
                // VGA
                iMaxVideoFrameLength = KMaxCodedPictureSizeVGA;
                }
            }
        else if ( iVideoType == MP4_TYPE_AVC_PROFILE_BASELINE ||
        		  iVideoType == MP4_TYPE_AVC_PROFILE_MAIN ||
        		  iVideoType == MP4_TYPE_AVC_PROFILE_EXTENDED    )
            {
			if (iVideoType == MP4_TYPE_AVC_PROFILE_BASELINE)
				aStreamParameters.iVideoFormat = EVideoFormatAVCProfileBaseline;
			else if (iVideoType == MP4_TYPE_AVC_PROFILE_MAIN)
				aStreamParameters.iVideoFormat = EVideoFormatAVCProfileMain;
			else if (iVideoType == MP4_TYPE_AVC_PROFILE_EXTENDED)
				aStreamParameters.iVideoFormat = EVideoFormatAVCProfileExtended;
			
            if ( aStreamParameters.iVideoWidth <= KTNEResolutionQCIF.iWidth )
                {
                iMaxVideoFrameLength = KMaxCodedPictureSizeMPEG4L0BQCIF;//distinction between L0 and L0B not possible here
                }
            else if (aStreamParameters.iVideoWidth <= KTNEResolutionCIF.iWidth )
                {
                iMaxVideoFrameLength = KMaxCodedPictureSizeMPEG4CIF;
                }
            else
                {
                // VGA
                iMaxVideoFrameLength = KMaxCodedPictureSizeVGA;
                }
            }
	}	


	if ( iVideoType == MP4_TYPE_MPEG4_VIDEO ||
		 iVideoType == MP4_TYPE_MPEG4_VIDEO ||
		 iVideoType == MP4_TYPE_NONE )
		aStreamParameters.iFileFormat = EFileFormatMP4;

	else if (iVideoType != MP4_TYPE_NONE)
		aStreamParameters.iFileFormat = EFileFormat3GP;

	if ( aStreamParameters.iStreamLength == 0 )
		aStreamParameters.iFileFormat = EFileFormatUnrecognized;
	
	if ( aStreamParameters.iVideoFormat != EVideoFormatNone ) 
	{
		aStreamParameters.iHaveVideo = ETrue;
	}
		
	aStreamParameters.iMaxPacketSize = 0;  // N/A
	aStreamParameters.iLogicalChannelNumberVideo = 0;  // N/A
	
	// get stream description
	error = MP4ParseRequestStreamDescription(iMP4Handle, (mp4_u32 *)&aStreamParameters.iStreamSize, 
		(mp4_u32 *)&aStreamParameters.iStreamBitrate);    
	if ( error != MP4_OK )     
		User::Leave(KErrorCode);
	
	aStreamParameters.iReferencePicturesNeeded = 0;     
	aStreamParameters.iNumScalabilityLayers = 0;
	
	// determine if the stream contains INTRA frames so seeking is possible
	// If the stream contains more than one INTRA frame, seeking is 
	// allowed. 
	
	if (hasVideo)
	{
		mp4_u32 position = aStreamParameters.iStreamLength + 1000;
		mp4_u32 audioTime, videoTime;
		
		// Seek past stream duration to find out the position of last INTRA frame
		error = MP4ParseSeek(iMP4Handle, position, &audioTime, &videoTime, ETrue);
		if ( error != MP4_OK )     
			User::Leave(KErrorCode);
		
		if (videoTime != 0) 
		{
			// at least two INTRAs
			aStreamParameters.iCanSeek = ETrue;
		}
		
		// rewind file back to beginning
		error = MP4ParseSeek(iMP4Handle, 0, &audioTime, &videoTime, EFalse);
		if ( error != MP4_OK )     
			User::Leave(KErrorCode);
	}

    PRINT((_L("CMP4Parser::ParseHeaderL() end")));

	return KErrNone; 
}

// ---------------------------------------------------------
// CMP4Parser::SeekOptimalIntraFrame
// Seeks to INTRA frame position before given time
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CMP4Parser::SeekOptimalIntraFrame(TTimeIntervalMicroSeconds aStartTime, TInt aIndex)
{
	MP4Err error = KErrNone; 
	TInt revisedNumberOfFrames = 0;
	mp4_u32 audioTime=0; 
	mp4_u32 videoTime=0; 
	mp4_u32 timeScale=0; 

	// calculate the start time of the cut operation
	TInt64 startTime = aStartTime.Int64() / TInt64(1000);    
	mp4_u32 startPosition = (mp4_u32)( I64INT(startTime) ); 

	// seek to the I-frame preceding the start time 
	error = MP4ParseSeek(iMP4Handle, startPosition, &audioTime, &videoTime, ETrue);
	if (error != MP4_OK)
	{		
		return KErrorCode; 
	}
	if (videoTime != 0) 
	{
        // at least one INTRA frame upto its last P frame will be ignored 
		// re-calculate the number of frames accordingly         

    	MPASSERT(aIndex > 0);            
            
		// search backwards from cut point to intra to find the index of the intra frame            
		TUint timeInMs;
		TInt timeInTicks;
		for (TInt i = aIndex; i >= 0; i--)
		{
			timeInMs = GetVideoFrameStartTime(i, &timeInTicks);
 			if ( timeInMs == videoTime )
			{
                    iStartFrameIndex = i;
                    revisedNumberOfFrames = iNumberOfFrames - iStartFrameIndex;
                    break;
			}
		}
        
		// update movie and clip number of frames         
        iOutputNumberOfFrames -= iStartFrameIndex;
		iNumberOfFrames = revisedNumberOfFrames; 

		// return to beginning of I-frame
		error = MP4ParseSeek(iMP4Handle, startPosition, &audioTime, &videoTime, ETrue);
        if (error != MP4_OK)
        {		
            return KErrorCode; 
        }
		else
		{
			iFrameNumber = iStartFrameIndex;
		}

	}
	return KErrNone;
}

// ---------------------------------------------------------
// CMP4Parser::GetNumberOfVideoFrames
// Gets the number of video frames in clip
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CMP4Parser::GetNumberOfVideoFrames()
{
    mp4_u32 numberOfFrames = 0;
    MP4Err error = 0;    

    error = MP4ParseGetNumberOfVideoFrames(iMP4Handle, &numberOfFrames);

    if (error != MP4_OK)
        return 0;

    return numberOfFrames;
  
}

// ---------------------------------------------------------
// CMP4Parser::GetVideoFrameSize
// Gets the size of video frame at given index
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CMP4Parser::GetVideoFrameSize(TInt aIndex)
{
    mp4_u32 frameSize = 0;
	mp4_u32 mp4Specific = 0;
    MP4Err error = 0;

	if ( aIndex == 0 && iVideoType == MP4_TYPE_MPEG4_VIDEO )
	{
		error = MP4ParseReadVideoDecoderSpecificInfo( iMP4Handle, 0, 0, &mp4Specific );
		
		if ( error != MP4_OK && error != MP4_BUFFER_TOO_SMALL )
			return KErrorCode;
	}

    error = MP4ParseGetVideoFrameSize(iMP4Handle, aIndex, &frameSize);

    if (error != MP4_OK)
        return KErrorCode;

    return frameSize + mp4Specific;
	
}


// @@ YHK need this for getting the optimal frame
// ---------------------------------------------------------
// CMP4Parser::GetVideoFrameStartTime
// Returns frame start time in millisec - optional iTimestamp for start time in ticks
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CMP4Parser::GetVideoFrameStartTime(TInt aIndex, TInt* aTimeStampInTicks)
{

    MP4Err error = 0;
    mp4_u32 timeStampInMs = 0;

    MPASSERT(aTimeStampInTicks);
  
    error = MP4ParseGetVideoFrameStartTime(iMP4Handle, aIndex, (mp4_u32*)aTimeStampInTicks, &timeStampInMs);

    if (error != MP4_OK)
        return KErrorCode;

    return timeStampInMs;
  
}

// ---------------------------------------------------------
// CMP4Parser::GetVideoFrameType
// Gets the type of video frame at given index
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt8 CMP4Parser::GetVideoFrameType(TInt aIndex)
{

    MP4Err error = 0;
    mp4_bool frameType;
    
    error = MP4ParseGetVideoFrameType(iMP4Handle, aIndex, &frameType);

    if (error != MP4_OK)
        return KErrGeneral;
 
	return TInt8(frameType);
}


TInt CMP4Parser::GetVideoFrameRate(TReal& aFrameRate)
{

    mp4_u32 length;
    mp4_double frameRate; 
    mp4_u32 videoType;
    mp4_u32 width;
    mp4_u32 height;
    mp4_u32 timeScale;

    MP4Err error = 0;

    // get video description
	error = MP4ParseRequestVideoDescription(iMP4Handle, &length, &frameRate, 
        &videoType, &width, &height, &timeScale);		

    if ( error != 0 )
        return KErrGeneral;
    
    TReal temp = frameRate * 2.0;
    TInt temp2 = TInt(temp + 0.5);

    aFrameRate = temp2 / 2.0;

    return KErrNone;
}

// End of File
