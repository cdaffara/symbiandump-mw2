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
* Description:    Test module to implement Advanced trigger supervision cases
*
*/





// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <e32property.h>
#include <e32cmn.h>
#include <e32des16.h>
#include <e32base.h>
#include <s32mem.h>
#include <e32math.h> 
#include <centralrepository.h>
#include <lbs/SimulationPSYInternalCRKeys.h>
#include "AdvancedTriggerSupervision.h"

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


#include "AsynOperationAO.h"

// Location aquisition API 

#include <lbscommon.h>
#include <LbsRequestor.h>
#include <LbsPosition.h>

#include <EPos_MPosModulesObserver.h> 
#include <EPos_CPosModuleIdList.h>
#include <EPos_CPosModules.h>
#include <EPos_CPosModuleUpdate.h> // CPosModuleUpdate

#include <MProEngEngine.h>
#include <Profile.hrh>
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
class CPosModules;
class CPosModuleIdList;
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
// CAdvancedTriggerSupervision::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CAdvancedTriggerSupervision::Delete() 
    {

    }
void CAdvancedTriggerSupervision::EnableSimPSYL() 
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
// CAdvancedTriggerSupervision::GetCurrentCoordinateL
// Returns current position
// -----------------------------------------------------------------------------
//
void CAdvancedTriggerSupervision::GetCurrentCoordinateL( TCoordinate& aCoordinate )
    {
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL();
    CleanupStack::PushL( notifier );
    CActiveSchedulerWait* wait = new ( ELeave ) CActiveSchedulerWait;
    CleanupStack::PushL( wait );
    TPositionInfo positionInfo;
    // Ownership of wait is transferred to notifier
    notifier->CurrentPositionL( positionInfo,wait );
    CleanupStack::Pop( wait );
    wait->Start();
    TPosition position;
    positionInfo.GetPosition( position );
    aCoordinate.SetCoordinate( position.Latitude(),position.Longitude(),position.Altitude() );
    CleanupStack::Pop( notifier ); // notifier
    delete notifier;
    }

// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::SetProfileToOfflineL
// -----------------------------------------------------------------------------
void CAdvancedTriggerSupervision::SetProfileToOfflineL()
    {
    // Store current profile id.
    iCurrentProfile =  iProEngine->ActiveProfileId();
    // Change the active profile to Off-line
    iProEngine->SetActiveProfileL( EProfileOffLineId );
    }
// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::RestoreProfileL
// -----------------------------------------------------------------------------
void CAdvancedTriggerSupervision::RestoreProfileL()
    {
    iProEngine->SetActiveProfileL( iCurrentProfile );
    }

// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CAdvancedTriggerSupervision::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "ATSTest1", CAdvancedTriggerSupervision::ATSTest1L ),
        ENTRY( "ATSTest2", CAdvancedTriggerSupervision::ATSTest2L ),
        ENTRY( "ATSTest3", CAdvancedTriggerSupervision::ATSTest3L ),
        ENTRY( "ATSTest4", CAdvancedTriggerSupervision::ATSTest4L ),
        ENTRY( "ATSTest5", CAdvancedTriggerSupervision::ATSTest5L ),
        ENTRY( "ATSTest6", CAdvancedTriggerSupervision::ATSTest6L ),
        ENTRY( "ATSTest7", CAdvancedTriggerSupervision::ATSTest7L ),
        ENTRY( "ATSTest8", CAdvancedTriggerSupervision::ATSTest8L ),
        ENTRY( "ATSTest9", CAdvancedTriggerSupervision::ATSTest9L ),
        ENTRY( "ATSTest10", CAdvancedTriggerSupervision::ATSTest10L ),
        ENTRY( "ATSTest11", CAdvancedTriggerSupervision::ATSTest11L),
		ENTRY( "ATSTest12", CAdvancedTriggerSupervision::ATSTest12L ),
		ENTRY( "ATSTest13", CAdvancedTriggerSupervision::ATSTest13L),
		ENTRY( "ATSTest14", CAdvancedTriggerSupervision::ATSTest14L),
		ENTRY( "ATSTest15", CAdvancedTriggerSupervision::ATSTest15L),
		ENTRY( "ATSTest16", CAdvancedTriggerSupervision::ATSTest16L),
		ENTRY( "ATSTest17", CAdvancedTriggerSupervision::ATSTest17L),
		ENTRY( "ATSTest18", CAdvancedTriggerSupervision::ATSTest18L),
		ENTRY( "ATSTest19", CAdvancedTriggerSupervision::ATSTest19L),
		ENTRY( "ATSTest20", CAdvancedTriggerSupervision::ATSTest20L),
		ENTRY( "ATSTest21", CAdvancedTriggerSupervision::ATSTest21L),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest1
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

