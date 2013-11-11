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
* Description:    Test module to implement Firing of startup trigger and Listing of trigger related test cases
*
*/






// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <e32cmn.h>
#include <e32des16.h>
#include <e32base.h>
#include <s32mem.h>
#include <e32math.h> 
#include <centralrepository.h>
#include <lbs/SimulationPSYInternalCRKeys.h>
//#include <time.h>




// LT API

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
#include <LbsPositionInfo.h>
#include <lbs.h>
//#include <epossimulationpsy.hrh>
#include "firingofstartuptriggerandlisttrigger.h"
#include "t_triggerfireobserver.h"


#include <EPos_MPosModulesObserver.h> 
#include <EPos_CPosModuleIdList.h>
#include <EPos_CPosModules.h>
#include <EPos_CPosModuleUpdate.h>
#include <MProEngEngine.h>
#include <Profile.hrh>
#include <ProEngFactory.h>

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
// CFiringofStartupTriggerAndListTrigger::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CFiringofStartupTriggerAndListTrigger::Delete() 
    {

    }

void CFiringofStartupTriggerAndListTrigger::EnableSimPSYL() 
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

void CFiringofStartupTriggerAndListTrigger::SetProfileToOfflineL()
    {
    if( !iProEngine )
        iProEngine= ProEngFactory::NewEngineL();
    // Store current profile id.
    iCurrentProfile =  iProEngine->ActiveProfileId();
    // Change the active profile to Off-line
    iProEngine->SetActiveProfileL( EProfileOffLineId );
    }

void CFiringofStartupTriggerAndListTrigger::RestoreProfileL()
    {
    if( !iProEngine )
    iProEngine = ProEngFactory::NewEngineL();
    iProEngine->SetActiveProfileL( iCurrentProfile );
    }

//------------------------------------------------------------------------------
//CFiringofStartupTriggerAndListTrigger::GetCurrentCoordinateL
//------------------------------------------------------------------------------
void CFiringofStartupTriggerAndListTrigger::GetCurrentCoordinateL( TCoordinate& aCoordinate )
    {
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL();
    CleanupStack::PushL( notifier );
    CActiveSchedulerWait* wait = new ( ELeave ) CActiveSchedulerWait;
    CleanupStack::PushL( wait );
    
    TPositionInfo positionInfo;
    // Ownership of wait is taken by notifier
    notifier->CurrentPositionL( positionInfo,wait );
    CleanupStack::Pop( wait );
    wait->Start();
    TPosition position;
    positionInfo.GetPosition( position );
    aCoordinate.SetCoordinate( position.Latitude(),position.Longitude(),position.Altitude() );
    CleanupStack::PopAndDestroy( notifier ); 
    }



