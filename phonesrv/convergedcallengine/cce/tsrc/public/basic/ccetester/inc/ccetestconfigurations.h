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
* Description:  Defenitoins for test configurations for test plugins.
*
*/


#ifndef CCETESTCONFIGURATIONS_H
#define CCETESTCONFIGURATIONS_H

/*
* Test case names (IDs) are formed as Classname + :: + methodname. E,g, as in c++ code.
* ID example is "MCCPCall::RemoteParty"
*
* Test configuration options:
* Example 1 - set call creating to leave with -1:
* test ConfigurePlugin 5 -1 ID=CConvergedCallProvider::NewCallL LEAVE=-1 TYPE=TInt
*
* Example 2 - set call creating to return NULL ponter 
* test ConfigurePlugin 5 0 ID=CConvergedCallProvider::NewCallL RETURN=0 TYPE=TInt
*
* Example 3 - set call recipient to return given string
* test ConfigurePlugin 5 0 ID=MCCPCall::RemoteParty RETURN=test@test.com TYPE=TDesC8
*
* Parameters:
*
* First parameter is ConfigurePlugin
* Second parameter is serviceid of the target plugin
* ID= is mandatory and has to be found from ActionList
* TYPE= is mandatory. If you add a new parameter type support add support to plugin too see e.g. CConfigurationHandler
* Either RETURN= or LEAVE= is mandatory.
* PERSISTANT= is optional. Used to set value as permanent until next set. If next set has no PERSISTANT= 
* 			  parameter or it is set to false, next call will remove the configuration.
*
* Other: 
* - Leaving methods that will return a value e.g. CConvergedCallProvider::NewCallL that return created call pointer, 
* 	will check for RETURN= and LEAVE= values. In case L-method is void leaved value can be set using either RETUEN or LEAVE.
* - Extension interface is used at the moment to pass values to plugin. In case test case parameters are malformed ext if observer is notified
* 	with KErrArgument. Note plug-in must used correct service-id to CCE in order to get the error. Also observer for CCE needs to be set.
*
* Example of creating a new test configuration case.
*
* - Add name of the case to ActionList
* - Use the value in test configuration file with ID= 
* - Give also used parameter type TYPE=
* - Set passed value with RETURN= or LEAVE=. RETURN= can pass any needed information as string. Note spaces are not allowed!
* - KFieldSeparator can be used in string to separate values.
* - Catch the value passed at plug-in. See examples from plug-in code. E.g. 
*
*    TInt value;
*    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Hold"),value ) )
*    	{
*    	return value;
*    	}
*
*/

// max string size 
const unsigned MaxStrSize = 1024;

_LIT( KFieldSeparator, "|" );

/**
*
* List of supported keywords.
* Names compared with equal char included.
* Therefore the tailing '=' is mandatory in name.
*/

/** Action ID */
_LIT8( KId, "ID=" ); 
/** Set to return value */
_LIT8( KReturn, "RETURN=" );  
/** Set to leave */
_LIT8( KLeave, "LEAVE=" ); 
/** Parameter type from ValueTypeList */
_LIT8( KType, "TYPE=" );  
/** Set changed value to be persistant */
_LIT8( KPersistant, "PERSISTANT=" ); 



/**
* List of supported value types to set. 
* Use with keyword TYPE=''
*/
static unsigned char ValueTypeList[][MaxStrSize] = 
	{
	/** use also to return enums/TUid/. */
 	"TInt",
 	"TUint",
 	/** At plugin use HBufC8 pointer (buf) as following to fetch TDesC8 value: IsActionConfigured( firstparam, *buf ) */
 	"TDesC8",
 	/** TUid */
 	"TUid",
 	/** TBool */
 	"TBool",
 	/** MCCPCall::TCCPCallParameters - not supported yet */
 	"MCCPCall::TCCPCallParameters"
	};


