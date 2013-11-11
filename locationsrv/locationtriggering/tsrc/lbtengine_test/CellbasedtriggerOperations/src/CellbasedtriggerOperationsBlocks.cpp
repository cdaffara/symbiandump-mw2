/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:     Test module to implement Cell based triggering test cases
*
*/






// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "CellbasedtriggerOperations.h"
#include "LbtAppCellInfo.h"
#include <e32cmn.h>
#include <e32des16.h>
#include <e32base.h>
#include <s32mem.h>
#include <e32math.h> 
#include <centralrepository.h>
#include <lbs/SimulationPSYInternalCRKeys.h>


// LT API

#include <lbtcommon.h>
#include <lbterrors.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeocell.h>
#include <lbtgeorect.h>
#include <lbtlisttriggeroptions.h>
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbtserver.h>
#include <lbttriggerchangeevent.h>
#include <lbttriggerchangeeventnotifier.h>
#include <lbttriggerchangeeventobserver.h>
#include <lbttriggerconditionarea.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerdynamicinfo.h>
#include <lbttriggerentry.h>
#include <lbttriggerfilterbase.h>
#include <lbttriggerfilterbyarea.h>
#include <lbttriggerfilterbyattribute.h>
#include <lbttriggerfiltercomposite.h>
#include <lbttriggerfiringeventnotifier.h>
#include <lbttriggerfiringeventobserver.h>
#include <lbttriggerinfo.h>
#include <lbttriggeringsystemsettings.h>
#include <lbttriggeringsystemsettingschangeeventnotifier.h>
#include <lbttriggeringsystemsettingschangeeventobserver.h>


// Location aquisition API 

#include <lbscommon.h>
#include <LbsRequestor.h>
#include <LbsPosition.h>
//#include <lbtoperation.h>


#include "AsynOperationAO.h"


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
// CCellbasedtriggerOperations::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCellbasedtriggerOperations::Delete() 
    {

    }


// -----------------------------------------------------------------------------
// CCellbasedtriggerOperations::GetCurrentCellInfo
// -----------------------------------------------------------------------------
void CCellbasedtriggerOperations::GetCurrentCellInfoL( 
                            RMobilePhone::TMobilePhoneNetworkMode& aNetworkMode,
                            CTelephony::TNetworkInfoV1& aNwInfo )  
    {
    iLog->Log(_L("+GetCurrentCellInfoL"));
    CLbtEventObserver* notifier= CLbtEventObserver::NewL();
    CleanupStack::PushL( notifier );
    CActiveSchedulerWait* wait = new( ELeave ) CActiveSchedulerWait;
    // Ownership of wait is taken by notifier
    iLog->Log(_L("+GetCurrentCGIInfo"));
    notifier->GetCurrentCGIInfo( aNwInfo,wait );
    iLog->Log(_L("-GetCurrentCGIInfo"));
    wait->Start();
    iLog->Log(_L("WaitStart"));
    switch( aNwInfo.iMode )
        {
        case CTelephony::ENetworkModeGsm:
            aNetworkMode = RMobilePhone::ENetworkModeGsm;
            break;
            
        case CTelephony::ENetworkModeWcdma:
            aNetworkMode = RMobilePhone::ENetworkModeWcdma;
            break;
            
        default:
            aNetworkMode = RMobilePhone::ENetworkModeUnknown;
        }
            
    CleanupStack::PopAndDestroy( notifier );
    iLog->Log(_L("-GetCurrentCellInfoL"));
    }

