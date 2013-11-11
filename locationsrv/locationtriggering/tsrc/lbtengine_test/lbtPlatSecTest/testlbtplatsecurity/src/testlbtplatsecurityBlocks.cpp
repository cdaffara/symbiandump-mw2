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
* Description:    Test module to implement Platform security related test cases
*
*/





// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "testlbtplatsecurity.h"
#include "t_triggerfireobserver.h"
#include <lbt.h> 
#include <lbtserver.h>
#include <e32property.h>
#include <lbtgeocircle.h> 
#include <lbtgeorect.h>
#include <lbttriggerconditionarea.h> 
#include <lbtsessiontrigger.h> 
#include <lbttriggerfilterbyattribute.h>
#include <lbttriggerchangeevent.h>



#include <lbtmanager.h> 
#include <lbttriggeringsystemsettings.h>
#include <lbttriggeringsystemmanagementsettings.h>
#include <lbttriggeringsystemstatus.h>

#include <flogger.h>
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ctestlbtplatsecurity::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ctestlbtplatsecurity::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ctestlbtplatsecurity::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ctestlbtplatsecurity::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", Ctestlbtplatsecurity::ExampleL ),
        ENTRY( "testCreateTrigger", Ctestlbtplatsecurity::testCreateTriggerL ),
        
        ENTRY( "testDeleteTrigger", Ctestlbtplatsecurity::testDeleteTriggerL ),
        ENTRY( "testDeleteTriggersSync", Ctestlbtplatsecurity::testDeleteTriggers1L ),
        ENTRY( "testDeleteTriggersAsync", Ctestlbtplatsecurity::testDeleteTriggers2L ),
        ENTRY( "testDeleteTriggersListSync", Ctestlbtplatsecurity::testDeleteTriggers3L ),
        ENTRY( "testDeleteTriggersListAsync", Ctestlbtplatsecurity::testDeleteTriggers4L ),
        
        ENTRY( "testGetTriggerLC", Ctestlbtplatsecurity::testGetTriggerLC ),
        ENTRY( "testUpdateTriggerL", Ctestlbtplatsecurity::testUpdateTriggerL ),
        ENTRY( "testSetTriggerStateL", Ctestlbtplatsecurity::testSetTriggerStateL ),
        ENTRY( "testSetTriggersStateLSync", Ctestlbtplatsecurity::testSetTriggersStateL ),
        ENTRY( "testSetTriggersStateLAsync", Ctestlbtplatsecurity::testSetTriggersState1L ),
       
        ENTRY( "testListTriggerIdsLSync", Ctestlbtplatsecurity::testListTriggerIdsL ),
       // 
        ENTRY( "testListTriggerIdsAsync", Ctestlbtplatsecurity::testListTriggerIds1L ),
      //  
        ENTRY( "testGetTriggersL", Ctestlbtplatsecurity::testGetTriggersL ),
      //  
        ENTRY( "testGetTriggers", Ctestlbtplatsecurity::testGetTriggers_test2L ),
      //  
        
       
        ENTRY( "testCreateGetTriggerIteratorL", Ctestlbtplatsecurity::testCreateGetTriggerIteratorL ),
        ENTRY( "testCreateGetTriggerIterator", Ctestlbtplatsecurity::testCreateGetTriggerIterator_test2L ),
       
        ENTRY( "testGetNextTriggerLC", Ctestlbtplatsecurity::testGetNextTriggerLC ),
        
        ENTRY( "testNotifyTriggerChangeEvent", Ctestlbtplatsecurity::testNotifyTriggerChangeEventL ),
       
        ENTRY( "testNotifyTriggerFired", Ctestlbtplatsecurity::testNotifyTriggerFiredL ),
       
        ENTRY( "testGetFiredTriggersL", Ctestlbtplatsecurity::testGetFiredTriggersL ),
        
        ENTRY( "testNotifyTriggeringSystemSettingChange", Ctestlbtplatsecurity::testNotifyTriggeringSystemSettingChangeL ),
       
        ENTRY( "testGetTriggeringSystemSettingsL", Ctestlbtplatsecurity::testGetTriggeringSystemSettingsL ),
      
        ENTRY( "testSetTriggeringSystemSettingsL", Ctestlbtplatsecurity::testSetTriggeringSystemSettingsL ),
       // #if 0
        ENTRY( "testSetTriggeringMechanismStateL", Ctestlbtplatsecurity::testSetTriggeringMechanismStateL ),
        ENTRY( "testGetTriggeringSystemStatusL", Ctestlbtplatsecurity::testGetTriggeringSystemStatusL ),
        ENTRY( "testGetTriggerManagerUiInfoL", Ctestlbtplatsecurity::testGetTriggerManagerUiInfoL ),
        ENTRY( "testNotifyTriggeringSystemStatusChange", Ctestlbtplatsecurity::testNotifyTriggeringSystemStatusChangeL ),
      //
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Ctestlbtplatsecurity::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestlbtplatsecurity::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Ktestlbtplatsecurity, "testlbtplatsecurity" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Ktestlbtplatsecurity, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Ktestlbtplatsecurity, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testCreateTrigger
// This test case will test CreateTriggerL for platform security in client library.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testCreateTriggerL(CStifItemParser& /*aItem*/ )
   {
 	
 	iLog->Log(_L("Entering testCreateTriggerL"));
   	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connected to server"));
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("subsession opened"));
 	 	 
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
      //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger created"));	
	TInt error=notifier->iStatus.Int();    
   
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;
	
   }

