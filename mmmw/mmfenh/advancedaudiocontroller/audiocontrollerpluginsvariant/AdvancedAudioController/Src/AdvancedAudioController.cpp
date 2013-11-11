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
* Description:  This class is the top base class in the Advanced Audio Controller
*				 inheritance hierarchy.  Provided are the default implementations.
*
*/



// INCLUDE FILES
#include	"AdvancedAudioController.h"
#include    "CustomInterfaceBuilder.h"
#include	"CustomInterfaceCustomCommandParser.h"
#include    "sounddevice.h"
#include    "DebugMacros.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAdvancedAudioController::CAdvancedAudioController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CAdvancedAudioController::CAdvancedAudioController()
	:	MDataSource(KUidMmfFormatEncode),
    	MDataSink(KUidMmfFormatDecode),
    	iDataSource(NULL),
    	iDataSink(NULL),
    	iDuration(0),
    	iTimePositionInMicroSecs(0)
    {
    }

// Destructor
EXPORT_C CAdvancedAudioController::~CAdvancedAudioController()
    {
        delete iMMFDevSound;
    }

void CAdvancedAudioController::ConstructL()
	{
	iMMFDevSound = CMMFDevSound::NewL();

    iMMFDevSound->SetClientThreadInfo( CMMFController::ClientThreadIdL() );  
    // Construct custom command parsers
    CMMFAudioControllerCustomCommandParser* audConParser = CMMFAudioControllerCustomCommandParser::NewL(*this);
    CleanupStack::PushL(audConParser);
    AddCustomCommandParserL(*audConParser);
    CleanupStack::Pop(audConParser);

    CCustomInterfaceCustomCommandParser* customInterfaceParser = CCustomInterfaceCustomCommandParser::NewL(*this);
    CleanupStack::PushL(customInterfaceParser);
    AddCustomCommandParserL(*customInterfaceParser);
    CleanupStack::Pop(customInterfaceParser);
    }

