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
* Description:     Test module to implement Update trigger related test cases
*
*/






// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "UpdateTrigger.h"
#include "LbtAppCellInfo.h"
#include "t_triggerfireobserver.h"
#include <e32property.h>
#include <centralrepository.h>
#include <SimulationPSYInternalCRKeys.h>
//LT API
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
//#include <lbtoperation.h>
#include <EPos_MPosModulesObserver.h> 
#include <EPos_CPosModuleIdList.h>
#include <EPos_CPosModules.h>
#include <EPos_CPosModuleUpdate.h> // CPosModuleUpdate

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
// CUpdateTrigger::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//

void CUpdateTrigger::Delete() 
    {

    }
    
void CUpdateTrigger::EnableSimPSYL() 
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
// CUpdateTrigger::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CUpdateTrigger::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
       // ENTRY( "Example", CUpdateTrigger::ExampleL ),
       ENTRY( "test01", CUpdateTrigger::TCLBTFW001_testL ),
       ENTRY( "test02", CUpdateTrigger::TCLBTFW002_testL ),
       ENTRY( "test03", CUpdateTrigger::TCLBTFW003_testL ),
       ENTRY( "test04", CUpdateTrigger::TCLBTFW004_testL ),
       ENTRY( "test05", CUpdateTrigger::TCLBTFW005_testL ),
       ENTRY( "test06", CUpdateTrigger::TCLBTFW006_testL ),
       ENTRY( "test07", CUpdateTrigger::TCLBTFW007_testL ),
       ENTRY( "test08", CUpdateTrigger::TCLBTFW008_testL ),
       ENTRY( "test09", CUpdateTrigger::TCLBTFW009_testL ),
       ENTRY( "test10", CUpdateTrigger::TCLBTFW010_testL ),
       ENTRY( "test11", CUpdateTrigger::TCLBTFW011_testL ),
       ENTRY( "test12", CUpdateTrigger::TCLBTFW012_testL ),
       ENTRY( "test13", CUpdateTrigger::TCLBTFW013_testL ),
       ENTRY( "test14", CUpdateTrigger::TCLBTFW014_testL ),
       ENTRY( "test15", CUpdateTrigger::TCLBTFW015_testL ),
       ENTRY( "test16", CUpdateTrigger::TCLBTFW016_testL ),
       ENTRY( "TestFireOnUpdate1", CUpdateTrigger::TestFireOnUpdate1L ),
       ENTRY( "TestFireOnUpdate2", CUpdateTrigger::TestFireOnUpdate2L ),
       ENTRY( "TestFireOnUpdate3", CUpdateTrigger::TestFireOnUpdate3L ),
       ENTRY( "TestFireOnUpdate4", CUpdateTrigger::TestFireOnUpdate4L ),
       ENTRY( "TestFireOnUpdate5", CUpdateTrigger::TestFireOnUpdate5L ),
       ENTRY( "TestFireOnUpdate6", CUpdateTrigger::TestFireOnUpdate6L ),
       ENTRY( "TestFireOnUpdate7", CUpdateTrigger::TestFireOnUpdate7L ),
       ENTRY( "TestFireOnUpdate8", CUpdateTrigger::TestFireOnUpdate8L ),
       ENTRY( "TestFireOnUpdate9", CUpdateTrigger::TestFireOnUpdate9L ),
       ENTRY( "TestFireOnUpdate10", CUpdateTrigger::TestFireOnUpdate10L ),
       ENTRY( "TestFireOnUpdate11", CUpdateTrigger::TestFireOnUpdate11L ),
       ENTRY( "TestFireOnUpdate12", CUpdateTrigger::TestFireOnUpdate12L ),
       ENTRY( "TestFireOnUpdate13", CUpdateTrigger::TestFireOnUpdate13L ),
       ENTRY( "TestFireOnUpdate14", CUpdateTrigger::TestFireOnUpdate14L ),
       ENTRY( "TestFireOnUpdate15", CUpdateTrigger::TestFireOnUpdate15L ),
       ENTRY( "TestAsyncUpdate1L", CUpdateTrigger::TestAsyncUpdate1L ),
       ENTRY( "TestAsyncUpdate2L", CUpdateTrigger::TestAsyncUpdate2L ),
       ENTRY( "TestAsyncUpdate3L", CUpdateTrigger::TestAsyncUpdate3L ),
       ENTRY( "TestAsyncUpdate4L", CUpdateTrigger::TestAsyncUpdate4L ),
       ENTRY( "TestAsyncUpdate5L", CUpdateTrigger::TestAsyncUpdate5L ),
       ENTRY( "TestAsyncUpdate6L", CUpdateTrigger::TestAsyncUpdate6L ),
       ENTRY( "TestAsyncUpdate7L", CUpdateTrigger::TestAsyncUpdate7L ),
       ENTRY( "TestAsyncUpdate8L", CUpdateTrigger::TestAsyncUpdate8L ),
       ENTRY( "TestAsyncUpdate9L", CUpdateTrigger::TestAsyncUpdate9L ),
       ENTRY( "TestAsyncUpdate10L", CUpdateTrigger::TestAsyncUpdate10L ),
       
       };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CUpdateTrigger::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CUpdateTrigger::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KUpdateTrigger, "UpdateTrigger" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KUpdateTrigger, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KUpdateTrigger, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }
    
    //Updating attributes of a startup trigger - TC_LBT012_01
     TInt CUpdateTrigger::TCLBTFW001_testL( CStifItemParser& /* aItem */ )
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
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
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
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition
    |CLbtTriggerEntry::EAttributeStartUpCommandLine,ELbtTrue
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
    
    
      //Updating attributes of a session trigger - TC_LBT012_02
     TInt CUpdateTrigger::TCLBTFW002_testL( CStifItemParser& /* aItem */ )
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
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object
	//Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId,trigId2;
    trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
 //   User::After(5000000);
    
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

    newtrig->SetId(trigId);
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
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
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
    //Updating Nonmodifiable attributes of a startup trigger - TC_LBT012_03
    TInt CUpdateTrigger::TCLBTFW003_testL( CStifItemParser& /* aItem */ )
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
    
    
      //Trying to update Nonmodifiable attributes of session trigger - TC_LBT012_04
     TInt CUpdateTrigger::TCLBTFW004_testL( CStifItemParser& /* aItem */ )
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
 //   User::After(5000000);
    
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
    
    newtrig->SetManagerUi(TUid::Uid(0x12345678));

    newtrig->SetId(trigId);
    
    newtrig->SetRequestorL(CRequestor::ERequestorContact,CRequestor::EFormatTelephone,_L("Test2") );
      
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
      
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
      
      
    }
    
    
     //Trying to update trigger with zero length name - TC_LBT012_05
      TInt CUpdateTrigger::TCLBTFW005_testL( CStifItemParser& /* aItem */ )
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
 //   User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L(""));
    
    newtrig->SetId(trigId);
    
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    TRAPD(error,lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName,ELbtTrue));
    if(error == KErrArgument)
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
    
   
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
    
      
    }
    
    
     //Trying to update trigger without defining the trigger area- TC_LBT012_06
      TInt CUpdateTrigger::TCLBTFW006_testL( CStifItemParser& /* aItem */ )
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
 //   User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    
       
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL();
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
    
    newtrig->SetId(trigId);
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
   
    TRAPD(error,lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue));
    if(error == KErrArgument)
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
    
   
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
      
    }
    
    
     //Trying to update trigger with trigger area less than minimum size of trigger area - TC_LBT012_07
      TInt CUpdateTrigger::TCLBTFW007_testL( CStifItemParser& /* aItem */ )
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
 //   User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,0);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
    newtrig->SetId(trigId);
      
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    TRAPD(error,lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue));
    if(error == KErrTriggerAreaTooSmall)
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
    
   
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
      
    }
    
    
      //Trying to update trigger without opening the subsession - TC_LBT012_08
      TInt CUpdateTrigger::TCLBTFW008_testL( CStifItemParser& /* aItem */ )
    {
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect());
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	     
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,1);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
    
     TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
      
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,
    CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
    iLog->Log(_L("After UpdateTriggerL "));
    
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
    
      //Trying to update trigger when it is not available - TC_LBT012_09
      TInt CUpdateTrigger::TCLBTFW009_testL( CStifItemParser& /* aItem */ )
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
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,1);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
     newtrig->SetId(99999999);
      
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    TRAPD(error,lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue));
    if(error == KErrNotFound)
    {
    iLog->Log(_L("After UpdateTriggerL "));
    
   
    
    
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 	
    }
    else
    {
    	iLog->Log(_L("After UpdateTriggerL "));
    
      
    
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
      
    }
    
    
        //Trying to update trigger owned by another process - TC_LBT012_10
      TInt CUpdateTrigger::TCLBTFW010_testL( CStifItemParser& /* aItem */ )
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
     /***************Another process Creating a trigger************/
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
      
     /***************************End*******************************/
 	     
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,1);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
     newtrig->SetId(triggerId);
      
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    TRAPD(error,lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue));
    if(error == KErrNotFound)
    {
    iLog->Log(_L("After UpdateTriggerL "));
    
   
    
   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 	
    }
    else
    {
    	iLog->Log(_L("After UpdateTriggerL "));
    
       
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
      
    }
    
    
         //Updating state of all the triggers owned by the client synchronously- TC_LBT012_11
      TInt CUpdateTrigger::TCLBTFW011_testL( CStifItemParser& /* aItem */ )
    {

 	RLbtServer lbtserver;
    RLbt lbt;
    iLog->Log(_L("Before Connect"));
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    iLog->Log(_L("Before Open"));
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 iLog->Log(_L("Before Create Trigger"));
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
    iLog->Log(_L("Trigger Created"));
        
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
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    //For reproducing crash for loop added
    for(int i=0;i<15;i++)  
    {
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );	
    }
        
        
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue);   
    
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateDisabled,ELbtTrue);  
    
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
    
    
         //Updating state of  the triggers filtered based on attribute filter - TC_LBT012_12
    TInt CUpdateTrigger::TCLBTFW012_testL( CStifItemParser& /* aItem */ )
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
    
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
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
    
    
         //Updating state of  the triggers filtered based on area filter - TC_LBT012_13
      TInt CUpdateTrigger::TCLBTFW013_testL( CStifItemParser& /* aItem */ )
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
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
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
      TInt CUpdateTrigger::TCLBTFW014_testL( CStifItemParser& /* aItem */ )
    {
	RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	TRAP_IGNORE( lbt.DeleteTriggersL() );
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
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
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
    
    
         //Updating state of all the triggers owned by the client asynchronously - TC_LBT012_15
      TInt CUpdateTrigger::TCLBTFW015_testL( CStifItemParser& /* aItem */ )
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
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
   
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    // CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
   // CleanupStack::PushL( notifier );
    
   // CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait,ELbtTrue);
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
    
    //Updating state of a trigger - TC_LBT012_16
      TInt CUpdateTrigger::TCLBTFW016_testL( CStifItemParser& /* aItem */ )
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
    trig->SetState(CLbtTriggerEntry::EStateDisabled);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
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
      
     /* 1)Update trigger with FireOnupdate = ELbtTrue for startup trigger and set state to Enabled(coordinate based)
      2)Update trigger with FireOnupdate = ELbtTrue for startup trigger and set state to Disabled(coordinate based)
      3)Update trigger with FireOnupdate = ELbtFalse for startup trigger and set state to Enabled(coordinate based)
      4)Update trigger with FireOnupdate = ELbtFalse for startup trigger and set state to Disabled(coordinate based)*/
      
     TInt CUpdateTrigger::TestFireOnUpdate1L( CStifItemParser&  aItem  )
     {
  	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 TInt value ;
  	 aItem.GetNextInt(value);
  	 
  	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
	 
  	 iLog->Log(_L("Before Connect "));
  	 
  	 User::LeaveIfError( lbtserver.Connect() );
  	 iLog->Log(_L("After Connect "));
      CleanupClosePushL( lbtserver );
      
  	 User::LeaveIfError( lbt.Open( lbtserver ) );
  	 CleanupClosePushL( lbt );
  	 iLog->Log(_L("After RLbt::Open "));
  	
  	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
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
     
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
     CleanupStack::PushL( notifier );
     
     CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
      TLbtTriggerId trigId,trigId2;   
     notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
     wait->Start( );
     User::After(5000000);
     
    //**************New Trigger Entry**************/ 
     CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
     newtrig->SetNameL(_L("TriggerTest"));
     if(value == 1 || value ==3)
     {
     newtrig->SetState(CLbtTriggerEntry::EStateEnabled);
     }
     else if(value == 2 || value ==4)
     {
     newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
     }
     newtrig->SetCommandLineL(_L("Commandline2" ));
    
     newtrig->SetId(trigId);
    
     /***************************End*******************/
     iLog->Log(_L("Before UpdateTriggerL "));
     if(value == 1 || value ==2)
    {	 
     lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
     |CLbtTriggerEntry::EAttributeState
     |CLbtTriggerEntry::EAttributeStartUpCommandLine,ELbtTrue
     );
    }
     else if(value == 3 || value ==4)
    {	 
     lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
     |CLbtTriggerEntry::EAttributeState
     |CLbtTriggerEntry::EAttributeStartUpCommandLine,ELbtFalse
     );
    }
    // lbt.UpdateTriggerL(*newtrig);
     iLog->Log(_L("After UpdateTriggerL "));
     if(value==1)
     {
     	notifier->StartNotification( wait );
  		wait->Start( );
     }
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
      
      
      /* 
	1)Update trigger with FireOnupdate = ELbtTrue for session trigger and set state to Enabled(coordinate based)
	2)Update trigger with FireOnupdate = ELbtTrue for session trigger and set state to Disabled(coordinate based)
	3)Update trigger with FireOnupdate = ELbtFalse for session trigger and set state to Enabled(coordinate based)
	4)Update trigger with FireOnupdate = ELbtFalse for session trigger and set state to Disabled(coordinate based)*/
      TInt CUpdateTrigger::TestFireOnUpdate2L( CStifItemParser&  aItem  )
     {
  	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 TInt value ;
  	 aItem.GetNextInt(value);
  	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
  	 
  	 iLog->Log(_L("Before Connect "));
  	 User::LeaveIfError( lbtserver.Connect() );
  	 iLog->Log(_L("After Connect "));
      CleanupClosePushL( lbtserver );
      
  	 User::LeaveIfError( lbt.Open( lbtserver ) );
  	 CleanupClosePushL( lbt );
  	 iLog->Log(_L("After RLbt::Open "));
  	
  	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
  //	  #if 0
 	  //Construct a startup trigger
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
     trig->SetManagerUi(aManagerUi);   
     
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
     CleanupStack::PushL( notifier );
     
     CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
      TLbtTriggerId trigId,trigId2;   
     notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
     wait->Start( );
     User::After(5000000);
     
    //**************New Trigger Entry**************/ 
     CLbtSessionTrigger* newtrig = CLbtSessionTrigger::NewL();
     newtrig->SetNameL(_L("TriggerTest"));
     if(value == 1 || value ==3)
     {
     newtrig->SetState(CLbtTriggerEntry::EStateEnabled);
     }
     else if(value == 2 || value ==4)
     {
     newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
     }
         
     newtrig->SetId(trigId);
    
     /***************************End*******************/
     iLog->Log(_L("Before UpdateTriggerL "));
     if(value == 1 || value ==2)
    {	 
     lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
     |CLbtTriggerEntry::EAttributeState
     ,ELbtTrue
     );
    }
     else if(value == 3 || value ==4)
    {	 
     lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
     |CLbtTriggerEntry::EAttributeState
     ,ELbtFalse
     );
    }
    // lbt.UpdateTriggerL(*newtrig);
     iLog->Log(_L("After UpdateTriggerL "));
     if(value ==1)
     {
     	notifier->StartNotification( wait );
  		wait->Start( );
     }
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
    /*
    9)SetTriggerState to Enabled with FireOnupdate = ELbtTrue --for startup trigger(coordinate based)
	10)SetTriggerState to Disabled with FireOnupdate = ELbtTrue --for startup trigger(coordinate based)
	11)SetTriggerState to Enabled with FireOnupdate = ELbtFalse --for startup trigger(coordinate based)
	12)SetTriggerState to Disabled with FireOnupdate = ELbtFalse --for startup trigger(coordinate based)
    */  
      
    TInt CUpdateTrigger::TestFireOnUpdate3L( CStifItemParser&  aItem  )
    {
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TInt value ;
 	 aItem.GetNextInt(value);
 	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
 	 
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
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
    trig->SetState(CLbtTriggerEntry::EStateDisabled);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
 //   User::After(5000000);
	   if(value ==1) 
	   {	   
	   lbt.SetTriggerStateL (trigId,CLbtTriggerEntry::EStateEnabled,ELbtTrue);
	   }
	   else if(value ==2) 
	   {	   
	   lbt.SetTriggerStateL (trigId,CLbtTriggerEntry::EStateDisabled,ELbtTrue);
	   }
	   else if(value ==3) 
	   {	   
	   lbt.SetTriggerStateL (trigId,CLbtTriggerEntry::EStateEnabled,ELbtFalse);
	   }
	   else if(value ==4) 
	   {	   
	   lbt.SetTriggerStateL (trigId,CLbtTriggerEntry::EStateDisabled,ELbtFalse);
	   }
   CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    if(TrigEntry->State()==CLbtTriggerEntry::EStateEnabled &&(value ==1 || value==3))
    {
    if(value ==1)
     {
     	notifier->StartNotification( wait );
  		wait->Start( );
     }
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone; 	
    }
    else if (TrigEntry->State()==CLbtTriggerEntry::EStateDisabled &&(value ==2 || value==4))
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
    
    /*
    13)SetTriggerState to Enabled with FireOnupdate = ELbtTrue --for session trigger(coordinate based)
	14)SetTriggerState to Disabled  with FireOnupdate = ELbtTrue --for session trigger(coordinate based)
	15)SetTriggerState  to Enabled with FireOnupdate = ELbtFalse --for session trigger(coordinate based)
	16)SetTriggerState  to Disabled with FireOnupdate = ELbtFalse --for session trigger(coordinate based)
    */
    
    TInt CUpdateTrigger::TestFireOnUpdate4L( CStifItemParser&  aItem  )
    {
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TInt value ;
 	 aItem.GetNextInt(value);
 	  EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
 	 
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 	TRAP_IGNORE(lbt.DeleteTriggersL()) ;        
	  //Construct a startup trigger
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
    trig->SetState(CLbtTriggerEntry::EStateDisabled);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
 //   User::After(5000000);
   if(value ==1) 
   {	   
   lbt.SetTriggerStateL (trigId,CLbtTriggerEntry::EStateEnabled,ELbtTrue);
   }
   else if(value ==2) 
   {	   
   lbt.SetTriggerStateL (trigId,CLbtTriggerEntry::EStateDisabled,ELbtTrue);
   }
   else if(value ==3) 
   {	   
   lbt.SetTriggerStateL (trigId,CLbtTriggerEntry::EStateEnabled,ELbtFalse);
   }
   else if(value ==4) 
   {	   
   lbt.SetTriggerStateL (trigId,CLbtTriggerEntry::EStateDisabled,ELbtFalse);
   }
   CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    if(TrigEntry->State()==CLbtTriggerEntry::EStateEnabled &&(value ==1 || value==3))
    {
    if(value ==1)
     {
     	notifier->StartNotification( wait );
  		wait->Start( );
     }
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone; 	
    }
    else if (TrigEntry->State()==CLbtTriggerEntry::EStateDisabled &&(value ==2 || value==4))
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
    /*
    17)SetTriggersStateL  with FireOnupdate = ELbtTrue and setstate as enabled
	18)SetTriggersStateL  with FireOnupdate = ELbtTrue set state as disabled
	19)SetTriggersStateL   with FireOnupdate = ELbtFalse and setstate as enabled
	20)SetTriggersStateL  with FireOnupdate = ELbtFalse and set state as disabled
    */  
    
    TInt CUpdateTrigger::TestFireOnUpdate5L( CStifItemParser&  aItem  )
    {
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TInt value ;
 	 aItem.GetNextInt(value);
 	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
 	 
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;     
	  //Construct a startup trigger
    CLbtStartupTrigger* startuptrig = CLbtStartupTrigger::NewL();
    //Construct a session trigger
    CLbtSessionTrigger* sessiontrig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( startuptrig );
    CleanupStack::PushL( sessiontrig );
    
    // Set Name
    startuptrig->SetNameL(_L("Trigger1"));
    sessiontrig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
    TSecureId secureid;
    startuptrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    TCoordinate coordinate1(62.5285,23.9385);
    TCoordinate coordinate2(62.5285,23.9385);
    
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate1,1000);
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate2,1000);
    CleanupStack::PushL( circle1 );
    CleanupStack::PushL( circle2 );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
                                                
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle2 );
    CleanupStack::Pop( circle1 );
    
    startuptrig->SetCondition(condition1); // ownership transferred to object
    sessiontrig->SetCondition(condition2); 
	//Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
	sessiontrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
    startuptrig->SetManagerUi(aManagerUi);   
    sessiontrig->SetManagerUi(aManagerUi);   
    startuptrig->SetState(CLbtTriggerEntry::EStateDisabled);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate1 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*startuptrig,trigId,EFalse,wait );
    wait->Start( );
    notifier->CreateTriggers( lbt,*sessiontrig,trigId,EFalse,wait );
    wait->Start( );
 //   User::After(5000000);
	   if(value ==1) 
	   {	   
	   lbt.SetTriggersStateL (CLbtTriggerEntry::EStateEnabled,ELbtTrue);
	   }
	   else if(value ==2) 
	   {	   
	   lbt.SetTriggersStateL (CLbtTriggerEntry::EStateDisabled,ELbtTrue);
	   }
	   else if(value ==3) 
	   {	   
	   lbt.SetTriggersStateL (CLbtTriggerEntry::EStateEnabled,ELbtFalse);
	   }
	   else if(value ==4) 
	   {	   
	   lbt.SetTriggersStateL (CLbtTriggerEntry::EStateDisabled,ELbtFalse);
	   }
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    if(TrigEntry->State()==CLbtTriggerEntry::EStateEnabled &&(value ==1 || value==3))
    {
    if(value ==1)
     {
     	notifier->StartNotification( wait );
  		wait->Start( );
     }
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( startuptrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone; 	
    }
    else if (TrigEntry->State()==CLbtTriggerEntry::EStateDisabled &&(value ==2 || value==4))
    {
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( startuptrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone; 	
    }
    else
    {
    	
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( startuptrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return -99; 	
    }
    
   }  
   
   /*
    21)SetTriggersState asynchronous with FireOnupdate = ELbtTrue and setstate as enabled
	22)SetTriggersState asynchronous with FireOnupdate = ELbtTrue set state as disabled
	23)SetTriggersState asynchronous with FireOnupdate = ELbtFalse and setstate as enabled
	24)SetTriggersState asynchronous with FireOnupdate = ELbtFalse and set state as disabled
   */
   
   TInt CUpdateTrigger::TestFireOnUpdate6L( CStifItemParser&  aItem  )
    {
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 TInt value ;
 	 aItem.GetNextInt(value);
 	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
 	 
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;    
	  //Construct a startup trigger
    CLbtStartupTrigger* startuptrig = CLbtStartupTrigger::NewL();
    //Construct a session trigger
    CLbtSessionTrigger* sessiontrig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( startuptrig );
    CleanupStack::PushL( sessiontrig );
    
    // Set Name
    startuptrig->SetNameL(_L("Trigger1"));
    sessiontrig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
    TSecureId secureid;
    startuptrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    TCoordinate coordinate1(62.5285,23.9385);
    TCoordinate coordinate2(62.5285,23.9385);
    
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate1,1000);
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate2,1000);
    CleanupStack::PushL( circle1 );
    CleanupStack::PushL( circle2 );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
                                                
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle2 );
    CleanupStack::Pop( circle1 );
    
    startuptrig->SetCondition(condition1); // ownership transferred to object
    sessiontrig->SetCondition(condition2); 
	//Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
	sessiontrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId1,trigId2;
    startuptrig->SetManagerUi(aManagerUi);   
    sessiontrig->SetManagerUi(aManagerUi);   
    startuptrig->SetState(CLbtTriggerEntry::EStateDisabled);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate1);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*startuptrig,trigId1,EFalse,wait );
    wait->Start( );
    notifier->CreateTriggers( lbt,*sessiontrig,trigId2,EFalse,wait );
    wait->Start( );
 //   User::After(5000000);
	   if(value ==1) 
	   {	   
	   notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait,ELbtTrue);
	   }
	   else if(value ==2) 
	   {	   
	   notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateDisabled,wait,ELbtTrue);
	   }
	   else if(value ==3) 
	   {	   
	   notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait,ELbtFalse);
	   }
	   else if(value ==4) 
	   {	   
	   notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateDisabled,wait,ELbtFalse);
	   }
	wait->Start( );   
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId1);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    if(TrigEntry->State()==CLbtTriggerEntry::EStateEnabled &&(value ==1 || value==3))
    {
    if(value ==1)
     {
     	notifier->StartNotification( wait );
  		wait->Start( );
     }
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( startuptrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone; 	
    }
    else if (TrigEntry->State()==CLbtTriggerEntry::EStateDisabled &&(value ==2 || value==4))
    {
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( startuptrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone; 	
    }
    else
    {
    	
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
   CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( startuptrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return -99; 	
    }
    
   }  
   
  //25)Update trigger with FireOnupdate = ELbtTrue but Triggering machanism state is OFF 
   TInt CUpdateTrigger::TestFireOnUpdate7L( CStifItemParser&  aItem  )
     {
  	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 
  	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
	 
  	 iLog->Log(_L("Before Connect "));
  	 
  	 User::LeaveIfError( lbtserver.Connect() );
  	 iLog->Log(_L("After Connect "));
      CleanupClosePushL( lbtserver );
      
  	 User::LeaveIfError( lbt.Open( lbtserver ) );
  	 CleanupClosePushL( lbt );
  	 iLog->Log(_L("After RLbt::Open "));
  	
  	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
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
     
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
     CleanupStack::PushL( notifier );
     
     CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
      TLbtTriggerId trigId,trigId2;   
     notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
     wait->Start( );
     User::After(5000000);
     
    //**************New Trigger Entry**************/ 
     CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
     newtrig->SetNameL(_L("TriggerTest"));
     
     newtrig->SetState(CLbtTriggerEntry::EStateEnabled);
     
     newtrig->SetCommandLineL(_L("Commandline2" ));
    
     newtrig->SetId(trigId);
    
     /***************************End*******************/
     iLog->Log(_L("Before UpdateTriggerL "));
    	 
     lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
     |CLbtTriggerEntry::EAttributeState
     |CLbtTriggerEntry::EAttributeStartUpCommandLine,ELbtTrue
     );
   
    // lbt.UpdateTriggerL(*newtrig);
     iLog->Log(_L("After UpdateTriggerL "));
     
     	notifier->StartNotification( wait );
     	notifier->After(5000000);
  		wait->Start( );
     
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
     
    //26)SetTriggerState  with FireOnupdate = ELbtTrue but Triggering machanism state is OFF 
    TInt CUpdateTrigger::TestFireOnUpdate8L( CStifItemParser&  aItem  )
    {
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
 	 
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
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
    trig->SetState(CLbtTriggerEntry::EStateDisabled);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
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
    if(TrigEntry->State()==CLbtTriggerEntry::EStateEnabled )
    {
    
     	notifier->StartNotification( wait );
     	notifier->After(5000000);
  		wait->Start( );
    
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
   //27)SetTriggersStateL   with FireOnupdate = ELbtTrue but Triggering machanism state is OFF
   TInt CUpdateTrigger::TestFireOnUpdate9L( CStifItemParser&  aItem  )
    {
     RLbtServer lbtserver;
 	 RLbt lbt;
 	
 	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
 	 
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 	TRAP_IGNORE(lbt.DeleteTriggersL()) ;        
	  //Construct a startup trigger
    CLbtStartupTrigger* startuptrig = CLbtStartupTrigger::NewL();
    //Construct a session trigger
    CLbtSessionTrigger* sessiontrig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( startuptrig );
    CleanupStack::PushL( sessiontrig );
    
    // Set Name
    startuptrig->SetNameL(_L("Trigger1"));
    sessiontrig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
    TSecureId secureid;
    startuptrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    TCoordinate coordinate1(62.5285,23.9385);
    TCoordinate coordinate2(62.5285,23.9385);
    
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate1,1000);
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate2,1000);
    CleanupStack::PushL( circle1 );
    CleanupStack::PushL( circle2 );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
                                                
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle2 );
    CleanupStack::Pop( circle1 );
    
    startuptrig->SetCondition(condition1); // ownership transferred to object
    sessiontrig->SetCondition(condition2); 
	//Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
	sessiontrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
    startuptrig->SetManagerUi(aManagerUi);   
    sessiontrig->SetManagerUi(aManagerUi);   
    startuptrig->SetState(CLbtTriggerEntry::EStateDisabled);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate1 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*startuptrig,trigId,EFalse,wait );
    wait->Start( );
    notifier->CreateTriggers( lbt,*sessiontrig,trigId,EFalse,wait );
    wait->Start( );
 	   
	lbt.SetTriggersStateL (CLbtTriggerEntry::EStateEnabled,ELbtTrue);
	   
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    if(TrigEntry->State()==CLbtTriggerEntry::EStateEnabled )
    {
    
    notifier->StartNotification( wait );
    notifier->After(5000000);
  	wait->Start( );
    
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( startuptrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone; 	
    }
   
    else
    {
    	
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( startuptrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return -99; 	
    }
    
   }  
   
   //28)SetTriggersState asynchronous   with FireOnupdate = ELbtTrue but Triggering machanism state is OFF
    TInt CUpdateTrigger::TestFireOnUpdate10L( CStifItemParser&  aItem  )
    {
     RLbtServer lbtserver;
 	 RLbt lbt;
 	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
 	 
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	
 	TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	  //Construct a startup trigger
    CLbtStartupTrigger* startuptrig = CLbtStartupTrigger::NewL();
    //Construct a session trigger
    CLbtSessionTrigger* sessiontrig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( startuptrig );
    CleanupStack::PushL( sessiontrig );
    
    // Set Name
    startuptrig->SetNameL(_L("Trigger1"));
    sessiontrig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
    TSecureId secureid;
    startuptrig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    TCoordinate coordinate1(62.5285,23.9385);
    TCoordinate coordinate2(62.5285,23.9385);
    
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate1,1000);
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate2,1000);
    CleanupStack::PushL( circle1 );
    CleanupStack::PushL( circle2 );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnEnter);
                                                
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle2 );
    CleanupStack::Pop( circle1 );
    
    startuptrig->SetCondition(condition1); // ownership transferred to object
    sessiontrig->SetCondition(condition2); 
	//Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	startuptrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
	sessiontrig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId;
    startuptrig->SetManagerUi(aManagerUi);   
    sessiontrig->SetManagerUi(aManagerUi);   
    startuptrig->SetState(CLbtTriggerEntry::EStateDisabled);    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate1 );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*startuptrig,trigId,EFalse,wait );
    wait->Start( );
    notifier->CreateTriggers( lbt,*sessiontrig,trigId,EFalse,wait );
    wait->Start( );
 //   User::After(5000000);
	  	   
	notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait,ELbtTrue);
	wait->Start( );   
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    if(TrigEntry->State()==CLbtTriggerEntry::EStateEnabled )
    {
    
    notifier->StartNotification( wait );
    notifier->After(5000000);
  	wait->Start( );
    
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( startuptrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone; 	
    }
   
    else
    {
    	
    CleanupStack::PopAndDestroy( 1 );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( sessiontrig );
    CleanupStack::PopAndDestroy( startuptrig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return -99; 	
    }
    
   }  
   
	/*   
	29)Check FireOnUpdate for Entry type trigger
	30)Check FireOnUpdate for Exit type trigger*/
    TInt CUpdateTrigger::TestFireOnUpdate11L( CStifItemParser&  aItem  )
     {
  	 RLbtServer lbtserver;
  	 RLbt lbt;
  	 TInt value ;
  	 aItem.GetNextInt(value);
  	 
  	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
	 
  	 iLog->Log(_L("Before Connect "));
  	 
  	 User::LeaveIfError( lbtserver.Connect() );
  	 iLog->Log(_L("After Connect "));
      CleanupClosePushL( lbtserver );
      
  	 User::LeaveIfError( lbt.Open( lbtserver ) );
  	 CleanupClosePushL( lbt );
  	 iLog->Log(_L("After RLbt::Open "));
  	
  	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
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
     CLbtTriggerConditionArea* condition = NULL;  
     // ownership of circle object transferred to the condition object
     if(value == 1)
     {
      condition=CLbtTriggerConditionArea::NewL(
                                                 circle,
                                                 CLbtTriggerConditionArea::EFireOnEnter);	
     }
     else if(value == 2)
     {
      condition=CLbtTriggerConditionArea::NewL(
                                             circle,
                                             CLbtTriggerConditionArea::EFireOnExit);	
     }
     
         
     CleanupStack::Pop( circle );
     
     trig->SetCondition(condition); // ownership transferred to object

     //Set Requestor
  	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
 	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
 	TBuf<KLbtMaxNameLength> ReqData=_L("");
 	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
     trig->SetManagerUi(aManagerUi);   
     
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
     CleanupStack::PushL( notifier );
     
     CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
      TLbtTriggerId trigId,trigId2;   
     
     notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
     wait->Start( );
     User::After(5000000);
     
    //**************New Trigger Entry**************/ 
     CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
     newtrig->SetNameL(_L("TriggerTest"));
    
     newtrig->SetState(CLbtTriggerEntry::EStateEnabled);
     
     newtrig->SetCommandLineL(_L("Commandline2" ));
    
     newtrig->SetId(trigId);
    
     /***************************End*******************/
     iLog->Log(_L("Before UpdateTriggerL "));
     if(value == 1 || value ==2)
    {	 
     lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
     |CLbtTriggerEntry::EAttributeState
     |CLbtTriggerEntry::EAttributeStartUpCommandLine,ELbtTrue
     );
    }
   
    // lbt.UpdateTriggerL(*newtrig);
     iLog->Log(_L("After UpdateTriggerL "));
     
     notifier->StartNotification( wait );
  	 wait->Start( );
    
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
     
   /*  31)Update trigger with FireOnupdate = ELbtTrue for startup trigger(cell based)
	32)Update trigger with FireOnupdate = ELbtFalse for startup trigger(cell based)*/
     TInt CUpdateTrigger::TestFireOnUpdate12L( CStifItemParser&  aItem  )
     {
  	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 TInt value ;
  	 aItem.GetNextInt(value);
  	 
  	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
	 
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
     
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 iLog->Log(_L("After RLbt::Open "));
 	TCoordinate coordinate(62.5285,23.9385);
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
     
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );   
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    // set condition
    
    RFileLogger aLog;
    CLbtAppCgiRequestor* CGIRequest=CLbtAppCgiRequestor::NewL(aLog);
    CleanupStack::PushL( CGIRequest );  
    CGIRequest->Start(wait);
    wait->Start( );
 		CTelephony::TNetworkInfoV1Pckg currcgi=CGIRequest->CurrentCGI();
     
 
 		CLbtGeoCell* Cellarea2= NULL;
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
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
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
    trig->SetManagerUi(aManagerUi);   
        
       
     TLbtTriggerId trigId,trigId2;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateEnabled);
    
