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
* Description:  Class definition for the AWB utility functions.
*
*/


//  INCLUDES
#include "AWBAudioControllerUtility.h"
#include <mmfdatabuffer.h>
#include "DebugMacros.h"

// CONSTANTS
_LIT8(KAwbMagicNumber,"#!AMR-WB\n");

// The size of AMR WB header, header must include bits for determining frame length
const TInt KRawAwbFrameHeaderSize = 1;

// Frame length table (number of frame bytes)
const TInt KRawAwbFrameLength[16] = {18, 24, 33, 37, 41, 47, 51, 59, 61,
                                        6, 6, 0, 0, 0, 1, 1};
// Stuffing length table (number of stuffing bits)
const TInt KRawAwbStuffLength[16] = {7, 10, 6, 6, 6, 6, 6, 6, 6,3,3,0,0,0,3,3};

// Maximum sampling rate
const TInt KRawAwbMaxSampleRate = 16000;

// Maximum number of channels
const TInt KRawAwbMaxChannels = 1;

// Maximum number of PCM samples in one AMR WB frame
const TInt KRawAwbMaxSamplesPerFrame = 320;

// constant time for each frame = 0.020 sec
const TInt KAwbMilliSecondsPerFrame  = 20;

const TInt KAwbSampleRate = 16000;
const TInt KAwbBitRate = 7200;
const TInt KAwbChannels = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAWBAudioControllerUtility::CAWBAudioControllerUtility
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAWBAudioControllerUtility::CAWBAudioControllerUtility()
    {
	iSamplingRate = KAwbSampleRate;
	iChannels = KAwbChannels;
	iBitRate = KAwbBitRate;
	iHeaderOffset = KAwbMagicNumber().Size();
	iFrameTimeMs = KAwbMilliSecondsPerFrame;
    iLenMetaData = 0;
    }

// -----------------------------------------------------------------------------
// CAWBAudioControllerUtility::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAWBAudioControllerUtility::ConstructL()
    {
	iPosArr = new(ELeave) CArrayFixSeg<TTimePos>(1024);
	}

