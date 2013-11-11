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
* Description:    Test module to implement Test cases for LBT Management settings
*
*/





// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "t_mgmtsettings.h"
#include "t_triggerfireobserver.h"
#include "t_aooperation.h"
#include <centralrepository.h>
#include <lbs/SimulationPSYInternalCRKeys.h>

//LT API
#include <lbtcommon.h>
#include <lbterrors.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeorect.h>
#include <lbtgeocell.h>
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


///LT Magmt API
#include <lbtmanager.h> 
#include <lbttriggeringsystemsettings.h>
#include <lbttriggeringsystemmanagementsettings.h>
#include <lbttriggeringsystemstatus.h>


#include <e32property.h>
#include <centralrepository.h>



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
// Ct_mgmtsettings::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ct_mgmtsettings::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        
        ENTRY( "GetDefaultSettings", Ct_mgmtsettings::GetDefaultSettingsL ),
        ENTRY( "SetFewSystemSettings", Ct_mgmtsettings::SetFewSystemSettingsL ),
        ENTRY( "SetAllSystemSettings", Ct_mgmtsettings::SetAllSystemSettingsL ),
        ENTRY( "GetAllSystemSettings", Ct_mgmtsettings::GetAllSystemSettingsL ),
        ENTRY( "NotifyTriggeringSettingsChange", Ct_mgmtsettings::NotifyTriggeringSettingsChangeL ),
        ENTRY( "GetTrigger1", Ct_mgmtsettings::GetTrigger1L ),
        ENTRY( "UpdateTrigger", Ct_mgmtsettings::UpdateTriggerL ),
        ENTRY( "SetTriggerState1", Ct_mgmtsettings::SetTriggerState1L ),
        ENTRY( "SetTriggerState2", Ct_mgmtsettings::SetTriggerState2L ),
        ENTRY( "SetTriggerState3", Ct_mgmtsettings::SetTriggerState3L ),
        ENTRY( "SetTriggerState4", Ct_mgmtsettings::SetTriggerState4L ),
        ENTRY( "SetTriggerState5", Ct_mgmtsettings::SetTriggerState5L ),
        ENTRY( "ListTriggerId1", Ct_mgmtsettings::ListTriggerId1L ),
        ENTRY( "ListTriggerId2", Ct_mgmtsettings::ListTriggerId2L ),
        ENTRY( "ListTriggerId3", Ct_mgmtsettings::ListTriggerId3L ),
        ENTRY( "ListTriggerId4", Ct_mgmtsettings::ListTriggerId4L ),
        ENTRY( "GetTriggers1", Ct_mgmtsettings::GetTriggers1L ),
        ENTRY( "GetTriggers2", Ct_mgmtsettings::GetTriggers2L ),
        ENTRY( "GetTriggers3", Ct_mgmtsettings::GetTriggers3L ),
        ENTRY( "GetTriggers4", Ct_mgmtsettings::GetTriggers4L ),
        ENTRY( "GetNextTrigger", Ct_mgmtsettings::GetNextTriggerL ),
        ENTRY( "TestDeleteTriggerL", Ct_mgmtsettings::TestDeleteTriggerL ),
        ENTRY( "TestDeleteTriggers1L", Ct_mgmtsettings::TestDeleteTriggers1L ),
        ENTRY( "TestDeleteTriggers2L", Ct_mgmtsettings::TestDeleteTriggers2L ),
        ENTRY( "TestDeleteTriggers3L", Ct_mgmtsettings::TestDeleteTriggers3L ),
        ENTRY( "testGetNextTrigger1L", Ct_mgmtsettings::testGetNextTrigger1L ),
        ENTRY( "testGetNextTrigger2L", Ct_mgmtsettings::testGetNextTrigger2L),
        ENTRY( "TestCreatetrigger", Ct_mgmtsettings::TestCreatetriggerL ),
        ENTRY( "Cell_GetTrigger1", Ct_mgmtsettings::Cell_GetTrigger1L ),
        ENTRY( "Cell_UpdateTrigger", Ct_mgmtsettings::Cell_UpdateTriggerL ),
        ENTRY( "Cell_SetTriggerState1", Ct_mgmtsettings::Cell_SetTriggerState1L ),
        ENTRY( "Cell_SetTriggerState2", Ct_mgmtsettings::Cell_SetTriggerState2L ),
        ENTRY( "Cell_SetTriggerState3", Ct_mgmtsettings::Cell_SetTriggerState3L ),
        ENTRY( "Cell_SetTriggerState4", Ct_mgmtsettings::Cell_SetTriggerState4L ),
        ENTRY( "Cell_SetTriggerState5", Ct_mgmtsettings::Cell_SetTriggerState5L ),
        ENTRY( "Cell_ListTriggerId1", Ct_mgmtsettings::Cell_ListTriggerId1L ),
        ENTRY( "Cell_ListTriggerId2", Ct_mgmtsettings::Cell_ListTriggerId2L ),
        ENTRY( "Cell_ListTriggerId3", Ct_mgmtsettings::Cell_ListTriggerId3L ),
        ENTRY( "Cell_ListTriggerId4", Ct_mgmtsettings::Cell_ListTriggerId4L ),
        ENTRY( "Cell_GetTriggers1", Ct_mgmtsettings::Cell_GetTriggers1L ),
        ENTRY( "Cell_GetTriggers2", Ct_mgmtsettings::Cell_GetTriggers2L ),
        ENTRY( "Cell_GetTriggers3", Ct_mgmtsettings::Cell_GetTriggers3L ),
        ENTRY( "Cell_GetTriggers4", Ct_mgmtsettings::Cell_GetTriggers4L ),
        //ENTRY( "Cell_GetNextTrigger", Ct_mgmtsettings::Cell_GetNextTriggerL ),
        ENTRY( "Cell_TestDeleteTriggerL", Ct_mgmtsettings::Cell_TestDeleteTriggerL ),
        ENTRY( "Cell_TestDeleteTriggers1L", Ct_mgmtsettings::Cell_TestDeleteTriggers1L ),
        ENTRY( "Cell_TestDeleteTriggers2L", Ct_mgmtsettings::Cell_TestDeleteTriggers2L ),
        ENTRY( "Cell_testGetNextTrigger1L", Ct_mgmtsettings::Cell_testGetNextTrigger1L ),
        ENTRY( "Cell_testGetNextTrigger2L", Ct_mgmtsettings::Cell_testGetNextTrigger2L),
        ENTRY( "Cell_TestCreatetrigger", Ct_mgmtsettings::Cell_TestCreatetriggerL ),
        ENTRY( "test53", Ct_mgmtsettings::TC_LBT015_01_testL ),
        ENTRY( "test54", Ct_mgmtsettings::TC_LBT015_02_testL ),
        ENTRY( "test55", Ct_mgmtsettings::TC_LBT015_03_testL ),
        ENTRY( "test56", Ct_mgmtsettings::TC_LBT015_04_testL ),
        ENTRY( "test57", Ct_mgmtsettings::TC_LBT015_05_testL ),
        ENTRY( "TestdeleteTrigger4L", Ct_mgmtsettings::TestdeleteTrigger4L ),
        ENTRY( "TestdeleteTrigger5L", Ct_mgmtsettings::TestdeleteTrigger5L ),
        ENTRY( "TestNotifyTriggerChangeEventL", Ct_mgmtsettings::TestNotifyTriggerChangeEventL ),

               
        
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetDefaultSettingsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::GetDefaultSettingsL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbtManager mgrclient;
    TLbtTriggeringSystemManagementSettings settings;
    
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
    TRAP_IGNORE(mgrclient.DeleteTriggersL());
    mgrclient.GetTriggeringSystemSettingsL(settings);
    
    AssertTrueL( TLbtTriggeringSystemSettings::ETypeManagementSystemSettings==settings.Type(),_L("Wrong type"));
    AssertTrueL( 100==settings.MinimumTriggerAreaSize(),_L("Wrong minimum trigger area"));
    AssertTrueL( ETriggeringMechanismOn==settings.TriggeringMechanismState(),_L("Wrong default trigger mechanism state"));
    AssertTrueL( 1==settings.MinimumLocationUpdateInterval (),_L("Wrong mnm update interval"));
    AssertTrueL( 10000000==settings.MinimumLocationUpdateIntervalWhenGpsFails(),_L("Wrong mnm update interval when GPS fails"));
    AssertTrueL( TUid::Uid(270526860)==settings.UsedPositioningModule(),_L("Wrong used positioning module"));
    AssertTrueL( 42==settings.MaximumUserSpeed(),_L("Wrong maximum user speed"));
   
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&server);
   
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetFewSystemSettingsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::SetFewSystemSettingsL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbtManager mgrclient;
    TLbtTriggeringSystemManagementSettings settings;
    
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
    
    settings.SetMinimumLocationUpdateInterval(2);
    settings.SetMaximumUserSpeed(15);
        
    mgrclient.SetTriggeringSystemSettingsL(settings,EMinimumLocationUpdateInterval|EMaximumUserSpeed);
    
    mgrclient.GetTriggeringSystemSettingsL(settings);
    
    AssertTrueL( TLbtTriggeringSystemSettings::ETypeManagementSystemSettings==settings.Type(),_L("Wrong type"));
    AssertTrueL( 100==settings.MinimumTriggerAreaSize (),_L("Wrong minimum trigger area"));
    AssertTrueL( ETriggeringMechanismOn==settings.TriggeringMechanismState(),_L("Wrong default trigger mechanism state"));
    AssertTrueL( 2==settings.MinimumLocationUpdateInterval (),_L("Wrong mnm update interval"));
    AssertTrueL( 10000000==settings.MinimumLocationUpdateIntervalWhenGpsFails(),_L("Wrong mnm update interval when GPS fails"));
   // AssertTrueL( TUid::Uid(0)==settings.UsedPositioningModule(),_L("Wrong used positioning module"));
    AssertTrueL( 15==settings.MaximumUserSpeed(),_L("Wrong maximum user speed"));
   
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&server);
    
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetAllSystemSettingsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::SetAllSystemSettingsL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbtManager mgrclient;
    TLbtTriggeringSystemManagementSettings settings;
    
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
    
    settings.SetTriggeringMechanismState(ETriggeringMechanismOn);
    settings.SetMinimumLocationUpdateInterval(10);
    settings.SetMinimumLocationUpdateIntervalWhenGpsFails(1);
    settings.SetUsedPositioningModule(TUid::Uid(0));
    settings.SetMaximumUserSpeed(30);
        
    mgrclient.SetTriggeringSystemSettingsL(settings,EMinimumLocationUpdateInterval|EMaximumUserSpeed|EMinimumLocationUpdateIntervalWhenGpsFails|EUsedPositioningmodule|EMaximumUserSpeed|ETriggeringMechanismState);
    mgrclient.GetTriggeringSystemSettingsL(settings);
    
    AssertTrueL( TLbtTriggeringSystemSettings::ETypeManagementSystemSettings==settings.Type(),_L("Wrong type"));
    AssertTrueL( ETriggeringMechanismOn==settings.TriggeringMechanismState(),_L("Wrong default trigger mechanism state"));
    AssertTrueL( 10==settings.MinimumLocationUpdateInterval (),_L("Wrong mnm update interval"));
    AssertTrueL( 1==settings.MinimumLocationUpdateIntervalWhenGpsFails(),_L("Wrong mnm update interval when GPS fails"));
    AssertTrueL( TUid::Uid(0)==settings.UsedPositioningModule(),_L("Wrong used positioning module"));
    AssertTrueL( 30==settings.MaximumUserSpeed(),_L("Wrong maximum user speed"));
   
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&server);
    
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetAllSystemSettingsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::GetAllSystemSettingsL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbtManager mgrclient;
    TLbtTriggeringSystemManagementSettings settings;
    
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
    
           
    mgrclient.GetTriggeringSystemSettingsL(settings);
    
    AssertTrueL( TLbtTriggeringSystemSettings::ETypeManagementSystemSettings==settings.Type(),_L("Wrong type"));
    AssertTrueL( ETriggeringMechanismOff==settings.TriggeringMechanismState(),_L("Wrong default trigger mechanism state"));
    AssertTrueL( 8==settings.MinimumLocationUpdateInterval (),_L("Wrong mnm update interval"));
    AssertTrueL( 2==settings.MinimumLocationUpdateIntervalWhenGpsFails(),_L("Wrong mnm update interval when GPS fails"));
    AssertTrueL( TUid::Uid(4)==settings.UsedPositioningModule(),_L("Wrong used positioning module"));
    AssertTrueL( 15==settings.MaximumUserSpeed(),_L("Wrong maximum user speed"));
   
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&server);
    
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::NotifyTriggeringSettingsChangeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------

