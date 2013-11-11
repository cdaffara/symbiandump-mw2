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
* Description:    Test module to implement Test cases for BC
*
*/





// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "AsynOperationAO.h"
#include <centralrepository.h>
#include <SimulationPSYInternalCRKeys.h>


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
#include <lbscommon.h>
#include <LbsRequestor.h>
#include <LbsPosition.h>
#include <LbsPositionInfo.h>
#include <lbs.h>
#include <e32property.h>
#include "lbtbctest.h"
#include "MyObserver.h"
#include "testlbtclasses.h"

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
// Clbtbctest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Clbtbctest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Clbtbctest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Clbtbctest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "LBTBCTest1", Clbtbctest::TestRlbtOpen1L ),
        ENTRY( "LBTBCTest2", Clbtbctest::TestRlbtOpen2L ),
        ENTRY( "LBTBCTest89", Clbtbctest::TestRlbtConnect1L ),
        ENTRY( "LBTBCTest3", Clbtbctest::TestRlbtCreateTriggerL ),
        ENTRY( "LBTBCTest3_2", Clbtbctest::TestRlbtCreateTrigger2L ),
        ENTRY( "LBTBCTest3_3", Clbtbctest::TestRlbtCreateTrigger3L ),
        ENTRY( "LBTBCTest3_4", Clbtbctest::TestRlbtCreateTrigger4L ),
        ENTRY( "LBTBCTest90", Clbtbctest::TestRlbtCreateTrigger5L ),
        ENTRY( "LBTBCTest4", Clbtbctest::TestRlbtCancelCreateTriggerL ),
        ENTRY( "LBTBCTest5", Clbtbctest::TestRlbtDeleteTriggerL ),
        ENTRY( "LBTBCTest6", Clbtbctest::TestRlbtDeleteTriggers1L ),
        ENTRY( "LBTBCTest7", Clbtbctest::TestRlbtDeleteTriggers2L ),
        ENTRY( "LBTBCTest8", Clbtbctest::TestRlbtDeleteTriggers3L ),
        ENTRY( "LBTBCTest9", Clbtbctest::TestRlbtDeleteTriggers4L ),
        ENTRY( "LBTBCTest9_2", Clbtbctest::TestRlbtDeleteTriggers5L ),
        ENTRY( "LBTBCTest9_3", Clbtbctest::TestRlbtDeleteTriggers6L ),
        ENTRY( "LBTBCTest9_4", Clbtbctest::TestRlbtDeleteTriggers7L ),
        ENTRY( "LBTBCTest9_5", Clbtbctest::TestRlbtDeleteTriggers8L ),
        ENTRY( "LBTBCTest10", Clbtbctest::TestRlbtCancelDeleteTriggersL ),
        ENTRY( "LBTBCTest11", Clbtbctest::TestRlbtGetTriggerLCL ),
        ENTRY( "LBTBCTest12", Clbtbctest::TestRlbtUpdateTriggerL ),
        ENTRY( "LBTBCTest12_2", Clbtbctest::TestRlbtUpdateTrigger2L ),
        ENTRY( "LBTBCTest12_3", Clbtbctest::TestRlbtUpdateTrigger3L ),
        ENTRY( "LBTBCTest12_4", Clbtbctest::TestRlbtUpdateTrigger4L ),
        ENTRY( "LBTBCTest12_5", Clbtbctest::TestRlbtUpdateTrigger5L ),
        ENTRY( "LBTBCTest12_6", Clbtbctest::TestRlbtUpdateTrigger6L ),
        ENTRY( "LBTBCTest12_7", Clbtbctest::TestRlbtUpdateTrigger7L ),
        ENTRY( "LBTBCTest91", Clbtbctest::TestRlbtUpdateTrigger8L ),
        ENTRY( "LBTBCTest13", Clbtbctest::TestRlbtSetTriggerStateL ),
        ENTRY( "LBTBCTest14", Clbtbctest::TestRlbtSetTriggersState1L ),
        ENTRY( "LBTBCTest15", Clbtbctest::TestRlbtSetTriggersState2L ),
        ENTRY( "LBTBCTest15_2", Clbtbctest::TestRlbtSetTriggersState3L ),
        ENTRY( "LBTBCTest15_3", Clbtbctest::TestRlbtSetTriggersState4L ),
        ENTRY( "LBTBCTest15_4", Clbtbctest::TestRlbtSetTriggersState5L ),
        ENTRY( "LBTBCTest15_5", Clbtbctest::TestRlbtSetTriggersState6L ),
        ENTRY( "LBTBCTest15_6", Clbtbctest::TestRlbtSetTriggersState7L ),
        ENTRY( "LBTBCTest15_7", Clbtbctest::TestRlbtSetTriggersState8L ),
        ENTRY( "LBTBCTest16", Clbtbctest::TestRlbtCancelSetTriggersStateL ),
        ENTRY( "LBTBCTest17", Clbtbctest::TestRlbtListTriggerIds1L ),
        ENTRY( "LBTBCTest18", Clbtbctest::TestRlbtListTriggerIds2L ),
        ENTRY( "LBTBCTest18_2", Clbtbctest::TestRlbtListTriggerIds3L ),
        ENTRY( "LBTBCTest18_3", Clbtbctest::TestRlbtListTriggerIds4L ),
        ENTRY( "LBTBCTest18_4", Clbtbctest::TestRlbtListTriggerIds5L ),
        ENTRY( "LBTBCTest18_5", Clbtbctest::TestRlbtListTriggerIds6L ),
        ENTRY( "LBTBCTest18_6", Clbtbctest::TestRlbtListTriggerIds7L ),
        ENTRY( "LBTBCTest18_7", Clbtbctest::TestRlbtListTriggerIds8L ),
        ENTRY( "LBTBCTest19", Clbtbctest::TestRlbtCancelListTriggerIdsL ),
        ENTRY( "LBTBCTest20", Clbtbctest::TestRlbtGetTriggers1L ),
        ENTRY( "LBTBCTest21", Clbtbctest::TestRlbtGetTriggers2L ),
        ENTRY( "LBTBCTest21_2", Clbtbctest::TestRlbtGetTriggers3L ),
        ENTRY( "LBTBCTest21_3", Clbtbctest::TestRlbtGetTriggers4L ),
        ENTRY( "LBTBCTest21_4", Clbtbctest::TestRlbtGetTriggers5L ),
        ENTRY( "LBTBCTest21_5", Clbtbctest::TestRlbtGetTriggers6L ),
        ENTRY( "LBTBCTest21_6", Clbtbctest::TestRlbtGetTriggers7L ),
        ENTRY( "LBTBCTest22", Clbtbctest::TestRlbtCancelGetTriggersL ),
        ENTRY( "LBTBCTest23", Clbtbctest::TestRlbtCreateGetTriggerIterator1L ),
        ENTRY( "LBTBCTest24", Clbtbctest::TestRlbtCreateGetTriggerIterator2L ),
        ENTRY( "LBTBCTest92", Clbtbctest::TestRlbtCreateGetTriggerIterator3L ),
        ENTRY( "LBTBCTest25", Clbtbctest::TestRlbtCancelCreateTriggerIteratorL ),
        ENTRY( "LBTBCTest26", Clbtbctest::TestRlbtNotifyTriggerChangeEventL ),
        ENTRY( "LBTBCTest27", Clbtbctest::TestRlbtNotifyTriggerChangeEvent2L ),
        ENTRY( "LBTBCTest28", Clbtbctest::TestRlbtNotifyTriggerFiredL ),
        ENTRY( "LBTBCTest93", Clbtbctest::TestRlbtNotifyTriggerFired2L ),
        ENTRY( "LBTBCTest29", Clbtbctest::TestRlbtCancelNotifyTriggerFiredL ),
        ENTRY( "LBTBCTest30", Clbtbctest::TestRlbtGetFiredTriggersL ),
        ENTRY( "LBTBCTest31", Clbtbctest::TestRlbtNotifyTriggeringSystemSettingChangeL ),
        ENTRY( "LBTBCTest32", Clbtbctest::TestRlbtCancelNotifyTriggeringSystemSettingChangeL ),
        ENTRY( "LBTBCTest33", Clbtbctest::TestRlbtGetTriggeringSystemSettingsL ),
        ENTRY( "LBTBCTest34", Clbtbctest::TestRlbtCancelAllL ),
        ENTRY( "LBTBCTest35", Clbtbctest::TestCLbtGeoCell1L ),
        ENTRY( "LBTBCTest36", Clbtbctest::TestCLbtGeoCell2L ),
        ENTRY( "LBTBCTest37", Clbtbctest::TestCLbtGeoCell3L ),
        ENTRY( "LBTBCTest38", Clbtbctest::TestCLbtGeoCell4L ),
        ENTRY( "LBTBCTest39", Clbtbctest::TestCLbtGeoCell5L ),
        ENTRY( "LBTBCTest39_2", Clbtbctest::TestCLbtGeoCell6L ),
        ENTRY( "LBTBCTest39_3", Clbtbctest::TestCLbtGeoCell7L ),
        ENTRY( "LBTBCTest40", Clbtbctest::TestCLbtGeoCircle1L ),
        ENTRY( "LBTBCTest41", Clbtbctest::TestCLbtGeoCircle2L ),
        ENTRY( "LBTBCTest42", Clbtbctest::TestCLbtGeoCircle3L ),
        ENTRY( "LBTBCTest43", Clbtbctest::TestCLbtGeoCircle4L ),
        ENTRY( "LBTBCTest44", Clbtbctest::TestCLbtGeoCircle5L ),
        ENTRY( "LBTBCTest45", Clbtbctest::TestCLbtGeoCircle6L ),
        ENTRY( "LBTBCTest46", Clbtbctest::TestCLbtGeoRect1L ),
        ENTRY( "LBTBCTest47", Clbtbctest::TestCLbtGeoRect2L ),
        ENTRY( "LBTBCTest48", Clbtbctest::TestCLbtGeoRect3L ),
        ENTRY( "LBTBCTest49", Clbtbctest::TestCLbtGeoRect4L ),
        ENTRY( "LBTBCTest50", Clbtbctest::TestCLbtGeoRect5L ),
        ENTRY( "LBTBCTest51", Clbtbctest::TestCLbtListTriggerOptions1L ),
        ENTRY( "LBTBCTest52", Clbtbctest::TestCLbtListTriggerOptions2L ),
        ENTRY( "LBTBCTest53", Clbtbctest::TestCLbtListTriggerOptions3L ),
        ENTRY( "LBTBCTest53_2", Clbtbctest::TestCLbtListTriggerOptions4L ),
        ENTRY( "LBTBCTest53_3", Clbtbctest::TestCLbtListTriggerOptions5L ),
        ENTRY( "LBTBCTest53_4", Clbtbctest::TestCLbtListTriggerOptions6L ),
        ENTRY( "LBTBCTest54", Clbtbctest::TestRLbtServerL ),
        ENTRY( "LBTBCTest55", Clbtbctest::TestCLbtSessionTrigger1L ),
        ENTRY( "LBTBCTest56", Clbtbctest::TestCLbtSessionTrigger2L ),
        ENTRY( "LBTBCTest57", Clbtbctest::TestCLbtSessionTrigger3L ),
        ENTRY( "LBTBCTest58", Clbtbctest::TestCLbtSessionTrigger4L ),
        ENTRY( "LBTBCTest59", Clbtbctest::TestCLbtSessionTrigger5L ),
        ENTRY( "LBTBCTest60", Clbtbctest::TestCLbtStartupTrigger1L ),
        ENTRY( "LBTBCTest61", Clbtbctest::TestCLbtStartupTrigger2L ),
        ENTRY( "LBTBCTest62", Clbtbctest::TestCLbtStartupTrigger3L ),
        ENTRY( "LBTBCTest63", Clbtbctest::TestCLbtStartupTrigger4L ),
        ENTRY( "LBTBCTest64", Clbtbctest::TestCLbtStartupTrigger5L ),
        ENTRY( "LBTBCTest65", Clbtbctest::TestCLbtTriggerChangeEventNotifierL ),
        ENTRY( "LBTBCTest66", Clbtbctest::TestCLbtTriggerConditionArea1L ),
        ENTRY( "LBTBCTest67", Clbtbctest::TestCLbtTriggerConditionArea2L ),
        ENTRY( "LBTBCTest68", Clbtbctest::TestCLbtTriggerConditionArea3L ),
        ENTRY( "LBTBCTest69", Clbtbctest::TestCLbtTriggerConditionArea4L ),
        ENTRY( "LBTBCTest69_2", Clbtbctest::TestCLbtTriggerConditionArea5L ),
        ENTRY( "LBTBCTest69_3", Clbtbctest::TestCLbtTriggerConditionArea6L ),
        ENTRY( "LBTBCTest69_4", Clbtbctest::TestCLbtTriggerConditionArea7L ),
        ENTRY( "LBTBCTest69_5", Clbtbctest::TestCLbtTriggerConditionArea8L ),
        ENTRY( "LBTBCTest70", Clbtbctest::TestCLbtTriggerEntry1L ),
        ENTRY( "LBTBCTest71", Clbtbctest::TestCLbtTriggerEntry2L ),
        ENTRY( "LBTBCTest72", Clbtbctest::TestCLbtTriggerFilterByArea1L ),
        ENTRY( "LBTBCTest73", Clbtbctest::TestCLbtTriggerFilterByArea2L ),
        ENTRY( "LBTBCTest74", Clbtbctest::TestCLbtTriggerFilterByArea3L ),
        ENTRY( "LBTBCTest75", Clbtbctest::TestCLbtTriggerFilterByArea4L ),
        ENTRY( "LBTBCTest75_2", Clbtbctest::TestCLbtTriggerFilterByArea5L ),
        ENTRY( "LBTBCTest75_3", Clbtbctest::TestCLbtTriggerFilterByArea6L ),
        ENTRY( "LBTBCTest75_4", Clbtbctest::TestCLbtTriggerFilterByArea7L ),
        ENTRY( "LBTBCTest75_5", Clbtbctest::TestCLbtTriggerFilterByArea8L ),
        ENTRY( "LBTBCTest76", Clbtbctest::TestCLbtTriggerFilterByAttribute1L ),
        ENTRY( "LBTBCTest77", Clbtbctest::TestCLbtTriggerFilterByAttribute2L ),
        ENTRY( "LBTBCTest78", Clbtbctest::TestCLbtTriggerFilterByAttribute3L ),
        ENTRY( "LBTBCTest79", Clbtbctest::TestCLbtTriggerFilterComposite1L ),
        ENTRY( "LBTBCTest80", Clbtbctest::TestCLbtTriggerFilterComposite2L ),
        ENTRY( "LBTBCTest81", Clbtbctest::TestCLbtTriggerFilterComposite3L ),
        ENTRY( "LBTBCTest81_2", Clbtbctest::TestCLbtTriggerFilterComposite4L ),
        ENTRY( "LBTBCTest82", Clbtbctest::TestCLbtTriggerFiringEventNotifierL ),
        ENTRY( "LBTBCTest83", Clbtbctest::TestCLbtTriggerInfo1L ),
        ENTRY( "LBTBCTest84", Clbtbctest::TestCLbtTriggerInfo2L ),
        ENTRY( "LBTBCTest85", Clbtbctest::TestCLbtTriggerInfo3L ),
        ENTRY( "LBTBCTest86", Clbtbctest::TestCLbtTriggerInfo4L ),
        ENTRY( "LBTBCTest87", Clbtbctest::TestTLbtTriggeringSystemSettingsL ),
        ENTRY( "LBTBCTest88", Clbtbctest::TestCLbtTriggeringSystemSettingsChangeEventNotifier1L ),
        ENTRY( "LBTBCTest94_1", Clbtbctest::TestRLbtAsyncUpdate1L ),
        ENTRY( "LBTBCTest94_2", Clbtbctest::TestRLbtAsyncUpdate2L ),
        ENTRY( "LBTBCTest94_3", Clbtbctest::TestRLbtAsyncUpdate3L ),
        ENTRY( "LBTBCTest94_4", Clbtbctest::TestRLbtAsyncUpdate4L ),
        ENTRY( "LBTBCTest94_5", Clbtbctest::TestRLbtAsyncUpdate5L ),
        ENTRY( "LBTBCTest94_6", Clbtbctest::TestRLbtAsyncUpdate6L ),
        ENTRY( "LBTBCTest94_7", Clbtbctest::TestRLbtAsyncUpdate7L ),
        ENTRY( "LBTBCTest95", Clbtbctest::TestRLbtCancelUpdateTriggerL ),
        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Clbtbctest::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void Clbtbctest::AssertTrueL( TBool aCondition,const TDesC& aMessage )
    {
           
         if(!aCondition)
         {
          
    	   iLog->Log( aMessage );
           User::LeaveIfError(KErrGeneral);
         }
    
    }
    
