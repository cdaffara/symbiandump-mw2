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
* Description:  This class implements ECOM controller plugin for playback of
*				 audio contents in 3GPP container family (e.g. 3GP, MP4, M4A, 3G2).
*
*/


#include "3GPAudioPlayControllerPluginUIDs.hrh"
#include "3GPAudioPlayControllerPlugin.h"
#include "3GPAudioPlayControllerDecoderBuilder.h"
#include "3GPAudioPlayControllerResource.h"
#include "DevSoundAudioOutput.h"
#include "FileAudioOutput.h"
#include "DebugMacros.h"
#include "EAACPlusAudioControllerUtility.h"
#include "AMRAudioControllerUtility.h"
#include "AWBAudioControllerUtility.h"



// CONSTANTS
const TInt KFirstBufferSize = 2000;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerPlugin::C3GPAudioPlayControllerPlugin
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
C3GPAudioPlayControllerPlugin::C3GPAudioPlayControllerPlugin()
	: 	iDecoderBuilder(NULL)
    {
    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerPlugin::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3GPAudioPlayControllerPlugin::ConstructL()
    {
    CAdvancedAudioPlayController::ConstructL();

	iDecoderBuilder = C3GPAudioPlayControllerDecoderBuilder::NewL();

    iMetaDataSupport = EFalse;

//#ifdef __AAC_PLUS_DECODER_PLUGIN
//	iAacUtil = CEAacPlusUtil::NewL(); // used to get frame info for all formats
//#endif

    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerPlugin::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
C3GPAudioPlayControllerPlugin* C3GPAudioPlayControllerPlugin::NewL()
    {
	DP0(_L("C3GPAudioPlayControllerPlugin::NewL"));

    C3GPAudioPlayControllerPlugin* self = new(ELeave) C3GPAudioPlayControllerPlugin;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerPlugin::~C3GPAudioPlayControllerPlugin
// Destructor
// -----------------------------------------------------------------------------
//
C3GPAudioPlayControllerPlugin::~C3GPAudioPlayControllerPlugin()
    {
	DP0(_L("C3GPAudioPlayControllerPlugin::~C3GPAudioPlayControllerPlugin"));
    delete iDecoderBuilder;
    delete iAudioResource;
    delete iAudioOutput;
    delete iAudioUtility;

//#ifdef __AAC_PLUS_DECODER_PLUGIN
	delete iAacUtil;
//#endif
    }


// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerPlugin::DoAddDataSourceL
// -----------------------------------------------------------------------------
//
void C3GPAudioPlayControllerPlugin::DoAddDataSourceL()
    {
    DP0(_L("C3GPAudioPlayControllerPlugin::DoAddDataSourceL"));
    delete iAudioUtility; // see 3gp
    delete iAacUtil;
    iAudioUtility = NULL;
    iAacUtil = NULL;
	iDataSourceAdapter = C3gpDataSourceAdapter::NewL();
	iSharedBufferMaxNum = 1;
	iSharedBufferMaxSize = KFirstBufferSize;
    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerPlugin::DoAddDataSinkL
// -----------------------------------------------------------------------------
//
void C3GPAudioPlayControllerPlugin::DoAddDataSinkL()
    {
    DP0(_L("C3GPAudioPlayControllerPlugin::DoAddDataSinkL"));

    if (iDataSink->DataSinkType() == KUidMmfAudioOutput)
		{
		iAudioOutput = CDevSoundAudioOutput::NewL(iPrioritySettings, *this, *iMMFDevSound);
		iDecoderType = K3GPDecoder;
        }
    else if (iDataSink->DataSinkType() == KUidMmfFileSink)
        {
		iAudioOutput = CFileAudioOutput::NewL(*iDataSink, iPrioritySettings, *this);
		iDecoderType = K3GPConverter;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    }

CAdvancedAudioDecoder* C3GPAudioPlayControllerPlugin::BuildDecoderL()
	{
	return iDecoderBuilder->BuildDecoderL(iDecoderType, iDataType);
	}
	
void C3GPAudioPlayControllerPlugin::DoStopL(TInt aError)
    {
   	iSharedBufferMaxNum = 1;
	iSharedBufferMaxSize = KFirstBufferSize;

    CAdvancedAudioPlayController::DoStopL(aError);
    }
// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerPlugin::ReadHeaderL
// -----------------------------------------------------------------------------
//
void C3GPAudioPlayControllerPlugin::DoReadHeaderL(CMMFDataBuffer* aBuffer)
	{
    TUint32 audioLength;
    TUint32 timeScale;
    TUint32 averateBitRate;
    TUint8  framesPerSample;

	TUint32 decSpecInfoSize;
	HBufC8* decSpecInfo = NULL;
	TUint8* decSpecInfoPtr = NULL;
	TInt    codecType = 0;

    TInt err = static_cast<C3gpDataSourceAdapter*>(iDataSourceAdapter)->ReadHeader(audioLength, iAudioType, framesPerSample, timeScale, averateBitRate);
	
	User::LeaveIfError(err);

   	iDuration = (TInt64)audioLength*1000;		// convert to microseconds
	iBitRate = averateBitRate;
	iBitRateFrozen = ETrue;
	iDurationFrozen = ETrue;
	// In audio tracks, the sampling rate is in timescale
	iSampleRate = timeScale;
	
	switch (iAudioType)
		{
		case MP4_TYPE_MPEG4_AUDIO:
			{
			if (!iAacUtil)
				{
				iAacUtil = CEAacPlusUtil::NewL(); // used to get frame info for all formats
				}
			codecType = K3GPResourceAAC;
			iDataType = TFourCC(' ','E','A','C');
            if (!iAudioUtility)
                {
    			iAudioUtility = CEAACPlusAudioControllerUtility::NewL();
                }

			// Query the size of AudioSpecificConfig first
		    err = static_cast<C3gpDataSourceAdapter*>(iDataSourceAdapter)->ReadAudioSpecificConfig(NULL, 0, &decSpecInfoSize);
			User::LeaveIfError(err);	
			
			decSpecInfo = HBufC8::NewLC(decSpecInfoSize);
			decSpecInfoPtr = const_cast<TUint8*>(decSpecInfo->Ptr());
	
		    err = static_cast<C3gpDataSourceAdapter*>(iDataSourceAdapter)->ReadAudioSpecificConfig(decSpecInfoPtr, decSpecInfoSize, &decSpecInfoSize);
			

			User::LeaveIfError(err);
		    TInt buffersize = aBuffer->BufferSize();
		    TInt infoSize = decSpecInfoSize;
			TUint8* bufPtr = const_cast<TUint8*>(aBuffer->Data().Ptr());

			CEAacPlusUtil::TEAacPlusFrameInfo frameInfo;
			TInt status = iAacUtil->GetAacFrameInfo(static_cast<const TUint8*>(decSpecInfoPtr), infoSize, frameInfo, bufPtr, buffersize);

			iOutFrameSize = frameInfo.iOutFrameSize;
			iNoOfSamples = frameInfo.iNoOfSamples;
			iSamplingFrequency = frameInfo.iSamplingFrequency;
		    iProfile = frameInfo.iObjectType-1; // convert from object type back to what decoder uses 
			iNoOfChannels = frameInfo.iNoOfChannels ;
			iOutSamplingFrequency = frameInfo.iOutSamplingFrequency;
			iDownSampledMode = frameInfo.iDownSampledMode;

		    iExtObjectType = 0;
			if (frameInfo.iSbr)
				{
				iExtObjectType = 5;
				}
	        iSampleRate = iOutSamplingFrequency;
			if (frameInfo.iPs)
	    		{
	    		iExtObjectType = 6;
	    		}

			CleanupStack::PopAndDestroy(decSpecInfo);	// decSpecInfo
		    break;
			}
		case MP4_TYPE_AMR_NB:
			codecType = K3GPResourceAMR;
			iDataType = TFourCC(' ','A','M','R');
			iSampleRate = 8000;
            if (!iAudioUtility)
                {
	  			iAudioUtility = CAMRAudioControllerUtility::NewL();
                }
			break;

		case MP4_TYPE_AMR_WB:
			codecType = K3GPResourceAWB;
			iDataType = TFourCC(' ','A','W','B');
			iSampleRate = 16000;
            if (!iAudioUtility)
                {
	  			iAudioUtility = CAWBAudioControllerUtility::NewL();
                }
			break;

		default:
			User::Leave(KErrNotSupported);
			break;
		}

	if (iAudioResource)
	    {
	    delete iAudioResource;
	    iAudioResource = NULL;
	    }

	iAudioResource = C3GPAudioPlayControllerResource::NewL(codecType);
    TAapProperties aapProperties = iAudioResource->PropertiesL();
    iSharedBufferMaxNum = aapProperties.iSharedBufferMaxNum;
    iSharedBufferMaxSize = aapProperties.iSharedBufferMaxSize;
    iSharedBufferMaxSizeForNonSeekableSrc = aapProperties.iSharedBufferMaxSizeForNonSeekableSrc;
    iMetaDataSupport = aapProperties.iMetaDataSupport;
	iChannels = (aapProperties.iStereoSupport < 2) ? 1 : 2;
	iSinkNumChannels = iChannels; // Default same as source
	
   	if (iSharedBufferMaxNum < 3)
        {
        iSharedBufferMaxNum = 3;
   	    }
	
    CMMFDataBuffer* tempBuffer = CreateSourceBufferOfSizeLC(KFirstBufferSize);
    tempBuffer->Data().Copy(iSharedBuffers[0]->Data());

    ResetSharedBuffersL(iSharedBufferMaxNum, iSharedBufferMaxSize);
	iSharedBuffers[0]->Data().Copy(tempBuffer->Data());
	iSharedBuffers[0]->SetStatus(EFull);
    CleanupStack::PopAndDestroy(tempBuffer);
    for (TInt i=1; i < iSharedBufferMaxNum; i++)
        {
        CAdvancedAudioPlayController::FillSharedBufferL(static_cast<CMMFBuffer*>(iSharedBuffers[i]));
        }
	}

TInt C3GPAudioPlayControllerPlugin::GetCodecConfigData(RArray<TInt>& aCodecConfigData)
    {
    TInt stat = KErrNone;
    if (iAudioType == MP4_TYPE_MPEG4_AUDIO)
    	{
		iChannels = iNoOfChannels;
		iSinkNumChannels = iNoOfChannels;

		aCodecConfigData[0] = iSampleRate; //Input Sampling frequency
		aCodecConfigData[1] = iNoOfChannels;
		aCodecConfigData[2] = iProfile;	//Use eAAC+ Profile
		aCodecConfigData[3] = iOutFrameSize;
		aCodecConfigData[4] = iNoOfSamples;
		aCodecConfigData[5] = iSamplingFrequency;

		aCodecConfigData[7] = iDownSampledMode;
		
		aCodecConfigData[9] = iOutSamplingFrequency;
		aCodecConfigData[10] = iExtObjectType;

		aCodecConfigData[12] = CEAacPlusUtil::EAacPlusFormatRaw; // this is only available after scan header
    	}
    return stat;
    }

// __________________________________________________________________________
// Exported proxy for instantiation method resolution
// Define the interface UIDs

/**
*
* ImplementationTable
*
*/
const TImplementationProxy ImplementationTable[] =
	{
		// defined in 3GPAudioPlayControllerPluginUIDs.hrh
	    IMPLEMENTATION_PROXY_ENTRY(KUID3GPPlayControllerPluginImplementation, C3GPAudioPlayControllerPlugin::NewL)
	};

/**
* ImplementationGroupProxy
* @param aTableCount
* @returns "TImplementationProxy*"
*/

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}

// End of file