// -------------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testDeleteTriggerL
// This test case will test DeleteTriggerL(given trigger id) for platform security in client library.
// (other items were commented in a header).
// -------------------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testDeleteTriggerL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
        
	 TRAPD(error,lbt.DeleteTriggerL(trigId));
   
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
   if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }
   
// -------------------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testDeleteTriggers1L
// This test case will test DeleteTriggerL(given attribute filter) for platform security in client library.
// (other items were commented in a header).
// --------------------------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testDeleteTriggers1L(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
        
    TRAPD(error,lbt.DeleteTriggersL());
   
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }
 
// -------------------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testDeleteTriggers2L
// This test case will test DeleteTriggers(given attribute filter) for platform security in client library.
// (other items were commented in a header).
// -------------------------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testDeleteTriggers2L(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter ); 
    
    notifier->DeleteTriggers(lbt,filter,wait);
   	wait->Start( );
   	TInt error=notifier->iStatus.Int(); 
   	
   	CleanupStack::PopAndDestroy( filter );
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }

// --------------------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testDeleteTriggers3L
// This test case will test DeleteTriggersL(given trigger id array) for platform security in client library.
// (other items were commented in a header).
// ---------------------------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testDeleteTriggers3L(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RArray<TLbtTriggerId> triggerArray;
    triggerArray.Append(trigId);
    TRAPD(error,lbt.DeleteTriggersL(triggerArray));
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }
   
// -------------------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testDeleteTriggers4L
// This test case will test DeleteTriggers(given trigger id array) for platform security in client library.
// (other items were commented in a header).
// -------------------------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testDeleteTriggers4L(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RArray<TLbtTriggerId> triggerArray;
    triggerArray.Append(trigId);
    notifier->DeleteTriggers( lbt,triggerArray,wait );
    wait->Start( );
    TInt error=notifier->iStatus.Int(); 
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }

 
// ------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testGetTriggerLC
// This test case will test GetTriggerLC for platform security in client library.
// (other items were commented in a header).
// ------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testGetTriggerLC(CStifItemParser& aItem )
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
    
      //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerInfo* triggerInfo=NULL;
    
    if(value)
    {
    triggerInfo=lbt.GetTriggerLC(trigId);
    CleanupStack::PopAndDestroy(triggerInfo);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;	  
	return KErrNone;
    }
        
    
    else
    {
    	TRAPD(error,triggerInfo=lbt.GetTriggerLC(trigId);	CleanupStack::PopAndDestroy(triggerInfo););
    	
        	
	   CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::PopAndDestroy( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    delete wait;	  
    	if(error!=KErrPermissionDenied)
    	{
    	return KErrNone;	
    	}
    	return error;
    }
    	   


   }
 
 // ------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testUpdateTriggerL
// This test case will test UpdateTriggerL for platform security in client library.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testUpdateTriggerL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    //**************New Trigger Entry**************/ 
    CLbtSessionTrigger* newtrig = CLbtSessionTrigger::NewL();
    CleanupStack::PushL(newtrig);
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
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    
    TRAPD(error,lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue));
    

    CleanupStack::PopAndDestroy(newtrig);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
  if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
  }
return error;

   }
   
 // --------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testSetTriggerStateL
