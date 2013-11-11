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
* Description:  This class provides utility functions such as parsing of header
*				 information for VBR type clips.
*
*/


//  INCLUDES
#include "AdvancedAudioUtility.h"
#include "AdvancedAudioDecoder.h"
#include "DebugMacros.h"
#include <mmffile.h>
#include <mmfcontrollerframeworkbase.h>
#include <stdlib.h> // for abs
#include <mmfclip.h>

// CONSTANTS
const TUint64 KMaxScanDuration = 36000000000U;	// 10 hours
const TUint KSizeOfTagOffset = 6;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::CAdvancedAudioUtility
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CAdvancedAudioUtility::CAdvancedAudioUtility()
	:	iPosArr(NULL),
    	iSamplingRate(0),
    	iBitRate(0),
    	iChannels(0),
    	iSamplesPerFrame(0),
    	iScanDuration(0),
    	iScanByteCount(0),
    	iScanInvByteCount(0),
    	iSavedLength(0),
    	iClipSize(0),
    	iHeaderOffset(0),
    	iSyncOffset(0),
    	iNumFrames(0),
    	iSumBitRate(0),
   	    iLenMetaData(0)

    {
    iObserver = NULL;
    iFrameTimeMs = 0;
    iReferenceBitRate = 0;
    iScannedOnce = EFalse;
    iDecoder = NULL;
    }

// Destructor
EXPORT_C CAdvancedAudioUtility::~CAdvancedAudioUtility()
    {
    }

	
// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::ScanFramesL
// Scan source buffer, frame by frame to update the frame count, byte count and
// duration. Record controller still uses this, but not play controller.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::ScanFramesL(
	CMMFClip* aClip,
	CMMFDataBuffer* aBuf,
	TBool aScanThisBufferOnly,
    TInt /*aSourcePosition*/)
	{
    if (iScanDuration && !aScanThisBufferOnly)
        {
        // Already scanned once!
	    return aBuf->Data().Size();
        }

    iClipSize = aClip->Size();

    if (!aScanThisBufferOnly)
        {
        iHeaderOffset = this->SeekSync(aBuf, aBuf->Data().Size());
        }

    TInt readLen;
    TInt seekLen;
    const TUint8* seekPtr;

    TBool Done = EFalse;
    TInt SeekOffset = this->SeekSync(aBuf, aBuf->Data().Size());

	if (SeekOffset == aBuf->Data().Size())
		return SeekOffset;

    while (!Done)
        {
        readLen = aBuf->Data().Length();
        seekLen = readLen - SeekOffset;
        seekPtr = aBuf->Data().Ptr() + SeekOffset;

        TAudioFrameInfo frameInfo;
        while (seekLen >= this->FrameHeaderSize())
            {
            // Scan frame by frame until free format frame or EOB detected.
            TInt length = this->FrameInfo(seekPtr, seekLen, frameInfo);
            if (length > 0  &&  frameInfo.iBitRate > 0)
                {
                // record time position every time the frame length changes
                RecordTimeStampL(length);
                // update counters
                iScanByteCount += length;
                iScanDuration += (1000000*frameInfo.iFrameSamples) /
                                  frameInfo.iSamplingRateOut;
                iSamplingRate = frameInfo.iSamplingRate;
			    // Ignoring NO_DATA and SID Frames in bitrate calculation
                if (frameInfo.iBitRate != 400 && frameInfo.iBitRate != 2400)
                   	{
                		iSumBitRate += frameInfo.iBitRate;
                		iNumFrames++;
                		iBitRate = iSumBitRate/iNumFrames;
                	}
                iSamplesPerFrame = frameInfo.iFrameSamples;
                iChannels = frameInfo.iChannels;
                seekPtr += length;
                seekLen -= length;
                }
            else if (frameInfo.iBitRate < 0)
                {
                seekLen = readLen = 0;
                }
            else
                {
                iScanInvByteCount++;
                seekPtr++;
                seekLen--;
                iSavedLength = 0;
                }
            }

        // request more data, if eof or max scan duration has not been exceeded
        if ((readLen < this->FrameHeaderSize()) || (iScanDuration > KMaxScanDuration) || aScanThisBufferOnly)
            {
            Done = ETrue;
            }
        else
            {
            // request more data
            SeekOffset = 0;
            aClip->ReadBufferL(aBuf, iHeaderOffset+iScanByteCount+iScanInvByteCount);
            }
        }

	return aBuf->Data().Size();
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::ScanHeaderL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioUtility::ScanHeaderL(
	CMMFDataBuffer* aDataBuf)
	{
    // currently we are not handling how to search past the metadata
	ScanFramesL(aDataBuf, EFalse, 0);
	
	// reset iScanDuration because we didn't scan through the whole file,
    // and iScanDuration would not be the Total Duration of the source		
    iScanDuration = 0;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::ScanFramesL
// Scan source buffer, frame by frame to update the frame count, byte count and
// duration.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::ScanFramesL(
	CMMFDataBuffer* aBuf,
	TBool aScanThisBufferOnly,
    TInt /*aSourcePosition*/)
	{
    if (!aScanThisBufferOnly)
        {
        // Already scanned once!
        if (iScannedOnce)
            return aBuf->Data().Size();

        iHeaderOffset = this->SeekSync(aBuf, aBuf->Data().Size());
        }

    TInt readLen;
    TInt seekLen;
    const TUint8* seekPtr;

    TBool Done = EFalse;
    TInt SeekOffset = this->SeekSync(aBuf, aBuf->Data().Size());

    while (!Done)
        {
        readLen = aBuf->Data().Length();
        seekLen = readLen - SeekOffset;
        seekPtr = aBuf->Data().Ptr() + SeekOffset;
        TAudioFrameInfo frameInfo;
        while (seekLen >= this->FrameHeaderSize())
            {
            // Scan frame by frame until free format frame or EOB detected.
            TInt length = this->FrameInfo(seekPtr, seekLen, frameInfo);
            if (length > 0  &&  frameInfo.iBitRate > 0)
                {
                // record time position every time the frame length changes
                RecordTimeStampL(length);
                // update counters
                iScanByteCount += length;
                iScanDuration += (1000000*frameInfo.iFrameSamples)/frameInfo.iSamplingRateOut;
                iSamplingRate = frameInfo.iSamplingRate;
                
                if (iScanDuration)
                    iScannedOnce = ETrue;
                
                // Ignoring NO_DATA and SID Frames in bitrate calculation
                // this logic here is kinda dangerous, because ScanFramesL can be called
                // multiple time when SetPositionL is supported in Streaming/PDL, hence
                // the avg bitrate can be mess up the the repeat of the same frame
                if (frameInfo.iBitRate != 400 && frameInfo.iBitRate != 2400)
                   	{
            		iSumBitRate += frameInfo.iBitRate;
            		iNumFrames++;
            		iBitRate = iSumBitRate/iNumFrames;
                	}

                iSamplesPerFrame = frameInfo.iFrameSamples;
                iChannels = frameInfo.iChannels;
                seekPtr += length;
                seekLen -= length;
                }
            else if (frameInfo.iBitRate < 0)
                {
                seekLen = readLen = 0;
                }
            else
                {
                iScanInvByteCount++;
                seekPtr++;
                seekLen--;
                iSavedLength = 0;
                }
            }

         // request more data how? , if eof or max scan duration has not been exceeded
        if (aScanThisBufferOnly)
            {
            Done = ETrue;
            }
		}

	return aBuf->Data().Size();
    }

// -----------------------------------------------------------------------------
//  CAdvancedAudioUtility::FindFramePos
//  Finds the byte position of the frame at given time position.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::FindFramePos(
	const TInt64& aTimePos,
	CMMFClip* /*aClip*/)
    {
    TInt ret = KErrNotFound;

    // calculate frame duration
    TInt64 frmTime((iSamplesPerFrame*1000000) / iSamplingRate);

    // find the closest larger time stamp corresponding to time position
    TInt pos = 0;
    TTimePos timeStamp((aTimePos), 0, 0);
    TKeyArrayFix timeKey(_FOFF(TTimePos,iTimePos),ECmpTInt64);
    iPosArr->FindIsq(timeStamp, timeKey, pos);

    // calculate byte position based on the closest smaller time stamp
    if (iPosArr->Count() > 0)
        {
        // get the closest smaller
        if (pos > 0)
        	{
			pos--;
			}
        TTimePos& ts = iPosArr->At(pos);
        // offset from the closest smaller time stamp
        TInt64 dTime = aTimePos - ts.iTimePos;
        // time stamp position plus byte offset within this segment
        ret = ts.iBytePos + ((dTime / frmTime) * ts.iFrameLen);
        }

    // limit check
    if (ret < 0)
    	{
		ret = 0;
		}

    if ((ret > iClipSize) && (iClipSize != 0))
    	{
		ret = iClipSize;
		}

	DP3(_L("CAdvancedAudioUtility::FindFramePos high[%d], low[%d], ret[%d]"), I64HIGH(aTimePos), I64LOW(aTimePos), ret);

    return ret;
    }