// Terminal starting from outside hysteresis-Entry type
TInt CAdvancedTriggerSupervision::ATSTest1L( CStifItemParser& aItem )
    {

    	iLog->Log(_L("Entering Test1"));
 //     _LIT( KSimulationFile,"c:\\system\\data\\simu_move1.sps" );
    
  _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
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
       
    	TSecureId secureid;
    	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    	//set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    	// set condition

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    coordinate.Move(90,200);
   	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
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
    	iLog->Log(_L("GFT"));
    	FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    	iLog->Log(_L("GP"));
    	firePosition.Distance(coordinate,trigDistance);
    	RestoreProfileL();
    	if( FireInfo.iTriggerId==trigId )
    	{
    
	  	CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
	   //delete wait;
	   return KErrNone;
    	}
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    
	    return -25; 
    }
   //  return KErrNone; 

    }


// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest2
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Terminal starting from inside hysteresis-Entry type
TInt CAdvancedTriggerSupervision::ATSTest2L( CStifItemParser& aItem )
    {

    	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
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
       
    	//set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    	// set condition

    	TCoordinate coordinate;
    	GetCurrentCoordinateL( coordinate );
    coordinate.Move(90,110);
   	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,200);
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
    	RestoreProfileL();
    	if( FireInfo.iTriggerId==trigId)
    	{
    
	  	CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
	   //delete wait;
	   iLog->Log(_L("Test case passed "));
	   return KErrNone;
    	}
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    
	    return -25; 
    }
   //  return KErrNone; 

    }


// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest3
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Terminal starting from inside trigger area-Entry type
TInt CAdvancedTriggerSupervision::ATSTest3L( CStifItemParser& aItem )
    {

    	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
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
       
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    	// set condition

    	TCoordinate coordinate;
    	GetCurrentCoordinateL( coordinate );
    
   	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
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
    	RestoreProfileL();
    	if( FireInfo.iTriggerId==trigId)
    	{
  
	  	CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
	   //delete wait;
	   iLog->Log(_L("Test case passed "));
	   return KErrNone;
    	}
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    
	    return -25; 
    }
   //  return KErrNone; 

    }

// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest4
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Terminal starting from outside hysteresis- Exit type
TInt CAdvancedTriggerSupervision::ATSTest4L( CStifItemParser& aItem )
    {

    	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
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
       
    	TSecureId secureid;
    	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    	//set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);	
    	// set condition

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    coordinate.Move(90,200);
   	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
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
    	iLog->Log(_L("Trigger Fired"));
    	TLbtTriggerFireInfo FireInfo;
    	TReal32 trigDistance;
    	TPosition firePosition;
    	FireInfo = notifier->GetFiredTrigger();
    	FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    	firePosition.Distance(coordinate,trigDistance);
    	RestoreProfileL();
    	if( FireInfo.iTriggerId==trigId)
    	{
   
	  	CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
	   //delete wait;
	   iLog->Log(_L("Test case passed "));
	   return KErrNone;
    	}
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    
	    return -25; 
    }
   //  return KErrNone; 

    }


// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest5
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Terminal starting from inside hysteresis- Exit type
TInt CAdvancedTriggerSupervision::ATSTest5L( CStifItemParser& aItem )
    {

    	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
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
       
    	//set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    	// set condition

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    coordinate.Move(90,110);
   	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
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
    	iLog->Log(_L("Trigger Fired"));
    	TLbtTriggerFireInfo FireInfo;
    	TReal32 trigDistance;
    	TPosition firePosition;
    	FireInfo = notifier->GetFiredTrigger();
    	FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    	firePosition.Distance(coordinate,trigDistance);
    	
    	RestoreProfileL();
    	if(  FireInfo.iTriggerId==trigId)
    	{
   
	  	CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
	   //delete wait;
	   iLog->Log(_L("Test case passed "));
	   return KErrNone;
    	}
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    
	    return -25; 
    }
   //  return KErrNone; 

    }


// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest6
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Terminal starting from inside trigger area- Exit type
TInt CAdvancedTriggerSupervision::ATSTest6L( CStifItemParser& aItem )
    {

    	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
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
       
    	//set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    	// set condition

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
   	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
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
    	iLog->Log(_L("Trigger Fired"));
    	TLbtTriggerFireInfo FireInfo;
    	TReal32 trigDistance;
    	TPosition firePosition;
    	FireInfo = notifier->GetFiredTrigger();
    	FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    	firePosition.Distance(coordinate,trigDistance);
    	
    	RestoreProfileL();
    	if( FireInfo.iTriggerId==trigId)
    	{
  
	  	CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
	   //delete wait;
	   iLog->Log(_L("Test case passed "));
	   return KErrNone;
    	}
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    
	    return -25; 
    }
   //  return KErrNone; 

    }


// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest7
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Nested triggers - one trigger inside another
TInt CAdvancedTriggerSupervision::ATSTest7L( CStifItemParser& aItem )
    {


    iLog->Log(_L("Entering Test1"));
  _LIT( KSimulationFile,"c:\\system\\data\\simu_move3.sps" );
// _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 RLbtServer lbtserver;
 RLbt lbt;
 iLog->Log(_L("Before connecting"));
 User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    iLog->Log(_L("Connection to RLbtServer Passed "));
 User::LeaveIfError( lbt.Open(lbtserver));
 iLog->Log(_L("Subsession opened "));
 CleanupClosePushL( lbt );
 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
     
 //Enable only simpsy
 EnableSimPSYL();
 //Clear all triggers
 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 iLog->Log(_L("Simulation PSY Repository object created"));
//   User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
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

TCoordinate coordinate;
GetCurrentCoordinateL( coordinate );

    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,200);
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
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,100);
    circle2->SetRadius(500);
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                            circle2,
                                            CLbtTriggerConditionArea::EFireOnEnter);
    
    trig->SetCondition(condition2);
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    
    iLog->Log(_L("Triggers Created"));
    notifier->StartNotification( wait );
    wait->Start( );
    wait->Start( );
    RArray<TLbtTriggerFireInfo> Firedtriggers;
    lbt.GetFiredTriggersL(Firedtriggers);
    RestoreProfileL();
    if(Firedtriggers.Count()==2)
    {
        CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( trig );
   CleanupStack::Pop( &lbt );
   CleanupStack::PopAndDestroy( &lbtserver );
        return KErrNone;
    }
    else
    {
        CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( trig );
   CleanupStack::Pop( &lbt );
   CleanupStack::PopAndDestroy( &lbtserver );
        return -99;
    }

//  return KErrNone; 

    }


// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest8
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Nested triggers - same as above - in this case inside is an exit and outside is an entry 

