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
* Description:    Test module to implement Cancellation of asynchronous requests related test cases
*
*/





// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "testlbtcancellation.h"
#include <lbt.h> 
#include <lbtserver.h>
#include <e32property.h>
//#include "lbtstatussdkpskeys.h"
#include <lbtgeocircle.h> 
#include <lbtgeorect.h>
#include <lbttriggerconditionarea.h> 
#include <lbtsessiontrigger.h> 
#include <lbttriggerfilterbyattribute.h>
#include <lbttriggerchangeevent.h>
#include <lbttriggerentry.h>
#include <lbttriggerinfo.h>
#include <lbtmanager.h> 
#include "t_triggerfireobserver.h"
#include <centralrepository.h>
#include <SimulationPSYInternalCRKeys.h>
#include <lbttriggeringsystemsettings.h>

#include <lbtmanager.h> 
#include <lbttriggeringsystemsettings.h>
#include <lbttriggeringsystemmanagementsettings.h>
#include <lbttriggeringsystemstatus.h>

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
// Ctestlbtcancellation::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ctestlbtcancellation::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ctestlbtcancellation::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ctestlbtcancellation::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        ENTRY( "Example", Ctestlbtcancellation::ExampleL ),
        ENTRY( "testCancelCreateTrigger1", Ctestlbtcancellation::testCancelCreateTrigger1L ),
        ENTRY( "testCancelCreateTrigger2", Ctestlbtcancellation::testCancelCreateTrigger2L ),
        ENTRY( "testCancelDeleteTriggers1", Ctestlbtcancellation::testCancelDeleteTriggers1L ),
        ENTRY( "testCancelDeleteTriggers2", Ctestlbtcancellation::testCancelDeleteTriggers2L ),
        ENTRY( "testCancelDeleteTriggers3", Ctestlbtcancellation::testCancelDeleteTriggers3L ),
        ENTRY( "testCancelDeleteTriggers4", Ctestlbtcancellation::testCancelDeleteTriggers4L ),
        ENTRY( "testCancelSetTriggersState1", Ctestlbtcancellation::testCancelSetTriggersState1L ),
        ENTRY( "testCancelSetTriggersState2", Ctestlbtcancellation::testCancelSetTriggersState2L ),
        ENTRY( "testCancelListTriggerIds1", Ctestlbtcancellation::testCancelListTriggerIds1L ),
        ENTRY( "testCancelListTriggerIds2", Ctestlbtcancellation::testCancelListTriggerIds2L ),
       	ENTRY( "testCancelGetTriggers1", Ctestlbtcancellation::testCancelGetTriggers1L ),
       	ENTRY( "testCancelGetTriggers2", Ctestlbtcancellation::testCancelGetTriggers2L ),
       	ENTRY( "testCancelCreatetTriggerIterator", Ctestlbtcancellation::testCancelCreatetTriggerIteratorL ),
       	ENTRY( "testCancelCreateTriggerFromDifferentSubsession", Ctestlbtcancellation::testCancelCreateTriggerFromDifferentSubsessionL ),
       	ENTRY( "testCancelDeleteTriggersFromDifferentSubsession", Ctestlbtcancellation::testCancelDeleteTriggersFromDifferentSubsessionL ),
        ENTRY( "testCancelSetTriggersStateFromDifferentSubsession", Ctestlbtcancellation::testCancelSetTriggersStateFromDifferentSubsessionL ),
        ENTRY( "testCancelListTriggerIdsFromDifferentSubsession", Ctestlbtcancellation::testCancelListTriggerIdsFromDifferentSubsessionL ),
        ENTRY( "testCancelGetTriggersFromDifferentSubsession", Ctestlbtcancellation::testCancelGetTriggersFromDifferentSubsessionL ),
        ENTRY( "testCancelCreatetTriggerIteratorFromDifferentSubsession", Ctestlbtcancellation::testCancelCreatetTriggerIteratorFromDifferentSubsessionL ),
        ENTRY( "testCancelCreateTriggerFromDifferentProcess", Ctestlbtcancellation::testCancelCreateTriggerFromDifferentProcessL ),
 	    ENTRY( "testCancelDeleteTriggersFromDifferentProcess", Ctestlbtcancellation::testCancelDeleteTriggersFromDifferentProcessL ),
        ENTRY( "testCancelSetTriggersStateFromDifferentProcess", Ctestlbtcancellation::testCancelSetTriggersStateFromDifferentProcessL ),
        ENTRY( "testCancelListTriggerIdsFromDifferentProcess", Ctestlbtcancellation::testCancelListTriggerIdsFromDifferentProcessL ),
        ENTRY( "testCancelGetTriggersFromDifferentProcess", Ctestlbtcancellation::testCancelGetTriggersFromDifferentProcessL ),
        ENTRY( "testCancelCreatetTriggerIteratorFromDifferentProcess", Ctestlbtcancellation::testCancelCreatetTriggerIteratorFromDifferentProcessL ),
        ENTRY( "testCancelUpdateTrigger1", Ctestlbtcancellation::testCancelUpdateTrigger1L ),
        ENTRY( "testCancelUpdateTrigger2", Ctestlbtcancellation::testCancelUpdateTrigger2L ),
        ENTRY( "testCancelNotifyTriggerChangeEvent1", Ctestlbtcancellation::testCancelNotifyTriggerChangeEvent1L ),
        ENTRY( "testCancelNotifyTriggerChangeEvent2", Ctestlbtcancellation::testCancelNotifyTriggerChangeEvent2L ),
        ENTRY( "testCanceltNotifyTriggerFired", Ctestlbtcancellation::testCanceltNotifyTriggerFiredL ),
        ENTRY( "testCancelNotifyTriggeringSettingsChange", Ctestlbtcancellation::testCancelNotifyTriggeringSettingsChangeL ),
        ENTRY( "testlbtmgrCancelNotifyTriggeringSettingsChange", Ctestlbtcancellation::testlbtmgrCancelNotifyTriggeringSettingsChangeL ),
        ENTRY( "testCancelNotifyTriggeringSystemStatusChange", Ctestlbtcancellation::testCancelNotifyTriggeringSystemStatusChangeL ),
        ENTRY( "testPartialDeleteTriggers1L", Ctestlbtcancellation::testPartialDeleteTriggers1L ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }
void Ctestlbtcancellation::EnableSimPSYL() 
    {
   CPosModules* db = CPosModules::OpenL();
    CleanupStack::PushL( db );

    // List entries
    CPosModuleIdList* idList = db->ModuleIdListLC();
    CPosModuleUpdate* updateParams = CPosModuleUpdate::NewLC();

    // Get the display name and status of each installed positioning plug-in
    for (TInt i = 0; i < idList->Count(); i++)
    {
    // get PSY info
    TPositionModuleInfo moduleInfo;
    db->GetModuleInfoL( (*idList)[i], moduleInfo );
    
    if ( moduleInfo.IsAvailable() )
        {
        // read PSY’s name
        TBuf<KPositionMaxModuleName> moduleName;
        moduleInfo.GetModuleName( moduleName );
        TPositionModuleId id = moduleInfo.ModuleId();
        
        updateParams->SetUpdateAvailability( EFalse );
        db->UpdateModuleL( id, *updateParams );

        }
    }
    TPositionModuleId id2 = {0x101F7A81};
    updateParams->SetUpdateAvailability( ETrue );
    db->UpdateModuleL( id2, *updateParams );

    CleanupStack::PopAndDestroy( updateParams );

    CleanupStack::PopAndDestroy( idList );
    CleanupStack::PopAndDestroy( db );
    }
// -----------------------------------------------------------------------------
// Ctestlbtcancellation::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestlbtcancellation::ExampleL( CStifItemParser& /*aItem*/ )
    {
    RLbtServer lbtserver;
  	RLbt lbt;
  	 
  	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
     
    User::LeaveIfError( lbt.Open(lbtserver));
 	CleanupClosePushL( lbt );
 	 
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

 	 
 	CLbtTriggerInfo* trig = lbt.GetTriggerLC(triggerId) ;
 	 
 	CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    	return KErrNone;
    }

// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelCreateTrigger
// This test case will test CancelCreateTrigger of RLbt class for startup trigger.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//1)Cancel Createtrigger immediately.(startup trigger)
//2)Try to Cancel Createtrigger after completion.(startup trigger)
//3)Try to Cancel Createtrigger from other subsession.(startup trigger)
//4)Try to Cancel Createtrigger Using CancelAll.(startup trigger)


TInt Ctestlbtcancellation::testCancelCreateTrigger1L(CStifItemParser& aItem)
   {
 
   	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
     CleanupClosePushL( lbt );
     
     User::LeaveIfError( lbt1.Open(lbtserver));
     CleanupClosePushL( lbt1 );
     
     TInt value;
     aItem.GetNextInt(value);	 
     //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
       
  	TSecureId secureid;
  	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);             
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId =0;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    switch(value)
    {
    	case 0:	lbt.CancelCreateTrigger();
    				wait->Start();
    		//	notifier->Cancel();
    			break;
    	
    	case 1:wait->Start();
    			lbt.CancelCreateTrigger();
    			break;
    	case 2:lbt1.CancelCreateTrigger();	
    			wait->Start();
    			break;
    	case 3:	lbt.CancelAll();
    			wait->Start();
    			break;
    	case 4: lbt.CancelUpdateTrigger();
    					wait->Start();
    	        break;
    	default:break;		
    }
    
    	
	TInt error=notifier->iStatus.Int();    
    TRAPD(error2, lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy( ););
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(value==0 ||value==3)
    	{
    	if(error!=KErrCancel)
    		{
    		return KErrGeneral;
    		}
    	
   		if(error2==KErrNotFound)
	   	{	
    	return KErrNone;
    	}
    	else
    	{
    	return KErrGeneral;	
    	}
    	}
    else 
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	return KErrNone;	
    	}	
    }
    
    // -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelCreateTrigger
// This test case will test CancelCreateTrigger of RLbt class for session trigger.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//1)Cancel Createtrigger immediately.(session trigger)
//2)Try to Cancel Createtrigger after completion.(session trigger)
//3)Try to Cancel Createtrigger from other subsession.(session trigger)
//4)Try to Cancel Createtrigger Using CancelAll.(session trigger)


