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
#include "AMRAudioPlayControllerDecoder.h"
#include <mmfcodec.h>
#include "DebugMacros.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAMRAudioPlayControllerDecoder::CAMRAudioPlayControllerDecoder()
    :   CAdvancedAudioDecoder(CActive::EPriorityStandard)
    {
    DP0(_L("CAMRAudioPlayControllerDecoder::CAMRAudioPlayControllerDecoder()"));
    }

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAMRAudioPlayControllerDecoder::ConstructL()
    {
    DP0(_L("CAMRAudioPlayControllerDecoder::ConstructL()"));
	iSoftCodec = CMMFCodec::NewL(TFourCC(' ','A','M','R'), TFourCC(' ','P','1','6'));

    iFrameTable = CFrameTable::NewL();
    
    InitCodecConfigs();

    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAMRAudioPlayControllerDecoder* CAMRAudioPlayControllerDecoder::NewL()
    {
    DP0(_L("CAMRAudioPlayControllerDecoder::NewL()"));
    CAMRAudioPlayControllerDecoder* self = new(ELeave) CAMRAudioPlayControllerDecoder();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CAMRAudioPlayControllerDecoder::~CAMRAudioPlayControllerDecoder()
	{
    DP0(_L("CAMRAudioPlayControllerDecoder::~CAMRAudioPlayControllerDecoder()"));
	delete iSoftCodec;
    delete iFrameTable;	
	}


void CAMRAudioPlayControllerDecoder::InitCodecConfigs()
	{
	iRenderEnableConfig.Reset();
	iRenderEnableConfig.Append(0); // [0]
	iRenderEnableConfig.Append(0); // [1] table pointer
	iRenderEnableConfig.Append(1); // [2] render enabled
	iRenderEnableConfig.Append(0); // [3]
	iRenderEnableConfig.Append(0); // [4]

	iRenderDisableConfig.Reset();
	iRenderDisableConfig.Append(0); // [0]
	iRenderDisableConfig.Append(0); // [1] 
	iRenderDisableConfig.Append(2); // [2] render disable
	iRenderDisableConfig.Append(0); // [3]
	iRenderDisableConfig.Append(0); // [4]
	
	iMarkPlayEndConfig.Reset();
	iMarkPlayEndConfig.Append(0); // [0]
	iMarkPlayEndConfig.Append(0); // [1]
	iMarkPlayEndConfig.Append(0); // [2]
	iMarkPlayEndConfig.Append(1); // [3] mark play end
	iMarkPlayEndConfig.Append(0); // [4] 
	
	iUnMarkPlayEndConfig.Reset();
	iUnMarkPlayEndConfig.Append(0); // [0]
	iUnMarkPlayEndConfig.Append(0); // [1]
	iUnMarkPlayEndConfig.Append(0); // [2]
	iUnMarkPlayEndConfig.Append(2); // [3] unmark play end
	iUnMarkPlayEndConfig.Append(0); // [4] 
	
	iEnableConfig.Reset();
	iEnableConfig.Append(0); // [0]
	iEnableConfig.Append(0); // [1]
	iEnableConfig.Append(0); // [2]
	iEnableConfig.Append(0); // [3]
	iEnableConfig.Append(1); // [4] enable
	
	iDisableConfig.Reset();
	iDisableConfig.Append(0); // [0]
	iDisableConfig.Append(0); // [1]
	iDisableConfig.Append(0); // [2]
	iDisableConfig.Append(0); // [3]
	iDisableConfig.Append(2); // [4] disable
	
	RenderEnable();
	UnMarkPlayEnd();
	Enable();
	}


// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoder::CodecConfig
// Sets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
TInt CAMRAudioPlayControllerDecoder::CodecConfig(RArray<TInt>& aCodecConfigData)
	{
	TInt stat;
   	aCodecConfigData[1] = reinterpret_cast<TInt>(iFrameTable);
	TRAP(stat, iSoftCodec->ConfigureL(KUidConfig, reinterpret_cast<TDesC8&>(aCodecConfigData)));
	return stat;
	}

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoder::IsHwAccelerated
// Always return true since soft codec is used.
// -----------------------------------------------------------------------------
//
TBool CAMRAudioPlayControllerDecoder::IsHwAccelerated()
	{
	return CAdvancedAudioDecoder::IsHwAccelerated();
	}

void CAMRAudioPlayControllerDecoder::ResetL()
	{
	CAdvancedAudioDecoder::ResetL();
	}

TCodecProcessResult CAMRAudioPlayControllerDecoder::ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst)
	{
	return CAdvancedAudioDecoder::ProcessL(aSrc, aDst);
	}

TInt CAMRAudioPlayControllerDecoder::CodecCmd(TCodecCmd aCmd)
	{
	return CAdvancedAudioDecoder::CodecCmd(aCmd);
	}

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoder::SeekSync
// Implemented for hw codec
// -----------------------------------------------------------------------------
TInt CAMRAudioPlayControllerDecoder::SeekSync(TUint8* /*aBuf*/, TInt /*aBufLen*/)
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoder::FrameLength
// Implemented for hw codec
// -----------------------------------------------------------------------------
TInt CAMRAudioPlayControllerDecoder::FrameLength(const TUint8* /*aBuf*/, TInt /*aBufLen*/, TInt& /*aFrameLength*/)
	{
    return KErrNotSupported;
	}

// End of file
