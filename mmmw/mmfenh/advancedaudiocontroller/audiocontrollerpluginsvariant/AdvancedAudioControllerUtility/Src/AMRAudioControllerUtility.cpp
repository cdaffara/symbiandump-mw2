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
* Description:  Class definition for the AMR utility functions.
*
*/


//  INCLUDES
#include "AMRAudioControllerUtility.h"
#include "DebugMacros.h"
#include <mmfdatabuffer.h>

// CONSTANTS
_LIT8(KAmrMagicNumber, "#!AMR\n");

// The size of AMR header, header must include bits for determining frame length
const TInt KAmrFrameHeaderSize = 1;

// Frame length table (number of frame bytes)
const TInt KAmrFrameLength[16] = {13,14,16,18,20,21,27,32,6,0,0,0,0,0,0,1};

// Stuffing length table (number of stuffing bits)
//const TInt KAmrStuffLength[16] = {4, 4, 5, 5, 7, 4, 7, 7, 4,0,0,0,0,0,0,3};

// Maximum number of PCM samples in one AMR frame
const TInt KAmrMaxSamplesPerFrame160 = 160;

// constant time for each frame = 0.020 sec
const TInt KAmrMilliSecondsPerFrame  = 20;

const TInt KAmrSampleRate8K = 8000;
const TInt KAmrBitRate = 12800;
const TInt KAmrChannels1 = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAMRAudioControllerUtility::CAMRAudioControllerUtility
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAMRAudioControllerUtility::CAMRAudioControllerUtility()
    {
	iSamplingRate = KAmrSampleRate8K;
	iBitRate = KAmrBitRate;
	iChannels = KAmrChannels1;
	iHeaderOffset = KAmrMagicNumber().Size(); // only because recorder controller needs this
    iFrameTimeMs = KAmrMilliSecondsPerFrame;
    iLenMetaData = 0;
    iDurationUs = 0;
    iTotalFrameBytes = 0;
    iTotalFrames = 0;
    iFrameDataOffset = 0;    
    }

// -----------------------------------------------------------------------------
// CAMRAudioControllerUtility::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAMRAudioControllerUtility::ConstructL()
    {
	iPosArr = new(ELeave) CArrayFixSeg<TTimePos>(1024);
	}

