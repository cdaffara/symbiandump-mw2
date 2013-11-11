/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The functions in this module implements the specific behaviors
*                of the AMR record controller encoder.
*
*/



// INCLUDE FILES
#include	"AMRAudioRecordControllerEncoder.h"
#include 	<mmfcodec.h>

// CONSTANTS
// AMR Bit Rate constant
const TUint KAmrBitRate4750		= 4750;
const TUint KAmrBitRate5150		= 5150;
const TUint KAmrBitRate5900     = 5900;
const TUint KAmrBitRate6700     = 6700;
const TUint KAmrBitRate7400		= 7400;
const TUint KAmrBitRate7950     = 7950;
const TUint KAmrBitRate10200    = 10200;
const TUint KAmrBitRate12200	= 12200;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAMRAudioRecordControllerEncoder::CAMRAudioRecordControllerEncoder()
    :   CAdvancedAudioEncoder(CActive::EPriorityStandard),
        iSoftCodec(NULL)
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::CAMRAudioRecordControllerEncoder()\n"));
#endif
    }

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerEncoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerEncoder::ConstructL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::ConstructL()\n"));
#endif
	iSoftCodec = CMMFCodec::NewL(TFourCC(' ','P','1','6'), TFourCC(' ','A','M','R'));
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAMRAudioRecordControllerEncoder* CAMRAudioRecordControllerEncoder::NewL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::NewL()\n"));
#endif
    CAMRAudioRecordControllerEncoder* self = new(ELeave) CAMRAudioRecordControllerEncoder();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CAMRAudioRecordControllerEncoder::~CAMRAudioRecordControllerEncoder()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::~CAMRAudioRecordControllerEncoder()"));