/**
*
* List of supported actions.
* When adding new tests add name of it (ID) here too.
*
*/
static unsigned char ActionList[][MaxStrSize] = 
	{
/** CConvergedCallProvider realted */
 	"CConvergedCallProvider::InitializeL",
 	"CConvergedCallProvider::Uid",
 	"CConvergedCallProvider::Caps",
 	
	/** todo */ 	
 	"CConvergedCallProvider::GetLifeTime",
	/** todo */ 	
	"CConvergedCallProvider::GetCSInfo",
 	
 	"CConvergedCallProvider::NewCallL",
 	"CConvergedCallProvider::ReleaseCall",				

 	"CConvergedCallProvider::NewEmergencyCallL",
 	"CConvergedCallProvider::ReleaseEmergencyCall",
 
 	"CConvergedCallProvider::NewConferenceL",
 	"CConvergedCallProvider::ReleaseConferenceCall",

	"CConvergedCallProvider::DTMFProviderL",
 
	"CConvergedCallProvider::ExtensionProviderL",


/** MCCPCall realted */
 	"MCCPCall::Answer",
 	"MCCPCall::Reject",
 	"MCCPCall::Queue",
 	"MCCPCall::Dial",
 	"MCCPCall::Ringing",
 	"MCCPCall::HangUp",
 	"MCCPCall::Cancel",
 	"MCCPCall::Hold",
 	"MCCPCall::Resume",
 	"MCCPCall::Swap",
 	"MCCPCall::IsCallForwarded",
 	"MCCPCall::IsMobileOriginated",
 	"MCCPCall::State",
 	"MCCPCall::Uid",
 	"MCCPCall::Caps",      
 	"MCCPCall::IsSecured",
 	"MCCPCall::Parameters",
 	"MCCPCall::ForwardProviderL",
 	"MCCPCall::TransferProviderL",
 	"MCCPCall::AddObserverL",
 	"MCCPCall::RemoveObserver",
 	"MCCPCall::RemoteParty",
 	"MCCPCall::DialledParty",	
 	
/** MCCPConferenceCall realted */
 
 	"MCCPConferenceCall::ServiceId_value",
 	"MCCPConferenceCall::HangUp",
 	"MCCPConferenceCall::Hold",
 	"MCCPConferenceCall::Resume",
 	"MCCPConferenceCall::Swap",
 	"MCCPConferenceCall::AddCallL",
 	"MCCPConferenceCall::RemoveCallL",
 	"MCCPConferenceCall::CallCount",
 	"MCCPConferenceCall::GoOneToOneL",
 	"MCCPConferenceCall::CurrentCallsToConferenceL",
 	"MCCPConferenceCall::AddObserverL",
 	"MCCPConferenceCall::RemoveObserver",

/** MCCPCSCall realted */

 	"MCCPCSCall::Dial_value",
 	"MCCPCSCall::GetMobileCallInfo",
 	"MCCPCSCall::GetMobileDataCallCaps",

/** MCCPDTMFProvider realted */
 
 	"MCCPDTMFProvider::CancelDtmfStringSending",
 	"MCCPDTMFProvider::StartDtmfTone",
 	"MCCPDTMFProvider::StopDtmfTone",
 	"MCCPDTMFProvider::SendDtmfToneString",
 	"MCCPDTMFProvider::ContinueDtmfStringSending",
 	"MCCPDTMFProvider::AddObserverL",
 	"MCCPDTMFProvider::RemoveObserver",

 
/** MCCPEmergencyCall realted */
 
 	"MCCPEmergencyCall::Dial",
 	"MCCPEmergencyCall::Cancel",
 	"MCCPEmergencyCall::HangUp",
 	"MCCPEmergencyCall::Answer",
 
/** MCCPExtensionProvider realted */
 	"MCCPExtensionProvider::RequestCommand",
 	"MCCPExtensionProvider::CancelRequest",
 	"MCCPExtensionProvider::AddObserverL",
 	"MCCPExtensionProvider::RemoveObserver",

/** MCCPForwardProvider realted */

	/* Getting forward addresses leaves
 	virtual const CDesC8Array& GetForwardAddressChoicesL() = 0;
 	*/
 	"MCCPForwardProvider::GetForwardAddressChoicesL",
 	"MCCPForwardProvider::ForwardToAddressL",
 	"MCCPForwardProvider::AddObserverL",
 	"MCCPForwardProvider::RemoveObserver",

/** MCCPTransferProvider realted */

	// two same named functions, identify by first param taken
 	"MCCPTransferProvider::AttendedTransfer_TDesC", 
	// two same named functions, identify by first param taken
 	"MCCPTransferProvider::AttendedTransfer_MCCPCall",
 	"MCCPTransferProvider::UnattendedTransfer",
 	"MCCPTransferProvider::AcceptTransfer",
 	"MCCPTransferProvider::TransferTarget",
 	"MCCPTransferProvider::AddObserverL",
 	"MCCPTransferProvider::RemoveObserver"
	};

#endif      // CCETESTCONFIGURATIONS_H

// End of File