// -----------------------------------------------------------------------------
// CFiringofStartupTriggerAndListTrigger::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CFiringofStartupTriggerAndListTrigger::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
     
        ENTRY( "test01", CFiringofStartupTriggerAndListTrigger::TCLBTFW001_testL ),
        ENTRY( "test02", CFiringofStartupTriggerAndListTrigger::TCLBTFW002_testL ),
        ENTRY( "test03", CFiringofStartupTriggerAndListTrigger::TCLBTFW003_testL ),
        ENTRY( "test04", CFiringofStartupTriggerAndListTrigger::TCLBTFW004_testL ),
        ENTRY( "test05", CFiringofStartupTriggerAndListTrigger::TCLBTFW005_testL ),
        ENTRY( "test06", CFiringofStartupTriggerAndListTrigger::TCLBTFW006_testL ),
        ENTRY( "test07", CFiringofStartupTriggerAndListTrigger::TCLBTFW007_testL ),
        ENTRY( "test08", CFiringofStartupTriggerAndListTrigger::TCLBTFW008_testL ),
        ENTRY( "test09", CFiringofStartupTriggerAndListTrigger::TCLBTFW009_testL ),
        ENTRY( "test10", CFiringofStartupTriggerAndListTrigger::TCLBTFW010_testL ),
        ENTRY( "test11", CFiringofStartupTriggerAndListTrigger::TCLBTFW011_testL ),
        ENTRY( "test12", CFiringofStartupTriggerAndListTrigger::TCLBTFW012_testL ),
        ENTRY( "test13", CFiringofStartupTriggerAndListTrigger::TCLBTFW013_testL ),
        ENTRY( "test14", CFiringofStartupTriggerAndListTrigger::TCLBTFW014_testL ),
        ENTRY( "test15", CFiringofStartupTriggerAndListTrigger::TCLBTFW015_testL ),
        
        ENTRY( "test70", CFiringofStartupTriggerAndListTrigger::TCLBTFW070_testL ),
        ENTRY( "test71", CFiringofStartupTriggerAndListTrigger::TCLBTFW071_testL ),
        ENTRY( "test72", CFiringofStartupTriggerAndListTrigger::TCLBTFW072_testL ),
        ENTRY( "test73", CFiringofStartupTriggerAndListTrigger::TCLBTFW073_testL ),
        ENTRY( "test74", CFiringofStartupTriggerAndListTrigger::TCLBTFW074_testL ),
        ENTRY( "test75", CFiringofStartupTriggerAndListTrigger::TCLBTFW075_testL ),
        ENTRY( "test76", CFiringofStartupTriggerAndListTrigger::TCLBTFW076_testL ),
        ENTRY( "test77", CFiringofStartupTriggerAndListTrigger::TCLBTFW077_testL ),
        ENTRY( "test78", CFiringofStartupTriggerAndListTrigger::TCLBTFW078_testL ),
        ENTRY( "test79", CFiringofStartupTriggerAndListTrigger::TCLBTFW079_testL ),
        ENTRY( "test80", CFiringofStartupTriggerAndListTrigger::TCLBTFW080_testL ),
        ENTRY( "test81", CFiringofStartupTriggerAndListTrigger::TCLBTFW081_testL ),
        ENTRY( "test82", CFiringofStartupTriggerAndListTrigger::TCLBTFW082_testL ),
        ENTRY( "test83", CFiringofStartupTriggerAndListTrigger::TCLBTFW083_testL ),
        ENTRY( "test84", CFiringofStartupTriggerAndListTrigger::TCLBTFW084_testL ),
        ENTRY( "test85", CFiringofStartupTriggerAndListTrigger::TCLBTFW085_testL ),
        ENTRY( "test86", CFiringofStartupTriggerAndListTrigger::TCLBTFW086_testL ),
        ENTRY( "test87", CFiringofStartupTriggerAndListTrigger::TCLBTFW087_testL ),
        ENTRY( "test88", CFiringofStartupTriggerAndListTrigger::TCLBTFW088_testL ),
        ENTRY( "test89", CFiringofStartupTriggerAndListTrigger::TCLBTFW089_testL ),
        ENTRY( "test90", CFiringofStartupTriggerAndListTrigger::TCLBTFW090_testL ),
        ENTRY( "test91", CFiringofStartupTriggerAndListTrigger::TCLBTFW091_testL ),
        ENTRY( "test92", CFiringofStartupTriggerAndListTrigger::TCLBTFW092_testL ),
        ENTRY( "test93", CFiringofStartupTriggerAndListTrigger::TCLBTFW093_testL ),
        ENTRY( "test94", CFiringofStartupTriggerAndListTrigger::TCLBTFW094_testL ),
        ENTRY( "test95", CFiringofStartupTriggerAndListTrigger::TCLBTFW095_testL ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CFiringofStartupTriggerAndListTrigger::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

//Firing of single Entry type startup trigger

TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW001_testL( CStifItemParser& /* aItem */ )
    {
   	
	iLog->Log(_L("Entering Test1"));
	_LIT( KSimulationFile,"c:\\system\\data\\simu_move1.sps" );
	
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
	 // Enable simulation psy
     EnableSimPSYL();
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
 	  	 
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    trig->SetNameL(_L("abc"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   //  _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    // set condition

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
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
    iLog->Log(_L("Trigger Created"));
    notifier->StartNotification( wait );
  	wait->Start( );
    iLog->Log(_L("Trigger Fired"));
    TLbtTriggerFireInfo FireInfo;
    TReal32 trigDistance;
    TPosition firePosition;
    FireInfo = notifier->GetFiredTrigger();
    FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    firePosition.Distance(coordinate,trigDistance);
      //close sim psy
    
    RestoreProfileL();
    if( trigDistance<=1000 && FireInfo.iTriggerId==trigId )
    {
    	lbt.DeleteTriggerL(trigId);
	   	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    delete wait;
	    iLog->Log(_L("Test case passed "));
	   
	    return KErrNone;
    }
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    delete wait;
	    
	    return -99; 
    }
   //  return KErrNone; 
    }
   
   //Firing of single Exit type Startup Trigger
   
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW002_testL( CStifItemParser& /* aItem */ )
    {

   
    _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	  RPositionServer iLocationServer;
     RPositioner iPositioner;
     
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     // Enable simulation psy
     EnableSimPSYL();
     
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 //Delete all the existing trggers
 	// lbt.DeleteTriggersL();
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
  //  _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,200);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object
	
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    notifier->StartNotification( wait );
    wait->Start( );
    iLog->Log(_L("Trigger fired"));
     TLbtTriggerFireInfo FireInfo;
    TReal32 trigDistance;
    TPosition firePosition;
    FireInfo = notifier->GetFiredTrigger();
    iLog->Log(_L("Trigger fired"));
    FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    firePosition.Distance(coordinate,trigDistance);
    
     //close sim psy
    
    iPositioner.Close();
    iLocationServer.Close();
    RestoreProfileL();
    if( FireInfo.iTriggerId==trigId )
    {
    	lbt.DeleteTriggerL(trigId);
	   	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    delete wait;
	    iLog->Log(_L("Test case passed "));
	    return KErrNone;
    }
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    iLog->Log(_L("Test case passed "));
    return -99; 
    }
	
    }
    
    // Firing of startup trigger when owner application of the trigger is not running
    
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW003_testL( CStifItemParser& /* aItem */ )
    {

   _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	  RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     // Enable simulation psy
     EnableSimPSYL();
     
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
     _LIT( KMyTriggerHandlingProcessName, "About.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    // set condition
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,200);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object
	
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    RestoreProfileL();
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    return KErrNone; 
      
    }
    
    
    
    //Firing of  startup trigger when trigger handling process is same as the owner of the trigger
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW004_testL( CStifItemParser& /* aItem */ )
    {

    
    _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     // Enable simulation psy
     EnableSimPSYL();
     
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
   // _LIT( KMyTriggerHandlingProcessName, "TestServerStarter.exe" );
   _LIT( KMyTriggerHandlingProcessName, "ConsoleUi.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object
	
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    notifier->StartNotification( wait );
    wait->Start( );
    
    TLbtTriggerFireInfo FireInfo;
    TReal32 trigDistance;
    TPosition firePosition;
    FireInfo = notifier->GetFiredTrigger();
    FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    firePosition.Distance(coordinate,trigDistance);
    
    //close sim psy
    
    iPositioner.Close();
    iLocationServer.Close();
    RestoreProfileL();
    if( FireInfo.iTriggerId==trigId )
    {
    	lbt.DeleteTriggerL(trigId);
	   	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    delete wait;
	    iLog->Log(_L("Test case passed "));
	    return KErrNone;
    }
    else
    {
    
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return -99; 
    }

    }
    
    
    //Testing of trigger firing when trigger is in disabled state
    
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW005_testL( CStifItemParser& /* aItem */ )
    {

       _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 	 RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 
    // Set profile to offline mode.This is required to avoid movement detection blocking the 
    // trigger firing.
    SetProfileToOfflineL();
    // Enable simulation psy
    EnableSimPSYL();
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
     CLbtTriggerEntry::TLbtTriggerState aState ;
    aState = CLbtTriggerEntry::EStateDisabled;
    trig->SetState(aState);
    
    // _LIT( KMyTriggerHandlingProcessName, "About.exe");
    _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    notifier->StartNotification( wait );
  //  wait->Start( );
    notifier->iWaitStatus = KRequestPending;
    
   // notifier->StartNotification( wait );
	notifier->After(1500000);
	wait->Start( );
  //  User::After(60000000);
  iPositioner.Close();
    iLocationServer.Close();
    RestoreProfileL();
     if(notifier->iTriggerFireCount> 0)
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  //  notifier->iTimer.Cancel();
    delete wait;
    return -99; 
    }
    else 
   {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // notifier->iTimer.Cancel();
    delete wait;
    
    return KErrNone; 
    }
    }
    
    
    //Firing of  startup trigger when trigger handling process is already running
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW006_testL( CStifItemParser& /* aItem */ )
    {

    _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 	 RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 
    // Set profile to offline mode.This is required to avoid movement detection blocking the 
    // trigger firing.
    SetProfileToOfflineL();
    // Enable simulation psy
    EnableSimPSYL();
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   // _LIT( KMyTriggerHandlingProcessName, "About.exe");
    _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
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
    iLog->Log(_L("Trigger Created"));
    notifier->StartNotification( wait );
    wait->Start( );
    iLog->Log(_L("Trigger Fired"));
    TLbtTriggerFireInfo FireInfo;
    TReal32 trigDistance;
    TPosition firePosition;
    FireInfo = notifier->GetFiredTrigger();
    FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    firePosition.Distance(coordinate,trigDistance);
    iPositioner.Close();
    iLocationServer.Close();
    RestoreProfileL();
    if(trigDistance<=1000 && FireInfo.iTriggerId==trigId )
    {
    
	   	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    delete wait;
	    iLog->Log(_L("Test case passed "));
	    
	    return KErrNone;
    }
    else
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return -99; 
    }
      

    }
    
   
   //Firing of entry type trigger when FireOnCreation flag set as true 
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW007_testL( CStifItemParser& /* aItem */ )
    {

     _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 	 RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     // Enable simulation psy
     EnableSimPSYL();
 	 
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
     _LIT( KMyTriggerHandlingProcessName, "About.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    notifier->StartNotification( wait );
    wait->Start( );
    
     TLbtTriggerFireInfo FireInfo;
    TReal32 trigDistance;
    TPosition firePosition;
    FireInfo = notifier->GetFiredTrigger();
    FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    firePosition.Distance(coordinate,trigDistance);
    iPositioner.Close();
    iLocationServer.Close();
    RestoreProfileL();
    if(trigDistance==0 && FireInfo.iTriggerId==trigId )
    {
    
	   	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    delete wait;
	    iLog->Log(_L("Test case passed "));
	    return KErrNone;
    }
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return -99; 
    }
      
    }