TInt Ctestlbtcancellation::testCancelCreateTrigger2L(CStifItemParser& aItem)
   {
 
   	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
     CleanupClosePushL( lbt );
     
     User::LeaveIfError( lbt1.Open(lbtserver));
     CleanupClosePushL( lbt1 );
     
     TInt value;
     aItem.GetNextInt(value);	 
  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
     
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId =0;
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    switch(value)
    {
    	case 0:	lbt.CancelCreateTrigger();
    		//	notifier->Cancel();
    			break;
    	
    	case 1:wait->Start();
    			lbt.CancelCreateTrigger();
    			
    			break;
    	case 2:lbt1.CancelCreateTrigger();	
    			wait->Start();
    			break;
    	case 3:	lbt.CancelAll();
    	wait->Start();
    			break;
    	case 4: lbt.CancelDeleteTriggers();
    					wait->Start();
    	                break;
    	default:break;		
    }
    
    	
	TInt error=notifier->iStatus.Int();    
	TRAPD(error2, lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy( );); 
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(value==0 ||value==3)
    	{
    	if(error!=KErrCancel)
    		{
    		return KErrGeneral;
    		}
    	
   		if(error2==KErrNotFound)
	   	{	
    	return KErrNone;
    	}
    	else
    	{
    	return KErrGeneral;	
    	}
    	}
    else 
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	return KErrNone;	
    	}	
    }


// -------------------------------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelDeleteTriggers
// This test case will test CancelDeleteTriggers of RLbt class for session trigger.
// (other items were commented in a header).
// -------------------------------------------------------------------------------------------------------
//1)Cancel Deletetriggers immediately.(session trigger)
//2)Try to Cancel Deletetriggers after completion.(session trigger)
//3)Try to Cancel Deletetriggers from other subsession.(session trigger)
//4)Cancel Deletetriggers in the middle.(session trigger)
//5) Cancel Deletetriggers Using CancelAll.(session trigger)

TInt Ctestlbtcancellation::testCancelDeleteTriggers1L(CStifItemParser& aItem)
   {
   	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 
  	 TInt value;
	 	 aItem.GetNextInt(value);
	 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 	 
	   //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
       
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
		CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId=0;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt,coordinate);
        CleanupStack::PushL( notifier2 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
   
    RArray<TLbtTriggerId> triggerArray;
    
    TRAP_IGNORE(lbt.DeleteTriggersL());
    triggerArray.Reset();
    for(TInt i=0;i<20;i++) 
    {   
      notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
      wait->Start( );
      if(notifier->iStatus.Int()!=KErrNone)
          {
          User::LeaveIfError(KErrGeneral);
          }
      triggerArray.Append(trigId);
        }
  TBool isCancelled = EFalse;
  switch(value)
    {
    	case 0:
    		
			notifier->DeleteTriggers( lbt,triggerArray,wait );
	    	lbt.CancelDeleteTriggers();
	    	isCancelled = ETrue;
    		//	notifier->Cancel();
    			break;
    	
    	case 1:
    			
				notifier->DeleteTriggers( lbt,triggerArray,wait );
    			wait->Start();
    			lbt.CancelDeleteTriggers();
    			break;
    	case 2:
    			
				notifier->DeleteTriggers( lbt,triggerArray,wait );
    			lbt1.CancelDeleteTriggers();	
    			wait->Start();
    			break;
    	case 3:    
    	                
                notifier->DeleteTriggers( lbt,triggerArray,wait );
                notifier2->After(5);
                notifier2->Setwait(wait);
                wait->Start();
                if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelDeleteTriggers();
                }
                
                break;
        case 4: 
                
                notifier->DeleteTriggers( lbt,triggerArray,wait );
                notifier2->After(2);
                notifier2->Setwait(wait);
                wait->Start();
                if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelAll();
                }
                
                break;
    	case 5:
                
                notifier->DeleteTriggers( lbt,triggerArray,wait );
                lbt.CancelSetTriggersState();    
                wait->Start();
                break;
    			
    	default:break;		
    }
  
    TInt error=notifier->iStatus.Int(); 
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    trigInfoList.Reset();
    TRAP_IGNORE(lbt.GetTriggersL(trigInfoList ));
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    triggerArray.Close();
    if(value==1 || value==2 || value==5)
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	if(trigInfoList.Count()!=0)
    		{
    		return KErrGeneral;
    		}
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;
    	}
    else 
    	{
    	if(isCancelled)
        {
            if(error!=KErrCancel)
            {
            return KErrGeneral;
            }
            if(trigInfoList.Count()==0)
            {
            return KErrGeneral;
            }
        }
    	
    	
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;	
    	}	

   }
   
   // -------------------------------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelDeleteTriggers
// This test case will test CancelDeleteTriggers of RLbt class for startuptrigger.
// (other items were commented in a header).
// -------------------------------------------------------------------------------------------------------
//1)Cancel Deletetriggers immediately.(startup trigger)
//2)Try to Cancel Deletetriggers after completion.(startup trigger)
//3)Try to Cancel Deletetriggers from other subsession.(startup trigger)
//4)Cancel Deletetriggers in the middle.(startup trigger)
//5) Cancel Deletetriggers Using CancelAll.(startup trigger)


TInt Ctestlbtcancellation::testCancelDeleteTriggers2L(CStifItemParser& aItem)
   {
   	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 
  	 TInt value;
	 	 aItem.GetNextInt(value);
	 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 	 
	   //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
       
  	TSecureId secureid;
  	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);             
          
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
		CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId=0;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier2 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    RArray<TLbtTriggerId> triggerArray;
    
    TRAP_IGNORE(lbt.DeleteTriggersL());
    triggerArray.Reset();
    for(TInt i=0;i<20;i++) 
    {   
      notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
      wait->Start( );
      if(notifier->iStatus.Int()!=KErrNone)
      {
      User::LeaveIfError(KErrGeneral);
      }
      triggerArray.Append(trigId);
        }
   TBool isCancelled = EFalse;
  switch(value)
    {
    	case 0:
    		
			notifier->DeleteTriggers( lbt,triggerArray,wait );
	    	lbt.CancelDeleteTriggers();
	    	isCancelled = ETrue;
    		//	notifier->Cancel();
    			break;
    	
    	case 1:
    			
				notifier->DeleteTriggers( lbt,triggerArray,wait );
    			wait->Start();
    			lbt.CancelDeleteTriggers();
    			break;
    	case 2:
    			
				notifier->DeleteTriggers( lbt,triggerArray,wait );
    			lbt1.CancelDeleteTriggers();	
    			wait->Start();
    			break;
    	case 3:	
    			
				notifier->DeleteTriggers( lbt,triggerArray,wait );
    			notifier2->After(10);
    			notifier2->Setwait(wait);
    			wait->Start();
    			if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelDeleteTriggers();
                }
    			
    			break;
    	case 4:	
    			
                notifier->DeleteTriggers( lbt,triggerArray,wait );
                notifier2->After(10);
                notifier2->Setwait(wait);
    			wait->Start();
    			if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelAll();
                }
    			
    			break;
    	case 5:
                
                notifier->DeleteTriggers( lbt,triggerArray,wait );
                lbt.CancelListTriggerIds();    
                wait->Start();
                break;
    			
    	default:break;		
    }
  
    TInt error=notifier->iStatus.Int(); 
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    TRAP_IGNORE(lbt.GetTriggersL(trigInfoList ));
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
     CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    triggerArray.Close();
    if(value==1 || value==2 || value==5)
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	if(trigInfoList.Count()!=0)
    		{
    		return KErrGeneral;
    		}
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;
    	}
    else 
    	{
    	if(isCancelled)
        {
            if(error!=KErrCancel)
            {
            return KErrGeneral;
            }
            if(trigInfoList.Count()==0)
            {
            return KErrGeneral;
            }
        }
    	
    	
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;	
    	}	

   }

// ---------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelSetTriggersState1
// This test case will test CancelSetTriggersState of RLbt class.
// (other items were commented in a header).
// ---------------------------------------------------------------------------------
//1)Cancel SetTriggersState immediately.(Session trigger)
//2)Try to Cancel SetTriggersState after completion.(Session trigger)
//3)Try to Cancel SetTriggersState from other subsession.(Session trigger)
//4)Cancel SetTriggersState in the middle.(Session trigger)
//5) Cancel SetTriggersState Using CancelAll.(Session trigger)

TInt Ctestlbtcancellation::testCancelSetTriggersState1L(CStifItemParser& aItem)
   {
   	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 TInt value;
	 	 aItem.GetNextInt(value);
	 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
     CleanupClosePushL( lbt );
     
     User::LeaveIfError( lbt1.Open(lbtserver));
     CleanupClosePushL( lbt1 );
 	 
 	 	 
	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt,coordinate);
        CleanupStack::PushL( notifier2 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
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
  
    TRAP_IGNORE(lbt.DeleteTriggersL());
    for(TInt i=0;i<20;i++) 
    {   
      notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
      wait->Start( );
      if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
      }    
    TBool isCancelled = EFalse;
    switch(value)
    {
    	case 0:
		    	
				notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateDisabled,wait);
		    	lbt.CancelSetTriggersState();
		    	isCancelled = ETrue;
    			break;
    	
    	case 1:
		    	notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateDisabled,wait);
	    		wait->Start();
	  			lbt.CancelSetTriggersState();
  				break;
    	case 2:
		    	notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateDisabled,wait);
		    	lbt1.CancelSetTriggersState();
					wait->Start();
					break;
			
    	case 3:  
                notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateEnabled,wait);
                notifier2->After(10);
                notifier2->Setwait(wait);
                wait->Start();
                if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelSetTriggersState();
                }
                break;
    	case 4:                 
                notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateEnabled,wait);
                notifier2->After(10);
                notifier2->Setwait(wait);
                wait->Start();
                if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelAll();
                }
    	                
    	                break;
    	case 5:
                notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateDisabled,wait);
                lbt.CancelGetTriggers();
                    wait->Start();
                    break;
    	default:break;		
    }
    
       
    TInt error=notifier->iStatus.Int(); 
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    TRAP_IGNORE(lbt.GetTriggersL(trigInfoList,listOptions ));
    
    CleanupStack::PopAndDestroy( listOptions );
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(value==1 || value==2 || value==5)
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	if(trigInfoList.Count()!=0)
    		{
    		return KErrGeneral;
    		}
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;
    	}
    else 
    	{
    	if(isCancelled)
        {
            if(error!=KErrCancel)
            {
            return KErrGeneral;
            }
            if(trigInfoList.Count()==0)
            {
            return KErrGeneral;
            }
        }
    	
    	
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;	
    	}	
   }
   
   // ---------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelSetTriggersState2
// This test case will test CancelSetTriggersState of RLbt class.
// (other items were commented in a header).
// ---------------------------------------------------------------------------------
//1)Cancel SetTriggersState immediately.(startup trigger)
//2)Try to Cancel SetTriggersState after completion.(startup trigger)
//3)Try to Cancel SetTriggersState from other subsession.(startup trigger)
//4)Cancel SetTriggersState in the middle.(startup trigger)
//5) Cancel SetTriggersState Using CancelAll.(startup trigger)

