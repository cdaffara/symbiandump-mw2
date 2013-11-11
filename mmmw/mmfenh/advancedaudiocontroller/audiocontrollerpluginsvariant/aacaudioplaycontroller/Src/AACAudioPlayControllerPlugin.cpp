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
* Description:  Play controller for AAC audio formats.
*
*/


// INCLUDES
#include "AACAudioPlayControllerPlugin.h"
#include "AACAudioPlayControllerPlugin.hrh"
#include "AACAudioPlayControllerDecoderBuilder.h"
#include "AACAudioPlayControllerResource.h"
#include "DevSoundAudioOutput.h"
#include "FileAudioOutput.h"
#include "DebugMacros.h"


// only EAACPlusAudioControllerUtility support AAC optimization
//#ifdef __AAC_PLUS_DECODER_PLUGIN
#include "EAACPlusAudioControllerUtility.h"
/*
#else
#include "AACAudioControllerUtility.h"
#endif
*/

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerPlugin::CAACAudioPlayControllerPlugin
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerPlugin::CAACAudioPlayControllerPlugin()
	:	iDecoderBuilder(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerPlugin::ConstructL
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerPlugin::ConstructL()
    {
    CAdvancedAudioPlayController::ConstructL();

	iAudioResource = CAACAudioPlayControllerResource::NewL();
    TAapProperties aapProperties = iAudioResource->PropertiesL();
    iSharedBufferMaxNum = aapProperties.iSharedBufferMaxNum;
    iSharedBufferMaxSize = aapProperties.iSharedBufferMaxSize;
    iMetaDataSupport = aapProperties.iMetaDataSupport;
    iSharedBufferMaxSizeForNonSeekableSrc = aapProperties.iSharedBufferMaxSizeForNonSeekableSrc;
	iAudioUtility = CEAACPlusAudioControllerUtility::NewL();
	iDecoderBuilder = CAACAudioPlayControllerDecoderBuilder::NewL();
    }

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerPlugin::NewL
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerPlugin* CAACAudioPlayControllerPlugin::NewL()
    {
	DP0(_L("CAACAudioPlayControllerPlugin::NewL"));

    CAACAudioPlayControllerPlugin* self = new(ELeave) CAACAudioPlayControllerPlugin;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CAACAudioPlayControllerPlugin::~CAACAudioPlayControllerPlugin()
    {
	DP0(_L("CAACAudioPlayControllerPlugin::~CAACAudioPlayControllerPlugin"));

    if (iIsDRMProtected)
		{
		delete iDataSource;
		}

    if ((iState != EStopped) && (iState != EInitializing) && iAudioOutput)
        {
        TRAPD(err, iAudioOutput->StopL());
		err = err;
        }

	delete iDecoderBuilder;
    delete iAudioUtility;
    delete iAudioResource;
    delete iAudioOutput;
    }

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerPlugin::DoAddDataSourceL
// Initialization of Clip properties. We build a table needed for positioning.
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerPlugin::DoAddDataSourceL()
    {
    DP0(_L("CAACAudioPlayControllerPlugin::DoAddDataSourceL"));
        // ou1cimx1#205863
    if (iSourceType != KUidMmfFileSource) 
    	{
	    DP0(_L("CAACAudioPlayControllerPlugin::DoAddDataSourceL not file source"));        
	    if (iSharedBufferMaxNum <= 2)
	        {
	        	iSharedBufferMaxNum = 3;
	        }
		 iSharedBufferMaxSize = iSharedBufferMaxSizeForNonSeekableSrc;
	    DP2(_L("CAACAudioPlayControllerPlugin::DoAddDataSourceL new iSharedBufferMaxNum[%d] iSharedBufferMaxSize[%d]"), iSharedBufferMaxNum, iSharedBufferMaxSize);
    	}

	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerPlugin::DoAddDataSinkL
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerPlugin::DoAddDataSinkL()
    {
    DP0(_L("CAACAudioPlayControllerPlugin::DoAddDataSinkL"));

    if (iDataSink->DataSinkType() == KUidMmfAudioOutput)
		{
		iAudioOutput = CDevSoundAudioOutput::NewL(iPrioritySettings, *this, *iMMFDevSound);
		iDecoderType = KAACDecoder;
        }
    else if (iDataSink->DataSinkType() == KUidMmfFileSink)
        {
		iAudioOutput = CFileAudioOutput::NewL(*iDataSink, iPrioritySettings, *this);
		iDecoderType = KAACConverter;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    }
    
// -----------------------------------------------------------------------------
// CAACAudioPlayControllerPlugin::DoReadHeaderL
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerPlugin::DoReadHeaderL(CMMFDataBuffer* aBuffer)
    { // update this to handle multiple buffers of header
    DP0(_L("CAACAudioPlayControllerPlugin::DoReadHeaderL"));

	iAudioUtility->ScanHeaderL(aBuffer);
    
    iHeaderOffset = iAudioUtility->HeaderOffset();
    iSyncOffset = iAudioUtility->SyncOffset();
    iSampleRate = iAudioUtility->SampleRate();
    iSinkSampleRate = iSampleRate; // Default same as source
    iChannels = iAudioUtility->Channels();
//    iSinkNumChannels = iAudioUtility->ChannelsOut();
    iDataType = TFourCC(' ','E','A','C');

    if (!(iChannels == 0 || iChannels == 1 || iChannels == 2))
        {
        User::Leave(KErrNotSupported);
        }
    }

CAdvancedAudioDecoder* CAACAudioPlayControllerPlugin::BuildDecoderL()
	{
	return iDecoderBuilder->BuildDecoderL(iDecoderType);
	}


// ========================== OTHER EXPORTED FUNCTIONS =========================

// Exported proxy for instantiation method resolution
// Define the interface UIDs
const TImplementationProxy ImplementationTable[] =
    {
	    IMPLEMENTATION_PROXY_ENTRY(KUIDAACPlayControllerPluginImplementation, CAACAudioPlayControllerPlugin::NewL)
    };

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

// End of file