// -----------------------------------------------------------------------------
// CCellbasedtriggerOperations::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCellbasedtriggerOperations::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "TC_LBT_034_01L", CCellbasedtriggerOperations::TC_LBT_034_01_testL ),
        ENTRY( "TC_LBT_034_02L", CCellbasedtriggerOperations::TC_LBT_034_02_testL ),
        ENTRY( "TC_LBT_034_03L", CCellbasedtriggerOperations::TC_LBT_034_03_testL ),
        ENTRY( "TC_LBT_034_04L", CCellbasedtriggerOperations::TC_LBT_034_04_testL ),
        ENTRY( "TC_LBT_034_05L", CCellbasedtriggerOperations::TC_LBT_034_05_testL ),
        ENTRY( "TC_LBT_034_06L", CCellbasedtriggerOperations::TC_LBT_034_06_testL ),
        ENTRY( "TC_LBT_034_07L", CCellbasedtriggerOperations::TC_LBT_034_07_testL ),
        ENTRY( "TC_LBT_034_08L", CCellbasedtriggerOperations::TC_LBT_034_08_testL ),
        ENTRY( "TC_LBT_034_09L", CCellbasedtriggerOperations::TC_LBT_034_09_testL ),
        ENTRY( "TC_LBT_034_10L", CCellbasedtriggerOperations::TC_LBT_034_10_testL ),
        ENTRY( "TC_LBT_034_11L", CCellbasedtriggerOperations::TC_LBT_034_11_testL ),
        ENTRY( "TC_LBT_034_12L", CCellbasedtriggerOperations::TC_LBT_034_12_testL ),
        ENTRY( "TC_LBT_034_13L", CCellbasedtriggerOperations::TC_LBT_034_13_testL ),
        ENTRY( "TC_LBT_034_14L", CCellbasedtriggerOperations::TC_LBT_034_14_testL ),
        ENTRY( "TC_LBT_034_15L", CCellbasedtriggerOperations::TC_LBT_034_15_testL ),
        ENTRY( "TC_LBT_034_16L", CCellbasedtriggerOperations::TC_LBT_034_16_testL ),
        ENTRY( "TC_LBT_034_17L", CCellbasedtriggerOperations::TC_LBT_034_17_testL ),
        ENTRY( "TC_LBT_034_18L", CCellbasedtriggerOperations::TC_LBT_034_18_testL ),
        ENTRY( "TC_LBT_034_19L", CCellbasedtriggerOperations::TC_LBT_034_19_testL ),
        ENTRY( "TC_LBT_034_20L", CCellbasedtriggerOperations::TC_LBT_034_20_testL ),
        ENTRY( "TC_LBT_034_21L", CCellbasedtriggerOperations::TC_LBT_034_21_testL ),
        ENTRY( "TC_LBT_034_22L", CCellbasedtriggerOperations::TC_LBT_034_22_testL ),
        ENTRY( "TC_LBT_034_23L", CCellbasedtriggerOperations::TC_LBT_034_23_testL ),
        ENTRY( "TC_LBT_034_24L", CCellbasedtriggerOperations::TC_LBT_034_24_testL ),
        ENTRY( "TC_LBT_034_25L", CCellbasedtriggerOperations::TC_LBT_034_25_testL ),
        ENTRY( "TC_LBT_034_26L", CCellbasedtriggerOperations::TC_LBT_034_26_testL ),
        ENTRY( "TC_LBT_034_27L", CCellbasedtriggerOperations::TC_LBT_034_27_testL ),
        ENTRY( "TC_LBT_034_28L", CCellbasedtriggerOperations::TC_LBT_034_28_testL ),
        ENTRY( "TC_LBT_034_29L", CCellbasedtriggerOperations::TC_LBT_034_29_testL ),
        ENTRY( "TC_LBT_034_30L", CCellbasedtriggerOperations::TC_LBT_034_30_testL ),
        ENTRY( "TC_LBT_034_31L", CCellbasedtriggerOperations::TC_LBT_034_31_testL ),
        ENTRY( "TC_LBT_034_32L", CCellbasedtriggerOperations::TC_LBT_034_32_testL ),
        ENTRY( "TC_LBT_034_33L", CCellbasedtriggerOperations::TC_LBT_034_33_testL ),
        ENTRY( "TC_LBT_034_34L", CCellbasedtriggerOperations::TC_LBT_034_34_testL ),
        ENTRY( "TC_LBT_034_35L", CCellbasedtriggerOperations::TC_LBT_034_35_testL ),
        ENTRY( "TC_LBT_034_36L", CCellbasedtriggerOperations::TC_LBT_034_36_testL ),
        ENTRY( "TC_LBT_034_37L", CCellbasedtriggerOperations::TC_LBT_034_37_testL ),
        ENTRY( "TC_LBT_034_38L", CCellbasedtriggerOperations::TC_LBT_034_38_testL ),
        ENTRY( "TC_LBT_034_39L", CCellbasedtriggerOperations::TC_LBT_034_39_testL ),
        ENTRY( "TC_LBT_034_40L", CCellbasedtriggerOperations::TC_LBT_034_40_testL ),
        ENTRY( "TC_LBT_034_41L", CCellbasedtriggerOperations::TC_LBT_034_41_testL ),
        ENTRY( "TC_LBT_034_42L", CCellbasedtriggerOperations::TC_LBT_034_42_testL ),
        ENTRY( "TC_LBT_034_43L", CCellbasedtriggerOperations::TC_LBT_034_43_testL ),
        ENTRY( "TC_LBT_034_44L", CCellbasedtriggerOperations::TC_LBT_034_44_testL ),
        ENTRY( "TC_LBT_034_45L", CCellbasedtriggerOperations::TC_LBT_034_45_testL ),
        /*ENTRY( "TC_LBT_034_46L", CCellbasedtriggerOperations::TC_LBT_034_46_testL ),
        ENTRY( "TC_LBT_034_47L", CCellbasedtriggerOperations::TC_LBT_034_47_testL ),
        ENTRY( "TC_LBT_034_48L", CCellbasedtriggerOperations::TC_LBT_034_48_testL ),
        ENTRY( "TC_LBT_034_49L", CCellbasedtriggerOperations::TC_LBT_034_49_testL ),
        ENTRY( "TC_LBT_034_50L", CCellbasedtriggerOperations::TC_LBT_034_50_testL ),
        ENTRY( "TC_LBT_034_51L", CCellbasedtriggerOperations::TC_LBT_034_51_testL ),
        ENTRY( "TC_LBT_034_52L", CCellbasedtriggerOperations::TC_LBT_034_52_testL ),
        ENTRY( "TC_LBT_034_53L", CCellbasedtriggerOperations::TC_LBT_034_53_testL ),*/
        ENTRY( "TC_LBT_034_54L", CCellbasedtriggerOperations::TC_LBT_034_54_testL ),
        ENTRY( "TC_LBT_034_55L", CCellbasedtriggerOperations::TC_LBT_034_55_testL ),
        ENTRY( "TC_LBT_034_56L", CCellbasedtriggerOperations::TC_LBT_034_56_testL ),
        /*ENTRY( "TC_LBT_034_57L", CCellbasedtriggerOperations::TC_LBT_034_57_testL ),
        ENTRY( "TC_LBT_034_58L", CCellbasedtriggerOperations::TC_LBT_034_58_testL ),
        ENTRY( "TC_LBT_034_59L", CCellbasedtriggerOperations::TC_LBT_034_59_testL ),
        ENTRY( "TC_LBT_034_60L", CCellbasedtriggerOperations::TC_LBT_034_60_testL ),*/
        ENTRY( "TC_LBT_034_61L", CCellbasedtriggerOperations::TC_LBT_034_61_testL ),
        ENTRY( "TC_LBT_034_62L", CCellbasedtriggerOperations::TC_LBT_034_62_testL ),
        
        ENTRY( "TC_LBT_034_65L", CCellbasedtriggerOperations::TC_LBT_034_65_testL ),
        ENTRY( "TC_LBT_034_66L", CCellbasedtriggerOperations::TC_LBT_034_66_testL ),
        ENTRY( "TC_LBT_034_67L", CCellbasedtriggerOperations::TC_LBT_034_67_testL ),
        ENTRY( "TC_LBT_034_68L", CCellbasedtriggerOperations::TC_LBT_034_68_testL ),
        ENTRY( "StartCellChangeNotifier", CCellbasedtriggerOperations::StartCellChangeNotifier_testL ),
        ENTRY( "TC_LBT_034_69L", CCellbasedtriggerOperations::TC_LBT_034_69_testL ),
        ENTRY( "TC_LBT_034_70L", CCellbasedtriggerOperations::TC_LBT_034_70_testL ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Ct_lbtltapi::AssertTrueL
// function for condition check.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCellbasedtriggerOperations::AssertTrueL( TBool aCondition,const TDesC& aMessage )
    {
           
         if(!aCondition)
         {
          
    	   iLog->Log( aMessage );
           User::LeaveIfError(KErrGeneral);
         }
    
    }

//Allocates and constructs a new geographical  cell area object 1.
TInt CCellbasedtriggerOperations::TC_LBT_034_01_testL( CStifItemParser& aItem )
    {

    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea );
    
    Cellarea->SetNetworkType(RMobilePhone :: ENetworkModeGsm);
    
    TBuf<4> Networkcountrycode = _L("1000");
    Cellarea->SetNetworkCountryCode(Networkcountrycode);
    
    TBuf<8> Networkidentitycode = _L("10000000");
    Cellarea->SetNetworkIdentityCode(Networkidentitycode);
    
    TUint Locationareacode = 100;
    Cellarea->SetLocationAreaCode(Locationareacode);
    
    TUint CellId = 100;
    Cellarea->SetCellId(CellId);
    
        
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(Cellarea->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(Cellarea->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(Cellarea->CellId()==CellId,_L("Wrong Cellid"));         
   
        
    CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;

    }
    
    //Allocates and constructs a new geographical current cell area object 2.
TInt CCellbasedtriggerOperations::TC_LBT_034_02_testL( CStifItemParser& aItem )
    {

    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewLC();
    
    Cellarea->SetNetworkType(RMobilePhone :: ENetworkModeGsm);
    
    TBuf<4> Networkcountrycode = _L("1000");
    Cellarea->SetNetworkCountryCode(Networkcountrycode);
    
    TBuf<8> Networkidentitycode = _L("10000000");
    Cellarea->SetNetworkIdentityCode(Networkidentitycode);
    
    TUint Locationareacode = 100;
    Cellarea->SetLocationAreaCode(Locationareacode);
    
    TUint CellId = 100;
    Cellarea->SetCellId(CellId);
    
     
    
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(Cellarea->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(Cellarea->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(Cellarea->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));   
        
    CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;

    }
    
    
   
 //Allocates and constructs a new geographical current cell area object 3.
TInt CCellbasedtriggerOperations::TC_LBT_034_03_testL( CStifItemParser& aItem )
    {

    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea );
       
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(Cellarea->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(Cellarea->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(Cellarea->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));   
        
    CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;

    }   
   
    //Allocates and constructs a new geographical current cell area object 4.
TInt CCellbasedtriggerOperations::TC_LBT_034_04_testL( CStifItemParser& aItem )
    {

    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
     
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
           
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(Cellarea->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(Cellarea->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(Cellarea->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));   
        
   // CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;

    }   
    
 
         //Setting invalid values for geographical cellular area object attributes.
TInt CCellbasedtriggerOperations::TC_LBT_034_05_testL( CStifItemParser& aItem )
    {

    TBuf<4> Networkcountrycode = _L("");
    TBuf<8> Networkidentitycode = _L("");
    TInt Locationareacode = -1;
    TInt CellId = -1;
     TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
       
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea );
       
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(Cellarea->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(Cellarea->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(Cellarea->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));   
        
    CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;

    }   
    
    
 //Internalize & Externalize(CLbtGeoCurrentCell).   
    
TInt CCellbasedtriggerOperations::TC_LBT_034_06_testL( CStifItemParser& /* aItem */ )
    {

     
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea );
    
    Cellarea->SetNetworkType(RMobilePhone :: ENetworkModeGsm);
    TBuf<4> Networkcountrycode = _L("1000");
    Cellarea->SetNetworkCountryCode(Networkcountrycode);
    
    TBuf<8> Networkidentitycode = _L("10000000");
    Cellarea->SetNetworkIdentityCode(Networkidentitycode);
    
    TUint Locationareacode = 100;
    Cellarea->SetLocationAreaCode(Locationareacode);
    
    TUint CellId = 100;
    Cellarea->SetCellId(CellId);
    
   
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    Cellarea->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtGeoCell* Cellarea2 = CLbtGeoCell::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea2 ); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    Cellarea2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
		AssertTrueL(Cellarea2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(Cellarea2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(Cellarea2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(Cellarea2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(Cellarea2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));  
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }  
    
  //Allocates and constructs a new Trigger Condition area object using CLbtGeoCurrentCell 1  
TInt CCellbasedtriggerOperations::TC_LBT_034_07_testL( CStifItemParser& /* aItem */ )
    {

    
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
  
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
     
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    cond1->SetTriggerArea( Cellarea );
    
        
    CLbtGeoAreaBase* base=cond1->TriggerArea();
    
    AssertTrueL(base->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(static_cast<CLbtGeoCell*>(base)->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(static_cast<CLbtGeoCell*>(base)->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(static_cast<CLbtGeoCell*>(base)->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(static_cast<CLbtGeoCell*>(base)->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(static_cast<CLbtGeoCell*>(base)->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(base->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));  
    CleanupStack::PopAndDestroy( 1 );
    
    return KErrNone;
    
    }      
    
    
    
    //Allocates and constructs a new Trigger Condition area object using CLbtGeoCurrentCell 2
    TInt CCellbasedtriggerOperations::TC_LBT_034_08_testL( CStifItemParser& /* aItem */ )
    {

     TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
        
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL(Cellarea,CLbtTriggerConditionArea::EFireOnExit );
    CleanupStack::PushL( cond1 ); 

    CLbtGeoAreaBase* base=cond1->TriggerArea();
    
		AssertTrueL(base->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(static_cast<CLbtGeoCell*>(base)->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(static_cast<CLbtGeoCell*>(base)->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(static_cast<CLbtGeoCell*>(base)->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(static_cast<CLbtGeoCell*>(base)->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(static_cast<CLbtGeoCell*>(base)->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(base->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
        
    CleanupStack::PopAndDestroy( 1 );
    
    return KErrNone;
    
    }      
    
    
  //Allocates and constructs a new Trigger Condition area object using CLbtGeoCurrentCell 3  
TInt CCellbasedtriggerOperations::TC_LBT_034_09_testL( CStifItemParser& /* aItem */ )
    {
    
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewLC( );
       
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
    
   
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
        
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    cond1->SetTriggerArea( Cellarea );
    
        
    CLbtGeoAreaBase* base=cond1->TriggerArea();
    
        
    AssertTrueL(base->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(static_cast<CLbtGeoCell*>(base)->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(static_cast<CLbtGeoCell*>(base)->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(static_cast<CLbtGeoCell*>(base)->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(static_cast<CLbtGeoCell*>(base)->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(static_cast<CLbtGeoCell*>(base)->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(base->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
        
    CleanupStack::PopAndDestroy( 1 );
    
    return KErrNone;
    
    }        
    
    
    
    //Internalize & Externalize (ClbtTriggerConditionArea)
    TInt CCellbasedtriggerOperations::TC_LBT_034_10_testL( CStifItemParser& /* aItem */ )
    {

    
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
        
    cond1->SetTriggerArea( Cellarea );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);    
    cond1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
    CLbtTriggerConditionArea* cond2= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond2 );
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);    
    cond2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset(); 
    
    CLbtGeoAreaBase* base=cond2->TriggerArea();
    
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (base);
    }
    
     AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(cell2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(cell2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
        
        
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }
    
    
    //Allocates and constructs a new CLbtTriggerFilterByArea object using CLbtGeoCurrentCell 1
    TInt CCellbasedtriggerOperations::TC_LBT_034_11_testL( CStifItemParser& /* aItem */ )
    {
            
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL();   
    
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    areafilter->SetArea(Cellarea);
    
  
    CLbtGeoAreaBase* geobase=areafilter->Area();
    CLbtGeoCell* cell2=NULL;
    if(geobase->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (geobase);
    }
    
     AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(cell2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(cell2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
    

    return KErrNone;
     }
     
     
        //Allocates and constructs a new CLbtTriggerFilterByArea object using CLbtGeoCurrentCell 2
    TInt CCellbasedtriggerOperations::TC_LBT_034_12_testL( CStifItemParser& /* aItem */ )
    {
            
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Cellarea);   
    
    
    CLbtGeoAreaBase* geobase=areafilter->Area();
    
    CLbtGeoCell* cell2=NULL;
    if(geobase->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (geobase);
    }
    
     AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(cell2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(cell2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
    
    

    return KErrNone;
     }
     
     
     
     //Allocates and constructs a new CLbtTriggerFilterByArea object using CLbtGeoCurrentCell 3
    TInt CCellbasedtriggerOperations::TC_LBT_034_13_testL( CStifItemParser& /* aItem */ )
    {
            
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewLC();   
    
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
         
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
        
    areafilter->SetArea(Cellarea);
    
  
    CLbtGeoAreaBase* geobase=areafilter->Area();
    CLbtGeoCell* cell2=NULL;
    if(geobase->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (geobase);
    }
    
     AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(cell2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(cell2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
    
    
	CleanupStack::PopAndDestroy( areafilter );
    return KErrNone;
     }
     
     
     //Internalize & Externalize(CLbtTriggerFilterByArea).
   TInt CCellbasedtriggerOperations::TC_LBT_034_14_testL( CStifItemParser& /* aItem */ )
    {

    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    CLbtTriggerFilterByArea* filterarea1= CLbtTriggerFilterByArea::NewL( Cellarea );
    CleanupStack::PushL( filterarea1 );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    filterarea1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtTriggerFilterByArea* filterarea2= CLbtTriggerFilterByArea::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( filterarea2); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    filterarea2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    CLbtGeoAreaBase* base=filterarea2->Area();
    
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (base);
    }
    
     AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(cell2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(cell2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
    CleanupStack::Pop(filterarea2);
    CleanupStack::Pop(buffer);
    CleanupStack::Pop(filterarea1);
    return KErrNone;
    
    }  
    
    
    //Creation of Cellular based Entry type startup trigger
    TInt CCellbasedtriggerOperations::TC_LBT_034_15_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
	
	TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
   
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object
	
	 TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
   	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
   Triginfo= lbt.GetTriggerLC(trigId);
    
    if(Triginfo==NULL)
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return -1;	
    }
    else
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return KErrNone;
    }
    //  return KErrNone; 
    }
    
    //Creation of Cellular based Exit type startup trigger
    
     TInt CCellbasedtriggerOperations::TC_LBT_034_16_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
	
	TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
         
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
   /* iLog->Log(_L("Trigger Created"));
    
   	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
   Triginfo= lbt.GetTriggerLC(trigId);*/
    TInt error=  notifier->iStatus.Int();
    if(error==KErrNotSupported)
    {
    //CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return KErrNone;	
    }
    else
    {
    //CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return -99;
    }
    }
    
    //Creation of Cellular based Entry type session trigger
     TInt CCellbasedtriggerOperations::TC_LBT_034_17_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
          
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
	
	TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    
   	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
   Triginfo= lbt.GetTriggerLC(trigId);
    
    if(Triginfo==NULL)
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return -1;	
    }
    else
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return KErrNone;
    }
    }
    
    //Creation of Cellular based Exit type session trigger
      TInt CCellbasedtriggerOperations::TC_LBT_034_18_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
          
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
	
	TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
      //set Requestor     
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    /* iLog->Log(_L("Trigger Created"));
    
   	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
   Triginfo= lbt.GetTriggerLC(trigId);*/
    TInt error=  notifier->iStatus.Int();
    if(error==KErrNotSupported)
    {
   // CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return KErrNone;	
    }
    else
    {
   // CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return -99;
    }
    }
 
 
 //Creation of multiple Cellular area based startup triggers
   TInt CCellbasedtriggerOperations::TC_LBT_034_19_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	//Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 
	  //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
          
    // set condition
	
	TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object

    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    for(int i =0;i<3 ;i++)
    {
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    }
    
    
    iLog->Log(_L("Trigger Created"));
    
   	RPointerArray < CLbtTriggerInfo > trigInfoList;    
    lbt.GetTriggersL(trigInfoList);
    
    if(trigInfoList.Count()!=3)
    {
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return -99;	
    }
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return KErrNone;
    }
    //  return KErrNone; 
    }
    
    //Creation of Disabled Cellular based startup trigger
     TInt CCellbasedtriggerOperations::TC_LBT_034_20_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
	
	TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object
	//set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
	trig->SetState(CLbtTriggerEntry::EStateDisabled);
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    
   	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
   Triginfo= lbt.GetTriggerLC(trigId);
   CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    CLbtTriggerEntry::TLbtTriggerState state = TrigEntry->State() ; 
    if(state !=CLbtTriggerEntry::EStateDisabled)
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return -99;	
    } 

    else
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return KErrNone;
    }
    //  return KErrNone; 
    }
    
    //Creation of Cellular based startup trigger when network is not available
    TInt CCellbasedtriggerOperations::TC_LBT_034_21_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
	
	TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object

    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    
   	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo= lbt.GetTriggerLC(trigId);
    
    if(Triginfo==NULL)
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return -1;	
    }
    else
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return KErrNone;
    }
    }
    
    //Deletion of Cellular based session trigger
     TInt CCellbasedtriggerOperations::TC_LBT_034_22_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
          
    // set condition
	
	TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object

    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    
    lbt.DeleteTriggerL(trigId);
   
   	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
   TRAPD(error, Triginfo= lbt.GetTriggerLC(trigId);
    CleanupStack::PopAndDestroy( ););
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    
    if(error==KErrNotFound)
    {
    
	//delete wait;
	return KErrNone;	
    }
    else
    {
    
	return -99;
    }
    //  return KErrNone; 
    }
    
    //Deletion of Cellular based startup trigger
      TInt CCellbasedtriggerOperations::TC_LBT_034_23_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
	
	TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object

    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    lbt.DeleteTriggerL(trigId);
    
   	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
   TRAPD(error, Triginfo= lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy( ););
    
   CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrNotFound)
    {
    
	//delete wait;
	return KErrNone;	
    }
    else
    {
    
	//delete wait;
	return -99;
    }
    //  return KErrNone; 
    }
    
   // Deleting triggers in given Cellular using area filter synchronously
  TInt CCellbasedtriggerOperations::TC_LBT_034_24_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
     
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(Cellarea,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    CLbtGeoCell* Cellarea2  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
        
    CLbtTriggerFilterByArea* filter=CLbtTriggerFilterByArea::NewL(Cellarea2); 
    CleanupStack::PushL( filter );    
    
   
    lbt.DeleteTriggersL(filter);
    CleanupStack::PopAndDestroy( filter );
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);
    CleanupStack::PopAndDestroy(););
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::Pop( trig1 );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    
    if(error==KErrNotFound)
    {
    
	//delete wait;
	return KErrNone;	
    }
    else
    {
    
	//delete wait;
	return -99;
    }
    
    }
    
    //Deleting triggers in current Cellular using area filter asynchronously  
    
    TInt CCellbasedtriggerOperations::TC_LBT_034_25_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
     
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(Cellarea,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
        
    CLbtTriggerFilterByArea* filter=CLbtTriggerFilterByArea::NewL(Cellarea); 
    CleanupStack::PushL( filter );    
    
   
    notifier->DeleteFilteredTriggers(lbt,filter,wait);
    wait->Start();
    CleanupStack::PopAndDestroy( filter );
//     CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1); CleanupStack::PopAndDestroy(););
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::Pop( trig1 );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    
    if(error==KErrNotFound)
    {
    
	//delete wait;
	return KErrNone;	
    }
    else
    {
    
	//delete wait;
	return -99;
    }
    
    }  
    
    //Retrieving informations about a Cellular based startup trigger
     TInt CCellbasedtriggerOperations::TC_LBT_034_26_testL( CStifItemParser& /* aItem */ )
    {
// 	_LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
  //  CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId,trigId2;
     trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    //User::After(5000000);
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    
   // CleanupStack::PopAndDestroy( trig );
   
    //Check the name of the trigger
    TDesC Name2 =TrigEntry->Name(); 
    if(TrigEntry->Name()!=trig->Name()) 
    {
    	return -99;
    }
    //Check the state of the trigger
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=trig->State())
    {
    	return -99;
    }
    //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 != trig->Type())
    {
    	return -99;
    }
    //Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	CLbtTriggerConditionArea::TDirection direction;
	direction = Condition2->Direction();
	if(direction!=CLbtTriggerConditionArea::EFireOnEnter)
	{
		return -99;
	}
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	
	CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId,_L("Wrong Cellid"));         
	//Check requestors 
    RRequestorStack   aRequestors,aRequestors2 ;
    TrigEntry->GetRequestorsL(aRequestors2);
    trig->GetRequestorsL(aRequestors);
   for(TInt i=0;i<aRequestors.Count();i++)
    {
    	CRequestor *requestor = aRequestors[i];
    	if(requestor->RequestorType()!=NULL)
    	{
    		return -99;
    	}
    	if(requestor->RequestorData()!=KNullDesC)
    	{
    		return -99;
    	}
    	else if(requestor->RequestorFormat()!=NULL)
    	{
    		return -99;
    	}
    
    }
    //Check trigger Id
    trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
    	return -99;
    }
    //Check ManagerUI
    TUid Managerui2 = TrigEntry->ManagerUi();
    if(Managerui2!=aManagerUi)
    {
    	return -99;
    }
    //Check validity
	TLbtTriggerDynamicInfo::TLbtTriggerValidity validity;
   validity= dyninfo -> iValidity;
   if(validity!=TLbtTriggerDynamicInfo::EValid)
   {
   	return -99;
   }
   //Check Activity status
/*	TLbtTriggerDynamicInfo::TLbtTriggerActivityStatus currstatus;
	currstatus = dyninfo -> iActivityStatus;
    if(currstatus!=TLbtTriggerDynamicInfo::EActive)
    {
    	return -99;
    }*/
    CleanupStack::PopAndDestroy(1 );//trig info
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // //delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    //Retrieving informations about a Cellular based session trigger
    
       TInt CCellbasedtriggerOperations::TC_LBT_034_27_testL( CStifItemParser& /* aItem */ )
    {
// 	_LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 
	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
  //  CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
       
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId,trigId2;
     trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    //User::After(5000000);
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    
   // CleanupStack::PopAndDestroy( trig );
   
    //Check the name of the trigger
    TDesC Name2 =TrigEntry->Name(); 
    if(TrigEntry->Name()!=trig->Name()) 
    {
    	return -99;
    }
    //Check the state of the trigger
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=trig->State())
    {
    	return -99;
    }
    //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 != trig->Type())
    {
    	return -99;
    }
    //Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	CLbtTriggerConditionArea::TDirection direction;
	direction = Condition2->Direction();
	if(direction!=CLbtTriggerConditionArea::EFireOnEnter)
	{
		return -99;
	}
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId,_L("Wrong Cellid"));         
	//Check requestors 
    RRequestorStack   aRequestors,aRequestors2 ;
    TrigEntry->GetRequestorsL(aRequestors2);
    trig->GetRequestorsL(aRequestors);
   for(TInt i=0;i<aRequestors.Count();i++)
    {
    	CRequestor *requestor = aRequestors[i];
    	if(requestor->RequestorType()!=NULL)
    	{
    		return -99;
    	}
    	if(requestor->RequestorData()!=KNullDesC)
    	{
    		return -99;
    	}
    	else if(requestor->RequestorFormat()!=NULL)
    	{
    		return -99;
    	}
    
    }
    //Check trigger Id
    trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
    	return -99;
    }
    //Check ManagerUI
    TUid Managerui2 = TrigEntry->ManagerUi();
    if(Managerui2!=aManagerUi)
    {
    	return -99;
    }
    //Check validity
	TLbtTriggerDynamicInfo::TLbtTriggerValidity validity;
   validity= dyninfo -> iValidity;
   if(validity!=TLbtTriggerDynamicInfo::EValid)
   {
   	return -99;
   }
   //Check Activity status
/*	TLbtTriggerDynamicInfo::TLbtTriggerActivityStatus currstatus;
	currstatus = dyninfo -> iActivityStatus;
    if(currstatus!=TLbtTriggerDynamicInfo::EActive)
    {
    	return -99;
    }*/
    CleanupStack::PopAndDestroy(1 );//trig info
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // //delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
    //Listing of triggers in given cellular area using area filter synchronously
    TInt CCellbasedtriggerOperations::TC_LBT_034_28_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    
    	  //Construct a session trigger
    CLbtSessionTrigger* sessiontrig = CLbtSessionTrigger::NewL();
    
    CLbtStartupTrigger* startuptrig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( sessiontrig );
    
    // Set Name
    sessiontrig->SetNameL(_L("Trigger1"));
    startuptrig->SetNameL(_L("Trigger2"));
    
    // set condition
    
    TBuf<4> Networkcountrycode1 = _L("1000");
    TBuf<8> Networkidentitycode1 = _L("10000000");
    TUint Locationareacode1 = 100;
    TUint CellId1 = 100;
    
    CLbtGeoCell* Cellarea1  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode1,
   														Networkidentitycode1,
   														Locationareacode1,
   														CellId1
   														);
   														
   	TBuf<4> Networkcountrycode2 = _L("2000");
    TBuf<8> Networkidentitycode2 = _L("20000000");
    TUint Locationareacode2 = 200;
    TUint CellId2 = 200;
    
    CLbtGeoCell* Cellarea2  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode2,
   														Networkidentitycode2,
   														Locationareacode2,
   														CellId2
   														);
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    sessiontrig->SetCondition(condition); // ownership transferred to object
	
	//set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	sessiontrig->SetRequestorL(ReqType,ReqFormat,ReqData);
	startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
    sessiontrig->SetManagerUi(aManagerUi);   
    startuptrig->SetManagerUi(aManagerUi);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    for(int i=0;i<10;i++)
    {
    	notifier->CreateTriggers( lbt,*sessiontrig,trigId,EFalse,wait );
    wait->Start( );
    }
    
     iLog->Log(_L("CreateTrigger Complete  "));  
    condition->SetTriggerArea(Cellarea2);
    startuptrig->SetCondition(condition);
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
    
     TSecureId secureid;
    startuptrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Create another trigger with cell area 2
    notifier->CreateTriggers( lbt,*startuptrig,trigId,EFalse,wait );
    wait->Start( );
     User::After(5000000);
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    
     CLbtGeoCell* Cellarea3  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode1,
   														Networkidentitycode1,
   														Locationareacode1,
   														CellId1
   														);
    
    areaFilter->SetArea( Cellarea3 ); 
    
     listOptions->SetFilter( areaFilter ); 
     
     CleanupStack::Pop( areaFilter ); 
     
     lbt.GetTriggersL(trigInfoList,listOptions );
      iLog->Log(_L("GetTriggers Complete  "));  
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
   for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
   //Check area of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	
	CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId1,_L("Wrong Cellid"));     
    }
      iLog->Log(_L("All checkings done "));    
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    iLog->Log(_L("Connection closed "));
	return KErrNone;
      
    }
    
    //Listing of triggers in current cellular area  using area filter asynchronously
    TInt CCellbasedtriggerOperations::TC_LBT_034_29_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    
	  //Construct a session trigger
    CLbtSessionTrigger* sessiontrig = CLbtSessionTrigger::NewL();
    
    CLbtStartupTrigger* startuptrig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( sessiontrig );
    
    // Set Name
    sessiontrig->SetNameL(_L("Trigger1"));
    startuptrig->SetNameL(_L("Trigger2"));
    
    // set condition
    
    TBuf<4> Networkcountrycode1 = _L("1000");
    TBuf<8> Networkidentitycode1 = _L("10000000");
    TUint Locationareacode1 = 100;
    TUint CellId1 = 100;
    
    CLbtGeoCell* Cellarea1  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode1,
   														Networkidentitycode1,
   														Locationareacode1,
   														CellId1
   														);
   														
   	TBuf<4> Networkcountrycode2 = _L("2000");
    TBuf<8> Networkidentitycode2 = _L("20000000");
    TUint Locationareacode2 = 200;
    TUint CellId2 = 200;
    
    CLbtGeoCell* Cellarea2  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode2,
   														Networkidentitycode2,
   														Locationareacode2,
   														CellId2
   														);
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    sessiontrig->SetCondition(condition); // ownership transferred to object

    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData);
		startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
    sessiontrig->SetManagerUi(aManagerUi);   
    startuptrig->SetManagerUi(aManagerUi);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*sessiontrig,trigId,EFalse,wait );
    wait->Start( );
    condition->SetTriggerArea(Cellarea2);
    startuptrig->SetCondition(condition);
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
    //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
    // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
     
     TSecureId secureid;
    startuptrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Create another trigger with cell area 2
    notifier->CreateTriggers( lbt,*startuptrig,trigId,EFalse,wait );
    wait->Start( );
     User::After(5000000);
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    
     CLbtGeoCell* Cellarea3  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode1,
   														Networkidentitycode1,
   														Locationareacode1,
   														CellId1
   														);
    
    areaFilter->SetArea( Cellarea3 ); 
    
     listOptions->SetFilter( areaFilter ); 
     
     CleanupStack::Pop( areaFilter ); 
     
     notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
     wait->Start();
    
        
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
   for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
   //Check area of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	
	CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId1,_L("Wrong Cellid"));         
    }
       
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
      
    }
    
    //Updating attributes of a Cellular based startup trigger
    
      TInt CCellbasedtriggerOperations::TC_LBT_034_30_testL( CStifItemParser& /* aItem */ )
    {

 	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
     
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 //	  #if 0
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "LbtReference.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    trig->SetCondition(condition); // ownership transferred to object

    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
     TLbtTriggerId trigId,trigId2;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TBuf<4> Networkcountrycode2 = _L("2000");
    TBuf<8> Networkidentitycode2 = _L("20000000");
    TUint Locationareacode2 = 200;
    TUint CellId2 = 200;
    
    CLbtGeoCell* Cellarea2  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode2,
   														Networkidentitycode2,
   														Locationareacode2,
   														CellId2
   														);
         
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                Cellarea2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
        
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
   // lbt.UpdateTriggerL(*newtrig);
    iLog->Log(_L("After UpdateTriggerL "));
    
    iLog->Log(_L("Before GetTriggerLC "));
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    //Check the name of the trigger
    TDesC Name2 =TrigEntry->Name(); 
    if(TrigEntry->Name()!=newtrig->Name()) 
    {
    	return -99;
    }
    //Check the state of the trigger
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=newtrig->State())
    {
    	return -99;
    }
    //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 != newtrig->Type())
    {
    	return -99;
    }
    //Check Condition of the trigger
    
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	
	CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId2,_L("Wrong Cellid"));     
    //}
    //Check trigger Id
    trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
    	return -99;
    }
    
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   // #endif
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
   // //delete wait;
   
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
    
     //Updating attributes of a Cellular based session trigger
    
      TInt CCellbasedtriggerOperations::TC_LBT_034_31_testL( CStifItemParser& /* aItem */ )
    {

 	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
     
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 //	  #if 0
	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    trig->SetCondition(condition); // ownership transferred to object

    
    trig->SetManagerUi(aManagerUi);   
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
     TLbtTriggerId trigId,trigId2;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtSessionTrigger* newtrig = CLbtSessionTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TBuf<4> Networkcountrycode2 = _L("2000");
    TBuf<8> Networkidentitycode2 = _L("20000000");
    TUint Locationareacode2 = 200;
    TUint CellId2 = 200;
    
    CLbtGeoCell* Cellarea2  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode2,
   														Networkidentitycode2,
   														Locationareacode2,
   														CellId2
   														);
         
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                Cellarea2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
        
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
   // lbt.UpdateTriggerL(*newtrig);
    iLog->Log(_L("After UpdateTriggerL "));
    
    iLog->Log(_L("Before GetTriggerLC "));
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    //Check the name of the trigger
    TDesC Name2 =TrigEntry->Name(); 
    if(TrigEntry->Name()!=newtrig->Name()) 
    {
    	return -99;
    }
    //Check the state of the trigger
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=newtrig->State())
    {
    	return -99;
    }
    //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 != newtrig->Type())
    {
    	return -99;
    }
    //Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	
	CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId2,_L("Wrong Cellid"));     
    
    //Check trigger Id
  	trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
    	return -99;
    }
    
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   // #endif
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
   // //delete wait;
   
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
    //Updating state of  the all the triggers using NULL filter synchronously 
    TInt CCellbasedtriggerOperations::TC_LBT_034_32_testL( CStifItemParser& /* aItem */ )
    {

 	RLbtServer lbtserver;
    RLbt lbt;
    iLog->Log(_L("Before Connect"));
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    iLog->Log(_L("Before Open"));
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	        
     	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    
    
 //   trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );    
        
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateDisabled,ELbtTrue);    
      
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    iLog->Log(_L("Before GetTriggersL"));
    lbt.GetTriggersL(trigInfoList);
    iLog->Log(_L("After GetTriggersL"));
    
    //Do something with the trigger list
   
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    	if(CLbtTriggerEntry::EStateDisabled!=(TrigEntry->State()))
    	{
    		return -99;
    	}
    }
    
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
	CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&lbtserver);
    return KErrNone;  
    }
    
    
    //Updating state of  the all the triggers using NULL filter asynchronously 
    TInt CCellbasedtriggerOperations::TC_LBT_034_33_testL( CStifItemParser& /* aItem */ )
    {

 	RLbtServer lbtserver;
    RLbt lbt;
    iLog->Log(_L("Before Connect"));
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    iLog->Log(_L("Before Open"));
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	        
     	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
        
   // trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );    
        
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateDisabled,wait);
    wait->Start();
      
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    iLog->Log(_L("Before GetTriggersL"));
    lbt.GetTriggersL(trigInfoList);
    iLog->Log(_L("After GetTriggersL"));
    
    //Do something with the trigger list
   
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    	if(CLbtTriggerEntry::EStateDisabled!=(TrigEntry->State()))
    	{
    		return -99;
    	}
    }
    
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
	CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&lbtserver);
    return KErrNone;  
    }
    
    
         //1.1	Updating state of  the Cellular based triggers filtered based on area filter synchronously
      TInt CCellbasedtriggerOperations::TC_LBT_034_34_testL( CStifItemParser& /* aItem */ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    
    	  //Construct a session trigger
    CLbtSessionTrigger* sessiontrig = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* startuptrig = CLbtStartupTrigger::NewL();
    //Push to cleanup stack
    CleanupStack::PushL( sessiontrig );
    
    // Set Name
    sessiontrig->SetNameL(_L("Trigger1"));
    startuptrig->SetNameL(_L("Trigger2"));
    
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    sessiontrig->SetState(CLbtTriggerEntry::EStateDisabled);
    startuptrig->SetState(CLbtTriggerEntry::EStateDisabled);
    sessiontrig->SetCondition(condition); // ownership transferred to object
    startuptrig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    sessiontrig->SetManagerUi(aManagerUi);   
    startuptrig->SetManagerUi(aManagerUi);   
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
    
    TSecureId secureid;
   startuptrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
	sessiontrig->SetRequestorL(ReqType,ReqFormat,ReqData);
	 
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*sessiontrig,trigId,EFalse,wait );
    wait->Start( );
    
    notifier->CreateTriggers( lbt,*startuptrig,trigId,EFalse,wait );
    wait->Start( );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    
    areaFilter->SetArea( Cellarea ); 
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    listOptions->SetFilter(areaFilter);
    
     lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,areaFilter);   
     
          
     lbt.GetTriggersL(trigInfoList ,listOptions);
        
     CleanupStack::PopAndDestroy( listOptions );
     CleanupStack::Pop( areaFilter );  
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
	    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
	   //Check area of the trigger
		CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

		CLbtGeoAreaBase* area = Condition2->TriggerArea();
		CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId,_L("Wrong Cellid"));      
	CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=CLbtTriggerEntry::EStateEnabled)
    {
    	 CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( sessiontrig );
	   // CleanupStack::PopAndDestroy( 1 );
	    CleanupStack::PopAndDestroy( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
    	return -99;
    }
	
		
	    
	  }
       
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
   CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
   // CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
 
    }
    
    
          //1.1	Updating state of  the Cellular based triggers filtered based on area filter asynchronously
    TInt CCellbasedtriggerOperations::TC_LBT_034_35_testL( CStifItemParser& /* aItem */ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    
	  //Construct a session trigger
    CLbtSessionTrigger* sessiontrig = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* startuptrig = CLbtStartupTrigger::NewL();
    //Push to cleanup stack
    CleanupStack::PushL( sessiontrig );
    
    // Set Name
    sessiontrig->SetNameL(_L("Trigger1"));
    startuptrig->SetNameL(_L("Trigger2"));
    
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    sessiontrig->SetState(CLbtTriggerEntry::EStateDisabled);
    startuptrig->SetState(CLbtTriggerEntry::EStateDisabled);
    sessiontrig->SetCondition(condition); // ownership transferred to object
    startuptrig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    sessiontrig->SetManagerUi(aManagerUi);   
    startuptrig->SetManagerUi(aManagerUi);   
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
    
    TSecureId secureid;
   startuptrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
        
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
	sessiontrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*sessiontrig,trigId,EFalse,wait );
    wait->Start( );
    
    notifier->CreateTriggers( lbt,*startuptrig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    
    areaFilter->SetArea( Cellarea ); 
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    listOptions->SetFilter(areaFilter);
    
     notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateDisabled,wait,areaFilter);
    wait->Start();
     
     lbt.GetTriggersL(trigInfoList,listOptions );
     CleanupStack::PopAndDestroy( listOptions );  
     CleanupStack::Pop( areaFilter );  
    
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
	    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
	   //Check area of the trigger
		CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

		CLbtGeoAreaBase* area = Condition2->TriggerArea();
		CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId,_L("Wrong Cellid"));     
	CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=CLbtTriggerEntry::EStateDisabled)
    {
    	 CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( sessiontrig );
	   // CleanupStack::PopAndDestroy( 1 );
	    CleanupStack::PopAndDestroy( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
    	return -99;
    }
	
		
	    
	  }
       
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
   CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
   // CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
 
    }
    
    
        //1.1	Updating state of  the Cellular based triggers filtered based on composite filter synchronously
    TInt CCellbasedtriggerOperations::TC_LBT_034_36_testL( CStifItemParser& /* aItem */ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    
    	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
        
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter1 = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter1 );
    attribFilter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    attribFilter1->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    
    CLbtTriggerFilterByAttribute* attribFilter2 = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter2 );
    attribFilter2->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled);
    attribFilter2->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    
    areaFilter->SetArea( Cellarea ); 
    
     //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areaFilter ); 
    
    
    compositeFilter->AddFilterL( attribFilter1 ); 
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,compositeFilter);   
     
     compositeFilter->RemoveFilter(1);
     compositeFilter->AddFilterL( attribFilter2); 
     
     listOptions->SetFilter(compositeFilter);
     lbt.GetTriggersL(trigInfoList,listOptions );
    
    CleanupStack::PopAndDestroy( listOptions );//listoptions   
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( areaFilter ); 
    CleanupStack::Pop( attribFilter2 ); 
    CleanupStack::Pop( attribFilter1 );      
    
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
	    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
	   //Check area of the trigger
		CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

		CLbtGeoAreaBase* area = Condition2->TriggerArea();
		CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId,_L("Wrong Cellid"));        
	CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=CLbtTriggerEntry::EStateEnabled)
    {
    	 CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	   // CleanupStack::PopAndDestroy( 1 );
	    CleanupStack::PopAndDestroy( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
    	return -99;
    }
	
		
	    
	  }
       
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
   CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   // CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
 
    }
    
    
        //1.1	Updating state of  the Cellular based triggers filtered based on composite filter asynchronously
      TInt CCellbasedtriggerOperations::TC_LBT_034_37_testL( CStifItemParser& /* aItem */ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    
    	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
        
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    //Construct an attribute filter
     CLbtTriggerFilterByAttribute* attribFilter1 = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter1 );
    attribFilter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    attribFilter1->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    
    CLbtTriggerFilterByAttribute* attribFilter2 = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter2 );
    attribFilter2->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled);
    attribFilter2->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    
    areaFilter->SetArea( Cellarea ); 
    
     //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areaFilter ); 
    
    
    compositeFilter->AddFilterL( attribFilter1 ); 
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
      notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait,compositeFilter);
    wait->Start();
     
     compositeFilter->RemoveFilter(1);
     compositeFilter->AddFilterL( attribFilter2); 
     
     listOptions->SetFilter(compositeFilter);
     lbt.GetTriggersL(trigInfoList,listOptions );
        
    CleanupStack::PopAndDestroy( listOptions );
     CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( areaFilter ); 