// -----------------------------------------------------------------------------
// Clbtbctest::TestRlbtOpen1L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Clbtbctest::TestRlbtOpen1L( CStifItemParser& aItem )
    {
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    RLbt lbt;
    TInt value;
    aItem.GetNextInt(value);
    if(value==2)
    {
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtServerBadHandle); 	
    TInt retOpen = lbt.Open(lbtServ);
    
    }
    
        //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    CleanupClosePushL(lbtServ);	
    iLog->Log(_L("Connection to RLbtServer Passed ")); 	
    TInt retOpen = lbt.Open(lbtServ);
    if(retOpen!=KErrNone)
    	{
    	iLog->Log(_L("Opening subsession to RLbtServer Failed %d"),retOpen);     
    	return retOpen;
    	}
    CleanupClosePushL(lbt);	
    //Closes the subsession
    lbt.Close();
    CleanupStack::PopAndDestroy(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);       
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Clbtbctest::TestRlbtOpen2L
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Clbtbctest::TestRlbtOpen2L( CStifItemParser& /*aItem*/ )
    {
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    RLbt lbt;
        //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    CleanupClosePushL(lbtServ);	
    iLog->Log(_L("Connection to RLbtServer Passed ")); 	
    TInt retOpen = lbt.Open();
    if(retOpen!=KErrNone)
    	{
    	iLog->Log(_L("Opening subsession to RLbtServer Failed %d"),retOpen);     
    	return retOpen;
    	}
    CleanupClosePushL(lbt);	
    //Closes the subsession
    lbt.Close();
    CleanupStack::PopAndDestroy(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);       
    return KErrNone;
    }    
    
TInt Clbtbctest::TestRlbtConnect1L( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("ErrorAlreadyConnected"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
        //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    if(retConnect!=KErrNone)
    	{
    	iLog->Log(_L("Connection to RLbtServer Failed %d"),retConnect);     
    	return retConnect;
    	}
    CleanupClosePushL(lbtServ);	
    iLog->Log(_L("Connection to RLbtServer Passed "));
    iLog->Log(_L("Trying to connect again ")); 
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 1);  	
    TInt err= lbtServ.Connect();
    if(err!=KErrNone)
    	{
    	iLog->Log(_L("Connection to RLbtServer Failed %d"),retConnect); 
       
    	}
 
    return KErrNone;

    }
// -----------------------------------------------------------------------------
// Clbtbctest::TestRlbtCreateTriggerL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Clbtbctest::TestRlbtCreateTriggerL( CStifItemParser& /*aItem*/ )
    {
        //Connect to Location Triggering Server
    RLbtServer lbtServ;
    User::LeaveIfError(lbtServ.Connect());
    CleanupClosePushL(lbtServ);

    RLbt lbt;
    User::LeaveIfError(lbt.Open(lbtServ));
    CleanupClosePushL(lbt);
    TRAP_IGNORE(lbt.DeleteTriggersL());   
    TRealX nan;
    nan.SetNaN();
    
    //Define the triggering area
    CLbtGeoCircle* trigArea = 
        CLbtGeoCircle::NewL( 
            TCoordinate( 61.5,24.3,0 ), //center coordinate
            100                       //radius
    );
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
   /* TUidType handlingUidType( 
        TUid::Uid( 0x00000001 ),
        TUid::Uid( 0x00000002 ),
        TUid::Uid( 0x00000003 ) );*/
        
        TSecureId secureid= 0x1333;
        CLbtTriggerEntry::TLbtTriggerState aState =CLbtTriggerEntry::EStateEnabled; 

    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    TUid aManagerUi;
		aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        aState,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        // CLbtTriggerEntry::EPostFiringActionNoAction, //Trigger fired option
        aManagerUi,				                      //Manager UI
        cond,                                       //Trigger condition
        KMyTriggerHandlingProcessName,              //Handling process name
        // handlingUidType,                            //Handling UID type
        secureid,
        KNullDesC() );                              //Command line
        
    //Pop trigger condition object
    CleanupStack::Pop( cond );
    
    //Push trigger object to cleanup stack
    CleanupStack::PushL( trig );

    //Create the trigger in Location Triggering Server
    TLbtTriggerId trigId = KLbtNullTriggerId;
    TRequestStatus status;
    
    CMyObserver *ao;
	  ao = CMyObserver::NewL();
	  CActiveSchedulerWait *schedulerwait;
	  schedulerwait= new CActiveSchedulerWait;
	  ao->GiveHandle(schedulerwait);
    
    lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
    
    //iLog->Log(_L("Session trigger created ")); 
    
    ao->MySetActive();	
	  schedulerwait->Start();  
	  TInt err =ao->iStatus.Int();  
    if( err!=KErrNone)
    	{
	    	//Delete the instance of trigger object
	    CleanupStack::PopAndDestroy( trig );
	    lbt.Close();
	    CleanupStack::Pop(&lbt); 
	    //Close connection to RLbtServer
	    lbtServ.Close(); 
	    CleanupStack::PopAndDestroy(&lbtServ);  
	    return err;     
    	}
    else
    	{	
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone; 
    	}    
    }  
    
    
TInt Clbtbctest::TestRlbtCreateTrigger2L( CStifItemParser& /*aItem*/ )
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
    }  
    
    //create trigger when request outstanding
TInt Clbtbctest::TestRlbtCreateTrigger3L( CStifItemParser& /*aItem*/ )
    {
        //Connect to Location Triggering Server
    RLbtServer lbtServ;
    User::LeaveIfError(lbtServ.Connect());
    CleanupClosePushL(lbtServ);

    RLbt lbt;
    User::LeaveIfError(lbt.Open(lbtServ));
    CleanupClosePushL(lbt);
        
    TRealX nan;
    nan.SetNaN();
    
    //Define the triggering area
    CLbtGeoCircle* trigArea = 
        CLbtGeoCircle::NewL( 
            TCoordinate( 61.5,24.3,0 ), //center coordinate
            100                       //radius
    );
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
          
    TSecureId secureid= 0x1333;
    CLbtTriggerEntry::TLbtTriggerState aState =CLbtTriggerEntry::EStateEnabled; 

    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    TUid aManagerUi;
		aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        aState,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        // CLbtTriggerEntry::EPostFiringActionNoAction, //Trigger fired option
        aManagerUi,				                      //Manager UI
        cond,                                       //Trigger condition
        KMyTriggerHandlingProcessName,              //Handling process name
        // handlingUidType,                            //Handling UID type
        secureid,
        KNullDesC() );                              //Command line
        
    //Pop trigger condition object
    CleanupStack::Pop( cond );
    
    //Push trigger object to cleanup stack
    CleanupStack::PushL( trig );

    //Create the trigger in Location Triggering Server
    TLbtTriggerId trigId;
    TRequestStatus status;
    
    CMyObserver *ao;
	ao = CMyObserver::NewL();
	CActiveSchedulerWait *schedulerwait;
	schedulerwait= new CActiveSchedulerWait;
	ao->GiveHandle(schedulerwait);
	TLbtTriggerFireInfo aFireInfo;
	lbt.NotifyTriggerFired(aFireInfo,ao->iStatus);
	TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtDuplicateRequest);  	
    lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
    
    //iLog->Log(_L("Session trigger created ")); 
    
    ao->MySetActive();	
	schedulerwait->Start();  
	TInt err =ao->iStatus.Int();  
    if( err!=KErrNone)
    	{
	    	//Delete the instance of trigger object
	    CleanupStack::PopAndDestroy( trig );
	    lbt.Close();
	    CleanupStack::Pop(&lbt); 
	    //Close connection to RLbtServer
	    lbtServ.Close(); 
	    CleanupStack::PopAndDestroy(&lbtServ);  
	    return -99;     
    	}
    else
    	{	
	    //Delete the instance of trigger object
	    CleanupStack::PopAndDestroy( trig );
	    lbt.Close();
	    CleanupStack::Pop(&lbt); 
	    //Close connection to RLbtServer
	    lbtServ.Close(); 
	    CleanupStack::PopAndDestroy(&lbtServ);  
	    return KErrNone; 
    	}    
    }  
    
