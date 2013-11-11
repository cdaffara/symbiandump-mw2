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
* Description: Audio Output Control Utility
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "AudioOutputControlTestClass.h"
#include "debug.h"

//using namespace multimedia;

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
// ?function_name .
//
// Returns: ?value_1:
//          ?value_n: _line1
//                    _line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  //
    ?arg_type arg)  //
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAudioOutputControlTestClass::Delete
// Delete here all resources allocated and opened from test methods.
// Called from destructor.
// -----------------------------------------------------------------------------
//
void CAudioOutputControlTestClass::Delete()
{

    FTRACE(FPrint(_L("CAudioOutputControlTestClass::Delete")));

    iExpectedEvents.Close();
    iOcurredEvents.Close();

    iLog->Log(_L("Deleting test class..."));
	iLog->Log(_L(""));
	iLog->Log(_L(""));


	if (iAudioOutputControlUtility)
		{
		delete iAudioOutputControlUtility;
		}
	if (iFactory)
			{
			delete iFactory;
			}

	if (iDevSound)
		{
		delete iDevSound;
		}
	REComSession::FinalClose();
}

// -----------------------------------------------------------------------------
// CAudioOutputControlTestClass::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CAudioOutputControlTestClass::RunMethodL(
    CStifItemParser& aItem )
    {

    static TStifFunctionInfo const KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        ENTRY( "ConfigureAudioOutput", CAudioOutputControlTestClass::ConfigureAudioOutput ),
        ENTRY( "CreateObject", CAudioOutputControlTestClass::CreateObject ),
        ENTRY( "SetDRMConfigIntfc", CAudioOutputControlTestClass::SetDRMConfigIntfc ),
        ENTRY( "CreateRestrictedAudioOutput", CAudioOutputControlTestClass::CreateRestrictedAudioOutput ),
        ENTRY( "SetDRMConfig", CAudioOutputControlTestClass::SetDRMConfig ),


        };

    const TInt count = sizeof( KFunctions ) /
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


