/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Call Audio Control
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "CallAudioControlTestClass.h"
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
// CCallAudioControlTestClass::Delete
// Delete here all resources allocated and opened from test methods.
// Called from destructor.
// -----------------------------------------------------------------------------
//
void CCallAudioControlTestClass::Delete()
{

    FTRACE(FPrint(_L("CCallAudioControlTestClass::Delete")));

    iExpectedEvents.Close();
    iOcurredEvents.Close();

    iLog->Log(_L("Deleting test class..."));
	iLog->Log(_L(""));
	iLog->Log(_L(""));

	delete iCallAudioControl;
/*
	iBitratesArray.Reset();

    delete iG711DecFormatIntfc;
    delete iG711EncFormatIntfc;
    delete iG729EncFormatIntfc;
    delete iG729DecFormatIntfc;
    delete iILBCEncFormatIntfc;
    delete iILBCDecFormatIntfc;
    delete iAMRNBEncFormatIntfc;
    delete iAMRNBDecFormatIntfc;
    delete iPCM16EncFormatIntfc;
    delete iPCM16DecFormatIntfc;

    delete iRingTonePlayer;
    delete iRingToneFactory;
    delete iDTMFTonePlayerDn;
    delete iDTMFTonePlayerUp;

    delete iPlayBuf;

#ifdef __JITTER_BUFFER_TEST__
    delete iJBIntfc;
    delete iPlayJBuf;
#endif //__JITTER_BUFFER_TEST__

    if (iVoIPDnlink )
    	{
        if (iDnLinkStatus != ENotReady)
        	{
            iVoIPDnlink->Close();
        	}
        delete iVoIPDnlink;
        iVoIPDnlink = NULL;
    	}

    if (iVoIPUplink )
    	{
        if (iUpLinkStatus != ENotReady)
        	{
            iVoIPUplink->Close();
        	}
        delete iVoIPUplink;
        iVoIPUplink = NULL;
    	}

    delete iFactory;

    delete iDecFormatIntfc;
    delete iEncFormatIntfc;
    */
}

// -----------------------------------------------------------------------------
// CCallAudioControlTestClass::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCallAudioControlTestClass::RunMethodL( CStifItemParser& aItem )
{

    static TStifFunctionInfo const KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.

        ENTRY( "SetTimeout", CCallAudioControlTestClass::SetTimeout ),
        ENTRY( "CallAudioControl", CCallAudioControlTestClass::CallAudioControl ),
        ENTRY( "AppendRoutingObserver", CCallAudioControlTestClass::AppendRoutingObserver ),
        ENTRY( "AppendUplinkGainObserver", CCallAudioControlTestClass::AppendUplinkGainObserver ),
        ENTRY( "AppendDownlinkVolumeObserver", CCallAudioControlTestClass::AppendDownlinkVolumeObserver ),
        ENTRY( "RemoveRoutingObserver", CCallAudioControlTestClass::RemoveRoutingObserver ),
        ENTRY( "RemoveUplinkGainObserver", CCallAudioControlTestClass::RemoveUplinkGainObserver ),
        ENTRY( "RemoveDownlinkVolumeObserver", CCallAudioControlTestClass::RemoveDownlinkVolumeObserver ),
        ENTRY( "GetAvailableOutputs", CCallAudioControlTestClass::GetAvailableOutputs ),
        ENTRY( "SetOutput", CCallAudioControlTestClass::SetOutput ),
        ENTRY( "GetOutput", CCallAudioControlTestClass::GetOutput ),
        ENTRY( "PreviousOutput", CCallAudioControlTestClass::PreviousOutput ),
        ENTRY( "SetDownlinkVolume", CCallAudioControlTestClass::SetDownlinkVolume ),
        ENTRY( "GetMinDownlinkVolume", CCallAudioControlTestClass::GetMinDownlinkVolume ),
        ENTRY( "GetDownlinkVolume", CCallAudioControlTestClass::GetDownlinkVolume ),
        ENTRY( "GetMaxDownlinkVolume", CCallAudioControlTestClass::GetMaxDownlinkVolume ),
        ENTRY( "SetUplinkMute", CCallAudioControlTestClass::SetUplinkMute ),
        ENTRY( "GetUplinkMute", CCallAudioControlTestClass::GetUplinkMute ),

        ENTRY( "Example", CCallAudioControlTestClass::ExampleL ),



        };

    const TInt count = sizeof( KFunctions ) /
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

}


