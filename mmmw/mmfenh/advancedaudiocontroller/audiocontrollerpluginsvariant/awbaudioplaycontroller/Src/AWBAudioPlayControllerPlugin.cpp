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
* Description:  Play controller for AWB audio formats.
*
*/


// INCLUDES
#include "AWBAudioPlayControllerPlugin.h"
#include "AWBAudioPlayControllerPlugin.hrh"
#include "AWBAudioControllerUtility.h"
#include "AWBAudioPlayControllerDecoderBuilder.h"
#include "AWBAudioPlayControllerResource.h"
#include "DevSoundAudioOutput.h"
#include "FileAudioOutput.h"
#include "DebugMacros.h"

// CONSTANTS
//const TInt KAapBufferSize = 8*1024;
const TUint KMonoChannel        = 1;
const TUint KStereoChannel      = 2;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerPlugin::CAWBAudioPlayControllerPlugin
// -----------------------------------------------------------------------------
//
CAWBAudioPlayControllerPlugin::CAWBAudioPlayControllerPlugin()
	:	iDecoderBuilder(NULL)
	{
    }

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerPlugin::ConstructL
// -----------------------------------------------------------------------------
//
void CAWBAudioPlayControllerPlugin::ConstructL()
    {
    CAdvancedAudioPlayController::ConstructL();

	iAudioResource = CAWBAudioPlayControllerResource::NewL();
    TAapProperties aapProperties = iAudioResource->PropertiesL();
    iSharedBufferMaxNum = aapProperties.iSharedBufferMaxNum;
    iSharedBufferMaxSize = aapProperties.iSharedBufferMaxSize;
    iMetaDataSupport = aapProperties.iMetaDataSupport;
    iSharedBufferMaxSizeForNonSeekableSrc = aapProperties.iSharedBufferMaxSizeForNonSeekableSrc;
	iAudioUtility = CAWBAudioControllerUtility::NewL();
	iDecoderBuilder = CAWBAudioPlayControllerDecoderBuilder::NewL();
    }

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerPlugin::NewL
// -----------------------------------------------------------------------------
//
CAWBAudioPlayControllerPlugin* CAWBAudioPlayControllerPlugin::NewL()
    {
	DP0(_L("CAWBAudioPlayControllerPlugin::NewL"));
    CAWBAudioPlayControllerPlugin* self = new(ELeave) CAWBAudioPlayControllerPlugin;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CAWBAudioPlayControllerPlugin::~CAWBAudioPlayControllerPlugin()
    {
    DP0(_L("CAWBAudioPlayControllerPlugin::~CAWBAudioPlayControllerPlugin"));
    if (iIsDRMProtected)
		{
		delete iDataSource;
		}

    if ((iState != EStopped) && iAudioOutput)
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
// CAWBAudioPlayControllerPlugin::DoAddDataSourceL
// Initialization of Clip properties. We build a table needed for positioning.
// -----------------------------------------------------------------------------
//
void CAWBAudioPlayControllerPlugin::DoAddDataSourceL()
    {
DP0(_L("CAWBAudioPlayControllerPlugin::DoAddDataSourceL"));
    // ou1cimx1#205863
    if (iSourceType != KUidMmfFileSource) 
    	{
	    DP0(_L("CAWBAudioPlayControllerPlugin::DoAddDataSourceL not file source"));        
	    if (iSharedBufferMaxNum <= 2)
	        {
	        	iSharedBufferMaxNum = 3;
	        }
		 iSharedBufferMaxSize = iSharedBufferMaxSizeForNonSeekableSrc;
	    DP2(_L("CAWBAudioPlayControllerPlugin::DoAddDataSourceL new iSharedBufferMaxNum[%d] iSharedBufferMaxSize[%d]"), iSharedBufferMaxNum, iSharedBufferMaxSize);
    	}

    }

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerPlugin::DoAddDataSinkL
// -----------------------------------------------------------------------------
//
void CAWBAudioPlayControllerPlugin::DoAddDataSinkL()
    {
	DP0(_L("CAWBAudioPlayControllerPlugin::DoAddDataSinkL"));
    if (iDataSink->DataSinkType() == KUidMmfAudioOutput)
		{
		iAudioOutput = CDevSoundAudioOutput::NewL(iPrioritySettings, *this, *iMMFDevSound);
		iDecoderType = KAWBDecoder;
        }
    else if (iDataSink->DataSinkType() == KUidMmfFileSink)
        {
		iAudioOutput = CFileAudioOutput::NewL(*iDataSink, iPrioritySettings, *this);
		iDecoderType = KAWBConverter;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerPlugin::DoReadHeaderL
// -----------------------------------------------------------------------------
//
void CAWBAudioPlayControllerPlugin::DoReadHeaderL(CMMFDataBuffer* aBuffer)
    {
    DP0(_L("CAWBAudioPlayControllerPlugin::DoReadHeaderL"));

  	iAudioUtility->ScanHeaderL(aBuffer);
	
	iHeaderOffset = iAudioUtility->HeaderOffset();
	iSyncOffset = iAudioUtility->SyncOffset();
	iSampleRate = iAudioUtility->SampleRate();
	iSinkSampleRate = iSampleRate; // Default same as source
//	iBitRate = iAudioUtility->BitRate();
	iChannels = iAudioUtility->Channels();
//	iSinkNumChannels = iChannels; // Default same as source
//	iDuration = iAudioUtility->ScanDuration();
	iDataType = TFourCC(' ','A','W','B');

    if (!(iChannels == KMonoChannel || iChannels == KStereoChannel))
       {
       User::Leave(KErrNotSupported);
       }
        	
//	iCurrentPosition = iHeaderOffset;
//	iReadHeader = EFalse;
    }

CAdvancedAudioDecoder* CAWBAudioPlayControllerPlugin::BuildDecoderL()
	{
	return iDecoderBuilder->BuildDecoderL(iDecoderType);
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// Exported proxy for instantiation method resolution
// Define the interface UIDs
const TImplementationProxy ImplementationTable[] =
    {
	    IMPLEMENTATION_PROXY_ENTRY(KUIDAWBPlayControllerPluginImplementation, CAWBAudioPlayControllerPlugin::NewL)
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