// -----------------------------------------------------------------------------
// CAudioOutputControlTestClass::EventName
// Return descriptor with the notification description
// -----------------------------------------------------------------------------
TPtrC CAudioOutputControlTestClass::EventName( TInt aKey )
{
	static TText* const badKeyword = (TText*)L"BadKeyword";
	static TText* const keywords[] =
	{

		(TText*)L"EInitializeComplete",

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
// CAudioOutputControlTestClass::AddExpectedEvent
// Add an event to the expected events' list
// -----------------------------------------------------------------------------
void CAudioOutputControlTestClass::AddExpectedEvent(TG711ExpectedEvent event, TInt ms)
{
	FTRACE(FPrint(_L("CAudioOutputControlTestClass::AddExpectedEvent")));
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
// CAudioOutputControlTestClass::RemoveExpectedEvent
// Remove the indicated event from the expected events' list
// -----------------------------------------------------------------------------
TBool CAudioOutputControlTestClass::RemoveExpectedEvent(TG711ExpectedEvent aEvent)
{
	FTRACE(FPrint(_L("CAudioOutputControlTestClass::RemoveExpectedEvent")));
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
// CAudioOutputControlTestClass::RemoveAllExpectedEvents
// Remove the indicated event from the expected events' list
// -----------------------------------------------------------------------------
void CAudioOutputControlTestClass::RemoveAllExpectedEvents()
{
	FTRACE(FPrint(_L("CAudioOutputControlTestClass::RemoveAllExpectedEvents")));
	iLog->Log(_L("Removing all expected events"));

	iExpectedEvents.Reset();
	iOcurredEvents.Reset();
}


// -----------------------------------------------------------------------------
// CAudioOutputControlTestClass::ProcessEvent
// -----------------------------------------------------------------------------
void CAudioOutputControlTestClass::ProcessEvent(TG711ExpectedEvent aEvent, TInt aError)
{
	FTRACE(FPrint(_L("CAudioOutputControlTestClass::ProcessExpectedEvent")));
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
// CAudioOutputControlTestClass::HandleTimeout
// Review if all the expected events have ocurred once the time is over
// -----------------------------------------------------------------------------

void CAudioOutputControlTestClass::HandleTimeout(TInt aError)
{
	FTRACE(FPrint(_L("CAudioOutputControlTestClass::HandleTimeout")));
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
// CAudioOutputControlTestClass::SetTimeout
// Create a timer and set a timeout
// When the timeout is reached the test case is marked as failed
// It's used rather than the "timeout" keyword in the configuration file
// because in this way the log continues
// -----------------------------------------------------------------------------
TInt CAudioOutputControlTestClass::SetTimeout( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CAudioOutputControlTestClass::SetTimeout")));
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
TInt CAudioOutputControlTestClass::SetAllowedPanic( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CAudioOutputControlTestClass::SetAllowedPanic")));
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
// CAudioOutputControlTestClass::SetExpectedEvents()
// -----------------------------------------------------------------------------
TInt CAudioOutputControlTestClass::SetExpectedEvents( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CAudioOutputControlTestClass::SetExpectedEvents")));
	TInt error = KErrNone;
	TInt event=0;
	while ( KErrNone == aItem.GetNextInt(event))
	{
		AddExpectedEvent(static_cast<TG711ExpectedEvent>(event), 0); // Default timeout value
	}
	return error;
}


// -----------------------------------------------------------------------------
// CAudioOutputControlTestClass::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAudioOutputControlTestClass::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KAudioOutputControlTestClass, "AudioOutputControlTestClass" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KAudioOutputControlTestClass, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KAudioOutputControlTestClass,
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CG711DecoderIntfcTestClass::CreateDevSoundInstance
// Create Factory
// -----------------------------------------------------------------------------
//

TInt CAudioOutputControlTestClass::CreateObject( CStifItemParser& aItem )
	{
	TPtrC object;
	TBuf<100> printBuf;
	aItem.GetNextString(object);
	printBuf.Copy(object);
	FTRACE(FPrint(_L("CAudioOutputControlTestClass::CreateObject [%S]\n"),&printBuf));
	iLog->Log(_L("CAudioOutputControlTestClass::CreateObject %S"),&printBuf);
	TInt error = KErrNone;
	if (object == _L("Factory"))
		{
		TRAP(error,CConfigurationComponentsFactory::CreateFactoryL(iFactory));
		}
	else if (object == _L("OutputControl"))
		{
		error = iFactory->CreateAudioOutputControlUtility(iAudioOutputControlUtility);
		}
	else if (object == _L("DevSound"))
		{
	    TRAP(error, iDevSound = CMMFDevSound::NewL());
		}

	if(error == KErrNone)
		{
		AddExpectedEvent(EInitializeComplete, KMediumTimeout);
		}
	else
		{
		iLog->Log(_L("CAudioOutputControlTestClass::CreateObject Error [%S]"),&printBuf);
		}

	return error;
	}

// -----------------------------------------------------------------------------
// CAudioOutputControlTestClass::CreateRestrictedAO
// Create RestrictedAO
// -----------------------------------------------------------------------------
//
TInt CAudioOutputControlTestClass::ConfigureAudioOutput( CStifItemParser& aItem )
{
	TPtrC object;
	TInt error = KErrNone;
	//aItem.GetNextString(object);
	//TRAP(error,CConfigurationComponentsFactory::CreateFactoryL(iFactory));
	//TRAP(error, iAudioOutputControlUtility->Configure(*iDevSound));
	error = iAudioOutputControlUtility->Configure(*iDevSound);
    if(error == KErrNone)
        {
    	FTRACE(FPrint(_L("CAudioOutputControlTestClass::ConfigureAudioOutput ")));
    	iLog->Log(_L("CAudioOutputControlTestClass::ConfigureAudioOutput "));
		}
	else
		{
    	FTRACE(FPrint(_L("CAudioOutputControlTestClass::ConfigureAudioOutput Error [%d]"),error));
    	iLog->Log(_L("CAudioOutputControlTestClass::ConfigureAudioOutput Error [%d]"),error);

		}

	return error;
}

TInt CAudioOutputControlTestClass::CreateRestrictedAudioOutput( CStifItemParser& aItem )
{
	TPtrC object;
	TInt error = KErrNone;
	//aItem.GetNextString(object);
	//TRAP(error,CConfigurationComponentsFactory::CreateFactoryL(iFactory));
	//TRAP(error, iAudioOutputControlUtility->Configure(*iDevSound));
	TRAP(error, iFactory->CreateRestrictedAudioOutput(*iDevSound,iRestrictedAudioOutput));
    if(error == KErrNone)
        {
    	FTRACE(FPrint(_L("CAudioOutputControlTestClass::CreateRestrictedAudioOutput ")));
    	iLog->Log(_L("CAudioOutputControlTestClass::CreateRestrictedAudioOutput "));
		}
	else
		{
    	FTRACE(FPrint(_L("CAudioOutputControlTestClass::CreateRestrictedAudioOutput Error [%d]"),error));
    	iLog->Log(_L("CAudioOutputControlTestClass::CreateRestrictedAudioOutput Error [%d]"),error);

		}

	return error;
}

TInt CAudioOutputControlTestClass::SetDRMConfig( CStifItemParser& aItem )
{
	TPtrC object;
	TInt error = KErrNone;
	TInt drmType;
	aItem.GetNextInt(drmType);
	/* ENone,
	   EOMA1,
	   EOMA2,
	   EWMDRM*/

	if (iDRMConfigIntfc)
		{
		iDRMConfigIntfc->SetDRMType(TDRMType(drmType));
		iDRMConfigIntfc->Commit();
		TInt allowdevice = 0;
		iDRMConfigIntfc->GetAllowedOutputDeviceCount(allowdevice);
    	FTRACE(FPrint(_L("GetAllowedOutputDeviceCount [%d]"),allowdevice));
    	iLog->Log(_L("GetAllowedOutputDeviceCount [%d]"),allowdevice);
		TDRMAllowedOutputDevice output;
		output = TDRMAllowedOutputDevice(0);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
		output = TDRMAllowedOutputDevice(1);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
		output = TDRMAllowedOutputDevice(2);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
		output = TDRMAllowedOutputDevice(3);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
		output = TDRMAllowedOutputDevice(4);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
		output = TDRMAllowedOutputDevice(5);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
		output = TDRMAllowedOutputDevice(6);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
		output = TDRMAllowedOutputDevice(7);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
		output = TDRMAllowedOutputDevice(8);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
    	//EAudioAllowHDMI added as part of RIM CR 417-7642: HDMI with HDCP to Resctricted Audio Output API
    	output = TDRMAllowedOutputDevice(11);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
    	//EAudioAllowHdmiHdcpRequired added as part of RIM CR 417-7642: HDMI with HDCP to Resctricted Audio Output API
    	output = TDRMAllowedOutputDevice(12);
    	iDRMConfigIntfc->AppendAllowedOutputDevice(output);
		iDRMConfigIntfc->Commit();
		iDRMConfigIntfc->GetAllowedOutputDeviceCount(allowdevice);
    	FTRACE(FPrint(_L("GetAllowedOutputDeviceCount [%d]"),allowdevice));
    	iLog->Log(_L("GetAllowedOutputDeviceCount [%d]"),allowdevice);

		}
	error = iAudioOutputControlUtility->Configure(*iDevSound);

    if(error == KErrNone)
        {
    	FTRACE(FPrint(_L("CAudioOutputControlTestClass::SetDRMConfig ")));
    	iLog->Log(_L("CAudioOutputControlTestClass::SetDRMConfig "));
		}
	else
		{
    	FTRACE(FPrint(_L("CAudioOutputControlTestClass::SetDRMConfig Error [%d]"),error));
    	iLog->Log(_L("CAudioOutputControlTestClass::SetDRMConfig Error [%d]"),error);

		}

	return error;
}

TInt CAudioOutputControlTestClass::CalcPref( CStifItemParser& aItem )
{
	TPtrC object;
	TInt error = KErrNone;
	TInt drmType;
	aItem.GetNextInt(drmType);
	/* ENone,
	   EOMA1,
	   EOMA2,
	   EWMDRM*/

	if (iDRMConfigIntfc)
		{
		iDRMConfigIntfc->SetDRMType(TDRMType(drmType));
		}
	error = iAudioOutputControlUtility->Configure(*iDevSound);
    if(error == KErrNone)
        {
    	FTRACE(FPrint(_L("CAudioOutputControlTestClass::SetDRMConfig ")));
    	iLog->Log(_L("CAudioOutputControlTestClass::SetDRMConfig "));
		}
	else
		{
    	FTRACE(FPrint(_L("CAudioOutputControlTestClass::SetDRMConfig Error [%d]"),error));
    	iLog->Log(_L("CAudioOutputControlTestClass::SetDRMConfig Error [%d]"),error);

		}

	return error;
}

TInt CAudioOutputControlTestClass::SetDRMConfigIntfc( CStifItemParser& aItem )
{
	TPtrC object;
	TInt error = KErrNone;
	TInt err = CMultimediaFactory::CreateFactory(iFactorys);
	if(error == KErrNone)
        {
        iLog->Log(_L("CMultimediaFactory::CreateFactory OK"));
        }
    else
        {
        iLog->Log(_L("CMultimediaFactory::CreateFactory [%d]"),error);
        }

	error = iFactorys->CreateSourceControl( KDataBufferSourceControl, iSourceControl );
    if(error == KErrNone)
        {
        iMDataBufferSource = static_cast<MDataBufferSource*>(iSourceControl);
        }
	if(error != KErrNone)
        {
        iLog->Log(_L("CreateSourceControl [%d]"),error);
        }

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

	error = iAudioOutputControlUtility->SetDRMConfigIntfc(iDRMConfigIntfc);

    if(error == KErrNone)
        {
    	FTRACE(FPrint(_L("CAudioOutputControlTestClass::SetDRMConfigIntfc ")));
    	iLog->Log(_L("CAudioOutputControlTestClass::SetDRMConfigIntfc "));
		}
	else
		{
    	FTRACE(FPrint(_L("CAudioOutputControlTestClass::SetDRMConfigIntfc Error [%d]"),error));
    	iLog->Log(_L("CAudioOutputControlTestClass::SetDRMConfigIntfc Error [%d]"),error);

		}

	return error;
}

void CAudioOutputControlTestClass::SourceMenuResetEOFFlag()
    {
    iIsEOFReached = EFalse;
    iBytesReadFromFile = 0;

    TInt seekPos(0);
    iFile.Seek( ESeekStart, seekPos );
    }

//  End of File
