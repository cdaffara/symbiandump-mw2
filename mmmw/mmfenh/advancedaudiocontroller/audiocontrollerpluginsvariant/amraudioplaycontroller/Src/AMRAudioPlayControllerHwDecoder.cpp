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
#include "AMRAudioPlayControllerDecoder.h"
#include "DebugMacros.h"
//#include <AmrDecHwDeviceTICIM.h>
#include <mmfpaniccodes.h>

// The size of AMR header, header must include bits for determining frame length
const TInt KAmrFrameHeaderSize1 = 1;

// Frame length table (number of frame bytes)
const TInt KAmrFrameLength[16] = {13,14,16,18,20,21,27,32,6,0,0,0,0,0,0,1};

// Maximum number of PCM samples in one AMR frame
const TInt KAmrSamplesPerFrame160 = 160;

const TInt KAmrSampleRate8K = 8000;
const TInt KAmrChannels1 = 1;

const TInt KAmrMaxFrameSize = 32;
const TUint KSizeOfInBuffer   = 2*KAmrMaxFrameSize;

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
CAMRAudioPlayControllerDecoder::CAMRAudioPlayControllerDecoder()
    :   CAdvancedAudioDecoder(CActive::EPriorityStandard)
    {
    DP0(_L("CAMRAudioPlayControllerDecoder::CAMRAudioPlayControllerDecoder - Hardware Accelerated"));
    iMaxFrameSize = KAmrMaxFrameSize;
   	iSizeOfInBuffer = KSizeOfInBuffer;
    }

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAMRAudioPlayControllerDecoder::ConstructL()
    {
    DP1(_L("CAMRAudioPlayControllerDecoder::ConstructL"), this);
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
CAMRAudioPlayControllerDecoder* CAMRAudioPlayControllerDecoder::NewL()
    {
    DP0(_L("CAMRAudioPlayControllerDecoder::NewL"));
    CAMRAudioPlayControllerDecoder* self = new(ELeave) CAMRAudioPlayControllerDecoder();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CAMRAudioPlayControllerDecoder::~CAMRAudioPlayControllerDecoder()
	{
    DP0(_L("CAMRAudioPlayControllerDecoder::~CAMRAudioPlayControllerDecoder"));
    delete iFrameTable;
	}

TInt CAMRAudioPlayControllerDecoder::CodecConfig(RArray<TInt>& /*aConfigData*/)
	{
	TInt status = KErrNone;
	iFrameTable->InitFrameTable(KAmrSampleRate8K, KAmrSamplesPerFrame160);
	return status;
	}

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoder::ResetL
// Resets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
void CAMRAudioPlayControllerDecoder::ResetL()
	{
    DP0(_L ("CAMRAudioPlayControllerDecoder::Reset - Enter"));

	delete[] iOutBuffer;
   	delete[] iInBuffer;
   	
   	iInBuffer = NULL;
   	iOutBuffer = NULL;
   	iInBufferCount = 0;
   	iOutBufferCount = 0;
   	iOutBufferPtr = NULL;
   	iOutFrameLength = KAmrMaxFrameSize; // since data is not decoded
   	iInBuffer = new (ELeave) TUint8[KSizeOfInBuffer];

    iOutBuffer = new (ELeave) TUint8[iOutFrameLength];
    iOutBufferPtr = iOutBuffer;

    iAccLen = 0;

    DP0(_L ("CAMRAudioPlayControllerDecoder::Reset - Exit"));
	}

TCodecProcessResult CAMRAudioPlayControllerDecoder::ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst)
	{
    DP0(_L ("CAMRAudioPlayControllerDecoder::ProcessL"));
	return CAdvancedAudioDecoder::ProcessHwL(aSrc, aDst);
	}

TInt CAMRAudioPlayControllerDecoder::FrameLength(const TUint8* aBuf, TInt aBufLen, TInt& aFrameLength)
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

TInt CAMRAudioPlayControllerDecoder::FrameInfo(
	const TUint8* aBuf,
	TInt aBufLen,
	TAudioFrameInfo& aInfo)
    {
    TInt length = 0;
    aInfo.iBitRate = 0;
    if (aBufLen >= KAmrFrameHeaderSize1)
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
        aInfo.iFrameSamples = KAmrSamplesPerFrame160;
        aInfo.iSamplingRateOut = aInfo.iSamplingRate;
        aInfo.iChannelsOut = aInfo.iChannels;
        aInfo.iFrameSamplesOut= aInfo.iFrameSamples;
    	}
    return length;
    }

// -----------------------------------------------------------------------------
// CAMRAudioControllerUtility::SeekSync
// -----------------------------------------------------------------------------
//
TInt CAMRAudioPlayControllerDecoder::SeekSync(TUint8* aBuf, TInt aBufLen)
    {
    const TInt KMaxFrames = 3;          // number of frames to check
    const TInt KNotFound = aBufLen;     // sync not found position
    TAudioFrameInfo frameInfo;			// frame parameters
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
            if (frameBufLen >= KAmrFrameHeaderSize1  &&  length == 0)
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
// CAMRAudioPlayControllerDecoder::IsHwAccelerated
// Always return true when no soft codec is used.
// -----------------------------------------------------------------------------
//
TBool CAMRAudioPlayControllerDecoder::IsHwAccelerated()
	{
	return ETrue;
	}

TInt CAMRAudioPlayControllerDecoder::CodecCmd(TCodecCmd /*aCmd*/)
	{
	return KErrNotSupported;
	}


// End of file