// -----------------------------------------------------------------------------
//  CAdvancedAudioController::CreateSourceBufferOfSizeL
//  Creates a source buffer with the given size.  The buffer is then pushed and
//  left on the cleanupstack.
// -----------------------------------------------------------------------------
//
EXPORT_C CMMFDataBuffer* CAdvancedAudioController::CreateSourceBufferOfSizeLC(
	TUint aSize )
    {
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL(aSize);
    CleanupStack::PushL(buffer);
    buffer->Data().FillZ(aSize);
    buffer->SetRequestSizeL(aSize);
	buffer->Data().SetLength(0);
    return buffer;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::ResetSharedBuffersL
// Destroys the current buffers and re-populates it with new buffers.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::ResetSharedBuffersL(
	TInt aNumOfBuffer,
	TInt aBufferSize )
    {
    DP0(_L("CAdvancedAudioController::ResetSharedBuffersL"));
	iSharedBuffers.ResetAndDestroy();

	for ( TInt i = 0; i < aNumOfBuffer; i++ )
		{
		CMMFDataBuffer* buffer = CreateSourceBufferOfSizeLC(aBufferSize);
        DP3(_L("CAdvancedAudioController::ResetSharedBuffersL, index[%d] ptr[%x] size[%d]"), i, buffer->Data().Ptr(), aBufferSize);
		User::LeaveIfError( iSharedBuffers.Append(buffer) );
		CleanupStack::Pop(buffer);	// buffer
		}
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::Panic
// Raises an AdvancedAudioController panic.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::Panic(
	TInt aPanicCode )
    {
    _LIT(KAdvancedAudioControllerPanicCategory, "AdvancedAudioController");
    User::Panic(KAdvancedAudioControllerPanicCategory, aPanicCode);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::AddDataSourceL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::AddDataSourceL(
	MDataSource& /*aSource*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::AddDataSinkL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::AddDataSinkL(
	MDataSink& /*aSink*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::RemoveDataSourceL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::RemoveDataSourceL(
	MDataSource& /*aDataSource*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::RemoveDataSinkL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::RemoveDataSinkL(
	MDataSink& /*aDataSink*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::ResetL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::ResetL()
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::PrimeL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::PrimeL()
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::Play
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::PlayL()
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::Pause
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::PauseL()
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::StopL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::StopL()
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::PositionL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalMicroSeconds CAdvancedAudioController::PositionL() const
    {
    return TTimeIntervalMicroSeconds(0);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::SetPositionL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::SetPositionL(
	const TTimeIntervalMicroSeconds& /*aPosition*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::DurationL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalMicroSeconds CAdvancedAudioController::DurationL() const
    {
	DP1(_L("CAdvancedAudioController::DurationL %d"), iDuration);
    return TTimeIntervalMicroSeconds(iDuration);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::SetPrioritySettings
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::SetPrioritySettings(
	const TMMFPrioritySettings& aPrioritySettings )
    {
    iPrioritySettings = aPrioritySettings;
	iMMFDevSound->SetPrioritySettings(iPrioritySettings);    
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::GetNumberOfMetaDataEntriesL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::GetNumberOfMetaDataEntriesL(
	TInt& /*aNumberOfEntries*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::GetMetaDataEntryL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C CMMFMetaDataEntry* CAdvancedAudioController::GetMetaDataEntryL(
	TInt /*aIndex*/ )
    {
	User::Leave(KErrNotSupported);
    return NULL;	// Need this to suppress compiler warning
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetSourceSampleRateL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetSourceSampleRateL(
	TUint /*aSampleRate*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetSourceBitRateL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetSourceBitRateL(
	TUint /*aBitRate*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetSourceNumChannelsL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetSourceNumChannelsL(
	TUint /*aNumChannels*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetSourceFormatL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetSourceFormatL(
	TUid /*aFormatUid*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetSourceDataTypeL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetSourceDataTypeL(
	TFourCC /*aDataType*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetSinkSampleRateL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetSinkSampleRateL(
	TUint /*aSampleRate*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetSinkBitRateL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetSinkBitRateL(
	TUint /*aRate*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetSinkNumChannelsL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetSinkNumChannelsL(
	TUint /*aNumChannels*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetSinkFormatL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetSinkFormatL(
	TUid /*aFormatUid*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetSinkDataTypeL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetSinkDataTypeL(
	TFourCC /*aDataType*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacSetCodecL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacSetCodecL(
	TFourCC /*aSourceDataType*/,
	TFourCC /*aSinkDataType*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSourceSampleRateL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSourceSampleRateL(
	TUint& /*aRate*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSourceBitRateL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSourceBitRateL(
	TUint& /*aRate*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSourceNumChannelsL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSourceNumChannelsL(
	TUint& /*aNumChannels*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSourceFormatL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSourceFormatL(
	TUid& /*aFormat*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSourceDataTypeL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSourceDataTypeL(
	TFourCC& /*aDataType*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSinkSampleRateL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSinkSampleRateL(
	TUint& /*aRate*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSinkBitRateL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSinkBitRateL(
	TUint& /*aRate*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSinkNumChannelsL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSinkNumChannelsL(
	TUint& /*aNumChannels*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSinkFormatL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSinkFormatL(
	TUid& /*aFormat*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSinkDataTypeL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSinkDataTypeL(
	TFourCC& /*aDataType*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSupportedSourceSampleRatesL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSupportedSourceSampleRatesL(
	RArray<TUint>& /*aSupportedRates*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSupportedSourceBitRatesL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSupportedSourceBitRatesL(
	RArray<TUint>& /*aSupportedRates*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSupportedSourceNumChannelsL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSupportedSourceNumChannelsL(
	RArray<TUint>& /*aSupportedChannels*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSupportedSourceDataTypesL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSupportedSourceDataTypesL(
	RArray<TFourCC>& /*aSupportedDataTypes*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSupportedSinkSampleRatesL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSupportedSinkSampleRatesL(
	RArray<TUint>& /*aSupportedRates*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSupportedSinkBitRatesL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSupportedSinkBitRatesL(
	RArray<TUint>& /*aSupportedRates*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSupportedSinkNumChannelsL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSupportedSinkNumChannelsL(
	RArray<TUint>& /*aSupportedChannels*/ )
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioController::MacGetSupportedSinkDataTypesL
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioController::MacGetSupportedSinkDataTypesL(
	RArray<TFourCC>& /*aSupportedDataTypes*/ )
    {
    User::Leave(KErrNotSupported);
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioController::SendEventToClient
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioController::SendEventToClient(
	const TMMFEvent& aEvent )
    {
    DP1(_L("Error code at SendEventToClient [%d]"), aEvent.iErrorCode);
    DP1(_L("Event type at SendEventToClient [%d]"), aEvent.iEventType);
    TMMFEvent eventParamsChange;
    eventParamsChange.iErrorCode = aEvent.iErrorCode;
    eventParamsChange.iEventType = aEvent.iEventType;

// This Change has been made for a special Case Scenario.
// If the MMC card is taken out during an Incoming call
// and the ringtone is in MMC, CMMFFile sends up
// a Event type KMMFErrorCategoryControllerGeneralError,
// instead of KMMFEventCategoryPlaybackComplete
// which the client expects.
//TSW: EAKN-63D9FW

    if (aEvent.iEventType == KMMFErrorCategoryControllerGeneralError)
    {
    eventParamsChange.iEventType = KMMFEventCategoryPlaybackComplete ;
    }

    return DoSendEventToClient(eventParamsChange);

    }

// -----------------------------------------------------------------------------
// From MDataSource
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C TFourCC CAdvancedAudioController::SourceDataTypeCode(
	TMediaId /*aMediaId*/ )
	{
	return KFourCCNULL;
	}

EXPORT_C void CAdvancedAudioController::FillBufferL(
	CMMFBuffer* /*aBuffer*/,
	MDataSink* /*aConsumer*/,
	TMediaId /*aMediaId*/ )
	{}

EXPORT_C void CAdvancedAudioController::BufferEmptiedL(
	CMMFBuffer* /*aBuffer*/ )
	{}

EXPORT_C TBool CAdvancedAudioController::CanCreateSourceBuffer()
	{
	return EFalse;
	}

EXPORT_C CMMFBuffer* CAdvancedAudioController::CreateSourceBufferL(
	TMediaId /*aMediaId*/,
	TBool& /*aReference*/ )
	{
	return NULL;
	}

EXPORT_C void CAdvancedAudioController::ConstructSourceL(
	const TDesC8& /*aInitData*/ )
	{}

// -----------------------------------------------------------------------------
// From MDataSink
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C TFourCC CAdvancedAudioController::SinkDataTypeCode(
	TMediaId /*aMediaId*/ )
	{
	return KFourCCNULL;
	}

EXPORT_C void CAdvancedAudioController::EmptyBufferL(
	CMMFBuffer* /*aBuffer*/,
	MDataSource* /*aSupplier*/,
	TMediaId /*aMediaId*/ )
	{}

EXPORT_C void CAdvancedAudioController::BufferFilledL(
	CMMFBuffer* /*aBuffer*/ )
	{}

EXPORT_C TBool CAdvancedAudioController::CanCreateSinkBuffer()
	{
	return EFalse;
	}

EXPORT_C CMMFBuffer* CAdvancedAudioController::CreateSinkBufferL(
	TMediaId /*aMediaId*/,
	TBool& /*aReference*/ )
	{
	return NULL;
	}

EXPORT_C void CAdvancedAudioController::ConstructSinkL(
	const TDesC8& /*aInitData*/ )
	{}

// -----------------------------------------------------------------------------
// From MCustomInterfaceCustomCommandImplementor
// Provides the default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C const TMMFMessageDestination& CAdvancedAudioController::GetCustomInterfaceBuilderL( )
	{
	// If an effects manager already exist just return the handle to it
	// otherwise, create one.
	if ( !iCustomInterfaceBuilder )
		{
		iCustomInterfaceBuilder = CCustomInterfaceBuilder::NewL(MMFObjectContainerL(),
		                                                        *iMMFDevSound);
		// Append custom interface builder to the array of MMFObjects
		User::LeaveIfError(MMFObjectContainerL().AddMMFObject(*iCustomInterfaceBuilder));

        TBool secure = EFalse;
		secure = CMMFController::IsSecureDrmModeL(); 
		DP1(_L("CAdvancedAudioController::GetCustomInterfaceBuilderL : ->SetSecureCustomInterfaces(%d)"), secure);
		iCustomInterfaceBuilder->SetSecureCustomInterfaces( secure );
		}
	return iCustomInterfaceBuilder->Handle();
	}
// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of file
