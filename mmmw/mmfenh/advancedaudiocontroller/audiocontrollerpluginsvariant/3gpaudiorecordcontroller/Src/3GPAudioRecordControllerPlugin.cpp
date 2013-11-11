/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation file for 3GPAudioRecordControllerPlugin
*
*/


#include "3GPAudioRecordControllerPluginUIDs.hrh"
#include "3GPAudioRecordControllerPlugin.h"
#include "3GPAudioRecordControllerEncoderBuilder.h"
#include "3GPAudioRecordControllerResource.h"
#include "DevSoundAudioInput.h"
#include <mmffile.h>
#include "DebugMacros.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::C3GPAudioRecordControllerPlugin
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
C3GPAudioRecordControllerPlugin::C3GPAudioRecordControllerPlugin()
    :     iMP4Handle(NULL),
        iEncoderBuilder(NULL),
        iBuffer(NULL)
    {
    iEnableDelayWrite = EFalse;
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::ConstructL()
    {
    CAdvancedAudioRecordController::ConstructL();
    DP0(_L("C3GPAudioRecordControllerPlugin::ConstructL(1)"));
    iEncoderBuilder = C3GPAudioRecordControllerEncoderBuilder::NewL();
    DP0(_L("C3GPAudioRecordControllerPlugin::ConstructL(2)"));
    iAudioResource = C3GPAudioRecordControllerResource::NewL(K3GPResourceAAC);

    TAapProperties aapProperties = iAudioResource->PropertiesL();
    DP0(_L("C3GPAudioRecordControllerPlugin::ConstructL(3)"));
    iSharedBufferMaxNum = aapProperties.iSharedBufferMaxNum;    // default size
    iSharedBufferMaxSize = aapProperties.iSharedBufferMaxSize;    // default size
    iSharedBufferMaxSizeForNonSeekableSrc = aapProperties.iSharedBufferMaxSizeForNonSeekableSrc;

    iMetaData = C3GPAudioRecordControllerMetaData::NewL();

    iMetaData->AddMetaDataWriteParserL(this);
    RArray<TInt>& codecConfigData = CONST_CAST(RArray<TInt>&, iAudioResource->CodecConfigParametersL());

    iSampleRate = codecConfigData[1]; // default sample rate for devsound
    iChannels = codecConfigData[3];   // default channels for devsound

    DP0(_L("C3GPAudioRecordControllerPlugin::ConstructL(4)"));
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
C3GPAudioRecordControllerPlugin* C3GPAudioRecordControllerPlugin::NewL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::NewL"));
    C3GPAudioRecordControllerPlugin* self = new(ELeave) C3GPAudioRecordControllerPlugin;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::~C3GPAudioRecordControllerPlugin
// Destructor
// -----------------------------------------------------------------------------
//
C3GPAudioRecordControllerPlugin::~C3GPAudioRecordControllerPlugin()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::~C3GPAudioRecordControllerPlugin"));

    TRAP_IGNORE(CloseMP4ComposerL());
    delete iMetaData;
    delete iEncoderBuilder;
    delete iAudioResource;
    delete iAudioInput;
    delete iBuffer;

    iSupportedChannels.Close();
    iSharedBuffers.ResetAndDestroy();
    iSharedBuffers.Close();
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::DoAddDataSourceL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::DoAddDataSourceL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::DoAddDataSourceL"));

    iAudioInput = CDevSoundAudioInput::NewL(iPrioritySettings, *this, *iMMFDevSound);

    iEncoderType = K3GPEncoder;
    iDataType = TFourCC(' ','A','A','C');

    if ( iDataSink )
        {
        CAdvancedAudioEncoder* encoder = iEncoderBuilder->BuildEncoderL(iEncoderType, iDataType);
        // AudioInput takes ownership of Encoder object
        iAudioInput->SetEncoder(encoder);
        iAudioInput->PrimeL();
        DetermineSupportedNumChannelsL();
        iAudioInput->StopL();
        }
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::DoAddDataSinkL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::DoAddDataSinkL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::DoAddDataSinkL"));
    CMMFClip* clip = STATIC_CAST(CMMFClip*, iDataSink);

    if ( iDataSink->DataSinkType() == KUidMmfFileSink )
        {
        iSinkType = KUidMmfFileSink;
        clip->SinkPrimeL();
        clip->SinkThreadLogon(*this);
        iDriveNumber = GetDriveNumber(STATIC_CAST(CMMFFile*, iDataSink)->FileDrive());
        
        TInt size = clip->Size();
 		if (size == 0)     // check for size = 0, to verify whether header has been written
            {
            WriteHeaderL();
            }

        clip->SinkStopL();
        TRAP_IGNORE(UpdateMP4DurationL());      // get duration if file contains valid recorded data
        }
    else    // KUidMmfDescriptorSink
        {
        iSinkType = KUidMmfDescriptorSink;
        }

    if ( iDataSource )
        {
        CAdvancedAudioEncoder* encoder = iEncoderBuilder->BuildEncoderL(iEncoderType, iDataType);
        // AudioInput takes ownership of Encoder object
        iAudioInput->SetEncoder(encoder);
        iAudioInput->PrimeL();
        DetermineSupportedNumChannelsL();
        iAudioInput->StopL();
        }
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::DoStopL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::DoStopL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::DoStopL"));

    iAudioInput->StopL();

    if (!iEnableDelayWrite)
        DoCommitMetaDataL();
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::EmptyBufferL
// -----------------------------------------------------------------------------
//
TInt C3GPAudioRecordControllerPlugin::EmptyBufferL(
    CMMFBuffer* aBuffer )
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::EmptyBufferL"));

    TUint buffersize = STATIC_CAST(CMMFDataBuffer*, aBuffer)->Data().Length();
    CMMFDataBuffer* buf = STATIC_CAST(CMMFDataBuffer*, aBuffer);
    TUint8* buffer = CONST_CAST(TUint8*, buf->Data().Ptr());

    MP4Err err = MP4ComposeWriteAudioFrames(iMP4Handle, buffer, buffersize, 0, 1024);

    aBuffer->SetStatus(EAvailable);
    iDuration = PositionL().Int64();   // update duration during recording state, base on position
    
    return TranslateMP4Err(err);
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::PauseL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::PauseL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::PauseL"));

    User::Leave(KErrNotSupported);
    
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::PrimeL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::PrimeL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::PrimeL"));

    if ( iState == ERecording )
        {
        User::Leave(KErrNotReady);
        }
    
    // Since Append is not supported on Recording, this function should leave if Duration is not equal to 0
    if (iDuration == 0)
        {
        PrepareMP4ComposerL();

        // Read the default codec configuration parameters from resource file
        RArray<TInt>& codecConfigData = CONST_CAST(RArray<TInt>&, iAudioResource->CodecConfigParametersL());

        codecConfigData[1] = iSampleRate; // configure the decoder with any user modified values.
        codecConfigData[3] = iChannels;
        iAudioInput->ConfigureL(iSampleRate, iChannels, iDataType, codecConfigData);
        iAudioInput->PrimeL();
        }
    else
        User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::Play
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::PlayL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::PlayL"));

    MP4Err err = MP4ComposeAddAudioDescription(iMP4Handle, (mp4_u32)iSampleRate, 1, 0);

    DP1(_L("MP4ComposeAddAudioDescription err = %d"), err);
    User::LeaveIfError(TranslateMP4Err(err));

    ResetSharedBuffersL(iSharedBufferMaxNum, iSharedBufferMaxSize);
    iAudioInput->RecordL(&iSharedBuffers);
    iState = ERecording;
    }


// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::StopL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::StopL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::StopL"));
    switch ( iState )
        {
        case ERecording:
            DoStopL();
            iState = EStopped;
            break;

        case EStopped:
            break;

        default:
            Panic(EBadState);
            break;
        }
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::SetPositionL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::SetPositionL(
    const TTimeIntervalMicroSeconds& aPosition )
    {
    DP1(_L("C3GPAudioRecordControllerPlugin::SetPositionL pos = %d"), aPosition.Int64());

    if ( aPosition > 0 ) // 3gp library doesn't support set position.
        {
        User::Leave(KErrNotSupported);
        }
    }


// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::SendEvent
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::SendEvent(
    const TMMFEvent& aEvent )
    {
    DP1(_L("C3GPAudioRecordControllerPlugin::SendEvent(%d)"), aEvent.iErrorCode);

    iTimePositionInMicroSecs = 0;
    iState = EStopped;
    TRAP_IGNORE(CloseMP4ComposerL());
    SendEventToClient(aEvent);
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::MarcCropL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::MarcCropL(
    TBool /*aToEnd*/ )
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::MarcCropL"));
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::MarcAddMetaDataEntryL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::MarcAddMetaDataEntryL(const CMMFMetaDataEntry& aNewEntry)
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::MarcAddMetaDataEntryL"));
    iMetaData->AddMetaDataEntryL(aNewEntry);
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::PrepareMP4ComposerL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::PrepareMP4ComposerL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::PrepareMP4ComposerL"));
    MP4Err err = 0;

    if ( iSinkType == KUidMmfFileSink )
        {
        CMMFFile* file = STATIC_CAST(CMMFFile*, iDataSink);
        RFile fileHandle;
        TRAP_IGNORE(fileHandle = file->FileL());
        
        if (file->FilePath().Length() && (fileHandle.SubSessionHandle() == 0)) // File Path exists, not file handle
            {
            TFileName fileName = file->FullName();
            err = MP4ComposeOpen(&iMP4Handle, (MP4FileName) fileName.Ptr(), MP4_TYPE_MPEG4_AUDIO);
            DP1(_L("MP4ComposeOpen err = %d - Using File Name"), err);
            }
        else
            {
            err = MP4ComposeOpenFileHandle(&iMP4Handle, &fileHandle, (TDriveNumber)EDriveC, MP4_TYPE_MPEG4_AUDIO);
            DP1(_L("MP4ComposeOpen err = %d - Using File Handle"), err);
            }
        User::LeaveIfError(TranslateMP4Err(err));

        mp4_u32 flags = (MP4_FLAG_METADATALAST | MP4_FLAG_GENERATE_MP4 | MP4_FLAG_LONGCLIP );

        err = MP4ComposeSetFlags(iMP4Handle, flags);
        User::LeaveIfError(TranslateMP4Err(err));

        }
    else    // KUidMmfDescriptorSink
        {
        User::Leave(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::UpdateMP4DurationL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::UpdateMP4DurationL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::UpdateMP4DurationL"));
    MP4Err err = 0;

    mp4_u32 audioLength, audioType, timeScale, averateBitRate;
    mp4_u8 framesPerSample;

    if ( iSinkType == KUidMmfFileSink )
        {
        CMMFFile* file = STATIC_CAST(CMMFFile*, iDataSink);
        if (file->FilePath().Length()) // File Path exists, not file handle
            {
            TFileName fileName = file->FullName();
            err = MP4ParseOpen(&iMP4Handle, (MP4FileName) fileName.Ptr());
            DP1(_L("MP4ParseOpen err = %d - Using File Name"), err);
            }
        else
            {
            err = MP4ParseOpenFileHandle(&iMP4Handle, &(file->FileL()));
            DP1(_L("MP4ParseOpen err = %d - Using File Handle"), err);
            }
        User::LeaveIfError(TranslateMP4Err(err));
        }

    err = MP4ParseRequestAudioDescription(iMP4Handle, &audioLength, &audioType,
        &framesPerSample, &timeScale, &averateBitRate);

    MP4ParseClose(iMP4Handle);
    iMP4Handle = NULL;
    User::LeaveIfError(TranslateMP4Err(err));
    iDuration = (TInt64)audioLength*1000;   // convert to microseconds
    }


// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::TranslateMP4Err
// -----------------------------------------------------------------------------
//
TInt C3GPAudioRecordControllerPlugin::TranslateMP4Err(
    MP4Err aError )
    {
    TInt err;
    switch ( aError )
        {
        case MP4_OK:
            err = KErrNone;
            break;
        case MP4_OUT_OF_MEMORY:
            err = KErrNoMemory;
            break;
        case MP4_NOT_AVAILABLE:
            err = KErrNotReady;
            break;
        case MP4_FILE_ERROR:
            err = KErrBadHandle;
            break;
        case MP4_INVALID_TYPE:
            err = KErrNotSupported;
            break;
        case MP4_TIMESCALE_NOT_SET:
            err = KErrNotReady;
            break;
        case MP4_NOT_STREAMABLE:
        case MP4_NO_REQUESTED_FRAME:
        case MP4_CANT_SEEK:
        case MP4_INVALID_INPUT_STREAM:
        case MP4_NO_FRAME:
            err = KErrArgument;
            break;
        case MP4_ERROR:
        case MP4_FILE_MODE:
        case MP4_BUFFER_TOO_SMALL:
        case MP4_END_OF_VIDEO:
        case MP4_NO_VIDEO:
        case MP4_NO_AUDIO:
            err = KErrGeneral;
            break;
        case MP4_METADATA_ERROR:
            err = KErrWrite;
            break;
        default:
            err = KErrGeneral;
            break;
        }
    return err;
    }


// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::WriteDecoderSpecificInfoL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::WriteAudioDecoderSpecificInfoL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::WriteDecoderSpecificInfoL"));

    HBufC8* decSpecInfo = NULL;
    TUint8* buf = NULL;

    decSpecInfo = HBufC8::NewLC(2);
    buf = CONST_CAST(TUint8*, decSpecInfo->Ptr());

    // constructing decoder specific information

    TInt profile = 2; // Object type 2 AAC-LC

    buf[0] = profile << 3;
    TInt sampleFreqIndex = 0;

    switch ( iSampleRate ) // maping sampling rate to sampling frequency index
        {
        case 96000:
            sampleFreqIndex = 0x0;
            break;
        case 88200:
            sampleFreqIndex = 0x1;
            break;
        case 64000:
            sampleFreqIndex = 0x2;
            break;
        case 48000:
            sampleFreqIndex = 0x3;
            break;
        case 44100:
            sampleFreqIndex = 0x4;
            break;
        case 32000:
            sampleFreqIndex = 0x5;
            break;
        case 24000:
            sampleFreqIndex = 0x6;
            break;
        case 22050:
            sampleFreqIndex = 0x7;
            break;
        case 16000:
            sampleFreqIndex = 0x8;
            break;
        case 12000:
            sampleFreqIndex = 0x9;
            break;
        case 11025:
            sampleFreqIndex = 0xa;
            break;
        case 8000:
            sampleFreqIndex = 0xb;
            break;
        default:
            User::Leave(KErrNotSupported);
        }

    buf[0] = buf[0] | (sampleFreqIndex >> 1);
    buf[1] = 0;
    buf[1] = sampleFreqIndex << 7;

    buf[1] = buf[1] | iChannels << 3;

    //buf[1] = 0; // Frame Length Flag ( Don't know what this is ? )
    //buf[1] = 0; // Depends on Core Coder flag ( no )
    //buf[1] = 0; // Extension flag ( no extension )

    MP4Err err = MP4ComposeWriteAudioDecoderSpecificInfo(iMP4Handle, buf, 2);
    User::LeaveIfError(TranslateMP4Err(err));

    CleanupStack::PopAndDestroy(decSpecInfo);    // decSpecInfo


    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::WriteHeaderL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::WriteHeaderL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::WriteHeaderL"));

    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL(32);
    buffer->Data().FillZ(32);
    buffer->Data().SetLength(0);
    TDes8& bufDes =  buffer->Data();

    bufDes.Append(0x00);
    bufDes.Append(0x00);
    bufDes.Append(0x00);
    bufDes.Append(0x1c);
    bufDes.Append(_L("ftypmp4"));

    CleanupStack::PushL(buffer);
    WriteIfEnoughSpaceL(buffer, 0);
    CleanupStack::PopAndDestroy(buffer);


    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::GetDriveNumber
// -----------------------------------------------------------------------------
//
TDriveNumber C3GPAudioRecordControllerPlugin::GetDriveNumber(
    const TDesC& aDriveName )
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::GetDriveNumber"));
    TDriveNumber driveNumber = EDriveC;

    if ( !aDriveName.Length() )
        {
        return driveNumber;
        }
    else
        {
        switch ( aDriveName[0] )
            {
            case 'a':
            case 'A':
                driveNumber = EDriveA;
                break;
            case 'b':
            case 'B':
                driveNumber = EDriveB;
                break;
            case 'c':
            case 'C':
                driveNumber = EDriveC;
                break;
            case 'd':
            case 'D':
                driveNumber = EDriveD;
                break;
            case 'e':
            case 'E':
                driveNumber = EDriveE;
                break;
            case 'f':
            case 'F':
                driveNumber = EDriveF;
                break;
            case 'g':
            case 'G':
                driveNumber = EDriveG;
                break;
            case 'h':
            case 'H':
                driveNumber = EDriveH;
                break;
            case 'i':
            case 'I':
                driveNumber = EDriveI;
                break;
            case 'j':
            case 'J':
                driveNumber = EDriveJ;
                break;
            case 'k':
            case 'K':
                driveNumber = EDriveK;
                break;
            case 'l':
            case 'L':
                driveNumber = EDriveL;
                break;
            case 'm':
            case 'M':
                driveNumber = EDriveM;
                break;
            case 'n':
            case 'N':
                driveNumber = EDriveN;
                break;
            case 'o':
            case 'O':
                driveNumber = EDriveO;
                break;
            case 'p':
            case 'P':
                driveNumber = EDriveP;
                break;
            case 'q':
            case 'Q':
                driveNumber = EDriveQ;
                break;
            case 'r':
            case 'R':
                driveNumber = EDriveR;
                break;
            case 's':
            case 'S':
                driveNumber = EDriveS;
                break;
            case 't':
            case 'T':
                driveNumber = EDriveT;
                break;
            case 'u':
            case 'U':
                driveNumber = EDriveU;
                break;
            case 'v':
            case 'V':
                driveNumber = EDriveV;
                break;
            case 'x':
            case 'X':
                driveNumber = EDriveX;
                break;
            case 'y':
            case 'Y':
                driveNumber = EDriveY;
                break;
            case 'z':
            case 'Z':
                driveNumber = EDriveZ;
                break;
            default:
                break;
            }
        }
        return driveNumber;
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::MacGetSupportedSinkBitRatesL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::MacGetSupportedSinkBitRatesL(
    RArray<TUint>& aSupportedRates )
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::MacGetSupportedSinkBitRatesL"));
    if ( !iDataSink )
        {
        User::Leave(KErrNotReady);
        }

    aSupportedRates.Reset();
    const RArray<TUint>& supportedBitRates = STATIC_CAST(C3GPAudioRecordControllerResource*, iAudioResource)->SupportedBitRatesL();
    TInt count = supportedBitRates.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        User::LeaveIfError( aSupportedRates.Append(supportedBitRates[i]) );
        }
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::MacSetSinkBitRateL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::MacSetSinkBitRateL(
    TUint aRate )
    {
    DP1(_L("C3GPAudioRecordControllerPlugin::MacSetSinkBitRateL [%d]"), aRate);
    if ( !iDataSink || !iDataSource )
        {
        User::Leave(KErrNotReady);
        }

    const RArray<TUint>& supportedBitRates = STATIC_CAST(C3GPAudioRecordControllerResource*, iAudioResource)->SupportedBitRatesL();
    TInt count = supportedBitRates.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        if ( supportedBitRates[i] == aRate )
            {
            iBitRate = aRate;
            // Read the default codec configuration parameters from resource file
            RArray<TInt>& codecConfigData = CONST_CAST(RArray<TInt>&, iAudioResource->CodecConfigParametersL());
            iAudioInput->SetBitRateL(aRate, codecConfigData);
            return;
            }
        }

    // aRate is not one of the supportedBitRates!
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::MacGetSupportedSinkSampleRatesL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::MacGetSupportedSinkSampleRatesL(
    RArray<TUint>& aSupportedRates )
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::MacGetSupportedSinkSampleRatesL"));
    if ( !iDataSink )
        {
        User::Leave(KErrNotReady);
        }

    aSupportedRates.Reset();

    const RArray<TUint>& sinkSupportedSampleRates = STATIC_CAST(C3GPAudioRecordControllerResource*, iAudioResource)->SupportedSampleRatesL();

    RArray<TUint> sourceSupportedSampleRates;
    CleanupClosePushL(sourceSupportedSampleRates);
    iAudioInput->CapabilitiesRatesL( sourceSupportedSampleRates );

    // Check if the source supported sampling rate are the same as sink sampling rate.
    for ( TInt i = 0; i < sourceSupportedSampleRates.Count(); i++ )
        {
        for ( TInt j = 0; j < sinkSupportedSampleRates.Count(); j++ )
            {
            if ( sinkSupportedSampleRates[j] == sourceSupportedSampleRates[i] )
                {
                User::LeaveIfError(aSupportedRates.Append(sourceSupportedSampleRates[i]));
                break;
                }
            }
        }
    CleanupStack::PopAndDestroy(&sourceSupportedSampleRates); // sourceSupportedSampleRates
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::MacSetSinkSampleRateL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::MacSetSinkSampleRateL(
    TUint aSampleRate )
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::MacSetSinkSampleRateL"));

    if ( !iDataSink )
        {
        User::Leave(KErrNotReady);
        }

    const RArray<TUint>& sinkSupportedSampleRates = STATIC_CAST(C3GPAudioRecordControllerResource*, iAudioResource)->SupportedSampleRatesL();

    RArray<TUint> sourceSupportedSampleRates;
    CleanupClosePushL(sourceSupportedSampleRates);
    iAudioInput->CapabilitiesRatesL( sourceSupportedSampleRates );

    // first check if the source supports the sampling rate
    for ( TInt i = 0; i < sourceSupportedSampleRates.Count(); i++ )
        {
        if ( sourceSupportedSampleRates[i] == aSampleRate )
            {
            // then check if the sink also supports the sampling rate
            for ( TInt j = 0; j < sinkSupportedSampleRates.Count(); j++ )
                {
                if ( sinkSupportedSampleRates[j] == aSampleRate )
                    {
                    iSampleRate = aSampleRate;
                    CleanupStack::PopAndDestroy(&sourceSupportedSampleRates); // sourceSupportedSampleRates
                    return;
                    }
                }
            User::Leave(KErrNotSupported);
            }
        }
    User::Leave(KErrNotSupported);
    }
    
// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::MacGetSupportedSinkNumChannelsL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::MacGetSupportedSinkNumChannelsL(
	RArray<TUint>& aSupportedChannels )
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::MacGetSupportedSinkNumChannelsL"));
    if ( !iDataSink )
    	{
        User::Leave(KErrNotReady);
		}
	
	aSupportedChannels.Reset();

    for (TInt i = 0; i < iSupportedChannels.Count(); i++)
        {
        User::LeaveIfError(aSupportedChannels.Append(iSupportedChannels[i]));
        }
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::MacSetSinkNumChannelsL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::MacSetSinkNumChannelsL(
    TUint aNumChannels )
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::MacSetSinkNumChannelsL"));
    if ( !iDataSink )
        {
        User::Leave(KErrNotReady);
        }

    if ( iSupportedChannels.Find(aNumChannels) >= 0 )
        {
        iChannels = aNumChannels;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    }
    
// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::DetermineSupportedNumChannelsL
// -----------------------------------------------------------------------------
//  
void C3GPAudioRecordControllerPlugin::DetermineSupportedNumChannelsL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::DetermineSupportedNumChannelsL"));
    if (iSupportedChannels.Count() == 0)
        {
        iAudioInput->CapabilitiesChannelsL(iSupportedChannels);
        iSupportedChannels.Sort();
        
        TAapProperties aapProperties = iAudioResource->PropertiesL();
        TInt channels = 1;
        if (aapProperties.iStereoSupport)
        	{
        	channels = 2;
        	}

        // number of sink number of channels supported is limited/tied to source can support
        while (channels < iSupportedChannels[iSupportedChannels.Count()-1])
            {
            iSupportedChannels.Remove(iSupportedChannels.Count()-1);
            }
        }
    }
    
// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::AddMetaDataWriteParserL
// Add a MetaDataWrite Parser.
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::AddMetaDataWriteParserL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::AddMetaDataWriteParserL"));
    CMetaDataWriteCustomCommandParser* metaDataWriteParser = CMetaDataWriteCustomCommandParser::NewL(*this);
    CleanupStack::PushL(metaDataWriteParser);
    AddCustomCommandParserL(*metaDataWriteParser);
    CleanupStack::Pop(metaDataWriteParser);
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::MmdwcEnableMetaDataWrite
// -----------------------------------------------------------------------------
//
TInt C3GPAudioRecordControllerPlugin::MmdwcEnableDelayWrite()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::MmdwcEnableDelayWrite"));
        iEnableDelayWrite = ETrue;
        return KErrNone;
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::MmdwcCommitMetaData
// -----------------------------------------------------------------------------
//
TInt C3GPAudioRecordControllerPlugin::MmdwcCommitMetaData()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::MmdwcCommitMetaData"));
    if (iEnableDelayWrite && (iState == EStopped))
        {
        TRAPD(err, DoCommitMetaDataL());

        return err;
        }            
    else
        return KErrNotReady;
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::DoCommitMetaDataL()
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::DoCommitMetaDataL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::DoCommitMetaDataL"));
    if (iMP4Handle)
        {
        TRAPD(err, WriteUdtaL());
        CloseMP4ComposerL();

        if (err != KErrNone)
            User::Leave(KErrWrite);
        }
    else
        User::Leave(KErrNotReady);
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::WriteUdtaL
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::WriteUdtaL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::WriteUdtaL"));
    HBufC8* buffer = iMetaData->GetUdtaAtomL();

    if (buffer)
        {
        mp4_u8 udtalocation = MP4_UDTA_MOOV;
        mp4_u32 udtalength = buffer->Length();

        MP4Err err = MP4ComposeSetUserDataAtom(iMP4Handle, udtalocation, (mp4_u8*)(buffer->Ptr()), udtalength);

        delete buffer;
        User::LeaveIfError(TranslateMP4Err(err));
        }
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerPlugin::CloseMP4ComposerL()
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerPlugin::CloseMP4ComposerL()
    {
    DP0(_L("C3GPAudioRecordControllerPlugin::CloseMP4ComposerL"));
    if (iMP4Handle)
        {
        CMMFClip* clip = STATIC_CAST(CMMFClip*, iDataSink);

        TRAPD(err0, WriteAudioDecoderSpecificInfoL());      // trap leave to ensure handle is closed properly in error situation
        
        TInt err1 = TranslateMP4Err(MP4ComposeClose(iMP4Handle));

        iMP4Handle = NULL;
        clip->SinkStopL();
        User::LeaveIfError(err0);
        User::LeaveIfError(err1);
        UpdateMP4DurationL();
        }
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
    // defined in 3GPAudioRecordControllerPluginUIDs.hrh
    IMPLEMENTATION_PROXY_ENTRY(KUID3GPRecordControllerPluginImplementation, C3GPAudioRecordControllerPlugin::NewL)
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
