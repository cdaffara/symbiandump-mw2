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
* Description:    Test module to implement Test cases for C,T Classes
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
#include <lbtmanager.h>
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
//#include <lbttriggeringsettingskeys.h>

// Location aquisition API 

#include <lbscommon.h>
#include <LbsRequestor.h>
#include <LbsPosition.h>

#include "t_lbtltapi.h"
#include "t_triggerfireobserver.h"

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
// Ct_lbtltapi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ct_lbtltapi::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ct_lbtltapi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ct_lbtltapi::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "test1", Ct_lbtltapi::TCLBTFW001_testL ),
        ENTRY( "test2", Ct_lbtltapi::TCLBTFW002_testL ),
        ENTRY( "test3", Ct_lbtltapi::TCLBTFW003_testL ),
        ENTRY( "test4", Ct_lbtltapi::TCLBTFW004_testL ),
        ENTRY( "test5", Ct_lbtltapi::TCLBTFW005_testL ),
        ENTRY( "test6", Ct_lbtltapi::TCLBTFW006_testL ),
        ENTRY( "test7", Ct_lbtltapi::TCLBTFW007_testL ),
        ENTRY( "test8", Ct_lbtltapi::TCLBTFW008_testL ),
        ENTRY( "test9", Ct_lbtltapi::TCLBTFW009_testL ),
        ENTRY( "test10", Ct_lbtltapi::TCLBTFW010_testL ),
        ENTRY( "test11", Ct_lbtltapi::TCLBTFW011_testL ),
        ENTRY( "test12", Ct_lbtltapi::TCLBTFW012_testL ),
        ENTRY( "test13", Ct_lbtltapi::TCLBTFW013_testL ),
        ENTRY( "test14", Ct_lbtltapi::TCLBTFW014_testL ),
        ENTRY( "test15", Ct_lbtltapi::TCLBTFW015_testL ),
        ENTRY( "test16", Ct_lbtltapi::TCLBTFW016_testL ),
        ENTRY( "test17", Ct_lbtltapi::TCLBTFW017_testL ),
        ENTRY( "test18", Ct_lbtltapi::TCLBTFW018_testL ),
        ENTRY( "test19", Ct_lbtltapi::TCLBTFW019_testL ),
        ENTRY( "test20", Ct_lbtltapi::TCLBTFW020_testL ),
        ENTRY( "test21", Ct_lbtltapi::TCLBTFW021_testL ),
        ENTRY( "test22", Ct_lbtltapi::TCLBTFW022_testL ),
        ENTRY( "test23", Ct_lbtltapi::TCLBTFW023_testL ),
        ENTRY( "test24", Ct_lbtltapi::TCLBTFW024_testL ),
        ENTRY( "test25", Ct_lbtltapi::TCLBTFW025_testL ),
        ENTRY( "test26", Ct_lbtltapi::TCLBTFW026_testL ),
        ENTRY( "test27", Ct_lbtltapi::TCLBTFW027_testL ),
        ENTRY( "test28", Ct_lbtltapi::TCLBTFW028_testL ),
        ENTRY( "test29", Ct_lbtltapi::TCLBTFW029_testL ),
        ENTRY( "test30", Ct_lbtltapi::TCLBTFW030_testL ),
        ENTRY( "test31", Ct_lbtltapi::TCLBTFW031_testL ),
        ENTRY( "test32", Ct_lbtltapi::TCLBTFW032_testL ),
        ENTRY( "test33", Ct_lbtltapi::TCLBTFW033_testL ),
        ENTRY( "test34", Ct_lbtltapi::TCLBTFW034_testL ),
        ENTRY( "test35", Ct_lbtltapi::TCLBTFW035_testL ),
        ENTRY( "test36", Ct_lbtltapi::TCLBTFW036_testL ),
        ENTRY( "test37", Ct_lbtltapi::TCLBTFW037_testL ),
        ENTRY( "test38", Ct_lbtltapi::TCLBTFW038_testL ),
        ENTRY( "test39", Ct_lbtltapi::TCLBTFW039_testL ),
        ENTRY( "test40", Ct_lbtltapi::TCLBTFW040_testL ),
        ENTRY( "test41", Ct_lbtltapi::TCLBTFW041_testL ),
        ENTRY( "test42", Ct_lbtltapi::TCLBTFW042_testL ),
        ENTRY( "test43", Ct_lbtltapi::TCLBTFW043_testL ),
        ENTRY( "test44", Ct_lbtltapi::TCLBTFW044_testL ),
        ENTRY( "test45", Ct_lbtltapi::TCLBTFW045_testL ),
        ENTRY( "test46", Ct_lbtltapi::TCLBTFW046_testL ),
        ENTRY( "test47", Ct_lbtltapi::TCLBTFW047_testL ),
        ENTRY( "test48", Ct_lbtltapi::TCLBTFW048_testL ),
        ENTRY( "test49", Ct_lbtltapi::TCLBTFW049_testL ),
        ENTRY( "test50", Ct_lbtltapi::TCLBTFW050_testL ),
        ENTRY( "test51", Ct_lbtltapi::TCLBTFW051_testL ),
        ENTRY( "test52", Ct_lbtltapi::TCLBTFW052_testL ),
        ENTRY( "test53", Ct_lbtltapi::TC_LBT015_01_testL ),
        ENTRY( "test54", Ct_lbtltapi::TC_LBT015_02_testL ),
        ENTRY( "test55", Ct_lbtltapi::TC_LBT015_03_testL ),
        ENTRY( "test56", Ct_lbtltapi::TC_LBT015_04_testL ),
        ENTRY( "test57", Ct_lbtltapi::TC_LBT015_05_testL ),

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Ct_lbtltapi::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ct_lbtltapi::TCLBTFW001_testL( CStifItemParser& /* aItem */ )
    {

        
    //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
         
    // Check for trigger type
    AssertTrueL( trig->Type()== CLbtTriggerEntry::ETypeSession ,_L("Wrong type") );
    
       
    // check for trigger id
    TLbtTriggerId tid= trig->Id();
    AssertTrueL( tid== KLbtNullTriggerId,_L("Wrong trigger id"));
         
    // check for trigger name
    
    const TDesC name1= trig->Name(); 
    TInt length=name1.Length();
    AssertTrueL( length==0,_L("Wrong Name"));
    
    // check for trigger state  
    AssertTrueL( trig->State() == CLbtTriggerEntry::EStateEnabled ,_L("Wrong state"));
    
    // check for trigger requestors 
    RRequestorStack reqStack;
    CleanupClosePushL( reqStack );
    
    trig->GetRequestorsL( reqStack );
    TInt count=reqStack.Count();
    AssertTrueL( count==0 ,_L("Wrong requestors count")); 
    
       
       
    // check for Manager Ui 
    AssertTrueL( trig->ManagerUi()==KNullUid,_L("Wrong Manager Ui"));
    
    // Check for condition
    CLbtTriggerConditionBase* cond= trig->GetCondition();
    AssertTrueL( cond==NULL,_L("Wrong condition"));
    
    // Set Name
    trig->SetNameL(_L("Name"));
    
    // Set trigger state
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    
    // set requestors
    
    //Construct requestor
    _LIT( KMyRequestorName, "Requestor" );
    CRequestor* req = CRequestor::NewLC( 
        CRequestorBase::ERequestorService,
        CRequestorBase::EFormatApplication,
        KMyRequestorName );
        
    //Append the requestor to the requestor stack
    reqStack.AppendL( req );
 
    //Set requestor
    trig->SetRequestorsL( reqStack ); 

    // set manager Ui
      
    trig->SetManagerUi(TUid::Uid(4));
    
    // set condition
    
    TCoordinate coordinate(5,10,15);
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,10);
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
    trig->SetCondition(condition); // ownership transferred to object
    
    const TDesC name2= trig->Name();
    //AssertTrueL( name2.Compare(_L("Name"))==0,_L("Wrong Name"));
    
    AssertTrueL( trig->State()== CLbtTriggerEntry::EStateDisabled ,_L("Wrong state"));
    
    trig->GetRequestorsL( reqStack );
    count=reqStack.Count();
    AssertTrueL( count==1 ,_L("Wrong requestors count"));
    
     
    TDesC data=reqStack[0]->RequestorData();
    
    AssertTrueL( reqStack[0]->RequestorType()==CRequestorBase::ERequestorService && 
    reqStack[0]->RequestorFormat()==CRequestorBase::EFormatApplication && data.Compare(KMyRequestorName),
    _L("Wrong Requestors")); 
    
    AssertTrueL( trig->ManagerUi()==TUid::Uid(4),_L("Wrong Manager Ui"));

    cond= trig->GetCondition();
    
    CLbtTriggerConditionArea* condarea=NULL;
    
    if(cond->Type()==CLbtTriggerConditionBase::ETriggerConditionArea)
      {
    	condarea= static_cast<CLbtTriggerConditionArea*> (cond); 
    	
      }
    
    AssertTrueL( condarea->Direction()==CLbtTriggerConditionArea::EFireOnExit,_L("Wrong Direction"));
  
    CLbtGeoAreaBase* base=condarea->TriggerArea(); 
    
    CLbtGeoCircle* circlerarea=NULL;
    
    if(base->Type()==CLbtGeoAreaBase::ECircle)
      {
    	circlerarea= static_cast<CLbtGeoCircle*> (base); 
    	
      }
    
    TCoordinate center=circlerarea->Center();
    
    AssertTrueL( center.Latitude()==5 && center.Longitude()==10 && center.Altitude()==15,_L("Wrong Center"));
    
    AssertTrueL( circlerarea->Radius()==10,_L("Wrong Radius"));
       
    
    CleanupStack::PopAndDestroy( 2 ); 
    CleanupStack::PopAndDestroy(trig);
    
    return KErrNone;

    }