TInt Ct_mgmtsettings::NotifyTriggeringSettingsChangeL( CStifItemParser& /* aItem */ )
    {

	 	 
	 RLbtServer server;
 	 RLbtManager mgrclient;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError(server.Connect());
 	 CleanupClosePushL( server );
 	 
     User::LeaveIfError(mgrclient.Open(server));
     CleanupClosePushL( mgrclient );
     
     
     User::LeaveIfError(lbt.Open(server));
     CleanupClosePushL( lbt );
 	 
     TCoordinate coordinate;
     TLbtTriggeringSystemManagementSettings settings,dummySettings;
     
     settings.SetTriggeringMechanismState(ETriggeringMechanismOff);
     settings.SetMinimumLocationUpdateInterval(1);
     settings.SetMinimumLocationUpdateIntervalWhenGpsFails(2);
     settings.SetUsedPositioningModule(TUid::Uid(4));
     settings.SetMaximumUserSpeed(15);
     
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( mgrclient,coordinate,0 );
     CleanupStack::PushL( notifier );
     
     CActiveSchedulerWait *waitLoop = new(ELeave) CActiveSchedulerWait;
     notifier->NotifyTriggeringSettingsChange( lbt,settings,dummySettings,waitLoop ); 
     mgrclient.SetTriggeringSystemSettingsL(settings,ETriggeringMechanismState);        
     waitLoop->Start();
     
     
     notifier->NotifyTriggeringSettingsChange( mgrclient,settings,dummySettings,waitLoop ); 
     settings.SetMinimumLocationUpdateInterval(1);
     mgrclient.SetTriggeringSystemSettingsL(settings,EMinimumLocationUpdateInterval);        
     waitLoop->Start();
     
     
     
     notifier->NotifyTriggeringSettingsChange( mgrclient,settings,dummySettings,waitLoop ); 
     mgrclient.SetTriggeringSystemSettingsL(settings,EMaximumUserSpeed);        
     waitLoop->Start();
     
     
     notifier->NotifyTriggeringSettingsChange( mgrclient,settings,dummySettings,waitLoop ); 
     mgrclient.SetTriggeringSystemSettingsL(settings,EMaximumUserSpeed);        
     waitLoop->Start();
    /* TRequestStatus aStatus;
     mgrclient.NotifyTriggeringSystemSettingChange( settings,aStatus );  
     mgrclient.SetTriggeringSystemSettingsL(settings,EMaximumUserSpeed);        
     User::WaitForRequest(aStatus);*/
     AssertTrueL( 15==settings.MaximumUserSpeed(),_L("Wrong maximum user speed"));
     
     AssertTrueL( ETriggeringMechanismOff==settings.TriggeringMechanismState(),_L("Wrong trigger mechanism state"));
     AssertTrueL( 1==settings.MinimumLocationUpdateInterval (),_L("Wrong mnm update interval"));
 //    AssertTrueL( TUid::Uid(4)==settings.UsedPositioningModule(),_L("Wrong used positioning module"));
     delete waitLoop;         
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy(&lbt);
     CleanupStack::PopAndDestroy(&mgrclient);
     CleanupStack::PopAndDestroy(&server);
     
     return KErrNone;
	 
    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetTrigger1L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::GetTrigger1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
      //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtTriggerInfo* trigger=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry=trigger->TriggerEntry();
    
    AssertTrueL( trigId==trigEntry->Id(),_L("Wrong Name"));
    
    CleanupStack::PopAndDestroy(trigger);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    delete wait;
        
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::UpdateTriggerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::UpdateTriggerL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
      //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetId(trigId);
        
    mgrclient.UpdateTriggerL(*trig,CLbtTriggerEntry::EAttributeState,ELbtTrue);
    
    CLbtTriggerInfo* trigger=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry=trigger->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateDisabled==trigEntry->State(),_L("Wrong Name"));    
        
    trig->SetNameL(_L("Trigger01"));
    
    mgrclient.UpdateTriggerL(*trig,CLbtTriggerEntry::EAttributeName,ELbtTrue);
  
    CleanupStack::PopAndDestroy(trigger);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    delete wait;
        
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetTriggerState1L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::SetTriggerState1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    mgrclient.SetTriggerStateL(trigId,CLbtTriggerEntry::EStateDisabled,ELbtTrue);
    
    CLbtTriggerInfo* trigger1=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry1=trigger1->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateDisabled==trigEntry1->State(),_L("Wrong state"));
       
    
    mgrclient.SetTriggerStateL(trigId,CLbtTriggerEntry::EStateEnabled,ELbtTrue);
    
    CLbtTriggerInfo* trigger2=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry2=trigger2->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateEnabled==trigEntry2->State(),_L("Wrong state"));
       
    
    CleanupStack::PopAndDestroy(2);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    delete wait;
        
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetTriggerState2L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::SetTriggerState2L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    
    
    mgrclient.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,filter);
    
    CLbtTriggerInfo* trigger1=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry1=trigger1->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateEnabled==trigEntry1->State(),_L("Wrong state"));
       
    
        
    CleanupStack::PopAndDestroy(trigger1);
    CleanupStack::PopAndDestroy(filter);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    delete wait;
        
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetTriggerState3L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::SetTriggerState3L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtTriggerFilterByAttribute* filter=NULL;
    
    mgrclient.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,filter);
    
    CLbtTriggerInfo* trigger1=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry1=trigger1->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateEnabled==trigEntry1->State(),_L("Wrong state"));
  
        
    CleanupStack::PopAndDestroy(trigger1);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
        
    delete wait;
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetTriggerState4L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::SetTriggerState4L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
     CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
     CleanupStack::PushL( filter );    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    
    CAoOperation* ao=CAoOperation::NewL(); 
    CleanupStack::PushL( ao ); 
    
    ao->SetTriggerState(mgrclient,CLbtTriggerEntry::EStateEnabled,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    
    CLbtTriggerInfo* trigger1=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry1=trigger1->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateEnabled==trigEntry1->State(),_L("Wrong state"));
       
    
    CleanupStack::PopAndDestroy(trigger1);
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::PopAndDestroy(filter);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
        
    delete wait;
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetTriggerState5L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::SetTriggerState5L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
     CLbtTriggerFilterByAttribute* filter=NULL;
    
    CAoOperation* ao=CAoOperation::NewL(); 
    CleanupStack::PushL( ao ); 
    
    ao->SetTriggerState(mgrclient,CLbtTriggerEntry::EStateEnabled,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    
    CLbtTriggerInfo* trigger1=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry1=trigger1->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateEnabled==trigEntry1->State(),_L("Wrong state"));
       
    
    CleanupStack::PopAndDestroy(trigger1);
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    delete wait;    
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::ListTriggerId1L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::ListTriggerId1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    
    listoptions->SetRetrievedFields( CLbtTriggerEntry::EAttributeState |  
                                     CLbtTriggerEntry::EAttributeName | 
                                     CLbtTriggerEntry::EAttributeManagerUi
                                   );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );
    
    RArray < TLbtTriggerId > triggerIdList;    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    
    listoptions->SetFilter(filter);
    
    mgrclient.ListTriggerIdsL(triggerIdList,listoptions);
    
    AssertTrueL( trigId==triggerIdList[0],_L("Wrong trigger"));
    
    
    CleanupStack::PopAndDestroy(filter);
    CleanupStack::PopAndDestroy(listoptions);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerIdList.Close();
    delete wait;    
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::ListTriggerId2L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::ListTriggerId2L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
    TRAP_IGNORE(mgrclient.DeleteTriggersL());
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
       
    RArray < TLbtTriggerId > triggerIdList;    
           
    mgrclient.ListTriggerIdsL(triggerIdList);
    
    AssertTrueL( trigId==triggerIdList[0],_L("Wrong trigger"));
    
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerIdList.Close();
    delete wait;    
    return KErrNone;

    }



