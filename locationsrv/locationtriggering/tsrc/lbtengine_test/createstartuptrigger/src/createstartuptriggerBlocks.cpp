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
* Description:    Test module to implement Startup trigger creation related test cases
*
*/





// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "createstartuptrigger.h"
#include <lbt.h> 
#include <lbtserver.h>
#include <e32property.h>
//#include "..\..\..\..\lbt\lbtservercore\inc\epos_lbtserverconsts.h"
//#include <lbtstatuspskeys.h>
#include <lbtgeocircle.h>
#include <lbtgeorect.h> 
#include <lbttriggerconditionarea.h> 
#include <lbtstartuptrigger.h> 
#include <lbttriggerentry.h> 
#include "MyObserver.h"





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
// Ccreatestartuptrigger::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ccreatestartuptrigger::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ccreatestartuptrigger::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", Ccreatestartuptrigger::ExampleL ),
        ENTRY( "CreatesingleStartupTrigger", Ccreatestartuptrigger::CreatesingleStartupTriggerL ),
        ENTRY( "CreateexittypeStartupTrigger", Ccreatestartuptrigger::CreateexittypeStartupTriggerL ),
        ENTRY( "CreateDisabledStartupTrigger", Ccreatestartuptrigger::CreateDisabledStartupTriggerL ),
        ENTRY( "CreatemultipleStartupTriggers", Ccreatestartuptrigger::CreatemultipleStartupTriggersL ),
        ENTRY( "CreateStartupTriggerwithoutconnect", Ccreatestartuptrigger::CreateStartupTriggerwithoutconnectL ),
        ENTRY( "CreateStartupTriggerwithoutopen", Ccreatestartuptrigger::CreateStartupTriggerwithoutopenL ),
        ENTRY( "StartupTriggerNameexceeds256", Ccreatestartuptrigger::StartupTriggerNameexceeds256_testL ),
        ENTRY( "StartupTriggerNameequals0", Ccreatestartuptrigger::StartupTriggerNameequals0_testL ),
        ENTRY( "CreateStartupTriggerwithoutName", Ccreatestartuptrigger::CreateStartupTriggerwithoutNameL ),
        ENTRY( "CreateStartupTriggerwithoutcond", Ccreatestartuptrigger::CreateStartupTriggerwithoutcondL ),
        ENTRY( "CreateStartupTriggerManagerUINotFound", Ccreatestartuptrigger::CreateStartupTriggerManagerUINotFoundL ),
        ENTRY( "CreateStartupTriggerwithNonUIApp", Ccreatestartuptrigger::CreateStartupTriggerwithNonUIAppL ),
        ENTRY( "ErrortriggerAreaToosmall", Ccreatestartuptrigger::ErrortriggerAreaToosmallL ),
        ENTRY( "CreateStartupTriggerwithoutTriggerhandlingprocess", Ccreatestartuptrigger::CreateStartupTriggerwithoutTriggerhandlingprocessL ),
        ENTRY( "CreateStartupTriggerwithoutRequestor", Ccreatestartuptrigger::CreateStartupTriggerwithoutRequestorL ),
        ENTRY( "ErrorRectangletriggerArea", Ccreatestartuptrigger::ErrorRectangletriggerAreaL ),

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Ccreatestartuptrigger::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Kcreatestartuptrigger, "createstartuptrigger" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Kcreatestartuptrigger, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Kcreatestartuptrigger, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }
    
    
// -----------------------------------------------------------------------------
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreatesingleStartupTriggerL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;     
	
    }


    // -----------------------------------------------------------------------------
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
//#if 0
TInt Ccreatestartuptrigger::CreateexittypeStartupTriggerL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        CLbtTriggerConditionArea::EFireOnExit );
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
   /* TUidType handlingUidType( 
        TUid::Uid( 0x00000001 ),
        TUid::Uid( 0x00000002 ),
        TUid::Uid( 0x00000003 ) );*/
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger name" );
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
    
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;     
	
    }


   // -----------------------------------------------------------------------------
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreateDisabledStartupTriggerL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateDisabled,       //Trigger state
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
    
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;     
	
    }
    
    
    
// -----------------------------------------------------------------------------
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreatemultipleStartupTriggersL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger name" );
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
    
    for(int i=0;i<5;i++)
    {
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
    
    
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;     
	
    }



// -----------------------------------------------------------------------------
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreateStartupTriggerwithoutconnectL( CStifItemParser& aItem )
    {

        //Connect to Location Triggering Server
    RLbtServer lbtServ;
    RLbt lbt;
      
    
    TRealX nan;
    nan.SetNaN();
    
    //Define the triggering area
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger name" );
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
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
    lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
    
    //iLog->Log(_L("Session trigger created ")); 
    
    ao->MySetActive();	
	schedulerwait->Start(); 
    
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;     
	
    }
    
    
      
    // -----------------------------------------------------------------------------
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreateStartupTriggerwithoutopenL( CStifItemParser& aItem )
    {

        //Connect to Location Triggering Server
    RLbtServer lbtServ;
    User::LeaveIfError(lbtServ.Connect());
    CleanupClosePushL(lbtServ);

    RLbt lbt;
    CleanupClosePushL(lbt);
    
    
    TRealX nan;
    nan.SetNaN();
    
    //Define the triggering area
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger name" );
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
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
    lbt.CreateTrigger( *trig, trigId, ETrue, ao->iStatus );
    
    //iLog->Log(_L("Session trigger created ")); 
    
    ao->MySetActive();	
	schedulerwait->Start(); 
    
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;     
	
    }


    // -----------------------------------------------------------------------------
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::StartupTriggerNameexceeds256_testL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