CleanupStack::Pop( attribFilter2 ); 
    CleanupStack::Pop( attribFilter1 );    
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
	    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
	   //Check area of the trigger
		CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

		CLbtGeoAreaBase* area = Condition2->TriggerArea();
		CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId,_L("Wrong Cellid"));        
	CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=CLbtTriggerEntry::EStateEnabled)
    {
    	 CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	   // CleanupStack::PopAndDestroy( 1 );
	    CleanupStack::PopAndDestroy( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
    	return -99;
    }
	
		
	    
	  }
       
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
   CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   // CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
 
    }
    
    
    
     // Deleting triggers in given Cellular using composite filter synchronously
  TInt CCellbasedtriggerOperations::TC_LBT_034_38_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
     
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(Cellarea,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);  
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
        
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    
    areaFilter->SetArea( Cellarea ); 
    
     //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areaFilter ); 
    
    
    compositeFilter->AddFilterL( attribFilter ); 
    
   
    lbt.DeleteTriggersL(compositeFilter);
    
//    CLbtTriggerInfo* trigEntry=NULL;
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( areaFilter ); 
    CleanupStack::Pop( attribFilter );   
    TRAPD(error,lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy( ););
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig1 );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrNotFound)
    {
    
	//delete wait;
	return KErrNone;	
    }
    else
    {
    	//delete wait;
	return -99;
    }
    
    }
    
    //Deleting triggers in current Cellular using composite filter asynchronously  
    
    TInt CCellbasedtriggerOperations::TC_LBT_034_39_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
     
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(Cellarea,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);  
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
        
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    
    areaFilter->SetArea( Cellarea ); 
    
     //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areaFilter ); 
    
    
    compositeFilter->AddFilterL( attribFilter ); 
   
    notifier->DeleteFilteredTriggers(lbt,compositeFilter,wait);
    wait->Start();
