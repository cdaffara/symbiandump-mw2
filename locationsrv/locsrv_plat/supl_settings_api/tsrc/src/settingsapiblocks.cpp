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
* Description:  Implementation of settings api classes
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h> 
#include <Stiftestinterface.h>
#include <epos_suplterminal.h>
#include <epos_suplterminalconstants.h>
#include <epos_suplnetwork.h>
#include <epos_MSuplSettingsObserver.h>
#include <epos_csuplsettingparams.h>
#include <epos_CSuplSettings.h>
#include <epos_csuplsettingsconstants.h>


#include "settingsapi.h"
#include "sessionobserver.h"


// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;
// CONSTANTS
_LIT(KSuplServerName, "SuplServer");
_LIT(KSuplServerImg, "EPosSuplServer");

// The third UID of the server DLL
const TUid KSuplServerUid3 = { 0x01fff01a };

const TInt KDefaultMessageSlots = 16;

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
// CSettingsApi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CSettingsApi::Delete() 
    {
    
    }
    
// -----------------------------------------------------------------------------
// CSettingsApi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CSettingsApi::RunMethodL( 
    CStifItemParser& aItem ) 
    {
   
    static TStifFunctionInfo const KFunctions[] =
        {  
				ENTRY( "InitializeImsi", CSettingsApi::InitializeImsi ),
        ENTRY( "CancelInitializeTest", CSettingsApi::CancelInitializeTest ),
        ENTRY( "GetHslpFromImsi", CSettingsApi::GetHslpFromImsi ),
        ENTRY( "IsImsiChangedTest", CSettingsApi::IsImsiChangedTest ),
        ENTRY( "RemoveObserver", CSettingsApi::RemoveObserver ),
     		ENTRY( "GetImsiWithoutInitialize", CSettingsApi::GetImsiWithoutInitialize ),
        ENTRY( "RemoveBeforeSetObserver", CSettingsApi::RemoveBeforeSetObserver ),
        ENTRY( "IsImsiChangedWithoutInitialize", CSettingsApi::IsImsiChangedWithoutInitialize ),
        ENTRY( "GenerateHslpFromImsiWithoutInitialize", CSettingsApi::GenerateHslpFromImsiWithoutInitialize ),
       	ENTRY( "GenerateHslpWithoutInitialize", CSettingsApi::GenerateHslpWithoutInitialize ),
				ENTRY( "SetUsageAutomatic", CSettingsApi::SetUsageAutomatic),  
				ENTRY( "SetUsageAlwaysAsk", CSettingsApi::SetUsageAlwaysAsk),  
				ENTRY( "SetUsageAutoInHMNw", CSettingsApi::SetUsageAutoInHMNw),  
				ENTRY( "SetUsageDisabled", CSettingsApi::SetUsageDisabled),  
				ENTRY( "GetUsageAutomatic", CSettingsApi::GetUsageAutomatic),  
				ENTRY( "GetUsageAlwaysAsk", CSettingsApi::GetUsageAlwaysAsk),  
				ENTRY( "GetUsageAutoInHMNw", CSettingsApi::GetUsageAutoInHMNw),  
				ENTRY( "GetUsageDisabled", CSettingsApi::GetUsageDisabled),  
				ENTRY( "SetUsageOtherThanAppropriate", CSettingsApi::SetUsageOtherThanAppropriate),  
				ENTRY( "GetImsiTest",CSettingsApi::GetImsiTest),
				ENTRY( "SetEditableFlag_Etrue", CSettingsApi::SetEditableFlag_Etrue),
				ENTRY( "SetEditableFlag_EFalse", CSettingsApi::SetEditableFlag_EFalse),
				ENTRY( "SetEditableFlag_id_negative", CSettingsApi::SetEditableFlag_id_negative),
				ENTRY( "SetEditableFlag_id_positive", CSettingsApi::SetEditableFlag_id_positive),
				ENTRY( "SetEditableFlag_id_zero", CSettingsApi::SetEditableFlag_id_zero),
				ENTRY( "RemoveServer", CSettingsApi::RemoveServer),
        ENTRY( "RemoveServer_Invalid_negative", CSettingsApi::RemoveServer_Invalid_negative),
        ENTRY( "RemoveServer_Id_NotExisting", CSettingsApi::RemoveServer_Id_NotExisting),
        ENTRY( "RemoveServer_Invalid_zero", CSettingsApi::RemoveServer_Invalid_zero),
    		ENTRY( "ChangePriority_Increase", CSettingsApi::ChangePriority_Increase),
        ENTRY( "ChangePriority_Decrease", CSettingsApi::ChangePriority_Decrease),
       	ENTRY( "ChangePriority_invalid_positive", CSettingsApi::ChangePriority_invalid_positive),
        ENTRY( "ChangePriority_priority0", CSettingsApi::ChangePriority_priority0),
        ENTRY( "ChangePriority_Negative", CSettingsApi::ChangePriority_Negative),
        ENTRY( "ChangePriority_InvalidServer", CSettingsApi::ChangePriority_InvalidServer),
        ENTRY( "ChangePriority_invalidserverpriority", CSettingsApi::ChangePriority_invalidserverpriority),
        ENTRY( "GetDefaultServer", CSettingsApi::GetDefaultServer),
				ENTRY( "AddNewServerWithDefault", CSettingsApi::AddNewServerWithDefault),
				ENTRY( "AddNewServerWithoutDefault", CSettingsApi::AddNewServerWithoutDefault),
				ENTRY( "AddNewServer_Duplicate", CSettingsApi::AddNewServer_Duplicate),
				ENTRY( "AddNewServer_aServerAddress_Null",CSettingsApi::AddNewServer_aServerAddress_Null),
				ENTRY( "AddNewServer_aIapNAme_Null",CSettingsApi::AddNewServer_aIapNAme_Null),
				ENTRY( "GetSlpInfoFromId", CSettingsApi::GetSlpInfoFromId),
				ENTRY( "GetSlpInfoFromId_invalid_positive", CSettingsApi::GetSlpInfoFromId_invalid_positive),
				ENTRY( "GetSlpInfoFromId_invalid_negative", CSettingsApi::GetSlpInfoFromId_invalid_negative),
				ENTRY( "GetSlpInfoFromId_invalid_zero", CSettingsApi::GetSlpInfoFromId_invalid_zero),
				ENTRY( "SetServerAddress", CSettingsApi::SetServerAddress),
				ENTRY( "SetServerAddress_invalidpositive", CSettingsApi::SetServerAddress_invalidpositive),
				ENTRY( "SetServerAddress_invalidnegative", CSettingsApi::SetServerAddress_invalidnegative),
				ENTRY( "SetServerAddress_invalidzero", CSettingsApi::SetServerAddress_invalidzero),
				ENTRY( "SetServerAddress_AddressNull", CSettingsApi::SetServerAddress_AddressNull),
				ENTRY( "SetServerAddress_InvalidAddressId", CSettingsApi::SetServerAddress_InvalidAddressId),
				ENTRY( "GetServerAddress", CSettingsApi::GetServerAddress),
				ENTRY( "GetServerAddress_invalidpositive", CSettingsApi::GetServerAddress_invalidpositive),
				ENTRY( "GetServerAddress_invalidnegative", CSettingsApi::GetServerAddress_invalidnegative),
				ENTRY( "GetServerAddress_invalidzero", CSettingsApi::GetServerAddress_invalidzero),
				ENTRY( "SetIAPName_invalidzero", CSettingsApi::SetIAPName_invalidzero),
				ENTRY( "SetIAPName_invalidnegative", CSettingsApi::SetIAPName_invalidnegative),
				ENTRY( "SetIAPName_invalidpositive", CSettingsApi::SetIAPName_invalidpositive),
				ENTRY( "SetIAPName", CSettingsApi::SetIAPName),
				ENTRY( "GetIAPName_invalidzero", CSettingsApi::GetIAPName_invalidzero),
				ENTRY( "GetIAPName_invalidnegative", CSettingsApi::GetIAPName_invalidnegative),
				ENTRY( "GetIAPName_invalidpositive", CSettingsApi::GetIAPName_invalidpositive),
				ENTRY( "GetIAPName", CSettingsApi::GetIAPName),
		
				ENTRY( "SetServerEnabledFlag_Etrue", CSettingsApi::SetServerEnabledFlag_Etrue),
				ENTRY( "SetServerEnabledFlag_EFalse", CSettingsApi::SetServerEnabledFlag_EFalse),
				ENTRY( "SetServerEnabledFlag_id_negative", CSettingsApi::SetServerEnabledFlag_id_negative),
				ENTRY( "SetServerEnabledFlag_id_positive", CSettingsApi::SetServerEnabledFlag_id_positive),
				ENTRY( "SetServerEnabledFlag_id_zero", CSettingsApi::SetServerEnabledFlag_id_zero),
				ENTRY( "GetServerEnabledFlag_id_negative", CSettingsApi::GetServerEnabledFlag_id_negative),
				ENTRY( "GetServerEnabledFlag_id_positive", CSettingsApi::GetServerEnabledFlag_id_positive),
				ENTRY( "GetServerEnabledFlag_id_zero", CSettingsApi::GetServerEnabledFlag_id_zero),
				ENTRY( "GetServerEnabledFlag_EFalse", CSettingsApi::GetServerEnabledFlag_EFalse),
				ENTRY( "GetServerEnabledFlag_Etrue", CSettingsApi::GetServerEnabledFlag_Etrue),
				ENTRY( "SetSimChangeRemoveFlag_Etrue", CSettingsApi::SetSimChangeRemoveFlag_Etrue),
				ENTRY( "SetSimChangeRemoveFlag_EFalse", CSettingsApi::SetSimChangeRemoveFlag_EFalse),
				ENTRY( "SetSimChangeRemoveFlag_id_negative", CSettingsApi::SetSimChangeRemoveFlag_id_negative),
				ENTRY( "SetSimChangeRemoveFlag_id_positive", CSettingsApi::SetSimChangeRemoveFlag_id_positive),
				ENTRY( "SetSimChangeRemoveFlag_id_zero", CSettingsApi::SetSimChangeRemoveFlag_id_zero),
				ENTRY( "GetSimChangeRemoveFlag_Etrue", CSettingsApi::GetSimChangeRemoveFlag_Etrue),
				ENTRY( "GetSimChangeRemoveFlag_EFalse", CSettingsApi::GetSimChangeRemoveFlag_EFalse),
				ENTRY( "GetSimChangeRemoveFlag_id_negative", CSettingsApi::GetSimChangeRemoveFlag_id_negative),
				ENTRY( "GetSimChangeRemoveFlag_id_positive", CSettingsApi::GetSimChangeRemoveFlag_id_positive),
				ENTRY( "GetSimChangeRemoveFlag_id_zero", CSettingsApi::GetSimChangeRemoveFlag_id_zero),
				ENTRY( "GetEditableFlag_Etrue", CSettingsApi::GetEditableFlag_Etrue),
				ENTRY( "GetEditableFlag_EFalse", CSettingsApi::GetEditableFlag_EFalse),
				ENTRY( "GetEditableFlag_id_negative", CSettingsApi::GetEditableFlag_id_negative),
				ENTRY( "GetEditableFlag_id_positive", CSettingsApi::GetEditableFlag_id_positive),
				ENTRY( "GetEditableFlag_id_zero", CSettingsApi::GetEditableFlag_id_zero),
				ENTRY( "GetEditableFlag", CSettingsApi::GetEditableFlag),
				ENTRY( "SetUsageInHomwNwFlag_Etrue", CSettingsApi::SetUsageInHomwNwFlag_Etrue),
				ENTRY( "SetUsageInHomwNwFlag_EFalse", CSettingsApi::SetUsageInHomwNwFlag_EFalse),
				ENTRY( "SetUsageInHomwNwFlag_id_negative", CSettingsApi::SetUsageInHomwNwFlag_id_negative),
				ENTRY( "SetUsageInHomwNwFlag_id_positive", CSettingsApi::SetUsageInHomwNwFlag_id_positive),
				ENTRY( "SetUsageInHomwNwFlag_id_zero", CSettingsApi::SetUsageInHomwNwFlag_id_zero),
				ENTRY( "GetUsageInHomwNwFlag_Etrue", CSettingsApi::GetUsageInHomwNwFlag_Etrue),
				ENTRY( "GetUsageInHomwNwFlag_EFalse", CSettingsApi::GetUsageInHomwNwFlag_EFalse),
				ENTRY( "GetUsageInHomwNwFlag_id_negative", CSettingsApi::GetUsageInHomwNwFlag_id_negative),
				ENTRY( "GetUsageInHomwNwFlag_id_positive", CSettingsApi::GetUsageInHomwNwFlag_id_positive),
				ENTRY( "GetUsageInHomwNwFlag_id_zero", CSettingsApi::GetUsageInHomwNwFlag_id_zero),
				ENTRY( "SetDefaultServer_aServerAddress_Null",CSettingsApi::SetDefaultServer_aServerAddress_Null),
				ENTRY( "SetDefaultServerWithDefaultValues",CSettingsApi::SetDefaultServerWithDefaultValues),
				ENTRY( "SetAllParameter_aServerAddress_Null",CSettingsApi::SetAllParameter_aServerAddress_Null),
				ENTRY( "SetAllParameter_WithDefaultValues",CSettingsApi::SetAllParameter_WithDefaultValues),
				ENTRY( "SetAllParameter_Invalid_Positive",CSettingsApi::SetAllParameter_Invalid_Positive),
				ENTRY( "SetAllParameter_Invalid_Negative",CSettingsApi::SetAllParameter_Invalid_Negative),
				ENTRY( "SetAllParameter_Invalid_Zero",CSettingsApi::SetAllParameter_Invalid_Zero),
				ENTRY( "IsSLPExists", CSettingsApi::IsSLPExists),
				ENTRY( "IsSLPExists_invalid_negative", CSettingsApi::IsSLPExists_invalid_negative),
				ENTRY( "IsSLPExists_invalid_positive", CSettingsApi::IsSLPExists_invalid_positive),
				ENTRY( "IsSLPExists_invalid_zero", CSettingsApi::IsSLPExists_invalid_zero),
				ENTRY( "GetAllSlp_sync",CSettingsApi::GetAllSlp_sync),
				ENTRY( "GetAllSlp_async",CSettingsApi::GetAllSlp_async),
				ENTRY( "IsSLPExistsString",CSettingsApi::IsSLPExistsString),
				ENTRY( "IsSLPExistsString_Null",CSettingsApi::IsSLPExistsString_Null),
				ENTRY( "IsSLPExistsString_invalid",CSettingsApi::IsSLPExistsString_invalid),
				ENTRY( "SlpCount",CSettingsApi::SlpCount),
				ENTRY( "GetSlpInfoFromAddress",CSettingsApi::GetSlpInfoFromAddress),
				ENTRY( "GetSlpInfoFromAddress_invalid",CSettingsApi::GetSlpInfoFromAddress_invalid),
				ENTRY( "SetFallBack_ETrue",CSettingsApi::SetFallBack_ETrue),
				ENTRY( "SetFallBack_EFalse",CSettingsApi::SetFallBack_EFalse),
				ENTRY( "GetFallBack",CSettingsApi::GetFallBack),
				
				ENTRY( "SetGetTriggerParams",CSettingsApi::SetGetTriggerParamsL),
				ENTRY( "SetSessionObserver",CSettingsApi::SetSessionObserverL),
				ENTRY( "SetSessionObserverTwice",CSettingsApi::SetSessionObserverTwiceL),
				ENTRY( "RemoveSessionObserver",CSettingsApi::RemoveSessionObserverL),
				ENTRY( "RemoveSessionObserverWithoutSet",CSettingsApi::RemoveSessionObserverWithoutSetL),
				ENTRY( "SetTriggerEmptySessionName",CSettingsApi::SetTriggerEmptySessionNameL),
				ENTRY( "SetTriggerLongSessionName",CSettingsApi::SetTriggerLongSessionNameL),
				ENTRY( "ChangeSuplTriggerServiceStatusON", CSettingsApi::ChangeSuplTriggerServiceStatusON), 
				ENTRY( "ChangeSuplTriggerServiceStatusOFF", CSettingsApi::ChangeSuplTriggerServiceStatusOFF),
				ENTRY( "GetSuplTriggerServiceStatusON", CSettingsApi::GetSuplTriggerServiceStatusON),
				ENTRY( "GetSuplTriggerServiceStatusOFF", CSettingsApi::GetSuplTriggerServiceStatusOFF),
				ENTRY( "ProductConfiguredServer1", CSettingsApi::ProductConfiguredServer1),	
				ENTRY( "ProductConfiguredServer2", CSettingsApi::ProductConfiguredServer2),	
				ENTRY( "ProductConfiguredServer3", CSettingsApi::ProductConfiguredServer3),	
				ENTRY( "ProductConfiguredServer4", CSettingsApi::ProductConfiguredServer4),	
			};
    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );


    }
    