//create trigger when trigger name is NULL && trig handling process length is 0
TInt Clbtbctest::TestRlbtCreateTrigger4L( CStifItemParser& /*aItem*/ )
    {
        //Connect to Location Triggering Server
    RLbtServer lbtServ;
    User::LeaveIfError(lbtServ.Connect());
    CleanupClosePushL(lbtServ);

    RLbt lbt;
    User::LeaveIfError(lbt.Open(lbtServ));
    CleanupClosePushL(lbt);
        
    TRealX nan;
    nan.SetNaN();
    
    //Define the triggering area
    CLbtGeoCircle* trigArea = 
        CLbtGeoCircle::NewL( 
            TCoordinate( 61.5,24.3,0 ), //center coordinate
            100                       //radius
    );
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
          
    TSecureId secureid= 0x1333;
    CLbtTriggerEntry::TLbtTriggerState aState =CLbtTriggerEntry::EStateEnabled; 

//     _LIT( KMyTriggerName, "" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    TUid aManagerUi;
		aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL( 
    		KNullDesC(),                             //Trigger name
        aState,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        // CLbtTriggerEntry::EPostFiringActionNoAction, //Trigger fired option
        aManagerUi,				                      //Manager UI
        NULL,                                       //Trigger condition
        KMyTriggerHandlingProcessName,              //Handling process name
        // handlingUidType,                            //Handling UID type
        secureid,
        KNullDesC() );                              //Command line
        
    //Pop trigger condition object
    CleanupStack::Pop( cond );
    trig->SetCondition(NULL);
    trig->SetNameL(KNullDesC() );
    //Push trigger object to cleanup stack
    CleanupStack::PushL( trig );

    //Create the trigger in Location Triggering Server
    TLbtTriggerId trigId;
    TRequestStatus status;
    
    CMyObserver *ao;
	ao = CMyObserver::NewL();
	CActiveSchedulerWait *schedulerwait;
	schedulerwait= new CActiveSchedulerWait;
	ao->GiveHandle(schedulerwait);
	
    lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
   
    ao->MySetActive();	
	schedulerwait->Start();  
	TInt err =ao->iStatus.Int(); 
	if(err!= KErrArgument)
		{
		//Delete the instance of trigger object
	    CleanupStack::PopAndDestroy( trig );
	    lbt.Close();
	    CleanupStack::Pop(&lbt); 
	    //Close connection to RLbtServer
	    lbtServ.Close(); 
	    CleanupStack::PopAndDestroy(&lbtServ);  
	    return -99;    
		}
	_LIT( KNewTriggerName, "Test" );
	_LIT( KNewTriggerHandlingProcessName, "" );
	trig->SetNameL(KNewTriggerName);
	trig->SetProcessId(KNewTriggerHandlingProcessName,secureid);
	trig->SetCondition(cond);
	lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
	
	//iLog->Log(_L("Session trigger created ")); 
	
	ao->MySetActive();	
	schedulerwait->Start();  
	err =ao->iStatus.Int(); 
	if(err!= KErrArgument)
		{
		//Delete the instance of trigger object
	    CleanupStack::PopAndDestroy( trig );
	    lbt.Close();
	    CleanupStack::Pop(&lbt); 
	    //Close connection to RLbtServer
	    lbtServ.Close(); 
	    CleanupStack::PopAndDestroy(&lbtServ);  
	    return -99;    
		}
    else
    	{	
	    //Delete the instance of trigger object
	    CleanupStack::PopAndDestroy( trig );
	    lbt.Close();
	    CleanupStack::Pop(&lbt); 
	    //Close connection to RLbtServer
	    lbtServ.Close(); 
	    CleanupStack::PopAndDestroy(&lbtServ);  
	    return KErrNone; 
    	}    
    }  
    //Trying to create a trigger with trigger area less than minimum trigger area
    TInt Clbtbctest::TestRlbtCreateTrigger5L( CStifItemParser& /*aItem*/ )
    {
        //Connect to Location Triggering Server
    RLbtServer lbtServ;
    User::LeaveIfError(lbtServ.Connect());
    CleanupClosePushL(lbtServ);

    RLbt lbt;
    User::LeaveIfError(lbt.Open(lbtServ));
    CleanupClosePushL(lbt);
        
    TRealX nan;
    nan.SetNaN();
  
    CLbtGeoCircle* trigArea = 
        CLbtGeoCircle::NewL( 
            TCoordinate( 61.5,24.3,0 ), //center coordinate
            0.1                       //radius
    );
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
   
    TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger Name" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    TUid aManagerUi;
	  aManagerUi = TUid::Uid(0x87654321);
	
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
       // CLbtTriggerEntry::EPostFiringActionNoAction, //Trigger fired option
        aManagerUi,				                      //Manager UI
        cond,                                       //Trigger condition
        KMyTriggerHandlingProcessName,              //Handling process name
       // handlingUidType,                            //Handling UID type
       secureid,
        KNullDesC() );                              //Command line
        
    //Pop trigger condition object
    CleanupStack::Pop( cond );
    
    //Push trigger object to cleanup stack
    CleanupStack::PushL( trig );

    //Create the trigger in Location Triggering Server
    TLbtTriggerId trigId;
    TRequestStatus status;
    
    CMyObserver *ao;
	  ao = CMyObserver::NewL();
	  CActiveSchedulerWait *schedulerwait;
	  schedulerwait= new CActiveSchedulerWait;
	  ao->GiveHandle(schedulerwait);
    
    lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
    
    //iLog->Log(_L("Session trigger created ")); 
    
    ao->MySetActive();	
	  schedulerwait->Start(); 
   
    TInt errorflag=0;
	  if(ao->iStatus == KErrTriggerAreaTooSmall)
	  {  
         errorflag=1;
	  }
	  else if(ao->iStatus == KLbtErrMaxEnabledValidTriggersLimitExceeded)
	  {
		 errorflag=0;
	  } 
	  
	  else 
	  {
		 errorflag=0;
	  } 
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
   
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    if(errorflag ==1)
    {
    	return KErrNone;
    }
    else
    {
    	return -10;
    }     
	
    }
 // -----------------------------------------------------------------------------
// Clbtbctest::TestRlbtCancelCreateTriggerL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Clbtbctest::TestRlbtCancelCreateTriggerL( CStifItemParser& aItem )
    {
     RLbtServer lbtserver;
  	 RLbt lbt;
  	   	 
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 TInt value;
 	 aItem.GetNextInt(value);	 
    //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("abc"));
          
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition

    TCoordinate coordinate(62.5285,23.9385);
   // TCoordinate coordinate(62.4438,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    
   	lbt.CancelCreateTrigger();
     	
	TInt error=notifier->iStatus.Int();    
   
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(error!=KErrCancel)
    		{
    		return KErrGeneral;
    		}
    return KErrNone;
    }         
    
    //Test DeletTriggerL
TInt Clbtbctest::TestRlbtDeleteTriggerL( CStifItemParser& /*aItem*/ )
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
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId;
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    lbt.DeleteTriggerL(trigId);
    
//    CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(error,lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(););
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(error == KErrNotFound)
	    {
	     return KErrNone;
	  
	    }
    else
	    {
	      return error;
	  
	    }
  }  
  //Test synchronous DeleteTriggersL
TInt Clbtbctest::TestRlbtDeleteTriggers1L( CStifItemParser& /*aItem*/ )
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
       
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
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
    return KErrNone;
    
  }             
  
//Test asynchronous DeleteTriggers
TInt Clbtbctest::TestRlbtDeleteTriggers2L( CStifItemParser& /*aItem*/ )
    {
      
    RLbtServer lbtserver;
 	RLbt lbt;
 	TCoordinate coordinate(10,20,0);
 	 
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
    trig1->SetProcessId(_L("About.exe"),KNullUid );   
         
    CLbtGeoCircle* area=CLbtGeoCircle::NewL();
    area->SetRadius(100);
    area->SetCenter(coordinate);
    
    CLbtTriggerConditionArea* condn=CLbtTriggerConditionArea::NewL(area,CLbtTriggerConditionArea::EFireOnEnter);
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
            
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    //CLbtGeoRect* area2 = CLbtGeoRect::NewL( 0,50,0,70);
    CLbtGeoCircle* area2=CLbtGeoCircle::NewL();
    area2->SetRadius(100);
    area2->SetCenter(coordinate);
    areaFilter->SetArea( area2 ); 
      
    notifier->DeleteFilteredTriggers(lbt,areaFilter,wait);
    wait->Start();
    //CLbtTriggerInfo* trigEntry=NULL;
    CLbtTriggerInfo* trigEntry=lbt.GetTriggerLC(trigId1);
           
    CleanupStack::PopAndDestroy( areaFilter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    //delete wait; 
    return KErrNone;
    
  }             
  
   //Testing of deleting triggers using array of trigger ids - synchronous
TInt Clbtbctest::TestRlbtDeleteTriggers3L( CStifItemParser& /*aItem*/ )
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
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
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
  
   // delete wait;
    list.Close(); 
    return KErrNone;
    
  }               
  
//Testing of deleting triggers using array of trigger ids - synchronous
TInt Clbtbctest::TestRlbtDeleteTriggers4L( CStifItemParser& /*aItem*/ )
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
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
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
    /*CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); */
    notifier->DeleteTriggersById(lbt,list,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==notifier->GetError(),_L("wrong error code"));
    
    CLbtTriggerInfo* trigEntry=NULL;
    
    trigEntry=lbt.GetTriggerLC(trigId3);
    
    trigEntry=lbt.GetTriggerLC(trigId2);
    
    //trigEntry=lbt.GetTriggerLC(trigId1);
            
    CleanupStack::PopAndDestroy( trigEntry );  
  //  CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    list.Close(); 
    return KErrNone;
    
  }       
  
  //Delete Cell based triggers in a cell area - synchronous
  TInt Clbtbctest::TestRlbtDeleteTriggers5L( CStifItemParser& /*aItem*/ )
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
//    CLbtTriggerInfo* Triginfo;
    
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy( ););
   
    if(error==KErrNotFound)
    {
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig1 );
	lbt.Close();
    
	CleanupStack::PopAndDestroy( &lbt );
	lbtserver.Close();
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return KErrNone;	
    }
    else
    {
    CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( trig1 );
	CleanupStack::Pop( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	//delete wait;
	return -99;
    }
  }     
  
  //Testing asynchronous DeleteTriggers using attribute filter
  TInt Clbtbctest::TestRlbtDeleteTriggers6L( CStifItemParser& /* aItem */ )
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
    
    
    
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
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
    
   /* CDeleteTriggerObserver* observer=CDeleteTriggerObserver::NewL();
    CleanupStack::PushL( observer ); */
    
    notifier->DeleteFilteredTriggers(lbt,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==notifier->GetError(),_L("wrong error code"));
            
    CLbtTriggerInfo* trigEntry=NULL;
       
       
    trigEntry=lbt.GetTriggerLC(trigId2);
    trigEntry=lbt.GetTriggerLC(trigId1);
    
    CleanupStack::PopAndDestroy( trigEntry );
   // CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( filter );
    CleanupStack::PopAndDestroy( trig2 );   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    return KErrNone;
	    
	 }
  //Testing asynchronous DeleteTriggers using area filter
  TInt Clbtbctest::TestRlbtDeleteTriggers7L( CStifItemParser& /* aItem */ )
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
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
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
    
    notifier->DeleteFilteredTriggers(lbt,filter,wait);
    wait->Start();
    
    AssertTrueL( KErrNone==notifier->GetError(),_L("wrong error code"));
    
//    CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(err,lbt.GetTriggerLC(trigId1);CleanupStack::Pop(););
    if(err==KErrNotFound)
    	{
   // CleanupStack::PopAndDestroy( observer );
    CleanupStack::Pop( filter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  	   
    return KErrNone;
    	}
    return KErrNone;
	    }
  
  //Deleting triggers in current Cellular using area filter asynchronously  
  
  TInt Clbtbctest::TestRlbtDeleteTriggers8L( CStifItemParser& /* aItem */ )
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
	CleanupStack::Pop( filter );
	CLbtTriggerInfo* Triginfo=NULL;
	TRAPD(error, Triginfo= lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(Triginfo ););
	  
	if(error==KErrNotFound)
	  {
	  CleanupStack::PopAndDestroy( notifier );
	  CleanupStack::PopAndDestroy( trig1 );
	  CleanupStack::PopAndDestroy( &lbt );
	  CleanupStack::PopAndDestroy( &lbtserver );
	  //delete wait;
	  return KErrNone;	
	  }
	  else
	  {
	  CleanupStack::PopAndDestroy( notifier );
	  CleanupStack::PopAndDestroy( trig1 );
	  CleanupStack::PopAndDestroy( &lbt );
	  CleanupStack::PopAndDestroy( &lbtserver );
		//delete wait;
	  return -99;
	  }
  
  }  
  
       //TODO: Change cfg file
TInt Clbtbctest::TestRlbtCancelDeleteTriggersL( CStifItemParser& /*aItem*/ )
    {
   	RLbtServer lbtserver;
  	RLbt lbt;
  	  
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
     
    User::LeaveIfError( lbt.Open(lbtserver));
 	CleanupClosePushL( lbt );
 	 
	   //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
       //Push to cleanup stack
    CleanupStack::PushL( trig );
        // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetNameL(_L("abc"));
       //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    TCoordinate coordinate(62.5285,23.9385);
   
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
      // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId=120;
            
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
	wait->Start( );
    
    RArray<TLbtTriggerId> triggerArray;
    triggerArray.Append(trigId);
    notifier->DeleteTriggersById( lbt,triggerArray,wait );
  
    lbt.CancelDeleteTriggers();
    	
    TInt error=notifier->iStatus.Int(); 
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   //CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    
   
   	if(error!=KErrCancel)
   		{
   			return KErrGeneral;
    	}
    else
    	{
    		return KErrNone;
    	}	
    
    }
    
  
  
  TInt Clbtbctest::TestRlbtGetTriggerLCL( CStifItemParser& /*aItem*/ )
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
 	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
 
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId,trigId2;
     trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    //User::After(5000000);
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtStartupTrigger *TrigEntry = static_cast <CLbtStartupTrigger*>(Triginfo->TriggerEntry());
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
   
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
	if(area ->Type()!=CLbtGeoAreaBase::ECircle )
		{
			return -99;
		}
		
	CLbtGeoCircle* circle2 = static_cast <CLbtGeoCircle*>(area);
	TCoordinate center = circle->Center();
	TCoordinate center2 = circle2->Center();
	TReal32 distance;
	center2.Distance(center,distance);
	if(distance!=0)
		{
			return -99;
		}
	TReal radius = circle2->Radius();
	if(radius != circle->Radius())
		{
			return -99;
		}
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
    HBufC* startupProcFile = HBufC::NewLC( KMaxFileName );
    TPtr aFileName = startupProcFile->Des();
    TSecureId aSecureId;
    TrigEntry->GetProcessId(aFileName,aSecureId);
    if(aFileName!=KMyTriggerHandlingProcessName)
	    {
	    return -99;	
	    }
    CleanupStack::PopAndDestroy(1 );//startupProcFile
   //Check Activity status