//    CLbtTriggerInfo* trigEntry=NULL;
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( areaFilter ); 
    CleanupStack::Pop( attribFilter );   
    TRAPD(error,lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(););
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig1 );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrNotFound)
    {
    
	//delete wait;
	return KErrNone;	
    }
    else
    {
    
	//delete wait;
	return -99;
    }
       
    }  
    
    
       // Deleting triggers in given Cellular using NULL filter synchronously
  TInt CCellbasedtriggerOperations::TC_LBT_034_40_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
     
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(Cellarea,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);  
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
        
    lbt.DeleteTriggersL();
    
//    CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(error,lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy( notifier ););
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig1 );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrNotFound)
    {
    
	//delete wait;
	return KErrNone;	
    }
    else
    {
   
	//delete wait;
	return -99;
    }
        
   
    }
    
    //Deleting triggers in current Cellular using NULL filter asynchronously  
    
    TInt CCellbasedtriggerOperations::TC_LBT_034_41_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
     
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(Cellarea,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);  
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
        
    notifier->DeleteFilteredTriggers(lbt,NULL,wait);
    wait->Start();
//    CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(error,lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy( ););
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig1 );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrNotFound)
    {
    
	//delete wait;
	return KErrNone;	
    }
    else
    {
    
	//delete wait;
	return -99;
    }
    
    
    }  
    
    
    //Listing of triggers in given cellular area using composite filter synchronously
    TInt CCellbasedtriggerOperations::TC_LBT_034_42_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    
    	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
    TBuf<4> Networkcountrycode1 = _L("1000");
    TBuf<8> Networkidentitycode1 = _L("10000000");
    TUint Locationareacode1 = 100;
    TUint CellId1 = 100;
    
    CLbtGeoCell* Cellarea1  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode1,
   														Networkidentitycode1,
   														Locationareacode1,
   														CellId1
   														);
   														
   	TBuf<4> Networkcountrycode2 = _L("2000");
    TBuf<8> Networkidentitycode2 = _L("20000000");
    TUint Locationareacode2 = 200;
    TUint CellId2 = 200;
    
    CLbtGeoCell* Cellarea2  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode2,
   														Networkidentitycode2,
   														Locationareacode2,
   														CellId2
   														);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);      
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    condition->SetTriggerArea(Cellarea2);
    trig->SetCondition(condition);
    //Create another trigger with cell area 2
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
     User::After(5000000);
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
   //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    
    CLbtGeoCell* Cellarea3  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode1,
   														Networkidentitycode1,
   														Locationareacode1,
   														CellId1
   														);
    
    areaFilter->SetArea( Cellarea3 ); 
    
     //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areaFilter ); 
    
    
    compositeFilter->AddFilterL( attribFilter ); 
   
     listOptions->SetFilter( compositeFilter ); 
     
     CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( areaFilter ); 
    CleanupStack::Pop( attribFilter );   
     
    // RPointerArray < CLbtTriggerInfo > trigInfoList;
    
     lbt.GetTriggersL(trigInfoList,listOptions );
        
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
   for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
   //Check area of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

	CLbtGeoAreaBase* area = Condition2->TriggerArea();
		CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId1,_L("Wrong Cellid"));    
    }
       
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
      
    }
    
    //Listing of triggers in current cellular area  using composite filter asynchronously
    TInt CCellbasedtriggerOperations::TC_LBT_034_43_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    
    	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
     
    TBuf<4> Networkcountrycode1 = _L("1000");
    TBuf<8> Networkidentitycode1 = _L("10000000");
    TUint Locationareacode1 = 100;
    TUint CellId1 = 100;
    
    CLbtGeoCell* Cellarea1  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode1,
   														Networkidentitycode1,
   														Locationareacode1,
   														CellId1
   														);
   														
   	TBuf<4> Networkcountrycode2 = _L("2000");
    TBuf<8> Networkidentitycode2 = _L("20000000");
    TUint Locationareacode2 = 200;
    TUint CellId2 = 200;
    
    CLbtGeoCell* Cellarea2  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode2,
   														Networkidentitycode2,
   														Locationareacode2,
   														CellId2
   														);
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                Cellarea1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    trig->SetCondition(condition1); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
      //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    //condition->SetTriggerArea(Cellarea2);
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                Cellarea2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    trig->SetCondition(condition2);
    //Create another trigger with cell area 2
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
     User::After(5000000);
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    
     CLbtGeoCell* Cellarea3  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode1,
   														Networkidentitycode1,
   														Locationareacode1,
   														CellId1
   														);
    
    areaFilter->SetArea( Cellarea3 ); 
    
     //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areaFilter ); 
    
    
    compositeFilter->AddFilterL( attribFilter ); 
     listOptions->SetFilter( compositeFilter ); 
     
     CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( areaFilter ); 
    CleanupStack::Pop( attribFilter );   
     
   //  RPointerArray < CLbtTriggerInfo > trigInfoList;

     notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
     wait->Start();
    
        
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
   for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
   //Check area of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId1,_L("Wrong Cellid"));
    }
       
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
      
    }
    
    
    //Listing of triggers in given cellular area using NULL filter synchronously
    TInt CCellbasedtriggerOperations::TC_LBT_034_44_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    //Delete all the existing triggers
    TRAP_IGNORE(lbt.DeleteTriggersL());
    
	  //Construct a session trigger
    CLbtSessionTrigger* sessiontrig = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* startuptrig = CLbtStartupTrigger::NewL();
    //Push to cleanup stack
    CleanupStack::PushL( sessiontrig );
    
    // Set Name
    sessiontrig->SetNameL(_L("Trigger1"));
    startuptrig->SetNameL(_L("Trigger2"));
    
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    sessiontrig->SetState(CLbtTriggerEntry::EStateDisabled);
    startuptrig->SetState(CLbtTriggerEntry::EStateDisabled);
    sessiontrig->SetCondition(condition); // ownership transferred to object
    startuptrig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    sessiontrig->SetManagerUi(aManagerUi);   
    startuptrig->SetManagerUi(aManagerUi);   
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
    
    TSecureId secureid;
   startuptrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
      //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData); 
	
	
	sessiontrig->SetRequestorL(ReqType,ReqFormat,ReqData);      
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*sessiontrig,trigId,EFalse,wait );
    wait->Start( );
    
    notifier->CreateTriggers( lbt,*startuptrig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
     lbt.GetTriggersL(trigInfoList);
        
    TInt count = trigInfoList.Count();
   for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
   //Check area of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

	CLbtGeoAreaBase* area = Condition2->TriggerArea();
		CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId,_L("Wrong Cellid"));   
    }
       
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
      
    }
    
    //Listing of triggers in current cellular area  using NULL filter asynchronously
    TInt CCellbasedtriggerOperations::TC_LBT_034_45_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
     //Delete all the existing triggers
    TRAP_IGNORE(lbt.DeleteTriggersL());
    
	  //Construct a session trigger
    CLbtSessionTrigger* sessiontrig = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* startuptrig = CLbtStartupTrigger::NewL();
    //Push to cleanup stack
    CleanupStack::PushL( sessiontrig );
    
    // Set Name
    sessiontrig->SetNameL(_L("Trigger1"));
    startuptrig->SetNameL(_L("Trigger2"));
    
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    sessiontrig->SetState(CLbtTriggerEntry::EStateDisabled);
    startuptrig->SetState(CLbtTriggerEntry::EStateDisabled);
    sessiontrig->SetCondition(condition); // ownership transferred to object
    startuptrig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    sessiontrig->SetManagerUi(aManagerUi);   
    startuptrig->SetManagerUi(aManagerUi);   
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
    
    TSecureId secureid;
   startuptrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
        
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
	
	sessiontrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*sessiontrig,trigId,EFalse,wait );
    wait->Start( );
    
    notifier->CreateTriggers( lbt,*startuptrig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
     notifier->GetTriggersList(lbt,trigInfoList,wait);
     wait->Start();
          
   
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
   //Check area of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

	CLbtGeoAreaBase* area = Condition2->TriggerArea();
		CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId,_L("Wrong Cellid"));  
    }
       
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
      
    }
    
  /*  
    
    //Allocates and constructs a new geographical cellular area object for current cell.
TInt CCellbasedtriggerOperations::TC_LBT_034_46L( CStifItemParser& aItem )
    {

    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea );
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
   
    
   notifier->PopulateFromCurrentCell(*Cellarea,wait);
   
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()!=_L(""),_L("Wrong Network Country code"));   
	AssertTrueL(Cellarea->NetworkIdentityCode()!=_L(""),_L("Wrong Network identity code"));            
	AssertTrueL(Cellarea->LocationAreaCode()!=-1,_L("Wrong Location Area code"));   
	AssertTrueL(Cellarea->CellId()!=-1,_L("Wrong Cellid"));      
   
        
    CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;

    }
    */
    
   #if 0 
      //Creation startup trigger for current cell
    TInt CCellbasedtriggerOperations::TC_LBT_034_46_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition

 	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    CLbtGeoAreaBase* Cellarea = NULL;
    CLBTOperation *LBTOperation = notifier->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, &Cellarea, wait);
    wait->Start( );
    delete LBTOperation;
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object
	 //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
	
    TLbtTriggerId trigId;
           
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));    
    
   	CLbtTriggerInfo *Triginfo;
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo = lbt.GetTriggerLC(trigId);
    
    if(Triginfo == NULL)
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return -1;	
    }
    else
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return KErrNone;
    }
    //  return KErrNone; 
    }
    
      //Creation of session trigger for current cell
    TInt CCellbasedtriggerOperations::TC_LBT_034_47_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
       // set condition

 	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    CLbtGeoAreaBase* Cellarea = NULL; 
    CLBTOperation *LBTOperation = notifier->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, &Cellarea, wait);
    wait->Start( );
    delete LBTOperation;
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object
	 //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
           
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    
   	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
   Triginfo= lbt.GetTriggerLC(trigId);
    
    if(Triginfo==NULL)
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return -1;	
    }
    else
    {
    CleanupStack::Pop(Triginfo);
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return KErrNone;
    }
    //  return KErrNone; 
    }
    
       //Try to create Geo area from current cell when network not available.
    TInt CCellbasedtriggerOperations::TC_LBT_034_48_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
 	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    CLbtGeoAreaBase* Cellarea = NULL; 
    CLBTOperation *LBTOperation = notifier->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, &Cellarea, wait);
    wait->Start( );
    delete LBTOperation;
    TInt error=notifier->GetError();
    if(error==KErrNone)
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    	return -99;
    }
    else
    {
    		CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    	return KErrNone;	
    }
	

    }
    
    
       //Try to create Geo area object  for current cellular area when passing invalid TGeoAreaType argument
    TInt CCellbasedtriggerOperations::TC_LBT_034_49_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
 	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    CLbtGeoCell* Cellarea = NULL;
    CLbtGeoAreaBase* base = static_cast<CLbtGeoAreaBase*>(Cellarea);
   // TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6);  
    CLBTOperation *LBTOperation = notifier->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECircle, 
    																		 &base, 
    																		 wait);
   // wait->Start( );
    delete LBTOperation;
     
	TInt error=notifier->GetError();
    if(base!=NULL)//Need to verify this
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
//	delete base;
    	return -99;
    }
    else
    {
    		CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
//	delete base;
	//	delete Cellarea;
		//delete wait;
    	return KErrNone;	
    }
	

    }
    
    
       //Try to create Geo area object  for current cellular area when passing invalid TGeoAreaType argument
    TInt CCellbasedtriggerOperations::TC_LBT_034_50_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 //	TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 5);  //to be decided
 	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    CLbtGeoCircle* Circle = NULL;
    CLbtGeoAreaBase* base = static_cast<CLbtGeoAreaBase*>(Circle);
    CLBTOperation *LBTOperation = notifier->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, &base,wait);
    wait->Start( );
    delete LBTOperation;
     
	TInt error=notifier->GetError();
    if(error==KErrNone)
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	delete base;
		delete Circle;
    	return error;
    }
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	delete base;
	//delete wait;
	delete Circle;
    	return error;	
    }
	

    }
    
    
    //Updating attributes of a Cellular based startup trigger
    
      TInt CCellbasedtriggerOperations::TC_LBT_034_51_testL( CStifItemParser& /* aItem */ )
    {

 	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
     
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 //	  #if 0
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "LbtReference.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
   // CLbtGeoCell* Cellarea = NULL; 
    CLbtGeoAreaBase * base = NULL;//static_cast<CLbtGeoAreaBase*>(Cellarea);
    CLBTOperation *LBTOperation = notifier->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, 
    																	     &base,
    																	     wait);
    wait->Start( );
    delete LBTOperation;
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                base,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    trig->SetCondition(condition); // ownership transferred to object

    
    trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);      
    TLbtTriggerId trigId,trigId2;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TBuf<4> Networkcountrycode2 = _L("2000");
    TBuf<8> Networkidentitycode2 = _L("20000000");
    TUint Locationareacode2 = 200;
    TUint CellId2 = 200;
    
    CLbtGeoCell* Cellarea2  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode2,
   														Networkidentitycode2,
   														Locationareacode2,
   														CellId2
   														);
         
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                Cellarea2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
        
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
   // lbt.UpdateTriggerL(*newtrig);
    iLog->Log(_L("After UpdateTriggerL "));
    
    iLog->Log(_L("Before GetTriggerLC "));
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    //Check the name of the trigger
    TDesC Name2 =TrigEntry->Name(); 
    if(TrigEntry->Name()!=newtrig->Name()) 
    {
    	return -99;
    }
    //Check the state of the trigger
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=newtrig->State())
    {
    	return -99;
    }
    //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 != newtrig->Type())
    {
    	return -99;
    }
    //Check Condition of the trigger
    
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	
	CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
	AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId2,_L("Wrong Cellid"));     
    //}
    //Check trigger Id
    trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
    	return -99;
    }
    
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   // #endif
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
   // //delete wait;
   
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
    
     //Allocates and constructs a new Trigger Condition area object using Geo area for current cell 