// -----------------------------------------------------------------------------
// Ct_mgmtsettings::ListTriggerId3L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::ListTriggerId3L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    
    listoptions->SetRetrievedFields( CLbtTriggerEntry::EAttributeState |  
                                     CLbtTriggerEntry::EAttributeName | 
                                     CLbtTriggerEntry::EAttributeManagerUi
                                   );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );
    
    RArray < TLbtTriggerId > triggerIdList;    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    
    listoptions->SetFilter(filter);
    
    CAoOperation* ao=CAoOperation::NewL();
    CleanupStack::PushL( ao );
    
    ao->ListTriggerIds(mgrclient,triggerIdList,listoptions,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    AssertTrueL( trigId==triggerIdList[0],_L("Wrong trigger"));
    
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::PopAndDestroy(filter);
    CleanupStack::PopAndDestroy(listoptions);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerIdList.Close();
    delete wait;    
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::ListTriggerId4L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::ListTriggerId4L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
	TRAP_IGNORE(mgrclient.DeleteTriggersL());  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
       
    RArray < TLbtTriggerId > triggerIdList;    
    
    
    CAoOperation* ao=CAoOperation::NewL();
    CleanupStack::PushL( ao );
    
    ao->ListTriggerIds(mgrclient,triggerIdList,NULL,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    AssertTrueL( trigId==triggerIdList[0],_L("Wrong trigger"));
    
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerIdList.Close();
    delete wait;    
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetTriggers1L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::GetTriggers1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    
    listoptions->SetRetrievedFields( CLbtTriggerEntry::EAttributeState |  
                                     CLbtTriggerEntry::EAttributeName | 
                                     CLbtTriggerEntry::EAttributeManagerUi
                                   );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );
    
    RPointerArray< CLbtTriggerInfo > triggerList;    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    
    listoptions->SetFilter(filter);
    
    mgrclient.GetTriggersL(triggerList,listoptions);
    
    CLbtTriggerEntry* entry=triggerList[0]->TriggerEntry();
      
    AssertTrueL( trigId==entry->Id(),_L("Wrong id"));
    
    
    CleanupStack::PopAndDestroy(filter);
    CleanupStack::PopAndDestroy(listoptions);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerList.Close();
    delete wait;    
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetTriggers2L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::GetTriggers2L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
    TRAP_IGNORE(mgrclient.DeleteTriggersL());
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    RPointerArray< CLbtTriggerInfo > triggerList;    
    
    mgrclient.GetTriggersL(triggerList);
    
    CLbtTriggerEntry* entry=triggerList[0]->TriggerEntry();
      
    AssertTrueL( trigId==entry->Id(),_L("Wrong id"));
       
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerList.Close();
    delete wait;    
    return KErrNone;

    }



// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetTriggers3L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::GetTriggers3L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    
    listoptions->SetRetrievedFields( CLbtTriggerEntry::EAttributeState |  
                                     CLbtTriggerEntry::EAttributeName | 
                                     CLbtTriggerEntry::EAttributeManagerUi
                                   );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );
    
    RPointerArray< CLbtTriggerInfo > triggerList;    
    
    filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    filter->AddTriggerManagerUiL(TUid::Uid(4));
    
    listoptions->SetFilter(filter);
    
    CAoOperation* ao=CAoOperation::NewL();
    CleanupStack::PushL( ao );
    
    ao->GetTriggers( mgrclient,triggerList,listoptions,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    
    CLbtTriggerEntry* entry=triggerList[0]->TriggerEntry();
      
    AssertTrueL( trigId==entry->Id(),_L("Wrong id"));
    
    
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::PopAndDestroy(filter);
    CleanupStack::PopAndDestroy(listoptions);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerList.Close();
    delete wait;    
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetTriggers4L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::GetTriggers4L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
    TRAP_IGNORE(mgrclient.DeleteTriggersL());
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    RPointerArray< CLbtTriggerInfo > triggerList;    
    
       
    CAoOperation* ao=CAoOperation::NewL();
    CleanupStack::PushL( ao );
    
    ao->GetTriggers( mgrclient,triggerList,NULL,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    
    CLbtTriggerEntry* entry=triggerList[0]->TriggerEntry();
      
    AssertTrueL( trigId==entry->Id(),_L("Wrong id"));
    
    
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerList.Close();
    delete wait;    
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetNextTriggerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::GetNextTriggerL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
    TRAP_IGNORE(lbt.DeleteTriggersL());
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    lbt.CreateGetTriggerIteratorL();
    
    RArray<CLbtTriggerInfo> triggers;
    //Get triggers. Note, the loop will block 
    //active schedular
    CLbtTriggerInfo* trigInfo = lbt.GetNextTriggerLC(); 
   /* TInt i=1;
    while ( trigInfo != NULL )
        {
        	i++;
          triggers.Append(*trigInfo);
       //   CleanupStack::PopAndDestroy(trigInfo);
          trigInfo = mgrclient.GetNextTriggerLC(); 
        }
        
          
    CLbtTriggerEntry* entry=triggers[i-2].TriggerEntry();
      
    AssertTrueL( trigId==triggers[i-2].TriggerEntry()->Id(),_L("Wrong id"));    
    CleanupStack::PopAndDestroy(i)  ;  */
    CleanupStack::PopAndDestroy(1)  ;  
    
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggers.Close();
    delete wait;    
    return KErrNone;

    }


//Test case for testing Delete TriggerL
// -----------------------------------------------------------------------------
// Ct_mgmtsettings::TestDeleteTriggerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::TestDeleteTriggerL( CStifItemParser& /* aItem */ )
    {

    RLbtServer lbtserver;
	 RLbtManager lbtmanager;
	  
	  RLbt lbt;	 
	 User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
	 User::LeaveIfError( lbtmanager.Open( lbtserver ) );
	 CleanupClosePushL( lbtmanager );
	 User::LeaveIfError( lbt.Open( lbtserver ) );
	 CleanupClosePushL( lbt );
	 
///*	 
_LIT( KFilename,"CreateTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt triggerId=0;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        KPSUidTriggerIdInfo, 
	        KLbttesttriggerid, 
	        triggerId));
	// */
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
    
  
  
   CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbtmanager,coordinate,0);
   CleanupStack::PushL( notifier );
   
   CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       
   TLbtTriggerId trigId;
   notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
   wait->Start( );
   
   lbtmanager.DeleteTriggerL(trigId);
   lbtmanager.DeleteTriggerL(triggerId);
   CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
   
  // CLbtTriggerInfo* trigEntry=NULL;
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(););
   if(error1==KErrNotFound)
   {
   TRAPD(error2, lbtmanager.GetTriggerLC(triggerId);CleanupStack::PopAndDestroy(););
   if(error2==KErrNotFound)
	   {
	   CleanupStack::PopAndDestroy( &lbt );
      CleanupStack::PopAndDestroy( &lbtmanager );
      CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return KErrNone;
	   }
   else
	   {
	      CleanupStack::PopAndDestroy( &lbt );
	   	  CleanupStack::PopAndDestroy( &lbtmanager );
	      CleanupStack::PopAndDestroy( &lbtserver );
	      delete wait; 
	      return KErrNone;
	   }
   }
   else
   {
   CleanupStack::PopAndDestroy( &lbt );
   CleanupStack::PopAndDestroy( &lbtmanager );
   CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return -99;	
   }
  }

