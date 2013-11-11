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
* Description:    Test module to implement Test cases found during review
*
*/





// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "Additionaltests.h"
#include "AsynOperationAO.h"
#include <centralrepository.h>
#include <lbs/SimulationPSYInternalCRKeys.h>


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
//#include <lbtclientrequester.h>  


#include <lbtmanager.h> 

// Location aquisition API 

#include <lbscommon.h>
#include <LbsRequestor.h>
#include <LbsPosition.h>
#include <LbsPositionInfo.h>
#include <lbs.h>
//#include <epossimulationpsy.hrh>

#include <e32property.h>


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
// CAdditionaltests::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CAdditionaltests::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CAdditionaltests::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CAdditionaltests::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
      //  ENTRY( "Example", CAdditionaltests::ExampleL ),
         ENTRY( "TestGetTriggers1L", CAdditionaltests::TestGetTriggers1L ),
         ENTRY( "TestGetTriggers2L", CAdditionaltests::TestGetTriggers2L ),
         ENTRY( "TestGetTriggers3L", CAdditionaltests::TestGetTriggers3L ),
         ENTRY( "TestAsynchronousGetTriggers1L", CAdditionaltests::TestAsynchronousGetTriggers1L ),
         ENTRY( "TestAsynchronousGetTriggers2L", CAdditionaltests::TestAsynchronousGetTriggers2L ),
         ENTRY( "TestAsynchronousGetTriggers3L", CAdditionaltests::TestAsynchronousGetTriggers3L ),
         ENTRY( "TestAsynchronousGetTriggers4L", CAdditionaltests::TestAsynchronousGetTriggers4L ),
         ENTRY( "TestListTriggerIds1L", CAdditionaltests::TestListTriggerIds1L ),
         ENTRY( "TestSortingL", CAdditionaltests::TestSortingL ),
         ENTRY( "TestAttributefilterL", CAdditionaltests::TestAttributefilterL ),
         ENTRY( "TestCreateGetTriggerIteratorL", CAdditionaltests::TestCreateGetTriggerIteratorL ),
         ENTRY( "TestAttributefilter2L", CAdditionaltests::TestAttributefilter2L ),
         ENTRY( "TestUpdateTrigger1L", CAdditionaltests::TestUpdateTrigger1L ),
         ENTRY( "TestSetTriggersState1L", CAdditionaltests::TestSetTriggersState1L ),
         ENTRY( "TestSetTriggersState2L", CAdditionaltests::TestSetTriggersState2L ),
         ENTRY( "TestSetTriggersState3L", CAdditionaltests::TestSetTriggersState3L ),
         ENTRY( "TestSetTriggersState4L", CAdditionaltests::TestSetTriggersState4L ),
         ENTRY( "TestSetTriggersState5L", CAdditionaltests::TestSetTriggersState5L ),
         ENTRY( "TestSetTriggersState6L", CAdditionaltests::TestSetTriggersState6L ),
         ENTRY( "TestdeleteTrigger1L", CAdditionaltests::TestdeleteTrigger1L ),
         ENTRY( "TestdeleteTrigger2L", CAdditionaltests::TestdeleteTrigger2L ),
         ENTRY( "TestdeleteTrigger3L", CAdditionaltests::TestdeleteTrigger3L ),
         ENTRY( "TestdeleteTrigger4L", CAdditionaltests::TestdeleteTrigger4L ),
         ENTRY( "TestdeleteTrigger5L", CAdditionaltests::TestdeleteTrigger5L ),
         ENTRY( "TestFilterbyAttribute3L", CAdditionaltests::TestFilterbyAttribute1L ),
         ENTRY( "TestFilterbyAttribute4L", CAdditionaltests::TestFilterbyAttribute2L ),
         ENTRY( "TestFilterbyAttribute5L", CAdditionaltests::TestFilterbyAttribute3L ),
         ENTRY( "TestClientRequestorL", CAdditionaltests::TestClientRequestorL ),
         ENTRY( "TestNotifyTriggerChangeEventL", CAdditionaltests::TestNotifyTriggerChangeEventL ),
         ENTRY( "TestNotifyTriggerChangeEvent2L", CAdditionaltests::TestNotifyTriggerChangeEvent2L ),
         ENTRY( "TestNotifyTriggerChangeEvent3L", CAdditionaltests::TestNotifyTriggerChangeEvent3L ),
         ENTRY( "TestNotifyTriggerChangeEvent4L", CAdditionaltests::TestNotifyTriggerChangeEvent4L ),
         ENTRY( "TestManagerUI1L", CAdditionaltests::TestManagerUI1L ),
         ENTRY( "TestManagerUI2L", CAdditionaltests::TestManagerUI2L ),
         ENTRY( "TestManagerUI3L", CAdditionaltests::TestManagerUI3L ),
         ENTRY( "TestTriggerHandlingproc1L", CAdditionaltests::TestTriggerHandlingproc1L ),
         ENTRY( "TestOpenL", CAdditionaltests::TestOpenL ),
         ENTRY( "TestUpdateTrigger2L", CAdditionaltests::TestUpdateTrigger2L ),
         ENTRY( "TestUpdateTrigger3L", CAdditionaltests::TestUpdateTrigger3L ),
         ENTRY( "TestUpdateTrigger4L", CAdditionaltests::TestUpdateTrigger4L ),
         ENTRY( "TestUpdateTrigger5L", CAdditionaltests::TestUpdateTrigger5L ),
         ENTRY( "TestUpdateTrigger6L", CAdditionaltests::TestUpdateTrigger6L ),
         ENTRY( "TestListtriggerIds1L", CAdditionaltests::TestListtriggerIds1L ),
         ENTRY( "TestListtriggerIds2L", CAdditionaltests::TestListtriggerIds2L ),
         ENTRY( "TestNotifyTriggeringsettingsChangeEvent1L", CAdditionaltests::TestNotifyTriggeringsettingsChangeEvent1L ),
         ENTRY( "TestTriggeringsettingsChangeEventDestructorL", CAdditionaltests::TestTriggeringsettingsChangeEventDestructorL ),
         ENTRY( "TestCancelAllL", CAdditionaltests::TestCancelAllL ),
         ENTRY( "TestCLbtGeoCircle1L", CAdditionaltests::TestCLbtGeoCircle1L ),
         ENTRY( "TestCLbtGeoCircle2L", CAdditionaltests::TestCLbtGeoCircle2L ),
         ENTRY( "TestCLbtGeoRect1L", CAdditionaltests::TestCLbtGeoRect1L ),
         ENTRY( "TestCLbtGeoRect2L", CAdditionaltests::TestCLbtGeoRect2L ),
         ENTRY( "TestCLbtGeoRect3L", CAdditionaltests::TestCLbtGeoRect3L ),
         ENTRY( "TestCLbtSessionTriggerL", CAdditionaltests::TestCLbtSessionTriggerL ),
         ENTRY( "TestCLbtStartupTriggerL", CAdditionaltests::TestCLbtStartupTriggerL ),
         ENTRY( "TestCLbtTriggerChangeEventNotifierL", CAdditionaltests::TestCLbtTriggerChangeEventNotifierL ),
         ENTRY( "TestCLbtTriggerInfoL", CAdditionaltests::TestCLbtTriggerInfoL ),
         
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


void CAdditionaltests::AssertTrueL( TBool aCondition,const TDesC& aMessage )
    {
           
         if(!aCondition)
         {
          
    	   iLog->Log( aMessage );
           User::LeaveIfError(KErrGeneral);
         }
    
    }