//trigger
TInt CAdvancedTriggerSupervision::ATSTest8L( CStifItemParser& aItem )
    {

    	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move3.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
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

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    
   		CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,200);
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
    	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,100);
    	circle2->SetRadius(500);
    	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnExit);
    	
    	trig->SetCondition(condition2);
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	
    	iLog->Log(_L("Triggers Created"));
    	notifier->StartNotification( wait );
  		wait->Start( );
  		wait->Start( );
  		RArray<TLbtTriggerFireInfo> Firedtriggers;
  		lbt.GetFiredTriggersL(Firedtriggers);
  		RestoreProfileL();
  		if(Firedtriggers.Count()==2)
  		{
  			CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
  			return KErrNone;
  		}
  		else
  		{
  			CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
  			return -99;
  		}
    
   //  return KErrNone; 

    }


// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest9
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Nested triggers - same as above - in this case inside is an Entry and outside is an 

//Exit trigger
TInt CAdvancedTriggerSupervision::ATSTest9L( CStifItemParser& aItem )
    {

    	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
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
        
        
    	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    	CleanupStack::PushL( notifier );
    
    	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,200);
    	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition2);
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	
    	iLog->Log(_L("Triggers Created"));
    	notifier->StartNotification( wait );
  		wait->Start( );
  	    wait->Start( );
  		
  		RArray<TLbtTriggerFireInfo> Firedtriggers;
  		lbt.GetFiredTriggersL(Firedtriggers);
  		CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    RestoreProfileL();
  		if(Firedtriggers.Count()==2)
  		{
  			return KErrNone;
  		}
  		else 
  		{
  			return -99;
  		}
    
    }


// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest10
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Overlapping triggers - triggers adjecent to each other and triggers whose boundaries 

//overlap one another
TInt CAdvancedTriggerSupervision::ATSTest10L( CStifItemParser& aItem )
    {

      	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move1.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
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

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
   		    
   		CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
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
    	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,100);
    	coordinate.Move(90,200);
    	circle2->SetCenter(coordinate);
    	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnExit);
    	
    	trig->SetCondition(condition2);
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	
    	iLog->Log(_L("Triggers Created"));
    	notifier->StartNotification( wait );
  		wait->Start( );
  		wait->Start( );
  		RArray<TLbtTriggerFireInfo> Firedtriggers;
  		lbt.GetFiredTriggersL(Firedtriggers);
  		RestoreProfileL();
  		if(Firedtriggers.Count()==2)
  		{
  			CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
  			return KErrNone;
  		}
  		else
  		{
  			CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
  			return -99;
  		}
    

    }
    
    
    // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest11
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Overlapping triggers - triggers adjecent to each other and triggers whose boundaries 