//Firing of exit type trigger when FireOnCreation flag set as true

TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW008_testL( CStifItemParser& /* aItem */ )
    {

    
    _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 	 RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     // Enable simulation psy
     EnableSimPSYL();
     
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
     //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);    
    // set condition
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    coordinate.Move(90,2000);
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    notifier->StartNotification( wait );
    wait->Start( );
    
     TLbtTriggerFireInfo FireInfo;
    TReal32 trigDistance;
    TPosition firePosition;
    FireInfo = notifier->GetFiredTrigger();
    FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    firePosition.Distance(coordinate,trigDistance);
    iPositioner.Close();
    iLocationServer.Close();
    RestoreProfileL();
    if( trigDistance>=1000 && FireInfo.iTriggerId==trigId )
    {
    
	   	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    delete wait;
	    iLog->Log(_L("Test case passed "));
	    return KErrNone;
    }
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return -99; 
    }
    }
 
 
 //Firing of entry type trigger when FireOnCreation flag set as false   
TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW009_testL( CStifItemParser& /* aItem */ )
    {
 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 	 RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 	 
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     // Enable simulation psy
     EnableSimPSYL();
 	 
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
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
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
   // notifier->StartNotification( wait );
    //  wait->Start( );
  //  notifier->iWaitStatus = KRequestPending;
    
    notifier->StartNotification( wait );
	notifier->After(5000000);
	wait->Start( );
    iPositioner.Close();
    iLocationServer.Close();
    RestoreProfileL();
       if(notifier->iTriggerFireCount== 0)
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  //  notifier->iTimer.Cancel();
    delete wait;
    return KErrNone; 
    }
    else 
   {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // notifier->iTimer.Cancel();
    delete wait;
    
    return -99; 
    }
      
    }
    

//Firing of exit type trigger when FireOnCreation flag set as false

TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW010_testL( CStifItemParser& /* aItem */ )
    {

     _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 	 RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 	 
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     // Enable simulation psy
     EnableSimPSYL();
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
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
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    coordinate.Move(90,1100);
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object
	
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
   
    
    notifier->StartNotification( wait );
	notifier->After(5000000);
	wait->Start( );
    iPositioner.Close();
    iLocationServer.Close();
    RestoreProfileL();
       if(notifier->iTriggerFireCount== 0)
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  //  notifier->iTimer.Cancel();
    delete wait;
    return KErrNone; 
    }
    else 
   {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // notifier->iTimer.Cancel();
    delete wait;
    
    return -99; 
    }
    }
    
    //Starting the trigger fire notifier without opening the subsession
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW011_testL( CStifItemParser& /* aItem */ )
    {

   //  _LIT( KSimulationFile,"c:\\system\\data\\simu_move1.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2);  
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    	
    notifier->StartNotification( wait );
    //wait->Start( );
    
    CleanupStack::PopAndDestroy( notifier );
   // CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
      
    }
    
    

//Starting the trigger fire notifier when it is already started
TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW012_testL( CStifItemParser& /* aItem */ )
    {
 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 	 RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
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
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    notifier->StartNotification( wait );
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 5);  	
    notifier->StartNotification( wait );
    wait->Start( );
    iPositioner.Close();
    iLocationServer.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
      
    }
    
 //   Firing of trigger when enabling a disabled trigger inside trigger area
    
TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW013_testL( CStifItemParser& /* aItem */ )
    {
 _LIT( KSimulationFile,"c:\\system\\data\\simu_move3.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 	 RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     // Enable simulation psy
     EnableSimPSYL();
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    // _LIT( KMyTriggerHandlingProcessName, "About.exe");
    _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,10000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    CLbtTriggerEntry::TLbtTriggerState aState ;
    aState = CLbtTriggerEntry::EStateDisabled;
   // aState = CLbtTriggerEntry::EStateEnabled;
    trig->SetState(aState);
    
    trig->SetCondition(condition); // ownership transferred to object
	
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    
    wait->Start( );
   
    
 //  
    
   notifier->iWaitStatus = KRequestPending;
    
   // User::LeaveIfError(notifier->iTimer.CreateLocal());
   
  // notifier->StartNotification( wait );
	notifier->After(450000);
	wait->Start( );
  //  User::After(60000000);
     
  //  {
     aState = CLbtTriggerEntry::EStateEnabled;
    lbt.SetTriggerStateL(trigId,CLbtTriggerEntry::EStateEnabled,ELbtTrue);
     notifier->StartNotification( wait );
 		wait->Start( );
 		iPositioner.Close();
    iLocationServer.Close();
    RestoreProfileL();
 if(notifier->iTriggerFireCount > 0)
 {
  lbt.DeleteTriggerL(trigId);
  CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //notifier->iTimer.Cancel();
    delete wait;
    return KErrNone; 
    }
    else
   {
    lbt.DeleteTriggerL(trigId);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt ); 
    CleanupStack::PopAndDestroy( &lbtserver );
   // notifier->iTimer.Cancel();
    delete wait;
    
    return KErrGeneral; 
    }
      
    }
    
    
   
   //Firing of  startup trigger when trigger handling process not found in the system
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW014_testL( CStifItemParser& /* aItem */ )
    {

 _LIT( KSimulationFile,"c:\\system\\data\\simu_move1.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 	 RPositionServer iLocationServer;
     RPositioner iPositioner;
 	 
 	 // Connect to the location server
    User::LeaveIfError(iLocationServer.Connect());

    // Open the positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     // Enable simulation psy
     EnableSimPSYL();
 	 
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   // _LIT( KMyTriggerHandlingProcessName, "About.exe");
    _LIT( KMyTriggerHandlingProcessName, "XYZ.exe");
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
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
    iLog->Log(_L("Trigger Created"));
    notifier->StartNotification( wait );
    wait->Start( );
    iLog->Log(_L("Trigger Fired"));
    TLbtTriggerFireInfo FireInfo;
    TReal32 trigDistance;
    TPosition firePosition;
    FireInfo = notifier->GetFiredTrigger();
    FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    firePosition.Distance(coordinate,trigDistance);
    iPositioner.Close();
    iLocationServer.Close();
    RestoreProfileL();
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
    
    }
    
    
    //Firing of multiple Entry type startup triggers

TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW015_testL( CStifItemParser& /* aItem */ )
    {

 _LIT( KSimulationFile,"c:\\system\\data\\simu_move1.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
     iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     // Enable simulation psy
     EnableSimPSYL();
 	 
 	 //Delete all the existing trggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL());
 	 
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
     CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
   // _LIT( KMyTriggerHandlingProcessName, "About.exe");
    _LIT( KMyTriggerHandlingProcessName1, "ABOUT.exe");
    _LIT( KMyTriggerHandlingProcessName2, "About.exe");
    
    TSecureId secureid1,secureid2;
    trig1->SetProcessId(KMyTriggerHandlingProcessName1,secureid1);
    trig2->SetProcessId(KMyTriggerHandlingProcessName2,secureid2);
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);  
	
	trig2->SetRequestorL(ReqType,ReqFormat,ReqData);     
    // set condition

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig1->SetCondition(condition); // ownership transferred to object
	trig2->SetCondition(condition);
    TLbtTriggerId trigId;
    
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig1,trigId,ETrue,wait );
    wait->Start( );
    notifier->CreateTriggers( lbt,*trig2,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    notifier->StartNotification( wait );
  	wait->Start( );
    iLog->Log(_L("Triggers Fired"));
    wait->Start( );
    iLog->Log(_L("Triggers Fired"));
    TLbtTriggerFireInfo FireInfo;
    TReal32 trigDistance;
    TPosition firePosition;
    FireInfo = notifier->GetFiredTrigger();
    FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    firePosition.Distance(coordinate,trigDistance);
    RestoreProfileL();
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig2);
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
   
    
    }
   

// Testing the hysteresis condition for entry type of trigger  
  
TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW016_testL( CStifItemParser& /* aItem */ )
  {

_LIT( KSimulationFile,"c:\\system\\data\\test1.nme" );

   RLbtServer lbtserver;
   RLbt lbt;
       RPositionServer iLocationServer;
   RPositioner iPositioner;
   
   // Connect to the location server
  User::LeaveIfError(iLocationServer.Connect());

  // Open the positioner
  User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
   
   
   User::LeaveIfError( lbtserver.Connect() );
   CleanupClosePushL( lbtserver );
   iLog->Log(_L("Connection to RLbtServer Passed "));
   User::LeaveIfError( lbt.Open( lbtserver ) );
   iLog->Log(_L("Subsession opened "));
   CleanupClosePushL( lbt );
   
   //Delete all the existing trggers
   TRAP_IGNORE(lbt.DeleteTriggersL());
  
   CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
   iLog->Log(_L("Simulation PSY Repository object created"));
   User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
   iLog->Log(_L("Simulation input file set "));
   CleanupStack::PopAndDestroy(repository);
   
    //Construct a session trigger
  CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
  
  //Push to cleanup stack
  CleanupStack::PushL( trig );
  iLog->Log(_L("Startup Trigger Entry Created "));
  
  // Set Name
  trig->SetNameL(_L("Trigger1"));
 // _LIT( KMyTriggerHandlingProcessName, "About.exe");
  _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
  
  TSecureId secureid;
  trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
  //Set Requestor
  CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
  CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
  TBuf<KLbtMaxNameLength> ReqData=_L("");
  trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
  // set condition
  
  TCoordinate coordinate(65.5285,23.9385);
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
  iLog->Log(_L("Trigger Created"));
  notifier->StartNotification( wait );
  wait->Start( );
 // time_t time1,time2;
 // Time();
 // notifier->StartNotification( wait );
 // wait->Start( );
  
//  notifier->StartNotification( wait );
  wait->Start( );
  notifier->iWaitStatus = KRequestPending;
  
//  notifier->StartNotification( wait );
  notifier->After(1000000);
  wait->Start( );
  
  iLog->Log(_L("Trigger Fired"));
  TLbtTriggerFireInfo FireInfo;
  TReal32 trigDistance;
  TPosition firePosition;
  FireInfo = notifier->GetFiredTrigger();
  FireInfo.iFiredPositionInfo.GetPosition(firePosition);
  firePosition.Distance(coordinate,trigDistance);
  iPositioner.Close();
  iLocationServer.Close();
  lbt.DeleteTriggerL(trigId);
  if(notifier->iTriggerFireCount ==2)
  {
      CleanupStack::PopAndDestroy( notifier );
  CleanupStack::PopAndDestroy( trig );
  CleanupStack::Pop( &lbt );
  CleanupStack::PopAndDestroy( &lbtserver );
  delete wait;
  
  return KErrNone; 
  }
  else
  {
      CleanupStack::PopAndDestroy( notifier );
  CleanupStack::PopAndDestroy( trig );
  CleanupStack::Pop( &lbt );
  CleanupStack::PopAndDestroy( &lbtserver );
  delete wait;
  
  return -99; 
  }
  }
  
  //Testing the hysteresis condition for exit type of trigger
  
  TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW017_testL( CStifItemParser& /* aItem */ )
  {

_LIT( KSimulationFile,"c:\\system\\data\\test2.nme" );
  
   RLbtServer lbtserver;
   RLbt lbt;
       RPositionServer iLocationServer;
   RPositioner iPositioner;
   
   // Connect to the location server
  User::LeaveIfError(iLocationServer.Connect());

  // Open the positioner
  User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
   
   
   User::LeaveIfError( lbtserver.Connect() );
   CleanupClosePushL( lbtserver );
   iLog->Log(_L("Connection to RLbtServer Passed "));
   User::LeaveIfError( lbt.Open( lbtserver ) );
   iLog->Log(_L("Subsession opened "));
   CleanupClosePushL( lbt );
   
   //Delete all the existing trggers
   TRAP_IGNORE(lbt.DeleteTriggersL());
  
   CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
   iLog->Log(_L("Simulation PSY Repository object created"));
   User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
   iLog->Log(_L("Simulation input file set "));
   CleanupStack::PopAndDestroy(repository);
   
    //Construct a session trigger
  CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
  
  //Push to cleanup stack
  CleanupStack::PushL( trig );
  iLog->Log(_L("Startup Trigger Entry Created "));
  
  // Set Name
  trig->SetNameL(_L("Trigger1"));
 // _LIT( KMyTriggerHandlingProcessName, "About.exe");
  _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
  
  TSecureId secureid;
  trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
  //Set Requestor
  CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
  CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
  TBuf<KLbtMaxNameLength> ReqData=_L("");
  trig->SetRequestorL(ReqType,ReqFormat,ReqData);     
  // set condition
  
  TCoordinate coordinate(65.5285,23.9385);
 // TCoordinate coordinate(62.4438,23.9385);
  
  CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
  CleanupStack::PushL( circle );
  
       
  // ownership of circle object transferred to the condition object
  CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                              circle,
                                              CLbtTriggerConditionArea::EFireOnExit);
      
  CleanupStack::Pop( circle );
  
  trig->SetCondition(condition); // ownership transferred to object

  TLbtTriggerId trigId;
      
      
  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
  CleanupStack::PushL( notifier );
  
  CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
      
  notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
  wait->Start( );
  iLog->Log(_L("Trigger Created"));
  notifier->StartNotification( wait );
  wait->Start( );
