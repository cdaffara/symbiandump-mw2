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
* Description:   TNE decoder wrapper
*
*/




// INCLUDE FILES
#include <devvideoconstants.h>
#include "TNEDecoderWrap.h"
#include "ctrsettings.h"

// MACROS
#define TRASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CTRANSCODERVIDEODECODERCLIENT"), -10010))


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTNEDecoderWrap::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTNEDecoderWrap* CTNEDecoderWrap::NewL(MThumbnailObserver* aObserver)
{
    PRINT((_L("CTNEDecoderWrap::NewL(), In")))
    CTNEDecoderWrap* self = new (ELeave) CTNEDecoderWrap(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();

    PRINT((_L("CTNEDecoderWrap::NewL(), Out")))
    return self;
}


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::CTNEDecoderWrap
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTNEDecoderWrap::CTNEDecoderWrap(MThumbnailObserver* aObserver)
{    
    iObserver = aObserver;
    iDevVideoPlay = NULL;
    iCompresedFormat = NULL;
    iClockSource = NULL;
    iUid = TUid::Null();
    iHwDeviceId = THwDeviceId(0);
    iInputBuffer = NULL;
    iCodedBuffer = NULL;
    iDecodedPicture = NULL;
    iFatalError = KErrNone;
    iDataUnitType = EDuCodedPicture;
    iStop = EFalse;
    iState = ETRNone;
    iLastTimestamp = -1;
}


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::ConstructL()
{
    iDevVideoPlay = CMMFDevVideoPlay::NewL(*this);
    
    // Support for AVC
   	iClockSource = CSystemClockSource::NewL();
   	iInputFrameNum = 1;	
}


// ---------------------------------------------------------
// CTNEDecoderWrap::~CTNEDecoderWrap()
// Destructor
// ---------------------------------------------------------
//
CTNEDecoderWrap::~CTNEDecoderWrap()
{
    PRINT((_L("CTNEDecoderWrap::~CTNEDecoderWrap(), In")))

    if (iDevVideoPlay)
    {
        delete iDevVideoPlay;
        iDevVideoPlay = NULL;
    }

    iInputBuffer = NULL;
    
    if (iClockSource)
    {
    	delete iClockSource;
    	iClockSource = 0;
    }
     
    
    if (iCompresedFormat)
    {
        delete iCompresedFormat;
    }
        
    iAcceleratedDecoders.Reset();
    iAcceleratedDecoders.Close();

    PRINT((_L("CTNEDecoderWrap::~CTNEDecoderWrap(), Out")))
}


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::SupportsCodec
// Checks whether this coded is supported
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CTNEDecoderWrap::SupportsCodec(const TDesC8& aFormat, const TDesC8& aShortFormat)
{
    TBool supports = EFalse;
    TBuf8<256> mime;
    
    if (iDevVideoPlay)
    {
        RArray<TUid> decoders;
        
        TRAPD( status, iDevVideoPlay->FindDecodersL(aShortFormat, 0/*aPreProcType*/, decoders, EFalse/*aExactMatch*/) );

        if( ( status != KErrNone ) || ( decoders.Count() <= 0 ) )
        {
            PRINT((_L("CTNEDecoderWrap::SupportsCodec(), status[%d]"), status))
            supports = EFalse;
        }
        else
        {
            // Keep mime type given by codec fo future use
            iMimeType = aFormat;
            iShortMimeType = aShortFormat;
            supports = ETrue;
        }

        decoders.Reset();
        decoders.Close();
    }

    return supports;
}


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::SetDecoderParametersL
// Sets codec parameters
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::SetDecoderParametersL(TInt aDecoderLevel, const TTRVideoFormat& aFormat)
{
    PRINT((_L("CTNEDecoderWrap::SetDecoderParametersL(), In")))
    RArray<TUid> decoders;
    TBool codecAcceleration = EFalse;
    TInt status = KErrNone;
    iCodecLevel = aDecoderLevel;
    iFormat = aFormat;

	// Find the list of decoders 
    iDevVideoPlay->FindDecodersL(iShortMimeType, 0/*aPreProcType*/, decoders, EFalse/*aExactMatch*/);

    // Input format
    if (!iCompresedFormat)
    {
        TRAP(status, iCompresedFormat = CCompressedVideoFormat::NewL( iMimeType ));
        
        if (status != KErrNone)
        {
            // Reset decoder's list
            decoders.Reset();
            decoders.Close();
            User::Leave(status);
        }
    }

    // get decoder info
    for ( TInt i = 0; i < decoders.Count(); i ++ )
    {
        TRAP( status, codecAcceleration = CheckDecoderInfoL(decoders[i]) );

        if (status == KErrNone)
        {
            if (codecAcceleration != KTRAccelerationPriorityDecoder) 
            {
                // Use the first available software decoder
                iUid = decoders[i];

                // Keep the list of non-checked codecs for future use
                i = i + 1;
                for (; i < decoders.Count(); i ++)
                {
                    iCheckDecoders.Insert(decoders[i], iCheckDecoders.Count() );
                }

                break;
            }
            else
            {
                // Keep track of accelerated decoder to the list of "good" decoders
                iAcceleratedDecoders.Insert( decoders[i], iAcceleratedDecoders.Count() );
            }
        }
    }
        
    decoders.Reset();
    decoders.Close();
        
    if (iUid == TUid::Null())
    {
        // Check others "good"
        if ( iAcceleratedDecoders.Count() <= 0 )
        {
            PRINT((_L("CTNEDecoderWrap::SetDecoderParametersL(), No suitable decoders found")))
            User::Leave(KErrNotSupported);
        }
        else
        {
            // Select the accelerated as no ARM decoder is available
            iUid = iAcceleratedDecoders[0];
            iAcceleratedDecoders.Remove(0);
        }
    }

    PRINT((_L("CTNEDecoderWrap::SetDecoderParametersL(), Out")))
}

// -----------------------------------------------------------------------------
// CTNEDecoderWrap::CheckDecoderInfoL
// Checks coded info
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CTNEDecoderWrap::CheckDecoderInfoL(TUid aUid)
{
    CVideoDecoderInfo* decoderInfo = NULL; // Decoder info for retrieving capabilities
    TInt status = KErrNone;
    TBool accelerated = EFalse;

    // Check decoder
    PRINT((_L("CTNEDecoderWrap::CheckDecoderInfoL(), getting info from [0x%x]"), aUid.iUid ))
    decoderInfo = iDevVideoPlay->VideoDecoderInfoLC( aUid );

    if (!decoderInfo)
	{
        PRINT((_L("CTNEDecoderWrap::CheckDecoderInfoL(), getting info from [0x%x] failed[%d]"), aUid.iUid, status ))
        User::Leave(KErrNotSupported);
	}
    else 
	{
        // Check max rate for requested image format
        TSize maxSize = decoderInfo->MaxPictureSize();

        if ( (iFormat.iSize.iWidth > maxSize.iWidth) || (iFormat.iSize.iHeight > maxSize.iHeight) )
        {
            PRINT((_L("CTNEDecoderWrap::CheckDecoderInfoL(), Picture size is not supported")))
            status = KErrNotSupported;
        }
	}
        
    accelerated = decoderInfo->Accelerated();

    // Delete codec info
    CleanupStack::PopAndDestroy(decoderInfo);

    if (status != KErrNone)
    {
        User::Leave(status);
    }

    return accelerated;
}


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::InitializeL
// Initializes decoder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::InitializeL()
    {
    PRINT((_L("CTNEDecoderWrap::InitializeL(), In")))
    TUint maxBufferSize = 0;
    TInt status = KErrNone;

	iState = ETRInitializing;

    switch(iFormat.iDataType)
        {
        case ETRDuCodedPicture:
            {
            iDataUnitType = EDuCodedPicture;
            break;
            }

        case ETRDuVideoSegment:
            {
            iDataUnitType = EDuVideoSegment;
            break;
            }

        default:
            {
            // Should never happend. Decoder does not support uncompressed input format. 
            TRASSERT(0);
            }
        }

    iBufferOptions.iMinNumInputBuffers = KTRDecoderMinNumberOfBuffers;

    // Select decoder first
    this->SelectDecoderL();

    // Set now output format for this device    
    TRAP(status, iDevVideoPlay->SetOutputFormatL(iHwDeviceId, iUncompressedFormat));
    
    // 3. Buffer options
    iBufferOptions.iPreDecodeBufferSize = 0;            // "0" - use default decoder value
    iBufferOptions.iMaxPostDecodeBufferSize = 0;        // No limitations
    iBufferOptions.iPreDecoderBufferPeriod = 0;
    iBufferOptions.iPostDecoderBufferPeriod = 0;
    
    // Check max coded picture size for specified codec level
    switch(iCodecLevel)
        {
        case KTRH263CodecLevel10:
            {
            maxBufferSize = KTRMaxBufferSizeLevel10;
            break;
            }

        case KTRH263CodecLevel20:
            {
            maxBufferSize = KTRMaxBufferSizeLevel20;
            break;
            }

        case KTRH263CodecLevel30:
            {
            maxBufferSize = KTRMaxBufferSizeLevel30;
            break;
            }

        case KTRH263CodecLevel40:
            {
            maxBufferSize = KTRMaxBufferSizeLevel40;
            break;
            }

        case KTRH263CodecLevel50:
            {
            maxBufferSize = KTRMaxBufferSizeLevel50;
            break;
            }

        case KTRH263CodecLevel60:
            {
            maxBufferSize = KTRMaxBufferSizeLevel60;
            break;
            }

        case KTRH263CodecLevel70:
            {
            maxBufferSize = KTRMaxBufferSizeLevel70;
            break;
            }

        case KTRMPEG4CodecLevel0:
            {
            maxBufferSize = KTRMaxBufferSizeLevel0;
            break;
            }
            
        case KTRMPEG4CodecLevel1:
            {
            maxBufferSize = KTRMaxBufferSizeLevel1;
            break;
            }
            
        case KTRMPEG4CodecLevel2:
            {
            maxBufferSize = KTRMaxBufferSizeLevel2;
            break;
            }
            
        case KTRMPEG4CodecLevel3:
            {
            maxBufferSize = KTRMaxBufferSizeLevel3;
            break;
            }

        case KTRMPEG4CodecLevel0b:
            {
            maxBufferSize = KTRMaxBufferSizeLevel0b;
            break;
            }

        case KTRMPEG4CodecLevel4a:
            {
            maxBufferSize = KTRMaxBufferSizeLevel4a;
            break;
            }

        default:
            {
            maxBufferSize = KTRMaxBufferSizeLevel0;
            break;
            }
        }

    iBufferOptions.iMaxInputBufferSize = maxBufferSize;
    
    PRINT((_L("CTNEDecoderWrap::InitializeL(), InputBufferSize[%d], NumberOfBuffers[%d]"), 
               iBufferOptions.iMaxInputBufferSize, iBufferOptions.iMinNumInputBuffers ))

    iDevVideoPlay->SetBufferOptionsL(iBufferOptions);

    // Initialize devVideoPlay
    iDevVideoPlay->Initialize();

    PRINT((_L("CTNEDecoderWrap::InitializeL(), Out")))
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::SelectDecoderL
// Selects decoder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::SelectDecoderL()
{
    PRINT(( _L("CTNEDecoderWrap::SelectDecoderL(), In") ))
    TInt status = KErrNone;
    TBool accelerated = EFalse;
    TBool exit = EFalse;


    if (iUid != TUid::Null())
	{
        TRAP( status, iHwDeviceId = iDevVideoPlay->SelectDecoderL(iUid) );
	}
    else
	{
        // Most likely an error exists, if iUid == NULL; 
        status = KErrAlreadyExists;
	}

    while ( !exit )
	{
        if (status == KErrNone)
		{
            // To get Output format list devvideoplay requires to define output format first. 
            iDevVideoPlay->SetInputFormatL(iHwDeviceId, *iCompresedFormat, iDataUnitType, EDuElementaryStream, ETrue);

            // It's time to check input format support (since the plugin is loaded to the memory)
            iUncompressedFormat.iDataFormat = EYuvRawData;
            
            TUncompressedVideoFormat uncFormat;
            TBool found = EFalse;
            TInt pattern1, pattern2;
                    
            pattern1 = EYuv420Chroma1;
            pattern2 = EYuv420Chroma2;
                
            RArray<TUncompressedVideoFormat> supportedOutputFormats; 
            TRAP(status, iDevVideoPlay->GetOutputFormatListL( iHwDeviceId, supportedOutputFormats ));
            
            TInt formatCount = 0;
            if (status == KErrNone)
            {
                formatCount = supportedOutputFormats.Count();
                PRINT((_L("CTNEDecoderWrap::InitializeL(), formatCount[%d]"), formatCount ))
            }
                
            if (formatCount <= 0)
            {
                supportedOutputFormats.Close();
                status = KErrAlreadyExists;
                PRINT((_L("CTNEDecoderWrap::InitializeL(), There are no supported output formats") ))
                //User::Leave(KErrNotSupported);
            }
            else
            {
                // Check the most important paramers
                for ( TInt i = 0; i < formatCount; i ++ )
                {
                    uncFormat = supportedOutputFormats[i];
                    PRINT((_L("CTNEDecoderWrap::InitializeL(), pattern[%d]"), uncFormat.iYuvFormat.iPattern ))
                    
                    if ( (uncFormat.iDataFormat == iUncompressedFormat.iDataFormat) && 
                         ( (uncFormat.iYuvFormat.iPattern == pattern1) || 
                           (uncFormat.iYuvFormat.iPattern == pattern2) ) )
                        {
                        // Assign the rest of parameters
                        iUncompressedFormat = uncFormat;
                        found = ETrue;
                        exit = ETrue;
                        supportedOutputFormats.Close();
                        break;
                        }
                }

                if (!found)
                {
                    supportedOutputFormats.Close();
                    PRINT((_L("CTNEDecoderWrap::InitializeL(), Supported format is not found") ))
                    //User::Leave(KErrNotSupported);
                }
            }
        }
        else
        {
            // Take another codec and try again
            // Reset Uid
            iUid = TUid::Null();

            // Check codecs
            for ( TInt i = 0; i < iCheckDecoders.Count(); i ++ )
            {
            	TRAP( status, accelerated = CheckDecoderInfoL(iCheckDecoders[i]) );

            	if (status == KErrNone)
                {
                	if (accelerated != KTRAccelerationPriorityDecoder) // The rule is defined based on study
                	{
                     	// pick the first ARM decoder
                    	iUid = iCheckDecoders[i];
                    	iCheckDecoders.Remove(i);
                            
                     	// Keep the list of non-checked codecs for future use
                     	// nothing to do, they are already are in the list
                    	break;
                	}
                 	else
                	{
                    	// its a DSP based decoder add it to the accelerated decoder array 
                    	iAcceleratedDecoders.Insert( iAcceleratedDecoders[i], iAcceleratedDecoders.Count() );
                    }
                }
            }
                
            if ( iUid == TUid::Null() )
            {
                // There are no ARM decoders in the system, use the accelerated ones
                if (iAcceleratedDecoders.Count() > 0)
                {
                    // decoders were already checked, take the first from the list
                    iUid = iAcceleratedDecoders[0];
                    iAcceleratedDecoders.Remove(0);
                }
                else
                {
                    PRINT((_L("CTNEDecoderWrap::SelectDecoderL(), No suitable accelerated decoders found")))
                    User::Leave(KErrNotSupported);
                }
            }
                
            status = KErrNone;
            TRAP(status, iHwDeviceId = iDevVideoPlay->SelectDecoderL(iUid));
        }
    }

    PRINT((_L("CTNEDecoderWrap::SelectDecoderL(), Out")))
}

// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoInitComplete
// Notifies for initialization complete with init status
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoInitComplete(TInt /*aError*/)
    {
		iState = ETRInitialized;
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::StartL
// Starts decoding
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::StartL()
    {
    PRINT((_L("CTNEDecoderWrap::StartL(), In")))

    // Start decoding
    if (iFatalError == KErrNone)
        {
        iDevVideoPlay->Start();
        }

    if (!iInputBuffer)
        {
        // Get buffer from the decoder to fill
        iInputBuffer = iDevVideoPlay->GetBufferL(iBufferOptions.iMaxInputBufferSize);
        }
    
    // Reset iStop    
    iStop = EFalse;
    iState = ETRRunning;
    
    // Reset ts monitor
    iLastTimestamp = -1;

    PRINT((_L("CTNEDecoderWrap::StartL(), Out")))
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoNewBuffers()
// New buffers are available
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoNewBuffers()
    {
    TInt status = KErrNone;


    if (iStop)
        {
        PRINT((_L("CTNEDecoderWrap::MdvpoNewBuffers(), Stop was already called, nothing to do")))
        return;
        }

    // One or more new empty input buffers are available
    if (!iInputBuffer)
        {
        // Get buffer from the decoder to fill
        TRAP(status, iInputBuffer = iDevVideoPlay->GetBufferL(iBufferOptions.iMaxInputBufferSize));
        
        if (status != KErrNone)
            {
            PRINT((_L("CTNEDecoderWrap::MdvpoNewBuffers(), GetBufferL status[%d]"), status))
            iObserver->MNotifyThumbnailReady(status);
            return;
            }

        if (!iInputBuffer)
            {
            PRINT((_L("CTNEDecoderWrap::MdvpoNewBuffers(), There are available buffer, but decoder returned NULL")))
            return;
            }
        }

    if (iCodedBuffer)
        {
        // Send coded buffer, since the client has already done request
        TRAP(status, this->SendBufferL(iCodedBuffer));

        // Reset buffer
        iCodedBuffer = NULL;

        if (status != KErrNone)
            {
            PRINT((_L("CTNEDecoderWrap::MdvpoNewBuffers(), Send buffer error[%d]"), status))
            iObserver->MNotifyThumbnailReady(status);
            return;
            }
        }
        
    if ((iInputBuffer) && (iState == ETRRunning))
    	{
			// If InputBuffer is available send 
			// more encoded packets to the decoder
            iObserver->MSendEncodedBuffer();
    	}
    
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::WriteCodedBufferL
// Writes coded data to decoder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::WriteCodedBufferL(TVideoBuffer* aBuffer)
    {
    PRINT((_L("CTNEDecoderWrap::WriteCodedBufferL(), In")))
    TVideoBuffer::TBufferType bufferType;
    
    if (!aBuffer)
        {
        PRINT((_L("CTNEDecoderWrap::WriteCodedBufferL(), Input buffer is invalid, Leave")))
        User::Leave(KErrArgument);
        }

    if (iFatalError != KErrNone)
        {
        PRINT((_L("CTNEDecoderWrap::WriteCodedBufferL(), FatalError was reported by decoder")))
        
        // Return coded buffer
        iObserver->MReturnCodedBuffer(aBuffer);
        return;
        }
    
    TTimeIntervalMicroSeconds ts = aBuffer->TimeStamp();
        
    if ( ts <= iLastTimestamp)
        {
        // Prevent propagation of the error now
        PRINT((_L("CTNEDecoderWrap::WriteCodedBufferL(), Client sends invalid data (ts field), Leave")))
        User::Leave(KErrArgument);
        }
    else
        {
        iLastTimestamp = ts;
        }
    
    if (aBuffer->BufferSize() <= 0)
        {
        PRINT((_L("CTNEDecoderWrap::WriteCodedBufferL(), Input data buffer is invalid (empty), Leave")))
        User::Leave(KErrArgument);
        }
        
    bufferType = aBuffer->Type();
        
    if ( ( bufferType != TVideoBuffer::EVideoH263 ) && 
         ( bufferType != TVideoBuffer::EVideoMPEG4 ) )
        {
        PRINT((_L("CTNEDecoderWrap::WriteCodedBufferL(), [%d] This data type is not supported, Leave"), aBuffer->Type() ))
        User::Leave(KErrNotSupported);
        }

    if (!iInputBuffer)
        {
        // Request new empty buffer
        iInputBuffer = iDevVideoPlay->GetBufferL(iBufferOptions.iMaxInputBufferSize);
        }

    if (iInputBuffer)
        {
        this->SendBufferL(aBuffer);
        }
    else
        {
        iCodedBuffer = aBuffer;
        }

    PRINT((_L("CTNEDecoderWrap::WriteCodedBufferL(), Out")))
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::SendBufferL
// Sends buffer to decoder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::SendBufferL(TVideoBuffer* aBuffer)
    {
    PRINT((_L("CTNEDecoderWrap::SendBufferL(), In")))

    PRINT((_L("CTNEDecoderWrap::SendBufferL(), iInputBuffer[%d], aBuffer[%d]"), iInputBuffer->iData.MaxLength(), 
               aBuffer->BufferSize() ))

    if ( iInputBuffer->iData.MaxLength() < aBuffer->BufferSize() )
        {
        PRINT((_L("CTNEDecoderWrap::SendBufferL(), buffer length exceeds max length")))
        User::Leave(KErrOverflow);
        }

    iInputBuffer->iData.Copy( aBuffer->Data().Ptr(), aBuffer->BufferSize() );
    iInputBuffer->iData.SetLength( aBuffer->BufferSize() );

    // Data unit presentation timestamp. Valid if EPresentationTimestamp is set in the options. 
    // If the input bitstream does not contain timestamp information, this field should be valid, 
    // otherwise pictures cannot be displayed at the correct time. If the input bitstream contains 
    // timestamp information (such as the TR syntax element of H.263 bitstreams) and valid 
    // iPresentationTimestamp is provided, the value of iPresentationTimestamp is used in playback.    
    iInputBuffer->iOptions = TVideoInputBuffer::EPresentationTimestamp;
    iInputBuffer->iPresentationTimestamp = aBuffer->TimeStamp();
 
 // @@ HARI AVC
    iInputBuffer->iSequenceNumber = iInputFrameNum;
    iInputFrameNum++;
    iInputBuffer->iDecodingTimestamp = aBuffer->TimeStamp();
    iInputBuffer->iPresentationTimestamp  = TTimeIntervalMicroSeconds(iClockSource->Time().Int64() + 1000000);

 
    /*Other data: TBC*/

    // Write data to decoder
    iDevVideoPlay->WriteCodedDataL(iInputBuffer);

    // Reset InputBuffer ptr
    iInputBuffer = NULL;

    // FIXME return buffer only after it's writtent to decoder (client could write next buffer synchronously from observer call)
    // Return buffer to the client immediately after copying
    iObserver->MReturnCodedBuffer(aBuffer);

    PRINT((_L("CTNEDecoderWrap::SendBufferL(), Out")))
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoNewPictures
// New decoded pictures available from decoder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoNewPictures()
    {
    TInt status = KErrNone;


    // 1 or more decoded pictures are available
    if (!iDecodedPicture)
        {
        // Get new picture
        TRAP(status, iDecodedPicture = iDevVideoPlay->NextPictureL());

        if (status != KErrNone)
            {
            PRINT((_L("CTNEDecoderWrap::MdvpoNewPictures(), NextPicture status[%d]"), status))
            iObserver->MNotifyThumbnailReady(status);
            return;
            }

        if (!iDecodedPicture)
            {
            // Error: DevVideo notified of new buffers, but returns NULL
            PRINT((_L("CTNEDecoderWrap::MdvpoNewPictures(), DevVideo notified of new buffers, but returns NULL")))
            iObserver->MNotifyThumbnailReady(KErrAlreadyExists);
            return;
            }

        // Send new picture to the client
        iObserver->MPictureFromDecoder(iDecodedPicture);
        }
    else
        {
        // Previous picture still was not returned by the client, nothing to do. 
        // FIXME SetActive();
        }
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::ReturnPicture
// Returns picture 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::ReturnPicture(TVideoPicture* aPicture)
    {
    PRINT((_L("CTNEDecoderWrap::ReturnPicture(), In")))
    TInt status = KErrNone;


    iDevVideoPlay->ReturnPicture(aPicture);

    // Reset decoded picture
    iDecodedPicture = NULL;

    TRAP(status, iDecodedPicture = iDevVideoPlay->NextPictureL());

    if (status != KErrNone)
        {
        PRINT((_L("CTNEDecoderWrap::ReturnPicture(), NextPicture status[%d]"), status))
        iObserver->MNotifyThumbnailReady(status);
        return;
        }

    if (iDecodedPicture)
        {
        // Send new picture to the client
        iObserver->MPictureFromDecoder(iDecodedPicture);
        }

    PRINT((_L("CTNEDecoderWrap::ReturnPicture(), Out")))
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::StopL
// Stops decoding synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::StopL()
    {
    PRINT((_L("CTNEDecoderWrap::StopL(), In")))

    if (iFatalError == KErrNone)
        {
        iDevVideoPlay->Stop();
        }
        
    iStop = ETrue;
    iState = ETRStopped;
        
    PRINT((_L("CTNEDecoderWrap::StopL(), Out")))
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::AsyncStopL
// Stops decoding asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::AsyncStopL()
    {
    PRINT((_L("CTNEDecoderWrap::StopL(), Async In")))

    if (iFatalError == KErrNone)
        {
        iDevVideoPlay->InputEnd();
        }
        
    iStop = ETrue;
    iState = ETRStopped;

    PRINT((_L("CTNEDecoderWrap::StopL(), Async Out")))
    }

// -----------------------------------------------------------------------------
// CTNEDecoderWrap::GetNumInputFreeBuffers
// returns the number of buffers that can be sent to 
// decoder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint CTNEDecoderWrap::GetNumInputFreeBuffers()
{
	TUint inputFreeBuffers = iDevVideoPlay->NumFreeBuffers();
	
	return inputFreeBuffers;
}


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoStreamEnd
// Indicates when stream end is reached
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoStreamEnd()
    {
    PRINT((_L("CTNEDecoderWrap::MdvpoStreamEnd()")))    
    }

// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoReturnPicture
// Returns a used input video picture back to the caller. The picture memory can be re-used or freed (only relevant to postprocessor)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoReturnPicture(TVideoPicture* /*aPicture*/)
    {
    PRINT((_L("CTNEDecoderWrap::MdvpoReturnPicture()")))
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoSupplementalInformation
// Sends SupplementalInformation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoSupplementalInformation(const TDesC8& /*aData*/, 
                                                         const TTimeIntervalMicroSeconds& /*aTimestamp*/, 
                                                         const TPictureId& /*aPictureId*/)
    {
    PRINT((_L("CTNEDecoderWrap::MdvpoSupplementalInformation()")))
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoPictureLoss
// Back channel information from the decoder, indicating a picture loss without specifying the lost picture
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoPictureLoss()
    {
    PRINT((_L("CTNEDecoderWrap::MdvpoPictureLoss(), report an error")))
    iObserver->MNotifyThumbnailReady(KErrAbort);
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoPictureLoss
// Back channel information from the decoder, indicating the pictures that have been lost
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoPictureLoss(const TArray< TPictureId >& /*aPictures*/)
    {
    PRINT((_L("CTNEDecoderWrap::MdvpoPictureLoss(), pictureId: report an error")))
    iObserver->MNotifyThumbnailReady(KErrAbort);
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoSliceLoss
// Reports that slice is lost
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoSliceLoss(TUint /*aFirstMacroblock*/, TUint /*aNumMacroblocks*/, 
                                           const TPictureId& /*aPicture*/)
    {
    PRINT((_L("CTNEDecoderWrap::MdvpoSliceLoss()")))
    // This error is not considered a s fatal for decoder or application, nothing to do
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoReferencePictureSelection
// Back channel information from the decoder, indicating a reference picture selection request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoReferencePictureSelection(const TDesC8& /*aSelectionData*/)
    {
    PRINT((_L("CTNEDecoderWrap::MdvpoReferencePictureSelection()")))
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoTimedSnapshotComplete
// Called when a timed snapshot request has been completed. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoTimedSnapshotComplete(TInt /*aError*/, TPictureData* /*aPictureData*/, 
                                                       const TTimeIntervalMicroSeconds& /*aPresentationTimestamp*/, 
                                                       const TPictureId& /*aPictureId*/)
    {
    PRINT((_L("CTNEDecoderWrap::MdvpoTimedSnapshotComplete()")))
    }


// -----------------------------------------------------------------------------
// CTNEDecoderWrap::MdvpoFatalError
// Reports the fatal error to the client
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEDecoderWrap::MdvpoFatalError(TInt aError)
    {
    PRINT((_L("CTNEDecoderWrap::MdvpoFatalError(), error[%d]"), aError))
    iFatalError = aError;
    iObserver->MNotifyThumbnailReady(iFatalError);
    }




// End of file
