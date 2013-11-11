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
#include "MP3AudioPlayControllerDecoder.h"
#include "DebugMacros.h"

#include <mmfcodec.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerDecoder::CMP3AudioPlayControllerDecoder()
    :   CAdvancedAudioDecoder(CActive::EPriorityStandard)
    {
    DP0(_L("CMP3AudioPlayControllerDecoder::CMP3AudioPlayControllerDecoder()"));
    }

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMP3AudioPlayControllerDecoder::ConstructL()
    {
    DP0(_L("CMP3AudioPlayControllerDecoder::ConstructL()"));
	iSoftCodec = CMMFCodec::NewL(TFourCC(' ','M','P','3'), TFourCC(' ','P','1','6'));

    iFrameTable = CFrameTable::NewL();
    
   	InitCodecConfigs();

   	CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerDecoder* CMP3AudioPlayControllerDecoder::NewL()
    {
    DP0(_L("CMP3AudioPlayControllerDecoder::NewL()"));
    CMP3AudioPlayControllerDecoder* self = new(ELeave) CMP3AudioPlayControllerDecoder();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CMP3AudioPlayControllerDecoder::~CMP3AudioPlayControllerDecoder()
	{
    DP0(_L("CMP3AudioPlayControllerDecoder::~CMP3AudioPlayControllerDecoder()"));

    delete iSoftCodec;
    delete iFrameTable;
	}

void CMP3AudioPlayControllerDecoder::InitCodecConfigs()
	{
	iRenderEnableConfig.Reset();
	iRenderEnableConfig.Append(0); // [0]
	iRenderEnableConfig.Append(0); // [1]
	iRenderEnableConfig.Append(0); // [2]
	iRenderEnableConfig.Append(0); // [3]
	iRenderEnableConfig.Append(0); // [4]
	iRenderEnableConfig.Append(0); // [5]
	iRenderEnableConfig.Append(0); // [6]
	iRenderEnableConfig.Append(1); // [7] render enabled
	iRenderEnableConfig.Append(0); // [8]
	iRenderEnableConfig.Append(0); // [9]
	iRenderEnableConfig.Append(0); // [10]

	iRenderDisableConfig.Reset();
	iRenderDisableConfig.Append(0); // [0]
	iRenderDisableConfig.Append(0); // [1]
	iRenderDisableConfig.Append(0); // [2]
	iRenderDisableConfig.Append(0); // [3]
	iRenderDisableConfig.Append(0); // [4]
	iRenderDisableConfig.Append(0); // [5]
	iRenderDisableConfig.Append(0); // [6]
	iRenderDisableConfig.Append(2); // [7] render disable
	iRenderDisableConfig.Append(0); // [8]
	iRenderDisableConfig.Append(0); // [9]
	iRenderDisableConfig.Append(0); // [10]
	
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
	iMarkPlayEndConfig.Append(1); // [9] mark play end
	iMarkPlayEndConfig.Append(0); // [10]
	
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
	iUnMarkPlayEndConfig.Append(2); // [9] unmark play end
	iUnMarkPlayEndConfig.Append(0); // [10]
	
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
	iEnableConfig.Append(1); // [10] enable
	
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
	iDisableConfig.Append(2); // [10] disable
	
	RenderEnable();
	UnMarkPlayEnd();
	Enable();
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoder::CodecConfig
// Sets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
TInt CMP3AudioPlayControllerDecoder::CodecConfig(RArray<TInt>& aCodecConfigData)
	{
	TInt stat;
//	RArray<TInt>& codecArrayConfigData = reinterpret_cast<RArray<TInt>&>(aCodecConfigData);
//   	codecArrayConfigData[6] = reinterpret_cast<TInt>(iFrameTable);
   	aCodecConfigData[6] = reinterpret_cast<TInt>(iFrameTable);
	TRAP(stat, iSoftCodec->ConfigureL(KUidConfig, reinterpret_cast<TDesC8&>(aCodecConfigData)));
	return stat;
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoder::IsHwAccelerated
// Always return true since soft codec is used.
// -----------------------------------------------------------------------------
//
TBool CMP3AudioPlayControllerDecoder::IsHwAccelerated()
	{
	return CAdvancedAudioDecoder::IsHwAccelerated();
	}

void CMP3AudioPlayControllerDecoder::ResetL()
	{
	CAdvancedAudioDecoder::ResetL();
	}

TCodecProcessResult CMP3AudioPlayControllerDecoder::ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst)
	{
	return CAdvancedAudioDecoder::ProcessL(aSrc, aDst);
	}

TInt CMP3AudioPlayControllerDecoder::CodecCmd(TCodecCmd aCmd)
	{
	return CAdvancedAudioDecoder::CodecCmd(aCmd);
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoder::SeekSync
// Implemented for hw codec
// -----------------------------------------------------------------------------
TInt CMP3AudioPlayControllerDecoder::SeekSync(TUint8* /*aBuf*/, TInt /*aBufLen*/)
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoder::FrameLength
// Implemented for hw codec
// -----------------------------------------------------------------------------
TInt CMP3AudioPlayControllerDecoder::FrameLength(const TUint8* /*aBuf*/, TInt /*aBufLen*/, TInt& /*aFrameLength*/)
	{
    return KErrNotSupported;
	}



// End of file
