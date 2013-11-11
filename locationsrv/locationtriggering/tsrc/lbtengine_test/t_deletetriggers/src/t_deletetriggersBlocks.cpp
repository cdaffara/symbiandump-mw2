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
* Description:    Test module to implement Delete trigger related test cases
*
*/





// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "t_deletetriggers.h"
#include "t_triggerfireobserver.h"
#include "t_deletetriggerobserver.h"

#include <lbtcommon.h>
#include <lbterrors.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
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
// Ct_deletetriggers::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ct_deletetriggers::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", Ct_deletetriggers::ExampleL ),
        ENTRY( "TC_LBT013_01", Ct_deletetriggers::TC_LBT013_01_testL ),
        ENTRY( "TC_LBT013_02", Ct_deletetriggers::TC_LBT013_02_testL ),
        ENTRY( "TC_LBT013_03", Ct_deletetriggers::TC_LBT013_03_testL ),
        ENTRY( "TC_LBT013_04", Ct_deletetriggers::TC_LBT013_04_testL ),
        ENTRY( "TC_LBT013_05", Ct_deletetriggers::TC_LBT013_05_testL ),
        ENTRY( "TC_LBT013_06", Ct_deletetriggers::TC_LBT013_06_testL ),
        ENTRY( "TC_LBT013_07", Ct_deletetriggers::TC_LBT013_07_testL ),
        ENTRY( "TC_LBT013_08", Ct_deletetriggers::TC_LBT013_08_testL ),
        ENTRY( "TC_LBT013_09", Ct_deletetriggers::TC_LBT013_09_testL ),
        ENTRY( "TC_LBT013_10", Ct_deletetriggers::TC_LBT013_10_testL ),
        ENTRY( "TC_LBT013_11", Ct_deletetriggers::TC_LBT013_11_testL ),
        ENTRY( "TC_LBT013_12", Ct_deletetriggers::TC_LBT013_12_testL ),
        ENTRY( "TC_LBT013_13", Ct_deletetriggers::TC_LBT013_13_testL ),
        ENTRY( "TC_LBT013_14", Ct_deletetriggers::TC_LBT013_14_testL ),
        ENTRY( "TC_LBT013_15", Ct_deletetriggers::TC_LBT013_15_testL ),
        ENTRY( "TC_LBT013_16", Ct_deletetriggers::TC_LBT013_16_testL ),
        ENTRY( "TC_LBT013_17", Ct_deletetriggers::TC_LBT013_17_testL ),
        ENTRY( "TC_LBT013_18", Ct_deletetriggers::TC_LBT013_18_testL ),
        ENTRY( "TC_LBT013_19", Ct_deletetriggers::TC_LBT013_19_testL ),
        ENTRY( "TC_LBT013_20", Ct_deletetriggers::TC_LBT013_20_testL ),
        ENTRY( "TC_LBT013_21", Ct_deletetriggers::TC_LBT013_21_testL ),
        ENTRY( "TC_LBT013_22", Ct_deletetriggers::TC_LBT013_22_testL ),
        ENTRY( "TC_LBT013_23", Ct_deletetriggers::TC_LBT013_23_testL ),
        ENTRY( "TC_LBT013_24", Ct_deletetriggers::TC_LBT013_24_testL ),
        ENTRY( "TC_LBT013_25", Ct_deletetriggers::TC_LBT013_25_testL ),
        ENTRY( "TC_LBT013_26", Ct_deletetriggers::TC_LBT013_26_testL ),
        ENTRY( "TC_LBT013_27", Ct_deletetriggers::TC_LBT013_27_testL ),
        ENTRY( "TC_LBT013_28", Ct_deletetriggers::TC_LBT013_28_testL ),
        ENTRY( "TC_LBT013_29", Ct_deletetriggers::TC_LBT013_29_testL ),
        ENTRY( "TC_LBT013_30", Ct_deletetriggers::TC_LBT013_30_testL ),
        ENTRY( "TC_LBT013_31", Ct_deletetriggers::TC_LBT013_31_testL ),
        ENTRY( "TC_LBT013_32", Ct_deletetriggers::TC_LBT013_32_testL ),
        ENTRY( "TC_LBT013_33", Ct_deletetriggers::TC_LBT013_33_testL ),
        ENTRY( "TC_LBT013_34", Ct_deletetriggers::TC_LBT013_34_testL ),
        ENTRY( "TC_LBT013_35", Ct_deletetriggers::TC_LBT013_35_testL ),
        ENTRY( "TC_LBT013_36", Ct_deletetriggers::TC_LBT013_36_testL ),
        ENTRY( "TC_LBT013_37", Ct_deletetriggers::TC_LBT013_37_testL ),
        ENTRY( "TC_LBT013_38", Ct_deletetriggers::TC_LBT013_38_testL ),
        ENTRY( "TC_LBT013_39", Ct_deletetriggers::TC_LBT013_39_testL ),

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Kt_deletetriggers, "t_deletetriggers" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Kt_deletetriggers, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Kt_deletetriggers, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_01L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_01_testL( CStifItemParser& /* aItem */ )
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
    trig->SetState( CLbtTriggerEntry::EStateDisabled );
    
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
       
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig->SetCondition(condition); // ownership transferred to object 
     
   
   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId;
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    lbt.DeleteTriggerL(trigId);
    
