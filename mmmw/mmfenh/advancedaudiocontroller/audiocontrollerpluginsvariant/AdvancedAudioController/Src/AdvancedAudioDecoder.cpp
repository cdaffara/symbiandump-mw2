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


// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CAdvancedAudioDecoder::CAdvancedAudioDecoder(
	TInt aPriority)
    :   CActive(aPriority),
    	iState(EIdle),
        iSharedBuffers(NULL),
        iMMFDevSound(NULL),
    	iObserver(NULL)
    {
    DP0(_L("CAdvancedAudioDecoder::CAdvancedAudioDecoder()"));

    iDecoderUtilityObserver = NULL;
    iFrameTable = NULL;
    }

// Destructor
EXPORT_C CAdvancedAudioDecoder::~CAdvancedAudioDecoder()
	{
    DP0(_L("CAdvancedAudioDecoder::~CAdvancedAudioDecoder()"));
	iRenderEnableConfig.Close();
	iRenderDisableConfig.Close();
	iMarkPlayEndConfig.Close();
	iUnMarkPlayEndConfig.Close();
	iEnableConfig.Close();
	iDisableConfig.Close();
	delete iConvertBuffer;
	delete iChannelAndSampleRateConverterFactory;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::SetDevSound
// Sets the DevSound instance.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::SetDevSound(
	CMMFDevSound& aDevSound)
	{
	iMMFDevSound = &aDevSound;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::SetObserver
// Sets the observer instance.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::SetObserver(
	MAdvancedAudioDecoderObserver& aObserver)
	{
	iObserver = &aObserver;
	}
	
// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::SetDecoderUtilityObserver
// Sets the observer instance.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::SetDecoderUtilityObserver(MAdvancedAudioDecoderUtilityObserver& aDecoderUtilityObserver)
	{
	iDecoderUtilityObserver = &aDecoderUtilityObserver;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::SetSourceBuffers
// Sets the source buffer for conversion operations.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::SetSourceBuffers(
	RPointerArray<CMMFDataBuffer>* aBuffers)
	{
	iSharedBuffers = aBuffers;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::SetConfigL
// Sets audio decoder settings and attributes
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::SetConfigL(
    TUint aSourceSampleRate,
    TUint aSourceChannels,
    TUint aSWConvertSampleRate,
    TUint aSWConvertChannels,
    const RArray<TInt>& aCodecConfigData,
    TInt aIndex)
    {
    DP0(_L("CAdvancedAudioDecoder::SetConfigL"));

	CodecConfig(const_cast<RArray<TInt>&>(aCodecConfigData));

	QueueThisBuffer(aIndex); // will set iNextBuffer (current buffer) and update iSharedBufferIndex (to next buffer)

   	if(aSourceSampleRate != aSWConvertSampleRate && !IsHwAccelerated())  // Sampling Rate Conversion is needed
   		{
	    if (!iChannelAndSampleRateConverterFactory)
			{
		    iChannelAndSampleRateConverterFactory = new(ELeave) CMMFChannelAndSampleRateConverterFactory;
	    	iChannelAndSampleRateConverter = iChannelAndSampleRateConverterFactory->CreateConverterL(aSourceSampleRate, aSourceChannels,
		                                                                                         aSWConvertSampleRate, aSWConvertChannels);
		    }
		iNeedsSWConversion = ETrue;
    	}

	Enable();
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::FillBuffer
// Request to fill the specified buffer with converted data.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::FillBufferL(
	CMMFBuffer* aBuffer)
	{
    DP0(_L("CAdvancedAudioDecoder::FillBufferL"));
	iBufferToFill = aBuffer;
	iState = EDecoding;
	iStatus = KRequestPending; // service request would be made here and pending set by service provider
    SetActive();
	Ready(KErrNone);
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::Stop
// Stops decoding process.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::Stop()
	{
    DP0(_L("CAdvancedAudioDecoder::Stop"));

	iState = EIdle;
	Disable();
	RenderEnable();
	iEventPos = 0;

	// cancel any outstanding FrameTable Seeking event callback
	if (iFrameTable)
	    {
	    TInt err = iFrameTable->UnRegisterForEvent(CFrameTable::EPosReached, this);
	    // Fix for the error ou1cimx1#137567 - SXUU-7S9SLD
	    //TInt err1 = iFrameTable->UnRegisterForEvent(CFrameTable::EPlayWindowEndPosReached, this);
	    //if (err|err1 != KErrNone)
	    if (err != KErrNone)
	        {
            //DP2(_L("CAdvancedAudioDecoder::Stop, UnRegisterForEvent errs[%d][%d]"), err, err1);
	        DP1(_L("CAdvancedAudioDecoder::Stop, UnRegisterForEvent err[%d]"), err);
	        }
	    }	
	}

EXPORT_C TInt CAdvancedAudioDecoder::SetSourceReference(TUint aTimeMs, TUint aPos)
	{
    if (!iFrameTable)
        return KErrNotSupported;

    return iFrameTable->SetSourceReference(aTimeMs, aPos);
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::FindFramePosFromTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioDecoder::FindFramePosFromTime(TUint& aTimeMs, TUint& aPos)
	{
    if (!iFrameTable)
        return KErrNotSupported;

    return iFrameTable->FindFramePosFromTime(aTimeMs, aPos);
	}

EXPORT_C TInt CAdvancedAudioDecoder::FindFrameTimeFromPos(TUint& aTime, TUint& aPos)
    {
    if (!iFrameTable)
        return KErrNotSupported;
    
    return iFrameTable->FindFrameTimeFromPos(aTime, aPos);
    }

	
// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::LastFramePos
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioDecoder::LastFramePos(TUint& aPos)
	{
    if (!iFrameTable)
        return KErrNotSupported;

    return iFrameTable->LastFramePos(aPos);
	}
	
// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::LastFrameTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioDecoder::LastFrameTime(TUint& aTimeMs)
	{
    if (!iFrameTable)
        return KErrNotSupported;

    return iFrameTable->LastFrameTime(aTimeMs);
	}
	
// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::ResetTable
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioDecoder::ResetTable()
	{
    if (!iFrameTable)
        return KErrNotSupported;

    iFrameTable->ResetTable();
    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::SeekToTimeMs
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CAdvancedAudioDecoder::SeekToTimeMs(TUint aTimeMs)
	{
    if (!iFrameTable)
        return KErrNotSupported;
    if (aTimeMs == KMaxTUint)
        {
        // Only other place that we unregister is when we stop the decoder.
        // Here we need to unregister explicitly such as when there is an outstanding
        // seek and we seek again, but if we are not playing yet, we won't stop the output
        // and unregister outstanding events by stopping the output.
        DP0(_L("CAdvancedAudioDecoder::SeekToTimeMs unregistering events"));
        TInt status1 = iFrameTable->UnRegisterForEvent(CFrameTable::EPosReached, this);
        return status1;
        }
    
    RenderDisable();
    iSeekToTimeMs = aTimeMs;
    return iFrameTable->RegisterForEvent(CFrameTable::EPosReached, this, aTimeMs);
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::SetPlayEndTimeMs
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CAdvancedAudioDecoder::SetPlayWindowEndTimeMs(TUint aTimeMs)
	{
	TInt status = KErrNotSupported;
    if (iFrameTable)
    	{
	    if (aTimeMs == 0)
    		{
	    	status = iFrameTable->UnRegisterForEvent(CFrameTable::EPlayWindowEndPosReached, this);
	    	}
    	else
	    	{
    		status = iFrameTable->RegisterForEvent(CFrameTable::EPlayWindowEndPosReached, this, aTimeMs);
    		}	
    	}
    return status;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::BitRate
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CAdvancedAudioDecoder::BitRate()
	{
	if (!iFrameTable)
        return KErrNotSupported;

	return iFrameTable->Bitrate();
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::EnableDecodeIntervalEvent
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CAdvancedAudioDecoder::EnableDecodeIntervalEvent(TBool aEnable, TUint aTimeMs)
	{
    if (!iFrameTable)
        return KErrNotSupported;
    else
	    {
    	if (aEnable)
            return iFrameTable->RegisterForEvent(CFrameTable::EDecodeInterval, this, aTimeMs);
        else
            return iFrameTable->UnRegisterForEvent(CFrameTable::EDecodeInterval, this);
    	}
    }


// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::HandleFrameTableEvent
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CAdvancedAudioDecoder::HandleFrameTableEvent(CFrameTable::TFrameTableEvent aEvent)
	{
	TInt status = KErrNone;
	switch (aEvent)
		{
		case CFrameTable::EPlayWindowEndPosReached:
		case CFrameTable::EPosReached:
			RenderEnable();
		    if (iDecoderUtilityObserver)
		        {
		        if (aEvent == CFrameTable::EPosReached)
		        	{
		            DP0(_L("CAdvancedAudioDecoder::HandleFrameTableEvent, EPosReached"));
			        // to allow the iTimePositionInMicroSecs to be updated to seek time in controller
					TInt offsets;
					TUint pos;
					status = iDecoderUtilityObserver->GetOffsets(offsets);
					status = iFrameTable->GetLastPosEvent(pos);
		            DP2(_L("CAdvancedAudioDecoder::HandleFrameTableEvent, pos = %U, offsets = %U"),pos,offsets);
					iEventPos = pos + offsets;
   					iDecoderUtilityObserver->SeekPositionReached(iSeekToTimeMs);
		        	}
		        else
		        	{ // play window end
                    DP0(_L("CAdvancedAudioDecoder::HandleFrameTableEvent play window end reached"));
   					iDecoderUtilityObserver->PlayWindowEndPositionReached();
   					MarkPlayEnd();
		        	}
		        }
			break;
			
		case CFrameTable::EDecodeInterval:
            DP0(_L("CAdvancedAudioDecoder::HandleFrameTableEvent, EDecodeInterval"));
		
		    if (iDecoderUtilityObserver)
		        {
			    iDecoderUtilityObserver->DecodeIntervalEvent();
		        }
		    
			break;
			
		default:
		    User::Panic(_L("CAdvancedAudioDecoder::HandleFrameTableEvent, Unexpected event"), KErrArgument);
		    break;
		}
		
	return status;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::Ready
// Utility function to post a request complete
// -----------------------------------------------------------------------------
//
void CAdvancedAudioDecoder::Ready(
    const TInt aStatus)
    {
    TRequestStatus* stat = &iStatus;
    User::RequestComplete(stat, aStatus);
    }


// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::QueueThisBuffer
// gets buffer at aBufferIndex ready for decoding
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::QueueThisBuffer(TUint aBufferIndex)
	{
    iNextBuffer = (*iSharedBuffers)[aBufferIndex];
        
    DP4(_L("CAdvancedAudioDecoder::QueueThisBuffer, Index[%d] [%x] last[%d] pos[%d]"), 
   			aBufferIndex, static_cast<CMMFDataBuffer*>(iNextBuffer)->Data().Ptr(), iNextBuffer->LastBuffer(), iNextBuffer->Position());

    iSharedBufferIndex = aBufferIndex + 1;
     
    if (iSharedBufferIndex == iSharedBuffers->Count())
     	{
   	    iSharedBufferIndex = 0;
       	}

    iFrameTable->ShrinkTable();
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::NextSharedBufferL
// Determine the next available shared buffer from the pool of shared buffers
// iNextBuffer is actually the current buffer in use
// iSharedBufferIndex points to the next buffer to be used
// -----------------------------------------------------------------------------
//
void CAdvancedAudioDecoder::NextSharedBufferL()
    {
    CMMFBuffer* refillBuffer = NULL;
    // index is the next buffer, index-1 is current buffer, check 2 buffers back from the one being selected
// ou1cimx1#205863    
    //TInt checkBufIndex = iSharedBufferIndex-2;
    TInt checkBufIndex = iSharedBufferIndex- (iSharedBuffers->Count()-1) ;
    	
    if (checkBufIndex < 0)
    	{
        checkBufIndex = checkBufIndex+iSharedBuffers->Count();
    	}
    	
    if ((*iSharedBuffers)[checkBufIndex]->Status() == EBeingEmptied)
        {
// ou1cimx1#205863
/*        if (iSharedBuffers->Count() <= 2)
            {
            DP1(_L("CAdvancedAudioDecoder::NextSharedBufferL leave, EBeingEmptied is used on Count[%d]"), iSharedBuffers->Count());
            User::Leave(KErrAbort);
            }
*/            
        DP2(_L("CAdvancedAudioDecoder::NextSharedBufferL, index[%d] [%x] is EBeingEmptied"), 
            	checkBufIndex, static_cast<CMMFDataBuffer*>((*iSharedBuffers)[checkBufIndex])->Data().Ptr() );
        refillBuffer = (*iSharedBuffers)[checkBufIndex];
	    }
	if (refillBuffer)
		{
    	iObserver->RefillBuffer(refillBuffer);
		}
    if ((*iSharedBuffers)[iSharedBufferIndex]->Status() == EFull)
   	    {
        QueueThisBuffer(iSharedBufferIndex);
   	    DP1(_L("CAdvancedAudioDecoder::NextSharedBufferL, After Increment, index[%d]"), iSharedBufferIndex);
     	}
   	else
   		{
       	DP2(_L("CAdvancedAudioDecoder::NextSharedBufferL - not ready, Index[%d] [%x]"),
       					iSharedBufferIndex, static_cast<CMMFDataBuffer*>((*iSharedBuffers)[iSharedBufferIndex])->Data().Ptr() );
        QueueThisBuffer(iSharedBufferIndex); // we should still have the buffer ready to be used when it is filled
        User::Leave(KErrBufferNotReady);
   	 	}
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::RunL
// Invoke by the active scheduler when a request completes
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::RunL()
	{
    DP0(_L("CAdvancedAudioDecoder::RunL"));
	if (iState == EDecoding)
		{
		TRAPD(err,HandleFillBufferL());
		if (err) // if error is encountered, an event is generated and the playback is paused.
			{
			iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, err));
			}
		}
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::HandleFillBufferL
// Handle the request to fill the specified buffer with converted data.
// Fill next buffer with audio data. If soft codec, data must first be decoded.
// If source buffer is depleted, a request to the controller is sent to refill
// the buffer with more data and the next source buffer is used.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::HandleFillBufferL()
	{
    DP3(_L("CAdvancedAudioDecoder::HandleFillBufferL ptr[%x] iRenderEnabled[%d] Position[%d]"), static_cast<CMMFDataBuffer*>(iNextBuffer)->Data().Ptr(), iRenderEnabled, iNextBuffer->Position());
    DP3(_L("CAdvancedAudioDecoder::HandleFillBufferL frm#[%d] d0[%x] dp[%x]"), static_cast<CMMFDataBuffer*>(iNextBuffer)->FrameNumber(), static_cast<CMMFDataBuffer*>(iNextBuffer)->Data().Ptr()[0],
     static_cast<CMMFDataBuffer*>(iNextBuffer)->Data().Ptr()[iNextBuffer->Position()]);
// ou1cimx1#205863
//    iBufferToFill->SetPosition(0);
//	static_cast<CMMFDataBuffer*>(iBufferToFill)->Data().SetLength(0);
    iBufferToFill->SetLastBuffer(EFalse);

    TCodecProcessResult result;
    TBool moreProcessing = ETrue;

    while (moreProcessing)
        {
        TRAPD(err, result = ProcessL(*iNextBuffer, *iBufferToFill));
        DP4(_L ("CAdvancedAudioDecoder::HandleFillBufferL - ProcessL stat[%d] result[%d] iEnabled[%d] iMarkPlayEnd[%d]"),
                err, result, iEnabled, iMarkPlayEnd);
   		if (!iEnabled)
            { // output buffers are invalid when stopped - don't use them
	        DP0(_L ("CAdvancedAudioDecoder::HandleFillBufferL - disabled - returning"));
            return;
            }

        if (err != 0)
            {
            if (err == KErrCorrupt)
                { // go ahead and use corrupt if the codec determines the data is corrupt.
                User::Leave(KErrCorrupt);
                }
            else
                { // don't use KErrDied or controller will pause
                User::Leave(KErrGeneral);
                }
            }
		
		if (iMarkPlayEnd)
			{
            DP0(_L ("CAdvancedAudioDecoder::HandleFillBufferL marking play end"));
            iNextBuffer->SetLastBuffer(ETrue);
            iMarkPlayEnd = EFalse;
            result.iStatus = TCodecProcessResult::EEndOfData;
			}
//      DP5(_L ("CAdvancedAudioDecoder::HandleFillBufferL after decode istatus[%d] iSrcBytesProcessed[%d] iDstBytesAdded[%d] srcPosition[%d] dstPosition[%d]"),
//               result.iStatus, result.iSrcBytesProcessed, result.iDstBytesAdded, iNextBuffer->Position(), iBufferToFill->Position());

	    TInt bytesInDstBuf = iBufferToFill->Position()+result.iDstBytesAdded;
	    TInt bytesUsedFromSrcBuf = iNextBuffer->Position()+result.iSrcBytesProcessed;
	   	DP3(_L("CAdvancedAudioDecoder::HandleFillBufferL bytesUsedFromSrcBuf[%d] Position[%d] iSrcBytesProcessed[%d]"), 
	   			bytesUsedFromSrcBuf, iNextBuffer->Position(), result.iSrcBytesProcessed);
	    
        if ((result.iStatus == TCodecProcessResult::EProcessComplete ||
				result.iStatus == TCodecProcessResult::EEndOfData ||
				result.iStatus == TCodecProcessResult::EDstNotFilled) && iNextBuffer->LastBuffer())
            {
   			iBufferToFill->SetPosition(bytesInDstBuf);
  			iNextBuffer->SetPosition(bytesUsedFromSrcBuf);
		   	DP2(_L("CAdvancedAudioDecoder::HandleFillBufferL set pos[%d] in src buf[0x%x]"), 
		   			bytesUsedFromSrcBuf, static_cast<CMMFDataBuffer*>(iNextBuffer)->Data().Ptr());
  			
            DP0(_L ("CAdvancedAudioDecoder::HandleFillBufferL setting output buffer as last buffer"));
            iBufferToFill->SetLastBuffer(ETrue);
            moreProcessing = EFalse;
            }
        else
            {
            switch (result.iStatus)
                {
                case TCodecProcessResult::EProcessComplete:
                	// finished processing input buffer, all data in output buffer
                case TCodecProcessResult::EDstNotFilled:
                	// the output buffer is not full
                case TCodecProcessResult::EProcessIncomplete:
                	// the output buffer was filled before all the input buffer was processed
                    {                
                    iBufferToFill->SetPosition(bytesInDstBuf);
                    iNextBuffer->SetPosition(bytesUsedFromSrcBuf);
				   	DP2(_L("CAdvancedAudioDecoder::HandleFillBufferL set pos[%d] in src buf[0x%x]"), 
				   			bytesUsedFromSrcBuf, static_cast<CMMFDataBuffer*>(iNextBuffer)->Data().Ptr());
  			
                    
                    TInt srclength = static_cast<CMMFDataBuffer*>(iNextBuffer)->Data().Length();
                	TInt dstlength = static_cast<CMMFDataBuffer*>(iBufferToFill)->Data().Length();
					TInt dstmaxlength = static_cast<CMMFDataBuffer*>(iBufferToFill)->Data().MaxLength();

                    // bytesUsedFromSrcBuf should always be <= iNextBuffer's length
					if (bytesUsedFromSrcBuf == srclength)
                        {
                        // all input buffer used
                        if (iNextBuffer->LastBuffer())
                            {
                            DP0(_L ("CAdvancedAudioDecoder::HandleFillBufferL all src used - setting output last buffer"));
                            iBufferToFill->SetLastBuffer(ETrue);
                            moreProcessing = EFalse;
                            }
                        else
                            {
                            // for streaming model keep this used buffer until we start decoding from the next buffer
							// so we can be sure to have  entries in the seek table for the next buffer for pause resume cases
							// for which we have non-seekable sources.
							// if we did send it to be refilled, we might pause before we start decoding into the next buffer
							// we could be decoding from the temp holding buffer in the codec.
                            // setting buffer status to EBeingEmptied indicates we are done with the buffer.
                            // it will be sent for refill later by NextSharedBufferL().
                            static_cast<CMMFDataBuffer*>(iNextBuffer)->SetStatus(EBeingEmptied);
                            NextSharedBufferL();
                            // length can only be <= maxlength
							if (dstlength == dstmaxlength)
								{
								moreProcessing = EFalse;
								}
                            }
                        }
                    else
                        {
                        // length can only be <= maxlength
						if (dstlength == dstmaxlength)
                            moreProcessing = EFalse;
                        }
                        
                    break;
                    }

                case TCodecProcessResult::EEndOfData:
                    {
                    DP0(_L("CAdvancedAudioDecoder::HandleFillBufferL, EEndOfData"));
                    iBufferToFill->SetLastBuffer(ETrue);
                    moreProcessing = EFalse;
                    break;
                    }

                default:
                    {
                    DP0(_L("CAdvancedAudioDecoder::HandleFillBufferL, leave on unexpected Result"));
                    User::Leave(KErrGeneral);
                    break;
                    }
                }
            }
            
        // break the loop during seeking, in case desired position is already reach during ProcessL
        if (!iRenderEnabled)
        	{
            moreProcessing = EFalse;
        	}
        }

    if (iNeedsSWConversion && iRenderEnabled)
    	{
	    iBufferToEmpty = iBufferToFill;
		CMMFDataBuffer* audio;
		
        if (iConvertBuffer)
            {
            delete iConvertBuffer;
            iConvertBuffer = NULL;
            }
            
		iConvertBuffer = CMMFDataBuffer::NewL(static_cast<CMMFDataBuffer*>(iBufferToFill)->Data().Length());
		iChannelAndSampleRateConverter->Convert(*(static_cast<CMMFDataBuffer*>(iBufferToFill)), *iConvertBuffer);
		audio = iConvertBuffer;

		//copy our converted data back into the real buffer to return to DevSound Output
		TDes8& dest = static_cast<CMMFDataBuffer*>(iBufferToFill)->Data();
		dest.SetLength(0);
		dest.Copy(audio->Data());
    	}

    if (iRenderEnabled)
    	{
        DP1(_L("CAdvancedAudioDecoder::HandleFillBufferL, iObserver->BufferFilled[%x]"),
        		static_cast<CMMFDataBuffer*>(iBufferToFill)->Data().Ptr());
    	iObserver->BufferFilled(iBufferToFill);     // send output buffer to devsound
    	}
    else
    	{
    	if (iBufferToFill->LastBuffer())
    		{
        	DP1(_L("CAdvancedAudioDecoder::HandleFillBufferL, iObserver->BufferFilled[%x] is last buffer"),
		       		static_cast<CMMFDataBuffer*>(iBufferToFill)->Data().Ptr());
	    	iObserver->BufferFilled(iBufferToFill); // be sure to send a last buffer to DS
    		}
    	else
    		{
			if (iState == EDecoding)
				{
				FillBufferL(iBufferToFill);
				}
    		}	
    	}
    	
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioDecoder::DoCancel
// Cancels the current and any on going requests/tasks.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioDecoder::DoCancel()
    {
    }

EXPORT_C void CAdvancedAudioDecoder::RenderDisable()
	{
   	DP0(_L("CAdvancedAudioDecoder::RenderDisable"));
	iRenderEnabled = EFalse;
	CodecCmd(ERenderDisable);
	}
	
EXPORT_C void CAdvancedAudioDecoder::RenderEnable()
	{
   	DP0(_L("CAdvancedAudioDecoder::RenderEnable"));
	iRenderEnabled = ETrue;
	CodecCmd(ERenderEnable);
	}

EXPORT_C void CAdvancedAudioDecoder::Disable()
	{
   	DP0(_L("CAdvancedAudioDecoder::Disable"));
	iEnabled = EFalse;
	CodecCmd(EDisable);
	}
	
EXPORT_C void CAdvancedAudioDecoder::Enable()
	{
   	DP0(_L("CAdvancedAudioDecoder::Enable"));
	iEnabled = ETrue;
	CodecCmd(EEnable);
	}

EXPORT_C void CAdvancedAudioDecoder::MarkPlayEnd()
	{
    DP0(_L("CAdvancedAudioDecoder::MarkPlayEnd"));
	iMarkPlayEnd = ETrue;
	CodecCmd(EMarkPlayEnd);
	}

EXPORT_C void CAdvancedAudioDecoder::UnMarkPlayEnd()
	{
    DP0(_L("CAdvancedAudioDecoder::UnMarkPlayEnd"));
	iMarkPlayEnd = EFalse;
	CodecCmd(EUnMarkPlayEnd);
	}

EXPORT_C TInt CAdvancedAudioDecoder::CodecConfig(RArray<TInt>& /*aConfig*/)
	{
    DP0(_L("CAdvancedAudioDecoder::CodecConfig not supported"));
	return KErrNotSupported;
	}
	
//EXPORT_C TInt CAdvancedAudioDecoder::IsSeeking(TBool& aIsSeeking)
//	{
//    if (!iFrameTable)
//        return KErrNotSupported;
//    return iFrameTable->IsSeeking(aIsSeeking);
//	}

EXPORT_C void CAdvancedAudioDecoder::ResetL()
	{
    DP0(_L("CAdvancedAudioDecoder::ResetL"));
	iSoftCodec->ResetL();
	// Enable the decoder for processing, to resume playback
	Enable();
	}

EXPORT_C TBool CAdvancedAudioDecoder::IsHwAccelerated()
	{
    DP0(_L ("CAdvancedAudioDecoder::IsHwAccelerated false"));
	return EFalse;
	}

EXPORT_C TCodecProcessResult CAdvancedAudioDecoder::ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst)
	{
    DP0(_L ("CAdvancedAudioDecoder::ProcessL"));
	TCodecProcessResult result;
    result = iSoftCodec->ProcessL(aSrc, aDst);
    return result;
	}

EXPORT_C TInt CAdvancedAudioDecoder::CodecCmd(TCodecCmd aCmd)
	{
    DP0(_L ("CAdvancedAudioDecoder::CodecCmd"));
	TInt stat = KErrNone;
	switch (aCmd)
		{
		case ERenderDisable:
			DP0(_L("CAdvancedAudioDecoder::CodecCmd RenderDisable"));
			TRAP(stat, iSoftCodec->ConfigureL(KUidConfig, reinterpret_cast<TDesC8&>(iRenderDisableConfig)));
			break;
		case ERenderEnable:
			DP0(_L("CAdvancedAudioDecoder::CodecCmd RenderEnable"));
			TRAP(stat, iSoftCodec->ConfigureL(KUidConfig, reinterpret_cast<TDesC8&>(iRenderEnableConfig)));
			break;
		case EDisable:
			DP0(_L("CAdvancedAudioDecoder::CodecCmd Disable"));
			TRAP(stat, iSoftCodec->ConfigureL(KUidConfig, reinterpret_cast<TDesC8&>(iDisableConfig)));
			break;
		case EEnable:
			DP0(_L("CAdvancedAudioDecoder::CodecCmd Enable"));
			TRAP(stat, iSoftCodec->ConfigureL(KUidConfig, reinterpret_cast<TDesC8&>(iEnableConfig)));
			break;
		case EMarkPlayEnd:
			DP0(_L("CAdvancedAudioDecoder::CodecCmd MarkPlayEnd"));
			TRAP(stat, iSoftCodec->ConfigureL(KUidConfig, reinterpret_cast<TDesC8&>(iMarkPlayEndConfig)));
			break;
		case EUnMarkPlayEnd:
			DP0(_L("CAdvancedAudioDecoder::CodecCmd UnMarkPlayEnd"));
			TRAP(stat, iSoftCodec->ConfigureL(KUidConfig, reinterpret_cast<TDesC8&>(iUnMarkPlayEndConfig)));
			break;
		default:
			break;	
		}
	return stat;	
	}


// End of file