TInt Ctestlbtcancellation::testCancelSetTriggersState2L(CStifItemParser& aItem)
   {
   	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 TInt value;
	 	 aItem.GetNextInt(value);
	 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
	 	 CleanupClosePushL( lbt );
	 	 
	 	 User::LeaveIfError( lbt1.Open(lbtserver));
	 	 CleanupClosePushL( lbt1 );
 	 
 	 	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
		CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
		TBuf<KLbtMaxNameLength> ReqData=_L("");
		trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // Set Name
    trig->SetNameL(_L("Trigger1"));
  	_LIT( KMyTriggerHandlingProcessName, "About.exe");
       
  	TSecureId secureid;
  	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);             
          
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
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter );
   
   
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeStartup );
    listOptions->SetFilter( attribFilter ); 
    
    CleanupStack::Pop( attribFilter );   
    TRAP_IGNORE(lbt.DeleteTriggersL());
        for(TInt i=0;i<20;i++) 
        {   
          notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
          wait->Start( );
          if(notifier->iStatus.Int()!=KErrNone)
                {
                User::LeaveIfError(KErrGeneral);
                }
          }   
     TBool   isCancelled = EFalse;   
    switch(value)
    {
    	case 0:
		    	notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateDisabled,wait);
		    	lbt.CancelSetTriggersState();
		    	isCancelled = ETrue;
    			break;
    	
    	case 1:
		    	notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateDisabled,wait);
	    		wait->Start();
	  			lbt.CancelSetTriggersState();
  				break;
    	case 2:
		    	notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateDisabled,wait);
		    	lbt1.CancelSetTriggersState();
					wait->Start();
					break;
			
			case 3:	
    			notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateEnabled,wait);
    			notifier2->After(10);
    			notifier2->Setwait(wait);
    			wait->Start();
    			if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
    			lbt.CancelSetTriggersState();
                }
    			break;
    	case 4:	    			
                notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateEnabled,wait);
                notifier2->After(10);
                notifier2->Setwait(wait);
                wait->Start();
                if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelAll();
                }
                
    			break;
    	case 5:
                notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateDisabled,wait);
                lbt.CancelCreateTriggerIterator();
                    wait->Start();
                    break;
    	default:break;		
    }
    
       
    TInt error=notifier->iStatus.Int(); 
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    TRAP_IGNORE(lbt.GetTriggersL(trigInfoList,listOptions ));
    
    CleanupStack::PopAndDestroy( listOptions );
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(value==1 || value==2 || value==5)
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	if(trigInfoList.Count()!=0)
    		{
    		return KErrGeneral;
    		}
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;
    	}
    else 
    	{
    	if(isCancelled)
    	        {
    	            if(error!=KErrCancel)
    	            {
    	            return KErrGeneral;
    	            }
    	            if(trigInfoList.Count()==0)
                    {
                    return KErrGeneral;
                    }
    	        }
    	
    	
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;	
    	}	
   }
// ------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelListTriggerIds1
// This test case will test CancelListTriggerIds of RLbt class.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//1)Cancel ListTriggerIds immediately.(session trigger)
//2)Try to Cancel ListTriggerIds after completion.(session trigger)
//3)Try to Cancel ListTriggerIds from other subsession.(session trigger)
//4)Cancel ListTriggerIds in the middle.(session trigger)
//5) Cancel ListTriggerIds Using CancelAll.(session trigger)

TInt Ctestlbtcancellation::testCancelListTriggerIds1L(CStifItemParser& aItem)
   {
   	 RLbtServer lbtserver;
   	 RLbt lbt;
  	 RLbt lbt1;
  	 TInt value;
	 	 aItem.GetNextInt(value);
	 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
     
     User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 
 	 
	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
		CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
		TBuf<KLbtMaxNameLength> ReqData=_L("");
		trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
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
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RArray < TLbtTriggerId > triggerIdArray;
    TRAP_IGNORE(lbt.DeleteTriggersL());
    triggerIdArray.Reset();
    for(TInt i=0;i<20;i++) 
    {   
      notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
      wait->Start( );
      if(notifier->iStatus.Int()!=KErrNone)
      {
      User::LeaveIfError(KErrGeneral);
      }
     // triggerIdArray.Append(trigId);
    }
    TBool isCancelled= EFalse;
    switch(value)
    {
    	case 0:
    			notifier->ListTriggerIds(lbt,triggerIdArray,wait);
				isCancelled = ETrue;
    			lbt.CancelListTriggerIds();
    			break;
    	
    	case 1:
    			notifier->ListTriggerIds(lbt,triggerIdArray,wait);
    			wait->Start();
    			lbt.CancelListTriggerIds();
    			break;
    	case 2:
    			
				notifier->ListTriggerIds(lbt,triggerIdArray,wait);
    			lbt1.CancelListTriggerIds();
    			wait->Start();
    			break;
    	case 3:	
    			
				notifier->ListTriggerIds(lbt,triggerIdArray,wait);
				if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelListTriggerIds();
                }
    			break;
    	case 4:	
    			
                notifier->ListTriggerIds(lbt,triggerIdArray,wait);
                if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelAll();
                }
    			break;
    	case 5:
                
                notifier->ListTriggerIds(lbt,triggerIdArray,wait);
                lbt.CancelNotifyTriggerChangeEvent();
                wait->Start();
                break;
    	default:break;		
    }
   
    TInt error=notifier->iStatus.Int();      
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    
    CleanupStack::PopAndDestroy( &lbt1 );
     CleanupStack::PopAndDestroy( &lbt);
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
   if(value==1 || value==2 ||value==5)
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	if(triggerIdArray.Count()==0)
        {
        return KErrGeneral;
        }
    	triggerIdArray.Close();
    	return KErrNone;
    	}
    else 
    {
        if(isCancelled)
        {
            if(error!=KErrCancel)
            {
            return KErrGeneral;
            }
            /*if(triggerIdArray.Count()!=0)
            {
            return KErrGeneral;
            }*/
        }
        triggerIdArray.Close();
    return KErrNone;	
    }	
   }
   
   // ------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelListTriggerIds1
// This test case will test CancelListTriggerIds of RLbt class.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//1)Cancel ListTriggerIds immediately.(startup trigger)
//2)Try to Cancel ListTriggerIds after completion.(startup trigger)
//3)Try to Cancel ListTriggerIds from other subsession.(startup trigger)
//4)Cancel ListTriggerIds in the middle.(startup trigger)
//5) Cancel ListTriggerIds Using CancelAll.(startup trigger)

TInt Ctestlbtcancellation::testCancelListTriggerIds2L(CStifItemParser& aItem)
   {
   	 RLbtServer lbtserver;
   	 RLbt lbt;
  	 RLbt lbt1;
  	 TInt value;
	 	 aItem.GetNextInt(value);
	 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
     
     User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 
 	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
		CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
		TBuf<KLbtMaxNameLength> ReqData=_L("");
		trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
       
  	TSecureId secureid;
  	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);       
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
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    TBool isCancelled= EFalse;
    RArray < TLbtTriggerId > triggerIdArray;
    TRAP_IGNORE(lbt.DeleteTriggersL());
    triggerIdArray.Reset();
    for(TInt i=0;i<20;i++) 
    {   
      notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
      wait->Start( );
      if(notifier->iStatus.Int()!=KErrNone)
      {
      User::LeaveIfError(KErrGeneral);
      }
     // triggerIdArray.Append(trigId);
    }
    switch(value)
    {
    	case 0:
    			
					notifier->ListTriggerIds(lbt,triggerIdArray,wait);
					isCancelled = ETrue;
    			lbt.CancelListTriggerIds();
    			break;
    	
    	case 1:
    			
					notifier->ListTriggerIds(lbt,triggerIdArray,wait);
    			wait->Start();
    			lbt.CancelListTriggerIds();
    			break;
    	case 2:
    			
					notifier->ListTriggerIds(lbt,triggerIdArray,wait);
    			lbt1.CancelListTriggerIds();
    			wait->Start();
    			break;
    	case 3:	
    			
				notifier->ListTriggerIds(lbt,triggerIdArray,wait);
    			/*notifier2->After(10);
    			notifier2->Setwait(wait);
    			wait->Start();*/
    			
    			if(notifier->iStatus==KRequestPending)
    			    {
    			    isCancelled = ETrue;
    			    lbt.CancelListTriggerIds();
    			    }
    			break;
    	case 4:	
    			
					notifier->ListTriggerIds(lbt,triggerIdArray,wait);
					if(notifier->iStatus==KRequestPending)
                    {
                    isCancelled = ETrue;
                    lbt.CancelAll();
                    }
	    		//lbt.CancelAll();
    			break;
    	case 5:
    	                
    	                notifier->ListTriggerIds(lbt,triggerIdArray,wait);
    	                lbt.CancelNotifyTriggerFired();
    	                wait->Start();
    	                break;
    	default:break;		
    }
   
    TInt error=notifier->iStatus.Int();      
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    
    CleanupStack::PopAndDestroy( &lbt1 );
     CleanupStack::PopAndDestroy( &lbt);
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
   if(value==1 || value==2 || value==5)
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	if(triggerIdArray.Count()==0)
            {
            return KErrGeneral;
            }
    	triggerIdArray.Close();
    	return KErrNone;
    	}
    else 
    	{
    	if(isCancelled)
    	{
            if(error!=KErrCancel)
            {
            return KErrGeneral;
            }
            /*if(triggerIdArray.Count()!=0)
            {
            return KErrGeneral;
            }*/
    	}
    	triggerIdArray.Close();
    return KErrNone;	
    	}	
   }

// ------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelGetTriggers1
// This test case will test CancelGetTriggers of RLbt class.
// (other items were commented in a header).
// ------------------------------------------------------------------------------
//1)Cancel GetTriggers immediately.(session trigger)
//2)Try to Cancel GetTriggers after completion.(session trigger)
//3)Try to Cancel GetTriggers from other subsession.(session trigger)
//4)Cancel GetTriggers in the middle.(session trigger)
//5) Cancel GetTriggers Using CancelAll.(session trigger)

