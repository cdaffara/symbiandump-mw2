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
* Description:  This file contains the base class from which specific audio
*                record controllers are derived. This class encapsulates common
*                behavior for all audio record controllers.
*
*/



// INCLUDE FILES
#include	"AdvancedAudioRecordController.h"
#include 	"AdvancedAudioResource.h"
#include 	"AdvancedAudioUtility.h"
#include 	<mmffile.h>
#include 	<sysutil.h>

// CONSTANTS
const TUint KDefineIOBufferSize = 512;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::CAdvancedAudioRecordController
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CAdvancedAudioRecordController::CAdvancedAudioRecordController()
	:	iState(EStopped),
		iAudioInput(NULL),
		iAudioResource(NULL),
		iAudioUtility(NULL),
		iSinkWritePosition(0),
		iMaxFileSize(-1)
    {
    RThread().SetPriority(EPriorityRealTime);
    }

EXPORT_C void CAdvancedAudioRecordController::ConstructL()
	{
	CAdvancedAudioController::ConstructL();

    // Construct custom command parsers
    CMMFAudioRecordDeviceCustomCommandParser* audRecDevParser = CMMFAudioRecordDeviceCustomCommandParser::NewL(*this);
    CleanupStack::PushL(audRecDevParser);
    AddCustomCommandParserL(*audRecDevParser);
    CleanupStack::Pop(audRecDevParser);

    CMMFAudioRecordControllerCustomCommandParser* audRecConParser = CMMFAudioRecordControllerCustomCommandParser::NewL(*this);
    CleanupStack::PushL(audRecConParser);
    AddCustomCommandParserL(*audRecConParser);
    CleanupStack::Pop(audRecConParser);
	}

