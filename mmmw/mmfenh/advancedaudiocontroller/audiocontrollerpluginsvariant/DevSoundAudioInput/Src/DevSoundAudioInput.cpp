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
* Description:  The functions in class implements the behavior that is
*                specific to the interface with DevSound.
*
*/



// INCLUDE FILES
#include 	"DevSoundAudioInput.h"
#include 	<mmfpaniccodes.h>
#include    "DebugMacros.h"

// CONSTANTS
const TUint KSampleRate8000Hz 	= 8000;
const TUint KSampleRate11025Hz	= 11025;
const TUint KSampleRate12000Hz  = 12000;
const TUint KSampleRate16000Hz 	= 16000;
const TUint KSampleRate22050Hz 	= 22050;
const TUint KSampleRate24000Hz  = 24000;
const TUint KSampleRate32000Hz 	= 32000;
const TUint KSampleRate44100Hz 	= 44100;
const TUint KSampleRate48000Hz 	= 48000;
const TUint KSampleRate88200Hz 	= 88200;
const TUint KSampleRate96000Hz 	= 96000;

const TUint KNumChannelsMono 	= 1;
const TUint KNumChannelsStereo 	= 2;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::CDevSoundAudioInput
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CDevSoundAudioInput::CDevSoundAudioInput(CMMFDevSound& aMMFDevSound)
	:	iMMFDevSound(aMMFDevSound)
    {
	iAdvancedAudioEncoder = NULL;
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::ConstructL(
	const TMMFPrioritySettings& aPrioritySettings,
	MAdvancedAudioInputObserver& aObserver )
    {
    DP0(_L("CDevSoundAudioInput::ConstructL"));
	iPrioritySettings = aPrioritySettings;
	iObserver = &aObserver;

	iState = EIdle;
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CDevSoundAudioInput* CDevSoundAudioInput::NewL(
    const TMMFPrioritySettings& aPrioritySettings,
    MAdvancedAudioInputObserver& aObserver,
    CMMFDevSound& aMMFDevSound )
    {
    DP0(_L("CDevSoundAudioInput::NewL"));
    CDevSoundAudioInput* self = new (ELeave) CDevSoundAudioInput(aMMFDevSound);
    CleanupStack::PushL(self);
    self->ConstructL(aPrioritySettings, aObserver);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C CDevSoundAudioInput::~CDevSoundAudioInput()
    {
    DP0(_L("CDevSoundAudioInput::~CDevSoundAudioInput"));
	iCodecConfigData.Close();
	iPendingCodecConfigData.Close();
	delete iAdvancedAudioEncoder;

    iMMFDevSound.Stop();
    if (iBlockInitialize)
        {
        DP0(_L("CDevSoundAudioInput::~CDevSoundAudioInput unblocking initialize"));
        iBlockInitialize->AsyncStop();
        }
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::PrimeL
// Prepare to start recording, building configuration parameters and initializing DevSound.
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::PrimeL()
    {
    DP0(_L("CDevSoundAudioInput::PrimeL"));

    iDevSoundInitialized = EFalse; //Reset

	if ( !iAdvancedAudioEncoder )
		{
		// SetEncoder hasn't been called!!!
		User::Leave(KErrNotReady);
		}

	if ( iAdvancedAudioEncoder->IsHwAccelerated() )
		{
		iMMFDevSound.InitializeL(*this, iSinkFourCC, EMMFStateRecording);
		}
	else
		{
		iMMFDevSound.InitializeL(*this, KMMFFourCCCodePCM16, EMMFStateRecording);
		}
	if (!iDevSoundInitialized)
	    {
	    iBlockInitialize = new (ELeave) CActiveSchedulerWait();
	    DP0(_L("CDevSoundAudioInput::PrimeL() blocking for iMMFDevSound.InitializeL"));
	    iBlockInitialize->Start();
	    DP0(_L("CDevSoundAudioInput::PrimeL() continuing after iMMFDevSound.InitializeL"));
	    delete iBlockInitialize;
	    iBlockInitialize = NULL;
	    }
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::PauseL
// Send pause command to DevSound pause and set own state to paused
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::PauseL()
    {
    DP0(_L("CDevSoundAudioInput::PauseL"));

    iMMFDevSound.Pause();
    iState = EPaused;
    }


// -----------------------------------------------------------------------------
// CDevSoundAudioInput::RecordL
// Start recording process by configuring the codec and initialize record on DevSound.
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::RecordL(
	TAny* aBuffer )
	{
	DP0(_L("CDevSoundAudioInput::RecordL"));

    //DevSound should be initailized before recording.
    if(!iDevSoundInitialized)
    {
        User::Leave(KErrNotReady);
    }
       
	// Give the audio encoder a handle to the sink buffers
	iAdvancedAudioEncoder->SetSinkBuffers(REINTERPRET_CAST(RPointerArray<CMMFDataBuffer>*, aBuffer));
	// Reset the encoder
	iAdvancedAudioEncoder->ResetL();
	// Configure the audio encoder
	iAdvancedAudioEncoder->SetConfigL(iSinkSampleRate, iSinkChannels, iSinkSampleRate,
										iSinkChannels, iCodecConfigData);

	// Configure DevSound
	iMMFDevSound.SetConfigL(iDevSoundConfig);
//	iMMFDevSound.SetPrioritySettings(iPrioritySettings);

    iMMFDevSound.RecordInitL();
    iState = ERecord;
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::BuildConfigurationParameters
// Build a configuration profile based on DevSound capabilities
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::BuildConfigurationParameters()
	{
    DP0(_L("CDevSoundAudioInput::BuildConfigurationParameters"));

	// Query DevSound capabilities and Try to use DevSound sample rate and
	// mono/stereo capability
	TMMFCapabilities devSoundCaps = iMMFDevSound.Capabilities();
	// get current config
	iDevSoundConfig = iMMFDevSound.Config();

	// Default PCM16
	iDevSoundConfig.iEncoding = EMMFSoundEncoding16BitPCM;

	// 1 = Monophonic and 2 == Stereo
	if (((iSinkChannels == 1) && (devSoundCaps.iChannels & EMMFMono)) ||
		((iSinkChannels == 2) && (devSoundCaps.iChannels & EMMFStereo)))
		iDevSoundConfig.iChannels = iSinkChannels;


	// Check for std sample rates.
	if ((iSinkSampleRate == 96000) && (devSoundCaps.iRate & EMMFSampleRate96000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate96000Hz;
	else if ((iSinkSampleRate == 88200) && (devSoundCaps.iRate & EMMFSampleRate88200Hz))
		iDevSoundConfig.iRate = EMMFSampleRate88200Hz;
	else if ((iSinkSampleRate == 48000) && (devSoundCaps.iRate & EMMFSampleRate48000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate48000Hz;
	else if ((iSinkSampleRate == 44100) && (devSoundCaps.iRate & EMMFSampleRate44100Hz))
		iDevSoundConfig.iRate = EMMFSampleRate44100Hz;
	else if ((iSinkSampleRate == 32000) && (devSoundCaps.iRate & EMMFSampleRate32000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate32000Hz;
	else if ((iSinkSampleRate == 22050) && (devSoundCaps.iRate & EMMFSampleRate22050Hz))
		iDevSoundConfig.iRate = EMMFSampleRate22050Hz;
	else if ((iSinkSampleRate == 16000) && (devSoundCaps.iRate & EMMFSampleRate16000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate16000Hz;
	else if ((iSinkSampleRate == 11025) && (devSoundCaps.iRate & EMMFSampleRate11025Hz))
		iDevSoundConfig.iRate = EMMFSampleRate11025Hz;
	else if ((iSinkSampleRate == 8000) && (devSoundCaps.iRate & EMMFSampleRate8000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate8000Hz;
	else if ((iSinkSampleRate == 12000) && (devSoundCaps.iRate & EMMFSampleRate12000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate12000Hz;
	else if ((iSinkSampleRate == 24000) && (devSoundCaps.iRate & EMMFSampleRate24000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate24000Hz;
	else
		{
		// pick the maximum sample rate available
		if (devSoundCaps.iRate & EMMFSampleRate96000Hz)
			iDevSoundConfig.iRate = EMMFSampleRate96000Hz;
		else if (devSoundCaps.iRate & EMMFSampleRate88200Hz)
			iDevSoundConfig.iRate = EMMFSampleRate88200Hz;
		else if (devSoundCaps.iRate & EMMFSampleRate48000Hz)
			iDevSoundConfig.iRate = EMMFSampleRate48000Hz;
		else if (devSoundCaps.iRate & EMMFSampleRate44100Hz)
			iDevSoundConfig.iRate = EMMFSampleRate44100Hz;
		else if (devSoundCaps.iRate & EMMFSampleRate32000Hz)
			iDevSoundConfig.iRate = EMMFSampleRate32000Hz;
		else if (devSoundCaps.iRate & EMMFSampleRate24000Hz)
			iDevSoundConfig.iRate = EMMFSampleRate24000Hz;
		else if (devSoundCaps.iRate & EMMFSampleRate22050Hz)
			iDevSoundConfig.iRate = EMMFSampleRate22050Hz;
		else if (devSoundCaps.iRate & EMMFSampleRate16000Hz)
			iDevSoundConfig.iRate = EMMFSampleRate16000Hz;
		else if (devSoundCaps.iRate & EMMFSampleRate12000Hz)
			iDevSoundConfig.iRate = EMMFSampleRate12000Hz;
		else if (devSoundCaps.iRate & EMMFSampleRate11025Hz)
			iDevSoundConfig.iRate = EMMFSampleRate11025Hz;
		else if (devSoundCaps.iRate & EMMFSampleRate8000Hz)
			iDevSoundConfig.iRate = EMMFSampleRate8000Hz;
		else
			ASSERT(EFalse); // if we don't support any sample rates, there is not much we can do
		}
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::StopL
// Send stop command to DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::StopL()
    {
    DP0(_L("CDevSoundAudioInput::StopL"));
	if( iState != EIdle )
		{
		iState = EIdle;
		iAdvancedAudioEncoder->Stop();
		iMMFDevSound.Stop();
		}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::SetPrioritySettings
// Send new priority settings to DevSound. Priorities are enumerated to
// determine audio recording priority. Higher numbers mean high priority (can
// interrupt lower priorities).
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::SetPrioritySettingsL(
	const TMMFPrioritySettings& aPrioritySettings )
    {
    DP0(_L("CDevSoundAudioInput::SetPrioritySettingsL"));
    iPrioritySettings = aPrioritySettings;
    iMMFDevSound.SetPrioritySettings(iPrioritySettings);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::CalculateAudioInputPositionL
// Gets the current record position in terms of microseconds. This time position
// is from the time playback started or resumed and is reset after a stop or
// pause.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt64 CDevSoundAudioInput::CalculateAudioInputPositionL() const
    {
    DP0(_L("CDevSoundAudioInput::CalculateAudioInputPositionL"));
	TReal samplesPlayed = iMMFDevSound.SamplesRecorded();
	TMMFCapabilities devSoundConfig = iMMFDevSound.Config();
	TInt samplingFreq = 0;

	switch ( devSoundConfig.iRate )
		{
		case EMMFSampleRate8000Hz:
			samplingFreq = 8000;
			break;
		case EMMFSampleRate11025Hz:
			samplingFreq = 11025;
			break;
		case EMMFSampleRate16000Hz:
			samplingFreq = 16000;
			break;
		case EMMFSampleRate22050Hz:
			samplingFreq = 22050;
			break;
		case EMMFSampleRate32000Hz:
			samplingFreq = 32000;
			break;
		case EMMFSampleRate44100Hz:
			samplingFreq = 44100;
			break;
		case EMMFSampleRate48000Hz:
			samplingFreq = 48000;
			break;
		case EMMFSampleRate88200Hz:
			samplingFreq = 88200;
			break;
		case EMMFSampleRate96000Hz:
			samplingFreq = 96000;
			break;
		case EMMFSampleRate12000Hz:
            samplingFreq = 12000;
		   break;
		case EMMFSampleRate24000Hz:
           samplingFreq = 24000;
		   break;
		default:
            User::Leave(KErrCorrupt);
		}

	TReal timePlayedSeconds = 0;
	if(samplesPlayed)
        timePlayedSeconds = samplesPlayed/samplingFreq;

	TInt64 timePlayed(timePlayedSeconds*1000000);
	return timePlayed;
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::MaxGain
// Get maximum gain value from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CDevSoundAudioInput::MaxGainL()
    {
    DP0(_L("CDevSoundAudioInput::MaxGainL"));
    return iMMFDevSound.MaxGain();
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::SetGain
// Sends given new gain value to DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::SetGainL(
	TInt aGain )
    {
    DP0(_L("CDevSoundAudioInput::SetGainL"));
    iMMFDevSound.SetGain(aGain);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::Gain
// Get current gain value from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CDevSoundAudioInput::GainL()
    {
    DP0(_L("CDevSoundAudioInput::GainL"));
    return iMMFDevSound.Gain();
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::ConfigRatesL
// Get configured sampling rates from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::ConfigRatesL(
	RArray<TUint>& aRates )
    {
    DP0(_L("CDevSoundAudioInput::ConfigRatesL"));
    ConvertFromDevSoundCapsToSampleRatesL(iMMFDevSound.Config(), aRates);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::ConfigChannelsL
// Get configured sampling rates from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::ConfigChannelsL(
	RArray<TUint>& aChannels )
    {
    DP0(_L("CDevSoundAudioInput::ConfigChannelsL"));
    ConvertFromDevSoundCapsToNumChannelsL(iMMFDevSound.Config(), aChannels);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::ConfigDataTypesL
// Get configured data types (FourCC) from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::ConfigDataTypesL(
	RArray<TFourCC>& aDataTypes )
    {
    DP0(_L("CDevSoundAudioInput::ConfigDataTypesL"));
    ConvertFromDevSoundCapsToDataTypesL(iMMFDevSound.Config(), aDataTypes);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::CapabilitiesRatesL
// CDevSoundAudioInput::CapabilitiesRatesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::CapabilitiesRatesL(
	RArray<TUint>& aRates )
    {
    DP0(_L("CDevSoundAudioInput::CapabilitiesRatesL"));
    ConvertFromDevSoundCapsToSampleRatesL(iMMFDevSound.Capabilities(), aRates);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::CapabilitiesChannelsL
// Get DevSound capabilities for channels
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::CapabilitiesChannelsL(
	RArray<TUint>& aChannels )
    {
    DP0(_L("CDevSoundAudioInput::CapabilitiesChannelsL"));
    ConvertFromDevSoundCapsToNumChannelsL(iMMFDevSound.Capabilities(), aChannels);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::CapabilitiesDataTypesL
// Get DevSound capabilities for data types (FourCC)
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::CapabilitiesDataTypesL(
	RArray<TFourCC>& aDataTypes )
    {
    DP0(_L("CDevSoundAudioInput::CapabilitiesDataTypesL"));
    ConvertFromDevSoundCapsToDataTypesL(iMMFDevSound.Capabilities(), aDataTypes);
    }



// -----------------------------------------------------------------------------
// CDevSoundAudioInput::ConfigureL
// Save configuration information and initializes DevSound and codec.
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::ConfigureL(
	TUint aSampleRate,
	TUint aNumChannel,
	TFourCC aFourCC,
	const RArray<TInt>& aCodecConfigData )
    {
    DP0(_L("CDevSoundAudioInput::ConfigureL"));
    iSinkSampleRate = aSampleRate;
    iSinkChannels = aNumChannel;
    iSinkFourCC = aFourCC;

	iCodecConfigData.Reset();
    for ( TInt i = 0; i < aCodecConfigData.Count(); i++ )
    	{
    	//RDebug::Print(_L("Index %d [%d]"), i, aCodecConfigData[i]);
    	User::LeaveIfError(iCodecConfigData.Append(aCodecConfigData[i]));
    	}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::SetEncoder
// Sets the Encoder instance to be used by the audio input.
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::SetEncoder(
	CAdvancedAudioEncoder* aEncoder )
	{
    DP0(_L("CDevSoundAudioInput::SetEncoder"));
	iAdvancedAudioEncoder = aEncoder;

	iAdvancedAudioEncoder->SetDevSound(iMMFDevSound);
	iAdvancedAudioEncoder->SetObserver(*this);
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::SetBitRateL
// Sets the the bitrate to be used by the devsound.
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioInput::SetBitRateL(
	TUint aBitRate,
	const RArray<TInt>& aCodecConfigData )
	{
    DP2(_L("CDevSoundAudioInput::SetBitRateL iDevSoundInitialized = %D, iEncoderConfigPending = %D"),iDevSoundInitialized,iEncoderConfigPending);
    iBitRate = aBitRate; //Save the bitrate
    // Only SetBitrate on DevSound after its been Initialized
    if(	iDevSoundInitialized)
        {
        iEncoderConfigPending = EFalse;
	    iAdvancedAudioEncoder->SetBitRateL(aBitRate, aCodecConfigData);
	    }
	else // We hot initialised devsound, so we will hold on to any Codec Specific Setting.
	    {
	    iEncoderConfigPending = ETrue;
	    // Save Confing Data, to be passed once initcomplete
	    for ( TInt i = 0; i < aCodecConfigData.Count(); i++ )
    		{
    		//RDebug::Print(_L("Index %d [%d]"), i, aCodecConfigData[i]);
    		User::LeaveIfError(iPendingCodecConfigData.Append(aCodecConfigData[i]));
    		} 
	    }
	    
    iCodecConfigData.Reset();
    for ( TInt i = 0; i < aCodecConfigData.Count(); i++ )
    	{
    	//RDebug::Print(_L("Index %d [%d]"), i, aCodecConfigData[i]);
    	User::LeaveIfError(iCodecConfigData.Append(aCodecConfigData[i]));
    	}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::InitializeComplete
// Callback from DevSound to notify initialization has completed
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::InitializeComplete(TInt aError)
    {
    DP2(_L("CDevSoundAudioInput::InitializeComplete aError = %D, iEncoderConfigPending = %D"),aError,iEncoderConfigPending);
    if (iBlockInitialize)
        {
        DP0(_L("CDevSoundAudioInput::InitializeComplete unblocking initialize"));
        iBlockInitialize->AsyncStop();
        }

    if ( aError == KErrNone )
	 	{
     	iState = EDevSoundReady;
     	iDevSoundInitialized = ETrue;
	 	BuildConfigurationParameters();
	 	//Set Pending Codec Config info
    	if(iEncoderConfigPending)
    	 	{
    	    TRAP_IGNORE(SetBitRateL(iBitRate, iPendingCodecConfigData));
    	 	}
		}
	else
		{
    	iState = EIdle;
    	iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
		}
    DP0(_L("CDevSoundAudioInput::InitializeComplete end"));
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::ToneFinished
// ToneFinished called when a tone has finished or interrupted
// aError == KErrNone means success
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::ToneFinished(
	TInt /*aError*/ )
     {
     //we should never get a tone error in MMFAudioOutput!
     __ASSERT_DEBUG(EFalse, Panic(EMMFAudioOutputPanicToneFinishedNotSupported));
     }


// -----------------------------------------------------------------------------
// CDevSoundAudioInput::PlayError
// Callback error to indicate has error occurred during playback
// Should not be called on DevSound Audio Input
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::PlayError(
	TInt /*aError*/ )
	{
	//we should never get here during a record session!
	__ASSERT_DEBUG(EFalse, Panic(EMMFAudioInputPanicPlayErrorNotSupported));
    }


// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::BufferToBeFilled
// Called when buffer has been used up by DevSound
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::BufferToBeFilled(
	CMMFBuffer* /*aBuffer*/ )
	{
	//we should never get a BufferToBeFilled in Audio Input!
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::BufferToBeEmptied
// Callback from MDevSoundObserver
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::BufferToBeEmptied(
	CMMFBuffer* aBuffer )
	{
//    DP0 (_L ("CDevSoundAudioInput::BufferToBeEmptied"));
	if ( iState != EIdle )
		{
		iAdvancedAudioEncoder->EmptyBuffer(aBuffer);
		}
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::RecordError
// Callback from DevSound to indicate error occurred during a recording
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::RecordError(
	TInt aError )
	{
    DP2(_L("CDevSoundAudioInput::RecordError(%d) iState[%d]"), aError, iState);
    iMMFDevSound.Stop(); // might not need to do this. test
    iState = EIdle;
    iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::ConvertError
// Called from DevSound when stopped due to error while coverting
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::ConvertError(
	TInt /*aError*/ )
     {
     }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::DeviceMessage
// Callback from DevSound for message from hardware device (no messages in
// current implementation of DevSound)
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::DeviceMessage(
	TUid /*aMessageType*/,
	const TDesC8& /*aMsg*/ )
     {
     }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::SendEventToClient
// Send given event to client
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::SendEventToClient(
	const TMMFEvent& aEvent )
     {
     DP0(_L("CDevSoundAudioInput::SendEventToClient"));
     iObserver->SendEvent(aEvent);
     }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::BufferEmptied
// Notification from Audio Converter that the specified buffer has been filled.
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::BufferEmptied()
    {
    DP0(_L("CDevSoundAudioInput::BufferEmptied"));
	if ( iState != EIdle )
		{
    	iMMFDevSound.RecordData();
		}
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::EmptyBuffer
// Notification from Audio Converter that the specified buffer needs to be
// emptied.
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::EmptyBuffer(
	CMMFBuffer* aBuffer )
	{
    DP0(_L("CDevSoundAudioInput::EmptyBuffer"));
	// forward the request to the controller
    TRAPD(err,iObserver->EmptyBufferL(aBuffer));
	if(err != KErrNone)
	    {
	    DP1(_L("CDevSoundAudioInput::EmptyBuffer err[%d]"),err);
		RecordError(err);
	    }
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::SendEvent
// Send given event to client
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::SendEvent(
	const TMMFEvent& aEvent )
     {
     DP0(_L("CDevSoundAudioInput::SendEvent"));
     iObserver->SendEvent(aEvent);
     }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::ConvertFromDevSoundCapsToSampleRatesL
// Internal function to convert sampling rate from DevSound representation to
// integer value
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::ConvertFromDevSoundCapsToSampleRatesL(
	const TMMFCapabilities& aDevSoundCaps,
	RArray<TUint>& aSampleRates )
    {
    DP0(_L("CDevSoundAudioInput::ConvertFromDevSoundCapsToSampleRatesL"));
    if ( aDevSoundCaps.iRate & EMMFSampleRate8000Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate8000Hz));
		}
    if ( aDevSoundCaps.iRate & EMMFSampleRate11025Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate11025Hz));
		}
	if ( aDevSoundCaps.iRate & EMMFSampleRate12000Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate12000Hz));
		}
    if ( aDevSoundCaps.iRate & EMMFSampleRate16000Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate16000Hz));
		}
    if ( aDevSoundCaps.iRate & EMMFSampleRate22050Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate22050Hz));
		}
	if ( aDevSoundCaps.iRate & EMMFSampleRate24000Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate24000Hz));
		}
    if ( aDevSoundCaps.iRate & EMMFSampleRate32000Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate32000Hz));
		}
    if ( aDevSoundCaps.iRate & EMMFSampleRate44100Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate44100Hz));
		}
    if ( aDevSoundCaps.iRate & EMMFSampleRate48000Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate48000Hz));
		}
    if ( aDevSoundCaps.iRate & EMMFSampleRate88200Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate88200Hz));
		}
    if ( aDevSoundCaps.iRate & EMMFSampleRate96000Hz )
    	{
        User::LeaveIfError(aSampleRates.Append(KSampleRate96000Hz));
		}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::ConvertFromDevSoundCapsToDataTypesL
// Internal function to convert data types from DevSound representation to
// FourCC
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::ConvertFromDevSoundCapsToDataTypesL(
	const TMMFCapabilities& aDevSoundCaps,
	RArray<TFourCC>& aDataTypes )
    {
    DP0(_L("CDevSoundAudioInput::ConvertFromDevSoundCapsToDataTypesL"));
    if ( aDevSoundCaps.iEncoding & EMMFSoundEncoding8BitPCM )
    	{
        User::LeaveIfError(aDataTypes.Append(KMMFFourCCCodePCM8));
		}
    if ( aDevSoundCaps.iEncoding & EMMFSoundEncoding16BitPCM )
    	{
        User::LeaveIfError(aDataTypes.Append(KMMFFourCCCodePCM16));
		}
    if ( aDevSoundCaps.iEncoding & EMMFSoundEncoding8BitALaw )
    	{
        User::LeaveIfError(aDataTypes.Append(KMMFFourCCCodeALAW));
		}
    if ( aDevSoundCaps.iEncoding & EMMFSoundEncoding8BitMuLaw )
    	{
        User::LeaveIfError(aDataTypes.Append(KMMFFourCCCodeMuLAW));
		}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::ConvertFromDevSoundCapsToNumChannelsL
// Internal function to convert number of channels from DevSound representation to
// integer value
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::ConvertFromDevSoundCapsToNumChannelsL(
	const TMMFCapabilities& aDevSoundCaps,
	RArray<TUint>& aNumChannels )
    {
    DP0(_L("CDevSoundAudioInput::ConvertFromDevSoundCapsToNumChannelsL"));
    if ( aDevSoundCaps.iChannels & EMMFMono )
    	{
        User::LeaveIfError(aNumChannels.Append(KNumChannelsMono));
		}
    if ( aDevSoundCaps.iChannels & EMMFStereo )
    	{
        User::LeaveIfError(aNumChannels.Append(KNumChannelsStereo));
		}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::Panic
// Raise user panic with appropriate panic code
// -----------------------------------------------------------------------------
//
void CDevSoundAudioInput::Panic(
	TInt aPanicCode ) const
    {
    _LIT(KDevSoundAudioInputPanicCategory, "DevSoundAudioInput");
    User::Panic(KDevSoundAudioInputPanicCategory, aPanicCode);
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of file