TInt CCellbasedtriggerOperations::TC_LBT_034_52_testL( CStifItemParser& /* aItem */ )
    {

    RLbtServer lbtserver;
 	RLbt lbt;
 	iLog->Log(_L("Before Connect "));
 	User::LeaveIfError( lbtserver.Connect() );
 	iLog->Log(_L("After Connect "));
 	CleanupClosePushL( lbtserver );
 
 	User::LeaveIfError( lbt.Open( lbtserver ) );
 	CleanupClosePushL( lbt );
 	iLog->Log(_L("After RLbt::Open "));
 	
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
  
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    CLbtGeoCell* Cellarea = NULL; 
    CLbtGeoAreaBase * base = static_cast<CLbtGeoAreaBase*>(Cellarea);
    CLBTOperation *LBTOperation = notifier->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, 
    																	     &base,
    																	     wait);
    wait->Start( );
    delete LBTOperation;
    
    
    cond1->SetTriggerArea( base );
    
    CleanupStack::PopAndDestroy( 4 );
    
    return KErrNone;
    
    } 
    
    
     //Allocates and constructs a new CLbtTriggerFilterByArea object using CLbtGeoCurrentCell 1
    TInt CCellbasedtriggerOperations::TC_LBT_034_53_testL( CStifItemParser& /* aItem */ )
    {

	RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
     
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	             
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL();   
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    CLbtGeoCell* Cellarea = NULL; 
    CLbtGeoAreaBase * base = static_cast<CLbtGeoAreaBase*>(Cellarea);
    CLBTOperation *LBTOperation = notifier->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, 
    																	     &base,
    																	     wait);
    wait->Start( );
    delete LBTOperation;
    
    areafilter->SetArea(base);
    
  
    CLbtGeoAreaBase* geobase=areafilter->Area();
    CLbtGeoCell* cell2=NULL;
    if(geobase->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (geobase);
    }    
    CleanupStack::PopAndDestroy(3);
    

    return KErrNone;
     }
  #endif   
     
    
    //Listing cell based trigger using iterator - synchronous
    
     TInt CCellbasedtriggerOperations::TC_LBT_034_54_testL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
   // RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    /*User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );*/
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
   
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
      //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);         
     TCoordinate coordinate;    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    lbt.CreateGetTriggerIteratorL();
    
    RArray<CLbtTriggerInfo> triggers;
    //Get triggers. Note, the loop will block 
    //active schedular
    TInt i=1;
    CLbtTriggerInfo* trigInfo = lbt.GetNextTriggerLC(); 
    while ( trigInfo != NULL )
        {
        	i++;
          triggers.Append(*trigInfo);
         // CleanupStack::PopAndDestroy(trigInfo);
          trigInfo = lbt.GetNextTriggerLC(); 
        }
          
  //  CLbtTriggerEntry* entry=triggers[0].TriggerEntry();
      
    AssertTrueL( trigId==triggers[i-2].TriggerEntry()->Id(),_L("Wrong id"));    
        
    CleanupStack::PopAndDestroy(i);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
  //  CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggers.Close();
    //delete wait;    
    return KErrNone;

    }

