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
*                of the AMR record controller hardware accelerated encoder
*
*/



// INCLUDE FILES
#include	"AMRAudioRecordControllerEncoder.h"
#include 	<mmfpaniccodes.h>
#include	<SpeechEncoderConfig.h> // Custom Interface for Speech Encoder Configuration

// AMR Bit Rate constant
const TUint KAmrBitRate4750		= 4750;
const TUint KAmrBitRate5150		= 5150;
const TUint KAmrBitRate5900		= 5900;
const TUint KAmrBitRate6700		= 6700;
const TUint KAmrBitRate7400		= 7400;
const TUint KAmrBitRate7950		= 7950;
const TUint KAmrBitRate10200	= 10200;
const TUint KAmrBitRate12200	= 12200;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAMRAudioRecordControllerEncoder::CAMRAudioRecordControllerEncoder()
    :   CAdvancedAudioEncoder(CActive::EPriorityStandard)
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::CAMRAudioRecordControllerEncoder - Hardware Accelerated"));
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
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::ConstructL"));
#endif
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
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::NewL"));
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
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::~CAMRAudioRecordControllerEncoder"));
#endif
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

	if ( !iMMFDevSound )
		{
    	_LIT(KAudioRecordControllerEncoderPanicCategory, "AMRAudioRecordControllerEncoder");
    	User::Panic(KAudioRecordControllerEncoderPanicCategory, EMMFAudioOutputDevSoundNotLoaded);
		}

	iSharedBufferIndex = 0;
	NextSharedBufferL();
    iNextBuffer->SetPosition(0);
    iNextBuffer->SetLastBuffer(EFalse);

    TInt amrBitRate;
    if (iBitRate)
        {
        // Client Passes in Actual Bitrates    
        amrBitRate = iBitRate;
	    }
	else
	    {
	    // This Config Bitrates is a Mode, 
	    //so we need to convert is to BitRate below
	    amrBitRate = aCodecConfigData[0];
	    }

#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::SetConfigL with amrBitRate = %D"),amrBitRate);
#endif

    switch ( amrBitRate )	// BitRate or Mode
	    {
   		case KAmrBitRate4750:
   		case 0: // Mode 0 - 4750 bps
    		amrBitRate = KAmrBitRate4750;
	    	break;
		case KAmrBitRate5150:
		case 1: // Mode 1 - 5150 bps
			amrBitRate = KAmrBitRate5150;
			break;
		case KAmrBitRate5900:
		case 2: // Mode 2 - 5900 bps
			amrBitRate = KAmrBitRate5900;
			break;
		case KAmrBitRate6700:
		case 3: // Mode 3 - 6700 bps
			amrBitRate = KAmrBitRate6700;
			break;
		case KAmrBitRate7400:
		case 4: // Mode 4 - 7400 bps
			amrBitRate = KAmrBitRate7400;
			break;
		case KAmrBitRate7950:
		case 5: // Mode 5 - 7950 bps
			amrBitRate = KAmrBitRate7950;
			break;
		case KAmrBitRate10200:
		case 6: // Mode 6 - 10200 bps
			amrBitRate = KAmrBitRate10200;
			break;
		case KAmrBitRate12200:
		case 7: // Mode 7 - 12200 bps
			amrBitRate = KAmrBitRate12200;
			break;
		default:
			User::Leave(KErrNotSupported);
			break;
		}


	CSpeechEncoderConfig* speechEncIntfc(NULL);// Creates a custom interface
	speechEncIntfc = CSpeechEncoderConfig::NewL(*iMMFDevSound); //Calls the custom on devsound Devsound will return the handle to the custom interface proxy
	TInt stat = speechEncIntfc->SetBitrate(amrBitRate); //Set Bit Rate by custom command
#ifdef _DEBUG
	RDebug::Print(_L("CAMRAudioRecordControllerEncoder::SetConfigL SetBitrate status = [%d]"),stat);
