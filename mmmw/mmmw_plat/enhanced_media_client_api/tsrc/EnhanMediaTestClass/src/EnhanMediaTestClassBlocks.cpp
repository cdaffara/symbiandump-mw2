/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  EMC - Test App
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <DRMConfigIntfc.h>
#include <DataBufferSource.h>

#include <CommDbConnPref.h>
#include <commdb.h>

#include "EnhanMediaTestClass.h"
#include "debug.h"

using namespace multimedia;
// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::Delete
// Delete here all resources allocated and opened from test methods.
// Called from destructor.
// -----------------------------------------------------------------------------
//
void CEnhanMediaTestClass::Delete()
	{
    FTRACE(FPrint(_L("CEnhanMediaTestClass::Delete")));


    iExpectedEvents.Reset();
    iExpectedEvents.Close();
    iFs.Close();
    //iOcurredEvents.Close();
	//iSupportedBitrates.Close();


    iLog->Log(_L("Deleting test class CEnhanMediaTestClass..."));
	iLog->Log(_L(""));
	iLog->Log(_L(""));

}

// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CEnhanMediaTestClass::RunMethodL(
    CStifItemParser& aItem )
    {

    static TStifFunctionInfo const KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
		ENTRY( "CreateFactory", CEnhanMediaTestClass::CreateFactory ),
		ENTRY( "DeleteFactory", CEnhanMediaTestClass::DeleteFactory ),
		ENTRY( "AddObserver", CEnhanMediaTestClass::AddObserver ),
		ENTRY( "RemoveObserver", CEnhanMediaTestClass::RemoveObserver ),
        ENTRY( "CreateSource", CEnhanMediaTestClass::CreateSourceControl ),
        ENTRY( "StreamRemoveSource", CEnhanMediaTestClass::StreamRemoveSource ),
        ENTRY( "CloseSource", CEnhanMediaTestClass::CloseSource ),
        ENTRY( "DeleteSource", CEnhanMediaTestClass::DeleteSource ),
        ENTRY( "StreamAddSource", CEnhanMediaTestClass::StreamAddSource ),
        ENTRY( "CreateSink", CEnhanMediaTestClass::CreateSink),
        ENTRY( "DeleteSink", CEnhanMediaTestClass::DeleteSink),
        ENTRY( "StreamAddSink", CEnhanMediaTestClass::StreamAddSink),
        ENTRY( "StreamRemoveSink", CEnhanMediaTestClass::StreamRemoveSink),
        ENTRY( "CreateEffect", CEnhanMediaTestClass::CreateEffect ),
        ENTRY( "DeleteEffect", CEnhanMediaTestClass::DeleteEffect ),
        ENTRY( "RemoveEffect", CEnhanMediaTestClass::RemoveEffect ),
        ENTRY( "StreamAddEffect", CEnhanMediaTestClass::StreamAddEffect ),
        ENTRY( "CreateStream", CEnhanMediaTestClass::CreateStream ),
        ENTRY( "DeleteStream", CEnhanMediaTestClass::DeleteStream ),
        ENTRY( "FileSOpen", CEnhanMediaTestClass::FileSOpen ),
        ENTRY( "FileGenOpen", CEnhanMediaTestClass::FileGenOpen ),
        ENTRY( "DescSOpen", CEnhanMediaTestClass::DescSOpen ),
        ENTRY( "DBSOpen", CEnhanMediaTestClass::DBSOpen ),
        ENTRY( "DBSClose", CEnhanMediaTestClass::DBSClose ),
        ENTRY( "StreamOpen", CEnhanMediaTestClass::StreamOpen ),
        ENTRY( "StreamClose", CEnhanMediaTestClass::StreamClose ),
        ENTRY( "StreamPrime", CEnhanMediaTestClass::StreamPrime ),
        ENTRY( "EqualizerBandLevel", CEnhanMediaTestClass::EqualizerBandLevel ),
        ENTRY( "EqualizerNumBands", CEnhanMediaTestClass::EqualizerNumBands ),
        ENTRY( "EqualizerBandWidth", CEnhanMediaTestClass::EqualizerBandWidth ),
        ENTRY( "EqualizerCenterFrequency", CEnhanMediaTestClass::EqualizerCenterFrequency ),
        ENTRY( "EqualizerCrossoverFrequency", CEnhanMediaTestClass::EqualizerCrossoverFrequency ),
        ENTRY( "EqualizerDbLevelLimits", CEnhanMediaTestClass::EqualizerDbLevelLimits ),
        ENTRY( "EqualizerSetBandLevel", CEnhanMediaTestClass::EqualizerSetBandLevel ),
        ENTRY( "StreamStart", CEnhanMediaTestClass::StreamStart ),
        ENTRY( "StreamPause", CEnhanMediaTestClass::StreamPause ),
        ENTRY( "StreamStop", CEnhanMediaTestClass::StreamStop ),
        ENTRY( "StreamCustomInterface", CEnhanMediaTestClass::StreamCustomInterface ),
        ENTRY( "DescSGetBitRate", CEnhanMediaTestClass::DescSGetBitRate ),
        ENTRY( "CreateDataBuffer", CEnhanMediaTestClass::CreateDataBuffer ),
        ENTRY( "DBSSetSize", CEnhanMediaTestClass::DBSSetSize ),
        ENTRY( "DBSGetSize", CEnhanMediaTestClass::DBSGetSize ),
        ENTRY( "DBSMinBufSize", CEnhanMediaTestClass::DBSMinBufSize ),
        ENTRY( "WriteLoop", CEnhanMediaTestClass::WriteLoop ),
        ENTRY( "GetBuffConfig", CEnhanMediaTestClass::GetBuffConfig ),
        ENTRY( "DBSSetBuffConfig", CEnhanMediaTestClass::DBSSetBuffConfig ),
        ENTRY( "DBSGetBuffType", CEnhanMediaTestClass::DBSGetBuffType ),
        ENTRY( "DBSGetInterface", CEnhanMediaTestClass::DBSGetInterface ),
        ENTRY( "DBSGetBitRate", CEnhanMediaTestClass::DBSGetBitRate ),
        ENTRY( "DeleteDataBuffer", CEnhanMediaTestClass::DeleteDataBuffer ),

        ENTRY( "VolGetMinVolume", CEnhanMediaTestClass::VolGetMinVolume ),
        ENTRY( "VolGetMaxVolume", CEnhanMediaTestClass::VolGetMaxVolume ),
        ENTRY( "VolSetVolume", CEnhanMediaTestClass::VolSetVolume ),
        ENTRY( "VolGetVolume", CEnhanMediaTestClass::VolGetVolume ),
        ENTRY( "VolGetDefaultVolume", CEnhanMediaTestClass::VolGetDefaultVolume ),
        ENTRY( "BalGetBalance", CEnhanMediaTestClass::BalGetBalance ),
        ENTRY( "BalSetBalance", CEnhanMediaTestClass::BalSetBalance ),
        ENTRY( "EffectApply", CEnhanMediaTestClass::EffectApply ),
        ENTRY( "VolSetVolumeRamp", CEnhanMediaTestClass::VolSetVolumeRamp ),
        ENTRY( "FileSGetBitRate", CEnhanMediaTestClass::FileSGetBitRate ),
        ENTRY( "FileSGetSize", CEnhanMediaTestClass::FileSGetSize ),
        ENTRY( "Source_GetMimeType", CEnhanMediaTestClass::Source_GetMimeType ),
        ENTRY( "Source_GetSize", CEnhanMediaTestClass::Source_GetSize ),
        ENTRY( "Stream_GetDuration", CEnhanMediaTestClass::Stream_GetDuration ),
        ENTRY( "Stream_SetPriority", CEnhanMediaTestClass::Stream_SetPriority ),
        ENTRY( "StreamGetSSL", CEnhanMediaTestClass::StreamGetSSL ),
        ENTRY( "StreamGetRSSL", CEnhanMediaTestClass::StreamGetRSSL ),
        ENTRY( "StreamGetStateL", CEnhanMediaTestClass::StreamGetStateL ),
        ENTRY( "StreamGetCIL", CEnhanMediaTestClass::StreamGetCIL ),
        ENTRY( "StreamGetPositionL", CEnhanMediaTestClass::StreamGetPositionL ),
        ENTRY( "StreamSetPositionL", CEnhanMediaTestClass::StreamSetPositionL ),
        ENTRY( "ControlTypeL", CEnhanMediaTestClass::ControlTypeL ),
        ENTRY( "ControlControlTypeL", CEnhanMediaTestClass::ControlControlTypeL ),

        ENTRY( "DRMSetType", CEnhanMediaTestClass::DRMSetType ),
        ENTRY( "DRMGetType", CEnhanMediaTestClass::DRMGetType ),
        ENTRY( "DRMCommit", CEnhanMediaTestClass::DRMCommit ),
        ENTRY( "DRMGetAllowedODCount", CEnhanMediaTestClass::DRMGetAllowedOutputDeviceCount ),
        ENTRY( "DRMGetAllowedOutputDevice", CEnhanMediaTestClass::DRMGetAllowedOutputDevice ),
        ENTRY( "DRMAppendAllowedOutputDevice", CEnhanMediaTestClass::DRMAppendAllowedOutputDevice ),
        ENTRY( "DRMRemoveAllowedOutputDevice", CEnhanMediaTestClass::DRMRemoveAllowedOutputDevice ),
        ENTRY( "DRMReset", CEnhanMediaTestClass::DRMReset ),

        ENTRY( "DSGetSize", CEnhanMediaTestClass::DSGetSize ),

        ENTRY( "AudioEffect_IsEnabled",CEnhanMediaTestClass::AudioEffectIsEnabled ),
        ENTRY( "AudioEffect_IsEnforced",CEnhanMediaTestClass::AudioEffectIsEnforced ),
        ENTRY( "AudioEffect_Uid",CEnhanMediaTestClass::AudioEffectUid ),
        ENTRY( "AudioEffect_HaveUpdateRights",CEnhanMediaTestClass::AudioEffectHaveUpdateRights ),
        ENTRY( "AudioEffect_Enforce",CEnhanMediaTestClass::AudioEffectEnforce ),

        ENTRY( "AudioEffect_Enable",CEnhanMediaTestClass::HandleAudioEffectEnableL ),
        ENTRY( "AudioEffect_Disable",CEnhanMediaTestClass::HandleAudioEffectDisableL ),

        ENTRY( "ER_DecayHFRatio",CEnhanMediaTestClass::HandleReverbDecayHFRatioL ),
        ENTRY( "ER_DecayHFRatioRange",CEnhanMediaTestClass::HandleReverbDecayHFRatioRangeL ),
        ENTRY( "ER_DecayTime",CEnhanMediaTestClass::HandleReverbDecayTimeL ),
        ENTRY( "ER_DecayTimeRange",CEnhanMediaTestClass::HandleReverbDecayTimeRangeL ),
        ENTRY( "ER_Density",CEnhanMediaTestClass::HandleReverbDensityL ),
        ENTRY( "ER_Diffusion",CEnhanMediaTestClass::HandleReverbDiffusionL ),
        ENTRY( "ER_ReflectionsDelay",CEnhanMediaTestClass::HandleReverbReflectionsDelayL ),
        ENTRY( "ER_ReflectionsDelayMax",CEnhanMediaTestClass::HandleReverbReflectionsDelayMaxL ),
        ENTRY( "ER_ReflectionsLevel",CEnhanMediaTestClass::HandleReverbReflectionsLevelL ),
        ENTRY( "ER_ReflectionLevelRange",CEnhanMediaTestClass::HandleReverbReflectionLevelRangeL ),
        ENTRY( "ER_ReverbDelay",CEnhanMediaTestClass::HandleReverbReverbDelayL ),
        ENTRY( "ER_ReverbDelayMax",CEnhanMediaTestClass::HandleReverbReverbDelayMaxL ),
        ENTRY( "ER_ReverbLevel",CEnhanMediaTestClass::HandleReverbReverbLevelL),
        ENTRY( "ER_ReverbLevelRange",CEnhanMediaTestClass::HandleReverbReverbLevelRangeL ),
        ENTRY( "ER_RoomHFLevel",CEnhanMediaTestClass::HandleReverbRoomHFLevelL ),
        ENTRY( "ER_RoomHFLevelRange",CEnhanMediaTestClass::HandleReverbRoomHFLevelRangeL ),
        ENTRY( "ER_RoomLevel",CEnhanMediaTestClass::HandleReverbRoomLevelL ),
        ENTRY( "ER_RoomLevelRange",CEnhanMediaTestClass::HandleReverbRoomLevelRangeL ),
        ENTRY( "ER_SetDecayHFRatio",CEnhanMediaTestClass::HandleReverbSetDecayHFRatioL ),
        ENTRY( "ER_SetDecayTime",CEnhanMediaTestClass::HandleReverbSetDecayTimeL ),
        ENTRY( "ER_SetDensity",CEnhanMediaTestClass::HandleReverbSetDensityL ),
        ENTRY( "ER_SetDiffusion",CEnhanMediaTestClass::HandleReverbSetDiffusionL ),
        ENTRY( "ER_SetReflectionsDelay",CEnhanMediaTestClass::HandleReverbSetReflectionsDelayL ),
        ENTRY( "ER_SetReflectionsLevel",CEnhanMediaTestClass::HandleReverbSetReflectionsLevelL ),
        ENTRY( "ER_SetReverbDelay",CEnhanMediaTestClass::HandleReverbSetReverbDelayL ),
        ENTRY( "ER_SetReverbLevel",CEnhanMediaTestClass::HandleReverbSetReverbLevelL ),
        ENTRY( "ER_SetRoomHFLevel",CEnhanMediaTestClass::HandleReverbSetRoomHFLevelL ),
        ENTRY( "ER_SetRoomLevel",CEnhanMediaTestClass::HandleReverbSetRoomLevelL ),
        ENTRY( "ER_DelayMax",CEnhanMediaTestClass::HandleReverbDelayMaxL ),

        ENTRY( "SW_IsContinuousLevelSupportedL",CEnhanMediaTestClass::HandleSWIsContinuousLevelSupportedL ),
        ENTRY( "SW_SetStereoWideningLevelL",CEnhanMediaTestClass::HandleSWSetStereoWideningLevelL ),
        ENTRY( "SW_StereoWideningLevelL",CEnhanMediaTestClass::HandleSWStereoWideningLevelL ),

        ENTRY( "RL_LevelRangeL",CEnhanMediaTestClass::HandleRLLevelRangeL ),
        ENTRY( "RL_SetRoomLevelL",CEnhanMediaTestClass::HandleRLSetRoomLevelL ),
        ENTRY( "RL_LevelL",CEnhanMediaTestClass::HandleRLLevelL ),

        ENTRY( "DA_DistanceAttenuationL",CEnhanMediaTestClass::HandleDADistanceAttenuationL ),
        ENTRY( "DA_RollOffFactorMaxL",CEnhanMediaTestClass::HandleDARollOffFactorMaxL ),
        ENTRY( "DA_RoomRollOffFactorMaxL",CEnhanMediaTestClass::HandleDARoomRollOffFactorMaxL ),


        ENTRY( "SL_LocationCartesianL",CEnhanMediaTestClass::HandleSLLocationCartesianL ),
        ENTRY( "SL_LocationSphericalL",CEnhanMediaTestClass::HandleSLLocationSphericalL ),
        ENTRY( "SL_SetLocationCartesianL",CEnhanMediaTestClass::HandleSLSetLocationCartesianL ),
        ENTRY( "SL_SetLocationSphericalL",CEnhanMediaTestClass::HandleSLSetLocationSphericalL ),
        ENTRY( "LO_OrientationL",CEnhanMediaTestClass::LO_OrientationL ),
        ENTRY( "LO_OrientationVectorsL",CEnhanMediaTestClass::LO_OrientationVectorsL ),


        ENTRY( "Control_Type",CEnhanMediaTestClass::HandleControlTypeL ),
        ENTRY( "Control_ControlType",CEnhanMediaTestClass::HandleControlControlTypeL ),

        ENTRY( "SD_SetFactorL",CEnhanMediaTestClass::HandleSDSetFactorL ),
        ENTRY( "SD_SetSphericalVelocityL",CEnhanMediaTestClass::HandleSDSetSphericalVelocityL ),
        ENTRY( "SD_SphericalVelocityL",CEnhanMediaTestClass::HandleSDSphericalVelocityL ),
        ENTRY( "SD_CartesianVelocityL",CEnhanMediaTestClass::HandleSDCartesianVelocityL ),
        ENTRY( "SD_FactorL",CEnhanMediaTestClass::HandleSDFactorL ),
        ENTRY( "SD_FactorMaxL",CEnhanMediaTestClass::HandleSDFactorMaxL ),
        ENTRY( "SD_SetCartesianVelocityL",CEnhanMediaTestClass::HandleSDSetCartesianVelocityL ),

		ENTRY( "AttachReverb",CEnhanMediaTestClass::AttachReverb ),
		ENTRY( "DetachReverb",CEnhanMediaTestClass::DetachReverb ),
	  //added
		ENTRY( "ER_DelayMaxMax",CEnhanMediaTestClass::HandleDelayMaxL ),

        };

    const TInt count = sizeof( KFunctions ) /
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::EventName
// Return descriptor with the notification description
// -----------------------------------------------------------------------------
TPtrC CEnhanMediaTestClass::EventName( TInt aKey )
{
	static TText* const badKeyword = (TText*)L"BadKeyword";
	static TText* const keywords[] =
		{
		(TText*)L"EEDBSOpen",
		(TText*)L"EEStreamPause",
		(TText*)L"EEDeleteBuffer",
		(TText*)L"EEWriteToStream",
		(TText*)L"EEDataBuffer",
		(TText*)L"EEStreamClose",
		(TText*)L"EEStreamAddEffect",
		(TText*)L"EEStreamOpen",
		(TText*)L"EEStreamPrime",
		(TText*)L"EEStreamStart",

	};
	if( (TUint)aKey >= (sizeof( keywords )/sizeof(TText*)) )
		{
		iLog->Log(_L("Keyword out of bounds"));
		TPtrC keyword( badKeyword );
		return keyword;
		}
	else
		{
		TPtrC keyword( keywords[aKey] );
		return keyword;
		}
}

// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::AddExpectedEvent
// Add an event to the expected events' list
// -----------------------------------------------------------------------------
void CEnhanMediaTestClass::AddExpectedEvent(TAEnhsExpectedEvent event, TInt ms)
{
	FTRACE(FPrint(_L("CEnhanMediaTestClass::AddExpectedEvent")));
	iExpectedEvents.Append(event);
	TPtrC eventName = EventName(event);
	iLog->Log(_L("Adding expected event:(0x%02x)%S Total=%d"), event, &eventName, iExpectedEvents.Count() );

	if ( iTimeoutController && !iTimeoutController->IsActive() )
		{
		if (ms > 0)
			{
			iTimeoutController->Start( TTimeIntervalMicroSeconds(ms * 1000) );
			}
		else
			{
			iLog->Log(_L("Timeout with default value (1s)"));
			iTimeoutController->Start( TTimeIntervalMicroSeconds(1000000) );
			}
		}
}

// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::RemoveExpectedEvent
// Remove the indicated event from the expected events' list
// -----------------------------------------------------------------------------
TBool CEnhanMediaTestClass::RemoveExpectedEvent(TAEnhsExpectedEvent aEvent)
{
	FTRACE(FPrint(_L("CEnhanMediaTestClass::RemoveExpectedEvent")));
	TBool match = EFalse;
	for (TUint i=0; i < iExpectedEvents.Count() ; i++)
		{
		if (iExpectedEvents[i] == aEvent)
			{
			iExpectedEvents.Remove(i);
			match = ETrue;
			break;
			}
		}

	return match;
}


// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::RemoveAllExpectedEvents
// Remove the indicated event from the expected events' list
// -----------------------------------------------------------------------------
void CEnhanMediaTestClass::RemoveAllExpectedEvents()
{
	FTRACE(FPrint(_L("CEnhanMediaTestClass::RemoveAllExpectedEvents")));
	iLog->Log(_L("Removing all expected events"));

	iExpectedEvents.Reset();
	//iOcurredEvents.Reset();
}


// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::ProcessEvent
// -----------------------------------------------------------------------------
void CEnhanMediaTestClass::ProcessEvent(TAEnhsExpectedEvent aEvent, TInt aError)
{
	FTRACE(FPrint(_L("CEnhanMediaTestClass::ProcessExpectedEvent")));
	TPtrC nameEvent = EventName(aEvent);

	// Check for error
	if (aError == KErrNone)
		{
		// Remove the event
		if (RemoveExpectedEvent(aEvent))
			{
			iLog->Log(_L("Expected Event: (0x%02x)%S has ocurred Total=%d"), aEvent, &nameEvent,iExpectedEvents.Count());
			}
		else
			{
			iLog->Log(_L("Event: (0x%02x)%S has ocurred"), aEvent, &nameEvent);
			return;
			}
		// All expected events have ocurred
		if (iExpectedEvents.Count() == 0 )
			{
			Signal();
			iTimeoutController->Cancel();
			}
		}
	else
		{
		iLog->Log(_L("[Error] Event: (0x%02x)%S return with error code=%d"), aEvent, &nameEvent, aError);
		if (iExpectedEvents.Count() != 0 )
			{
			RemoveExpectedEvent(aEvent);
			}
		iTimeoutController->Cancel();
		Signal(KErrCallbackErrorCode);
		}
}

// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::HandleTimeout
// Review if all the expected events have ocurred once the time is over
// -----------------------------------------------------------------------------

void CEnhanMediaTestClass::HandleTimeout(TInt aError)
{
	FTRACE(FPrint(_L("CEnhanMediaTestClass::HandleTimeout")));
	// All expected events have ocurred
	if (aError != KErrNone)
		{
		if (iExpectedEvents.Count() == 0 )
			{
			iLog->Log(_L("Timing out but events have ocurred"));
			Signal();
			}
		else
			{
			RemoveAllExpectedEvents();
			iLog->Log(_L("Timing out and events still pending"));
			Signal(KErrEventPending);
			}
		}
	else
		{
		iLog->Log(_L("Timing out return a error %d"), aError);
		Signal(aError);
		}
}

// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::SetTimeout
// Create a timer and set a timeout
// When the timeout is reached the test case is marked as failed
// It's used rather than the "timeout" keyword in the configuration file
// because in this way the log continues
// -----------------------------------------------------------------------------
TInt CEnhanMediaTestClass::SetTimeout( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CEnhanMediaTestClass::SetTimeout")));
	TInt timeout=0;
	TInt error = aItem.GetNextInt(timeout) ;
	if ( iTimeoutController )
		{
		if ( timeout > 0 )
			{
			iTimeoutController->Start( TTimeIntervalMicroSeconds(timeout*1000) );
			}
		else
			{
			iTimeoutController->Start( TTimeIntervalMicroSeconds(1000000) );
			}
		}
	else
		{
		iLog->Log(_L("Timeout Controller doesn't exist"));
		error = KErrTimeoutController;
		}
	return error;
}


// -----------------------------------------------------------------------------
// Uses the TestModuleBase API to allow a panic as exit reason for a test case
// -----------------------------------------------------------------------------
TInt CEnhanMediaTestClass::SetAllowedPanic( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CEnhanMediaTestClass::SetAllowedPanic")));
	TInt error = KErrNone;
	TInt panicCode;
	TPtrC panicType;
	if (  ( KErrNone == aItem.GetNextString(panicType) ) &&
	( KErrNone == aItem.GetNextInt(panicCode) )  )
		{
		iLog->Log(_L("Allowing panic: %S %d"), &panicType, panicCode);
		iTestModuleIf.SetExitReason( CTestModuleIf::EPanic, panicCode );
		iNormalExitReason = EFalse;
		}
	else
		{
		iLog->Log(KMsgBadTestParameters);
		error = KErrBadTestParameter;
		}
	return error;
}

// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::SetExpectedEvents()
// -----------------------------------------------------------------------------
TInt CEnhanMediaTestClass::SetExpectedEvents( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CEnhanMediaTestClass::SetExpectedEvents")));
	TInt error = KErrNone;
	TInt event=0;
	while ( KErrNone == aItem.GetNextInt(event))
		{
		AddExpectedEvent(static_cast<TAEnhsExpectedEvent>(event), 0); // Default timeout value
		}
	return error;
}


// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CEnhanMediaTestClass::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KEnhanMediaTestClass, "EnhanMediaTestClass" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KEnhanMediaTestClass, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KEnhanMediaTestClass,
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CEnhanMediaTestClass::CreateRestrictedAO
// Create RestrictedAO
// -----------------------------------------------------------------------------
//
TInt CEnhanMediaTestClass::CreateFactory( CStifItemParser& /*aItem*/ )
	{
	//AddExpectedEvent(EECreateFactory,KShortTimeout);
	TInt error = KErrNone;
	TInt err = CMultimediaFactory::CreateFactory(iFactory);
	if(error == KErrNone)
        {
        iLog->Log(_L("CreateFactory OK"));
        }
    else
        {
        iLog->Log(_L("CreateFactory [%d]"),error);
        }
	//ProcessEvent(EECreateFactory, error);
	return error;
	}

TInt CEnhanMediaTestClass::DeleteFactory( CStifItemParser& /*aItem*/ )
	{
	TInt error = KErrNone;
    if(iMimeType)
        {
        delete iMimeType;
        iMimeType = NULL;
        }

    if(iFileName)
        {
        delete iFileName;
        iFileName = NULL;
        }

    if(iDescData)
        {
        delete iDescData;
        iDescData = NULL;
        }

    if (iDRMConfigIntfc)
        {
        delete iDRMConfigIntfc;
        iDRMConfigIntfc = NULL;
        }

   // ProcessEvent(EECreateFactory, error);
/*	for (TUint i =0; i < iExpectedEvents.Count() ; i++)
		{
		ProcessEvent(TAEnhsExpectedEvent(i),0);
		}*/
	delete iFactory;
    iLog->Log(_L("Delete Factory OK"));
	return error;
	}

TInt CEnhanMediaTestClass::CreateSourceControl( CStifItemParser& aItem )
	{
	//AddExpectedEvent(EECreateSource,KShortTimeout);

	TInt object;
	TInt status = KErrNone;
	aItem.GetNextInt(object);
    switch(object)
    	{
        case EFILESOURCE:
            {
            status = iFactory->CreateSourceControl( KFileSourceControl, iSourceControl );
            if(status == KErrNone)
                {
                iMFileSource = static_cast<MFileSource*>(iSourceControl);
                }
            break;
            }
        case EDATABUFFERSOURCE:
            {
            status = iFactory->CreateSourceControl( KDataBufferSourceControl, iSourceControl );
            if(status == KErrNone)
                {
                iMDataBufferSource = static_cast<MDataBufferSource*>(iSourceControl);
                }
            break;
            }
        case EDESCRIPTORSOURCE:
            {
            status = iFactory->CreateSourceControl( KDescriptorSourceControl, iSourceControl );
            if(status == KErrNone)
                {
                iMDescriptorSource = static_cast<MDescriptorSource*>(iSourceControl);
                }
            break;
            }
        default:
        	{
        	status = KErrNotSupported;
        	break;
        	}
    	};
	if(status == KErrNone)
        {
       // iMFileSource = static_cast<MFileSource*>(iSourceControl);
        iLog->Log(_L("CreateSourceControl OK"));

        }
    else
        {
        iLog->Log(_L("CreateSourceControl [%d]"),status);
        }
	return status;
	}

TInt CEnhanMediaTestClass::DeleteSource(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("DeleteSourceControl"));
	TInt status = KErrNone;
    if(iMDataBufferSource)
        {
        MSourceControl* objPtr = iMDataBufferSource;
        iFactory->DeleteSourceControl(objPtr);
        iMDataBufferSource = NULL;
        }

    if(iMDescriptorSource)
        {
        MSourceControl* objPtr = iMDescriptorSource;
        iFactory->DeleteSourceControl(objPtr);
        iMDescriptorSource = NULL;
        }

    if(iMFileSource)
        {
        MSourceControl* objPtr = iMFileSource;
        iFactory->DeleteSourceControl(objPtr);
        iMFileSource = NULL;
        }
    iLog->Log(_L("DeleteSourceControl OK"));
    //ProcessEvent(EECreateSource, status);
	return status;
	}

TInt CEnhanMediaTestClass::CloseSource(CStifItemParser& /*aItem*/)
	{
	TInt status = KErrNone;
    status = iSourceControl->Close();
    if (status == KErrNone)
    	{
    	iLog->Log(_L("CloseSource OK"));
    	}
    else
    	{
    	iLog->Log(_L("CloseSource FAILED [%d]"),status);
    	}
    return status;
	}

TInt CEnhanMediaTestClass::CreateSink(CStifItemParser& /*aItem*/)
	{
	//AddExpectedEvent(EECreateSink,KShortTimeout);
	TInt status(KErrNone);
    if ( iMAudioSink )
        {
        status = KErrAlreadyExists;
        }
    else
        {
        status = iFactory->CreateSinkControl( KMMFAudioOutputSinkControl, iMAudioSink );
        }
    if (status == KErrNone)
     	{
     	iLog->Log(_L("CreateSinkL OK"));
     	}
     else
     	{
     	iLog->Log(_L("CreateSinkL FAILED [%d]"),status);
     	}

	//ProcessEvent(ECreateSink, status);
    return status;
    }

TInt CEnhanMediaTestClass::DeleteSink(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
    if ( iMAudioSink )
        {
        status = iFactory->DeleteSinkControl(iMAudioSink);
        iMAudioSink = NULL;
        }
    if (status == KErrNone)
     	{
     	iLog->Log(_L("DeleteSink OK"));
     	}
     else
     	{
     	iLog->Log(_L("DeleteSink FAILED [%d]"),status);
     	}

	//ProcessEvent(EECreateSink, status);

    return status;
	}
TInt CEnhanMediaTestClass::CreateStream(CStifItemParser& /*aItem*/)
	{
	//AddExpectedEvent(EECreateStream,KShortTimeout);
	TInt status(KErrNone);
    if ( iMStreamControl )
        {
        status = KErrAlreadyExists;
        }
    else
        {
        status = iFactory->CreateStreamControl( KStreamControl, iMStreamControl );
        }
	//ProcessEvent(ECreateStream, status);
    if (status == KErrNone)
     	{
     	iLog->Log(_L("CreateStream OK"));
     	}
     else
     	{
     	iLog->Log(_L("CreateStream FAILED [%d]"),status);
     	}
	//AddExpectedEvent(EECreateStream,KShortTimeout);

    return status;
	}

TInt CEnhanMediaTestClass::DeleteStream(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);

    if ( iMStreamControl )
        {
        status = iFactory->DeleteStreamControl(iMStreamControl);
        iMStreamControl = NULL;
        }
    if (status == KErrNone)
     	{
     	iLog->Log(_L("DeleteStream OK"));
     	}
     else
     	{
     	iLog->Log(_L("DeleteStream FAILED [%d]"),status);
     	}
	//ProcessEvent(EECreateStream, status);
    return status;
	}

TInt CEnhanMediaTestClass::RemoveEffect(CStifItemParser& aItem)
	{

	TInt object;
	TInt status(KErrNone);
	aItem.GetNextInt(object);
 	iLog->Log(_L("RemoveEffect INIT [%d]"),object);
    switch(object)
        {
        case EVOLUMECONTROL:
            {
            iEffectControl = iMVolumeControl;
            }
            break;
        case EBALANCECONTROL:
            {
            iEffectControl = iMBalanceControl;
            }
            break;
        case EEQUALIZERCONTROL:
            {
            iEffectControl = iMEqualizerControl;
            }
            break;
        case EREVERBCONTROL:
            {
            iEffectControl = iMReverbControl;
            }
            break;
        case EBASSBOOSTCONTROL:
            {
            iEffectControl = iMBassBoostControl;
            }
            break;
        case ESTEREOWIDCONTROL:
            {
            iEffectControl = iMStereoWideningControl;
            }
            break;


        case ELOUDNESSCONTROL:
            {
            iEffectControl = iMLoudnessControl;
            }
            break;


        case EROOMLEVELCONTROL:
            {
            iEffectControl = iMRoomLevelControl;
            }
            break;


        case EDISTATTCONTROL:
            {
            iEffectControl = iMDistanceAttenuationControl;
            }
            break;


        case ELISTDOPPCONTROL:
            {
            iEffectControl = iMListenerDopplerControl;
            }
            break;


        case ESOURDOPPCONTROL:
            {
            iEffectControl = iMSourceDopplerControl;
            }
            break;


        case ELISTLOCCONTROL:
            {
            iEffectControl = iMListenerLocationControl;
            }
            break;


        case ESOURLOCCONTROL:
            {
            iEffectControl = iMSourceLocationControl;
            }
            break;


        case ELISTORIENTCONTROL:
            {
            iEffectControl = iMListenerOrientationControl;
            }
            break;


        case ESOURORIENTCONTROL:
            {
            iEffectControl = iMSourceOrientationControl;
            }
            break;

        }
	status = iMStreamControl->RemoveEffect(*iEffectControl);
    if (status == KErrNone)
     	{
     	iLog->Log(_L("RemoveEffect OK"));
     	}
     else
     	{
     	iLog->Log(_L("RemoveEffect FAILED [%d]"),status);
     	}
	ProcessEvent(EEStreamAddEffect, status);
 	iLog->Log(_L("RemoveEffect END [%d]"),object);
	return status;
	}

