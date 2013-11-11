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
* Description:  The functions in this module implements the common behavior
*                for the audio decoder base class.
*
*/


// INCLUDE FILES
#include "AdvancedAudioDecoder.h"
#include "DebugMacros.h"
#include <mmfdatabuffer.h>
//#include <Mmfcodec.h>

EXPORT_C TCodecProcessResult CAdvancedAudioDecoder::ProcessHwL(CMMFBuffer& aSrc, CMMFBuffer& aDst)
	{
    DP0(_L ("CAdvancedAudioDecoder::ProcessAccelL ---start"));
	
	TMMFPtr8 codecSrc;
	TMMFPtr8 codecDst;

	TCodecProcessResult result;
	result.iStatus = TCodecProcessResult::EProcessIncomplete;

	//convert from generic CMMFBuffer to CMMFDataBuffer
	iSrc = static_cast<const CMMFDataBuffer*>(&aSrc);
	iDst = static_cast<CMMFDataBuffer*>(&aDst);

	const TUint srcLen = iSrc->Data().Length();
	const TUint dstAvail = iDst->Data().MaxLength() - iDst->Position();

	if (!dstAvail)
		User::Leave(KErrArgument);

	TUint dstAdded = 0;
	TUint srcUsed = 0;

	TInt srcPos = iSrc->Position();
	TInt dstPos = iDst->Position();

//  DP4(_L ("CAdvancedAudioDecoder::ProcessAccelL: srcLen[%d], srcPos[%d] dstPos[%d], dstAvailLen[%d]"), srcLen, srcPos, dstPos, dstAvailLen);

	while ((dstAdded < dstAvail) && (srcPos + srcUsed < srcLen))
        {
        codecSrc.Set(iSrc->Data());
        codecDst.Set(iDst->Data());
        codecDst.SetLength(codecDst.MaxLength());
		codecSrc.Shift(srcPos + srcUsed);
		codecDst.Shift(dstPos + dstAdded);

//		DP2(_L ("CAdvancedAudioDecoder::ProcessAccelL: before ProcessDecodeL offsetting srcptr to srcPos[%d]+srcUsed[%d]"), srcPos, srcUsed);

		ProcessParseL(&codecSrc, &codecDst);
		if (!iEnabled)
			{ // output buffer is invalid when stopped - don't use it
	    	DP0(_L ("CAdvancedAudioDecoder::ProcessAccelL --- end - disabled - returning"));
	    	result.iStatus = TCodecProcessResult::EProcessComplete;
			return result;
			}

        dstAdded += codecDst.Length();
        srcUsed += codecSrc.Length();

        if (iMarkPlayEnd)
        	{ // don't parse any more data
            DP0(_L ("CAdvancedAudioDecoder::ProcessAccelL --- mark play end breaking"));
			break;        	
        	}
        }

	result.iSrcBytesProcessed = srcUsed;
	result.iDstBytesAdded     = dstAdded;

	if (result.iDstBytesAdded && (result.iSrcBytesProcessed + srcPos >= srcLen))
		result.iStatus = TCodecProcessResult::EProcessComplete;
	else if (result.iDstBytesAdded < dstAvail)
		result.iStatus = TCodecProcessResult::EDstNotFilled;
	
    iDst->Data().SetLength(iDst->Position()+result.iDstBytesAdded);

    DP0(_L ("CAdvancedAudioDecoder::ProcessAccelL ---end"));
    return result;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::ProcessParseL
// ProcessParseL method that shall be called from the ProcessL method to perform
// the parsing of the encoded data
// -----------------------------------------------------------------------------
EXPORT_C void CAdvancedAudioDecoder::ProcessParseL(TMMFPtr8* aSrc, TMMFPtr8* aDst)
    {
    DP0(_L ("CAdvancedAudioDecoder::ProcessParseL ---start"));
    TUint8* srcPtr  = NULL;
    TUint8* dstPtr  = NULL;
    TInt    srcLen  = aSrc->Length();
    TInt    dstAvail= aDst->Length();
    TInt    srcUsed = 0;
    TInt    dstUsed = 0;

    //**************************************************
    //* Get Input Data:  Only want to process more input
    //* data if the output buffer is empty.
    //**************************************************

    if(iOutBufferCount == 0)
        {
        srcPtr = (TUint8*) aSrc->Ptr();

        //*************************************************
        //* If the input buffer is empty, and there is
        //* enough data in the input stream, then use the
        //* data from the input stream.  Else must append
        //* available input data to the input buffer.
        //*************************************************

        srcPtr = PrepareInFrame(srcPtr, srcLen, srcUsed);

        if (iInBufferCount > 0)
            {
            srcLen = iInBufferCount;
            }
        }

    //*************************************************
    //* If there is enough input data, then determine
    //* where the output should go (output stream or
    //* output buffer if output stream doesn't have
    //* enough room).  Then parse the data.
    //*************************************************

    if(srcPtr)
        {
        // Determine where to put output data
        dstPtr = (TUint8*) aDst->Ptr();
        dstUsed = iOutFrameLength;
		TInt inLen, outLen;

		outLen = dstAvail;
		
        if(dstAvail < iOutFrameLength)
            {
//	        DP2(_L ("CAdvancedAudioDecoder::ProcessParseL: dstAvail[%d], iOutFrameLength[%d]"), dstAvail, iOutFrameLength);

            dstPtr = iOutBuffer;
			outLen = iOutFrameLength; // Set the dst length to the new length
            }

        // Parse one frame
        inLen = srcLen;
        
        ParseL(srcPtr, inLen, dstPtr, outLen);
		if (!iEnabled)
			{ // aDst and dstPtr invalid when stopped - don't use
	    	DP0(_L ("CAdvancedAudioDecoder::ProcessParseL --- end - disabled - returning"));
			return;
			}

        // adjust in counters
        if (iInBufferCount > 0)
            {
            // if parse from the temp buf, adjust indexes
//	    	DP3(_L ("CAdvancedAudioDecoder::ProcessParseL adjusting tempbuf after parse count[%d] indx[%d] used[%d]"),
//	    			iInBufferCount, iInBufferIndx, inLen);
            iInBufferCount -= inLen;
            iInBufferIndx += inLen;
            }
        else
            {
            srcUsed = inLen;
            }
        
		if ((inLen == 0) && (iSrc->LastBuffer()))
			{
			srcUsed = srcLen;
			dstUsed = 0;
			}

        // adjust out counters
        dstUsed = outLen;
        
        if (dstAvail < iOutFrameLength)
            iOutBufferCount = outLen;
        }

    //*************************************************
    //* If any data is stored in the output buffer,
    //* then output as much of it as well fit in the
    //* output stream.
    //*************************************************

    if(iOutBufferCount > 0)
        {
        dstUsed = Min (iOutBufferCount, dstAvail);
        TPtrC8 outPtr (iOutBufferPtr, dstUsed);

        aDst->SetLength(0);
        aDst->Append (outPtr);

        iOutBufferCount -= dstUsed;
        iOutBufferPtr   += dstUsed;

        if(iOutBufferCount == 0)
            {
            iOutBufferPtr = iOutBuffer;
            }
        }


    //*************************************************
    //* Modify the length attributes of the source and
    //* destination data streams to inform the caller
    //* of how much data was used in each buffer.
    //*************************************************


//    DP2(_L ("CAdvancedAudioDecoder::ProcessParseL maxlen[%d] used[%d]"), aSrc->MaxLength(), srcUsed);
    aSrc->SetLength(srcUsed);
    aDst->SetLength(dstUsed);
    DP0(_L ("CAdvancedAudioDecoder::ProcessParseL ---end"));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::PrepareInFrame
// -----------------------------------------------------------------------------
EXPORT_C TUint8* CAdvancedAudioDecoder::PrepareInFrame(TUint8* aSrc, TInt aSrcLen, TInt& aSrcUsed)
	{
//    DP0(_L ("CAdvancedAudioDecoder::PrepareInFrame ---start"));
    // note: aSrcUsed returns how much data is used from source buffer.
    // when it reaches the value of aSrcLen, we will be provided the next input buffer.
    // while we are decoding from the temp buf, we set it to 0 because we have not decoded any from
    // the current src buffer.
    TUint8* dataPtr = NULL;
	aSrcUsed = 0;

    if (iInBufferCount == 0)
        {
        // decoding from src buffer
//        DP2(_L("CAdvancedAudioDecoder::PrepareInFrame, aSrc[0x%x] aSrcLen[%d]"), aSrc, aSrcLen);

        iInBufferFilled = EFalse;
        iInBufferIndx = 0;
        
		if (aSrcLen >= iMaxFrameSize)
			{ // if enough data in input buffer, just use input buffer
//			DP2(_L ("CAdvancedAudioDecoder::PrepareInFrame using input buffer[0x%x][0x%x]"), aSrc, aSrc[0]);
			dataPtr = aSrc;
			}
		else if (iSrc->LastBuffer())
			{ // if input buffer is last buffer, then use it
//			DP2(_L ("CAdvancedAudioDecoder::PrepareInFrame using last input buffer[0x%x][0x%x]"), aSrc, aSrc[0]);
			dataPtr = aSrc;
			}
		else
			{
			// save input data
			// When not enough data in input buffer, move it to temp buffer
//			DP5(_L ("CAdvancedAudioDecoder::PrepareInFrame moving to temp buffer[0x%x] src[0x%x]d0[0x%x]srclen[%d]dend[0x%x]"),
//						iInBuffer, aSrc, aSrc[0], aSrcLen, aSrc[aSrcLen-1]);
			Mem::Copy(iInBuffer, aSrc, aSrcLen);
			iInBufferCount = aSrcLen;
			aSrcUsed = aSrcLen;     // inform that we used input buffer so we will get a new input buffer
			iInBufferIndx = 0;      // index will move with amount of data decoded
			dataPtr = NULL;         // NULL prevents decoder from decoding. We need to fill rest of temp buffer
			iUsedInBuffer = iInBufferCount;
	        iSavedBytesCopiedFromSrc = 0;
			}
        }
    else
        {
        // decoding from temp buffer
        dataPtr = iInBuffer+iInBufferIndx;  // index is adjusted after decode and ptr is data to decode
//		DP4(_L ("CAdvancedAudioDecoder::PrepareInFrame using temp buffer dataPtr[0x%x] iInBufferIndx[%d] iInBufferCount[%d] iInBufferFilled[%d]"),
//					dataPtr, iInBufferIndx, iInBufferCount, iInBufferFilled);
		
		if (!iInBufferFilled)
            {
            // need to fill rest of temp buffer
            TInt bytesToCopy = iSizeOfInBuffer-iUsedInBuffer;
            TInt bytesAvailable = aSrcLen;
            
            if (bytesAvailable < bytesToCopy)
                {
                bytesToCopy = bytesAvailable;
                }
                
            if (bytesToCopy)
                {
                // only do this once - partial input buffer means last buffer
//				DP5(_L ("CAdvancedAudioDecoder::PrepareInFrame putting more to temp buffer dstptr[0x%x] srcptr[0x%x]d0[0x%x]cnt[%d]dend[0x%x]"),
//							dataPtr+iUsedInBuffer, aSrc, aSrc[0], bytesToCopy, iInBuffer[KSizeOfInBuffer-1]);
       			Mem::Copy(dataPtr+iUsedInBuffer, aSrc, bytesToCopy);
        		iSavedBytesCopiedFromSrc = bytesToCopy; // save byte count used from this src buffer
	        	iInBufferCount += bytesToCopy;
	        	iUsedInBuffer += bytesToCopy;
	        	iInBufferFilled = ETrue;
                }
            }
        else
           	{
           	// temp buffer is filled and we are decoding from it
       	    TInt decodeOffsetIntoInputBuf = iSavedBytesCopiedFromSrc - iInBufferCount;
//			DP1(_L ("CAdvancedAudioDecoder::PrepareInFrame using temp buf aSrcUsed[%d]"), decodeOffsetIntoInputBuf);

       	    if (decodeOffsetIntoInputBuf >= 0)
       	    	{
       	    	// within the temp buffer, we have decoded all of the prior src buffer's data
       	    	// we are decoding into the data copied from the current input buffer
       	    	// so we can set the pointer to start using input buffer again
    	        dataPtr = NULL; // NULL prevents decoder from decoding. Need to let src get updated by aSrcUsed
				iInBufferCount = 0;
				aSrcUsed = decodeOffsetIntoInputBuf;
//				DP1(_L ("CAdvancedAudioDecoder::PrepareInFrame using inbuffer again, aSrcUsed[%d]"), aSrcUsed);
       	    	}
       	    else
       	    	{
       	    	aSrcUsed = 0; // here references current src buffer that we are not decoding from yet.
//				DP1(_L ("CAdvancedAudioDecoder::PrepareInFrame using inbuffer again, aSrcUsed[%d] b"), aSrcUsed);
       	    	}	
        	}
        }

    return dataPtr;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::ParseL
// -----------------------------------------------------------------------------
EXPORT_C void CAdvancedAudioDecoder::ParseL(TUint8* aSrc, TInt& aSrcUsed, TUint8* aDst, TInt& aDstLen)
	{
    DP1(_L ("CAdvancedAudioDecoder::ParseL [0x%x]"), aSrc[0]);
	TInt savedLen = aSrcUsed;
	TInt err = KErrNone;
	TInt inLen = aSrcUsed;
//    TAudioFrameInfo frameInfo;
	TInt frameLen;

    // Parse one frame

	err = FrameLength(aSrc, inLen, frameLen);
	aSrcUsed = frameLen;
	if (aSrcUsed > savedLen)
		{ // in case util reports more bytes used than what is available at the end of the content
		aSrcUsed = savedLen;
		}

	if (!iRenderEnabled)
		{ // seeking
        aDstLen = 0;
		}	
	else
		{
		if (err == KErrNone)
			{
			Mem::Copy(aDst, aSrc, aSrcUsed);
			aDstLen = aSrcUsed;
			}
		}

	if (err != KErrNone)
		{
		aSrcUsed = 0;
        err = KErrCorrupt;
		}	
		
//	DP2(_L ("CAdvancedAudioDecoder::ParseL after decode aSrcUsed[%d] aDstLen[%d]"), aSrcUsed, aDstLen);

    DP1(_L ("CAdvancedAudioDecoder::ParseL err [%d]"),err);

	if (err == KErrNone)
		{
		if (iFrameTable)
			{
	   	    iFrameTable->SubmitTableEntry(iAccLen); // to add an entry for the frame actually decoded
			}
		}
		
	if (err == KErrCorrupt)
		{
		aDstLen = 0;
		// decode was not successful at this position, need to move forward because sync may still be found here.
		TInt syncPos = SeekSync(aSrc+1, savedLen-1); // decoder's seek returns 0 if not found, so we need to use our own seek function
		if (syncPos == savedLen-1)
			{ // search whole buffer - no sync found - so indicate the rest of the data has been used
			aSrcUsed = savedLen;
			}
		else 
			{
			aSrcUsed = syncPos+1;
			}	
		err = KErrNone;
		}

   	iAccLen += aSrcUsed;

    if(err != KErrNone)
        { // unrecoverable error
	    if (iSrc->LastBuffer())
			{ // don't have to error on last buffer, just indicate all the data was used
			aSrcUsed = savedLen;
			}
        else
            {
            User::Leave(err);
            }
        }
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::SeekSync
// -----------------------------------------------------------------------------
TInt CAdvancedAudioDecoder::SeekSync(TUint8* /*aBuf*/, TInt /*aBufLen*/)
    {
    return KErrNotSupported;
    }

TInt CAdvancedAudioDecoder::FrameLength(const TUint8* /*aBuf*/, TInt /*aBufLen*/, TInt& /*aFrameLength*/)
	{
    return KErrNotSupported;
	}


// End of file