//Test DeleteTriggersL
TInt Ct_mgmtsettings::TestDeleteTriggers1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer lbtserver;
	 RLbtManager lbtmanager;
	 RLbt lbt;
	  	 
	 User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
	 User::LeaveIfError( lbtmanager.Open( lbtserver ) );
	 CleanupClosePushL( lbtmanager );
	 User::LeaveIfError( lbt.Open( lbtserver ) );
	 CleanupClosePushL( lbt );
	 
///*	 
_LIT( KFilename,"CreateTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt triggerId=0;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        KPSUidTriggerIdInfo, 
	        KLbttesttriggerid, 
	        triggerId));
	        //*/
	 
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
    
  
  
   CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbtmanager,coordinate,0);
   CleanupStack::PushL( notifier );
   
   CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       
   TLbtTriggerId trigId;
   notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
   wait->Start( );
   CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( trig );   
   lbtmanager.DeleteTriggersL();
   
  // CLbtTriggerInfo* trigEntry=NULL;
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(););
   if(error1==KErrNotFound)
   {
   TRAPD(error2, lbtmanager.GetTriggerLC(triggerId);CleanupStack::PopAndDestroy(););
   if(error2==KErrNotFound)
	   {
      CleanupStack::PopAndDestroy( &lbt );
      CleanupStack::PopAndDestroy( &lbtmanager );
      CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return KErrNone;
	   }
   else
	   {
	    CleanupStack::PopAndDestroy( &lbt );
	   CleanupStack::PopAndDestroy( &lbtmanager );
	      CleanupStack::PopAndDestroy( &lbtserver );
	      delete wait; 
	      return KErrNone;
	   }
   }
   else
   {
   CleanupStack::PopAndDestroy( &lbt );
   CleanupStack::PopAndDestroy( &lbtmanager );
   CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return -99;	
   }
  }


//Test DeleteTriggers
TInt Ct_mgmtsettings::TestDeleteTriggers2L( CStifItemParser& /* aItem */ )
    {

    RLbtServer lbtserver;
	 RLbtManager lbtmanager;
	 RLbt lbt;
	  	 
	 User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
	 User::LeaveIfError( lbtmanager.Open( lbtserver ) );
	 CleanupClosePushL( lbtmanager );
	 User::LeaveIfError( lbt.Open( lbtserver ) );
	 CleanupClosePushL( lbt );
//	/* 
	_LIT( KFilename,"CreateTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt triggerId=0;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        KPSUidTriggerIdInfo, 
	        KLbttesttriggerid, 
	        triggerId));
	        //*/
	 
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
    
  
  
   CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbtmanager,coordinate,0);
   CleanupStack::PushL( notifier );
   
   CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       
   TLbtTriggerId trigId;
   notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
   wait->Start( );
   
   notifier->DeleteAllTriggers(lbtmanager,wait);
   wait->Start();
   
   CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( trig );   
  // CLbtTriggerInfo* trigEntry=NULL;
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(););
   if(error1==KErrNotFound)
   {
   TRAPD(error2, lbtmanager.GetTriggerLC(triggerId);CleanupStack::PopAndDestroy(););
   if(error2==KErrNotFound)
	   {
	   CleanupStack::PopAndDestroy( &lbt );
      CleanupStack::PopAndDestroy( &lbtmanager );
      CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return KErrNone;
	   }
   else
	   {
	   CleanupStack::PopAndDestroy( &lbt );
	   CleanupStack::PopAndDestroy( &lbtmanager );
	      CleanupStack::PopAndDestroy( &lbtserver );
	      delete wait; 
	      return KErrNone;
	   }
   }
   else
   {
   CleanupStack::PopAndDestroy( &lbt );
   CleanupStack::PopAndDestroy( &lbtmanager );
   CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return -99;	
   }
  }
  
  
  //Test DeleteTriggers
//Test DeleteTriggersL
TInt Ct_mgmtsettings::TestDeleteTriggers3L( CStifItemParser& /* aItem */ )
    {

    RLbtServer lbtserver;
	 RLbtManager lbtmanager;
	 RLbt lbt;
	  	 
	 User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
	 User::LeaveIfError( lbtmanager.Open( lbtserver ) );
	 CleanupClosePushL( lbtmanager );
	 User::LeaveIfError( lbt.Open( lbtserver ) );
	 CleanupClosePushL( lbt );
	 
///*	 
_LIT( KFilename,"CreateTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt triggerId=0;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        KPSUidTriggerIdInfo, 
	        KLbttesttriggerid, 
	        triggerId));
	        //*/
	 
	 //Construct a session trigger
   CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
   
   //Push to cleanup stack
   CleanupStack::PushL( trig );
       
    // Set Name
   trig->SetNameL(_L("Trigger1"));
   trig->SetState( CLbtTriggerEntry::EStateEnabled );
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
    
  
  
   CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbtmanager,coordinate,0);
   CleanupStack::PushL( notifier );
   
   CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       
   TLbtTriggerId trigId;
   notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
   wait->Start( );
   CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( trig ); 
   //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerValidityL( TLbtTriggerDynamicInfo::EValid );  
   // attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession);  
   lbtmanager.DeleteTriggersL(attribFilter);
   
 //  CLbtTriggerInfo* trigEntry=NULL;
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(););
   if(error1==KErrNotFound)
   {
   TRAPD(error2, lbtmanager.GetTriggerLC(triggerId);CleanupStack::PopAndDestroy(););
   if(error2==KErrNotFound)
	   {
      CleanupStack::PopAndDestroy(attribFilter);
      CleanupStack::PopAndDestroy( &lbt );
      CleanupStack::PopAndDestroy( &lbtmanager );
      CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return KErrNone;
	   }
   else
	   {
	    CleanupStack::PopAndDestroy(attribFilter);
	    CleanupStack::PopAndDestroy( &lbt );
	   CleanupStack::PopAndDestroy( &lbtmanager );
	      CleanupStack::PopAndDestroy( &lbtserver );
	      delete wait; 
	      return KErrNone;
	   }
   }
   else
   {
   CleanupStack::PopAndDestroy( &lbt );
   CleanupStack::PopAndDestroy( &lbtmanager );
   CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return -99;	
   }
  }


//Test CreateTriggerIterator and GetNextTriggerLC
// -----------------------------------------------------------------------------
// Ct_mgmtsettings::testGetNextTriggerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::testGetNextTrigger1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
       
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    mgrclient.CreateGetTriggerIteratorL();
    
    RArray<CLbtTriggerInfo> triggers;
    //Get triggers. Note, the loop will block 
    //active schedular
    triggers.Reset();
    CLbtTriggerInfo* trigInfo = mgrclient.GetNextTriggerLC(); 
    TInt i=1;
    while ( trigInfo != NULL )
        {
        	i++;
          triggers.Append(*trigInfo);
       //   CleanupStack::PopAndDestroy(trigInfo);
          trigInfo = mgrclient.GetNextTriggerLC(); 
        }
        
          
    CLbtTriggerEntry* entry=triggers[i-2].TriggerEntry();
      
    AssertTrueL( trigId==triggers[i-2].TriggerEntry()->Id(),_L("Wrong id"));    
    CleanupStack::PopAndDestroy(i)  ;  
    
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggers.Close();
    delete wait;    
    return KErrNone;

    }

//Test Asynchronous CreateTriggerIterator and GetNextTriggerLC
// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetNextTriggerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::testGetNextTrigger2L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  	
    
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetManagerUi(TUid::Uid(4));
    
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    notifier->CreateIterator(mgrclient,wait);
    wait->Start( );
    RArray<CLbtTriggerInfo> triggers;
    //Get triggers. Note, the loop will block 
    //active schedular
    CLbtTriggerInfo* trigInfo = mgrclient.GetNextTriggerLC(); 
    TInt i=1;
    while ( trigInfo != NULL )
        {
        	i++;
          triggers.Append(*trigInfo);
        //  CleanupStack::PopAndDestroy(trigInfo);
          trigInfo = mgrclient.GetNextTriggerLC(); 
        }
          
    //CLbtTriggerEntry* entry=trigInfo->TriggerEntry();
      
    AssertTrueL( trigId==triggers[i-2].TriggerEntry()->Id(),_L("Wrong id"));    
        
    CleanupStack::PopAndDestroy(i);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggers.Close();
    delete wait;    
    return KErrNone;

    }