/*	TLbtTriggerDynamicInfo::TLbtTriggerActivityStatus currstatus;
	currstatus = dyninfo -> iActivityStatus;
    if(currstatus!=TLbtTriggerDynamicInfo::EActive)
    {
    	return -99;
    }*/
    CleanupStack::PopAndDestroy(1 );//list options
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 
  }    
  //Testing of updating trigger attributes
  TInt Clbtbctest::TestRlbtUpdateTriggerL( CStifItemParser& /*aItem*/ )
    {
   	RLbtServer lbtserver;
 	RLbt lbt;
 	  
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
     
 	User::LeaveIfError( lbt.Open( lbtserver ) );
 	CleanupClosePushL( lbt );
 	 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
      //Push to cleanup stack
    CleanupStack::PushL( trig );
     // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "LbtReference.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
   // set commandline
    trig->SetCommandLineL(_L("commandline1"));
       
    // set condition
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    trig->SetCondition(condition); // ownership transferred to object
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    trig->SetManagerUi(aManagerUi);   
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
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
    newtrig->SetCommandLineL(_L("Commandline2" ));
    TCoordinate newcoordinate(10,10);
    
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
    |CLbtTriggerEntry::EAttributeCondition
    |CLbtTriggerEntry::EAttributeRearmTime,
    ELbtTrue
    );
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
	CLbtTriggerConditionArea::TDirection direction;
	direction = Condition2->Direction();
	if(direction!=CLbtTriggerConditionArea::EFireOnEnter)
	{
		return -99;
	}
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	if(area ->Type()!=CLbtGeoAreaBase::ECircle )
	{
		return -99;
	}
		
	CLbtGeoCircle* circle2 = static_cast <CLbtGeoCircle*>(area);
	TCoordinate center = newcircle->Center();
	TCoordinate center2 = circle2->Center();
	TReal32 distance;
	center2.Distance(center,distance);
	if(distance!=0)
	{
		return -99;
	}
	TReal radius = circle2->Radius();
	if(radius != newcircle->Radius())
	{
		return -99;
	}

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
   // delete wait;
   
    iLog->Log(_L("Test passed "));
    return KErrNone; 
  } 
  
  //Trying to update non modifiable trigger attributes
  TInt Clbtbctest::TestRlbtUpdateTrigger2L( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError(lbt.Open(lbtserver));
	CleanupClosePushL( lbt);
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
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
    newtrig->SetId(1);
   // TestModuleIf().SetExitReason( CTestModuleIf::EPanic,-21); 
 	    
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeId
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
      return KErrNone;
   
  } 
  
  //Trying to update condition with NULL condition
  TInt Clbtbctest::TestRlbtUpdateTrigger3L( CStifItemParser& /*aItem*/ )
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
    TLbtTriggerId trigId;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
   
  //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    newtrig->SetCondition(NULL);
    newtrig->SetId(trigId);
  //TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
   /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
  // lbt.UpdateTriggerL(*newtrig);
    iLog->Log(_L("After UpdateTriggerL "));
   
 
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
  
  //Trying to update trigger with NULL trigger area
  TInt Clbtbctest::TestRlbtUpdateTrigger4L( CStifItemParser& /*aItem*/ )
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
	   
  //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    CleanupStack::PushL( newtrig );
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    newtrig->SetCommandLineL(_L("Commandline2" ));
    TCoordinate newcoordinate(10,10);
         
   // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL();