//overlap one another(Here one is exit type and other one is Entry type)
TInt CAdvancedTriggerSupervision::ATSTest11L( CStifItemParser& aItem )
    {

        	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
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

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
   		    
   		CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
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
    	
       	coordinate.Move(90,100);
       	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,100);
    	circle2->SetCenter(coordinate);
    	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition2);
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	
    	iLog->Log(_L("Triggers Created"));
    	notifier->StartNotification( wait );
  		wait->Start( );
        wait->Start( );

    	iLog->Log(_L("Trigger Fired"));
    	RArray<TLbtTriggerFireInfo> Firedtriggers;
  		lbt.GetFiredTriggersL(Firedtriggers);
  		RestoreProfileL();
  		if(Firedtriggers.Count()==2)
  		{
  			CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
  			return KErrNone;
  		}
  		else
  		{
  			CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
  			return -99;
  		}
    

    }
    
    
   // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest12
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//Testing of cluster of triggers
TInt CAdvancedTriggerSupervision::ATSTest12L( CStifItemParser& aItem )
    {

        	iLog->Log(_L("Entering Test1"));
     _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
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

	    TCoordinate coordinate;
	    GetCurrentCoordinateL( coordinate );
  		    
   		CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
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
    	TCoordinate movCoordinates( coordinate.Latitude(),coordinate.Longitude() );
    	
    	TCoordinate coordinates[4];
    	// Initialise the coordinate
    	for( TInt i=0;i<4;i++ )
   		{
   		movCoordinates.Move( 90,300 );
   		coordinates[i] = movCoordinates;
   		}
    	for(int i=0;i<4;i++)
    	{
    	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);	
      //	coordinate.Move(45,100);
    	circle->SetCenter(coordinates[i]);
    	if(i%2==0)
    	{
    		
       	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	trig->SetCondition(condition2);
    	}
    	else
    	{
    	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);	
    	trig->SetCondition(condition2);
    	}
    	
    	
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	}
    
    	iLog->Log(_L("Triggers Created"));
    	
    	notifier->StartNotification( wait );
  		wait->Start( );
        wait->Start( );
        wait->Start( );
        wait->Start( );


  		RArray<TLbtTriggerFireInfo> Firedtriggers;
  		lbt.GetFiredTriggersL(Firedtriggers);
  		RestoreProfileL();
  		if(Firedtriggers.Count()==4)
  		{
  			CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
  			return KErrNone;
  		}
  		else
  		{
  			CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
  			return -99;
  		}
    

    }
    
    
    // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest13
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// Inaccuracy of position fixes - Horizontal accuracy is really poor.
TInt CAdvancedTriggerSupervision::ATSTest13L( CStifItemParser& aItem )
    {

    	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move4.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
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
       
    	TSecureId secureid;
    	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    	//set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    	// set condition

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
    	coordinate.Move(90,1000);
   	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,2000);
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
    	RestoreProfileL();
    	if( FireInfo.iTriggerId==trigId)
    	{
  
	  	CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
	  // //delete wait;
	   iLog->Log(_L("Test case passed "));
	   return KErrNone;
    	}
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    
	    return -25; 
    }
   //  return KErrNone; 

    }
    
    
    
        // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest14
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//When two triggers are far away from each other.
TInt CAdvancedTriggerSupervision::ATSTest14L( CStifItemParser& aItem )
    {

        	iLog->Log(_L("Entering Test1"));
    //  _LIT( KSimulationFile,"c:\\system\\data\\simu_move1.sps" );
  _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a startup trigger
    	 CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    	//Push to cleanup stack
    	CleanupStack::PushL( trig );
    	iLog->Log(_L("Startup Trigger Entry Created "));
    
    	// Set Name
    	trig->SetNameL(_L("abc"));
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

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
   		    
   		CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
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
    	
       	coordinate.Move(90,500);
       	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,100);
    	circle2->SetCenter(coordinate);
    	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition2);
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	
    	iLog->Log(_L("Triggers Created"));
    	notifier->StartNotification( wait );
  		wait->Start( );
  		
    	iLog->Log(_L("Trigger Fired"));
    	TLbtTriggerFireInfo FireInfo;
    	TReal32 trigDistance;
    	TPosition firePosition;
    	FireInfo = notifier->GetFiredTrigger();
    	RestoreProfileL();
    	if(FireInfo.iTriggerId!=trigId)
    	{
    //	notifier->StartNotification( wait );
  		wait->Start( );	
    	}
    	FireInfo = notifier->GetFiredTrigger();
    	FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    	firePosition.Distance(coordinate,trigDistance);
     
    	if( FireInfo.iTriggerId==trigId)
    	{
    
	  	CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
	   //delete wait;
	   iLog->Log(_L("Test case passed "));
	   
	   	
	   	 return KErrNone;
	 
    	}
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    
	    return -25; 
    }
   //  return KErrNone; 


    }
    


       // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest15
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//When we delete intermediate trigger when three triggers are present in the system
TInt CAdvancedTriggerSupervision::ATSTest15L( CStifItemParser& aItem )
    {

        	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
 	 iLog->Log(_L("Simulation PSY Repository object created"));
//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
	 
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

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
   		    
   		CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
    	CleanupStack::PushL( circle );
    
         
    	// ownership of circle object transferred to the condition object
    	CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    	CleanupStack::Pop( circle );
    
    	trig->SetCondition(condition); // ownership transferred to object

    	TLbtTriggerId trigId,trigIdtochange;
        
        
    	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    	CleanupStack::PushL( notifier );
    
    	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	
       	coordinate.Move(90,300);
       	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,100);
    	circle2->SetCenter(coordinate);
    	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition2);
    	notifier->CreateTriggers( lbt,*trig,trigIdtochange,ETrue,wait );
    	wait->Start( );
    	
    	coordinate.Move(90,400);
    	CLbtGeoCircle* circle3=CLbtGeoCircle::NewL(coordinate,100);
    	circle3->SetCenter(coordinate);
    	CLbtTriggerConditionArea* condition3=CLbtTriggerConditionArea::NewL(
                                                circle3,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition3);
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	notifier->StartNotification( wait );
  		wait->Start( );
    //	notifier->After(10000000);
    //	wait->Start( );
    	lbt.DeleteTriggerL(trigIdtochange);
    	
    	iLog->Log(_L("Triggers Created"));
    //	notifier->StartNotification( wait );
  		wait->Start( );
    	iLog->Log(_L("Trigger Fired"));
    	TLbtTriggerFireInfo FireInfo;
    	TReal32 trigDistance;
    	TPosition firePosition;
    	FireInfo = notifier->GetFiredTrigger();
    	FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    	firePosition.Distance(coordinate,trigDistance);
    	RestoreProfileL();
     	if(FireInfo.iTriggerId ==trigIdtochange)
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
    
    
      // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest16
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//If we create a new trigger in the path of the movement of the terminal.
TInt CAdvancedTriggerSupervision::ATSTest16L( CStifItemParser& aItem )
    {

        	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt,lbt1;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 User::LeaveIfError( lbt1.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt1 );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
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

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
   		    
   		CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
    	CleanupStack::PushL( circle );
    
         
    	// ownership of circle object transferred to the condition object
    	CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    	CleanupStack::Pop( circle );
    
    	trig->SetCondition(condition); // ownership transferred to object

    	TLbtTriggerId trigId,trigIdtochange;
        
        
    	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    	CleanupStack::PushL( notifier );
    
    	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	
       	coordinate.Move(90,500);
       	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,100);
    	circle2->SetCenter(coordinate);
    	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition2);
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	notifier->StartNotification( wait );
  		wait->Start( );
    //	User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
    //	notifier->After(3000000);
    //	wait->Start( );
    	coordinate.Move(270,200);
    	CLbtGeoCircle* circle3=CLbtGeoCircle::NewL(coordinate,100);
    	circle3->SetCenter(coordinate);
    	CLbtTriggerConditionArea* condition3=CLbtTriggerConditionArea::NewL(
                                                circle3,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition3);
    	notifier->CreateTriggers( lbt1,*trig,trigIdtochange,ETrue,wait );
    	wait->Start( );
    
       	
    	iLog->Log(_L("Triggers Created"));
    //	notifier->StartNotification( wait );
  		wait->Start( );
  		
  	//	notifier->After(9000000);
    //	wait->Start( );
    	iLog->Log(_L("Trigger Fired"));
    	TLbtTriggerFireInfo FireInfo;
    	TReal32 trigDistance;
    	TPosition firePosition;
    	FireInfo = notifier->GetFiredTrigger();
    	FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    	firePosition.Distance(coordinate,trigDistance);
    	RestoreProfileL();
     	if(FireInfo.iTriggerId ==trigIdtochange)
     	{
     		CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt1 );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
     		return KErrNone;
     	}
   
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt1 );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    
	    return -25; 
    }
   //  return KErrNone; 


    }
    

      // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest17
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//If we disable an existing trigger in the path of the movement of the terminal.
TInt CAdvancedTriggerSupervision::ATSTest17L( CStifItemParser& aItem )
    {

        	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
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

    TCoordinate coordinate;
    GetCurrentCoordinateL( coordinate );
   		    
   		CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,100);
    	CleanupStack::PushL( circle );
    
         
    	// ownership of circle object transferred to the condition object
    	CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    	CleanupStack::Pop( circle );
    
    	trig->SetCondition(condition); // ownership transferred to object

    	TLbtTriggerId trigId,trigIdtochange;
        
        
    	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    	CleanupStack::PushL( notifier );
    
    	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	
       	coordinate.Move(90,500);
       	CLbtGeoCircle* circle3=CLbtGeoCircle::NewL(coordinate,100);
    	circle3->SetCenter(coordinate);
    	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle3,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition2);
    	notifier->CreateTriggers( lbt,*trig,trigIdtochange,ETrue,wait );
    	wait->Start( );
    	
    	coordinate.Move(90,500);
    	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,100);
    	circle2->SetCenter(coordinate);
    	CLbtTriggerConditionArea* condition3=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition3);
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	notifier->StartNotification( wait );
  		wait->Start( );
    //	notifier->After(20000000);
    //	wait->Start( );
    	lbt.SetTriggerStateL(trigIdtochange,CLbtTriggerEntry::EStateDisabled,ELbtTrue);
    	
    
    //	notifier->StartNotification( wait );
  		wait->Start( );
    	iLog->Log(_L("Trigger Fired"));
    	TLbtTriggerFireInfo FireInfo;
    	TReal32 trigDistance;
    	TPosition firePosition;
    	FireInfo = notifier->GetFiredTrigger();
    	FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    	firePosition.Distance(coordinate,trigDistance);
    	RestoreProfileL();
     	if(FireInfo.iTriggerId ==trigIdtochange)
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


  // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest18
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//When we change the direction of movement of the terminal 
TInt CAdvancedTriggerSupervision::ATSTest18L( CStifItemParser& aItem )
    {

        	iLog->Log(_L("Entering Test1"));
      _LIT( KSimulationFile,"c:\\system\\data\\test1.nme" );//Need to create nme file
 // _LIT( KSimulationFile0,"c:\\system\\data\\simu_move2.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before connecting"));
 	 User::LeaveIfError( lbtserver.Connect() );
        CleanupClosePushL( lbtserver );
        iLog->Log(_L("Connection to RLbtServer Passed "));
 	 User::LeaveIfError( lbt.Open(lbtserver));
 	 iLog->Log(_L("Subsession opened "));
 	 CleanupClosePushL( lbt );
 	 
     // Set profile to offline mode.This is required to avoid movement detection blocking the 
     // trigger firing.
     SetProfileToOfflineL();
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
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

    	TCoordinate coordinate(65.5285,23.9385);
   		    
   		CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    	CleanupStack::PushL( circle );
    
         
    	// ownership of circle object transferred to the condition object
    	CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    	CleanupStack::Pop( circle );
    
    	trig->SetCondition(condition); // ownership transferred to object

    	TLbtTriggerId trigId,trigIdtochange;
        
        
    	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate);
    	CleanupStack::PushL( notifier );
    
    	CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	
       	coordinate.Move(90,5000);
       	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,500);
    	circle2->SetCenter(coordinate);
    	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition2);
    	notifier->CreateTriggers( lbt,*trig,trigIdtochange,ETrue,wait );
    	wait->Start( );
    	
    	coordinate.Move(90,1000);
    	CLbtGeoCircle* circle3=CLbtGeoCircle::NewL(coordinate,500);
    	circle3->SetCenter(coordinate);
    	
    	CLbtTriggerConditionArea* condition3=CLbtTriggerConditionArea::NewL(
                                                circle3,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
    	trig->SetCondition(condition3);
    	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    	wait->Start( );
    	
    	notifier->After(10000000);
    
    
    
    	iLog->Log(_L("Triggers Created"));
    	notifier->StartNotification( wait );
  		wait->Start( );
  		
  		notifier->StartNotification( wait );
  		wait->Start( );
  		
    	iLog->Log(_L("Trigger Fired"));
    	TLbtTriggerFireInfo FireInfo;
    	TReal32 trigDistance;
    	TPosition firePosition;
    	FireInfo = notifier->GetFiredTrigger();
    	FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    	firePosition.Distance(coordinate,trigDistance);
    	RestoreProfileL();
    	if( trigDistance>=1000)
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
   
   
	  	CleanupStack::PopAndDestroy( notifier );
	   CleanupStack::PopAndDestroy( trig );
	   CleanupStack::Pop( &lbt );
	   CleanupStack::PopAndDestroy( &lbtserver );
	   //delete wait;
	   iLog->Log(_L("Test case passed "));
	   if(numofprocesses>0)
	       return KErrNone; 
	   else
	   return -99;
    	}
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::Pop( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
	    //delete wait;
	    
	    return -25; 
    }
   //  return KErrNone; 


    }

  // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest19
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
 // Testing the hysteresis condition for entry type of trigger  
    
