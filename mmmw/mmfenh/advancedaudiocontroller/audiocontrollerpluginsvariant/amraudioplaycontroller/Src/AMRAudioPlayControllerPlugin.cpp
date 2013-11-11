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
* Description:  Play controller for AMR audio formats.
*
*/


// INCLUDES
#include "AMRAudioPlayControllerPlugin.h"
#include "AMRAudioPlayControllerPlugin.hrh"
#include "AMRAudioControllerUtility.h"
#include "AMRAudioPlayControllerDecoderBuilder.h"
#include "AMRAudioPlayControllerResource.h"
#include "DevSoundAudioOutput.h"
#include "FileAudioOutput.h"
#include "DebugMacros.h"
#include <mmfformatimplementationuids.hrh>

// CONSTANTS
//const TInt KAapBufferSize       = 8*1024;
const TUint KMonoChannel        = 1;
const TUint KStereoChannel      = 2;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerPlugin::CAMRAudioPlayControllerPlugin
// -----------------------------------------------------------------------------
//
CAMRAudioPlayControllerPlugin::CAMRAudioPlayControllerPlugin()
	:	iDecoderBuilder(NULL)
	{
    }

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerPlugin::ConstructL
// -----------------------------------------------------------------------------
//
void CAMRAudioPlayControllerPlugin::ConstructL()
    {
    CAdvancedAudioPlayController::ConstructL();

	  iAudioResource = CAMRAudioPlayControllerResource::NewL();
    TAapProperties aapProperties = iAudioResource->PropertiesL();
    iSharedBufferMaxNum = aapProperties.iSharedBufferMaxNum;
    iSharedBufferMaxSize = aapProperties.iSharedBufferMaxSize;
    iMetaDataSupport = aapProperties.iMetaDataSupport;
  	iAudioUtility = CAMRAudioControllerUtility::NewL();
	  iDecoderBuilder = CAMRAudioPlayControllerDecoderBuilder::NewL();
    }

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerPlugin::NewL
// -----------------------------------------------------------------------------
//
CAMRAudioPlayControllerPlugin* CAMRAudioPlayControllerPlugin::NewL()
    {
	DP0(_L("CAMRAudioPlayControllerPlugin::NewL"));
    CAMRAudioPlayControllerPlugin* self = new(ELeave) CAMRAudioPlayControllerPlugin;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CAMRAudioPlayControllerPlugin::~CAMRAudioPlayControllerPlugin()
    {
	DP0(_L("CAMRAudioPlayControllerPlugin::~CAMRAudioPlayControllerPlugin"));
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
// CAMRAudioPlayControllerPlugin::DoAddDataSourceL
// Initialization of Clip properties. We build a table needed for positioning.
// -----------------------------------------------------------------------------
//
void CAMRAudioPlayControllerPlugin::DoAddDataSourceL()
    {
    DP0(_L("CAMRAudioPlayControllerPlugin::DoAddDataSourceL"));
    }

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerPlugin::DoAddDataSinkL
// -----------------------------------------------------------------------------
//
void CAMRAudioPlayControllerPlugin::DoAddDataSinkL()
    {
    DP0(_L("CAMRAudioPlayControllerPlugin::DoAddDataSinkL"));
    if (iDataSink->DataSinkType() == KUidMmfAudioOutput)
		{
		iAudioOutput = CDevSoundAudioOutput::NewL(iPrioritySettings, *this, *iMMFDevSound);
		iDecoderType = KAMRDecoder;
        }
    else if (iDataSink->DataSinkType() == KUidMmfFileSink)
        {
		iAudioOutput = CFileAudioOutput::NewL(*iDataSink, iPrioritySettings, *this);
		iDecoderType = KAMRConverter;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerPlugin::DoReadHeaderL
// -----------------------------------------------------------------------------
//
void CAMRAudioPlayControllerPlugin::DoReadHeaderL(CMMFDataBuffer* aBuffer)
    {
    DP0(_L("CAMRAudioPlayControllerPlugin::DoReadHeaderL"));
	// While parsing past header or id3 data, ScanHeaderL will leave with KErrNotReady.
	// ScanHeaderL will leave if the util still wants more data for averaging
	// by returning KErrCompletion.
	// We need to trap so that we can save the config info and then
	// do the leave below.
    TRAPD(err, iAudioUtility->ScanHeaderL(aBuffer));

    if(err != KErrNotReady)
	    {
	    iHeaderOffset = iAudioUtility->HeaderOffset();
	    iSyncOffset = iAudioUtility->SyncOffset();
	    iSampleRate = iAudioUtility->SampleRate();
	    iSinkSampleRate = iSampleRate; // Default same as source
	//    iBitRate = iAudioUtility->BitRate();
	    iChannels = iAudioUtility->Channels();
	//    iSinkNumChannels = iChannels; // Default same as source
	//    iDuration = iAudioUtility->ScanDuration();
	    iDataType = TFourCC(' ','A','M','R');
	    iSourceFormat = TUid::Uid(KAdvancedUidFormatAMRRead); // what is this for ???
	
	    if (!(iChannels == KMonoChannel || iChannels == KStereoChannel))
	       {
	       User::Leave(KErrNotSupported);
	       }
		}

	User::LeaveIfError(err);
        	
//	iCurrentPosition = iHeaderOffset;
//	iReadHeader = EFalse;

    }

CAdvancedAudioDecoder* CAMRAudioPlayControllerPlugin::BuildDecoderL()
	{
	return iDecoderBuilder->BuildDecoderL(iDecoderType);
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// Exported proxy for instantiation method resolution
// Define the interface UIDs
const TImplementationProxy ImplementationTable[] =
    {
	    IMPLEMENTATION_PROXY_ENTRY(KUIDAMRPlayControllerPluginImplementation, CAMRAudioPlayControllerPlugin::NewL)
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
