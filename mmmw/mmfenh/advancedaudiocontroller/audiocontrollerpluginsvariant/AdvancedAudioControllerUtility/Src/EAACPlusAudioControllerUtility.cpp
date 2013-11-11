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
* Description:  Class definition for the AAC utility functions.
*
*/


//  INCLUDES
#include "EAACPlusAudioControllerUtility.h"
#include "DebugMacros.h"
#include <e32svr.h>                             //For Debug
#include <mmfdatabuffer.h>

// CONSTANTS
//const TInt KAdtsNumFramesToScan = 500;
//const TInt KAdifNumFramesToScan = 200;
//const TInt KMaxAacFrameSize     = 1536;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CEAACPlusAudioControllerUtility::CEAACPlusAudioControllerUtility
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CEAACPlusAudioControllerUtility::CEAACPlusAudioControllerUtility()
    {
    }

// -----------------------------------------------------------------------------
// CEAACPlusAudioControllerUtility::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CEAACPlusAudioControllerUtility::ConstructL()
    {
	DP0(_L("CEAACPlusAudioControllerUtility::ConstructL"));

	iAacUtil = CEAacPlusUtil::NewL();
    }

// -----------------------------------------------------------------------------
// CEAACPlusAudioControllerUtility::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CEAACPlusAudioControllerUtility* CEAACPlusAudioControllerUtility::NewL()
	{
	DP0(_L("CEAACPlusAudioControllerUtility::NewL"));

    CEAACPlusAudioControllerUtility* self = new(ELeave) CEAACPlusAudioControllerUtility;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C CEAACPlusAudioControllerUtility::~CEAACPlusAudioControllerUtility()
    {
	delete iAacUtil;
    }

// -----------------------------------------------------------------------------
// CEAACPlusAudioControllerUtility::PrependHeader
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CEAACPlusAudioControllerUtility::PrependHeader()
    { // not needed
	DP0(_L("CEAACPlusAudioControllerUtility::PrependHeader"));

    return (iFormatType == CEAacPlusUtil::EAacPlusFormatAdif) ? ETrue : EFalse;
    }

// -----------------------------------------------------------------------------
// CEAACPlusAudioControllerUtility::ScanHeaderL
// Used to scan the header information
// -----------------------------------------------------------------------------
//
void CEAACPlusAudioControllerUtility::ScanHeaderL(
	CMMFDataBuffer* aDataBuf)
	{
	DP0(_L("CEAACPlusAudioControllerUtility::ScanHeaderL"));

    const TUint8* bufferPtr = aDataBuf->Data().Ptr();
    TInt bufferSize = aDataBuf->Data().Size();
	
	TInt scannedBuffer = 0;
	iBitRateFrozen = EFalse;
	
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


    TInt seekOffset = 0;
    aDataBuf->SetPosition(scannedBuffer);
	seekOffset = SeekSync(aDataBuf, aDataBuf->Data().Size());
	
    iSyncOffset += seekOffset; // offset to this point from content beginning
    scannedBuffer += seekOffset; // offset to this point in this buffer
    aDataBuf->SetPosition(scannedBuffer);
    
	if (seekOffset == aDataBuf->Data().Size())
		{
		User::Leave(KErrNotReady);
		}

//    iSyncOffset += seekOffset; // offset to this point from content beginning
//    scannedBuffer += seekOffset; // offset to this point in this buffer
//    aDataBuf->SetPosition(scannedBuffer);
    
	DP0(_L("CEAACPlusAudioControllerUtility::ScanHeaderL - Calling FrameInfo"));

    CEAacPlusUtil::TEAacPlusFrameInfo frameInfo;
    TInt hdrLen = bufferSize-scannedBuffer;
    
    TInt status = iAacUtil->GetAacFrameInfo(bufferPtr+scannedBuffer, hdrLen, frameInfo);

    iOutFrameSize = frameInfo.iOutFrameSize;
    iSamplesPerFrame = frameInfo.iNoOfSamples;
    iSamplingFrequency = frameInfo.iSamplingFrequency;
    iChannels = frameInfo.iNoOfChannels;
    iChannelsOut = frameInfo.iNoOfChannels; // util should set this to 2 if PS is present
    iProfile = frameInfo.iObjectType-1; // convert from object type back to what decoder uses 
    iOutSamplingFrequency = frameInfo.iOutSamplingFrequency;
    iDownSampledMode = frameInfo.iDownSampledMode;

    iExtObjectType = 0;
	if (frameInfo.iSbr)
		{
		iExtObjectType = 5;
// ??????????????? double-check this - if this is done, dsm should also have to be enabled
//        if (iOutSamplingFrequency > 48000)
//        	iSamplingRate = iOutSamplingFrequency/2;
//        else
//        	iSamplingRate = iOutSamplingFrequency;
		}
//	else    // For Regular AAC Files
//    	{
        iSamplingRate = iOutSamplingFrequency;
//    	}
	if (frameInfo.iPs)
	    {
	    iExtObjectType = 6;
	    }

	status = iAacUtil->GetAacFormatType(bufferPtr+scannedBuffer, bufferSize-scannedBuffer, iFormatType);

	iHeaderOffset = 0;
    iBitRate = 0;
    iDurationUs = 0;
    TInt frameLen = 0;
    
	switch (iFormatType)
		{
		case CEAacPlusUtil::EAacPlusFormatAdif:
            iHeaderOffset = hdrLen;
            iBitRate = GetBitrate(aDataBuf);
			if (iBitRate != 0)
				{ // if we can read bitrate from the header, then use it from now on
				DP1(_L("CEAACPlusAudioControllerUtility::ScanHeaderL freezing bitrate [%d]"), iBitRate);
				iBitRateFrozen = ETrue;
				break;
				}
			scannedBuffer += iHeaderOffset;	

		case CEAacPlusUtil::EAacPlusFormatAdts:
		case CEAacPlusUtil::EAacPlusFormatRaw:
			{
//        	status = iAacUtil->GetAacFrameLength(bufferPtr+scannedBuffer, bufferSize-scannedBuffer, frameLen);
//        	// don't use first frame in averaging below, it may be short
//	        if (frameLen <= 0)
//	            { // don't event try to calculate bitrate
//	            frameLen = 0;
//	            break;
//	            }
	
	        TInt frameBytes = 0;
	        TInt avgFrameLen = 0;
	        TInt frames = 0;
//            scannedBuffer += frameLen;
            for (TInt i=1; i<=100; i++)
                {
                if (scannedBuffer > bufferSize)
                    {
                    break;
                    }
    	        status = iAacUtil->GetAacFrameLength(bufferPtr+scannedBuffer, bufferSize-scannedBuffer, frameLen);
                scannedBuffer += frameLen;
    	        if (frameLen <= 0)
    	            {
    	            break;
    	            }
//    	        if (frameLen > (avgFrameLen << 1))
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
            iBitRate = (avgFrameLen * 8 * iSamplingFrequency) / iSamplesPerFrame;
			break;
			}
		default:
			User::Leave(KErrNotSupported);
			break;
		}
    }

// -----------------------------------------------------------------------------
// CEAACPlusAudioControllerUtility::SeekSync
// -----------------------------------------------------------------------------
//
TInt CEAACPlusAudioControllerUtility::SeekSync(
	CMMFDataBuffer* aBuf, TInt aBufLen)
	{ // should return aBufLen if not found
	DP0(_L("CEAACPlusAudioControllerUtility::SeekSync"));
    const TUint8* buf = aBuf->Data().Ptr() + aBuf->Position();
    TInt bufLen = aBufLen - aBuf->Position();

    const TInt KNotFound = aBufLen;     // sync not found position
    TInt syncPos = KNotFound;
    TInt seekLen = 0;
    
    while (seekLen < bufLen)
        {
        if (IsValidAacFrame(buf + seekLen, bufLen - seekLen))
        	{
        	syncPos = seekLen;
            break;
			}
        seekLen++;
        }
    return syncPos;
    }

// -----------------------------------------------------------------------------
// CEAACPlusAudioControllerUtility::SetCodecConfigData
// -----------------------------------------------------------------------------
// This is really a get, not a set
void CEAACPlusAudioControllerUtility::SetCodecConfigData(
	RArray<TInt>& aCodecConfig)
    {
    aCodecConfig[0]  = iSamplingRate;
    aCodecConfig[1]  = iChannels;
    aCodecConfig[2]  = iProfile;
    aCodecConfig[3]  = iOutFrameSize;
    aCodecConfig[4]  = iSamplesPerFrame;
    aCodecConfig[5]  = iSamplingFrequency;
    aCodecConfig[7]  = iDownSampledMode;
    aCodecConfig[9]  = iOutSamplingFrequency;
    aCodecConfig[10] = iExtObjectType;
    aCodecConfig[12] = iFormatType; // this is only available after scan header
	}

// -----------------------------------------------------------------------------
// CEAACPlusAudioControllerUtility::GetBitrate
// -----------------------------------------------------------------------------
//
TInt CEAACPlusAudioControllerUtility::GetBitrate(CMMFDataBuffer* aBuffer)
    {
    const TUint8* ptr = aBuffer->Data().Ptr();
    TInt offset = 4;            // "ADIF"
    TInt bitrate = 0;
    
    offset += aBuffer->Position();
    
    if (ptr[offset] >> 7 == 1)  // copyright
        offset += 9;

    bitrate = ptr[offset] & 0x7;                    // 4 bits
    bitrate = (bitrate << 8) | ptr[offset+1];       // 8 bits
    bitrate = (bitrate << 8) | ptr[offset+2];       // 8 bits
    bitrate = (bitrate << 3) | ptr[offset+3] >> 5;  // 3 bits
    return bitrate;
    }

// -----------------------------------------------------------------------------
// CEAACPlusAudioControllerUtility::IsValidAacFrame
// -----------------------------------------------------------------------------
//
TBool CEAACPlusAudioControllerUtility::IsValidAacFrame(
	const TUint8* aPtr,	TInt aBufLen)
    {
	DP0(_L("CEAACPlusAudioControllerUtility::IsValidAacFrame"));

	if (aBufLen >= 4)
		{
		if ((aPtr[0] == 'A') && (aPtr[1] == 'D') && (aPtr[2] == 'I') && (aPtr[3] == 'F'))
			return ETrue;
		}

	if (aBufLen >= 6)
		{
		if (((aPtr[0] == 0xFF) && ((aPtr[1] >> 4) == 0xF) &&    // Check syncword FFF
    	     ((aPtr[1] >> 1) & 0x3) == 0))                      // Check layer == 0
	        {
    	    TInt frameLen = 0;
	        frameLen = (aPtr[3] & 0x3) << 11;
    	    frameLen |= aPtr[4] << 3;
        	frameLen |= ((aPtr[5] >> 5) & 0x7);

	        if (!frameLen || (frameLen > (aBufLen-4))) // check this for correctness
    	       return EFalse;

	        // Check next frame
	        if (aBufLen - frameLen >= 2)
	        	{
	    	    const TUint8* nextFramePtr = aPtr + frameLen;
		    	if ((((nextFramePtr[0] == 0xFF) && ((nextFramePtr[1] >> 4) == 0xF)) && // Check syncword FFF
	    	         ((nextFramePtr[1] >> 1) & 0x3) == 0)) // Check layer == 0
	    	        {
    	    	    return ETrue;
	    	        }
	        	}
        	}
		}

    return EFalse;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File