// -----------------------------------------------------------------------------
// CAdditionaltests::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAdditionaltests::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KAdditionaltests, "Additionaltests" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KAdditionaltests, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KAdditionaltests, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }
    //Call RLbt::GetTriggers using area filter and the area is not a type of geographical circular area
    TInt CAdditionaltests::TestGetTriggers1L( CStifItemParser& /*aItem*/ )
    {

    RLbtServer lbtserver;
    RLbt lbt;
    iLog->Log(_L("Before connecting"));
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    iLog->Log(_L("Before Open"));
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 iLog->Log(_L("Before DeleteTriggersL"));
    TRAP_IGNORE(lbt.DeleteTriggersL());
    
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
     User::After(5000000);
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter );
    CLbtGeoRect* area = CLbtGeoRect::NewL( 20,70,20,70);
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
    
    //Test case to test gettriggers using composite filter containing two area filters.
     TInt CAdditionaltests::TestGetTriggers2L( CStifItemParser& /* aItem */ )
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
   
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter1 = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter1 );
    CLbtGeoCircle* area1 = CLbtGeoCircle::NewL( 
        TCoordinate( 62.5285,23.9385 ),
        1000 );
    areaFilter1->SetArea( area1 ); 
        
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter2 = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter2 );
    CLbtGeoCircle* area2 = CLbtGeoCircle::NewL( 
        TCoordinate( 62.5285,23.9385 ),
        1000 );
    areaFilter2->SetArea( area2 ); 
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areaFilter1 ); 
    
    
    compositeFilter->AddFilterL( areaFilter2 ); 
    
    
    listOptions->SetFilter( compositeFilter ); 
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( areaFilter2 ); 
    CleanupStack::Pop( areaFilter1 );     
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
   //Check condition
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
	if(distance>10)
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
    
    //Test case to test  gettriggers using composite filter containing two attribute filters.
      TInt CAdditionaltests::TestGetTriggers3L( CStifItemParser& /* aItem */ )
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
   
    //construct attribute filter
    CLbtTriggerFilterByAttribute* attribFilter1 = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter1 );
 //   attribFilter1->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter1->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
        
     //construct attribute filter
    CLbtTriggerFilterByAttribute* attribFilter2 = 
    CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( attribFilter2 );
    attribFilter2->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
   // attribFilter2->AddTriggerTypeL( CLbtTriggerEntry::ETypeStartup );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter1 ); 
    
    
    compositeFilter->AddFilterL( attribFilter2 ); 
    
    
    listOptions->SetFilter( compositeFilter ); 
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( attribFilter2 ); 
    CleanupStack::Pop( attribFilter1 );     
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
    
    //Call asynchronous RLbt::GetTriggers() using attribute filter
    TInt CAdditionaltests::TestAsynchronousGetTriggers1L( CStifItemParser& /* aItem */ )
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
       
	//CleanupStack::PopAndDestroy(4, &lbtserver);

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
    TInt CAdditionaltests::TestAsynchronousGetTriggers2L( CStifItemParser& /* aItem */ )
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
    
 //Call asynchronous RLbt::GetTriggers() using composite filter
  
     TInt CAdditionaltests::TestAsynchronousGetTriggers3L( CStifItemParser& /* aItem */ )
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
    
        
     notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();    
        
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
    
    
     //Listing of triggers without opening the subsession
   
    TInt CAdditionaltests::TestAsynchronousGetTriggers4L( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
   // CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
   // CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
  //  CleanupStack::PushL( notifier );
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
   TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
   TRequestStatus aStatus=KRequestPending;
   lbt.GetTriggers(aStatus,trigInfoList,listOptions);
   //wait->Start();    
   CleanupStack::PopAndDestroy( listOptions );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
   
    }
    
    //Listing of trigger ids without opening the subsession
    TInt CAdditionaltests::TestListTriggerIds1L( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
    	 
   
    RArray< TLbtTriggerId>  aTriggerIdList;
    
        
   //  CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
   // CleanupStack::PushL( notifier );
    
  //  CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
   TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
   TRequestStatus aStatus=KRequestPending;
    lbt.ListTriggerIds(aStatus,aTriggerIdList,listOptions);
   // wait->Start();
    //Do something with the trigger list
      
   
    aTriggerIdList.Close();
  
   	CleanupStack::PopAndDestroy( listOptions );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
   
      
    }
    
    //Testing sorting based on a field that was not requested while calling Get triggers . ( in this case , the sorting will not take place )
    TInt CAdditionaltests::TestSortingL( CStifItemParser& /* aItem */ )
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
    
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
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
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
   	coordinate.Move(90,50000);
   	CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate,1000);
   	circle2->SetCenter(coordinate);
   	CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
   	trig->SetCondition(condition2);
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    	coordinate.Move(90,4000);
    	CLbtGeoCircle* circle3=CLbtGeoCircle::NewL(coordinate,1000);
   	circle3->SetCenter(coordinate);
   	CLbtTriggerConditionArea* condition3=CLbtTriggerConditionArea::NewL(
                                                circle3,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    	
   	trig->SetCondition(condition3);
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    
    //Construct list options
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    listOptions->SetRetrievedFields( 
        CLbtTriggerEntry::EAttributeName|TLbtTriggerDynamicInfo::EDistanceToLatestLocation );   
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
    
    
    //Create an attribute filter with a trigger id which is not present in the system. Now do filter by attribute. This scenario must be handled correctly.
    TInt CAdditionaltests::TestAttributefilterL( CStifItemParser& /* aItem */ )
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
    attribFilter->AddTriggerIdL(99999999);
   // attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
   // attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    listOptions->SetFilter( attribFilter ); 
    
    CleanupStack::Pop( attribFilter );   
    
    
    //lbt.GetTriggersL(trigInfoList,listOptions );
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();    
       
	if(trigInfoList.Count()==0)
	{
		trigInfoList.Close();
		CleanupStack::PopAndDestroy( listOptions );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
     return KErrNone; 
	}
    else
    {
    	return -99;
    }
    }
    
    //Call CreateGetTriggerIteratorL without opening subsession.
    TInt CAdditionaltests::TestCreateGetTriggerIteratorL( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
 	    //First create iterator
    lbt.CreateGetTriggerIteratorL();
    
    CleanupStack::PopAndDestroy( &lbtserver );
      return KErrNone;
    }
    
    
    
    //Listing of all the valid and active triggers
    TInt CAdditionaltests::TestAttributefilter2L( CStifItemParser& /* aItem */ )
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
   // attribFilter->AddTriggerIdL(99999999);
    attribFilter->AddTriggerValidityL(TLbtTriggerDynamicInfo::EValid);
   
   // attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
   // attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    listOptions->SetFilter( attribFilter ); 
    
    CleanupStack::Pop( attribFilter );   
    
    
    //lbt.GetTriggersL(trigInfoList,listOptions );
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();    
       
	if(trigInfoList.Count()>=0)
	{
		trigInfoList.Close();
		CleanupStack::PopAndDestroy( listOptions );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
     return KErrNone; 
	}
    else
    {
        trigInfoList.Close();
		CleanupStack::PopAndDestroy( listOptions );
	    CleanupStack::PopAndDestroy( notifier );
	    CleanupStack::PopAndDestroy( trig );
	    CleanupStack::PopAndDestroy( &lbt );
	    CleanupStack::PopAndDestroy( &lbtserver );
    	return -99;
    }
    }
    
    
    //Call UpdateTriggerL() without opening the subsession 
    TInt CAdditionaltests::TestUpdateTrigger1L( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
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
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
 	    
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
    
    CleanupStack::PopAndDestroy( &lbtserver );
      return KErrNone;
    }
    
    
    
    //Call SetTriggersStateL() without opening the subsession 
    TInt CAdditionaltests::TestSetTriggersState1L( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
   
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
 	    
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue);
    
    CleanupStack::PopAndDestroy( &lbtserver );
      return KErrNone;
    }
    
    
    //Call SetTriggersState() without opening the subsession 
    TInt CAdditionaltests::TestSetTriggersState2L( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    
   //	CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
   // CleanupStack::PushL( notifier );
    
   // CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
 	TRequestStatus aStatus=KRequestPending;    
   lbt.SetTriggersState(aStatus,CLbtTriggerEntry::EStateEnabled,ELbtTrue,NULL);
   // wait->Start();
    
  
 //  	CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
   }
   
    
         //Updating state of  the triggers filtered based on attribute filter - 
    TInt CAdditionaltests::TestSetTriggersState3L( CStifItemParser& /* aItem */ )
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
    
    
         //Updating state of  the triggers filtered based on area filter - TC_LBT012_13
      TInt CAdditionaltests::TestSetTriggersState4L( CStifItemParser& /* aItem */ )
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
    
    
         //Updating state of  the triggers filtered based on composite filter - TC_LBT012_14
      TInt CAdditionaltests::TestSetTriggersState5L( CStifItemParser& /* aItem */ )
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
    
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait,compositeFilter);  
    wait->Start();
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
    
    //updating trigger state when none of the filter criteria is satisfied.
     TInt CAdditionaltests::TestSetTriggersState6L( CStifItemParser& /* aItem */ )
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
    attribFilter->AddTriggerIdL(99999);
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,attribFilter);   
    
    CleanupStack::Pop( attribFilter );   
    
    
    lbt.GetTriggersL(trigInfoList);    
       

    //Do something with the trigger list
    TInt count = trigInfoList.Count();
    if(count >0)
    {
    		CLbtTriggerEntry *TrigEntry =trigInfoList[0]->TriggerEntry();
	        
	     //Check type of the trigger
	    TInt Type2 = TrigEntry ->Type();
	    if(Type2 ==CLbtTriggerEntry::ETypeSession)
	    {
		    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
		    if(State2 !=CLbtTriggerEntry::EStateEnabled)
		    {
		    	
		    }
		    else
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
    
    
    //delete triggers based on rectangular area filter(synchronous)
    TInt CAdditionaltests::TestdeleteTrigger1L( CStifItemParser& /* aItem */ )
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
        
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
          
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
    CLbtGeoRect* area2 = CLbtGeoRect::NewL( 0,50,0,70);
    areaFilter->SetArea( area2 ); 
    
   
    lbt.DeleteTriggersL(areaFilter);
    
   //CLbtTriggerInfo* trigEntry =NULL;
    CLbtTriggerInfo* trigEntry=lbt.GetTriggerLC(trigId1);
    
       
        
    CleanupStack::PopAndDestroy( areaFilter );
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig1 );   
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
  
   // delete wait; 
    return KErrNone;
    
    }
    
    
     //delete triggers based on rectangular area filter(asynchronous)
    TInt CAdditionaltests::TestdeleteTrigger2L( CStifItemParser& /* aItem */ )
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
        
    trig1->SetProcessId(_L("LbtReference.exe"),KNullUid );   
    
          
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
    CLbtGeoRect* area2 = CLbtGeoRect::NewL( 0,50,0,70);
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
    
    
    
    //Test case to test deletetriggers using composite filter containing two area filters.
     TInt CAdditionaltests::TestdeleteTrigger3L( CStifItemParser& /* aItem */ )
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

    TLbtTriggerId trigId1,trigId2;
    trigId2 =0;
     trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId1,EFalse,wait );
    wait->Start( );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter1 = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter1 );
    CLbtGeoCircle* area1 = CLbtGeoCircle::NewL( 
        TCoordinate( 62.5285,23.9385,0 ),
        1000 );
    areaFilter1->SetArea( area1 ); 
        
    //Construct an area filter
    CLbtTriggerFilterByArea* areaFilter2 = 
        CLbtTriggerFilterByArea::NewL();
    CleanupStack::PushL( areaFilter2 );
    CLbtGeoCircle* area2 = CLbtGeoCircle::NewL( 
        TCoordinate( 62.5285,23.9385,0 ),
        1000 );
    areaFilter2->SetArea( area2 ); 
    //set conditon for second trigger
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(TCoordinate( 62.5285,23.9385,0 ),1000);
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
    //condition2->SetTriggerArea(circle2);
    trig->SetCondition(condition2); 
    
    //create second trigger
    notifier->CreateTriggers( lbt,*trig,trigId1,EFalse,wait );
    wait->Start( );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areaFilter1 ); 
    
    
    compositeFilter->AddFilterL( areaFilter2 ); 
    
    lbt.DeleteTriggersL(compositeFilter);
    TInt error2;