// -----------------------------------------------------------------------------
// CCallAudioControlTestClass::EventName
// Return descriptor with the notification description
// -----------------------------------------------------------------------------
TPtrC CCallAudioControlTestClass::EventName( TInt aKey )
{
    static TText* const badKeyword = (TText*)L"BadKeyword";
    static TText* const keywords[] =
    {
        (TText*)L"EDownlinkVolumeChanged",
   //     (TText*)L"KDownlinkMuteStateChanged",
        (TText*)L"EUplinkMuteStateChanged",
        (TText*)L"EOutputChanged",
        (TText*)L"ESetOutputComplete",

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
// CCallAudioControlTestClass::AddExpectedEvent
// Add an event to the expected events' list
// -----------------------------------------------------------------------------
void CCallAudioControlTestClass::AddExpectedEvent(TCallControlExpectedEvent event, TInt ms)
{
    FTRACE(FPrint(_L("CCallAudioControlTestClass::AddExpectedEvent")));
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
// CCallAudioControlTestClass::RemoveExpectedEvent
// Remove the indicated event from the expected events' list
// -----------------------------------------------------------------------------
TBool CCallAudioControlTestClass::RemoveExpectedEvent(TCallControlExpectedEvent aEvent)
{
    FTRACE(FPrint(_L("CCallAudioControlTestClass::RemoveExpectedEvent")));
    iLog->Log(_L("CCallAudioControlTestClass::RemoveExpectedEvent"));
    TBool match = EFalse;
    for (TUint i=0; i < iExpectedEvents.Count() ; i++)
    	{
        if (iExpectedEvents[i] == aEvent)
        	{
        	iLog->Log(_L("Expeted events: %d"), iExpectedEvents.Count());
            iExpectedEvents.Remove(i);
            match = ETrue;
            break;
        	}
    	}

    return match;
}


// -----------------------------------------------------------------------------
// CCallAudioControlTestClass::RemoveAllExpectedEvents
// Remove the indicated event from the expected events' list
// -----------------------------------------------------------------------------
void CCallAudioControlTestClass::RemoveAllExpectedEvents()
{
    FTRACE(FPrint(_L("CCallAudioControlTestClass::RemoveAllExpectedEvents")));
    iLog->Log(_L("Removing all expected events"));

    iExpectedEvents.Reset();
    iOcurredEvents.Reset();
}


// -----------------------------------------------------------------------------
// CCallAudioControlTestClass::ProcessEvent
// -----------------------------------------------------------------------------
void CCallAudioControlTestClass::ProcessEvent(TCallControlExpectedEvent aEvent, TInt aError)
{
    FTRACE(FPrint(_L("CCallAudioControlTestClass::ProcessExpectedEvent")));
    iLog->Log(_L("CCallAudioControlTestClass::ProcessExpectedEvent"));
    TPtrC nameEvent = EventName(aEvent);

    if (aError == KErrNone)
    	{
        if (RemoveExpectedEvent(aEvent))
        	{
            iLog->Log(_L("Expected Event: (0x%02x)%S has ocurred Total=%d"), aEvent, &nameEvent,iExpectedEvents.Count());
        	}
        else
        	{
            iLog->Log(_L("Event: (0x%02x)%S has ocurred"), aEvent, &nameEvent);
            return;
        	}

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
// CCallAudioControlTestClass::HandleTimeout
// Review if all the expected events have ocurred once the time is over
// -----------------------------------------------------------------------------

void CCallAudioControlTestClass::HandleTimeout(TInt aError)
{
    FTRACE(FPrint(_L("CCallAudioControlTestClass::HandleTimeout")));

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
// CCallAudioControlTestClass::SetTimeout
// Create a timer and set a timeout
// When the timeout is reached the test case is marked as failed
// It's used rather than the "timeout" keyword in the configuration file
// because in this way the log continues
// -----------------------------------------------------------------------------
TInt CCallAudioControlTestClass::SetTimeout( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CCallAudioControlTestClass::SetTimeout")));
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
TInt CCallAudioControlTestClass::SetAllowedPanic( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CCallAudioControlTestClass::SetAllowedPanic")));
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
// CCallAudioControlTestClass::SetExpectedEvents()
// -----------------------------------------------------------------------------
TInt CCallAudioControlTestClass::SetExpectedEvents( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CCallAudioControlTestClass::SetExpectedEvents")));
    TInt error = KErrNone;
    TInt event=0;
    while ( KErrNone == aItem.GetNextInt(event))
    	{
        AddExpectedEvent(static_cast<TCallControlExpectedEvent>(event), 0); // Default timeout value
    	}

    return error;
}

TInt CCallAudioControlTestClass::CallAudioControl( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::CallAudioControl"));
    TInt error = KErrNone;

    TRAP( error, iCallAudioControl = CCallAudioControl::NewL() );
		iLog->Log(_L("CallAudioControl::CCallAudioControl, error:[%d] "), error);

    return error;
}

TInt CCallAudioControlTestClass::AppendRoutingObserver( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::AppendRoutingObserver"));
    TInt error = KErrNone;

    error = iCallAudioControl->AppendRoutingObserver(*this);
		iLog->Log(_L("CallAudioControl::AppendRoutingObserver, error:[%d] "), error);

    return error;
}


TInt CCallAudioControlTestClass::AppendUplinkGainObserver( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::AppendUplinkGainObserver"));
    TInt error = KErrNone;

    error = iCallAudioControl->AppendUplinkGainObserver(*this);
		iLog->Log(_L("CallAudioControl::AppendUplinkGainObserver, error:[%d] "), error);

    return error;
}


TInt CCallAudioControlTestClass::AppendDownlinkVolumeObserver( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::CallAudioControl"));
    TInt error = KErrNone;

    error = iCallAudioControl->AppendDownlinkVolumeObserver(*this);
		iLog->Log(_L("CallAudioControl::AppendDownlinkVolumeObserver, error:[%d] "), error);

    return error;
}

TInt CCallAudioControlTestClass::RemoveRoutingObserver( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::RemoveRoutingObserver"));
    TInt error = KErrNone;

    error = iCallAudioControl->RemoveRoutingObserver(*this);
		iLog->Log(_L("CallAudioControl::RemoveRoutingObserver, error:[%d] "), error);

    return error;
}



TInt CCallAudioControlTestClass::RemoveUplinkGainObserver( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::RemoveUplinkGainObserver"));
    TInt error = KErrNone;

    error = iCallAudioControl->RemoveUplinkGainObserver(*this);
		iLog->Log(_L("CallAudioControl::RemoveUplinkGainObserver, error:[%d] "), error);

    return error;
}


TInt CCallAudioControlTestClass::RemoveDownlinkVolumeObserver( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::RemoveDownlinkVolumeObserver"));
    TInt error = KErrNone;

    error = iCallAudioControl->RemoveDownlinkVolumeObserver(*this);
		iLog->Log(_L("CallAudioControl::RemoveDownlinkVolumeObserver, error:[%d] "), error);

    return error;
}




TInt CCallAudioControlTestClass::GetAvailableOutputs( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::GetAvailableOutputs"));
    TInt error = KErrNone;

    if (iCallAudioControl)
		{
			  RArray<CCallAudioControl::TAudioOutput> iAvailableOutputArray;
				iAvailableOutputArray.Reset();
				error = iCallAudioControl->GetAvailableOutputs(iAvailableOutputArray);
				iLog->Log(_L("Get Available Outputs Error: %d"), error);
				if (error == KErrNone)
				{
					TInt count = iAvailableOutputArray.Count();
					iLog->Log(_L("GetAvailableOutputs: Available Outputs:\n"));
					for (TInt i = 0; i < count; i++)
					{
						DisplayOutput(iAvailableOutputArray[i]);
					}
				}
		}

    return error;
}



TInt CCallAudioControlTestClass::SetOutput( CStifItemParser& aItem )
{
    iLog->Log(_L("CallAudioControl::SetOutput"));
    TInt error = KErrNone;

    TPtrC OutputType;
    error = aItem.GetNextString(OutputType);
    if (error == KErrNone)
    	{

        if (OutputType == KTagNotActive)
        {
    				error = iCallAudioControl->SetOutput(CCallAudioControl::ENotActive);
						iLog->Log(_L("CallAudioControl::SetOutput - ENotActive, error:[%d] "), error);
				}
				else if (OutputType == KTagNone)
				{
					  error = iCallAudioControl->SetOutput(CCallAudioControl::ENone);
						iLog->Log(_L("CallAudioControl::SetOutput - ENone, error:[%d] "), error);
				}
				else if (OutputType == KTagHandset)
				{
					  error = iCallAudioControl->SetOutput(CCallAudioControl::EHandset);
						iLog->Log(_L("CallAudioControl::SetOutput - EHandset, error:[%d] "), error);
				}
				else if (OutputType == KTagLoudspeaker)
				{
					  error = iCallAudioControl->SetOutput(CCallAudioControl::ELoudspeaker);
						iLog->Log(_L("CallAudioControl::SetOutput - ELoudspeaker, error:[%d] "), error);
			  }
				else if (OutputType == KTagWiredAudioAccessory)
				{
					  error = iCallAudioControl->SetOutput(CCallAudioControl::EWiredAudioAccessory);
						iLog->Log(_L("CallAudioControl::SetOutput - EWiredAudioAccessory, error:[%d] "), error);
				}
				else if (OutputType == KTagBTAudioAccessory)
				{
					  error = iCallAudioControl->SetOutput(CCallAudioControl::EBTAudioAccessory);
						iLog->Log(_L("CallAudioControl::SetOutput - EBTAudioAccessory, error:[%d] "), error);
				}
				else if (OutputType == KTagTTY)
				{
					  error = iCallAudioControl->SetOutput(CCallAudioControl::ETTY);
						iLog->Log(_L("CallAudioControl::SetOutput - ETTY, error:[%d] "), error);
				}
				else
        {
            iLog->Log(KMsgBadTestParameters);
            error = KErrBadTestParameter;
            return error;
        }
     //   AddExpectedEvent(EOutputChanged, KLongTimeout);
        AddExpectedEvent(ESetOutputComplete, KLongTimeout);

		}

    return error;
}

TInt CCallAudioControlTestClass::GetOutput( CStifItemParser& aItem )
{
    iLog->Log(_L("CallAudioControl::GetOutput"));
    TInt error = KErrNone;

    if (iCallAudioControl)
		{
				error = iCallAudioControl->GetOutput(iCurrentAudioOutput);
				iLog->Log(_L("CallAudioControl::GetOutput, error:[%d] "), error);

				if (error == KErrNone)
				{
					DisplayOutput(iCurrentAudioOutput);
				}
		}

		if (error != KErrNone)
			return error;

		TPtrC OutputType;
    error = aItem.GetNextString(OutputType);
    if (error == KErrNone)
    {
    	  if (OutputType == KTagNotActive)
    	  {
    	  	if (iCurrentAudioOutput != CCallAudioControl::ENotActive)
        	{
    				error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::GetOutput - not expected output, error:[%d] "), error);
					}
				}
				else if (OutputType == KTagNone)
				{
					if (iCurrentAudioOutput != CCallAudioControl::ENone)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::GetOutput - not expected output, error:[%d] "), error);
					}
				}
				else if (OutputType == KTagHandset)
				{
					if (iCurrentAudioOutput != CCallAudioControl::EHandset)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::GetOutput - not expected output, error:[%d] "), error);
					}
				}
				else if (OutputType == KTagLoudspeaker)
				{
					if (iCurrentAudioOutput != CCallAudioControl::ELoudspeaker)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::GetOutput - not expected output, error:[%d] "), error);
					}
			  }
				else if (OutputType == KTagWiredAudioAccessory)
				{
					if (iCurrentAudioOutput != CCallAudioControl::EWiredAudioAccessory)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::GetOutput - not expected output, error:[%d] "), error);
					}
				}
				else if (OutputType == KTagBTAudioAccessory)
				{
					if (iCurrentAudioOutput != CCallAudioControl::EBTAudioAccessory)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::GetOutput - not expected output, error:[%d] "), error);
					}
				}
				else if (OutputType == KTagTTY)
				{
					if (iCurrentAudioOutput != CCallAudioControl::ETTY)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::GetOutput - not expected output, error:[%d] "), error);
					}
				}
				else
        {
            iLog->Log(KMsgBadTestParameters);
            error = KErrBadTestParameter;
            return error;
        }

        return error;
    }

    return KErrNone;  // GetOutput doesn't expect any specific output value.
}


TInt CCallAudioControlTestClass::PreviousOutput( CStifItemParser& aItem )
{
    iLog->Log(_L("CallAudioControl::PreviousOutput"));
    TInt error = KErrNone;

    if (iCallAudioControl)
		{
				error = iCallAudioControl->PreviousOutput(iPreviousAudioOutput);
				iLog->Log(_L("CallAudioControl::PreviousOutput, error:[%d] "), error);

				if (error == KErrNone)
				{
					DisplayOutput(iPreviousAudioOutput);
				}
		}

    if (error != KErrNone)
			return error;

		TPtrC OutputType;
    error = aItem.GetNextString(OutputType);
    if (error == KErrNone)
    {
    	  if (OutputType == KTagNotActive)
    	 	{
    	   	if (iPreviousAudioOutput != CCallAudioControl::ENotActive)
        	{
    				error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::PreviousOutput - not expected output, error:[%d] "), error);
					}
				}
				else if (OutputType == KTagNone)
				{
					if (iPreviousAudioOutput != CCallAudioControl::ENone)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::PreviousOutput - not expected output, error:[%d] "), error);
					}
				}
				else if (OutputType == KTagHandset)
				{
					if (iPreviousAudioOutput != CCallAudioControl::EHandset)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::PreviousOutput - not expected output, error:[%d] "), error);
					}
				}
				else if (OutputType == KTagLoudspeaker)
				{
					if (iPreviousAudioOutput != CCallAudioControl::ELoudspeaker)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::PreviousOutput - not expected output, error:[%d] "), error);
					}
			  }
				else if (OutputType == KTagWiredAudioAccessory)
				{
					if (iPreviousAudioOutput != CCallAudioControl::EWiredAudioAccessory)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::PreviousOutput - not expected output, error:[%d] "), error);
					}
				}
				else if (OutputType == KTagBTAudioAccessory)
				{
					if (iPreviousAudioOutput != CCallAudioControl::EBTAudioAccessory)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::PreviousOutput - not expected output, error:[%d] "), error);
					}
				}
				else if (OutputType == KTagTTY)
				{
					if (iPreviousAudioOutput != CCallAudioControl::ETTY)
					{
					  error = KErrUnexpectedValue;
						iLog->Log(_L("CallAudioControl::PreviousOutput - not expected output, error:[%d] "), error);
					}
				}
				else
        {
            iLog->Log(KMsgBadTestParameters);
            error = KErrBadTestParameter;
            return error;
        }

        return error;
    }

    return KErrNone;  // PreviousOutput doesn't expect any specific output value.
}