TInt Ctestlbtcancellation::testCancelGetTriggers1L(CStifItemParser& aItem)
   {
   	 RLbtServer lbtserver;
   	 RLbt lbt;
  	 RLbt lbt1;
  	 TInt value;
 	 aItem.GetNextInt(value);
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
     
     User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 
 	 
	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt,coordinate);
        CleanupStack::PushL( notifier2 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > triggerIdArray;
    TBool isCancelled = EFalse;
    TRAP_IGNORE(lbt.DeleteTriggersL());
    for(TInt i=0;i<20;i++) 
    {   
      notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
      wait->Start( );
      if(notifier->iStatus.Int()!=KErrNone)
        {
        User::LeaveIfError(KErrGeneral);
        }
    }
    switch(value)
    {
    	case 0:
    			
				  notifier->GetTriggers(lbt,triggerIdArray,wait);
				  isCancelled = ETrue;
    			lbt.CancelGetTriggers();
    			break;
    	
    	case 1:
    			
				  notifier->GetTriggers(lbt,triggerIdArray,wait);
    			wait->Start();
    			lbt.CancelGetTriggers();
    			break;
    	case 2:
    			
				  notifier->GetTriggers(lbt,triggerIdArray,wait);
    			lbt1.CancelGetTriggers();
    			wait->Start();
    			break;
    	case 3:	
    			
				  notifier->GetTriggers(lbt,triggerIdArray,wait);
    			notifier2->After(10);
    			notifier2->Setwait(wait);
    			wait->Start();
    			if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelGetTriggers();
                }
    			
    			break;
    	case 4:	
    			
				  notifier->GetTriggers(lbt,triggerIdArray,wait);
					notifier2->After(10);
					notifier2->Setwait(wait);
    			wait->Start();
    			if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelAll();
                }
	    		
    			break;
    	default:break;		
    }
    
    
    TInt error=notifier->iStatus.Int(); 
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(value==1 || value==2)
            {
            if(error!=KErrNone)
                {
                return KErrGeneral;
                }
            if(triggerIdArray.Count()==0)
            {
            return KErrGeneral;
            }
            triggerIdArray.ResetAndDestroy();
            return KErrNone;
            }
        else 
            {
            if(isCancelled)
            {
                if(error!=KErrCancel)
                {
                return KErrGeneral;
                }
                if(triggerIdArray.Count()!=0)
                {
                return KErrGeneral;
                }
            }
            triggerIdArray.ResetAndDestroy();
        return KErrNone;    
            }   
   }
   
   // ------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelGetTriggers2
// This test case will test CancelGetTriggers of RLbt class.
// (other items were commented in a header).
// ------------------------------------------------------------------------------
//1)Cancel GetTriggers immediately.(startup trigger)
//2)Try to Cancel GetTriggers after completion.(startup trigger)
//3)Try to Cancel GetTriggers from other subsession.(startup trigger)
//4)Cancel GetTriggers in the middle.(startup trigger)
//5) Cancel GetTriggers Using CancelAll.(startup trigger)


TInt Ctestlbtcancellation::testCancelGetTriggers2L(CStifItemParser& aItem)
   {
   	 RLbtServer lbtserver;
   	 RLbt lbt;
  	 RLbt lbt1;
  	 TInt value;
 	 aItem.GetNextInt(value);
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
     
     User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 
 	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
       
  	TSecureId secureid;
  	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);       
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
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > triggerIdArray;
    TBool isCancelled = EFalse;
    TRAP_IGNORE(lbt.DeleteTriggersL());
    for(TInt i=0;i<20;i++) 
    {   
      notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
      wait->Start( );
      if(notifier->iStatus.Int()!=KErrNone)
        {
        User::LeaveIfError(KErrGeneral);
        }
    }
    switch(value)
    {
    	case 0:
    			
				  notifier->GetTriggers(lbt,triggerIdArray,wait);
				  isCancelled = ETrue;
    			lbt.CancelGetTriggers();
    			break;
    	
    	case 1:
    			
				  notifier->GetTriggers(lbt,triggerIdArray,wait);
    			wait->Start();
    			lbt.CancelGetTriggers();
    			break;
    	case 2:
    			
				  notifier->GetTriggers(lbt,triggerIdArray,wait);
    			lbt1.CancelGetTriggers();
    			wait->Start();
    			break;
    	case 3:	
    			
				  notifier->GetTriggers(lbt,triggerIdArray,wait);
    			notifier2->After(10);
    			notifier2->Setwait(wait);
    			wait->Start();
    			if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelGetTriggers();
                }
    			
    			break;
    	case 4:	
    			
				  notifier->GetTriggers(lbt,triggerIdArray,wait);
					notifier2->After(10);
					notifier2->Setwait(wait);
    			wait->Start();
    			if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelAll();
                }
    			break;
    	default:break;		
    }
    
    
    TInt error=notifier->iStatus.Int(); 
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(value==1 || value==2)
      {
      if(error!=KErrNone)
          {
          return KErrGeneral;
          }
      if(triggerIdArray.Count()==0)
          {
          return KErrGeneral;
          }
      triggerIdArray.ResetAndDestroy();
      return KErrNone;
      }
      else 
      {
       if(isCancelled)
          {
              if(error!=KErrCancel)
              {
              return KErrGeneral;
              }
              if(triggerIdArray.Count()!=0)
              {
              return KErrGeneral;
              }
       		}
       		triggerIdArray.ResetAndDestroy();
        return KErrNone;    
      }   
   }

// -----------------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelCreatetTriggerIterator
// This test case will test CancelCreateTriggerIterator of RLbt class.
// (other items were commented in a header).
// -----------------------------------------------------------------------------------------
//1)Cancel CreatetTriggerIterator immediately.
//2)Try to Cancel CreatetTriggerIterator after completion.
//3)Try to Cancel CreatetTriggerIterator from other subsession.
//4)Cancel CreatetTriggerIterator Using CancelAll.

TInt Ctestlbtcancellation::testCancelCreatetTriggerIteratorL(CStifItemParser& aItem )
   {
   	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 
  	 TInt value;
 	 aItem.GetNextInt(value);
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 
 	TRAP_IGNORE(lbt.DeleteTriggersL());
 	 	 
	    //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    notifier->CreateGetTriggerIterator(lbt,wait);
//   TBool iscancelled =ETrue;
   switch(value)
    {
    	case 0:lbt.CancelCreateTriggerIterator();
    			break;
    	
    	case 1:wait->Start();
    			lbt.CancelCreateTriggerIterator();
    			break;
    	case 2:lbt1.CancelCreateTriggerIterator();
    			wait->Start();
    			break;
    	case 3:lbt.CancelAll();
    			
    			break;
    	default:break;		
    }

    TInt error=notifier->iStatus.Int(); 
   /* CLbtTriggerInfo* trigInfo =lbt.GetNextTriggerLC(); 
    if(trigInfo==NULL)
        {
        iscancelled =ETrue;
     //   CleanupStack::PopAndDestroy( trigInfo );
        }
    else
        {
        iscancelled =EFalse;
        CleanupStack::PopAndDestroy( trigInfo );
        }*/
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
   if(value ==0 || value ==3)
    	{
    	if(error!=KErrCancel)
    		{
    		return KErrGeneral;
    		}
    	
    	return KErrNone;
    	}
    else 
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	
    	return KErrNone;	
    	}	
   }

// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelCreateTriggerFromDifferentSubsession.
// This test case will test CancelCreateTrigger of RLbt class from different subsession
// (other items were commented in a header).
// -------------------------------------------------------------------------------


TInt Ctestlbtcancellation::testCancelCreateTriggerFromDifferentSubsessionL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetNameL(_L("abc"));
          
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId,trigId1;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate);
    CleanupStack::PushL( notifier1 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait1);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    
    notifier1->CreateTriggers( lbt1,*trig,trigId1,ETrue,wait1 );
    lbt1.CancelCreateTrigger();	
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
                {
                User::LeaveIfError(KErrGeneral);
                }
	TInt error=notifier->iStatus.Int();    
    TInt error1=notifier1->iStatus.Int();
   
	TRAPD(error2, lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy( ););
    CleanupStack::PopAndDestroy(wait1);
    CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier1 );
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    
    if(error1!=KErrCancel || error!=KErrNone || error2!=KErrNone)
    	{
    	return KErrGeneral;
    	}
    return KErrNone;	
    }

// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelDeleteTriggersFromDifferentSubsession.
// This test case will test CancelDeleteTriggers of RLbt class from different subsession
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelDeleteTriggersFromDifferentSubsessionL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId,trigId1;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate);
    CleanupStack::PushL( notifier1 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait1);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    notifier1->CreateTriggers( lbt1,*trig,trigId1,ETrue,wait1 );
    wait1->Start();
    if(notifier1->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    RArray<TLbtTriggerId> triggerArray,triggerArray1;
    triggerArray.Append(trigId);
    triggerArray1.Append(trigId1);
    notifier->DeleteTriggers( lbt,triggerArray,wait );
    
    notifier1->DeleteTriggers( lbt1,triggerArray1,wait1 );
    lbt1.CancelDeleteTriggers();
    wait->Start();
    
	TInt error=notifier->iStatus.Int();    
    TInt error1=notifier1->iStatus.Int();
    TRAPD(error2, lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy( ););
    CleanupStack::PopAndDestroy(wait1);
    CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier1 );
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    triggerArray.Close();
    triggerArray1.Close();
    if(error!=KErrNone || error1!=KErrCancel ||error2!=KErrNotFound)
    	{
    	return KErrGeneral;
    	}
    return KErrNone;	
    }
    
// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelSetTriggersStateFromDifferentSubsession.
// This test case will test CancelSetTriggersState of RLbt class from different subsession.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelSetTriggersStateFromDifferentSubsessionL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId,trigId1;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate);
    CleanupStack::PushL( notifier1 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait1);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    notifier1->CreateTriggers( lbt1,*trig,trigId1,ETrue,wait1 );
    wait1->Start();
    if(notifier1->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );     
    
    notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateDisabled,wait);
    notifier1->SetTriggersState(lbt1,NULL,CLbtTriggerEntry::EStateDisabled,wait1);
    lbt1.CancelSetTriggersState();
    wait->Start();
    
	TInt error=notifier->iStatus.Int();    
    TInt error1=notifier1->iStatus.Int();
    CLbtTriggerInfo *Triginfo = NULL;
    Triginfo = lbt.GetTriggerLC(trigId);;
    
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=CLbtTriggerEntry::EStateDisabled)
    {
    	User::LeaveIfError(KErrGeneral);
    }
    CleanupStack::PopAndDestroy(Triginfo);
    CleanupStack::PopAndDestroy(filter);
    CleanupStack::PopAndDestroy(wait1);
    CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier1 );
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    
    if(error1!=KErrCancel || error!=KErrNone)
    	{
    	return KErrGeneral;
    	}
    return KErrNone;	
    }

// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelListTriggerIdsFromDifferentSubsession.
// This test case will test CancelListTriggerIds of RLbt class from different subsession.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelListTriggerIdsFromDifferentSubsessionL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 RLbtManager mgrclient;
  	 RLbtManager mgrclient1;
  	 
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	
 	 
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId,trigId1;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate);
    CleanupStack::PushL( notifier1 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait1);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    notifier1->CreateTriggers( lbt1,*trig,trigId1,ETrue,wait1 );
    wait1->Start();
    if(notifier1->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    RArray < TLbtTriggerId > triggerIdArray,triggerIdArray1;
    notifier->ListTriggerIds(lbt,triggerIdArray,wait);
    notifier1->ListTriggerIds(lbt1,triggerIdArray1,wait1);
    lbt1.CancelListTriggerIds();
    
    wait->Start();
    
	TInt error=notifier->iStatus.Int();    
    TInt error1=notifier1->iStatus.Int();
    if(triggerIdArray.Count()==0)
  	{
  	User::LeaveIfError(KErrGeneral);
  	}  

    CleanupStack::PopAndDestroy(wait1);
    CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier1 );
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    triggerIdArray.Close();
    triggerIdArray1.Close();
    if(error1!=KErrCancel || error!=KErrNone)
    	{
    	return KErrGeneral;
    	}
    return KErrNone;	
    }
 
 
// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelGetTriggersFromDifferentSubsession.
// This test case will test CancelGetTriggers of RLbt class from different subsession.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelGetTriggersFromDifferentSubsessionL(CStifItemParser& /*aItem*/)
   {
    RLbtServer lbtserver;
  	RLbt lbt;
  	RLbt lbt1;
  	RLbtManager mgrclient;
  	RLbtManager mgrclient1;
  	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
     
    User::LeaveIfError( lbt.Open(lbtserver));
	 	CleanupClosePushL( lbt );
	 	 
	 	User::LeaveIfError( lbt1.Open(lbtserver));
	 	CleanupClosePushL( lbt1 );
 	 
 		TRAP_IGNORE(lbt.DeleteTriggersL());
 	 
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
		CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId,trigId1;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate);
    CleanupStack::PushL( notifier1 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait1);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    notifier1->CreateTriggers( lbt1,*trig,trigId1,ETrue,wait1 );
    wait1->Start();
    if(notifier1->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    RPointerArray < CLbtTriggerInfo > triggerIdArray,triggerIdArray1;
    notifier->GetTriggers(lbt,triggerIdArray,wait);
    
    notifier1->GetTriggers(lbt1,triggerIdArray1,wait1);
    lbt1.CancelGetTriggers();
    
    wait->Start();
    
		TInt error=notifier->iStatus.Int();    
    TInt error1=notifier1->iStatus.Int();
	  if(triggerIdArray.Count()==0)
	  {
	  	User::LeaveIfError(KErrGeneral);
	  }  

    CleanupStack::PopAndDestroy(wait1);
    CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier1 );
 		CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig ); 
    
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    triggerIdArray.ResetAndDestroy();
    triggerIdArray1.ResetAndDestroy();
    if(error1!=KErrCancel || error!=KErrNone)
    	{
    	return KErrGeneral;
    	}
    return KErrNone;	
    }
   
// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelCreatetTriggerIteratorFromDifferentSubsession.
// This test case will test CancelCreatetTriggerIterator of RLbt class from different subsession.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelCreatetTriggerIteratorFromDifferentSubsessionL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 RLbtManager mgrclient;
  	 RLbtManager mgrclient1;
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 
 	TRAP_IGNORE(lbt.DeleteTriggersL());
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId,trigId1;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate);
    CleanupStack::PushL( notifier1 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait1);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    notifier1->CreateTriggers( lbt1,*trig,trigId1,ETrue,wait1 );
    wait1->Start();
    if(notifier1->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    notifier->CreateGetTriggerIterator(lbt,wait);
    
    notifier1->CreateGetTriggerIterator(lbt1,wait1);
    lbt1.CancelCreateTriggerIterator();
    
    wait->Start();
    
	TInt error=notifier->iStatus.Int();    
    TInt error1=notifier1->iStatus.Int();
    CLbtTriggerInfo* trigInfo =lbt.GetNextTriggerLC(); 
    if(trigInfo!=NULL)
        {
        CleanupStack::PopAndDestroy( trigInfo );
        }
   
    CleanupStack::PopAndDestroy(wait1);
    CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier1 );
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    
    if(error1!=KErrCancel || error!=KErrNone)
    	{
    	return KErrGeneral;
    	}
    return KErrNone;	
    }


// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelCreateTriggerFromDifferentProcess.
// This test case will test CancelCreateTrigger of RLbt class from different process.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelCreateTriggerFromDifferentProcessL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
  	 RLbt lbt;
  	 
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	TRAP_IGNORE(lbt.DeleteTriggersL());	 
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    
     _LIT( KFilename,"CancelCreateTest.exe" );
    
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    if(retVal == KErrNone)
	{
		proc.Resume();
		TRequestStatus status = KRequestPending;
		proc.Rendezvous(status);
		User::WaitForRequest(status);
	}	
    wait->Start();
    
	TInt error=notifier->iStatus.Int();    
	TRAPD(error2, lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy( ););
    
   
    CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    
    if(error==KErrCancel)
    	{
    	return KErrGeneral;
    	}
    if(error2==KErrNotFound)
        {
        return KErrGeneral;
        }
    return KErrNone;	
    }

// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelDeleteTriggersFromDifferentProcess.
// This test case will test CancelDeleteTriggers of RLbt class from different process.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelDeleteTriggersFromDifferentProcessL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
  	 RLbt lbt;
  
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	TRAP_IGNORE(lbt.DeleteTriggersL());
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
 
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    RArray<TLbtTriggerId> triggerArray;
    triggerArray.Append(trigId);
    
    notifier->DeleteTriggers( lbt,triggerArray,wait );
    
    _LIT( KFilename,"CancelDeleteTest.exe" );
    
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    if(retVal == KErrNone)
	{
		proc.Resume();
		TRequestStatus status = KRequestPending;
		proc.Rendezvous(status);
		User::WaitForRequest(status);
	}
	
    wait->Start();
    
	TInt error=notifier->iStatus.Int();    
	TRAPD(error2, lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy( ););
    
    if(error==KErrCancel || error2!=KErrNotFound)
    	{
    	CleanupStack::PopAndDestroy(wait);
        CleanupStack::PopAndDestroy( notifier );
        CleanupStack::PopAndDestroy( trig );
        CleanupStack::PopAndDestroy( &lbt );
        CleanupStack::PopAndDestroy( &lbtserver );
      triggerArray.Close();
    	return KErrGeneral;
    	}
    CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    triggerArray.Close();
    return KErrNone;	
    }

// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelSetTriggersStateFromDifferentProcess.
// This test case will test CancelSetTriggersState of RLbt class from different process.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelSetTriggersStateFromDifferentProcessL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
  	 RLbt lbt;
  	 
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	TRAP_IGNORE(lbt.DeleteTriggersL());
 	 
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL(); 
    CleanupStack::PushL( filter );     
    
    notifier->SetTriggersState(lbt,NULL,CLbtTriggerEntry::EStateDisabled,wait);
    _LIT( KFilename,"CancelSetTriggersTest.exe" );
    
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    if(retVal == KErrNone)
	{
		proc.Resume();
		TRequestStatus status = KRequestPending;
		proc.Rendezvous(status);
		User::WaitForRequest(status);
	}
    wait->Start();
    
	TInt error=notifier->iStatus.Int();    
	//Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter );
   
   
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    listOptions->SetFilter( attribFilter ); 
    
    CleanupStack::Pop( attribFilter );   
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    TRAP_IGNORE(lbt.GetTriggersL(trigInfoList,listOptions));
    CleanupStack::PopAndDestroy(listOptions);
    CleanupStack::PopAndDestroy(filter);
    CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrCancel)
    	{
    	return KErrGeneral;
    	}
    if(trigInfoList.Count()==0)
        {
        return KErrGeneral;
        }
    trigInfoList.ResetAndDestroy();
    return KErrNone;	
    }


// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelListTriggerIdsFromDifferentProcess.
// This test case will test CancelListTriggerIds of RLbt class from different process.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelListTriggerIdsFromDifferentProcessL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
     RLbt lbt;
     RLbtManager mgrclient;
  	 
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( mgrclient.Open(lbtserver));
 	 CleanupClosePushL( mgrclient );
 	 
 	TRAP_IGNORE(lbt.DeleteTriggersL());
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    RArray < TLbtTriggerId > triggerIdArray;
    notifier->ListTriggerIds(lbt,triggerIdArray,wait);
    
    _LIT( KFilename,"CancelListTriggersTest.exe" );
    
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    if(retVal == KErrNone)
	{
		proc.Resume();
		TRequestStatus status = KRequestPending;
		proc.Rendezvous(status);
		User::WaitForRequest(status);
	}
    wait->Start();
    
	TInt error=notifier->iStatus.Int();    
  if(triggerIdArray.Count()==0)
  {
  	User::LeaveIfError(KErrGeneral);
  }  
	CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &mgrclient);
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    triggerIdArray.Close();
   if(error==KErrCancel)
    	{
    	return KErrGeneral;
    	}
    return KErrNone;	
    }	


// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelGetTriggersFromDifferentProcess.
// This test case will test CancelGetTriggers of RLbt class from different process.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelGetTriggersFromDifferentProcessL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
  	 RLbt lbt;
     RLbtManager mgrclient;
  	 
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( mgrclient.Open(lbtserver));
 	 CleanupClosePushL( mgrclient );
 	 
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    RPointerArray < CLbtTriggerInfo > triggerIdArray;
    notifier->GetTriggers(lbt,triggerIdArray,wait);
    _LIT( KFilename,"CancelGetTriggersTest.exe" );
    
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    if(retVal == KErrNone)
	{
		proc.Resume();
		TRequestStatus status = KRequestPending;
		proc.Rendezvous(status);
		User::WaitForRequest(status);
	}
    wait->Start();
    
	TInt error=notifier->iStatus.Int();    
   if(triggerIdArray.Count()==0)
  {
  	User::LeaveIfError(KErrGeneral);
  }   

    CleanupStack::PopAndDestroy(wait);
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &mgrclient );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    triggerIdArray.ResetAndDestroy();
     if(error==KErrCancel)
    	{
    	return KErrGeneral;
    	}
    return KErrNone;
    }

// -------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelCreatetTriggerIteratorFromDifferentProcess.
// This test case will test CancelCreatetTriggerIterator of RLbt class from different process.
// (other items were commented in a header).
// -------------------------------------------------------------------------------
//

TInt Ctestlbtcancellation::testCancelCreatetTriggerIteratorFromDifferentProcessL(CStifItemParser& /*aItem*/)
   {
     RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbtManager mgrclient;
  	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( mgrclient.Open(lbtserver));
 	 CleanupClosePushL( mgrclient );
 	 
 	 
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CleanupStack::PushL(wait);
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start();
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    notifier->CreateGetTriggerIterator(lbt,wait);
    _LIT( KFilename,"CancelCreateIteratorTest.exe" );
    
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    if(retVal == KErrNone)
	{
		proc.Resume();
		TRequestStatus status = KRequestPending;
		proc.Rendezvous(status);
		User::WaitForRequest(status);
	}
    wait->Start();
    
	TInt error=notifier->iStatus.Int();    
   CLbtTriggerInfo* trigInfo =lbt.GetNextTriggerLC(); 
    if(trigInfo!=NULL)
        {
        CleanupStack::PopAndDestroy( trigInfo );
        } 

    CleanupStack::PopAndDestroy(wait);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &mgrclient );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    
     if(error==KErrCancel)
    	{
    	return KErrGeneral;
    	}
    return KErrNone;
    }