TInt Ct_lbtltapi::TCLBTFW002_testL( CStifItemParser& /* aItem */ )
    {

    
    //Construct requestor
    _LIT( KMyRequestorName, "Requestor" );
    
    // set condition
    
    TCoordinate coordinate(5,10,15);
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,10);
         
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);
       
    
    //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL(_L("Name"),
                                CLbtTriggerEntry::EStateDisabled,
                                CRequestorBase::ERequestorService,
                                CRequestorBase::EFormatApplication,
                                KMyRequestorName,
                                TUid::Uid(-4),
                                condition);
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
         
      
    
    const TDesC name= trig->Name();
    AssertTrueL( (trig->Name()).Compare(_L("Name"))==0,_L("Wrong Name"));
    
    AssertTrueL( trig->State()== CLbtTriggerEntry::EStateDisabled ,_L("Wrong state"));
    
    RRequestorStack reqStack;
    CleanupClosePushL( reqStack );
    
    trig->GetRequestorsL( reqStack );
    TInt count=reqStack.Count();
    AssertTrueL( count==1 ,_L("Wrong requestors count"));
    
       
    TDesC data=reqStack[0]->RequestorData();
    
    AssertTrueL( reqStack[0]->RequestorType()==CRequestorBase::ERequestorService && 
    reqStack[0]->RequestorFormat()==CRequestorBase::EFormatApplication && data.Compare(KMyRequestorName),
    _L("Wrong Requestors")); 
    
    AssertTrueL( trig->ManagerUi()==TUid::Uid(-4),_L("Wrong Manager Ui"));

    CLbtTriggerConditionBase* cond= trig->GetCondition();
    
    
    CLbtTriggerConditionArea* condarea=NULL;
    
    if(cond->Type()==CLbtTriggerConditionBase::ETriggerConditionArea)
      {
    	condarea= static_cast<CLbtTriggerConditionArea*> (cond); 
    	
      }
    
     AssertTrueL( condarea->Direction()==CLbtTriggerConditionArea::EFireOnExit,_L("Wrong Direction"));
  
     CLbtGeoAreaBase* base=condarea->TriggerArea();
     
     CLbtGeoCircle* circlerarea=NULL;
     
     if(base->Type()==CLbtGeoAreaBase::ECircle)
      {
    	circlerarea= static_cast<CLbtGeoCircle*> (base); 
    	
      }
     
    
     TCoordinate center=circlerarea->Center();
    
     AssertTrueL( center.Latitude()==5 && center.Longitude()==10 && center.Altitude()==15,_L("Wrong Center"));
    
     AssertTrueL( circlerarea->Radius()==10,_L("Wrong Radius"));
       
    
    CleanupStack::PopAndDestroy( &reqStack ); 
    CleanupStack::PopAndDestroy( 1 ); 
    
    return KErrNone;

    }



TInt Ct_lbtltapi::TCLBTFW003_testL( CStifItemParser& /* aItem */ )
    {

    
    
    _LIT(KTriggername,"aaaa");
    
    TBuf<400> name;
    //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    for(int i=0;i<100;i++)
    {
    	name.Append(KTriggername);
    }
         
    TRAPD(err,trig->SetNameL(name));
    AssertTrueL( err==KErrArgument,_L("Wrong error code for exceeding name"));
    
    CLbtSessionTrigger* trig1 = NULL;
    TRAP(err, trig1 = CLbtSessionTrigger::NewL(name,
                                               CLbtTriggerEntry::EStateEnabled,
                                               CRequestor::ERequestorService,
                                               CRequestor::EFormatApplication,
                                               _L(""),
                                                KNullUid,
                                                NULL
                                               ));

    AssertTrueL( err==KErrArgument,_L("Wrong error code for exceeding name"));    
    
    delete trig1;
    CleanupStack::PopAndDestroy( trig );
         
    return KErrNone;
    }