// -----------------------------------------------------------------------------
// CAWBAudioControllerUtility::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CAWBAudioControllerUtility* CAWBAudioControllerUtility::NewL()
	{
    DP0(_L("CAWBAudioControllerUtility::NewL"));
    CAWBAudioControllerUtility* self = new(ELeave) CAWBAudioControllerUtility;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C CAWBAudioControllerUtility::~CAWBAudioControllerUtility()
    {
	iPosArr->Reset();
	delete iPosArr;
    }

// -----------------------------------------------------------------------------
// CAWBAudioControllerUtility::SeekSync
// -----------------------------------------------------------------------------
//
TInt CAWBAudioControllerUtility::SeekSync(
	CMMFDataBuffer* aBuf,
	TInt aBufLen)
    {
    const TUint8* buf = aBuf->Data().Ptr();
    const TInt KMaxFrames = 3;          // number of frames to check
    const TInt KNotFound = aBufLen;     // sync not found position
    TAudioFrameInfo frameInfo;            // frame parameters
    TInt i = 0;
    TInt syncPos = KNotFound;
    TInt maxSeek = KMaxFrames;
    const TUint8* endPtr = buf + aBufLen;

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
            if (frameBufLen >= KRawAwbFrameHeaderSize  &&  length == 0)
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
// CAWBAudioControllerUtility::FrameInfo
// -----------------------------------------------------------------------------
//
TInt CAWBAudioControllerUtility::FrameInfo(
	const TUint8* aBuf,
	TInt aBufLen,
	TAudioFrameInfo& aInfo)
    {
    TInt length = 0;
    aInfo.iBitRate = 0;
    if (aBufLen >= KRawAwbFrameHeaderSize)
    	{
        // extract mode information
        const TInt mode = (aBuf[0] & 0x78) >> 3; // 1st byte 0b.MODE...
        // get length
        length = KRawAwbFrameLength[mode];

        // check start stuffing bits
        if((aBuf[0] & 0x83) != 0)
			{
            length = 0; // syntax error
			}

        // check end stuffing bits
        if (length > 0  &&  aBufLen >= length)
			{
            TUint32 stuffBits = aBuf[length-1];
            stuffBits <<= (11 - KRawAwbStuffLength[mode]);
            if ((stuffBits & 0x0000FF) != 0)
				{
                length = 0; // syntax error
				}
			}

        // update frame parameters
        aInfo.iMode = mode;
        aInfo.iBitRate = length * 400;
        aInfo.iSamplingRate = KRawAwbMaxSampleRate;
        aInfo.iChannels = KRawAwbMaxChannels;
        aInfo.iFrameSize = length;
        aInfo.iFrameSamples = KRawAwbMaxSamplesPerFrame;
        aInfo.iSamplingRateOut = aInfo.iSamplingRate;
        aInfo.iChannelsOut = aInfo.iChannels;
        aInfo.iFrameSamplesOut= aInfo.iFrameSamples;
		}
    return length;
    }

// -----------------------------------------------------------------------------
// CAWBAudioControllerUtility::FrameHeaderSize
// -----------------------------------------------------------------------------
//
TInt CAWBAudioControllerUtility::FrameHeaderSize()
    {
    return KRawAwbFrameHeaderSize;
    }


// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::ScanHeaderL
// Used to scan the header information
// -----------------------------------------------------------------------------
//
void CAWBAudioControllerUtility::ScanHeaderL(
	CMMFDataBuffer* aDataBuf)
	{
	DP0(_L("CAMRAudioControllerUtility::ScanHeaderL"));
	iBitRateFrozen = EFalse; // AMR does not have bitrate in container header, so it will be averaged while playing
    const TUint8* bufferPtr = aDataBuf->Data().Ptr();
    TInt bufferSize = aDataBuf->Data().Size();
	
	TInt scannedBuffer = 0;
	
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
	TInt seekOffset = SeekSync(aDataBuf, bufferSize);
	TInt realOffset = seekOffset - scannedBuffer; // we just want offset from where we were

	iHeaderOffset = 0;
	if (realOffset >= 9)
		{
		bufferPtr = aDataBuf->Data().Ptr() + seekOffset - 9;
   		TUint32* w1p = (TUint32*)bufferPtr;
   		TUint32* w2p = (TUint32*)(bufferPtr+4);
   		TUint8*  w3p = (TUint8*)(bufferPtr+8);
    	if (((*w1p & 0x4D412123)==0x4D412123) && 
    		((*w2p & 0x42572D52)==0x42572D52) &&
    		((*w3p & 0x0A)==0x0A))
    		{
			iHeaderOffset = 9;
			realOffset -= 9;
    		}
		else if (seekOffset == aDataBuf->Data().Size())
			{
			User::Leave(KErrNotReady);
			}	
		}

    iSyncOffset += realOffset; // offset to this point from content beginning
    scannedBuffer += realOffset + iHeaderOffset; // offset to this point in this buffer
	bufferPtr = aDataBuf->Data().Ptr();
    aDataBuf->SetPosition(scannedBuffer);
    
	DP0(_L("CAWBAudioControllerUtility::ScanHeaderL - Calling FrameInfo"));

	TAudioFrameInfo frameInfo;
	TInt frameLen = FrameInfo(bufferPtr+scannedBuffer, bufferSize-scannedBuffer, frameInfo);

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
	    
	TInt frameBytes = 0;
	TInt avgFrameLen = 0;
	TInt frames = 0;
	for (TInt i=1; i<=100; i++)
		{
		if (scannedBuffer > bufferSize)
			{
			break;
			}
		frameLen = FrameInfo(bufferPtr+scannedBuffer, bufferSize-scannedBuffer, frameInfo);
		scannedBuffer += frameLen;
		if (frameLen <= 0)
			{
			break;
			}
		if (i == 10) // ignore the first 10 frames
			{
			frameBytes = frameLen;
			frames = 1;    	        	
			}
		else
			{
			frameBytes += frameLen;
			frames++;    	        	
			}	
		avgFrameLen = frameBytes / frames; // frames is always non-zero here
		}
	iBitRate = (avgFrameLen * 8 * iSamplingRate) / iSamplesPerFrame;
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File

