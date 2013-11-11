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
*                for the audio output base class. Audio output provides the
*                interface between the controller and the sound device.
*
*/


// INCLUDE FILES
#include "AWBAudioPlayControllerDecoder.h"
#include <mmfcodec.h>
#include "DebugMacros.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAWBAudioPlayControllerDecoder::CAWBAudioPlayControllerDecoder()
    :   CAdvancedAudioDecoder(CActive::EPriorityStandard)
    {
    DP0(_L("CAWBAudioPlayControllerDecoder::CAWBAudioPlayControllerDecoder()"));
    }

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAWBAudioPlayControllerDecoder::ConstructL()
    {
    DP0(_L("CAWBAudioPlayControllerDecoder::ConstructL()"));
	iSoftCodec = CMMFCodec::NewL(TFourCC(' ','A','W','B'), TFourCC(' ','P','1','6'));

    iFrameTable = CFrameTable::NewL();
    
    InitCodecConfigs();

    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAWBAudioPlayControllerDecoder* CAWBAudioPlayControllerDecoder::NewL()
    {
    DP0(_L("CAWBAudioPlayControllerDecoder::NewL()"));
    CAWBAudioPlayControllerDecoder* self = new(ELeave) CAWBAudioPlayControllerDecoder();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CAWBAudioPlayControllerDecoder::~CAWBAudioPlayControllerDecoder()
	{
    DP0(_L("CAWBAudioPlayControllerDecoder::~CAWBAudioPlayControllerDecoder()"));
	delete iSoftCodec;
    delete iFrameTable;	
	}


void CAWBAudioPlayControllerDecoder::InitCodecConfigs()
	{
	iRenderEnableConfig.Reset();
	iRenderEnableConfig.Append(0); // [0]
	iRenderEnableConfig.Append(0); // [1] 
	iRenderEnableConfig.Append(0); // [2] table pointer
	iRenderEnableConfig.Append(1); // [3] render enable
	iRenderEnableConfig.Append(0); // [4]
	iRenderEnableConfig.Append(0); // [5]

	iRenderDisableConfig.Reset();
	iRenderDisableConfig.Append(0); // [0]
	iRenderDisableConfig.Append(0); // [1] 	
	iRenderDisableConfig.Append(0); // [2] 
	iRenderDisableConfig.Append(2); // [3] render disable
	iRenderDisableConfig.Append(0); // [4]
	iRenderDisableConfig.Append(0); // [5]
	
	iMarkPlayEndConfig.Reset();
	iMarkPlayEndConfig.Append(0); // [0]
	iMarkPlayEndConfig.Append(0); // [1]
	iMarkPlayEndConfig.Append(0); // [2]
	iMarkPlayEndConfig.Append(0); // [3]
	iMarkPlayEndConfig.Append(1); // [4] mark play end
	iMarkPlayEndConfig.Append(0); // [5] 
	
	iUnMarkPlayEndConfig.Reset();
	iUnMarkPlayEndConfig.Append(0); // [0]	
	iUnMarkPlayEndConfig.Append(0); // [1]
	iUnMarkPlayEndConfig.Append(0); // [2]
	iUnMarkPlayEndConfig.Append(0); // [3]
	iUnMarkPlayEndConfig.Append(2); // [4] unmark play end
	iUnMarkPlayEndConfig.Append(0); // [5] 
	
	iEnableConfig.Reset();
	iEnableConfig.Append(0); // [0]	
	iEnableConfig.Append(0); // [1]
	iEnableConfig.Append(0); // [2]
	iEnableConfig.Append(0); // [3]
	iEnableConfig.Append(0); // [4]
	iEnableConfig.Append(1); // [5] enable
	
	iDisableConfig.Reset();
	iDisableConfig.Append(0); // [0]	
	iDisableConfig.Append(0); // [1]
	iDisableConfig.Append(0); // [2]
	iDisableConfig.Append(0); // [3]
	iDisableConfig.Append(0); // [4]
	iDisableConfig.Append(2); // [5] disable
	
	RenderEnable();
	UnMarkPlayEnd();
	Enable();
	}

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoder::CodecConfig
// Sets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
TInt CAWBAudioPlayControllerDecoder::CodecConfig(RArray<TInt>& aCodecConfigData)
	{
	TInt stat;
   	aCodecConfigData[2] = reinterpret_cast<TInt>(iFrameTable);
	TRAP(stat, iSoftCodec->ConfigureL(KUidConfig, reinterpret_cast<TDesC8&>(aCodecConfigData)));
	return stat;
	}


// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoder::IsHwAccelerated
// Always return true since soft codec is used.
// -----------------------------------------------------------------------------
//
TBool CAWBAudioPlayControllerDecoder::IsHwAccelerated()
	{
	return CAdvancedAudioDecoder::IsHwAccelerated();
	}

void CAWBAudioPlayControllerDecoder::ResetL()
	{
	CAdvancedAudioDecoder::ResetL();
	}

TCodecProcessResult CAWBAudioPlayControllerDecoder::ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst)
	{
	return CAdvancedAudioDecoder::ProcessL(aSrc, aDst);
	}

TInt CAWBAudioPlayControllerDecoder::CodecCmd(TCodecCmd aCmd)
	{
	return CAdvancedAudioDecoder::CodecCmd(aCmd);
	}

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoder::SeekSync
// Implemented for hw codec
// -----------------------------------------------------------------------------
TInt CAWBAudioPlayControllerDecoder::SeekSync(TUint8* /*aBuf*/, TInt /*aBufLen*/)
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoder::FrameLength
// Implemented for hw codec
// -----------------------------------------------------------------------------
TInt CAWBAudioPlayControllerDecoder::FrameLength(const TUint8* /*aBuf*/, TInt /*aBufLen*/, TInt& /*aFrameLength*/)
	{
    return KErrNotSupported;
	}

// End of file