// -------------------------------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelDeleteTriggers
// This test case will test CancelDeleteTriggers of RLbt class for session trigger.
// (other items were commented in a header).
// -------------------------------------------------------------------------------------------------------
//1)Cancel Deletetriggers immediately.(session trigger)
//2)Try to Cancel Deletetriggers after completion.(session trigger)
//3)Try to Cancel Createtrigger from other subsession.(session trigger)
//4)Cancel Deletetriggers in the middle.(session trigger)
//5) Cancel Createtrigger Using CancelAll.(session trigger)

TInt Ctestlbtcancellation::testCancelDeleteTriggers3L(CStifItemParser& aItem)
   {
   RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 
  	 TInt value;
     aItem.GetNextInt(value);
     User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 	 
	   //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
		CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId=0;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt,coordinate);
        CleanupStack::PushL( notifier2 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
   
    TRAP_IGNORE(lbt.DeleteTriggersL());
    for(TInt i=0;i<20;i++) 
    {   
      notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
      wait->Start( );
      if(notifier->iStatus.Int()!=KErrNone)
        {
        User::LeaveIfError(KErrGeneral);
        }
    }
    TBool isCancelled = EFalse;
  switch(value)
    {
    	case 0:
    		notifier->DeleteTriggers( lbt,NULL,wait );
	    	lbt.CancelDeleteTriggers();
	    	isCancelled = ETrue;
    		//	notifier->Cancel();
    			break;
    	
    	case 1:
    			notifier->DeleteTriggers( lbt,NULL,wait );
    			wait->Start();
    			lbt.CancelDeleteTriggers();
    			break;
    	case 2:
    			notifier->DeleteTriggers( lbt,NULL,wait );
    			lbt1.CancelDeleteTriggers();	
    			wait->Start();
    			break;
    	case 3:	
    			notifier->DeleteTriggers( lbt,NULL,wait );
    			notifier2->After(10);
    			notifier2->Setwait(wait);
    			wait->Start();
    			if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelDeleteTriggers();
                }
    			
    			break;
    	case 4:	
                notifier->DeleteTriggers( lbt,NULL,wait );
                notifier2->After(10);
                notifier2->Setwait(wait);
    			wait->Start();
    			if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelAll();
                }
    			
    			break;
    			
    	default:break;		
    }
  
    TInt error=notifier->iStatus.Int(); 
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    TRAP_IGNORE(lbt.GetTriggersL(trigInfoList ));
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
     CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(value==1 || value==2)
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	if(trigInfoList.Count()!=0)
    		{
    		return KErrGeneral;
    		}
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;
    	}
    else 
    	{
    	
        if(isCancelled)
        {
            if(error!=KErrCancel)
            {
            return KErrGeneral;
            }
            if(trigInfoList.Count()==0)
            {
            return KErrGeneral;
            }
        }
    	
    	
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;	
    	}	


   }
   
   // -------------------------------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelDeleteTriggers
// This test case will test CancelDeleteTriggers of RLbt class for startuptrigger.
// (other items were commented in a header).
// -------------------------------------------------------------------------------------------------------
//1)Cancel Deletetriggers immediately.(startup trigger)
//2)Try to Cancel Deletetriggers after completion.(startup trigger)
//3)Try to Cancel Deletetriggers from other subsession.(startup trigger)
//4)Cancel Deletetriggers in the middle.(startup trigger)
//5) Cancel Deletetriggers Using CancelAll.(startup trigger)

TInt Ctestlbtcancellation::testCancelDeleteTriggers4L(CStifItemParser& aItem)
   {
   	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 RLbt lbt1;
  	 
  	 TInt value;
     aItem.GetNextInt(value);
     User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 CleanupClosePushL( lbt1 );
 	 	 
	   //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
       
  	TSecureId secureid;
  	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);             
          
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId=0;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt,coordinate);
        CleanupStack::PushL( notifier2 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
   
    TRAP_IGNORE(lbt.DeleteTriggersL());
    for(TInt i=0;i<20;i++) 
    {   
      notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
      wait->Start( );
      if(notifier->iStatus.Int()!=KErrNone)
        {
        User::LeaveIfError(KErrGeneral);
        }
    }
    TBool isCancelled = EFalse;
  switch(value)
    {
    	case 0:
    		notifier->DeleteTriggers( lbt,NULL,wait );
	    	lbt.CancelDeleteTriggers();
	    	isCancelled = ETrue;
    		//	notifier->Cancel();
    			break;
    	
    	case 1:
    			notifier->DeleteTriggers( lbt,NULL,wait );
    			wait->Start();
    			lbt.CancelDeleteTriggers();
    			break;
    	case 2:
    			notifier->DeleteTriggers( lbt,NULL,wait );
    			lbt1.CancelDeleteTriggers();	
    			wait->Start();
    			break;
    	case 3:    
                notifier->DeleteTriggers( lbt,NULL,wait );
                notifier2->After(10);
                notifier2->Setwait(wait);
                wait->Start();
                if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelDeleteTriggers();
                }
                
                break;
        case 4: 
                notifier->DeleteTriggers( lbt,NULL,wait );
                notifier2->After(10);
                notifier2->Setwait(wait);
                wait->Start();
                if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelAll();
                }
                break;
    			
    	default:break;		
    }
  
    TInt error=notifier->iStatus.Int(); 
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    TRAP_IGNORE(lbt.GetTriggersL(trigInfoList ));
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
     CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(value==1 || value==2)
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    		if(trigInfoList.Count()!=0)
    		{
    		return KErrGeneral;
    		}
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;
    	}
    else 
    	{
    	if(isCancelled)
    	        {
    	            if(error!=KErrCancel)
    	            {
    	            return KErrGeneral;
    	            }
    	            if(trigInfoList.Count()==0)
                    {
                    return KErrGeneral;
                    }
    	        }
    	
    	
    	trigInfoList.ResetAndDestroy();
    	return KErrNone;	
    	}	

   }