//  notifier->StartNotification( wait );
  wait->Start( );
//  notifier->StartNotification( wait );
  //  wait->Start( );
  notifier->iWaitStatus = KRequestPending;
  
 // notifier->StartNotification( wait );
  notifier->After(15000000);
  wait->Start( );
  iLog->Log(_L("Trigger Fired"));
  TLbtTriggerFireInfo FireInfo;
  TReal32 trigDistance;
  TPosition firePosition;
  FireInfo = notifier->GetFiredTrigger();
  FireInfo.iFiredPositionInfo.GetPosition(firePosition);
  firePosition.Distance(coordinate,trigDistance);
  iPositioner.Close();
  iLocationServer.Close();
  lbt.DeleteTriggerL(trigId);
  if( notifier->iTriggerFireCount ==2)
  {
      CleanupStack::PopAndDestroy( notifier );
  CleanupStack::PopAndDestroy( trig );
  CleanupStack::Pop( &lbt );
  CleanupStack::PopAndDestroy( &lbtserver );
  delete wait;
  
  return KErrNone; 
  }
  else
  {
      CleanupStack::PopAndDestroy( notifier );
  CleanupStack::PopAndDestroy( trig );
  CleanupStack::Pop( &lbt );
  CleanupStack::PopAndDestroy( &lbtserver );
  delete wait;
  
  return -99; 
  }
  }
  
  
  //Registering for trigger fire notification after trigger fires multiple times
  TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW018_testL( CStifItemParser& /* aItem */ )
  {

_LIT( KSimulationFile,"c:\\system\\data\\test3.nme" );

   RLbtServer lbtserver;
   RLbt lbt;
       RPositionServer iLocationServer;
   RPositioner iPositioner;
   
   // Connect to the location server
  User::LeaveIfError(iLocationServer.Connect());

  // Open the positioner
  User::LeaveIfError(iPositioner.Open(iLocationServer));//,KPosSimulationPsyImplUid));
   
   
   User::LeaveIfError( lbtserver.Connect() );
   CleanupClosePushL( lbtserver );
   iLog->Log(_L("Connection to RLbtServer Passed "));
   User::LeaveIfError( lbt.Open( lbtserver ) );
   iLog->Log(_L("Subsession opened "));
   CleanupClosePushL( lbt );
   
   //Delete all the existing trggers
   TRAP_IGNORE(lbt.DeleteTriggersL());
  
   CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
   iLog->Log(_L("Simulation PSY Repository object created"));
   User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
   iLog->Log(_L("Simulation input file set "));
   CleanupStack::PopAndDestroy(repository);
   
    //Construct a session trigger
  CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
  
  //Push to cleanup stack
  CleanupStack::PushL( trig );
  iLog->Log(_L("Startup Trigger Entry Created "));
  
  // Set Name
  trig->SetNameL(_L("Trigger1"));
 // _LIT( KMyTriggerHandlingProcessName, "About.exe");
  _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
  
  TSecureId secureid;
  trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
  //Set Requestor
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
  
  CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
      
  notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
  wait->Start( );
  iLog->Log(_L("Trigger Created"));
  notifier->After(50000000);
  notifier->StartNotification(wait);
  wait->Start( );
  iLog->Log(_L("Trigger Fired"));
  TLbtTriggerFireInfo FireInfo;
  TReal32 trigDistance;
  TPosition firePosition;
  FireInfo = notifier->GetFiredTrigger();
  FireInfo.iFiredPositionInfo.GetPosition(firePosition);
  firePosition.Distance(coordinate,trigDistance);
  iPositioner.Close();
  iLocationServer.Close();
  
      CleanupStack::PopAndDestroy( notifier );
  CleanupStack::PopAndDestroy( trig );
  CleanupStack::Pop( &lbt );
  CleanupStack::PopAndDestroy( &lbtserver );
  delete wait;
  
  return KErrNone; 
  
  }

   

   //List Trigger Test cases
    //Retrieving all the information about single startup trigger
    
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW070_testL( CStifItemParser& /* aItem */ )
    {
// _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 	 /*CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);*/
    
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
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
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    //User::After(5000000);
    
    iLog->Log(_L("Before GetTriggerLC "));
    CLbtTriggerInfo *Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtStartupTrigger *TrigEntry = static_cast <CLbtStartupTrigger*>(Triginfo->TriggerEntry());
    
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
   aRequestors.ResetAndDestroy();
   aRequestors2.ResetAndDestroy();
   
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
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    iLog->Log(_L("Test passed "));

    return KErrNone; 
      
    }
    
   //Retrieving all the information about single session trigger
   
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW071_testL( CStifItemParser& /* aItem */ )
    {
// _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	
 	 //CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 //User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 //CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a startup trigger
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
     
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    
   // TLbtTriggerId trigId=1;
    Triginfo = lbt.GetTriggerLC(trigId);
    
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
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
   //Check Activity status
/*	TLbtTriggerDynamicInfo::TLbtTriggerActivityStatus currstatus;
	currstatus = dyninfo -> iActivityStatus;
    if(currstatus!=TLbtTriggerDynamicInfo::EActive)
    {
    	return -99;
    }*/
    CleanupStack::PopAndDestroy( 1);//list options
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    
    return KErrNone; 
      
    }
    
  //Retrieving partial information about single startup trigger
       TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW072_testL( CStifItemParser& /* aItem */ )
    {
 	_LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
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
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object
	
    TLbtTriggerId trigId,trigId2;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    
    //TLbtTriggerId trigId=1;
    Triginfo = lbt.GetTriggerLC(trigId,
    							CLbtTriggerEntry::EAttributeState |  CLbtTriggerEntry::EAttributeName , 
    							TLbtTriggerDynamicInfo::EValidityStatus);
    
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    //Check Name of the trigger
    TDesC Name2 =TrigEntry->Name();  
     if(TrigEntry->Name()!=trig->Name()) 
    {
    	return -99;
    }
    //Check State of the trigger  
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=trig->State())
    {
    	return -99;
    } 
     //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    /*if(Type2 != NULL)
    {
    	return -99;
    }*/
    //Check condition
		CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