//   newcondition->SetTriggerArea(NULL)   ;
  
    newtrig->SetCondition(newcondition); // ownership transferred to object
	TLbtTriggerId trigId = 1;
    newtrig->SetId(trigId);
  
   /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition
    |CLbtTriggerEntry::EAttributeStartUpCommandLine,ELbtTrue
   );
  
   
    CleanupStack::PopAndDestroy( newtrig );
  
    CleanupStack::Pop( &lbt );
    lbt.Close();
   
    CleanupStack::PopAndDestroy( &lbtserver );
  // delete wait;
  
    iLog->Log(_L("Test passed "));
    return KErrNone; 
   
  } 
  
  //Trying to update trigger handling process
  TInt Clbtbctest::TestRlbtUpdateTrigger5L( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError(lbt.Open(lbtserver));
	CleanupClosePushL( lbt);
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    _LIT( KMyTriggerHandlingProcessName, "LbtReference.exe" );
    TSecureId secureid;
    newtrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
   // set commandline
    newtrig->SetCommandLineL(_L("commandline1"));
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,500);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(1);
   // TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 21); 
 	    
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeStartUpProcessId,ELbtTrue);
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
      return KErrNone;
   
  } 
  
  
  //Trying to update trigger with commandline parameter length 0
  TInt Clbtbctest::TestRlbtUpdateTrigger6L( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError(lbt.Open(lbtserver));
	CleanupClosePushL( lbt);
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    _LIT( KMyTriggerHandlingProcessName, "LbtReference.exe" );
    TSecureId secureid;
    newtrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
   // set commandline
    newtrig->SetCommandLineL(_L(""));
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,500);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(1);
   // TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
 	    
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeStartUpCommandLine,ELbtTrue);
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone;
   
  } 
  //Trying to update trigger name with zero length name
  TInt Clbtbctest::TestRlbtUpdateTrigger7L( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError(lbt.Open(lbtserver));
	CleanupClosePushL( lbt);
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L(""));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
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
    newtrig->SetId(1);
   // TestModuleIf().SetExitReason( CTestModuleIf::EPanic,-21); 
 	    
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone;
	} 
	
	//Updating Nonmodifiable attributes of a startup trigger - TC_LBT012_03
    TInt Clbtbctest::TestRlbtUpdateTrigger8L( CStifItemParser& /* aItem */ )
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
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object
		//Set Requestor
	 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
		CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
		TBuf<KLbtMaxNameLength> ReqData=_L("");
		trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
    trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
 //   User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,500);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
    
    newtrig->SetManagerUi(TUid::Uid(0x12345678));

    newtrig->SetId(trigId);
    
    newtrig->SetRequestorL(CRequestor::ERequestorContact,CRequestor::EFormatTelephone,_L("Test2") );
    _LIT( KNewTriggerHandlingProcessName, "ConsoleUI.exe" );
    
    TSecureId newsecureid;
    newtrig->SetProcessId(KNewTriggerHandlingProcessName,newsecureid);
       
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    TRAPD(error,lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeRequestor
    |CLbtTriggerEntry::EAttributeManagerUi
    |CLbtTriggerEntry::EAttributeStartUpProcessId,ELbtTrue
    ));
    if(error==KErrAccessDenied)
    {
    iLog->Log(_L("After UpdateTriggerL "));
      
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 	
    }
    else
    {
    	iLog->Log(_L("After UpdateTriggerL "));
      CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    CleanupStack::PopAndDestroy(1);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
          
    }
  
  TInt Clbtbctest::TestRlbtSetTriggerStateL( CStifItemParser& /*aItem*/ )
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
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object
		//Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
    trig->SetManagerUi(aManagerUi);   
    trig->SetState(CLbtTriggerEntry::EStateDisabled);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
 //   User::After(5000000);
    
   	lbt.SetTriggerStateL (trigId,CLbtTriggerEntry::EStateEnabled,ELbtTrue);
   	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    if(TrigEntry->State()==CLbtTriggerEntry::EStateEnabled)
    {
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone; 	
    }
    else
    {
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return -99; 	
    }
  }   
  
  TInt Clbtbctest::TestRlbtSetTriggersState1L( CStifItemParser& /*aItem*/ )
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
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetCondition(condition); // ownership transferred to object
	//Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
    trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    //For reproducing crash for loop added
    for(int i=0;i<15;i++)  
    {
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );	
    }
       
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue);   
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    iLog->Log(_L("Before GetTriggersL"));
    lbt.GetTriggersL(trigInfoList);
    iLog->Log(_L("After GetTriggersL"));
    
    //Do something with the trigger list
   
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    	if(CLbtTriggerEntry::EStateEnabled!=(TrigEntry->State()))
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
  
  TInt Clbtbctest::TestRlbtSetTriggersState2L( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	TCoordinate coordinate(65.5285,23.9385);
       	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
   // TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
   
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait);
    wait->Start();
    
    lbt.GetTriggersL(trigInfoList );
    //Do something with the trigger list
    
    
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
   	CLbtTriggerEntry *TrigEntry = trigInfoList[i]->TriggerEntry();
  	if(CLbtTriggerEntry::EStateEnabled!=(TrigEntry->State()))
  	{
  		return -99;
  	}
  	TLbtTriggerDynamicInfo * dyninfo = trigInfoList[i] -> DynInfo() ;
  
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
  //Updating state of  the triggers filtered based on attribute filter - asynchronous
  TInt Clbtbctest::TestRlbtSetTriggersState3L( CStifItemParser& /*aItem*/ )
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
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeStartup );
    
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait,attribFilter);   
    wait->Start();
    CleanupStack::Pop( attribFilter );   
    
    lbt.GetTriggersL(trigInfoList);    
   
    //Do something with the trigger list
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
	  CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
	        
	     //Check type of the trigger
	  TInt Type2 = TrigEntry ->Type();
	  if(Type2 ==CLbtTriggerEntry::ETypeStartup)
	    {
		  CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
		  if(State2 !=CLbtTriggerEntry::EStateEnabled)
		    {
		    	return -99;
		    }
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
  
  //Updating state of  the triggers filtered based on area filter - asynchronous
  TInt Clbtbctest::TestRlbtSetTriggersState4L( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	TRAP_IGNORE(lbt.DeleteTriggersL());	 ;
    	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
   
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    CLbtGeoCircle* area = CLbtGeoCircle::NewL( 
        TCoordinate( 62.5285,23.9385,0 ),
        1000 );
    areaFilter->SetArea( area ); 
    
     notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait,areaFilter);   
     wait->Start();
     CleanupStack::Pop( areaFilter ); 
     
     lbt.GetTriggersL(trigInfoList );
        
    
    TInt count = trigInfoList.Count();
   for ( TInt i = 0; i < count; i++ )
    {
	    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
	   //Check area of the trigger
		CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

		CLbtGeoAreaBase* area = Condition2->TriggerArea();
		if(area ->Type()!=CLbtGeoAreaBase::ECircle )
		{
			return -99;
		}
		
		CLbtGeoCircle* circle2 = static_cast <CLbtGeoCircle*>(area);

		TCoordinate center2 = circle2->Center();
		TReal32 distance;
		center2.Distance(TCoordinate( 62.5285,23.9385,0 ),distance);
		if(distance==0)
		{
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
  //Updating state of  the cell based triggers filtered based on area filter - synchronous
   TInt Clbtbctest::TestRlbtSetTriggersState5L( CStifItemParser& /*aItem*/ )
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
   //Updating state of  the triggers filtered based on attribute filter - synchronous
   TInt Clbtbctest::TestRlbtSetTriggersState6L( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
    
    TRAP_IGNORE(lbt.DeleteTriggersL());	 
     	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    trig->SetManagerUi(aManagerUi);   
     //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
   
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,attribFilter);   
    
    CleanupStack::Pop( attribFilter );   
    
    
    lbt.GetTriggersL(trigInfoList);    
       

    //Do something with the trigger list
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
	    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
	        
	     //Check type of the trigger
	    TInt Type2 = TrigEntry ->Type();
	    if(Type2 ==CLbtTriggerEntry::ETypeSession)
	    {
		    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
		    if(State2 !=CLbtTriggerEntry::EStateEnabled)
		    {
		    	return -99;
		    }
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
   //Updating state of  the triggers filtered based on area filter - synchronous
   TInt Clbtbctest::TestRlbtSetTriggersState7L( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	TRAP_IGNORE(lbt.DeleteTriggersL());
    	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
     //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
    CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    CLbtGeoCircle* area = CLbtGeoCircle::NewL( 
        TCoordinate( 62.5285,23.9385,0 ),
        1000 );
    areaFilter->SetArea( area ); 
    
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,areaFilter);   
     
    CleanupStack::Pop( areaFilter ); 
     
    lbt.GetTriggersL(trigInfoList );
        
    
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
	    CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
	   //Check area of the trigger
		CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

		CLbtGeoAreaBase* area = Condition2->TriggerArea();
		if(area ->Type()!=CLbtGeoAreaBase::ECircle )
		{
			return -99;
		}
		
		CLbtGeoCircle* circle2 = static_cast <CLbtGeoCircle*>(area);

		TCoordinate center2 = circle2->Center();
		TReal32 distance;
		center2.Distance(TCoordinate( 62.5285,23.9385,0 ),distance);
		if(distance==0)
		{
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
   
   //Updating state of  the triggers filtered based on composite filter - TC_LBT012_14
   TInt Clbtbctest::TestRlbtSetTriggersState8L( CStifItemParser& /* aItem */ )
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
	 
	 TCoordinate coordinate(62.5285,23.9385);
	 
	 CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
	 CleanupStack::PushL( circle );
	 
	 TUid aManagerUi = TUid::Uid(0x87654321);     
	 // ownership of circle object transferred to the condition object
	 CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
	                                             circle,
	                                             CLbtTriggerConditionArea::EFireOnEnter);
	     
	 CleanupStack::Pop( circle );
	 
	 trig->SetState(CLbtTriggerEntry::EStateDisabled);
	 trig->SetCondition(condition); // ownership transferred to object
	
	 TLbtTriggerId trigId;
	 trig->SetManagerUi(aManagerUi);   
	  //Set Requestor
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
	 CLbtTriggerFilterByAttribute* attribFilter = 
	 CLbtTriggerFilterByAttribute::NewL();
	 CleanupStack::PushL( attribFilter );
	 attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
	 attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
	     
	 //Construct an area filter
	 CLbtTriggerFilterByArea* areaFilter = 
	     CLbtTriggerFilterByArea::NewL();
	 CleanupStack::PushL( areaFilter );
	 CLbtGeoCircle* area = CLbtGeoCircle::NewL( 
	     TCoordinate( 61.5,24.3,0 ),
	     1000 );
	 areaFilter->SetArea( area ); 
	 
	 //Construct a composite filter
	 CLbtTriggerFilterComposite* compositeFilter = 
	     CLbtTriggerFilterComposite::NewL( 
	         CLbtTriggerFilterComposite::ECompositionTypeAnd );
	 
	 CleanupStack::PushL( compositeFilter );
	 
	 //Add filters to the list option object
	 compositeFilter->AddFilterL( areaFilter ); 
	 
	 
	 compositeFilter->AddFilterL( attribFilter ); 
	 
	 lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,compositeFilter);  
	 
	 CleanupStack::Pop( compositeFilter );
	 CleanupStack::Pop( areaFilter ); 
	 CleanupStack::Pop( attribFilter );     
	 
	     
	 lbt.GetTriggersL(trigInfoList );
	 
	 //Do something with the trigger list
	 TInt count = trigInfoList.Count();
	 for ( TInt i = 0; i < count; i++ )
	 {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
        
   
	    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
	    if(State2 !=CLbtTriggerEntry::EStateEnabled)
	    {
	    	return -99;
	    } 
	     
	    TInt Type2 = TrigEntry ->Type();
	    
	    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	
		CLbtGeoAreaBase* area = Condition2->TriggerArea();
		CLbtGeoCircle* circle2 = static_cast <CLbtGeoCircle*>(area);
	
		TCoordinate center2 = circle2->Center();
		TReal32 distance;
		center2.Distance(TCoordinate( 62.5285,23.9385,0 ),distance);
	    if(Type2 ==CLbtTriggerEntry::ETypeStartup && distance==0)
	    {
	    	CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
		    if(State2 !=CLbtTriggerEntry::EStateEnabled)
		    {
		    	return -99;
		    } 
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
  
  TInt Clbtbctest::TestRlbtCancelSetTriggersStateL( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
  	RLbt lbt;
  	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
     
    User::LeaveIfError( lbt.Open(lbtserver));
	CleanupClosePushL( lbt );
	 	 
	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // Set Name
    trig->SetNameL(_L("abc"));
          
    // set condition

    TCoordinate coordinate(62.5285,23.9385);
   // TCoordinate coordinate(62.4438,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
      
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );     
    
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait);
    lbt.CancelSetTriggersState();
          
    TInt error=notifier->iStatus.Int(); 
    
    CleanupStack::PopAndDestroy(filter);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   // CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    
    if(error!=KErrCancel)
    	{
    	return KErrGeneral;
    	}
    
    else 
    	{
    	return KErrNone;	
    	}	
  }               
  
  TInt Clbtbctest::TestRlbtListTriggerIds1L( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	TRAP_IGNORE(lbt.DeleteTriggersL());

      //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
  //  CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );    
    RArray< TLbtTriggerId>  aTriggerIdList;
    
    lbt.ListTriggerIdsL (aTriggerIdList);
       
    
    TInt count = aTriggerIdList.Count();
    TBool found = EFalse;
    for ( TInt i = 0; i < count; i++ )
     {
       	if(trigId == aTriggerIdList[i])
    	{
    	found = ETrue;
    	}
     }
 	if(!found)
 	{
 	aTriggerIdList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return -99;
 	}
    aTriggerIdList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
  }               
  
  TInt Clbtbctest::TestRlbtListTriggerIds2L( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
	RLbt lbt;
	User::LeaveIfError( lbtserver.Connect() );
	CleanupClosePushL( lbtserver );
	User::LeaveIfError( lbt.Open( lbtserver ) );
	CleanupClosePushL( lbt );
	
	TRAP_IGNORE(lbt.DeleteTriggersL());
	
	  //Construct a startup trigger
	CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
	
	//Push to cleanup stack
	  //  CleanupStack::PushL( trig );
	
	// Set Name
	trig->SetNameL(_L("Trigger1"));
	_LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
	
	TSecureId secureid;
	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
	//Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
	// set condition
	
	TCoordinate coordinate(62.5285,23.9385);
	
	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
	CleanupStack::PushL( circle );
	
	TUid aManagerUi = TUid::Uid(0x87654321);     
	// ownership of circle object transferred to the condition object
	CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
	                                            circle,
	                                            CLbtTriggerConditionArea::EFireOnEnter);
	    
	CleanupStack::Pop( circle );
	
	trig->SetCondition(condition); // ownership transferred to object
	
	TLbtTriggerId trigId;
	trig->SetManagerUi(aManagerUi);   
	    
	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
	
	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	    
	notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
	wait->Start( );   
	RArray< TLbtTriggerId>  aTriggerIdList; 
	notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait);
	wait->Start();
	TBool found = EFalse;
	TInt count = aTriggerIdList.Count();
	
	for ( TInt i = 0; i < count; i++ )
	 {
	   	if(trigId == aTriggerIdList[i])
		{
		found = ETrue;
		}
	 }
	if(!found)
	{
	aTriggerIdList.Close();
	CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	return -99;
	}
	aTriggerIdList.Close();
	CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
  }    
  
  //Listing of all the trigger Ids owned by the client synchronously using attribute filter
  TInt Clbtbctest::TestRlbtListTriggerIds3L( CStifItemParser& /* aItem */ )
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
	  
	  TCoordinate coordinate(62.5285,23.9385);
	  
	  CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
	  CleanupStack::PushL( circle );
	  
	  TUid aManagerUi = TUid::Uid(0x87654321);     
	  // ownership of circle object transferred to the condition object
	  CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
	                                              circle,
	                                              CLbtTriggerConditionArea::EFireOnEnter);
	      
	  CleanupStack::Pop( circle );
	  
	  trig->SetState(CLbtTriggerEntry::EStateDisabled);
	  trig->SetCondition(condition); // ownership transferred to object
	
	  TLbtTriggerId trigId;
	  trig->SetManagerUi(aManagerUi);   
	   //Set Requestor
	  CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	  CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	  TBuf<KLbtMaxNameLength> ReqData=_L("");
	  trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
	  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
	  CleanupStack::PushL( notifier );
	  
	  CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	      
	  notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
	  wait->Start( );
	   //Construct list options
	  CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
	  CleanupStack::PushL( listOptions );
	  //Construct an attribute filter
	  CLbtTriggerFilterByAttribute* attribFilter = 
	  CLbtTriggerFilterByAttribute::NewL();
	  CleanupStack::PushL( attribFilter );
	  attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
	  attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeStartup );
	  
	  listOptions->SetFilter( attribFilter ); 
	   
	  CleanupStack::Pop( attribFilter );   
	  RArray< TLbtTriggerId>  aTriggerIdList;
	  
	  lbt.ListTriggerIdsL (aTriggerIdList,listOptions);
	  
	  CleanupStack::PopAndDestroy( listOptions );
	     
	  //Do something with the trigger list
	  
	  
	  TInt count = aTriggerIdList.Count();
	  
	  CleanupStack::PopAndDestroy( notifier );
	  CleanupStack::PopAndDestroy( trig );
	  CleanupStack::PopAndDestroy( &lbt );
	  CleanupStack::PopAndDestroy( &lbtserver );
	  //free memory
	  if(count<0)
		  {
		  return -99;
		  }
	  aTriggerIdList.Close();
	  
	  return KErrNone;
    
  }
  
  //Listing of all the trigger Ids owned by the client synchronously using area filter
  TInt Clbtbctest::TestRlbtListTriggerIds4L( CStifItemParser& /* aItem */ )
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
	  
	  TCoordinate coordinate(62.5285,23.9385);
	  
	  CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
	  CleanupStack::PushL( circle );
	  
	  TUid aManagerUi = TUid::Uid(0x87654321);     
	  // ownership of circle object transferred to the condition object
	  CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
	                                              circle,
	                                              CLbtTriggerConditionArea::EFireOnEnter);
	      
	  CleanupStack::Pop( circle );
	  
	  trig->SetState(CLbtTriggerEntry::EStateDisabled);
	  trig->SetCondition(condition); // ownership transferred to object
	
	  TLbtTriggerId trigId;
	  trig->SetManagerUi(aManagerUi);   
	   //Set Requestor
	  CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	  CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	  TBuf<KLbtMaxNameLength> ReqData=_L("");
	  trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
	  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
	  CleanupStack::PushL( notifier );
	  
	  CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	      
	  notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
	  wait->Start( );
	   //Construct list options
	  CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
	  CleanupStack::PushL( listOptions );
	  //Construct an area filter
	  CLbtTriggerFilterByArea* areaFilter = 
	  CLbtTriggerFilterByArea::NewL();
	  CleanupStack::PushL( areaFilter );
	  CLbtGeoCircle* area = CLbtGeoCircle::NewL( 
	      TCoordinate( 62.5285,23.9385,0 ),
	      1000 );
	  areaFilter->SetArea( area ); 
	  
	  listOptions->SetFilter( areaFilter ); 
	   
	  CleanupStack::Pop( areaFilter );     
	  RArray< TLbtTriggerId>  aTriggerIdList;
	  
	  lbt.ListTriggerIdsL (aTriggerIdList,listOptions);
	  
	  CleanupStack::PopAndDestroy( listOptions );
	
	  TInt count = aTriggerIdList.Count();
	  
	  CleanupStack::PopAndDestroy( notifier );
	  CleanupStack::PopAndDestroy( trig );
	  CleanupStack::PopAndDestroy( &lbt );
	  CleanupStack::PopAndDestroy( &lbtserver );
	  //free memory
	  if(count<0)
		  {
		  return -99;
		  }
	  aTriggerIdList.Close();
	  
	  return KErrNone;
  }
  
  //Listing of trigger ids in given cellular area using area filter synchronously
  TInt Clbtbctest::TestRlbtListTriggerIds5L( CStifItemParser& /* aItem */ )
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
	   
	  RArray< TLbtTriggerId>  aTriggerIdList;
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
	   
	  lbt.ListTriggerIdsL (aTriggerIdList,listOptions);
	  iLog->Log(_L("GetTriggers Complete  "));  
	  CleanupStack::PopAndDestroy( listOptions );
	  TInt count = aTriggerIdList.Count();
	  if(count<0)
		  {
		  return -99;
		  }
	  aTriggerIdList.Close();
	  
	  CleanupStack::PopAndDestroy( notifier );
	  CleanupStack::PopAndDestroy( sessiontrig );
	  CleanupStack::PopAndDestroy( &lbt );
	  CleanupStack::PopAndDestroy( &lbtserver );
	  iLog->Log(_L("Connection closed "));
	  return KErrNone;
    
  }
  
  //Listing of all the trigger Ids owned by the client asynchronously using attribute filter
  TInt Clbtbctest::TestRlbtListTriggerIds6L( CStifItemParser& /* aItem */ )
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
	  
	  TCoordinate coordinate(62.5285,23.9385);
	  
	  CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
	  CleanupStack::PushL( circle );
	  
	  TUid aManagerUi = TUid::Uid(0x87654321);     
	  // ownership of circle object transferred to the condition object
	  CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
	                                              circle,
	                                              CLbtTriggerConditionArea::EFireOnEnter);
	      
	  CleanupStack::Pop( circle );
	  
	  trig->SetState(CLbtTriggerEntry::EStateDisabled);
	  trig->SetCondition(condition); // ownership transferred to object
	
	  TLbtTriggerId trigId;
	  trig->SetManagerUi(aManagerUi);   
	   //Set Requestor
	  CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	  CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	  TBuf<KLbtMaxNameLength> ReqData=_L("");
	  trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
	  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
	  CleanupStack::PushL( notifier );
	  
	  CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	      
	  notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
	  wait->Start( );
	   //Construct list options
	  CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
	  CleanupStack::PushL( listOptions );
	  //Construct an attribute filter
	  CLbtTriggerFilterByAttribute* attribFilter = 
	  CLbtTriggerFilterByAttribute::NewL();
	  CleanupStack::PushL( attribFilter );
	  attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
	  attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeStartup );
	  
	  listOptions->SetFilter( attribFilter ); 
	   
	  CleanupStack::Pop( attribFilter );   
	  RArray< TLbtTriggerId>  aTriggerIdList;
	  
	  notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait,listOptions);
	  wait->Start();
	  
	  CleanupStack::PopAndDestroy( listOptions );
	  
	  TInt count = aTriggerIdList.Count();
	  
	  CleanupStack::PopAndDestroy( notifier );
	  CleanupStack::PopAndDestroy( trig );
	  CleanupStack::PopAndDestroy( &lbt );
	  CleanupStack::PopAndDestroy( &lbtserver );
	  //free memory
	  if(count<0)
		  {
		  return -99;
		  }
	  aTriggerIdList.Close();
	  
	  return KErrNone;
    
  }
  
  //Listing of all the trigger Ids owned by the client asynchronously using area filter
  TInt Clbtbctest::TestRlbtListTriggerIds7L( CStifItemParser& /* aItem */ )
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
	  
	  TCoordinate coordinate(62.5285,23.9385);
	  
	  CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
	  CleanupStack::PushL( circle );
	  
	  TUid aManagerUi = TUid::Uid(0x87654321);     
	  // ownership of circle object transferred to the condition object
	  CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
	                                              circle,
	                                              CLbtTriggerConditionArea::EFireOnEnter);
	      
	  CleanupStack::Pop( circle );
	  
	  trig->SetState(CLbtTriggerEntry::EStateDisabled);
	  trig->SetCondition(condition); // ownership transferred to object
	
	  TLbtTriggerId trigId;
	  trig->SetManagerUi(aManagerUi);   
	   //Set Requestor
	  CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	  CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	  TBuf<KLbtMaxNameLength> ReqData=_L("");
	  trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
	  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
	  CleanupStack::PushL( notifier );
	  
	  CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	      
	  notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
	  wait->Start( );
	   //Construct list options
	  CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
	  CleanupStack::PushL( listOptions );
	  //Construct an area filter
	  CLbtTriggerFilterByArea* areaFilter = 
	      CLbtTriggerFilterByArea::NewL();
	  CleanupStack::PushL( areaFilter );
	  CLbtGeoCircle* area = CLbtGeoCircle::NewL( 
	      TCoordinate( 62.5285,23.9385,0 ),
	      1000 );
	  areaFilter->SetArea( area ); 
	  
	  listOptions->SetFilter( areaFilter ); 
	   
	  CleanupStack::Pop( areaFilter );     
	  RArray< TLbtTriggerId>  aTriggerIdList;
	  
	  notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait,listOptions);
	  wait->Start();
	  
	  CleanupStack::PopAndDestroy( listOptions );
	  
	  TInt count = aTriggerIdList.Count();
	  
	  CleanupStack::PopAndDestroy( notifier );
	  CleanupStack::PopAndDestroy( trig );
	  CleanupStack::PopAndDestroy( &lbt );
	  CleanupStack::PopAndDestroy( &lbtserver );
	  //free memory
	  if(count<0)
		  {
		  return -99;
		  }
	  aTriggerIdList.Close();
	  
	  return KErrNone;
  }
  
  //Listing of trigger ids in given cellular area using area filter asynchronously
  TInt Clbtbctest::TestRlbtListTriggerIds8L( CStifItemParser& /* aItem */ )
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
	   
	   RArray< TLbtTriggerId>  aTriggerIdList;
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
	  notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait,listOptions);
	  wait->Start();
	  iLog->Log(_L("GetTriggers Complete  "));  
	  CleanupStack::PopAndDestroy( listOptions );
	  TInt count = aTriggerIdList.Count();
	  if(count<0)
		  {
		  return -99;
		  }
	  aTriggerIdList.Close();
	  
	  CleanupStack::PopAndDestroy( notifier );
	  CleanupStack::PopAndDestroy( sessiontrig );
	  CleanupStack::PopAndDestroy( &lbt );
	  CleanupStack::PopAndDestroy( &lbtserver );
	  iLog->Log(_L("Connection closed "));
	  return KErrNone;
    
  }
  
  TInt Clbtbctest::TestRlbtCancelListTriggerIdsL( CStifItemParser& /*aItem*/ )
    {
	 RLbtServer lbtserver;
   	 RLbt lbt;
  	
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetNameL(_L("abc"));
          
    // set condition

    TCoordinate coordinate(62.5285,23.9385);
   // TCoordinate coordinate(62.4438,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RArray < TLbtTriggerId > triggerIdArray;
    notifier->ListTriggerIdsList(lbt,triggerIdArray,wait);
    lbt.CancelListTriggerIds();
    			
    TInt error=notifier->iStatus.Int();      
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    
  //  CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt);
    CleanupStack::PopAndDestroy( &lbtserver );
    
   	if(error!=KErrCancel)
    	{
    		return KErrGeneral;
    	}
    
    else 
    	{
    	
    	return KErrNone;	
    	}		
  }               
  //GetTriggersL using NULL filter -synchronous
  TInt Clbtbctest::TestRlbtGetTriggers1L( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    iLog->Log(_L("Before Connect"));
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    iLog->Log(_L("Before Open"));
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
    TRAP_IGNORE(lbt.DeleteTriggersL());	 ;
 	  
    //create another trigger
      //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
  //  CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
   
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    iLog->Log(_L("Before GetTriggersL"));
    lbt.GetTriggersL(trigInfoList);
    iLog->Log(_L("After GetTriggersL"));
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&lbtserver);
    //Do something with the trigger list
   
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    	if(trigId != (TrigEntry->Id()))
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
	
    return KErrNone;  
  }               
  
  //Asynchronous GetTriggers using NULL Filter
  TInt Clbtbctest::TestRlbtGetTriggers2L( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	TRAP_IGNORE(lbt.DeleteTriggersL());	 ;	 
     	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object
		trig->SetState(CLbtTriggerEntry::EStateDisabled);
    TLbtTriggerId trigId;
    trig->SetManagerUi(aManagerUi);   
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(500000);
    notifier->GetTriggersList(lbt,trigInfoList,wait);
    wait->Start();
    //Do something with the trigger list
    
    
    TInt count = trigInfoList.Count();
    if(count<1)
    	{
    		return -99;
    	}
     for ( TInt i = 0; i < count; i++ )
     {
     	CLbtTriggerEntry *TrigEntry = trigInfoList[i]->TriggerEntry();
           	
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
  //synchronous GetTriggersL for cell based triggers
  TInt Clbtbctest::TestRlbtGetTriggers3L( CStifItemParser& /*aItem*/ )
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
  
  //Call asynchronous RLbt::GetTriggers() using attribute filter
  TInt Clbtbctest::TestRlbtGetTriggers4L( CStifItemParser& /* aItem */ )
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
	  
	  //set Requestor     
	  CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	  CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	  TBuf<KLbtMaxNameLength> ReqData=_L("");
	  trig->SetRequestorL(ReqType,ReqFormat,ReqData);
	  // set condition
	  
	  TCoordinate coordinate(62.5285,23.9385);
	  
	  CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
	  CleanupStack::PushL( circle );
	  
	  TUid aManagerUi = TUid::Uid(0x87654321);     
	  // ownership of circle object transferred to the condition object
	  CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
	                                              circle,
	                                              CLbtTriggerConditionArea::EFireOnEnter);
	      
	  CleanupStack::Pop( circle );
	  
	  trig->SetCondition(condition); // ownership transferred to object
	
	  TLbtTriggerId trigId;
	  trig->SetManagerUi(aManagerUi);   
	      
	  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
	  CleanupStack::PushL( notifier );
	  
	  CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	      
	  notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
	  wait->Start( );
	  RPointerArray < CLbtTriggerInfo > trigInfoList;
	  
	  User::After(5000000);
	  //Construct list options
	  CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
	  CleanupStack::PushL( listOptions );
	  
	  //Construct an attribute filter
	  CLbtTriggerFilterByAttribute* attribFilter = 
	  CLbtTriggerFilterByAttribute::NewL();
	  CleanupStack::PushL( attribFilter );
	  attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
	  attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
	  listOptions->SetFilter( attribFilter ); 
	  
	  CleanupStack::Pop( attribFilter );   
	  
	  //lbt.GetTriggersL(trigInfoList,listOptions );
	  notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
	  wait->Start();    
	  
	  //Do something with the trigger list
	  TInt count = trigInfoList.Count();
	  if(count<1)
	  {
	  	return -99;
	  }
	  for ( TInt i = 0; i < count; i++ )
	  {
	  	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
	      //Check Name of the trigger
	  TDesC Name2 =TrigEntry->Name();  
	   if(Name2==KNullDesC) 
	  {
	  	return -99;
	  }
	  //Check State of the trigger  
	  CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
	  if(State2 !=CLbtTriggerEntry::EStateEnabled)
	  {
	  	return -99;
	  } 
	   //Check type of the trigger
	  TInt Type2 = TrigEntry ->Type();
	  if(Type2 !=CLbtTriggerEntry::ETypeSession)
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
			CleanupStack::PopAndDestroy( listOptions );
	  CleanupStack::PopAndDestroy( notifier );
	  CleanupStack::PopAndDestroy( trig );
	  CleanupStack::PopAndDestroy( &lbt );
	  CleanupStack::PopAndDestroy( &lbtserver );
	   return KErrNone; 
  }

//Call asynchronous RLbt::GetTriggers() using area filter
  TInt Clbtbctest::TestRlbtGetTriggers5L( CStifItemParser& /* aItem */ )
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
	  
	  //set Requestor     
	  CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	  CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	  TBuf<KLbtMaxNameLength> ReqData=_L("");
	  trig->SetRequestorL(ReqType,ReqFormat,ReqData);
	  // set condition
	  TCoordinate coordinate(62.5285,23.9385);
	  
	  CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
	  CleanupStack::PushL( circle );
	  
	  TUid aManagerUi = TUid::Uid(0x87654321);     
	  // ownership of circle object transferred to the condition object
	  CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
	                                              circle,
	                                              CLbtTriggerConditionArea::EFireOnEnter);
	      
	  CleanupStack::Pop( circle );
	  
	  trig->SetCondition(condition); // ownership transferred to object
	
	  TLbtTriggerId trigId;
	  trig->SetManagerUi(aManagerUi);   
	      
	  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
	  CleanupStack::PushL( notifier );
	  
	  CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	      
	  notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
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
	  CLbtGeoCircle* area = CLbtGeoCircle::NewL( 
	      TCoordinate( 62.5285,23.9385,0 ),
	      1000 );
	  areaFilter->SetArea( area ); 
	  
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
		if(area ->Type()!=CLbtGeoAreaBase::ECircle )
		{
			return -99;
		}
		
		CLbtGeoCircle* circle2 = static_cast <CLbtGeoCircle*>(area);
	
		TCoordinate center2 = circle2->Center();
		TReal32 distance;
		center2.Distance(TCoordinate( 62.5285,23.9385,0 ),distance);
		if(distance!=0)
		{
			return -99;
		}
		TReal radius = circle2->Radius();
		if(radius != 1000)
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
	  CleanupStack::PopAndDestroy( &lbt );
	  CleanupStack::PopAndDestroy( &lbtserver );
	  return KErrNone;
    
  }
  //Synchronous GettriggersL using attribute filter
  TInt Clbtbctest::TestRlbtGetTriggers6L( CStifItemParser& /* aItem */ )
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
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    trig->SetManagerUi(aManagerUi);   
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);      
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    User::After(5000000);
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    listOptions->SetFilter( attribFilter ); 
    
    CleanupStack::Pop( attribFilter );   
    
    lbt.GetTriggersL(trigInfoList,listOptions );    
    //CleanupStack::PopAndDestroy(4, &lbtserver);

    //Do something with the trigger list
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
        //Check Name of the trigger
    TDesC Name2 =TrigEntry->Name();  
     if(Name2==KNullDesC) 
    {
    	return -99;
    }
    //Check State of the trigger  
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=CLbtTriggerEntry::EStateEnabled)
    {
    	return -99;
    } 
     //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 !=CLbtTriggerEntry::ETypeSession)
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
		CleanupStack::PopAndDestroy( listOptions );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
     return KErrNone; 
	    }

	 //List  triggers based on area filter - synchronous
  TInt Clbtbctest::TestRlbtGetTriggers7L( CStifItemParser& /* aItem */ )
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
    
    TCoordinate coordinate1(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate1,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
     //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    TCoordinate coordinate2(10,20);
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate2,1000);
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
    		circle2,
            CLbtTriggerConditionArea::EFireOnEnter);
    
    trig->SetCondition(condition2); // ownership transferred to object
    
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    // User::After(5000000);
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    CLbtGeoCircle* area = CLbtGeoCircle::NewL( 
        TCoordinate( 62.5285,23.9385,0 ),
        1000 );
    areaFilter->SetArea( area ); 
    
     listOptions->SetFilter( areaFilter ); 
     
     CleanupStack::Pop( areaFilter ); 
     
     lbt.GetTriggersL(trigInfoList,listOptions );
        
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
   //Check area of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	if(area ->Type()!=CLbtGeoAreaBase::ECircle )
	{
		CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
		return -99;
	}
	
	CLbtGeoCircle* circle3 = static_cast <CLbtGeoCircle*>(area);

	TCoordinate center2 = circle3->Center();
	TReal32 distance;
	center2.Distance(TCoordinate( 62.5285,23.9385,0 ),distance);
	if(distance>=1000)
	{
		CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
		return -99;
	}
	TReal radius = circle3->Radius();
	/*if(radius != 1000)
	{
		return -99;
	}*/
    
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
  
  TInt Clbtbctest::TestRlbtCancelGetTriggersL( CStifItemParser& /*aItem*/ )
    {
	 RLbtServer lbtserver;
   	 RLbt lbt;
  	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
         
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetNameL(_L("abc"));
          
    // set condition

    TCoordinate coordinate(62.5285,23.9385);
   // TCoordinate coordinate(62.4438,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
       
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
       
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > triggerIdArray;
    notifier->GetTriggersList(lbt,triggerIdArray,wait);
    lbt.CancelGetTriggers();
    		
    TInt error=notifier->iStatus.Int(); 
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
 	if(error!=KErrCancel)
		{
		return KErrGeneral;
		}
	else 
		{
		return KErrNone;	
		}	
  }          
  
  
  TInt Clbtbctest::TestRlbtCreateGetTriggerIterator1L( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
    
           //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
  //  CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );    
 	    //First create iterator
    lbt.CreateGetTriggerIteratorL();
    
    //Get triggers. Note, the loop will block 
    //active schedular
    CLbtTriggerInfo* trigInfo = lbt.GetNextTriggerLC(); 
    while ( trigInfo != NULL )
        {
        CLbtTriggerEntry* TrigEntry = trigInfo->TriggerEntry();
        TLbtTriggerDynamicInfo * dyninfo = trigInfo -> DynInfo() ;
        CleanupStack::PushL( TrigEntry );
        //do something with entry
    	TDesC Name2 =TrigEntry->Name();  
	     if(Name2==KNullDesC) 
	    {
	    	return -99;
	    }
	    //Check State of the trigger  
	    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
	    if(State2 ==NULL)
	    {
	    	return -99;
	    } 
	     //Check type of the trigger
	    TInt Type2 = TrigEntry ->Type();
	    if(Type2 == NULL)
	    {
	    	return -99;
	    }
	    //Check condition
			CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
		//CLbtTriggerConditionArea::TDirection direction;
		CLbtTriggerConditionArea::TDirection direction = Condition2->Direction();
		if(Condition2==NULL)
		{
			return -99;
		}
			//Check requestors 
	    RRequestorStack   aRequestors,aRequestors2 ;
	    TrigEntry->GetRequestorsL(aRequestors2);
	   // trig->GetRequestorsL(aRequestors);
	    for(TInt i=0;i<aRequestors2.Count();i++)
	    {
	    	CRequestor *requestor = aRequestors2[i];
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
	  
	    //Check ManagerUI
	    TUid Managerui2 = TrigEntry->ManagerUi();
	   /* if(Managerui2!=KNullUid)
	    {
	    	return -99;
	    }*/
	    
	     //Check validity
		 TLbtTriggerDynamicInfo::TLbtTriggerValidity validity;
	   validity= dyninfo -> iValidity;
	   if(validity!=TLbtTriggerDynamicInfo::EValid)
	   {
	   	return -99;
	   }
	
    CleanupStack::Pop( TrigEntry );
    CleanupStack::PopAndDestroy( trigInfo );
    trigInfo = lbt.GetNextTriggerLC();
        };
  
	CleanupStack::PopAndDestroy( trigInfo);
	CleanupStack::PopAndDestroy( notifier);
	CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone;
  }               
  
  TInt Clbtbctest::TestRlbtCreateGetTriggerIterator2L( CStifItemParser& /*aItem*/ )
    {
	RLbtServer server;
    RLbt lbt;
   
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    
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
    CLbtTriggerInfo* trigInfo = lbt.GetNextTriggerLC(); 
    TInt i=1;
    while ( trigInfo != NULL )
        {
        	i++;
          triggers.Append(*trigInfo);
        //  CleanupStack::PopAndDestroy(trigInfo);
          trigInfo = lbt.GetNextTriggerLC(); 
        }
          
    //CLbtTriggerEntry* entry=trigInfo->TriggerEntry();
      
    AssertTrueL( trigId==triggers[i-2].TriggerEntry()->Id(),_L("Wrong id"));    
        
    CleanupStack::PopAndDestroy(i);
    CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
    
    triggers.Close();
        
    return KErrNone;
  }     
  
   //Getting trigger information incrementally without creating Iterator
      TInt Clbtbctest::TestRlbtCreateGetTriggerIterator3L( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	    
    //Get triggers. Note, the loop will block 
    //active schedular
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtIteratorNotCreated); 
    CLbtTriggerInfo* trigInfo = lbt.GetNextTriggerLC(); 
   
   
	  	CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
      return KErrNone;
    }          
  
  TInt Clbtbctest::TestRlbtCancelCreateTriggerIteratorL( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
  	RLbt lbt;
  	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
    User::LeaveIfError( lbt.Open(lbtserver));
 	CleanupClosePushL( lbt );
 	 
	    //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetNameL(_L("abc"));
          
    // set condition

    TCoordinate coordinate(62.5285,23.9385);
   // TCoordinate coordinate(62.4438,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    notifier->CreateIterator(lbt,wait);
    lbt.CancelCreateTriggerIterator();
    		
    TInt error=notifier->iStatus.Int(); 
   
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   
   // CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(error!=KErrCancel)
    	{
    	return KErrGeneral;
    	}
    else 
	   	{
	   	return KErrNone;	
	   	}	
  }               
  
  TInt Clbtbctest::TestRlbtNotifyTriggerChangeEventL( CStifItemParser& /*aItem*/ )
    {
   	RLbtServer server;
    RLbt lbt1,lbt2;
    
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt1.Open(server));
    CleanupClosePushL( lbt1 );
    
    User::LeaveIfError(lbt2.Open(server));
    CleanupClosePushL( lbt2 );
    
    
    CLbtEventObserver* notifier1= CLbtEventObserver::NewL( lbt1);
    CleanupStack::PushL( notifier1 );
    //notifier->After(50000000); 
    
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
   // TLbtTriggerChangeEvent   aEvent ;
    
  
      //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   
	TSecureId secureid;
	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);   
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
		CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
		TBuf<KLbtMaxNameLength> ReqData=_L("");
		trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    // set condition
    
    TCoordinate coordinate(61.5285,23.9385,0);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2 );
    CleanupStack::PushL( notifier2);
    notifier2->CreateTriggers( lbt2,*trig,trigId,ETrue,wait2 );
    wait2->Start( );
    TLbtTriggerChangeEvent   aEvent ;
    notifier1->NotifyTriggerChangeEvent(lbt1,aEvent,wait1);
    // notifier2->After(1000000);
   // wait2->Start( );
    User::After(10000);
    lbt2.DeleteTriggerL(trigId);
    if(aEvent.iEventType == ELbtTriggerChangeEventCreated)
	    {
	    	iLog->Log(_L("Trigger Created"));
	    }
	    else if(aEvent.iEventType == ELbtTriggerChangeEventDeleted)
	    {
	    	iLog->Log(_L("Trigger Deleted"));
	    }
	    else if(aEvent.iEventType == ELbtTriggerChangeEventUpdated)
	    {
	    	iLog->Log(_L("Trigger Modified"));
	    }
	    else if(aEvent.iEventType == ELbtTriggerChangeEventMultiple)
	    {
	    	iLog->Log(_L("Multiple Events"));
	    }
    wait1->Start( );
   	    
    CleanupStack::PopAndDestroy(notifier2);
   
	CleanupStack::PopAndDestroy(trig);
	CleanupStack::PopAndDestroy(notifier1);
    //CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt2);
    CleanupStack::PopAndDestroy(&lbt1);
    CleanupStack::PopAndDestroy(&server);
 
    return KErrNone;
	
  }               
  
  TInt Clbtbctest::TestRlbtNotifyTriggerChangeEvent2L( CStifItemParser& /*aItem*/ )
    {
   	RLbtServer server;
    RLbt lbt1,lbt2;
    
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt1.Open(server));
    CleanupClosePushL( lbt1 );
    
    User::LeaveIfError(lbt2.Open(server));
    CleanupClosePushL( lbt2 );
    
    
    CLbtEventObserver* notifier1= CLbtEventObserver::NewL( lbt1);
    CleanupStack::PushL( notifier1 );
    //notifier->After(50000000); 
    
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
   // TLbtTriggerChangeEvent   aEvent ;
    
  
      //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   
	TSecureId secureid;
	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);   
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    // set condition
    
    TCoordinate coordinate(61.5285,23.9385,0);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2 );
    CleanupStack::PushL( notifier2);
    notifier2->CreateTriggers( lbt2,*trig,trigId,ETrue,wait2 );
    wait2->Start( );
    TLbtTriggerChangeEvent   aEvent ;
    notifier1->NotifyTriggerChangeEvent(lbt1,aEvent,wait1);
    lbt1.CancelNotifyTriggerChangeEvent();
   	    
    CleanupStack::PopAndDestroy(notifier2);
   

 	
 	TInt error=notifier1->iStatus.Int(); 
 		CleanupStack::PopAndDestroy(trig);
	CleanupStack::PopAndDestroy(notifier1);
    //CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt2);
    CleanupStack::PopAndDestroy(&lbt1);
    CleanupStack::PopAndDestroy(&server);
    if(error!=KErrCancel)
		{
		return KErrGeneral;
		}
    else 
    	{
    
    	return KErrNone;	
    	}	
  }    
  
  TInt Clbtbctest::TestRlbtNotifyTriggerFiredL( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
  	RLbt lbt;
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open(lbtserver));
 	CleanupClosePushL( lbt );
 	 
 	   //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetNameL(_L("abc"));
          
    // set condition

    TCoordinate coordinate(62.5285,23.9385);
   // TCoordinate coordinate(62.4438,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerFireInfo triggerFireInfo;
    
    //TRequestStatus requestStatus( KErrNone );      
    notifier->NotifyTriggerFired(lbt,triggerFireInfo,wait);
    //wait->Start( );
            
    RTimer timer;
    timer.CreateLocal();
    TRequestStatus tstatus;
    timer.After( tstatus, 5000000 );
    User::WaitForRequest( tstatus );
    timer.Close();
    TInt error=notifier->iStatus.Int(); 
 
  	lbt.CancelNotifyTriggerFired();
  	notifier->CancelNotification();
   
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone;	
   
  }                         
  
  TInt Clbtbctest::TestRlbtNotifyTriggerFired2L( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
  	RLbt lbt;
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open(lbtserver));
 	CleanupClosePushL( lbt );
 	 
 	   //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetNameL(_L("abc"));
          
    // set condition

    TCoordinate coordinate(62.5285,23.9385);
   // TCoordinate coordinate(62.4438,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerFireInfo triggerFireInfo;
    
    //TRequestStatus requestStatus( KErrNone );      
    notifier->NotifyTriggerFired(lbt,triggerFireInfo,wait);
    //wait->Start( );
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtRequestsNotCancelled);    
    lbt.Close();    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  	
    return KErrNone;	
   
  }                         
  
  TInt Clbtbctest::TestRlbtCancelNotifyTriggerFiredL( CStifItemParser& /*aItem*/ )
    {
	RLbtServer lbtserver;
  	RLbt lbt;
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open(lbtserver));
 	CleanupClosePushL( lbt );
 	 
 	   //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetNameL(_L("abc"));
          
    // set condition

    TCoordinate coordinate(62.5285,23.9385);
   // TCoordinate coordinate(62.4438,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    TLbtTriggerFireInfo triggerFireInfo;
    
    //TRequestStatus requestStatus( KErrNone );      
    notifier->NotifyTriggerFired(lbt,triggerFireInfo,wait);
    //wait->Start( );
    lbt.CancelNotifyTriggerFired();        
    
    TInt error=notifier->iStatus.Int(); 
	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if( KErrCancel==error)
    	{
    	return KErrNone;	
    	
    	} 
    else
    	{
    	return -99;
      }	
  }       
  
  TInt Clbtbctest::TestRlbtGetFiredTriggersL( CStifItemParser& /*aItem*/ )
   {
	 
    _LIT( KSimulationFile,"c:\\system\\data\\simu_move1.sps" );
 
 	RLbtServer lbtserver;
 	RLbt lbt;
 	iLog->Log(_L("Before connecting"));
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open(lbtserver));
	CleanupClosePushL( lbt );
 	CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	
	User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	iLog->Log(_L("Simulation input file set "));
	CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
  	//Push to cleanup stack
  	CleanupStack::PushL( trig );
  	iLog->Log(_L("Startup Trigger Entry Created "));
  
  	// Set Name
  	trig->SetNameL(_L("Trigger1"));
  	trig->SetNameL(_L("abc"));
  	_LIT( KMyTriggerHandlingProcessName, "About.exe");
 	
  	TSecureId secureid;
  	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    	//set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    	// set condition

  	TCoordinate coordinate(62.5285,23.9385);
 	// TCoordinate coordinate(62.4438,23.9385);
  
 	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
  	CleanupStack::PushL( circle );
  
       
  	// ownership of circle object transferred to the condition object
  	CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                              circle,
                                              CLbtTriggerConditionArea::EFireOnEnter);
      
  	CleanupStack::Pop( circle );
  
  	trig->SetCondition(condition); // ownership transferred to object

  	TLbtTriggerId trigId;
      
      
  	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
  	CleanupStack::PushL( notifier );
  
  	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
      
  	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
  	wait->Start( );
  	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,500);
  	circle2->SetRadius(500);
  	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                              circle2,
                                              CLbtTriggerConditionArea::EFireOnEnter);
  	
  	trig->SetCondition(condition2);
  	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
  	wait->Start( );
  	
	RArray<TLbtTriggerFireInfo> Firedtriggers;
	lbt.GetFiredTriggersL(Firedtriggers);
	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone;
	}                 
	
	TInt Clbtbctest::TestRlbtNotifyTriggeringSystemSettingChangeL( CStifItemParser& /*aItem*/ )
   {
	 RLbtServer server;
	 RLbt lbt;
	 	 
	 User::LeaveIfError(server.Connect());
	 CleanupClosePushL( server );
 	 
     User::LeaveIfError(lbt.Open(server));
     CleanupClosePushL( lbt );
 	 
     TCoordinate coordinate;
     TLbtTriggeringSystemSettings settings,dummySettings;
     
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
     CleanupStack::PushL( notifier );
     
     CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt);
     CleanupStack::PushL( notifier2 );
     
     CActiveSchedulerWait *waitLoop = new(ELeave) CActiveSchedulerWait;
     CActiveSchedulerWait *waitLoop2 = new(ELeave) CActiveSchedulerWait;
     notifier->NotifyTriggeringSettingsChange( lbt,settings,dummySettings,waitLoop ); 
     RTimer timer;
     timer.CreateLocal();
     TRequestStatus tstatus;
     timer.After( tstatus, 5000000 );
     User::WaitForRequest( tstatus );
     timer.Close();
     TInt error=notifier->iStatus.Int(); 
     lbt.CancelNotifyTriggeringSystemSettingChange();   
    // waitLoop->Start();
     CleanupStack::PopAndDestroy( notifier2 );
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy(&lbt);
     //CleanupStack::PopAndDestroy(&mgrclient);
     CleanupStack::PopAndDestroy(&server);
     
     return KErrNone;
	}                                   
	
	TInt Clbtbctest::TestRlbtCancelNotifyTriggeringSystemSettingChangeL( CStifItemParser& /*aItem*/ )
   {
	 RLbtServer server;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError(server.Connect());
 	 CleanupClosePushL( server );
 	 
     User::LeaveIfError(lbt.Open(server));
     CleanupClosePushL( lbt );
 	 
     TCoordinate coordinate;
     TLbtTriggeringSystemSettings settings,dummySettings;
     
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
     CleanupStack::PushL( notifier );
     
     CActiveSchedulerWait *waitLoop = new(ELeave) CActiveSchedulerWait;
     notifier->NotifyTriggeringSettingsChange( lbt,settings,dummySettings,waitLoop ); 
     RTimer timer;
     timer.CreateLocal();
     TRequestStatus tstatus;
     timer.After( tstatus, 5000000 );
     User::WaitForRequest( tstatus );
     timer.Close();
     TInt error=notifier->iStatus.Int(); 
     lbt.CancelNotifyTriggeringSystemSettingChange();   
     error=notifier->iStatus.Int(); 
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy(&lbt);
     //CleanupStack::PopAndDestroy(&mgrclient);
     CleanupStack::PopAndDestroy(&server);
     
     if( KErrCancel==error)
    	{
    	return KErrNone;	
    	
    	} 
    	else
    	{
    	return -99;
      }	
	}                 
	
	TInt Clbtbctest::TestRlbtGetTriggeringSystemSettingsL( CStifItemParser& /*aItem*/ )
   {
	RLbtServer server;
    RLbt lbt;
    TLbtTriggeringSystemSettings settings;
    
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt.Open(server));
    CleanupClosePushL( lbt );
    
    lbt.GetTriggeringSystemSettingsL(settings);
    
    CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&server);
   
    return KErrNone; 
   } 
   
   
   TInt Clbtbctest::TestRlbtCancelAllL( CStifItemParser& /*aItem*/ )
   {
	RLbtServer server;
    RLbt lbt1,lbt2;
   
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt1.Open(server));
    CleanupClosePushL( lbt1 );
    
    User::LeaveIfError(lbt2.Open(server));
    CleanupClosePushL( lbt2 );
    
    
    CLbtEventObserver* notifier1= CLbtEventObserver::NewL( lbt1);
    CleanupStack::PushL( notifier1 );
    //notifier->After(50000000); 
    
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
      
      //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   
	TSecureId secureid;
	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);   
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  

    // set condition
    
    
    TCoordinate coordinate(61.5285,23.9385,0);
          
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2 );
    CleanupStack::PushL( notifier2);
    notifier2->CreateTriggers( lbt2,*trig,trigId,ETrue,wait2 );
    wait2->Start( );
    
   	notifier1->NotifyTriggeringsettingsChangeEventL(lbt1,wait1);
   	lbt1.CancelAll(); 
   	notifier1->TriggeringsettingsChangeEventDestructor();
    CleanupStack::PopAndDestroy(notifier2);
   
	CleanupStack::PopAndDestroy(trig);
	CleanupStack::PopAndDestroy(notifier1);
    //CleanupStack::PopAndDestroy(&mgrclient);
    CleanupStack::PopAndDestroy(&lbt2);
    CleanupStack::PopAndDestroy(&lbt1);
    CleanupStack::PopAndDestroy(&server);
 
    return KErrNone;
	    
   } 
   
   TInt Clbtbctest::TestCLbtGeoCell1L( CStifItemParser& aItem )
   {
   TTestlbtclasses classtest(iLog);
   return classtest.TestCLbtGeoCell1L(aItem); 
   } 
   
    TInt Clbtbctest::TestCLbtGeoCell2L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCell2L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtGeoCell3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCell3L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtGeoCell4L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCell4L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtGeoCell5L( CStifItemParser& aItem )
   {
   	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCell5L(aItem); 
   } 
   TInt Clbtbctest::TestCLbtGeoCell6L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCell6L(aItem); 
   } 
   
    TInt Clbtbctest::TestCLbtGeoCell7L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCell7L(aItem); 
   } 
   
    TInt Clbtbctest::TestCLbtGeoCircle1L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCircle1L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtGeoCircle2L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCircle2L(aItem); 
   } 
   
   
     TInt Clbtbctest::TestCLbtGeoCircle3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCircle3L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtGeoCircle4L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCircle4L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtGeoCircle5L( CStifItemParser& aItem )
   {
	   TTestlbtclasses classtest(iLog);
	   return classtest.TestCLbtGeoCircle5L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtGeoCircle6L( CStifItemParser& aItem )
   {
   	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoCircle6L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtGeoRect1L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoRect1L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtGeoRect2L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoRect2L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtGeoRect3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoRect3L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtGeoRect4L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoRect4L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtGeoRect5L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtGeoRect5L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtListTriggerOptions1L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtListTriggerOptions1L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtListTriggerOptions2L( CStifItemParser& aItem )
   {
		 TTestlbtclasses classtest(iLog);
	   return classtest.TestCLbtListTriggerOptions2L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtListTriggerOptions3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtListTriggerOptions3L(aItem); 
   } 
    TInt Clbtbctest::TestCLbtListTriggerOptions4L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtListTriggerOptions4L(aItem); 
   } 
    TInt Clbtbctest::TestCLbtListTriggerOptions5L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtListTriggerOptions5L(aItem); 
   } 
    TInt Clbtbctest::TestCLbtListTriggerOptions6L( CStifItemParser& aItem )
   {
   	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtListTriggerOptions6L(aItem); 
   } 
   
    TInt Clbtbctest::TestRLbtServerL( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestRLbtServerL(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtSessionTrigger1L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtSessionTrigger1L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtSessionTrigger2L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtSessionTrigger2L(aItem); 
   } 

  	TInt Clbtbctest::TestCLbtSessionTrigger3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtSessionTrigger3L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtSessionTrigger4L( CStifItemParser& aItem )
   {
   	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtSessionTrigger4L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtSessionTrigger5L( CStifItemParser& aItem )
   {
   	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtSessionTrigger5L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtStartupTrigger1L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtStartupTrigger1L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtStartupTrigger2L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtStartupTrigger2L(aItem); 
   } 
   
     TInt Clbtbctest::TestCLbtStartupTrigger3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtStartupTrigger3L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtStartupTrigger4L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtStartupTrigger4L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtStartupTrigger5L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtStartupTrigger5L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtTriggerChangeEventNotifierL( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerChangeEventNotifierL(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtTriggerConditionArea1L( CStifItemParser& aItem )
   {
   	TTestlbtclasses classtest(iLog);
   	return classtest.TestCLbtTriggerConditionArea1L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtTriggerConditionArea2L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerConditionArea2L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtTriggerConditionArea3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerConditionArea3L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtTriggerConditionArea4L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerConditionArea4L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtTriggerConditionArea5L( CStifItemParser& aItem )
   {
    TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerConditionArea5L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtTriggerConditionArea6L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerConditionArea6L(aItem); 
   } 
   TInt Clbtbctest::TestCLbtTriggerConditionArea7L( CStifItemParser& aItem )
   {
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument);
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerConditionArea7L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtTriggerConditionArea8L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerConditionArea8L(aItem); 
   } 
   
   TInt Clbtbctest::TestCLbtTriggerEntry1L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerEntry1L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerEntry2L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerEntry2L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterByArea1L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterByArea1L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterByArea2L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterByArea2L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterByArea3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterByArea3L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterByArea4L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterByArea4L(aItem); 
   }
   TInt Clbtbctest::TestCLbtTriggerFilterByArea5L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterByArea5L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterByArea6L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterByArea6L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterByArea7L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterByArea7L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterByArea8L( CStifItemParser& aItem )
   {
   	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterByArea8L(aItem); 
   }
	   
   TInt Clbtbctest::TestCLbtTriggerFilterByAttribute1L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterByAttribute1L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterByAttribute2L( CStifItemParser& aItem )
   {
		 TTestlbtclasses classtest(iLog);
	   return classtest.TestCLbtTriggerFilterByAttribute2L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterByAttribute3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterByAttribute3L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterComposite1L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterComposite1L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterComposite2L( CStifItemParser& aItem )
   {
    TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterComposite2L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFilterComposite3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterComposite3L(aItem); 
   }
   TInt Clbtbctest::TestCLbtTriggerFilterComposite4L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFilterComposite4L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerFiringEventNotifierL( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerFiringEventNotifierL(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerInfo1L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerInfo1L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerInfo2L( CStifItemParser& aItem )
   {
		 TTestlbtclasses classtest(iLog);
	   return classtest.TestCLbtTriggerInfo2L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerInfo3L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerInfo3L(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggerInfo4L( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestCLbtTriggerInfo4L(aItem); 
   }
   
   TInt Clbtbctest::TestTLbtTriggeringSystemSettingsL( CStifItemParser& aItem )
   {
	TTestlbtclasses classtest(iLog);
	return classtest.TestTLbtTriggeringSystemSettingsL(aItem); 
   }
   
   TInt Clbtbctest::TestCLbtTriggeringSystemSettingsChangeEventNotifier1L( CStifItemParser& aItem )
   {
   	TTestlbtclasses classtest(iLog);
   	return classtest.TestCLbtTriggeringSystemSettingsChangeEventNotifier1L(aItem); 
   }
   
    TInt Clbtbctest::TestRLbtAsyncUpdate1L( CStifItemParser& /* aItem */ )
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
    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
   // set commandline
   trig->SetCommandLineL(_L("commandline1"));
    
       
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    trig->SetManagerUi(aManagerUi);   
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
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
    newtrig->SetCommandLineL(_L("Commandline2" ));
    TCoordinate newcoordinate(10,10);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,500);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition
    |CLbtTriggerEntry::EAttributeStartUpCommandLine,wait,ELbtTrue
    );
    wait->Start( );
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
	CLbtTriggerConditionArea::TDirection direction;
	direction = Condition2->Direction();
	if(direction!=CLbtTriggerConditionArea::EFireOnExit)
	{
		return -99;
	}
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	if(area ->Type()!=CLbtGeoAreaBase::ECircle )
	{
		return -99;
	}
	
	CLbtGeoCircle* circle2 = static_cast <CLbtGeoCircle*>(area);
	TCoordinate center = newcircle->Center();
	TCoordinate center2 = circle2->Center();
	TReal32 distance;
	center2.Distance(center,distance);
	if(distance!=0)
	{
		return -99;
	}
	TReal radius = circle2->Radius();
	if(radius != newcircle->Radius())
	{
		return -99;
	}
	if((static_cast <CLbtStartupTrigger*>(TrigEntry))->CommandLine()!=newtrig->CommandLine()) 
    {
    	return -99;
    }
	

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
   // delete wait;
   
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
       //Trying to update non modifiable trigger attributes
  TInt Clbtbctest::TestRLbtAsyncUpdate2L( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError(lbt.Open(lbtserver));
	CleanupClosePushL( lbt);
	
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier ); 
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;       
    
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
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
    newtrig->SetId(1);
   // TestModuleIf().SetExitReason( CTestModuleIf::EPanic,-21); 
 	    
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeId
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,wait,ELbtTrue);
    wait->Start();
    TInt retval = notifier->iStatus.Int();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    
    return retval;
   
  } 
  
  //Trying to update condition with NULL condition
  TInt Clbtbctest::TestRLbtAsyncUpdate3L( CStifItemParser& /*aItem*/ )
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
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
   
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    TLbtTriggerId trigId;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
   
  //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    newtrig->SetCondition(NULL);
    newtrig->SetId(trigId);
  //TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
   /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,wait,ELbtTrue);
    wait->Start();
   
    TInt retval = notifier->iStatus.Int();
  // lbt.UpdateTriggerL(*newtrig);
    iLog->Log(_L("After UpdateTriggerL "));
   
 
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
  // #endif
    CleanupStack::Pop( &lbt );
    lbt.Close();
   
    CleanupStack::PopAndDestroy( &lbtserver );
  // //delete wait;
  
    iLog->Log(_L("Test passed "));
    return retval; 
   
  } 
  
  //Trying to update trigger with NULL trigger area
  TInt Clbtbctest::TestRLbtAsyncUpdate4L( CStifItemParser& /*aItem*/ )
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

	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier ); 
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;       
	   
  //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    CleanupStack::PushL( newtrig );
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    newtrig->SetCommandLineL(_L("Commandline2" ));
    TCoordinate newcoordinate(10,10);
         
   // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL();
//   newcondition->SetTriggerArea(NULL)   ;
  
    newtrig->SetCondition(newcondition); // ownership transferred to object
	TLbtTriggerId trigId = 1;
    newtrig->SetId(trigId);
  
   /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition
    |CLbtTriggerEntry::EAttributeStartUpCommandLine,wait,ELbtTrue
   );
  
    wait->Start();
    TInt retval = notifier->iStatus.Int();
    
    CleanupStack::PopAndDestroy( newtrig );
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( &lbt );
    lbt.Close();
   
    CleanupStack::PopAndDestroy( &lbtserver );
  // delete wait;
  
    iLog->Log(_L("Test passed "));
    return retval; 
   
  } 
  
  //Trying to update trigger handling process
  TInt Clbtbctest::TestRLbtAsyncUpdate5L( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError(lbt.Open(lbtserver));
	CleanupClosePushL( lbt);
	
	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier ); 
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;       
    
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    _LIT( KMyTriggerHandlingProcessName, "LbtReference.exe" );
    TSecureId secureid;
    newtrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
   // set commandline
    newtrig->SetCommandLineL(_L("commandline1"));
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,500);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(1);
   // TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 21); 
 	    
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeStartUpProcessId,wait,ELbtTrue);
    wait->Start();
    TInt retval = notifier->iStatus.Int();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
      return retval;
   
  } 
  
  
  //Trying to update trigger with commandline parameter length 0
  TInt Clbtbctest::TestRLbtAsyncUpdate6L( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError(lbt.Open(lbtserver));
	CleanupClosePushL( lbt);
	
	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier ); 
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;       
    
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    _LIT( KMyTriggerHandlingProcessName, "LbtReference.exe" );
    TSecureId secureid;
    newtrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
   // set commandline
    newtrig->SetCommandLineL(_L(""));
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,500);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(1);
   // TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
 	    
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeStartUpCommandLine,wait,ELbtTrue);
    wait->Start();
    TInt retval = notifier->iStatus.Int();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return retval;
   
  } 
  //Trying to update trigger name with zero length name
  TInt Clbtbctest::TestRLbtAsyncUpdate7L( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError(lbt.Open(lbtserver));
	CleanupClosePushL( lbt);
	 
	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier ); 
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;       
    
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L(""));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
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
    newtrig->SetId(1);
   // TestModuleIf().SetExitReason( CTestModuleIf::EPanic,-21); 
 	    
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,wait,ELbtTrue);
    wait->Start();
    TInt retval = notifier->iStatus.Int();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return retval;
	} 
	
	TInt Clbtbctest::TestRLbtCancelUpdateTriggerL( CStifItemParser& /* aItem */ )
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
    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
   // set commandline
   	trig->SetCommandLineL(_L("commandline1"));
    
       
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    //Set Requestor
	 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
		CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
		TBuf<KLbtMaxNameLength> ReqData=_L("");
		trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    trig->SetManagerUi(aManagerUi);   
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
     TLbtTriggerId trigId;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    newtrig->SetCommandLineL(_L("Commandline2" ));
    TCoordinate newcoordinate(10,10);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,500);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition
    |CLbtTriggerEntry::EAttributeStartUpCommandLine,wait,ELbtTrue
    );
    lbt.CancelUpdateTrigger();
     	
		TInt error=notifier->iStatus.Int();    
    
    //CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   // #endif
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
  	if(error!=KErrCancel)
    		{
    		return KErrGeneral;
    		}
    return KErrNone;
      
    }




// -----------------------------------------------------------------------------
// Clbtbctest::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt Clbtbctest::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