TInt Ct_lbtltapi::TCLBTFW004_testL( CStifItemParser& /* aItem */ )
    {

    
    
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
         
    // Check for trigger type
    AssertTrueL( trig->Type()== CLbtTriggerEntry::ETypeStartup ,_L("Wrong type") );
    
       
        
    // Check Secure Id
    TBuf<256> filename;
    TSecureId secureid;
    
    trig->GetProcessId(filename,secureid);
    AssertTrueL( filename.Length()==0 && KNullUid==secureid,_L("wrong Process Info"));
    
    // Check for commandline string
    const TDesC commandline1=trig->CommandLine(); 
    AssertTrueL( commandline1.Length()==0 ,_L("wrong CommandLine string"));
    
        
    // Set Process Id & command Line string
    _LIT( KFileName, "HandlingProcess.exe" );
//    _LIT( KCommandLine, "CommandLine" );
    filename.Format(KFileName);
    TUint32 sid=0x1333;
    secureid=sid;
   
    trig->SetProcessId(filename,secureid);
    
    trig->SetCommandLineL( _L("CommandLine") );
       
    
    trig->GetProcessId(filename,secureid);
    AssertTrueL( filename.Compare(KFileName)==0 && sid==secureid,_L("Wrong Process Id"));
        
    const TDesC commandline2=trig->CommandLine();
    AssertTrueL( trig->CommandLine().Compare(_L("CommandLine"))==0 ,_L("Wrong CommandLine"));
    
   
    CleanupStack::PopAndDestroy( 1 ); 
    
    return KErrNone;

    }




TInt Ct_lbtltapi::TCLBTFW005_testL( CStifItemParser& /* aItem */ )
    {

    
    //Construct requestor
    _LIT( KMyRequestorName, "Requestor" );
    
    TBuf<256> filename;
    TSecureId secureid;
    
        
     // Set Process Id & command Line string
    _LIT( KFileName, "HandlingProcess.exe" );
    _LIT( KCommandLine, "CommandLine" );
    filename.Format(KFileName);
    TUint32 sid= 0x1333;
    secureid=sid;
   
     
    
    //Construct a session trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL(_L("Name"),
                                CLbtTriggerEntry::EStateDisabled,
                                CRequestorBase::ERequestorService,
                                CRequestorBase::EFormatApplication,
                                KMyRequestorName,
                                TUid::Uid(-4),
                                NULL,
                                KFileName,
                                secureid,
                                KCommandLine);
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
           
    
    trig->GetProcessId(filename,secureid);
    AssertTrueL( filename.Compare(KFileName)==0 && sid==secureid,_L("Wrong Process Id"));
        
    const TDesC commandline=trig->CommandLine();
    AssertTrueL( trig->CommandLine().Compare(KCommandLine)==0 ,_L("Wrong CommandLine"));
    
       
    
    CleanupStack::PopAndDestroy( 1); 
    
    return KErrNone;

    }

TInt Ct_lbtltapi::TCLBTFW006_testL( CStifItemParser& /* aItem */ )
    {

    
    
    _LIT(KFilename,"aaaa");
    
    TBuf<400> name;
    TSecureId secureid=0x1234;
    
        
    for(int i=0;i<100;i++)
    {
    	name.Append(KFilename);
    }
         
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();   
    
    
    // Set panic with code ELbtErrArgument to acceptable exit reason
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    trig->SetProcessId( name,secureid );
    
    return KErrNone;
    }


TInt Ct_lbtltapi::TCLBTFW007_testL( CStifItemParser& /* aItem */ )
    {

    
    
    _LIT(KFilename,"aaaa");
    
    TBuf<400> name;
        
    for(int i=0;i<100;i++)
    {
    	name.Append(KFilename);
    }
         
       
    // Set panic with code ELbtErrArgument to acceptable exit reason
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument ); 
    
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL(_L(""),
                                                        CLbtTriggerEntry::EStateEnabled,
                                                        CRequestor::ERequestorService,
                                                        CRequestor::EFormatApplication,
                                                        _L(""),
                                                         KNullUid,
                                                         NULL,
                                                         name
                                                        );
   
    return KErrNone;
    }




TInt Ct_lbtltapi::TCLBTFW008_testL( CStifItemParser& /* aItem */ )
    {

    
    
    //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
         
        
    // check for trigger requestors 
    RRequestorStack reqStack;
    CleanupClosePushL( reqStack );
    
    TRAPD(err,trig->SetRequestorsL( reqStack ));
    
    AssertTrueL( err==KErrArgument,_L("Wrong error code for empty requestor stack")); 
    
    CleanupStack::PopAndDestroy( &reqStack );
    
        
    CleanupStack::PopAndDestroy( trig ); 
    
    return KErrNone;

    }