// Destructor
EXPORT_C CAdvancedAudioRecordController::~CAdvancedAudioRecordController()
    {
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::DoStopL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::DoStopL()
    {
    iAudioInput->StopL(); 	// This might call EmptySharedBufferL

    //Check the Sink is Valid since it might have been set to NULL
    // because of an Out of Memory Situation

    if(iDataSink)
    	iDataSink->SinkStopL();
    iTimePositionInMicroSecs = iDuration;
    iState = EStopped;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::WriteIfEnoughSpaceL
//
// Writes data starting from aWritePosition in iBuffer.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::WriteIfEnoughSpaceL(
	CMMFBuffer* aBuffer,
	TInt aWritePosition,
	MDataSource* aSupplier,
	TInt aWriteLen )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::WriteIfEnoughSpaceL"));
#endif

    //Using the outstanding position for determining the correct current write position
    TInt writePosition = aWritePosition+iOutstandingPos;
    if ( SysUtil::DiskSpaceBelowCriticalLevelL(NULL, aBuffer->BufferSize(), iDriveNumber) )
        {
		iDataSink->SinkStopL();
        iDataSink = NULL;
        User::Leave(KErrDiskFull);
        }

    if ( aWriteLen && aSupplier )
        {
        aBuffer->SetStatus(EBeingEmptied);
        static_cast <CMMFClip*>(iDataSink)->WriteBufferL(aWriteLen, aBuffer, writePosition, aSupplier);
        }
    else if ( aSupplier )
        {
        aBuffer->SetStatus(EBeingEmptied);
        static_cast <CMMFClip*>(iDataSink)->WriteBufferL(aBuffer, writePosition, aSupplier);
        }
    else
        {
        aBuffer->SetStatus(EBeingEmptied);
        static_cast <CMMFClip*>(iDataSink)->WriteBufferL(aBuffer, writePosition);
        // Use EUnAvailable since EAvailable changes the buffer size to 0
        aBuffer->SetStatus(EUnAvailable);
        }
    //Update outstanding write position
    if (aBuffer->Status() == EBeingEmptied)   // Async request
        iOutstandingPos += aBuffer->BufferSize();
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::WriteIfMaxFileSizeReachedL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::WriteIfMaxFileSizeReachedL(
	CMMFBuffer* aBuffer )
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAdvancedAudioRecordController::WriteIfMaxFileSizeReachedL"));
#endif
    TInt maxWriteLen = iMaxFileSize - iSinkWritePosition;
    TInt scanLen = 0, frameLen = 0, writeLen = 0;
    TDes8& aBufDes = static_cast <CMMFDataBuffer*> (aBuffer)->Data();

    if ( iSinkWritePosition )
        {
        maxWriteLen = iMaxFileSize - iSinkWritePosition;
        }
    else
        {
        maxWriteLen = iMaxFileSize - iAudioUtility->HeaderOffset();
        }

    TInt seekOffset = iAudioUtility->SeekSync(static_cast <CMMFDataBuffer*> (aBuffer),
                                         aBufDes.Size());

    if ( seekOffset >= maxWriteLen )
        {
#ifdef _DEBUG
    RDebug::Print(_L("Wrote %d bytes..."), writeLen);
#endif
        User::Leave(KErrCompletion);
        }

    scanLen = seekOffset;

    while ( scanLen <= maxWriteLen )
		{
        writeLen = scanLen;

        TAudioFrameInfo frameInfo;
        frameLen = iAudioUtility->FrameInfo(aBufDes.Ptr() + scanLen,
                                       aBufDes.Size() - scanLen, frameInfo);
        if ( !frameLen )
            {
            break;
            }
        scanLen += frameLen;
        }

    if ( writeLen )
        {
        CMMFDataBuffer* buf = CreateSourceBufferOfSizeLC(writeLen);
        static_cast <CMMFDataBuffer*>(buf)->Data().Copy(aBufDes.Ptr(), writeLen);
        WriteIfEnoughSpaceL(buf, iSinkWritePosition, NULL, writeLen); // Synchronous write
        BufferEmptiedL(buf);
        CleanupStack::PopAndDestroy(buf);
        }

#ifdef _DEBUG
    RDebug::Print(_L("Wrote %d bytes..."), writeLen);
#endif
    User::Leave(KErrCompletion);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::AddDataSourceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::AddDataSourceL(
	MDataSource& aSource )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::AddDataSourceL"));
#endif

    if ( iDataSource )
    	{
        User::Leave(KErrAlreadyExists);
		}

    if ( aSource.DataSourceType() == KUidMmfAudioInput )
        {
		iDataSource = &aSource;
		DoAddDataSourceL();
		}
    else
        {
        User::Leave(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::AddDataSinkL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::AddDataSinkL(
	MDataSink& aSink )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::AddDataSinkL"));
#endif
    if ( iDataSink )
    	{
        User::Leave(KErrAlreadyExists);
		}

    if ( aSink.DataSinkType() == KUidMmfFileSink )
        {
        iDataSink = &aSink;
//        iDataSink->SinkPrimeL();
//        iDataSink->SinkThreadLogon(*this);
//		iDriveNumber = iAudioUtility->GetDriveNumber(static_cast <CMMFFile*> (iDataSink)->FileDrive());
        TRAPD(err, DoAddDataSinkL());

        if (err)
        	{
			if(iDataSink)
			{
				iDataSink->SinkStopL();
	        	iDataSink = NULL;
        	}
        	User::Leave(err);
        	}
        }
     else
        {
        User::Leave(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::RemoveDataSourceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::RemoveDataSourceL(
	MDataSource& aDataSource )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::RemoveDataSourceL"));
#endif
	if ( !iDataSource )
		{
        User::Leave(KErrNotReady);
		}

    if ( iDataSource != &aDataSource )
    	{
        User::Leave(KErrArgument);
		}

    if ( iState == ERecording )
    	{
        User::Leave(KErrNotReady);
		}

	delete iAudioInput;
	iAudioInput = NULL;
    iDataSource = NULL;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::RemoveDataSinkL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::RemoveDataSinkL(
	MDataSink& aDataSink )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::RemoveDataSourceL"));
#endif
    if ( !iDataSink )
		{
        User::Leave(KErrNotReady);
		}

    if ( iDataSink != &aDataSink )
		{
        User::Leave(KErrArgument);
		}

    if ( iState == ERecording )
		{
        User::Leave(KErrNotReady);
		}

	iDataSink->SinkThreadLogoff();
	iDataSink->SinkStopL();
    iDataSink = NULL;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::ResetL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::ResetL()
    {
    RemoveDataSourceL(*iDataSource);
    RemoveDataSinkL(*iDataSink);
    //ResetSharedBuffersL(iSharedBufferMaxNum, iSharedBufferMaxSize);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::PrimeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::PrimeL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::PrimeL"));
#endif
    if ( iState == ERecording )
    	{
        User::Leave(KErrNotReady);
		}

    CMMFClip* clip = static_cast <CMMFClip*> (iDataSink);
    clip->SinkPrimeL();
    clip->SinkThreadLogon(*this);

	// Read the default codec configuration parameters from resource file
	RArray<TInt>& codecConfigData = CONST_CAST(RArray<TInt>&, iAudioResource->CodecConfigParametersL());
	iAudioInput->ConfigureL(iSampleRate, iChannels, iDataType, codecConfigData);
    iAudioInput->PrimeL();
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::Play
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::PlayL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::PlayL"));
#endif
    ResetSharedBuffersL(iSharedBufferMaxNum, iSharedBufferMaxSize);
    for (TInt i = 0; i < iSharedBufferMaxNum; i++)
        {
        iSharedBuffers[i]->SetStatus(EAvailable);
        }
    iOutstandingPos = 0;
    iAudioInput->RecordL(&iSharedBuffers);
    iState = ERecording;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::StopL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::StopL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::StopL [state=%d]"), iState);
#endif
    switch ( iState )
        {
        case ERecording:
            iState = EStopping;
            DoStopL();
            SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, KErrNone));
            break;

        case EWritingStopped:
            DoStopL();
            break;

        case EStopped:
        case EStopping:
            break;

        default:
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::StopL Panic iState [%d]"), iState);
#endif
            Panic(KErrGeneral);
            break;
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::PositionL
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalMicroSeconds CAdvancedAudioRecordController::PositionL() const
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::PositionL"));
#endif
    if ( iState == ERecording )
        {
        return TTimeIntervalMicroSeconds(iTimePositionInMicroSecs + iAudioInput->CalculateAudioInputPositionL());
        }
    else
        {
        return TTimeIntervalMicroSeconds(iTimePositionInMicroSecs);
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::SetPositionL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::SetPositionL(
	const TTimeIntervalMicroSeconds& aPosition )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioRecordController::SetPositionL(%d)"), I64INT(aPosition.Int64()));
#endif
    if ( aPosition.Int64() == iDuration )
        {// We hit the end boundary
        iTimePositionInMicroSecs = aPosition.Int64();
        iSinkWritePosition = static_cast <CMMFClip*> (iDataSink)->Size();
        }
    else if ( aPosition.Int64() == 0 )
        {
        iTimePositionInMicroSecs = 0;
        iSinkWritePosition = 0;
        }
    else // Non-boundary request
        {
        TInt framePos = iAudioUtility->FindFramePos(aPosition.Int64());
        if ( framePos != KErrNotFound )
            {
            iTimePositionInMicroSecs = aPosition.Int64();
            iSinkWritePosition = framePos + iAudioUtility->HeaderOffset();
            }
        else
            {
            User::Leave(KErrArgument);
            }
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSourceSampleRateL
// Returns the sampling rate obtrained from the audio input.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSourceSampleRateL(
	TUint& aRate )
    {
    if ( !iDataSource )
    	{
        User::Leave(KErrNotReady);
		}

    RArray<TUint> rates;
    CleanupClosePushL(rates);
    iAudioInput->ConfigRatesL(rates);

    ASSERT(rates.Count() == 1);
    aRate = rates[0];
    CleanupStack::PopAndDestroy(&rates);	// rates
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSourceBitRateL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSourceBitRateL(
	TUint& /*aRate*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSourceNumChannelsL
// Returns the number of channels obtrained from the audio input.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSourceNumChannelsL(
	TUint& aNumChannels )
    {
    if ( !iDataSource )
    	{
        User::Leave(KErrNotReady);
		}

    RArray<TUint> channels;
    CleanupClosePushL(channels);
    iAudioInput->ConfigChannelsL(channels);

    ASSERT(channels.Count() == 1);
    aNumChannels = channels[0];
    CleanupStack::PopAndDestroy(&channels);	// channels
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSourceDataTypeL
// Returns the data type obtrained from the audio input.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSourceDataTypeL(
	TFourCC& aDataType )
    {
    if ( !iDataSource )
    	{
        User::Leave(KErrNotReady);
		}

    RArray<TFourCC> dataTypes;
    CleanupClosePushL(dataTypes);
    iAudioInput->ConfigDataTypesL(dataTypes);

    ASSERT(dataTypes.Count() == 1);
    aDataType = dataTypes[0];
    CleanupStack::PopAndDestroy(&dataTypes);	// dataTypes
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSinkSampleRateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSinkSampleRateL(
	TUint& aRate )
    {
    if ( !iDataSink )
    	{
        User::Leave(KErrNotReady);
    	}
    aRate = iSampleRate;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSinkBitRateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSinkBitRateL(
	TUint& aRate )
    {
    if ( !iDataSink )
    	{
        User::Leave(KErrNotReady);
    	}
    aRate = iBitRate;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSinkNumChannelsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSinkNumChannelsL(
	TUint& aNumChannels )
    {
    if ( !iDataSink )
    	{
        User::Leave(KErrNotReady);
    	}
    aNumChannels = iChannels;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSinkDataTypeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSinkDataTypeL(
	TFourCC& aDataType )
    {
    if ( !iDataSink )
    	{
        User::Leave(KErrNotReady);
		}
    aDataType = iDataType;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSupportedSourceSampleRatesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSupportedSourceSampleRatesL(
	RArray<TUint>& aSupportedRates )
    {
    if ( !iDataSource )
    	{
        User::Leave(KErrNotReady);
		}
    aSupportedRates.Reset();
    iAudioInput->CapabilitiesRatesL(aSupportedRates);
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSupportedSourceBitRatesL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSupportedSourceBitRatesL(
	RArray<TUint>& /*aSupportedRates*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSupportedSourceNumChannelsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSupportedSourceNumChannelsL(
	RArray<TUint>& aSupportedChannels )
    {
    if ( !iDataSource )
    	{
        User::Leave(KErrNotReady);
		}
    aSupportedChannels.Reset();
    iAudioInput->CapabilitiesChannelsL(aSupportedChannels);
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSupportedSourceDataTypesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSupportedSourceDataTypesL(
	RArray<TFourCC>& aSupportedDataTypes )
    {
    if ( !iDataSource )
    	{
        User::Leave(KErrNotReady);
		}
    aSupportedDataTypes.Reset();
    iAudioInput->CapabilitiesDataTypesL(aSupportedDataTypes);
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSupportedSinkSampleRatesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSupportedSinkSampleRatesL(
	RArray<TUint>& aSupportedRates )
    {
    if ( !iDataSink )
    	{
        User::Leave(KErrNotReady);
		}
    aSupportedRates.Reset();
    User::LeaveIfError(aSupportedRates.Append(iSampleRate));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSupportedSinkBitRatesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSupportedSinkBitRatesL(
	RArray<TUint>& aSupportedRates )
    {
    if ( !iDataSink )
    	{
        User::Leave(KErrNotReady);
		}
    aSupportedRates.Reset();
    User::LeaveIfError(aSupportedRates.Append(iBitRate));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSupportedSinkNumChannelsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSupportedSinkNumChannelsL(
	RArray<TUint>& aSupportedChannels )
    {
    if ( !iDataSink )
    	{
        User::Leave(KErrNotReady);
		}
    aSupportedChannels.Reset();
    User::LeaveIfError(aSupportedChannels.Append(iChannels));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MacGetSupportedSinkDataTypesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MacGetSupportedSinkDataTypesL(
	RArray<TFourCC>& aSupportedDataTypes )
    {
    if ( !iDataSink )
    	{
        User::Leave(KErrNotReady);
		}
    aSupportedDataTypes.Reset();
    User::LeaveIfError(aSupportedDataTypes.Append(iDataType));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MarcGetRecordTimeAvailableL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MarcGetRecordTimeAvailableL(
	TTimeIntervalMicroSeconds& /*aTime*/ )
    {
	User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MarcSetMaxDurationL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MarcSetMaxDurationL(
	const TTimeIntervalMicroSeconds& /*aMaxDuration*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MarcSetMaxFileSizeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MarcSetMaxFileSizeL(
	TInt aFileSize )
    {
    iMaxFileSize = aFileSize;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MarcCropL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MarcCropL(
	TBool aToEnd )
    {
    TInt clipSize = static_cast <CMMFClip*> (iDataSink)->Size();
    TInt64 position = PositionL().Int64();
    TInt64 duration = DurationL().Int64();

    if ( !clipSize )
        {
        User::Leave(KErrNotFound);
        }

    TInt cropPosition = 0;

    // Convert current position to cropPosition in bytes
    if ( iSinkWritePosition == clipSize )
        { // end boundary
        cropPosition = clipSize;
        }
    else if ( iSinkWritePosition == 0 )
        { // start boundary
        cropPosition = 0;
        }
    else
        {
        cropPosition = iAudioUtility->FindFramePos(position) + iAudioUtility->HeaderOffset();
        }

    TUint dataSize;  // This will be the size of the data left after cropping.

    if ( !aToEnd && !cropPosition )
        {
        iDataSink->SinkStopL();
        return;
        }
    else if ( !aToEnd ) // crop to start
        {
        // Shift the data physically
        // move the data in blocks
        // Create a CMMFDataBuffer and use CMMFClip to shift the data
        dataSize = clipSize - cropPosition;
        if ( dataSize > 0 )
            {
            TUint bufSize = (dataSize < KDefineIOBufferSize ? dataSize : KDefineIOBufferSize); //max bufSize 512
            CMMFDataBuffer* buffer = CMMFDataBuffer::NewL(bufSize);
            CleanupStack::PushL(buffer);

            TInt rPos = cropPosition; // read position
            TInt wPos = 0;
            TInt dataToShift = ETrue;
            while ( dataToShift )
                {
                static_cast <CMMFClip*>(iDataSink)->ReadBufferL(buffer, rPos);  // synchronous calls
                if ( wPos == 0 )
                    {
                    WriteHeaderL();
                    wPos = iAudioUtility->HeaderOffset();
                    dataSize += wPos;
                    }
                WriteIfEnoughSpaceL(buffer, wPos);
                if ( rPos > clipSize )
                    {
                    dataToShift = EFalse;  // past the end:  Done
                    }
                else
                    { // shift the pointers
                    rPos += bufSize;
                    wPos += bufSize;
                    }
                }// while data to shift
            CleanupStack::PopAndDestroy(buffer); // buffer
            }// if data to shift
        }
    else // crop to end
        {
        dataSize = cropPosition;
        }

    // Physical chop
    static_cast <CMMFFile*> (iDataSink)->FileL().SetSize(dataSize);

    delete iAudioUtility;
    iAudioUtility = NULL; // Get rid of old position array
    DoAddDataSinkL();	// AK - Used to be InitSinkDataL(EFalse);

    if ( !aToEnd )
        {
        iSinkWritePosition = static_cast <CMMFClip*> (iDataSink)->Size();
        iTimePositionInMicroSecs = duration - position;
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MarcAddMetaDataEntryL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MarcAddMetaDataEntryL(
	const CMMFMetaDataEntry& /*aNewEntry*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MarcRemoveMetaDataEntryL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MarcRemoveMetaDataEntryL(
	TInt /*aIndex*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MarcReplaceMetaDataEntryL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MarcReplaceMetaDataEntryL(
	TInt /*aIndex*/,
	const CMMFMetaDataEntry& /*aNewEntry*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MardSetGainL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MardSetGainL(
	TInt aGain )
    {
    if ( !iDataSource )
    	{
        User::Leave(KErrNotReady);
		}

    if( aGain > iAudioInput->MaxGainL() )
    	{
        User::Leave(KErrArgument);
		}

    iAudioInput->SetGainL(aGain);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MardGetMaxGainL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MardGetMaxGainL(
	TInt& aMaxGain )
    {
    if ( !iDataSource )
    	{
        User::Leave(KErrNotReady);
		}

    aMaxGain = iAudioInput->MaxGainL();
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MardGetGainL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MardGetGainL(
	TInt& aGain )
    {
    if ( !iDataSource )
    	{
        User::Leave(KErrNotReady);
		}

    aGain = iAudioInput->GainL();
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MardSetBalanceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MardSetBalanceL(
	TInt /*aBalance*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::MardGetBalanceL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::MardGetBalanceL(
	TInt& /*aBalance*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::BufferEmptiedL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::BufferEmptiedL(
	CMMFBuffer* aBuffer )
    {
    iSinkWritePosition += aBuffer->BufferSize();

    if (aBuffer->Status() == EBeingEmptied)    // Async request callback
        iOutstandingPos -= aBuffer->BufferSize();
#ifdef _DEBUG
    RDebug::Print(_L("CAdvancedAudioRecordController::BufferEmptiedL iSinkWritePosition [%d]"), iSinkWritePosition);
#endif

    iAudioUtility->ScanFramesL(static_cast <CMMFClip*> (iDataSink), static_cast <CMMFDataBuffer*> (aBuffer), ETrue);
    iSampleRate = iAudioUtility->SampleRate();
    iBitRate = iAudioUtility->BitRate();
    iChannels = iAudioUtility->Channels();
    iDuration = iAudioUtility->ScanDuration();
    aBuffer->SetStatus(EAvailable);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::EmptyBufferL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioRecordController::EmptyBufferL(
	CMMFBuffer* aBuffer )
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAdvancedAudioRecordController::EmptyBufferL iSinkWritePosition: %d"), iSinkWritePosition);
#endif
    TInt retVal = KErrNone;
    TRAP(retVal, DoEmptySharedBufferL(aBuffer));
    if ( retVal == KErrCompletion )
        {
        iState = EWritingStopped;
        TRAPD(err, DoStopL());
		err = err; //to suppress warnings
        SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, KErrEof));
        }
    else if ( retVal )
        {
#ifdef _DEBUG
    RDebug::Print(_L("Error emptying shared buffer!!! %d"), retVal);
#endif
        iState = EWritingStopped;
        TRAPD(err, DoStopL());
		err = err;
        SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, retVal));
        }
    return retVal;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::RecordComplete
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::RecordComplete()
    {
    iState = EStopped;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::SendEvent
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioRecordController::SendEvent(
	const TMMFEvent& aEvent )
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAdvancedAudioRecordController::SendEvent[%d] this[%x]"), aEvent.iErrorCode, this);
#endif
    // fix for DALM-853QJX, handling preemption use case during recording, this handles only preemption errors and other errors are ignored
    // MMFDevSound throws the following error codes incase of any preemption events
    // (DevSound instance has been thrown-off or initial request has been rejected)
    if ( (aEvent.iErrorCode == KErrAccessDenied) || (aEvent.iErrorCode == KErrInUse) || (aEvent.iErrorCode == KErrDied) )
        {
        // this might be a DevSound Preemption
        iState = EStopping;
        TRAP_IGNORE(DoStopL());
        }
    SendEventToClient(aEvent);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioRecordController::DoEmptySharedBufferL
// -----------------------------------------------------------------------------
//
TInt CAdvancedAudioRecordController::DoEmptySharedBufferL(
	CMMFBuffer* aBuffer )
    {
    switch ( iState )
        {
        case ERecording:
            if ( (iMaxFileSize != -1 && // Max file size is set
                 !iSinkWritePosition && // Zero file size
                 (aBuffer->BufferSize() + iAudioUtility->HeaderOffset() > static_cast <TUint> (iMaxFileSize)))
                 || // OR
                 (iMaxFileSize != -1 &&  // Max file size is set
                 iSinkWritePosition + aBuffer->BufferSize() > static_cast <TUint> (iMaxFileSize)) )
                {
                WriteIfMaxFileSizeReachedL(aBuffer);
                }
            else // Normal case
                {
                WriteIfEnoughSpaceL(aBuffer, iSinkWritePosition, this, aBuffer->BufferSize());
                }
            break;

        case EStopping:
            if ( (iMaxFileSize != -1 && // Max file size is set
                 !iSinkWritePosition && // Zero file size
                 (aBuffer->BufferSize() + iAudioUtility->HeaderOffset() > static_cast <TUint> (iMaxFileSize)))
                 || // OR
                 (iMaxFileSize != -1 && // Max file size is set
                 iSinkWritePosition + aBuffer->BufferSize() > static_cast <TUint> (iMaxFileSize)) )
                {
                WriteIfMaxFileSizeReachedL(aBuffer);
                }
            else
                {
                // Synchronous write
                WriteIfEnoughSpaceL(aBuffer, iSinkWritePosition, NULL, aBuffer->BufferSize());
                BufferEmptiedL(aBuffer);
                }
            break;

        case EWritingStopped:
        case EStopped:
            break;

        default:
            Panic(KErrGeneral);
            break;
        }
    return KErrNone;
    }

// End of file