//    CLbtTriggerInfo* trigEntry=NULL;
    TRAPD(error1, lbt.GetTriggerLC(trigId1);TRAP(error2, lbt.GetTriggerLC(trigId2);
    CleanupStack::Pop( );););
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( areaFilter2 ); 
    CleanupStack::Pop( areaFilter1 );     

    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   if(error1==KErrNotFound)
   {
   
   if(error2==KErrNotFound)
	   {
    
 
  // delete wait; 
   return KErrNone;
	   }
   else
   {
	// delete wait; 
      return -99;
   }
   }
   else
   {
   
  // delete wait; 
   return -99;	
   }
   
    
      
    }
    
//Deleting a trigger from another client  - leave with -1
	TInt CAdditionaltests::TestdeleteTrigger4L( CStifItemParser& /* aItem */ )
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
    
  
  
   CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbtmanager);
   CleanupStack::PushL( notifier );
   
   CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       
   TLbtTriggerId trigId;
   notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
   wait->Start( );
   
   //lbtmanager.DeleteTriggerL(trigId);
   lbt.DeleteTriggerL(triggerId);
   CleanupStack::PopAndDestroy(notifier);
    CleanupStack::PopAndDestroy(trig);
   TInt error2;
//   CLbtTriggerInfo* trigEntry=NULL;
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId);TRAP(error2, lbtmanager.GetTriggerLC(triggerId);
   CleanupStack::PopAndDestroy();););
   CleanupStack::PopAndDestroy( &lbt );
   CleanupStack::PopAndDestroy( &lbtmanager );
   CleanupStack::PopAndDestroy( &lbtserver );
   if(error1==KErrNotFound)
   {
   
   if(error2==KErrNotFound)
	   {
	  
 
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
   return -99;	
   }
  }
  
  
  
  //Deletin triggers from other client
  TInt CAdditionaltests::TestdeleteTrigger5L( CStifItemParser& /* aItem */ )
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
    
  
  
   CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
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
   TRAPD(error1, lbtmanager.GetTriggerLC(trigId); CleanupStack::PopAndDestroy( ););
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
  
  //Test public methods of CLbtTriggerFilterbyAttribute 1
  TInt CAdditionaltests::TestFilterbyAttribute1L( CStifItemParser& /* aItem */ )
    {

    
    CLbtTriggerFilterByAttribute* filter1=CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( filter1 );
    filter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    filter1->AddTriggerManagerUiL( TUid::Uid(5) );
    filter1->AddTriggerValidityL(TLbtTriggerDynamicInfo::EInvalid) ;
		filter1->AddTriggerIdL (1) ;   
		filter1->AddTriggerIdL (2) ;   
        
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    filter1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtTriggerFilterByAttribute* filter2=CLbtTriggerFilterByAttribute::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( filter2 ); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    filter2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    TBool result=filter2->IsTriggerTypeInFilter(CLbtTriggerEntry::ETypeSession);
    AssertTrueL(result==1 ,_L("Type is  in filter")); 
    result=filter2->IsTriggerStateInFilter(CLbtTriggerEntry::EStateDisabled);
    AssertTrueL(result==1 ,_L("state is  in filter"));
    result=filter2->IsTriggerManagerUiInFilter( TUid::Uid(5) );
    AssertTrueL(result==1 ,_L("Manager ui is  in filter"));  
    result=filter2->IsTriggerValidityStatusInFilter( TLbtTriggerDynamicInfo::EInvalid );
    AssertTrueL(result==1 ,_L("Validity status in filter"));  
		result=filter2->IsTriggerIdInFilter ( 2);
    AssertTrueL(result==1 ,_L("trigger id in filter"));      
		result=filter2->IsTriggerIdInFilter ( 1);
    AssertTrueL(result==1 ,_L("trigger id in filter"));      
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }
    //  //Test public methods of CLbtTriggerFilterbyAttribute 2
    TInt CAdditionaltests::TestFilterbyAttribute2L( CStifItemParser& /* aItem */ )
    {

    
    CLbtTriggerFilterByAttribute* filter1=CLbtTriggerFilterByAttribute::NewLC();
   // CleanupStack::PushL( filter1 );
    filter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    filter1->AddTriggerManagerUiL( TUid::Uid(5) );
    filter1->AddTriggerValidityL(TLbtTriggerDynamicInfo::EInvalid) ;
		filter1->AddTriggerIdL (1) ;   
		filter1->AddTriggerIdL (2) ;   
        
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    filter1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtTriggerFilterByAttribute* filter2=CLbtTriggerFilterByAttribute::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( filter2 ); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    filter2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    TBool result=filter2->IsTriggerTypeInFilter(CLbtTriggerEntry::ETypeSession);
    AssertTrueL(result==1 ,_L("Type is  in filter")); 
    result=filter2->IsTriggerStateInFilter(CLbtTriggerEntry::EStateDisabled);
    AssertTrueL(result==1 ,_L("state is  in filter"));
    result=filter2->IsTriggerManagerUiInFilter( TUid::Uid(5) );
    AssertTrueL(result==1 ,_L("Manager ui is  in filter"));  
    result=filter2->IsTriggerValidityStatusInFilter( TLbtTriggerDynamicInfo::EInvalid );
    AssertTrueL(result==1 ,_L("Validity status in filter"));  
		result=filter2->IsTriggerIdInFilter ( 2);
    AssertTrueL(result==1 ,_L("trigger id in filter"));      
		result=filter2->IsTriggerIdInFilter ( 1);
    AssertTrueL(result==1 ,_L("trigger id in filter"));      
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }
    //  //Test public methods of CLbtTriggerFilterbyAttribute 3
    TInt CAdditionaltests::TestFilterbyAttribute3L( CStifItemParser& /* aItem */ )
    {

    
    CLbtTriggerFilterByAttribute* filter1=CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( filter1 );
    filter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    filter1->AddTriggerManagerUiL( TUid::Uid(5) );
    filter1->AddTriggerValidityL(TLbtTriggerDynamicInfo::EInvalid) ;
		filter1->AddTriggerIdL (1) ;   
		filter1->AddTriggerIdL (2) ;   
        
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    filter1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtTriggerFilterByAttribute* filter2=CLbtTriggerFilterByAttribute::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( filter2 ); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    filter2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    
    RArray< CLbtTriggerEntry::TLbtTriggerState > aTriggerStateArray;
    filter2->GetStateArrayL(aTriggerStateArray);
    AssertTrueL(aTriggerStateArray[0] == CLbtTriggerEntry::EStateDisabled ,_L("State in filter")); 
    
    
    RArray< CLbtTriggerEntry::TType > aTriggerTypeArray;
    filter2->GetTypeArrayL (aTriggerTypeArray);
    AssertTrueL(aTriggerTypeArray[0] == CLbtTriggerEntry::ETypeSession ,_L("trigger type in filter")); 
    
    RArray< TLbtTriggerDynamicInfo::TLbtTriggerValidity > aTriggerValidityArray;
    filter2->GetValidityArrayL (aTriggerValidityArray);
    AssertTrueL(aTriggerValidityArray[0] == TLbtTriggerDynamicInfo::EInvalid ,_L("validity type in filter")); 
    
    RArray< TUid > aManagerUiArray;
    filter2->GetManagerUiArrayL (aManagerUiArray);
    AssertTrueL(aManagerUiArray[0] == TUid::Uid(5) ,_L("managerUI in filter")); 
    
    RArray< TLbtTriggerId > aIdArray;
    filter2->GetTriggerIdArrayL (aIdArray);
    AssertTrueL(aIdArray[0] == 1 ,_L("triggerid in filter")); 
   
    
    CleanupStack::PopAndDestroy(3);
    return KErrNone;
    
    }
  
  
    TInt CAdditionaltests::TestClientRequestorL( CStifItemParser& /* aItem */ )
    {
      	
    /* RLbtServer lbtserver;
		 RLbt lbt,lbt2;
		 iLog->Log(_L("Before connecting"));
		 User::LeaveIfError( lbtserver.Connect() );
		 CleanupClosePushL( lbtserver );
		 iLog->Log(_L("Connection to RLbtServer Passed "));
		 User::LeaveIfError( lbt.Open(lbtserver));
		 CleanupClosePushL( lbt );
		 User::LeaveIfError( lbt2.Open(lbtserver));
		 iLog->Log(_L("Subsession opened "));
		 CleanupClosePushL( lbt2 );
	
	
	  //Construct a startup trigger

	
		CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
		CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
		
		CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
		CleanupStack::PushL( notifier );
		CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2);
		CleanupStack::PushL( notifier2 );
	
	  TCoordinate coordinate(12.9422763,77.6950090);
    
   	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,300);
    CleanupStack::PushL( circle );
    
         
  	// ownership of circle object transferred to the condition object
  	CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                              circle,
                                              CLbtTriggerConditionArea::EFireOnEnter);
      
  	CleanupStack::Pop( circle );
  	//New code start
  	    TSecureId secureid= 0x1333;
      CLbtTriggerEntry::TLbtTriggerState aState =CLbtTriggerEntry::EStateEnabled; 

    _LIT( KMyTriggerName, "Trigger name" );
    _LIT( KMyRequestor , "Requestor" );
    _LIT( KMyTriggerHandlingProcessName, "calendar.exe" );
    TUid aManagerUi;
    TBuf<256> triggerId(_L("test"));   
	 aManagerUi = TUid::Uid(0x87654321);
    
  	CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL( 
      KMyTriggerName,                             //Trigger name
      aState,       //Trigger state
      CRequestorBase::ERequestorService,          //Requestor type
      CRequestorBase::EFormatApplication,         //Requestor format
      KMyRequestor,                               //Requestor data
     // CLbtTriggerEntry::EPostFiringActionNoAction, //Trigger fired option
      aManagerUi,				                      //Manager UI
      condition,                                       //Trigger condition
      KMyTriggerHandlingProcessName,              //Handling process name
     // handlingUidType,                            //Handling UID type
     secureid,
      triggerId );   
        
        //New code end      
    
    //	trig->SetCondition(condition); // ownership transferred to object

	
	TLbtTriggerId trigId;
	
	       
	notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
	wait->Start( );
	CLbtClientRequester* requester = CLbtClientRequester::NewL  ( ) ;
	CleanupStack::PushL(requester);
	requester->SetStatus(KRequestPending);
	requester->Start();
   User::WaitForRequest(requester->Status());
   lbt.DeleteTriggerL(trigId);
   CleanupStack::PopAndDestroy( requester );   
  CleanupStack::PopAndDestroy( notifier2 );
  CleanupStack::PopAndDestroy( notifier );
	CleanupStack::PopAndDestroy( &lbt2 );
  CleanupStack::PopAndDestroy( &lbt );
  CleanupStack::PopAndDestroy( &lbtserver );
		//	//delete wait2;
		//	//delete wait;
			iLog->Log(_L("All popped out "));
	   iLog->Log(_L("Test case passed "));
	   */
   
          return KErrNone; 
        }  
    //Test trigger change notification when trigger is created from management client
    
    TInt CAdditionaltests::TestNotifyTriggerChangeEventL( CStifItemParser& /*aItem*/ )
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
        
        //Test trigger change notification when trigger is deleted
        TInt CAdditionaltests::TestNotifyTriggerChangeEvent2L( CStifItemParser& /*aItem*/ )
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
	    notifier1->NotifyTriggerChangeEvent2L(lbt1,wait1);
	    notifier2->After(10000);
	    wait2->Start( );
	    lbt2.DeleteTriggerL(trigId);
	    wait1->Start( );
	   	    
	    CleanupStack::PopAndDestroy(notifier2);
	   
	    	CleanupStack::PopAndDestroy(trig);
	    	CleanupStack::PopAndDestroy(notifier1);
		    //CleanupStack::PopAndDestroy(&mgrclient);
		    CleanupStack::PopAndDestroy(&lbt2);
		    CleanupStack::PopAndDestroy(&lbt1);
		    CleanupStack::PopAndDestroy(&server);
		 //   delete wait1;
		 //   delete wait2;
		    return KErrNone;
	    


        }
        //Test trigger change notification when new trigger is created
         TInt CAdditionaltests::TestNotifyTriggerChangeEvent3L( CStifItemParser& /*aItem*/ )
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
	    //TLbtTriggerChangeEvent   aEvent ;
	    
	  
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
	    /*notifier2->CreateTriggers( lbt2,*trig,trigId,ETrue,wait2 );
	    wait2->Start( );*/
	    notifier1->NotifyTriggerChangeEvent2L(lbt1,wait1);
	  //  notifier2->After(10000);
	  //  wait2->Start( );
	    notifier2->CreateTriggers( lbt2,*trig,trigId,ETrue,wait2 );
	    wait2->Start( );
	    
	    wait1->Start( );
	   
	    CleanupStack::PopAndDestroy(notifier2);
	   
	    	CleanupStack::PopAndDestroy(trig);
	    	CleanupStack::PopAndDestroy(notifier1);
		    //CleanupStack::PopAndDestroy(&mgrclient);
		    CleanupStack::PopAndDestroy(&lbt2);
		    CleanupStack::PopAndDestroy(&lbt1);
		    CleanupStack::PopAndDestroy(&server);
