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
* Description:  Play controller for MP3 audio formats.
*
*/


// INCLUDES
#include "MP3AudioPlayControllerPlugin.h"
#include "MP3AudioPlayControllerPlugin.hrh"
#include "MP3AudioControllerUtility.h"
#include "MP3AudioPlayControllerDecoderBuilder.h"
#include "MP3AudioPlayControllerResource.h"
#include "DevSoundAudioOutput.h"
#include "FileAudioOutput.h"
#include "DebugMacros.h"


// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerPlugin::CMP3AudioPlayControllerPlugin
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerPlugin::CMP3AudioPlayControllerPlugin()
	:	iDecoderBuilder(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerPlugin::ConstructL
// -----------------------------------------------------------------------------
//
void CMP3AudioPlayControllerPlugin::ConstructL()
    {
    CAdvancedAudioPlayController::ConstructL();

	iAudioResource = CMP3AudioPlayControllerResource::NewL();
    TAapProperties aapProperties = iAudioResource->PropertiesL();
    iSharedBufferMaxNum = aapProperties.iSharedBufferMaxNum;
    iSharedBufferMaxSize = aapProperties.iSharedBufferMaxSize;
    iMetaDataSupport = aapProperties.iMetaDataSupport;
    iSharedBufferMaxSizeForNonSeekableSrc = aapProperties.iSharedBufferMaxSizeForNonSeekableSrc;
	iAudioUtility = CMP3AudioControllerUtility::NewL();
	iDecoderBuilder = CMP3AudioPlayControllerDecoderBuilder::NewL();
    }

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerPlugin::NewL
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerPlugin* CMP3AudioPlayControllerPlugin::NewL()
    {
	DP0(_L("CMP3AudioPlayControllerPlugin::NewL"));
    CMP3AudioPlayControllerPlugin* self = new(ELeave) CMP3AudioPlayControllerPlugin;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CMP3AudioPlayControllerPlugin::~CMP3AudioPlayControllerPlugin()
    {
	DP0(_L("CMP3AudioPlayControllerPlugin::~CMP3AudioPlayControllerPlugin"));
   
    if (iIsDRMProtected)
		{
		delete iDataSource;
		}

    if ((iState != EStopped) && (iState != EInitialized) && iAudioOutput)
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
// CMP3AudioPlayControllerPlugin::DoAddDataSourceL
// Initialization of Clip properties. We build a table needed for positioning.
// -----------------------------------------------------------------------------
//
void CMP3AudioPlayControllerPlugin::DoAddDataSourceL()
    {
    DP0(_L("CMP3AudioPlayControllerPlugin::DoAddDataSourceL"));
    // ou1cimx1#205863
    if (iSourceType != KUidMmfFileSource) 
    	{
	    DP0(_L("CMP3AudioPlayControllerPlugin::DoAddDataSourceL not file source"));        
	    if (iSharedBufferMaxNum <= 2)
	        {
	        	iSharedBufferMaxNum = 3;
	        }
		 iSharedBufferMaxSize = iSharedBufferMaxSizeForNonSeekableSrc;
	    DP2(_L("CMP3AudioPlayControllerPlugin::DoAddDataSourceL new iSharedBufferMaxNum[%d] iSharedBufferMaxSize[%d]"), iSharedBufferMaxNum, iSharedBufferMaxSize);
    	}

    }

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerPlugin::DoAddDataSinkL
// -----------------------------------------------------------------------------
//
void CMP3AudioPlayControllerPlugin::DoAddDataSinkL()
    {
    DP0(_L("CMP3AudioPlayControllerPlugin::DoAddDataSinkL"));
    if (iDataSink->DataSinkType() == KUidMmfAudioOutput)
		{
		iAudioOutput = CDevSoundAudioOutput::NewL(iPrioritySettings, *this, *iMMFDevSound);
		iDecoderType = KMP3Decoder;
        }
    else if (iDataSink->DataSinkType() == KUidMmfFileSink)
        {
		iAudioOutput = CFileAudioOutput::NewL(*iDataSink, iPrioritySettings, *this);
		iDecoderType = KMP3Converter;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerPlugin::DoReadHeaderL
// -----------------------------------------------------------------------------
//
void CMP3AudioPlayControllerPlugin::DoReadHeaderL(CMMFDataBuffer* aBuffer)
    {
    DP0(_L("CMP3AudioPlayControllerPlugin::DoReadHeaderL"));

    iAudioUtility->ScanHeaderL(aBuffer);

    iHeaderOffset = iAudioUtility->HeaderOffset();
    iSyncOffset = iAudioUtility->SyncOffset();
    iSampleRate = iAudioUtility->SampleRate();
    iSinkSampleRate = iSampleRate; // Default same as source
//    iBitRate = iAudioUtility->BitRate();
//    iDuration = iAudioUtility->Duration();
    iChannels = iAudioUtility->Channels();
//    iSinkNumChannels = iChannels; // Default same as source
//    iSinkNumChannels = iAudioUtility->ChannelsOut();
    iDataType = TFourCC(' ','M','P','3');
    
    if (!(iChannels == 1 || iChannels == 2))
        {
        User::Leave(KErrNotSupported);
        }
    
//    iCurrentPosition = iHeaderOffset;
//    iReadHeader = EFalse;
    }

CAdvancedAudioDecoder* CMP3AudioPlayControllerPlugin::BuildDecoderL()
	{
	return iDecoderBuilder->BuildDecoderL(iDecoderType);
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// Exported proxy for instantiation method resolution
// Define the interface UIDs
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(KUIDMP3PlayControllerPluginImplementation, CMP3AudioPlayControllerPlugin::NewL)
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
