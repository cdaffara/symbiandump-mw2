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
* Description:  Class definition for the MP3 utility functions.
*
*/


//  INCLUDES
#include "MP3AudioControllerUtility.h"
#include "DebugMacros.h"
#include <mmfdatabuffer.h>

// CONSTANTS
const TUint KFlagsOffset            = 4;
const TUint KFramesOffset           = 8;

// Bit rates in bits/sec supported by MPEG2, MPEG1 and MPEG2.5 respectively
const TInt16 cBitRateTable[3][16] =
	{
		{-1,8,16,24,32,40,48,56,64,80,96,112,128,144,160,0},
		{-1,32,40,48,56,64,80,96,112,128,160,192,224,256,320,0},
		{-1,8,16,24,32,40,48,56,64,80,96,112,128,144,160,0},
	};

// Sampling frequencies supported by MPEG2, MPEG1 and MPEG2.5 respectively
const TUint16 cSamplingFrequencyTable[3][4] =
	{
		{22050,24000,16000,0},
		{44100,48000,32000,0},
		{11025,12000,8000,0},
	};

// Maximum MP3 frame size
const TInt KRawMp3MaxFrameSize      = 1441;     // 320kbit/s @ 32kHz = int((144*320000/32000)+1)

// The size of MP3 header, header must include bits for determining frame length
const TInt KRawMp3FrameHeaderSize   = 5;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::CMP3AudioControllerUtility
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMP3AudioControllerUtility::CMP3AudioControllerUtility()
    {
    }

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMP3AudioControllerUtility::ConstructL()
    {
	DP0(_L("CMP3AudioControllerUtility::ConstructL"));
    }

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMP3AudioControllerUtility* CMP3AudioControllerUtility::NewL()
	{
	DP0(_L("CMP3AudioControllerUtility::NewL"));

    CMP3AudioControllerUtility* self = new(ELeave) CMP3AudioControllerUtility;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C CMP3AudioControllerUtility::~CMP3AudioControllerUtility()
    {
	iFramesArray.Close();
    }

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::ScanHeaderL
// Used to scan the header information
// -----------------------------------------------------------------------------
//
void CMP3AudioControllerUtility::ScanHeaderL(
	CMMFDataBuffer* aDataBuf)
	{
	DP0(_L("CMP3AudioControllerUtility::ScanHeaderL"));
	iBitRateFrozen = EFalse;
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
	        // this buffer contains all metadata
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


    TInt seekOffset = 0;
    aDataBuf->SetPosition(scannedBuffer);
	seekOffset = SeekSync(aDataBuf, bufferSize);

    iSyncOffset += seekOffset; // offset to this point from content beginning
    scannedBuffer += seekOffset; // offset to this point in this buffer
    aDataBuf->SetPosition(scannedBuffer);
    
	if (seekOffset == bufferSize)
		{
        User::Leave(KErrNotReady);
		}

	DP0(_L("CMP3AudioControllerUtility::ScanHeaderL - Calling FrameInfo"));

	TAudioFrameInfo frameInfo;
	TInt frameLen = FrameInfo(bufferPtr+scannedBuffer, bufferSize-scannedBuffer, frameInfo);
	
	iHeaderOffset = 0;
    iBitRate = 0;
    iDurationUs = 0;
	
    iSamplingRate = frameInfo.iSamplingRate;
    iBitRate = frameInfo.iBitRate;
    iSamplesPerFrame = frameInfo.iFrameSamples;
    iChannels = frameInfo.iChannels;
	iSamplingRateOut = frameInfo.iSamplingRateOut;
	if (iSamplingRate > 0)
	    {
    	iFrameTimeMs = (iSamplesPerFrame*1000)/iSamplingRate;
	    }
    
    TInt aNumFrames;
	iFormatIsVBR = ParseFirstFrame(aDataBuf, &aNumFrames);
	iNumFrames = aNumFrames;
	
	TInt frameBytes = 0;
	TInt avgFrameLen = 0;
	TInt frames = 0;
	TInt scaling=0;
	TInt round=0;
	// We could have used the bitrate found above for CBR MP3 content and not continue to average
	// the bitrate. However, some users can manipulated the file and append other bitrate content.
	// Since it is not too difficult, we can update the bitrate above with an averaged bitrate.
	// If we want to only use the bitate above for cbr, we would set the bitrate frozen flag and
	// on the condition that it's not iFormatIsVBR and the bitrate is non-zero. 
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
		scaling = frameBytes*10/frames;
		round = scaling%10;
		if(round > 5)
		    {
		     ++avgFrameLen;
		    }
		}
	iBitRate = (avgFrameLen * 8 * iSamplingRate) / iSamplesPerFrame;
	}

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::SeekSync
// -----------------------------------------------------------------------------
//
TInt CMP3AudioControllerUtility::SeekSync(CMMFDataBuffer* aBuf,	TInt aBufLen)
    {
    const TUint bufStart = aBuf->Position();
    const TUint8* buf = aBuf->Data().Ptr() + bufStart;
    const TInt KMaxFrames = 3;          // number of frames to check
    const TInt KNotFound = aBufLen;     // sync not found position
    TAudioFrameInfo frameInfo;           // frame parameters
    TInt i = 0;
    TInt syncPos = KNotFound;
    TInt maxSeek = KMaxFrames;
    const TUint8* endPtr = buf+aBufLen-bufStart;

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
        while (framePtr < endPtr  &&  syncPos != KNotFound  && seekCount < maxSeek)
	        {
            TInt length = FrameInfo(framePtr, frameBufLen, frameInfo);

			if (length == 0)
            	{
                syncPos = KNotFound;
				}
            if ((length > 0) && (frameInfo.iBitRate < 0))
            	{
                maxSeek = 1; // free formatcase
				}
            framePtr += length;
            frameBufLen -= length;
            seekCount++;

			// consider SYNC not found if we reach end of buffer before finding 3 SYNC frames
			if ((framePtr >= endPtr) && (seekCount < maxSeek))
				{
				syncPos = KNotFound;
				buf += (aBufLen-1);      // force an exit from while loop
				}

        	}
        buf++; aBufLen--; i++;
    	}
    return syncPos;
    }

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::FrameInfo
// -----------------------------------------------------------------------------
//
TInt CMP3AudioControllerUtility::FrameInfo(
	const TUint8* aBuf,
	TInt aBufLen,
	TAudioFrameInfo& aInfo)
    {
    TInt length = 0;
    TUint temp;
    TUint lTempVal;

	if (aBufLen >= KRawMp3FrameHeaderSize)
	    {
		// Extract header fields to aInfo and check their bit syntax
		// (including the sync word!). If the syntax is not OK the length
		// is set to zero.

		temp = 0;
		temp = aBuf[0] << 24;
		temp |= (aBuf[1] << 16);
		temp |= (aBuf[2] << 8);
		temp |= aBuf[3];
		if (((temp >> 21) & 0x7FF) != 0x7FF)
			{
			return length;
			}

		lTempVal = (temp >> 19) & 0x03;
		switch (lTempVal)
			{
			case 0:
				aInfo.iId = 2;  // MPEG2.5
				aInfo.iFrameSamples = 576;
				break;
			case 1:
				return length;
			case 2:
				aInfo.iId = 0;  // MPEG 2
				aInfo.iFrameSamples = 576;
				break;
			case 3:
				aInfo.iId = 1;  // MPEG 1
				aInfo.iFrameSamples = 1152;
				break;
			}

		lTempVal = (temp >> 17) & 0x03;
		if (lTempVal != 1)
			{
			return length;
			}

		lTempVal = (temp >> 12) & 0x0F;
		aInfo.iBitRate = cBitRateTable[aInfo.iId][lTempVal]*1000;

		if (aInfo.iBitRate == 0)
			{
			return length;
			}

		lTempVal = (temp >> 10) & 0x03;
		if (lTempVal == 3)
			{
			return length;
			}
		else
			{
			aInfo.iSamplingRate = cSamplingFrequencyTable[aInfo.iId][lTempVal];
			}

		aInfo.iPadding = (temp >> 9) & 0x01;

		lTempVal = (temp >> 6) & 0x03;
		aInfo.iMode = lTempVal;

		if (lTempVal == 3)
			{
			aInfo.iChannels = 1;
			}
		else
            {
            aInfo.iChannels = 2;
            }

        aInfo.iSamplingRateOut = aInfo.iSamplingRate;
//        aInfo.iChannelsOut = 2; /* always set to stereo output */
        aInfo.iChannelsOut = aInfo.iChannels;
        aInfo.iFrameSamplesOut = aInfo.iFrameSamples;

		if (aInfo.iBitRate == -1)
			{
			// For free mode operation
			length = KRawMp3MaxFrameSize;
			}

		if (aInfo.iSamplingRate > 0  &&  aInfo.iBitRate > 0)
			{
			length = (144*aInfo.iBitRate)/aInfo.iSamplingRate;

			if (aInfo.iId != 1)
				{
				length >>= 1; /*for MPEG2 and MPEG2.5 */
				}

			if (aInfo.iPadding)
				{
				length++;
				}
			}

		aInfo.iFrameSize = length;
        iChannelsOut = aInfo.iChannelsOut;
    	}
    return length;
    }

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::SetCodecConfigData
// -----------------------------------------------------------------------------
//
void CMP3AudioControllerUtility::SetCodecConfigData(
	RArray<TInt>& aCodecConfig)
    {
//    aCodecConfig[0] = 0; /*(iChannels == 2) ? 0 : 1*/; // don't have decoder downmix stereo to mono
    // the sw codec always outputs 2 ch for mono and stereo,
    // but for mono, we will downmix the 2ch to mono out
    aCodecConfig[0] = (iChannels == 2) ? 0 : 1; // stereo to mono downmix
    aCodecConfig[5] = iSamplingRate;
    aCodecConfig[8] = iSamplesPerFrame;
    }

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::ParseFirstFrame
// -----------------------------------------------------------------------------
//
TBool CMP3AudioControllerUtility::ParseFirstFrame(
	const CMMFDataBuffer* aBuf,
	TInt* aNumFrames)
    {
	_LIT8(KTagXing, "Xing");
	_LIT8(KTagVBRI, "VBRI");

//	const TInt tocPresent = 0x00000001;
//	const TInt bytesPresent = 0x00000002;
	const TInt framesPresent = 0x00000004;
//	const TInt qualityPresent = 0x00000008;

	TBool firstFrameIsVBR = EFalse;

	TPtrC8 searchBuf;
	TInt numFrames = 0;

	// Set search buffer
//	searchBuf.Set(aBuf->Data());
	const TUint8* ptr = aBuf->Data().Ptr();
	TInt pos = aBuf->Position();
	TInt len = aBuf->Data().Size();
	searchBuf.Set(ptr+pos, len-pos);

    TInt startTagXing = searchBuf.Find (KTagXing);
    TInt startTagVBRI = searchBuf.Find (KTagVBRI);


	if (startTagXing != KErrNotFound)
		{
        TInt flags = searchBuf[startTagXing+KFlagsOffset];
        flags = (flags << 8) | searchBuf[startTagXing+KFlagsOffset+1];
		flags = (flags << 8) | searchBuf[startTagXing+KFlagsOffset+2];
		flags = (flags << 8) | searchBuf[startTagXing+KFlagsOffset+3];
	    firstFrameIsVBR = ETrue;

		if (flags & framesPresent)
			{
            numFrames = searchBuf[startTagXing+KFramesOffset];
            numFrames = (numFrames << 8) | searchBuf[startTagXing+KFramesOffset+1];
		    numFrames = (numFrames << 8) | searchBuf[startTagXing+KFramesOffset+2];
		    numFrames = (numFrames << 8) | searchBuf[startTagXing+KFramesOffset+3];
			}
		}
	else if (startTagVBRI != KErrNotFound)
		{
        numFrames = searchBuf[startTagVBRI+14];
        numFrames = (numFrames << 8) | searchBuf[startTagVBRI+14+1];
	    numFrames = (numFrames << 8) | searchBuf[startTagVBRI+14+2];
	    numFrames = (numFrames << 8) | searchBuf[startTagVBRI+14+3];
	    firstFrameIsVBR = ETrue;
		}
	else
		{
	    firstFrameIsVBR = EFalse;
		}

	*aNumFrames = numFrames;
    return firstFrameIsVBR;
    }

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::FrameHeaderSize
// Returns the Frame Header Size for the MP3 file.
// -----------------------------------------------------------------------------
//
TInt CMP3AudioControllerUtility::FrameHeaderSize()
	{
	return KRawMp3FrameHeaderSize;
	}

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::Duration
// -----------------------------------------------------------------------------
//
TInt64 CMP3AudioControllerUtility::Duration()
    {
    if (iFormatIsVBR && (iNumFrames > 0))
        {
		if (iSamplingRateOut > 0)
		    { // numframes may not be present
    		iDurationUs = ((TInt64)iSamplesPerFrame*(TInt64)iNumFrames*(TInt64)1000000)/iSamplingRateOut;
		    }
        }
    else
        {
        CAdvancedAudioUtility::Duration();
        }    
    return iDurationUs;
    }

// -----------------------------------------------------------------------------
// CMP3AudioControllerUtility::BitRate
// -----------------------------------------------------------------------------
//
TInt CMP3AudioControllerUtility::BitRate()
    {
    if (iFormatIsVBR && (iNumFrames > 0))
        {
        Duration(); // for VBR bitrate depends on duration
       	TInt nonAudioBytes = iSyncOffset+iHeaderOffset;
		TInt audioBytes = iClipSize - nonAudioBytes;
        if (iDurationUs > 0)
            {
            iBitRate = 8*(((TInt64)audioBytes*(TInt64)1000000)/iDurationUs);
            }
        }
    else
        {
        CAdvancedAudioUtility::BitRate();
        }    
    return iBitRate;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File