//		    delete wait1;
//		    delete wait2;
		    return KErrNone;
	    


        }
        
        //Test trigger change notification when trigger is modified
        TInt CAdditionaltests::TestNotifyTriggerChangeEvent4L( CStifItemParser& /*aItem*/ )
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
	  //  TLbtTriggerChangeEvent   aEvent ;
	    
	  
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
	    notifier1->NotifyTriggerChangeEvent2L(lbt1,wait1);
	    notifier2->After(10000);
	    wait2->Start( );
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
	   
	    /***************************End*******************/
	    iLog->Log(_L("Before UpdateTriggerL "));
	    lbt2.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
	    |CLbtTriggerEntry::EAttributeState
	    |CLbtTriggerEntry::EAttributeCondition
	    |CLbtTriggerEntry::EAttributeStartUpCommandLine,
	    ELbtTrue
	    );
	   // lbt.UpdateTriggerL(*newtrig);
	    iLog->Log(_L("After UpdateTriggerL "));
	    wait1->Start( );
	   
	    CleanupStack::PopAndDestroy(notifier2);
	   
	    	CleanupStack::PopAndDestroy(trig);
	    	CleanupStack::PopAndDestroy(notifier1);
		    //CleanupStack::PopAndDestroy(&mgrclient);
		    CleanupStack::PopAndDestroy(&lbt2);
		    CleanupStack::PopAndDestroy(&lbt1);
		    CleanupStack::PopAndDestroy(&server);
		 //   delete wait1;
		 //   delete wait2;
		    return KErrNone;
	    


        }
        
        //Trying to delete trigger from managerUI application.
        TInt CAdditionaltests::TestManagerUI1L( CStifItemParser& /*aItem*/ )
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
	    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
	    
	    TSecureId secureid;
	    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
	       
	    // set condition
	    
	    TCoordinate coordinate(62.5285,23.9385);
	    
	    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
	    CleanupStack::PushL( circle );
	    
	    TUid aManagerUi = TUid::Uid(0x2001FDE1);     
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
	     trig->SetManagerUi(aManagerUi);   
	        
	    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
	    CleanupStack::PushL( notifier );
	    
	    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	        
	    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
	    wait->Start( );	
	    RProperty property;
		CleanupClosePushL(property);
		
		//Allow all to ready from status information
		_LIT_SECURITY_POLICY_PASS(EReadPolicyAlwaysPass);
			
		// Read policy is always pass and write device data capability
		// is required to write to the status information P&S key
		property.Define(KPSUid,
										   KLbttriggerid,
										   RProperty::EInt,
										   EReadPolicyAlwaysPass,
										   TSecurityPolicy(ECapabilityWriteDeviceData) );
		

		User::LeaveIfError( property.Attach(KPSUid, 
											KLbttriggerid) );
											
		User::LeaveIfError( property.Set(KPSUid, 
										 KLbttriggerid, 
										 trigId));
		
		
	    CleanupStack::PopAndDestroy(5,&lbtserver);
	    
	    	_LIT( KFilename,"DeleteTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt result2;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        DeletetestUid, 
	        KLbttesttriggerid, 
	        result2));
	        
	    return result2;    
	     
        }
        
        
        //Trying to modify and access a trigger from managerUI application
        TInt CAdditionaltests::TestManagerUI2L( CStifItemParser& /*aItem*/ )
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
	    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
	    
	    TSecureId secureid;
	    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
	       
	    // set condition
	    
	    TCoordinate coordinate(62.5285,23.9385);
	    
	    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
	    CleanupStack::PushL( circle );
	    
	    TUid aManagerUi = TUid::Uid(0x2001FDDF);     
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
	     trig->SetManagerUi(aManagerUi);   
	        
	    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
	    CleanupStack::PushL( notifier );
	    
	    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
	        
	    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
	    wait->Start( );	
	    RProperty property;
		CleanupClosePushL(property);
		
		//Allow all to ready from status information
		_LIT_SECURITY_POLICY_PASS(EReadPolicyAlwaysPass);
			
		// Read policy is always pass and write device data capability
		// is required to write to the status information P&S key
		property.Define(KPSUid,
										   KLbttriggerid,
										   RProperty::EInt,
										   EReadPolicyAlwaysPass,
										   TSecurityPolicy(ECapabilityWriteDeviceData) );
		

		User::LeaveIfError( property.Attach(KPSUid, 
											KLbttriggerid) );
											
		User::LeaveIfError( property.Set(KPSUid, 
										 KLbttriggerid, 
										 trigId));
		
		
	    CleanupStack::PopAndDestroy(5,&lbtserver);
	    
	    _LIT( KFilename,"ModifyTest.exe" );
	    RProcess proc;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			TRequestStatus status = KRequestPending;
			proc.Rendezvous(status);
			User::WaitForRequest(status);
		}	
	    TInt result2=-1;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	            ModifytestUid, 
	        KLbttesttriggerid, 
	        result2));
	        
	    return result2;    
	     
        }
        
        
        // Testing notification to ManagerUI application
        TInt CAdditionaltests::TestManagerUI3L( CStifItemParser& /*aItem*/ )
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
	    TUid aManagerUi = TUid::Uid(0x2001FDDE);     
	    trig->SetManagerUi(aManagerUi);       
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
	    
	    //To invoke the managerUI
	    _LIT( KFilename,"NotificationTest.exe" );
	    RProcess proc;
	    TRequestStatus status = KRequestPending;
	    TInt retVal =proc.Create(KFilename,KNullDesC);
	    if(retVal == KErrNone)
		{
			proc.Resume();
			
			proc.Rendezvous(status);
		//	User::WaitForRequest(status);
		}	
	    /*TInt result2=-1;
	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        DeletetestUid, 
	        KLbttesttriggerid, 
	        result2));*/
	        
	    notifier2->After(50000000);
	    wait2->Start();
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
	   
	    /***************************End*******************/
	    iLog->Log(_L("Before UpdateTriggerL "));
	    lbt2.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
	    |CLbtTriggerEntry::EAttributeState
	    |CLbtTriggerEntry::EAttributeCondition
	    |CLbtTriggerEntry::EAttributeStartUpCommandLine,
	    ELbtTrue
	    );
	   // lbt.UpdateTriggerL(*newtrig);
	    iLog->Log(_L("After UpdateTriggerL "));
		User::WaitForRequest(status);
	  // notifier2->After(50000000);
	  //  wait2->Start();
	    CleanupStack::PopAndDestroy(notifier2);
	   
	    	CleanupStack::PopAndDestroy(trig);
	    	CleanupStack::PopAndDestroy(notifier1);
		    //CleanupStack::PopAndDestroy(&mgrclient);
		    CleanupStack::PopAndDestroy(&lbt2);
		    CleanupStack::PopAndDestroy(&lbt1);
		    CleanupStack::PopAndDestroy(&server);
		 //   delete wait1;
		 //   delete wait2;
		    return KErrNone;
	    


        }
        
        
        //Test accessing trigger from trigger handling process 1
        TInt CAdditionaltests::TestTriggerHandlingproc1L( CStifItemParser& /*aItem*/ )
        {

      		RLbtServer server;
	    RLbt lbt1,lbt2;
	 
	        
	    User::LeaveIfError(server.Connect());
	    CleanupClosePushL( server );
	    
	    User::LeaveIfError(lbt1.Open(server));
	    CleanupClosePushL( lbt1 );
	    
	    User::LeaveIfError(lbt2.Open(server));
	    CleanupClosePushL( lbt2 );
	     _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );

	    CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 	 iLog->Log(_L("Simulation PSY Repository object created"));
	//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
		 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
		 iLog->Log(_L("Simulation input file set "));
		 CleanupStack::PopAndDestroy(repository);
	    CLbtEventObserver* notifier1= CLbtEventObserver::NewL( lbt1);
	    CleanupStack::PushL( notifier1 );
	    //notifier->After(50000000); 
	    
	    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
	    TLbtTriggerChangeEvent   aEvent ;
	    
	  
	      //Construct a session trigger
	    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
	    
	    //Push to cleanup stack
	    CleanupStack::PushL( trig );
	    
	    // Set Name
	    trig->SetNameL(_L("Trigger1"));
	    
	    _LIT( KMyTriggerHandlingProcessName, "HandlingprocessTest.exe");
       
    	TSecureId secureid=0xE0000208;
    	trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);  
    	trig->SetCommandLineL (_L("1"));
    	
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
	    
	    RProperty property;
		CleanupClosePushL(property);
		
		//Allow all to ready from status information
		_LIT_SECURITY_POLICY_PASS(EReadPolicyAlwaysPass);
			
		// Read policy is always pass and write device data capability
		// is required to write to the status information P&S key
		property.Define(KPSUid,
										   KLbttriggerid,
										   RProperty::EInt,
										   EReadPolicyAlwaysPass,
										   TSecurityPolicy(ECapabilityWriteDeviceData) );
		

		User::LeaveIfError( property.Attach(KPSUid, 
											KLbttriggerid) );
											
		User::LeaveIfError( property.Set(KPSUid, 
										 KLbttriggerid, 
										 trigId));
	    notifier2->StartNotification(wait2);
	    wait2->Start();
	    notifier1->NotifyTriggerChangeEvent(lbt1,aEvent,wait1);
	    wait1->Start();
	    CleanupStack::PopAndDestroy();//property
	    CleanupStack::PopAndDestroy(notifier2);
	   
	    	CleanupStack::PopAndDestroy(trig);
	    	CleanupStack::PopAndDestroy(notifier1);
		    //CleanupStack::PopAndDestroy(&mgrclient);
		    CleanupStack::PopAndDestroy(&lbt2);
		    CleanupStack::PopAndDestroy(&lbt1);
		    CleanupStack::PopAndDestroy(&server);
		 //   delete wait1;
		 //   delete wait2;