// This test case will test SetTriggerStateL for platform security in client library.
// (other items were commented in a header).
// ---------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testSetTriggerStateL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    TRAPD(error,lbt.SetTriggerStateL(trigId,CLbtTriggerEntry::EStateDisabled,ELbtTrue));
   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
    return error;

   }
 
// -----------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testSetTriggersStateL
// This test case will test SetTriggersStateL for platform security in client library.
// (other items were commented in a header).
// -----------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testSetTriggersStateL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    TRAPD(error,lbt.SetTriggersStateL(CLbtTriggerEntry::EStateDisabled,ELbtTrue));
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
    return error;

   }
  
 
// ---------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testSetTriggersState2L
// This test case will test SetTriggersState for platform security in client library.
// (other items were commented in a header).
// ---------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testSetTriggersState1L(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );     
    
    notifier->SetTriggersState(lbt,filter,CLbtTriggerEntry::EStateDisabled,wait);
    wait->Start( );
    
    TInt error=notifier->iStatus.Int(); 
    
    CleanupStack::PopAndDestroy(filter);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
    return error;

   }
 
 // -------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testListTriggerIdsL
// This test case will test ListTriggerIdsL for platform security in client library.
// (other items were commented in a header).
// --------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testListTriggerIdsL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RArray < TLbtTriggerId > triggerIdArray;
    TRAPD(error,lbt.ListTriggerIdsL(triggerIdArray));
       
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
    return error;

   }

 // ------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testListTriggerIds1L
// This test case will test ListTriggerIds for platform security in client library.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testListTriggerIds1L(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RArray < TLbtTriggerId > triggerIdArray;
    notifier->ListTriggerIds(lbt,triggerIdArray,wait);
    wait->Start();  
    
    TInt error=notifier->iStatus.Int();      
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
    return error;

   }

// -----------------------------------------------------------------------------
// Ctestlbtplatsecurity::testGetTriggersL
// This test case will test GetTriggersL for platform security in client library.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testGetTriggersL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > triggerIdArray;
    TRAPD(error,lbt.GetTriggersL(triggerIdArray));
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
    return error;

   }
 
// ------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testGetTriggers
// This test case will test GetTriggers for platform security in client library.
// (other items were commented in a header).
// ------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testGetTriggers_test2L(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > triggerIdArray;
    notifier->GetTriggers(lbt,triggerIdArray,NULL,wait);
    wait->Start();
    
    TInt error=notifier->iStatus.Int(); 
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
    return error;

   }

    
 // -----------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testCreateGetTriggerIteratorL
// This test case will test CreateGetTriggerIteratorL for platform security in client library.
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testCreateGetTriggerIteratorL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    TRAPD(error,lbt.CreateGetTriggerIteratorL());
      
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
    return error;

   }

// -----------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testCreateGetTriggerIterator
// This test case will test CreateGetTriggerIterator for platform security in client library.
// (other items were commented in a header).
// -----------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testCreateGetTriggerIterator_test2L(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    notifier->CreateGetTriggerIterator(lbt,wait);
    wait->Start();
    TInt error=notifier->iStatus.Int(); 
   
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }
  
 
 // --------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testGetNextTriggerLC
// This test case will test GetNextTriggerLC for platform security in client library.
// (other items were commented in a header).
// ---------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testGetNextTriggerLC(CStifItemParser& /*aItem*/ )
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

    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig->SetRequestorL(ReqType,ReqFormat,ReqData);

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    User::LeaveIfError( notifier->iStatus.Int());
    
    lbt.CreateGetTriggerIteratorL();
    CLbtTriggerInfo* triggerInfo= NULL;
    TRAPD(error,triggerInfo=lbt.GetNextTriggerLC();CleanupStack::Pop(triggerInfo));
    
   	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
  //  delete triggerInfo;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }
   
   
// -----------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testNotifyTriggerChangeEvent
// This test case will test NotifyTriggerChangeEvent for platform security in client library.
// (other items were commented in a header).
// -----------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testNotifyTriggerChangeEventL(CStifItemParser& /*aItem*/ )
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
  //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier2 );
    
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait1 );
    wait1->Start( );
    
    TLbtTriggerChangeEvent triggerChangeEvent;
    triggerChangeEvent.iEventType=ELbtTriggerChangeEventMultiple;
    triggerChangeEvent.iTriggerId=trigId;
    
    notifier->NotifyTriggerChangeEvent( lbt,triggerChangeEvent, wait1);
		notifier2->After(50000);
    notifier2->Setwait(wait1);
    wait1->Start();
		
    TInt error=notifier->iStatus.Int();

    if( KErrPermissionDenied!=error)
    	{
    	lbt.CancelNotifyTriggerChangeEvent();
    	notifier->CancelNotification();
    	} 
    
    CleanupStack::PopAndDestroy( notifier2 );
 		CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait1;
	delete wait2;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }

// -----------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testNotifyTriggerFired
// This test case will test NotifyTriggerFired for platform security in client library.
// (other items were commented in a header).
// -----------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testNotifyTriggerFiredL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier2 );
    
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait1 );
    wait1->Start( );
    
    TLbtTriggerFireInfo triggerFireInfo;
    
    //TRequestStatus requestStatus( KErrNone );      
    notifier->NotifyTriggerFired(lbt,triggerFireInfo,wait1);
    //wait->Start( );
            
    notifier2->After(50000);
    notifier2->Setwait(wait1);
    wait1->Start();
    TInt error=notifier->iStatus.Int(); 

    if( KErrPermissionDenied!=error)
    	{
    	lbt.CancelNotifyTriggerFired();
    	notifier->CancelNotification();
    	} 
    CleanupStack::PopAndDestroy( notifier2 );  
 		CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait1;
	delete wait2;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }
   
  
// ----------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testGetFiredTriggersL
// This test case will test GetFiredTriggersL for platform security in client library.
// (other items were commented in a header).
// ----------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testGetFiredTriggersL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RArray < TLbtTriggerFireInfo > triggerFireInfoArray;       
    TRAPD(error,lbt.GetFiredTriggersL(triggerFireInfoArray));
           
 	CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    lbt.Close();
    CleanupStack::PopAndDestroy( &lbtserver );
  delete wait;
   if(error!=KErrPermissionDenied)
   {
    	return KErrNone;	
  }
	return error;

   }
  
 // ---------------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testNotifyTriggeringSystemSettingChange
// This test case will test NotifyTriggeringSystemSettingChange for platform security in client library.
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testNotifyTriggeringSystemSettingChangeL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier2 );
    
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait1 );
    wait1->Start( );
    
    TLbtTriggeringSystemSettings triggeringSystemSettings;     
    TRequestStatus requestStatus( KErrNone );
     
    notifier->NotifyTriggeringSystemSettingChange(lbt,triggeringSystemSettings,wait1);
       
    notifier2->After(50000);
    notifier2->Setwait(wait1);
    wait1->Start();
    TInt error=notifier->iStatus.Int(); 

	if( KErrPermissionDenied!=error)
    	{
    	lbt.CancelNotifyTriggeringSystemSettingChange();
    	notifier->CancelNotification();
    	} 
    
    CleanupStack::PopAndDestroy( notifier2 );      
 		CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait1;
	delete wait2;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }
   
 // --------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testGetTriggeringSystemSettingsL
// This test case will test GetTriggeringSystemSettingsL for platform security in client library.
// (other items were commented in a header).
// ---------------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testGetTriggeringSystemSettingsL(CStifItemParser& /*aItem*/ )
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    TLbtTriggeringSystemSettings triggeringSystemSettings;      
    TRAPD(error,lbt.GetTriggeringSystemSettingsL(triggeringSystemSettings));
               
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

   }
  
   
// -------------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testSetTriggeringSystemSettingsL
// This test case will test SetTriggeringSystemSettingsL for platform security in management library.
// (other items were commented in a header).
// --------------------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testSetTriggeringSystemSettingsL(CStifItemParser& /*aItem*/ )
{
	RLbtServer lbtserver;
  	RLbtManager lbtManager;
 	
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
    User::LeaveIfError( lbtManager.Open(lbtserver));
 	CleanupClosePushL( lbtManager );
 	
 	TLbtTriggeringSystemManagementSettings settings;
 	
 	settings.SetMinimumLocationUpdateInterval(10);
    settings.SetMaximumUserSpeed(15);
    
 	TRAPD(error,lbtManager.SetTriggeringSystemSettingsL(settings,EMinimumLocationUpdateInterval|EMaximumUserSpeed));
  	CleanupStack::PopAndDestroy( &lbtManager );
    CleanupStack::PopAndDestroy( &lbtserver );

  	if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

}