//	CLbtTriggerConditionArea::TDirection direction;
//	direction = Condition2->Direction();
	if(Condition2!=NULL)
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
    if(Managerui2!=KNullUid)
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

    CleanupStack::PopAndDestroy( 1 );//TrigInfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  //  delete wait;
    
    return KErrNone; 
      
    }
 
	//Retrieving partial information about single session  trigger
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW073_testL( CStifItemParser& /* aItem */ )
    {
// _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

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
    //Set Requestor
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

    TLbtTriggerId trigId,trigId2;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    
    
    Triginfo = lbt.GetTriggerLC(trigId,
    							CLbtTriggerEntry::EAttributeState |  CLbtTriggerEntry::EAttributeName , 
    							TLbtTriggerDynamicInfo::EValidityStatus);
    
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    //Check Name of the trigger
    TDesC Name2 =TrigEntry->Name();  
     if(TrigEntry->Name()!=trig->Name()) 
    {
    	return -99;
    }
    //Check State of the trigger  
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=trig->State())
    {
    	return -99;
    } 
     //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
   /* if(Type2 != NULL)
    {
    	return -99;
    }*/
    //Check condition
		CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
//	CLbtTriggerConditionArea::TDirection direction;
//	direction = Condition2->Direction();
	if(Condition2!=NULL)
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
    if(Managerui2!=KNullUid)
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
   
    CleanupStack::PopAndDestroy( 1 );//listoptions
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
      
    }

    //Retrieving trigger Informations which are not been set during creation
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW074_testL( CStifItemParser& /* aItem */ )
    {
// _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

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
    
   
   trig->SetNameL(_L("Trigger1"));
   _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
   //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
   TSecureId secureid;
   trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
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

    TLbtTriggerId trigId,trigId2;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    
    
    Triginfo = lbt.GetTriggerLC(trigId);
    
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    
     //Check Name of the trigger
    TDesC Name2 =TrigEntry->Name();  
     if(TrigEntry->Name()==KNullDesC) 
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
    if(Type2 != CLbtTriggerEntry::ETypeStartup)
    {
    	return -99;
    }
  
		//Check requestors 
    RRequestorStack   aRequestors,aRequestors2 ;
    TrigEntry->GetRequestorsL(aRequestors2);
    trig->GetRequestorsL(aRequestors);
    if(aRequestors.Count()!=1)
    {
    	return -99;
    }
    
 
    //Check trigger Id
    trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
    	return -99;
    }
    //Check ManagerUI
    TUid Managerui2 = TrigEntry->ManagerUi();
    if(Managerui2!=KNullUid)
    {
    	return -99;
    }
  
    CleanupStack::PopAndDestroy( 1 );//listoptions
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
      
    }
  
      
    //Retrieving Triggerhandling process,Commandline Information for session trigger
     TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW075_testL( CStifItemParser& /* aItem */ )
    {
 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
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
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    
    
    Triginfo = lbt.GetTriggerLC(trigId,CLbtTriggerEntry::EAttributeStartUpProcessId |  CLbtTriggerEntry::EAttributeStartUpCommandLine 
    							);
    /*
    CLbtStartupTrigger *TrigEntry = static_cast <CLbtStartupTrigger*>(Triginfo->TriggerEntry());
    
   // TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    HBufC* startupProcFile = HBufC::NewLC( KMaxFileName );
    TPtr aFileName = startupProcFile->Des();
    TSecureId aSecureId;
   TrigEntry->GetProcessId(aFileName,aSecureId);
   */
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
  
    
    }
    
  //Retrieving trigger Informations when subsession not opened
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW076_testL( CStifItemParser& /* aItem */ )
    {
// _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	
    
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    TLbtTriggerId trigId=1;
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
    Triginfo = lbt.GetTriggerLC(trigId);
    
   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
    
    return KErrNone; 
      
    }
    
    
   //Retrieving Informations when trigger not found
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW077_testL( CStifItemParser& /* aItem */ )
    {
// _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 
    
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    
    TLbtTriggerId trigId=99999;
    //TestModuleIf().SetExitReason( CTestModuleIf::EPanic, -1); 
    TRAPD(error, lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(); );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrNotFound)
    {
     
   // CleanupStack::PopAndDestroy( 1 );//listoptions
   
      
    return KErrNone; 
    }
    else
    {
    return -99;	
    }
      
    }
    
  //Retrieving Informations when trigger not owned by the client
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW078_testL( CStifItemParser& /* aItem */ )
    {
 
// 	_LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
     CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    _LIT( KFilename,"CreateTest.exe" );
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    TInt triggerId = KLbtNullTriggerId;
    if(retVal == KErrNone)
        {
		proc.Resume();
		TRequestStatus status = KRequestPending;
		proc.Rendezvous(status);
		User::WaitForRequest(status);
		triggerId = status.Int();
        }	
    
    TRAPD(error, lbt.GetTriggerLC(triggerId); CleanupStack::PopAndDestroy(  ););
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(error==KErrNotFound)
    {
     
   // CleanupStack::PopAndDestroy( 1 );//listoptions
    
      
    return KErrNone; 
    }
    else
    {
    return -99;	
    }
      
      
    }
    
 //Listing all the triggers owned by the client   
   
 TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW079_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    iLog->Log(_L("Before Connect"));
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    iLog->Log(_L("Before Open"));
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
    lbt.DeleteTriggersL();
 	 iLog->Log(_L("Before Create Trigger"));
     _LIT( KFilename,"CreateTest.exe" );
     RProcess proc;
     TInt retVal =proc.Create(KFilename,KNullDesC);
     TInt triggerId = KLbtNullTriggerId;
     if(retVal == KErrNone)
         {
         proc.Resume();
         TRequestStatus status = KRequestPending;
         proc.Rendezvous(status);
         User::WaitForRequest(status);
         triggerId = status.Int();
         }   
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
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
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
    	if(triggerId==(TrigEntry->Id())&& trigId != (TrigEntry->Id()))
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
    
   //List  triggers based on attribute filter  
     TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW080_testL( CStifItemParser& /* aItem */ )
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
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
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
    //Check condition
