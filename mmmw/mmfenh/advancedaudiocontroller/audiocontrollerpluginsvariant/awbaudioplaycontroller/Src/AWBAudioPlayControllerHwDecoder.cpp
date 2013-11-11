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
#include "AWBAudioPlayControllerDecoder.h"
#include "DebugMacros.h"

//#include <AmrWbDecHwDeviceTICIM.h>
#include <mmfpaniccodes.h>

const TInt KAwbFrameHeaderSize1 = 1;

// Frame length table (number of frame bytes)
const TInt KAwbFrameLength[16] = {18, 24, 33, 37, 41, 47, 51, 59, 61,
                                        6, 6, 0, 0, 0, 1, 1};
// Stuffing length table (number of stuffing bits)
const TInt KAwbStuffLength[16] = {7, 10, 6, 6, 6, 6, 6, 6, 6,3,3,0,0,0,3,3};

// Sampling rate
const TInt KAwbSampleRate16k = 16000;

// Number of channels
const TInt KAwbChannels1 = 1;

// PCM samples per frame
const TInt KAwbSamplesPerFrame320 = 320;

const TInt KAwbMaxFrameSize = 61;
const TUint KSizeOfInBuffer   = 2*KAwbMaxFrameSize;

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

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAWBAudioPlayControllerDecoder::CAWBAudioPlayControllerDecoder()
    :   CAdvancedAudioDecoder(CActive::EPriorityStandard)
    {
    DP0(_L("CAWBAudioPlayControllerDecoder::CAWBAudioPlayControllerDecoder - Hardware Accelerated"));
    iMaxFrameSize = KAwbMaxFrameSize;
   	iSizeOfInBuffer = KSizeOfInBuffer;
    }

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAWBAudioPlayControllerDecoder::ConstructL()
    {
    DP1(_L("CAWBAudioPlayControllerDecoder::ConstructL"), this);
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
CAWBAudioPlayControllerDecoder* CAWBAudioPlayControllerDecoder::NewL()
    {
    DP0(_L("CAWBAudioPlayControllerDecoder::NewL"));
    CAWBAudioPlayControllerDecoder* self = new(ELeave) CAWBAudioPlayControllerDecoder();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CAWBAudioPlayControllerDecoder::~CAWBAudioPlayControllerDecoder()
	{
    DP0(_L("CAWBAudioPlayControllerDecoder::~CAWBAudioPlayControllerDecoder"));
    delete iFrameTable;
	}

TInt CAWBAudioPlayControllerDecoder::CodecConfig(RArray<TInt>& /*aConfigData*/)
	{
	TInt status = KErrNone;
	iFrameTable->InitFrameTable(KAwbSampleRate16k, KAwbSamplesPerFrame320);
	return status;
	}

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoder::ResetL
// Resets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
void CAWBAudioPlayControllerDecoder::ResetL()
	{
    DP0(_L ("CAWBAudioPlayControllerDecoder::Reset - Enter"));

	delete[] iOutBuffer;
   	delete[] iInBuffer;
   	
   	iInBuffer = NULL;
   	iOutBuffer = NULL;
   	iInBufferCount = 0;
   	iOutBufferCount = 0;
   	iOutBufferPtr = NULL;
   	iOutFrameLength = KAwbMaxFrameSize; // since data is not decoded
   	iInBuffer = new (ELeave) TUint8[KSizeOfInBuffer];

    iOutBuffer = new (ELeave) TUint8[iOutFrameLength];
    iOutBufferPtr = iOutBuffer;

    iAccLen = 0;

    DP0(_L ("CAWBAudioPlayControllerDecoder::Reset - Exit"));
	}

TCodecProcessResult CAWBAudioPlayControllerDecoder::ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst)
	{
    DP0(_L ("CAWBAudioPlayControllerDecoder::ProcessL"));
	return CAdvancedAudioDecoder::ProcessHwL(aSrc, aDst);
	}

TInt CAWBAudioPlayControllerDecoder::FrameLength(const TUint8* aBuf, TInt aBufLen, TInt& aFrameLength)
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
// CAWBAudioPlayControllerDecoder::SeekSync
// -----------------------------------------------------------------------------
TInt CAWBAudioPlayControllerDecoder::SeekSync(TUint8* aBuf, TInt aBufLen)
    {
    const TInt KMaxFrames = 3;          // number of frames to check
    const TInt KNotFound = aBufLen;     // sync not found position
    TAudioFrameInfo frameInfo;            // frame parameters
    TInt i = 0;
    TInt syncPos = KNotFound;
    TInt maxSeek = KMaxFrames;
    const TUint8* endPtr = aBuf + aBufLen;

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
        while (framePtr < endPtr  &&  syncPos != KNotFound  &&  seekCount < maxSeek)
			{
            TInt length = FrameInfo(framePtr, frameBufLen, frameInfo);
            if (frameBufLen >= KAwbFrameHeaderSize1  &&  length == 0)
            	{
				syncPos = KNotFound;
				}
            framePtr += length;
            frameBufLen -= length;
            seekCount++;
			}
        aBuf++; aBufLen--; i++;
		}
    return syncPos;
    }

// -----------------------------------------------------------------------------
// CAWBAudioControllerUtility::FrameInfo
// -----------------------------------------------------------------------------
//
TInt CAWBAudioPlayControllerDecoder::FrameInfo(
	const TUint8* aBuf,
	TInt aBufLen,
	TAudioFrameInfo& aInfo)
    {
    TInt length = 0;
    aInfo.iBitRate = 0;
    if (aBufLen >= KAwbFrameHeaderSize1)
    	{
        // extract mode information
        const TInt mode = (aBuf[0] & 0x78) >> 3; // 1st byte 0b.MODE...
        // get length
        length = KAwbFrameLength[mode];

        // check start stuffing bits
        if((aBuf[0] & 0x83) != 0)
			{
            length = 0; // syntax error
			}

        // check end stuffing bits
        if (length > 0  &&  aBufLen >= length)
			{
            TUint32 stuffBits = aBuf[length-1];
            stuffBits <<= (11 - KAwbStuffLength[mode]);
            if ((stuffBits & 0x0000FF) != 0)
				{
                length = 0; // syntax error
				}
			}

        // update frame parameters
        aInfo.iMode = mode;
        aInfo.iBitRate = length * 400;
        aInfo.iSamplingRate = KAwbSampleRate16k;
        aInfo.iChannels = KAwbChannels1;
        aInfo.iFrameSize = length;
        aInfo.iFrameSamples = KAwbSamplesPerFrame320;
        aInfo.iSamplingRateOut = aInfo.iSamplingRate;
        aInfo.iChannelsOut = aInfo.iChannels;
        aInfo.iFrameSamplesOut= aInfo.iFrameSamples;
		}
    return length;
    }

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoder::IsHwAccelerated
// Always return true when no soft codec is used.
// -----------------------------------------------------------------------------
//
TBool CAWBAudioPlayControllerDecoder::IsHwAccelerated()
	{
	return ETrue;
	}

TInt CAWBAudioPlayControllerDecoder::CodecCmd(TCodecCmd /*aCmd*/)
	{
	return KErrNotSupported;
	}


// End of file