TInt CCallAudioControlTestClass::SetDownlinkVolume( CStifItemParser& aItem )
{
    iLog->Log(_L("CallAudioControl::SetDownlinkVolume"));
    TInt error = KErrNone;

    TPtrC volumeLevel;
    error = aItem.GetNextString(volumeLevel);

		TUint volume;
		TUint volume1;
		if (error == KErrNone)
		{
			if (volumeLevel == KTagMin)
			{
				error = iCallAudioControl->GetMinDownlinkVolume(volume);
				if (error == KErrNone)
				{
					iLog->Log(_L("Set minimum valume:[%d] "), volume);
					error = iCallAudioControl->SetDownlinkVolume(volume);
				//	AddExpectedEvent(EDownlinkVolumeChanged, KMediumTimeout);
				}
			}
			else if (volumeLevel == KTagMax)
			{
				error = iCallAudioControl->GetMaxDownlinkVolume(volume);
				if (error == KErrNone)
				{
					iLog->Log(_L("Set maximum valume:[%d] "), volume);
					error = iCallAudioControl->SetDownlinkVolume(volume);
				//	AddExpectedEvent(EDownlinkVolumeChanged, KMediumTimeout);
				}
			}
		}
		else
		{
				error = iCallAudioControl->GetMinDownlinkVolume(volume);
				if (error == KErrNone)
				{
					error = iCallAudioControl->GetMaxDownlinkVolume(volume1);
		//			AddExpectedEvent(EDownlinkMuteStateChanged, KMediumTimeout);
		//			AddExpectedEvent(EDownlinkVolumeChanged, KMediumTimeout);
				}
				if (error == KErrNone)
				{
					//TInt temp = (volume+volume1)/2;
					//iLog->Log(_L("Set medium valume:[%d] "), ((TInt)((volume+volume1)/2))  ));
					//error = iCallAudioControl->SetDownlinkVolume((TInt)((volume+volume1)/2));
					iLog->Log(_L("Set medium valume:[%d] "), (volume+volume1)/2);
					error = iCallAudioControl->SetDownlinkVolume((volume+volume1)/2);

		//			AddExpectedEvent(EDownlinkVolumeChanged, KMediumTimeout);
				}
		}




		iLog->Log(_L("CallAudioControl::SetDownlinkVolume, error:[%d] "), error);

    return error;
}


