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
* Description:  The functions in this class implements the behavior that is
*                specific to the interface with DevSound.
*
*/


// INCLUDE FILES
#include "DevSoundAudioOutput.h"
#include "DebugMacros.h"
#include <mmfpaniccodes.h>
#include <ConfigurationComponentsFactory.h>
#include <AudioOutputControlUtility.h>
#include <S60FourCC.h>

// CONSTANTS
const TUint KSampleRate8000Hz       = 8000;
const TUint KSampleRate11025Hz      = 11025;
const TUint KSampleRate12000Hz      = 12000;
const TUint KSampleRate16000Hz      = 16000;
const TUint KSampleRate22050Hz      = 22050;
const TUint KSampleRate24000Hz      = 24000;
const TUint KSampleRate32000Hz      = 32000;
const TUint KSampleRate44100Hz      = 44100;
const TUint KSampleRate48000Hz      = 48000;
const TUint KSampleRate64000Hz      = 64000;
const TUint KSampleRate88200Hz      = 88200;
const TUint KSampleRate96000Hz      = 96000;

const TUint KNumChannelsMono        = 1;
const TUint KNumChannelsStereo      = 2;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::CDevSoundAudioOutput
// -----------------------------------------------------------------------------
//
CDevSoundAudioOutput::CDevSoundAudioOutput(CMMFDevSound& aMMFDevSound)
	:	iMMFDevSound(aMMFDevSound),
		iLoopPlayEnabled(EFalse),
	    iIgnoreBTBFDuringLoopPlay(EFalse),
	    iUnSetLastBuffer(EFalse),
	    iDSStopped(ETrue),
	    iLastBufferSentToDevSound(EFalse)
    {
	iAdvancedAudioDecoder = NULL;
	iDataSourceAdapter = NULL;
	iFactory = NULL;
	iAudioOutputControlUtility = NULL;
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::ConstructL
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::ConstructL(
	const TMMFPrioritySettings& aPrioritySettings,
	MAdvancedAudioOutputObserver& aObserver)
	{
    DP0(_L("CDevSoundAudioOutput::ConstructL"));

	iPrioritySettings = aPrioritySettings;
	iObserver = &aObserver;

	iState = EIdle;

	//This is done to maintain compatibility with the media server
	iMMFDevSound.SetVolume (iMMFDevSound.MaxVolume() - 1);

	//We need to set PrioritySettings
	iMMFDevSound.SetPrioritySettings(iPrioritySettings);
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::NewL
// Static function for creating an instance of the DevSound Audio Output
// object.
// -----------------------------------------------------------------------------
//
EXPORT_C CDevSoundAudioOutput* CDevSoundAudioOutput::NewL(
    const TMMFPrioritySettings& aPrioritySettings,
    MAdvancedAudioOutputObserver& aObserver,
    CMMFDevSound& aMMFDevSound)
    {
    DP0(_L("CDevSoundAudioOutput::NewL"));
    CDevSoundAudioOutput* self = new (ELeave) CDevSoundAudioOutput(aMMFDevSound);
    CleanupStack::PushL(self);
    self->ConstructL(aPrioritySettings, aObserver);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C CDevSoundAudioOutput::~CDevSoundAudioOutput()
    {
    DP0(_L("CDevSoundAudioOutput::~CDevSoundAudioOutput"));

	iCodecConfigData.Close();
	delete iAdvancedAudioDecoder;

    iMMFDevSound.Stop();
    
    iDataSourceAdapter = NULL;
    if(iAudioOutputControlUtility)
        delete iAudioOutputControlUtility;
    if(iFactory)
        delete iFactory;    
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::PrimeL
// Prepare to start playing, building configuration parameters and initializing DevSound.
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::PrimeL()
    {
    DP0(_L("CDevSoundAudioOutput::PrimeL start"));
	if (!iAdvancedAudioDecoder)
		{
		// SetDecoder hasn't been called!!!
		User::Leave(KErrNotReady);
		}
	
	// Devsound will be initialized only once for the given data source.
	// The current datasource would need to be removed, which would delete this audiooutput,
	// and another datasource added, which will create this audiooutput with
	// iDevSoundInitialized clear, which would allow devsound to be initialized.
	// This check is to prevent devsound from being initialized again in the case where
	// the same file is played from the beginning and the datasink is reinitialized
	// after the header is read and this output is primed.
	//
	// If we want to initialize devsound again for the same source, we can remove
	// the condition of !iDevSoundInitialized, but we need to set it EFalse before calling
	// InitializeL. We would also need to make a check in our own DoPlayL function
	// to see if devsound is being initialized like we do in PlayL in the controller.

	//  iDevSoundInitialized = EFalse;
	if (!iDevSoundInitialized)
	    {
	    DP0(_L("CDevSoundAudioOutput::PrimeL initializing DevSound"));
	    if(iAdvancedAudioDecoder->IsHwAccelerated())
	        {
	        iMMFDevSound.InitializeL(*this, iSourceFourCC, EMMFStatePlaying);
	        }
	    else
	        {
	        iMMFDevSound.InitializeL(*this, KMMFFourCCCodePCM16, EMMFStatePlaying);
	        }
	    }
//	iDevSoundInitialized = EFalse;
    DP0(_L("CDevSoundAudioOutput::PrimeL end"));
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::PauseL
// Send pause command to DevSound pause and set own state to paused
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::PauseL()
    {
    DP0(_L("CDevSoundAudioOutput::PauseL"));
    iState = EPaused;
    iMMFDevSound.Pause();
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::PlayL
// Start playback process by configuring the codec and initialize play on DevSound.
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::PlayL(
	TAny* aBuffer, TInt aIndex)
    {
    DP0(_L("CDevSoundAudioOutput::PlayL"));

	if (iDevSoundInitialized)
		{
	    // Set the flag to EFalse for processing the data for playback.
	    iIgnoreBTBFDuringLoopPlay = EFalse;
		// start play only after we get the initialize complete
		DoPlayL(aBuffer,aIndex);
		}
	else
		{
    	DP0(_L("CDevSoundAudioOutput::PlayL pending"));
		iState = EPlayPending;
		iBufferIndex = aIndex;
		iPlayPendingBuffer = aBuffer;
		}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::PlayL
// Start playback process by configuring the codec and initialize play on DevSound.
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::DoPlayL(
	TAny* aBuffer, TInt aIndex)
    {
    DP0(_L("CDevSoundAudioOutput::DoPlayL"));
    
	// Give the audio converter a handle to the source buffers
	iAdvancedAudioDecoder->SetSourceBuffers(reinterpret_cast<RPointerArray<CMMFDataBuffer>*>(aBuffer));
	// Reset the converter
	iAdvancedAudioDecoder->ResetL();
	// Configure the audio converter
	iAdvancedAudioDecoder->SetConfigL(iSourceSampleRate, iSourceChannels, iSWConvertSampleRate,
										iSWConvertChannels, iCodecConfigData, aIndex);

	// Configure DevSound
	// Can devsound capabilities change between pause and play? May not be necessary to
	// do this multiple times.
	iMMFDevSound.SetConfigL(iDevSoundConfig);
    DP1(_L("CDevSoundAudioOutput::DoPlayL config DS sample rate[%d]"), iDevSoundConfig.iRate );
    iState = EPlaying;
    iDSStopped = EFalse;

    if ( (iDataSourceAdapter != NULL) && (iDataSourceAdapter->IsProtectedL()) )
        {
        if (iAudioOutputControlUtility)
                iAudioOutputControlUtility->Configure(iMMFDevSound);    //ignoring errors since rouitng changes are only suggestions to adaptation
        }
    
    iMMFDevSound.PlayInitL();
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::BuildConfigurationParametersL
// Build a configuration profile based on DevSound capabilities
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::BuildConfigurationParametersL()
	{
	// Query DevSound capabilities and Try to use DevSound sample rate and
	// mono/stereo capability

	// Reset the following flag in case DevSound's capabilities have
	// changed since we were last here:
	iNeedsSWConversion = EFalse;
	iSWConvertSampleRate = iSourceSampleRate;
	TMMFCapabilities devSoundCaps = iMMFDevSound.Capabilities();

    DP4(_L("CDevSoundAudioOutput::BuildConfigurationParametersL [%d] [%d] [%d] [%d]"),
		iSourceSampleRate, iSourceChannels, devSoundCaps.iRate, devSoundCaps.iChannels);

	// Default PCM16
	iDevSoundConfig.iEncoding = EMMFSoundEncoding16BitPCM;
	// 1 = Monophonic and 2 == Stereo
	if (((iSourceChannels == 1) && (devSoundCaps.iChannels & EMMFMono)) ||
		((iSourceChannels == 2) && (devSoundCaps.iChannels & EMMFStereo)))
		{
		iDevSoundConfig.iChannels = iSourceChannels;
		}
	else //default or SW conversion, e.g. stereo on mono support
		{
		iDevSoundConfig.iChannels = EMMFMono;
		iNeedsSWConversion = ETrue;
		iSWConvertChannels = 1;
		}

	// Check for std sample rates.
	if ((iSourceSampleRate == 96000) && (devSoundCaps.iRate & EMMFSampleRate96000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate96000Hz;
	else if ((iSourceSampleRate == 88200) && (devSoundCaps.iRate & EMMFSampleRate88200Hz))
		iDevSoundConfig.iRate = EMMFSampleRate88200Hz;
	else if ((iSourceSampleRate == 48000) && (devSoundCaps.iRate & EMMFSampleRate48000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate48000Hz;
	else if ((iSourceSampleRate == 44100) && (devSoundCaps.iRate & EMMFSampleRate44100Hz))
		iDevSoundConfig.iRate = EMMFSampleRate44100Hz;
	else if ((iSourceSampleRate == 32000) && (devSoundCaps.iRate & EMMFSampleRate32000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate32000Hz;
	else if ((iSourceSampleRate == 24000) && (devSoundCaps.iRate & EMMFSampleRate24000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate24000Hz;
	else if ((iSourceSampleRate == 22050) && (devSoundCaps.iRate & EMMFSampleRate22050Hz))
		iDevSoundConfig.iRate = EMMFSampleRate22050Hz;
	else if ((iSourceSampleRate == 16000) && (devSoundCaps.iRate & EMMFSampleRate16000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate16000Hz;
	else if ((iSourceSampleRate == 12000) && (devSoundCaps.iRate & EMMFSampleRate12000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate12000Hz;
	else if ((iSourceSampleRate == 11025) && (devSoundCaps.iRate & EMMFSampleRate11025Hz))
		iDevSoundConfig.iRate = EMMFSampleRate11025Hz;
	else if ((iSourceSampleRate == 8000) && (devSoundCaps.iRate & EMMFSampleRate8000Hz))
		iDevSoundConfig.iRate = EMMFSampleRate8000Hz;
	else // non standard sample rate
		{
		iNeedsSWConversion = ETrue;
		// we need to choose to the closest, and smaller standard sample rate
		// and eventually convert the audio samples to this standard sample rate
		// NB: this list must be in ascending order
		const TInt KNumSampleRates = 11;
		TUint supportedSR[KNumSampleRates][2] = {{8000,  EMMFSampleRate8000Hz},
												 {11025, EMMFSampleRate11025Hz},
												 {12000, EMMFSampleRate12000Hz},
												 {16000, EMMFSampleRate16000Hz},
												 {22050, EMMFSampleRate22050Hz},
												 {24000, EMMFSampleRate24000Hz},
												 {32000, EMMFSampleRate32000Hz},
												 {44100, EMMFSampleRate44100Hz},
												 {48000, EMMFSampleRate48000Hz},
												 {88200, EMMFSampleRate88200Hz},
												 {96000, EMMFSampleRate96000Hz}};

		// Only support down sampling
		if (iSourceSampleRate < supportedSR[0][0])
			{
			User::Leave(KErrNotSupported);
			}

		TInt sampleRateIndex = KNumSampleRates - 1;

		// find the source sampleRateIndex
		for (; sampleRateIndex >= 0; sampleRateIndex--)
			{
			if(iSourceSampleRate >= supportedSR[sampleRateIndex][0])
				{
				break;
				}
			}

		//aac HWcodec will downsample by 2, if it is greater than 48k 
		//Hence devsound should be onfigured with that value
		if(iSourceSampleRate > KSampleRate48000Hz && iSourceFourCC == KS60FourCCCodeEAAC && iAdvancedAudioDecoder->IsHwAccelerated())
		    {
            TUint samplerate = iSourceSampleRate/2;
            for (; sampleRateIndex >= 0; sampleRateIndex--)
                {
                if(samplerate >= supportedSR[sampleRateIndex][0])
                    {
                    break;
                    }
                }
		    }
		// find the highest sink sample rate below the source rate
		for (; sampleRateIndex >= 0; sampleRateIndex--)
			{
			if(devSoundCaps.iRate & supportedSR[sampleRateIndex][1])
				{
				iSWConvertSampleRate = supportedSR[sampleRateIndex][0];
				iDevSoundConfig.iRate = supportedSR[sampleRateIndex][1];
				break;
				}
			}

		// if a suitable sink sample rate is not available
		if (sampleRateIndex < 0)
			{
			User::Leave(KErrNotSupported);
			}

		// set the channels as well
		iSWConvertChannels = iDevSoundConfig.iChannels;
		} // else // non standard sample rate

	}

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::StopL
// Send stop command to DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::StopL(TBool aStopDevsound)
    {
    DP2(_L("CDevSoundAudioOutput::StopL state[%d] condition[%d]"), iState, aStopDevsound);

	if (iState != EIdle)
		{
        iAdvancedAudioDecoder->Stop(); // stop the decoder before devound
    	if (aStopDevsound)
        	{
            DP0(_L("CDevSoundAudioOutput::StopL calling iMMFDevSound.Stop()"));
            iDSStopped = ETrue;
        	iMMFDevSound.Stop();           // devsound buffers become invalid here
        	}
        iState = EIdle;
		}
	else
	    { // EIdle state
	    if (aStopDevsound || iLastBufferSentToDevSound)
	        { // for play error / playcomplete if resume not supported and paused in loop play (devsound not stopped)
            DP0(_L("CDevSoundAudioOutput::StopL calling iMMFDevSound.Stop()"));
            iDSStopped = ETrue;
            iMMFDevSound.Stop();           // devsound buffers become invalid here
	        }
	    }
    }
    
// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::MaxVolumeL
// Get maximum gain volume from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CDevSoundAudioOutput::MaxVolumeL()
    {
    return iMMFDevSound.MaxVolume();
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::SetVolumeL
// Sends given new volume value to DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::SetVolumeL(TInt aVolume)
     {
     iMMFDevSound.SetVolume(aVolume);
     }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::VolumeL
// Get current volume value from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CDevSoundAudioOutput::VolumeL()
    {
    return iMMFDevSound.Volume();
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::SetPrioritySettings
// Send new priority settings to DevSound. Priorities are enumerated to
// determine audio playback priority. Higher numbers mean high priority (can
// interrupt lower priorities).
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::SetPrioritySettingsL(
	const TMMFPrioritySettings& aPrioritySettings)
    {
    iPrioritySettings = aPrioritySettings;
    iMMFDevSound.SetPrioritySettings(iPrioritySettings);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::CalculateAudioOutputPosition
// Gets the current playback position in terms of microseconds. This time position
// is from the time playback started or resumed and is reset after a stop or
// pause.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt64 CDevSoundAudioOutput::CalculateAudioOutputPositionL() const
    {
    DP0(_L("CDevSoundAudioOutput::CalculateAudioOutputPosition"));
    if (!iDevSoundInitialized)
        { // if devsound hasn't been initialized, we will get an error when calling Config()
        return 0;
        }
	TInt64 timePlayedInMicroSeconds = 0;

	if (iMMFDevSound.IsGetTimePlayedSupported())
        {
        DP0(_L("CDevSoundAudioOutput::CalculateAudioOutputPosition GetTimeplayed is supported by MMFDevSound"));
        TTimeIntervalMicroSeconds timePlayedInMS = 0;
        iMMFDevSound.GetTimePlayed(timePlayedInMS);
        timePlayedInMicroSeconds = timePlayedInMS.Int64();
        }
	else
		{
		DP0(_L("CDevSoundAudioOutput::CalculateAudioOutputPosition GetTimeplayed is not supported by MMFDevSound"));
		TInt64 samplesPlayed = iMMFDevSound.SamplesPlayed();
        DP1(_L("CDevSoundAudioOutput::CalculateAudioOutputPosition SamplesPlayed = %g"), I64REAL(samplesPlayed));
        DP2(_L("CDevSoundAudioOutput::CalculateAudioOutputPosition SamplesPlayed = [%x][%x]"), I64HIGH(samplesPlayed), I64LOW(samplesPlayed));
         TMMFCapabilities devSoundConfig = iMMFDevSound.Config();
         TInt samplingFreq = 0;
        DP4(_L("CDevSoundAudioOutput::CalculateAudioOutputPositionL [%d] [%d] [%d] [%d]"),
            iSourceSampleRate, iSourceChannels, devSoundConfig.iRate, devSoundConfig.iChannels);
    
             switch (devSoundConfig.iRate)
                  {
                  case EMMFSampleRate8000Hz:
                       samplingFreq = 8000;
                       break;
                  case EMMFSampleRate11025Hz:
                       samplingFreq = 11025;
                       break;
                  case EMMFSampleRate12000Hz:
                       samplingFreq = 12000;
                       break;
                  case EMMFSampleRate16000Hz:
                       samplingFreq = 16000;
                       break;
                  case EMMFSampleRate22050Hz:
                       samplingFreq = 22050;
                       break;
                  case EMMFSampleRate24000Hz:
                       samplingFreq = 24000;
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
                  default:
                       DP0(_L("CDevSoundAudioOutput::CalculateAudioOutputPosition DevSound Config Data Not Available"));
                       //Panic(EMMFDataPathPanicBadState);
                       return 0;
                  }
            DP1(_L("CDevSoundAudioOutput::CalculateAudioOutputPosition: SamplingFreq = %d"), samplingFreq);
            timePlayedInMicroSeconds = (samplesPlayed*1000000)/samplingFreq;
       }
    DP1(_L("CDevSoundAudioOutput::CalculateAudioOutputPosition: timePlayedInMicroSeconds = %g"), I64REAL(timePlayedInMicroSeconds));

    return timePlayedInMicroSeconds;
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::ConfigRatesL
// Get configured sampling rates from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::ConfigRatesL(
	RArray<TUint>& aRates)
    {
    ConvertFromDevSoundCapsToSampleRatesL(iMMFDevSound.Config(), aRates);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::ConfigChannelsL
// Get configured sampling rates from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::ConfigChannelsL(
	RArray<TUint>& aChannels)
    {
    ConvertFromDevSoundCapsToNumChannelsL(iMMFDevSound.Config(), aChannels);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::ConfigDataTypesL
// Get configured data types (FourCC) from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::ConfigDataTypesL(
	RArray<TFourCC>& aDataTypes)
    {
    ConvertFromDevSoundCapsToDataTypesL(iMMFDevSound.Config(), aDataTypes);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::CapabilitiesRatesL
// CDevSoundAudioInput::CapabilitiesRatesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::CapabilitiesRatesL(
	RArray<TUint>& aRates)
    {
    ConvertFromDevSoundCapsToSampleRatesL(iMMFDevSound.Capabilities(), aRates);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::CapabilitiesChannelsL
// Get DevSound capabilities for channels
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::CapabilitiesChannelsL(
	RArray<TUint>& aChannels)
    {
    ConvertFromDevSoundCapsToNumChannelsL(iMMFDevSound.Capabilities(), aChannels);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::CapabilitiesDataTypesL
// Get DevSound capabilities for data types (FourCC)
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::CapabilitiesDataTypesL(
	RArray<TFourCC>& aDataTypes)
    {
    ConvertFromDevSoundCapsToDataTypesL(iMMFDevSound.Capabilities(), aDataTypes);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::SetVolumeRampL
// Send new ramp duration to DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::SetVolumeRampL(
	const TTimeIntervalMicroSeconds& aRampDuration)
    {
    iMMFDevSound.SetVolumeRamp(aRampDuration);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::SetPlayBalanceL
// Send new channel balance information (left and right channel) to DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::SetPlayBalanceL(
	TInt aLeftPercentage, TInt aRightPercentage)
    {
    iMMFDevSound.SetPlayBalanceL(aLeftPercentage, aRightPercentage);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::GetPlayBalanceL
// Get current playback channel balance information (for left and right channel)
// from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::GetPlayBalanceL(
	TInt& aLeftPercentage, TInt& aRightPercentage)
    {
    iMMFDevSound.GetPlayBalanceL(aLeftPercentage, aRightPercentage);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::ConfigureL
// Saves configuration information to be used later to initialize DevSound and
// configure the codec.
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::ConfigureL(
	TUint aSampleRate,
	TUint aNumChannel,
	TFourCC aFourCC,
	const RArray<TInt>& aCodecConfigData)
    {
    DP0(_L("CDevSoundAudioOutput::ConfigureL"));
    iSourceSampleRate = aSampleRate;
    iSourceChannels = aNumChannel;
    iSourceFourCC = aFourCC;

	iCodecConfigData.Reset();

    for (TInt i = 0; i < aCodecConfigData.Count(); i++)
    	{
    	//DP2(_L("Index %d [%d]"), i, aCodecConfigData[i]);
    	User::LeaveIfError(iCodecConfigData.Append(aCodecConfigData[i]));
		}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::SetDecoder
// Sets the Decoder instance to be used by the audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::SetDecoder(
	CAdvancedAudioDecoder* aDecoder)
	{
    DP0(_L("CDevSoundAudioOutput::SetDecoder"));

    // delete existing iAdvancedAudioDecoder if any
    if (iAdvancedAudioDecoder)
        {
        delete iAdvancedAudioDecoder;
        iAdvancedAudioDecoder = NULL;
        }
        
	iAdvancedAudioDecoder = aDecoder;

	iAdvancedAudioDecoder->SetDevSound(iMMFDevSound);
	iAdvancedAudioDecoder->SetObserver(*this);
	}

EXPORT_C void CDevSoundAudioOutput::SetDataSourceAdapter(CDataSourceAdapter* aDataSourceAdapter)
    {
    DP0(_L("CDevSoundAudioOutput::SetDataSourceAdapter"));
    iDataSourceAdapter = aDataSourceAdapter;
    }
	
// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::Resume
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CDevSoundAudioOutput::Resume(TInt aBufferIndex)
    {
    DP2(_L("CDevSoundAudioOutput::Resume+++ iIgnoreBTBFDuringLoopPlay[%d] iState[%d]"), iIgnoreBTBFDuringLoopPlay, iState);
    if (aBufferIndex >= 0)
        {
        // get the buffer for decoding at this buffer index
        iAdvancedAudioDecoder->QueueThisBuffer(aBufferIndex);
        }
    // Reset the Decoder for resuming the codec processing during LoopPlay.
    // Decoder is disabled during every playback / pause operation in a LoopPlay
    if ((iLoopPlayEnabled) && (iState != EPlaying))
        {
        // reset the decoder. it flushes the codec's in & out buffers and
        // enables the decoder for furhter processing when DoPlay / Resume is called
        iAdvancedAudioDecoder->ResetL();
        }
        // Set the flag to false for resuming the playback (Processing the data in BufferToBeFilled)
        iIgnoreBTBFDuringLoopPlay = EFalse;
        // Change the state to EPlaying for resuming the playback
        iState = EPlaying;
    if (iSavedBufferPtr)
        {
		// Have the decoder fill the specified buffer with decoded audio.
 		BufferToBeFilled(iSavedBufferPtr);
		}
	else
		{
        DP0(_L("CDevSoundAudioOutput::Resume ignored"));
		}	
	DP2(_L("CDevSoundAudioOutput::Resume--- iIgnoreBTBFDuringLoopPlay[%d] iState[%d]"), iIgnoreBTBFDuringLoopPlay, iState);
    }
    
// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::InitializeComplete
// Callback from DevSound to notify initialization has completed
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::InitializeComplete(
	TInt aError)
    {
    TInt err;
    DP2(_L("CDevSoundAudioOutput::InitializeComplete aError[%d] iState[%d]"), aError, iState);
    
    if(aError == KErrNone)
	    {
        iDevSoundInitialized = ETrue;
	 	TRAP(err, BuildConfigurationParametersL());
		if (err)
			{
	        iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
    	    iState = EIdle;
			}
		
		if ( (iDataSourceAdapter != NULL) && (iDataSourceAdapter->IsProtectedL()) )
		    {
		    TRAP(err,CConfigurationComponentsFactory::CreateFactoryL(iFactory));
            
            if (err == KErrNone && iFactory)
                {
                TRAP(err,iFactory->CreateAudioOutputControlUtility(iAudioOutputControlUtility));
                }    
            
			if (err == KErrNone && iAudioOutputControlUtility)
				{
	            TRAP(err,iDataSourceAdapter->SetAudioOutputControlUtil(iAudioOutputControlUtility));
				}
	
			if (err)
				{
	        	iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
    	    	iState = EIdle;
				}
		    }

        if (iState == EPlayPending)
            {
            TRAP(err,DoPlayL(iPlayPendingBuffer,iBufferIndex));
			if (err)
				{
	    	    iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
    	    	iState = EIdle;
				}
            }
        else
            {
            iState = EDevSoundReady;
            }
        }
	else
		{
        iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
        iState = EIdle;
		}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::ToneFinished
// ToneFinished called when a tone has finished or interrupted
// aError == KErrNone means success
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::ToneFinished(
	TInt /*aError*/)
     {
     //we should never get a tone error in MMFAudioOutput!
     __ASSERT_DEBUG(EFalse, Panic(EMMFAudioOutputPanicToneFinishedNotSupported));
     }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::BufferToBeFilled
// Called when the specified buffer needs to be filled for playback.
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::BufferToBeFilled(
	CMMFBuffer* aBuffer)
    {
    DP1(_L("CDevSoundAudioOutput::BufferToBeFilled iIgnoreBTBFDuringLoopPlay[%d]"), iIgnoreBTBFDuringLoopPlay);
	iSavedBufferPtr = aBuffer;
    // if DevSound asks for another buffer after the LastBuffer has been sent, then we ignore those callbacks.
    if (!iIgnoreBTBFDuringLoopPlay)
        {
        // Have the decoder fill the specified buffer with decoded audio.
        TRAPD(err, iAdvancedAudioDecoder->FillBufferL(aBuffer));
        DP1(_L("CDevSoundAudioOutput::BufferToBeFilled FillBufferL err[%d]"), err);
        if (err != KErrNone)
            {
            PlayError(err);
            }
        }
    else
        {
        DP0(_L("CDevSoundAudioOutput::BufferToBeFilled ignored"));
        }
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::PlayError
// Callback error to indicate success or error during playback.
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::PlayError(
	TInt aError)
     {
     DP2(_L("CDevSoundAudioOutput::PlayError(%d) iState[%d]"), aError, iState);
    //iMMFDevSound->Stop(); doesn't need this probably since devsound should be stopped on
    // error or otherwise. Should test this and see if it can be removed.

    if (aError == KErrUnderflow && (iState == EIdle || iState == EPreIdle))
        {
        iObserver->PlaybackComplete();
        iAdvancedAudioDecoder->RenderEnable();
        }
    else
        {
        if (aError == KErrCorrupt)
        	{
        	iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, KErrDied));
        	}
        else
        	{
       	 	iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
        	}
        iState = EIdle;
        }
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::BufferToBeEmptied
// Callback from MDevSoundObserver
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::BufferToBeEmptied(
	CMMFBuffer* /*aBuffer*/)
     {
     __ASSERT_DEBUG(EFalse, Panic(EMMFAudioOutputPanicRecordErrorNotSupported));
     }


// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::RecordError
// Callback from DevSound when a complete buffer of audio data has been collected
// (not applicable to playback)
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::RecordError(
	TInt /*aError*/)
     {
     //we should never get a recording error in MMFAudioOutput!
     __ASSERT_DEBUG(EFalse, Panic(EMMFAudioOutputPanicRecordErrorNotSupported));
     }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::ConvertError
// Called from DevSound when stopped due to error while coverting
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::ConvertError(
	TInt /*aError*/)
     {
     }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::DeviceMessage
// Callback from DevSound for message from hardware device (no messages in
// current implementation of DevSound)
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::DeviceMessage(
	TUid /*aMessageType*/,
	const TDesC8& /*aMsg*/)
     {
     }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::SendEventToClient
// Send given event to client
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::SendEventToClient(
	const TMMFEvent& aEvent)
     {
     iObserver->SendEvent(aEvent);
     }


// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::BufferFilled
// Notification from Audio decoder that the specified buffer has been filled.
// DevSound is requested to play the data.
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::BufferFilled(
	CMMFBuffer* aBuffer)
     {
#ifdef _DEBUG
     TUint bytes = static_cast<CMMFDataBuffer*>(aBuffer)->Data().Length();
//     iTestByteCount += bytes;
     DP2(_L("CDevSoundAudioOutput::BufferFilled length[%d] last[%d]"),bytes,aBuffer->LastBuffer());
#endif
     TBool sendCallback = EFalse;
     if (iState != EIdle)
		{
		if (aBuffer->LastBuffer())
			{
			iState = EPreIdle;
			DP2(_L("CDevSoundAudioOutput::BufferFilled This is the LastBuffer. IsLoopPlayEnabled[%d] iUnSetLastBuffer[%d]"), iLoopPlayEnabled ,iUnSetLastBuffer);
			if (iUnSetLastBuffer)
			    {
			    // If Repeat is ON then set the LastBuffer flag to False
			    DP0(_L("CDevSoundAudioOutput::BufferFilled This is the LastBuffer. LastBuffer is set to EFalse"));
			    aBuffer->SetLastBuffer(EFalse);
			    sendCallback = ETrue;
                iLastBufferSentToDevSound = EFalse;
			    }
			else
			    {
			    DP0(_L("CDevSoundAudioOutput::BufferFilled LastBuffer is sent to MMFDevSound.... wait for PlayError callback .."));
			    iLastBufferSentToDevSound = ETrue;
			    }
			}
 		iSavedBufferPtr = NULL;
        // Request DevSound to play the buffer
    	iMMFDevSound.PlayData();
		if (sendCallback)
		    {
            // Callback to CAdvancedAudioPlayController for handling the repeated play case
            DP0(_L("CDevSoundAudioOutput::BufferFilled Callback LastBufferSent to Observer"));
            iIgnoreBTBFDuringLoopPlay = ETrue;
		    iObserver->LastBufferSent();
		    }
        DP0(_L("CDevSoundAudioOutput::BufferFilled End "));
		}
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::RefillBuffer
// Notification from Audio Converter that the specified buffer needs to be
// refilled.
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::RefillBuffer(CMMFBuffer* aEmptyBuffer)
	{
	 // forward the request to the controller
    iObserver->RefillBuffer(aEmptyBuffer);      // Async
	}
/*
// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::RefillBuffer
// Notification from Audio Converter that the specified buffer needs to be
// refilled.
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::RefillBuffer(CMMFBuffer* aEmptyBuffer, TBool aDoSynchronousRead)
	{
	 // forward the request to the controller
    iObserver->RefillBuffer(aEmptyBuffer, aDoSynchronousRead);
	}
*/
// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::SendEvent
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::SendEvent(
	const TMMFEvent& aEvent)
	{
    iObserver->SendEvent(aEvent);
	}

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::ConvertFromDevSoundCapsToSampleRatesL
// Internal function to convert sampling rate from DevSound representation to
// integer value
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::ConvertFromDevSoundCapsToSampleRatesL(
     const TMMFCapabilities& aDevSoundCaps,
     RArray<TUint>& aSampleRates)
    {
    if (aDevSoundCaps.iRate & EMMFSampleRate8000Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate8000Hz));
		}
    if (aDevSoundCaps.iRate & EMMFSampleRate11025Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate11025Hz));
		}
	if (aDevSoundCaps.iRate & EMMFSampleRate12000Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate12000Hz));
		}
    if (aDevSoundCaps.iRate & EMMFSampleRate16000Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate16000Hz));
		}
    if (aDevSoundCaps.iRate & EMMFSampleRate22050Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate22050Hz));
		}
	if (aDevSoundCaps.iRate & EMMFSampleRate24000Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate24000Hz));
		}
    if (aDevSoundCaps.iRate & EMMFSampleRate32000Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate32000Hz));
		}
    if (aDevSoundCaps.iRate & EMMFSampleRate44100Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate44100Hz));
		}
    if (aDevSoundCaps.iRate & EMMFSampleRate48000Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate48000Hz));
		}
    if (aDevSoundCaps.iRate & EMMFSampleRate64000Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate64000Hz));
		}
    if (aDevSoundCaps.iRate & EMMFSampleRate88200Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate88200Hz));
		}
    if (aDevSoundCaps.iRate & EMMFSampleRate96000Hz)
		{
        User::LeaveIfError(aSampleRates.Append(KSampleRate96000Hz));
		}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::ConvertFromDevSoundCapsToDataTypesL
// Internal function to convert data types from DevSound representation to
// FourCC
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::ConvertFromDevSoundCapsToDataTypesL(
     const TMMFCapabilities& aDevSoundCaps,
     RArray<TFourCC>& aDataTypes)
    {
    if (aDevSoundCaps.iEncoding & EMMFSoundEncoding8BitPCM)
		{
        User::LeaveIfError(aDataTypes.Append(KMMFFourCCCodePCM8));
		}
    if (aDevSoundCaps.iEncoding & EMMFSoundEncoding16BitPCM)
		{
        User::LeaveIfError(aDataTypes.Append(KMMFFourCCCodePCM16));
		}
    if (aDevSoundCaps.iEncoding & EMMFSoundEncoding8BitALaw)
		{
        User::LeaveIfError(aDataTypes.Append(KMMFFourCCCodeALAW));
		}
    if (aDevSoundCaps.iEncoding & EMMFSoundEncoding8BitMuLaw)
		{
        User::LeaveIfError(aDataTypes.Append(KMMFFourCCCodeMuLAW));
		}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::ConvertFromDevSoundCapsToNumChannelsL
// Internal function to convert number of channels from DevSound representation to
// integer value
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::ConvertFromDevSoundCapsToNumChannelsL(
     const TMMFCapabilities& aDevSoundCaps,
     RArray<TUint>& aNumChannels)
    {
    if (aDevSoundCaps.iChannels & EMMFMono)
		{
        User::LeaveIfError(aNumChannels.Append(KNumChannelsMono));
		}
    if (aDevSoundCaps.iChannels & EMMFStereo)
		{
        User::LeaveIfError(aNumChannels.Append(KNumChannelsStereo));
		}
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::Panic
// Raise user panic with appropriate panic code
// -----------------------------------------------------------------------------
//
void CDevSoundAudioOutput::Panic(
     TInt aPanicCode) const
    {
    _LIT(KDevSoundAudioOutputPanicCategory, "DevSoundAudioOutput");
    User::Panic(KDevSoundAudioOutputPanicCategory, aPanicCode);
    }

EXPORT_C void CDevSoundAudioOutput::IsLoopPlayEnabled(const TBool aLoopPlayEnabled)
    {
    DP1(_L("CDevSoundAudioOutput::IsLoopPlayEnabled LoopPlayEnabled[%d]"), aLoopPlayEnabled);
    iLoopPlayEnabled = aLoopPlayEnabled;
    }

EXPORT_C void CDevSoundAudioOutput::UnSetLastBuffer(const TBool aUnSetLastBuffer)
    {
    DP1(_L("CDevSoundAudioOutput::UnSetLastBuffer UnSetLastBuffer[%d]"), aUnSetLastBuffer);
    iUnSetLastBuffer = aUnSetLastBuffer;
    }

EXPORT_C TBool CDevSoundAudioOutput::IsDSStopped()
    {
    return iDSStopped;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of file
