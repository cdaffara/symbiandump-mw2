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
* Description:  The functions in this module implements the specific behavior
*                for the advanced audio converter class for configuration of
*				 hardware accelerated codec.
*
*/


// INCLUDE FILES
#include "MP3AudioPlayControllerDecoder.h"
#include "DebugMacros.h"
#include <mmfpaniccodes.h>
#include <mmfcodec.h>

const TUint KRawMp3MaxFrameSize  = 1441;
const TUint KSizeOfInBuffer   = 2*KRawMp3MaxFrameSize;
const TInt 	KRawMp3FrameHeaderSize = 5;

class TAudioFrameInfo
    {
	public:
		TInt iMode;            // encoding mode
		TInt iBitRate;         // bitrate (bit/s)
		TInt iSamplingRate;    // sampling frequency (Hz)
		TInt iChannels;        // number of channels
		TInt iFrameSize;       // encoded size (bytes)
		TInt iFrameSamples;    // samples per frame
		TInt iSamplingRateOut; // sampling frequency after conversion (Hz)
		TInt iChannelsOut;     // number of audio channels after conversion (1 or 2)
		TInt iFrameSamplesOut; // decoded size after conversion (samples per frame)
		TInt iPadding;         // padding flag (TRUE or FALSE, TRUE if p slot exists)
		TInt iId;              // id of algorithm (1 MPEG-1, 0 MPEG-2)
    };
    
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

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerDecoder::CMP3AudioPlayControllerDecoder()
    :   CAdvancedAudioDecoder(CActive::EPriorityStandard)
    {
    DP0(_L("CMP3AudioPlayControllerDecoder::CMP3AudioPlayControllerDecoder - Hardware Accelerated"));
    iMaxFrameSize = KRawMp3MaxFrameSize;
   	iSizeOfInBuffer = KSizeOfInBuffer;
    }

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMP3AudioPlayControllerDecoder::ConstructL()
    {
    DP1(_L("CMP3AudioPlayControllerDecoder::ConstructL"), this);

    iFrameTable = CFrameTable::NewL();
    
	RenderEnable();
	UnMarkPlayEnd();
	Enable();
    
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerDecoder* CMP3AudioPlayControllerDecoder::NewL()
    {
    DP0(_L("CMP3AudioPlayControllerDecoder::NewL"));

    CMP3AudioPlayControllerDecoder* self = new(ELeave) CMP3AudioPlayControllerDecoder();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CMP3AudioPlayControllerDecoder::~CMP3AudioPlayControllerDecoder()
	{
    DP0(_L("CMP3AudioPlayControllerDecoder::~CMP3AudioPlayControllerDecoder"));
    delete iFrameTable;
	}

TInt CMP3AudioPlayControllerDecoder::CodecConfig(RArray<TInt>& aConfigData)
	{
	TInt status = KErrNone;
	TInt sampFreq = ((RArray<TInt>&) aConfigData)[5];
	TInt samplesPerFrame = ((RArray<TInt>&) aConfigData)[8];
	iFrameTable->InitFrameTable(sampFreq, samplesPerFrame);
	return status;
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoder::ResetL
// Resets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
void CMP3AudioPlayControllerDecoder::ResetL()
	{
    DP0(_L ("CMP3AudioPlayControllerDecoder::Reset - Enter"));

	delete[] iOutBuffer;
   	delete[] iInBuffer;
   	
   	iInBuffer = NULL;
   	iOutBuffer = NULL;
   	iInBufferCount = 0;
   	iOutBufferCount = 0;
   	iOutBufferPtr = NULL;
   	iOutFrameLength = KRawMp3MaxFrameSize; // since data is not decoded
   	iInBuffer = new (ELeave) TUint8[KSizeOfInBuffer];

    iOutBuffer = new (ELeave) TUint8[iOutFrameLength];
    iOutBufferPtr = iOutBuffer;

    iAccLen = 0;

    DP0(_L ("CMP3AudioPlayControllerDecoder::Reset - Exit"));
	}

TCodecProcessResult CMP3AudioPlayControllerDecoder::ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst)
	{
    DP0(_L ("CMP3AudioPlayControllerDecoder::ProcessL"));
	return CAdvancedAudioDecoder::ProcessHwL(aSrc, aDst);
	}

TInt CMP3AudioPlayControllerDecoder::FrameLength(const TUint8* aBuf, TInt aBufLen, TInt& aFrameLength)
	{
	TInt stat = KErrNone;
	TAudioFrameInfo info;
	TInt len = FrameInfo(aBuf, aBufLen, info);
	if (len > 0)
		{
		aFrameLength = len;
		}
	else
		{
		stat = KErrUnknown;
		}
	return stat;
	}
	
// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoder::FrameInfo
// -----------------------------------------------------------------------------
//
TInt CMP3AudioPlayControllerDecoder::FrameInfo(
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
        aInfo.iChannelsOut = 2; /* always set to stereo output */
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
//        iChannelsOut = aInfo.iChannelsOut;
    	}
    return length;
    }

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoder::SeekSync
// -----------------------------------------------------------------------------
TInt CMP3AudioPlayControllerDecoder::SeekSync(TUint8* aBuf, TInt aBufLen)
    {
    const TInt KMaxFrames = 3;          // number of frames to check
    const TInt KNotFound = aBufLen;     // sync not found position
    TAudioFrameInfo frameInfo;           // frame parameters
    TInt i = 0;
    TInt syncPos = KNotFound;
    TInt maxSeek = KMaxFrames;
    const TUint8* endPtr = aBuf+aBufLen;

    // Seek a valid frame candidate byte by byte until a valid frame
    // is found or all bytes have been checked.
    while (aBuf < endPtr  &&  syncPos == KNotFound)
	    {
        TInt seekCount = 0;
        const TUint8* framePtr = aBuf;
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
				aBuf += (aBufLen-1);      // force an exit from while loop
				}
        	}
        aBuf++; aBufLen--; i++;
    	}
    return syncPos;
    }


// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoder::IsHwAccelerated
// Always return true when no soft codec is used.
// -----------------------------------------------------------------------------
//
TBool CMP3AudioPlayControllerDecoder::IsHwAccelerated()
	{
	return ETrue;
	}

TInt CMP3AudioPlayControllerDecoder::CodecCmd(TCodecCmd /*aCmd*/)
	{
	return KErrNotSupported;
	}


// End of file