// CAdvancedAudioUtility::FrameInfo
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::FrameInfo(
	const TUint8* /*aBuf*/,
	TInt /*aBufLen*/,
	TAudioFrameInfo& /*aInfo*/)
	{
	return 0;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::SampleRate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::SampleRate()
    {
    return iSamplingRate;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::Channels
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::Channels()
    {
    return iChannels;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::ChannelsOut
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::ChannelsOut()
    {
    return iChannelsOut;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::ScanDuration
// -----------------------------------------------------------------------------
// get rid of this
EXPORT_C TInt64 CAdvancedAudioUtility::ScanDuration()
    {
    return iScanDuration;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::Duration
// -----------------------------------------------------------------------------
// default impl
EXPORT_C TInt64 CAdvancedAudioUtility::Duration()
    {
    DP0(_L("CAdvancedAudioUtility::Duration()"));
    
    TInt64 bitRate = BitRate(); // update bitrate
   	TInt64 size = iClipSize;
   	size -= (iSyncOffset+iHeaderOffset);
    if (size >= 0)
       	{
    	if (bitRate > 0)
    		{
    		iDurationUs = size*8000000/bitRate;
    		}
    	}
    DP3(_L("CAdvancedAudioUtility::Duration() iDuration(ms)[%d] bitRate[%d] size[%d]"), iDurationUs/1000, bitRate, iClipSize-iSyncOffset-iHeaderOffset);
    return iDurationUs;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::BitRate
// -----------------------------------------------------------------------------
// default impl - BitRate() would be called by user and updates reference
EXPORT_C TInt CAdvancedAudioUtility::BitRate()
    {
    if (iDecoder && !iBitRateFrozen)
        {
        TInt bitrate = iDecoder->BitRate();
        if (bitrate > 0)
        	{
            iBitRate = bitrate;
        	}
        }
    return iBitRate;
    }


// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::HeaderOffset
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::HeaderOffset()
    {
    return iHeaderOffset;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::SyncOffset
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::SyncOffset()
    {
    return iSyncOffset;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::SetCodecConfigData
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioUtility::SetCodecConfigData(
	RArray<TInt>& /*aCodecConfig*/)
    {
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::SetObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioUtility::SetObserver(MAdvancedAudioUtilityObserver& aObserver)
    {
    iObserver = &aObserver;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::SetDecoder
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioUtility::SetDecoder(CAdvancedAudioDecoder& aDecoder)
    {
    iDecoder = &aDecoder;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::DeReferenceDecoder
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioUtility::DeReferenceDecoder()
    {
    iDecoder = NULL;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::FrameHeaderSize
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::FrameHeaderSize()
	{
	return 0;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::GetDriveNumber
// -----------------------------------------------------------------------------
// is this needed?
EXPORT_C TDriveNumber CAdvancedAudioUtility::GetDriveNumber(
	const TDesC& aDriveName)
    {
    TDriveNumber driveNumber = EDriveC;

    if (!aDriveName.Length())
        {
        return driveNumber;
        }
    else
        {
        switch (aDriveName[0])
            {
            case 'a':
            case 'A':
                driveNumber = EDriveA;
                break;
            case 'b':
            case 'B':
                driveNumber = EDriveB;
                break;
            case 'c':
            case 'C':
                driveNumber = EDriveC;
                break;
            case 'd':
            case 'D':
                driveNumber = EDriveD;
                break;
            case 'e':
            case 'E':
                driveNumber = EDriveE;
                break;
            case 'f':
            case 'F':
                driveNumber = EDriveF;
                break;
            case 'g':
            case 'G':
                driveNumber = EDriveG;
                break;
            case 'h':
            case 'H':
                driveNumber = EDriveH;
                break;
            case 'i':
            case 'I':
                driveNumber = EDriveI;
                break;
            case 'j':
            case 'J':
                driveNumber = EDriveJ;
                break;
            case 'k':
            case 'K':
                driveNumber = EDriveK;
                break;
            case 'l':
            case 'L':
                driveNumber = EDriveL;
                break;
            case 'm':
            case 'M':
                driveNumber = EDriveM;
                break;
            case 'n':
            case 'N':
                driveNumber = EDriveN;
                break;
            case 'o':
            case 'O':
                driveNumber = EDriveO;
                break;
            case 'p':
            case 'P':
                driveNumber = EDriveP;
                break;
            case 'q':
            case 'Q':
                driveNumber = EDriveQ;
                break;
            case 'r':
            case 'R':
                driveNumber = EDriveR;
                break;
            case 's':
            case 'S':
                driveNumber = EDriveS;
                break;
            case 't':
            case 'T':
                driveNumber = EDriveT;
                break;
            case 'u':
            case 'U':
                driveNumber = EDriveU;
                break;
            case 'v':
            case 'V':
                driveNumber = EDriveV;
                break;
            case 'x':
            case 'X':
                driveNumber = EDriveX;
                break;
            case 'y':
            case 'Y':
                driveNumber = EDriveY;
                break;
            case 'z':
            case 'Z':
                driveNumber = EDriveZ;
                break;
            default:
                break;
            }
        }
        return driveNumber;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::SetClipSizeL
// Set/Update the iClipSize varilable
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioUtility::SetClipSizeL(TUint aSize)
    {
    DP1(_L("CAdvancedAudioUtility::SetClipSizeL size[%d]"), aSize);
    if (aSize > (iSyncOffset+iHeaderOffset))
        {
        iClipSize = aSize;
        }
    else
    	{
        User::Leave(KErrArgument);
    	}
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::EnableBitRateChangedEventL
// Enable bitrate change callback
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioUtility::EnableBitRateChangedEventL(TInt aIntervalInMilliSeconds)
    {
    if (!iObserver)
        User::Leave(KErrNotReady);
    else if (aIntervalInMilliSeconds < -1)
        User::Leave(KErrArgument);
    
    if (iDecoder)
        {
        TInt err = KErrNone;

        if (aIntervalInMilliSeconds == -1)
            err = iDecoder->EnableDecodeIntervalEvent(EFalse);
        else
            err = iDecoder->EnableDecodeIntervalEvent(ETrue, aIntervalInMilliSeconds);

        // KErrNone means the decoder would callback on desire interval as it decodes
        // KErrNotSupported implys Utility would need to keep track of the interval
        // otherwise, leave on any other situation
        if (err == KErrNone)
            return;
        else if (err != KErrNotSupported)
            User::LeaveIfError(err);
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::SetSourceReference
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::SetSourceReference(TUint aTimeMs, TUint aPos)
	{
    TInt stat;
    if (iDecoder)
    	{
	    stat = iDecoder->SetSourceReference(aTimeMs, aPos);
    	}
    else
    	{
        stat = KErrNotSupported;
    	}	
    return stat;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::FindFramePosFromTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::FindFramePosFromTime(TUint& aTimeMs, TUint& aPos)
    {
    TInt stat;
    if (iDecoder)
    	{
	    stat = iDecoder->FindFramePosFromTime(aTimeMs, aPos);
    	}
    else
    	{
        stat = KErrNotSupported;
    	}
    return stat;
    }

EXPORT_C TInt CAdvancedAudioUtility::FindFrameTimeFromPos(TUint& aTime, TUint& aPos)
    {
    TInt stat;
    if (iDecoder)
    	{
	    stat = iDecoder->FindFrameTimeFromPos(aTime, aPos);
    	}
    else
    	{
        stat = KErrNotSupported;
    	}	
    return stat;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::LastFramePos
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::LastFramePos(TUint& aPos)
    {
    TInt stat;
    if (iDecoder)
    	{
	    stat = iDecoder->LastFramePos(aPos);
    	}
    else
    	{
        stat = KErrNotSupported;
    	}	
    return stat;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::LastFrameTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::LastFrameTime(TUint& aTimeMs)
    {
    TInt stat;
    if (iDecoder)
    	{
	    stat = iDecoder->LastFrameTime(aTimeMs);
    	}
    else
    	{
        stat = KErrNotSupported;
    	}
    return stat;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::ResetTable
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::ResetTable()
    {
    TInt stat;
    if (iDecoder)
    	{
	    stat = iDecoder->ResetTable();
    	}
    else
    	{
        stat = KErrNotSupported;
    	}	
    return stat;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::SeekToTimeMs
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::SeekToTimeMs(TUint aTimeMs)
    {
    TInt stat;
    if (iDecoder)
    	{
	    stat = iDecoder->SeekToTimeMs(aTimeMs);
    	}
    else
    	{
        stat = KErrNotSupported;
    	}	
    return stat;
    }

EXPORT_C TInt CAdvancedAudioUtility::SetPlayWindowEndTimeMs(TUint aTimeMs)
	{
    TInt stat;
    if (iDecoder)
    	{
	    stat = iDecoder->SetPlayWindowEndTimeMs(aTimeMs);
    	}
    else
    	{
        stat = KErrNotSupported;
    	}	
    return stat;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::RecordTimeStampL
// Record time position every time the frame length changes or when the stamp
// array is empty. Positioning is accurate up to KMaxScanDuration. After that
// the last time stamp is used for estimating the position..
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioUtility::RecordTimeStampL(
	TInt aFrameLength)
    {
    // record time position every time the frame length
    // changes or empty stamp array
    TInt pos = iScanInvByteCount + iScanByteCount;
    TInt count = iPosArr->Count();
    TTimePos lastEntry(0,0,0);
    
    if (count > 0)
    	lastEntry = (*iPosArr)[iPosArr->Count()-1];
    
    DP5(_L("CAdvancedAudioUtility::RecordTimeStampL Count[%d], iScanDuration[%d]millsec, BytePos[%d], aFrameLength[%d], lastEntry.iTimePos[%d]millsec"), count, I64LOW(iScanDuration/1000), pos, aFrameLength, I64LOW(lastEntry.iTimePos/1000));

	if (((pos > lastEntry.iBytePos) && aFrameLength != 0 && aFrameLength != iSavedLength) || (count == 0))
        {
        if (iScanDuration <= KMaxScanDuration)
            {
            DP0(_L("CAdvancedAudioUtility::RecordTimeStampL, AppendL new entry"));

            TTimePos timeStamp(I64LOW(iScanDuration), pos, aFrameLength);

            iPosArr->AppendL(timeStamp);
			iSavedLength = aFrameLength;
            }
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::MetaDataLength
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioUtility::ID3HeaderLength(
	const CMMFDataBuffer* aBuf)
    {
	TInt lenMetaData;
	TUint offset = aBuf->Position();

	_LIT8 (KTagID3, "ID3");
	TPtrC8 searchBuf;

	// Set search buffer
	searchBuf.Set(aBuf->Data());

	const TUint8* ptr = aBuf->Data().Ptr();
	TInt len = aBuf->Data().Length();
	searchBuf.Set(ptr+offset, len-offset);

    TInt startTagPos = searchBuf.Find (KTagID3);
	if (startTagPos == KErrNotFound || startTagPos != 0)
		{
        lenMetaData = 0;
		}
	else
		{
        lenMetaData = searchBuf[KSizeOfTagOffset];
        lenMetaData = ((lenMetaData << 8) | (searchBuf[KSizeOfTagOffset+1] << 1)) >> 1;
		lenMetaData = ((lenMetaData << 8) | (searchBuf[KSizeOfTagOffset+2] << 1)) >> 1;
		lenMetaData = ((lenMetaData << 8) | (searchBuf[KSizeOfTagOffset+3] << 1)) >> 1;
		lenMetaData += 10;
		}

    return lenMetaData;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::CreateBufferOfSizeLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMMFDataBuffer* CAdvancedAudioUtility::CreateBufferOfSizeLC(
	TUint aSize)
    {
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL(aSize);
    CleanupStack::PushL(buffer);
    buffer->Data().FillZ(aSize);
    buffer->SetRequestSizeL(aSize);
	buffer->Data().SetLength(0);
    return buffer;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioUtility::DecodeIntervalEvent
// -----------------------------------------------------------------------------
//	
EXPORT_C void CAdvancedAudioUtility::DecodeIntervalEvent()
    {
    if (iObserver)
        {
        BitRate(); // updates iBitRate
        TInt lim = iReferenceBitRate/10; // 10% tolerance
        if (abs(iReferenceBitRate - iBitRate) > lim)
            {
            iObserver->BitRateChanged();
            iReferenceBitRate = iBitRate;
            }
        }
    }
    
/*    
EXPORT_C TInt CAdvancedAudioUtility::IsSeeking(TBool& aIsSeeking)
    {
    TInt stat;
    if (iDecoder)
    	{
	    stat = iDecoder->IsSeeking(aIsSeeking);
    	}
    else
    	{
        stat = KErrNotSupported;
    	}	
    return stat;
    }
*/    
// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::SeekPositionReached
// -----------------------------------------------------------------------------
//	
EXPORT_C void CAdvancedAudioUtility::SeekPositionReached(TUint aTimeMs)
	{
	iObserver->SeekPositionReached(aTimeMs);
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::PlayWindowEndPositionReached
// -----------------------------------------------------------------------------
//	
EXPORT_C void CAdvancedAudioUtility::PlayWindowEndPositionReached()
	{
	iObserver->PlayWindowEndPositionReached();
	}

EXPORT_C TInt CAdvancedAudioUtility::GetOffsets(TInt& aOffsets)
	{
	aOffsets = iHeaderOffset + iSyncOffset;
	return KErrNone;
	}

// End of File