// -------------------------------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelUpdateTrigger1L
// This test case will test CancelUpdateTrigger of RLbt class for startuptrigger.
// (other items were commented in a header).
// -------------------------------------------------------------------------------------------------------
//1)Cancel UpdateTrigger immediately.(startup trigger)
//2)Try to Cancel UpdateTrigger after completion.(startup trigger)
//3)Try to Cancel UpdateTrigger from other subsession.(startup trigger)
//4) Cancel UpdateTrigger Using CancelAll.(startup trigger)
   
   TInt Ctestlbtcancellation::testCancelUpdateTrigger1L( CStifItemParser&  aItem )
    {
 	 RLbtServer lbtserver;
 	 RLbt lbt,lbt1;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
   CleanupClosePushL( lbtserver );
   
   TInt value;
	 aItem.GetNextInt(value);
     
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open( lbtserver ) );
 	 CleanupClosePushL( lbt1 );
 	 
 	TRAP_IGNORE(lbt.DeleteTriggersL());
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
    CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    trig->SetManagerUi(aManagerUi);   
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
     TLbtTriggerId trigId;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    
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
    switch(value)
    {
    	case 0:	lbt.CancelUpdateTrigger();
    		//	notifier->Cancel();
    			break;
    	
    	case 1:wait->Start();
    			lbt.CancelUpdateTrigger();
    			break;
    	case 2:lbt1.CancelUpdateTrigger();	
    			wait->Start();
    			break;
    	case 3:	lbt.CancelAll();
    			break;
    	default:break;		
    }
    	
		TInt error=notifier->iStatus.Int();    
    CLbtTriggerInfo *Triginfo = NULL;
    Triginfo = lbt.GetTriggerLC(trigId);;
    
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TBool isUpdated=EFalse;
    if(TrigEntry->Name()==newtrig->Name()) 
    {
        isUpdated = ETrue;
    }
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   // #endif
   CleanupStack::Pop( &lbt1 );
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
  	if(value==0 ||value==3)
    	{
    	if(error!=KErrCancel)
    		{
    		return KErrGeneral;
    		}
    	
   		if(!isUpdated)
	   	{	
    	return KErrNone;
    	}
    	else
    	{
    	return KErrGeneral;	
    	}
    	}
    else 
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	if(isUpdated)
	   	{	
    	return KErrNone;
    	}
    	else
    	{
    	return KErrGeneral;	
    	}	
    	}	
      
    }
   // -------------------------------------------------------------------------------------------------------
   // Ctestlbtcancellation::testCancelUpdateTrigger2L
   // This test case will test CancelUpdateTrigger of RLbt class for sessiontrigger.
   // (other items were commented in a header).
   // -------------------------------------------------------------------------------------------------------
   //1)Cancel UpdateTrigger immediately.(session trigger)
   //2)Try to Cancel UpdateTrigger after completion.(session trigger)
   //3)Try to Cancel UpdateTrigger from other subsession.(session trigger)
   //4) Cancel UpdateTrigger Using CancelAll.(session trigger)   
    TInt Ctestlbtcancellation::testCancelUpdateTrigger2L( CStifItemParser&  aItem )
    {
 	 RLbtServer lbtserver;
 	 RLbt lbt,lbt1;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
 	 CleanupClosePushL( lbtserver );
 	 
 	 TInt value;
	 aItem.GetNextInt(value);
     
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	 User::LeaveIfError( lbt1.Open( lbtserver ) );
 	 CleanupClosePushL( lbt1 );
 	 iLog->Log(_L("After RLbt::Open "));
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 //	  #if 0
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

    //Set Requestor
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    trig->SetManagerUi(aManagerUi);   
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
     TLbtTriggerId trigId;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    
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
    ,wait,ELbtTrue
    );
    switch(value)
    {
    	case 0:	lbt.CancelUpdateTrigger();
    		//	notifier->Cancel();
    			break;
    	
    	case 1:wait->Start();
    			lbt.CancelUpdateTrigger();
    			break;
    	case 2:lbt1.CancelUpdateTrigger();	
    			wait->Start();
    			break;
    	case 3:	lbt.CancelAll();
    			break;
    	default:break;		
    }
    	
		TInt error=notifier->iStatus.Int();    
    CLbtTriggerInfo* TrigInfo=NULL;
    TrigInfo=lbt.GetTriggerLC(trigId);
    CLbtTriggerEntry *TrigEntry = TrigInfo->TriggerEntry();
    
    TBool isUpdated=EFalse;
    if(TrigEntry->Name()==newtrig->Name()) 
    {
        isUpdated = ETrue;
    }
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   // #endif
   CleanupStack::Pop( &lbt1 );
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
  	if(value==0 ||value==3)
    	{
    	if(error!=KErrCancel)
    		{
    		return KErrGeneral;
    		}
    	
   		if(!isUpdated)
	   	{	
    	return KErrNone;
    	}
    	else
    	{
    	return KErrGeneral;	
    	}
    	}
    else 
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	if(isUpdated)
	   	{	
    	return KErrNone;
    	}
    	else
    	{
    	return KErrGeneral;	
    	}	
    	}	
      
    }
    // -------------------------------------------------------------------------------------------------------
   // Ctestlbtcancellation::testCancelNotifyTriggerChangeEvent1L
   // This test case will test CancelNotifyTriggerChangeEvent of RLbt class for startuptrigger.
   // (other items were commented in a header).
   // -------------------------------------------------------------------------------------------------------
   //1)Cancel NotifyTriggerChangeEvent immediately.(startup trigger)
   //2)Try to Cancel NotifyTriggerChangeEvent after completion.(startup trigger)
   //3)Try to Cancel NotifyTriggerChangeEvent from other subsession.(startup trigger)
   //4) Cancel NotifyTriggerChangeEvent Using CancelAll.(startup trigger)   
  TInt Ctestlbtcancellation::testCancelNotifyTriggerChangeEvent1L( CStifItemParser& aItem )
  {

      RLbtServer server;
    RLbt lbt1,lbt2;
  
    TInt value;
        aItem.GetNextInt(value);    
        
    User::LeaveIfError(server.Connect());
    CleanupClosePushL( server );
    
    User::LeaveIfError(lbt1.Open(server));
    CleanupClosePushL( lbt1 );
    
    User::LeaveIfError(lbt2.Open(server));
    CleanupClosePushL( lbt2 );
    TRAP_IGNORE(lbt2.DeleteTriggersL());
    TCoordinate coordinate(61.5285,23.9385,0);
    
    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate);
    CleanupStack::PushL( notifier1 );
    //notifier->After(50000000); 
    
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    
      //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);   
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig->SetRequestorL(ReqType,ReqFormat,ReqData);  

    // set condition
          
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
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2,coordinate );
    CleanupStack::PushL( notifier2);
    notifier2->CreateTriggers( lbt2,*trig,trigId,ETrue,wait2 );
    wait2->Start( );
    if(notifier2->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    TLbtTriggerChangeEvent myEvent;
    notifier2->NotifyTriggerChangeEvent(lbt2,myEvent,wait2);
    notifier1->After(10000);
    notifier1->Setwait(wait1);
    wait1->Start( );
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
    
    switch(value)
{
    case 0:	lbt2.CancelNotifyTriggerChangeEvent();
        //	notifier->Cancel();
            break;
    
    case 1:
            lbt2.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
            |CLbtTriggerEntry::EAttributeState
            |CLbtTriggerEntry::EAttributeCondition,
            ELbtTrue
            );
            wait2->Start();
            lbt2.CancelNotifyTriggerChangeEvent();
            break;
    case 2:
            lbt1.CancelNotifyTriggerChangeEvent();	
            lbt2.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
            |CLbtTriggerEntry::EAttributeState
            |CLbtTriggerEntry::EAttributeCondition
            |CLbtTriggerEntry::EAttributeStartUpCommandLine,
            ELbtTrue
            );
            wait2->Start();
            break;
    case 3:	lbt2.CancelAll();
            break;
    default:break;		
    }
	  
	    TInt error=notifier2->iStatus.Int();    
	    CleanupStack::PopAndDestroy(notifier2);
	   
        CleanupStack::PopAndDestroy(trig);
        CleanupStack::PopAndDestroy(notifier1);
        //CleanupStack::PopAndDestroy(&mgrclient);
        CleanupStack::PopAndDestroy(&lbt2);
        CleanupStack::PopAndDestroy(&lbt1);
        CleanupStack::PopAndDestroy(&server);
        delete wait1;
        delete wait2;
        if(value==0 ||value==3)
    	{
    	if(error!=KErrCancel)
    		{
    		return KErrGeneral;
    		}
    	   		
    	return KErrNone;
    	
    	}
    else 
    	{
    	if(error!=KErrNone)
    		{
    		return KErrGeneral;
    		}
    	
    	return KErrNone;
    	
    	}	
	   }
    // -------------------------------------------------------------------------------------------------------
    // Ctestlbtcancellation::testCancelNotifyTriggerChangeEvent2L
    // This test case will test CancelNotifyTriggerChangeEvent of RLbt class for sessiontrigger.
    // (other items were commented in a header).
    // -------------------------------------------------------------------------------------------------------
    //1)Cancel NotifyTriggerChangeEvent immediately.(session trigger)
    //2)Try to Cancel NotifyTriggerChangeEvent after completion.(session trigger)
    //3)Try to Cancel NotifyTriggerChangeEvent from other subsession.(session trigger)
    //4) Cancel NotifyTriggerChangeEvent Using CancelAll.(session trigger)    
	  TInt Ctestlbtcancellation::testCancelNotifyTriggerChangeEvent2L( CStifItemParser& aItem )
      {

      RLbtServer server;
       RLbt lbt1,lbt2;
     
       TInt value;
           aItem.GetNextInt(value);    
           
       User::LeaveIfError(server.Connect());
       CleanupClosePushL( server );
       
       User::LeaveIfError(lbt1.Open(server));
       CleanupClosePushL( lbt1 );
       
       User::LeaveIfError(lbt2.Open(server));
       CleanupClosePushL( lbt2 );
       TRAP_IGNORE(lbt2.DeleteTriggersL());
       TCoordinate coordinate(61.5285,23.9385,0);
       
       CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate);
       CleanupStack::PushL( notifier1 );
       //notifier->After(50000000); 
       
       CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
       
         //Construct a startup trigger
       CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
       
       //Push to cleanup stack
       CleanupStack::PushL( trig );
       
       // Set Name
       trig->SetNameL(_L("Trigger1"));
       
      
       CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
       CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
       TBuf<KLbtMaxNameLength> ReqData=_L("");
       trig->SetRequestorL(ReqType,ReqFormat,ReqData);  

       // set condition
             
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
       CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2,coordinate );
       CleanupStack::PushL( notifier2);
       notifier2->CreateTriggers( lbt2,*trig,trigId,ETrue,wait2 );
       wait2->Start( );
       if(notifier2->iStatus.Int()!=KErrNone)
               {
               User::LeaveIfError(KErrGeneral);
               }
       TLbtTriggerChangeEvent myEvent;
       notifier2->NotifyTriggerChangeEvent(lbt2,myEvent,wait2);
       notifier1->After(10000);
       notifier1->Setwait(wait1);
       wait1->Start( );
       //**************New Trigger Entry**************/ 
       CLbtSessionTrigger* newtrig = CLbtSessionTrigger::NewL();
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
   //  trigId = 1;
       newtrig->SetId(trigId);
       
       switch(value)
       {
       case 0: lbt2.CancelNotifyTriggerChangeEvent();
           //  notifier->Cancel();
               break;
       
       case 1:
               lbt2.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
               |CLbtTriggerEntry::EAttributeState
               |CLbtTriggerEntry::EAttributeCondition,
               ELbtTrue
               );
               wait2->Start();
               lbt2.CancelNotifyTriggerChangeEvent();
               break;
       case 2:
               lbt1.CancelNotifyTriggerChangeEvent();  
               lbt2.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
               |CLbtTriggerEntry::EAttributeState
               |CLbtTriggerEntry::EAttributeCondition,
                           ELbtTrue
               );
               wait2->Start();
               break;
       case 3: lbt2.CancelAll();
               break;
       default:break;      
       }
         
           TInt error=notifier2->iStatus.Int();    
           CleanupStack::PopAndDestroy(notifier2);
          
           CleanupStack::PopAndDestroy(trig);
           CleanupStack::PopAndDestroy(notifier1);
           //CleanupStack::PopAndDestroy(&mgrclient);
           CleanupStack::PopAndDestroy(&lbt2);
           CleanupStack::PopAndDestroy(&lbt1);
           CleanupStack::PopAndDestroy(&server);
           delete wait1;
           delete wait2;
           if(value==0 ||value==3)
           {
           if(error!=KErrCancel)
               {
               return KErrGeneral;
               }
                   
           return KErrNone;
           
           }
       else 
           {
           if(error!=KErrNone)
               {
               return KErrGeneral;
               }
           
           return KErrNone;
           
           }   
	   }
	  //1)Cancel NotifyTriggerFired immediately.(session trigger)
      //2)Try to Cancel NotifyTriggerFired after completion.(session trigger)
      //3)Try to Cancel NotifyTriggerFired from other subsession.(session trigger)
      //4) Cancel NotifyTriggerFired Using CancelAll.(session trigger)       
	TInt Ctestlbtcancellation::testCanceltNotifyTriggerFiredL( CStifItemParser& aItem )
    {
    RLbtServer lbtserver;
    RLbt lbt1,lbt2;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt1.Open(lbtserver));
 	CleanupClosePushL( lbt1 );
 	User::LeaveIfError( lbt2.Open(lbtserver));
 	CleanupClosePushL( lbt2 );
 	
 	TInt value;
 	aItem.GetNextInt(value);    
 	
 	_LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	
 	//Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
     TRAP_IGNORE(lbt1.DeleteTriggersL()) ;       
     
     CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
     iLog->Log(_L("Simulation PSY Repository object created"));
     User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
     iLog->Log(_L("Simulation input file set "));
     CleanupStack::PopAndDestroy(repository);
 	     
 	   //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
   CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
   TBuf<KLbtMaxNameLength> ReqData=_L("");
   trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    // set condition

    TCoordinate coordinate(62.5285,23.9385);
   // TCoordinate coordinate(62.4438,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,10000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt1,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt1,*trig,trigId,ETrue,wait );
    wait->Start( );
    if(notifier->iStatus.Int()!=KErrNone)
            {
            User::LeaveIfError(KErrGeneral);
            }
    TLbtTriggerFireInfo triggerFireInfo;
    
    //TRequestStatus requestStatus( KErrNone );      
    notifier->NotifyTriggerFired(lbt1,triggerFireInfo,wait);
    switch(value)
           {
           case 0: lbt1.CancelNotifyTriggerFired();
               //  notifier->Cancel();
                   break;
           case 1:
                   wait->Start();
                  lbt1.CancelNotifyTriggerFired();  
                  break;
          
           case 2:
                   lbt2.CancelNotifyTriggerFired();  
                   wait->Start();
                   break;
           case 3: lbt1.CancelAll();
                   break;
           default:break;      
       }
            
    
    TInt error=notifier->iStatus.Int();    
 	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt2 );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    if(value==0 ||value==3)
    {
    if(error!=KErrCancel)
        {
        return KErrGeneral;
        }
            
    return KErrNone;
    
    }
    else 
    {
    if(error!=KErrNone)
        {
        return KErrGeneral;
        }
    
    return KErrNone;
    
    }   
   
  }  
  
  TInt Ctestlbtcancellation::testCancelNotifyTriggeringSettingsChangeL( CStifItemParser&  aItem  )
    {
	 
	 RLbtServer lbtserver;
	 RLbt lbt1,lbt2;
 	 RLbtManager mgrclient;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt1.Open( lbtserver ) );
 	 CleanupClosePushL( lbt1 );
 	 User::LeaveIfError( lbt2.Open( lbtserver ) );
 	 CleanupClosePushL( lbt2 );
 	 User::LeaveIfError(mgrclient.Open(lbtserver));
 	 CleanupClosePushL( mgrclient );
 	 
 	TInt value;
 	aItem.GetNextInt(value);    
     TCoordinate coordinate;
     TLbtTriggeringSystemSettings settings,dummySettings;
     TLbtTriggeringSystemManagementSettings mgmtsettings;
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt1,coordinate);
     CleanupStack::PushL( notifier );
     
     CActiveSchedulerWait *waitLoop = new(ELeave) CActiveSchedulerWait;
     notifier->NotifyTriggeringSettingsChange( lbt1,settings,dummySettings,waitLoop ); 
     
     switch(value)
            {
            case 0: lbt1.CancelNotifyTriggeringSystemSettingChange();
                //  notifier->Cancel();
                    break;
            case 1:
                    mgmtsettings.SetTriggeringMechanismState(ETriggeringMechanismOff);
                    mgrclient.SetTriggeringSystemSettingsL(mgmtsettings,ETriggeringMechanismState);        
                    waitLoop->Start();
                   lbt1.CancelNotifyTriggeringSystemSettingChange();  
                   break;
           
            case 2:
                    lbt2.CancelNotifyTriggeringSystemSettingChange();  
                    mgmtsettings.SetTriggeringMechanismState(ETriggeringMechanismOn);
                    mgrclient.SetTriggeringSystemSettingsL(mgmtsettings,ETriggeringMechanismState);     
                    waitLoop->Start();
                    break;
            case 3: lbt1.CancelAll();
                    break;
            default:break;      
            }
    
     TInt error=notifier->iStatus.Int(); ;
     
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy( &mgrclient );
     CleanupStack::PopAndDestroy( &lbt2 );
     CleanupStack::PopAndDestroy( &lbt1 );
     CleanupStack::PopAndDestroy( &lbtserver );
     if(value==0 ||value==3)
         {
         if(error!=KErrCancel)
             {
             return KErrGeneral;
             }
                 
         return KErrNone;
         
         }
         else 
         {
         if(error!=KErrNone)
             {
             return KErrGeneral;
             }
         
         return KErrNone;
         
         }   
	 
    }        
  
  TInt Ctestlbtcancellation::testlbtmgrCancelNotifyTriggeringSettingsChangeL( CStifItemParser&  aItem  )
      {
       
       RLbtServer lbtserver;
       RLbtManager mgrclient1,mgrclient2;
       
       User::LeaveIfError( lbtserver.Connect() );
       CleanupClosePushL( lbtserver );
       User::LeaveIfError( mgrclient1.Open( lbtserver ) );
       CleanupClosePushL( mgrclient1 );
       User::LeaveIfError( mgrclient2.Open( lbtserver ) );
       CleanupClosePushL( mgrclient2 );
       
      TInt value;
      aItem.GetNextInt(value);    
       TCoordinate coordinate;
     
       TLbtTriggeringSystemManagementSettings mgmtsettings,dummySettings;
       CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( mgrclient1,coordinate);
       CleanupStack::PushL( notifier );
       
       CActiveSchedulerWait *waitLoop = new(ELeave) CActiveSchedulerWait;
       notifier->NotifyTriggeringSettingsChange( mgrclient1,mgmtsettings,dummySettings,waitLoop ); 
       
       switch(value)
              {
              case 0: mgrclient1.CancelNotifyTriggeringSystemSettingChange();
                  //  notifier->Cancel();
                      break;
              case 1:
                      mgmtsettings.SetMinimumLocationUpdateInterval(1);
                      mgrclient1.SetTriggeringSystemSettingsL(mgmtsettings,EMinimumLocationUpdateInterval);        
                      waitLoop->Start();
                      mgrclient1.CancelNotifyTriggeringSystemSettingChange();  
                     break;
             
              case 2:
                  mgrclient2.CancelNotifyTriggeringSystemSettingChange();  
                  mgmtsettings.SetTriggeringMechanismState(ETriggeringMechanismOff);
                   mgrclient1.SetTriggeringSystemSettingsL(mgmtsettings,ETriggeringMechanismState);     
                      waitLoop->Start();
                      break;
              case 3: mgrclient1.CancelAll();
                      break;
              default:break;      
              }
      
       TInt error=notifier->iStatus.Int(); 
       
       CleanupStack::PopAndDestroy( notifier );
       CleanupStack::PopAndDestroy( &mgrclient2 );
       CleanupStack::PopAndDestroy( &mgrclient1 );
       CleanupStack::PopAndDestroy( &lbtserver );
       delete waitLoop;
       if(value==0 ||value==3)
           {
           if(error!=KErrCancel)
               {
               return KErrGeneral;
               }
                   
           return KErrNone;
           
           }
           else 
           {
           if(error!=KErrNone)
               {
               return KErrGeneral;
               }
           
           return KErrNone;
           
           }   
      }       
  
  TInt Ctestlbtcancellation::testCancelNotifyTriggeringSystemStatusChangeL( CStifItemParser& aItem )
      {

          RLbtServer server;
          
          RLbtManager mgrclient1,mgrclient2;
              
          User::LeaveIfError(server.Connect());
          CleanupClosePushL( server );
          
             
          User::LeaveIfError(mgrclient1.Open(server));
          CleanupClosePushL( mgrclient1 );
          User::LeaveIfError(mgrclient2.Open(server));
          CleanupClosePushL( mgrclient2 );
          TCoordinate coordinate;
          TInt value;
          aItem.GetNextInt(value);    
          CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( mgrclient1,coordinate);
          CleanupStack::PushL( notifier );
          
          CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
          
          notifier->NotifyTriggeringSystemStatusChange(mgrclient1,wait);
          TLbtTriggeringSystemStatus TriggeringSystemStatus;
          switch(value)
                {
                case 0: mgrclient1.CancelNotifyTriggeringSystemStatusChange();
                    //  notifier->Cancel();
                        break;
                case 1:
                    
                            
                    mgrclient1.GetTriggeringSystemStatusL(TriggeringSystemStatus);
                    if(TriggeringSystemStatus.iLocationAcquisitionStatus ==  ELocationAcquisitionOff)
                    {        
                    mgrclient1.SetTriggeringMechanismStateL(ETriggeringMechanismOn);  
                    }
                    else
                    {
                    mgrclient1.SetTriggeringMechanismStateL(ETriggeringMechanismOff);  
                    }
                    wait->Start();
                    mgrclient1.CancelNotifyTriggeringSystemStatusChange();  
                    break;
               
                case 2:
                    mgrclient2.CancelNotifyTriggeringSystemStatusChange();  
                    mgrclient1.GetTriggeringSystemStatusL(TriggeringSystemStatus);
                    if(TriggeringSystemStatus.iLocationAcquisitionStatus ==  ELocationAcquisitionOff)
                    {        
                    mgrclient1.SetTriggeringMechanismStateL(ETriggeringMechanismOn);  
                    }
                    else
                    {
                    mgrclient1.SetTriggeringMechanismStateL(ETriggeringMechanismOff);  
                    }
                    wait->Start();
                        break;
                case 3: mgrclient1.CancelAll();
                        break;
                default:break;      
                }
          TInt error=notifier->iStatus.Int(); 
          CleanupStack::PopAndDestroy(notifier);
          CleanupStack::PopAndDestroy(&mgrclient2);
          CleanupStack::PopAndDestroy(&mgrclient1);
          CleanupStack::PopAndDestroy(&server);
          delete wait;
          if(value==0 ||value==3)
              {
              if(error!=KErrCancel)
                  {
                  return KErrGeneral;
                  }
                      
              return KErrNone;
              
              }
              else 
              {
              if(error!=KErrNone)
                  {
                  return KErrGeneral;
                  }
              
              return KErrNone;
              
              }   
          

      }
  