//Test RlbtManager::CreateTrigger
TInt Ct_mgmtsettings::TestCreatetriggerL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
      //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( mgrclient,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtTriggerInfo* trigger=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry=trigger->TriggerEntry();
    
    AssertTrueL( trigId==trigEntry->Id(),_L("Wrong Name"));
    
    CleanupStack::PopAndDestroy(trigger);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    delete wait;
        
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetTrigger1L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test RLbtManager::GetTriggerLC For Cellbased triggers
TInt Ct_mgmtsettings::Cell_GetTrigger1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    
    trig->SetCondition(condition); // ownership transferred to object
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
    TCoordinate coordinate;    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtTriggerInfo* trigger=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry=trigger->TriggerEntry();
    
    AssertTrueL( trigId==trigEntry->Id(),_L("Wrong Name"));
    
    CleanupStack::PopAndDestroy(trigger);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    delete wait;
        
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::UpdateTriggerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test RLbtmanager::UpdateTriggerL for Cell based triggers
TInt Ct_mgmtsettings::Cell_UpdateTriggerL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    
    trig->SetCondition(condition); // ownership transferred to object
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
    TCoordinate coordinate;    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetId(trigId);
 //   TLbtTriggerAttributeFieldsMask  aFieldMask = KLbtTriggerAttributeFieldsAll ;    
    mgrclient.UpdateTriggerL(*trig,CLbtTriggerEntry::EAttributeState,ELbtTrue);
   // mgrclient.UpdateTriggerL(*trig,aFieldMask);
    
    CLbtTriggerInfo* trigger=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry=trigger->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateDisabled==trigEntry->State(),_L("Wrong Name"));    
        
    trig->SetNameL(_L("Trigger01"));
    
    mgrclient.UpdateTriggerL(*trig,CLbtTriggerEntry::EAttributeName,ELbtTrue);
   
    CleanupStack::PopAndDestroy(trigger);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    delete wait;
        
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetTriggerState1L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test RLbtmanager::SetTriggerStateL for Cell based triggers
TInt Ct_mgmtsettings::Cell_SetTriggerState1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    
    trig->SetCondition(condition); // ownership transferred to object
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;     
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    mgrclient.SetTriggerStateL(trigId,CLbtTriggerEntry::EStateDisabled,ELbtTrue);
    
    CLbtTriggerInfo* trigger1=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry1=trigger1->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateDisabled==trigEntry1->State(),_L("Wrong state"));
       
    
    mgrclient.SetTriggerStateL(trigId,CLbtTriggerEntry::EStateEnabled,ELbtTrue);
    
    CLbtTriggerInfo* trigger2=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry2=trigger2->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateEnabled==trigEntry2->State(),_L("Wrong state"));
       
    
    CleanupStack::PopAndDestroy(2);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    delete wait;
        
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetTriggerState2L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test RLbtmanager::SetTriggersStateL for Cell based triggers using filter
TInt Ct_mgmtsettings::Cell_SetTriggerState2L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;     
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtTriggerFilterByAttribute* attribFilter1=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( attribFilter1 );    
    
    attribFilter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    attribFilter1->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    attribFilter1->AddTriggerManagerUiL(TUid::Uid(4));
    
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
    mgrclient.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,compositeFilter);
    
    CLbtTriggerInfo* trigger1=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry1=trigger1->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateEnabled==trigEntry1->State(),_L("Wrong state"));
       
    
        
    CleanupStack::PopAndDestroy(trigger1);
    CleanupStack::Pop(compositeFilter);
    CleanupStack::Pop(areaFilter);
    CleanupStack::Pop(attribFilter1);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    delete wait;
        
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetTriggerState3L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test RLbtmanager::SetTriggersStateL for Cell based triggers using NULL filter
TInt Ct_mgmtsettings::Cell_SetTriggerState3L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
     TCoordinate coordinate;         
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtTriggerFilterByAttribute* filter=NULL;
    
    mgrclient.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,filter);
    
    CLbtTriggerInfo* trigger1=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry1=trigger1->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateEnabled==trigEntry1->State(),_L("Wrong state"));
  
        
    CleanupStack::PopAndDestroy(trigger1);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
        
    delete wait;
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetTriggerState4L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test Asynchronous RLbtmanager::SetTriggersState for Cell based triggers using filter
TInt Ct_mgmtsettings::Cell_SetTriggerState4L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;     
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtTriggerFilterByAttribute* attribFilter1=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( attribFilter1 );    
    
    attribFilter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    attribFilter1->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    attribFilter1->AddTriggerManagerUiL(TUid::Uid(4));
    
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
    
    CAoOperation* ao=CAoOperation::NewL(); 
    CleanupStack::PushL( ao ); 
    
    ao->SetTriggerState(mgrclient,CLbtTriggerEntry::EStateEnabled,compositeFilter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    
    CLbtTriggerInfo* trigger1=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry1=trigger1->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateEnabled==trigEntry1->State(),_L("Wrong state"));
       
    
    CleanupStack::PopAndDestroy(trigger1);
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::Pop(compositeFilter);
    CleanupStack::Pop(areaFilter);
    CleanupStack::Pop(attribFilter1);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
        
    delete wait;
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::SetTriggerState5L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test Asynchronous RLbtmanager::SetTriggersState for Cell based triggers using NULL filter
TInt Ct_mgmtsettings::Cell_SetTriggerState5L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;       
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
     CLbtTriggerFilterByAttribute* filter=NULL;
    
    CAoOperation* ao=CAoOperation::NewL(); 
    CleanupStack::PushL( ao ); 
    
    ao->SetTriggerState(mgrclient,CLbtTriggerEntry::EStateEnabled,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    
    CLbtTriggerInfo* trigger1=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry1=trigger1->TriggerEntry();
    
    AssertTrueL( CLbtTriggerEntry::EStateEnabled==trigEntry1->State(),_L("Wrong state"));
       
    
    CleanupStack::PopAndDestroy(trigger1);
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    delete wait;    
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::ListTriggerId1L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test RLbtmanager::ListTriggerIdsL for Cell based triggers using filter
TInt Ct_mgmtsettings::Cell_ListTriggerId1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;       
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    
    listoptions->SetRetrievedFields( CLbtTriggerEntry::EAttributeState |  
                                     CLbtTriggerEntry::EAttributeName | 
                                     CLbtTriggerEntry::EAttributeManagerUi
                                   );
    
    CLbtTriggerFilterByAttribute* attribFilter1=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( attribFilter1 );    
    
    attribFilter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    attribFilter1->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    attribFilter1->AddTriggerManagerUiL(TUid::Uid(4));
    
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
    
    RArray < TLbtTriggerId > triggerIdList;    
    
    
    
    listoptions->SetFilter(compositeFilter);
    
    mgrclient.ListTriggerIdsL(triggerIdList,listoptions);
    
    AssertTrueL( trigId==triggerIdList[0],_L("Wrong trigger"));
    
    
    CleanupStack::Pop(compositeFilter);
    CleanupStack::Pop(areaFilter);
    CleanupStack::Pop(attribFilter1);
    CleanupStack::PopAndDestroy(listoptions);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerIdList.Close();
    delete wait;    
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::ListTriggerId2L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test RLbtmanager::ListTriggerIdsL for Cell based triggers using NULL filter
TInt Ct_mgmtsettings::Cell_ListTriggerId2L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  	TRAP_IGNORE(lbt.DeleteTriggersL());
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;       
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
       
    RArray < TLbtTriggerId > triggerIdList;    
           
    mgrclient.ListTriggerIdsL(triggerIdList);
    
    AssertTrueL( trigId==triggerIdList[0],_L("Wrong trigger"));
    
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerIdList.Close();
    delete wait;    
    return KErrNone;

    }