TInt Ct_lbtltapi::TCLBTFW009_testL( CStifItemParser& /* aItem */ )
    {

    
    
   
    CLbtGeoCircle* circle = CLbtGeoCircle::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( circle );
         
    TReal radius=circle->Radius();
    TCoordinate center=circle->Center();
    
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



TInt Ct_lbtltapi::TCLBTFW010_testL( CStifItemParser& /* aItem */ )
    {

    
    
   
    CLbtGeoCircle* circle = CLbtGeoCircle::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    circle->SetRadius( -10 );
    
    return KErrNone;
    }
    
TInt Ct_lbtltapi::TCLBTFW011_testL( CStifItemParser& /* aItem */ )
    {

    TCoordinate center(5,10,15);
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
   
    CLbtGeoCircle* circle = CLbtGeoCircle::NewL(center,-10);
    
    return KErrNone;   
    }
    



TInt Ct_lbtltapi::TCLBTFW012_testL( CStifItemParser& /* aItem */ )
    {

    
    
    
    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( rect );
    
    TReal aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude ; 
    
    AssertTrueL(rect->Type()==CLbtGeoAreaBase::ERectangular,_L("Wrong Type"));   
    
    rect->GetBorder(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude );

    AssertTrueL(aSouthLatitude==0 && aNorthLatitude==0 && aWestLongitude==0 && aEastLongitude==0,_L("Borders are numbers"));   
    
    CleanupStack::PopAndDestroy( rect ); 
    
    return KErrNone;

    }

TInt Ct_lbtltapi::TCLBTFW013_testL( CStifItemParser& /* aItem */ )
    {

    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    rect->SetBorder(-91,89,90,-179);
     
    return KErrNone;
    }

TInt Ct_lbtltapi::TCLBTFW014_testL( CStifItemParser& /* aItem */ )
    {

    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    rect->SetBorder(91,89,90,-179);
     
    return KErrNone;
    }
    
TInt Ct_lbtltapi::TCLBTFW015_testL( CStifItemParser& /* aItem */ )
    {

    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    rect->SetBorder(-90,-91,90,179);
     
    return KErrNone;
    }
TInt Ct_lbtltapi::TCLBTFW016_testL( CStifItemParser& /* aItem */ )
    {

    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    rect->SetBorder(-90,91,-90,179);
     
    return KErrNone;
    }
TInt Ct_lbtltapi::TCLBTFW017_testL( CStifItemParser& /* aItem */ )
    {

    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    rect->SetBorder(-90,89,-181,179);
     
    return KErrNone;
    }
TInt Ct_lbtltapi::TCLBTFW018_testL( CStifItemParser& /* aItem */ )
    {

    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    rect->SetBorder(-90,89,181,179);
     
    return KErrNone;
    }
TInt Ct_lbtltapi::TCLBTFW019_testL( CStifItemParser& /* aItem */ )
    {

    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    rect->SetBorder(-90,89,179,-181);
     
    return KErrNone;
    }

TInt Ct_lbtltapi::TCLBTFW033_testL( CStifItemParser& /* aItem */ )
    {

    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    rect->SetBorder(-90,89,179,181);
     
    return KErrNone;
    }


TInt Ct_lbtltapi::TCLBTFW020_testL( CStifItemParser& /* aItem */ )
    {

    
    CLbtTriggerConditionArea* cond=CLbtTriggerConditionArea::NewL( );
    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
    
    cond->SetTriggerArea(rect);
    
    return KErrNone;
    }


TInt Ct_lbtltapi::TCLBTFW021_testL( CStifItemParser& /* aItem */ )
    {

    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
         
    CLbtTriggerConditionArea* cond=CLbtTriggerConditionArea::NewL(rect,CLbtTriggerConditionArea::EFireOnExit);
    
    return KErrNone;
    }


TInt Ct_lbtltapi::TCLBTFW022_testL( CStifItemParser& /* aItem */ )
    {

    
    
    CLbtTriggerFilterComposite * composite=CLbtTriggerFilterComposite::NewL(CLbtTriggerFilterComposite::ECompositionTypeAnd);
    
     TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 130 );
    
    CLbtTriggerFilterBase* base=composite->GetFilter(1);
    
    return KErrNone;
    }

TInt Ct_lbtltapi::TCLBTFW023_testL( CStifItemParser& /* aItem */ )
    {

     
    
    CLbtTriggerFilterComposite * composite=CLbtTriggerFilterComposite::NewL(CLbtTriggerFilterComposite::ECompositionTypeAnd);
    
     TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 130 );
    
    composite->RemoveFilter(1);
    
     return KErrNone;
     }

TInt Ct_lbtltapi::TCLBTFW024_testL( CStifItemParser& /* aItem */ )
    {

     
    TCoordinate center;
    CLbtTriggerFilterComposite * composite=CLbtTriggerFilterComposite::NewL(CLbtTriggerFilterComposite::ECompositionTypeAnd);
    CleanupStack::PushL( composite );
   
    CLbtGeoCircle* circle1 = CLbtGeoCircle::NewL(center,5);
        
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(circle1);   
    composite->AddFilterL(areafilter1);
    
    TInt count=composite->NumberOfFilters();
    AssertTrueL(count==1,_L("wrong count"));
    
    CLbtTriggerFilterBase* basefilter=composite->GetFilter(0);
    
    CLbtTriggerFilterByArea* areafilter2=NULL;
    if( basefilter->Type()==CLbtTriggerFilterBase::EFilterByArea)
    {
       areafilter2= static_cast<CLbtTriggerFilterByArea*> (basefilter);	
    }
  
    CLbtGeoAreaBase* geobase=areafilter2->Area();
    
    CLbtGeoCircle* circle2=NULL;
    
    if( geobase->Type()==CLbtGeoAreaBase::ECircle)
    {
    	circle2= static_cast<CLbtGeoCircle*> (geobase); 
    }
    
    
    AssertTrueL( circle2->Radius()==5,_L("wrong Radius"));
    
    
    CleanupStack::PopAndDestroy( 1 ); 
    return KErrNone;
     }
     
TInt Ct_lbtltapi::TCLBTFW025_testL( CStifItemParser& /* aItem */ )
    {

     
    CLbtTriggerFilterComposite * composite=CLbtTriggerFilterComposite::NewL(CLbtTriggerFilterComposite::ECompositionTypeAnd);
    CleanupStack::PushL( composite );
   
    composite->Reset();
       
    CleanupStack::PopAndDestroy( composite ); 
    return KErrNone;
     }     


TInt Ct_lbtltapi::TCLBTFW026_testL( CStifItemParser& /* aItem */ )
    {

     
    TCoordinate center;
    CLbtTriggerFilterComposite * composite=CLbtTriggerFilterComposite::NewL(CLbtTriggerFilterComposite::ECompositionTypeAnd);
       
    CLbtGeoCircle* circle = CLbtGeoCircle::NewL(center,5);
        
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(circle);   
    composite->AddFilterL(areafilter);
    
    composite->Reset();
    
    TInt count=composite->NumberOfFilters();
    AssertTrueL(count==0,_L("wrong count"));
    
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 130 );
    CLbtTriggerFilterBase* basefilter=composite->GetFilter(0);
    
    return KErrNone;
    
    }