//================== SetEditableFlag =======================================================================================================
 
 //Priya: 1. Pass valid id and aEditFlag = ETrue

TInt CSettingsApi::SetEditableFlag_Etrue(CStifItemParser& aItem)
    {
 /*
IMPORT_C TInt SetEditableFlag( const TInt64 aSlpId, const TBool aEditFlag);  
  */
  
iLog->Log(_L("SetEditableFlag with valid id and  aEditFlag = ETrue"));
TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
TBool aEditFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
		ret=aSettings->SetEditableFlag(aSlpId,aEditFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetEditableFlag=%d" ),ret);

TInt ret1;
 
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else
		return -1;
 }
 
 //Priya: 2. Pass valid id and aEditFlag = EFalse

TInt CSettingsApi::SetEditableFlag_EFalse(CStifItemParser& aItem)
    {
 /*
TInt SetEditableFlag(const TInt64 aSlpId, const TBool aEditFlag)
  */
 iLog->Log(_L("SetEditableFlag with valid id and  aEnable = EFalse"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aEditFlag = EFalse;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetEditableFlag(aSlpId,aEditFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetEditableFlag=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else
		return -1;
 }

 //Priya: 3. Pass invalid id which is negative
TInt CSettingsApi::SetEditableFlag_id_negative(CStifItemParser& aItem)
    {
 /*
TInt SetEditableFlag(const TInt64 aSlpId, const TBool aEditFlag)
  */
 iLog->Log(_L("SetEditableFlag with invalid id which is negative"));
	TInt64 aSlpId = -1872;
	TBool aEditFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->SetEditableFlag(aSlpId,aEditFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of SetEditableFlag=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrArgument)
		return 0;
	else
		return -1;
 }
 
  //Priya: 4. Pass invalid id which is Positive
TInt CSettingsApi::SetEditableFlag_id_positive(CStifItemParser& aItem)
    {
 /*
TInt SetEditableFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("SetEditableFlag with invalid id which is positive"));
	TInt64 aSlpId = 1872;
	TBool aEditFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->SetEditableFlag(aSlpId,aEditFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of SetEditableFlag=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrArgument)
		return 0;
	else
		return -1;
}
//Priya: 4. Pass invalid id  which is zero
 
 TInt CSettingsApi::SetEditableFlag_id_zero(CStifItemParser& aItem)
    {
 /*
TInt SetEditableFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("SetEditableFlag with invalid id which is zero"));
	TInt64 aSlpId = 0;
	TBool aEditFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->SetEditableFlag(aSlpId,aEditFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of SetEditableFlag=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrArgument)
		return 0;
	else
		return -1;
}








//============================ REMOVE SERVER =============================================================================================================================================================
//Priya: 1. Remove server with valid id

TInt CSettingsApi::RemoveServer(CStifItemParser& aItem)
    {
 
	TInt aSlpId1;
	TInt64 aSlpId;
	aItem.GetNextInt(aSlpId1);
	aSlpId = aSlpId1;

	iLog->Log(_L("Remove Server with valid id."));
	CSuplSettings *aSettings=CSuplSettings::NewL();
	TInt ret, ret1;
	
	ret=aSettings->RemoveServer(aSlpId);
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of RemoveServer=%d" ),ret);
	
	delete aSettings;
		
	if( ret == KErrNone) 
	{
			return 0;
	}
	else
	{
		return -1;
	}	
 }
 
//Priya 2. Remove server with invalid id

TInt CSettingsApi::RemoveServer_Invalid_negative(CStifItemParser& aItem)
{
 
	TInt64 aSlpId= -1900;
	iLog->Log(_L("Remove Server with invalid id which is negative"));
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->RemoveServer(aSlpId);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	iLog->Log(_L("Return Value of RemoveServer=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrNotFound)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

//Priya 3. Remove server with invalid id

TInt CSettingsApi::RemoveServer_Invalid_zero(CStifItemParser& aItem)
{
 
	TInt64 aSlpId= 0;
		iLog->Log(_L("Remove Server with invalid id which is zero"));
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;

	ret=aSettings->RemoveServer(aSlpId);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNotFound or KErrArguement"));
iLog->Log(_L("Return Value of RemoveServer=%d" ),ret);

	delete aSettings;
	if(ret == KErrNotFound)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

//Priya 4. Remove server with non existing id

TInt CSettingsApi::RemoveServer_Id_NotExisting(CStifItemParser& aItem)
{
 	TInt64 aSlpId=7809;

		iLog->Log(_L("Remove Server with invalid id which does not exist"));
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->RemoveServer(aSlpId);

	iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	iLog->Log(_L("Return Value of RemoveServer=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrNotFound)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

//=================================================================================================================================================================================================
 
 
 //Priya 1. Execute this when with valid slp id. OUTPUT:KErrNone

TInt CSettingsApi::GetSlpInfoFromId(CStifItemParser& aItem)
    {
 /*
IMPORT_C TInt Get(
                TInt64& aSlpId,                    
                TPtr aServerAddress,
                TPtr aIapName,
                TBool& aServerEnabled,
                TBool& aSimChangeRemove,
                TBool& aUsageInHomeNw,
                TBool& aEditable
                ) const;

  */
 
  iLog->Log(_L("GetSlpInfoFromId for valid slpid"));
	CServerParams *aParamValues = CServerParams::NewL() ;
	
TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	
	HBufC* aServerAddress = HBufC::NewL(255);
	HBufC* aIapName = HBufC::NewL(255);
TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
_LIT( example, "suplsettings.txt" ); 
		
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
	
	ret=aSettings->GetSlpInfoFromId(aSlpId,aParamValues);
		
	aParamValues->Get(aSlpId,aServerAddress->Des(),aIapName->Des(),aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of GetSLPInfoFromId=%d" ),ret);
			
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else
		return -1;
			
 }


//Priya 2. Execute this when with invalid slp id=positive number. OUTPUT:KErrNotFound
TInt CSettingsApi::GetSlpInfoFromId_invalid_positive(CStifItemParser& aItem)
    {
 
   iLog->Log(_L("GetSlpInfoFromId for invalid slpid which is positive"));
	CServerParams *aParamValues = CServerParams::NewL() ;
	
TInt64 aSlpId=6749;

	HBufC* aServerAddress = HBufC::NewL(255);
	HBufC* aIapName = HBufC::NewL(255);
TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
_LIT( example, "suplsettings.txt" ); 
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
	   
	ret=aSettings->GetSlpInfoFromId(aSlpId,aParamValues);
		
	aParamValues->Get(aSlpId,aServerAddress->Des(),aIapName->Des(),aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	iLog->Log(_L("Return Value of GetSLPInfoFromId=%d" ),ret);
			
	delete aSettings;
	if(ret == KErrNotFound)
		return 0;
	else
		return -1;
 }
 
 //Priya 3. Execute this when with invalid slp id=negative number. OUTPUT:KErrNotFound
TInt CSettingsApi::GetSlpInfoFromId_invalid_negative(CStifItemParser& aItem)
    {
 
   iLog->Log(_L("GetSlpInfoFromId for invalid slpid which is negative"));
	CServerParams *aParamValues = CServerParams::NewL() ;
	
TInt64 aSlpId=-6749;

	HBufC* aServerAddress = HBufC::NewL(255);
	HBufC* aIapName = HBufC::NewL(255);
TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
_LIT( example, "suplsettings.txt" ); 
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;

   
	ret=aSettings->GetSlpInfoFromId(aSlpId,aParamValues);
		
	aParamValues->Get(aSlpId,aServerAddress->Des(),aIapName->Des(),aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	iLog->Log(_L("Return Value of GetSLPInfoFromId=%d" ),ret);
			
	delete aSettings;
	if(ret == KErrNotFound)
		return 0;
	else
		return -1;
 }
 
 //Priya 4. Execute this when with invalid slp id=zero. OUTPUT:KErrNotFound
TInt CSettingsApi::GetSlpInfoFromId_invalid_zero(CStifItemParser& aItem)
    {
  
   iLog->Log(_L("GetSlpInfoFromId for invalid slpid which is zero"));
	CServerParams *aParamValues = CServerParams::NewL() ;
	
TInt64 aSlpId = 0;

HBufC* aServerAddress = HBufC::NewL(255);
	HBufC* aIapName = HBufC::NewL(255);
TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
_LIT( example, "suplsettings.txt" ); 
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
	
	ret=aSettings->GetSlpInfoFromId(aSlpId,aParamValues);
		
	aParamValues->Get(aSlpId,aServerAddress->Des(),aIapName->Des(),aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	iLog->Log(_L("Return Value of GetSLPInfoFromId=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrNotFound)
		return 0;
	else
		return -1;
}

//============================================================ Set Server Addresss ======================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================

//Priya: 1. Pass valid id and server address

TInt CSettingsApi::SetServerAddress(CStifItemParser& aItem)
    {
 /*
  Int SetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
 iLog->Log(_L("SetServerAddress with valid id and server address"));
 
	_LIT8(KServer, "supl1.nokia.com");
 	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;

	RBuf aServerAddress;
	aServerAddress.Create(256);
	aServerAddress.Copy(KServer);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
		ret=aSettings->SetServerAddress(aSlpId,aServerAddress);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetServer=%d" ),ret);
	delete aSettings;
aServerAddress.Close();
	

		if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
	
 }
 
 //Priya: 2. Pass server address and invalid id  which is positive

TInt CSettingsApi::SetServerAddress_invalidpositive(CStifItemParser& aItem)
    {
 /*
  Int SetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
 	
 	iLog->Log(_L("SetServerAddress which is invalid and positive"));
	_LIT8(KServer, "suplserver& testcase2");
 	TInt64 aSlpId=8907;
	
	RBuf aServerAddress;
	aServerAddress.Create(256);
	aServerAddress.Copy(KServer);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
  iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	
	ret=aSettings->SetServerAddress(aSlpId,aServerAddress);
		
	iLog->Log(_L("Return Value of SetServer=%d" ),ret);

	delete aSettings;
	
	

	aServerAddress.Close();
		if(ret == KErrNotFound || ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
 }

 //Priya: 3. Pass invalid id and server address which is negative
TInt CSettingsApi::SetServerAddress_invalidnegative(CStifItemParser& aItem)
    {
 /*
  Int SetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
 _LIT8(KServer, "suplserver1.com & testcase3");
 	iLog->Log(_L("SetServerAddress which is invalid and negative"));
	TInt64 aSlpId=-2828;
	
	RBuf aServerAddress;
	aServerAddress.Create(256);
	aServerAddress.Copy(KServer);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
;
  TInt ret;

	ret=aSettings->SetServerAddress(aSlpId,aServerAddress);
	iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	iLog->Log(_L("Return Value of SetServer=%d" ),ret);
	delete aSettings;

	
	
	aServerAddress.Close();
		if(ret == KErrNotFound || ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
}
//Priya: 4. Pass invalid id and server address which is zero
 
 TInt CSettingsApi::SetServerAddress_invalidzero(CStifItemParser& aItem)
    {
 /*
  Int SetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
  
  _LIT8(KServer, "suplserver.com & testcase4");
 	iLog->Log(_L("SetServerAddress which is invalid and zero"));
	TInt64 aSlpId=0;
	
	RBuf aServerAddress;
	aServerAddress.Create(256);
	aServerAddress.Copy(KServer);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;

	ret=aSettings->SetServerAddress(aSlpId,aServerAddress);
		iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));

iLog->Log(_L("Return Value of SetServer=%d" ),ret);

	delete aSettings;

	iLog->Log(_L("Return Value of GetServer=%d" ),ret);
	aServerAddress.Close();
		if(ret == KErrNotFound || ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
}
//Priya: 5. Pass aServerAddress = Null
 
 TInt CSettingsApi::SetServerAddress_AddressNull(CStifItemParser& aItem)
    {
 /*
  Int SetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
  
  _LIT8(KServer, "suplserver.com & testcase5");
 	iLog->Log(_L("SetServerAddress is NULL"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	
	RBuf aServerAddress;
	aServerAddress.Create(256);
	aServerAddress.Append(KNullDesC);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->SetServerAddress(aSlpId,aServerAddress);
		iLog->Log(_L("EXPECTED RESULT: KErrArgument"));

iLog->Log(_L("Return Value of SetServer=%d" ),ret);

	delete aSettings;
	aServerAddress.Close();

	
		if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
}	

//Priya: 8. Pass invalid SLP address and invalid SLP id
 TInt CSettingsApi::SetServerAddress_InvalidAddressId(CStifItemParser& aItem)
    {
 /*
  Int SetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
  
 	iLog->Log(_L("SetServerAddress consists of invalid SLP id and invalid Server Address"));
	TInt64 aSlpId = 9080;
	
	RBuf aServerAddress;
	aServerAddress.Create(256);
	aServerAddress.Append(KNullDesC);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->SetServerAddress(aSlpId,aServerAddress);
		iLog->Log(_L("EXPECTED RESULT: KErrNotFound or KErrArgument"));

	iLog->Log(_L("Return Value of SetServer=%d" ),ret);
	
	delete aSettings;
	aServerAddress.Close();

	if(ret == KErrArgument || ret == KErrNotFound)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
}

//==============================================================GetServerAddress======================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================
 //Priya: 1. Pass valid id  
TInt CSettingsApi::GetServerAddress(CStifItemParser& aItem)
    {
 /*
  Int GetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
 iLog->Log(_L("GetServerAddress"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	
	RBuf aServerAddress;
	aServerAddress.Create(256);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
  	
	ret=aSettings->GetServerAddress(aSlpId,aServerAddress);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);

	aServerAddress.Close();
	delete aSettings;
	if(ret == KErrNone)
		{
			return 0;
		}	
	else
	{ 
		return -1;
	}	
 }
 
 TInt CSettingsApi::GetServerAddress(TInt64 aSlpId1)
{
 /*
  Int GetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
 iLog->Log(_L("GetServerAddress"));
	TInt64 aSlpId;
	aSlpId = aSlpId1;
	
	RBuf aServerAddress;
	aServerAddress.Create(256);
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
//	TRequestStatus aStatus;
 //  aSettings->Initialize(aStatus);
 //  User::WaitForRequest(aStatus);
  TInt ret;
  	
	ret=aSettings->GetServerAddress(aSlpId,aServerAddress);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);
	
	delete aSettings;
	
	TBuf<255> logMsg;
	logMsg.Append(_L("Server Id is: "));
	logMsg.AppendNum(aSlpId);
	logMsg.Append(_L("\nServer address"));
	logMsg.Append(aServerAddress);
	iLog->Log(logMsg);	
	
	return ret;
} 
 //Priya: 2. Pass invalid id  which is positive

TInt CSettingsApi::GetServerAddress_invalidpositive(CStifItemParser& aItem)
    {
 /*
  Int GetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
 	
 	iLog->Log(_L("GetServerAddress with ID which is invalid and positive"));
	
 	TInt64 aSlpId= 7979;
	
	RBuf aServerAddress;
	aServerAddress.Create(256);
	
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
  
    	iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	
	ret=aSettings->GetServerAddress(aSlpId,aServerAddress);
		
	iLog->Log(_L("Return Value=%d" ),ret);
//iLog->Log(_L("SLP id of the server: %d, Server address = %s" ),aSlpId,aServerAddress);
aServerAddress.Close();
	delete aSettings;
	if(ret == KErrArgument)
		{
			return 0;
		}	
	else
	{ 
		return -1;
	}	
 }

 //Priya: 3. Pass invalid id  which is negative

TInt CSettingsApi::GetServerAddress_invalidnegative(CStifItemParser& aItem)
    {
 /*
  Int GetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
 	
 	iLog->Log(_L("GetServerAddress with ID which is invalid and negative"));
	TInt64 aSlpId= -1987;
	
	RBuf aServerAddress;
	aServerAddress.Create(256);
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
  
  
  	iLog->Log(_L("EXPECTED RESULT: KErrNotFound or KErrArgument"));
	
	ret=aSettings->GetServerAddress(aSlpId,aServerAddress);
		
	iLog->Log(_L("Return Value=%d" ),ret);
//	iLog->Log(_L("SLP id of the server: %d, Server address = %s" ),aSlpId,aServerAddress);
	delete aSettings;
	aServerAddress.Close();
	if(ret == KErrArgument)
		{
			return 0;
		}	
	else
	{ 
		return -1;
	}	
 }

//Priya: 4. Pass invalid id which is zero
 TInt CSettingsApi::GetServerAddress_invalidzero(CStifItemParser& aItem)
    {
 /*
  Int GetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
  */
 	
 	iLog->Log(_L("GetServerAddress with ID which is invalid and negative"));
	 	TInt64 aSlpId= 0;
	
	RBuf aServerAddress;
	aServerAddress.Create(256);
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
  
  	iLog->Log(_L("EXPECTED RESULT: KErrNotFound or KErrArgument"));

	ret=aSettings->GetServerAddress(aSlpId,aServerAddress);
		
	iLog->Log(_L("Return Value=%d" ),ret);
	//iLog->Log(_L("SLP id of the server: %d, Server address = %s" ),aSlpId,aServerAddress);
	delete aSettings;
	aServerAddress.Close();
	if(ret == KErrArgument)
		{
			return 0;
		}	
	else
	{ 
		return -1;
	}	
 }
	
 
 
//============================================================ Set IAP Name ======================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================

//Priya: 1. Pass valid id and IAPName

TInt CSettingsApi::SetIAPName(CStifItemParser& aItem)
    {
 /*
  Int SetIAPName( const TInt64 aSlpId, const TDesC& aIAPName)
  */
 iLog->Log(_L("SetIAPName"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;

	_LIT8(KIAP, "changing the IAP name");
 	
	
	RBuf aIAPName;
	aIAPName.Create(256);
	aIAPName.Copy(KIAP);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret, ret1;

	ret=aSettings->SetIapName(aSlpId,aIAPName);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetIAPName=%d" ),ret);
	aIAPName.Close();
delete aSettings;
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}	
 }
 
 //Priya: 2. Pass invalid id and IAPName which is positive

TInt CSettingsApi::SetIAPName_invalidpositive(CStifItemParser& aItem)
    {
  	
 	iLog->Log(_L("SetIAPName which is invalid and positive"));
	_LIT8(KIAP, "suplserver1234& djfdjfdj.com");
 	TInt64 aSlpId=5683;
	
	RBuf aIAPName;
	aIAPName.Create(256);
	aIAPName.Copy(KIAP);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret, ret1;
  
  iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	
	ret=aSettings->SetIapName(aSlpId,aIAPName);
		
	iLog->Log(_L("Return Value of SetIAPName=%d" ),ret);

	aIAPName.Close();
	delete aSettings;
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
 }

 //Priya: 3. Pass invalid id and IAPName which is negative
TInt CSettingsApi::SetIAPName_invalidnegative(CStifItemParser& aItem)
    {
 
	iLog->Log(_L("SetIAPName which is invalid and negative"));
	_LIT8(KIAP, "suplserver1234& djfdjfdj.com");
 	TInt64 aSlpId=-5683;
	
	RBuf aIAPName;
	aIAPName.Create(256);
	aIAPName.Copy(KIAP);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret, ret1;
  
  iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	
	ret=aSettings->SetIapName(aSlpId,aIAPName);
		
	iLog->Log(_L("Return Value of SetIAPName=%d" ),ret);

	aIAPName.Close();
	delete aSettings;
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
 
}
//Priya: 4. Pass invalid id and IAPName which is zero
 
 TInt CSettingsApi::SetIAPName_invalidzero(CStifItemParser& aItem)
    {
   
	iLog->Log(_L("SetIAPName which is invalid and zero"));
	_LIT8(KIAP, "suplserver1234& djfdjfdj.com");
 	TInt64 aSlpId=0;
	
	RBuf aIAPName;
	aIAPName.Create(256);
	aIAPName.Copy(KIAP);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret, ret1;
  
  iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	
	ret=aSettings->SetIapName(aSlpId,aIAPName);
		
	iLog->Log(_L("Return Value of SetIAPName=%d" ),ret);

	aIAPName.Close();
	delete aSettings;
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
}

	//============================================================ Get IAP Name======================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================
 //Priya: 1. Pass valid id  
TInt CSettingsApi::GetIAPName(CStifItemParser& aItem)
    {

 iLog->Log(_L("GetIAPName"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	
	RBuf aIAPName;
	aIAPName.Create(256);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
  	
	ret=aSettings->GetIapName(aSlpId,aIAPName);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);
	//iLog->Log(_L("SLP id of the server: %d, Server address = %s" ),aSlpId,aIAPName);
		aIAPName.Close();
	delete aSettings;
	if(ret == KErrNone)
	{
			return 0;
	}
	else
	{
		return -1;
	}
 }
 //Get iap name with int:
 
 TInt CSettingsApi::GetIAPName(TInt64 aSlpId1)
  {

 iLog->Log(_L("GetIAPName"));
	
TInt64 aSlpId;
aSlpId = aSlpId1;
	
	RBuf aIAPName;
	aIAPName.Create(256);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
//	TRequestStatus aStatus;
 //  aSettings->Initialize(aStatus);
//   User::WaitForRequest(aStatus);
  TInt ret;
  	
	ret=aSettings->GetIapName(aSlpId,aIAPName);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of GetIAPName=%d" ),ret);
	
	TBuf<255> logMsg;
	logMsg.Append(_L("Server Id is: "));
	logMsg.AppendNum(aSlpId);
	logMsg.Append(_L("\nIAP Name"));
	logMsg.Append(aIAPName);
	
	iLog->Log(logMsg);
	
	delete aSettings;
	return ret;
 }
 
 //Priya: 2. Pass invalid id  which is positive

TInt CSettingsApi::GetIAPName_invalidpositive(CStifItemParser& aItem)
    {
 
 	iLog->Log(_L("GetIAPName with ID which is invalid and positive"));
	
 	TInt64 aSlpId=9099;
	
	RBuf aIAPName;
	aIAPName.Create(256);
	
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
  
    	iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	
	ret=aSettings->GetIapName(aSlpId,aIAPName);
		
	iLog->Log(_L("Return Value=%d" ),ret);
//iLog->Log(_L("SLP id of the server: %d, Server address = %s" ),aSlpId,aIAPName);
	aIAPName.Close();
	delete aSettings;
	if(ret == KErrArgument)
	{
			return 0;
	}
	else
	{
		return -1;
	}
 }

 //Priya: 3. Pass invalid id  which is negative

TInt CSettingsApi::GetIAPName_invalidnegative(CStifItemParser& aItem)
    {

 	iLog->Log(_L("GetIAPName with ID which is invalid and negative"));
	TInt64 aSlpId= -1987;
	
	RBuf aIAPName;
	aIAPName.Create(256);
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
  
  
  	iLog->Log(_L("EXPECTED RESULT: KErrNotFound or KErrArgument"));
	
	ret=aSettings->GetIapName(aSlpId,aIAPName);
		
	iLog->Log(_L("Return Value=%d" ),ret);
	//iLog->Log(_L("SLP id of the server: %d, Server address = %s" ),aSlpId,aIAPName);
		aIAPName.Close();
	delete aSettings;
	if(ret == KErrArgument)
	{
			return 0;
	}
	else
	{
		return -1;
	}
 }

//Priya: 4. Pass invalid id which is zero
 TInt CSettingsApi::GetIAPName_invalidzero(CStifItemParser& aItem)
    {
 
 	iLog->Log(_L("GetIAPName with ID which is invalid and negative"));
	 	TInt64 aSlpId=0;
	
	RBuf aIAPName;
	aIAPName.Create(256);
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
  
  	iLog->Log(_L("EXPECTED RESULT: KErrNotFound or KErrArgument"));

	ret=aSettings->GetIapName(aSlpId,aIAPName);
		
	iLog->Log(_L("Return Value=%d" ),ret);
//	iLog->Log(_L("SLP id of the server: %d, Server address = %s" ),aSlpId,aIAPName);
	aIAPName.Close();
	delete aSettings;
	if(ret == KErrArgument)
	{
			return 0;
	}
	else
	{
		return -1;
	}
 }
 
 //================== SetServerEnabledFlag=======================================================================================================
 
 //Priya: 1. Pass valid id and aEnable = ETrue

TInt CSettingsApi::SetServerEnabledFlag_Etrue(CStifItemParser& aItem)
    {
 /*
TInt SetServerEnabledFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("SetServerEnabledFlag with valid id and  aEnable = ETrue"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aEnable = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
		ret=aSettings->SetServerEnabledFlag(aSlpId,aEnable);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetServerEnabledFlag=%d" ),ret);

	delete aSettings;
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }
 
 //Priya: 2. Pass valid id and aEnable = EFalse

TInt CSettingsApi::SetServerEnabledFlag_EFalse(CStifItemParser& aItem)
    {
 /*
TInt SetServerEnabledFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("SetServerEnabledFlag with valid id and  aEnable = EFalse"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aEnable = EFalse;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetServerEnabledFlag(aSlpId,aEnable);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetServerEnabledFlag=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }

 //Priya: 3. Pass invalid id which is negative
TInt CSettingsApi::SetServerEnabledFlag_id_negative(CStifItemParser& aItem)
    {
 /*
TInt SetServerEnabledFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("SetServerEnabledFlag with invalid id which is negative"));
	TInt64 aSlpId = -1872;
	TBool aEnable = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetServerEnabledFlag(aSlpId,aEnable);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArguement"));
	iLog->Log(_L("Return Value of SetServerEnabledFlag=%d" ),ret);

	delete aSettings;
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }
 
  //Priya: 4. Pass invalid id which is Positive
TInt CSettingsApi::SetServerEnabledFlag_id_positive(CStifItemParser& aItem)
    {
 /*
TInt SetServerEnabledFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("SetServerEnabledFlag with invalid id which is positive"));
	TInt64 aSlpId = 1872;
	TBool aEnable = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetServerEnabledFlag(aSlpId,aEnable);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of SetServerEnabledFlag=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
//Priya: 4. Pass invalid id  which is zero
 
 TInt CSettingsApi::SetServerEnabledFlag_id_zero(CStifItemParser& aItem)
    {
 /*
TInt SetServerEnabledFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("SetServerEnabledFlag with invalid id which is zero"));
	TInt64 aSlpId = 0;
	TBool aEnable = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetServerEnabledFlag(aSlpId,aEnable);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of sEtServerEnabledFlag=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

	

//================== GetServerEnabledFlag=======================================================================================================
 

 TInt CSettingsApi::GetServerEnabledFlag_Etrue(CStifItemParser& aItem)
    {
 /*
TInt GetServerEnabledFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("GetServerEnabledFlag with valid id and  flag is set to ETrue"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aEnable;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
ret=aSettings->GetServerEnabledFlag(aSlpId,aEnable);
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);
	
		
	delete aSettings;
	if(ret == KErrNone)
	{
			return 0;
	}
	else
	{
		return -1;
	}
	
 }
 //Priya: 1. Pass valid id and flag is set to EFalse

TInt CSettingsApi::GetServerEnabledFlag_EFalse(CStifItemParser& aItem)
    {
 /*
TInt GetServerEnabledFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("GetServerEnabledFlag with valid id and  flag is set to EFalse"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aEnable;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
ret=aSettings->GetServerEnabledFlag(aSlpId,aEnable);
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);
	
		iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aEnable);
		
	delete aSettings;
	if(ret == KErrNone)
	{
			return 0;
	}
	else
	{
		return -1;
	}
 }

 //Priya: 3. Pass invalid id which is negative
TInt CSettingsApi::GetServerEnabledFlag_id_negative(CStifItemParser& aItem)
    {
 /*
TInt GetServerEnabledFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("GetServerEnabledFlag with invalid id which is negative"));
	TInt64 aSlpId = -98;
	TBool aEnable;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
ret=aSettings->GetServerEnabledFlag(aSlpId,aEnable);
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aEnable);
	delete aSettings;
	if(ret == KErrArgument)
	{
			return 0;
	}
	else
	{
		return -1;
	}
 }
 
//Priya: 4. Pass invalid id which is positive
TInt CSettingsApi::GetServerEnabledFlag_id_positive(CStifItemParser& aItem)
    {
 /*
TInt GetServerEnabledFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("GetServerEnabledFlag with invalid id which is negative"));
	TInt64 aSlpId = 1245;
	TBool aEnable;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
ret=aSettings->GetServerEnabledFlag(aSlpId,aEnable);
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aEnable);
	delete aSettings;
	if(ret == KErrArgument)
	{
			return 0;
	}
	else
	{
		return -1;
	}
 }
 
//Priya: 5. Pass invalid id  which is zero
 
TInt CSettingsApi::GetServerEnabledFlag_id_zero(CStifItemParser& aItem)
    {
 /*
TInt GetServerEnabledFlag(const TInt64 aSlpId, const TBool aEnable)
  */
 iLog->Log(_L("GetServerEnabledFlag with invalid id which is negative"));
	TInt64 aSlpId = 0;
	TBool aEnable;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
ret=aSettings->GetServerEnabledFlag(aSlpId,aEnable);
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aEnable);
	delete aSettings;
	if(ret == KErrArgument)
	{
			return 0;
	}
	else
	{
		return -1;
	}
 }
 
//================== SetUsageInHomwNwFlag=======================================================================================================
 
 //Priya: 1. Pass valid id and aHomeNwFlag = ETrue

TInt CSettingsApi::SetUsageInHomwNwFlag_Etrue(CStifItemParser& aItem)
    {
 /*
TInt SetUsageInHomwNwFlag(const TInt64 aSlpId, const TBool aHomeNwFlag)
  */
 iLog->Log(_L("SetUsageInHomwNwFlag with valid id and  aHomeNwFlag = ETrue"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;

	TBool aHomeNwFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
		ret=aSettings->SetUsageInHomwNwFlag(aSlpId,aHomeNwFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetUsageInHmNwFlag=%d" ),ret);
	
	delete aSettings;
	
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}		

 }
 
 //Priya: 1. Pass valid id and aHomeNwFlag = EFalse

TInt CSettingsApi::SetUsageInHomwNwFlag_EFalse(CStifItemParser& aItem)
    {
 /*
TInt SetUsageInHomwNwFlag(const TInt64 aSlpId, const TBool aHomeNwFlag)
  */
 iLog->Log(_L("SetUsageInHomwNwFlag with valid id and  aHomeNwFlag = EFalse"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aHomeNwFlag = EFalse;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetUsageInHomwNwFlag(aSlpId,aHomeNwFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetUsageInHmNwFlag=%d" ),ret);
	
		delete aSettings;
	
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
 }

 //Priya: 3. Pass invalid id which is negative
TInt CSettingsApi::SetUsageInHomwNwFlag_id_negative(CStifItemParser& aItem)
    {
 /*
TInt SetUsageInHomwNwFlag(const TInt64 aSlpId, const TBool aHomeNwFlag)
  */
 iLog->Log(_L("SetUsageInHomwNwFlag with invalid id which is negative"));
	TInt64 aSlpId = -1872;
	TBool aHomeNwFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetUsageInHomwNwFlag(aSlpId,aHomeNwFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of SetUsageInHmNwFlag=%d" ),ret);
	
	delete aSettings;
	
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
 }
 
  //Priya: 4. Pass invalid id which is Positive
TInt CSettingsApi::SetUsageInHomwNwFlag_id_positive(CStifItemParser& aItem)
    {
 /*
TInt SetUsageInHomwNwFlag(const TInt64 aSlpId, const TBool aHomeNwFlag)
  */
 iLog->Log(_L("SetUsageInHomwNwFlag with invalid id which is positive"));
	TInt64 aSlpId = 1872;
	TBool aHomeNwFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetUsageInHomwNwFlag(aSlpId,aHomeNwFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of SetUsageInHmNwFlag=%d" ),ret);

	delete aSettings;
	
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
}
//Priya: 4. Pass invalid id  which is zero
 
 TInt CSettingsApi::SetUsageInHomwNwFlag_id_zero(CStifItemParser& aItem)
    {
 /*
TInt SetUsageInHomwNwFlag(const TInt64 aSlpId, const TBool aHomeNwFlag)
  */
 iLog->Log(_L("SetUsageInHomwNwFlag with invalid id which is zero"));
	TInt64 aSlpId = 0;
	TBool aHomeNwFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetUsageInHomwNwFlag(aSlpId,aHomeNwFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of SetUsageInHmNwFlag=%d" ),ret);
	
	delete aSettings;
	
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
}



//================== GetUsageInHomwNwFlag=======================================================================================================
 
 //Priya: 1. Pass valid id and flag is set to ETrue

TInt CSettingsApi::GetUsageInHomwNwFlag_Etrue(CStifItemParser& aItem)
    {
 /*
TInt GetUsageInHomwNwFlag(const TInt64 aSlpId, const TBool aHomeNwFlag)
  */
 iLog->Log(_L("GetUsageInHomwNwFlag with valid id and  flag is set to ETrue"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aHomeNwFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;

		ret=aSettings->GetUsageInHomwNwFlag(aSlpId,aHomeNwFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);
		iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aHomeNwFlag);
	
	
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else
		return -1;
 }
 


 //Priya: 1. Pass valid id and flag is set to EFalse

TInt CSettingsApi::GetUsageInHomwNwFlag_EFalse(CStifItemParser& aItem)
    {
 /*
TInt GetUsageInHomwNwFlag(const TInt64 aSlpId, const TBool aHomeNwFlag)
  */
 iLog->Log(_L("GetUsageInHomwNwFlag with valid id and  flag is set to EFalse"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aHomeNwFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
	
		ret=aSettings->GetUsageInHomwNwFlag(aSlpId,aHomeNwFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);
	
		iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aHomeNwFlag);
	
	
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else
		return -1;
 }

 //Priya: 3. Pass invalid id which is negative
TInt CSettingsApi::GetUsageInHomwNwFlag_id_negative(CStifItemParser& aItem)
    {
 /*
TInt GetUsageInHomwNwFlag(const TInt64 aSlpId, const TBool aHomeNwFlag)
  */
 iLog->Log(_L("GetUsageInHomwNwFlag with invalid id which is negative"));
	TInt64 aSlpId = -98;
	TBool aHomeNwFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;

	
		ret=aSettings->GetUsageInHomwNwFlag(aSlpId,aHomeNwFlag);

	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aHomeNwFlag);
	delete aSettings;
	if(ret == KErrArgument)
		return 0;
	else
		return -1;
 }
 
//Priya: 4. Pass invalid id which is positive
TInt CSettingsApi::GetUsageInHomwNwFlag_id_positive(CStifItemParser& aItem)
    {
 /*
TInt GetUsageInHomwNwFlag(const TInt64 aSlpId, const TBool aHomeNwFlag)
  */
 iLog->Log(_L("GetUsageInHomwNwFlag with invalid id which is negative"));
	TInt64 aSlpId = 1245;
	TBool aHomeNwFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
	
		ret=aSettings->GetUsageInHomwNwFlag(aSlpId,aHomeNwFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aHomeNwFlag);
	delete aSettings;
	if(ret == KErrArgument)
		return 0;
	else
		return -1;
 }
 
//Priya: 5. Pass invalid id  which is zero
 
TInt CSettingsApi::GetUsageInHomwNwFlag_id_zero(CStifItemParser& aItem)
    {
 /*
TInt GetUsageInHomwNwFlag(const TInt64 aSlpId, const TBool aHomeNwFlag)
  */
 iLog->Log(_L("GetUsageInHomwNwFlag with invalid id which is negative"));
	TInt64 aSlpId = 0;
	TBool aHomeNwFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
	
		ret=aSettings->GetUsageInHomwNwFlag(aSlpId,aHomeNwFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aHomeNwFlag);
	delete aSettings;
	if(ret == KErrArgument)
		return 0;
	else
		return -1;
 }


 
//================== SetSimChangeRemoveFlag=======================================================================================================
 
 //Priya: 1. Pass valid id and aEditFlag = ETrue

TInt CSettingsApi::SetSimChangeRemoveFlag_Etrue(CStifItemParser& aItem)
    {
 /*
TInt SetSimChangeRemoveFlag(const TInt64 aSlpId, const TBool aSimChangeFlag);
  */
 iLog->Log(_L("SetSimChangeRemoveFlag with valid id and  aSimChangeFlag = ETrue"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;

	TBool aSimChangeFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
		ret=aSettings->SetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetSimChangeRemoveFlag=%d" ),ret);
	
	delete aSettings;
	
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
	
 }
 
 //Priya: 1. Pass valid id and aSimChangeFlag = EFalse

TInt CSettingsApi::SetSimChangeRemoveFlag_EFalse(CStifItemParser& aItem)
    {
 /*
TInt SetSimChangeRemoveFlag(const TInt64 aSlpId, const TBool aSimChangeFlag)
  */
 iLog->Log(_L("SetSimChangeRemoveFlag with valid id and  aSimChangeFlag = EFalse"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aSimChangeFlag = EFalse;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->SetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetUsageInHmNwFlag=%d" ),ret);
	
	delete aSettings;
	
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
 }

 //Priya: 3. Pass invalid id which is negative
TInt CSettingsApi::SetSimChangeRemoveFlag_id_negative(CStifItemParser& aItem)
    {
 /*
TInt SetSimChangeRemoveFlag(const TInt64 aSlpId, const TBool aSimChangeFlag)
  */
 iLog->Log(_L("SetSimChangeRemoveFlag with invalid id which is negative"));
	TInt64 aSlpId = -1872;
	TBool aSimChangeFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->SetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of SetUsageInHmNwFlag=%d" ),ret);
	
	delete aSettings;
	
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
 }
 
  //Priya: 4. Pass invalid id which is Positive
TInt CSettingsApi::SetSimChangeRemoveFlag_id_positive(CStifItemParser& aItem)
    {
 /*
TInt SetSimChangeRemoveFlag(const TInt64 aSlpId, const TBool aSimChangeFlag)
  */
 iLog->Log(_L("SetSimChangeRemoveFlag with invalid id which is positive"));
	TInt64 aSlpId = 1872;
	TBool aSimChangeFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->SetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of SetUsageInHmNwFlag=%d" ),ret);
	
	delete aSettings;
	
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
}
//Priya: 4. Pass invalid id  which is zero
 
 TInt CSettingsApi::SetSimChangeRemoveFlag_id_zero(CStifItemParser& aItem)
    {
 /*
TInt SetSimChangeRemoveFlag(const TInt64 aSlpId, const TBool aSimChangeFlag)
  */
 iLog->Log(_L("SetSimChangeRemoveFlag with invalid id which is zero"));
	TInt64 aSlpId = 0;
	TBool aSimChangeFlag = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->SetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of SetUsageInHmNwFlag=%d" ),ret);
	
	delete aSettings;
	
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
}



//================== GetSimChangeRemoveFlag=======================================================================================================
 
 //Priya: 1. Pass valid id and flag is set to ETrue

TInt CSettingsApi::GetSimChangeRemoveFlag_Etrue(CStifItemParser& aItem)
    {
 /*
TInt GetSimChangeRemoveFlag(const TInt64 aSlpId, TBool& aSimChangeFlag) const;
  */
 iLog->Log(_L("GetSimChangeRemoveFlag with valid id and  flag is set to ETrue"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aSimChangeFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;

		ret=aSettings->GetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);
		iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aSimChangeFlag);
		
	delete aSettings;
	if( ret == KErrNone)
		return 0;
	else
	return -1;
 }
 
 TInt CSettingsApi::GetSimChangeRemoveFlag(TInt64 aSlpId1)
    {
 /*
TInt GetSimChangeRemoveFlag(const TInt64 aSlpId, const TBool aSimChangeFlag)
  */
 iLog->Log(_L("GetSimChangeRemoveFlag"));
	
TInt64 aSlpId;
aSlpId = aSlpId1;
	TBool aSimChangeFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	//TRequestStatus aStatus;
  // aSettings->Initialize(aStatus);
  // User::WaitForRequest(aStatus);
  TInt ret;

	ret=aSettings->GetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of GetUsageInHmNwFlag=%d" ),ret);
	
	TBuf<255> logMsg;
	logMsg.Append(_L("Server Id is: "));
	logMsg.AppendNum(aSlpId);
	logMsg.Append(_L("\nHome Nw Flag:"));
	logMsg.AppendNum(aSimChangeFlag);
	
	iLog->Log(logMsg);
	
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else
		return -1;
 }
 
 //Priya: 1. Pass valid id and flag is set to EFalse

TInt CSettingsApi::GetSimChangeRemoveFlag_EFalse(CStifItemParser& aItem)
    {
 /*
TInt GetSimChangeRemoveFlag(const TInt64 aSlpId, const TBool aSimChangeFlag)
  */
 iLog->Log(_L("GetSimChangeRemoveFlag with valid id and  flag is set to EFalse"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aSimChangeFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;

	
		ret=aSettings->GetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);

	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);
	
		iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aSimChangeFlag);
	
	
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else
		return -1;
 }

 //Priya: 3. Pass invalid id which is negative
TInt CSettingsApi::GetSimChangeRemoveFlag_id_negative(CStifItemParser& aItem)
    {
 /*
TInt GetSimChangeRemoveFlag(const TInt64 aSlpId, const TBool aSimChangeFlag)
  */
 iLog->Log(_L("GetSimChangeRemoveFlag with invalid id which is negative"));
	TInt64 aSlpId = -98;
	TBool aSimChangeFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;

	
		ret=aSettings->GetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);

	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aSimChangeFlag);
	delete aSettings;
	if(ret == KErrArgument)
		return 0;
	else
		return -1;
 }
 
//Priya: 4. Pass invalid id which is positive
TInt CSettingsApi::GetSimChangeRemoveFlag_id_positive(CStifItemParser& aItem)
    {
 /*
TInt GetSimChangeRemoveFlag(const TInt64 aSlpId, const TBool aSimChangeFlag)
  */
 iLog->Log(_L("GetSimChangeRemoveFlag with invalid id which is negative"));
	TInt64 aSlpId = 1245;
	TBool aSimChangeFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
	
		ret=aSettings->GetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aSimChangeFlag);
	delete aSettings;
	if(ret == KErrArgument)
		return 0;
	else
		return -1;
 }
 
//Priya: 5. Pass invalid id  which is zero
 
TInt CSettingsApi::GetSimChangeRemoveFlag_id_zero(CStifItemParser& aItem)
    {
 /*
TInt GetSimChangeRemoveFlag(const TInt64 aSlpId, const TBool aSimChangeFlag)
  */
 iLog->Log(_L("GetSimChangeRemoveFlag with invalid id which is negative"));
	TInt64 aSlpId = 0;
	TBool aSimChangeFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
	
		ret=aSettings->GetSimChangeRemoveFlag(aSlpId,aSimChangeFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("Server id= %d , Flag set status = "),aSlpId, aSimChangeFlag);
	delete aSettings;
	if(ret == KErrArgument)
		return 0;
	else
		return -1;
 }


//================== GetEditableFlag=======================================================================================================
 
 //Priya: 1. Pass valid id and flag is set to ETrue

TInt CSettingsApi::GetEditableFlag_Etrue(CStifItemParser& aItem)
    {
 /*
TInt GetEditableFlag(const TInt64 aSlpId, TBool& aEditFlag ) const;       
  */
 iLog->Log(_L("GetEditableFlag with valid id and  flag is set to ETrue"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aEditFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  
  TInt ret;

		ret=aSettings->GetEditableFlag(aSlpId,aEditFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));

	iLog->Log(_L("Return Value=%d" ),ret);
	TBuf<255> logMsg;
	logMsg.Append(_L("Server Id is: "));
	logMsg.AppendNum(aSlpId);
	logMsg.Append(_L("\nEditable Flag:"));
	logMsg.AppendNum(aEditFlag);
	iLog->Log(logMsg);
	
	
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else 
		return -1;
 }
 
 
 TInt CSettingsApi::GetEditableFlag(TInt64 aSlpId1)
    {
 /*
TInt GetEditableFlag(const TInt64 aSlpId, TBool& aEditFlag ) const;       
  */
 iLog->Log(_L("GetEditableFlag"));
	
TInt64 aSlpId;
aSlpId = aSlpId1;
	TBool aEditFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  //TRequestStatus aStatus;
  // aSettings->Initialize(aStatus);
  // User::WaitForRequest(aStatus);
  TInt ret;

		ret=aSettings->GetEditableFlag(aSlpId,aEditFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));

	iLog->Log(_L("Return Value of GetEditableFlag=%d" ),ret);
	TBuf<255> logMsg;
	logMsg.Append(_L("Server Id is: "));
	logMsg.AppendNum(aSlpId);
	logMsg.Append(_L("\nEditable Flag:"));
	logMsg.AppendNum(aEditFlag);
	iLog->Log(logMsg);
		
	delete aSettings;
	return ret;
 }

 //Priya: 1. Pass valid id and flag is set to EFalse

TInt CSettingsApi::GetEditableFlag_EFalse(CStifItemParser& aItem)
    {
 /*
TInt GetEditableFlag(const TInt64 aSlpId, const TBool aEditFlag)
  */
 iLog->Log(_L("GetEditableFlag with valid id and  flag is set to EFalse"));
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
	TBool aEditFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;

		ret=aSettings->GetEditableFlag(aSlpId,aEditFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);
	
		TBuf<255> logMsg;
	logMsg.Append(_L("Server Id is: "));
	logMsg.AppendNum(aSlpId);
	logMsg.Append(_L("\nEdit flag:"));
	logMsg.AppendNum(aEditFlag);
	iLog->Log(logMsg);
	
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else 
		return -1;
 }

 //Priya: 3. Pass invalid id which is negative
TInt CSettingsApi::GetEditableFlag_id_negative(CStifItemParser& aItem)
    {
 /*
TInt GetEditableFlag(const TInt64 aSlpId, const TBool aEditFlag)
  */
 iLog->Log(_L("GetEditableFlag with invalid id which is negative"));
	TInt64 aSlpId = -98;
	TBool aEditFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;

			ret=aSettings->GetEditableFlag(aSlpId,aEditFlag);

	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	TBuf<255> logMsg;
	logMsg.Append(_L("Server Id is: "));
	logMsg.AppendNum(aSlpId);
	logMsg.Append(_L("\nEdit flag:"));
	logMsg.AppendNum(aEditFlag);
	iLog->Log(logMsg);
	delete aSettings;
	if(ret == KErrArgument || ret == KErrNotFound)
		return 0;
	else 
		return -1;
 }
 
//Priya: 4. Pass invalid id which is positive
TInt CSettingsApi::GetEditableFlag_id_positive(CStifItemParser& aItem)
    {
 /*
TInt GetEditableFlag(const TInt64 aSlpId, const TBool aEditFlag)
  */
 iLog->Log(_L("GetEditableFlag with invalid id which is negative"));
	TInt64 aSlpId = 1245;
	TBool aEditFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
		ret=aSettings->GetEditableFlag(aSlpId,aEditFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	TBuf<255> logMsg;
	logMsg.Append(_L("Server Id is: "));
	logMsg.AppendNum(aSlpId);
	logMsg.Append(_L("\nEdit flag:"));
	logMsg.AppendNum(aEditFlag);
	iLog->Log(logMsg);
	delete aSettings;
	if(ret == KErrNotFound || ret == KErrArgument)
		return 0;
	else 
		return -1;
 }
 
//Priya: 5. Pass invalid id  which is zero
 
TInt CSettingsApi::GetEditableFlag_id_zero(CStifItemParser& aItem)
    {
 /*
TInt GetEditableFlag(const TInt64 aSlpId, const TBool aEditFlag)
  */
 iLog->Log(_L("GetEditableFlag with invalid id which is negative"));
	TInt64 aSlpId = 0;
	TBool aEditFlag;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
		ret=aSettings->GetEditableFlag(aSlpId,aEditFlag);
	
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	TBuf<255> logMsg;
	logMsg.Append(_L("Server Id is: "));
	logMsg.AppendNum(aSlpId);
	logMsg.Append(_L("\nEdit flag:"));
	logMsg.AppendNum(aEditFlag);
	iLog->Log(logMsg);
	delete aSettings;
	
	if(ret == KErrArgument || ret == KErrNotFound)
		return 0;
	else 
		return -1;
 }



//============ ADD SERVER ================================================================================================================================================================================
// priya 1. add server with default arguements 
TInt CSettingsApi::AddNewServerWithDefault(CStifItemParser& aItem)
    {
  /*  	
 AddNewServer( const CServerParams aParamValues, TInt64& aSlpId );

IMPORT_C TInt Set(
                const TDesC& aServerAddress,
                const TDesC& aIapName,
                const TBool aServerEnabled = ETrue,
                const TBool aSimChangeRemove = EFalse,
                const TBool aUsageInHomeNw = ETrue,
                const TBool aEditable = ETrue,
                const TInt64 aSlpId = 0
                );
              
*/

//Input parameters for Add server:
CServerParams *aParamValues= CServerParams::NewL();

TInt64 aSlpId;

iLog->Log(_L("Add new Server with Default parameters")); 
_LIT8(KServer, "supl.nokia.com");
_LIT8(KIap, "airtelgprs.com");
	
RBuf aServerAddress, aIapName; 	
		
aServerAddress.Create(256);
aServerAddress.Copy(KServer);
aIapName.Create(256);
aIapName.Copy(KIap);

aParamValues->Set(aServerAddress,aIapName,ETrue,ETrue,ETrue,ETrue);
	
  CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret,ret1;

	ret=aSettings->AddNewServer(aParamValues,aSlpId);
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("SLP id of the new server: %d" ),aSlpId);
	
  
	aServerAddress.Close();
  aIapName.Close();
	
	delete aSettings;
	
	if( ret == KErrNone) 
	{
			return 0;
	}
	else
	{
		return -1;
	}	
}


// priya 2. add server by changing the default arguements

TInt CSettingsApi::AddNewServerWithoutDefault(CStifItemParser& aItem)

{
CServerParams *aParamValues= CServerParams::NewL();

TInt64 aSlpId;

iLog->Log(_L("Add new Server by changing default parameters")); 
_LIT8(KServer, "suplserver.com");
_LIT8(KIap, "airtelgprs.com");
	
RBuf aServerAddress, aIapName; 	
		
aServerAddress.Create(256);
aServerAddress.Copy(KServer);
aIapName.Create(256);
aIapName.Copy(KIap);

aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,ETrue);
	
  CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret,ret1;

	ret=aSettings->AddNewServer(aParamValues,aSlpId);
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("SLP id of the new server: %d" ),aSlpId);
	
  
	aServerAddress.Close();
  aIapName.Close();
	
	delete aSettings;
	
	if( ret == KErrNone) 
	{
			return 0;
	}
	else
	{
		return -1;
	}	
}

// priya 10. add server with name and other details already present

TInt CSettingsApi::AddNewServer_Duplicate(CStifItemParser& aItem)
    {
  
CServerParams *aParamValues= CServerParams::NewL();

TInt64 aSlpId;

iLog->Log(_L("Add new Server by changing default parameters")); 
_LIT8(KServer, "suplserver.com");
_LIT8(KIap, "airtelgprs.com");
	
RBuf aServerAddress, aIapName; 	
		
aServerAddress.Create(256);
aServerAddress.Copy(KServer);
aIapName.Create(256);
aIapName.Copy(KIap);

aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,ETrue);
	
  CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret,ret1;
	
	ret=aSettings->AddNewServer(aParamValues,aSlpId);
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("SLP id of the new server: %d" ),aSlpId);
	
  		
	aServerAddress.Close();
  aIapName.Close();
	
	delete aSettings;
	
	if( ret == KErrAlreadyExists) 
	{
			return 0;
	}
	else
	{
		return -1;
	}	
}

// priya 4.aserveraddress = null 
TInt CSettingsApi::AddNewServer_aServerAddress_Null(CStifItemParser& aItem)
{
iLog->Log(_L("Add new server with aserver having NULL value"));
CServerParams *aParamValues= CServerParams::NewL();

TInt64 aSlpId;

_LIT8(KIap, "Airtel");

	RBuf aServerAddress, aIapName; 	
		
	aServerAddress.Create(256);
  aServerAddress.Append(KNullDesC);
  aIapName.Create(256);
	aIapName.Copy(KIap);

aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,EFalse);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret, ret1;
	
	ret=aSettings->AddNewServer(aParamValues,aSlpId);
	iLog->Log(_L("Expected Result: KErrArgument"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("SLP id of the new server: %d" ),aSlpId);
	
	aServerAddress.Close();
  aIapName.Close();
	
	delete aSettings;
	
	
	if( ret == KErrArgument || ret == KErrGeneral) 
	{
			return 0;
	}
	else
	{
		return -1;
	}	
	
}

// priya 6.aIapName = null

TInt CSettingsApi::AddNewServer_aIapNAme_Null(CStifItemParser& aItem)
    {
  
iLog->Log(_L("Add new server with aIapName having NULL value"));
CServerParams *aParamValues= CServerParams::NewL();

TInt64 aSlpId;

_LIT8(KServer, "Airtel.nokia.com");

	RBuf aServerAddress, aIapName; 	
		
	aServerAddress.Create(256);
  aServerAddress.Copy(KServer);
  aIapName.Create(256);
	aIapName.Append(KNullDesC);

aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,EFalse);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret,ret1;
	
	ret=aSettings->AddNewServer(aParamValues,aSlpId);
	iLog->Log(_L("Expected Result: KErrNone"));
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("SLP id of the new server: %d" ),aSlpId);
	
	aServerAddress.Close();
  aIapName.Close();
	

	aServerAddress.Close();
  aIapName.Close();
	
	delete aSettings;
	if( ret == KErrNone) 
	{
			return 0;
	}
	else
	{
		return -1;
	}	
	
}

// ======================= Set default Server =====================================================================================================
 
// priya 1. Set Default Server with default arguements 
//Priya: URGENT 12. Set Default Server when no server is default... Here first slp in the list is set
//Priya: URGENT 12. Set Default Server when no server is there in the list... Return value is KErrNotFound 
TInt CSettingsApi::SetDefaultServerWithDefaultValues(CStifItemParser& aItem)
{
  /*  	
Int SetDefaultServer( const CServerParams aParamValues);      
 
IMPORT_C TInt Set(
                const TDesC& aServerAddress,
                const TDesC& aIapName,
                const TBool aServerEnabled = ETrue,
                const TBool aSimChangeRemove = EFalse,
                const TBool aUsageInHomeNw = ETrue,
                const TBool aEditable = ETrue,
                const TInt64 aSlpId = 0
                ); 
              
*/

//Input parameters for SetDefaultServer:

CServerParams *aParamValues= CServerParams::NewL();


	_LIT8(KServer, "setting.default.case1");
_LIT8(KIap, "Airtel_case1");
	
	RBuf aServerAddress, aIapName;
			
	aServerAddress.Create(256);
  aServerAddress.Copy(KServer);
  aIapName.Create(256);
	aIapName.Copy(KIap);
		
aParamValues->Set(aServerAddress,aIapName);

	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetDefaultServer(aParamValues);
	iLog->Log(_L("Expected result: KErrNone"));
	iLog->Log(_L("Return Value of SetDefaultServer=%d" ),ret);
	
	aServerAddress.Close();
  aIapName.Close();
	
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else
		return -1;

}



// priya 3.aserveraddress = null 
TInt CSettingsApi::SetDefaultServer_aServerAddress_Null(CStifItemParser& aItem)
    {
  
iLog->Log(_L("Set Default server with aserver having NULL value"));
CServerParams *aParamValues= CServerParams::NewL();

TInt64 aSlpId;

_LIT8(KIap, "Airtel   checking null");

	RBuf aServerAddress, aIapName;

	aServerAddress.Create(256);
  aServerAddress.Append(KNullDesC);
  aIapName.Create(256);
	aIapName.Copy(KIap);

aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,EFalse,aSlpId);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetDefaultServer(aParamValues);
	iLog->Log(_L("Expected Result: KErrArgument"));
	iLog->Log(_L("Return Value of SetDefaultSErver=%d" ),ret);
	
	aServerAddress.Close();
  aIapName.Close();

	delete aSettings;
	if(ret == KErrArgument || ret == KErrGeneral)
		return 0;
	else
		return -1;
}


//=========================================== GetDefaultServer ===============================================================================

//Priya 1. Execute this when a server is set as default: OUTPUT KErrNone
//Priya 2. Execute this when no server is set as default: OUTPUT KErrNone...the first slp in the list is retrieved
//Priya 3. Execute this when there is no server in the settings: OUTPUT KErrNotFound

TInt CSettingsApi::GetDefaultServer()
    {
 /*
 
 IMPORT_C TInt Get(
                TInt64& aSlpId,                    
                TPtr aServerAddress,
                TPtr aIapName,
                TBool& aServerEnabled,
                TBool& aSimChangeRemove,
                TBool& aUsageInHomeNw,
                TBool& aEditable
                ) const;
  */
 
 
	CServerParams *aParamValues = CServerParams::NewL() ;
	
	TInt64 aSlpId;
		iLog->Log(_L("Get Default Server")); 
	HBufC* aServerAddress = HBufC::NewL(255);
	HBufC* aIapName = HBufC::NewL(255);
	
	TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->GetDefaultServer(aParamValues);
	
	aParamValues->Get(aSlpId,aServerAddress->Des(),aIapName->Des(),aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable);
		iLog->Log(_L("Return Value of GetDefaultSErver=%d" ),ret);
	
	delete aSettings;
	return ret;
 }
 
//=========================================================================================================================================================================================================

// ======================= Set All Parameters =====================================================================================================
 
// priya 1. SetAllParameter with default arguements 

TInt CSettingsApi::SetAllParameter_WithDefaultValues(CStifItemParser& aItem)
{
  /*  	
TInt SetAllParameter( const CServerParams aParamValues):-
 
 
 IMPORT_C TInt Set(
                const TDesC& aServerAddress,
                const TDesC& aIapName,
                const TBool aServerEnabled = ETrue,
                const TBool aSimChangeRemove = EFalse,
                const TBool aUsageInHomeNw = ETrue,
                const TBool aEditable = ETrue,
                const TInt64 aSlpId = 0
                );
              
*/

//Input parameters for SetAllParameter:
iLog->Log(_L("Set All Parameters with default values"));
CServerParams *aParamValues= CServerParams::NewL();
TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;
_LIT8(KServer, "suplserverorange.com");
_LIT8(KIap, "Airtelgprs.com");

	RBuf aServerAddress, aIapName;
	
	aServerAddress.Create(256);
  aServerAddress.Copy(KServer);
  aIapName.Create(256);
	aIapName.Copy(KIap);
	
aParamValues->Set(aServerAddress,aIapName,EFalse,EFalse,EFalse,EFalse,aSlpId);

	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret,ret1;
	
	ret=aSettings->SetAllParameter(aParamValues);
	iLog->Log(_L("Expected Result: KErrNone"));
	iLog->Log(_L("Return Value for SetAllParameters=%d" ),ret);
	
	aServerAddress.Close();
  aIapName.Close();

	delete aSettings;
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}		
}

// priya 2. SetAllParameter giving SLP Id  invalid  and positive

TInt CSettingsApi::SetAllParameter_Invalid_Positive(CStifItemParser& aItem)
    {
  iLog->Log(_L("Set All Parameters by giving SLP id invalid and positive"));
CServerParams *aParamValues= CServerParams::NewL();

TInt64 aSlpId = 8902;

_LIT8(KServer, "61.95.200.231");
_LIT8(KIap, "Airtel");
	
	RBuf aServerAddress,aIapName; 	
		
	aServerAddress.Create(256);
  aServerAddress.Copy(KServer);
  aIapName.Create(256);
	aIapName.Copy(KIap);
	
aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,EFalse,aSlpId);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
	ret=aSettings->SetAllParameter(aParamValues);
	iLog->Log(_L("Expected Result: KErrNotFound"));
	iLog->Log(_L("Return Value for SetAllParameters=%d" ),ret);
		
	aServerAddress.Close();
  aIapName.Close();
	
	delete aSettings;
	if(ret == KErrNotFound)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

// priya 2. SetAllParameter giving SLP Id  invalid  and negative
TInt CSettingsApi::SetAllParameter_Invalid_Negative(CStifItemParser& aItem)
    {
  iLog->Log(_L("Set All Parameters by giving SLP id invalid and negative"));
CServerParams *aParamValues= CServerParams::NewL();

TInt64 aSlpId = -8902;

_LIT8(KServer, "61.95.200.231");
_LIT8(KIap, "Airtel");

	RBuf aServerAddress, aIapName;
	
	aServerAddress.Create(256);
  aServerAddress.Copy(KServer);
  aIapName.Create(256);
	aIapName.Copy(KIap);

aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,EFalse,aSlpId);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();
	//TRequestStatus aStatus;
 //  aSettings->Initialize(aStatus);
 //  User::WaitForRequest(aStatus);
  TInt ret;
	
	ret=aSettings->SetAllParameter(aParamValues);
	iLog->Log(_L("Expected Result: KErrNotFound"));
	iLog->Log(_L("Return Value for SetAllParameters=%d" ),ret);
	
	aServerAddress.Close();
  aIapName.Close();

	delete aSettings;
	if(ret == KErrNotFound)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

// priya 2. SetAllParameter giving SLP Id  invalid  and Zero
TInt CSettingsApi::SetAllParameter_Invalid_Zero(CStifItemParser& aItem)
    {
  iLog->Log(_L("Set All Parameters by giving SLP id invalid and zero"));
CServerParams *aParamValues= CServerParams::NewL();

TInt64 aSlpId = 0;

_LIT8(KServer, "61.95.200.231");
_LIT8(KIap, "Airtel");
	
	RBuf aServerAddress, aIapName;

	aServerAddress.Create(256);
  aServerAddress.Copy(KServer);
  aIapName.Create(256);
	aIapName.Copy(KIap);

aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,EFalse,aSlpId);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetAllParameter(aParamValues);
	iLog->Log(_L("Expected Result: KErrNotFound"));
	iLog->Log(_L("Return Value for SetAllParameters=%d" ),ret);
	
	aServerAddress.Close();
  aIapName.Close();
	
	delete aSettings;
	if(ret == KErrNotFound)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}


// priya 4.aserveraddress = null 
TInt CSettingsApi::SetAllParameter_aServerAddress_Null(CStifItemParser& aItem)
    {
  iLog->Log(_L("Set All Parameters by giving serveraddress NULL"));

CServerParams *aParamValues= CServerParams::NewL();
TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;

_LIT8(KIap, "Airtel");

	RBuf aServerAddress, aIapName;
	
	aServerAddress.Create(256);
  aServerAddress.Append(KNullDesC);
  aIapName.Create(256);
	aIapName.Copy(KIap);

aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,EFalse,aSlpId);
	
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->SetAllParameter(aParamValues);
	iLog->Log(_L("Expected Result: KErrArgument"));
	iLog->Log(_L("Return Value for SetAllParameters=%d" ),ret);
	TInt ret1;
	
	aServerAddress.Close();
  aIapName.Close();

	delete aSettings;
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}


//---------------------------------- IS SLP EXISTS --------------------------------------------------------------------------
//Priya : 1. Is SLP Exists by passing the correct SLP id:
TBool CSettingsApi::IsSLPExists(CStifItemParser& aItem)
    {
 /*
TBool IsSlpExists(const TInt64 aSlpId )
  */
 iLog->Log(_L("IsSLPExists with valid id"));
	TInt64 aSlpId;
	TInt aSlpId1;
	aItem.GetNextInt(aSlpId1);
	aSlpId = aSlpId1;

	CSuplSettings *aSettings=CSuplSettings::NewL();
  TBool ret;

	ret=aSettings->IsSlpExists(aSlpId);

	iLog->Log(_L("EXPECTED RESULT: ETrue"));
	iLog->Log(_L("Return Value=%d" ),ret);
		
	delete aSettings;
	if(ret)
		return 0;
	else 
		return -1;
	
 }

//Priya : 2. Is SLP Exists by passing invalid SLP id which is positive
TBool CSettingsApi::IsSLPExists_invalid_positive(CStifItemParser& aItem)
    {
 /*
TBool IsSlpExists(const TInt64 aSlpId )
  */
 iLog->Log(_L("IsSLPExists with invalid id which is positive"));
	TInt64 aSlpId=3838;
			
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TBool ret;
	
	ret=aSettings->IsSlpExists(aSlpId);
	
	iLog->Log(_L("EXPECTED RESULT: EFalse"));
	iLog->Log(_L("Return Value=%d" ),ret);
	
	delete aSettings;
	
	if (ret)
		return -1;
	else 
		return 0;
 }
 
 //Priya : 3. Is SLP Exists by passing the invalid SLP id which is negative
TBool CSettingsApi::IsSLPExists_invalid_negative(CStifItemParser& aItem)
    {
 /*
TBool IsSlpExists(const TInt64 aSlpId )
  */
 iLog->Log(_L("IsSLPExists with invalid id which is negative"));
	TInt64 aSlpId=-1991;
			
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TBool ret;
	
	ret=aSettings->IsSlpExists(aSlpId);
	
	iLog->Log(_L("EXPECTED RESULT: EFalse"));
	iLog->Log(_L("Return Value=%d" ),ret);

	delete aSettings;
	
if(ret)
		return -1;
	else 
		return 0;
 }
 
 //Priya : 4. Is SLP Exists by passing the invalid slp id which is zero
TBool CSettingsApi::IsSLPExists_invalid_zero(CStifItemParser& aItem)
    {
 /*
TBool IsSlpExists(const TInt64 aSlpId )
  */
 iLog->Log(_L("IsSLPExists with invalid id which is negative"));
	TInt64 aSlpId=0;
			
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TBool ret;
	
	ret=aSettings->IsSlpExists(aSlpId);
	
	iLog->Log(_L("EXPECTED RESULT: EFalse"));
	iLog->Log(_L("Return Value=%d" ),ret);
	
	delete aSettings;
	
if(EFalse == ret)
		return 0;
	else 
		return -1;
 }
 //=============================================Get All SLP Synchronous===============================================================================================

//Get all slp synchronously
TInt CSettingsApi::GetAllSlp_sync(CStifItemParser& aItem)
    {
  
 iLog->Log(_L("Get All SLP synchronous call"));

RPointerArray<CServerParams> paramvaluearray;

CSuplSettings *aSettings=CSuplSettings::NewL();


TInt64 aSlpId;
HBufC* aServerAddress = HBufC::NewL(255);
HBufC* aIapName = HBufC::NewL(255);
TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;

TInt ret =aSettings->GetAllSlp(paramvaluearray);
iLog->Log(_L("Return value of Synchronous GetAllSlp: %d"),ret);

	delete aSettings;
	
	if(ret == KErrNone)
		return 0;
	else
		return -1;	
	
}
 //=============================================Get All SLP ASynchronous===============================================================================================

//Get all slp asynchronously
TInt CSettingsApi::GetAllSlp_async(CStifItemParser& aItem)
    {
  
 iLog->Log(_L("Get All SLP asynchronous call"));

CSuplSettings *aSettings=CSuplSettings::NewL();

RPointerArray<CServerParams> paramvaluearray;
TRequestStatus status = KRequestPending;
TInt ret =aSettings->GetAllSlp(paramvaluearray,status);

User::WaitForRequest(status);
iLog->Log(_L("Return value for GetAllSLP asynchronous: %d"),ret);

TInt64 aSlpId;
HBufC* aServerAddress = HBufC::NewL(255);
HBufC* aIapName = HBufC::NewL(255);
TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;

delete aSettings;
	
	if(ret == KErrNone)
		return 0;
	else
		return -1;	
	
}

//============================== SET FALL BACK =======================================================

TInt CSettingsApi::SetFallBack_ETrue()
    {
 /*
IMPORT_C TInt SetFallBack(TBool aFallBack);
  */
 iLog->Log(_L("SetFallBack with aFallBack=ETrue"));
TBool aFallBack = ETrue;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
		ret=aSettings->SetFallBack(aFallBack);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetFallBack=%d" ),ret);

	delete aSettings;
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }
 
 TInt CSettingsApi::SetFallBack_EFalse()
    {
 /*
IMPORT_C TInt SetFallBack(TBool aFallBack);
  */
 iLog->Log(_L("SetFallBack with aFallBack=EFalse"));
TBool aFallBack = EFalse;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
		ret=aSettings->SetFallBack(aFallBack);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of SetFallBack=%d" ),ret);

	delete aSettings;
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }
//================================GET FALL BACK =======================================================

TInt CSettingsApi::GetFallBack()
    {
 /*
IMPORT_C TInt GetFallBack(TBool& aFallBack) const;
  */
 iLog->Log(_L("GetFallBack"));
	
	TBool aFallBack;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
		ret=aSettings->GetFallBack(aFallBack);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value GetFallback=%d" ),ret);
	
	delete aSettings;
	return ret;
 }

//================================Set SLP Count =======================================================

TInt CSettingsApi::SlpCount()
    {
 /*
 IMPORT_C TInt SlpCount(TInt& aCount);
 */
 
 iLog->Log(_L("Slp Count"));
	
	TInt aCount;
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TInt ret;
	
		ret=aSettings->SlpCount(aCount);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value Slp Count=%d" ),ret);
		
	delete aSettings;
	
	if(ret == KErrNone)
		return 0;
	else
		return -1;	
 }


//================================IS SLP Exists =======================================================

//1. Is slp exists for valid  serveraddress
TInt CSettingsApi::IsSLPExistsString()
    {
 /*
 IMPORT_C TBool IsSlpExists(const TDesC& aServerAddress  );
*/
 
 iLog->Log(_L("Is SLP Exists for valid server address"));
	
_LIT8(KServer, "supl.nokia.com");

	RBuf aServer;
	aServer.Create(256);
	aServer.Copy(KServer);
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
		
  TBool ret;
	
		ret=aSettings->IsSlpExists(aServer);
	
	iLog->Log(_L("EXPECTED RESULT: ETrue"));
	iLog->Log(_L("Return Value of Is SLP Exists=%d" ),ret);
	
	aServer.Close();
	delete aSettings;
	
	if(ret)
		return 0;
	else
		return -1;
 }


//2. Is slp exists for Null  serveraddress
TInt CSettingsApi::IsSLPExistsString_Null()
    {
 /*
 IMPORT_C TBool IsSlpExists(const TDesC& aServerAddress  );
*/
 
 iLog->Log(_L("Is SLP Exists for NULL server address"));
	
	RBuf aServer;
	aServer.Create(256);
	aServer.Append(KNullDesC);
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
	
  TBool ret;
	
		ret=aSettings->IsSlpExists(aServer);
	
	iLog->Log(_L("EXPECTED RESULT: EFalse"));
	iLog->Log(_L("Return Value of Is SLP Exists=%d" ),ret);
	
	aServer.Close();
	delete aSettings;
	
	if(ret)
		return -1;
	else
		return 0;
 }
 
 
 //3. Is slp exists for non existent  serveraddress
TInt CSettingsApi::IsSLPExistsString_invalid()
    {
 /*
 IMPORT_C TBool IsSlpExists(const TDesC& aServerAddress  );
*/
 
 iLog->Log(_L("Is SLP Exists for Invalid server address"));
	
	_LIT8(KServer, "suplserverpriya.com");

	RBuf aServer;
	aServer.Create(256);
	aServer.Copy(KServer);
		
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TBool ret;
	
		ret=aSettings->IsSlpExists(aServer);
	
	iLog->Log(_L("EXPECTED RESULT: EFalse"));
	iLog->Log(_L("Return Value of Is SLP Exists=%d" ),ret);

	aServer.Close();
	delete aSettings;
	
	if(ret)
		return -1;
	else
		return 0;
 }
 
//=================================== Get SLP Info Address ===============================================================================================================================
 
//GetSlpInfoFromAddres for Valid address
TInt CSettingsApi::GetSlpInfoFromAddress(CStifItemParser& aItem)
    {
 /*
IMPORT_C TInt GetSlpInfoAddress(const TDesC& aServerAddress, CServerParams *aParamValues) const;
*/
 
iLog->Log(_L("GetSlpInfoFromAddress for valid slpaddress"));
CServerParams *aParamValues = CServerParams::NewL() ;
TInt aSlpId;	
HBufC* aServerAddress = HBufC::NewL(255);
HBufC* aIapName = HBufC::NewL(255);
TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;

_LIT8(KServer, "supl.nokia.com");

	RBuf aServer;
	aServer.Create(256);
	aServer.Copy(KServer);
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
	
	ret=aSettings->GetSlpInfoAddress(aServer,aParamValues);
		TInt64 aSlpId1;
	aParamValues->Get(aSlpId1,aServerAddress->Des(),aIapName->Des(),aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable);
	
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of GetSLPInfoAddress=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrNone)
		return 0;
	else
		return -1;
			
 }
 
 
 //GetSlpInfoFromAddres for InValid  or non existent address
TInt CSettingsApi::GetSlpInfoFromAddress_invalid(CStifItemParser& aItem)
    {
 /*
IMPORT_C TInt GetSlpInfoAddress(const TDesC& aServerAddress, CServerParams *aParamValues) const;
*/
 
  iLog->Log(_L("GetSlpInfoFromAddress for invalid slpaddress"));
	CServerParams *aParamValues = CServerParams::NewL() ;
TInt aSlpId;	
HBufC* aServerAddress = HBufC::NewL(255);
HBufC* aIapName = HBufC::NewL(255);
TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;

_LIT8(KServer, "suplserverpriya.com");

	RBuf aServer;
	aServer.Create(256);
	aServer.Copy(KServer);
		
	CSuplSettings *aSettings=CSuplSettings::NewL();
  TInt ret;
	
	ret=aSettings->GetSlpInfoAddress(aServer,aParamValues);
		
		TInt64 aSlpId1;;
	aParamValues->Get(aSlpId1,aServerAddress->Des(),aIapName->Des(),aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable);
	
	delete aSettings;
	if(ret == KErrNotFound)
		return 0;
	else
		return -1;
			
 }

 
//======================================= CHANGE PRIORITY ===============================================================================================================================
//Priya: 1. Increase priority from m to n and check the priority of the other servers are changed or not

TInt CSettingsApi::ChangePriority_Increase(CStifItemParser& aItem)
    {
 /* 
 IMPORT_C TInt ChangePriority(
                        TInt64 aSlpId, 
                        TInt aPriority,
                        TBool aDirection
                        );

 */
TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;

TInt aPriority;
	aItem.GetNextInt(aPriority);
	iLog->Log(_L("Change Priority: Increase the priority"));
	CSuplSettings *aSettings=CSuplSettings::NewL();
 
 TBool aDirection = ETrue;
		
	TInt ret=aSettings->ChangePriority(aSlpId,aPriority,aDirection);
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of Change priority=%d"),ret);

	delete aSettings;
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }

//Priya: 2. Decrease priority from n to m and check the priority of the other servers are changed or not

TInt CSettingsApi::ChangePriority_Decrease(CStifItemParser& aItem)
    {

	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;

TInt aPriority;
	aItem.GetNextInt(aPriority);
	iLog->Log(_L("Change Priority: Decrease the priority"));
	CSuplSettings *aSettings=CSuplSettings::NewL();
	TBool aDirection = EFalse;

  TInt ret;
	
	ret=aSettings->ChangePriority(aSlpId,aPriority,aDirection);
	iLog->Log(_L("EXPECTED RESULT: KErrNone"));
	iLog->Log(_L("Return Value of Change priority=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrNone)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }

//priya: 3. Give priority value greater than number of servers

TInt CSettingsApi::ChangePriority_invalid_positive(CStifItemParser& aItem)
 {

	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;

TInt aPriority =1001;
TBool aDirection = ETrue;
	iLog->Log(_L("Change Priority: Invalid priority"));
	CSuplSettings *aSettings=CSuplSettings::NewL();
 
  TInt ret;
	
	ret=aSettings->ChangePriority(aSlpId,aPriority,aDirection);
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of Change priority=%d" ),ret);
	
	delete aSettings;
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }

//priya: 4. give a priority = 0

TInt CSettingsApi::ChangePriority_priority0(CStifItemParser& aItem)
 {

	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;

TInt aPriority = 0;
TBool aDirection= ETrue; 

	iLog->Log(_L("Change Priority: Invalid priority which is zero"));
	CSuplSettings *aSettings=CSuplSettings::NewL();
 
    TInt ret;
	
	ret=aSettings->ChangePriority(aSlpId,aPriority,aDirection);
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of Change priority=%d" ),ret);

	delete aSettings;
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }

//priya: 5. Give a priority = -ve value

TInt CSettingsApi::ChangePriority_Negative(CStifItemParser& aItem)
 {
 
	TInt aSlpId1;
TInt64 aSlpId;
aItem.GetNextInt(aSlpId1);
aSlpId = aSlpId1;

TInt aPriority = -10;
TBool aDirection= EFalse;
	iLog->Log(_L("Change Priority: Invalid priority which is negative"));
	CSuplSettings *aSettings=CSuplSettings::NewL();
 
	TInt ret;
	ret=aSettings->ChangePriority(aSlpId,aPriority,aDirection);
	iLog->Log(_L("EXPECTED RESULT: KErrArgument"));
	iLog->Log(_L("Return Value of Change Priority=%d" ),ret);

	delete aSettings;
	if(ret == KErrArgument)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }
 
//priya: 6. Give invalid server id

TInt CSettingsApi::ChangePriority_InvalidServer(CStifItemParser& aItem)
 {

	TInt64 aSlpId=5674; 
	TInt aPriority = 1;
	TBool aDirection = EFalse;
	
	iLog->Log(_L("Change Priority: Invalid Server"));
	CSuplSettings *aSettings=CSuplSettings::NewL();

  TInt ret;
	
	ret=aSettings->ChangePriority(aSlpId,aPriority,aDirection);
	iLog->Log(_L("EXPECTED RESULT: KErrNotFound"));
	iLog->Log(_L("Return Value of Change Priority=%d" ),ret);

	delete aSettings;
	if(ret == KErrNotFound)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }
 
//priya: 7. Give invalid server id and invalid priority

TInt CSettingsApi::ChangePriority_invalidserverpriority(CStifItemParser& aItem)
 {
 
	TInt64 aSlpId=12390;

	TInt aPriority=-109;
	TBool aDirection = EFalse; 
	iLog->Log(_L("Change Priority: Invalid priority and Invalid Server id"));
	CSuplSettings *aSettings=CSuplSettings::NewL();
  //TRequestStatus aStatus;
  // aSettings->Initialize(aStatus);
  // User::WaitForRequest(aStatus);
  TInt ret;
	ret=aSettings->ChangePriority(aSlpId,aPriority,aDirection);
	iLog->Log(_L("EXPECTED RESULT: KErrArguement or KErrNotFound"));
	iLog->Log(_L("Return Value of Change Priority=%d" ),ret);

	delete aSettings;
	if(ret == KErrArgument || ret == KErrNotFound)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }


TInt CSettingsApi::GenerateHslpFromImsiWithoutInitialize( )
    {
    CSuplSettings *aSettings=CSuplSettings::NewL();
    RBuf aHslp;
    aHslp.Create(256);
    iLog->Log(_L("GenerateHslpFromImsiWithoutInitialize"));
    TInt ret=aSettings->GenerateHslpAddressFromImsi(aHslp);
    iLog->Log(_L("GenerateHslpAddressFromImsi = %d"),ret);
    delete aSettings;
    if(ret == KErrNotReady)
    		ret = KErrNone;
    return ret;
    }


TInt CSettingsApi::IsImsiChangedWithoutInitialize( )
    {
    CSuplSettings *aSettings=CSuplSettings::NewL();
    TBool aChanged;
    TInt ret=aSettings->IsImsiChanged(aChanged) ;
    iLog->Log(_L("IsImsiChangedWithoutInitialize=%d"),ret);
    delete aSettings;
    if(ret == KErrNotReady)
    		ret = KErrNone;
    return ret;
    }

TInt CSettingsApi::RemoveBeforeSetObserver( )
    {
    CSuplSettings *aSettings=CSuplSettings::NewL();
    aSettings->RemoveObserver();
    iLog->Log(_L("RemoveBeforeSetObserver"));
    delete aSettings;
    return KErrNone;
    }
        
TInt CSettingsApi::GetImsiWithoutInitialize( )
    {
    CSuplSettings *aSettings=CSuplSettings::NewL();
    RBuf aImsi;
    aImsi.Create(256);
    TInt ret= aSettings->GetImsi(aImsi);
    iLog->Log(_L("GetImsiWithoutInitialize=%d"),ret);
    delete aSettings;
    if(ret==KErrNotReady)
    		ret = KErrNone;
    return ret;
    
    }
 

TInt CSettingsApi::RemoveObserver()
	{
		CSuplSettings *aSettings=CSuplSettings::NewL();
		TInt err=aSettings->SetObserverL(*this);
		aSettings->RemoveObserver();
		delete aSettings;
		return err;		
	}



TInt CSettingsApi::IsImsiChangedTest( )
    {
    	TInt ret;
    	CSuplSettings *aSettings=CSuplSettings::NewL();
    	iSettings=aSettings;
    		iLog->Log(_L("Entered Initialize IMSI"));	
    imyAO=CTerminalInitiatedRunSession :: NewL(this);
    	iLog->Log(_L("afternewl"));	
    imyAO->RunSession(aSettings);
    	iLog->Log(_L("after runsession"));	
    CActiveScheduler::Start();	
    iLog->Log(_L("InitializeImsi returned=%d"),iErrCode);
  
		TBool aChanged;
		 ret=aSettings->IsImsiChanged(aChanged);
		 iLog->Log(_L("IsImsiChanged=%d"),ret);
		delete aSettings;
		delete imyAO;
    return ret;
    }



TInt CSettingsApi::CancelInitializeTest( )
    {
  
    CSuplSettings *aSettings=CSuplSettings::NewL();
        
    TInt ret;
    iSettings=aSettings;
    imyAO=CTerminalInitiatedRunSession :: NewL(this);
    imyAO->RunSession(aSettings);
    CActiveScheduler::Start();		
    //	InitializeImsi();
    if(iErrCode==100)
    {
    imyAO->CancelRunSession(aSettings);	
    CActiveScheduler::Start();		
    delete iSettings;
    delete imyAO;
    return imyAO->iStatus.Int();
    }
    else
    {
    delete iSettings;
    delete imyAO;
    return KErrNone;
    	
    }
   }

TInt CSettingsApi::GetHslpFromImsi( )
    {
    
    CSuplSettings *aSettings=CSuplSettings::NewL();
      
    TInt ret;
    
    iSettings=aSettings;
    imyAO=CTerminalInitiatedRunSession :: NewL(this);
    imyAO->RunSession(aSettings);
    CActiveScheduler::Start();	
    iLog->Log(_L("InitializeImsi returned=%d"),iErrCode);

    RBuf aHslp;
    aHslp.Create(256);
  ret = iSettings->GenerateHslpAddressFromImsi(aHslp);	
	iLog->Log(_L("GetHslpFromImsi=%d"),ret);
	iLog->Log(_L("Value of HSLP="));
	iLog->Log(aHslp);	

    delete aSettings;
    delete imyAO;   	
    return ret;
    }

TInt CSettingsApi::InitializeImsi()
{
    CSuplSettings *aSettings=CSuplSettings::NewL();
        
    TInt ret;
  
    iSettings=aSettings;
    imyAO=CTerminalInitiatedRunSession :: NewL(this);
    imyAO->RunSession(aSettings);
    CActiveScheduler::Start();	
    iLog->Log(_L("InitializeImsi returned=%d"),iErrCode);
    delete iSettings;
    delete imyAO;
    
		return iErrCode;
}

TInt CSettingsApi::GenerateHslpWithoutInitialize()
{
		CSuplSettings *aSettings=CSuplSettings::NewL();
		RBuf aHslp;
		aHslp.Create(256);
		TInt ret=aSettings->GenerateHslpAddressFromImsi(aHslp);
		iLog->Log(_L("Generate HSLP without Initialize=%d" ),ret);
		aHslp.Close();
		delete aSettings;
		if(ret == KErrNotReady)
				ret = KErrNone;
		return ret;
		 
}


void CSettingsApi::GetImsiTest(CSuplSettings* aSettings)
{
	RBuf aImsi;
	aImsi.Create(256);
	TInt ret=aSettings->GetImsi(aImsi);
	iLog->Log(aImsi);
	delete aSettings;

	}



CTerminalInitiatedRunSession::CTerminalInitiatedRunSession():CActive(EPriorityStandard)
	{
		 iSettings=NULL;
  }

CTerminalInitiatedRunSession* CTerminalInitiatedRunSession::NewL(MSettingsObserver *aObserver)
	{
	
	CTerminalInitiatedRunSession* self = new (ELeave) CTerminalInitiatedRunSession();
    CleanupStack::PushL(self);
    self->ConstructL(aObserver);
    CleanupStack::Pop(); 
        return self; 
	}

CTerminalInitiatedRunSession::~CTerminalInitiatedRunSession()
	{
		Cancel();
		iTimer.Close();
	//	CActiveScheduler::Stop();	
	
	}
	
void CTerminalInitiatedRunSession::ConstructL(MSettingsObserver *aObserver)
	{
//	CActiveScheduler::Start();
	iObserver=aObserver;
	CActiveScheduler::Add(this);
	}
	
void CTerminalInitiatedRunSession::RunL()
	{
		TInt i = iStatus.Int();
	//	RBuf aImsi;
	//	aImsi.Create(256);
	//	iSettings->GetImsi(aImsi);
//	iTest=CSettingsApi::NewL();	
	iObserver->CompleteInitialize(i);

	//	CActiveScheduler::Stop();
	}

void CTerminalInitiatedRunSession::DoCancel()
	{
	}
	     
void CTerminalInitiatedRunSession::RunSession(CSuplSettings *aSettings)
	{
		//iSession.RunSession(iStatus, aCaps,aReqId);
	//	User::After(10000000);
	
		iSettings=aSettings;
		iSettings->Initialize(iStatus);
				SetActive();
//		iSettings->CancelInitialize();
	}
void CTerminalInitiatedRunSession::CancelRunSession(CSuplSettings *aSettings)
	{
	//	iSession.CancelRunSession();
		aSettings->CancelInitialize();

	}

void CTerminalInitiatedRunSession::StartListening()
	{
			 iTimer.CreateLocal();
			 iTimer.After(iStatus,1000000);
			 SetActive();
	}
void CSettingsApi::HandleSuplSettingsChangeL (TSuplSettingsEventType aEvent,TInt aSlpId)
{
	iLog->Log(_L("SUPL Settings Changed"));	
//	iSettings->RemoveObserver();
	CActiveScheduler::Stop();
	imyAO=CTerminalInitiatedRunSession :: NewL(this);
	imyAO->StartListening();
}



CObserverTest::CObserverTest(MSuplSettingsObserver *aObserver)
    :iObserver(aObserver)
    {
   

    }

// Destructor
CObserverTest::~CObserverTest()
    {
    

    }

// Two-phased constructor
CObserverTest* CObserverTest::NewL(MSuplSettingsObserver *aObserver)
    {
    
    }




TInt CSettingsApi::SetUsageAutomatic()
{
	
    /*
    CSuplSettings* settings = CSuplSettings::NewL();
    TInt ret;	
	
			//ret=settings->SetSuplUsage(CSuplSettings::ESuplUsageAutomatic);
			iLog->Log(_L("SetUsageAutomatic returned=%d" ),ret);
	
	delete settings;*/
    return KErrNone;
}

TInt CSettingsApi::SetUsageAlwaysAsk()
{
	/*
    CSuplSettings* settings = CSuplSettings::NewL();
	TInt ret;
			//ret=settings->SetSuplUsage(CSuplSettings::ESuplUsageAlwaysAsk);
			iLog->Log(_L("SetUsageAlwaysAsk returned=%d" ),ret);
	
	delete settings;*/
    return KErrNone;
}

TInt CSettingsApi::SetUsageAutoInHMNw()
{
	/*
    CSuplSettings* settings = CSuplSettings::NewL();
	TInt ret;
			//ret=settings->SetSuplUsage(CSuplSettings::ESuplUsageHomeAutomatic);
			iLog->Log(_L("SetUsageAutoInHMNw returned=%d" ),ret);
	
	delete settings;
	*/
    return KErrNone;;
}

TInt CSettingsApi::SetUsageDisabled()
{
	/*
    CSuplSettings* settings = CSuplSettings::NewL();
	TInt ret;
			//ret=settings->SetSuplUsage(CSuplSettings::ESuplUsageDisabled);
			iLog->Log(_L("SetUsageDisabled returned=%d" ),ret);
	
	delete settings;
	*/
	return KErrNone;
}

//////////////////////////////////////////////////////////////////////

TInt CSettingsApi::GetUsageAutomatic()
	{
	/*	
    TInt ret = SetUsageAutomatic();
		if(ret == KErrNone)
			{
				CSuplSettings::TSuplSettingsUsage usage;
				CSuplSettings* settings = CSuplSettings::NewL();
				ret = settings->GetSuplUsage(usage);
				if (ret == KErrNone && usage == CSuplSettings::ESuplUsageAutomatic)
					{
						ret = KErrNone;	
						iLog->Log(_L("Usages are matching...returned=%d" ),(TInt)usage);	
					}
				else
					{
						ret = KErrGeneral;	
						iLog->Log(_L("Usages are not matching...returned=%d" ),(TInt)usage);	
					}	
					
				iLog->Log(_L("GetSuplUsage returned = %d" ),ret);
				delete settings;
			}
		else
			{
				iLog->Log(_L("SetUsageAutomatic returned error =%d" ),ret);
			}	
    */
	return KErrNone;			
	}
	
TInt CSettingsApi::GetUsageAlwaysAsk()
	{
	/*	
    TInt ret = SetUsageAlwaysAsk();
		if(ret == KErrNone)
			{
				CSuplSettings::TSuplSettingsUsage usage;
				CSuplSettings* settings = CSuplSettings::NewL();
				ret = settings->GetSuplUsage(usage);
				if (ret == KErrNone && usage == CSuplSettings::ESuplUsageAlwaysAsk)
					{
						ret = KErrNone;	
						iLog->Log(_L("Usages are matching...returned=%d" ),(TInt)usage);	
					}
				else
					{
						ret = KErrGeneral;	
						iLog->Log(_L("Usages are not matching...returned=%d" ),(TInt)usage);	
					}	
					
				iLog->Log(_L("GetSuplUsage returned = %d" ),ret);
				delete settings;
			}
		else
			{
				iLog->Log(_L("SetUsageAlwaysAsk returned error =%d" ),ret);
			}	
    */
	return KErrNone;			

	}


TInt CSettingsApi::GetUsageAutoInHMNw()
	{
	
	/*	
    TInt ret = SetUsageAutoInHMNw();
		if(ret == KErrNone)
			{
				CSuplSettings::TSuplSettingsUsage usage;
				CSuplSettings* settings = CSuplSettings::NewL();
				ret = settings->GetSuplUsage(usage);
				if (ret == KErrNone && usage == CSuplSettings::ESuplUsageHomeAutomatic)
					{
						ret = KErrNone;	
						iLog->Log(_L("Usages are  matching...returned=%d" ),(TInt)usage);	
					}
				else
					{
						ret = KErrGeneral;	
						iLog->Log(_L("Usages are not matching...returned=%d" ),(TInt)usage);	
					}	
					
				iLog->Log(_L("GetSuplUsage returned = %d" ),ret);
				delete settings;
			}
		else
			{
				iLog->Log(_L("SetUsageAutoInHMNw returned error =%d" ),ret);
			}	
     */
	return KErrNone;			
	
	}

TInt CSettingsApi::GetUsageDisabled()
	{
	
	/*	
    TInt ret = SetUsageDisabled();
		if(ret == KErrNone)
			{
				CSuplSettings::TSuplSettingsUsage usage;
				CSuplSettings* settings = CSuplSettings::NewL();
				ret = settings->GetSuplUsage(usage);
				if (ret == KErrNone && usage == CSuplSettings::ESuplUsageDisabled)
					{
						ret = KErrNone;	
						iLog->Log(_L("Usages are  matching...returned=%d" ),(TInt)usage);	
					}
				else
					{
						ret = KErrGeneral;	
						iLog->Log(_L("Usages are not matching...returned=%d" ),(TInt)usage);	
					}	
					
				iLog->Log(_L("GetSuplUsage returned = %d" ),ret);
				delete settings;
			}
		else
			{
				iLog->Log(_L("SetUsageDisabled returned error =%d" ),ret);
			}	
    */
	return KErrNone;			
	
	}
	

//////////////////////////////////////////////////////////////////////

TInt CSettingsApi::SetUsageOtherThanAppropriate()
{
	/*
    CSuplSettings::TSuplSettingsUsage usage = (CSuplSettings::TSuplSettingsUsage)100;
	CSuplSettings* settings = CSuplSettings::NewL();
	TInt ret=settings->SetSuplUsage(usage);
	
	if(ret == KErrArgument)
		{
			ret = KErrNone;
		}
	else
		{
			ret = KErrGeneral;
		}	
		
	iLog->Log(_L("SetSuplUsage returned=%d" ),ret);	

	delete settings;
	*/
	return KErrNone;
}


TInt CSettingsApi::SetGetTriggerParamsL( CStifItemParser& aItem )
    {
    CTriggerParams* params = CTriggerParams::NewL();
    TBuf<64> sessionName;
    sessionName.Copy(_L("Session1"));
    TTime endTime1;
    endTime1 = 100;
    TInt ret=params->Set( 5,sessionName,ETrue,ETrue,CTriggerParams::EPeriodic,CTriggerParams::ETerminalInitiated,10,10);
    	params->SetTriggerEndTime(endTime1);
    	
    if( KErrNone!=ret)
        {
        delete params;
        return ret;
        }
    TInt64 sessionId;
    TBool notificationPresent;
    TBool triggerNotificationStatus;
    CTriggerParams::TTriggerType triggerType;
    CTriggerParams::TRequestType requestType;
    TUint64 outstandingTrigger;
    TUint64 interval;
    TTime endTime2;
    HBufC* name = HBufC::NewL( 256);
    ret = params->Get(sessionId,name->Des(),notificationPresent,triggerNotificationStatus,
                                 triggerType,requestType,outstandingTrigger,interval);
                                 
		params->GetTriggerEndTime(endTime2 );                                 
    
    delete params;
    if( KErrNone!=ret)
        {
        delete name;
        return ret;
        }
    
    ret=KErrGeneral;
    TInt comp=name->Compare(sessionName);
    if(sessionId==5 &&  comp==0 && notificationPresent && 
       triggerNotificationStatus && triggerType== CTriggerParams::EPeriodic &&
       requestType==CTriggerParams::ETerminalInitiated && outstandingTrigger==10
       && interval==10 && endTime1 == endTime2 )
        {
        ret=KErrNone;
        }
    
    delete name;
    return ret;
    }

TInt CSettingsApi::SetTriggerLongSessionNameL( CStifItemParser& aItem )
    {
    CTriggerParams* params = CTriggerParams::NewLC();
    TBuf<300> sessionName;
    for(TInt i=0;i<290;i++)
        {
        sessionName.Append(_L("a"));
        }
        
    TInt ret=params->Set( 5,sessionName,ETrue,ETrue,CTriggerParams::EPeriodic,CTriggerParams::ETerminalInitiated,10,10);
   
    	
    	
    CleanupStack::PopAndDestroy();
    	
    if( KErrArgument!= ret )
        {
        return KErrGeneral;
        }
    return KErrNone;
    
    }

TInt CSettingsApi::SetTriggerEmptySessionNameL( CStifItemParser& aItem )
    {
    CTriggerParams* params = CTriggerParams::NewL();
    TBuf<300> sessionName;
    sessionName.Zero();
    TInt ret=params->Set( 5,sessionName,ETrue,ETrue,CTriggerParams::EPeriodic,CTriggerParams::ETerminalInitiated,10,10);
    
       TInt64 sessionId;
        TBool notificationPresent;
        TBool triggerNotificationStatus;
        CTriggerParams::TTriggerType triggerType;
        CTriggerParams::TRequestType requestType;
        TUint64 outstandingTrigger;
        TUint64 interval;
        TTime endTime;
        HBufC* name = HBufC::NewL( 256);
        ret = params->Get(sessionId,name->Des(),notificationPresent,triggerNotificationStatus,
                                     triggerType,requestType,outstandingTrigger,interval);
        
        delete params;
     TInt comp=name->Compare(sessionName);
     TInt length=name->Length();
     delete name;
     if(comp==0 && length==0)
         {
         return KErrNone;
         }
     
     return KErrGeneral;
    
    }






	




	


TInt CSettingsApi::GetAllSessionSetApiL( CStifItemParser& aItem )
	{
	RPointerArray<CTriggerParams> values;
	CSuplSettings* settings = CSuplSettings::NewL();
	TInt err=settings->GetTriggerParams(values);
	
	values.ResetAndDestroy();
	delete settings;
	return err;
	}



	
TInt CSettingsApi::SetSessionObserverL( CStifItemParser& aItem )
	{
	CSuplSettings* settings = CSuplSettings::NewLC();
	CSessionObserver* obs = CSessionObserver::NewL();
	TInt err=settings->SetSessionObserverL(*obs);
	CleanupStack::PopAndDestroy(1);
	delete obs;
	return err;
	}
	
TInt CSettingsApi::SetSessionObserverTwiceL( CStifItemParser& aItem )
	{
	CSuplSettings* settings = CSuplSettings::NewLC();
	CSessionObserver* obs = CSessionObserver::NewL();
	TInt err=settings->SetSessionObserverL(*obs);
	err=settings->SetSessionObserverL(*obs);
			
	CleanupStack::PopAndDestroy(1);
	delete obs;
	if ( err == KErrAlreadyExists)
		return KErrNone ;
	return err;
	}					

TInt CSettingsApi::RemoveSessionObserverL( CStifItemParser& aItem )
	{
	CSuplSettings* settings = CSuplSettings::NewLC();
	CSessionObserver* obs = CSessionObserver::NewL();
	TInt err=settings->SetSessionObserverL(*obs);
	settings->RemoveSessionObserver();
	CleanupStack::PopAndDestroy(1);
	delete obs;
	return err;
	}	
	
TInt CSettingsApi::RemoveSessionObserverWithoutSetL( CStifItemParser& aItem )
	{
	CSuplSettings* settings = CSuplSettings::NewL();
	settings->RemoveSessionObserver();
	delete settings;
	return KErrNone;
	}	

TInt CSettingsApi::CheckSessionChangeEventsL( CStifItemParser& aItem )
	{
	CSuplSettings* settings = CSuplSettings::NewLC();
	CSessionObserver* obs = CSessionObserver::NewL();
	TInt err=settings->SetSessionObserverL(*obs);
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(1);
	delete obs;
	return err;
	}


TInt CSettingsApi::GetASessionParamNullL( CStifItemParser& aItem )
    {
    CTriggerParams* params=NULL;
    CSuplSettings* settings = CSuplSettings::NewL();
    TInt err=settings->GetTriggerParams(10,params);
    delete settings;
    if( KErrArgument!=err)
        {
        return KErrGeneral;
        }
    return KErrNone;
    
    }

TInt CSettingsApi::GetASessionParamInValidL( CStifItemParser& aItem )
    {
    CTriggerParams* params=CTriggerParams::NewL();
    CSuplSettings* settings = CSuplSettings::NewL();
    TInt err=settings->GetTriggerParams(11,params);//Invalid Id
    delete settings;
    delete params;
    if( KErrNotFound!=err)
        {
        return KErrGeneral;
        }
    return KErrNone;
    
    }

TInt CSettingsApi::CancelTriggerSessionL( CStifItemParser& aItem )
    {
    CSuplSettings* settings = CSuplSettings::NewL();
    TInt err=settings->CancelTriggerSession(10);
    return err;
    }

//Change the status with SuplTriggerON 

TInt CSettingsApi::ChangeSuplTriggerServiceStatusON()
    {
    CSuplSettings* settings = CSuplSettings::NewL() ;
    TInt ret ;
    //settings->SetObserverL(*this);
    ret = settings->SetSuplTriggeredServiceStatus(CSuplSettings::ESuplTriggerOn);
    iLog->Log(_L("Supltrigger ON = %d "),ret ) ;
    if(ret != KErrNone) 
        {
          iLog->Log(_L("Supltrigger service status:Error = %d"), ret); 
        }
    iLog->Log(_L("ChangeSuplTriggerServiceStatus ON = %d "),ret ) ;
    delete settings;
    return ret;
       
    
    }
//Change the status with SuplTriggerOFF

TInt CSettingsApi::ChangeSuplTriggerServiceStatusOFF()
    {
    CSuplSettings* settings = CSuplSettings::NewL() ;
    TInt ret ;
   // settings->SetObserverL(*this);
    ret = settings->SetSuplTriggeredServiceStatus(CSuplSettings::ESuplTriggerOff);
    iLog->Log(_L("Supltrigger OFF = %d "),ret ) ;
    if(ret != KErrNone) 
        {
            iLog->Log(_L("Supltrigger service status:Error = %d"), ret); 
        }
     //   CActiveScheduler::Start();
    iLog->Log(_L("ChangeSuplTriggerServiceStatus OFF = %d "),ret ) ;
    delete settings;
    return ret;
       
    
    }

//Get the status changes of SuplTriggerON 

TInt CSettingsApi::GetSuplTriggerServiceStatusON()
    {
     TInt ret = ChangeSuplTriggerServiceStatusON();
     iLog->Log(_L("ChangeSuplTriggerServiceStatus ON = %d "),ret ) ;
     if(ret == KErrNone)
        {
         CSuplSettings::TSuplTriggerStatus status;
         CSuplSettings* settings = CSuplSettings::NewL();
         ret = settings->GetSuplTriggeredServiceStatus(status);
         if (ret == KErrNone && status == CSuplSettings::ESuplTriggerOn)
            {
              iLog->Log(_L("status are matching...returned=%d" ),(TInt)status);    
            }
         else
            {
              iLog->Log(_L("status are not matching...returned=%d" ),(TInt)status);    
            }   
                    
         iLog->Log(_L("GetSuplTriggerServiceStatus returned = %d" ),ret);
         delete settings;
        }
        else
            {
                iLog->Log(_L("GetSuplTriggerServiceStatus returned error =%d" ),ret);
            }   

    return ret;         
    }

//Get the status changes of SuplTriggerOFF
TInt CSettingsApi::GetSuplTriggerServiceStatusOFF()
    {
    TInt ret = ChangeSuplTriggerServiceStatusOFF();
    if(ret == KErrNone)
      {
       CSuplSettings::TSuplTriggerStatus status;
       CSuplSettings* settings = CSuplSettings::NewL();
       ret = settings->GetSuplTriggeredServiceStatus(status);
       if (ret == KErrNone && status == CSuplSettings::ESuplTriggerOff)
          {
           iLog->Log(_L("status are matching...returned=%d" ),(TInt)status);    
          }
       else
          {
           iLog->Log(_L("status are not matching...returned=%d" ),(TInt)status);    
          }   
                    
       iLog->Log(_L("GetSuplTriggerServiceStatus returned = %d" ),ret);
       delete settings;
      }
        else
            {
                iLog->Log(_L("GetSuplTriggerServiceStatus returned error =%d" ),ret);
            }   

    return ret;         
    }


 
 TInt CSettingsApi::ProductConfiguredServer1()
   {
						CServerParams *aParamValues= CServerParams::NewL();
						
						TInt64 aSlpId =10;
						
						iLog->Log(_L("Setting CServerParams with product config parameter")); 
						_LIT8(KServer, "supl.nokia.com");
						_LIT8(KIap, "airtelgprs.com");
							
						RBuf aServerAddress, aIapName; 	
								
						aServerAddress.Create(128);
						aServerAddress.Copy(KServer);
						aIapName.Create(128);
						aIapName.Copy(KIap);
						
						aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,ETrue,aSlpId);
						
						aParamValues->SetServerConfigurationType(ETrue);
						
						aIapName.Close();
						aServerAddress.Close();
						
						HBufC* aServerAddress1 = HBufC::NewL(64);
						HBufC* aIapName1 = HBufC::NewL(64);

						TBool aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable,aProductConfigured;
						
						TInt ret = aParamValues->Get(aSlpId,aServerAddress1->Des(),aIapName1->Des(),aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable);
						aParamValues->GetServerConfigurationType(aProductConfigured);
						
						delete aParamValues;
						delete aServerAddress1;
						delete aIapName1;
						
						if(ret != KErrNone) 
							return ret;
							
						if (aProductConfigured)
							return KErrNone;
						else
							return KErrGeneral;
}


TInt CSettingsApi::ProductConfiguredServer2()
   {
   	CServerParams *aParamValues = CServerParams::NewL();
   	CSuplSettings *aSettings = CSuplSettings::NewL();
  	TInt ret;
  	TInt64 slp;

iLog->Log(_L("Setting CServerParams with product config parameter")); 
_LIT8(KServer, "product.supl.nokia.com");
_LIT8(KIap, "airtelgprs.com");
	
RBuf aServerAddress, aIapName; 	
		
aServerAddress.Create(128);
aServerAddress.Copy(KServer);
aIapName.Create(128);
aIapName.Copy(KIap);

			aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,ETrue);
			aParamValues->SetServerConfigurationType(ETrue);

ret = aSettings->AddNewServer(aParamValues,slp);
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("SLP id of the new server: %d" ),slp);	

if (ret != KErrNone)
	return ret;
	
	CServerParams *aParamValues1 = CServerParams::NewL();
ret = aSettings->GetSlpInfoFromId(slp,aParamValues1);
	
aIapName.Close();
aServerAddress.Close();

HBufC* aServerAddress1 = HBufC::NewL(64);
HBufC* aIapName1 = HBufC::NewL(64);
	
TInt64 aSlpId;
TBool aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable,aProductConfigured;

ret = aParamValues1->Get(aSlpId,aServerAddress1->Des(),aIapName1->Des(),aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable);

aParamValues1->GetServerConfigurationType(aProductConfigured);

delete aParamValues;
delete aParamValues1;
delete aSettings;
delete aServerAddress1;
delete aIapName1;

if(ret != KErrNone) 
	return ret;
	
if (aProductConfigured)
	return KErrNone;
else
	return KErrGeneral;
}

TInt CSettingsApi::ProductConfiguredServer3()
   {
   	CServerParams *aParamValues = CServerParams::NewL();
   	CSuplSettings *aSettings = CSuplSettings::NewL();
  	TInt ret;
  	TInt64 slp;

iLog->Log(_L("Setting CServerParams with product config parameter")); 
_LIT8(KServer, "user.supl.nokia.com");
_LIT8(KIap, "airtelgprs.com");
	
RBuf aServerAddress, aIapName; 	
		
aServerAddress.Create(128);
aServerAddress.Copy(KServer);
aIapName.Create(128);
aIapName.Copy(KIap);

			aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,ETrue);
			aParamValues->SetServerConfigurationType(EFalse);

 ret = aSettings->AddNewServer(aParamValues,slp);
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("SLP id of the new server: %d" ),slp);	

if (ret != KErrNone)
	return ret;
	
	CServerParams *aParamValues1 = CServerParams::NewL();
ret = aSettings->GetSlpInfoAddress(aServerAddress,aParamValues1);
	
aIapName.Close();
aServerAddress.Close();

HBufC* aServerAddress1 = HBufC::NewL(64);
HBufC* aIapName1 = HBufC::NewL(64);
TInt64 aSlpId;
TBool aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable,aProductConfigured;

ret = aParamValues1->Get(aSlpId,aServerAddress1->Des(),aIapName1->Des(),aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable);
aParamValues1->GetServerConfigurationType(aProductConfigured);


delete aParamValues;
delete aParamValues1;
delete aSettings;
delete aServerAddress1;
delete aIapName1;

if(ret != KErrNone) 
	return ret;
	
if (!aProductConfigured)
	return KErrNone;
else
	return KErrGeneral;
    
}

TInt CSettingsApi::ProductConfiguredServer4()
   {
   	CServerParams *aParamValues = CServerParams::NewL();
   	CSuplSettings *aSettings = CSuplSettings::NewL();
  	TInt ret,slp;

iLog->Log(_L("Setting CServerParams with product config parameter")); 
_LIT8(KServer, "user.google.nokia.com");
_LIT8(KIap, "airtelgprs.com");
	
RBuf aServerAddress, aIapName; 	
		
aServerAddress.Create(128);
aServerAddress.Copy(KServer);
aIapName.Create(128);
aIapName.Copy(KIap);

			aParamValues->Set(aServerAddress,aIapName,ETrue,EFalse,ETrue,ETrue);
			aParamValues->SetServerConfigurationType(EFalse);


ret = aSettings->SetDefaultServer(aParamValues);
	iLog->Log(_L("Return Value=%d" ),ret);
	iLog->Log(_L("SLP id of the new server: %d" ),slp);	

if (ret != KErrNone)
	return ret;
	
	CServerParams *aParamValues1 = CServerParams::NewL();
ret = aSettings->GetDefaultServer(aParamValues1);
	
aIapName.Close();
aServerAddress.Close();

HBufC* aServerAddress1 = HBufC::NewL(64);
HBufC* aIapName1 = HBufC::NewL(64);
TInt64 aSlpId;
TBool aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable,aProductConfigured;

ret = aParamValues1->Get(aSlpId,aServerAddress1->Des(),aIapName1->Des(),aServerEnabled,aSimChangeRemove,aUsageInHomeNw,aEditable);
aParamValues1->GetServerConfigurationType(aProductConfigured);

delete aParamValues;
delete aParamValues1;
delete aSettings;
delete aServerAddress1;
delete aIapName1;

if(ret != KErrNone) 
	return ret;
	
if (aProductConfigured)
	return KErrNone;
else
	return KErrGeneral;
    
}

 

// EPOC default constructor
void CObserverTest::ConstructL()
    {
    
    
    }
void CSettingsApi::CompleteInitialize(TInt err)
{
}