//Test Asynchronous CreateTriggerIterator and GetNextTriggerLC for Cell based triggers
// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetNextTriggerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCellbasedtriggerOperations::TC_LBT_034_55_testL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
  //  RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
   /* User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );*/
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
   
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    
    
    trig->SetCondition(condition); // ownership transferred to object
	 //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
            
      TCoordinate coordinate;   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    notifier->CreateIterator(lbt,wait);
    wait->Start( );
    RArray<CLbtTriggerInfo> triggers;
    //Get triggers. Note, the loop will block 
    //active schedular
    TInt i=1;
    CLbtTriggerInfo* trigInfo = lbt.GetNextTriggerLC(); 
    while ( trigInfo != NULL )
        {
        	i++;
          triggers.Append(*trigInfo);
         
          trigInfo = lbt.GetNextTriggerLC(); 
        }
          
    CLbtTriggerEntry* entry=triggers[0].TriggerEntry();
      
    AssertTrueL( trigId==triggers[i-2].TriggerEntry()->Id(),_L("Wrong id"));    
        
     CleanupStack::PopAndDestroy(i);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
   // CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggers.Close();
    //delete wait;    
    return KErrNone;

    }
     
//Setting secondary attributes of a cell without setting primary attributes
TInt CCellbasedtriggerOperations::TC_LBT_034_56_testL( CStifItemParser& aItem )
    {

    RLbtServer server;
    RLbt lbt;
  //  RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
   /* User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );*/
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    //CleanupStack::PushL( cond1 ); 
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL();
    
    //Push to cleanup stack
   // CleanupStack::PushL( Cellarea );
    
     
    TUint Locationareacode = 100;
    Cellarea->SetLocationAreaCode(Locationareacode);
    
    TUint CellId = 100;
    Cellarea->SetCellId(CellId);
    
           
    cond1->SetTriggerArea(Cellarea);
    
    trig->SetCondition(cond1);
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    
    TLbtTriggerId trigId;
            
      TCoordinate coordinate;   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->GetError()==KErrArgument)  
    {
    
    CleanupStack::PopAndDestroy(notifier);
   // CleanupStack::PopAndDestroy( Cellarea ); 
   // CleanupStack::Pop( cond1 ); 
    CleanupStack::PopAndDestroy(trig);
   // CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    return KErrNone;	
    }
    return -99;

    }