TInt CCallAudioControlTestClass::GetMinDownlinkVolume( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::GetMinDownlinkVolume"));
    TInt error = KErrNone;

		TUint minVolume;
		error = iCallAudioControl->GetMinDownlinkVolume(minVolume);
		iLog->Log(_L("Get MIN Downlink Volume:[%d], Error: %d"),minVolume, error);

    return error;
}


TInt CCallAudioControlTestClass::GetDownlinkVolume( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::GetDownlinkVolume"));
    TInt error = KErrNone;

		TUint volume;
		error = iCallAudioControl->GetDownlinkVolume(volume);
		iLog->Log(_L("Get Downlink Volume:[%d], Error: %d"),volume, error);

    return error;
}

TInt CCallAudioControlTestClass::GetMaxDownlinkVolume( CStifItemParser& /*aItem */)
{
    iLog->Log(_L("CallAudioControl::GetMaxDownlinkVolume"));
    TInt error = KErrNone;

		TUint maxVolume;
		error = iCallAudioControl->GetMaxDownlinkVolume(maxVolume);
		iLog->Log(_L("Get MAX Downlink Volume:[%d], Error: %d"),maxVolume, error);

    return error;
}


TInt CCallAudioControlTestClass::SetUplinkMute( CStifItemParser& aItem )
{
    iLog->Log(_L("CallAudioControl::SetUplinkMute"));
    TInt error = KErrNone;


    TPtrC isMute;
    error = aItem.GetNextString(isMute);
		if (error == KErrNone)
		{
        if (isMute == KTagTrue)
        {
        	error = iCallAudioControl->SetUplinkMute(ETrue);
        	iLog->Log(_L("CallAudioControl::SetUplinkMute(ETrue) "));
        }
        else
        {
        	error = iCallAudioControl->SetUplinkMute(EFalse);
        	iLog->Log(_L("CallAudioControl::SetUplinkMute(EFalse) "));
        }
		}

		iLog->Log(_L("CallAudioControl::SetUplinkMute, error:[%d] "), error);
		AddExpectedEvent(EUplinkMuteStateChanged, KMediumTimeout);

    return error;
}