/*		CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	CLbtTriggerConditionArea::TDirection direction;
	direction = Condition2->Direction();
	if(Condition2->TriggerArea()!=NULL) 
	{
		return -99;
	}
	if(Condition2->Direction()!=0)
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
    if(Managerui2!=KNullUid)
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
		CleanupStack::PopAndDestroy( listOptions );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
     return KErrNone; 
    }

 //List  triggers based on area filter
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW081_testL( CStifItemParser& /* aItem */ )
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
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate1 );
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
    
 //List  triggers based on composite filter
  
     TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW082_testL( CStifItemParser& /* aItem */ )
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
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
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
    
    
    listOptions->SetFilter( compositeFilter ); 
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( areaFilter ); 
    CleanupStack::Pop( attribFilter );     
    //Set retrieved fields
    listOptions->SetRetrievedFields( 
        CLbtTriggerEntry::EAttributeName | 
        CLbtTriggerEntry::EAttributeCondition );
    
        
    lbt.GetTriggersL(trigInfoList,listOptions );
        
    CleanupStack::PopAndDestroy( listOptions );
    
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
    if(Managerui2!=KNullUid)
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
    
    //Sorted listing of triggers based on Name in ascending order
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW083_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    	  //Construct a session trigger Entry
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("bac"));
//     _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
   // TSecureId secureid;
  //  trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
     
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
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    trig->SetNameL(_L("abc"));
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
       
    listOptions->SetSortingOption(CLbtListTriggerOptions::ELbtTriggerNameAscending );
    
    
    lbt.GetTriggersL(trigInfoList,listOptions );
        
    CleanupStack::PopAndDestroy( listOptions );
    
    //Do something with the trigger list
    
    
    TInt count = trigInfoList.Count();
    	TBuf<256> currname;
    	TBuf<256> prevname;
   // TDesC prevname=KNullDesC;
     for ( TInt i = 0; i < count; i++ )
     {
     	CLbtTriggerEntry *TrigEntry = trigInfoList[i]->TriggerEntry();
    
    	TLbtTriggerDynamicInfo * dyninfo = trigInfoList[i] -> DynInfo() ;
    
        currname =TrigEntry->Name(); 
    	if(i==0) 
    	{
    		prevname = currname;
    	}
	    if(currname< prevname) 
	    {
	    	return -99;
	    }
	    else
	    {
	    	prevname = currname;
	    }
    
     }
        
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier);
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
      
    }
    
   
    //Sorted listing of triggers based on Distance to latest location
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW084_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    	  //Construct a session trigger Entry
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("abc"));
//     _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
    //TSecureId secureid;
    //trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
     
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
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
   	coordinate.Move(90,50000);
   	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,1000);
   //	circle->SetCenter(coordinate);
   	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
   	trig->SetCondition(condition2);
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
       
    listOptions->SetSortingOption(CLbtListTriggerOptions::ELbtDistanceToLatestLocationAscending );
    
    
    lbt.GetTriggersL(trigInfoList,listOptions );
        
    CleanupStack::PopAndDestroy( listOptions );
    
    //Do something with the trigger list
    
    TReal prevdistance=0;
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
     {
     	CLbtTriggerEntry *TrigEntry = trigInfoList[i]->TriggerEntry();
    
    	TLbtTriggerDynamicInfo * dyninfo = trigInfoList[i] -> DynInfo() ;
    	
    	TReal currdistance =dyninfo->iDistanceToLatestLocation; 
    	if(i==0) 
    	{
    		prevdistance = currdistance;
    	}
	    if(currdistance<prevdistance) 
	    {
	    	return -99;
	    }
	    else
	    {
	    	prevdistance = currdistance;
	    }
    
     }
    //free memory
    for ( TInt i = 0; i < count; i++ )
        {
        delete trigInfoList[i];
        }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig);
	CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone;  
    }
    
    //listing of triggers with only partial attributes filled in the result
    
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW085_testL( CStifItemParser& /* aItem */ )
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
	trig->SetState(CLbtTriggerEntry::EStateDisabled);
    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
    //Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);      
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
     User::After(5000000);
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
       
    listOptions->SetRetrievedFields( 
        CLbtTriggerEntry::EAttributeName | 
        CLbtTriggerEntry::EAttributeCondition );
    
    
    
    lbt.GetTriggersL(trigInfoList,listOptions );
        
    CleanupStack::PopAndDestroy( listOptions );
    
     TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
     {
     	CLbtTriggerEntry *TrigEntry = trigInfoList[i]->TriggerEntry();
    
    	TLbtTriggerDynamicInfo * dyninfo = trigInfoList[i] -> DynInfo() ;
    	
    	 //Check Name of the trigger
	    TDesC Name2 =TrigEntry->Name();  
	     if((TrigEntry->Name())==KNullDesC) 
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
	   /* TInt Type2 = TrigEntry ->Type();
	    if(Type2 != NULL)
	    {
	    	return -99;
	    }*/
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
	    if(Managerui2!=KNullUid)
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
	//	TLbtTriggerDynamicInfo::TLbtTriggerActivityStatus currstatus;
	//	currstatus = dyninfo -> iActivityStatus;
	    /*if(currstatus!=NULL)
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
    	CleanupStack::PopAndDestroy(2);
	CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone;  
    }

 //Listing of all the triggers owned by the client Asynchronously
TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW086_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	lbt.DeleteTriggersL();	 
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
      _LIT( KFilename,"CreateTest.exe" );

    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    TInt triggerId = KLbtNullTriggerId;
    if(retVal == KErrNone)
        {
        proc.Resume();
        TRequestStatus status = KRequestPending;
        proc.Rendezvous(status);
        User::WaitForRequest(status);
        triggerId = status.Int();
        }     
      
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
     notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(500000);
    notifier->GetTriggersList(lbt,trigInfoList,wait);
    wait->Start();
    //Do something with the trigger list
    
    
    TInt count = trigInfoList.Count();
     for ( TInt i = 0; i < count; i++ )
     {
     	CLbtTriggerEntry *TrigEntry = trigInfoList[i]->TriggerEntry();
    
    	TLbtTriggerDynamicInfo * dyninfo = trigInfoList[i] -> DynInfo() ;
    	if(triggerId==(TrigEntry->Id()))
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
    

   
    //Listing of triggers without opening the subsession
   
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW087_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
 	 TCoordinate coordinate(65.5285,23.9385);
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
   TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
    lbt.GetTriggersL(trigInfoList);
    
    //Do something with the trigger list
    
    return KErrNone;     
    }
    
    
    
    
   
  //Listing of all the trigger Ids owned by the client synchronously
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW088_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	  TRAP_IGNORE(lbt.DeleteTriggersL());
       _LIT( KFilename,"CreateTest.exe" );
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    TInt triggerId = KLbtNullTriggerId;
    if(retVal == KErrNone)
        {
        proc.Resume();
        TRequestStatus status = KRequestPending;
        proc.Rendezvous(status);
        User::WaitForRequest(status);
        triggerId = status.Int();
        }           
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
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );    
    RArray< TLbtTriggerId>  aTriggerIdList;
    
    lbt.ListTriggerIdsL (aTriggerIdList);
       
    //Do something with the trigger list
    
    
    TInt count = aTriggerIdList.Count();
    
    for ( TInt i = 0; i < count; i++ )
     {
       	if(triggerId==aTriggerIdList[i] && trigId != aTriggerIdList[i])
    	{
    		return -99;
    	}
     }
    //free memory
  /* for ( TInt i = 0; i < count; i++ )
        {
        delete aTriggerIdList;
        }*/
    aTriggerIdList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
      
    }
    
    //Listing of all the triggerIds owned by the client Asynchronously  
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW089_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	// TCoordinate coordinate(65.5285,23.9385);
    	 
    _LIT( KFilename,"CreateTest.exe" );
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    TInt triggerId = KLbtNullTriggerId;
    if(retVal == KErrNone)
        {
        proc.Resume();
        TRequestStatus status = KRequestPending;
        proc.Rendezvous(status);
        User::WaitForRequest(status);
        triggerId = status.Int();
        }   
    
    RArray< TLbtTriggerId>  aTriggerIdList;
    
    TRAP_IGNORE(lbt.DeleteTriggersL());
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
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );    
    notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait);
    wait->Start();
    //Do something with the trigger list
    
    
    
    TInt count = aTriggerIdList.Count();
    
    for ( TInt i = 0; i < count; i++ )
     {
       	if(triggerId==aTriggerIdList[i] && trigId!=aTriggerIdList[i])
    	{
    		return -99;
    	}
     }
    //free memory
    /*for ( TInt i = 0; i < count; i++ )
        {
        delete aTriggerIdList[i];
        }*/
    aTriggerIdList.Close();
     CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
      
    }
    
    //Getting trigger information incrementally using Iterator
    
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW090_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
    
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
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
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
	   //Check Activity status
	/*	TLbtTriggerDynamicInfo::TLbtTriggerActivityStatus currstatus;
		currstatus = dyninfo -> iActivityStatus;
	    if(currstatus==NULL)
	    {
	    	return -99;
	    }*/
        CleanupStack::Pop( TrigEntry );
        CleanupStack::PopAndDestroy( trigInfo );
        trigInfo = lbt.GetNextTriggerLC();
        };
   /* 
    //Increment retrieving without blocking active schedular
    CActiveSchedulerWait* wait = new ( ELeave ) CActiveSchedulerWait();
    CleanupStack::PushL( wait );
    
    //Create idle object to 
    CTrigIterator* trigIt = CTrigIterator::NewL( lbt, wait );
    CleanupStack::PushL( trigIt );
    wait->Start();//block current function
    
    CleanupStack::PopAndDestroy( trigIt );
    CleanupStack::PopAndDestroy( wait );
    
    //Close Location Triggering Server
	*/
	CleanupStack::PopAndDestroy( trigInfo);
	CleanupStack::PopAndDestroy( notifier);
	CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
      return KErrNone;
    }
    
    //Getting trigger information incrementally without creating Iterator
      TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW091_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	    
    //Get triggers. Note, the loop will block 
    //active schedular
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 4); 
    CLbtTriggerInfo* trigInfo = lbt.GetNextTriggerLC(); 
   
   
	  	CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
      return KErrNone;
    }
    
    
      //Asynchronous CreateTriggerIterator
    
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW092_testL( CStifItemParser& /* aItem */ )
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
            
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
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
    delete wait;    
    return KErrNone;

    }
    
     TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW094_testL( CStifItemParser& /* aItem */ )
    {
  	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	  //Construct a session trigger Entry
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("b3"));
     _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
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
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    
    return KErrNone; 
      
    }
    
    
     //Sorted listing of triggers based on Name in descending order
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW093_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    	  //Construct a session trigger Entry
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("abc"));
//     _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
   // TSecureId secureid;
   // trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
     
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
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    trig->SetNameL(_L("bac"));
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    listOptions->SetRetrievedFields( 
        TLbtTriggerDynamicInfo::EDistanceToLatestLocation  
 );      
    listOptions->SetSortingOption(CLbtListTriggerOptions::ELbtTriggerNameDescending );
    
    
    lbt.GetTriggersL(trigInfoList,listOptions );
        
    CleanupStack::PopAndDestroy( listOptions );
    
    //Do something with the trigger list
    
    
    TInt count = trigInfoList.Count();
    	TBuf<256> currname;
    	TBuf<256> prevname;
   // TDesC prevname=KNullDesC;
     for ( TInt i = 0; i < count; i++ )
     {
     	CLbtTriggerEntry *TrigEntry = trigInfoList[i]->TriggerEntry();
    
    	TLbtTriggerDynamicInfo * dyninfo = trigInfoList[i] -> DynInfo() ;
    
        currname =TrigEntry->Name(); 
    	if(i==0) 
    	{
    		prevname = currname;
    	}
	    if(currname> prevname) 
	    {
	    	return -99;
	    }
	    else
	    {
	    	prevname = currname;
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
    
    
       //deletion of swssion triggers once session closed
    TInt CFiringofStartupTriggerAndListTrigger::TCLBTFW095_testL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
    
    	  //Construct a session trigger Entry
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("abc"));
//     _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
   // TSecureId secureid;
   // trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
     
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
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    trig->SetNameL(_L("bac"));
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
    
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
    if(count!=0)
    {
    	return -99;
    }
    
	return KErrNone;
      
    }
    
//#endif
// -----------------------------------------------------------------------------
// CFiringofStartupTriggerAndListTrigger::AssertTrueL
// function for condition check.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFiringofStartupTriggerAndListTrigger::AssertTrueL( TBool aCondition,const TDesC& aMessage )
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