#endif

	delete speechEncIntfc; // We have the handle so we Delete the Custom Interface and Adptation should Not delete this

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
	if ( !iMMFDevSound )
		{
    	_LIT(KAudioRecordControllerEncoderPanicCategory, "AMRAudioRecordControllerEncoder");
    	User::Panic(KAudioRecordControllerEncoderPanicCategory, EMMFAudioOutputDevSoundNotLoaded);
		}

	TInt amrBitRate = KAmrBitRate4750;
	switch ( aBitRate )
		{
		case KAmrBitRate4750:
			amrBitRate = KAmrBitRate4750;
			break;
		case KAmrBitRate5150:
			amrBitRate = KAmrBitRate5150;
			break;
		case KAmrBitRate5900:
			amrBitRate = KAmrBitRate5900;
			break;
		case KAmrBitRate6700:
			amrBitRate = KAmrBitRate6700;
			break;
		case KAmrBitRate7400:
			amrBitRate = KAmrBitRate7400;
			break;
		case KAmrBitRate7950:
			amrBitRate = KAmrBitRate7950;
			break;
		case KAmrBitRate10200:
			amrBitRate = KAmrBitRate10200;
			break;
		case KAmrBitRate12200:
			amrBitRate = KAmrBitRate12200;
			break;
		default:
			User::Leave(KErrNotSupported);
			break;
		}

	CSpeechEncoderConfig* speechEncIntfc(NULL);
	speechEncIntfc = CSpeechEncoderConfig::NewL(*iMMFDevSound);
	TInt stat = speechEncIntfc->SetBitrate(amrBitRate);
	iBitRate = aBitRate;
#ifdef _DEBUG
	RDebug::Print(_L("CAMRAudioRecordControllerEncoder::SetBitRateL SetBitrate status = [%d]"),stat);
#endif

	delete speechEncIntfc;
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerEncoder::ResetL
// Resets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerEncoder::ResetL()
	{
//#ifdef _DEBUG
//    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::ResetL"));
//#endif
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerEncoder::IsHwAccelerated
// Always return true since soft codec is used.
// -----------------------------------------------------------------------------
//
TBool CAMRAudioRecordControllerEncoder::IsHwAccelerated()
	{
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerEncoder::HandleFillBufferL
// Handle the request to empty the specified buffer. The specified buffer is
// received in EmptyBuffer().
//
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerEncoder::HandleEmptyBufferL()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerEncoder::HandleEmptyBufferL iBufferToEmpty[%x]"),
    	iBufferToEmpty);
#endif

	TNextEvent nextEvent = EDoNothing;
	iNextBuffer->SetLastBuffer(EFalse); //Reset
	CMMFBuffer* flushBuffer = iNextBuffer;

	iBufferToEmpty->SetPosition(0);

	TDes8& srcBufferDes = STATIC_CAST(CMMFDataBuffer*, iBufferToEmpty)->Data();
	TDes8& destBufferDes = STATIC_CAST(CMMFDataBuffer*, iNextBuffer)->Data();
	TInt srcLength = STATIC_CAST(CMMFDataBuffer*, iBufferToEmpty)->Data().Length();
	TInt destLength = STATIC_CAST(CMMFDataBuffer*, iNextBuffer)->Data().MaxLength() - iNextBuffer->Position();

	if ( srcLength > destLength )
		{

		iNextBuffer->SetStatus(EFull);
		nextEvent = EFlushBuffer;
		flushBuffer = iNextBuffer;
		NextSharedBufferL();
		iNextBuffer->SetStatus(EBeingFilled);
		iNextBuffer->SetPosition(0);

		TDes8& newDestBufferDes = STATIC_CAST(CMMFDataBuffer*, iNextBuffer)->Data();
		newDestBufferDes.Copy(srcBufferDes.Ptr(), srcLength);
		iNextBuffer->SetPosition(srcLength);
		}
	else
		{
		iNextBuffer->SetStatus(EBeingFilled);
		destBufferDes.Append(srcBufferDes.Ptr(), srcLength);
		iNextBuffer->SetPosition(iNextBuffer->Position() + srcLength);
		}

	if ( iBufferToEmpty->LastBuffer() ) //This is last Buffer so Flash out data.
		{
		iNextBuffer->SetStatus(EFull);  // last buffer is actually mean the buffer is full
		iNextBuffer->SetLastBuffer(ETrue);
		iBufferToEmpty->SetLastBuffer(EFalse); //Reset the Flag
		nextEvent = EFlushBuffer; 
		}

#ifdef __SEND_RAW_DATA__
	TDes8& bufferDes = STATIC_CAST(CMMFDataBuffer*, iBufferToEmpty)->Data();
	TDes8& nextBufferDes = STATIC_CAST(CMMFDataBuffer*, flushBuffer)->Data();
	TInt   copyLength = STATIC_CAST(CMMFDataBuffer*, iBufferToEmpty)->Data().Length();
	nextBufferDes.Copy(bufferDes.Ptr(), copyLength);
#endif

    iState = EIdle;
    if(nextEvent != EPlayEnd)
        {
        iObserver->BufferEmptied();
        }

	if (nextEvent == EFlushBuffer)  // Shared buffer is full, needs to be flushed.
		{
        iObserver->EmptyBuffer(flushBuffer);
		}
	}

// End of File