//		   TInt result2=0;
/*	    RProperty iProperty;
	    User::LeaveIfError(iProperty.Get(
	        DeletetestUid, 
	        KLbttesttriggerid, 
	        result2));*/
	        
	    return KErrNone;;    
	    


        }
        
        //Test RLbt::Open()
          TInt CAdditionaltests::TestOpenL( CStifItemParser& /*aItem*/ )
        {

	    RLbt lbt;
	 	    
	    TInt result = lbt.Open();
	    
		return result;


        }
        
     //Trying to update trigger id   
    TInt CAdditionaltests::TestUpdateTrigger2L( CStifItemParser& /* aItem */ )
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
    |CLbtTriggerEntry::EAttributeCondition,
    ELbtTrue);
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
      return KErrNone;
    }
    
    
    //Trying to set condition when condition is NULL
    TInt CAdditionaltests::TestUpdateTrigger3L( CStifItemParser& /* aItem */ )
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
    |CLbtTriggerEntry::EAttributeCondition,
    ELbtTrue);
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
     TInt CAdditionaltests::TestUpdateTrigger4L( CStifItemParser& /* aItem */ )
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

    newtrig->SetCondition(newcondition); // ownership transferred to object
	  TLbtTriggerId trigId = 1;
    newtrig->SetId(trigId);
  
   /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition
    |CLbtTriggerEntry::EAttributeStartUpCommandLine,
    ELbtTrue
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
    TInt CAdditionaltests::TestUpdateTrigger5L( CStifItemParser& /* aItem */ )
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
    TInt CAdditionaltests::TestUpdateTrigger6L( CStifItemParser& /* aItem */ )
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
    
   
   //Listing of all the trigger Ids owned by the client synchronously using area filter
    TInt CAdditionaltests::TestListtriggerIds1L( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 
       _LIT( KFilename,"CreateTest.exe" );
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);
    TRequestStatus status = KRequestPending;
    TInt triggerId = KLbtNullTriggerId;
    if(retVal == KErrNone)
        {
        proc.Resume();
        proc.Rendezvous(status);
        User::WaitForRequest(status);
        triggerId = status.Int();
        }
    
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
       
    //Do something with the trigger list
    
    
    TInt count = aTriggerIdList.Count();
    
    for ( TInt i = 0; i < count; i++ )
     {
       	if(triggerId==aTriggerIdList[i])
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
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
      
    }
    
    //Listing of all the triggerIds owned by the client Asynchronously using area filter 
    TInt CAdditionaltests::TestListtriggerIds2L( CStifItemParser& /* aItem */ )
    {
    RLbtServer lbtserver;
    RLbt lbt;
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
 	 TCoordinate coordinate(65.5285,23.9385);
    	 
    _LIT( KFilename,"CreateTest.exe" );
    RProcess proc;
    TInt retVal =proc.Create(KFilename,KNullDesC);

    TRequestStatus status = KRequestPending;
    TInt triggerId = KLbtNullTriggerId;
    
    if(retVal == KErrNone)
        {
        proc.Resume();
        proc.Rendezvous(status);
        User::WaitForRequest(status);
        triggerId = status.Int();
        }
    
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
    
        
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait,listOptions);
    wait->Start();
    
   
    //Do something with the trigger list
    
    
    
    TInt count = aTriggerIdList.Count();
    
    for ( TInt i = 0; i < count; i++ )
     {
       	if(triggerId==aTriggerIdList[i])
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
      CleanupStack::PopAndDestroy( listOptions );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
	return KErrNone;
      
    } 
    
    
     //Test triggering settings change notification 
        TInt CAdditionaltests::TestNotifyTriggeringsettingsChangeEvent1L( CStifItemParser& /*aItem*/ )
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
	  //  TLbtTriggerChangeEvent   aEvent ;
	    
	  
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
	    
	    wait1->Start( );
	   	
	    CleanupStack::PopAndDestroy(notifier2);
	   
	    	CleanupStack::PopAndDestroy(trig);
	    	CleanupStack::PopAndDestroy(notifier1);
		    //CleanupStack::PopAndDestroy(&mgrclient);
		    CleanupStack::PopAndDestroy(&lbt2);
		    CleanupStack::PopAndDestroy(&lbt1);
		    CleanupStack::PopAndDestroy(&server);
		 //   delete wait1;
		 //   delete wait2;
		    return KErrNone;
	    


        }
        
         //Test CLbtTriggeringSystemSettingsChangeEventNotifier Destructor 
        TInt CAdditionaltests::TestTriggeringsettingsChangeEventDestructorL( CStifItemParser& /*aItem*/ )
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
	    
	   	notifier1->NotifyTriggeringsettingsChangeEventL(lbt1,wait1);
	   	lbt1.CancelNotifyTriggeringSystemSettingChange ();
	   	notifier1->Cancel();   
	   //	lbt1.CancelAll(); 
	   //	notifier1->TriggeringsettingsChangeEventDestructor();
	    CleanupStack::PopAndDestroy(notifier2);
	   
	    	CleanupStack::PopAndDestroy(trig);
	    	CleanupStack::PopAndDestroy(notifier1);
		    //CleanupStack::PopAndDestroy(&mgrclient);
		    CleanupStack::PopAndDestroy(&lbt2);
		    CleanupStack::PopAndDestroy(&lbt1);
		    CleanupStack::PopAndDestroy(&server);
		 //   delete wait1;
		 //   delete wait2;
		    return KErrNone;
	    

        }
        
  
  
      //Test TestCancelAllL  
        TInt CAdditionaltests::TestCancelAllL( CStifItemParser& /*aItem*/ )
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
	    //TLbtTriggerChangeEvent   aEvent ;
	    
	  
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
	    
	   	notifier2->CreateTriggers( lbt2,*trig,trigId,ETrue,wait2 );
	   	lbt2.CancelAll(); 
	   	TInt error=notifier2->iStatus.Int();    
	   		CleanupStack::PopAndDestroy(notifier2);
	    	
	    	CleanupStack::PopAndDestroy(trig);
	    	CleanupStack::PopAndDestroy(notifier1);
		    //CleanupStack::PopAndDestroy(&mgrclient);
		    CleanupStack::PopAndDestroy(&lbt2);
		    CleanupStack::PopAndDestroy(&lbt1);
		    CleanupStack::PopAndDestroy(&server);
	   	if(error==KErrCancel)
	    
		     return KErrNone;
		else
			 return -99; 
	    

        }
        
      //Test   CLbtGeoCircle::NewL()
    TInt CAdditionaltests::TestCLbtGeoCircle1L( CStifItemParser& /* aItem */ )
    {

    
    CLbtGeoCircle* circle = CLbtGeoCircle::NewL();
    
    TReal radius=circle->Radius();
    TCoordinate center=circle->Center();
    CleanupStack::PushL( circle );
    TReal value=center.Altitude();
    AssertTrueL(Math::IsNaN(value),_L("Altitude is a number"));
    value=center.Latitude();
    AssertTrueL(Math::IsNaN(value),_L("Latitude is a number"));
    value=center.Longitude();
    AssertTrueL(Math::IsNaN(value),_L("Longitude is a number"));
    
    AssertTrueL(radius==0,_L("Radius is not 0"));
    
        
    CleanupStack::PopAndDestroy( circle ); 
    
    return KErrNone;

    }
    
    
    //Test CLbtGeoCircle::NewLC(center,radius)      
    TInt CAdditionaltests::TestCLbtGeoCircle2L( CStifItemParser& /* aItem */ )
    {

    TCoordinate center(5,10,15);
    
    CLbtGeoCircle* circle = CLbtGeoCircle::NewLC(center,100);
    
            
    TReal radius=circle->Radius();
    TCoordinate center2=circle->Center();
    
    AssertTrueL(center2.Latitude()==5 && center2.Longitude()==10 && center2.Altitude()==15,_L("wrong centre"));
    AssertTrueL( radius==100,_L("Wrong Radius"));
        
    CleanupStack::PopAndDestroy( circle ); 
    
    return KErrNone;

    }
    
    //Test CLbtGeoRect::NewLC()
    TInt CAdditionaltests::TestCLbtGeoRect1L( CStifItemParser& /* aItem */ )
    {
    
    CLbtGeoRect* rect = CLbtGeoRect::NewLC();
    rect->SetBorder(10,20,30,40);
    TReal aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude ; 
    
    AssertTrueL(rect->Type()==CLbtGeoAreaBase::ERectangular,_L("Wrong Type"));   
    
    rect->GetBorder(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude );

    AssertTrueL(aSouthLatitude==10 && aNorthLatitude==20 && aWestLongitude==30 && aEastLongitude==40,_L("Borders are numbers"));   
    
    CleanupStack::PopAndDestroy( rect ); 
    
    return KErrNone;

    }
    
    //Test CLbtGeoRect::NewLC(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude)
     TInt CAdditionaltests::TestCLbtGeoRect2L( CStifItemParser& /* aItem */ )
    {
    
    CLbtGeoRect* rect = CLbtGeoRect::NewLC(10,20,30,40);
   
    TReal aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude ; 
    
    AssertTrueL(rect->Type()==CLbtGeoAreaBase::ERectangular,_L("Wrong Type"));   
    
    rect->GetBorder(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude );

    AssertTrueL(aSouthLatitude==10 && aNorthLatitude==20 && aWestLongitude==30 && aEastLongitude==40,_L("Borders are incorrect"));   
    
    CleanupStack::PopAndDestroy( rect ); 
    
    return KErrNone;

    }
    //Test CLbtGeoRect::NewL(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude)
      TInt CAdditionaltests::TestCLbtGeoRect3L( CStifItemParser& /* aItem */ )
    {
    
    CLbtGeoRect* rect = CLbtGeoRect::NewL(10,20,30,40);
    
    //Push to cleanup stack
    CleanupStack::PushL( rect );
    
    TReal aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude ; 
    
    AssertTrueL(rect->Type()==CLbtGeoAreaBase::ERectangular,_L("Wrong Type"));   
    
    rect->GetBorder(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude );

    AssertTrueL(aSouthLatitude==10 && aNorthLatitude==20 && aWestLongitude==30 && aEastLongitude==40,_L("Borders are incorrect"));   
    
    CleanupStack::PopAndDestroy( rect ); 
    
    return KErrNone;

    }
    
    //Test a version of CLbtSessionTrigger::NewL
     TInt CAdditionaltests::TestCLbtSessionTriggerL( CStifItemParser& /*aItem*/ )
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
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,10.1);
   							
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
//    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
	
	_LIT( KMyRequestorName, "Requestor" );
    CRequestor* req = CRequestor::NewLC( 
        CRequestorBase::ERequestorService,
        CRequestorBase::EFormatApplication,
        KMyRequestorName );
        
    //Append the requestor to the requestor stack
    RRequestorStack Reqstack;
    
    Reqstack.AppendL( req );
	
    //Construct a startup trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        Reqstack,
        aManagerUi,				                      //Manager UI
        cond                                       //Trigger condition
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
     
     CleanupStack::Pop( req );   
    //Pop trigger condition object
    CleanupStack::Pop( cond );
    
    //Push trigger object to cleanup stack
    CleanupStack::PushL( trig );
    TLbtTriggerId trigId;
   CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier2);
    notifier2->CreateTriggers( lbt,*trig,trigId,ETrue,wait2 );
    wait2->Start( );
    
   CleanupStack::PopAndDestroy(notifier2);
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
    lbt.Close();
    CleanupStack::PopAndDestroy(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;
    
    }
    
    //Test a version of CLbtStartupTrigger::NewL
    TInt CAdditionaltests::TestCLbtStartupTriggerL( CStifItemParser& /*aItem*/ )
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
    TRealX myradius =10; 
    TCoordinate centre(myLatitude,myLongitude)  ;     
    //Define the triggering area
        
   CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
   						centre,10.1);
   							
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
//    _LIT( KMyRequestor , "Requestor" );
   
   	TUid aManagerUi;
	aManagerUi = TUid::Uid(0x87654321);
	
	_LIT( KMyRequestorName, "Requestor" );
    CRequestor* req = CRequestor::NewLC( 
        CRequestorBase::ERequestorService,
        CRequestorBase::EFormatApplication,
        KMyRequestorName );
        
    //Append the requestor to the requestor stack
    RRequestorStack Reqstack;
    
    Reqstack.AppendL( req );
    
     _LIT( KMyTriggerHandlingProcessName, "About.exe");
     _LIT( KCommandline, "commandline");
       
    	TSecureId secureid;
	
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL( 
        KMyTriggerName,                             //Trigger name
        CLbtTriggerEntry::EStateEnabled,       //Trigger state
        Reqstack,
        aManagerUi,				                      //Manager UI
        cond,                                       //Trigger condition
        KMyTriggerHandlingProcessName,
        secureid,
        KCommandline
        );     
        
        iLog->Log(_L("Session trigger entry created ")); 	                         
     CleanupStack::Pop( req );    
    //Pop trigger condition object
    CleanupStack::Pop( cond );
    
    //Push trigger object to cleanup stack
    CleanupStack::PushL( trig );
    TLbtTriggerId trigId;
   CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier2);
    notifier2->CreateTriggers( lbt,*trig,trigId,ETrue,wait2 );
    wait2->Start( );
    
   CleanupStack::PopAndDestroy(notifier2);
    //Delete the instance of trigger object
   CleanupStack::PopAndDestroy( trig );
   
    
   /* User::WaitForRequest( status );
    User::LeaveIfError( status.Int() );*/
    
    //Close connection to Location Triggering Server.
        
    lbt.Close();
    CleanupStack::PopAndDestroy(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;
    
    }
    
       //Test CLbtTriggerChangeEventNotifier Destructor
        TInt CAdditionaltests::TestCLbtTriggerChangeEventNotifierL( CStifItemParser& /*aItem*/ )
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
	    //TLbtTriggerChangeEvent   aEvent ;
	    
	  
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
	    notifier1->NotifyTriggerChangeEvent2L(lbt1,wait1);
	    lbt1.CancelNotifyTriggerChangeEvent();
	   //	notifier1->TriggerChangeEventDestructor();    
	    CleanupStack::PopAndDestroy(notifier2);
	   
	    	CleanupStack::PopAndDestroy(trig);
	    	CleanupStack::PopAndDestroy(notifier1);
		    //CleanupStack::PopAndDestroy(&mgrclient);
		    CleanupStack::PopAndDestroy(&lbt2);
		    CleanupStack::PopAndDestroy(&lbt1);
		    CleanupStack::PopAndDestroy(&server);
		 //   delete wait1;
		 //   delete wait2;
		    return KErrNone;
	    


        }
        
     //Testing CLbtTriggerInfo::NewL() another version.   
    TInt CAdditionaltests::TestCLbtTriggerInfoL( CStifItemParser& /* aItem */ )
    {

	_LIT( KMyRequestorName, "Requestor" );


	TLbtTriggerDynamicInfo* dyninfo = new(ELeave) TLbtTriggerDynamicInfo;
	dyninfo->iValidity= TLbtTriggerDynamicInfo::EInvalid;
//	dyninfo->iActivityStatus= TLbtTriggerDynamicInfo::EInactive;
     
    
	//Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL(_L("Name"),
                                CLbtTriggerEntry::EStateDisabled,
                                CRequestorBase::ERequestorService,
                                CRequestorBase::EFormatApplication,
                                KMyRequestorName,
                                TUid::Uid(-4),
                                NULL);
    
    
    CLbtTriggerInfo* triggerinfo=CLbtTriggerInfo::NewL(trig,dyninfo);
	CleanupStack::PushL( triggerinfo );
    CLbtTriggerEntry* entry= triggerinfo->TriggerEntry();
    TLbtTriggerDynamicInfo* dyninfo1= triggerinfo->DynInfo();
    
    const TDesC name= entry->Name();
    AssertTrueL( entry->Name().Compare(_L("Name"))==0,_L("Wrong Name"));
    
    AssertTrueL( entry->State()== CLbtTriggerEntry::EStateDisabled ,_L("Wrong state"));
    
    AssertTrueL( dyninfo1->iValidity== TLbtTriggerDynamicInfo::EInvalid,_L("Wrong validity info"));
  //  AssertTrueL( dyninfo1->iActivityStatus== TLbtTriggerDynamicInfo::EInactive,_L("Wrong activity status"));
                                
      
    CleanupStack::PopAndDestroy( triggerinfo ); 
    
    return KErrNone;                

   
    }

// -----------------------------------------------------------------------------
// CAdditionaltests::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CAdditionaltests::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