TInt CEnhanMediaTestClass::DeleteEffect(CStifItemParser& aItem)
	{

	TInt object;
	TInt status(KErrNone);
    MEffectControl* tempCtrl;
	aItem.GetNextInt(object);
 	iLog->Log(_L("DeleteEffect INIT [%d]"),object);
    switch(object)
        {
        case EVOLUMECONTROL:
            {
            if ( iMVolumeControl )
                {
                tempCtrl = iMVolumeControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMVolumeControl = NULL;
                }
            break;
            }
        case EBALANCECONTROL:
            {
            if ( iMBalanceControl )
                {
                tempCtrl = iMBalanceControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMBalanceControl = NULL;
                }
            break;
            }
        case EEQUALIZERCONTROL:
            {
            if ( iMEqualizerControl )
                {
                tempCtrl = iMEqualizerControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMEqualizerControl = NULL;
                }
            break;
            }
        case EREVERBCONTROL:
            {
            if ( iMReverbControl )
                {
                tempCtrl = iMReverbControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMReverbControl = NULL;
                }
            break;
            }

        case EBASSBOOSTCONTROL:
            {
            if ( iMBassBoostControl )
                {
                tempCtrl = iMBassBoostControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMBassBoostControl = NULL;
                }
            break;
            }

        case ESTEREOWIDCONTROL:
            {
            if ( iMStereoWideningControl )
                {
                tempCtrl = iMStereoWideningControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMStereoWideningControl = NULL;
                }
            break;
            }
        case ELOUDNESSCONTROL:
            {
           if ( iMLoudnessControl )
                {
                tempCtrl = iMLoudnessControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMLoudnessControl = NULL;
                }
            break;
            }
        case EROOMLEVELCONTROL:
            {
            if ( iMRoomLevelControl )
                {
                tempCtrl = iMRoomLevelControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMRoomLevelControl = NULL;
                }
            break;
            }
        case EDISTATTCONTROL:
            {
            if ( iMDistanceAttenuationControl )
                {
                tempCtrl = iMDistanceAttenuationControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMDistanceAttenuationControl = NULL;
                }
            break;
            }
        case ELISTDOPPCONTROL:
            {
            if ( iMListenerDopplerControl )
                {
                tempCtrl = iMListenerDopplerControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMListenerDopplerControl = NULL;
                }
            break;
            }
        case ESOURDOPPCONTROL:
            {
            if ( iMSourceDopplerControl )
                {
                tempCtrl = iMSourceDopplerControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMSourceDopplerControl = NULL;
                }
            break;
            }
        case ELISTLOCCONTROL:
            {
            if ( iMListenerLocationControl )
                {
                tempCtrl = iMListenerLocationControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMListenerLocationControl = NULL;
                }
            break;
            }
        case ESOURLOCCONTROL:
            {
            if ( iMSourceLocationControl )
                {
                tempCtrl = iMSourceLocationControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMSourceLocationControl = NULL;
                }
            break;
            }
        case ELISTORIENTCONTROL:
            {
           if ( iMListenerOrientationControl )
                {
                tempCtrl = iMListenerOrientationControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMListenerOrientationControl = NULL;
                }
            break;
            }
        case ESOURORIENTCONTROL:
            {
            if ( iMSourceOrientationControl )
                {
                tempCtrl = iMSourceOrientationControl;
                status = iFactory->DeleteEffectControl(tempCtrl);
                iMSourceOrientationControl = NULL;
                }
            break;
            }
        }
    if (status == KErrNone)
     	{
     	iLog->Log(_L("DeleteEffectControl OK"));
     	}
     else
     	{
     	iLog->Log(_L("DeleteEffectControl FAILED [%d]"),status);
     	}
	iLog->Log(_L("DeleteEffect END[%d]"),object);

	return status;
	}

TInt CEnhanMediaTestClass::CreateEffect(CStifItemParser& aItem)
	{
	//AddExpectedEvent(EECreateEffect,KShortTimeout);
	TInt object;
	TInt status = KErrNone;
    MEffectControl* tempCtrl;
	aItem.GetNextInt(object);
	iLog->Log(_L("CreateEffect INIT[%d]"),object);

    switch(object)
        {
        case EVOLUMECONTROL:
            {
            status = iFactory->CreateEffectControl( KVolumeEffectControl, tempCtrl );
             if(status == KErrNone)
                 {
                 iMVolumeControl = static_cast<MVolumeControl*> (tempCtrl);
                 }
             break;
            }
        case EBALANCECONTROL:
            {
            status = iFactory->CreateEffectControl( KBalanceEffectControl, tempCtrl );
             if(status == KErrNone)
                 {
                 iMBalanceControl = static_cast<MBalanceControl*> (tempCtrl);
                 }
             break;
            }
        case EEQUALIZERCONTROL:
            {
           status = iFactory->CreateEffectControl( KEqualizerEffectControl, tempCtrl );
            if(status == KErrNone)
                {
                iMEqualizerControl = static_cast<MEqualizerControl*> (tempCtrl);
                }
            break;
            }
        case EREVERBCONTROL:
            {
            status = iFactory->CreateEffectControl( KReverbEffectControl, tempCtrl );
             if(status == KErrNone)
                 {
                 iMReverbControl = static_cast<MReverbControl*> (tempCtrl);
                 }
             break;
            }

        case EBASSBOOSTCONTROL:
            {
           status = iFactory->CreateEffectControl( KBassBoostEffectControl, tempCtrl );
            if(status == KErrNone)
                {
                iMBassBoostControl = static_cast<MBassBoostControl*> (tempCtrl);
                }
            break;
            }

        case ESTEREOWIDCONTROL:
            {
            status = iFactory->CreateEffectControl( KStereoWideningEffectControl, tempCtrl );
             if(status == KErrNone)
                 {
                 iMStereoWideningControl = static_cast<MStereoWideningControl*> (tempCtrl);
                 }
             break;
            }


        case ELOUDNESSCONTROL:
            {
            status = iFactory->CreateEffectControl( KLoudnessEffectControl, tempCtrl );
             if(status == KErrNone)
                 {
                 iMLoudnessControl = static_cast<MLoudnessControl*> (tempCtrl);
                 }
             break;
            }


        case EROOMLEVELCONTROL:
            {
            status = iFactory->CreateEffectControl( KRoomLevelEffectControl, tempCtrl );
             if(status == KErrNone)
                 {
                 iMRoomLevelControl = static_cast<MRoomLevelControl*> (tempCtrl);
                 }
             break;
            }


        case EDISTATTCONTROL:
            {
            status = iFactory->CreateEffectControl( KDistanceAttenuationEffectControl, tempCtrl );
             if(status == KErrNone)
                 {
                 iMDistanceAttenuationControl = static_cast<MDistanceAttenuationControl*> (tempCtrl);
                 }
             break;
            }

        case ELISTDOPPCONTROL:
            {
           status = iFactory->CreateEffectControl( KListenerDopplerEffectControl, tempCtrl );
            if(status == KErrNone)
                {
                iMListenerDopplerControl = static_cast<MListenerDopplerControl*> (tempCtrl);
                }
            break;
            }


        case ESOURDOPPCONTROL:
            {
           status = iFactory->CreateEffectControl( KSourceDopplerEffectControl, tempCtrl );
            if(status == KErrNone)
                {
                iMSourceDopplerControl = static_cast<MSourceDopplerControl*> (tempCtrl);
                }
            break;
            }


        case ELISTLOCCONTROL:
            {
            status = iFactory->CreateEffectControl( KListenerLocationEffectControl, tempCtrl );
             if(status == KErrNone)
                 {
                 iMListenerLocationControl = static_cast<MListenerLocationControl*> (tempCtrl);
                 }
             break;
            }

        case ESOURLOCCONTROL:
            {
            status = iFactory->CreateEffectControl( KSourceLocationEffectControl, tempCtrl );
             if(status == KErrNone)
                 {
                 iMSourceLocationControl = static_cast<MSourceLocationControl*> (tempCtrl);
                 }
             break;
            }


        case ELISTORIENTCONTROL:
            {
            status = iFactory->CreateEffectControl( KListenerOrientationEffectControl, tempCtrl );
             if(status == KErrNone)
                 {
                 iMListenerOrientationControl = static_cast<MListenerOrientationControl*> (tempCtrl);
                 }
             break;
            }


        case ESOURORIENTCONTROL:
            {
           status = iFactory->CreateEffectControl( KSourceOrientationEffectControl, tempCtrl );
            if(status == KErrNone)
                {
                iMSourceOrientationControl = static_cast<MSourceOrientationControl*> (tempCtrl);
                }
            break;
            }
        default:
        	break;
        };
    if (status == KErrNone)
     	{
     	iLog->Log(_L("CreateEffect OK"));
     	}
     else
     	{
     	iLog->Log(_L("CreateEffect FAILED [%d]"),status);
     	}

	//ProcessEvent(ECreateEffect, error);
	return status;
	}

TInt CEnhanMediaTestClass::StreamAddEffect(CStifItemParser& aItem)
	{
	AddExpectedEvent(EEStreamAddEffect,KShortTimeout);
	TInt status(KErrNotReady);
	TInt object;
	aItem.GetNextInt(object);
 	iLog->Log(_L("StreamAddEffect INIT [%d]"),object);

    switch(object)
        {
        case EVOLUMECONTROL:
            {
            iEffectControl = iMVolumeControl;
            }
            break;
        case EBALANCECONTROL:
            {
            iEffectControl = iMBalanceControl;
            }
            break;
        case EEQUALIZERCONTROL:
            {
            iEffectControl = iMEqualizerControl;
            }
            break;
        case EREVERBCONTROL:
            {
            iEffectControl = iMReverbControl;
            }
            break;

        case EBASSBOOSTCONTROL:
            {
            iEffectControl = iMBassBoostControl;
            }
            break;


        case ESTEREOWIDCONTROL:
            {
            iEffectControl = iMStereoWideningControl;
            }
            break;


        case ELOUDNESSCONTROL:
            {
            iEffectControl = iMLoudnessControl;
            }
            break;


        case EROOMLEVELCONTROL:
            {
            iEffectControl = iMRoomLevelControl;
            }
            break;


        case EDISTATTCONTROL:
            {
            iEffectControl = iMDistanceAttenuationControl;
            }
            break;


        case ELISTDOPPCONTROL:
            {
            iEffectControl = iMListenerDopplerControl;
            }
            break;


        case ESOURDOPPCONTROL:
            {
            iEffectControl = iMSourceDopplerControl;
            }
            break;


        case ELISTLOCCONTROL:
            {
            iEffectControl = iMListenerLocationControl;
            }
            break;


        case ESOURLOCCONTROL:
            {
           iEffectControl = iMSourceLocationControl;
            }
            break;


        case ELISTORIENTCONTROL:
            {
            iEffectControl = iMListenerOrientationControl;
            }
            break;


        case ESOURORIENTCONTROL:
            {
            iEffectControl = iMSourceOrientationControl;
            }
            break;

        }
    status = iMStreamControl->AddEffect(*iEffectControl);
    if (status == KErrNone)
     	{
     	iLog->Log(_L("AddEffect OK"));
     	}
     else
     	{
     	iLog->Log(_L("AddEffect FAILED [%d]"),status);
     	}
	iLog->Log(_L("StreamAddEffect END [%d]"),object);

	//ProcessEvent(EEStreamAddEffect, status);
    return status;
	}

TInt CEnhanMediaTestClass::AttachReverb (CStifItemParser& /*aItem*/)
{
	iLog->Log(_L("CEnhanMediaTestClass::AttachReverb BEGIN"));
	TInt status(KErrNotReady);

	if (iMReverbControl && iMRoomLevelControl)
	{
		status = iMRoomLevelControl->AttachReverb ( *iMReverbControl );
	}

    if (status == KErrNone)
     	{
     	iLog->Log(_L("CEnhanMediaTestClass::AttachReverb OK"));
     	}
     else
     	{
     	iLog->Log(_L("CEnhanMediaTestClass::AttachReverb FAILED [%d]"),status);
     	}

	iLog->Log(_L("CEnhanMediaTestClass::AttachReverb END"));
    return status;
}

TInt CEnhanMediaTestClass::DetachReverb(CStifItemParser& /*aItem*/)
{
	iLog->Log(_L("CEnhanMediaTestClass::DetachReverb BEGIN"));
	TInt status(KErrNotReady);

	if (iMReverbControl && iMRoomLevelControl)
	{
		status = iMRoomLevelControl->DetachReverb( *iMReverbControl );
	}

	if (status == KErrNone)
	{
		iLog->Log(_L("CEnhanMediaTestClass::DetachReverb OK"));
	}
    else
	{
		iLog->Log(_L("CEnhanMediaTestClass::DetachReverb FAILED [%d]"),status);
	}

 	iLog->Log(_L("CEnhanMediaTestClass::DetachReverb END "));
	return status;
}