#endif
	delete iSoftCodec;
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerEncoder::SetConfigL
// Sets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerEncoder::SetConfigL(
	TUint /*aSampleRate*/,
	TUint /*aChannels*/,
	TUint /*aSWConvertSampleRate*/,
	TUint /*aSWConvertChannels*/,
	const RArray<TInt>& aCodecConfigData )
	{
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::SetConfigL"));
#endif

	iSharedBufferIndex = 0;
	NextSharedBufferL();
    iNextBuffer->SetPosition(0);

	RArray<TInt>& codecArrayConfigData = CONST_CAST(RArray<TInt>&, aCodecConfigData);
	TDesC8& codecConfigData = REINTERPRET_CAST(TDesC8&, codecArrayConfigData[0]);

	if ( iBitRate )
		{
		switch ( iBitRate )
			{  // supported soft codec bit rate
			case KAmrBitRate4750:
				codecArrayConfigData[0] = 0; // Mode 0 - 4750 bps
				break;
			case KAmrBitRate5150:
				codecArrayConfigData[0] = 1; // Mode 1 - 5150 bps
				break;
			case KAmrBitRate5900:
				codecArrayConfigData[0] = 2; // Mode 2 - 5900 bps
				break;
			case KAmrBitRate6700:
				codecArrayConfigData[0] = 3; // Mode 3 - 6700 bps
				break;
			case KAmrBitRate7400:
				codecArrayConfigData[0] = 4; // Mode 4 - 7400 bps
				break;
			case KAmrBitRate7950:
				codecArrayConfigData[0] = 5; // Mode 5 - 7950 bps
				break;
			case KAmrBitRate10200:
				codecArrayConfigData[0] = 6; // Mode 6 - 10200 bps
				break;
			case KAmrBitRate12200:
				codecArrayConfigData[0] = 7; // Mode 7 - 12200 bps
				break;
			default:
				User::Leave(KErrNotSupported);
				break;
			}
		}

	iSoftCodec->ConfigureL(KUidConfig, codecConfigData);
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerEncoder::SetBitRateL
// Sets the bitrate to the codec.
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerEncoder::SetBitRateL(
	TUint aBitRate,
	const RArray<TInt>& /*aCodecConfigData*/ )
	{
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::SetBitRateL"));
#endif
	switch ( aBitRate )
		{  // supported soft codec bit rate
		case KAmrBitRate4750:
		case KAmrBitRate5150:
		case KAmrBitRate5900:
		case KAmrBitRate6700:
		case KAmrBitRate7400:
		case KAmrBitRate7950:
		case KAmrBitRate10200:
		case KAmrBitRate12200:
			iBitRate = aBitRate;
			break;
		default:
			User::Leave(KErrNotSupported);
			break;
		}
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerEncoder::ResetL
// Resets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerEncoder::ResetL()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::ResetL"));
#endif
	iSoftCodec->ResetL();
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerEncoder::IsHwAccelerated
// Always return true since soft codec is used.
// -----------------------------------------------------------------------------
//
TBool CAMRAudioRecordControllerEncoder::IsHwAccelerated()
	{
	return EFalse;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioEncoder::HandleFillBufferL
// Handle the request to empty the specified buffer. The specified buffer is
// received in EmptyBuffer().
//
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerEncoder::HandleEmptyBufferL()
	{
#ifdef _DEBUG
//    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::HandleEmptyBufferL aBuffer[%x]"), iBufferToEmpty);
#endif

    TCodecProcessResult result;
	TBool moreProcessing = ETrue;

	iBufferToEmpty->SetPosition(0);

	while ( moreProcessing )
		{
        result = iSoftCodec->ProcessL(*iBufferToEmpty, *iNextBuffer);
//		result.iStatus = TCodecProcessResult::EProcessComplete;
		iNextBuffer->SetStatus(EBeingFilled);

#ifdef _DEBUG
//    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::HandleEmptyBufferL - [%d] :: [%d] [%d] :: [%d] [%d]"),
//		result.iStatus,
//		iNextBuffer->Position(), STATIC_CAST(CMMFDataBuffer*, iNextBuffer)->Data().Length(),
//		iBufferToEmpty->Position(), STATIC_CAST(CMMFDataBuffer*, iBufferToEmpty)->Data().Length());
#endif

		switch ( result.iStatus )
			{
			case TCodecProcessResult::EProcessComplete:
			//    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::HandleEmptyBufferL aBuffer[%x]--EPRocessComplete"), iBufferToEmpty);
			                    moreProcessing = EFalse;
			                    if ( iBufferToEmpty->LastBuffer() )
			                    {
			                       iNextBuffer->SetLastBuffer(ETrue);
			                    }
			                   iNextBuffer->SetStatus(EFull);
			                    iObserver->EmptyBuffer(iNextBuffer);
			                    NextSharedBufferL();
			                    iNextBuffer->SetPosition(0);
			                    break;
				// src has been fully consumed and dst is full
			case TCodecProcessResult::EDstNotFilled:
				
			    // src has been fully consumed, but there is still room in dst
			  //   RDebug::Print(_L("CAMRAudioRecordControllerEncoder::HandleEmptyBufferL aBuffer[%x]--EDstNotFilled"), iBufferToEmpty);
			                    moreProcessing = EFalse;
			                    if ( iBufferToEmpty->LastBuffer() )
			                    {
			                         iNextBuffer->SetLastBuffer(ETrue);
			                    }
			                    iNextBuffer->SetStatus(EFull);
			                    iObserver->EmptyBuffer(iNextBuffer);
			                    NextSharedBufferL();
			                    iNextBuffer->SetPosition(0);
			                    break;
			case TCodecProcessResult::EProcessIncomplete:
				// src has not been consumed, but dst is full
		        iNextBuffer->SetStatus(EFull);
				iObserver->EmptyBuffer(iNextBuffer);
				NextSharedBufferL();
				iNextBuffer->SetPosition(0);
				break;

			default:
				break;
			}
		}

#ifdef __SEND_RAW_DATA__
	TDes8& bufferDes = STATIC_CAST(CMMFDataBuffer*, iBufferToEmpty)->Data();
	TDes8& nextBufferDes = STATIC_CAST(CMMFDataBuffer*, flushBuffer)->Data();
	TInt   copyLength = STATIC_CAST(CMMFDataBuffer*, iBufferToEmpty)->Data().Length();
	nextBufferDes.Copy (bufferDes.Ptr(), copyLength);
#endif

    iState = EIdle;
    if ( !iBufferToEmpty->LastBuffer() )
        {
        iObserver->BufferEmptied();
        }
	}

// End of File

