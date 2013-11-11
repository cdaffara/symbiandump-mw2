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
* Description:  RadioUtilityTestClass test class for STIF Test Framework TestScripter.
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "RadioUtilityTestClass.h"
#include "debug.h"

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
// CRadioUtilityTestClass::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CRadioUtilityTestClass::Delete() 
{
    FTRACE(FPrint(_L("CTunerUtilityTestClass::Delete")));
    
    iExpectedEvents.Close();
    iOcurredEvents.Close();
    
    iLog->Log(_L("Deleting test class..."));
	iLog->Log(_L(""));
	iLog->Log(_L(""));

    if(iRadioUtility)
   	{
        delete iRadioUtility;
   	}

    if(iRadioFMTunerUtility)
   	{
        iRadioFMTunerUtility = NULL;
   	}

    if (iRadioPlayerUtility)
   	{
        iRadioPlayerUtility = NULL;
   	}
    
    if(iRadioRdsUtility)
   	{
        iRadioRdsUtility = NULL;
   	}
    if ( iRadioMonitor )
    {
        delete iRadioMonitor;
    }
    if ( iFmPresetUtility )
    {
        iFmPresetUtility->DeletePresetL(-1);
        delete iFmPresetUtility;
    }
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::RunMethodL( 
    CStifItemParser& aItem ) 
    {
    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "StartMonitor", CRadioUtilityTestClass::StartMonitor ),
        ENTRY( "StopMonitor", CRadioUtilityTestClass::StopMonitor ),
        ENTRY( "IsRadioOn", CRadioUtilityTestClass::IsRadioOn ),
        ENTRY( "CreateFactory", CRadioUtilityTestClass::CreateFactory ),
        ENTRY( "CreateUtility", CRadioUtilityTestClass::CreateUtility ),
        ENTRY( "RequestTunerControl", CRadioUtilityTestClass::RequestTunerControl ),
        ENTRY( "SetFrequencyRange", CRadioUtilityTestClass::SetFrequencyRange ),
        ENTRY( "SetFrequency", CRadioUtilityTestClass::SetFrequency ),
        ENTRY( "StationSeek", CRadioUtilityTestClass::StationSeek ),
        ENTRY( "Cancel", CRadioUtilityTestClass::Cancel ),
        ENTRY( "Check", CRadioUtilityTestClass::Check ),
        ENTRY( "Set" , CRadioUtilityTestClass::Set ),
        ENTRY( "Get" , CRadioUtilityTestClass::Get ),
        ENTRY( "Play" , CRadioUtilityTestClass::Play ),
        ENTRY( "Stop" , CRadioUtilityTestClass::Stop ),
        ENTRY( "Close" , CRadioUtilityTestClass::Close ),
        ENTRY( "SetBalance" , CRadioUtilityTestClass::SetBalance ),
        ENTRY( "CheckBalance" , CRadioUtilityTestClass::CheckBalance ),
        ENTRY( "ChangeBalance" , CRadioUtilityTestClass::ChangeBalance ),
        ENTRY( "ChangeVolume", CRadioUtilityTestClass::ChangeVolume ),
        ENTRY( "VerifyMaxValues" , CRadioUtilityTestClass::VerifyMaxValues ),
        ENTRY( "VerifyCapabilities" , CRadioUtilityTestClass::VerifyCapabilities ),
        ENTRY( "OfflineMode" , CRadioUtilityTestClass::OfflineMode), 
        ENTRY( "GetRdsCapabilities", CRadioUtilityTestClass::GetRdsCapabilities ),
        ENTRY( "GetRdsSignalStatus", CRadioUtilityTestClass::GetRdsSignalStatus ),
        ENTRY( "NotifyRdsDataChange", CRadioUtilityTestClass::NotifyRdsDataChange ),
        ENTRY( "CancelNotifyRdsDataChange", CRadioUtilityTestClass::CancelNotifyRdsDataChange ),
        ENTRY( "CancelAFSearch", CRadioUtilityTestClass::CancelAFSearch ),
        ENTRY( "StationSeekByPTY", CRadioUtilityTestClass::StationSeekByPTY ),
        ENTRY( "StationSeekByTA", CRadioUtilityTestClass::StationSeekByTA ),
        ENTRY( "StationSeekByTP", CRadioUtilityTestClass::StationSeekByTP ),
        ENTRY( "GetProgrammeIdentification", CRadioUtilityTestClass::GetProgrammeIdentification ),
        ENTRY( "GetProgrammeType", CRadioUtilityTestClass::GetProgrammeType ),
        ENTRY( "GetProgrammeService", CRadioUtilityTestClass::GetProgrammeService ),
        ENTRY( "GetRadioText", CRadioUtilityTestClass::GetRadioText ),
        ENTRY( "GetClockTime", CRadioUtilityTestClass::GetClockTime ),
        ENTRY( "GetTrafficAnnouncementStatus", CRadioUtilityTestClass::GetTrafficAnnouncementStatus ),
        ENTRY( "GetTrafficProgrammeStatus", CRadioUtilityTestClass::GetTrafficProgrammeStatus ),
        ENTRY( "SetPreset", CRadioUtilityTestClass::SetPreset ),
        ENTRY( "GetPreset", CRadioUtilityTestClass::GetPreset ),
        ENTRY( "GetMaxNumberOfPresets", CRadioUtilityTestClass::GetMaxNumberOfPresets ),
        ENTRY( "GetNumberOfPresets", CRadioUtilityTestClass::GetNumberOfPresets ),
        ENTRY( "GetFirstPresetL", CRadioUtilityTestClass::GetFirstPresetL ),
        ENTRY( "GetNextPresetL", CRadioUtilityTestClass::GetNextPresetL ),
        ENTRY( "DeletePreset", CRadioUtilityTestClass::DeletePreset ),  
        ENTRY( "SetUrlToPreset", CRadioUtilityTestClass::SetUrlToPreset ),
        ENTRY( "GetUrlFromPreset", CRadioUtilityTestClass::GetUrlFromPreset ),
        ENTRY( "SetFavouriteInfoToPreset", CRadioUtilityTestClass::SetFavouriteInfoToPreset ),
        ENTRY( "GetFavouriteInfoFromPreset", CRadioUtilityTestClass::GetFavouriteInfoFromPreset ),
        ENTRY( "SetPtyToPreset", CRadioUtilityTestClass::SetPtyToPreset ),
        ENTRY( "GetPtyFromPreset", CRadioUtilityTestClass::GetPtyFromPreset ),
        ENTRY( "SetPIToPreset", CRadioUtilityTestClass::SetPIToPreset ),
        ENTRY( "GetPIFromPreset", CRadioUtilityTestClass::GetPIFromPreset ),
        
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }


// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::EventName
// Return descriptor with the notification description
// -----------------------------------------------------------------------------
TPtrC CRadioUtilityTestClass::EventName( TInt aKey )
{
	static TText* const badKeyword = (TText*)L"BadKeyword";
	static TText* const keywords[] =
	{
		(TText*)L"ERequestTunerControlComplete",
		(TText*)L"EStationSeekComplete", 
		(TText*)L"ESetFrequencyRangeComplete", 
		(TText*)L"ETuneComplete",
		(TText*)L"EMrpoStateChange",
		(TText*)L"EFrequencyChanged",
		(TText*)L"EMrftoOfflineModeStatusChange",
		(TText*)L"EMrpoBalanceChange",
		(TText*)L"EMrpoVolumeChange",
		(TText*)L"EMrpoMuteChange",
		(TText*)L"EMrftoSquelchChange",
		(TText*)L"EMrftoForcedMonoChange",
		(TText*)L"EMrroStationSeekByPTYComplete",
		(TText*)L"EMrroStationSeekByTAComplete",
		(TText*)L"EMrroStationSeekByTPComplete",
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
// CRadioUtilityTestClass::AddExpectedEvent
// Add an event to the expected events' list
// -----------------------------------------------------------------------------
void CRadioUtilityTestClass::AddExpectedEvent(TRadioExpectedEvent event, TInt ms)
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::AddExpectedEvent")));
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
// CRadioUtilityTestClass::RemoveExpectedEvent
// Remove the indicated event from the expected events' list
// Returns: ETrue: Event found.
//          EFalse: Event not found.
// -----------------------------------------------------------------------------
TBool CRadioUtilityTestClass::RemoveExpectedEvent(TRadioExpectedEvent aEvent)
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::RemoveExpectedEvent")));
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
// CRadioUtilityTestClass::RemoveAllExpectedEvents
// Remove the indicated event from the expected events' list
// -----------------------------------------------------------------------------
void CRadioUtilityTestClass::RemoveAllExpectedEvents()
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::RemoveAllExpectedEvents")));
	iLog->Log(_L("Removing all expected events"));

	iExpectedEvents.Reset();
	iOcurredEvents.Reset();
}


// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::ProcessEvent
// Process events.
// -----------------------------------------------------------------------------
void CRadioUtilityTestClass::ProcessEvent(TRadioExpectedEvent aEvent, TInt aError)
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::ProcessExpectedEvent")));
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
// CRadioUtilityTestClass::HandleTimeout
// Review if all the expected events have ocurred once the time is over
// -----------------------------------------------------------------------------
void CRadioUtilityTestClass::HandleTimeout(TInt aError)
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::HandleTimeout")));
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
// CRadioUtilityTestClass::SetTimeout
// Create a timer and set a timeout
// When the timeout is reached the test case is marked as failed
// It's used rather than the "timeout" keyword in the configuration file
// because in this way the log continues
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::SetTimeout( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::SetTimeout")));
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
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::SetAllowedPanic( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::SetAllowedPanic")));
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
// CRadioUtilityTestClass::SetExpectedEvents()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::SetExpectedEvents( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::SetExpectedEvents")));
	TInt error = KErrNone;
	TInt event=0;
	while ( KErrNone == aItem.GetNextInt(event))
	{
		AddExpectedEvent(static_cast<TRadioExpectedEvent>(event), 0); // Default timeout value
	}
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::StartMonitor()
// Start RadioMonitor.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::StartMonitor( CStifItemParser& /*aItem*/ )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::StartMonitor")));
    iLog->Log(_L("CRadioUtilityTestClass::StartMonitor"));
    TInt err=KErrNone;
    iLog->Log(_L("CRadioUtilityTestClass::StartMonitorL"));
	TRAP( err, iRadioMonitor = CRadioMonitor::NewL(*this));
	return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::StopMonitor()
// Stop RadioMonitor.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::StopMonitor( CStifItemParser& /*aItem */)
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::StopMonitor")));
    iLog->Log(_L("CRadioUtilityTestClass::StopMonitor"));
	delete iRadioMonitor;
    iRadioMonitor = NULL;
	return KErrNone;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::IsRadioOn()
// Stop RadioMonitor.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::IsRadioOn( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::IsRadioOn")));
    iLog->Log(_L("CRadioUtilityTestClass::IsRadioOn"));
    TInt err = KErrNone;
    TBool expectedResult;
    
    if ( aItem.GetNextInt(expectedResult) )
    {
        iLog->Log(_L("Missing Parameter: Expected result."));
        err = KErrBadTestParameter;
    }
    TBool result = iRadioMonitor->IsRadioOn();
    // Verification
    if ( result != expectedResult )
    {
        iLog->Log(_L("Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
    }
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::CreateFactory
// Create radio utility
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::CreateFactory( CStifItemParser& /*aItem */)
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::CreateFactory")));
	iLog->Log(_L("CRadioUtilityTestClass::CreateFactory"));
	TInt error = KErrNone;

	TRAP(error, iRadioUtility = CRadioUtility::NewL(ETrue) ); 

	return error;
}

// -----------------------------------------------------------------------------
// create first FMTuner, Player, RDS
// create Player without FMTuner
// create RDS without FMTuner
// create first FMTuner, check capabilities and create RDS
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::CreateUtility( CStifItemParser& aItem )
{

	FTRACE(FPrint(_L("CRadioUtilityTestClass::CreateUtility")));
	TInt error = KErrNone;
	
	iLog->Log(_L("CreateUtility"));
	TPtrC utility;
    error = aItem.GetNextString(utility);
    if (error == KErrNone)
    {
        iLog->Log(_L("Creating %S"), &utility);
        if (utility == KTagTuner)
        {
            TRAP(error, iRadioFMTunerUtility = &iRadioUtility->RadioFmTunerUtilityL(*this) );
        }
        else if (utility == KTagPlayer)
        {
            TRAP(error, iRadioPlayerUtility = &iRadioUtility->RadioPlayerUtilityL(*this) );
        }
        else if (utility == KTagRds)
        {
            TRAP(error, iRadioRdsUtility = &iRadioUtility->RadioRdsUtilityL(*this) );
        }
        else if (utility == KTagPreset)
        {
            TRAP(error, iFmPresetUtility = CRadioFmPresetUtility::NewL(*this));
        }
        else
        {
            iLog->Log(KMsgBadTestParameters);
            error = KErrBadTestParameter;
        }
    }
     
     return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::RequestTunerControl
// Request radio tuner control.
// Callbacks: MrftoRequestTunerControlComplete
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::RequestTunerControl( CStifItemParser& /*aItem */)
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::RequestTunerControl")));
	
	iLog->Log(_L("CRadioUtilityTestClass::RequestTunerControl"));
	TInt error = KErrNone;
	
	iRadioFMTunerUtility->RequestTunerControl();
	
	AddExpectedEvent(ERequestTunerControlComplete, KMediumTimeout);
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::SetFrequencyRange
// Set the frequency range.
// Callbacks: MrftoSetFrequencyRangeComplete
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::SetFrequencyRange( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::SetFrequencyRange")));
	
	iLog->Log(_L("SetFrequencyRange"));
	
	TInt error = KErrNone;
	TPtrC RangeType;
	TFmTunerCapabilities caps;
	error = iRadioFMTunerUtility->GetCapabilities(caps);
	if (error != KErrNone)
	{
		iLog->Log(_L("Verify capability failed, return error %d"), error);
		return error;
	}
	
	error = aItem.GetNextString(RangeType);

	if (error == KErrNone)
	{
		if (RangeType == KTagAmerica)
		{
			if ( caps.iTunerBands & EFmRangeEuroAmerica )
			{
				iLog->Log(_L("Set FrequencyRange to America"));
				iRadioFMTunerUtility->SetFrequencyRange(EFmRangeEuroAmerica);
			}
			else
			{
				iLog->Log(_L("FrequencyRange EFmRangeEuroAmerica is not supported"));
				return KErrNotSupported;				
			}
		}
		else if (RangeType == KTagJapan)
		{
			if ( caps.iTunerBands & EFmRangeJapan )
			{
				iLog->Log(_L("Set FrequencyRange to Japan"));
				iRadioFMTunerUtility->SetFrequencyRange(EFmRangeJapan);
			}
			else
			{
				iLog->Log(_L("FrequencyRange EFmRangeJapan is not supported"));
				return KErrNotSupported;				
			}
		}
	}
	else
	{
		iLog->Log(KMsgBadTestParameters);
        error = KErrBadTestParameter;
	}
		
	if (error == KErrNone)
		AddExpectedEvent(ESetFrequencyRangeComplete, KMediumTimeout);
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::SetFrequency
// Tune to a frequency.
// Callbacks: MrftoSetFrequencyComplete, MrftoFrequencyChange
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::SetFrequency( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::SetFrequency")));
	
	iLog->Log(_L("SetFrequency"));
	TInt error = KErrNone;
	TInt freq;
    error = aItem.GetNextInt(freq);
    
    if (error == KErrNone)
    {
        iLog->Log(_L("SetFrequency, freq= %d"),freq);
        iRadioFMTunerUtility->SetFrequency(freq);
    }

	AddExpectedEvent(ETuneComplete, KShortTimeout);
	AddExpectedEvent(EFrequencyChanged, KShortTimeout);
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::Close
// Close the radio utility.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::Close( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::Close")));
	iLog->Log(_L("CRadioUtilityTestClass::Close!!! "));
	
	TInt error = KErrNone;
	TPtrC closeType;

	if (  error == aItem.GetNextString(closeType) )
	{
		iLog->Log(_L("Close %S"), &closeType);
		
		if ((closeType == KTagTuner) && (iRadioFMTunerUtility))
		{
			iRadioFMTunerUtility->Close();
			AddExpectedEvent(ERequestTunerControlComplete, KShortTimeout);
		}
		else if ((closeType == KTagPlayer) && (iRadioPlayerUtility))
		{
			iRadioPlayerUtility->Close();
		}
		else if ((closeType == KTagRds) && (iRadioRdsUtility))
		{
			iRadioRdsUtility->Close();
		}
		else
		{
			iLog->Log(KMsgBadTestParameters);
            error = KErrBadTestParameter;
		}
	}
	
	return error;
}


// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::StationSeek
// Seek up/down.
// Callbacks: MrftoStationSeekComplete
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::StationSeek( CStifItemParser& aItem )
{
	TInt error;
    TPtrC seekType;
    TBool Upwards = ETrue;
    error = aItem.GetNextString(seekType);

    if (error == KErrNone)
    {
        iLog->Log(_L("Seeking %S"),&seekType);

        if (seekType == KTagUp)
        {
            iRadioFMTunerUtility->StationSeek(Upwards);
			iLog->Log(_L("Start Seeking Up!!!"));
        }
        else if (seekType == KTagDown)
        {
            iRadioFMTunerUtility->StationSeek(!Upwards);
			iLog->Log(_L("Start Seeking Down!!!"));
        }
        else
        {
            iLog->Log(KMsgBadTestParameters);
            error = KErrBadTestParameter;
        }
    }

	if (error == KErrNone)
	{
		AddExpectedEvent(EStationSeekComplete, KMediumTimeout);
	}
	else
	{
		iLog->Log(_L("Not start seeking, error %d"), error);
	}

    return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::Cancel
// Cancel requests.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::Cancel( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::Cancel")));
	iLog->Log(_L("CRadioUtilityTestClass::Cancel!!! "));
	
	TInt error = KErrNone;
	TPtrC cancelType;
	
	if (  error == aItem.GetNextString(cancelType) )
	{
		iLog->Log(_L("Cancel %S"), &cancelType);
		
		if (cancelType == KTagFrequencyRange)
		{
			iRadioFMTunerUtility->CancelSetFrequencyRange();
    		if (RemoveExpectedEvent(ESetFrequencyRangeComplete))
			{
    			Signal();
				iLog->Log(_L("Cancel Event; ESetFrequencyRangeComplete"));
			}
		}
		else if (cancelType == KTagFrequency)
		{	
			iRadioFMTunerUtility->CancelSetFrequency();
    		if (RemoveExpectedEvent(ETuneComplete) && RemoveExpectedEvent(EFrequencyChanged))
    			Signal();
		}
		else if (cancelType == KTagStationSeek)
		{
			iRadioFMTunerUtility->CancelStationSeek();
    		if (RemoveExpectedEvent(EStationSeekComplete))
    			Signal();
		}
		else if (cancelType == KTagStationSeekByPTY)
		{
			iRadioRdsUtility->CancelRdsStationSeek();
			iLog->Log(_L("CancelRdsStationSeek"));
    		if (RemoveExpectedEvent(EMrroStationSeekByPTYComplete))
    			Signal();
		}
		else if (cancelType == KTagStationSeekByTA)
		{
			iRadioRdsUtility->CancelRdsStationSeek();
    		if (RemoveExpectedEvent(EMrroStationSeekByTAComplete))
    			Signal();
		}
		else if (cancelType == KTagStationSeekByTP)
		{
			iRadioRdsUtility->CancelRdsStationSeek();
    		if (RemoveExpectedEvent(EMrroStationSeekByTPComplete))
    			Signal();
		}
		else
		{
			iLog->Log(KMsgBadTestParameters);
			error = KErrBadTestParameter;
		}
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::Play()
// Start playing the radio.
// Callback: MrpoStateChange
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::Play( CStifItemParser& /*aItem */)
{
	TInt error = KErrNone;

    iRadioPlayerUtility->Play();  
	AddExpectedEvent(EMrpoStateChange, KShortTimeout);
	
    return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::Stop()
// Start playing the radio.
// Callback: MrpoStateChange
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::Stop( CStifItemParser& /*aItem*/ )
{
    TInt error = KErrNone;
    
	iRadioPlayerUtility->Stop();
	AddExpectedEvent(EMrpoStateChange, KShortTimeout);
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::VerifyCapabilities()
// Verify capabilities.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::VerifyCapabilities( CStifItemParser&/* aItem*/ )
{
	TInt error = KErrNone;
	
	TFmTunerCapabilities caps;
	error = iRadioFMTunerUtility->GetCapabilities(caps);
	if (error == KErrNone)
	{
		// Range
		if ( caps.iTunerBands & EFmRangeEuroAmerica )
		{
			iLog->Log(_L("VerifyCapabilities -- frequency range EFmRangeEuroAmerica is supported!!! "));
		}
		
		if ( caps.iTunerBands & EFmRangeJapan )
		{
			iLog->Log(_L("VerifyCapabilities -- frequency range EFmRangeJapan is supported!!! "));
		}
		
		// Supported features
		if ( caps.iTunerFunctions & TFmTunerCapabilities::ETunerAvailableInOfflineMode )
		{
			iLog->Log(_L("VerifyCapabilities -- OfflineMode is available!!! "));
		}
		
		if ( caps.iTunerFunctions & TFmTunerCapabilities::ETunerRdsSupport )
		{
			iLog->Log(_L("VerifyCapabilities -- Rds is supported!!! "));
		}
		
		if ( caps.iTunerFunctions & TFmTunerCapabilities::ETunerDualTunerSupport )
		{
			iLog->Log(_L("VerifyCapabilities -- Dual Tuner is Supported!!! "));
		}
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::VerifyMaxValues()
// Verify maximum values.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::VerifyMaxValues( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::VerifyMaxValues")));
	iLog->Log(_L("CRadioUtilityTestClass::VerifyMaxValues!!! "));
	TInt vError = KErrNone;
	TPtrC verifyType;
	
	if (  vError == aItem.GetNextString(verifyType) )
	{
		iLog->Log(_L("VerifyMaxValues %S"), &verifyType);
		
		if (verifyType == KTagSignalStrength)
		{
			TInt maxSignalStrength;
			vError = iRadioFMTunerUtility->GetMaxSignalStrength(maxSignalStrength);
			iLog->Log(_L("GetMaxSignalStrength: %d\n"),maxSignalStrength);
		}
		else if (verifyType == KTagVolume)
		{
			TInt maxVolume = 0;
			vError = iRadioPlayerUtility->GetMaxVolume(maxVolume);
			if (vError == KErrNone)
			{
				iLog->Log(_L("GetMaxVolume: %d"),maxVolume);
			}
			else
			{
				iLog->Log(_L("GetMaxVolume failed: return error = %d"),vError);
			}
		}
		else 
		{
			iLog->Log(KMsgBadTestParameters);
			vError = KErrBadTestParameter;
		}
		
	}
	
	return vError;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::OfflineMode()
// Set offline mode.
// Callbacks: MrftoOfflineModeStatusChange
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::OfflineMode( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::OfflineMode")));
	iLog->Log(_L("CRadioUtilityTestClass::OfflineMode!!! "));
	TInt error = KErrNone;
	
	TFmTunerCapabilities caps;
	error = iRadioFMTunerUtility->GetCapabilities(caps);
	if (error != KErrNone)
	{
		iLog->Log(_L("Verify capability failed, return error %d"), error);
		return error;
	}
	
	// Supported features
	if ( !(caps.iTunerFunctions & TFmTunerCapabilities::ETunerAvailableInOfflineMode ) )
	{
		iLog->Log(_L("VerifyCapabilities -- OfflineMode is not available!!! "));
		return KErrNotSupported;
	}
	
	TInt offlineMode;
	error = aItem.GetNextInt(offlineMode);
	if (error == KErrNone)
	{
		if (offlineMode)
		{
			error = iRadioFMTunerUtility->EnableTunerInOfflineMode(ETrue);
		}
		else
		{
			error = iRadioFMTunerUtility->EnableTunerInOfflineMode(EFalse);
		}
	}
	else
	{
		iLog->Log(KMsgBadTestParameters);
		error = KErrBadTestParameter;
	}
	
	if (error == KErrNone)
		AddExpectedEvent(EMrftoOfflineModeStatusChange, KMediumTimeout);

	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::SetBalance()
// Set balance.
// Callbacks: MrpoBalanceChange
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::SetBalance( CStifItemParser& aItem )
{	
	FTRACE(FPrint(_L("CRadioUtilityTestClass::CheckBalance")));
	iLog->Log(_L("CRadioUtilityTestClass::SetBalance!!! "));
	
	TInt error = KErrNone;
	TPtrC side;
	TInt leftBalance, rightBalance;
	
	if ( KErrNone == aItem.GetNextString(side) )
	{
		if (side == KTagLeft)
		{
			error = aItem.GetNextInt(leftBalance);
			iLog->Log(_L("left balance is %d"), leftBalance);
		}
		else
		{
			iLog->Log(KMsgBadTestParameters);
			error = KErrBadTestParameter;
		}
	}
	else
	{
		iLog->Log(KMsgBadTestParameters);
		error = KErrBadTestParameter;
	}
	
	if (error == KErrNone)	
	{
		if ( KErrNone == aItem.GetNextString(side)) 
		{
			if (side == KTagRight)
			{
				error = aItem.GetNextInt(rightBalance);
				iLog->Log(_L("right balance is %d"), rightBalance);
			}
			else
			{
				iLog->Log(KMsgBadTestParameters);
				error = KErrBadTestParameter;
			}
		}
	
		if (error == KErrNone)
		{
		
			error = iRadioPlayerUtility->SetBalance(leftBalance, rightBalance);
			iLog->Log(_L("Call SetBalance, return code = %d, l =%d, r =%d"), error, leftBalance, rightBalance);
		
		}

		if (error == KErrNone)
		{
			AddExpectedEvent(EMrpoBalanceChange, KShortTimeout);
		}
		else
		{
			iLog->Log(_L("SetBalance failed. Return error = %d"), error);
		}
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::ChangeVolume()
// Change volume up/down.
// Callbacks: MrpoVolumeChange
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::ChangeVolume( CStifItemParser& aItem )
{	
	FTRACE(FPrint(_L("CRadioUtilityTestClass::ChangeVolume")));
	iLog->Log(_L("CRadioUtilityTestClass::ChangeVolume!!! "));
	
	TInt error = KErrNone;
	TPtrC direction;
	TInt volume;
	
	error = iRadioPlayerUtility->GetVolume(volume);
	
	if (error == KErrNone)
	{
		
		if ( KErrNone == aItem.GetNextString(direction) )
		{
			if (direction == KTagUp)
			{
				error = iRadioPlayerUtility->SetVolume(volume + 1);
			}
			else if (direction == KTagDown)
			{
				error = iRadioPlayerUtility->SetVolume(volume - 1);		
			}
			else
			{
				iLog->Log(KMsgBadTestParameters);
				error = KErrBadTestParameter;
			}
		}
		else
		{
			iLog->Log(KMsgBadTestParameters);
			error = KErrBadTestParameter;
		}

		if (error == KErrNone)
		{
			AddExpectedEvent(EMrpoVolumeChange, KShortTimeout);
		}
		else
		{
			iLog->Log(_L("SetVolume failed. Return error = %d"), error);
		}
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::ChangeBalance()
// Change balance left/right.
// Callbacks: MrpoBalanceChange
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::ChangeBalance( CStifItemParser& aItem )
{	
	FTRACE(FPrint(_L("CRadioUtilityTestClass::ChangeBalance")));
	iLog->Log(_L("CRadioUtilityTestClass::ChangeBalance!!! "));
	
	TInt error = KErrNone;
	TPtrC side;
	TInt leftBalance, rightBalance, changedBalance;
	
	error = iRadioPlayerUtility->GetBalance(leftBalance, rightBalance);
	
	if (error == KErrNone)
	{
		
		if ( (KErrNone == aItem.GetNextString(side) )&& 				
			(KErrNone == aItem.GetNextInt(changedBalance)) )
		{
			if (side == KTagLeft)
			{
				error = iRadioPlayerUtility->SetBalance(leftBalance + changedBalance, rightBalance - changedBalance);
			}
			else if (side == KTagRight)
			{
				error = iRadioPlayerUtility->SetBalance(leftBalance - changedBalance, rightBalance + changedBalance);		
			}
			else
			{
				iLog->Log(KMsgBadTestParameters);
				error = KErrBadTestParameter;
			}
		}
		else
		{
			iLog->Log(KMsgBadTestParameters);
			error = KErrBadTestParameter;
		}
	}

	if (error == KErrNone)
	{
		AddExpectedEvent(EMrpoBalanceChange, KShortTimeout);
	}
	else
	{
		iLog->Log(_L("ChangeBalance failed. Return error = %d"), error);
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::CheckBalance()
// Check balance.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::CheckBalance( CStifItemParser& aItem )
{	
	FTRACE(FPrint(_L("CRadioUtilityTestClass::CheckBalance")));
	iLog->Log(_L("CRadioUtilityTestClass::CheckBalance!!! "));
	
	TInt error = KErrNone;
	TPtrC side;
	TPtrC expectedCompareResult;
	TInt leftBalance, rightBalance, expectedBalance;
	TBool fRealResult = EFalse;
	TBool fExpectedCompareResult = EFalse;
		
	error = iRadioPlayerUtility->GetBalance(leftBalance, rightBalance);

	if (error == KErrNone)
	{
		
		if ( (KErrNone == aItem.GetNextString(side) ) &&
		 	(KErrNone == aItem.GetNextString(expectedCompareResult) ) &&
			(KErrNone ==  aItem.GetNextInt(expectedBalance) ) )
		{
		
			if (expectedCompareResult == KTagCompareEqual)
			{
				fExpectedCompareResult = ETrue;
			}
			else if (expectedCompareResult == KTagCompareNotEqual)
			{
				fExpectedCompareResult = EFalse;
			}
			else
			{
				iLog->Log(KMsgBadTestParameters);
				error = KErrBadTestParameter;
			}
		}
		else
		{
			iLog->Log(KMsgBadTestParameters);
			error = KErrBadTestParameter;
		}
		
		if (error == KErrNone)	
		{
				
			if (side == KTagLeft)
			{
				fRealResult = (expectedBalance == leftBalance ? ETrue: EFalse );
			}
			else if (side == KTagRight)
			{
				fRealResult = (expectedBalance == rightBalance ? ETrue: EFalse );	
			}
			else
			{
				iLog->Log(KMsgBadTestParameters);
				error = KErrBadTestParameter;
			}
		}
	}
		
	if (error == KErrNone)
	{
		if (fRealResult != fExpectedCompareResult)
		{
			iLog->Log(_L("[Error] Expected and current balance don't match"));
			error = KErrUnexpectedValue;
		}
	}
			
	return error;
}
	
// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::Check()
// Check values.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::Check( CStifItemParser& aItem )
{	
	FTRACE(FPrint(_L("CRadioUtilityTestClass::Check")));
	iLog->Log(_L("CRadioUtilityTestClass::Check!!! "));
	
	TInt error = KErrNone;
	TPtrC checkType;
	TPtrC expectedCompareResult;
	TInt  expectedResult;
	TBool fExpectedCompareResult = EFalse;
	TBool fRealResult = EFalse;
	
	if (  (KErrNone == aItem.GetNextString(checkType) ) &&
	(KErrNone == aItem.GetNextString(expectedCompareResult) ) &&
	(KErrNone ==  aItem.GetNextInt(expectedResult) ) )
	{
		// Get expected result compare
		if (expectedCompareResult == KTagCompareEqual)
		{
			fExpectedCompareResult = ETrue;
		}
		else if (expectedCompareResult == KTagCompareNotEqual)
		{
			fExpectedCompareResult = EFalse;
		}
		else
		{
			iLog->Log(KMsgBadTestParameters);
			error = KErrBadTestParameter;
		}
	}
	else
	{
		iLog->Log(KMsgBadTestParameters);
		error = KErrBadTestParameter;
	}
	
	// No parameters missing
	if (error == KErrNone)
	{
		iLog->Log(_L("Checking value for %S"), &checkType);
		_LIT(KFormatCheck, "Expected %S%S%d. Current=%d");
		
		// TUNER
		if (checkType == KTagFrequencyRange)
		{
			TFmRadioFrequencyRange range;
			TInt minFreq, maxFreq;
			error = iRadioFMTunerUtility->GetFrequencyRange(range, minFreq, maxFreq);
			iLog->Log(_L("Expected %S %S 0x%02x. Current = 0x%02x"), &checkType,&expectedCompareResult,expectedResult,range);
			iLog->Log(_L("Min frequency = %d Hz, Max frequency = %d Hz"),minFreq, maxFreq );
			fRealResult = (expectedResult == range ? ETrue: EFalse );
		}
		else if(checkType == KTagFrequency)
		{	
			TInt frequency;
			error = iRadioFMTunerUtility->GetFrequency(frequency);
			iLog->Log(KFormatCheck, &checkType,&expectedCompareResult,expectedResult,frequency);
			fRealResult = (expectedResult == frequency ? ETrue: EFalse );
		}
		else if(checkType == KTagForcedMono)
		{	
			TBool value;
			error = iRadioFMTunerUtility->GetForcedMonoReception(value);
			iLog->Log(KFormatCheck, &checkType,&expectedCompareResult,expectedResult,value);
			fRealResult = (expectedResult == value ? ETrue: EFalse );
		}
		else if(checkType == KTagSquelch)
		{	
			TBool value;
			error = iRadioFMTunerUtility->GetSquelch(value);
			iLog->Log(KFormatCheck, &checkType,&expectedCompareResult,expectedResult,value);
			fRealResult = (expectedResult == value ? ETrue: EFalse );
		}
		// Player
		else if(checkType == KTagVolume)
		{	
			TBool value;
			error = iRadioPlayerUtility->GetVolume(value);
			iLog->Log(KFormatCheck, &checkType,&expectedCompareResult,expectedResult,value);
			fRealResult = (expectedResult == value ? ETrue: EFalse );
		}
		else if(checkType == KTagMute)
		{	
			TBool value;
			value = iRadioPlayerUtility->IsMute();
			iLog->Log(KFormatCheck, &checkType,&expectedCompareResult,expectedResult,value);
			fRealResult = (expectedResult == value ? ETrue: EFalse );
		}
		else if(checkType == KTagPlayerState)
		{	
			TPlayerState aPlayerState;
			aPlayerState = iRadioPlayerUtility->PlayerState();
			iLog->Log(KFormatCheck, &checkType,&expectedCompareResult,expectedResult,aPlayerState);
			fRealResult = (expectedResult == aPlayerState ? ETrue: EFalse );
		}
		else if(checkType == KTagAutomaticSwitching)
		{	
			TBool value;
			error = iRadioRdsUtility->GetAutomaticSwitching(value);
			iLog->Log(KFormatCheck, &checkType,&expectedCompareResult,expectedResult,value);
			fRealResult = (expectedResult == value ? ETrue: EFalse );
		}
		else
		{
			iLog->Log(KMsgBadTestParameters);
			error = KErrBadTestParameter;
		}
		
		if (error == KErrNone)
		{
			if (fRealResult != fExpectedCompareResult)
			{
				iLog->Log(_L("[Error] Expected and current values don't match"));
				error = KErrUnexpectedValue;
			}
		}
	}
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::Set()
// Set values.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::Set( CStifItemParser& aItem )
{
	TInt sError = KErrNone;
	TPtrC setType;
	TInt value;
	
	if (  (KErrNone == aItem.GetNextString(setType) ) &&
			(KErrNone ==  aItem.GetNextInt(value) ) )
	{
		iLog->Log(_L("Set %S"), &setType);
	}
	else
	{
		iLog->Log(KMsgBadTestParameters);
		sError = KErrBadTestParameter;
	}

	if (  sError == KErrNone )
	{
		//TUNER
		if (setType == KTagForcedMono)
		{
			sError = iRadioFMTunerUtility->ForceMonoReception(value);
			
			if (sError == KErrNone)
			{
				AddExpectedEvent(EMrftoForcedMonoChange, KShortTimeout);
				iLog->Log(_L("%S= %d"), &setType, value);
			}
			else
			{
				iLog->Log(_L("ForceMonoReception failed, return error =%d"), sError);
			}
		}
		else if (setType == KTagSquelch)
		{
			sError = iRadioFMTunerUtility->SetSquelch(value);

			if (sError == KErrNone)
			{
				AddExpectedEvent(EMrftoSquelchChange, KShortTimeout);
				iLog->Log(_L("%S= %d"), &setType, value);
			}
			else
			{
				iLog->Log(_L("SetSquelch failed, return error =%d"), sError);
			}
		} 
		
		// PLAYER		
		else if (setType == KTagVolume)
		{
			sError = iRadioPlayerUtility->SetVolume( value );
			if (sError == KErrNone)
			{
				iLog->Log(_L("%S= %d"), &setType, value);
				AddExpectedEvent(EMrpoVolumeChange, KShortTimeout);
			}
			else
			{
				iLog->Log(_L("Set volume failed, return error = %d"),  sError);
			}
		}
		else if (setType == KTagMute)
		{
			sError = iRadioPlayerUtility->Mute( value );
			if (sError == KErrNone)
			{
				iLog->Log(_L("%S= %d"), &setType, value);
				AddExpectedEvent(EMrpoMuteChange, KShortTimeout);
			}
			else
			{
				iLog->Log(_L("Set Mute failed, return error = %d"), sError);
			}
		}
		else if (setType == KTagVolumeRamp)
		{
			TInt ms;
			TInt us;
			sError = aItem.GetNextInt(ms);
			sError = aItem.GetNextInt(us);
			TInt64 total = value * 1000000 + ms * 1000 + us;
			sError = iRadioPlayerUtility->SetVolumeRamp(TTimeIntervalMicroSeconds(total));
		}
		
		// RDS
		else if (setType == KTagAutomaticSwitching)
		{
			sError = iRadioRdsUtility->SetAutomaticSwitching(value);
			if (sError == KErrNone)
			{
				iLog->Log(_L("%S= %d"), &setType, value);
			}
			else
			{
				iLog->Log(_L("Set Automatic Switching failed, return error = %d"), sError);
			}
		}
		
		else 
		{
			iLog->Log(KMsgBadTestParameters);
			sError = KErrBadTestParameter;
		}
	}
   return sError;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::Get()
// Get values.
// Returns: Errors
// -----------------------------------------------------------------------------
//
TInt CRadioUtilityTestClass::Get( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::Get")));
	iLog->Log(_L("CRadioUtilityTestClass::Get!!! "));
	TInt gError = KErrNone;
	TPtrC getType;
	
	if (  gError == aItem.GetNextString(getType) )
	{
		iLog->Log(_L("Get %S"), &getType);
		
		if (getType == KTagSignalStrength)
		{
			TInt signalStrength;
			gError = iRadioFMTunerUtility->GetSignalStrength(signalStrength);
			iLog->Log(_L("%S= %d"), &getType, signalStrength);
		}
		else if (getType == KTagStereo)
		{
			TBool Stereo;
			gError = iRadioFMTunerUtility->GetStereoMode(Stereo);
			iLog->Log(_L("%S= %d"), &getType, Stereo);
		}
		else if (getType == KTagForcedMono)
		{
			TBool value;
			gError = iRadioFMTunerUtility->GetForcedMonoReception(value);
			iLog->Log(_L("%S= %d"), &getType, value);
		}
		else if (getType == KTagSquelch)
		{
			TBool value;
			gError = iRadioFMTunerUtility->GetSquelch(value);
			if (value == 0 || value == 1)
			{
				iLog->Log(_L("%S= %d"), &getType, value);
			}
			else
			{
				iLog->Log(_L("invalid return value %S= %d"), &getType, value);
				gError = KErrUnexpectedValue;
			}
		}
		// PLAYER
		else if (getType == KTagPlayerState)
		{
			TPlayerState aPlayerState;
			aPlayerState = iRadioPlayerUtility->PlayerState();
			iLog->Log(_L("%S= %d"), &getType, aPlayerState);
		}
		else if (getType == KTagVolume)
		{
			TInt volume;
			gError = iRadioPlayerUtility->GetVolume(volume);
			iLog->Log(_L("%S= %d"), &getType, volume);
		} 
		else if (getType == KTagBalance)
		{
			TInt leftBalance, rightBalance;
			gError = iRadioPlayerUtility->GetBalance(leftBalance, rightBalance);
			iLog->Log(_L("%S , LeftBalance=%d, RightBalance=%d"), &getType,leftBalance,rightBalance);
		}

		// RDS
		else if (getType == KTagAutomaticSwitching)
		{
			TBool autoSwitch;
			gError = iRadioRdsUtility->GetAutomaticSwitching(autoSwitch);
			iLog->Log(_L("%S = %d"), &getType, autoSwitch);
		}

		else 
		{
			iLog->Log(KMsgBadTestParameters);
			gError = KErrBadTestParameter;
		}
	}
	
	return gError;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetRdsCapabilities()
// Get RDS capabilities.
//    ERdsProgrammeIdentification | ERdsProgrammeType | ERdsProgrammeService |
//    ERdsRadioText | ERdsRadioTextPlus | ERdsClockTime | ERdsTrafficProgramme |
//    ERdsTrafficAnnouncement | ERdsAlternateFrequency
// Returns: Errors
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetRdsCapabilities( CStifItemParser& /*aItem*/ )
{
	TInt error = KErrNone;
	TRdsCapabilities caps;

	error = iRadioRdsUtility->GetCapabilities(caps);
	if (error == KErrNone)
	{
		if ( caps.iRdsFunctions & ERdsProgrammeIdentification )
		{
			iLog->Log(_L("RDS Capabilities - Program ID is supported!!! "));
		}

		if ( caps.iRdsFunctions & ERdsProgrammeType )
		{
			iLog->Log(_L("RDS Capabilities - Program Type is supported!!! "));
		}

		if ( caps.iRdsFunctions & ERdsProgrammeService )
		{
			iLog->Log(_L("RDS Capabilities - Program Service is supported!!! "));
		}

		if ( caps.iRdsFunctions & ERdsRadioText )
		{
			iLog->Log(_L("RDS Capabilities - Radio Text is supported!!! "));
		}

		if ( caps.iRdsFunctions & ERdsRadioTextPlus )
		{
			iLog->Log(_L("RDS Capabilities - Radio Text+ is supported!!! "));
		}

		if ( caps.iRdsFunctions & ERdsClockTime )
		{
			iLog->Log(_L("RDS Capabilities - Clock Time is supported!!! "));
		}

		if ( caps.iRdsFunctions & ERdsTrafficProgramme )
		{
			iLog->Log(_L("RDS Capabilities - Traffic Program is supported!!! "));
		}

		if ( caps.iRdsFunctions & ERdsTrafficAnnouncement )
		{
			iLog->Log(_L("RDS Capabilities - Traffic Announcement is supported!!! "));
		}

		if ( caps.iRdsFunctions & ERdsAlternateFrequency )
		{
			iLog->Log(_L("RDS Capabilities - Alternate Frequency is supported!!! "));
		}
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetRdsSignalStatus()
// Get RDS signal status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetRdsSignalStatus( CStifItemParser& /*aItem*/ )
{
	TInt error = KErrNone;
	TBool rdsSignal;
	
	error = iRadioRdsUtility->GetRdsSignalStatus(rdsSignal);
	
	if (error == KErrNone)
	{
		iLog->Log(_L("RDS Signal Status: %d"), rdsSignal);
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::NotifyRdsDataChange()
// Get notified when RDS data changes.
// Callbacks: MrroRdsDataPI, MrroRdsDataPTY, MrroRdsDataPS, MrroRdsDataRT,
//            MrroRdsDataCT, MrroRdsDataTA
// Returns: Errors
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::NotifyRdsDataChange( CStifItemParser& aItem )
{
	TInt error = KErrNone;
	TRdsData rdsData;
	TPtrC data;
	TRdsCapabilities caps;

	error = iRadioRdsUtility->GetCapabilities(caps);
	if (error)
	{
		iLog->Log(_L("Unable to get RDS capabilities. Error = %d"), error);
		return error;
	}
	
	rdsData.iRdsFunctions = 0;
	
	// parse parameters and add RDS Data
	while(KErrNone == aItem.GetNextString(data))
	{
		if (data == KTagRdsProgrammeIdentification)
		{
			if ( caps.iRdsFunctions & ERdsProgrammeIdentification )
			{
				rdsData.iRdsFunctions |= ERdsProgrammeIdentification;
				iLog->Log(_L("NotifyRdsDataChange : added %S"), &KTagRdsProgrammeIdentification);
			}
			else
			{
				iLog->Log(_L("%S is not supported."), &KTagRdsProgrammeIdentification);
			}
		}
		else if (data == KTagRdsProgrammeType)
		{
			if ( caps.iRdsFunctions & ERdsProgrammeType )
			{
				rdsData.iRdsFunctions |= ERdsProgrammeType;
				iLog->Log(_L("NotifyRdsDataChange : added %S"), &KTagRdsProgrammeType);
			}
			else
			{
				iLog->Log(_L("%S is not supported."), &KTagRdsProgrammeType);
			}
		}
		else if (data == KTagRdsProgrammeService)
		{
			if ( caps.iRdsFunctions & ERdsProgrammeService )
			{
				rdsData.iRdsFunctions |= ERdsProgrammeService;
				iLog->Log(_L("NotifyRdsDataChange : added %S"), &KTagRdsProgrammeService);
			}
			else
			{
				iLog->Log(_L("%S is not supported."), &KTagRdsProgrammeService);
			}
		}
		else if (data == KTagRdsRadioText)
		{
			if ( caps.iRdsFunctions & ERdsRadioText )
			{
				rdsData.iRdsFunctions |= ERdsRadioText;
				iLog->Log(_L("NotifyRdsDataChange : added %S"), &KTagRdsRadioText);
			}
			else
			{
				iLog->Log(_L("%S is not supported."), &KTagRdsRadioText);
			}
		}
		else if (data == KTagRdsClockTime)
		{
			if ( caps.iRdsFunctions & ERdsClockTime )
			{
				rdsData.iRdsFunctions |= ERdsClockTime;
				iLog->Log(_L("NotifyRdsDataChange : added %S"), &KTagRdsClockTime);
			}
			else
			{
				iLog->Log(_L("%S is not supported."), &KTagRdsClockTime);
			}
		}
		else if (data == KTagRdsTrafficAnnouncement)
		{
			if ( caps.iRdsFunctions & ERdsTrafficAnnouncement )
			{
				rdsData.iRdsFunctions |= ERdsTrafficAnnouncement;
				iLog->Log(_L("NotifyRdsDataChange : added %S"), &KTagRdsTrafficAnnouncement);
			}
			else
			{
				iLog->Log(_L("%S is not supported."), &KTagRdsTrafficAnnouncement);
			}
		}
	}

	error = iRadioRdsUtility->NotifyRdsDataChange(rdsData);
	
	if (error == KErrNone)
	{
		iLog->Log(_L("NotifyRdsDataChange was successful."));
	}
	else
	{
		iLog->Log(_L("NotifyRdsDataChange failed. Error = %d"), error);
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::CancelNotifyRdsDataChange()
// Cancel RDS data notificatin.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::CancelNotifyRdsDataChange( CStifItemParser& /*aItem*/ )
{
	iRadioRdsUtility->CancelNotifyRdsDataChange();
	iLog->Log(_L("CancelNotifyRdsDataChange!!! "));
	return KErrNone;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::CancelAFSearch()
// Cancel AF search.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::CancelAFSearch( CStifItemParser& /*aItem*/ )
{
	iRadioRdsUtility->CancelAFSearch();
	iLog->Log(_L("CancelAFSearch!!! "));
	return KErrNone;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::StationSeekByPTY()
// Station seek by PTY.
// Callbacks: MrroStationSeekByPTYComplete
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::StationSeekByPTY( CStifItemParser& aItem )
{
	TInt error = KErrNone;
	TPtrC pType;
	TPtrC seek;
    TBool seekUp = ETrue;
    TRdsProgrammeType pty = 0;

	error = aItem.GetNextString(pType);
    if (error)
    {
		iLog->Log(_L("Missing Programme Type. Error = %d"), error);
		return error;
    }
    	
	error = aItem.GetNextString(seek);
    if (error)
    {
		iLog->Log(_L("Missing seek direction. Error = %d"), error);
		return error;
    }
	
	if (seek == KTagDown)
	{
		seekUp = EFalse;
	}

	//Rds
	if (pType == KTagRdsPtyNone)
	{
		pty = KRdsPtyNone;
	}
	else if (pType == KTagRdsPtyNews)
	{
		pty = KRdsPtyNews;
	}
	else if (pType == KTagRdsPtyCurrentAffairs)
	{
		pty = KRdsPtyCurrentAffairs;
	}
	else if (pType == KTagRdsPtyInformation)
	{
		pty = KRdsPtyInformation;
	}
	else if (pType == KTagRdsPtySport)
	{
		pty = KRdsPtySport;
	}
	else if (pType == KTagRdsPtyEducation)
	{
		pty = KRdsPtyEducation;
	}
	else if (pType == KTagRdsPtyDrama)
	{
		pty = KRdsPtyDrama;
	}
	else if (pType == KTagRdsPtyCulture)
	{
		pty = KRdsPtyCulture;
	}
	else if (pType == KTagRdsPtyScience)
	{
		pty = KRdsPtyScience;
	}
	else if (pType == KTagRdsPtyVariedSpeech)
	{
		pty = KRdsPtyVariedSpeech;
	}
	else if (pType == KTagRdsPtyPopMusic)
	{
		pty = KRdsPtyPopMusic;
	}
	else if (pType == KTagRdsPtyRockMusic)
	{
		pty = KRdsPtyRockMusic;
	}
	else if (pType == KTagRdsPtyEasyListening)
	{
		pty = KRdsPtyEasyListening;
	}
	else if (pType == KTagRdsPtyLightClassical)
	{
		pty = KRdsPtyLightClassical;
	}
	else if (pType == KTagRdsPtySeriousClassical)
	{
		pty = KRdsPtySeriousClassical;
	}
	else if (pType == KTagRdsPtyOtherMusic)
	{
		pty = KRdsPtyOtherMusic;
	}
	else if (pType == KTagRdsPtyWeather)
	{
		pty = KRdsPtyWeather;
	}
	else if (pType == KTagRdsPtyFinance)
	{
		pty = KRdsPtyFinance;
	}
	else if (pType == KTagRdsPtyChildrensProgrammes)
	{
		pty = KRdsPtyChildrensProgrammes;
	}
	else if (pType == KTagRdsPtySocialAffairs)
	{
		pty = KRdsPtySocialAffairs;
	}
	else if (pType == KTagRdsPtyReligion)
	{
		pty = KRdsPtyReligion;
	}
	else if (pType == KTagRdsPtyPhoneIn)
	{
		pty = KRdsPtyPhoneIn;
	}
	else if (pType == KTagRdsPtyTravel)
	{
		pty = KRdsPtyTravel;
	}
	else if (pType == KTagRdsPtyLeisure)
	{
		pty = KRdsPtyLeisure;
	}
	else if (pType == KTagRdsPtyJazzMusic)
	{
		pty = KRdsPtyJazzMusic;
	}
	else if (pType == KTagRdsPtyCountryMusic)
	{
		pty = KRdsPtyCountryMusic;
	}
	else if (pType == KTagRdsPtyNationalMusic)
	{
		pty = KRdsPtyNationalMusic;
	}
	else if (pType == KTagRdsPtyOldiesMusic)
	{
		pty = KRdsPtyOldiesMusic;
	}
	else if (pType == KTagRdsPtyFolkMusic)
	{
		pty = KRdsPtyFolkMusic;
	}
	else if (pType == KTagRdsPtyDocumentary)
	{
		pty = KRdsPtyDocumentary;
	}
	else if (pType == KTagRdsPtyAlarmTest)
	{
		pty = KRdsPtyAlarmTest;
	}
	else if (pType == KTagRdsPtyAlarm)
	{
		pty = KRdsPtyAlarm;
	}
	//Rbds
	else if (pType == KTagRbdsPtyNone)
	{
		pty = KRbdsPtyNone;
	}
	else if (pType == KTagRbdsPtyNews)
	{
		pty = KRbdsPtyNews;
	}
	else if (pType == KTagRbdsPtyInformation)
	{
		pty = KRbdsPtyInformation;
	}
	else if (pType == KTagRbdsPtySports)
	{
		pty = KRbdsPtySports;
	}
	else if (pType == KTagRbdsPtyTalk)
	{
		pty = KRbdsPtyTalk;
	}
	else if (pType == KTagRbdsPtyRock)
	{
		pty = KRbdsPtyRock;
	}
	else if (pType == KTagRbdsPtyClassicRock)
	{
		pty = KRbdsPtyClassicRock;
	}
	else if (pType == KTagRbdsPtyAdultHits)
	{
		pty = KRbdsPtyAdultHits;
	}
	else if (pType == KTagRbdsPtySoftRock)
	{
		pty = KRbdsPtySoftRock;
	}
	else if (pType == KTagRbdsPtyTop40)
	{
		pty = KRbdsPtyTop40;
	}
	else if (pType == KTagRbdsPtyCountry)
	{
		pty = KRbdsPtyCountry;
	}
	else if (pType == KTagRbdsPtyOldies)
	{
		pty = KRbdsPtyOldies;
	}
	else if (pType == KTagRbdsPtySoft)
	{
		pty = KRbdsPtySoft;
	}
	else if (pType == KTagRbdsPtyNostalgia)
	{
		pty = KRbdsPtyNostalgia;
	}
	else if (pType == KTagRbdsPtyJazz)
	{
		pty = KRbdsPtyJazz;
	}
	else if (pType == KTagRbdsPtyClassical)
	{
		pty = KRbdsPtyClassical;
	}
	else if (pType == KTagRbdsPtyRhythmAndBlues)
	{
		pty = KRbdsPtyRhythmAndBlues;
	}
	else if (pType == KTagRbdsPtySoftRhythmAndBlues)
	{
		pty = KRbdsPtySoftRhythmAndBlues;
	}
	else if (pType == KTagRbdsPtyLanguage)
	{
		pty = KRbdsPtyLanguage;
	}
	else if (pType == KTagRbdsPtyReligiousMusic)
	{
		pty = KRbdsPtyReligiousMusic;
	}
	else if (pType == KTagRbdsPtyReligiousTalk)
	{
		pty = KRbdsPtyReligiousTalk;
	}
	else if (pType == KTagRbdsPtyPersonality)
	{
		pty = KRbdsPtyPersonality;
	}
	else if (pType == KTagRbdsPtyPublic)
	{
		pty = KRbdsPtyPublic;
	}
	else if (pType == KTagRbdsPtyCollege)
	{
		pty = KRbdsPtyCollege;
	}
	else if (pType == KTagRbdsPtyUnassigned1)
	{
		pty = KRbdsPtyUnassigned1;
	}
	else if (pType == KTagRbdsPtyUnassigned2)
	{
		pty = KRbdsPtyUnassigned2;
	}
	else if (pType == KTagRbdsPtyUnassigned3)
	{
		pty = KRbdsPtyUnassigned3;
	}
	else if (pType == KTagRbdsPtyUnassigned4)
	{
		pty = KRbdsPtyUnassigned4;
	}
	else if (pType == KTagRbdsPtyUnassigned5)
	{
		pty = KRbdsPtyUnassigned5;
	}
	else if (pType == KTagRbdsPtyWeather)
	{
		pty = KRbdsPtyWeather;
	}
	else if (pType == KTagRbdsPtyEmergencyTest)
	{
		pty = KRbdsPtyEmergencyTest;
	}
	else if (pType == KTagRbdsPtyEmergency)
	{
		pty = KRbdsPtyEmergency;
	}
	
	iRadioRdsUtility->StationSeekByPTY(pty, seekUp);
	
	iLog->Log(_L("StationSeekByPTY: pty = %S; seek = %S"), &pType, &seek);
	AddExpectedEvent(EMrroStationSeekByPTYComplete, KShortTimeout);
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::StationSeekByTA()
// Station seek by TA.
// Callbacks: MrroStationSeekByTAComplete
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::StationSeekByTA( CStifItemParser& aItem )
{
	TInt error = KErrNone;
	TPtrC seek;
    TBool seekUp = ETrue;

	error = aItem.GetNextString(seek);
    if (error)
    {
		iLog->Log(_L("Missing seek direction. Error = %d"), error);
		return error;
    }
	
	if (seek == KTagDown)
	{
		seekUp = EFalse;
	}
	
	iRadioRdsUtility->StationSeekByTA(seekUp);
	
	iLog->Log(_L("StationSeekByTA: seek = %S"), &seek);
	AddExpectedEvent(EMrroStationSeekByTAComplete, KMediumTimeout);
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::StationSeekByTP()
// Station seek by TP.
// Callbacks: MrroStationSeekByTPComplete
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::StationSeekByTP( CStifItemParser& aItem )
{
	TInt error = KErrNone;
	TPtrC seek;
    TBool seekUp = ETrue;

	error = aItem.GetNextString(seek);
    if (error)
    {
		iLog->Log(_L("Missing seek direction. Error = %d"), error);
		return error;
    }
	
	if (seek == KTagDown)
	{
		seekUp = EFalse;
	}
	
	iRadioRdsUtility->StationSeekByTP(seekUp);
	
	iLog->Log(_L("StationSeekByTP: seek = %S"), &seek);
	AddExpectedEvent(EMrroStationSeekByTPComplete, KMediumTimeout);
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetProgrammeIdentification()
// Get program ID.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetProgrammeIdentification( CStifItemParser& /*aItem*/ )
{
	TInt error = KErrNone;
	TInt pi;
	
	error = iRadioRdsUtility->GetProgrammeIdentification(pi);
	
	if (error == KErrNotFound)
	{
		iLog->Log(_L("GetProgrammeIdentification: No data found."));
		error = KErrNone;
	}
	else if (error)
	{
		iLog->Log(_L("GetProgrammeIdentification FAILED: error = %d"), error);
	}
	else
	{
		iLog->Log(_L("GetProgrammeIdentification: PI = %d"), pi);
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetProgrammeType()
// Get program type.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetProgrammeType( CStifItemParser& /*aItem*/ )
{
	TInt error = KErrNone;
	TRdsProgrammeType pty;
	
	error = iRadioRdsUtility->GetProgrammeType(pty);
	
	if (error == KErrNotFound)
	{
		iLog->Log(_L("GetProgrammeType: No data found."));
		error = KErrNone;
	}
	else if (error)
	{
		iLog->Log(_L("GetProgrammeType FAILED: error = %d"), error);
	}
	else
	{
		iLog->Log(_L("GetProgrammeType: Type = %d"), pty);
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetProgrammeService()
// Get program Service.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetProgrammeService( CStifItemParser& /*aItem*/ )
{
	TInt error = KErrNone;
	TRdsPSName ps;
	
	error = iRadioRdsUtility->GetProgrammeService(ps);
	
	if (error == KErrNotFound)
	{
		iLog->Log(_L("GetProgrammeService: No data found."));
		error = KErrNone;
	}
	else if (error)
	{
		iLog->Log(_L("GetProgrammeService FAILED: error = %d"), error);
	}
	else
	{
		iLog->Log(_L("GetProgrammeService: Name = %S"), &ps);
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetRadioText()
// Get radio text.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetRadioText( CStifItemParser& /*aItem*/ )
{
	TInt error = KErrNone;
	TRdsRadioText rt;
	
	error = iRadioRdsUtility->GetRadioText(rt);
	
	if (error == KErrNotFound)
	{
		iLog->Log(_L("GetRadioText: No data found."));
		error = KErrNone;
	}
	else if (error)
	{
		iLog->Log(_L("GetRadioText FAILED: error = %d"), error);
	}
	else
	{
		iLog->Log(_L("GetRadioText: Text = %S"), &rt);
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetClockTime()
// Get clock time.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetClockTime( CStifItemParser& /*aItem*/ )
{
	TInt error = KErrNone;
	TDateTime dt;
	
	error = iRadioRdsUtility->GetClockTime(dt);
	
	if (error == KErrNotFound)
	{
		iLog->Log(_L("GetClockTime: No data found."));
		error = KErrNone;
	}
	else if (error)
	{
		iLog->Log(_L("GetClockTime FAILED: error = %d"), error);
	}
	else
	{
		iLog->Log(_L("GetClockTime: %d/%d/%d %d:%d:%d"), dt.Month(), dt.Day(), dt.Year(),
		              dt.Hour(), dt.Minute(), dt.Second());
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetTrafficAnnouncementStatus()
// Get traffic announcement status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetTrafficAnnouncementStatus( CStifItemParser& /*aItem*/ )
{
	TInt error = KErrNone;
	TBool taStatus;
	
	error = iRadioRdsUtility->GetTrafficAnnouncementStatus(taStatus);

	if (error)
	{
		iLog->Log(_L("GetTrafficAnnouncementStatus FAILED: error = %d"), error);
	}
	else
	{
		iLog->Log(_L("GetTrafficAnnouncementStatus: Status = %d"), taStatus);
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetTrafficProgrammeStatus()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetTrafficProgrammeStatus( CStifItemParser& /*aItem*/ )
{
	TInt error = KErrNone;
	TBool tpStatus;
	
	error = iRadioRdsUtility->GetTrafficProgrammeStatus(tpStatus);

	if (error)
	{
		iLog->Log(_L("GetTrafficProgrammeStatus FAILED: error = %d"), error);
	}
	else
	{
		iLog->Log(_L("GetTrafficProgrammeStatus: Status = %d"), tpStatus);
	}
	
	return error;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::SetPreset()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::SetPreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::SetPreset")));
    iLog->Log(_L("CRadioUtilityTestClass::SetPreset"));
    TInt err=KErrNone;
    TInt index;
    TPtrC name;
    TInt freq;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index."));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextString(name) )
    {
        iLog->Log(_L("Missing Parameter: Preset name."));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(freq) )
    {
        iLog->Log(_L("Missing Parameter: Preset frequency."));
        err = KErrBadTestParameter;
    }
    
    TRAP(err, iFmPresetUtility->SetPresetL(index, name, freq));
    iLog->Log(_L("SetPreset: Index=%d, Name=%S, Freq=%d"), index, &name, freq);
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetPreset()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetPreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::GetPreset")));
    iLog->Log(_L("CRadioUtilityTestClass::GetPreset"));
    TInt err = KErrNone;
    TInt index;
    TPtrC expectedName;
    TInt expectedFreq;
    TFmPresetName name;
    TInt freq;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index."));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextString(expectedName) )
    {
        iLog->Log(_L("Missing Parameter: Expected preset name."));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(expectedFreq) )
    {
        iLog->Log(_L("Missing Parameter: Expected preset frequency."));
        err = KErrBadTestParameter;
    }
    
    TRAP(err, iFmPresetUtility->GetPresetL(index, name, freq));
    iLog->Log(_L("GetPreset: Index=%d, Name=%S, Freq=%d"), index, &name, freq);
    if ( !err )
    {
        // Verification
        if ( name != expectedName )
        {
            iLog->Log(_L("Failed: name=%S, expectedName=%S."), &name, &expectedName);
            err = KErrUnexpectedValue;
        }
        else if ( freq != expectedFreq )
        {
            iLog->Log(_L("Failed: freq=%d, expectedFreq=%d."), freq, expectedFreq);
            err = KErrUnexpectedValue;
        }
    }
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetMaxNumberOfPresets()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetMaxNumberOfPresets( CStifItemParser& /*aItem*/ )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::GetMaxNumberOfPresets")));
    iLog->Log(_L("CRadioUtilityTestClass::GetMaxNumberOfPresets"));
    TInt err=KErrNone;
    TInt maxPreset;
    
    iFmPresetUtility->GetMaxNumberOfPresets(maxPreset);
    iLog->Log(_L("GetMaxNumberOfPresets: maxPreset=%d"), maxPreset);
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetNumberOfPresets()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetNumberOfPresets( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::GetNumberOfPresets")));
    iLog->Log(_L("CRadioUtilityTestClass::GetNumberOfPresets"));
    TInt err=KErrNone;
    TInt numPresets;
    TInt expectedNumPresets;
    
    // read parameters
    if ( aItem.GetNextInt(expectedNumPresets) )
    {
        iLog->Log(_L("Missing Parameter: Expected number of presets."));
        err = KErrBadTestParameter;
    }

    iFmPresetUtility->GetNumberOfPresets(numPresets);
    iLog->Log(_L("GetMaxNumberOfPresets: numPresets=%d"), numPresets);
    // Verification
    if ( numPresets != expectedNumPresets )
    {
        iLog->Log(_L("Failed: numPresets=%d, expectedNumPresets=%d."), numPresets, expectedNumPresets);
        err = KErrUnexpectedValue;
    }
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetFirstPresetL()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetFirstPresetL( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::GetFirstPresetL")));
    iLog->Log(_L("CRadioUtilityTestClass::GetFirstPresetL"));
    TInt err = KErrNone;
    TInt index;
    TInt expectedIndex;
    
    // read parameters
    if ( aItem.GetNextInt(expectedIndex) )
    {
        iLog->Log(_L("Missing Parameter: Expected preset index."));
        err = KErrBadTestParameter;
    }

    iFmPresetUtility->GetFirstPresetL(index);
    iLog->Log(_L("GetFirstPresetL: index=%d"), index);
    // Verification
    if ( index != expectedIndex )
    {
        iLog->Log(_L("Failed: index=%d, expectedIndex=%d."), index, expectedIndex);
        err = KErrUnexpectedValue;
    }
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetNextPresetL()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetNextPresetL( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::GetNextPresetL")));
    iLog->Log(_L("CRadioUtilityTestClass::GetNextPresetL"));
    TInt err=KErrNone;
    TInt index;
    TInt nextIndex;
    TInt expectedNextIndex;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Current preset index."));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(expectedNextIndex) )
    {
        iLog->Log(_L("Missing Parameter: Expected next preset index."));
        err = KErrBadTestParameter;
    }

    iFmPresetUtility->GetNextPresetL(index, nextIndex);
    iLog->Log(_L("GetNextPresetL: nextIndex=%d"), nextIndex);
    // Verification
    if ( nextIndex != expectedNextIndex )
    {
        iLog->Log(_L("Failed: nextIndex=%d, expectedNextIndex=%d."), nextIndex, expectedNextIndex);
        err = KErrUnexpectedValue;
    }
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::DeletePreset()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::DeletePreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::DeletePreset")));
    iLog->Log(_L("CRadioUtilityTestClass::DeletePreset"));
    TInt err=KErrNone;
    TInt index;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index to delete."));
        err = KErrBadTestParameter;
    }

    TRAP( err, iFmPresetUtility->DeletePresetL(index));
    iLog->Log(_L("DeletePresetL: index=%d"), index);
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::SetUrlToPreset()
// Set Url to Preset
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::SetUrlToPreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::SetUrlToPreset")));
    iLog->Log(_L("CRadioUtilityTestClass::SetUrlToPreset"));
    TInt err=KErrNone;
    TInt index;
    TPtrC url;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index to store Url to"));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextString(url) )
    {
        iLog->Log(_L("Missing Parameter: Url"));
        err = KErrBadTestParameter;
    }
  
    TRAP( err, iFmPresetUtility->SetPresetUrlL( index, url ); )            
    
    iLog->Log(_L("Set Url To PresetL: index=%d"), index);
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetUrlFromPreset()
// Set Url to Preset
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetUrlFromPreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::GetUrlFromPreset")));
    iLog->Log(_L("CRadioUtilityTestClass::GetUrlFromPreset"));
    TInt err=KErrNone;
    TInt index;
    TFmPresetUrl storedUrl;
    TPtrC url;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index where to retrieve Url from"));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextString(url) )
    {
        iLog->Log(_L("Missing Parameter: Url"));
        err = KErrBadTestParameter;
    }
  
    TRAP( err, iFmPresetUtility->GetPresetUrlL( index, storedUrl );  )          
    
    if ( storedUrl != url )
        err = KErrUnexpectedValue;
    
    iLog->Log(_L("Get Url From PresetL: index=%d"), index);
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetFavouriteInfoFromPreset()
// Set Url to Preset
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetFavouriteInfoFromPreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::GetFavouriteInfoFromPreset")));
    iLog->Log(_L("CRadioUtilityTestClass::GetFavouriteInfoFromPreset"));
    TInt err=KErrNone;
    TInt index, favourite;
    TBool storedFavouriteBit = EFalse;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index where to retrieve favourite info"));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(favourite) )
    {
        iLog->Log(_L("Missing Parameter: Favourite value to test"));
        err = KErrBadTestParameter;
    }
    TRAP( err, iFmPresetUtility->GetPresetFavouriteInfoL( index, storedFavouriteBit );  )          
    
    if ( (favourite && storedFavouriteBit) || (!favourite && !storedFavouriteBit) )
        err = KErrNone;
    else
        err = KErrUnexpectedValue;
    
    iLog->Log(_L("Get Favourite info From PresetL: index=%d"), index);
    return err;
}
// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::SetFavouriteInfoToPreset()
// Set favourite to preset
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::SetFavouriteInfoToPreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::GetUrlFromPreset")));
    iLog->Log(_L("CRadioUtilityTestClass::GetUrlFromPreset"));
    TInt err=KErrNone;
    TInt index, favourite;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index where to retrieve Url from"));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(favourite) )
    {
        iLog->Log(_L("Missing Parameter: Favourite value to set"));
        err = KErrBadTestParameter;
    }
    TRAP( err, iFmPresetUtility->SetPresetFavouriteInfoL( index, favourite > 0 );  )          
   
    iLog->Log(_L("Set favourite info to PresetL: index=%d, favourite value=%d"), index, favourite);
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::SetPtyToPreset()
// Set Url to Preset
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::SetPtyToPreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::SetPtyToPreset")));
    iLog->Log(_L("CRadioUtilityTestClass::SetPtyToPreset"));
    TInt err=KErrNone;
    TInt index, pty;
    TPtrC url;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index to Programme Type to"));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(pty) )
    {
        iLog->Log(_L("Missing Parameter: Programme Type"));
        err = KErrBadTestParameter;
    }
  
    TRAP( err, iFmPresetUtility->SetPresetPtyL( index, pty ); )            
    
    iLog->Log(_L("Set PTY To PresetL: index=%d, PTY=%d"), index, pty);
    return err;
}
// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetPtyFromPreset()
// Get Programme Type stored to Preset
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetPtyFromPreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::GetPtyFromPreset")));
    iLog->Log(_L("CRadioUtilityTestClass::GetPtyFromPreset"));
    TInt err=KErrNone;
    TInt index, pty;
    TUint storedpty = 0;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index where to retrieve PTY"));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(pty) )
    {
        iLog->Log(_L("Missing Parameter: Programme Type value to test"));
        err = KErrBadTestParameter;
    }
    TRAP( err, iFmPresetUtility->GetPresetPtyL( index, storedpty );  )          
    if (storedpty != pty)
        err = KErrUnexpectedValue;
    
    iLog->Log(_L("Get Programme Type (PTY) From PresetL: index=%d, PTY=%d"), index, pty);
    return err;
}

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::SetPIToPreset()
// Set Url to Preset
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::SetPIToPreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::SetPIToPreset")));
    iLog->Log(_L("CRadioUtilityTestClass::SetPIToPreset"));
    TInt err=KErrNone;
    TInt index, pi;
    TPtrC url;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index to Programme Type to"));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(pi) )
    {
        iLog->Log(_L("Missing Parameter: Programme Type"));
        err = KErrBadTestParameter;
    }
  
    TRAP( err, iFmPresetUtility->SetPresetPICodeL( index, pi ); )            
    
    iLog->Log(_L("Set PI To PresetL: index=%d, PI=%d"), index, pi);
    return err;
}
// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::GetPIFromPreset()
// Get Programme Type stored to Preset
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CRadioUtilityTestClass::GetPIFromPreset( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::GetPIFromPreset")));
    iLog->Log(_L("CRadioUtilityTestClass::GetPIFromPreset"));
    TInt err=KErrNone;
    TInt index, pi;
    TUint storedpi = 0;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        iLog->Log(_L("Missing Parameter: Preset index where to retrieve PI"));
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(pi) )
    {
        iLog->Log(_L("Missing Parameter: Programme Type value to test"));
        err = KErrBadTestParameter;
    }
    TRAP( err, iFmPresetUtility->GetPresetPICodeL( index, storedpi );  )          
    if (storedpi != pi)
        err = KErrUnexpectedValue;
    
    iLog->Log(_L("Get Programme Type (PI) From PresetL: index=%d, PI=%d"), index, pi);
    return err;
}



    /**
     * Retrieves a custom interface to the device. (Used by Audio Enhancement)
     *
     * @since S60 3.2
     * @param aInterfaceId The interface UID, defined with the custom interface.
     * @return A pointer to the interface implementation, or NULL if the device does not
     * implement the interface requested. The return value must be cast to the correct
     * type by the user.
     */

//TInt CRadioUtilityTestClass::AudioEnhancement( CStifItemParser& aItem )
//{

//    IMPORT_C TAny* CustomInterface( TUid aInterfaceId );

//}

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  End of File