TInt CAdvancedTriggerSupervision::ATSTest19L( CStifItemParser& /* aItem */ )
    {

 _LIT( KSimulationFile,"c:\\system\\data\\test1.nme" );

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
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	
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
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    //set Requestor     
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
	trig->SetTimeToRearm(0);
    
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
  //  notifier->StartNotification( wait );
  //  wait->Start( );
    
    notifier->StartNotification( wait );
    wait->Start( );
    notifier->iWaitStatus = KRequestPending;
    
   // notifier->StartNotification( wait );
	notifier->After(1000000);
	wait->Start( );
    
    iLog->Log(_L("Trigger Fired"));
    TLbtTriggerFireInfo FireInfo;
    TReal32 trigDistance;
    TPosition firePosition;
    FireInfo = notifier->GetFiredTrigger();
    FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    firePosition.Distance(coordinate,trigDistance);
    RestoreProfileL();
    if( trigDistance<=1000 &&notifier->iTriggerFireCount ==2)
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    
    return KErrNone; 
    }
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    
    return -25; 
    }
    }
    
    
    // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest20
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
    //Testing the hysteresis condition for exit type of trigger
    
    TInt CAdvancedTriggerSupervision::ATSTest20L( CStifItemParser& /* aItem */ )
    {

 _LIT( KSimulationFile,"c:\\system\\data\\test2.nme" );
	
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
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	
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
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
   // _LIT( KMyTriggerHandlingProcessName, "ConsoleUI.exe");
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    //set Requestor     
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
	trig->SetTimeToRearm(0);

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
    RestoreProfileL();
    if( trigDistance>=1000 &&notifier->iTriggerFireCount ==2)
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    
    return KErrNone; 
    }
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    
    return -25; 
    }
    }

  // -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::ATSTest21
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
    //Test to check sudden accellaration of the terminal.
    
    TInt CAdvancedTriggerSupervision::ATSTest21L( CStifItemParser& /* aItem */ )
    {

 _LIT( KSimulationFile,"c:\\system\\data\\test2.nme" );
	
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
 	 
 	 //Enable only simpsy
     EnableSimPSYL();
     //Clear all triggers
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 
 	
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
       
    //set Requestor     
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
    notifier->StartNotification( wait );
    wait->Start( );
    notifier->StartNotification( wait );
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
    RestoreProfileL();
    if( trigDistance<=1000 &&notifier->iTriggerFireCount ==2)
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    
    return KErrNone; 
    }
    else
    {
    	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    
    return -25; 
    }
    }

// -----------------------------------------------------------------------------
// CAdvancedTriggerSupervision::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CAdvancedTriggerSupervision::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