//    CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(error,lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(); );
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(error == KErrNotFound)
    {
   
    delete wait; 
    return error;
  
    }
    else
    {
    
    delete wait; 
    return error;
  
    }
        
    
    
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_02L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_02_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	  	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
        
     // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState( CLbtTriggerEntry::EStateDisabled );
    
    trig->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig->SetCondition(condition); // ownership transferred to object 
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId;
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    lbt.DeleteTriggerL(trigId);
    
//    CLbtTriggerInfo* trigEntry=NULL;
    CLbtTriggerInfo* trigEntry =lbt.GetTriggerLC(trigId);
    
        
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_03L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_03_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
       
    TLbtTriggerId trigId=0;
        
    TRAPD(err,lbt.DeleteTriggerL(trigId));
           
    AssertTrueL( KErrNotFound==err,_L("wrong error code"));
    
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
     
    return KErrNone;
    
    }


// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_04L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_04_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
       
    TLbtTriggerId trigId=0x0;
        
    TRAPD(err,lbt.DeleteTriggerL(trigId));
           
    AssertTrueL( KErrNotFound==err,_L("wrong error code"));
    
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
     
    return KErrNone;
    
    }


// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_05L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_05_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate;
 	 	 
       
    TLbtTriggerId trigId=0x5;
        
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtServerBadHandle );
    lbt.DeleteTriggerL(trigId);
         
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_06L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_06_testL( CStifItemParser& /* aItem */ )
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
    trig1->SetManagerUi(TUid::Uid(4));
    
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);      
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object 
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    
     //Construct a session trigger
    CLbtSessionTrigger* trig2 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 );
        
    trig2->SetNameL(_L("Trigger2"));
    trig2->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetManagerUi(TUid::Uid(5));
    //Set Requestor
	
	trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig2->SetCondition(condition2); // ownership transferred to object 
    
    
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    lbt.DeleteTriggersL(filter);
    
    CLbtTriggerInfo* trigEntry=NULL;
            
    trigEntry=lbt.GetTriggerLC(trigId2);
    
    trigEntry=lbt.GetTriggerLC(trigId1);
    
    CleanupStack::PopAndDestroy( trigEntry );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( trig2 );   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_07L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_07_testL( CStifItemParser& /* aItem */ )
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
    trig1->SetManagerUi(TUid::Uid(4));
    
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object 
    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    
     //Construct a session trigger
    CLbtSessionTrigger* trig2 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 );
        
    trig2->SetNameL(_L("Trigger2"));
    trig2->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetManagerUi(TUid::Uid(5));
    
    //Set Requestor

	trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
                  
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig2->SetCondition(condition2); // ownership transferred to object 
    
    
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    lbt.DeleteTriggersL(filter);
    
    CLbtTriggerInfo* trigEntry=NULL;
                
    trigEntry=lbt.GetTriggerLC(trigId2);
    
    trigEntry=lbt.GetTriggerLC(trigId1);
    
    CleanupStack::PopAndDestroy( trigEntry );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( trig2 );   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_08L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_08_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	  	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetManagerUi(TUid::Uid(4));
    
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object
    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    
     //Construct a session trigger
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 );
        
    trig2->SetNameL(_L("Trigger2"));
    trig2->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetManagerUi(TUid::Uid(5));
    
    
    trig2->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    //Set Requestor
	
	trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
       
          
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig2->SetCondition(condition2); // ownership transferred to object
    
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
    filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    lbt.DeleteTriggersL(filter);
    
    CLbtTriggerInfo* trigEntry=NULL;
        
    trigEntry=lbt.GetTriggerLC(trigId2);
    trigEntry=lbt.GetTriggerLC(trigId1);
     
    CleanupStack::PopAndDestroy( trigEntry );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( trig2 );   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_09L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_09_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	  	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetManagerUi(TUid::Uid(4));
    
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object
    
    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    
     //Construct a session trigger
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 );
        
    trig2->SetNameL(_L("Trigger2"));
    trig2->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetManagerUi(TUid::Uid(5));
    
    
    trig2->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    //Set Requestor

	trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
       
          
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig2->SetCondition(condition2); // ownership transferred to object
    
    
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    lbt.DeleteTriggersL(filter);
    
    CLbtTriggerInfo* trigEntry=NULL;
           
    trigEntry=lbt.GetTriggerLC(trigId2);
    
    trigEntry=lbt.GetTriggerLC(trigId1);
    
    CleanupStack::PopAndDestroy( trigEntry );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( trig2 );   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_10L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_10_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
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
     
    
    CLbtGeoCircle* area=CLbtGeoCircle::NewL();
    area->SetRadius(100);
    area->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate);    
    CLbtTriggerFilterByArea* filter=CLbtTriggerFilterByArea::NewL(area2); 
    CleanupStack::PushL( filter );    
    
   
    lbt.DeleteTriggersL(filter);
    