// -----------------------------------------------------------------------------
// CAMRAudioControllerUtility::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CAMRAudioControllerUtility* CAMRAudioControllerUtility::NewL()
	{
	DP0(_L("CAMRAudioControllerUtility::NewL"));
    CAMRAudioControllerUtility* self = new(ELeave) CAMRAudioControllerUtility;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C CAMRAudioControllerUtility::~CAMRAudioControllerUtility()
    {
	iPosArr->Reset();
	delete iPosArr;
    }

// -----------------------------------------------------------------------------
// CAMRAudioControllerUtility::SeekSync
// -----------------------------------------------------------------------------
//
TInt CAMRAudioControllerUtility::SeekSync(
	CMMFDataBuffer* aBuf,
	TInt aBufLen)
    {
    // aBufLen is still the total buffer size.
    // we'll use the buffer position as the starting point of the seek for a frame
    // the endPtr is the ending point of the seek
    
    const TUint8* buf = aBuf->Data().Ptr() + aBuf->Position();
    const TInt KMaxFrames = 3;          // number of frames to check
    const TInt KNotFound = aBufLen;     // sync not found position
    TAudioFrameInfo frameInfo;			// frame parameters
    TInt i = 0;
    TInt syncPos = KNotFound;
    TInt maxSeek = KMaxFrames;
    const TUint8* endPtr = buf + aBufLen - aBuf->Position();
    
    //Check if start position is same as end position
    //If so, the reset the start position to the beginning of the buffer
    //Bug-Id:ESLM-7ZFAHF
    if (buf == endPtr)
        {
        buf = aBuf->Data().Ptr();
        }

    // Seek a valid frame candidate byte by byte until a valid frame
    // is found or all bytes have been checked.
    while (buf < endPtr  &&  syncPos == KNotFound)
    	{
        TInt seekCount = 0;
        const TUint8* framePtr = buf;
        TInt frameBufLen = aBufLen;
        syncPos = i;
        // Check the validity of this frame candidate and the nearest next
        // frames. If they are not OK, syncPos will be set to KNotFound.
        while (framePtr < endPtr  &&  syncPos != KNotFound  &&  seekCount < maxSeek)
        	{
            TInt length = FrameInfo(framePtr, frameBufLen, frameInfo);
            if (frameBufLen >= KAmrFrameHeaderSize  &&  length == 0)
            	{
				syncPos = KNotFound;
				}
            framePtr += length;
            frameBufLen -= length;
            seekCount++;
        	}
        buf++; aBufLen--; i++;
    	}
    return syncPos;
    }

// -----------------------------------------------------------------------------
// CAMRAudioControllerUtility::FrameInfo
// -----------------------------------------------------------------------------
//
TInt CAMRAudioControllerUtility::FrameInfo(
	const TUint8* aBuf,
	TInt aBufLen,
	TAudioFrameInfo& aInfo)
    {
    TInt length = 0;
    aInfo.iBitRate = 0;
    if (aBufLen >= KAmrFrameHeaderSize)
    	{
        // extract mode information
        const TInt mode = (aBuf[0] & 0x78) >> 3; // 1st byte 0b.MODE...
        // get length
        length = KAmrFrameLength[mode];

        // check start stuffing bits
        if ((aBuf[0] & 0x83) != 0)
	        {
            length = 0; // syntax error
	        }

// SKIP CHECKING FOR STUFFING BITS
/*
        // check end stuffing bits
        if (length > 0  &&  aBufLen >= length)
        	{
            TUint32 stuffBits = aBuf[length-1];
            stuffBits <<= (11 - KAmrStuffLength[mode]);
            if ((stuffBits & 0x0000FF) != 0)
            	{
                length = 0; // syntax error
            	}
        	}
*/
        // update frame parameters
        aInfo.iMode = mode;
        aInfo.iBitRate = length * 400;
        aInfo.iSamplingRate = KAmrSampleRate8K;
        aInfo.iChannels = KAmrChannels1;
        aInfo.iFrameSize = length;
        aInfo.iFrameSamples = KAmrMaxSamplesPerFrame160;
        aInfo.iSamplingRateOut = aInfo.iSamplingRate;
        aInfo.iChannelsOut = aInfo.iChannels;
        aInfo.iFrameSamplesOut= aInfo.iFrameSamples;
    	}
    return length;
    }

// -----------------------------------------------------------------------------
// CAMRAudioControllerUtility::FrameHeaderSize
// -----------------------------------------------------------------------------
//
TInt CAMRAudioControllerUtility::FrameHeaderSize()
    {
    return KAmrFrameHeaderSize;
    }


// -----------------------------------------------------------------------------
// CAMRAudioControllerUtility::ScanHeaderL
// Used to scan the header information
// -----------------------------------------------------------------------------
//
void CAMRAudioControllerUtility::ScanHeaderL(
	CMMFDataBuffer* aDataBuf)
	{
	DP0(_L("CAMRAudioControllerUtility::ScanHeaderL"));
	iBitRateFrozen = EFalse; // AMR does not have bitrate in container header, so it will be averaged while playing
    const TUint8* bufferPtr = aDataBuf->Data().Ptr();
    TInt bufferSize = aDataBuf->Data().Size();
	
	TInt scannedBuffer = 0;
	TAudioFrameInfo frameInfo;
	TInt frameLen;
	
	if (!iHdrIsRead)
        {
        if (iLenMetaData == 0)
            {
            iSyncOffset = 0;
            iLenMetaData = ID3HeaderLength(aDataBuf);
            }
        TInt bufferRemaining = bufferSize;
        
        while (iLenMetaData > 0)
            {
            if (iLenMetaData >= bufferRemaining)
                {
                // this buffer contain no frame
                iSyncOffset += bufferRemaining;
                iLenMetaData -= bufferRemaining;
                User::Leave(KErrNotReady);
                }
            else
                {
                iSyncOffset += iLenMetaData;
                scannedBuffer += iLenMetaData;
                // be sure to check for following id3 tags
                bufferRemaining = bufferSize - scannedBuffer;
                aDataBuf->SetPosition(scannedBuffer);
                iLenMetaData = ID3HeaderLength(aDataBuf);
                }
            }
    
        aDataBuf->SetPosition(scannedBuffer);
        // should change seeksync to start seek from position and not start of buffer (done)
        // this is a little akward because of the implementation of seeksync and its use by the recoder controller.
        TInt seekOffset = SeekSync(aDataBuf, bufferSize); // seeksync now scans from start of buffer + Position()
        TInt realOffset = seekOffset - scannedBuffer; // we just want offset from where we were
    
        iHeaderOffset = 0;
        if (realOffset >= 6)
            {
            bufferPtr = aDataBuf->Data().Ptr() + seekOffset - 6;
            TUint32* w1p = (TUint32*)bufferPtr;
            TUint16* w2p = (TUint16*)(bufferPtr+4);
            if (((*w1p & 0x4D412123)==0x4D412123) && 
                ((*w2p & 0x0A52)==0x0A52))
                {
                iHeaderOffset = 6;
                realOffset -= 6;
                }
            else if (seekOffset == bufferSize)
                {
                User::Leave(KErrNotReady);
                }	
            }
    
        iSyncOffset += realOffset; // offset to this point from content beginning
        scannedBuffer += realOffset + iHeaderOffset; // offset to this point in this buffer
        bufferPtr = aDataBuf->Data().Ptr();
        aDataBuf->SetPosition(scannedBuffer);
        
        DP0(_L("CAMRAudioControllerUtility::ScanHeaderL - Calling FrameInfo"));
    
        //TAudioFrameInfo frameInfo;
        /*TInt*/ frameLen = FrameInfo(bufferPtr+scannedBuffer, bufferSize-scannedBuffer, frameInfo);
    
        if (frameLen == 0)
            {
            User::Leave(KErrNotReady);
            }
            
    //	scannedBuffer += frameLen;
        iBitRate = 0;
        iDurationUs = 0;
        
        iSamplingRate = frameInfo.iSamplingRate;
        iBitRate = frameInfo.iBitRate;
        iSamplesPerFrame = frameInfo.iFrameSamples;
        iChannels = frameInfo.iChannels;
        iChannelsOut = frameInfo.iChannelsOut;
    //	iSamplingRateOut = frameInfo.iSamplingRateOut;
    
        if (iSamplingRate > 0)
            {
            iFrameTimeMs = (iSamplesPerFrame*1000)/iSamplingRate;
            }
        DP0(_L("CAMRAudioControllerUtility::ScanHeaderL - setting iHdrIsRead"));
        iHdrIsRead = ETrue;
        }
	TInt frameBytes = 0;
	TInt frames = 0;

        scannedBuffer += iFrameDataOffset;
	while (scannedBuffer < bufferSize) // buffersize is not a function of buffer.Position()
		{
		frameLen = FrameInfo(bufferPtr+scannedBuffer, bufferSize-scannedBuffer, frameInfo);
		if (frameLen > 0)
			{
		scannedBuffer += frameLen;
	        frameBytes += frameLen;
	        frames++;                   
			}
		else
			{
		    scannedBuffer++; // since frameLen had to be 0 to come here, go ahead to the next byte
	        aDataBuf->SetPosition(scannedBuffer);
		    TInt i = SeekSync(aDataBuf, bufferSize); // still using bufferSize as total size of buffer
		    scannedBuffer += i;
			}
		}
	
	iFrameDataOffset = scannedBuffer - bufferSize;
	DP3(_L("scannedBuffer[%d] frameBytes[%d] iFrameDataOffset[%d]"), scannedBuffer, frameBytes, iFrameDataOffset);
	
	iTotalFrameBytes = iTotalFrameBytes + frameBytes;
	iTotalFrames = iTotalFrames + frames;
	
	// We only leave if its not the last buffer.
	// we will not leave with the last buffer since
	// the AMR Controller has to populate other configuration
	// variables.
	
	if(!aDataBuf->LastBuffer())
		{ // keep reading data for calculation
		User::Leave(KErrCompletion);  // since we have read the header already
		}
    DP0(_L("CAMRAudioControllerUtility::ScanHeaderL - calculating bitrate"));

    iFrameDataOffset = 0; // reset for next time
    TInt64 num = (TInt64)iTotalFrameBytes * TInt64(8 * 1000);
    TInt64 num1 = iFrameTimeMs * iTotalFrames;
    TInt64 ans = 0;
    if (num1 != 0)
		{
        ans = num/num1;
        ans = num > ((ans + 0.5)*num1) ? (ans + 1):ans;
		}
    iBitRate = TInt(ans);
    iBitRateFrozen = ETrue; // bitrate has been calculated using the whole file, we can use this one now
    DP3(_L("CAMRAudioControllerUtility::ScanHeaderL - iTotalFrameBytes[%d] iTotalFrames[%d] iBitRate[%d]"),
            iTotalFrameBytes, iTotalFrames, iBitRate);
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File