// -------------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testSetTriggeringMechanismStateL
// This test case will test SetTriggeringMechanismStateL for platform security in management library.
// (other items were commented in a header).
// -------------------------------------------------------------------------------------------------
//

TInt Ctestlbtplatsecurity::testSetTriggeringMechanismStateL(CStifItemParser& /*aItem*/ )
{
	RLbtServer lbtserver;
  	RLbtManager lbtManager;
 	
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
    User::LeaveIfError( lbtManager.Open(lbtserver));
 	CleanupClosePushL( lbtManager );
 	
 	TLbtTriggeringMechanismState triggeringMechanismState = ETriggeringMechanismOn;
    
 	TRAPD(error,lbtManager.SetTriggeringMechanismStateL(triggeringMechanismState));
  	CleanupStack::PopAndDestroy( &lbtManager );
    CleanupStack::PopAndDestroy( &lbtserver );

  	if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

}

// -----------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testGetTriggeringSystemStatusL
// This test case will test GetTriggeringSystemStatusL for platform security in management library.
// (other items were commented in a header).
// -----------------------------------------------------------------------------------------------
//
TInt Ctestlbtplatsecurity::testGetTriggeringSystemStatusL(CStifItemParser& /*aItem*/ )
{
	RLbtServer lbtserver;
  	RLbtManager lbtManager;
 	
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
    User::LeaveIfError( lbtManager.Open(lbtserver));
 	CleanupClosePushL( lbtManager );
 	
 	TLbtTriggeringSystemStatus triggeringSystemStatus;
    
 	TRAPD(error,lbtManager.GetTriggeringSystemStatusL(triggeringSystemStatus));
  	CleanupStack::PopAndDestroy( &lbtManager );
    CleanupStack::PopAndDestroy( &lbtserver );

  	/*if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }*/
	return error;

}

// -------------------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testNotifyTriggeringSystemStatusChange
// This test case will test NotifyTriggeringSystemStatusChange for platform security in management library.
// (other items were commented in a header).
// --------------------------------------------------------------------------------------------------------
//
TInt Ctestlbtplatsecurity::testNotifyTriggeringSystemStatusChangeL(CStifItemParser& /*aItem*/ )
{
	RLbtServer lbtserver;
  	RLbtManager lbtManager;
 	
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
    User::LeaveIfError( lbtManager.Open(lbtserver));
 	CleanupClosePushL( lbtManager );
 	
 	TCoordinate coordinate(62.5285,23.9385);
 	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbtManager,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    
 	notifier->NotifyTriggeringSystemStatusChange(lbtManager,wait);
 	
 	RTimer timer;
    timer.CreateLocal();
    TRequestStatus tstatus;
    timer.After( tstatus, 5000000 );
    User::WaitForRequest( tstatus );
    timer.Close();
    
     TInt error=notifier->iStatus.Int(); 

	if( KErrPermissionDenied!=error)
    	{
    	lbtManager.CancelNotifyTriggeringSystemStatusChange();
    	notifier->CancelNotification();
    	} 

 	CleanupStack::PopAndDestroy( notifier );
  	CleanupStack::PopAndDestroy( &lbtManager );
  	CleanupStack::PopAndDestroy( &lbtserver );
 	if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

}


// ---------------------------------------------------------------------------------------------
// Ctestlbtplatsecurity::testGetTriggerManagerUiInfoL
// This test case will test GetTriggerManagerUiInfoL for platform security in management library.
// (other items were commented in a header).
// ---------------------------------------------------------------------------------------------
//
TInt Ctestlbtplatsecurity::testGetTriggerManagerUiInfoL(CStifItemParser& /*aItem*/ )
{
	RLbtServer lbtserver;
  	RLbtManager lbtManager;
 	
 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
    User::LeaveIfError( lbtManager.Open(lbtserver));
 	CleanupClosePushL( lbtManager );
 	
 	RArray < TLbtTriggerManagerUiInfo > managerUiList;
    
 	TRAPD(error,lbtManager.GetTriggerManagerUiInfoL(managerUiList));
  	CleanupStack::PopAndDestroy( &lbtManager );
    CleanupStack::PopAndDestroy( &lbtserver );

  	if(error!=KErrPermissionDenied)
    {
    	return KErrNone;	
    }
	return error;

}



//  [End of File] - Do not remove