#if 0
	//Firing of CGI+NMR based cellular session trigger with fire on creation False
	TInt CCellbasedtriggerOperations::TC_LBT_034_57_testL( CStifItemParser& /* aItem */ )
	{
		
	RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
        
    // set condition

 	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
 	CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
 	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier2 );
    CLbtGeoAreaBase* Cellarea = NULL;
    CLBTOperation *LBTOperation = notifier2->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, &Cellarea, wait2);
    wait2->Start( );
    delete LBTOperation;
    CLbtGeoCell* cell=NULL;
    if(Cellarea->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (Cellarea);
    }
    TUint cellid=cell->CellId();
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);                                              
        
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
           
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    iLog->Log(_L("Trigger Created"));    
    notifier->StartCidNotification(wait);
    iLog->Log(_L("Cell id change notification requested"));
    notifier2->StartNotification( wait2 );
  	wait2->Start( );
    iLog->Log(_L("Trigger Fired"));
    
    notifier->Cancel();
    TLbtTriggerFireInfo FireInfo;
    FireInfo = notifier2->GetFiredTrigger();
    CLbtTriggerInfo *Triginfo;
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo = lbt.GetTriggerLC(FireInfo.iTriggerId);
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    iLog->Log(_L("After GetTriggerLC "));
    //Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	iLog->Log(_L("Area received"));
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    	if(cell->CellId()==cellid)
		    {
		    iLog->Log(_L("Cell id matched "));
		    CleanupStack::PopAndDestroy( Triginfo );
		    CleanupStack::PopAndDestroy( notifier2 );
		    iLog->Log(_L("notifier2 popped out "));
		    CleanupStack::PopAndDestroy( notifier );
		    iLog->Log(_L("notifier popped out "));
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::Pop( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
			iLog->Log(_L("All popped out "));
		//	//delete wait;
		//	//delete wait2;
			return KErrNone;	
		    }
		    else
		    {
		    iLog->Log(_L("Cell id not matched "));
		    CleanupStack::PopAndDestroy( Triginfo );
		      CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::Pop( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait;
		//	//delete wait2;
			return -99;
		    }
    }
	
	//  return KErrNone; 
	}  
	
	//Firing of CGI+NMR based cellular session trigger with fire on creation True
	TInt CCellbasedtriggerOperations::TC_LBT_034_58_testL( CStifItemParser& /* aItem */ )
	{
		
		 RLbtServer lbtserver;
		 RLbt lbt,lbt2;
		 iLog->Log(_L("Before connecting"));
		 User::LeaveIfError( lbtserver.Connect() );
		 CleanupClosePushL( lbtserver );
		 iLog->Log(_L("Connection to RLbtServer Passed "));
		 User::LeaveIfError( lbt.Open(lbtserver));
		 User::LeaveIfError( lbt2.Open(lbtserver));
		 iLog->Log(_L("Subsession opened "));
		 CleanupClosePushL( lbt );
		 CleanupClosePushL( lbt2 );
	
	
	  //Construct a startup trigger
	CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
	
	//Push to cleanup stack
	CleanupStack::PushL( trig );
	iLog->Log(_L("Startup Trigger Entry Created "));
	
	// Set Name
	trig->SetNameL(_L("Trigger1"));
	
	
	// set condition
	
	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
	
	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
	CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2);
	CleanupStack::PushL( notifier2 );
	
	CLbtGeoAreaBase* Cellarea = NULL;
	CLBTOperation *LBTOperation = notifier2->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, &Cellarea, wait2);
	wait2->Start( );
	delete LBTOperation;
	CLbtGeoCell* cell=NULL;
	if(Cellarea->Type()==CLbtGeoAreaBase::ECellular)
	{
		cell= static_cast<CLbtGeoCell*> (Cellarea);
	}
	TUint cellid=cell->CellId();
	
	// ownership of Cellarea object transferred to the condition object
	CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
	                                            Cellarea,
	                                            CLbtTriggerConditionArea::EFireOnEnter);
	  //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);                                             
	    
	trig->SetCondition(condition); // ownership transferred to object
	
	TLbtTriggerId trigId;
	       
	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
	//wait->Start( );
	iLog->Log(_L("Trigger Created"));    
	
	notifier2->StartNotification( wait2 );
	wait2->Start( );
	
	iLog->Log(_L("Trigger Fired"));
	TLbtTriggerFireInfo FireInfo;
	FireInfo = notifier2->GetFiredTrigger();
	CLbtTriggerInfo *Triginfo;
	
	iLog->Log(_L("Before GetTriggerLC "));
	
	Triginfo = lbt.GetTriggerLC(FireInfo.iTriggerId);
	CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
	
	//Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	 
	if(area->Type()==CLbtGeoAreaBase::ECellular)
	{
		cell= static_cast<CLbtGeoCell*> (area);
		if(cell->CellId()==cellid)
		    {
		    CleanupStack::PopAndDestroy( Triginfo );
		    CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::PopAndDestroy( &lbt2 );
			CleanupStack::PopAndDestroy( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait2;
		//	//delete wait;
			iLog->Log(_L("All popped out "));
			return KErrNone;	
		    }
		    else
		    {
		    CleanupStack::PopAndDestroy( Triginfo );
		      CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::Pop( &lbt2 );
			CleanupStack::Pop( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait2;
		//	//delete wait;
			return -99;
		    }
	}
	
	//  return KErrNone; 
	}  

      //Firing of CGI+NMR based cellular startup trigger with fire on creation False
    TInt CCellbasedtriggerOperations::TC_LBT_034_59_testL( CStifItemParser& /* aItem */ )
    {
   	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
     iLog->Log(_L("Before creating notifiers "));   
    // set condition

 	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
 	CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
 	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier2 );
    iLog->Log(_L("After creating notifiers "));   
    CLbtGeoAreaBase* Cellarea = NULL;
    CLBTOperation *LBTOperation = notifier2->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, &Cellarea, wait2);
    wait2->Start( );
    iLog->Log(_L("After Cell area object create "));   
    delete LBTOperation;
    CLbtGeoCell* cell=NULL;
    if(Cellarea->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (Cellarea);
    }
    TUint cellid=cell->CellId();
    iLog->Log(_L("before condition creation"));  
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
                                                
        
    iLog->Log(_L("After condition creation"));  
    trig->SetCondition(condition); // ownership transferred to object
	
	 //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
     iLog->Log(_L("before CreateTriggers"));          
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    iLog->Log(_L("Trigger Created"));    
    notifier->StartCidNotification(wait);
    iLog->Log(_L("Cell id change notification requested"));
    notifier2->StartNotification( wait2 );
  	wait2->Start( );
    iLog->Log(_L("Trigger Fired"));
    
    notifier->Cancel();
    TLbtTriggerFireInfo FireInfo;
    FireInfo = notifier2->GetFiredTrigger();
    CLbtTriggerInfo *Triginfo;
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo = lbt.GetTriggerLC(FireInfo.iTriggerId);
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    iLog->Log(_L("After GetTriggerLC "));
    //Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	iLog->Log(_L("Area received"));
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    	if(cell->CellId()==cellid)
		    {
		    iLog->Log(_L("Cell id matched "));
		    CleanupStack::PopAndDestroy( Triginfo );
		    CleanupStack::PopAndDestroy( notifier2 );
		    iLog->Log(_L("notifier2 popped out "));
		    CleanupStack::PopAndDestroy( notifier );
		    iLog->Log(_L("notifier popped out "));
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::Pop( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
			iLog->Log(_L("All popped out "));
		//	//delete wait;
		//	//delete wait2;
			return KErrNone;	
		    }
		    else
		    {
		    iLog->Log(_L("Cell id not matched "));
		    CleanupStack::PopAndDestroy( Triginfo );
		      CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::Pop( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait;
		//	//delete wait2;
			return -99;
		    }
    }
    }  
    
    //Firing of CGI+NMR based cellular startup trigger with fire on creation True
    TInt CCellbasedtriggerOperations::TC_LBT_034_60_testL( CStifItemParser& /* aItem */ )
    {
   	
 	
 	
 		 RLbtServer lbtserver;
		 RLbt lbt,lbt2;
		 iLog->Log(_L("Before connecting"));
		 User::LeaveIfError( lbtserver.Connect() );
		 CleanupClosePushL( lbtserver );
		 iLog->Log(_L("Connection to RLbtServer Passed "));
		 User::LeaveIfError( lbt.Open(lbtserver));
		 User::LeaveIfError( lbt2.Open(lbtserver));
		 iLog->Log(_L("Subsession opened "));
		 CleanupClosePushL( lbt );
		 CleanupClosePushL( lbt2 );
	
	
	   //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
	
	// set condition
	
	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
	
	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
	CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2);
	CleanupStack::PushL( notifier2 );
	
	CLbtGeoAreaBase* Cellarea = NULL;
	CLBTOperation *LBTOperation = notifier2->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, &Cellarea, wait2);
	wait2->Start( );
	delete LBTOperation;
	CLbtGeoCell* cell=NULL;
	if(Cellarea->Type()==CLbtGeoAreaBase::ECellular)
	{
		cell= static_cast<CLbtGeoCell*> (Cellarea);
	}
	TUint cellid=cell->CellId();
	
	// ownership of Cellarea object transferred to the condition object
	CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
	                                            Cellarea,
	                                            CLbtTriggerConditionArea::EFireOnEnter);
	    
	trig->SetCondition(condition); // ownership transferred to object
	 //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
	
	TLbtTriggerId trigId;
	       
	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
	//wait->Start( );
	iLog->Log(_L("Trigger Created"));    
	
	notifier2->StartNotification( wait2 );
	wait2->Start( );
	
	iLog->Log(_L("Trigger Fired"));
	TLbtTriggerFireInfo FireInfo;
	FireInfo = notifier2->GetFiredTrigger();
	CLbtTriggerInfo *Triginfo;
	
	iLog->Log(_L("Before GetTriggerLC "));
	
	Triginfo = lbt.GetTriggerLC(FireInfo.iTriggerId);
	CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
	
	//Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	 
	if(area->Type()==CLbtGeoAreaBase::ECellular)
	{
		cell= static_cast<CLbtGeoCell*> (area);
		if(cell->CellId()==cellid)
		    {
		    CleanupStack::PopAndDestroy( Triginfo );
		    CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::PopAndDestroy( &lbt2 );
			CleanupStack::PopAndDestroy( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait2;
		//	//delete wait;
			iLog->Log(_L("All popped out "));
			return KErrNone;	
		    }
		    else
		    {
		    CleanupStack::PopAndDestroy( Triginfo );
		      CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::Pop( &lbt2 );
			CleanupStack::Pop( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait2;
		//	//delete wait;
			return -99;
		    }
	}
	
    }  
    #endif
    
    //Firing of CGI based Cellular Startup trigger
    TInt CCellbasedtriggerOperations::TC_LBT_034_61_testL( CStifItemParser& /* aItem */ )
    {
   	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
 	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier2 );
	
  
    RMobilePhone::TMobilePhoneNetworkMode networkMode;
    CTelephony::TNetworkInfoV1 nwInfo;
    GetCurrentCellInfoL( networkMode,nwInfo );
    
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL( networkMode,
                                                nwInfo.iCountryCode,
                                                nwInfo.iNetworkId,
                                                nwInfo.iLocationAreaCode,
                                                nwInfo.iCellId );
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object
	 //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
           
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    iLog->Log(_L("Trigger created"));
    notifier2->StartNotification( wait2 );
    wait2->Start( );
    iLog->Log(_L("Trigger Fired"));
    
    notifier->Cancel();
    TLbtTriggerFireInfo FireInfo;
    FireInfo = notifier2->GetFiredTrigger();
    
    CLbtTriggerInfo *Triginfo;
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo = lbt.GetTriggerLC(FireInfo.iTriggerId);
    //Triginfo = lbt.GetTriggerLC(trigId);
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    //Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    CLbtGeoCell* cell=NULL;
    	cell= static_cast<CLbtGeoCell*> (area);
    	if(cell->CellId()==nwInfo.iCellId)
    	    {
    	    CleanupStack::PopAndDestroy( Triginfo );
		    CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::Pop( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait;
		//	//delete wait2;
    		return KErrNone;	
    	    }
    	    else
    	    {
    	    CleanupStack::PopAndDestroy( Triginfo );
		      CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::Pop( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait;
		//	//delete wait2;
    		return -99;
    	    }
    }
    
    
    return KErrNone;
    
    }
    
    //Firing of CGI based Cellular Session trigger
    TInt CCellbasedtriggerOperations::TC_LBT_034_62_testL( CStifItemParser& /* aItem */ )
    {
        
        RLbtServer lbtserver;
        RLbt lbt;
        iLog->Log(_L("Before connecting"));
        User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
        User::LeaveIfError( lbt.Open(lbtserver));
        iLog->Log(_L("Subsession opened "));
        CleanupClosePushL( lbt );
    
    
         //Construct a startup trigger
       CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
       
       //Push to cleanup stack
       CleanupStack::PushL( trig );
       iLog->Log(_L("Session Trigger Entry Created "));
       
       // Set Name
       trig->SetNameL(_L("Trigger1"));
       // set condition
       
       CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
       CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
       CleanupStack::PushL( notifier );
       CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt);
       CleanupStack::PushL( notifier2 );
       
     
       RMobilePhone::TMobilePhoneNetworkMode networkMode;
       CTelephony::TNetworkInfoV1 nwInfo;
       GetCurrentCellInfoL( networkMode,nwInfo );
       
       CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL( networkMode,
                                                   nwInfo.iCountryCode,
                                                   nwInfo.iNetworkId,
                                                   nwInfo.iLocationAreaCode,
                                                   nwInfo.iCellId );
       
       
       // ownership of Cellarea object transferred to the condition object
       CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                   Cellarea,
                                                   CLbtTriggerConditionArea::EFireOnEnter);
           
       trig->SetCondition(condition); // ownership transferred to object
        //set Requestor     
       CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
       CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
       TBuf<KLbtMaxNameLength> ReqData=_L("");
       trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
       TLbtTriggerId trigId;
              
       notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
       wait->Start( );

       notifier2->StartNotification( wait2 );
       wait2->Start( );
       iLog->Log(_L("Trigger Fired"));
       
       notifier->Cancel();
       TLbtTriggerFireInfo FireInfo;
       FireInfo = notifier2->GetFiredTrigger();
       
       CLbtTriggerInfo *Triginfo;
       iLog->Log(_L("Before GetTriggerLC "));
       
       Triginfo = lbt.GetTriggerLC(FireInfo.iTriggerId);
       //Triginfo = lbt.GetTriggerLC(trigId);
       CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
       
       //Check Condition of the trigger
       CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
       
       CLbtGeoAreaBase* area = Condition2->TriggerArea();
       
       if(area->Type()==CLbtGeoAreaBase::ECellular)
       {
       CLbtGeoCell* cell=NULL;
           cell= static_cast<CLbtGeoCell*> (area);
           if(cell->CellId()==nwInfo.iCellId)
               {
               CleanupStack::PopAndDestroy( Triginfo );
               CleanupStack::PopAndDestroy( notifier2 );
               CleanupStack::PopAndDestroy( notifier );
               CleanupStack::PopAndDestroy( trig );
               CleanupStack::Pop( &lbt );
               CleanupStack::PopAndDestroy( &lbtserver );
           //  //delete wait;
           //  //delete wait2;
               return KErrNone;    
               }
               else
               {
               CleanupStack::PopAndDestroy( Triginfo );
                 CleanupStack::PopAndDestroy( notifier2 );
               CleanupStack::PopAndDestroy( notifier );
               CleanupStack::PopAndDestroy( trig );
               CleanupStack::Pop( &lbt );
               CleanupStack::PopAndDestroy( &lbtserver );
           //  //delete wait;
           //  //delete wait2;
               return -99;
               }
       }
       
       
       return KErrNone;
       
       }
    
    //Test case to check hysteresis condition for cell based triggers
    TInt CCellbasedtriggerOperations::TC_LBT_034_65_testL( CStifItemParser& /* aItem */ )
    	{
    		
     	
 	RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
        
    // set condition

 	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
 	CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
 	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier2 );
    
    RFileLogger aLog;
    CLbtAppCgiRequestor* CGIRequest=CLbtAppCgiRequestor::NewL(aLog);
    CleanupStack::PushL( CGIRequest );  
    CGIRequest->Start(wait);
    wait->Start( );
 		CTelephony::TNetworkInfoV1Pckg currcgi=CGIRequest->CurrentCGI();
     
 
 		CLbtGeoCell* Cellarea2=NULL;
    CTelephony::TNetworkMode Networkmode = currcgi().iMode;
     		   											
 		if(Networkmode ==CTelephony::ENetworkModeGsm)
 		{
		 Cellarea2= CLbtGeoCell::NewL(           RMobilePhone :: ENetworkModeGsm,
	 											 currcgi().iCountryCode,
												 currcgi().iNetworkId,
												 currcgi().iLocationAreaCode,
												 currcgi().iCellId
													);	
		}
    TUint cellid=currcgi().iCellId;
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object
	
	 //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
           
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    iLog->Log(_L("Trigger Created"));    
    notifier->StartCidNotification(wait);
    iLog->Log(_L("Cell id change notification requested"));
    notifier2->StartNotification( wait2 );
  	wait2->Start( );
    iLog->Log(_L("Trigger Fired"));
    
    notifier->Cancel();
    TLbtTriggerFireInfo FireInfo;
    FireInfo = notifier2->GetFiredTrigger();
    CLbtTriggerInfo *Triginfo;
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo = lbt.GetTriggerLC(FireInfo.iTriggerId);
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    iLog->Log(_L("After GetTriggerLC "));
    //Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	iLog->Log(_L("Area received"));
	CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    	if(cell->CellId()==cellid)
		    {
		    iLog->Log(_L("Cell id matched "));
		    CleanupStack::PopAndDestroy( Triginfo );
			CleanupStack::PopAndDestroy( CGIRequest );
		    CleanupStack::PopAndDestroy( notifier2 );
		    iLog->Log(_L("notifier2 popped out "));
		    CleanupStack::PopAndDestroy( notifier );
		    iLog->Log(_L("notifier popped out "));
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::Pop( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
			iLog->Log(_L("All popped out "));
		//	//delete wait;
		//	//delete wait2;
			return KErrNone;	
		    }
		    else
		    {
		    iLog->Log(_L("Cell id not matched "));
		    CleanupStack::PopAndDestroy( Triginfo );
			CleanupStack::PopAndDestroy( CGIRequest );
		      CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
			CleanupStack::PopAndDestroy( trig );
			CleanupStack::Pop( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait;
		//	//delete wait2;
			return -99;
		    }
    }
		return KErrNone;
    	}  
    
    
    //Testing the behaviour of cell based triggers when SIM card is not available/No Network
    TInt CCellbasedtriggerOperations::TC_LBT_034_66_testL( CStifItemParser& /* aItem */ )
        {
       	
      RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 	TRAP_IGNORE(lbt.DeleteTriggersL());
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
 	CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
 	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier2 );
	
	TBuf<4> Networkcountrycode = _L("404");
    TBuf<8> Networkidentitycode = _L("45");
    TUint Locationareacode = 1627;
    TUint CellId = 11681;
   
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object
	 //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
           
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    iLog->Log(_L("Trigger Created"));    
    
    
    	   
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
//	//delete wait;
//	//delete wait2;
	return KErrNone;	
   }
    //Testing the behaviour of cell based triggers when SIM card replaced with another operator SIM
    
    TInt CCellbasedtriggerOperations::TC_LBT_034_67_testL( CStifItemParser& /* aItem */ )
        {
       	
       RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 
 	lbt.DeleteTriggersL();
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
 	CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
 	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier2 );
	
	TBuf<4> Networkcountrycode = _L("404");
    TBuf<8> Networkidentitycode = _L("45");
    TUint Locationareacode = 1627;
    TUint CellId = 11681;
   
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object
	
	 //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
           
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    iLog->Log(_L("Trigger Created"));    
    
    
    	   
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
//	//delete wait;
//	//delete wait2;
	return KErrNone;	
        }
        
        	
    //Testing of simultaneous firing of cellular based and Coordinate based triggers
    TInt CCellbasedtriggerOperations::TC_LBT_034_68_testL( CStifItemParser& /* aItem */ )
        {
       	
     		 RLbtServer lbtserver;
		 RLbt lbt,lbt2;
		 iLog->Log(_L("Before connecting"));
		 User::LeaveIfError( lbtserver.Connect() );
		 CleanupClosePushL( lbtserver );
		 iLog->Log(_L("Connection to RLbtServer Passed "));
		 User::LeaveIfError( lbt.Open(lbtserver));
		 User::LeaveIfError( lbt2.Open(lbtserver));
		 iLog->Log(_L("Subsession opened "));
		 CleanupClosePushL( lbt );
		 CleanupClosePushL( lbt2 );
	
	
	  //Construct a startup trigger
/*	CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
	
	//Push to cleanup stack
	CleanupStack::PushL( trig );
	iLog->Log(_L("Startup Trigger Entry Created "));
	
	// Set Name
	trig->SetNameL(_L("Trigger1"));
	
	_LIT( KMyTriggerHandlingProcessName, "Calendar.exe");
   //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);*/
	// set condition
	
	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
	
	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
	CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2);
	CleanupStack::PushL( notifier2 );
	
		 TCoordinate coordinate(12.9422763,77.6950090);
	//TCoordinate coordinate(62.5285,23.9385);
    
   	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,300);
    	CleanupStack::PushL( circle );
    
         
    	// ownership of circle object transferred to the condition object
    	CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    	CleanupStack::Pop( circle );
    	//New code start
    	    TSecureId secureid= 0x1333;
        CLbtTriggerEntry::TLbtTriggerState aState =CLbtTriggerEntry::EStateEnabled; 

    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "calendar.exe" );
    TUid aManagerUi;
    TBuf<256> triggerId(_L("test"));   
	aManagerUi = TUid::Uid(0x87654321);
    
    	CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        aState,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
       // CLbtTriggerEntry::EPostFiringActionNoAction, //Trigger fired option
        aManagerUi,				                      //Manager UI
        condition,                                       //Trigger condition
        KMyTriggerHandlingProcessName,              //Handling process name
       // handlingUidType,                            //Handling UID type
       secureid,
        triggerId );   
       
       CleanupStack::PushL(trig) ;
        //New code end      
    
    /*	trig->SetCondition(condition); // ownership transferred to object*/

