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
* Description:  Record controller for AMR audio formats.
*
*/


// INCLUDES
#include	"AMRAudioRecordControllerPlugin.h"
#include	"AMRAudioRecordControllerPlugin.hrh"
#include 	"AMRAudioRecordControllerEncoder.h"
#include 	"AMRAudioRecordControllerResource.h"
#include 	"AMRAudioControllerUtility.h"
#include 	"DevSoundAudioInput.h"
#include	<mmffile.h>


// CONSTANTS
const TInt KAapBufferSize = 8*1024;
_LIT8(KAmrMagicNumber, "#!AMR\n");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::CAMRAudioRecordControllerPlugin
// -----------------------------------------------------------------------------
//
CAMRAudioRecordControllerPlugin::CAMRAudioRecordControllerPlugin()
    {
    }

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::ConstructL
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::ConstructL()
    {
    CAdvancedAudioRecordController::ConstructL();

	iAudioResource = CAMRAudioRecordControllerResource::NewL();
    TAapProperties aapProperties = iAudioResource->PropertiesL();
    iSharedBufferMaxNum = aapProperties.iSharedBufferMaxNum;
    iSharedBufferMaxSize = aapProperties.iSharedBufferMaxSize;
    iSharedBufferMaxSizeForNonSeekableSrc = aapProperties.iSharedBufferMaxSizeForNonSeekableSrc;
	iAudioUtility = CAMRAudioControllerUtility::NewL();
    }

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::NewL
// -----------------------------------------------------------------------------
//
CAMRAudioRecordControllerPlugin* CAMRAudioRecordControllerPlugin::NewL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAMRAudioRecordControllerPlugin::NewL"));
#endif
    CAMRAudioRecordControllerPlugin* self = new(ELeave) CAMRAudioRecordControllerPlugin;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CAMRAudioRecordControllerPlugin::~CAMRAudioRecordControllerPlugin()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAMRAudioRecordControllerPlugin::~CAMRAudioRecordControllerPlugin"));
#endif

	// Has recording been started ?
	if ( iDataSink )
		{
		CMMFClip* clip = STATIC_CAST(CMMFClip*, iDataSink);

		if ( clip && clip->DataSinkType() == KUidMmfFileSink )
			{
			// We're going to remove the file if this wasn't a successful session.
			// How do we know if this was a successfull session? Well if we
			// wrote more data than merely the header its a good bet.

			if ( iAudioUtility->HeaderOffset() >= clip->Size() ) //no data written
				{
				if ( !iClipAlreadyExists ) // this is a new file
					{
#ifdef _DEBUG
					RDebug::Print (_L ("CAMRAudioRecordControllerPlugin::~CAMRAudioRecordControllerPlugin :-> clip->SetSize(0)"));
#endif
					// delete the file (for now, set the file size to zero)
					// Note: Deleting the file causes S60 Voice Recorder
					//       to hang in the record state
//					STATIC_CAST(CMMFFile*, iDataSink)->Delete() ;

					// Don't set the file size to 0, so  client will not re-use 
					// this file for the next recording activity
//					clip->SetSize(0) ;
					}
				}
			}
		}

    delete iAudioUtility;
    delete iAudioResource;
    delete iAudioInput;

	iSharedBuffers.ResetAndDestroy();
	iSharedBuffers.Close();
    }

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::DoAddDataSourceL
// Initialization of Clip properties. We build a table needed for positioning.
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::DoAddDataSourceL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerPlugin::DoAddDataSourceL"));
#endif
    if ( !iMMFDevSound )
        {
        User::Leave(KErrNotReady);
        }    
	iAudioInput = CDevSoundAudioInput::NewL(iPrioritySettings, *this, *iMMFDevSound);

	CAMRAudioRecordControllerEncoder* encoder = CAMRAudioRecordControllerEncoder::NewL();
	// AudioInput takes ownership of encoder object
	iAudioInput->SetEncoder(encoder);
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::DoAddDataSinkL
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::DoAddDataSinkL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerPlugin::DoAddDataSinkL"));
#endif
    if ( !iAudioUtility )
        {
 //       User::Leave(KErrNotReady);
        iAudioUtility = CAMRAudioControllerUtility::NewL();
        }  
    iClipAlreadyExists = EFalse ;

    CMMFClip* clip = STATIC_CAST(CMMFClip*, iDataSink);
	clip->SinkPrimeL();

    CMMFDataBuffer* buf = CreateSourceBufferOfSizeLC(KAapBufferSize);
    clip->ReadBufferL(buf, 0);

    if ( clip->Size() )
        {
        iAudioUtility->ScanFramesL(clip, buf);
        // file exists
        iClipAlreadyExists = ETrue ;
        }

    iSampleRate = iAudioUtility->SampleRate();
    iBitRate = iAudioUtility->BitRate();
    iChannels = iAudioUtility->Channels();
    iDuration = iAudioUtility->ScanDuration();
    iTimePositionInMicroSecs = iDuration;
    iSinkWritePosition = clip->Size();
    iDataType = TFourCC(' ','A','M','R');
    iDriveNumber = iAudioUtility->GetDriveNumber(STATIC_CAST(CMMFFile*, iDataSink)->FileDrive());

    if ( !iSinkWritePosition )
        {
        WriteHeaderL();
        iSinkWritePosition = iAudioUtility->HeaderOffset();
        }

    CleanupStack::PopAndDestroy(buf);
    clip->SinkStopL();
    }

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::StopL
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::StopL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerPlugin::StopL [state=%d]"), iState);
#endif
    switch ( iState )
        {
        case ERecording:
            iState = EStopping;
            DoStopL();
            //SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, KErrNone));
            break;

        case EWritingStopped:
            DoStopL();
            break;

        case EStopped:
        case EStopping:
            break;

        default:
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerPlugin::StopL Panic iState [%d]"), iState);
#endif
            Panic(KErrGeneral);
            break;
        }
    }
            
// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::WriteHeaderL
// Writes the AMR magic number to the beginning of an AMR clip.
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::WriteHeaderL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAMRAudioRecordControllerPlugin::WriteHeaderL"));
#endif
    if ( !iAudioUtility )
        {
        User::Leave(KErrNotReady);
        }  
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL(KAmrMagicNumber().Size());
    CleanupStack::PushL(buffer);

    buffer->Data() = KAmrMagicNumber();
    buffer->SetRequestSizeL(KAmrMagicNumber().Size());
    WriteIfEnoughSpaceL(buffer, 0);
    iAudioUtility->ScanFramesL(STATIC_CAST(CMMFClip*, iDataSink), buffer, ETrue);

    CleanupStack::PopAndDestroy(buffer);
    }

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::MacSetSinkBitRateL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::MacSetSinkBitRateL(
	TUint aRate )
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAMRAudioRecordControllerPlugin::MacSetSinkBitRateL [%d]"), aRate);
#endif
    if ( !iDataSink || !iDataSource || !iAudioInput )
        {
        User::Leave(KErrNotReady);
        }

	const RArray<TUint>& supportedBitRates = STATIC_CAST(CAMRAudioRecordControllerResource*, iAudioResource)->SupportedBitRatesL();

   	if (supportedBitRates.Find(aRate) >= 0 )
   		{
		iBitRate = aRate;
		// Read the default codec configuration parameters from resource file
		RArray<TInt>& codecConfigData = CONST_CAST(RArray<TInt>&, iAudioResource->CodecConfigParametersL());
		iAudioInput->SetBitRateL(aRate, codecConfigData);
   		}
   	else
   		{
		// aRate is not one of the supportedBitRates!
		User::Leave(KErrNotSupported);
   		}	
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::MacGetSupportedSinkBitRatesL
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::MacGetSupportedSinkBitRatesL(
	RArray<TUint>& aSupportedRates )
    {
    if ( !iDataSink )
        {
        User::Leave(KErrNotReady);
        }

    aSupportedRates.Reset();
	const RArray<TUint>& supportedBitRates = STATIC_CAST(CAMRAudioRecordControllerResource*, iAudioResource)->SupportedBitRatesL();
	TInt count = supportedBitRates.Count();
	for ( TInt i = 0; i < count; i++ )
		{
		aSupportedRates.AppendL(supportedBitRates[i]);
		}
    }

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::MacGetSupportedSinkSampleRatesL
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::MacGetSupportedSinkSampleRatesL(
    RArray<TUint>& aSupportedRates )
    {
    if ( !iDataSink || !iAudioInput )
        {
        User::Leave(KErrNotReady);
        }

    aSupportedRates.Reset();

    const RArray<TUint>& sinkSupportedSampleRates = STATIC_CAST(CAMRAudioRecordControllerResource*, iAudioResource)->SupportedSampleRatesL();

    RArray<TUint> sourceSupportedSampleRates;
    CleanupClosePushL(sourceSupportedSampleRates);

    iAudioInput->CapabilitiesRatesL(sourceSupportedSampleRates);
	TInt count = sinkSupportedSampleRates.Count();

    for (TInt i = 0; i < count; i++)
        {
       	if (sourceSupportedSampleRates.Find(sinkSupportedSampleRates[i]) >= 0 )
       		{
            aSupportedRates.AppendL(sinkSupportedSampleRates[i]);
       		}
        }
    CleanupStack::PopAndDestroy(&sourceSupportedSampleRates);
    }

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::MacSetSinkSampleRateL
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::MacSetSinkSampleRateL(
    TUint aSampleRate )
    {

    if ( !iDataSink || !iAudioInput)
        {
        User::Leave(KErrNotReady);
        }

    const RArray<TUint>& sinkSupportedSampleRates = STATIC_CAST(CAMRAudioRecordControllerResource*, iAudioResource)->SupportedSampleRatesL();

    RArray<TUint> sourceSupportedSampleRates;
    CleanupClosePushL(sourceSupportedSampleRates);

    iAudioInput->CapabilitiesRatesL( sourceSupportedSampleRates );

	if ((sourceSupportedSampleRates.Find(aSampleRate) >= 0 ) &&
		(sinkSupportedSampleRates.Find(aSampleRate) >= 0))
		{ // supports the rate
        iSampleRate = aSampleRate;
		}
	else
		{
	    User::Leave(KErrNotSupported);
		}
    CleanupStack::PopAndDestroy(&sourceSupportedSampleRates);
    }
    
// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::MacGetSupportedSinkNumChannelsL
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::MacGetSupportedSinkNumChannelsL(
	RArray<TUint>& aSupportedChannels )
    {
    if ( !iDataSink || !iAudioInput || !iAudioResource)
        {
        User::Leave(KErrNotReady);
        }
	
	aSupportedChannels.Reset();
    TAapProperties aapProperties = iAudioResource->PropertiesL();
    TInt channels = aapProperties.iStereoSupport+1; // num channels supported by encoder

	RArray<TUint> srcChannels;
    CleanupClosePushL(srcChannels);

    iAudioInput->CapabilitiesChannelsL(srcChannels); // channels supported by source
    for (TInt i = 1; i <= channels; i++) // check source for 1 to n channels supported by encoder
        {
	    if ( srcChannels.Find(i) >= 0 )
	    	{
	        aSupportedChannels.AppendL(i);
	    	}
        }
    CleanupStack::PopAndDestroy(&srcChannels);
    }

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerPlugin::MacSetSinkNumChannelsL
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerPlugin::MacSetSinkNumChannelsL(
	TUint aNumChannels )
    {
    if ( !iDataSink )
        {
        User::Leave(KErrNotReady);
        }

	RArray<TUint> supportedChannels;
    CleanupClosePushL(supportedChannels);

	MacGetSupportedSinkNumChannelsL(supportedChannels);
    if ( supportedChannels.Find(aNumChannels) >= 0 )
        {
        iChannels = aNumChannels;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    CleanupStack::PopAndDestroy(&supportedChannels);
    }

    
// ========================== OTHER EXPORTED FUNCTIONS =========================

// Exported proxy for instantiation method resolution
// Define the interface UIDs
const TImplementationProxy ImplementationTable[] =
    {
	    IMPLEMENTATION_PROXY_ENTRY(KUIDAMRRecordControllerPluginImplementation, CAMRAudioRecordControllerPlugin::NewL)
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
