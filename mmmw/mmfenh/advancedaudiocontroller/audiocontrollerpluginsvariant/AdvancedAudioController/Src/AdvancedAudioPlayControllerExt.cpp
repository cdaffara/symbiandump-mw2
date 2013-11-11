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
*                play controllers are derived. This class encapsulates common
*                behavior for all audio play controllers.
*
*/


// INCLUDE FILES
#include "AdvancedAudioPlayController.h"
#include "AdvancedAudioResource.h"
#include "DebugMacros.h"

#include <AudioOutput.h>
#include <MetaDataFieldContainer.h>
#include <MetaDataUtility.h>
#include <mmfformatimplementationuids.hrh>
#include <mmfmeta.h>
#include <MultimediaDataSourceEvents.h>
#include <MultimediaDataSourceFactory.h>
#include <oma2dcf.h>
#include <DRMConfigIntfc.h>
#include <AudioOutputControlUtility.h>

// CONSTANTS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacSetSinkSampleRateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacSetSinkSampleRateL(
	TUint aSampleRate)
    {
    if (!iDataSink || !iDataSource)
		{
        User::Leave(KErrNotReady);
		}

    if (iDataSink->DataSinkType() != KUidMmfFileSink)
        {
        User::Leave(KErrNotSupported);
        }

    switch (aSampleRate)
        {
/*
		case KAAPSampleRate8000Hz:
        case KAAPSampleRate11025Hz:
        case KAAPSampleRate12000Hz:
        case KAAPSampleRate16000Hz:
        case KAAPSampleRate22050Hz:
        case KAAPSampleRate24000Hz:
        case KAAPSampleRate32000Hz:
        case KAAPSampleRate44100Hz:
        case KAAPSampleRate48000Hz:
        case KAAPSampleRate64000Hz:
        case KAAPSampleRate88200Hz:
        case KAAPSampleRate96000Hz:
            if ((TInt)aSampleRate > iSampleRate)
                {
                // Only allow downsampling
                User::Leave(KErrNotSupported);
                }
            else
                {
                iSinkSampleRate = aSampleRate;
                }
			break;
//For future use...
*/
        case KAAPSampleRate8000Hz:
            if ((TUint)iSampleRate == KAAPSampleRate8000Hz ||
                 (TUint)iSampleRate == KAAPSampleRate16000Hz)
                {
                iSinkSampleRate = aSampleRate;
                }
            else
                {
                // Very limited due to Symbian conversion limitations...
                User::Leave(KErrNotSupported);
                }
            break;
        case KAAPSampleRate16000Hz:
            if ((TUint)iSampleRate == KAAPSampleRate16000Hz)
                {
                iSinkSampleRate = aSampleRate;
                }
            else
                {
                // Very limited due to Symbian conversion limitations...
                User::Leave(KErrNotSupported);
                }
            break;
        default:
            User::Leave(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSinkBitRateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacSetSinkBitRateL(
	TUint aRate)
    {
    if (!iDataSink || !iDataSource)
		{
        User::Leave(KErrNotReady);
		}

    if (iDataSink->DataSinkType() != KUidMmfFileSink ||
		(TInt)aRate != iBitRate)
        {
        User::Leave(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacSetSinkNumChannelsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacSetSinkNumChannelsL(
	TUint aNumChannels)
	{
    if (!iDataSink)
		{
        User::Leave(KErrNotReady);
		}

    if (iDataSink->DataSinkType() != KUidMmfFileSink)
        {
        User::Leave(KErrNotSupported);
        }

    if (aNumChannels <= 2)
        {
        iSinkNumChannels = aNumChannels;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacSetSinkFormatL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacSetSinkFormatL(
	TUid aFormatUid)
    {
    DP1(_L("CAdvancedAudioPlayController::MacSetSinkFormatL[%x]"), aFormatUid);

    if (aFormatUid != TUid::Uid(KMmfUidFormatRAWWrite))
        {
        User::Leave(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacSetSinkDataTypeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacSetSinkDataTypeL(
	TFourCC aDataType)
	{
    if (!iDataSink)
		{
        User::Leave(KErrNotReady);
		}

    if (iDataSink->DataSinkType() != KUidMmfFileSink ||
		 aDataType != KMMFFourCCCodePCM16)
        {
        User::Leave(KErrNotSupported);
        }
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSourceSampleRateL
// Returns the sampling rate obtrained from the source.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSourceSampleRateL(
	TUint& aRate)
    {
    if (!iDataSource)
		{
        User::Leave(KErrNotReady);
		}
    aRate = iSampleRate;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSourceBitRateL
// Returns the bit rate obtained from the source.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSourceBitRateL(
	TUint& aRate)
    {
    if (!iDataSource)
		{
        User::Leave(KErrNotReady);
		}
		
	// bit rate might be changed implicitly within a Controller's Utility
//    UpdateBitRate();
    aRate = iBitRate;
	DP1(_L("CAdvancedAudioPlayController::MacGetSourceBitRateL[%d]"), iBitRate);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSourceNumChannelsL
// Returns the number of channels from the source.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSourceNumChannelsL(
	TUint& aNumChannels)
    {
    if (!iDataSource)
		{
        User::Leave(KErrNotReady);
		}
    aNumChannels = iChannels;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSourceFormatL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSourceFormatL(
	TUid& aFormat)
    {
    aFormat = iSourceFormat;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSourceDataTypeL
// Returns the data type from the source.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSourceDataTypeL(
	TFourCC& aDataType)
    {
    if (!iDataSource)
    	{
        User::Leave(KErrNotReady);
		}
    aDataType = iDataType;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSinkSampleRateL
// Returns the sample rates configured in the audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSinkSampleRateL(
	TUint& aRate)
    {
    if (!iDataSink)
    	{
        User::Leave(KErrNotReady);
		}

    RArray<TUint> rates;
    CleanupClosePushL(rates);
    iAudioOutput->ConfigRatesL(rates);

    ASSERT(rates.Count() == 1);
    aRate = rates[0];
    CleanupStack::PopAndDestroy(&rates);	// rates
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSinkBitRateL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSinkBitRateL(
	TUint& aRate)
    {
    if (!iDataSink || !iDataSource)
		{
        User::Leave(KErrNotReady);
		}

    if (iDataSink->DataSinkType() != KUidMmfFileSink)
        {
        User::Leave(KErrNotSupported);
        }

    aRate = iBitRate;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSinkNumChannelsL
// Returns the number of channels configured in the audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSinkNumChannelsL(
	TUint& aNumChannels)
    {
    if (!iDataSink)
     	{
		User::Leave(KErrNotReady);
		}

    RArray<TUint> channels;
    CleanupClosePushL(channels);
	iAudioOutput->ConfigChannelsL(channels);

    ASSERT(channels.Count() == 1);
    aNumChannels = channels[0];
    CleanupStack::PopAndDestroy(&channels);	// channels
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSinkFormatL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSinkFormatL(
	TUid& aFormat)
    {
    aFormat = TUid::Uid(KMmfUidFormatRAWWrite);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSinkDataTypeL
// Returns the data types configured in the audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSinkDataTypeL(
	TFourCC& aDataType)
    {
    if (!iDataSink)
    	{
		User::Leave(KErrNotReady);
		}

    RArray<TFourCC> dataTypes;
    CleanupClosePushL(dataTypes);
    iAudioOutput->ConfigDataTypesL(dataTypes);

    ASSERT(dataTypes.Count() == 1);
    aDataType = dataTypes[0];
    CleanupStack::PopAndDestroy(&dataTypes);	// dataTypes
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSupportedSourceSampleRatesL
// Returns the supported sample rates.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSupportedSourceSampleRatesL(
	RArray<TUint>& aSupportedRates)
    {
    if (!iDataSource)
    	{
        User::Leave(KErrNotReady);
		}

    aSupportedRates.Reset();
    User::LeaveIfError(aSupportedRates.Append(iSampleRate));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSupportedSourceBitRatesL
// Returns the supported bit rate by the source.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSupportedSourceBitRatesL(
	RArray<TUint>& aSupportedRates)
    {
    if (!iDataSource)
    	{
        User::Leave(KErrNotReady);
		}

    aSupportedRates.Reset();
    User::LeaveIfError(aSupportedRates.Append(iBitRate));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSupportedSourceNumChannelsL
// Returns the supported number of channels by the source.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSupportedSourceNumChannelsL(
	RArray<TUint>& aSupportedChannels)
    {
    if (!iDataSource)
    	{
        User::Leave(KErrNotReady);
		}

    aSupportedChannels.Reset();
    User::LeaveIfError(aSupportedChannels.Append(iChannels));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSupportedSourceDataTypesL
// Returns the supported data types by the source.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSupportedSourceDataTypesL(
	RArray<TFourCC>& aSupportedDataTypes)
    {
    if (!iDataSource)
    	{
        User::Leave(KErrNotReady);
		}

    aSupportedDataTypes.Reset();
    User::LeaveIfError(aSupportedDataTypes.Append(iDataType));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSupportedSinkSampleRatesL
// Returns the supported sample rates by audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSupportedSinkSampleRatesL(
	RArray<TUint>& aSupportedRates)
    {
    if (!iDataSource)
    	{
        User::Leave(KErrNotReady);
		}

    aSupportedRates.Reset();
    iAudioOutput->CapabilitiesRatesL(aSupportedRates);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSupportedSinkBitRatesL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSupportedSinkBitRatesL(
	RArray<TUint>& aSupportedRates)
    {
    if (!iDataSink || !iDataSource)
		{
        User::Leave(KErrNotReady);
		}

    if (iDataSink->DataSinkType() != KUidMmfFileSink)
        {
        User::Leave(KErrNotSupported);
        }

    aSupportedRates.Reset();
    aSupportedRates.Append(iBitRate);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSupportedSinkNumChannelsL
// Returns the supported number of channels by audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSupportedSinkNumChannelsL(
	RArray<TUint>& aSupportedChannels)
    {
    if (!iDataSink)
    	{
        User::Leave(KErrNotReady);
		}

    aSupportedChannels.Reset();
    iAudioOutput->CapabilitiesChannelsL(aSupportedChannels);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MacGetSupportedSinkDataTypesL
// Returns the supported data types by audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MacGetSupportedSinkDataTypesL(
	RArray<TFourCC>& aSupportedDataTypes)
    {
    if (!iDataSink)
    	{
        User::Leave(KErrNotReady);
		}

    aSupportedDataTypes.Reset();
    iAudioOutput->CapabilitiesDataTypesL(aSupportedDataTypes);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MapcSetPlaybackWindowL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MapcSetPlaybackWindowL(
    const TTimeIntervalMicroSeconds& aStart,
    const TTimeIntervalMicroSeconds& aEnd)
    {
	DP0(_L("CAdvancedAudioPlayController::MapcSetPlaybackWindowL"));
    SetPlaybackWindowBoundariesL(aStart, aEnd);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MapcDeletePlaybackWindowL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MapcDeletePlaybackWindowL()
    {
    DP0(_L("CAdvancedAudioPlayController::MapcDeletePlaybackWindowL"));
    // iPlayWindowStartPosition = TTimeIntervalMicroSeconds(0);
    // iPlayWindowEndPosition = TTimeIntervalMicroSeconds(0);
    // ClearPlayWindow() can be called in either EPlaying or EPaused or EStopped state.
    // If ClearPlayWindow() is called before opening an audio file, then it doesn't have any effect.
    // This will call the Unregister event on the play window end position and sets the start and end position accordingly
    SetPlaybackWindowBoundariesL(0, 0);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MapcGetLoadingProgressL
// Default implementation.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MapcGetLoadingProgressL(
	TInt& /*aPercentageComplete*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MapdSetVolumeL
// Sets the volume in audion output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MapdSetVolumeL(
	TInt aVolume)
    {
    DP1(_L("CAdvancedAudioPlayController::MapdSetVolumeL[%d]"), aVolume);

    if (!iDataSink)
    	{
        User::Leave(KErrNotReady);
		}

    if (0 <= aVolume && aVolume <= iAudioOutput->MaxVolumeL())
        {
        iAudioOutput->SetVolumeL(aVolume);
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MapdGetMaxVolumeL
// Returns the max volume supported by the audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MapdGetMaxVolumeL(
	TInt& aMaxVolume)
    {
    if (!iDataSink)
    	{
        User::Leave(KErrNotReady);
		}

	aMaxVolume = iAudioOutput->MaxVolumeL();
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MapdGetVolumeL
// Returns the current volume set in audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MapdGetVolumeL(
	TInt& aVolume)
    {
    if (!iDataSink)
    	{
        User::Leave(KErrNotReady);
		}

    aVolume = iAudioOutput->VolumeL();
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MapdSetVolumeRampL
// Sets the volume ramp duration in audio output.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MapdSetVolumeRampL(
	const TTimeIntervalMicroSeconds& aRampDuration)
    {
    if (!iDataSink)
    	{
        User::Leave(KErrNotReady);
		}

	iAudioOutput->SetVolumeRampL(aRampDuration);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MapdSetBalanceL
// Sets the balance.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MapdSetBalanceL(
	TInt aBalance)
    {
    if(!iDataSink)
    	{
        User::Leave(KErrNotReady);
		}

    if(aBalance < KMMFBalanceMaxLeft || aBalance > KMMFBalanceMaxRight)
    	{
        User::Leave(KErrArgument);
		}

    TInt left  = 0;
    TInt right = 0;
    TInt rightBalance = 0;
    TInt leftBalance  = 0;
    
    CalculateLeftRightBalanceL(left, right, aBalance);
	iAudioOutput->SetPlayBalanceL(left, right);
    iAudioOutput->GetPlayBalanceL(leftBalance, rightBalance);

    TBool postCondition = ((rightBalance == right) && (leftBalance == left));
    
    if (!postCondition)
        User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MapdGetBalanceL
// Returns the currently set balance value.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::MapdGetBalanceL(
	TInt& aBalance)
    {
    if (!iDataSink)
    	{
        User::Leave(KErrNotReady);
		}

    TInt left = 0;
    TInt right = 0;

	iAudioOutput->GetPlayBalanceL(left, right);
    CalculateBalanceL(aBalance, left, right);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::GetBitRate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::GetBitRate(TUint& aBitRate)
    {
	DP0(_L("CAdvancedAudioPlayController::GetBitRate"));
    UpdateBitRate();    
    aBitRate = iBitRate;
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MscEnableEvents
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::MscEnableEvents(TBool aEnable)
    {
    iEventsEnabled = aEnable;
        
    // bit rate might be changed implicitly within a Controller's Utility, that would have effect on the duration event
    if (iAudioUtility)
        {
        TInt err = KErrNone;
            
        if (iEventsEnabled)
            {
            // callback on every 1000 millisecond (1 second) if bitrate changes
            TRAP(err, iAudioUtility->EnableBitRateChangedEventL(1000));
            }
        else
            {
            TRAP(err, iAudioUtility->EnableBitRateChangedEventL(-1));
            }

		DP1(_L("CAdvancedAudioPlayController::MscEnableEvents, EnableBitRateChangedEventL err[%d]"), err);
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MscGetSeekingSupported
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::MscGetSeekingSupported(TBool& /*aSupported*/)
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MscGetRandomSeekingSupported
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::MscGetRandomSeekingSupported(TBool& /*aSupported*/)
    {
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::CalculateLeftRightBalanceL
// Calculates percentage of the balance going into left and right channels.
// -----------------------------------------------------------------------------
//
void CAdvancedAudioPlayController::CalculateLeftRightBalanceL(
	TInt& aLeft,
	TInt& aRight,
	TInt aBalance)
    {
    if ((aBalance < KMMFBalanceMaxLeft) || (aBalance > KMMFBalanceMaxRight))
        User::Leave(KErrArgument);
    
    aLeft = (100 * (aBalance-KMMFBalanceMaxRight)) / (KMMFBalanceMaxLeft-KMMFBalanceMaxRight);
    aRight = 100 - aLeft;
    
    if ((aLeft > 100) || (aLeft < 0) || (aRight > 100) || (aRight < 0))
        User::Leave(KErrArgument);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::CalculateBalanceL
// Calculates the balance based on the current left and right values.
// -----------------------------------------------------------------------------
//
void CAdvancedAudioPlayController::CalculateBalanceL(
	TInt& aBalance,
	TInt aLeft,
	TInt aRight)
    {
    if ((aLeft == 0) && (aRight == 0))
        {
        aBalance = 0;
        }
    else
        {
        if ((aLeft + aRight) != 100)
            User::Leave(KErrArgument);
        
        if ((aLeft > 100) || (aLeft < 0) || (aRight > 100) || (aRight < 0))
            User::Leave(KErrArgument);
        
        aBalance = (aLeft * (KMMFBalanceMaxLeft-KMMFBalanceMaxRight))/100 + KMMFBalanceMaxRight;
        
        if ((aBalance < KMMFBalanceMaxLeft) || (aBalance > KMMFBalanceMaxRight))
            User::Leave(KErrArgument);
        }
    }

#ifdef SYMBIAN_CAF_V2
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MdcEvaluateIntent
// Returns the result of EvaluateIntent() when DataSourceType is KUidMmfFileSource.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::MdcEvaluateIntent(ContentAccess::TIntent aIntent)
	{
	if (iDataSourceAdapter == NULL)
	    {
	    return KErrNotReady;
	    }
	TInt status = iDataSourceAdapter->EvaluateIntent(aIntent);
	return status;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MdcExecuteIntent
// Returns the result of ExecuteIntent() when DataSourceType is KUidMmfFileSource.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::MdcExecuteIntent(ContentAccess::TIntent aIntent)
	{
    if (iDataSourceAdapter == NULL)
        {
        return KErrNotReady;
        }
	TInt status = iDataSourceAdapter->ExecuteIntent(aIntent);
	return status;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MdcDisableAutomaticIntent
// Returns KErrNone. Setting the iDisableAutoIntent based on the client request.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::MdcDisableAutomaticIntent(TBool aDisableAutoIntent)
	{
	iDisableAutoIntent = aDisableAutoIntent;
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MdcSetAgentProperty
// Returns the result of SetAgentProperty() when DataSourceType is KUidMmfFileSource.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::MdcSetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue)
	{
    if (iDataSourceAdapter == NULL)
        {
        return KErrNotReady;
        }
	TInt status = iDataSourceAdapter->SetAgentProperty(aProperty, aValue);
	return status;
	}

#endif

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::MapcSetRepeats
// Sets the number of times the audio sample is to be repeated during the playback operation.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::MapcSetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds& aTrailingSilence)
    {
    DP0(_L("CAdvancedAudioPlayController::MapcSetRepeats"));
    if (iState == EInitializing)
        {
        return KErrNotReady;
        }
    else
        {
        TInt result = DoSetRepeats(aRepeatNumberOfTimes, aTrailingSilence);
        return result;
        }
    }
// End of file
