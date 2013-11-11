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
*                of the AAC record controller encoder.
*
*/



// INCLUDE FILES
#include	"3GPAACAudioRecordControllerEncoder.h"
#include 	<mmfcodec.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
C3GPAACAudioRecordControllerEncoder::C3GPAACAudioRecordControllerEncoder()
    :   CAdvancedAudioEncoder(CActive::EPriorityStandard),
        iSoftCodec(NULL),
        iBitRate(0)
   {
#ifdef _DEBUG
    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::C3GPAACAudioRecordControllerEncoder()\n"));
#endif
    }

// -----------------------------------------------------------------------------
// C3GPAACAudioRecordControllerEncoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3GPAACAudioRecordControllerEncoder::ConstructL(
	const TFourCC& aSinkDatatype )
    {
#ifdef _DEBUG
    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::ConstructL(1)\n"));
#endif
	iSoftCodec = CMMFCodec::NewL(TFourCC(' ','P','1','6'), aSinkDatatype);

#ifdef _DEBUG
    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::ConstructL(2)\n"));
#endif
	iDataType.Set(aSinkDatatype);

   	//User::LeaveIfError(iFs.Connect());
    //_LIT(KBufferFileName, "C:\\audio.raw");
    //iOutputFile.Replace(iFs, KBufferFileName, EFileWrite);

    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
C3GPAACAudioRecordControllerEncoder* C3GPAACAudioRecordControllerEncoder::NewL(
	const TFourCC& aSinkDatatype )
    {
#ifdef _DEBUG
    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::NewL()\n"));
#endif
    C3GPAACAudioRecordControllerEncoder* self = new(ELeave) C3GPAACAudioRecordControllerEncoder();
    CleanupStack::PushL(self);
    self->ConstructL(aSinkDatatype);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
C3GPAACAudioRecordControllerEncoder::~C3GPAACAudioRecordControllerEncoder()
	{
#ifdef _DEBUG
    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::~C3GPAACAudioRecordControllerEncoder()"));
#endif
	delete iSoftCodec;

	//iOutputFile.Close();
	//iFs.Close();
	}

// -----------------------------------------------------------------------------
// C3GPAACAudioRecordControllerEncoder::SetConfigL
// Sets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
void C3GPAACAudioRecordControllerEncoder::SetConfigL(
	TUint aSampleRate,
	TUint /*aChannels*/,
	TUint /*aSWConvertSampleRate*/,
	TUint /*aSWConvertChannels*/,
	const RArray<TInt>& aCodecConfigData)
	{
#ifdef _DEBUG
    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::SetConfigL"));
#endif

	iSharedBufferIndex = 0;
	NextSharedBufferL();
    iNextBuffer->SetPosition(0);

	RArray<TInt>& codecArrayConfigData = CONST_CAST(RArray<TInt>&, aCodecConfigData);
	if ( iBitRate )
		{
		codecArrayConfigData[0] = iBitRate;
		}

	codecArrayConfigData[1] = aSampleRate;
	TDesC8& codecConfigData = REINTERPRET_CAST(TDesC8&, codecArrayConfigData);
	iSoftCodec->ConfigureL(KUidConfig, codecConfigData);
	}


// -----------------------------------------------------------------------------
// C3GPAACAudioRecordControllerEncoder::SetBitRateL
// Sets the bitrate to the codec.
// -----------------------------------------------------------------------------
//
void C3GPAACAudioRecordControllerEncoder::SetBitRateL(
	TUint aBitRate,
	const RArray<TInt>& /*aCodecConfigData*/ )
	{
#ifdef _DEBUG
    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::SetBitRateL"));
#endif
	iBitRate = aBitRate;
	}


// -----------------------------------------------------------------------------
// C3GPAACAudioRecordControllerEncoder::ResetL
// Resets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
void C3GPAACAudioRecordControllerEncoder::ResetL()
	{
#ifdef _DEBUG
    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::ResetL"));
#endif
	iSoftCodec->ResetL();
	}

// -----------------------------------------------------------------------------
// C3GPAACAudioRecordControllerEncoder::IsHwAccelerated
// Always return true since soft codec is used.
// -----------------------------------------------------------------------------
//
TBool C3GPAACAudioRecordControllerEncoder::IsHwAccelerated()
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
void C3GPAACAudioRecordControllerEncoder::HandleEmptyBufferL()
	{
#ifdef _DEBUG
//    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::HandleEmptyBufferL aBuffer[%x]"), iBufferToEmpty);
#endif

    TCodecProcessResult result;
	TBool moreProcessing = ETrue;

	//iOutputFile.Write(STATIC_CAST(CMMFDataBuffer*, iBufferToEmpty)->Data());

	iBufferToEmpty->SetPosition(0);

	while ( moreProcessing )
		{
        result = iSoftCodec->ProcessL(*iBufferToEmpty, *iNextBuffer);

//#ifdef _DEBUG
//    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::HandleEmptyBufferL - [%d] :: [%d] [%d] :: [%d] [%d]"),
//		result.iStatus,
//		iNextBuffer->Position(), STATIC_CAST(CMMFDataBuffer*, iNextBuffer)->Data().Length(),
//		iBufferToEmpty->Position(), STATIC_CAST(CMMFDataBuffer*, iBufferToEmpty)->Data().Length());
//#endif

		switch ( result.iStatus )
			{
			case TCodecProcessResult::EProcessComplete:
				// src has been fully consumed and dst is full
			case TCodecProcessResult::EDstNotFilled:
				// src has been fully consumed, but there is still room in dst
				moreProcessing = EFalse;
				if ( iBufferToEmpty->LastBuffer() )
					{
//#ifdef _DEBUG
//				    RDebug::Print(_L("C3GPAACAudioRecordControllerEncoder::HandleEmptyBufferL - Last buffer"));
//#endif					
					iNextBuffer->SetLastBuffer(ETrue);
					}
				iObserver->EmptyBuffer(iNextBuffer);
				NextSharedBufferL();
				iNextBuffer->SetPosition(0);
				break;

			case TCodecProcessResult::EProcessIncomplete:
				// src has not been consumed, but dst is full
				iBufferToEmpty->SetPosition(result.iSrcBytesProcessed + iBufferToEmpty->Position());
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