// -----------------------------------------------------------------------------
// Ct_mgmtsettings::ListTriggerId3L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test Asynchronous RLbtmanager::ListTriggerIdsL for Cell based triggers using filter
TInt Ct_mgmtsettings::Cell_ListTriggerId3L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;       
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    
    listoptions->SetRetrievedFields( CLbtTriggerEntry::EAttributeState |  
                                     CLbtTriggerEntry::EAttributeName | 
                                     CLbtTriggerEntry::EAttributeManagerUi
                                   );
    
    CLbtTriggerFilterByAttribute* attribFilter1=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( attribFilter1 );    
    
    attribFilter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    attribFilter1->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    attribFilter1->AddTriggerManagerUiL(TUid::Uid(4));
    
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
    
    RArray < TLbtTriggerId > triggerIdList;    
    
    listoptions->SetFilter(compositeFilter);
    
    CAoOperation* ao=CAoOperation::NewL();
    CleanupStack::PushL( ao );
    
    ao->ListTriggerIds(mgrclient,triggerIdList,listoptions,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    AssertTrueL( trigId==triggerIdList[0],_L("Wrong trigger"));
    
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::Pop(compositeFilter);
    CleanupStack::Pop(areaFilter);
    CleanupStack::Pop(attribFilter1);
    CleanupStack::PopAndDestroy(listoptions);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerIdList.Close();
    delete wait;    
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::ListTriggerId4L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
///Test Asynchronous RLbtmanager::ListTriggerIdsL for Cell based triggers using NULL filter
TInt Ct_mgmtsettings::Cell_ListTriggerId4L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
  	TRAP_IGNORE(lbt.DeleteTriggersL());
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;       
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
       
    RArray < TLbtTriggerId > triggerIdList;    
    
 
    CAoOperation* ao=CAoOperation::NewL();
    CleanupStack::PushL( ao );
    
    ao->ListTriggerIds(mgrclient,triggerIdList,NULL,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    AssertTrueL( trigId==triggerIdList[0],_L("Wrong trigger"));
    
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerIdList.Close();
    delete wait;    
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetTriggers1L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test  RLbtmanager::GetTriggerssL for Cell based triggers using filter
TInt Ct_mgmtsettings::Cell_GetTriggers1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;       
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    
    listoptions->SetRetrievedFields( CLbtTriggerEntry::EAttributeState |  
                                     CLbtTriggerEntry::EAttributeName | 
                                     CLbtTriggerEntry::EAttributeManagerUi
                                   );
    
    CLbtTriggerFilterByAttribute* attribFilter1=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( attribFilter1 );    
    
    attribFilter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    attribFilter1->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    attribFilter1->AddTriggerManagerUiL(TUid::Uid(4));
    
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
    
    RPointerArray< CLbtTriggerInfo > triggerList;    
    
    listoptions->SetFilter(compositeFilter);
    
    mgrclient.GetTriggersL(triggerList,listoptions);
    
    CLbtTriggerEntry* entry=triggerList[0]->TriggerEntry();
      
    AssertTrueL( trigId==entry->Id(),_L("Wrong id"));
    
    
    CleanupStack::Pop(compositeFilter);
    CleanupStack::Pop(areaFilter);
    CleanupStack::Pop(attribFilter1);
    CleanupStack::PopAndDestroy(listoptions);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerList.Close();
    delete wait;    
    return KErrNone;

    }


// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetTriggers2L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test  RLbtmanager::GetTriggersLL for Cell based triggers using NULL filter
TInt Ct_mgmtsettings::Cell_GetTriggers2L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    RPointerArray< CLbtTriggerInfo > triggerList;    
    
    mgrclient.GetTriggersL(triggerList);
    
    CLbtTriggerEntry* entry=triggerList[0]->TriggerEntry();
      
    AssertTrueL( trigId==entry->Id(),_L("Wrong id"));
       
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerList.Close();
    delete wait;    
    return KErrNone;

    }



// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetTriggers3L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test Asynchronous RLbtmanager::GetTriggers for Cell based triggers using filter
TInt Ct_mgmtsettings::Cell_GetTriggers3L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    
    listoptions->SetRetrievedFields( CLbtTriggerEntry::EAttributeState |  
                                     CLbtTriggerEntry::EAttributeName | 
                                     CLbtTriggerEntry::EAttributeManagerUi
                                   );
    
    CLbtTriggerFilterByAttribute* attribFilter1=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( attribFilter1 );    
    
    attribFilter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    attribFilter1->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    attribFilter1->AddTriggerManagerUiL(TUid::Uid(4));
    
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
    RPointerArray< CLbtTriggerInfo > triggerList;    
    
   
    listoptions->SetFilter(compositeFilter);
    
    CAoOperation* ao=CAoOperation::NewL();
    CleanupStack::PushL( ao );
    
    ao->GetTriggers( mgrclient,triggerList,listoptions,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    
    CLbtTriggerEntry* entry=triggerList[0]->TriggerEntry();
      
    AssertTrueL( trigId==entry->Id(),_L("Wrong id"));
    
    
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::Pop(compositeFilter);
    CleanupStack::Pop(areaFilter);
    CleanupStack::Pop(attribFilter1);
    CleanupStack::PopAndDestroy(listoptions);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerList.Close();
    delete wait;    
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetTriggers4L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test Asynchronous RLbtmanager::GetTriggers for Cell based triggers using NULL filter
TInt Ct_mgmtsettings::Cell_GetTriggers4L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
			 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    RPointerArray< CLbtTriggerInfo > triggerList;    
    
       
    CAoOperation* ao=CAoOperation::NewL();
    CleanupStack::PushL( ao );
    
    ao->GetTriggers( mgrclient,triggerList,NULL,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==ao->iCompletionCode,_L("Wrong error code"));
    
    CLbtTriggerEntry* entry=triggerList[0]->TriggerEntry();
      
    AssertTrueL( trigId==entry->Id(),_L("Wrong id"));
    
    
    CleanupStack::PopAndDestroy(ao);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggerList.Close();
    delete wait;    
    return KErrNone;

    }


//Test case for testing Delete TriggerL
// -----------------------------------------------------------------------------
// Ct_mgmtsettings::TestDeleteTriggerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Test Asynchronous RLbtmanager::DeleteTriggerL for Cell based trigger
TInt Ct_mgmtsettings::Cell_TestDeleteTriggerL( CStifItemParser& /* aItem */ )
    {

    RLbtServer lbtserver;
	 RLbtManager lbtmanager;
	  	 
	 User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
	 User::LeaveIfError( lbtmanager.Open( lbtserver ) );
	 CleanupClosePushL( lbtmanager );
	 
///*	 
_LIT( KFilename,"CreateTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt triggerId=0;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        KPSUidTriggerIdInfo, 
	        KLbttesttriggerid, 
	        triggerId));
	        //*/
	 
	 //Construct a session trigger
   CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
   
   //Push to cleanup stack
   CleanupStack::PushL( trig );
       
    // Set Name
   trig->SetNameL(_L("Trigger1"));
   trig->SetState( CLbtTriggerEntry::EStateDisabled );
         
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
    
  	 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
   TCoordinate coordinate;
   CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbtmanager,coordinate,0);
   CleanupStack::PushL( notifier );
   
   CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       
   TLbtTriggerId trigId;
   notifier->CreateTriggers( lbtmanager,*trig,trigId,ETrue,wait );
   wait->Start( );
   
   lbtmanager.DeleteTriggerL(trigId);
   lbtmanager.DeleteTriggerL(triggerId);
   CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( trig );   
  // CLbtTriggerInfo* trigEntry=NULL;
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(););
   if(error1==KErrNotFound)
   {
   TRAPD(error2, lbtmanager.GetTriggerLC(triggerId);CleanupStack::PopAndDestroy(););
   if(error2==KErrNotFound)
	   {
      CleanupStack::PopAndDestroy( &lbtmanager );
      CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return KErrNone;
	   }
   else
	   {
	   CleanupStack::PopAndDestroy( &lbtmanager );
	      CleanupStack::PopAndDestroy( &lbtserver );
	      delete wait; 
	      return KErrNone;
	   }
   }
   else
   {
   CleanupStack::PopAndDestroy( &lbtmanager );
   CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return -99;	
   }
  }

//Test RLbtManager::DeleteTriggersL for Cellbased trigger
TInt Ct_mgmtsettings::Cell_TestDeleteTriggers1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer lbtserver;
	 RLbtManager lbtmanager;
	  	 
	 User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
	 User::LeaveIfError( lbtmanager.Open( lbtserver ) );
	 CleanupClosePushL( lbtmanager );
	 
///*	
 _LIT( KFilename,"CreateTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt triggerId=0;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        KPSUidTriggerIdInfo, 
	        KLbttesttriggerid, 
	        triggerId));
	        //*/
	 
	 //Construct a session trigger
   CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
   
   //Push to cleanup stack
   CleanupStack::PushL( trig );
       
    // Set Name
   trig->SetNameL(_L("Trigger1"));
   trig->SetState( CLbtTriggerEntry::EStateDisabled );
         
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
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
  
   TCoordinate coordinate;
   CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbtmanager,coordinate,0);
   CleanupStack::PushL( notifier );
   
   CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       
   TLbtTriggerId trigId;
   notifier->CreateTriggers( lbtmanager,*trig,trigId,ETrue,wait );
   wait->Start( );
   CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( trig );   
   lbtmanager.DeleteTriggersL();
   
  // CLbtTriggerInfo* trigEntry=NULL;
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(););
   if(error1==KErrNotFound)
   {
   TRAPD(error2, lbtmanager.GetTriggerLC(triggerId);CleanupStack::PopAndDestroy(););
   if(error2==KErrNotFound)
	   {
      CleanupStack::PopAndDestroy( &lbtmanager );
      CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return KErrNone;
	   }
   else
	   {
	   CleanupStack::PopAndDestroy( &lbtmanager );
	      CleanupStack::PopAndDestroy( &lbtserver );
	      delete wait; 
	      return KErrNone;
	   }
   }
   else
   {
   CleanupStack::PopAndDestroy( &lbtmanager );
   CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return -99;	
   }
  }


