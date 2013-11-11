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
* Description:    Test module to implement Session trigger creation related test cases
*
*/





// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "CreateSessiontrigger.h"
#include "MyObserver.h"
#include <lbt.h> 
#include <lbtserver.h>
#include <e32property.h>
//#include "lbtstatuspskeys.h"
#include <lbtgeocircle.h> 
#include <lbtgeorect.h>
#include <lbttriggerconditionarea.h> 
#include <lbtsessiontrigger.h> 

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
// CCreateSessiontrigger::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCreateSessiontrigger::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CCreateSessiontrigger::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCreateSessiontrigger::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", CCreateSessiontrigger::ExampleL ),
        ENTRY( "CreateSessiontrigger",CCreateSessiontrigger::CreatesingleSessiontriggerL),
        ENTRY( "CreateExittypeSessiontrigger",CCreateSessiontrigger::CreateExittypeSessiontriggerL),
        ENTRY( "CreateDisabledSessiontrigger",CCreateSessiontrigger::CreateDisabledSessiontriggerL),
        ENTRY( "CreatemultipeSessiontriggers",CCreateSessiontrigger::CreatemultipleSessiontriggersL),
        ENTRY( "CreateSessiontriggerwithoutConnect",CCreateSessiontrigger::CreateSessiontriggerwithoutConnectL),
        ENTRY( "CreateSessiontriggerwithoutOpen",CCreateSessiontrigger::CreateSessiontriggerwithoutOpenL),
        ENTRY( "SessiontriggerNameexceeds256L",CCreateSessiontrigger::SessiontriggerNameexceeds256L),
        ENTRY( "SessiontriggerNameequals0",CCreateSessiontrigger::SessiontriggerNameequals0L),
        ENTRY( "CreateSessiontriggerwithoutName",CCreateSessiontrigger::CreateSessiontriggerwithoutNameL),
        ENTRY( "CreateSessiontriggerwithoutcond",CCreateSessiontrigger::CreateSessiontriggerwithoutcondL),
        ENTRY( "CreateSessiontriggermanagerUINotFound",CCreateSessiontrigger::CreateSessiontriggermanagerUINotFoundL),
        ENTRY( "CreateSessiontriggerwithNonUIApplication",CCreateSessiontrigger::CreateSessiontriggerwithNonUIApplicationL),
        ENTRY( "ErrortriggerAreaToosmall",CCreateSessiontrigger::ErrortriggerAreaToosmallL),
        ENTRY( "ErrortriggerAreaRectangle",CCreateSessiontrigger::ErrortriggerAreaRectangleL),
        ENTRY( "CreateSessiontriggerwithoutRequestor",CCreateSessiontrigger::CreateSessiontriggerwithoutRequestorL),

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CCreateSessiontrigger::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCreateSessiontrigger::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KCreateSessiontrigger, "CreateSessiontrigger" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KCreateSessiontrigger, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KCreateSessiontrigger, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }
    // -----------------------------------------------------------------------------
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::CreatesingleSessiontriggerL( CStifItemParser& aItem )
    {
        //Connect to Location Triggering Server
   
  // #if 0
    RLbtServer lbtServ;
    User::LeaveIfError(lbtServ.Connect());
    CleanupClosePushL(lbtServ);

    RLbt lbt;
    User::LeaveIfError(lbt.Open(lbtServ));
    CleanupClosePushL(lbt);
        
    TRealX nan;
    nan.SetNaN();
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =100; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;
  //  #endif
    #if 0
    RLbtServer lbtServ;
    User::LeaveIfError(lbtServ.Connect());
    CleanupClosePushL(lbtServ);

    RLbt lbt;
    User::LeaveIfError(lbt.Open(lbtServ));
    CleanupClosePushL(lbt);
    CLbtSessionTrigger *iTrigger;
    CLbtGeoCircle* circle = CLbtGeoCircle::NewLC();
		
	iTrigger = CLbtSessionTrigger::NewL();
		
	iTrigger->SetNameL(_L("LbtRefAppTrigger"));
	iTrigger->SetState(CLbtTriggerEntry::EStateEnabled);
	/*iTrigger->SetRequestorL(CRequestorBase::ERequestorService,
						   CRequestorBase::EFormatApplication,
						   _L("LbtRefAppTrigger"));*/
	iTrigger->SetManagerUi(TUid::Uid(0x10181081));
	
	CLbtTriggerConditionArea* conditionBasic = CLbtTriggerConditionArea::NewL(circle, CLbtTriggerConditionArea::EFireOnEnter);
	CleanupStack::PushL(conditionBasic);
	
	iTrigger->SetCondition(conditionBasic);	
	 TLbtTriggerId iTriggerid;
    TRequestStatus iStatus;
	CMyObserver *ao;
	ao = CMyObserver::NewL();

//	test->CallCreateTrigger(lbt,&iTrigger,iTriggerid,EFalse,iStatus);
	CActiveSchedulerWait *schedulerwait;
	schedulerwait= new CActiveSchedulerWait;
	ao->GiveHandle(schedulerwait);
	lbt.CreateTrigger(*iTrigger, iTriggerid, EFalse, ao->iStatus);
	ao->MySetActive();	
	
	
	schedulerwait->Start();
	
	//User::WaitForRequest( iStatus );
    //User::LeaveIfError( iStatus.Int() );
	CleanupStack::Pop(4); 
	return KErrNone;
	#endif
    }
  
  
  
      // -----------------------------------------------------------------------------
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::CreateExittypeSessiontriggerL( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnExit );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
    
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;
    
    }
    
    
        // -----------------------------------------------------------------------------
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::CreateDisabledSessiontriggerL( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
    
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
  /*  User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;
    
    }
    
    
        // -----------------------------------------------------------------------------
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::CreatemultipleSessiontriggersL( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
    //Pop trigger condition object
    CleanupStack::Pop( cond );
    
    //Push trigger object to cleanup stack
    CleanupStack::PushL( trig );

    //Create the trigger in Location Triggering Server
    TLbtTriggerId trigId;
    TRequestStatus status;
   
    for(int i=0;i<5;i++)
    {//Remove braces for error test case LocTriggering:5(ELbtDuplicateRequest
      CMyObserver *ao;
	ao = CMyObserver::NewL();
	CActiveSchedulerWait *schedulerwait;
	schedulerwait= new CActiveSchedulerWait;
	ao->GiveHandle(schedulerwait);	
    lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
    
    //iLog->Log(_L("Session trigger created ")); 
    ao->MySetActive();
   	schedulerwait->Start();    
    }
    	

    
   /* for(int i=0;i<5;i++)
    lbt.CreateTrigger( *trig, trigId, ETrue, status );*/
    
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;
    
    }
    
    
       // -----------------------------------------------------------------------------
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::CreateSessiontriggerwithoutConnectL( CStifItemParser& aItem )
    {
        //Connect to Location Triggering Server
    RLbtServer lbtServ;
    
    RLbt lbt;
    
    TRealX nan;
    nan.SetNaN();
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
	TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 	
    lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
    
    //iLog->Log(_L("Session trigger created ")); 
    ao->MySetActive();
   	schedulerwait->Start();  
    
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
     
    return KErrNone;
    
    }
  
  
  
  TInt CCreateSessiontrigger::CreateSessiontriggerwithoutOpenL( CStifItemParser& aItem )
    {
        //Connect to Location Triggering Server
    RLbtServer lbtServ;
    User::LeaveIfError(lbtServ.Connect());
    CleanupClosePushL(lbtServ);

    RLbt lbt;
    
    CleanupClosePushL(lbt);
        
    TRealX nan;
    nan.SetNaN();
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
	TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 	
    lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
    
    //iLog->Log(_L("Session trigger created ")); 
    ao->MySetActive();
   	schedulerwait->Start();  
    
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
    
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;
    
    }
  
  
  
   // -----------------------------------------------------------------------------
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::SessiontriggerNameexceeds256L( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
 //  TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 	        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz" );
    
    
    
   // _LIT( KMyTriggerName, "tst");
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
	
//	TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6);
    //Construct a startup trigger
    TRAPD(error,CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        ));    
        if(error ==-6)
        {
        CleanupStack::Pop( cond );
        	lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
        	return KErrNone;
        }
        else
        {
        	CleanupStack::Pop( cond );
        	lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
        	return error;	
        }
       
        
    }
  
  
     // -----------------------------------------------------------------------------
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::SessiontriggerNameequals0L( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
        
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
	if(ao->iStatus == KErrArgument)
	{  
         errorflag=1;
	}
	else
	{
		 errorflag=0;
	}
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
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
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::CreateSessiontriggerwithoutNameL( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
   // _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KNullDesC,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
    
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;
    
    }
  
  
      // -----------------------------------------------------------------------------
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::CreateSessiontriggerwithoutcondL( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
   /* CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );*/
        CLbtTriggerConditionArea* cond=CLbtTriggerConditionArea::NewL(); ;
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
       
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
	if(ao->iStatus == KErrArgument)
	{  
         errorflag=1;
	}
	else
	{
		 errorflag=0;
	}
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
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
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::CreateSessiontriggermanagerUINotFoundL( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =200; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,200.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
	//aManagerUi=TUid::Null();
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
       
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
   	
   	TInt err = ao->iStatus.Int();
   	
   	 //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
        
    return err;
    }   
    
   // }
  
  
         // -----------------------------------------------------------------------------
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::CreateSessiontriggerwithNonUIApplicationL( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);//Give server's uid

    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
       
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
    
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;
    
    }
  
  
           // -----------------------------------------------------------------------------
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::CreateSessiontriggerwithoutRequestorL( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,100.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
   // _LIT( KMyRequestor , "Requestor" );
   
   //	TUid aManagerUi;
	TUid aManagerUi = TUid::Uid(0x87654321);//Give server's uid

    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
     trig->SetNameL(KMyTriggerName);   
     trig->SetCondition(cond);           
        
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
	if(ao->iStatus == KErrAccessDenied)
	{  
         errorflag=1;
	}
	else
	{
		 errorflag=0;
	}
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
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
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::ErrortriggerAreaToosmallL( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =1; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,0.1);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
	else
	{
		 errorflag=0;
	} 
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
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
// CCreateSessiontrigger::CreatesingleSessiontriggerL
// Testing the creation of single entry type session trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
       
    TInt CCreateSessiontrigger::ErrortriggerAreaRectangleL( CStifItemParser& aItem )
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
        
    TRealX myLatitude=61.5;
    TRealX myLongitude= 24.3;   
    TRealX myAltitude =0; 
    TRealX myradius =1; 
    TCoordinate centre(myLatitude,myLongitude)  ;  
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 	   
    //Define the triggering area
        
   CLbtGeoRect* trigArea = CLbtGeoRect::NewL(40,80,160,80);
   							
   /*	CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL();
   	trigArea->SetCenter(centre);
   	trigArea->SetRadius(10);*/
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond = 
        CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
    
    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor,                               //Requestor data
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
        
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
	TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 	
    lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
    
    //iLog->Log(_L("Session trigger created ")); 
    ao->MySetActive();
   	schedulerwait->Start();  
    
    TInt errorflag=0;
	if(ao->iStatus == KErrNotSupported)
	{  
         errorflag=1;
	}
	else
	{
		 errorflag=0;
	}
    iLog->Log(_L("Session trigger created ")); 
    
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
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
// CCreateSessiontrigger::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CCreateSessiontrigger::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  End of File