//    CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(error,lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(););
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrNotFound)
    {
    
  
    delete wait; 
    return error;	
    }
    else
    {
    
    delete wait; 
    return -99;	
    }  
        
    
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_11L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_11_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
        
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
          
    CLbtGeoCircle* area=CLbtGeoCircle::NewL();
    area->SetRadius(100);
    area->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate);    
    CLbtTriggerFilterByArea* filter=CLbtTriggerFilterByArea::NewL(area2); 
    CleanupStack::PushL( filter );    
    
   
    lbt.DeleteTriggersL(filter);
    
//    CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(error,lbt.GetTriggerLC(trigId1);
  	CleanupStack::PopAndDestroy( ););
  	CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrNotFound)
    {
    
    delete wait; 
    return error;	
    }
    else
    {
    
    delete wait; 
    return -99;	
    }  
    
    }


// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_36L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_36_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
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
    trig1->SetManagerUi(TUid::Uid(4));
         
    
    CLbtGeoCircle* area=CLbtGeoCircle::NewL();
    area->SetRadius(100);
    area->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate);        
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(area2); 
       
   
    CLbtTriggerFilterByAttribute* attfilter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( attfilter );    
    
    attfilter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    attfilter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    attfilter->AddTriggerManagerUiL(TUid::Uid(4));
    
    CLbtTriggerFilterComposite* compfilter=CLbtTriggerFilterComposite::NewL();
    CleanupStack::PushL( compfilter );
    
    compfilter->AddFilterL(areafilter);
    compfilter->AddFilterL(attfilter);
    
      
    lbt.DeleteTriggersL(compfilter);
    