//	trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    if(value ==1)
    {
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    ,ELbtTrue);	
    }
    
    else if(value ==2)
    {
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    ,ELbtFalse);	
    }
    
   // lbt.UpdateTriggerL(*newtrig);
    iLog->Log(_L("After UpdateTriggerL "));
    
    if(value == 1)
    {
    	notifier->StartNotification( wait );
  		wait->Start( );
    }
    
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
    
    //}
    //Check trigger Id
    trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
    	return -99;
    }
    
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( CGIRequest );
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
     
 /*    33)Update trigger with FireOnupdate = ELbtTrue for session trigger( cell based)
34)Update trigger with FireOnupdate = ELbtFalse for session trigger( cell based)*/
     TInt CUpdateTrigger::TestFireOnUpdate13L( CStifItemParser&  aItem  )
     {
  	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 TInt value ;
  	 aItem.GetNextInt(value);
  	 
  	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
	 
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect() );
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
     
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 iLog->Log(_L("After RLbt::Open "));
 	 
 	 TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
 	 TCoordinate coordinate(62.5285,23.9385);
 //	  #if 0
	  //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
    // set condition
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    
    RFileLogger aLog;
    CLbtAppCgiRequestor* CGIRequest=CLbtAppCgiRequestor::NewL(aLog);
    CleanupStack::PushL( CGIRequest );  
    CGIRequest->Start(wait);
    wait->Start( );
 		CTelephony::TNetworkInfoV1Pckg currcgi=CGIRequest->CurrentCGI();
     
 
 		CLbtGeoCell* Cellarea2 = NULL;
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
    
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
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
    trig->SetManagerUi(aManagerUi);   
        
    
     TLbtTriggerId trigId,trigId2;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtSessionTrigger* newtrig = CLbtSessionTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateEnabled);
    