//Test Asynchronous RLbtManager::DeleteTriggers for Cellbased trigger
TInt Ct_mgmtsettings::Cell_TestDeleteTriggers2L( CStifItemParser& /* aItem */ )
    {

    RLbtServer lbtserver;
	 RLbtManager lbtmanager;
	  	 
	 User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
	 User::LeaveIfError( lbtmanager.Open( lbtserver ) );
	 CleanupClosePushL( lbtmanager );
	 
//	/* 
	_LIT( KFilename,"CreateTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt triggerId=0;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        KPSUidTriggerIdInfo, 
	        KLbttesttriggerid, 
	        triggerId));
//	 */
	 //Construct a session trigger
   CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
   
   //Push to cleanup stack
   CleanupStack::PushL( trig );
       
    // Set Name
   trig->SetNameL(_L("Trigger1"));
   trig->SetState( CLbtTriggerEntry::EStateDisabled );
         
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
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
  
   TCoordinate coordinate;
   CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbtmanager,coordinate,0);
   CleanupStack::PushL( notifier );
   
   CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       
   TLbtTriggerId trigId;
   notifier->CreateTriggers( lbtmanager,*trig,trigId,ETrue,wait );
   wait->Start( );
   
   notifier->DeleteAllTriggers(lbtmanager,wait);
   wait->Start();
   CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( trig );   
  // CLbtTriggerInfo* trigEntry=NULL;
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(););
   if(error1==KErrNotFound)
   {
   TRAPD(error2, lbtmanager.GetTriggerLC(triggerId);CleanupStack::PopAndDestroy(););
   if(error2==KErrNotFound)
	   {
      CleanupStack::PopAndDestroy( &lbtmanager );
      CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return KErrNone;
	   }
   else
	   {
	   CleanupStack::PopAndDestroy( &lbtmanager );
	      CleanupStack::PopAndDestroy( &lbtserver );
	      delete wait; 
	      return KErrNone;
	   }
   }
   else
   {
   CleanupStack::PopAndDestroy( &lbtmanager );
   CleanupStack::PopAndDestroy( &lbtserver );
 
   delete wait; 
   return -99;	
   }
  }


//Test CreateTriggerIterator and GetNextTriggerLC for Cell based triggers
// -----------------------------------------------------------------------------
// Ct_mgmtsettings::testGetNextTriggerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::Cell_testGetNextTrigger1L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
     TCoordinate coordinate;    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    mgrclient.CreateGetTriggerIteratorL();
    
    RArray<CLbtTriggerInfo> triggers;
    //Get triggers. Note, the loop will block 
    //active schedular
    TInt i=1;
    CLbtTriggerInfo* trigInfo = mgrclient.GetNextTriggerLC(); 
    while ( trigInfo != NULL )
        {
        	i++;
          triggers.Append(*trigInfo);
         // CleanupStack::PopAndDestroy(trigInfo);
          trigInfo = mgrclient.GetNextTriggerLC(); 
        }
          
  //  CLbtTriggerEntry* entry=triggers[0].TriggerEntry();
      
    AssertTrueL( trigId==triggers[i-2].TriggerEntry()->Id(),_L("Wrong id"));    
        
    CleanupStack::PopAndDestroy(i);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggers.Close();
    delete wait;    
    return KErrNone;

    }

//Test Asynchronous CreateTriggerIterator and GetNextTriggerLC for Cell based triggers
// -----------------------------------------------------------------------------
// Ct_mgmtsettings::GetNextTriggerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_mgmtsettings::Cell_testGetNextTrigger2L( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
      TCoordinate coordinate;   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    notifier->CreateIterator(mgrclient,wait);
    wait->Start( );
    RArray<CLbtTriggerInfo> triggers;
    //Get triggers. Note, the loop will block 
    //active schedular
    TInt i=1;
    CLbtTriggerInfo* trigInfo = mgrclient.GetNextTriggerLC(); 
    while ( trigInfo != NULL )
        {
        	i++;
          triggers.Append(*trigInfo);
         
          trigInfo = mgrclient.GetNextTriggerLC(); 
        }
          
    CLbtTriggerEntry* entry=triggers[0].TriggerEntry();
      
    AssertTrueL( trigId==triggers[i-2].TriggerEntry()->Id(),_L("Wrong id"));    
        
     CleanupStack::PopAndDestroy(i);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggers.Close();
    delete wait;    
    return KErrNone;

    }
//Test RlbtManager::CreateTrigger for cell based trigger
TInt Ct_mgmtsettings::Cell_TestCreatetriggerL( CStifItemParser& /* aItem */ )
    {

    RLbtServer server;
    RLbt lbt;
    RLbtManager mgrclient;
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    User::LeaveIfError(mgrclient.Open(server));
    CleanupClosePushL( mgrclient );
  
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
    
    
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    
    trig->SetCondition(condition); // ownership transferred to object
		 //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
            
    TCoordinate coordinate ;  
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
  
    CLbtTriggerInfo* trigger=mgrclient.GetTriggerLC(trigId);
    CLbtTriggerEntry* trigEntry=trigger->TriggerEntry();
    
    AssertTrueL( trigId==trigEntry->Id(),_L("Wrong Name"));
    
    CleanupStack::PopAndDestroy(trigger);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    delete wait;
        
    return KErrNone;

    }
    
    
    
    
    TInt Ct_mgmtsettings::TC_LBT015_01_testL( CStifItemParser& /* aItem */ )
    {

	 	 
	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	 TLbtTriggeringSystemSettings settings;
 	 lbt.GetTriggeringSystemSettingsL( settings );
 	// /*
 	 AssertTrueL( settings.MinimumTriggerAreaSize( )==100,_L(" Wrong Minimum trigger area" ) );
 	 AssertTrueL( settings.TriggeringMechanismState ( )==1,_L(" Wrong triggering state" ) );
 //	 */
 	 CleanupStack::PopAndDestroy( &lbt );
     CleanupStack::PopAndDestroy( &lbtserver );
     return KErrNone;
	 
    }

TInt Ct_mgmtsettings::TC_LBT015_02_testL( CStifItemParser& /* aItem */ )
    {

	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	 TLbtTriggeringSystemSettings settings;
 	 lbt.GetTriggeringSystemSettingsL( settings );
 	 TReal minArea=settings.MinimumTriggerAreaSize( );
 	 
 	 
       //Construct a session trigger
     CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
      //Push to cleanup stack
     CleanupStack::PushL( trig );
    
     // Set Name
     trig->SetNameL(_L("Trigger1"));
    
       //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData); 
     // set condition
       
     TCoordinate coordinate(61.5285,23.9385);
     TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );      
     CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,minArea-100);
     CleanupStack::PushL( circle );
    
         
     // ownership of circle object transferred to the condition object
     CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
     CleanupStack::Pop( circle );
    
     trig->SetCondition(condition); // ownership transferred to object

     TLbtTriggerId trigId;
        
        
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,minArea-100 );
     CleanupStack::PushL( notifier );
    
     CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
     notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
     wait->Start( );
    
     TInt err=notifier->iCompletionCode;
     AssertTrueL( KErrTriggerAreaTooSmall==err,_L("Wrong error code")); 
     
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy( trig );   
 	 CleanupStack::PopAndDestroy( &lbt );
     CleanupStack::PopAndDestroy( &lbtserver );
     return KErrNone;
	 
    }
 TInt Ct_mgmtsettings::TC_LBT015_03_testL( CStifItemParser& /* aItem */ )
    {

	 	 
	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 RLbtManager mgrclient;
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 User::LeaveIfError(mgrclient.Open(lbtserver));
    CleanupClosePushL( mgrclient );
 	 
     TCoordinate coordinate;
     TLbtTriggeringSystemSettings settings,dummySettings;
     TLbtTriggeringSystemManagementSettings settings2;
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,0 );
     CleanupStack::PushL( notifier );
     TRAP_IGNORE(lbt.DeleteTriggersL());
     CActiveSchedulerWait *waitLoop = new(ELeave) CActiveSchedulerWait;
     dummySettings.SetTriggeringMechanismState(ETriggeringMechanismOff);
     settings2.SetTriggeringMechanismState(ETriggeringMechanismOff);
     notifier->NotifyTriggeringSettingsChange( lbt,settings,dummySettings,waitLoop ); 
     mgrclient.SetTriggeringSystemSettingsL(settings2,ETriggeringMechanismState);
     
     waitLoop->Start();
	  
	 delete waitLoop;         
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy(&mgrclient);
     CleanupStack::PopAndDestroy( &lbt );
     CleanupStack::PopAndDestroy( &lbtserver );
     return KErrNone;
	 
    }
    
    TInt Ct_mgmtsettings::TC_LBT015_04_testL( CStifItemParser& /* aItem */ )
    {

	 	 
	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
     TCoordinate coordinate;
     TLbtTriggeringSystemSettings settings,dummySettings;
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,500 );
     CleanupStack::PushL( notifier );
     
     CActiveSchedulerWait *waitLoop = new(ELeave) CActiveSchedulerWait;
     notifier->NotifyTriggeringSettingsChange( lbt,settings,dummySettings,waitLoop ); 
     notifier->CancelNotifyTriggeringSettingsChange(lbt);
     
     waitLoop->Start();
          
     TInt err=notifier->iCompletionCode;
     AssertTrueL( KErrCancel==err,_L("Wrong error code"));  
        
    // notifier->CancelNotifyTriggeringSettingsChange(lbt);
     
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy( &lbt );
     CleanupStack::PopAndDestroy( &lbtserver );
     return KErrNone;
	 
    } 

