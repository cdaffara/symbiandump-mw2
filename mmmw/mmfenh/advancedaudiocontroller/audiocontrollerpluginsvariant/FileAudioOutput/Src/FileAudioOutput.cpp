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
* Description:  The functions in this module implements the behavior that is
*                specifically for file output.
*
*/


// INCLUDE FILES
#include <mmfclip.h>
#include "FileAudioOutput.h"
#include "DebugMacros.h"

// CONSTANTS
const TUint KSinkBufferSize = 4096;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFileAudioOutput::CFileAudioOutput
// -----------------------------------------------------------------------------
//
CFileAudioOutput::CFileAudioOutput()
    :   iDataSink(NULL),
        iSinkWritePosition(0)
    {
    iAdvancedAudioDecoder = NULL;
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::NewL
// Static function for creating an instance of the File Audio Output
// class, derived from the CAdvancedAudioOutput base class.
// -----------------------------------------------------------------------------
//
EXPORT_C CFileAudioOutput* CFileAudioOutput::NewL(
    MDataSink& aDataSink,
    const TMMFPrioritySettings& aPrioritySettings,
    MAdvancedAudioOutputObserver& aObserver)
    {
#ifdef _DEBUG
    RDebug::Print(_L("CFileAudioOutput::NewL"));
#endif
    CFileAudioOutput* self = new (ELeave) CFileAudioOutput;
    CleanupStack::PushL(self);
    self->ConstructL(aDataSink, aPrioritySettings, aObserver);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::ConstructL
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
void CFileAudioOutput::ConstructL(
    MDataSink& aDataSink,
    const TMMFPrioritySettings& /*aPrioritySettings*/,
    MAdvancedAudioOutputObserver& aObserver)
    {
#ifdef _DEBUG
    RDebug::Print(_L("CFileAudioOutput::ConstructL"));
#endif

    iDataSink = &aDataSink;
    iClip = static_cast<CMMFClip*>(iDataSink);
    iObserver = &aObserver;

    iFAOEventGenerator = CFAOEventGenerator::NewL(*this);

    iSinkBuffer = CMMFDataBuffer::NewL(KSinkBufferSize);

    iSinkBuffer->Data().FillZ(KSinkBufferSize);
    iSinkBuffer->Data().SetLength(0);

    iState = EIdle;
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::~CFileAudioOutput
// Standard SymbianOS destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CFileAudioOutput::~CFileAudioOutput()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CFileAudioOutput::~CFileAudioOutput"));
#endif

    iCodecConfigData.Close();
    delete iAdvancedAudioDecoder;
    delete iSinkBuffer;
    delete iFAOEventGenerator;
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::PrimeL
// Prepare to start playing.
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::PrimeL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CFileAudioOutput::PrimeL"));
#endif
    if (!iAdvancedAudioDecoder)
        {
        // SetDecoder hasn't been called!!!
        User::Leave(KErrNotReady);
        }
    //BuildConfigurationParameters();
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::PauseL
// Send pause command to DevSound pause and set own state to paused
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::PauseL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CFileAudioOutput::PauseL"));
#endif

    iState = EPaused;
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::PlayL
// Start playback process by configuring DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::PlayL(TAny* aBuffer, TInt aIndex)
    {
#ifdef _DEBUG
    RDebug::Print(_L("CFileAudioOutput::PlayL"));
#endif

    // Give the audio converter a handle to the source buffers
    iAdvancedAudioDecoder->SetSourceBuffers(reinterpret_cast<RPointerArray<CMMFDataBuffer>*>(aBuffer));
    // Reset the converter
    iAdvancedAudioDecoder->ResetL();
    // Configure the audio converter
    iAdvancedAudioDecoder->SetConfigL(iSourceSampleRate, iSourceChannels, iSourceSampleRate,
                                        iSourceChannels, iCodecConfigData, aIndex);

    iAdvancedAudioDecoder->FillBufferL(iSinkBuffer);
    iState = EPlaying;
    }


// -----------------------------------------------------------------------------
// CFileAudioOutput::BuildConfigurationParameters
// Noting to do for file output.
// -----------------------------------------------------------------------------
//
void CFileAudioOutput::BuildConfigurationParameters()
    {
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::StopL
// Send stop command to DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::StopL(TBool /* aStopDevsound*/ )
    {
#ifdef _DEBUG
    RDebug::Print(_L("CFileAudioOutput::StopL"));
#endif

    if(iState != EIdle)
        {
        iState = EIdle;
        }

    iFAOEventGenerator->GenerateEvent(KIOError, KErrCancel);

    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::MaxVolumeL
// Get maximum gain volume from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CFileAudioOutput::MaxVolumeL()
    {
    User::Leave(KErrNotSupported);
    return 0;
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::SetVolumeL
// Sends given new volume value to DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::SetVolumeL(TInt /*aVolume*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::VolumeL
// Get current volume value from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CFileAudioOutput::VolumeL()
    {
    User::Leave(KErrNotSupported);
    return 0;
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::SetPrioritySettings
// Nothing to do for file output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::SetPrioritySettingsL(
    const TMMFPrioritySettings& /*aPrioritySettings*/)
    {
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::CalculateAudioOutputPosition
// Gets the current playback position in terms of microseconds. This time position
// is from the time playback started or resumed and is reset after a stop or
// pause.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt64 CFileAudioOutput::CalculateAudioOutputPositionL() const
    {
    User::Leave(KErrNotSupported);
    return 0;
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::ConfigRatesL
// Get configured sampling rates from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::ConfigRatesL(
    RArray<TUint>& /*aRates*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::ConfigChannelsL
// Get configured sampling rates from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::ConfigChannelsL(
    RArray<TUint>& /*aChannels*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::ConfigDataTypesL
// Get configured data types (FourCC) from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::ConfigDataTypesL(
    RArray<TFourCC>& /*aDataTypes*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::CapabilitiesRatesL
// CDevSoundAudioInput::CapabilitiesRatesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::CapabilitiesRatesL(
    RArray<TUint>& /*aRates*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::CapabilitiesChannelsL
// Get DevSound capabilities for channels
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::CapabilitiesChannelsL(
    RArray<TUint>& /*aChannels*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::CapabilitiesDataTypesL
// Get DevSound capabilities for data types (FourCC)
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::CapabilitiesDataTypesL(
    RArray<TFourCC>& /*aDataTypes*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::SetVolumeRampL
// Send new ramp duration to DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::SetVolumeRampL(
    const TTimeIntervalMicroSeconds& /*aRampDuration*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::SetPlayBalanceL
// Send new channel balance information (left and right channel) to DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::SetPlayBalanceL(
    TInt /*aLeftPercentage*/,
    TInt /*aRightPercentage*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::GetPlayBalanceL
// Get current playback channel balance information (for left and right channel)
// from DevSound
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::GetPlayBalanceL(
    TInt& /*aLeftPercentage*/,
    TInt& /*aRightPercentage*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::ConfigureL
// Save configuration information and initializes DevSound and codec.
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::ConfigureL(
    TUint aSampleRate,
    TUint aNumChannel,
    TFourCC aFourCC,
    const RArray<TInt>& aCodecConfigData)
    {
#ifdef _DEBUG
    RDebug::Print(_L("CFileAudioOutput::ConfigureL"));
#endif
    iSourceSampleRate = aSampleRate;
    iSourceChannels = aNumChannel;
    iSourceFourCC = aFourCC;

    for (TInt i = 0; i < aCodecConfigData.Count(); i++)
        {
        //RDebug::Print(_L("Index %d [%d]"), i, aCodecConfigData[i]);
        User::LeaveIfError(iCodecConfigData.Append(aCodecConfigData[i]));
        }
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::SetDecoder
// Sets the Decoder instance to be used by the audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::SetDecoder(
    CAdvancedAudioDecoder* aDecoder)
    {
#ifdef _DEBUG
    RDebug::Print(_L("CDevSoundAudioOutput::SetDecoder"));
#endif
    iAdvancedAudioDecoder = aDecoder;

    iAdvancedAudioDecoder->SetObserver(*this);
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::SetDataSourceAdapter
// function from base class not needed here 
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::SetDataSourceAdapter(CDataSourceAdapter* /*aDataSourceAdapter*/)
    {
    
    }


// -----------------------------------------------------------------------------
// CFileAudioOutput::Resume
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CFileAudioOutput::Resume(TInt /*aBufferIndex*/)
    {
    DP0(_L("CFileAudioOutput::Resume"));
    if (iSavedBufferPtr)
        {
        // Have the decoder fill the specified buffer with decoded audio.
        TRAPD(err,iAdvancedAudioDecoder->FillBufferL(iSavedBufferPtr));
        if (err)
            {
            iFAOEventGenerator->GenerateEvent(KIOError, err);
            }
        }
    else
        {
        DP0(_L("CFileAudioOutput::Resume ignored"));
        }
    }
// -----------------------------------------------------------------------------
// CFileAudioOutput::BufferFilled
// Notification from Audio Converter that the specified buffer has been filled.
// -----------------------------------------------------------------------------
//
void CFileAudioOutput::BufferFilled(
    CMMFBuffer* aBuffer)
     {
#ifdef _DEBUG
     RDebug::Print(_L("CFileAudioOutput::BufferFilled"));
#endif
    if (iState != EIdle)
        {
        // Write data to sink
        iSavedBufferPtr = NULL;
        TRAPD(err, iClip->WriteBufferL(aBuffer, iSinkWritePosition));
        if (err)
            {
            iFAOEventGenerator->GenerateEvent(KIOError, err);
            }
        else
            {
            if (aBuffer->LastBuffer())
                {
                iState = EPreIdle;
                iFAOEventGenerator->GenerateEvent(KIOError, KErrUnderflow);
                return;
                }
            iSinkWritePosition += iSinkBuffer->Data().Length();
            iSinkBuffer->Data().FillZ(KSinkBufferSize);
            iSinkBuffer->Data().SetLength(0);
            iSavedBufferPtr = iSinkBuffer;
            TRAPD(err,iAdvancedAudioDecoder->FillBufferL(iSinkBuffer));
            if (err)
            {
            iFAOEventGenerator->GenerateEvent(KIOError, err);
            }
            }
        }
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::RefillBuffer
// Notification from Audio Converter that the specified buffer needs to be
// refilled.
// -----------------------------------------------------------------------------
//
void CFileAudioOutput::RefillBuffer(
    CMMFBuffer* aEmptyBuffer)
    {
     // forward the request to the controller
    //iObserver->RefillBuffer(aEmptyBuffer, ETrue);
    iObserver->RefillBuffer(aEmptyBuffer);
    }

/***
// -----------------------------------------------------------------------------
// CFileAudioOutput::RefillBuffer
// Notification from Audio Converter that the specified buffer needs to be
// refilled.
// -----------------------------------------------------------------------------
//
void CFileAudioOutput::RefillBuffer(
    CMMFBuffer* aEmptyBuffer, TBool aDoSynchronousRead)
    {
     // forward the request to the controller
    iObserver->RefillBuffer(aEmptyBuffer, aDoSynchronousRead);
    }
***/

// -----------------------------------------------------------------------------
// CFileAudioOutput::SendEvent
// -----------------------------------------------------------------------------
//
void CFileAudioOutput::SendEvent(
    const TMMFEvent& aEvent)
    {
    iObserver->SendEvent(aEvent);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioOutput::IOError
// Callback error to indicate has error occurred during writing
// -----------------------------------------------------------------------------
//
void CFileAudioOutput::IOError(
    TInt aError)
     {
#ifdef _DEBUG
     RDebug::Print(_L("CFileAudioOutput::IOError(%d) iState[%d]"), aError, iState);
#endif

     if (aError == KErrUnderflow && (iState == EIdle || iState == EPreIdle))
        {
        iState = EIdle;
        iObserver->PlaybackComplete();
        }
    else
        {
        iState = EIdle;
        iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
        }
    }

// -----------------------------------------------------------------------------
// CFileAudioOutput::Panic
// Raise user panic with appropriate panic code
// -----------------------------------------------------------------------------
//
void CFileAudioOutput::Panic(
     TInt aPanicCode) const
    {
    _LIT(KFileAudioOutputPanicCategory, "FileAudioOutput");
    User::Panic(KFileAudioOutputPanicCategory, aPanicCode);
    }

EXPORT_C void CFileAudioOutput::IsLoopPlayEnabled(const TBool /*aLoopPlayEnabled*/)
    {
    }

EXPORT_C void CFileAudioOutput::UnSetLastBuffer(const TBool /*aUnSetLastBuffer*/)
    {
    }
EXPORT_C TBool CFileAudioOutput::IsDSStopped()
    {
    return EFalse;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of file