TInt Ct_lbtltapi::TCLBTFW027_testL( CStifItemParser& /* aItem */ )
    {
     
    TCoordinate center;
    TLbtTriggerAttributeFieldsMask aEntryMask;
    TLbtTriggerDynamicInfoFieldsMask aDynInfoMask;
    
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    listoptions->GetRetrievedFields( aEntryMask,aDynInfoMask );
    AssertTrueL(aEntryMask==KLbtTriggerAttributeFieldsAll,_L("wrong entry field mask"));
    AssertTrueL(aDynInfoMask==KLbtTriggerDynInfoFieldsAll ,_L("wrong dyn. info field mask"));
    
    CLbtTriggerFilterBase* filter=listoptions->Filter();
    AssertTrueL(filter==NULL ,_L("wrong filter"));
    
    AssertTrueL(listoptions->SortingOption()==CLbtListTriggerOptions::ELbtNoSorting ,_L("wrong sorting option"));
       
    listoptions->SetRetrievedFields( CLbtTriggerEntry::EAttributeState |  
                                     CLbtTriggerEntry::EAttributeName | 
                                     CLbtTriggerEntry::EAttributeManagerUi,
                                     TLbtTriggerDynamicInfo::EValidityStatus |
                                     TLbtTriggerDynamicInfo::EDistanceToLatestLocation);
                                     

    CLbtGeoCircle* circle1 = CLbtGeoCircle::NewL(center,5);
        
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(circle1);
    CleanupStack::PushL( areafilter1 );
        
    listoptions->SetFilter( areafilter1 );
 
    listoptions->SetSortingOption( CLbtListTriggerOptions::ELbtTriggerNameAscending );
    
    listoptions->GetRetrievedFields( aEntryMask,aDynInfoMask );
    
    AssertTrueL(aEntryMask==0x16,_L("wrong entry field mask"));
    AssertTrueL(aDynInfoMask==0x3,_L("wrong dyn. info field mask"));
    
    CLbtTriggerFilterBase* basefilter=listoptions->Filter();
    
    CLbtTriggerFilterByArea* areafilter2=NULL;
    if( basefilter->Type()==CLbtTriggerFilterBase::EFilterByArea)
    {
       areafilter2= static_cast<CLbtTriggerFilterByArea*> (basefilter);	
    }
    
    
    CLbtGeoAreaBase* geobase=areafilter2->Area();
    
    CLbtGeoCircle* circle2=NULL;
    if(geobase->Type()==CLbtGeoAreaBase::ECircle)
    {
    	circle2= static_cast<CLbtGeoCircle*> (geobase);
    }
    
    AssertTrueL( circle2->Radius()==5,_L("wrong Radius"));
    
    AssertTrueL( listoptions->SortingOption()==CLbtListTriggerOptions::ELbtTriggerNameAscending,_L("wrong sorting option"));
 
    
    CleanupStack::PopAndDestroy( 2 ); 
    
     
    return KErrNone;

    }



TInt Ct_lbtltapi::TCLBTFW028_testL( CStifItemParser& /* aItem */ )
    {

     CLbtTriggerFilterByAttribute* filter=CLbtTriggerFilterByAttribute::NewL();
     CleanupStack::PushL( filter );
     filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
     TBool result=filter->IsTriggerTypeInFilter(CLbtTriggerEntry::ETypeSession);
     AssertTrueL(result==1 ,_L("type is not in filter"));
     
     result=filter->IsTriggerStateInFilter(CLbtTriggerEntry::EStateEnabled);
     AssertTrueL(result==0 ,_L("state is in filter")); 
     
     result=filter->IsTriggerValidityStatusInFilter(TLbtTriggerDynamicInfo::EInvalid);
     AssertTrueL(result==0 ,_L("validity is in filter")); 
     
     filter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
     filter->AddTriggerValidityL(TLbtTriggerDynamicInfo::EInvalid);
     
     result=filter->IsTriggerTypeInFilter(CLbtTriggerEntry::ETypeSession);
     AssertTrueL(result==1 ,_L("type is not in filter"));
     
     result=filter->IsTriggerStateInFilter(CLbtTriggerEntry::EStateEnabled);
     AssertTrueL(result==1 ,_L("state is not in filter")); 
     
     result=filter->IsTriggerValidityStatusInFilter(TLbtTriggerDynamicInfo::EInvalid);
     AssertTrueL(result==1 ,_L("validity is not in filter"));
     
     filter->ResetFilter();
     
     result=filter->IsTriggerTypeInFilter(CLbtTriggerEntry::ETypeSession);
     AssertTrueL(result==0 ,_L("type is in filter"));
     
     result=filter->IsTriggerStateInFilter(CLbtTriggerEntry::EStateEnabled);
     AssertTrueL(result==0 ,_L("state is  in filter")); 
     
     result=filter->IsTriggerValidityStatusInFilter(TLbtTriggerDynamicInfo::EInvalid);
     AssertTrueL(result==0 ,_L("validity is in filter"));
     
     filter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
     
     result=filter->IsTriggerTypeInFilter(CLbtTriggerEntry::ETypeSession);
     AssertTrueL(result==0 ,_L("type is in filter"));
     
     result=filter->IsTriggerTypeInFilter(CLbtTriggerEntry::ETypeStartup);
     AssertTrueL(result==1 ,_L("type is not in filter"));
     
     filter->AddTriggerManagerUiL( TUid::Uid(4) );
     
     result=filter->IsTriggerManagerUiInFilter( TUid::Uid(4) );
     AssertTrueL(result==1 ,_L("manager is not in filter"));
     
     filter->AddTriggerIdL( 0x1 );
     
     result=filter->IsTriggerIdInFilter( 0x1 );
     AssertTrueL(result==1 ,_L("Trigger id is not in filter"));
     

     CleanupStack::PopAndDestroy( filter );   
     
     
     return KErrNone;

    }


TInt Ct_lbtltapi::TCLBTFW029_testL( CStifItemParser& /* aItem */ )
    {

     //Construct a session trigger
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );  
   // _LIT( Name, "Name" );
    // Set Name
    trig1->SetNameL(_L("Name"));
    
    // Set trigger state
    trig1->SetState( CLbtTriggerEntry::EStateDisabled );  
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    trig1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     //Construct a session trigger
    CLbtSessionTrigger* trig2 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 ); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    
    CleanupClosePushL(rd);
    
    trig2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    const TDesC name= trig2->Name();
    AssertTrueL( trig2->Name().Compare(_L("Name"))==0,_L("Wrong Name"));
    
    AssertTrueL( trig2->State()== CLbtTriggerEntry::EStateDisabled ,_L("Wrong state"));
        
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }


TInt Ct_lbtltapi::TCLBTFW034_testL( CStifItemParser& /* aItem */ )
    {

    
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
    
    TCoordinate center(5,10,15);
    
    CLbtGeoCircle* circle1= CLbtGeoCircle::NewL( center,10 );
        
    cond1->SetTriggerArea( circle1 );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);    
    cond1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
    CLbtTriggerConditionArea* cond2= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond2 );
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);    
    cond2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset(); 
    
    CLbtGeoAreaBase* base=cond2->TriggerArea();
    
    CLbtGeoCircle* circle2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECircle)
    {
    	circle2= static_cast<CLbtGeoCircle*> (base);
    }
    
    center= circle2->Center();
    
    AssertTrueL( center.Latitude()==5 && center.Longitude()==10 && center.Altitude()==15,_L("Wrong Center"));
    
    AssertTrueL( circle2->Radius()==10,_L("Wrong Radius"));
    
    AssertTrueL( cond2->Direction()== CLbtTriggerConditionArea::EFireOnExit ,_L("Wrong Direction"));
        
        
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }


TInt Ct_lbtltapi::TCLBTFW035_testL( CStifItemParser& /* aItem */ )
    {

     //Construct a session trigger
    CLbtStartupTrigger* trig1 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );  
    
    TBuf<256> filename;
    TSecureId secureid;
    
   // Set Process Id & command Line string
    _LIT( KFileName, "HandlingProcess.exe" );
    _LIT( KCommandLine, "CommandLine" );
    filename.Format(KFileName);
    TUint32 sid=0x1333;
    secureid=sid;
   
    trig1->SetProcessId(filename,secureid);
    
    trig1->SetCommandLineL( KCommandLine );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    trig1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     //Construct a session trigger
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 ); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    trig2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    trig2->GetProcessId(filename,secureid);
    AssertTrueL( filename.Compare(KFileName)==0 && sid==secureid,_L("Wrong Process Id"));
        
    const TDesC commandline2=trig2->CommandLine();
    AssertTrueL( trig2->CommandLine().Compare(KCommandLine)==0 ,_L("Wrong CommandLine"));
        
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }

TInt Ct_lbtltapi::TCLBTFW036_testL( CStifItemParser& /* aItem */ )
    {

     
    CLbtGeoRect* trig1 = CLbtGeoRect::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );  
    
    trig1->SetBorder(-90,90,-180,180);
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    trig1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtGeoRect* trig2 = CLbtGeoRect::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 ); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    trig2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    TReal aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude ; 
    trig2->GetBorder(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude);
    
    AssertTrueL( aSouthLatitude==-90 && aNorthLatitude==90 &&
                 aWestLongitude==-180 && aEastLongitude==180,_L("Wrong Border values"));
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }


TInt Ct_lbtltapi::TCLBTFW037_testL( CStifItemParser& /* aItem */ )
    {

    
    TCoordinate coordinate(5,10,15);
    
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,10);
    CleanupStack::PushL( circle1 );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    circle1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtGeoCircle* circle2 = CLbtGeoCircle::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( circle2 ); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    
    CleanupClosePushL(rd);
    
    circle2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    TCoordinate center=circle2->Center();
    
    AssertTrueL( center.Latitude()==5 && center.Longitude()==10 && center.Altitude()==15,_L("Wrong Center"));
    
    AssertTrueL( circle2->Radius()==10,_L("Wrong Radius"));
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }

TInt Ct_lbtltapi::TCLBTFW038_testL( CStifItemParser& /* aItem */ )
    {

    
    TCoordinate coordinate(5,10,15);
    
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,10);
    
    CLbtTriggerFilterByArea* filterarea1= CLbtTriggerFilterByArea::NewL( circle1 );
    CleanupStack::PushL( filterarea1 );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    filterarea1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtTriggerFilterByArea* filterarea2= CLbtTriggerFilterByArea::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( filterarea2); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    filterarea2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    CLbtGeoAreaBase* base=filterarea2->Area();
    
    CLbtGeoCircle* circle2=NULL;
    if( base->Type()== CLbtGeoAreaBase::ECircle )
    {
    	circle2= static_cast<CLbtGeoCircle*> (base);
    }
        
    TCoordinate center=circle2->Center();
    
    AssertTrueL( center.Latitude()==5 && center.Longitude()==10 && center.Altitude()==15,_L("Wrong Center"));
    
    AssertTrueL( circle2->Radius()==10,_L("Wrong Radius"));
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }

TInt Ct_lbtltapi::TCLBTFW039_testL( CStifItemParser& /* aItem */ )
    {

    
    CLbtTriggerFilterByAttribute* filter1=CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( filter1 );
    filter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    filter1->AddTriggerManagerUiL( TUid::Uid(5) );
    
        
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
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }

TInt Ct_lbtltapi::TCLBTFW040_testL( CStifItemParser& /* aItem */ )
    {

    
    CLbtTriggerFilterByAttribute* filter1=CLbtTriggerFilterByAttribute::NewL();
    filter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    filter1->AddTriggerManagerUiL( TUid::Uid(5) );
    
    CLbtTriggerFilterComposite* compfilter1=CLbtTriggerFilterComposite::NewL();
    CleanupStack::PushL( compfilter1 ); 
    
    compfilter1->AddFilterL( filter1 );
            
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    compfilter1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtTriggerFilterComposite* compfilter2=CLbtTriggerFilterComposite::NewL();
    CleanupStack::PushL( compfilter2 );  
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    compfilter2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    CLbtTriggerFilterBase* base=compfilter2->GetFilter(0);
    
    CLbtTriggerFilterByAttribute* filter2=NULL;
    if( base->Type()== CLbtTriggerFilterBase::EFilterByAttribute )
    {
    	filter2= static_cast<CLbtTriggerFilterByAttribute*> (base);
    } 

  
    TBool result=filter2->IsTriggerTypeInFilter(CLbtTriggerEntry::ETypeSession);
    AssertTrueL(result==1 ,_L("Type is  in filter")); 
    result=filter2->IsTriggerStateInFilter(CLbtTriggerEntry::EStateDisabled);
    AssertTrueL(result==1 ,_L("state is  in filter"));
    result=filter2->IsTriggerManagerUiInFilter( TUid::Uid(5) );
    AssertTrueL(result==1 ,_L("Manager ui is  in filter"));  
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }

TInt Ct_lbtltapi::TCLBTFW041_testL( CStifItemParser& /* aItem */ )
    {

    
    _LIT( KMyRequestorName, "Requestor" );
	CLbtTriggerInfo* triggerinfo1=CLbtTriggerInfo::NewL();
	CleanupStack::PushL( triggerinfo1);

	TLbtTriggerDynamicInfo* dyninfo = new(ELeave) TLbtTriggerDynamicInfo;
	dyninfo->iValidity= TLbtTriggerDynamicInfo::EInvalid;
//	dyninfo->iActivityStatus= TLbtTriggerDynamicInfo::EInactive;
     
    triggerinfo1->SetDynInfo( dyninfo );
	
     
	//Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL(_L("Name"),
                                CLbtTriggerEntry::EStateDisabled,
                                CRequestorBase::ERequestorService,
                                CRequestorBase::EFormatApplication,
                                KMyRequestorName,
                                TUid::Uid(-4),
                                NULL);
    
    triggerinfo1->SetTriggerEntry( trig );
            
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    triggerinfo1->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtTriggerInfo* triggerinfo2=CLbtTriggerInfo::NewL();
	CleanupStack::PushL( triggerinfo2); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    triggerinfo2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    CLbtTriggerEntry* entry= triggerinfo2->TriggerEntry();
    TLbtTriggerDynamicInfo* dyninfo1= triggerinfo2->DynInfo();
    
    const TDesC name= entry->Name();
    AssertTrueL( entry->Name().Compare(_L("Name"))==0,_L("Wrong Name"));
    
    AssertTrueL( entry->State()== CLbtTriggerEntry::EStateDisabled ,_L("Wrong state"));
    
    AssertTrueL( dyninfo1->iValidity== TLbtTriggerDynamicInfo::EInvalid,_L("Wrong validity info"));
   // AssertTrueL( dyninfo1->iActivityStatus== TLbtTriggerDynamicInfo::EInactive,_L("Wrong activity status"));
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }



TInt Ct_lbtltapi::TCLBTFW030_testL( CStifItemParser& /* aItem */ )
    {

	_LIT( KMyRequestorName, "Requestor" );
	CLbtTriggerInfo* triggerinfo=CLbtTriggerInfo::NewL();
	CleanupStack::PushL( triggerinfo );

	TLbtTriggerDynamicInfo* dyninfo = new(ELeave) TLbtTriggerDynamicInfo;
	dyninfo->iValidity= TLbtTriggerDynamicInfo::EInvalid;
//	dyninfo->iActivityStatus= TLbtTriggerDynamicInfo::EInactive;
     
    triggerinfo->SetDynInfo( dyninfo );
	
     
	//Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL(_L("Name"),
                                CLbtTriggerEntry::EStateDisabled,
                                CRequestorBase::ERequestorService,
                                CRequestorBase::EFormatApplication,
                                KMyRequestorName,
                                TUid::Uid(-4),
                                NULL);
    
    triggerinfo->SetTriggerEntry( trig );
    
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



TInt Ct_lbtltapi::TCLBTFW031_testL( CStifItemParser& /* aItem */ )
    {

     TLbtTriggeringSystemSettings settings;
          
     AssertTrueL( settings.MinimumTriggerAreaSize()== 0 ,_L("Wrong Minimum Area"));
     AssertTrueL( settings.TriggeringMechanismState()== ETriggeringMechanismOn ,_L("Wrong Triggering state"));
     
     settings.SetMinimumTriggerAreaSize( 10 );
     settings.SetTriggeringMechanismState( ETriggeringMechanismOff );
        
     AssertTrueL( settings.MinimumTriggerAreaSize()== 10 ,_L("Wrong Minimum Area"));
     AssertTrueL( settings.TriggeringMechanismState()== ETriggeringMechanismOff ,_L("Wrong Triggering state"));
     
     return KErrNone;
     }

TInt Ct_lbtltapi::TCLBTFW032_testL( CStifItemParser& /* aItem */ )
    {

     TLbtTriggeringSystemSettings settings;
     
     TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtErrArgument );
          
     settings.SetMinimumTriggerAreaSize( -10 );
     
     return KErrNone;
     }


TInt Ct_lbtltapi::TCLBTFW042_testL( CStifItemParser& /* aItem */ )
    {

	     // Simulation filename
	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move.sps" );

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
    
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
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
    wait->Start( );
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;

    return KErrNone; 
      
    }


TInt Ct_lbtltapi::TCLBTFW043_testL( CStifItemParser& /* aItem */ )
    {

	     // Simulation filename
	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt1,lbt2,lbt3;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt1.Open( lbtserver ) );
 	 CleanupClosePushL( lbt1 );
 	 User::LeaveIfError( lbt2.Open( lbtserver ) );
 	 CleanupClosePushL( lbt2 );
 	 User::LeaveIfError( lbt3.Open( lbtserver ) );
 	 CleanupClosePushL( lbt3 );
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
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
    TInt err=coordinate.Move(90,118000);
	    
	       
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,115000);
    CleanupStack::PushL( circle );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId1,trigId2,trigId3;
        
        
    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate,115000 );
    CleanupStack::PushL( notifier1 );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2,coordinate,115000 );
    CleanupStack::PushL( notifier2 );
    
    CTriggerFireObserver* notifier3= CTriggerFireObserver::NewL( lbt3,coordinate,115000 );
    CleanupStack::PushL( notifier3 );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
            
    notifier1->CreateTriggers( lbt1,*trig,trigId1,EFalse,wait );
    wait->Start( );
    
    notifier2->CreateTriggers( lbt2,*trig,trigId2,ETrue,wait );
    wait->Start( );
    
    notifier3->CreateTriggers( lbt3,*trig,trigId3,ETrue,wait );
    wait->Start( );
    
    notifier1->StartNotification( wait );
    wait->Start( );
    
    RArray< TLbtTriggerFireInfo> triggerarray;
    
    lbt1.GetFiredTriggersL( triggerarray );
    AssertTrueL( triggerarray.Count( )==3,_L(" Wrong retrieval of fired triggers" ) );
   // AssertTrueL( trigId2==triggerarray[0].iTriggerId,_L(" Wrong trigger fired info 1") );
   // AssertTrueL( trigId3==triggerarray[1].iTriggerId,_L(" Wrong trigger fired info 2") );
        
    triggerarray.Close();
    CleanupStack::PopAndDestroy( 3 );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt3 );
    CleanupStack::PopAndDestroy( &lbt2 );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
        
    return KErrNone; 
      
    }