//    CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(err,lbt.GetTriggerLC(trigId1);
    	//{
    CleanupStack::PopAndDestroy( ););
    CleanupStack::PopAndDestroy( compfilter );
    CleanupStack::Pop( attfilter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return err;
    //	}
    }
    
    
 // -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_37L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_37_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetManagerUi(TUid::Uid(4));
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
         
    
    CLbtGeoCircle* area=CLbtGeoCircle::NewL();
    area->SetRadius(100);
    area->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate);        
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(area2); 
       
   
    CLbtTriggerFilterByAttribute* attfilter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( attfilter );    
    
    attfilter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
    attfilter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    attfilter->AddTriggerManagerUiL(TUid::Uid(4));
    
    CLbtTriggerFilterComposite* compfilter=CLbtTriggerFilterComposite::NewL();
    CleanupStack::PushL( compfilter );
    
    compfilter->AddFilterL(areafilter);
    compfilter->AddFilterL(attfilter);
    
      
    lbt.DeleteTriggersL(compfilter);
    
//    CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(err,lbt.GetTriggerLC(trigId1);	
  	CleanupStack::PopAndDestroy(  ););
  	CleanupStack::PopAndDestroy( compfilter );
    CleanupStack::Pop( attfilter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    
    if(err==KErrNotFound)
    {
    
    delete wait; 
    return KErrNone;
    }
    return KErrNone;
    
    }   



// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_12L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_12_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	  	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
        
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object
     
    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
     
    lbt.DeleteTriggersL();
    
    //CLbtTriggerInfo* trigEntry=NULL;
    CLbtTriggerInfo* trigEntry=lbt.GetTriggerLC(trigId1);
    
               
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_13L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_13_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate1(50,60,70);
 	  	 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	       
    CLbtGeoCircle* area1=CLbtGeoCircle::NewL();
    area1->SetRadius(100);
    area1->SetCenter(coordinate1);
    
         
    CLbtTriggerFilterByArea* filter=CLbtTriggerFilterByArea::NewL(area1); 
    CleanupStack::PushL( filter );    
    
   
    TRAPD(err,lbt.DeleteTriggersL(filter));
    
   // AssertTrueL( KErrNone!=err,_L("wrong error code"));
    if(err!=KErrNone)
    {
    }
  //  CLbtTriggerInfo* trigEntry=NULL;
   // TLbtTriggerId trigId1=0x05; 
    
  //  trigEntry=lbt.GetTriggerLC(trigId1);
        
  //  CleanupStack::PopAndDestroy( trigEntry );    
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
     
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_14L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_14_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate1(10,20,30);
 	 TCoordinate coordinate2(70,80,90);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
        
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);  
    
    CLbtGeoCircle* area1=CLbtGeoCircle::NewL();
    area1->SetRadius(100);
    area1->SetCenter(coordinate1);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area1,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate1,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate2);    
    
    
    CLbtTriggerFilterByArea* filter=CLbtTriggerFilterByArea::NewL(area2); 
    CleanupStack::PushL( filter );    
    
   
    lbt.DeleteTriggersL(filter);
    
    CLbtTriggerInfo* trigEntry=NULL;
    trigEntry=lbt.GetTriggerLC(trigId1);
    
    CleanupStack::PopAndDestroy( trigEntry );           
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }


// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_15L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_15_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 	 
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtServerBadHandle );
    lbt.DeleteTriggersL();
         
    return KErrNone;
    
    }


// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_16L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_16_testL( CStifItemParser& /* aItem */ )
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
    trig1->SetManagerUi(TUid::Uid(4));
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object 
    
    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    
     //Construct a session trigger
    CLbtSessionTrigger* trig2 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 );
        
    trig2->SetNameL(_L("Trigger2"));
    trig2->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetManagerUi(TUid::Uid(5));
    
    
	trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
    
              
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig2->SetCondition(condition2); // ownership transferred to object 
    
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteFilteredTriggers(lbt,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
            
    CLbtTriggerInfo* trigEntry=NULL;
       
       
    trigEntry=lbt.GetTriggerLC(trigId2);
    trigEntry=lbt.GetTriggerLC(trigId1);
    
    CleanupStack::PopAndDestroy( trigEntry );
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( trig2 );   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_17L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_17_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(61.5285,23.9385);
 	 
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
    trig1->SetManagerUi(TUid::Uid(4));
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
     // set condition
    
    
              
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object 
    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    
     //Construct a session trigger
    CLbtSessionTrigger* trig2 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 );
        
    trig2->SetNameL(_L("Trigger2"));
    trig2->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetManagerUi(TUid::Uid(5));
    
    
	trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
     // set condition
    
    
              
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig2->SetCondition(condition2); // ownership transferred to object 
    
    
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteFilteredTriggers(lbt,filter,wait2);
    wait2->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
    CLbtTriggerInfo* trigEntry=NULL;
       
        
    trigEntry=lbt.GetTriggerLC(trigId2);
    trigEntry=lbt.GetTriggerLC(trigId1);
    
    CleanupStack::PopAndDestroy( trigEntry );
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( trig2 );   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_18L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_18_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetManagerUi(TUid::Uid(4));
    
    
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    
     //Construct a session trigger
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 );
        
    trig2->SetNameL(_L("Trigger2"));
    trig2->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetManagerUi(TUid::Uid(5));
    
    
    trig2->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    
	trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
             
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig2->SetCondition(condition2); // ownership transferred to object
    
    
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
    filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteFilteredTriggers(lbt,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
    
    CLbtTriggerInfo* trigEntry=NULL;
        
       
    trigEntry=lbt.GetTriggerLC(trigId2);
    trigEntry=lbt.GetTriggerLC(trigId1);
    
    CleanupStack::PopAndDestroy( trigEntry );
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( trig2 );   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_19L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_19_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(61.5285,23.9385);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetManagerUi(TUid::Uid(4));
    
    
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
             
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    
     //Construct a session trigger
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 );
        
    trig2->SetNameL(_L("Trigger2"));
    trig2->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetManagerUi(TUid::Uid(5));
    
    trig2->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
    
	trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
             
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig2->SetCondition(condition2); // ownership transferred to object
    
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteFilteredTriggers(lbt,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
    
    CLbtTriggerInfo* trigEntry=NULL;
       
      
    trigEntry=lbt.GetTriggerLC(trigId2);
    trigEntry=lbt.GetTriggerLC(trigId1);
    
    CleanupStack::PopAndDestroy( trigEntry );
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( trig2 );   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_20L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_20_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
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
    
    CLbtGeoCircle* area=CLbtGeoCircle::NewL();
    area->SetRadius(100);
    area->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area,CLbtTriggerConditionArea::EFireOnEnter);
       
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig1->SetCondition(condn);
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate);    
    CLbtTriggerFilterByArea* filter=CLbtTriggerFilterByArea::NewL(area2); 
    CleanupStack::PushL( filter );    
    
   
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteFilteredTriggers(lbt,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
    
    //CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(err,lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(););
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::Pop( filter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(err==KErrNotFound)
    	{
    
  
    delete wait; 
    return err;
    	}
    return err;
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_21L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_21_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );
       
    CLbtGeoCircle* area=CLbtGeoCircle::NewL();
    area->SetRadius(100);
    area->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate);        
    CLbtTriggerFilterByArea* filter=CLbtTriggerFilterByArea::NewL(area2); 
    CleanupStack::PushL( filter );    
    
   
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteFilteredTriggers(lbt,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
  //  CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(err,lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(););
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::Pop( filter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(err==KErrNotFound)
    	{
    
  
    delete wait; 
    return err;
    	}
    return err;
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_38L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_38_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
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
    trig1->SetManagerUi(TUid::Uid(4));
         
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CLbtGeoCircle* area=CLbtGeoCircle::NewL();
    area->SetRadius(100);
    area->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate);        
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(area2); 
       
   
    CLbtTriggerFilterByAttribute* attfilter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( attfilter );    
    
    attfilter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    attfilter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    attfilter->AddTriggerManagerUiL(TUid::Uid(4));
    
    CLbtTriggerFilterComposite* compfilter=CLbtTriggerFilterComposite::NewL();
    CleanupStack::PushL( compfilter );
    
    compfilter->AddFilterL(areafilter);
    compfilter->AddFilterL(attfilter);
    
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteFilteredTriggers(lbt,compfilter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
      
    
  //  CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(error,lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(););
    CleanupStack::PopAndDestroy( observer );    
    CleanupStack::PopAndDestroy( compfilter );
    CleanupStack::Pop( attfilter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrNotFound)
    	{
    
  
    delete wait; 
    return error;
    	}
    	return error;
    }
    
    
 // -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_39L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_39_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetManagerUi(TUid::Uid(4));
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
         
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CLbtGeoCircle* area=CLbtGeoCircle::NewL();
    area->SetRadius(100);
    area->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
     CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate);    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(area2); 
       
   
    CLbtTriggerFilterByAttribute* attfilter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( attfilter );    
    
    attfilter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
    attfilter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    attfilter->AddTriggerManagerUiL(TUid::Uid(4));
    
    CLbtTriggerFilterComposite* compfilter=CLbtTriggerFilterComposite::NewL();
    CleanupStack::PushL( compfilter );
    
    compfilter->AddFilterL(areafilter);
    compfilter->AddFilterL(attfilter);
    
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteFilteredTriggers(lbt,compfilter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
      
    
    //CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(err,lbt.GetTriggerLC(trigId1);	CleanupStack::PopAndDestroy(  );    );
    	CleanupStack::PopAndDestroy( observer );    
    
    CleanupStack::PopAndDestroy( compfilter );
    CleanupStack::Pop( attfilter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(err==KErrNotFound)
    {
    
  
    delete wait; 
    return KErrNone;
    }
    return err;
    
    }   





// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_22L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_22_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	   
    CLbtGeoCircle* area=CLbtGeoCircle::NewL();
    area->SetRadius(100);
    area->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn); 
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
     
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteAllTriggers(lbt,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
   // CLbtTriggerInfo* trigEntry=NULL;
    CLbtTriggerInfo* trigEntry=lbt.GetTriggerLC(trigId1);
    
   
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_23L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_23_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate1(10,20,30);
 	  	 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	       
    CLbtGeoCircle* area1=CLbtGeoCircle::NewL();
    area1->SetRadius(100);
    area1->SetCenter(coordinate1);
    
         
    CLbtTriggerFilterByArea* filter=CLbtTriggerFilterByArea::NewL(area1); 
    CleanupStack::PushL( filter );    
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteFilteredTriggers(lbt,filter,wait);
    wait->Start();
    
  //  AssertTrueL( KErrNotFound==observer->CompletionCode(),_L("wrong error code"));
    
        
    CLbtTriggerInfo* trigEntry=NULL;
    TLbtTriggerId trigId1=0x05; 
    
    trigEntry=lbt.GetTriggerLC(trigId1);
        
    CleanupStack::PopAndDestroy( trigEntry );    
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
     
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_24L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_24_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate1(10,20,30);
 	 TCoordinate coordinate2(75,85,95);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	TRAP_IGNORE(lbt.DeleteTriggersL());
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CLbtGeoCircle* area1=CLbtGeoCircle::NewL();
    area1->SetRadius(100);
    area1->SetCenter(coordinate1);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area1,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate1,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate2);    
    
    
    CLbtTriggerFilterByArea* filter=CLbtTriggerFilterByArea::NewL(area2); 
    CleanupStack::PushL( filter );    
    
   
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteFilteredTriggers(lbt,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
    CLbtTriggerInfo* trigEntry=NULL;
    trigEntry=lbt.GetTriggerLC(trigId1);
        
    CleanupStack::PopAndDestroy( trigEntry );    
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait; 
    return KErrNone;
    
    }


// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_25L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_25_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 	 
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtServerBadHandle );
        
    observer->DeleteAllTriggers(lbt,wait);
    wait->Start();
         
    CleanupStack::PopAndDestroy( observer );
    delete wait;
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_26L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_26_testL( CStifItemParser& /* aItem */ )
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
     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object 
    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId2,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerId trigId3;
    notifier->CreateTriggers( lbt,*trig1,trigId3,ETrue,wait );
    wait->Start( );
    
    RArray< TLbtTriggerId > list;
    list.Append(trigId1);
    list.Append(trigId2);
         
   
    lbt.DeleteTriggersL(list);
    
    CLbtTriggerInfo* trigEntry=NULL;
    
          
           
    trigEntry=lbt.GetTriggerLC(trigId3);
    
    trigEntry=lbt.GetTriggerLC(trigId2);
    
    //trigEntry=lbt.GetTriggerLC(trigId1);
            
    CleanupStack::PopAndDestroy( trigEntry );   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait;
    list.Close(); 
    return KErrNone;
    
    }


// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_27L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_27_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CLbtGeoCircle* area1=CLbtGeoCircle::NewL();
    area1->SetRadius(100);
    area1->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area1,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId2,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerId trigId3;
    notifier->CreateTriggers( lbt,*trig1,trigId3,ETrue,wait );
    wait->Start( );
    
    RArray< TLbtTriggerId > list;
    list.Append(trigId1);
    list.Append(trigId2);
         
   
    lbt.DeleteTriggersL(list); 
    
    CLbtTriggerInfo* trigEntry=NULL;
    
    trigEntry=lbt.GetTriggerLC(trigId3);
    
    trigEntry=lbt.GetTriggerLC(trigId2);
    
    //trigEntry=lbt.GetTriggerLC(trigId1);
            
    CleanupStack::PopAndDestroy( trigEntry );  
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait;
    list.Close(); 
    return KErrNone;
    
    }
// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_28L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_28_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CLbtGeoCircle* area1=CLbtGeoCircle::NewL();
    area1->SetRadius(100);
    area1->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area1,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerId trigId2=1022;
    
    
    RArray< TLbtTriggerId > list;
    list.Append(trigId2);
    list.Append(trigId1);
    
    TRAPD(err,lbt.DeleteTriggersL(list)); 
    if(err!= KErrNone)
    {
    }
   // AssertTrueL( KErrNotFound==err,_L("wrong error code"));
    
    CLbtTriggerInfo* trigEntry=lbt.GetTriggerLC(trigId1);
          
    CleanupStack::PopAndDestroy( trigEntry );  
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait;
    list.Close(); 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_29L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_29_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CLbtGeoCircle* area1=CLbtGeoCircle::NewL();
    area1->SetRadius(100);
    area1->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area1,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
     
    
    RArray< TLbtTriggerId > list;
        
    lbt.DeleteTriggersL(list); 
    
       
    CLbtTriggerInfo* trigEntry=lbt.GetTriggerLC(trigId1);
          
    CleanupStack::PopAndDestroy( trigEntry );  
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait;
    list.Close(); 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_30L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_30_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	     
    RArray< TLbtTriggerId > list;
        
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtServerBadHandle );
    lbt.DeleteTriggersL(list); 
    
    list.Close(); 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_31L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_31_testL( CStifItemParser& /* aItem */ )
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
      
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    // set condition
    
     
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,115000);
      
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
           
    trig1->SetCondition(condition1); // ownership transferred to object 
    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId2,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerId trigId3;
    notifier->CreateTriggers( lbt,*trig1,trigId3,ETrue,wait );
    wait->Start( );
    
    RArray< TLbtTriggerId > list;
    list.Append(trigId1);
    list.Append(trigId2);
         
   
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteTriggersById(lbt,list,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
    CLbtTriggerInfo* trigEntry=NULL;
    
    trigEntry=lbt.GetTriggerLC(trigId3);
    
    trigEntry=lbt.GetTriggerLC(trigId2);
    
    //trigEntry=lbt.GetTriggerLC(trigId1);
            
    CleanupStack::PopAndDestroy( trigEntry );  
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait;
    list.Close(); 
    return KErrNone;
    
    }


// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_32L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_32_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CLbtGeoCircle* area1=CLbtGeoCircle::NewL();
    area1->SetRadius(100);
    area1->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area1,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerId trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId2,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerId trigId3;
    notifier->CreateTriggers( lbt,*trig1,trigId3,ETrue,wait );
    wait->Start( );
    
    RArray< TLbtTriggerId > list;
    list.Append(trigId1);
    list.Append(trigId2);
         
   
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteTriggersById(lbt,list,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
    CLbtTriggerInfo* trigEntry=NULL;
    
    trigEntry=lbt.GetTriggerLC(trigId3);
    
    trigEntry=lbt.GetTriggerLC(trigId2);
    
    //trigEntry=lbt.GetTriggerLC(trigId1);
            
    CleanupStack::PopAndDestroy( trigEntry );  
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait;
    list.Close(); 
    return KErrNone;
    
    }
// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_33L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_33_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CLbtGeoCircle* area1=CLbtGeoCircle::NewL();
    area1->SetRadius(100);
    area1->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area1,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerId trigId2=0;
    
    
    RArray< TLbtTriggerId > list;
    list.Append(trigId2);
    
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteTriggersById(lbt,list,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
        
    CLbtTriggerInfo* trigEntry=lbt.GetTriggerLC(trigId1);
          
    CleanupStack::PopAndDestroy( trigEntry );
    CleanupStack::PopAndDestroy( observer );  
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait;
    list.Close(); 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_34L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_34_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate(10,20,30);
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );
    
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    CLbtGeoCircle* area1=CLbtGeoCircle::NewL();
    area1->SetRadius(100);
    area1->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area1,CLbtTriggerConditionArea::EFireOnEnter);
       
    trig1->SetCondition(condn);
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
     
    
    RArray< TLbtTriggerId > list;
        
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
    
    observer->DeleteTriggersById(lbt,list,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==observer->CompletionCode(),_L("wrong error code"));
    
       
    CLbtTriggerInfo* trigEntry=lbt.GetTriggerLC(trigId1);
          
    CleanupStack::PopAndDestroy( trigEntry );
    CleanupStack::PopAndDestroy( observer );  
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    delete wait;
    list.Close(); 
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// Ct_deletetriggers::TC_LBT013_35L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_deletetriggers::TC_LBT013_35_testL( CStifItemParser& /* aItem */ )
    {
     
     RLbtServer lbtserver;
 	 RLbt lbt;
 	     
    RArray< TLbtTriggerId > list;
        
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); 
        
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtServerBadHandle );
    observer->DeleteTriggersById(lbt,list,wait);
    wait->Start(); 
    
    CleanupStack::PopAndDestroy( observer );  
    list.Close(); 
    delete wait;
    return KErrNone;
    
    }
// -----------------------------------------------------------------------------
// Ct_deletetriggers::AssertTrueL
// function for condition check.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void Ct_deletetriggers::AssertTrueL( TBool aCondition,const TDesC& aMessage )
    {
           
         if(!aCondition)
         {
          
    	   iLog->Log( aMessage );
           User::LeaveIfError(KErrGeneral);
         }
    
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  End of File
