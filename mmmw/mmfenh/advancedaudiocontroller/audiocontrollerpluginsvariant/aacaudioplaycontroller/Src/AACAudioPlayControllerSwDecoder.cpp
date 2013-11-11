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
#include "AACAudioPlayControllerDecoder.h"
#include "DebugMacros.h"
#include <mmfcodec.h>

// CONSTANT

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerDecoder::CAACAudioPlayControllerDecoder()
    :   CAdvancedAudioDecoder(CActive::EPriorityStandard)
    {
    DP0(_L("CAACAudioPlayControllerDecoder::CAACAudioPlayControllerDecoder()"));
    }

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerDecoder::ConstructL()
    {
#ifdef __AAC_PLUS_DECODER_PLUGIN
    DP0(_L("CAACAudioPlayControllerDecoder::ConstructL() for eAAC+ Codec"));
	iSoftCodec = CMMFCodec::NewL(TFourCC(' ','E','A','C'), TFourCC(' ','P','1','6'));
#else
	DP0(_L("CAACAudioPlayControllerDecoder::ConstructL() for AAC Codec"));
	iSoftCodec = CMMFCodec::NewL(TFourCC(' ','A','A','C'), TFourCC(' ','P','1','6'));
#endif

    iFrameTable = CFrameTable::NewL();
    
   	InitCodecConfigs();

   	CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerDecoder* CAACAudioPlayControllerDecoder::NewL()
    {
    DP0(_L("CAACAudioPlayControllerDecoder::NewL()"));
    CAACAudioPlayControllerDecoder* self = new(ELeave) CAACAudioPlayControllerDecoder();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CAACAudioPlayControllerDecoder::~CAACAudioPlayControllerDecoder()
	{
    DP0(_L("CAACAudioPlayControllerDecoder::~CAACAudioPlayControllerDecoder()"));
    delete iSoftCodec;
    delete iFrameTable;
	}

void CAACAudioPlayControllerDecoder::InitCodecConfigs()
	{
	iRenderEnableConfig.Reset();
	iRenderEnableConfig.Append(0); // [0]
	iRenderEnableConfig.Append(0); // [1]
	iRenderEnableConfig.Append(0); // [2]
	iRenderEnableConfig.Append(0); // [3]
	iRenderEnableConfig.Append(0); // [4]
	iRenderEnableConfig.Append(0); // [5]
	iRenderEnableConfig.Append(0); // [6]
	iRenderEnableConfig.Append(0); // [7]
	iRenderEnableConfig.Append(0); // [8]
	iRenderEnableConfig.Append(0); // [9]
	iRenderEnableConfig.Append(0); // [10]
	iRenderEnableConfig.Append(0); // [11]
	iRenderEnableConfig.Append(0); // [12]
	iRenderEnableConfig.Append(1); // [13] render enabled
	iRenderEnableConfig.Append(0); // [14] 
	iRenderEnableConfig.Append(0); // [15] 
	
	iRenderDisableConfig.Reset();
	iRenderDisableConfig.Append(0); // [0]
	iRenderDisableConfig.Append(0); // [1]
	iRenderDisableConfig.Append(0); // [2]
	iRenderDisableConfig.Append(0); // [3]
	iRenderDisableConfig.Append(0); // [4]
	iRenderDisableConfig.Append(0); // [5]
	iRenderDisableConfig.Append(0); // [6]
	iRenderDisableConfig.Append(0); // [7]
	iRenderDisableConfig.Append(0); // [8]
	iRenderDisableConfig.Append(0); // [9]
	iRenderDisableConfig.Append(0); // [10]
	iRenderDisableConfig.Append(0); // [11]
	iRenderDisableConfig.Append(0); // [12]
	iRenderDisableConfig.Append(2); // [13] render disable
	iRenderDisableConfig.Append(0); // [14]
	iRenderDisableConfig.Append(0); // [15]

	iMarkPlayEndConfig.Reset();
	iMarkPlayEndConfig.Append(0); // [0]
	iMarkPlayEndConfig.Append(0); // [1]
	iMarkPlayEndConfig.Append(0); // [2]
	iMarkPlayEndConfig.Append(0); // [3]
	iMarkPlayEndConfig.Append(0); // [4]
	iMarkPlayEndConfig.Append(0); // [5]
	iMarkPlayEndConfig.Append(0); // [6]
	iMarkPlayEndConfig.Append(0); // [7]
	iMarkPlayEndConfig.Append(0); // [8]
	iMarkPlayEndConfig.Append(0); // [9]
	iMarkPlayEndConfig.Append(0); // [10]
	iMarkPlayEndConfig.Append(0); // [11]
	iMarkPlayEndConfig.Append(0); // [12]
	iMarkPlayEndConfig.Append(0); // [13]
	iMarkPlayEndConfig.Append(1); // [14] mark play end
	iMarkPlayEndConfig.Append(0); // [15]
	
	iUnMarkPlayEndConfig.Reset();
	iUnMarkPlayEndConfig.Append(0); // [0]
	iUnMarkPlayEndConfig.Append(0); // [1]
	iUnMarkPlayEndConfig.Append(0); // [2]
	iUnMarkPlayEndConfig.Append(0); // [3]
	iUnMarkPlayEndConfig.Append(0); // [4]
	iUnMarkPlayEndConfig.Append(0); // [5]
	iUnMarkPlayEndConfig.Append(0); // [6]
	iUnMarkPlayEndConfig.Append(0); // [7]
	iUnMarkPlayEndConfig.Append(0); // [8]
	iUnMarkPlayEndConfig.Append(0); // [9]
	iUnMarkPlayEndConfig.Append(0); // [10]
	iUnMarkPlayEndConfig.Append(0); // [11]
	iUnMarkPlayEndConfig.Append(0); // [12]
	iUnMarkPlayEndConfig.Append(0); // [13]
	iUnMarkPlayEndConfig.Append(2); // [14] unmark play end
	iUnMarkPlayEndConfig.Append(0); // [15]
	
	iEnableConfig.Reset();
	iEnableConfig.Append(0); // [0]
	iEnableConfig.Append(0); // [1]
	iEnableConfig.Append(0); // [2]
	iEnableConfig.Append(0); // [3]
	iEnableConfig.Append(0); // [4]
	iEnableConfig.Append(0); // [5]
	iEnableConfig.Append(0); // [6]
	iEnableConfig.Append(0); // [7]
	iEnableConfig.Append(0); // [8]
	iEnableConfig.Append(0); // [9] 
	iEnableConfig.Append(0); // [10]
	iEnableConfig.Append(0); // [11]
	iEnableConfig.Append(0); // [12]
	iEnableConfig.Append(0); // [13]
	iEnableConfig.Append(0); // [14]
	iEnableConfig.Append(1); // [15] enable
	
	iDisableConfig.Reset();
	iDisableConfig.Append(0); // [0]
	iDisableConfig.Append(0); // [1]
	iDisableConfig.Append(0); // [2]
	iDisableConfig.Append(0); // [3]
	iDisableConfig.Append(0); // [4]
	iDisableConfig.Append(0); // [5]
	iDisableConfig.Append(0); // [6]
	iDisableConfig.Append(0); // [7]
	iDisableConfig.Append(0); // [8]
	iDisableConfig.Append(0); // [9] 
	iDisableConfig.Append(0); // [10]
	iDisableConfig.Append(0); // [11]
	iDisableConfig.Append(0); // [12]
	iDisableConfig.Append(0); // [13]
	iDisableConfig.Append(0); // [14]
	iDisableConfig.Append(2); // [15] disable

	RenderEnable();
	UnMarkPlayEnd();
	Enable();
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoder::CodecConfig
// Sets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
TInt CAACAudioPlayControllerDecoder::CodecConfig(RArray<TInt>& aCodecConfigData)
	{
	TInt stat;
   	aCodecConfigData[11] = reinterpret_cast<TInt>(iFrameTable);
   	iFormatType = (CEAacPlusUtil::TEAacPlusFormatType)(aCodecConfigData[12]);
	TRAP(stat, iSoftCodec->ConfigureL(KUidConfig, reinterpret_cast<TDesC8&> (aCodecConfigData)));
	return stat;
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoder::IsHwAccelerated
// Always return true since soft codec is used.
// -----------------------------------------------------------------------------
//
TBool CAACAudioPlayControllerDecoder::IsHwAccelerated()
	{
	return CAdvancedAudioDecoder::IsHwAccelerated();
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoder::ResetL
// Resets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerDecoder::ResetL()
	{
	CAdvancedAudioDecoder::ResetL();
	}

TCodecProcessResult CAACAudioPlayControllerDecoder::ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst)
	{
	return CAdvancedAudioDecoder::ProcessL(aSrc, aDst);
	}

TInt CAACAudioPlayControllerDecoder::CodecCmd(TCodecCmd aCmd)
	{
	return CAdvancedAudioDecoder::CodecCmd(aCmd);
	}

TInt CAACAudioPlayControllerDecoder::FrameLength(const TUint8* /*aBuf*/, TInt /*aBufLen*/, TInt& /*aFrameLength*/)
	{
	return KErrNotSupported;
	}

TInt CAACAudioPlayControllerDecoder::SeekSync(TUint8* /*aBuf*/, TInt /*aBufLen*/)
	{
	return KErrNotSupported;
	}

// End of file