TInt Ct_lbtltapi::TCLBTFW044_testL( CStifItemParser& /* aItem */ )
    {

	    // Simulation filename
	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move.sps" );

 	 RLbtServer lbtserver;
 	 RLbt lbt1,lbt2,lbt3;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt1.Open( lbtserver ) );
 	 CleanupClosePushL( lbt1 );
 	 User::LeaveIfError( lbt2.Open( lbtserver ) );
 	 CleanupClosePushL( lbt2 );
 	 User::LeaveIfError( lbt3.Open( lbtserver ) );
 	 CleanupClosePushL( lbt3 );
 	
 	 CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
	 CleanupStack::PopAndDestroy(repository);
	 
	  //Construct a session trigger
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    
     
    CLbtSessionTrigger* trig2 = CLbtSessionTrigger::NewL();
    CleanupStack::PushL( trig2 );
    
     
    CLbtSessionTrigger* trig3 = CLbtSessionTrigger::NewL();
    CleanupStack::PushL( trig3 );
    
    // Set Name
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig3->SetNameL(_L("Trigger3"));
    
      //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
	
	trig2->SetRequestorL(ReqType,ReqFormat,ReqData);  
	
	trig3->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
    
    TCoordinate coordinate1(61.5285,23.9385);
    TCoordinate coordinate2(61.5285,23.9385);
    TCoordinate coordinate3(61.5285,23.9385);
    
    TInt err=coordinate1.Move(90,1000);
    err=coordinate2.Move(90,1500);
    err=coordinate3.Move(90,2000);
	    
	    
    
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate1,500);
    CleanupStack::PushL( circle1 );
    
    CLbtGeoCircle* circle2=CLbtGeoCircle::NewL(coordinate2,500);
    CleanupStack::PushL( circle2 );
    
    CLbtGeoCircle* circle3=CLbtGeoCircle::NewL(coordinate3,1000);
    CleanupStack::PushL( circle3 );
    
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition1=CLbtTriggerConditionArea::NewL(
                                                circle1,
                                                CLbtTriggerConditionArea::EFireOnExit);
        
        
    CLbtTriggerConditionArea* condition2=CLbtTriggerConditionArea::NewL(
                                                circle2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
        
    CLbtTriggerConditionArea* condition3=CLbtTriggerConditionArea::NewL(
                                                circle3,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( 3 );
    
    
    trig1->SetCondition(condition1); // ownership transferred to object
    trig2->SetCondition(condition2);
    trig3->SetCondition(condition3);

    TLbtTriggerId trigId1,trigId2,trigId3;
        
        
    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate1,500 );
    CleanupStack::PushL( notifier1 );
    
    CTriggerFireObserver* notifier2= CTriggerFireObserver::NewL( lbt2,coordinate2,500 );
    CleanupStack::PushL( notifier2 );
    
    CTriggerFireObserver* notifier3= CTriggerFireObserver::NewL( lbt3,coordinate3,1000 );
    CleanupStack::PushL( notifier3 );
    
    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
    CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
    CActiveSchedulerWait* wait3=new(ELeave)CActiveSchedulerWait;
            
    notifier1->CreateTriggers( lbt1,*trig1,trigId1,ETrue,wait1 );
    wait1->Start( );
    
    notifier2->CreateTriggers( lbt2,*trig2,trigId2,ETrue,wait2 );
    wait2->Start( );
    
    notifier3->CreateTriggers( lbt3,*trig3,trigId3,ETrue,wait3 );
    wait3->Start( );
    
    notifier1->StartNotification( wait1 );
    wait1->Start( );
    notifier2->StartNotification( wait2 );
    wait2->Start( );
  //  notifier3->StartNotification( wait3 );
    notifier3->After(120000000);
    wait3->Start( );// Start wait loop for the trigger which fires at last.
    
    RArray< TLbtTriggerFireInfo> triggerarray;
    
    lbt1.GetFiredTriggersL( triggerarray );
    
    // returned list should be empty.Since for all trigger fire events client 
    // has been reported before.
    
    AssertTrueL( triggerarray.Count( )==3,_L(" Wrong retrieval of fired triggers" ) );
    triggerarray.Close();
    
    CleanupStack::PopAndDestroy( 6 );
    CleanupStack::PopAndDestroy( &lbt3 );
    CleanupStack::PopAndDestroy( &lbt2 );
    CleanupStack::PopAndDestroy( &lbt1 );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait1;
    delete wait2;
    delete wait3;
        
    return KErrNone; 
     
    }

TInt Ct_lbtltapi::TCLBTFW045_testL( CStifItemParser& /* aItem */ )
    {

	    // Simulation filename
	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move.sps" );

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
    
      //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);  
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
    
    notifier->StartNotification( );
    notifier->CancelNotification( );
    
    RArray< TLbtTriggerFireInfo> triggerarray;
    
    lbt.GetFiredTriggersL( triggerarray );
    
    // Trigger should not fire since request has been cancelled
    AssertTrueL( triggerarray.Count( )==0,_L(" Wrong retrieval of fired triggers" ) );
    triggerarray.Close();
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
     
    }

TInt Ct_lbtltapi::TCLBTFW046_testL( CStifItemParser& /* aItem */ )
    {

	    // Simulation filename
	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move.sps" );

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
    
     //Set Requestor
	CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
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
    wait->Start( );
    
    notifier->CancelNotification( );
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
      
    }

TInt Ct_lbtltapi::TCLBTFW047_testL( CStifItemParser& /* aItem */ )
    {

	 
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 TCoordinate coordinate;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 	   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
    CleanupStack::PushL( notifier );
    
    notifier->CancelNotification( );
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
       
    return KErrNone; 
      
    }

TInt Ct_lbtltapi::TCLBTFW048_testL( CStifItemParser& /* aItem */ )
    {

	 
 	 RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	
 	 RArray< TLbtTriggerFireInfo> triggerarray;
 	 lbt.GetFiredTriggersL( triggerarray );
 	 AssertTrueL( triggerarray.Count( )==0,_L(" Wrong retrieval of fired triggers" ) );
    
     triggerarray.Close( );
     CleanupStack::PopAndDestroy( &lbt );
     CleanupStack::PopAndDestroy( &lbtserver );
       
     return KErrNone; 
      
    }

TInt Ct_lbtltapi::TCLBTFW049_testL( CStifItemParser& /* aItem */ )
    {

	     // Simulation filename
	 _LIT( KSimulationFile,"c:\\system\\data\\simu_move.sps" );

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
    
    TCoordinate coordinate(61.5285,23.9385);
    TInt err=coordinate.Move(90,118000);
    
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
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    delete wait;
    
    return KErrNone; 
      
    }

TInt Ct_lbtltapi::TCLBTFW050_testL( CStifItemParser& /* aItem */ )
    {

	 
 	 RLbt lbt;
 	 TCoordinate coordinate;
 	 
 	 TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtServerBadHandle );
 	 CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate,115000 );
           
     return KErrNone;
    }

TInt Ct_lbtltapi::TCLBTFW051_testL( CStifItemParser& /* aItem */ )
    {

	 
 	 RLbt lbt;
 	 TLbtTriggerFireInfo fireinfo;
 	 TRequestStatus status; 
 	 
 	 TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtServerBadHandle );	 
 	 lbt.NotifyTriggerFired( fireinfo,status );
 	            
     return KErrNone; 
    }

TInt Ct_lbtltapi::TCLBTFW052_testL( CStifItemParser& /* aItem */ )
    {

	 
 	 RLbt lbt;
 	 	 	 
 	 TestModuleIf().SetExitReason( CTestModuleIf::EPanic, ELbtServerBadHandle );
 	 lbt.CancelNotifyTriggerFired( );
 	            
     return KErrNone;
    }

TInt Ct_lbtltapi::TC_LBT015_01_testL( CStifItemParser& /* aItem */ )
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

TInt Ct_lbtltapi::TC_LBT015_02_testL( CStifItemParser& /* aItem */ )
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
 TInt Ct_lbtltapi::TC_LBT015_03_testL( CStifItemParser& /* aItem */ )
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
    
    TInt Ct_lbtltapi::TC_LBT015_04_testL( CStifItemParser& /* aItem */ )
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

TInt Ct_lbtltapi::TC_LBT015_05_testL( CStifItemParser& /* aItem */ )
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
// -----------------------------------------------------------------------------
// Ct_lbtltapi::AssertTrueL
// function for condition check.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void Ct_lbtltapi::AssertTrueL( TBool aCondition,const TDesC& aMessage )
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