//	trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    if(value ==1)
    {
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    ,ELbtTrue);	
    }
    
    else if(value ==2)
    {
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    ,ELbtFalse);	
    }
    
   // lbt.UpdateTriggerL(*newtrig);
    iLog->Log(_L("After UpdateTriggerL "));
    
    if(value == 1)
    {
    	notifier->StartNotification( wait );
  		wait->Start( );
    }
    
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
    
    //}
    //Check trigger Id
    trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
    	return -99;
    }
    
    CleanupStack::PopAndDestroy( 1 );//Triginfo
	CleanupStack::PopAndDestroy( CGIRequest );
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
     
     /*37)SetTriggerState  with FireOnupdate = ELbtTrue --for session trigger(cell based)
     38)SetTriggerState  with FireOnupdate = ELbtFalse --for session trigger(cell based)*/
     TInt CUpdateTrigger::TestFireOnUpdate14L( CStifItemParser&  aItem  )
     {
	 RLbtServer lbtserver;
	 RLbt lbt;
    
     TInt value ;
  	 aItem.GetNextInt(value);
  	 
  	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
	 
    iLog->Log(_L("Before Connect"));
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    iLog->Log(_L("Before Open"));
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
     	  //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    TCoordinate coordinate(62.5285,23.9385);
    // set condition
    
     
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    
    RFileLogger aLog;
    CLbtAppCgiRequestor* CGIRequest=CLbtAppCgiRequestor::NewL(aLog);
    CleanupStack::PushL( CGIRequest );  
    CGIRequest->Start(wait);
    wait->Start( );
 		CTelephony::TNetworkInfoV1Pckg currcgi=CGIRequest->CurrentCGI();
     
 
 		CLbtGeoCell* Cellarea2 = NULL;
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
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea2,
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
   
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );    
    if(value ==1)
    {
    	lbt.SetTriggerStateL(trigId,CLbtTriggerEntry::EStateEnabled,ELbtTrue);    
    }
    
    if(value ==2)
    {
    	lbt.SetTriggerStateL(trigId,CLbtTriggerEntry::EStateEnabled,ELbtFalse);    
    }
    
    if(value == 1)
    {
    	notifier->StartNotification( wait );
  		wait->Start( );
    }
      
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    iLog->Log(_L("Before GetTriggersL"));
    lbt.GetTriggersL(trigInfoList);
    iLog->Log(_L("After GetTriggersL"));
    
    //Do something with the trigger list
   
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    	if(CLbtTriggerEntry::EStateEnabled!=(TrigEntry->State()) )
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
	CleanupStack::PopAndDestroy( CGIRequest );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
	CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&lbtserver);
    return KErrNone;  
     }    
     
  /*  35)SetTriggerState  with FireOnupdate = ELbtTrue --for startup trigger( cell based)
    36)SetTriggerState  with FireOnupdate = ELbtFalse --for startup trigger(cell based) */
    TInt CUpdateTrigger::TestFireOnUpdate15L( CStifItemParser&  aItem  )
     {
	 RLbtServer lbtserver;
	 RLbt lbt;
    
     TInt value ;
  	 aItem.GetNextInt(value);
  	 
  	 EnableSimPSYL();
 	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
 	 iLog->Log(_L("Simulation PSY Repository object created"));
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 iLog->Log(_L("Simulation input file set "));
	 CleanupStack::PopAndDestroy(repository); 
	 
    iLog->Log(_L("Before Connect"));
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    iLog->Log(_L("Before Open"));
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	TCoordinate coordinate(62.5285,23.9385);        
    TRAP_IGNORE(lbt.DeleteTriggersL()) ;       
     	  //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    // set condition
    
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    
    RFileLogger aLog;
    CLbtAppCgiRequestor* CGIRequest=CLbtAppCgiRequestor::NewL(aLog);
    CleanupStack::PushL( CGIRequest );  
    CGIRequest->Start(wait);
    wait->Start( );
 		CTelephony::TNetworkInfoV1Pckg currcgi=CGIRequest->CurrentCGI();
     
 
 		CLbtGeoCell* Cellarea2 = NULL;
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
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Cellarea2,
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
    
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );    
    if(value ==1)
    {
    	lbt.SetTriggerStateL(trigId,CLbtTriggerEntry::EStateEnabled,ELbtTrue);    
    }
    
    if(value ==2)
    {
    	lbt.SetTriggerStateL(trigId,CLbtTriggerEntry::EStateEnabled,ELbtFalse);    
    }
    
    if(value == 1)
    {
    	notifier->StartNotification( wait );
  		wait->Start( );
    }
      
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    iLog->Log(_L("Before GetTriggersL"));
    lbt.GetTriggersL(trigInfoList);
    iLog->Log(_L("After GetTriggersL"));
    
    //Do something with the trigger list
   
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    	CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    	if(CLbtTriggerEntry::EStateEnabled!=(TrigEntry->State()) )
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
	CleanupStack::PopAndDestroy( CGIRequest );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
	CleanupStack::PopAndDestroy(&lbt);
    CleanupStack::PopAndDestroy(&lbtserver);
    return KErrNone;  
     }  
     
     
     
    TInt CUpdateTrigger::TestAsyncUpdate1L( CStifItemParser& /* aItem */ )
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
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
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
    
    
      //Updating attributes of a session trigger - TC_LBT012_02
     TInt CUpdateTrigger::TestAsyncUpdate2L( CStifItemParser& /* aItem */ )
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
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object
	//Set Requestor
 	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
    TLbtTriggerId trigId,trigId2;
    trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
 //   User::After(5000000);
    
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

    newtrig->SetId(trigId);
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,wait,ELbtTrue);
    wait->Start( );
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
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
    //Updating Nonmodifiable attributes of a startup trigger - TC_LBT012_03
    TInt CUpdateTrigger::TestAsyncUpdate3L( CStifItemParser& /* aItem */ )
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
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeRequestor
    |CLbtTriggerEntry::EAttributeManagerUi
    |CLbtTriggerEntry::EAttributeStartUpProcessId,wait,ELbtTrue
    );
    wait->Start( );
    if(notifier->iStatus.Int()==KErrAccessDenied)
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
    
    
      //Trying to update Nonmodifiable attributes of session trigger - TC_LBT012_04
     TInt CUpdateTrigger::TestAsyncUpdate4L( CStifItemParser& /* aItem */ )
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
 //   User::After(5000000);
    
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
    
    newtrig->SetManagerUi(TUid::Uid(0x12345678));

    newtrig->SetId(trigId);
    
    newtrig->SetRequestorL(CRequestor::ERequestorContact,CRequestor::EFormatTelephone,_L("Test2") );
      
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeRequestor
    |CLbtTriggerEntry::EAttributeManagerUi
    |CLbtTriggerEntry::EAttributeStartUpProcessId,wait,ELbtTrue
    );
    wait->Start();
    if(notifier->iStatus.Int()==KErrAccessDenied)
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
      
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
      
      
    }
    
    
     //Trying to update trigger with zero length name - TC_LBT012_05
      TInt CUpdateTrigger::TestAsyncUpdate5L( CStifItemParser& /* aItem */ )
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
 //   User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L(""));
    
    newtrig->SetId(trigId);
    
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName,wait,ELbtTrue);
    wait->Start();
    if(notifier->iStatus.Int() == KErrArgument)
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
    
   
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
    
      
    }
    
    
     //Trying to update trigger without defining the trigger area- TC_LBT012_06
      TInt CUpdateTrigger::TestAsyncUpdate6L( CStifItemParser& /* aItem */ )
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
 //   User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    
       
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL();
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
    
    newtrig->SetId(trigId);
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
   
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,wait,ELbtTrue);
    wait->Start();
    if(notifier->iStatus.Int() == KErrArgument)
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
    
   
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
      
    }
    
    
     //Trying to update trigger with trigger area less than minimum size of trigger area - TC_LBT012_07
      TInt CUpdateTrigger::TestAsyncUpdate7L( CStifItemParser& /* aItem */ )
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
 //   User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,0);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
    newtrig->SetId(trigId);
      
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,wait,ELbtTrue);
    wait->Start();
    if(notifier->iStatus.Int() == KErrTriggerAreaTooSmall)
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
    
   
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
      
    }
    
    
      //Trying to update trigger without opening the subsession - TC_LBT012_08
      TInt CUpdateTrigger::TestAsyncUpdate8L( CStifItemParser& /* aItem */ )
    {
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 iLog->Log(_L("Before Connect "));
 	 User::LeaveIfError( lbtserver.Connect());
 	 iLog->Log(_L("After Connect "));
     CleanupClosePushL( lbtserver );
 /*	 TCoordinate coordinate(62.5285,23.9385);
 	  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier ); */
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;   
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,1);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
      
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    TRequestStatus reqtatus;
    lbt.UpdateTrigger(*newtrig,
    CLbtTriggerEntry::EAttributeCondition,ELbtTrue,reqtatus);
    iLog->Log(_L("After UpdateTriggerL "));
   // CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
    
      //Trying to update trigger when it is not available - TC_LBT012_09
      TInt CUpdateTrigger::TestAsyncUpdate9L( CStifItemParser& /* aItem */ )
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
 	 	 TCoordinate coordinate(62.5285,23.9385);
 	  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier ); 
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;   
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,1);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
     newtrig->SetId(99999999);
      
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,wait,ELbtTrue);
    wait->Start();
    if(notifier->iStatus.Int() == KErrNotFound)
    {
    iLog->Log(_L("After UpdateTriggerL "));
    
   
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 	
    }
    else
    {
    	iLog->Log(_L("After UpdateTriggerL "));
    
      
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
      
    }
    
    
        //Trying to update trigger owned by another process - TC_LBT012_10
      TInt CUpdateTrigger::TestAsyncUpdate10L( CStifItemParser& /* aItem */ )
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
     /***************Another process Creating a trigger************/
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
      
     /***************************End*******************************/
 	 	 TCoordinate coordinate(62.5285,23.9385);
 	  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier ); 
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;       
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,1);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
     newtrig->SetId(triggerId);
      
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,wait,ELbtTrue);
    wait->Start();
    if(notifier->iStatus.Int() == KErrNotFound)
    {
    iLog->Log(_L("After UpdateTriggerL "));
    
   
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 	
    }
    else
    {
    	iLog->Log(_L("After UpdateTriggerL "));
    
    CleanupStack::PopAndDestroy( notifier );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // delete wait;
    iLog->Log(_L("Test passed "));
    return -99; 
    }
      
    }