TInt Ct_mgmtsettings::TC_LBT015_05_testL( CStifItemParser& /* aItem */ )
    {

	     // Simulation filename
	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 RLbtManager mgrclient;
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 User::LeaveIfError(mgrclient.Open(lbtserver));
    CleanupClosePushL( mgrclient );
 	
 	 CRepository* repository1 = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository1->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository1);
	 
	 // Set Triggering state to Off using LT Management API: TO-DO
	 TLbtTriggeringSystemSettings settings,dummySettings;
     TLbtTriggeringSystemManagementSettings settings2;
     
     
     dummySettings.SetTriggeringMechanismState(ETriggeringMechanismOff);
     settings2.SetTriggeringMechanismState(ETriggeringMechanismOff);
     mgrclient.SetTriggeringSystemSettingsL(settings2,ETriggeringMechanismState);
	
	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
       
    // set condition
    
    TCoordinate coordinate(61.5285,23.9385);
    TInt err=coordinate.Move(90,118000);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    notifier->StartNotification( wait );
    notifier->After(120000000);
    wait->Start( );
    
	     if(notifier->iTriggerFired)
	     {
	     	_LIT(KMessage,"Trigger fired");
	     	iLog->Log( KMessage );
	        User::LeaveIfError(KErrGeneral);
	     }
    
    CleanupStack::PopAndDestroy( notifier );
   
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
      
    }
    
    
    //Deleting a trigger from another client  - leave with -1
	TInt Ct_mgmtsettings::TestdeleteTrigger4L( CStifItemParser& /* aItem */ )
    {

    RLbtServer lbtserver;
	RLbtManager lbtmanager;
	  
	  RLbt lbt;	 
	 User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
	 User::LeaveIfError( lbtmanager.Open( lbtserver ) );
	 CleanupClosePushL( lbtmanager );
	 User::LeaveIfError( lbt.Open( lbtserver ) );
	 CleanupClosePushL( lbt );
	 
///*	 
_LIT( KFilename,"CreateTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt triggerId=0;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        KPSUidTriggerIdInfo, 
	        KLbttesttriggerid, 
	        triggerId));
	// */
	 //Construct a session trigger
   CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
   
   //Push to cleanup stack
   CleanupStack::PushL( trig );
       
    // Set Name
   trig->SetNameL(_L("Trigger1"));
   trig->SetState( CLbtTriggerEntry::EStateDisabled );
         
   //set Requestor     
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
    
  
  
   CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbtmanager,coordinate,0);
   CleanupStack::PushL( notifier );
   
   CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       
   TLbtTriggerId trigId;
   notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
   wait->Start( );
   
   //lbtmanager.DeleteTriggerL(trigId);
   lbt.DeleteTriggerL(triggerId);
   CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
   
//   CLbtTriggerInfo* trigEntry=NULL;
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(););
   if(error1==KErrNone)
   {
   TRAPD(error2, lbtmanager.GetTriggerLC(triggerId);CleanupStack::PopAndDestroy(););
   if(error2==KErrNone)
	   {
	   CleanupStack::PopAndDestroy( &lbt );
      CleanupStack::PopAndDestroy( &lbtmanager );
      CleanupStack::PopAndDestroy( &lbtserver );
 
 //  delete wait; 
   return KErrNone;
	   }
   else
	   {
	      CleanupStack::PopAndDestroy( &lbt );
	   	  CleanupStack::PopAndDestroy( &lbtmanager );
	      CleanupStack::PopAndDestroy( &lbtserver );
	   //   delete wait; 
	      return KErrNone;
	   }
   }
   else
   {
   CleanupStack::PopAndDestroy( &lbt );
   CleanupStack::PopAndDestroy( &lbtmanager );
   CleanupStack::PopAndDestroy( &lbtserver );
 
  // delete wait; 
   return KErrGeneral;	
   }
  }
  
  
  
  //Deletin triggers from other client
  TInt Ct_mgmtsettings::TestdeleteTrigger5L( CStifItemParser& /* aItem */ )
    {

    RLbtServer lbtserver;
	 RLbtManager lbtmanager;
	 RLbt lbt;
	  	 
	 User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
	 User::LeaveIfError( lbtmanager.Open( lbtserver ) );
	 CleanupClosePushL( lbtmanager );
	 User::LeaveIfError( lbt.Open( lbtserver ) );
	 CleanupClosePushL( lbt );
//	/* 
	_LIT( KFilename,"CreateTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt triggerId=0;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        KPSUidTriggerIdInfo, 
	        KLbttesttriggerid, 
	        triggerId));
	        //*/
	 
	 //Construct a session trigger
   CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
   
   //Push to cleanup stack
   CleanupStack::PushL( trig );
       
    // Set Name
   trig->SetNameL(_L("Trigger1"));
   trig->SetState( CLbtTriggerEntry::EStateDisabled );
         
   //set Requestor     
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
   
   lbt.DeleteTriggersL();
//   wait->Start();
   
   CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( trig );   
   CLbtTriggerInfo* trigEntry=NULL;
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(););
   if(error1==KErrNotFound)
   {
   trigEntry=lbtmanager.GetTriggerLC(triggerId);
   if(trigEntry!=NULL)
	   {
	  CleanupStack::PopAndDestroy( trigEntry );
	   CleanupStack::PopAndDestroy( &lbt );
      CleanupStack::PopAndDestroy( &lbtmanager );
      CleanupStack::PopAndDestroy( &lbtserver );
 
   //delete wait; 
   return KErrNone;
	   }
   else
	   {
	   CleanupStack::PopAndDestroy( &lbt );
	   CleanupStack::PopAndDestroy( &lbtmanager );
	      CleanupStack::PopAndDestroy( &lbtserver );
	     // delete wait; 
	      return -99;
	   }
   }
   else
   {
   CleanupStack::PopAndDestroy( &lbt );
   CleanupStack::PopAndDestroy( &lbtmanager );
   CleanupStack::PopAndDestroy( &lbtserver );
 
   //delete wait; 
   return -99;	
   }
  }
  
  
   //Test trigger change notification for management client when trigger is created  
    
    TInt Ct_mgmtsettings::TestNotifyTriggerChangeEventL( CStifItemParser& aItem )
        {

       		RLbtServer server;
    	    RLbt lbt;
    	    RLbtManager mgrclient;
    	        
    	    User::LeaveIfError(server.Connect());
    	    CleanupClosePushL( server );
    	    
    	    User::LeaveIfError(lbt.Open(server));
    	    CleanupClosePushL( lbt );
    	    
    	    User::LeaveIfError(mgrclient.Open(server));
    	    CleanupClosePushL( mgrclient );
    	    
    	    TRAP_IGNORE(mgrclient.DeleteTriggersL()); 
    	    
    	    CLbtEventObserver* notifier1= CLbtEventObserver::NewL( lbt );
    	    CleanupStack::PushL( notifier1 );
    	    //notifier->After(50000000); 
    	    
    	    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    	    TLbtTriggerChangeEvent   aEvent ;
    	    notifier1->NotifyTriggerChangeEvent(mgrclient,aEvent,wait);
    	  
    	    _LIT( KFilename,"CreateTest.exe" );
    	    RProcess proc;
    	    TInt retVal =proc.Create(KFilename,KNullDesC);
    	    if(retVal == KErrNone)
    		{
    			proc.Resume();
    			TRequestStatus status = KRequestPending;
    			proc.Rendezvous(status);
    			User::WaitForRequest(status);
    		}	
    	    TInt triggerId=0;
    	    RProperty iProperty;
    	    User::LeaveIfError(iProperty.Get(
    	        KPSUidTriggerIdInfo, 
    	        KLbttesttriggerid, 
    	        triggerId));
    	    wait->Start( );
    	   // notifier->After(50000000);
    	    TLbtTriggeringSystemStatus TriggeringSystemStatus;
    	    
    	    
    	    
    	    	CleanupStack::PopAndDestroy(notifier1);
    		    CleanupStack::PopAndDestroy(&mgrclient);
    		    CleanupStack::PopAndDestroy(&lbt);
    		    CleanupStack::PopAndDestroy(&server);
    		  //  delete wait;
    		    return KErrNone;
    	    

        }

//Test RlbtManager::CancelCreateTrigger()

// -----------------------------------------------------------------------------
// Ct_mgmtsettings::AssertTrueL
// function for condition check.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void Ct_mgmtsettings::AssertTrueL( TBool aCondition,const TDesC& aMessage )
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