//	#if 0
	TLbtTriggerId trigId;
	       
	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
	wait->Start( );
	
	
	iLog->Log(_L("Trigger Created"));    
	
//	notifier2->StartNotification( wait2 );
//	wait2->Start( );
//	notifier2->After(50000000);
//	wait2->Start( );
 /* 	iLog->Log(_L("Trigger Fired"));
	
    	TLbtTriggerFireInfo FireInfo;
    	TReal32 trigDistance;
    	TPosition firePosition;
    	FireInfo = notifier2->GetFiredTrigger();
    	FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    	firePosition.Distance(coordinate,trigDistance);
   
    	if( trigDistance<=300)
    	{
    	_LIT(KProcessToSearch, "About"); 
    //_LIT(KProcessToSearch, "TestServerStarter"); 
	TFullName fullName;
	TInt err = KErrNone;
	TFindProcess findprocess;
	TInt numofprocesses=0;
	while (err=findprocess.Next(fullName), err==KErrNone)
	{
	if(err=fullName.FindF(KProcessToSearch), err!=KErrNotFound)
	{
	iLog->Log(_L("Trig handling process started"));
	numofprocesses++;
	}
	}
   
   
	   		CleanupStack::PopAndDestroy( trig );
		    CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
		//	CleanupStack::PopAndDestroy( trig );
			CleanupStack::PopAndDestroy( &lbt2 );
			CleanupStack::PopAndDestroy( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait2;
		//	//delete wait;
			iLog->Log(_L("All popped out "));
	   iLog->Log(_L("Test case passed "));
	   if(numofprocesses>0)
	       return KErrNone; 
	   else
	   return -99;
    	}
    else
    {*/
 //    #endif
		   	CleanupStack::PopAndDestroy( trig );
		    CleanupStack::PopAndDestroy( notifier2 );
		    CleanupStack::PopAndDestroy( notifier );
		//	CleanupStack::PopAndDestroy( trig );
			CleanupStack::PopAndDestroy( &lbt2 );
			CleanupStack::PopAndDestroy( &lbt );
			CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait2;
		//	//delete wait;
			iLog->Log(_L("All popped out "));
	    
//	    return -25; 
   // }
          return KErrNone; 
        }  
        
        
        
   //Firing of CGI based Cellular Session trigger
    TInt CCellbasedtriggerOperations::StartCellChangeNotifier_testL( CStifItemParser& /* aItem */ )
    {
   	RLbtServer lbtserver;
    RLbt lbt;
	 iLog->Log(_L("Before connecting"));
	 User::LeaveIfError( lbtserver.Connect() );
	 CleanupClosePushL( lbtserver );
	 iLog->Log(_L("Connection to RLbtServer Passed "));
	 User::LeaveIfError( lbt.Open(lbtserver));
	 CleanupClosePushL( lbt );
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
 	
 	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
    
    
    
    notifier->StartCidNotification(wait);
    iLog->Log(_L("Cell id change notification requested"));
    wait->Start();
    CleanupStack::PopAndDestroy( notifier );
		
	CleanupStack::PopAndDestroy( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone;
    }  
    
    
     //Updating Cellular based startup trigger to a coordinate based startup trigger.
    
    TInt CCellbasedtriggerOperations::TC_LBT_034_69_testL( CStifItemParser& /* aItem */ )
    {

 	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
     
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 //	  #if 0
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "LbtReference.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
   TBuf<4> Networkcountrycode = _L("404");
    TBuf<8> Networkidentitycode = _L("45");
    TUint Locationareacode = 1627;
    TUint CellId = 11681;
   
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    trig->SetCondition(condition); // ownership transferred to object

    
    trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);      
    TLbtTriggerId trigId,trigId2;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    newtrig->SetCommandLineL(_L("Commandline2" ));
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,500);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
   // lbt.UpdateTriggerL(*newtrig);
    iLog->Log(_L("After UpdateTriggerL "));
    
    iLog->Log(_L("Before GetTriggerLC "));
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    //Check the name of the trigger
    TDesC Name2 =TrigEntry->Name(); 
    if(TrigEntry->Name()!=newtrig->Name()) 
    {
    	iLog->Log(_L("wrong name "));
    	return -99;
    }
    //Check the state of the trigger
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=newtrig->State())
    {
    	iLog->Log(_L("wrong state "));
    	return -99;
    }
    //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 != newtrig->Type())
    {
    	iLog->Log(_L("wrong type "));
    	return -99;
    }
    //Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	CLbtTriggerConditionArea::TDirection direction;
	direction = Condition2->Direction();
	if(direction!=CLbtTriggerConditionArea::EFireOnEnter)
	{
		iLog->Log(_L("wrong Direction "));
		return -99;
	}
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	if(area ->Type()!=CLbtGeoAreaBase::ECircle )
	{
	CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
	iLog->Log(_L("wrong area type "));
		return -99;
	}
	
	CLbtGeoCircle* circle2 = static_cast <CLbtGeoCircle*>(area);
	TCoordinate center = newcircle->Center();
	TCoordinate center2 = circle2->Center();
	TReal32 distance;
	center2.Distance(center,distance);
	if(distance!=0)
	{
		iLog->Log(_L("wrong center "));
		return -99;
	}
	TReal radius = circle2->Radius();
	if(radius != newcircle->Radius())
	{
		iLog->Log(_L("wrong radius "));
		return -99;
	}
	
	

    //Check trigger Id
    trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
    	iLog->Log(_L("wrong trigger id "));
    	return -99;
    }
    
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
   
   
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
    
     //Updating coordinate based to a Cellular based startup trigger
    
      TInt CCellbasedtriggerOperations::TC_LBT_034_70_testL( CStifItemParser& /* aItem */ )
    {

 	
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
     
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 	TBuf<4> Networkcountrycode = _L("404");
    TBuf<8> Networkidentitycode = _L("45");
  //  TUint Locationareacode = 1627;
  //  TUint CellId = 40331;
 
 //	  #if 0
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "LbtReference.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
  //  TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    
    TUid aManagerUi = TUid::Uid(0x87654321);   
    trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);      
    TLbtTriggerId trigId,trigId2;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    // CLbtGeoCell* Cellarea = NULL; 
   /* CLbtGeoAreaBase * base = NULL;//static_cast<CLbtGeoAreaBase*>(Cellarea);
    CLBTOperation *LBTOperation = notifier->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, 
    																	     &base,
    																	     wait);
    wait->Start( );
    delete LBTOperation;
    
      
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                base,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    newtrig->SetCondition(newcondition); // ownership transferred to object
*/

TBuf<4> Networkcountrycode2 = _L("2000");
    TBuf<8> Networkidentitycode2 = _L("20000000");
    TUint Locationareacode2 = 200;
    TUint CellId2 = 200;
    
    CLbtGeoCell* Cellarea2  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode2,
   														Networkidentitycode2,
   														Locationareacode2,
   														CellId2
   														);
         
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                Cellarea2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
        
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
   // lbt.UpdateTriggerL(*newtrig);
    iLog->Log(_L("After UpdateTriggerL "));
    
    iLog->Log(_L("Before GetTriggerLC "));
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    //Check the name of the trigger
    TDesC Name2 =TrigEntry->Name(); 
    if(TrigEntry->Name()!=newtrig->Name()) 
    {
    	return -99;
    }
    iLog->Log(_L("Name OK "));
    //Check the state of the trigger
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=newtrig->State())
    {
    	return -99;
    }
     iLog->Log(_L("State OK "));
    //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 != newtrig->Type())
    {
    	return -99;
    }
    
    //Check Condition of the trigger
    
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
 	iLog->Log(_L("condition OK "));
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	 iLog->Log(_L("area OK "));
	CLbtGeoCell* cell=NULL;
    if(area->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell= static_cast<CLbtGeoCell*> (area);
    }
		AssertTrueL(cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
	AssertTrueL(cell->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
	AssertTrueL(cell->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
	AssertTrueL(cell->CellId()==CellId2,_L("Wrong Cellid"));    
    //}
    //Check trigger Id
    trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
    	return -99;
    }
    
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   // #endif
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
   // //delete wait;
   
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
       
  //  #endif
    
// -----------------------------------------------------------------------------
// CCellbasedtriggerOperations::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CCellbasedtriggerOperations::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