TInt CCallAudioControlTestClass::GetUplinkMute( CStifItemParser& aItem )
{
    iLog->Log(_L("CallAudioControl::GetUplinkMute"));
    TInt error = KErrNone;

    TBool uplinkMute = EFalse;
		error = iCallAudioControl->GetUplinkMute(uplinkMute);

		TPtrC isMute;
    error = aItem.GetNextString(isMute);
		if (error == KErrNone)
		{
			if (((isMute == KTagTrue)&& (uplinkMute) ) || ((isMute == KTagFalse)&& (!uplinkMute) ))
			{
				return KErrNone;
			}
	  	else
  		{
				error = KErrUnexpectedValue;
				if (uplinkMute)
				{
					iLog->Log(_L("CallAudioControl::GetUplinkMute - ETrue "));
				}
				else
				{
					iLog->Log(_L("CallAudioControl::GetUplinkMute - EFalse "));
				}

			}
		}

    return error;
}


// ----------------------------------------------------------------------------
// CCallAudioControlTestClass::DisplayOutput
//
// ----------------------------------------------------------------------------
//
void CCallAudioControlTestClass::DisplayOutput(CCallAudioControl::TAudioOutput aOutput)
{
    if (aOutput == CCallAudioControl::ENotActive)
        {
        iLog->Log(_L("NotActive"));
        }
    else if (aOutput == CCallAudioControl::ENone)
        {
        iLog->Log(_L("None"));
        }
    else if (aOutput == CCallAudioControl::EHandset)
        {
        iLog->Log(_L("Handset"));
        }
    else if (aOutput == CCallAudioControl::ELoudspeaker)
        {
        iLog->Log(_L("Loudspeaker"));
        }
    else if (aOutput == CCallAudioControl::EWiredAudioAccessory)
        {
        iLog->Log(_L("Wired Audio Accessory "));
        }
    else if (aOutput == CCallAudioControl::EBTAudioAccessory)
        {
        iLog->Log(_L("BT Audio Accessory"));
        }
    else if (aOutput == CCallAudioControl::ETTY)
        {
        iLog->Log(_L("TTY"));
        }
}

// -----------------------------------------------------------------------------
// CCallAudioControlTestClass::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCallAudioControlTestClass::ExampleL( CStifItemParser& aItem )
{

    // Print to UI
    _LIT( KCallAudioControlTestClass, "CallAudioControlTestClass" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KCallAudioControlTestClass, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KCallAudioControlTestClass, KParam, i, &string );
        i++;
        }

    return KErrNone;
}