TInt CEnhanMediaTestClass::AddObserver(CStifItemParser& aItem)
	{
	//AddExpectedEvent(EEObserver,KMediumTimeout);
	TInt object;
	TInt status = KErrNone;
	aItem.GetNextInt(object);
    iLog->Log(_L(" AddObserver = [%d]"),object);
    switch(object)
	    {
	    case EFILESOURCE:
	        {
	        if(iMFileSource)
	            {
                iMFileSource->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EDATABUFFERSOURCE:
	        {
	        if(iMDataBufferSource)
	            {
                iMDataBufferSource->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EDESCRIPTORSOURCE:
	        {
	        if(iMDescriptorSource)
	            {
                iMDescriptorSource->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case ESTREAMCONTROL:
	        {
	        if(iMStreamControl)
	            {
                iMStreamControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EMMFAUDIOOUTPUTSINKCONTROL:
	        {
	        if(iMAudioSink)
	            {
	            iMAudioSink->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EVOLUMECONTROL:
	        {
	        if(iMVolumeControl)
	            {
                iMVolumeControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EBALANCECONTROL:
	        {
	        if(iMBalanceControl)
	            {
                iMBalanceControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EEQUALIZERCONTROL:
	        {
	        if(iMEqualizerControl)
	            {
                iMEqualizerControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EREVERBCONTROL:
	        {
	        if(iMReverbControl)
	            {
                iMReverbControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EBASSBOOSTCONTROL:
	        {
	        if(iMBassBoostControl)
	            {
                iMBassBoostControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESTEREOWIDCONTROL:
	        {
	        if(iMStereoWideningControl)
	            {
                iMStereoWideningControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELOUDNESSCONTROL:
	        {
	        if(iMLoudnessControl)
	            {
                iMLoudnessControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EROOMLEVELCONTROL:
	        {
	        if(iMRoomLevelControl)
	            {
                iMRoomLevelControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EDISTATTCONTROL:
	        {
	        if(iMDistanceAttenuationControl)
	            {
                iMDistanceAttenuationControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELISTDOPPCONTROL:
	        {
	        if(iMListenerDopplerControl)
	            {
                iMListenerDopplerControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURDOPPCONTROL:
	        {
	        if(iMSourceDopplerControl)
	            {
                iMSourceDopplerControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case ELISTLOCCONTROL:
	        {
	        if(iMListenerLocationControl)
	            {
                iMListenerLocationControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURLOCCONTROL:
	        {
	        if(iMSourceLocationControl)
	            {
                iMSourceLocationControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELISTORIENTCONTROL:
	        {
	        if(iMListenerOrientationControl)
	            {
                iMListenerOrientationControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case ESOURORIENTCONTROL:
        	{
	        if(iMSourceOrientationControl)
	            {
                iMSourceOrientationControl->AddObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        default:
	   		break;
	    };

     	iLog->Log(_L("AddObserver OK [%d]"),status);
    return status;
	}

TInt CEnhanMediaTestClass::FileGenOpen(CStifItemParser& /*aItem*/)
    {
	//AddExpectedEvent(EEFileSOpen,KMediumTimeout);
    TInt status(KErrNotReady);
    TFileName fileName;
    fileName.Copy(KTagFileTest);
    if(iFileName)
        {
        delete iFileName;
        iFileName = NULL;
        }
    iFileName = HBufC::NewL(fileName.Length());
    iFileName->Des().Copy(fileName);


    iFs.Connect();
    iIsEOFReached = EFalse;
    status = iFile.Open(iFs,*iFileName,EFileShareReadersOnly);
    if (status == KErrNone)
     	{
        iLog->Log(_L("CEnhanMediaTestClass::FileGenOpen = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("CEnhanMediaTestClass::FileGenOpen ERROR = [%d]"), status);
     	}
	//ProcessEvent(EEFileSOpen, status);
    return status;
    }

TInt CEnhanMediaTestClass::DBSMinBufSize(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNotReady);
	TUint bufferSize =0;
	if(!iMDataBufferSource)
	    {
	    status = KErrNotReady;
	    }
	else
		{
		status = iMDataBufferSource->GetMinimumBufferSize(bufferSize);
		}
    if (status == KErrNone)
     	{
        iLog->Log(_L("iMDataBufferSource->GetMinimumBufferSize = [%d]"), bufferSize);
     	}
     else
     	{
        iLog->Log(_L("iMDataBufferSource->GetMinimumBufferSize ERROR = [%d]"), status);
     	}
	return status;
	}

TInt CEnhanMediaTestClass::CreateDataBuffer(CStifItemParser& aItem)
    {
	AddExpectedEvent(EEDataBuffer,KMediumTimeout);

	TInt status(KErrNone);
    TInt bufferSize = 0;
    TInt numBuffers = 0;

    status = aItem.GetNextInt(bufferSize);
    if ( status != KErrNone )
        {
        iLog->Log(_L("CreateDataBuffer Missing Argument Buffer Size in config file "));
        return status;
        }
    status = aItem.GetNextInt(numBuffers);

    if ( status != KErrNone )
        {
        iLog->Log(_L("CreateDataBuffer Missing Argument NumberOfBuffers in config file "));
        return status;
        }

    MDataBuffer* buffer;
    iBuffers.ResetAndDestroy();
    iAvailable.Reset();

    for (int i = 0; i < numBuffers; i++ )
        {
        status = iFactory->CreateDataBuffer(KDataBufferSourceControl,bufferSize,buffer);
        if(status == KErrNone)
            {
            status = iBuffers.Append(buffer);
            buffer = NULL;
            iAvailable.AppendL(ETrue);
            }
        else
            {
            break;
            }
        }
    return status;
    }

TInt CEnhanMediaTestClass::FileSOpen(CStifItemParser& /*aItem*/)
    {
	//AddExpectedEvent(EEFileSOpen,KMediumTimeout);
    TInt status(KErrNotReady);
    TFileName fileName;
    TBuf8<256> mimeType;

    fileName.Copy(KTagFileTest);
    mimeType.Copy(KTagMimeType);

	if(!iMFileSource)
	    {
	    return KErrNotReady;
	    }
    if(iFileName)
        {
        delete iFileName;
        iFileName = NULL;
        }
    iFileName = HBufC::NewL(fileName.Length());
    iFileName->Des().Copy(fileName);
    iMimeType = HBufC8::NewL(mimeType.Length());
    iMimeType->Des().Copy(mimeType);
    status = iMFileSource->Open(*iFileName,*iMimeType);
    if (status == KErrNone)
     	{
        iLog->Log(_L("CEnhanMediaTestClass::FileSOpen = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("CEnhanMediaTestClass::FileSOpen ERROR = [%d]"), status);
     	}
	//ProcessEvent(EEFileSOpen, status);
    return status;
    }

TInt CEnhanMediaTestClass::DescSOpen(CStifItemParser& /*aItem*/)
    {
	//AddExpectedEvent(EEFileSOpen,KMediumTimeout);
    TInt status(KErrNotReady);
    TBuf8<256> mimeType;
    mimeType.Copy(KTagMimeType);
    if(!iMDescriptorSource)
	    {
	    status = KErrNotReady;
	    return status;
	    }

    if(iMimeType)
        {
        delete iMimeType;
        iMimeType = NULL;
        }

    iMimeType = HBufC8::NewL(mimeType.Length());
    iMimeType->Des().Copy(mimeType);

    TInt size = 0;
    iFile.Size(size);
    if(iDescData)
        {
        delete iDescData;
        iDescData = NULL;
        }

    iDescData = HBufC8::NewLC(size);
    TPtr8 des = iDescData->Des();
    iFile.Read(des);

    status = iMDescriptorSource->Open(*iMimeType,*iDescData);
    CleanupStack::Pop(iDescData);

    if (status == KErrNone)
     	{
        iLog->Log(_L("CEnhanMediaTestClass::FileSOpenL = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("CEnhanMediaTestClass::FileSOpenL ERROR = [%d]"), status);
     	}
	//ProcessEvent(EEFileSOpen, status);
    return status;
    }

TInt CEnhanMediaTestClass::DBSSetSize(CStifItemParser& aItem)
	{
	TInt status(KErrNone);
	TInt size = 0;
	status = aItem.GetNextInt(size);
	if(!iMDataBufferSource)
		{
		status = KErrNotReady;
		}
	else
		{
		status = iMDataBufferSource->SetSize(size);
		}
    if (status == KErrNone)
     	{
        iLog->Log(_L("DBSSetSize = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("DBSSetSize ERROR = [%d]"), status);
     	}
	return status;
	}

TInt CEnhanMediaTestClass::DBSOpen(CStifItemParser& aItem)
    {
    iLog->Log(_L(" CEnhanMediaTestClass::DBSOpen("));
	AddExpectedEvent(EEDBSOpen,KMediumTimeout);
    TInt status(KErrNotReady);
    TBuf8<256> mimeType;
    TPtrC string;
    //mimeType.Copy(KTagMimeType);


    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    status = aItem.GetNextString ( string );
    if ( status != KErrNone )
        {
        iLog->Log(_L("CHelixSTIFClient:: MimeType missing in config file "));
        return status;
        }
    else
        {
        mimeType.Copy(string);
        }


	if(!iMDataBufferSource)
	    {
	    return status = KErrNotReady;
	    }


    if(iMimeType)
        {
        delete iMimeType;
        iMimeType = NULL;
        }

    iMimeType = HBufC8::NewL(mimeType.Length());
    iMimeType->Des().Copy(mimeType);

    MDataBuffer* headerData(NULL);
    status = iMDataBufferSource->Open(*iMimeType,*headerData);

    if (status == KErrNone)
     	{
        iLog->Log(_L("CEnhanMediaTestClass::FileSOpenL = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("CEnhanMediaTestClass::FileSOpenL ERROR = [%d]"), status);
     	}
	//ProcessEvent(EEFileSOpen, status);
    iLog->Log(_L(" CEnhanMediaTestClass::DBSOpen END"));
    return status;
    }
TInt CEnhanMediaTestClass::StreamRemoveSource(CStifItemParser& aItem)
	{
	//AddExpectedEvent(EEAddSource,KShortTimeout);
	TInt status(KErrNone);
	TInt controlType;
	aItem.GetNextInt(controlType);
    iLog->Log(_L(" CEnhanMediaTestClass::StreamRemoveSource INIT [%d]"),controlType);
    switch(controlType)
        {
        case EFILESOURCE:
            {
            iSourceControl = iMFileSource;
            }
            break;
        case EDATABUFFERSOURCE:
            {
            iSourceControl = iMDataBufferSource;
            }
            break;
        case EDESCRIPTORSOURCE:
            {
            iSourceControl = iMDescriptorSource;
            }
            break;
        }

	status = iMStreamControl->RemoveSource(*iSourceControl);
    if (status == KErrNone)
     	{
        iLog->Log(_L("RemoveSource = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("StreamRemoveSource ERROR = [%d]"), status);
     	}
	//ProcessEvent(EStreamAddSource, status);
    iLog->Log(_L(" CEnhanMediaTestClass::StreamRemoveSource END [%d]"),controlType);
	return status;
	}

TInt CEnhanMediaTestClass::StreamAddSource(CStifItemParser& aItem)
	{
	//AddExpectedEvent(EEAddSource,KShortTimeout);
	TInt status(KErrNone);
	TInt controlType;
	aItem.GetNextInt(controlType);
    iLog->Log(_L(" CEnhanMediaTestClass::StreamAddSource INIT [%d]"),controlType);
    switch(controlType)
        {
        case EFILESOURCE:
            {
            iSourceControl = iMFileSource;
            }
            break;
        case EDATABUFFERSOURCE:
            {
            iSourceControl = iMDataBufferSource;
            }
            break;
        case EDESCRIPTORSOURCE:
            {
            iSourceControl = iMDescriptorSource;
            }
            break;
        }
	status = iMStreamControl->AddSource(*iSourceControl);
    if (status == KErrNone)
     	{
        iLog->Log(_L("AddSource = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("AddSource ERROR = [%d]"), status);
     	}
	//ProcessEvent(EStreamAddSource, status);
    iLog->Log(_L(" CEnhanMediaTestClass::StreamAddSource END [%d]"),controlType);
	return status;
	}
TInt CEnhanMediaTestClass::StreamAddSink(CStifItemParser& /*aItem*/)
	{
	//AddExpectedEvent(EEStreamAddSink,KShortTimeout);
    iLog->Log(_L(" CEnhanMediaTestClass::StreamAddSink INIT"));
	TInt status(KErrNone);
	status = iMStreamControl->AddSink(*iMAudioSink);
    if (status == KErrNone)
     	{
        iLog->Log(_L("AddSink = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("AddSink ERROR = [%d]"), status);
     	}
	//ProcessEvent(EStreamAddSink, status);
    iLog->Log(_L(" CEnhanMediaTestClass::StreamAddSink END"));

	return status;
	}

TInt CEnhanMediaTestClass::StreamRemoveSink(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L(" CEnhanMediaTestClass::StreamRemoveSink INIT"));
	TInt status(KErrNone);
	status = iMStreamControl->RemoveSink(*iMAudioSink);
    if (status == KErrNone)
     	{
        iLog->Log(_L("RemoveSink = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("RemoveSink ERROR = [%d]"), status);
     	}
	//ProcessEvent(EStreamAddSink, status);
    iLog->Log(_L(" CEnhanMediaTestClass::StreamRemoveSink END"));

	return status;
	}

TInt CEnhanMediaTestClass::StreamOpen(CStifItemParser& /*aItem*/)
	{

	//RemoveAllExpectedEvents();
	TInt status(KErrNone);
	AddExpectedEvent(EEStreamOpen,KMediumTimeout);
    if ( !iMStreamControl )
        {
        status = KErrNotReady;
        iLog->Log(_L("CEnhanMediaTestClass::StreamOpenL = %d"), status);
        }

	status = iMStreamControl->Open();
    if (status == KErrNone)
     	{
        iLog->Log(_L("iMStreamControl->Open() = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("iMStreamControl->Open() ERROR = [%d]"), status);
     	}
    //ProcessEvent(EEStreamOpen, aEventType);
	return status;
	}

TInt CEnhanMediaTestClass::StreamClose(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNotReady);
	TInt state;
	state=iMStreamControl->GetState();
	AddExpectedEvent(EEStreamClose,KMediumTimeout);
	if ( !iMStreamControl )
		{
		status = KErrNotReady;
		}
	else
		{
	        iLog->Log(_L("iMStreamControl->GetState() = %d"), state);
			status = iMStreamControl->Close();
	        iLog->Log(_L("iMStreamControl->GetState() = %d"), iMStreamControl->GetState());

		}
    if (status == KErrNone)
     	{
        iLog->Log(_L("iMStreamControl->Close = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("iMStreamControl->Close ERROR = [%d]"), status);
     	}
	return status;
	}

TInt CEnhanMediaTestClass::RemoveObserver(CStifItemParser& aItem)
	{
	TInt status(KErrNone);
	TInt controlType;
	aItem.GetNextInt(controlType);
    iLog->Log(_L("RemoveObserver = [%d]"), controlType);
	switch(controlType)
	    {
	    case EFILESOURCE:
	        {
	        if(iMFileSource)
	            {
                iMFileSource->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EDATABUFFERSOURCE:
	        {
	        if(iMDataBufferSource)
	            {
	            iMDataBufferSource->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EDESCRIPTORSOURCE:
	        {
	        if(iMDescriptorSource)
	            {
                iMDescriptorSource->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case ESTREAMCONTROL:
	        {
	        if(iMStreamControl)
	            {
                iMStreamControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EMMFAUDIOOUTPUTSINKCONTROL:
	        {
	        if(iMAudioSink)
	            {
                iMAudioSink->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EVOLUMECONTROL:
	        {
	        if(iMVolumeControl)
	            {
                iMVolumeControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EBALANCECONTROL:
	        {
	        if(iMBalanceControl)
	            {
                iMBalanceControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EEQUALIZERCONTROL:
	        {
	        if(iMEqualizerControl)
	            {
                iMEqualizerControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EREVERBCONTROL:
	        {
	        if(iMReverbControl)
	            {
                iMReverbControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EBASSBOOSTCONTROL:
	        {
	        if(iMBassBoostControl)
	            {
                iMBassBoostControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESTEREOWIDCONTROL:
	        {
	        if(iMStereoWideningControl)
	            {
                iMStereoWideningControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELOUDNESSCONTROL:
	        {
	        if(iMLoudnessControl)
	            {
                iMLoudnessControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EROOMLEVELCONTROL:
	        {
	        if(iMRoomLevelControl)
	            {
                iMRoomLevelControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EDISTATTCONTROL:
	        {
	        if(iMDistanceAttenuationControl)
	            {
                iMDistanceAttenuationControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELISTDOPPCONTROL:
	        {
	        if(iMListenerDopplerControl)
	            {
                iMListenerDopplerControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURDOPPCONTROL:
	        {
	        if(iMSourceDopplerControl)
	            {
                iMSourceDopplerControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELISTLOCCONTROL:
	        {
	        if(iMListenerLocationControl)
	            {
                iMListenerLocationControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURLOCCONTROL:
	        {
	        if(iMSourceLocationControl)
	            {
                iMSourceLocationControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }


        case ELISTORIENTCONTROL:
	        {
	        if(iMListenerOrientationControl)
	            {
                iMListenerOrientationControl->RemoveObserver(*this);
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURORIENTCONTROL:
	        {
	        break;
	        }

	    };
    if (status == KErrNone)
     	{
        iLog->Log(_L("RemoveObserver = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("RemoveObserver ERROR = [%d]"), status);
     	}
	//ProcessEvent(EEObserver, status);
    return status;
	}

TInt CEnhanMediaTestClass::StreamPrime(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	AddExpectedEvent(EEStreamPrime,KMediumTimeout);
    if ( !iMStreamControl )
        {
            status = KErrNotReady;
            iLog->Log(_L("CEnhanMediaTestClass::StreamOpenL = %d"), status);
        }
    status = iMStreamControl->Prime();
    if (status == KErrNone)
     	{
        iLog->Log(_L("StreamPrime = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("StreamPrime ERROR = [%d]"), status);
     	}
    return status;
	}

TInt CEnhanMediaTestClass::StreamStart(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
    iLog->Log(_L("StreamStart "));
	AddExpectedEvent(EEStreamStart,KMediumTimeout);
    if ( !iMStreamControl )
        {
            status = KErrNotReady;
            iLog->Log(_L("StreamStart = %d"), status);
        }
    status = iMStreamControl->Start();
    if (status == KErrNone)
     	{
        iLog->Log(_L("StreamStart = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("StreamStart ERROR = [%d]"), status);
     	}
    iLog->Log(_L("StreamStart END"));
    return status;
	}

TInt CEnhanMediaTestClass::StreamPause(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
    iLog->Log(_L("StreamPause *-*-*-*-*-*"));
	AddExpectedEvent(EEStreamPause,KMediumTimeout);
    if ( !iMStreamControl )
        {
            status = KErrNotReady;
            iLog->Log(_L("StreamPause = %d"), status);
        }
    status = iMStreamControl->Pause();
    if (status == KErrNone)
     	{
        iLog->Log(_L("StreamPause = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("StreamPause ERROR = [%d]"), status);
     	}
    iLog->Log(_L("StreamPause END"));
    return status;
	}

TInt CEnhanMediaTestClass::StreamStop(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	//AddExpectedEvent(EEStreamPause,KMediumTimeout);
    iLog->Log(_L("StreamStop "));
    if ( !iMStreamControl )
        {
            status = KErrNotReady;
            iLog->Log(_L("StreamStart = %d"), status);
        }
    status = iMStreamControl->Stop();
    if (status == KErrNone)
     	{
        iLog->Log(_L("StreamStop = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("StreamStop ERROR = [%d]"), status);
     	}
    iLog->Log(_L("StreamStop END "));
    return status;
	}


TInt CEnhanMediaTestClass::EqualizerBandLevel(CStifItemParser& aItem)
    {
	TInt status(KErrNone);
	TInt band;
	TInt bandlevel;

	status = aItem.GetNextInt(band);
	if (status != KErrNone)
		{
		iLog->Log(_L("EqualizerBandLevel ERROR = [%d]"), status);
		return status;
		}
	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }
	status = static_cast<MEqualizerControl*>(iEffectControl)->BandLevel(band,bandlevel);
	   if (status == KErrNone)
	     	{
		iLog->Log(_L("EqualizerBandLevel status = %d"), status);
	     	}
	     else
	     	{
	        iLog->Log(_L("EqualizerBandLevel ERROR = [%d]"), status);
		}
	return status;
    }
TInt CEnhanMediaTestClass::EqualizerNumBands (CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	TInt bands;

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MEqualizerControl*>(iEffectControl)->NumberOfBands(bands);

	if (status == KErrNone)
		{
		iLog->Log(_L("EqualizerNumBands status = %d"), status);
		}
	else
		{
		iLog->Log(_L("EqualizerNumBands ERROR = [%d]"), status);
		}

	return status;
	}

TInt CEnhanMediaTestClass::EqualizerBandWidth (CStifItemParser& aItem)
	{
	TInt status(KErrNone);
	TInt band;
	TInt bandwidth;

	status = aItem.GetNextInt(band);
	if (status != KErrNone)
		{
		iLog->Log(_L("EqualizerBandWidth ERROR = [%d]"), status);
		return status;
		}

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MEqualizerControl*>(iEffectControl)->BandWidth(band,bandwidth);

	if (status == KErrNone)
			{
			iLog->Log(_L("EqualizerBandWidth status = %d"), status);
			}
		else
			{
			iLog->Log(_L("EqualizerBandWidth ERROR = [%d]"), status);
			}

		return status;
	}

TInt CEnhanMediaTestClass::EqualizerCenterFrequency (CStifItemParser& aItem)
	{
	TInt status(KErrNone);
	TInt band;
	TInt freq;

	status = aItem.GetNextInt(band);
	if (status != KErrNone)
		{
		iLog->Log(_L("EqualizerCenterFrequency ERROR = [%d]"), status);
		return status;
		}

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MEqualizerControl*>(iEffectControl)->CenterFrequency(band,freq);

	if (status == KErrNone)
			{
			iLog->Log(_L("EqualizerCenterFrequency status = %d"), status);
			}
		else
			{
			iLog->Log(_L("EqualizerCenterFrequency ERROR = [%d]"), status);
			}

	return status;
	}

TInt CEnhanMediaTestClass::EqualizerCrossoverFrequency (CStifItemParser& aItem)
	{
	TInt status(KErrNone);
	TInt band;
	TInt freq;

	status = aItem.GetNextInt(band);
	if (status != KErrNone)
		{
		iLog->Log(_L("EqualizerCrossoverFrequency ERROR = [%d]"), status);
		return status;
		}

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MEqualizerControl*>(iEffectControl)->CrossoverFrequency(band,freq);

	if (status == KErrNone)
		{
		iLog->Log(_L("EqualizerCrossoverFrequency status = %d"), status);
		}
	else
		{
		iLog->Log(_L("EqualizerCrossoverFrequency ERROR = [%d]"), status);
		}

	return status;
	}

TInt CEnhanMediaTestClass::EqualizerDbLevelLimits (CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	TInt min, max;

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MEqualizerControl*>(iEffectControl)->DbLevelLimits(min,max);

	if (status == KErrNone)
		{
		iLog->Log(_L("EqualizerDbLevelLimits status = %d"), status);
		}
	else
		{
		iLog->Log(_L("EqualizerDbLevelLimits ERROR = [%d]"), status);
		}

	return status;
	}

TInt CEnhanMediaTestClass::EqualizerSetBandLevel (CStifItemParser& aItem)
	{
	TInt status(KErrNone);
	TInt band, bandlevel;

	status = aItem.GetNextInt(band);
	if (status != KErrNone)
		{
		iLog->Log(_L("EqualizerSetBandLevel on band variable, ERROR = [%d]"), status);
		return status;
		}

	status = aItem.GetNextInt(bandlevel);
	if (status != KErrNone)
		{
		iLog->Log(_L("EqualizerSetBandLevel on bandlevel variable, ERROR = [%d]"), status);
		return status;
		}

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MEqualizerControl*>(iEffectControl)->SetBandLevel(band, bandlevel);

	if (status == KErrNone)
		{
		iLog->Log(_L("EqualizerSetBandLevel status = %d"), status);
		}
	else
		{
		iLog->Log(_L("EqualizerSetBandLevel ERROR = [%d]"), status);
	     	}

	return status;
    }

TInt CEnhanMediaTestClass::DescSGetBitRate(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	TUint bitrate = 0;

	if(!iMDescriptorSource)
	    {
	    return status = KErrNotReady;
	    }
    status = iMDescriptorSource->GetBitRate(bitrate);
	   if (status == KErrNone)
	     	{
	        iLog->Log(_L("iMDescriptorSource->GetBitRate() = [%d]"), bitrate);
	     	}
	     else
	     	{
	        iLog->Log(_L("iMDescriptorSource->GetBitRate ERROR = [%d]"), status);
	     	}
	   return status;
	}

void CEnhanMediaTestClass::WriteLoop(CStifItemParser& aItem)
	{
	AddExpectedEvent(EEWriteToStream,KMediumTimeout);
    iLog->Log(_L("WriteLoop INIT"));
	aItem.GetNextInt(iAutoWriteBuffer);
    TInt index = iAvailable.Find(ETrue);
    while ( (index != KErrNotFound ) && (!iIsEOFReached))
        {
        User::LeaveIfError( ReadFromFileAndWriteToStream( index ) );
        // Exit the loop if file EOF.
        if ( !iIsEOFReached )
            {
            // Get the next free buffer
            index = iAvailable.Find(ETrue);
            }
        }
    iLog->Log(_L("WriteLoop END"));
	}

TInt CEnhanMediaTestClass::ReadFromFileAndWriteToStream(TInt aIndex)
    {
    iLog->Log(_L("ReadFromFileAndWriteToStream INIT"));
    MDataBuffer* buffer = iBuffers[aIndex];
    // Read data into CAudioBuffer
    iFile.Read( buffer->GetBufferPtr() );
    iBytesReadFromFile += buffer->GetBufferPtr().Length();

    if ( buffer->GetBufferPtr().Length() < buffer->GetBufferPtr().MaxLength() )
        {
        // Last buffer is reached, set the flag
        buffer->SetLastBuffer( ETrue );
        iIsEOFReached = ETrue;
        }
    else
        {
        buffer->SetLastBuffer(EFalse);
        }

    // Write data into iMDataBufferSource
	#ifdef _DEBUG
    RDebug::Print(_L("CEnhanMediaTestClass::ReadFromFileAndWriteToStream\
    [AudioBuffer[0x%x]FileSize[%d]BytesRead[%d]]") , buffer, iFileSizeInBytes, iBytesReadFromFile );
	#endif
    TInt err(KErrNone);
    err = iMDataBufferSource->WriteData( *buffer );

    if (err == KErrNone)
        {
        iAvailable[aIndex] = EFalse;
        }
    else if ( err == KErrOverflow )
        {
        // There isn't enough memory in the player to buffer the data.
        // reset the file pos
        TInt size = -(buffer->GetBufferPtr().Length());
        iFile.Seek( ESeekCurrent, size );
        iBytesReadFromFile -= size;
        }
    iLog->Log(_L("ReadFromFileAndWriteToStream END"));

    return err;
    //	iAutoWriteBuffer
    }

TInt CEnhanMediaTestClass::GetBuffConfig(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("GetBuffConfig INIT"));
	TInt status(KErrNone);
	TInt buffer=0;
    TPckgBuf<TBufferingConfig> pckg;
    TIpcArgs args(&pckg);

	if(!iMDataBufferSource)
		{
		status = KErrNotReady;
		}
	else
		{
		status = iMDataBufferSource->GetBufferingConfig(pckg());
		}
	if (status == KErrNone)
		{
		iLog->Log(_L("iMDataBufferSource->GetBufferingConfig = [%d]"), buffer);
		}
	else
		{
		iLog->Log(_L("iMDataBufferSource->GetBufferingConfig ERROR = [%d]"), status);
		}
	return status;
	}

TInt CEnhanMediaTestClass::DBSSetBuffConfig(CStifItemParser& aItem)
	{
    iLog->Log(_L("DBSSetBuffConfig "));
    TInt status(KErrNone);
    TIpcArgs args;
    TInt type=0;
    TInt amount=0;
    TPckgBuf<TBufferingConfig> pckg;
    status = aItem.GetNextInt(type);

    if ( status != KErrNone )
         {
         iLog->Log(_L("DBSSetBufConfig Missing Argument Type in config file "));
         return status;
         }
     else
         {
         pckg().iType = TBufferingConfig::TBufferingType(type);
         }

     status = aItem.GetNextInt(amount);
     if ( status != KErrNone )
         {
         iLog->Log(_L("DBSSetBufConfigL Missing Argument Amount in config file "));
         return status;
         }
     else
         {
         pckg().iAmount = amount;
         }

     if(!iMDataBufferSource)
 	    {
 	    status = KErrNotReady;
 	    }
     else
    	 {
    	 status = iMDataBufferSource->SetBufferingConfig(pckg());
    	 }
     iLog->Log(_L("DBSSetBuffConfig END"));

     return status;
	}
TInt CEnhanMediaTestClass::DBSGetBuffType(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::DBSGetBuffType "));
	TInt status(KErrNone);
	RArray<TBufferingConfig::TBufferingType> aArray;
    TArray<TBufferingConfig::TBufferingType>  array = aArray.Array();

	if(!iMDataBufferSource)
	    {
	    return status = KErrNotReady;
	    }

    status = iMDataBufferSource->GetBufferingTypesSupported(array);
    aArray.Reset();
    aArray.Close();
    iLog->Log(_L("CEnhanMediaTestClass::DBSGetBuffType END [%d]"),status);
    return status;
	}

TInt CEnhanMediaTestClass::DBSGetInterface(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
    iLog->Log(_L("CEnhanMediaTestClass::DBSGetInterface "));
    // For descriptor source set DRM Type and allowed output
    if (iMDataBufferSource)
        {
        if (iDRMConfigIntfc)
            {
            delete iDRMConfigIntfc;
            iDRMConfigIntfc = NULL;
            }
        TAny* intfc(NULL);
        TVersion ver(KDRMConfigIntfcMajorVer1, KDRMConfigIntfcMinorVer1, KDRMConfigIntfcBuildVer1);
        TInt status = iMDataBufferSource->GetInterface(KDRMConfigIntfc,
                                                      ver,
                                                      intfc);
        if (status == KErrNone)
            {
            iDRMConfigIntfc = (CDRMConfigIntfc*)intfc;
            }
        }
	AddExpectedEvent(EEDBSGetInterface,KShortTimeout);
    iLog->Log(_L("CEnhanMediaTestClass::DBSGetInterface END"));
    return status;
	}

TInt CEnhanMediaTestClass::DBSGetBitRate(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::DBSGetBitRate "));
    TInt status(KErrNone);
	TUint bitrate = 0;
	if(!iMDataBufferSource)
	    {
	    status = KErrNotReady;
	    }
	else
		{
		status = iMDataBufferSource->GetBitRate(bitrate);
		}
    iLog->Log(_L("CEnhanMediaTestClass::DBSGetBitRate "));

    return status;
	}

TInt CEnhanMediaTestClass::DBSGetSize(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::DBSGetSize "));
    TInt status(KErrNone);
	TUint sizes = 0;
	if(!iMDataBufferSource)
	    {
	    status = KErrNotReady;
	    }
	else
		{
		status = iMDataBufferSource->GetSize(sizes);
		}
    iLog->Log(_L("CEnhanMediaTestClass::DBSGetSize [%d]"), status);

    return status;
	}

TInt CEnhanMediaTestClass::DBSClose(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::DBSClose "));
    TInt status(KErrNone);
	if(!iMDataBufferSource)
	    {
	    status = KErrNotReady;
	    }
	else
		{
		iMDataBufferSource->EmptyBuffers();
		status = iMDataBufferSource->Close();
		}
    iLog->Log(_L("CEnhanMediaTestClass::DBSClose [%d]"), status);

    return status;
	}


TInt CEnhanMediaTestClass::DSGetSize(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::DSGetSize "));
    TInt status(KErrNone);
	TUint sizes = 0;
	if(!iMDescriptorSource)
	    {
	    status = KErrNotReady;
	    }
	else
		{
		status = iMDescriptorSource->GetSize(sizes);
		}
    iLog->Log(_L("CEnhanMediaTestClass::DSGetSize [%d]"), status);

    return status;
	}
TInt CEnhanMediaTestClass::DeleteDataBuffer(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::DeleteDataBuffer "));
	AddExpectedEvent(EEDeleteBuffer,KMediumTimeout);
	TInt status(KErrNone);
	MDataBuffer* temp ;
    while(iBuffers.Count())
        {
        temp = iBuffers[0];
        iBuffers.Remove(0);
        status = iFactory->DeleteDataBuffer(temp);
        }
    iAvailable.Reset();
    iBuffers.ResetAndDestroy();
    iLog->Log(_L("CEnhanMediaTestClass::DeleteDataBuffer "));
    return status;
	}

TInt CEnhanMediaTestClass::VolGetMinVolume(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::VolGetMinVolume "));
	TInt status(KErrNone);

	TInt volume=0;
	if(!iMVolumeControl)
	    {
	    return status = KErrNotReady;
	    }
	status = iMVolumeControl->GetMinVolume(volume);
    iLog->Log(_L("CEnhanMediaTestClass::VolGetMinVolume [%d]"),volume);
    return status;
	}

TInt CEnhanMediaTestClass::VolGetMaxVolume(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::VolGetMaxVolume "));
	TInt status(KErrNone);
	TInt volume=0;
	if(!iMVolumeControl)
	    {
	    return status = KErrNotReady;
	    }
	status = iMVolumeControl->GetMaxVolume(volume);
    iLog->Log(_L("CEnhanMediaTestClass::VolGetMaxVolume [%d]"),volume);
    return status;
	}

TInt CEnhanMediaTestClass::VolGetVolume(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::VolGetVolume "));
	TInt status(KErrNone);
	TInt volume=0;
	if(!iMVolumeControl)
	    {
	    return status = KErrNotReady;
	    }
	status = iMVolumeControl->GetVolume(volume);
    iLog->Log(_L("CEnhanMediaTestClass::VolGetVolume [%d]"),volume);
    return status;
	}

TInt CEnhanMediaTestClass::VolGetDefaultVolume(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::VolGetDefaultVolume "));
	TInt status(KErrNone);
	TInt volume=0;
	if(!iMVolumeControl)
	    {
	    return status = KErrNotReady;
	    }
	status = iMVolumeControl->GetDefaultVolume(volume);
    iLog->Log(_L("CEnhanMediaTestClass::VolGetDefaultVolume [%d]"),volume);
    return status;
	}

TInt CEnhanMediaTestClass::VolSetVolume(CStifItemParser& aItem)
	{
	TInt volume;
	TInt status(KErrNone);
    iLog->Log(_L("VolSetVolume INIT"));
	aItem.GetNextInt(volume);
	status = iMVolumeControl->SetVolume(volume);
	if (status!=KErrNone)
		{
		iLog->Log(_L("VolSetVolume FAILED"));
		}
	else
		{
		iLog->Log(_L("VolSetVolume OK"));
		}
    return status;
	}

TInt CEnhanMediaTestClass::BalGetBalance(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::BalGetBalance "));
	TInt status(KErrNone);
	TInt status1(KErrNone);
	TInt status2(KErrNone);
	TInt balance=0;
	if(!iMBalanceControl)
	    {
	    return status = KErrNotReady;
	    }
	status1 = iMBalanceControl->GetBalance(balance);
	status2=iMBalanceControl->SetBalance(balance);
	status = status1 || status2;
    iLog->Log(_L("CEnhanMediaTestClass::BalGetBalance [%d]"),balance);
    return status;
	}

TInt CEnhanMediaTestClass::BalSetBalance(CStifItemParser& aItem)
	{
	TInt balance;
	TInt status(KErrNone);
    iLog->Log(_L("BalSetBalance INIT"));
	aItem.GetNextInt(balance);
	status = iMBalanceControl->SetBalance(balance);
	if (status!=KErrNone)
		{
		iLog->Log(_L("BalSetBalance FAILED"));
		}
	else
		{
		iLog->Log(_L("BalSetBalance OK"));
		}
    return status;
	}

TInt CEnhanMediaTestClass::EffectApply(CStifItemParser& aItem)
	{
	TInt control;
	TInt status(KErrNone);
    iLog->Log(_L("EffectApply INIT"));
	aItem.GetNextInt(control);

    switch(TControl(control))
        {
        case EVOLUMECONTROL:
            {
            iEffectControl = iMVolumeControl;
            }
            break;
        case EBALANCECONTROL:
            {
            iEffectControl = iMBalanceControl;
            }
            break;
        case EEQUALIZERCONTROL:
            {
            iEffectControl = iMEqualizerControl;
            }
            break;
        case EREVERBCONTROL:
            {
            iEffectControl = iMReverbControl;
            }
            break;
        case EBASSBOOSTCONTROL:
            {
            iEffectControl = iMBassBoostControl;
            }
            break;

        case ESTEREOWIDCONTROL:
            {
            iEffectControl = iMStereoWideningControl;
            }
            break;


        case ELOUDNESSCONTROL:
            {
            iEffectControl = iMLoudnessControl;
            }
            break;


        case EROOMLEVELCONTROL:
            {
            iEffectControl = iMRoomLevelControl;
            }
            break;


        case EDISTATTCONTROL:
            {
            iEffectControl = iMDistanceAttenuationControl;
            }
            break;


        case ELISTDOPPCONTROL:
            {
            iEffectControl = iMListenerDopplerControl;
            }
            break;


        case ESOURDOPPCONTROL:
            {
            iEffectControl = iMSourceDopplerControl;
            }
            break;


        case ELISTLOCCONTROL:
            {
            iEffectControl = iMListenerLocationControl;
            }
            break;


        case ESOURLOCCONTROL:
            {
            iEffectControl = iMSourceLocationControl;
            }
            break;


        case ELISTORIENTCONTROL:
            {
            iEffectControl = iMListenerOrientationControl;
            }
            break;


        case ESOURORIENTCONTROL:
            {
            iEffectControl = iMSourceOrientationControl;
            }
            break;
        }
	status = iEffectControl->Apply();
	if (status!=KErrNone)
		{
		iLog->Log(_L("EffectApply FAILED"));
		}
	else
		{
		iLog->Log(_L("EffectApply OK"));
		}
    return status;
	}

TInt CEnhanMediaTestClass::VolSetVolumeRamp(CStifItemParser& aItem)
	{
	TInt status(KErrNone);
	TInt initialvolume;
	TInt finalvolume;
	TInt rampmodevolume;
	TInt rampduration_par;
    iLog->Log(_L("VolSetVolume INIT"));

	if(!iMVolumeControl)
	    {
	    status = KErrNotReady;
	    }
	else
		{
		aItem.GetNextInt(initialvolume);
		aItem.GetNextInt(finalvolume);
		aItem.GetNextInt(rampduration_par);
		TPckgBuf<TUint64> pckg;
		pckg = rampduration_par;
		aItem.GetNextInt(rampmodevolume);
		TUint64 rampduration = pckg();
		MVolumeControl::TVolumeRampMode rampmode = MVolumeControl::TVolumeRampMode(rampmodevolume);
		status = iMVolumeControl->SetVolumeRamp(initialvolume,finalvolume,rampduration,rampmode);

		if (status!=KErrNone)
			{
			iLog->Log(_L("VolSetVolume FAILED"));
			}
		else
			{
			iLog->Log(_L("VolSetVolume OK"));
			}
		}
    return status;
	}


TInt CEnhanMediaTestClass::FileSGetBitRate(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::FileSGetBitRate "));
	TInt status(KErrNone);
	TUint bitrate = 0;
	if(!iMFileSource)
	    {
	    status = KErrNotReady;
	    }
	else
		{
	    status = iMFileSource->GetBitRate(bitrate);
		}
    iLog->Log(_L("CEnhanMediaTestClass::FileSGetBitRate [%d]"),bitrate);
    return status;
	}

TInt CEnhanMediaTestClass::FileSGetSize(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::FileSGetSize "));
	TInt status(KErrNone);
	TUint sizes = 0;
	if(!iMFileSource)
	    {
	    status = KErrNotReady;
	    }
	else
		{
	    status = iMFileSource->GetSize(sizes);
		}
    iLog->Log(_L("CEnhanMediaTestClass::FileSGetSize [%d]"),sizes);
    return status;
	}

TInt CEnhanMediaTestClass::Source_GetMimeType(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::FileSGetBitRate "));
	TInt status(KErrNone);
    TBuf8<256> mimeType;
    mimeType.Copy(KTagMimeType);

    TInt mimeTypeSize = mimeType.Size();

	if(iMimeType)
	    {
    	delete iMimeType;
    	iMimeType = NULL;
	    }

	iMimeType = HBufC8::NewL(mimeType.Size());
	TPtr8 des = iMimeType->Des();
	status = iSourceControl->GetMimeType(des);
    if (status == KErrNone)
     	{
        iLog->Log(_L("CEnhanMediaTestClass::Source_GetMimeType = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("CEnhanMediaTestClass::Source_GetMimeType ERROR = [%d]"), status);
     	}
    return status;
	}

TInt CEnhanMediaTestClass::Source_GetSize(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::Source_GetSize "));
	TInt status(KErrNone);
	TUint size = 0;
	status = iSourceControl->GetSize(size);
    iLog->Log(_L("CEnhanMediaTestClass::Source_GetSize [%d]"),size);
    return status;
	}

TInt CEnhanMediaTestClass::Stream_GetDuration(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::Stream_GetDuration "));
	TInt status(KErrNone);
	TInt64 dur=0;
	status = iMStreamControl->GetDuration(dur);
    iLog->Log(_L("CEnhanMediaTestClass::Stream_GetDuration [%d]"),dur);
    return status;
	}

TInt CEnhanMediaTestClass::Stream_SetPriority(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::Stream_SetPriority "));
	TInt status(KErrNone);
    if ( !iMStreamControl )
        {
        status = KErrNotReady;
        }
    else
    	{
    	TInt priority;
    	TInt mdaprioritypref;
        aItem.GetNextInt(priority);
        aItem.GetNextInt(mdaprioritypref);
        TMdaPriorityPreference pref = TMdaPriorityPreference(mdaprioritypref);
        status = iMStreamControl->SetPriority(priority, pref );
    	}
    if (status == KErrNone)
     	{
        iLog->Log(_L("CEnhanMediaTestClass::Stream_SetPriority = %d"), status);
     	}
     else
     	{
        iLog->Log(_L("CEnhanMediaTestClass::Stream_SetPriority ERROR = [%d]"), status);
     	}
    return status;
	}

TInt CEnhanMediaTestClass::StreamGetSSL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::StreamGetSSL "));
	TInt status(KErrNone);
	TBool seekingSupported(EFalse);
	status = iMStreamControl->GetSeekingSupport(seekingSupported);
    iLog->Log(_L("CEnhanMediaTestClass::StreamGetSSL [%d]"),status);
    status = KErrNone;
    return status;

	}
TInt CEnhanMediaTestClass::StreamGetRSSL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::StreamGetRSSL "));
	TInt status(KErrNone);
	TBool seekingSupported(EFalse);
	status = iMStreamControl->GetRandomSeekingSupport(seekingSupported);
    iLog->Log(_L("CEnhanMediaTestClass::StreamGetRSSL [%d]"),status);
    status = KErrNone;
    return status;
	}
TInt CEnhanMediaTestClass::StreamGetStateL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::StreamGetStateL "));
	TInt status(KErrNone);
	MStreamControl::TStreamState state = iMStreamControl->GetState();
    iLog->Log(_L("CEnhanMediaTestClass::StreamGetStateL [%d]"),status);
    return status;
	}
TInt CEnhanMediaTestClass::StreamGetCIL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::StreamGetCIL "));
	TInt status(KErrNone);

    iLog->Log(_L("CEnhanMediaTestClass::StreamGetCIL [%d]"),status);
    return status;
	}
TInt CEnhanMediaTestClass::StreamGetPositionL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::StreamGetStateL "));
	TInt status(KErrNone);
	TInt64 pos=0;
	status = iMStreamControl->GetPosition(pos);
    iLog->Log(_L("CEnhanMediaTestClass::StreamGetStateL [%d]"),status);
    return status;
	}
TInt CEnhanMediaTestClass::StreamSetPositionL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::StreamSetPositionL"));
	TInt status(KErrNone);
    if ( !iMStreamControl )
        {
        status = KErrNotReady;
        }
	TInt data;
	aItem.GetNextInt(data);
	TPckgBuf<TInt64> pckg;
	pckg = data;
	status = iMStreamControl->SetPosition(pckg());
    iLog->Log(_L("CEnhanMediaTestClass::StreamSetPositionL [%d]"),status);
    return status;
	}
TInt CEnhanMediaTestClass::ControlTypeL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::ControlTypeL INIT"));
	TInt status(KErrNone);
	TInt control;
	aItem.GetNextInt(control);
	TUid controlUid(TUid::Uid(0));

	switch(TControl(control))
	    {
	    case EFILESOURCE:
	        {
	        if(iMFileSource)
	            {
                controlUid = iMFileSource->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EDATABUFFERSOURCE:
	        {
	        if(iMDataBufferSource)
	            {
                controlUid = iMDataBufferSource->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EDESCRIPTORSOURCE:
	        {
	        if(iMDescriptorSource)
	            {
                controlUid = iMDescriptorSource->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case ESTREAMCONTROL:
	        {
	        if(iMStreamControl)
	            {
                controlUid = iMStreamControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EMMFAUDIOOUTPUTSINKCONTROL:
	        {
	        if(iMAudioSink)
	            {
                controlUid = iMAudioSink->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
	    case EVOLUMECONTROL:
	        {
	        if(iMVolumeControl)
	            {
                controlUid = iMVolumeControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
	    case EBALANCECONTROL:
	        {
	        if(iMBalanceControl)
	            {
                controlUid = iMBalanceControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
	    }
    iLog->Log(_L("CEnhanMediaTestClass::ControlTypeL  = [%d]"),control);
	return status;
	}
TInt CEnhanMediaTestClass::ControlControlTypeL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::ControlControlTypeL INIT"));
    TInt status(KErrNone);
	TInt control;
	aItem.GetNextInt(control);
	TControlType controlType(TControlType(0));
	switch(TControl(control))
	    {
	    case EFILESOURCE:
	        {
	        if(iMFileSource)
	            {
				controlType = iMFileSource->ControlType();
	            }
            else
              {
              status = KErrNotFound;
              }
	        break;
	        }
        case EDATABUFFERSOURCE:
	        {
	        if(iMDataBufferSource)
	            {
				controlType = iMDataBufferSource->ControlType();
	            }
            else
              {
              status = KErrNotFound;
              }
	        break;
	        }
        case EDESCRIPTORSOURCE:
	        {
	        if(iMDescriptorSource)
	            {
				controlType = iMDescriptorSource->ControlType();
	            }
            else
              {
              status = KErrNotFound;
              }
	        break;
	        }
        case ESTREAMCONTROL:
	        {
	        if(iMStreamControl)
	            {
				controlType = iMStreamControl->ControlType();
	            }
            else
              {
              status = KErrNotFound;
              }
	        break;
	        }
        case EMMFAUDIOOUTPUTSINKCONTROL:
	        {
	        if(iMAudioSink)
	            {
				controlType = iMAudioSink->ControlType();
	            }
            else
              {
              status = KErrNotFound;
              }
	        break;
	        }
	    case EVOLUMECONTROL:
	        {
	        if(iMVolumeControl)
	            {
	            controlType = iMVolumeControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
	    case EBALANCECONTROL:
	        {
	        if(iMBalanceControl)
	            {
	            controlType = iMBalanceControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
	    }
    iLog->Log(_L("CEnhanMediaTestClass::ControlControlTypeL = [%d]"),control);
    return status;
	}

TInt CEnhanMediaTestClass::DRMSetType(CStifItemParser& aItem)
	{
	TInt typedrm;
	TInt status(KErrNone);
    iLog->Log(_L("DRMSetType INIT"));
    if (!iDRMConfigIntfc)
    	{
    	iLog->Log(_L("iDRMConfigIntfc Not ready"));
    	status = KErrNotReady;
    	}
    else
    	{
    	status = aItem.GetNextInt(typedrm);
    	if (status != KErrNone)
    		{
    		iLog->Log(_L("Parameter was not set into config to set DRM type"));
    		status = KErrNotReady;
    		}
    	else
    		{
    		status = iDRMConfigIntfc->SetDRMType(TDRMType(typedrm));
    		if (status!=KErrNone)
    			{
    			iLog->Log(_L("DRMSetType FAILED"));
    			}
    		else
    			{
    			iLog->Log(_L("DRMSetType OK"));
    			}
    		}
    	}
    return status;
	}
TInt CEnhanMediaTestClass::DRMGetType(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	TDRMType typedrm;
    iLog->Log(_L("DRMGetType INIT"));
    if (!iDRMConfigIntfc)
    	{
    	iLog->Log(_L("iDRMConfigIntfc Not ready"));
    	status = KErrNotReady;
    	}
    else
    	{
    	status = iDRMConfigIntfc->GetDRMType(typedrm);
    	if (status!=KErrNone)
    		{
    		iLog->Log(_L("DRMGetType FAILED"));
    		}
    	else
    		{
    		iLog->Log(_L("DRMGetType OK [%d]"),typedrm);
    		}
    	}
    return status;
	}

TInt CEnhanMediaTestClass::DRMCommit(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
    iLog->Log(_L("DRMCommit INIT"));
    if (!iDRMConfigIntfc)
    	{
    	iLog->Log(_L("iDRMConfigIntfc Not ready"));
    	status = KErrNotReady;
    	}
    else
    	{
    	status = iDRMConfigIntfc->Commit();
    	if (status!=KErrNone)
    		{
    		iLog->Log(_L("DRMCommit FAILED"));
    		}
    	else
    		{
    		iLog->Log(_L("DRMCommit OK"));
    		}
    	}
    return status;
	}
TInt CEnhanMediaTestClass::DRMGetAllowedOutputDeviceCount( CStifItemParser& /*aItem */)
	{
	TInt status(KErrNone);
    iLog->Log(_L("DRMGetAllowedOutputDeviceCount INIT"));
    if (!iDRMConfigIntfc)
    	{
    	iLog->Log(_L("iDRMConfigIntfc Not ready"));
    	status = KErrNotReady;
    	}
    else
    	{
    	status = iDRMConfigIntfc->GetAllowedOutputDeviceCount(iAllowedDevice);
    	if (status!=KErrNone)
    		{
    		iLog->Log(_L("DRMGetAllowedOutputDeviceCount FAILED"));
    		}
    	else
    		{
    		iLog->Log(_L("DRMGetAllowedOutputDeviceCount = [%d]"),iAllowedDevice);
    		}
    	}
    return status;

	}
TInt CEnhanMediaTestClass::DRMGetAllowedOutputDevice(CStifItemParser& aItem )
	{
	TInt status(KErrNone);
	TDRMAllowedOutputDevice output;
	TInt index;
	iLog->Log(_L("DRMGetAllowedOutputDevice INIT"));
    if (!iDRMConfigIntfc)
    	{
    	iLog->Log(_L("iDRMConfigIntfc Not ready"));
    	status = KErrNotReady;
    	}
    else
    	{
    	aItem.GetNextInt(index);
    	status = iDRMConfigIntfc->GetAllowedOutputDevice(index, output);
    	if (status!=KErrNone)
    		{
    		iLog->Log(_L("DRMGetAllowedOutputDevice FAILED [%d]"),status);
    		}
    	else
    		{
    		iLog->Log(_L("DRMGetAllowedOutputDevice = [%d]"),output);
    		}
    	}
    return status;
	}

TInt CEnhanMediaTestClass::DRMAppendAllowedOutputDevice(CStifItemParser& aItem )
	{
	TInt status(KErrNone);
	TDRMAllowedOutputDevice output;
	TInt index;
	iLog->Log(_L("DRMAppendAllowedOutputDevice INIT"));
    if (!iDRMConfigIntfc)
    	{
    	iLog->Log(_L("iDRMConfigIntfc Not ready"));
    	status = KErrNotReady;
    	}
    else
    	{
    	aItem.GetNextInt(index);
    	output = TDRMAllowedOutputDevice(index);
    	status = iDRMConfigIntfc->AppendAllowedOutputDevice(output);
    	if (status!=KErrNone)
    		{
    		iLog->Log(_L("DRMAppendAllowedOutputDevice FAILED [%d]"),status);
    		}
    	else
    		{
    		iLog->Log(_L("DRMAppendAllowedOutputDevice = [%d]"),output);
    		}
    	}
    return status;
	}

TInt CEnhanMediaTestClass::DRMRemoveAllowedOutputDevice(CStifItemParser& aItem )
	{
	TInt status(KErrNone);
	TDRMAllowedOutputDevice output;
	TInt index;
	iLog->Log(_L("DRMRemoveAllowedOutputDevice INIT"));
    if (!iDRMConfigIntfc)
    	{
    	iLog->Log(_L("iDRMConfigIntfc Not ready"));
    	status = KErrNotReady;
    	}
    else
    	{
    	aItem.GetNextInt(index);
    	output = TDRMAllowedOutputDevice(index);
    	status = iDRMConfigIntfc->RemoveAllowedOutputDevice(output);
    	if (status != KErrNone)
    		{
    		iLog->Log(_L("DRMRemoveAllowedOutputDevice ERROR = [%d]"),status);
    		}
    	else
    		{
    		iLog->Log(_L("DRMRemoveAllowedOutputDevice = [%d]"),output);
    		}
    	}
    status = KErrNone; //remove after fixed
    return status;
	}
TInt CEnhanMediaTestClass::DRMReset()
	{
	TInt status(KErrNone);
	iLog->Log(_L("DRMReset INIT"));
    if (!iDRMConfigIntfc)
    	{
    	iLog->Log(_L("iDRMConfigIntfc Not ready"));
    	status = KErrNotReady;
    	}
    else
    	{
    	status = iDRMConfigIntfc->Reset();
    	if (status!=KErrNone)
    		{
    		iLog->Log(_L("DRMReset FAILED "));
    		}
    	else
    		{
    		iLog->Log(_L("DRMReset OK"));
    		}
    	}
    return status;
    }

TInt CEnhanMediaTestClass::CreateSeekableData(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	iLog->Log(_L("CreateSeekableData INIT"));
    if (!iMDataBufferSource)
    	{
    	status = KErrNotReady;
    	}
    else
    	{
    	//iDataSourceConfigIntfcImpl =  CDataSourceConfigIntfcImpl::NewL();
        if(status == KErrNone)
        	{
//        	iSourceControl = CCacheSource::NewL(*iSourceControl,iDataSourceConfigIntfcImpl);

        	}

    	}
    return status;
    }

TInt CEnhanMediaTestClass::StreamCustomInterface(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::StreamCustomInterface "));
    TInt status(KErrNone);
	if(!iMStreamControl)
	    {
	    status = KErrNotReady;
	    }
	else
		{
	    TUid controlUid(TUid::Uid(0x101F7D8D));
	    iMStreamControl->CustomInterface(controlUid);
	    controlUid = TUid::Uid(0x10207BA8);
	    iMStreamControl->CustomInterface(controlUid);
	    controlUid = TUid::Uid(0x10207BA9);
	    iMStreamControl->CustomInterface(controlUid);
		}
    iLog->Log(_L("CEnhanMediaTestClass::StreamCustomInterface"));

    return status;
	}
TInt CEnhanMediaTestClass::AudioEffectIsEnabled (CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	TBool enabled;

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MAudioEffectControl*>(iEffectControl)->IsEnabled(enabled);

	if (status == KErrNone)
		{
		iLog->Log(_L("AudioEffectIsEnabled status = %d"), status);
		}
	else
		{
		iLog->Log(_L("AudioEffectIsEnabled ERROR = [%d]"), status);
		}

	return status;
	}

///*TInt CEnhanMediaTestClass::AudioCreateEffect (CStifItemParser& /*aItem*/)
 /*  {
    TInt status(KErrNone);
    TBool enabled;

    if(!iEffectControl)
        {
        status = KErrNotReady;
        }
   // CEqualizerEffect *ptr=iEffectControl;
    //status = static_cast<MControl*>(iEffectControl);
   // status = ptr->CreateEffectProxy();
    if (status == KErrNone)
        {
        iLog->Log(_L("AudioEffectIsEnabled status = %d"), status);
        }
    else
        {
        iLog->Log(_L("AudioEffectIsEnabled ERROR = [%d]"), status);
        }

    return status;
    }
*/
TInt CEnhanMediaTestClass::AudioEffectIsEnforced (CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	TBool enforced;

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MAudioEffectControl*>(iEffectControl)->IsEnforced(enforced);

	if (status == KErrNone)
		{
		iLog->Log(_L("AudioEffectIsEnforced status = %d"), status);
		}
	else
		{
		iLog->Log(_L("AudioEffectIsEnforced ERROR = [%d]"), status);
		}

	return status;
	}

TInt CEnhanMediaTestClass::AudioEffectUid (CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	TUid effectUid;

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MAudioEffectControl*>(iEffectControl)->Uid(effectUid);

	if (status == KErrNone)
		{
		iLog->Log(_L("AudioEffectUid status = %d"), status);
		}
	else
		{
		iLog->Log(_L("AudioEffectUid ERROR = [%d]"), status);
		}

	return status;
	}

TInt CEnhanMediaTestClass::AudioEffectHaveUpdateRights (CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	TBool rights;

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MAudioEffectControl*>(iEffectControl)->HaveUpdateRights(rights);

	if (status == KErrNone)
		{
		iLog->Log(_L("AudioEffectHaveUpdateRights status = %d"), status);
		}
	else
		{
		iLog->Log(_L("AudioEffectHaveUpdateRights ERROR = [%d]"), status);
		}

	return status;
	}

TInt CEnhanMediaTestClass::AudioEffectEnforce (CStifItemParser& aItem)
	{
	TInt status(KErrNone);
	TBool enforce;

	status = aItem.GetNextInt(enforce);
	if (status != KErrNone)
		{
		iLog->Log(_L("AudioEffectEnforce ERROR = [%d]"), status);
		return status;
		}

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    }

	status = static_cast<MAudioEffectControl*>(iEffectControl)->Enforce(enforce);

	if (status == KErrNone)
		{
		iLog->Log(_L("AudioEffectEnforce status = %d"), status);
		}
	else
		{
		iLog->Log(_L("AudioEffectEnforce ERROR = [%d]"), status);
		}

	return status;
	}

TInt CEnhanMediaTestClass::HandleAudioEffectEnableL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleAudioEffectEnableL"));
	TInt status(KErrNone);
	TInt control;
	status = aItem.GetNextInt(control);
	if (status != KErrNone)
		{
		iLog->Log(_L("AudioEffectEnable ERROR = [%d]"), status);
		return status;
		}

    TControl controlType = TControl(control);

    switch(controlType)
        {
        case EVOLUMECONTROL:
            {
            iEffectControl = iMVolumeControl;
            }
            break;
        case EBALANCECONTROL:
            {
            iEffectControl = iMBalanceControl;
            }
            break;
        case EEQUALIZERCONTROL:
            {
            iEffectControl = iMEqualizerControl;
            }
            break;
        case EREVERBCONTROL:
            {
            iEffectControl = iMReverbControl;
            }
            break;
        case EBASSBOOSTCONTROL:
            {
            iEffectControl = iMBassBoostControl;
            }
            break;

        case ESTEREOWIDCONTROL:
            {
            iEffectControl = iMStereoWideningControl;
            }
            break;


        case ELOUDNESSCONTROL:
            {
            iEffectControl = iMLoudnessControl;
            }
            break;


        case EROOMLEVELCONTROL:
            {
            iEffectControl = iMRoomLevelControl;
            }
            break;


        case EDISTATTCONTROL:
            {
            iEffectControl = iMDistanceAttenuationControl;
            }
            break;


        case ELISTDOPPCONTROL:
            {
            iEffectControl = iMListenerDopplerControl;
            }
            break;


        case ESOURDOPPCONTROL:
            {
            iEffectControl = iMSourceDopplerControl;
            }
            break;


        case ELISTLOCCONTROL:
            {
            iEffectControl = iMListenerLocationControl;
            }
            break;


        case ESOURLOCCONTROL:
            {
            iEffectControl = iMSourceLocationControl;
            }
            break;


        case ELISTORIENTCONTROL:
            {
            iEffectControl = iMListenerOrientationControl;
            }
            break;


        case ESOURORIENTCONTROL:
            {
            iEffectControl = iMSourceOrientationControl;
            }
            break;

        }
	status = static_cast<MAudioEffectControl*>(iEffectControl)->Enable();
    iLog->Log(_L("CEnhanMediaTestClass::HandleAudioEffectEnableL END"));
	if (status == KErrNone)
		{
		iLog->Log(_L("AudioEffectEnable status = %d"), status);
		}
	else
		{
		iLog->Log(_L("AudioEffectEnable ERROR = [%d]"), status);
		}
	return status;
	}

TInt CEnhanMediaTestClass::HandleAudioEffectDisableL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleAudioEffectDisableL"));
	TInt status(KErrNone);
	TInt controltype = 0;
	status = aItem.GetNextInt(controltype);
	if (status != KErrNone)
		{
		iLog->Log(_L("AudioEffectDisable ERROR = [%d]"), status);
		return status;
		}

    switch(TControl(controltype))
        {
        case EVOLUMECONTROL:
            {
            iEffectControl = iMVolumeControl;
            }
            break;
        case EBALANCECONTROL:
            {
            iEffectControl = iMBalanceControl;
            }
            break;
        case EEQUALIZERCONTROL:
            {
            iEffectControl = iMEqualizerControl;
            }
            break;
        case EREVERBCONTROL:
            {
            iEffectControl = iMReverbControl;
            }
            break;
        case EBASSBOOSTCONTROL:
            {
            iEffectControl = iMBassBoostControl;
            }
            break;

        case ESTEREOWIDCONTROL:
            {
            iEffectControl = iMStereoWideningControl;
            }
            break;


        case ELOUDNESSCONTROL:
            {
            iEffectControl = iMLoudnessControl;
            }
            break;


        case EROOMLEVELCONTROL:
            {
            iEffectControl = iMRoomLevelControl;
            }
            break;


        case EDISTATTCONTROL:
            {
            iEffectControl = iMDistanceAttenuationControl;
            }
            break;


        case ELISTDOPPCONTROL:
            {
            iEffectControl = iMListenerDopplerControl;
            }
            break;


        case ESOURDOPPCONTROL:
            {
            iEffectControl = iMSourceDopplerControl;
            }
            break;


        case ELISTLOCCONTROL:
            {
            iEffectControl = iMListenerLocationControl;
            }
            break;


        case ESOURLOCCONTROL:
            {
            iEffectControl = iMSourceLocationControl;
            }
            break;


        case ELISTORIENTCONTROL:
            {
            iEffectControl = iMListenerOrientationControl;
            }
            break;


        case ESOURORIENTCONTROL:
            {
            iEffectControl = iMSourceOrientationControl;
            }
            break;

        }
	status = static_cast<MAudioEffectControl*>(iEffectControl)->Disable();
    if (status == KErrNone)
    	{
    	iLog->Log(_L("AudioEffectDisable status = %d"), status);
    	}
    else
    	{
    	iLog->Log(_L("AudioEffectDisable ERROR = [%d]"), status);
		}
    iLog->Log(_L("CEnhanMediaTestClass::HandleAudioEffectDisableL END"));
	return status;
	}


TInt CEnhanMediaTestClass::HandleReverbDecayHFRatioL(CStifItemParser& /*aItem*/)
	{
	TInt status(KErrNone);
	TUint ratio;
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayHFRatioL"));
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->DecayHFRatio(ratio);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayHFRatioL END"));

	return status;
    }

TInt CEnhanMediaTestClass::HandleReverbDecayHFRatioRangeL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayHFRatioRangeL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TUint min,max;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->DecayHFRatioRange(min,max);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayHFRatioRangeL min [%d] - max [%d]"),min,max);
	return status;
    }


TInt CEnhanMediaTestClass::HandleReverbDecayTimeL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayTimeL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TUint value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->DecayTime(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayTimeL [%d]"),value);
	return status;
    }

TInt CEnhanMediaTestClass::HandleReverbDecayTimeRangeL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayTimeL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TUint max, min;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->DecayTimeRange(min,max);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayTimeL min[%d] - max[%d]"),min,max);
	return status;
    }

TInt CEnhanMediaTestClass::HandleReverbDensityL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayTimeL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TUint value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->Density(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayTimeL [%d]"),value);
	return status;
    }

TInt CEnhanMediaTestClass::HandleReverbDiffusionL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayTimeL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TUint value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->Diffusion(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDecayTimeL value = [%d]"),value);
	return status;
    }

TInt CEnhanMediaTestClass::HandleReverbReflectionsDelayL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReflectionsDelayL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TUint value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->ReflectionsDelay(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReflectionsDelayL value = [%d]"),value);
	return status;
    }


TInt CEnhanMediaTestClass::HandleReverbReflectionsDelayMaxL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReflectionsDelayMaxL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TUint value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->ReflectionsDelayMax(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReflectionsDelayMaxL value = [%d]"),value);
	return status;
    }


TInt CEnhanMediaTestClass::HandleReverbReflectionsLevelL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReflectionsLevelL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->ReflectionsLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReflectionsLevelL value = [%d]"),value);
	return status;
    }

TInt CEnhanMediaTestClass::HandleReverbReflectionLevelRangeL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReflectionLevelRangeL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt min,max;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->ReflectionLevelRange(min,max);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReflectionLevelRangeL min = [%d]- max = [%d]"),min,max);
	return status;
    }

TInt CEnhanMediaTestClass::HandleReverbReverbDelayL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReverbDelayL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TUint value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->ReverbDelay(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReverbDelayL value = [%d]"),value);
	return status;
    }

TInt CEnhanMediaTestClass::HandleReverbReverbDelayMaxL(CStifItemParser& /*aItem*/)
	{

    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReverbDelayMaxL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TUint value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->ReverbDelayMax(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReverbDelayMaxL value = [%d]"),value);
	return status;
    }

TInt CEnhanMediaTestClass::HandleReverbReverbLevelL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReverbLevelL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->ReverbLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReverbLevelL value = [%d]"),value);
	return status;
    }

TInt CEnhanMediaTestClass::HandleReverbReverbLevelRangeL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReverbLevelRangeL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt min,max;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->ReverbLevelRange(min,max);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbReverbLevelRangeL min = [%d] - max = [%d]"),min,max);
	return status;
    }





TInt CEnhanMediaTestClass::HandleReverbRoomHFLevelL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbRoomHFLevelL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->RoomHFLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbRoomHFLevelL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbRoomHFLevelRangeL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbRoomHFLevelRangeL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt min,max;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->RoomHFLevelRange(min,max);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbRoomHFLevelRangeL min = [%d] - max = [%d]"),min,max);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbRoomLevelL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbRoomLevelL"));
	TInt status(KErrNone);
	TInt value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->RoomLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbRoomLevelL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbRoomLevelRangeL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbRoomLevelRangeL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt min,max;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->RoomLevelRange(min,max);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbRoomLevelRangeL min = [%d] - max = [%d]"),min,max);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbSetDecayHFRatioL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetDecayHFRatioL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->SetDecayHFRatio(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetDecayHFRatioL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbSetDecayTimeL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetDecayTimeL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->SetDecayTime(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetDecayTimeL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbSetDensityL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetDensityL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->SetDensity(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetDensityL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbSetDiffusionL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetDiffusionL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->SetDiffusion(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetDiffusionL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbSetReflectionsDelayL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetReflectionsDelayL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->SetReflectionsDelay(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetReflectionsDelayL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbSetReflectionsLevelL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetReflectionsLevelL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->SetReflectionsLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetReflectionsLevelL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbSetReverbDelayL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetReverbDelayL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->SetReverbDelay(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetReverbDelayL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbSetReverbLevelL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetReverbLevelL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->SetReverbLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetReverbLevelL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbSetRoomHFLevelL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetRoomHFLevelL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->SetRoomHFLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetRoomHFLevelL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbSetRoomLevelL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetRoomLevelL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->SetRoomLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbSetRoomLevelL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleReverbDelayMaxL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDelayMaxL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TUint value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MReverbControl*>(iEffectControl)->ReverbDelayMax(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDelayMaxL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleDelayMaxL(CStifItemParser& /*aItem*/)
    {
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDelayMaxL"));
    TInt status(KErrNone);
    //status = aItem.GetNextInt(controltype);
    TUint value;
    if(!iEffectControl)
        {
        return status = KErrNotReady;
        }
    status = static_cast<MReverbControl*>(iEffectControl)->DelayMax(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleDelayMaxL value = [%d]"),value);
    return status;
    }

TInt CEnhanMediaTestClass::HandleSWIsContinuousLevelSupportedL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDelayMaxL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MStereoWideningControl*>(iEffectControl)->IsContinuousLevelSupported(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleReverbDelayMaxL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleSWSetStereoWideningLevelL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleSWSetStereoWideningLevelL"));
	TInt status(KErrNone);
	TUint value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MStereoWideningControl*>(iEffectControl)->SetStereoWideningLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleSWSetStereoWideningLevelL value = [%d]"),value);
	return status;
	}
TInt CEnhanMediaTestClass::HandleSWStereoWideningLevelL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleSWStereoWideningLevelL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MStereoWideningControl*>(iEffectControl)->StereoWideningLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleSWStereoWideningLevelL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::LO_OrientationL (CStifItemParser& aItem)
	{
    TInt status(KErrNone);
	TInt status1(KErrNone);
	TInt status2(KErrNone);

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    return status;
	    }

    TInt a,b,c;


	TInt control = 0;
    status = aItem.GetNextInt(control);

   if (TControl(control) == ELISTORIENTCONTROL)
	   {
	   status1 = static_cast<MListenerOrientationControl*>(iEffectControl)->Orientation(a,b,c);
	   status2=static_cast<MListenerOrientationControl*>(iEffectControl)->SetOrientation(a,b,c);
	   }
   else if (TControl(control) == ESOURORIENTCONTROL)
	   {
	   status1 = static_cast<MSourceOrientationControl*>(iEffectControl)->Orientation(a,b,c);
	   status2= static_cast<MSourceOrientationControl*>(iEffectControl)->SetOrientation(a,b,c);
	   }
    status=status1 || status2;
    if (status == KErrNone)
    	{
    	iLog->Log(_L("LO_OrientationL status = %d"), status);
    	}
    else
    	{
    	iLog->Log(_L("LO_OrientationL ERROR = [%d]"), status);
    	}

    return status;
	}

TInt CEnhanMediaTestClass::LO_OrientationVectorsL (CStifItemParser& aItem)
	{
	TInt status(KErrNone);
	TInt status1(KErrNone);
	TInt status2(KErrNone);

	if(!iEffectControl)
	    {
	    status = KErrNotReady;
	    return status;
	    }

    TInt a,b,c,d,e,f;
	TInt control = 0;
    status = aItem.GetNextInt(control);
    if (TControl(control) == ELISTORIENTCONTROL)
 	   {
 	    status1 = static_cast<MListenerOrientationControl*>(iEffectControl)->OrientationVectors(a,b,c,d,e,f);
 	    status2=  static_cast<MListenerOrientationControl*>(iEffectControl)->SetOrientationVectors(a,b,c,d,e,f);
 	   }
    else if (TControl(control) == ESOURORIENTCONTROL)
 	   {
	    status1 = static_cast<MSourceOrientationControl*>(iEffectControl)->OrientationVectors(a,b,c,d,e,f);
	    status2=  static_cast<MSourceOrientationControl*>(iEffectControl)->SetOrientationVectors(a,b,c,d,e,f);
 	   }
    status = status1 || status2;

    if (status == KErrNone)
    	{
    	iLog->Log(_L("LO_OrientationVectorsL status = %d"), status);
    	}
    else
    	{
    	iLog->Log(_L("LO_OrientationVectorsL ERROR = [%d]"), status);
    	}

    return status;
	}



TInt CEnhanMediaTestClass::HandleRLLevelRangeL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleRLLevelRangeL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt value1,value2;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MRoomLevelControl*>(iEffectControl)->LevelRange(value1,value2);
    iLog->Log(_L("CEnhanMediaTestClass::HandleRLLevelRangeL value = [%d] - [%d]"),value1,value2);
	return status;
	}

TInt CEnhanMediaTestClass::HandleRLSetRoomLevelL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleRLSetRoomLevelL"));
	TInt status(KErrNone);
	TInt value;
	status = aItem.GetNextInt(value);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MRoomLevelControl*>(iEffectControl)->SetRoomLevel(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleRLSetRoomLevelL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleRLLevelL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleRLLevelL"));
	TInt status(KErrNone);
	//status = aItem.GetNextInt(controltype);
	TInt value;
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	status = static_cast<MRoomLevelControl*>(iEffectControl)->Level(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleRLLevelL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleDADistanceAttenuationL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleDADistanceAttenuationL"));
	TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
    TInt a,b,c;
    TUint d,e;
    status = static_cast<MDistanceAttenuationControl*>(iEffectControl)->DistanceAttenuation(a,b,c,d,e);
    iLog->Log(_L("CEnhanMediaTestClass::HandleDADistanceAttenuationL a = [%d] - b = [%d] - c = [%d] - d = [%d]- e = [%d]"),a,b,c,d,e);
    //added
    status=static_cast<MDistanceAttenuationControl*>(iEffectControl)->SetDistanceAttenuation(a,b,c,d,e);
	return status;
	}
TInt CEnhanMediaTestClass::HandleDARollOffFactorMaxL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleDARollOffFactorMaxL"));
	TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
    TUint value;
    status = static_cast<MDistanceAttenuationControl*>(iEffectControl)->RollOffFactorMax(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleDARollOffFactorMaxL value = [%d]"),value);
	return status;
	}
TInt CEnhanMediaTestClass::HandleDARoomRollOffFactorMaxL(CStifItemParser& /*aItem*/)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleDARoomRollOffFactorMaxL"));
	TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
    TUint value;
    status = static_cast<MDistanceAttenuationControl*>(iEffectControl)->RoomRollOffFactorMax(value);
    iLog->Log(_L("CEnhanMediaTestClass::HandleDARoomRollOffFactorMaxL value = [%d]"),value);
	return status;
	}

TInt CEnhanMediaTestClass::HandleSLLocationCartesianL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleSLLocationCartesianL"));
	TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
    TInt a,b,c;
	TInt control = 0;
    status = aItem.GetNextInt(control);

   if (TControl(control) == ESOURLOCCONTROL)
	   {
	   status = static_cast<MSourceLocationControl*>(iEffectControl)->LocationCartesian(a,b,c);
	   }
   else if (TControl(control) == ELISTLOCCONTROL)
	   {
	    status = static_cast<MSourceLocationControl*>(iEffectControl)->LocationCartesian(a,b,c);
	   }
    iLog->Log(_L("CEnhanMediaTestClass::HandleSLLocationCartesianL status = [%d]"),status);
	return status;
	}

TInt CEnhanMediaTestClass::HandleSLLocationSphericalL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleSLLocationSphericalL"));
	TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
    TInt a,b,c;
	TInt control = 0;
    status = aItem.GetNextInt(control);

   if (TControl(control) == ESOURLOCCONTROL)
	   {
	   status = static_cast<MSourceLocationControl*>(iEffectControl)->LocationSpherical(a,b,c);
	   }
   else if (TControl(control) == ELISTLOCCONTROL)
	   {
	   status = static_cast<MListenerLocationControl*>(iEffectControl)->LocationSpherical(a,b,c);
	   }
    iLog->Log(_L("CEnhanMediaTestClass::HandleSLLocationSphericalL status = [%d]"),status);
	return status;
	}

TInt CEnhanMediaTestClass::HandleSLSetLocationCartesianL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleSLSetLocationCartesianL"));
	TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	TInt control = 0;
    status = aItem.GetNextInt(control);
    TInt a,b,c;
    status = aItem.GetNextInt(a);
    status = aItem.GetNextInt(b);
    status = aItem.GetNextInt(c);

   if (TControl(control) == ESOURLOCCONTROL)
	   {
	   status = static_cast<MSourceLocationControl*>(iEffectControl)->SetLocationCartesian(a,b,c);
	   }
   else if (TControl(control) == ELISTLOCCONTROL)
	   {
	   status = static_cast<MListenerLocationControl*>(iEffectControl)->SetLocationCartesian(a,b,c);
	   }
    iLog->Log(_L("CEnhanMediaTestClass::HandleSLSetLocationCartesianL status = [%d]"),status);
	return status;
	}
TInt CEnhanMediaTestClass::HandleSLSetLocationSphericalL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleSLSetLocationSphericalL"));
	TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	TInt control = 0;
    status = aItem.GetNextInt(control);
    TInt a,b,c;
    status = aItem.GetNextInt(a);
    status = aItem.GetNextInt(b);
    status = aItem.GetNextInt(c);
    if (TControl(control) == ESOURLOCCONTROL)
 	   {
 	    status = static_cast<MSourceLocationControl*>(iEffectControl)->SetLocationSpherical(a,b,c);
 	   }
    else if (TControl(control) == ELISTLOCCONTROL)
 	   {
	   status = static_cast<MListenerLocationControl*>(iEffectControl)->SetLocationSpherical(a,b,c);
 	   }

    iLog->Log(_L("CEnhanMediaTestClass::HandleSLSetLocationSphericalL status = [%d]"),status);
	return status;
	}


TInt CEnhanMediaTestClass::HandleControlTypeL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleControlTypeL"));
	TInt status(KErrNone);
	TInt control = 0;
	status = aItem.GetNextInt(control);
	TUid controlUid(TUid::Uid(0));
	switch(TControl(control))
	    {
	    case EFILESOURCE:
	        {
	        if(iMFileSource)
	            {
                controlUid = iMFileSource->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EDATABUFFERSOURCE:
	        {
	        if(iMDataBufferSource)
	            {
                controlUid = iMDataBufferSource->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EDESCRIPTORSOURCE:
	        {
	        if(iMDescriptorSource)
	            {
                controlUid = iMDescriptorSource->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case ESTREAMCONTROL:
	        {
	        if(iMStreamControl)
	            {
                controlUid = iMStreamControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EMMFAUDIOOUTPUTSINKCONTROL:
	        {
	        if(iMAudioSink)
	            {
                controlUid = iMAudioSink->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EVOLUMECONTROL:
	        {
	        if(iMVolumeControl)
	            {
                controlUid = iMVolumeControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EBALANCECONTROL:
	        {
	        if(iMBalanceControl)
	            {
                controlUid = iMBalanceControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EEQUALIZERCONTROL:
	        {
	        if(iMEqualizerControl)
	            {
                controlUid = iMEqualizerControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EREVERBCONTROL:
	        {
	        if(iMReverbControl)
	            {
                controlUid = iMReverbControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EBASSBOOSTCONTROL:
	        {
	        if(iMBassBoostControl)
	            {
                controlUid = iMBassBoostControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESTEREOWIDCONTROL:
	        {
	        if(iMStereoWideningControl)
	            {
                controlUid = iMStereoWideningControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELOUDNESSCONTROL:
	        {
	        if(iMLoudnessControl)
	            {
                controlUid = iMLoudnessControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EROOMLEVELCONTROL:
	        {
	        if(iMRoomLevelControl)
	            {
                controlUid = iMRoomLevelControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EDISTATTCONTROL:
	        {
	        if(iMDistanceAttenuationControl)
	            {
                controlUid = iMDistanceAttenuationControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELISTDOPPCONTROL:
	        {
	        if(iMListenerDopplerControl)
	            {
                controlUid = iMListenerDopplerControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURDOPPCONTROL:
	        {
	        if(iMSourceDopplerControl)
	            {
                controlUid = iMSourceDopplerControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELISTLOCCONTROL:
	        {
	        if(iMListenerLocationControl)
	            {
                controlUid = iMListenerLocationControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURLOCCONTROL:
	        {
	        if(iMSourceLocationControl)
	            {
                controlUid = iMSourceLocationControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }


        case ELISTORIENTCONTROL:
	        {
	        if(iMListenerOrientationControl)
	            {
                controlUid = iMListenerOrientationControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURORIENTCONTROL:
	        {
	        if(iMSourceOrientationControl)
	            {
                controlUid = iMSourceOrientationControl->Type();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
	    }

	iLog->Log(_L("CEnhanMediaTestClass::HandleControlTypeL status = [%d]"),status);
	return status;
	}

TInt CEnhanMediaTestClass::HandleControlControlTypeL(CStifItemParser& aItem)
	{
    iLog->Log(_L("CEnhanMediaTestClass::HandleControlControlTypeL"));
	TInt status(KErrNone);

	TInt control = 0;
	status = aItem.GetNextInt(control);
	TControlType controlType(TControlType(0));
	switch(TControl(control))
	    {
	    case EFILESOURCE:
	        {
	        if(iMFileSource)
	            {
				controlType = iMFileSource->ControlType();
	            }
            else
              {
              status = KErrNotFound;
              }
	        break;
	        }
        case EDATABUFFERSOURCE:
	        {
	        if(iMDataBufferSource)
	            {
				controlType = iMDataBufferSource->ControlType();
	            }
            else
              {
              status = KErrNotFound;
              }
	        break;
	        }
        case EDESCRIPTORSOURCE:
	        {
	        if(iMDescriptorSource)
	            {
				controlType = iMDescriptorSource->ControlType();
	            }
            else
              {
              status = KErrNotFound;
              }
	        break;
	        }
        case ESTREAMCONTROL:
	        {
	        if(iMStreamControl)
	            {
				controlType = iMStreamControl->ControlType();
	            }
            else
              {
              status = KErrNotFound;
              }
	        break;
	        }
        case EMMFAUDIOOUTPUTSINKCONTROL:
	        {
	        if(iMAudioSink)
	            {
				controlType = iMAudioSink->ControlType();
	            }
            else
              {
              status = KErrNotFound;
              }
	        break;
	        }
        case EVOLUMECONTROL:
	        {
	        if(iMVolumeControl)
	            {
                controlType = iMVolumeControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EBALANCECONTROL:
	        {
	        if(iMBalanceControl)
	            {
                controlType = iMBalanceControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EEQUALIZERCONTROL:
	        {
	        if(iMEqualizerControl)
	            {
                controlType = iMEqualizerControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }
        case EREVERBCONTROL:
	        {
	        if(iMReverbControl)
	            {
                controlType = iMReverbControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EBASSBOOSTCONTROL:
	        {
	        if(iMBassBoostControl)
	            {
                controlType = iMBassBoostControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESTEREOWIDCONTROL:
	        {
	        if(iMStereoWideningControl)
	            {
                controlType = iMStereoWideningControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELOUDNESSCONTROL:
	        {
	        if(iMLoudnessControl)
	            {
                controlType = iMLoudnessControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EROOMLEVELCONTROL:
	        {
	        if(iMRoomLevelControl)
	            {
                controlType = iMRoomLevelControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case EDISTATTCONTROL:
	        {
	        if(iMDistanceAttenuationControl)
	            {
                controlType = iMDistanceAttenuationControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELISTDOPPCONTROL:
	        {
	        if(iMListenerDopplerControl)
	            {
                controlType = iMListenerDopplerControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURDOPPCONTROL:
	        {
	        if(iMSourceDopplerControl)
	            {
                controlType = iMSourceDopplerControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ELISTLOCCONTROL:
	        {
	        if(iMListenerLocationControl)
	            {
                controlType = iMListenerLocationControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURLOCCONTROL:
	        {
	        if(iMSourceLocationControl)
	            {
                controlType = iMSourceLocationControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }


        case ELISTORIENTCONTROL:
	        {
	        if(iMListenerOrientationControl)
	            {
                controlType = iMListenerOrientationControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }

        case ESOURORIENTCONTROL:
	        {
	        if(iMSourceOrientationControl)
	            {
                controlType = iMSourceOrientationControl->ControlType();
	            }
            else
                {
                status = KErrNotFound;
                }
	        break;
	        }


	    }

    iLog->Log(_L("CEnhanMediaTestClass::HandleControlControlTypeL status = [%d]"),status);
	return status;
	}






TInt CEnhanMediaTestClass::HandleSDSetFactorL(CStifItemParser& aItem)
    {
    iLog->Log(_L("CEnhanMediaTestClass::HandleLDSetFactorL"));
	TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return KErrNotReady;
	    }
	TInt control = 0;
    status = aItem.GetNextInt(control);
	TUint value = 0;
    aItem.GetNextInt(value);

    if (TControl(control) == ELISTDOPPCONTROL)
    	{
        status = static_cast<MListenerDopplerControl*>(iEffectControl)->SetFactor(value);
    	}
    if (TControl(control) == ESOURDOPPCONTROL)
    	{
        status = static_cast<MSourceDopplerControl*>(iEffectControl)->SetFactor(value);
    	}
    iLog->Log(_L("CEnhanMediaTestClass::HandleLDSetFactorL END"));
	return status;
    }

TInt CEnhanMediaTestClass::HandleSDSetSphericalVelocityL(CStifItemParser& aItem)
    {
    iLog->Log(_L("CEnhanMediaTestClass::HandleSDSetSphericalVelocityL"));
    TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	TInt control = 0;
    status = aItem.GetNextInt(control);
    TInt a,b,c;
    status = aItem.GetNextInt(a);
    status = aItem.GetNextInt(b);
    status = aItem.GetNextInt(c);

    if (TControl(control) == ELISTDOPPCONTROL)
    	{
    
        status = static_cast<MListenerDopplerControl*>(iEffectControl)->SetSphericalVelocity(a,b,c);
    	}
    if (TControl(control) == ESOURDOPPCONTROL)
    	{
    
        status = static_cast<MSourceDopplerControl*>(iEffectControl)->SetSphericalVelocity(a,b,c);
    	}
    iLog->Log(_L("CEnhanMediaTestClass::HandleSDSetSphericalVelocityL END"));
	return status;
    }

TInt CEnhanMediaTestClass::HandleSDSphericalVelocityL(CStifItemParser& aItem)
    {
    iLog->Log(_L("CEnhanMediaTestClass::HandleSDSphericalVelocityL"));
	TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	TInt control = 0;
    status = aItem.GetNextInt(control);
    TInt a,b,c;

    if (TControl(control) == ELISTDOPPCONTROL)
    	{
        status = static_cast<MListenerDopplerControl*>(iEffectControl)->SphericalVelocity(a,b,c);
    	}
    if (TControl(control) == ESOURDOPPCONTROL)
    	{
        status = static_cast<MSourceDopplerControl*>(iEffectControl)->SphericalVelocity(a,b,c);
    	}
    iLog->Log(_L("CEnhanMediaTestClass::HandleSDSphericalVelocityL END"));
    return status;
    }

TInt CEnhanMediaTestClass::HandleSDCartesianVelocityL(CStifItemParser& aItem)
    {
	TInt status(KErrNone);
    iLog->Log(_L("CEnhanMediaTestClass::HandleSDCartesianVelocityL"));
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	TInt control = 0;
    status = aItem.GetNextInt(control);
    TInt a,b,c;

    if (TControl(control) == ELISTDOPPCONTROL)
    	{
        status = static_cast<MListenerDopplerControl*>(iEffectControl)->CartesianVelocity(a,b,c);
    	}
    if (TControl(control) == ESOURDOPPCONTROL)
    	{
        status = static_cast<MSourceDopplerControl*>(iEffectControl)->CartesianVelocity(a,b,c);
    	}
    iLog->Log(_L("CEnhanMediaTestClass::HandleSDCartesianVelocityL END"));
    return status;
    }

TInt CEnhanMediaTestClass::HandleSDFactorL(CStifItemParser& aItem)
    {
	TInt status(KErrNone);
    iLog->Log(_L("CEnhanMediaTestClass::HandleSDFactorL"));
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	TInt control = 0;
    status = aItem.GetNextInt(control);
    TUint value;

    if (TControl(control) == ELISTDOPPCONTROL)
    	{
        status = static_cast<MListenerDopplerControl*>(iEffectControl)->Factor(value);
    	}
    if (TControl(control) == ESOURDOPPCONTROL)
    	{
        status = static_cast<MSourceDopplerControl*>(iEffectControl)->Factor(value);
    	}
    iLog->Log(_L("CEnhanMediaTestClass::HandleSDFactorL END"));
    return status;
    }

TInt CEnhanMediaTestClass::HandleSDFactorMaxL(CStifItemParser& aItem)
    {
    iLog->Log(_L("CEnhanMediaTestClass::HandleLDSetFactorL"));
	TInt status(KErrNone);
	if(!iEffectControl)
	    {
	    return KErrNotReady;
	    }
	TInt control = 0;
    status = aItem.GetNextInt(control);
    TUint value;

    if (TControl(control) == ELISTDOPPCONTROL)
    	{
        status = static_cast<MListenerDopplerControl*>(iEffectControl)->FactorMax(value);
    	}
    if (TControl(control) == ESOURDOPPCONTROL)
    	{
        status = static_cast<MSourceDopplerControl*>(iEffectControl)->FactorMax(value);
    	}
    iLog->Log(_L("CEnhanMediaTestClass::HandleLDSetFactorL END"));
	return status;
    }

TInt CEnhanMediaTestClass::HandleSDSetCartesianVelocityL(CStifItemParser& aItem)
    {
	TInt status(KErrNone);
    iLog->Log(_L("CEnhanMediaTestClass::HandleSDSetCartesianVelocityL"));
	if(!iEffectControl)
	    {
	    return status = KErrNotReady;
	    }
	TInt control = 0;
    status = aItem.GetNextInt(control);
    TInt a,b,c;
    status = aItem.GetNextInt(a);
    status = aItem.GetNextInt(b);
    status = aItem.GetNextInt(c);

    if (TControl(control) == ELISTDOPPCONTROL)
    	{
        status = static_cast<MListenerDopplerControl*>(iEffectControl)->SetCartesianVelocity(a,b,c);
    	}
    if (TControl(control) == ESOURDOPPCONTROL)
    	{
        status = static_cast<MSourceDopplerControl*>(iEffectControl)->SetCartesianVelocity(a,b,c);
    	}
    iLog->Log(_L("CEnhanMediaTestClass::HandleSDSetCartesianVelocityL END"));
    return status;
    }

//  End of File
