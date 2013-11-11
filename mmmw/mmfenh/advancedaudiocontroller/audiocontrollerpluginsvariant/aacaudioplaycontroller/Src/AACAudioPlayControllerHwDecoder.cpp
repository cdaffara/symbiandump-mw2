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
* Description:  The functions in this module implements the specific behavior
*                for the play controller decoder class for configuration of
*				 hardware accelerated codec.
*
*/


// INCLUDE FILES
#include "AACAudioPlayControllerDecoder.h"
#include "DebugMacros.h"
#include <mmfcodec.h>

#include <mmfpaniccodes.h>

// New eAAC+ custom interface is used only when using DSP decoder and 
// the eAAC+ feature is turned on.
#ifdef __AAC_PLUS_DECODER_PLUGIN 
#include <EAacPlusDecoderIntfc.h>
#else
#include <AacDecoderConfig.h> 
#include <AudioConfig.h>	
#endif

#include <EAacPlusDecoderIntfc.h>
//#include "EAacPlusFrameParser.h"

// CONSTANT
const TUint KMaxAacFrameSize  = 1536;
const TUint KSizeOfInBuffer   = 2*KMaxAacFrameSize;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerDecoder::CAACAudioPlayControllerDecoder()
    :   CAdvancedAudioDecoder(CActive::EPriorityStandard)
    {
    DP0(_L("CAACAudioPlayControllerDecoder::CAACAudioPlayControllerDecoder - Hardware Accelerated"));
    iMaxFrameSize = KMaxAacFrameSize;
   	iSizeOfInBuffer = KSizeOfInBuffer;
    }

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerDecoder::ConstructL()
    {
    DP0(_L("CAACAudioPlayControllerDecoder::ConstructL"));

	iAacUtil = CEAacPlusUtil::NewL();
    iFrameTable = CFrameTable::NewL();
    
	RenderEnable();
	UnMarkPlayEnd();
	Enable();
    
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
    DP0(_L("CAACAudioPlayControllerDecoder::~CAACAudioPlayControllerDecoder"));
	
	delete iAacUtil;
    delete iFrameTable;
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoder::SetConfigL
// Sets configuration data and configures the codec.
// -----------------------------------------------------------------------------
//
TInt CAACAudioPlayControllerDecoder::CodecConfig(RArray<TInt>& aCodecConfigData)
	{
	DP0(_L("CAACAudioPlayControllerDecoder::SetConfigL"));

    TInt status = KErrNone;

   	aCodecConfigData[11] = reinterpret_cast<TInt>(iFrameTable);
   	iFormatType = (CEAacPlusUtil::TEAacPlusFormatType)(aCodecConfigData[12]);

	if (!iMMFDevSound)
		{
    	_LIT(KAudioPlayControllerDecoderPanicCategory, "AACAudioPlayControllerDecoder");
    	User::Panic(KAudioPlayControllerDecoderPanicCategory, EMMFAudioOutputDevSoundNotLoaded);
		}

#ifdef __AAC_PLUS_DECODER_PLUGIN
//uses new eAAC+ interface

	DP0(_L("CAACAudioPlayControllerDecoder::SetConfigL eAAC+ intfc"));
    CEAacPlusDecoderIntfc::TAudioObjectType objType(CEAacPlusDecoderIntfc::ENull);
    TInt count = aCodecConfigData.Count();
    ASSERT(count > 2);

    DP1(_L("CAACAudioPlayControllerDecoder::SetConfigL, aCodecConfigData[2] = %d"), aCodecConfigData[2]);

	switch (aCodecConfigData[2])
    	{
    	case 1:
    		objType = CEAacPlusDecoderIntfc::EAacLc;
    		break;
    	case 3:
    		objType = CEAacPlusDecoderIntfc::EAacLtp;
    		break;
    	default:
    		return status = KErrNotSupported;
    	}

    CEAacPlusDecoderIntfc* aacPlusDecCustomInterface(NULL);
	TRAP_IGNORE(aacPlusDecCustomInterface = CEAacPlusDecoderIntfc::NewL(*iMMFDevSound)); 
	if (!aacPlusDecCustomInterface)
		{
		DP0(_L("CAACAudioPlayControllerDecoder::SetConfigL eAAC+ intfc not supported"));
		return KErrNotSupported;
		}

    aacPlusDecCustomInterface->SetAudioObjectType(objType);
    aacPlusDecCustomInterface->SetInputSamplingFrequency(aCodecConfigData[5]);
    aacPlusDecCustomInterface->SetNumOfChannels(aCodecConfigData[1]);
    aacPlusDecCustomInterface->SetSbr(EFalse);
    aacPlusDecCustomInterface->SetDownSampledMode(EFalse);

    if (objType == CEAacPlusDecoderIntfc::EAacLc)
        {
        if ((aCodecConfigData[10] == 5) ||      //AAC + SBR
            (aCodecConfigData[10] == 6))        //AAC + SBR + PS
            {
            aacPlusDecCustomInterface->SetSbr(ETrue);
            }
            
        if ((aCodecConfigData[7] == 1) &&      //DSM
            ((aCodecConfigData[10] == 5) ||     //AAC + SBR
             (aCodecConfigData[10] == 6)))      //AAC + SBR + PS
            {
       	    aacPlusDecCustomInterface->SetDownSampledMode(ETrue);
            }
        }

    status = aacPlusDecCustomInterface->ApplyConfig();
   	delete aacPlusDecCustomInterface;
	DP1(_L("CAACAudioPlayControllerDecoder::SetConfigL, ApplyConfig status = %d"), status);
		
#else // __AAC_PLUS_DECODER_PLUGIN
//uses old AAC interface   
 
	DP0(_L("CAACAudioPlayControllerDecoder::SetConfigL AAC intfc"));
	TAudioConfig aacAudioConfig; // AAC Configuration structure

	// Profile Data is comming from AAC Codec
	// For That Codec... AAC Profile object type is Main=0; LC=1, SSR=2; LTP=3 (Based on Old AAC Codec)
	// Custom Interface Profile ENUMS are based on MPEG4 Audio Object Types which is
	// ENull=0;EAacMain=1;EAacLc=2;EAacSsr=3;EAacLtp=4;ESbr=5.. So the Codec Audio Object Type is
	// one off from MPEG4 Object Type.

    ASSERT(aCodecConfigData.Count() > 2);

    DP1(_L("CAACAudioPlayControllerDecoder::SetConfigL, aCodecConfigData[2] = %d"), aCodecConfigData[2]);

	switch (aCodecConfigData[2]) // So Map Old Object Type to the New MPEG4 Object Type
    	{
    	case 0: // Old Codec Object Type 0 (Main)
    		aacAudioConfig.iAudioObjectType = TAudioConfig::EAacMain;
    		break;
    	case 1:// Old Codec Object Type 1 (LC)
    		aacAudioConfig.iAudioObjectType = TAudioConfig::EAacLc;
    		break;
    	case 2:// Old Codec Object Type 2 (SSR)
    		aacAudioConfig.iAudioObjectType = TAudioConfig::EAacSsr;
    		break;
    	case 3:// Old Codec Object Type 3 (LTP)
    		aacAudioConfig.iAudioObjectType = TAudioConfig::EAacLtp;
    		break;
    	case 4:
    		aacAudioConfig.iAudioObjectType = TAudioConfig::ESbr;
    		break;
    	default:
	    	return status = KErrNotSupported;
    	}

	CAacDecoderConfig* aacDecCustomInterface(NULL);// Creates a custom interface
	
	//Calls the custom on devsound Devsound will return the handle to the custom interface proxy
	TRAP(status, aacDecCustomInterface = CAacDecoderConfig::NewL(*iMMFDevSound)); 
	if (status != KErrNone)
	{
		return status;	
	}
	status = aacDecCustomInterface->SetAudioConfig(aacAudioConfig);
	delete aacDecCustomInterface; // We have the handle so we Delete the Custom Interface and Adptation should Not delete this
	
	DP1(_L("CAACAudioPlayControllerDecoder::SetConfigL SetAudioConfig, status = %d"), status);

#endif //__AAC_PLUS_DECODER_PLUGIN

    if (status != KErrNone)
        {
        return status;
        }

    // pass FrameTable Handle to CMMFCodec so that it can build the table
    aCodecConfigData[11] = reinterpret_cast<TInt>(iFrameTable);

	
	CEAacPlusUtil::TEAacPlusFormatType formatType = static_cast<CEAacPlusUtil::TEAacPlusFormatType>(aCodecConfigData[12]);
	TInt inSampFreq = aCodecConfigData[5];
	TInt outSampFreq = aCodecConfigData[9];
	TInt numSampPerFrame = aCodecConfigData[4];

//	CEAacPlusUtil::TEAacPlusFrameLenConfig iFrameLenConfig;
    iFrameLenConfig.iFormatType = formatType;
	iFrameLenConfig.iSamplingFrequency = inSampFreq;
	iFrameLenConfig.iObjectType = objType;
	iAacUtil->Config(iFrameLenConfig);
	if(inSampFreq!=outSampFreq) 
		 { 
		 numSampPerFrame =2*numSampPerFrame;
		 }

//	TInt sampFreq = ((RArray<TInt>&) aConfigData)[5];
//	TInt samplesPerFrame = ((RArray<TInt>&) aConfigData)[8];
//	iFrameTable->InitFrameTable(sampFreq, samplesPerFrame);
	iFrameTable->InitFrameTable(outSampFreq, numSampPerFrame);
	
	return status;
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoder::IsHwAccelerated
// Always return true when no soft codec is used.
// -----------------------------------------------------------------------------
//
TBool CAACAudioPlayControllerDecoder::IsHwAccelerated()
	{
    DP0(_L ("CAACAudioPlayControllerDecoder::IsHwAccelerated true"));
	return ETrue;
	}
	
void CAACAudioPlayControllerDecoder::ResetL()
	{
    DP0(_L ("CAACAudioPlayControllerDecoder::Reset - Enter"));

	delete[] iOutBuffer;
   	delete[] iInBuffer;
   	
   	iInBuffer = NULL;
   	iOutBuffer = NULL;
   	iInBufferCount = 0;
   	iOutBufferCount = 0;
   	iOutBufferPtr = NULL;
   	iOutFrameLength = KMaxAacFrameSize; // since data is not decoded
   	iInBuffer = new (ELeave) TUint8[KSizeOfInBuffer];

    iOutBuffer = new (ELeave) TUint8[iOutFrameLength];
    iOutBufferPtr = iOutBuffer;

    iAccLen = 0;

	iAacUtil->Config(iFrameLenConfig);
    DP0(_L ("CAACAudioPlayControllerDecoder::Reset - Exit"));
	}
	
TCodecProcessResult CAACAudioPlayControllerDecoder::ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst)
	{
    DP0(_L ("CAACAudioPlayControllerDecoder::ProcessL"));
	return CAdvancedAudioDecoder::ProcessHwL(aSrc, aDst);
	}

TInt CAACAudioPlayControllerDecoder::FrameLength(const TUint8* aBuf, TInt aBufLen, TInt& aFrameLength)
	{
	TInt stat;
	TInt frameLen;
	stat = iAacUtil->GetAacFrameLength(aBuf, aBufLen, frameLen);
	aFrameLength = frameLen;
	return stat;
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoder::SeekSync
// -----------------------------------------------------------------------------
TInt CAACAudioPlayControllerDecoder::SeekSync(TUint8* aBuf, TInt aBufLen)
	{ // should return aBufLen if not found
	DP0(_L("CAACAudioPlayControllerDecoder::SeekSync"));
	TInt syncPos = aBufLen;
	if (iFormatType == 2) // ADTS=2
		{
		syncPos = SeekAdtsSync(aBuf, aBufLen);
		}
	return syncPos;
	}
	
// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoder::SeekAdtsSync
// -----------------------------------------------------------------------------
TInt CAACAudioPlayControllerDecoder::SeekAdtsSync(TUint8* aBuf, TInt aBufLen)
	{ // should return aBufLen if not found
	DP0(_L("CMMFeAacPlusTo16PcmCodec::SeekSync"));
    const TInt KNotFound = aBufLen;     // sync not found position
    TInt syncPos = KNotFound;
    TInt seekLen = 0;

    while (seekLen < aBufLen)
	    {
		if (IsValidAdtsFrame(aBuf+seekLen, aBufLen-seekLen))
			{
	        syncPos = seekLen;
			break;
			}
		seekLen++;	
    	}
    return syncPos;
	}

TBool CAACAudioPlayControllerDecoder::IsValidAdtsFrame(const TUint8* aPtr, TInt aBufLen)
    {
	DP0(_L("CAACAudioPlayControllerDecoder::IsValidAdtsFrame"));
	if (aBufLen < 6)
		{
		return EFalse;
		}

	if (((aPtr[0] == 0xFF) && ((aPtr[1] >> 4) == 0xF) &&    // Check syncword FFF
   	     ((aPtr[1] >> 1) & 0x3) == 0))                      // Check layer == 0
        {
   	    TInt frameLen = 0;
        frameLen = (aPtr[3] & 0x3) << 11;
   	    frameLen |= aPtr[4] << 3;
       	frameLen |= ((aPtr[5] >> 5) & 0x7);

        if (!frameLen || (frameLen > (aBufLen-4)))
   	       return EFalse;

        // Check next frame
        if (aBufLen - frameLen >= 2)
        	{
	   	    const TUint8* nextFramePtr = aPtr + frameLen;
		    if ((((nextFramePtr[0] == 0xFF) && ((nextFramePtr[1] >> 4) == 0xF)) && // Check syncword FFF
   	    	     ((nextFramePtr[1] >> 1) & 0x3) == 0)) // Check layer == 0
	            return ETrue;
        	}
   	    }
    return EFalse;
    }


TInt CAACAudioPlayControllerDecoder::CodecCmd(TCodecCmd /*aCmd*/)
	{
	return KErrNotSupported;
	}

// End of file