/*    _LIT( KMyTriggerName, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz
    abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz
    abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvw" );*/
    _LIT( KMyTriggerName, "a");
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
    
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;     
	
    }


   // -----------------------------------------------------------------------------
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::StartupTriggerNameequals0_testL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "" );
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
	if(ao->iStatus == KErrArgument)
	{  
         errorflag=1;
	}
	else
	{
		 errorflag=0;
	}
	
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
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
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreateStartupTriggerwithoutNameL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

   // _LIT( KMyTriggerName, "" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL( 
        KNullDesC,                             //Trigger name
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
    
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;     
	
    }


 // -----------------------------------------------------------------------------
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreateStartupTriggerwithoutcondL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
    CLbtGeoCircle* trigArea = 
        CLbtGeoCircle::NewL( 
            TCoordinate( 61.5,24.3,0 ), //center coordinate
            100                       //radius
    );
    CleanupStack::PushL( trigArea );

    //Construct trigger condition
    CLbtTriggerConditionArea* cond =CLbtTriggerConditionArea::NewL(); 
       /* CLbtTriggerConditionArea::NewL( 
        trigArea,
        CLbtTriggerConditionArea::EFireOnEnter );*/
        
    CleanupStack::Pop( trigArea ); //ownership is transfered.
    CleanupStack::PushL( cond );
   /* TUidType handlingUidType( 
        TUid::Uid( 0x00000001 ),
        TUid::Uid( 0x00000002 ),
        TUid::Uid( 0x00000003 ) );*/
        
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
	if(ao->iStatus == KErrArgument)
	{  
         errorflag=1;
	}
	else
	{
		 errorflag=0;
	}
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
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
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreateStartupTriggerManagerUINotFoundL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger Name" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    TUid aManagerUi;
	//aManagerUi = TUid::Uid(0x87654321);
	aManagerUi=TUid::Null();
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
	if(ao->iStatus == KErrNone)
	{  
         errorflag=1;
	}
	else
	{
		 errorflag=0;
	}
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
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
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreateStartupTriggerwithNonUIAppL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger Name" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);//Give server's uid
	
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
    
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
    lbt.Close();
    CleanupStack::Pop(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;     
	
    }
    
    
    // -----------------------------------------------------------------------------
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::ErrortriggerAreaToosmallL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
   /* TUidType handlingUidType( 
        TUid::Uid( 0x00000001 ),
        TUid::Uid( 0x00000002 ),
        TUid::Uid( 0x00000003 ) );*/
        
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
	else
	{
		 errorflag=0;
	} 
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
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
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreateStartupTriggerwithoutTriggerhandlingprocessL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger Name" );
    _LIT( KMyRequestor , "Requestor" );
   // _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
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
        KNullDesC(),              //Handling process name
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
	if(ao->iStatus == KErrArgument)
	{  
         errorflag=1;
	}
	else
	{
		 errorflag=0;
	}
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
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
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::CreateStartupTriggerwithoutRequestorL( CStifItemParser& aItem )
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
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
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
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger Name" );
   // _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
  //  TUid aManagerUi;
	TUid aManagerUi = TUid::Uid(0x87654321);
	
    //Construct a startup trigger
                           //Command line
        
     CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
     trig->SetNameL(KMyTriggerName);   
     trig->SetCondition(cond);
     trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
        
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
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
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
// Ccreatestartuptrigger::CreatesingleStartupTrigger
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ccreatestartuptrigger::ErrorRectangletriggerAreaL( CStifItemParser& aItem )
    {

        //Connect to Location Triggering Server
    RLbtServer lbtServ;
    User::LeaveIfError(lbtServ.Connect());
    CleanupClosePushL(lbtServ);

    RLbt lbt;
    User::LeaveIfError(lbt.Open(lbtServ));
    CleanupClosePushL(lbt);
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
    TRealX nan;
    nan.SetNaN();
    
    //Define the triggering area
    //The setting of trigger area radius is omitted. In this case, 
    //Location Triggering Server will use minimum size of trigger 
    //area as the radius. 
    CLbtGeoRect* trigArea = 
        CLbtGeoRect::NewL(40,80,160,80);
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
        
        TSecureId secureid;

    _LIT( KMyTriggerName, "Trigger Name" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
	TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6);
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        CRequestorBase::ERequestorService,          //Requestor type
        CRequestorBase::EFormatApplication,         //Requestor format
        KMyRequestor ,                               //Requestor data
       // CLbtTriggerEntry::EPostFiringActionNoAction, //Trigger fired option
        aManagerUi,				                      //Manager UI
        cond,                                       //Trigger condition
        KMyTriggerHandlingProcessName,              //Handling process name
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
    //Delete the instance of trigger object
    CleanupStack::PopAndDestroy( trig );
    
    /*User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
    //Start - up trigger will be stored by Location Triggering Server
    //persistently. When the trigger is fired, the specified
    //process will be launched.
    
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




//#endif

// -----------------------------------------------------------------------------
// Ccreatestartuptrigger::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt Ccreatestartuptrigger::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  End of File