//#if 0
  // -------------------------------------------------------------------------------------------------------
// Ctestlbtcancellation::testCancelDeleteTriggers
// This test case will test CancelDeleteTriggers of RLbt class for startuptrigger.
// (other items were commented in a header).
// -------------------------------------------------------------------------------------------------------


TInt Ctestlbtcancellation::testPartialDeleteTriggers1L(CStifItemParser& aItem)
   {
     RLbtServer lbtserver;
     RLbt lbt;
     RLbt lbt1;
     
     TInt value;
     aItem.GetNextInt(value);
     User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     
     User::LeaveIfError( lbt.Open(lbtserver));
     CleanupClosePushL( lbt );
     
     User::LeaveIfError( lbt1.Open(lbtserver));
     CleanupClosePushL( lbt1 );
         
       //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
       
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);             
          
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
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

    TLbtTriggerId trigId=0;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate);
        CleanupStack::PushL( notifier1 );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt1,coordinate);
        CleanupStack::PushL( notifier2 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
   
    TRAP_IGNORE(lbt.DeleteTriggersL());
    for(TInt i=0;i<20;i++) 
    {   
      notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
      wait->Start( );
      if(notifier->iStatus.Int()!=KErrNone)
        {
        User::LeaveIfError(KErrGeneral);
        }
    }
    
    TLbtTriggerChangeEvent myEvent;
    notifier2->NotifyTriggerChangeEvent(lbt1,myEvent,wait2);
    //notifier2->Setwait(wait2);
    TBool isCancelled = EFalse;
  switch(value)
    {
        case 1:    
                notifier->DeleteTriggers( lbt,NULL,wait );
                notifier1->After(5);
                notifier1->Setwait(wait);
                wait->Start();
                
                if(notifier->iStatus==KRequestPending)
                {
                isCancelled = ETrue;
                lbt.CancelDeleteTriggers();
                }
                notifier2->Setwait(wait2);
                wait2->Start();
                break;
        
                
        default:break;      
    }
  
    TInt error=notifier->iStatus.Int(); 
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    TRAP_IGNORE(lbt.GetTriggersL(trigInfoList ));
    CleanupStack::PopAndDestroy( notifier2 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
     CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    delete wait2;
    
   
    if(isCancelled)
        {
            if(error!=KErrCancel)
            {
            return KErrGeneral;
            }
            if(trigInfoList.Count()==0)
            {
            return KErrGeneral;
            }
        }
        
        
        trigInfoList.ResetAndDestroy();
        return KErrNone;    
     }
//#endif
//  [End of File] - Do not remove
