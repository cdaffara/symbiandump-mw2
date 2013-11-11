/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    This file contains testclass implementation.
*
*/




// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "TestHybridTriggers.h"



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
// CTestHybridTriggers::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTestHybridTriggers::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "TC_TestNewL1L", CTestHybridTriggers::TC_TestNewL1L ),
        ENTRY( "TC_TestNewL2L", CTestHybridTriggers::TC_TestNewL2L ),
        ENTRY( "TC_TestNewL3L", CTestHybridTriggers::TC_TestNewL3L ),
        ENTRY( "TC_TestNewL4L", CTestHybridTriggers::TC_TestNewL4L ),
        ENTRY( "TC_TestNewL5L", CTestHybridTriggers::TC_TestNewL5L ),
        ENTRY( "TC_TestNewLC1L", CTestHybridTriggers::TC_TestNewLC1L ),
        ENTRY( "TC_TestNewLC2L", CTestHybridTriggers::TC_TestNewLC2L ),
        ENTRY( "TC_TestNewLC3L", CTestHybridTriggers::TC_TestNewLC3L ),
        ENTRY( "TC_TestNewLC4L", CTestHybridTriggers::TC_TestNewLC4L ),
        ENTRY( "TC_TestNewLC5L", CTestHybridTriggers::TC_TestNewLC5L ),
        ENTRY( "TC_TestSetHybridArea1L", CTestHybridTriggers::TC_TestSetHybridArea1L ),
        ENTRY( "TC_TestSetHybridArea2L", CTestHybridTriggers::TC_TestSetHybridArea2L ),
        ENTRY( "TC_TestSetHybridArea3L", CTestHybridTriggers::TC_TestSetHybridArea3L ),
        ENTRY( "TC_TestInternalizeAndExternalizeL", CTestHybridTriggers::TC_TestInternalizeAndExternalizeL ),
        ENTRY( "TC_TestCLbtTriggerConditionArea1L", CTestHybridTriggers::TC_TestCLbtTriggerConditionArea1L ),
        ENTRY( "TC_TestCLbtTriggerConditionArea2L", CTestHybridTriggers::TC_TestCLbtTriggerConditionArea2L ),
        ENTRY( "TC_TestCLbtTriggerConditionArea3L", CTestHybridTriggers::TC_TestCLbtTriggerConditionArea3L ),
        ENTRY( "TC_TestCLbtTriggerConditionArea4L", CTestHybridTriggers::TC_TestCLbtTriggerConditionArea4L ),
        ENTRY( "TC_TestCLbtTriggerFilterByArea1L", CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea1L ),
        ENTRY( "TC_TestCLbtTriggerFilterByArea2L", CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea2L ),
        ENTRY( "TC_TestCLbtTriggerFilterByArea3L", CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea3L ),
        ENTRY( "TC_TestCLbtTriggerFilterByArea4L", CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea4L ),
        ENTRY( "TC_TestCreatetrigger1L", CTestHybridTriggers::TC_TestCreatetrigger1L ),
        ENTRY( "TC_TestCreatetrigger2L", CTestHybridTriggers::TC_TestCreatetrigger2L ),
        ENTRY( "TC_TestCreatetrigger3L", CTestHybridTriggers::TC_TestCreatetrigger3L ),
        ENTRY( "TC_TestCreatetrigger4L", CTestHybridTriggers::TC_TestCreatetrigger4L ),
        ENTRY( "TC_TestDeleteTrigger1L", CTestHybridTriggers::TC_TestDeleteTrigger1L ),
        ENTRY( "TC_TestDeleteTrigger2L", CTestHybridTriggers::TC_TestDeleteTrigger2L ),
        ENTRY( "TC_TestDeleteTriggers1L", CTestHybridTriggers::TC_TestDeleteTriggers1L ),
        ENTRY( "TC_TestDeleteTriggers2L", CTestHybridTriggers::TC_TestDeleteTriggers2L ),
        ENTRY( "TC_TestDeleteTriggers3L", CTestHybridTriggers::TC_TestDeleteTriggers3L ),
        ENTRY( "TC_TestDeleteTriggers4L", CTestHybridTriggers::TC_TestDeleteTriggers4L ),
        ENTRY( "TC_TestDeleteTriggers5L", CTestHybridTriggers::TC_TestDeleteTriggers5L ),
        ENTRY( "TC_TestDeleteTriggers6L", CTestHybridTriggers::TC_TestDeleteTriggers6L ),
        ENTRY( "TC_TestDeleteTriggers7L", CTestHybridTriggers::TC_TestDeleteTriggers7L ),
        ENTRY( "TC_TestDeleteTriggers8L", CTestHybridTriggers::TC_TestDeleteTriggers8L ),
        ENTRY( "TC_TestDeleteTriggers9L", CTestHybridTriggers::TC_TestDeleteTriggers9L ),
        ENTRY( "TC_TestDeleteTriggers10L", CTestHybridTriggers::TC_TestDeleteTriggers10L ),
        ENTRY( "TC_TestDeleteTriggers11L", CTestHybridTriggers::TC_TestDeleteTriggers11L ),
        ENTRY( "TC_TestDeleteTriggers12L", CTestHybridTriggers::TC_TestDeleteTriggers12L ),
        ENTRY( "TC_TestGetTriggers1L", CTestHybridTriggers::TC_TestGetTriggers1L ),
        ENTRY( "TC_TestGetTriggers2L", CTestHybridTriggers::TC_TestGetTriggers2L ),
        ENTRY( "TC_TestGetTriggers3L", CTestHybridTriggers::TC_TestGetTriggers3L ),
        ENTRY( "TC_TestGetTriggers4L", CTestHybridTriggers::TC_TestGetTriggers4L ),
        ENTRY( "TC_TestGetTriggers5L", CTestHybridTriggers::TC_TestGetTriggers5L ),
        ENTRY( "TC_TestGetTriggers6L", CTestHybridTriggers::TC_TestGetTriggers6L ),
        ENTRY( "TC_TestGetTriggers7L", CTestHybridTriggers::TC_TestGetTriggers7L ),
        ENTRY( "TC_TestGetTriggers8L", CTestHybridTriggers::TC_TestGetTriggers8L ),
        ENTRY( "TC_TestGetTriggers9L", CTestHybridTriggers::TC_TestGetTriggers9L ),
        ENTRY( "TC_TestGetTriggers10L", CTestHybridTriggers::TC_TestGetTriggers10L ),
        ENTRY( "TC_TestGetTriggers11L", CTestHybridTriggers::TC_TestGetTriggers11L ),
        ENTRY( "TC_TestGetTriggers12L", CTestHybridTriggers::TC_TestGetTriggers12L ),
        ENTRY( "TC_TestGetTriggers_Iterator1L", CTestHybridTriggers::TC_TestGetTriggers_Iterator1L ),
        ENTRY( "TC_TestGetTriggers_Iterator2L", CTestHybridTriggers::TC_TestGetTriggers_Iterator2L ),
        ENTRY( "TC_TestGetTriggers_Iterator3L", CTestHybridTriggers::TC_TestGetTriggers_Iterator3L ),
        ENTRY( "TC_TestGetTriggers_Iterator4L", CTestHybridTriggers::TC_TestGetTriggers_Iterator4L ),
        ENTRY( "TC_TestGetTriggers_Iterator5L", CTestHybridTriggers::TC_TestGetTriggers_Iterator5L ),
        ENTRY( "TC_TestGetTriggers_Iterator6L", CTestHybridTriggers::TC_TestGetTriggers_Iterator6L ),
        ENTRY( "TC_TestGetTriggers_Iterator7L", CTestHybridTriggers::TC_TestGetTriggers_Iterator7L ),
        ENTRY( "TC_TestGetTriggers_Iterator8L", CTestHybridTriggers::TC_TestGetTriggers_Iterator8L ),
        ENTRY( "TC_TestGetTriggers_Iterator9L", CTestHybridTriggers::TC_TestGetTriggers_Iterator9L ),
        ENTRY( "TC_TestGetTriggers_Iterator10L", CTestHybridTriggers::TC_TestGetTriggers_Iterator10L ),
        ENTRY( "TC_TestGetTriggers_Iterator11L", CTestHybridTriggers::TC_TestGetTriggers_Iterator11L ),
        ENTRY( "TC_TestGetTriggers_Iterator12L", CTestHybridTriggers::TC_TestGetTriggers_Iterator12L ),
        ENTRY( "TC_TestSetTriggersState1L", CTestHybridTriggers::TC_TestSetTriggersState1L ),
        ENTRY( "TC_TestSetTriggersState2L", CTestHybridTriggers::TC_TestSetTriggersState2L ),
        ENTRY( "TC_TestSetTriggersState3L", CTestHybridTriggers::TC_TestSetTriggersState3L ),
        ENTRY( "TC_TestSetTriggersState4L", CTestHybridTriggers::TC_TestSetTriggersState4L ),
        ENTRY( "TC_TestSetTriggersState5L", CTestHybridTriggers::TC_TestSetTriggersState5L ),
        ENTRY( "TC_TestSetTriggersState6L", CTestHybridTriggers::TC_TestSetTriggersState6L ),
        ENTRY( "TC_TestSetTriggersState7L", CTestHybridTriggers::TC_TestSetTriggersState7L ),
        ENTRY( "TC_TestSetTriggersState8L", CTestHybridTriggers::TC_TestSetTriggersState8L ),
        ENTRY( "TC_TestSetTriggersState9L", CTestHybridTriggers::TC_TestSetTriggersState9L ),
        ENTRY( "TC_TestSetTriggersState10L", CTestHybridTriggers::TC_TestSetTriggersState10L ),
        ENTRY( "TC_TestSetTriggersState11L", CTestHybridTriggers::TC_TestSetTriggersState11L ),
        ENTRY( "TC_TestSetTriggersState12L", CTestHybridTriggers::TC_TestSetTriggersState12L ),
        ENTRY( "TC_TestSetTriggerState1L", CTestHybridTriggers::TC_TestSetTriggerState1L ),
        ENTRY( "TC_TestListtriggerIds1L", CTestHybridTriggers::TC_TestListtriggerIds1L ),
        ENTRY( "TC_TestListtriggerIds2L", CTestHybridTriggers::TC_TestListtriggerIds2L ),
        ENTRY( "TC_TestListtriggerIds3L", CTestHybridTriggers::TC_TestListtriggerIds3L ),
        ENTRY( "TC_TestListtriggerIds4L", CTestHybridTriggers::TC_TestListtriggerIds4L ),
        ENTRY( "TC_TestListtriggerIds5L", CTestHybridTriggers::TC_TestListtriggerIds5L ),
        ENTRY( "TC_TestListtriggerIds6L", CTestHybridTriggers::TC_TestListtriggerIds6L ),
        ENTRY( "TC_TestListtriggerIds7L", CTestHybridTriggers::TC_TestListtriggerIds7L ),
        ENTRY( "TC_TestListtriggerIds8L", CTestHybridTriggers::TC_TestListtriggerIds8L ),
        ENTRY( "TC_TestListtriggerIds9L", CTestHybridTriggers::TC_TestListtriggerIds9L ),
        ENTRY( "TC_TestListtriggerIds10L", CTestHybridTriggers::TC_TestListtriggerIds10L ),
        ENTRY( "TC_TestListtriggerIds11L", CTestHybridTriggers::TC_TestListtriggerIds11L ),
        ENTRY( "TC_TestListtriggerIds12L", CTestHybridTriggers::TC_TestListtriggerIds12L ),
        ENTRY( "TC_TestGettriggerLC1L", CTestHybridTriggers::TC_TestGettriggerLC1L ),
        ENTRY( "TC_TestGettriggerLC2L", CTestHybridTriggers::TC_TestGettriggerLC2L ),
        ENTRY( "TC_TestUpdateTrigger1L", CTestHybridTriggers::TC_TestUpdateTrigger1L ),
        ENTRY( "TC_TestUpdateTrigger2L", CTestHybridTriggers::TC_TestUpdateTrigger2L ),
        ENTRY( "TC_TestUpdateTrigger3L", CTestHybridTriggers::TC_TestUpdateTrigger3L ),
        ENTRY( "TC_TestUpdateTrigger4L", CTestHybridTriggers::TC_TestUpdateTrigger4L ),
        ENTRY( "TC_TestUpdateTrigger5L", CTestHybridTriggers::TC_TestUpdateTrigger5L ),
        ENTRY( "TC_TestUpdateTrigger6L", CTestHybridTriggers::TC_TestUpdateTrigger6L ),
        ENTRY( "TC_TestHybridTriggerFiring1L", CTestHybridTriggers::TC_TestHybridTriggerFiring1L ),
        ENTRY( "TC_TestHybridTriggerFiring2L", CTestHybridTriggers::TC_TestHybridTriggerFiring2L ),
        ENTRY( "StartCellChangeNotifierL", CTestHybridTriggers::StartCellChangeNotifierL ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

void CTestHybridTriggers::AssertTrueL( TBool aCondition,const TDesC& aMessage )
    {
           
         if(!aCondition)
         {
          
           iLog->Log( aMessage );
           User::LeaveIfError(KErrGeneral);
         }
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewL1L
// Testing CLbtGeoWlan::NewL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestNewL1L( CStifItemParser& aItem )
    {

    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL();
    //Push to cleanup stack
    CleanupStack::PushL( Hybridarea );
        
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
    Hybridareaarray1.Append(Cellarea1);
    Hybridareaarray1.Append(Cellarea2);
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2;
    
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==0,_L("wrong array count"));
    Hybridarea->SetHybridArea(Hybridareaarray1);
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
        
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));      
    CleanupStack::PopAndDestroy( Hybridarea ); 
        
    return KErrNone;    

    }


// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewL2L
// Testing CLbtGeoWlan::NewL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestNewL2L( CStifItemParser& aItem )
    {

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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
    Hybridareaarray1.Append(Cellarea1);
    Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    //Push to cleanup stack
    CleanupStack::PushL( Hybridarea );
        
    
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2;
    
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
    
    TBuf<4> Networkcountrycode3 = _L("3000");
    TBuf<8> Networkidentitycode3 = _L("30000000");
    TUint Locationareacode3 = 300;
    TUint CellId3 = 300;
        
    CLbtGeoCell* Cellarea3  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                        Networkcountrycode3,
                                                        Networkidentitycode3,
                                                        Locationareacode3,
                                                        CellId3
                                                            );
                                                            
    TBuf<4> Networkcountrycode4 = _L("4000");
    TBuf<8> Networkidentitycode4 = _L("40000000");
    TUint Locationareacode4 = 400;
    TUint CellId4 = 400;
        
    CLbtGeoCell* Cellarea4  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                        Networkcountrycode4,
                                                        Networkidentitycode4,
                                                        Locationareacode4,
                                                        CellId4
                                                            );
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3 ;
   // Hybridareaarray3.Append(Cellarea1);
   // Hybridareaarray3.Append(Cellarea2);
    Hybridareaarray3.Append(Cellarea3);
    Hybridareaarray3.Append(Cellarea4);
    Hybridarea->SetHybridArea(Hybridareaarray3);
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
        
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode3,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode3,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode3,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId3,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode4,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode4,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode4,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId4,_L("Wrong Cellid"));      
    
    /*base=Hybridareaarray2[2];
                
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode3,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode3,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode3,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId3,_L("Wrong Cellid"));   */   
    CleanupStack::PopAndDestroy( Hybridarea ); 
        
    return KErrNone;    

    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewL3L
// Testing CLbtGeoWlan::NewL() with circular area as part of the Hybrid area array
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestNewL3L( CStifItemParser& aItem )
    {

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
    TCoordinate coordinate(62.5285,23.9385);
        
    CLbtGeoCircle* circlearea=CLbtGeoCircle::NewL(coordinate,1000);
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
    Hybridareaarray1.Append(Cellarea1);
    Hybridareaarray1.Append(circlearea);
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    //Push to cleanup stack
    CleanupStack::PushL( Hybridarea );
        
    
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2;
    
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
      
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
        
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    CLbtGeoCircle* circle2 =NULL;
    if(base->Type()==CLbtGeoAreaBase::ECircle)
    {
    circle2= static_cast<CLbtGeoCircle*> (base);
    }    
    
    TCoordinate center2 = circle2->Center();
    TReal32 distance;
    center2.Distance(TCoordinate( 62.5285,23.9385,0 ),distance);
    if(distance!=0)
    {
        return KErrGeneral;
    }
    TReal radius = circle2->Radius();
    if(radius != 1000)
    {
        return KErrGeneral;
    }
   
    CleanupStack::PopAndDestroy( Hybridarea ); 
        
    return KErrNone;    

    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewL4L
// Testing CLbtGeoWlan::NewL() using empty array
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestNewL4L( CStifItemParser& aItem )
    {
 
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
           
    return KErrNone;    

    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewL5L
// Testing CLbtGeoWlan::NewL() with CLbtGeoRect as one of the arae in the array
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestNewL5L( CStifItemParser& aItem )
    {
 
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
    CLbtGeoRect* Rectarea = CLbtGeoRect::NewL( 20,70,20,70);
    
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
    Hybridareaarray1.Append(Cellarea1);
    Hybridareaarray1.Append(Rectarea);
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
		return KErrNone;
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewLC1L
// Testing CLbtGeoWlan::NewLC()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestNewLC1L( CStifItemParser& aItem )
    {

    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewLC();
    
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
            Hybridareaarray1.Append(Cellarea1);
            Hybridareaarray1.Append(Cellarea2);
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2;
    
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==0,_L("wrong array count"));
    Hybridarea->SetHybridArea(Hybridareaarray1);
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
        
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));      
    CleanupStack::PopAndDestroy( Hybridarea ); 
        
    return KErrNone;    

    }


// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewLC2L
// Testing CLbtGeoWlan::NewLC()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestNewLC2L( CStifItemParser& aItem )
    {

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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                Hybridareaarray1.Append(Cellarea1);
                Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewLC(Hybridareaarray1);
    
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2;
    
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
    
    TBuf<4> Networkcountrycode3 = _L("3000");
    TBuf<8> Networkidentitycode3 = _L("30000000");
    TUint Locationareacode3 = 300;
    TUint CellId3 = 200;
        
    CLbtGeoCell* Cellarea3  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                        Networkcountrycode3,
                                                        Networkidentitycode3,
                                                        Locationareacode3,
                                                        CellId3
                                                            );
    TBuf<4> Networkcountrycode4 = _L("4000");
    TBuf<8> Networkidentitycode4 = _L("40000000");
    TUint Locationareacode4 = 400;
    TUint CellId4 = 400;
        
    CLbtGeoCell* Cellarea4  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                        Networkcountrycode4,
                                                        Networkidentitycode4,
                                                        Locationareacode4,
                                                        CellId4
                                                            );
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3 ;
   // Hybridareaarray3.Append(Cellarea1);
   // Hybridareaarray3.Append(Cellarea2);
    Hybridareaarray3.Append(Cellarea3);
    Hybridareaarray3.Append(Cellarea4);
    Hybridarea->SetHybridArea(Hybridareaarray3);
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
        
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode3,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode3,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode3,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId3,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode4,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode4,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode4,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId4,_L("Wrong Cellid"));      
    
    
    CleanupStack::PopAndDestroy( Hybridarea ); 
    return KErrNone;    

    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewL3L
// Testing CLbtGeoWlan::NewL() with circular area as part of the Hybrid area array
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestNewLC3L( CStifItemParser& aItem )
    {

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
    TCoordinate coordinate(62.5285,23.9385);
        
    CLbtGeoCircle* circlearea=CLbtGeoCircle::NewL(coordinate,1000);
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                    Hybridareaarray1.Append(Cellarea1);
                    Hybridareaarray1.Append(circlearea);
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewLC(Hybridareaarray1);
    
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2;
    
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
      
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
        
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    CLbtGeoCircle* circle2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECircle)
    {
    circle2= static_cast<CLbtGeoCircle*> (base);
    }    
    
    TCoordinate center2 = circle2->Center();
    TReal32 distance;
    center2.Distance(TCoordinate( 62.5285,23.9385,0 ),distance);
    if(distance!=0)
    {
        return KErrGeneral;
    }
    TReal radius = circle2->Radius();
    if(radius != 1000)
    {
        return KErrGeneral;
    }
   
    CleanupStack::PopAndDestroy( Hybridarea ); 
        
    return KErrNone;    

    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewLC3L
// Testing CLbtGeoWlan::NewLC() using empty array
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestNewLC4L( CStifItemParser& aItem )
    {
 
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewLC(Hybridareaarray1);
        
    return KErrNone;    

    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewLC5L
// Testing CLbtGeoWlan::NewLC() with CLbtGeoRect as one of the arae in the array
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestNewLC5L( CStifItemParser& aItem )
    {
 
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
    CLbtGeoRect* Rectarea = CLbtGeoRect::NewL( 20,70,20,70);
    
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                        Hybridareaarray1.Append(Cellarea1);
                        Hybridareaarray1.Append(Rectarea);
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewLC(Hybridareaarray1);
		return KErrNone;
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetHybridArea1L
// Testing SetHybridArea with one cellular area and one circular area
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestSetHybridArea1L( CStifItemParser& aItem )
    {

    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL();
    //Push to cleanup stack
    CleanupStack::PushL( Hybridarea );
        
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
    TCoordinate coordinate(62.5285,23.9385);
           
    CLbtGeoCircle* circlearea=CLbtGeoCircle::NewL(coordinate,1000);
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                            Hybridareaarray1.Append(Cellarea1);
                            Hybridareaarray1.Append(circlearea);
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2;
    
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==0,_L("wrong array count"));
    Hybridarea->SetHybridArea(Hybridareaarray1);
    Hybridareaarray2 = Hybridarea->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
        
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
                
    CLbtGeoCircle* circle2 =NULL;
    if(base->Type()==CLbtGeoAreaBase::ECircle)
    {
    circle2= static_cast<CLbtGeoCircle*> (base);
    }    
    
    TCoordinate center2 = circle2->Center();
    TReal32 distance;
    center2.Distance(TCoordinate( 62.5285,23.9385,0 ),distance);
    if(distance!=0)
    {
        return KErrGeneral;
    }
    TReal radius = circle2->Radius();
    if(radius != 1000)
    {
        return KErrGeneral;
    }
    CleanupStack::PopAndDestroy( Hybridarea ); 
        
    return KErrNone;    

    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewL4L
// Testing SetHybridArea with empty array
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestSetHybridArea2L( CStifItemParser& aItem )
    {
 
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL();
    Hybridarea->SetHybridArea(Hybridareaarray1);
           
    return KErrNone;    

    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestNewL5L
// Testing SetHybridArea with CLbtGeoRect as one of the arae in the array
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestSetHybridArea3L( CStifItemParser& aItem )
    {
 
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
    CLbtGeoRect* Rectarea = CLbtGeoRect::NewL( 20,70,20,70);
    
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                                Hybridareaarray1.Append(Cellarea1);
                                Hybridareaarray1.Append(Rectarea);
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6); 
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL();
    Hybridarea->SetHybridArea(Hybridareaarray1);
		return KErrNone;
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestInternalizeAndExternalizeL
// Calling Bssid() function by passing index greater than number of elements in the array
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestInternalizeAndExternalizeL( CStifItemParser& aItem )
    {

    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewLC();
        
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
    Hybridareaarray1.Append(Cellarea1);
    Hybridareaarray1.Append(Cellarea2);
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2;
    Hybridarea->SetHybridArea(Hybridareaarray1);
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
   
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    Hybridarea->ExternalizeL(wr);
   
    wr.CommitL();
    CleanupStack::PopAndDestroy(&wr);

    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL();
       
    //Push to cleanup stack
    CleanupStack::PushL( Hybridarea2 ); 
   
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
   
    Hybridarea2->InternalizeL(rd);
   
    CleanupStack::PopAndDestroy(&rd);
   
    buffer->Reset();
   
    Hybridareaarray2 = Hybridarea2->HybridArea();
    AssertTrueL(Hybridareaarray2.Count()==2,_L("wrong array count"));
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
        
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));      
          
    CleanupStack::PopAndDestroy( 3 );
        
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCLbtTriggerConditionArea1L
// Allocates and constructs a new Trigger Condition area object using CLbtGeoHybrid 1
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestCLbtTriggerConditionArea1L( CStifItemParser& aItem )
    {

    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
  
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
    Hybridareaarray1.Append(Cellarea1);
    Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
        
    cond1->SetTriggerArea( Hybridarea );
    
        
    CLbtGeoAreaBase* TrigArea=cond1->TriggerArea();
    
    AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
    CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));      
    
    CleanupStack::PopAndDestroy( 1 );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCLbtTriggerConditionArea2L
// Allocates and constructs a new Trigger Condition area object using CLbtGeoWlan 2
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestCLbtTriggerConditionArea2L( CStifItemParser& aItem )
    {

    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewLC( );
   
   cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
 
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
   RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
   Hybridareaarray1.Append(Cellarea1);
   Hybridareaarray1.Append(Cellarea2);
   CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
       
   cond1->SetTriggerArea( Hybridarea );
   
       
   CLbtGeoAreaBase* TrigArea=cond1->TriggerArea();
   
   AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
   CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
       RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
   
   CLbtGeoAreaBase* base=Hybridareaarray2[0];
           
   CLbtGeoCell* cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
   
   base=Hybridareaarray2[1];
           
   cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));      
   
   CleanupStack::PopAndDestroy( 1 );
   
   return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCLbtTriggerConditionArea3L
// Allocates and constructs a new Trigger Condition area object using CLbtGeoWlan 3
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestCLbtTriggerConditionArea3L( CStifItemParser& aItem )
    {

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
   RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
   Hybridareaarray1.Append(Cellarea1);
   Hybridareaarray1.Append(Cellarea2);
   CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL(Hybridarea,CLbtTriggerConditionArea::EFireOnExit );
    CleanupStack::PushL( cond1 ); 
        
    CLbtGeoAreaBase* TrigArea=cond1->TriggerArea();
       
   AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
   CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
       RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
   
   CLbtGeoAreaBase* base=Hybridareaarray2[0];
           
   CLbtGeoCell* cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
   
   base=Hybridareaarray2[1];
           
   cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
    
    CleanupStack::PopAndDestroy( 1 );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCLbtTriggerConditionArea4L
// Internalize & Externalize (ClbtTriggerConditionArea)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//


TInt CTestHybridTriggers::TC_TestCLbtTriggerConditionArea4L( CStifItemParser& /* aItem */ )
    {
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
    
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
   RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
   Hybridareaarray1.Append(Cellarea1);
   Hybridareaarray1.Append(Cellarea2);
   CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
       
   cond1->SetTriggerArea( Hybridarea );
    
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
    
    CLbtGeoAreaBase* TrigArea=cond2->TriggerArea();
           
   AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
   CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
       RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
   
   CLbtGeoAreaBase* base=Hybridareaarray2[0];
           
   CLbtGeoCell* cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
   
   base=Hybridareaarray2[1];
           
   cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
             
   
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea1L
// Allocates and constructs a new CLbtTriggerFilterByArea object using CLbtGeoHybrid 1
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea1L( CStifItemParser& /* aItem */ )
    {
            
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL();   
    CleanupStack::PushL( areafilter ); 
    
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
   RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
   Hybridareaarray1.Append(Cellarea1);
   Hybridareaarray1.Append(Cellarea2);
   CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    areafilter->SetArea(Hybridarea);
    
  
    CLbtGeoAreaBase* filterarea=areafilter->Area();
    AssertTrueL(filterarea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
            
    CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(filterarea); 
        RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
       
   CLbtGeoAreaBase* base=Hybridareaarray2[0];
           
   CLbtGeoCell* cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
   
   base=Hybridareaarray2[1];
           
   cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
             
    CleanupStack::PopAndDestroy();

    return KErrNone;
     }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea2L
// Allocates and constructs a new CLbtTriggerFilterByArea object using CLbtGeoHybrid 1
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea2L( CStifItemParser& /* aItem */ )
    {
            
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewLC();   
    
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
   RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
   Hybridareaarray1.Append(Cellarea1);
   Hybridareaarray1.Append(Cellarea2);
   CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    areafilter->SetArea(Hybridarea);
    
  
    CLbtGeoAreaBase* filterarea=areafilter->Area();
    AssertTrueL(filterarea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
            
    CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(filterarea); 
        RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
       
   CLbtGeoAreaBase* base=Hybridareaarray2[0];
           
   CLbtGeoCell* cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
   
   base=Hybridareaarray2[1];
           
   cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
             
    CleanupStack::PopAndDestroy(1);

    return KErrNone;
     }


// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea3L
// Allocates and constructs a new CLbtTriggerFilterByArea object using CLbtGeoWlan 3
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea3L( CStifItemParser& /* aItem */ )
    {
            
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
      RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea); 
    CleanupStack::PushL( areafilter ); 
  
    CLbtGeoAreaBase* filterarea=areafilter->Area();
        AssertTrueL(filterarea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
                
        CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(filterarea); 
            RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
           
       CLbtGeoAreaBase* base=Hybridareaarray2[0];
               
       CLbtGeoCell* cell2=NULL;
       if(base->Type()==CLbtGeoAreaBase::ECellular)
       {
           cell2= static_cast<CLbtGeoCell*> (base);
       }    
       
       AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
       AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
       AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
       AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
       AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
       
       base=Hybridareaarray2[1];
               
       cell2=NULL;
       if(base->Type()==CLbtGeoAreaBase::ECellular)
       {
           cell2= static_cast<CLbtGeoCell*> (base);
       }    
       
       AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
       AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
       AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
       AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
       AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
    CleanupStack::PopAndDestroy();

    return KErrNone;
     }
// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea4L
// Internalize & Externalize(CLbtTriggerFilterByArea).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestCLbtTriggerFilterByArea4L( CStifItemParser& /* aItem */ )
    {
            
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
      RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
       
    CLbtTriggerFilterByArea* filterarea1= CLbtTriggerFilterByArea::NewL( Hybridarea );
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
   
    CLbtGeoAreaBase* filterareabase=filterarea2->Area();
    AssertTrueL(filterareabase->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
            
    CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(filterareabase); 
        RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
       
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
           
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
        {
       cell2= static_cast<CLbtGeoCell*> (base);
        }    
   
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
   
    base=Hybridareaarray2[1];
           
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
        {
       cell2= static_cast<CLbtGeoCell*> (base);
        }    
   
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
    CleanupStack::Pop(filterarea2);
    CleanupStack::Pop(buffer);
    CleanupStack::Pop(filterarea1);
    return KErrNone;
     }


// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCreatetrigger1L
// Creation of hybrid Entry type startup trigger
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestCreatetrigger1L(CStifItemParser& /*aItem*/)
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
     TRAP_IGNORE(lbt.DeleteTriggersL());
      //Construct a startup trigger
     CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
     CleanupStack::PushL( trig );
     iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
     trig->SetNameL(_L("Trigger1"));
   
     _LIT( KMyTriggerHandlingProcessName, "About.exe");
  
    
     TSecureId secureid;
     trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
     //set Requestor     
     CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
     CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
     TBuf<KLbtMaxNameLength> ReqData=_L("");
     trig->SetRequestorL(ReqType,ReqFormat,ReqData);
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
       RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
       Hybridareaarray1.Append(Cellarea1);
       Hybridareaarray1.Append(Cellarea2);
       CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    
    // ownership of Cellarea object transferred to the condition object
     CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                 Hybridarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
     trig->SetCondition(condition); // ownership transferred to object
    
     TLbtTriggerId trigId;
        
        
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
     CleanupStack::PushL( notifier );
    
     CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
     notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
     wait->Start( );
     TInt err =notifier->iStatus.Int(); 
     if(err!=KErrNone)
         {
         User::LeaveIfError(err);
         }
     iLog->Log(_L("Trigger Created"));
    
      
     CLbtTriggerInfo *Triginfo ;
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
     return KErrNotFound;  
     }
     else
     {
     CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
     CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
     CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
         RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
     CleanupStack::Pop(Triginfo);
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy( trig );
     CleanupStack::Pop( &lbt );
     CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
     return KErrNone;
     }
   
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCreatetrigger2L
// Creation of hybrid Entry type session trigger
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestCreatetrigger2L(CStifItemParser& /*aItem*/)
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
     CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
     CleanupStack::PushL( trig );
     iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
     trig->SetNameL(_L("Trigger1"));
    
     //set Requestor     
     CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
     CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
     TBuf<KLbtMaxNameLength> ReqData=_L("");
     trig->SetRequestorL(ReqType,ReqFormat,ReqData);
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
       RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
              Hybridareaarray1.Append(Cellarea1);
              Hybridareaarray1.Append(Cellarea2);
       CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    
    // ownership of Cellarea object transferred to the condition object
     CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                 Hybridarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
     trig->SetCondition(condition); // ownership transferred to object
    
     TLbtTriggerId trigId;
        
        
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
     CleanupStack::PushL( notifier );
    
     CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
     notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
     wait->Start( );
     iLog->Log(_L("Trigger Created"));
    
      
     CLbtTriggerInfo *Triginfo ;
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
     return KErrNotFound;  
     }
     else
     {
     CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
     CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
     CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
         RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
     CleanupStack::Pop(Triginfo);
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy( trig );
     CleanupStack::Pop( &lbt );
     CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
     return KErrNone;
     }
   
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCreatetrigger3L
// Creation of hybrid Entry type startup trigger with one circular area and one cellular area
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestCreatetrigger3L(CStifItemParser& /*aItem*/)
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
  
     TSecureId secureid;
     trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
     //set Requestor     
     CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
     CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
     TBuf<KLbtMaxNameLength> ReqData=_L("");
     trig->SetRequestorL(ReqType,ReqFormat,ReqData);
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
       TCoordinate coordinate(62.5285,23.9385);
       TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6);            
       CLbtGeoCircle* circlearea=CLbtGeoCircle::NewL(coordinate,1000);
       RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
              Hybridareaarray1.Append(Cellarea1);
              Hybridareaarray1.Append(circlearea);
       CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    
    // ownership of Cellarea object transferred to the condition object
     CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                 Hybridarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
     trig->SetCondition(condition); // ownership transferred to object
    
     TLbtTriggerId trigId;
        
        
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
     CleanupStack::PushL( notifier );
    
     CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
     notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
     wait->Start( );
     iLog->Log(_L("Trigger Created"));
    
      
     CLbtTriggerInfo *Triginfo ;
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
     return KErrNotFound;  
     }
     else
     {
     CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
     CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
     CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
         RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
                    
    CLbtGeoCircle* circle2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECircle)
    {
    circle2= static_cast<CLbtGeoCircle*> (base);
    }    
    
    TCoordinate center2 = circle2->Center();
    TReal32 distance;
    center2.Distance(TCoordinate( 62.5285,23.9385,0 ),distance);
    if(distance!=0)
    {
        return KErrGeneral;
    }
    TReal radius = circle2->Radius();
    if(radius != 1000)
    {
        return KErrGeneral;
    }
     CleanupStack::Pop(Triginfo);
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy( trig );
     CleanupStack::Pop( &lbt );
     CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
     return KErrNone;
     }
   
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestCreatetrigger4L
// Creation of hybrid Entry type session trigger with one circular area and one cellular area
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestCreatetrigger4L(CStifItemParser& /*aItem*/)
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
         CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
        
        //Push to cleanup stack
         CleanupStack::PushL( trig );
         iLog->Log(_L("Startup Trigger Entry Created "));
        
        // Set Name
         trig->SetNameL(_L("Trigger1"));
       
                  
         //set Requestor     
         CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
         CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
         TBuf<KLbtMaxNameLength> ReqData=_L("");
         trig->SetRequestorL(ReqType,ReqFormat,ReqData);
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
           TCoordinate coordinate(62.5285,23.9385);
           TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 6);            
           CLbtGeoCircle* circlearea=CLbtGeoCircle::NewL(coordinate,1000);
           RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                         Hybridareaarray1.Append(Cellarea1);
                         Hybridareaarray1.Append(circlearea);
           CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
        
        
        // ownership of Cellarea object transferred to the condition object
         CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                     Hybridarea,
                                                    CLbtTriggerConditionArea::EFireOnEnter);
            
         trig->SetCondition(condition); // ownership transferred to object
        
         TLbtTriggerId trigId;
            
            
         CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
         CleanupStack::PushL( notifier );
        
         CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
            
         notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
         wait->Start( );
         iLog->Log(_L("Trigger Created"));
        
          
         CLbtTriggerInfo *Triginfo ;
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
         return KErrNotFound;  
         }
         else
         {
         CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
         CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
         CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
         AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
         CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
             RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
        
        CLbtGeoAreaBase* base=Hybridareaarray2[0];
                
        CLbtGeoCell* cell2=NULL;
        if(base->Type()==CLbtGeoAreaBase::ECellular)
        {
            cell2= static_cast<CLbtGeoCell*> (base);
        }    
        
        AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
        AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
        AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
        AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
        AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
        
        base=Hybridareaarray2[1];
                        
        CLbtGeoCircle* circle2=NULL;
        if(base->Type()==CLbtGeoAreaBase::ECircle)
        {
        circle2= static_cast<CLbtGeoCircle*> (base);
        }    
        
        TCoordinate center2 = circle2->Center();
        TReal32 distance;
        center2.Distance(TCoordinate( 62.5285,23.9385,0 ),distance);
        if(distance!=0)
        {
            return KErrGeneral;
        }
        TReal radius = circle2->Radius();
        if(radius != 1000)
        {
            return KErrGeneral;
        }
         CleanupStack::Pop(Triginfo);
         CleanupStack::PopAndDestroy( notifier );
         CleanupStack::PopAndDestroy( trig );
         CleanupStack::Pop( &lbt );
         CleanupStack::PopAndDestroy( &lbtserver );
        //delete wait;
         return KErrNone;
         }
   
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTrigger1L
// Deletion of Hybrid startup trigger
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTrigger1L(CStifItemParser& /*aItem*/)
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
   
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    TBuf<4> Networkcountrycode1 = _L("1000");
        TBuf<8> Networkidentitycode1 = _L("10000000");
        TUint Locationareacode1 = 100;
        TUint CellId1 = 100;   
    // set condition
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                  Hybridareaarray1.Append(Cellarea1);
                  Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Hybridarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object

    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    lbt.DeleteTriggerL(trigId);
    
   // CLbtTriggerInfo *Triginfo ;
    iLog->Log(_L("Before GetTriggerLC "));
    
    TRAPD(error, lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(  ););
    
    if(error==KErrNotFound)
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
    return KErrGeneral;
    }
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTrigger2L
// Deletion of Hybrid session trigger
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTrigger2L(CStifItemParser& /*aItem*/)
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
     CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
   
   
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Hybridarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    trig->SetCondition(condition); // ownership transferred to object

    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    TLbtTriggerId trigId;
        
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    lbt.DeleteTriggerL(trigId);
    
  //  CLbtTriggerInfo *Triginfo ;
    iLog->Log(_L("Before GetTriggerLC "));
    
    TRAPD(error, lbt.GetTriggerLC(trigId);CleanupStack::PopAndDestroy(  ););
    
    if(error==KErrNotFound)
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
    return KErrGeneral;
    }
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers1L
// Deleting triggers using NULL filter synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers1L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
     
    TRAP_IGNORE(lbt.DeleteTriggersL());
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);
        
    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
     
    lbt.DeleteTriggersL();
    
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error,  lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ));
    TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ));
    if(error==KErrNotFound && error2==KErrNotFound)
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    }
    else
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrGeneral;
    }
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers2L
// Deleting triggers in given Cellular using NULL filter asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers2L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
     
    TRAP_IGNORE(lbt.DeleteTriggersL());
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
     
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);

    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    notifier->DeleteFilteredTriggers(lbt,NULL,wait);
    wait->Start();
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ));
    TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ));
    if(error==KErrNotFound && error2==KErrNotFound)
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    }
    else
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrGeneral;
    }
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers3L
// Deleting triggers using Area filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers3L(CStifItemParser& /*aItem*/)
    {
    return TC_TestDeleteTriggers3L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers4L
// Deleting triggers in given Cellular using Area filter asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers4L(CStifItemParser& /*aItem*/)
    {
    return TC_TestDeleteTriggers4L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers3L
// Deleting triggers using Area filter  constructed using Multiple CGI Area synchronously(when both the cells are not matching)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers5L(CStifItemParser& /*aItem*/)
    {
    return TC_TestDeleteTriggers5L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers6L
// Deleting triggers in given Hybrid area using Area filter asynchronously(when both the cells are not matching)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers6L(CStifItemParser& /*aItem*/)
    {
    return TC_TestDeleteTriggers6L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers7L
// Deleting triggers using Area filter  constructed using Multiple CGI Area synchronously(when one of the cells are not matching)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers7L(CStifItemParser& /*aItem*/)
    {
    return TC_TestDeleteTriggers7L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers8L
// Deleting triggers in given Hybrid using Area filter asynchronously(when one of the cells are not matching)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers8L(CStifItemParser& /*aItem*/)
    {
    return TC_TestDeleteTriggers8L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers9L
// Deleting triggers using composite filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers9L(CStifItemParser& /*aItem*/)
    {
    return TC_TestDeleteTriggers9L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers10L
// Deleting triggers in given Hybrid using composite filter asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers10L(CStifItemParser& /*aItem*/)
    {
    return TC_TestDeleteTriggers10L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers11L
// Deleting triggers using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers11L(CStifItemParser& /*aItem*/)
    {
    return TC_TestDeleteTriggers11L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers12L
// Deleting triggers using composite filter  constructed using Two Multiple CGI Area filters asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers12L(CStifItemParser& /*aItem*/)
    {
    return TC_TestDeleteTriggers12L();
    
    }



// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers5L
// listing triggers  using NULL filter synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers1L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
    TRAP_IGNORE(lbt.DeleteTriggersL());
 
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
    
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,NULL );
    
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    //Check area of the trigger
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

    CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
              RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));                
    }
    iLog->Log(_L("All checkings done "));    
    //free memory
    for ( TInt i = 0; i < count; i++ )
    {
    delete trigInfoList[i];
    }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers2L
// Listing triggers using NULL filter synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers2L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
     
    TRAP_IGNORE(lbt.DeleteTriggersL());
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,NULL);
    wait->Start();
   
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    //Check area of the trigger
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

    CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
              RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
    }
    iLog->Log(_L("All checkings done "));    
    //free memory
    for ( TInt i = 0; i < count; i++ )
    {
    delete trigInfoList[i];
    }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers3L
// listing triggers  using Area filter synchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers3L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers3L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers4L
// listing triggers  using Area filter asynchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers4L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers4L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers5L
// listing triggers  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers5L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers5L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers6L
// listing triggers  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers6L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers6L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers7L
// listing triggers  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers7L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers7L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers8L
// listing triggers  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers8L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers8L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers3L
// Deleting triggers using composite filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers9L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers9L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers10L
// listing triggers using composite filter  constructed using Multiple CGI Area asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers10L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers10L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers11L
// Listing triggers using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers11L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers11L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers12L
// Listing triggers using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers12L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers12L();
    }


// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator1L
// listing triggers using NULL filter synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator1L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
    TRAP_IGNORE(lbt.DeleteTriggersL());
 
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
     
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    RArray<CLbtTriggerInfo> trigInfoList;
    lbt.CreateGetTriggerIteratorL(NULL);
    
    TInt i=0;
    CLbtTriggerInfo* trigInfo = lbt.GetNextTriggerLC(); 
    while ( trigInfo != NULL )
        {
            CleanupStack::Pop(trigInfo);
            i++;
            trigInfoList.Append(*trigInfo);
         // CleanupStack::PopAndDestroy(trigInfo);
          trigInfo = lbt.GetNextTriggerLC(); 
        }
    
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i].TriggerEntry();
    //Check area of the trigger
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

    CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
              RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
    }
    iLog->Log(_L("All checkings done "));    
    //free memory
    
    trigInfoList.Close();
    CleanupStack::Pop(trigInfo);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator2L
// listing triggers using NULL filter asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator2L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
    TRAP_IGNORE(lbt.DeleteTriggersL()); 
 
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
        
    RArray<CLbtTriggerInfo> trigInfoList;
    notifier->CreateIterator(lbt,wait,NULL);
    wait->Start( );
    
    TInt i=1;
    CLbtTriggerInfo* trigInfo = lbt.GetNextTriggerLC(); 
    while ( trigInfo != NULL )
        {
            CleanupStack::Pop(trigInfo);
            i++;
            trigInfoList.Append(*trigInfo);
         // CleanupStack::PopAndDestroy(trigInfo);
          trigInfo = lbt.GetNextTriggerLC(); 
        }
    
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i].TriggerEntry();
    //Check area of the trigger
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

    CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
              RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
    }
    iLog->Log(_L("All checkings done "));    
    //free memory
   
    trigInfoList.Close();
    CleanupStack::Pop(trigInfo);
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator3L
// listing triggers using Area filter synchronously using iterator
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator3L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers_Iterator3L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator4L
// listing triggers using Area filter asynchronously using iterator
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator4L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers_Iterator4L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator5L
// listing triggers  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator5L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers_Iterator5L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers6L
// listing triggers  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator6L(CStifItemParser& /*aItem*/)
    {
    
    return TC_TestGetTriggers_Iterator6L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers7L
// listing triggers  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator7L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers_Iterator7L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers8L
// listing triggers  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator8L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers_Iterator8L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers9L
// Deleting triggers using composite filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator9L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers_Iterator9L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers10L
// listing triggers using composite filter  constructed using Multiple CGI Area asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator10L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers_Iterator10L();
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator11L
// Listing triggers using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator11L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers_Iterator11L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers12L
// Listing triggers using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator12L(CStifItemParser& /*aItem*/)
    {
    return TC_TestGetTriggers_Iterator12L();
    }


// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState1L
// listing triggers using NULL filter synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState1L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
     
    TRAP_IGNORE(lbt.DeleteTriggersL());
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
        
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,NULL);   
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,NULL );
    TBool err=EFalse;
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    //Check area of the trigger
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

    CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
              RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
   
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=CLbtTriggerEntry::EStateEnabled)
    {
    err = ETrue;
    }
    }
    iLog->Log(_L("All checkings done "));    
    //free memory
    for ( TInt i = 0; i < count; i++ )
    {
    delete trigInfoList[i];
    }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    if(err)
    {
    iLog->Log(_L("State not changed "));    
    return KErrGeneral;
    }
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggerState2L
// listing triggers using NULL filter synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState2L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
     
    TRAP_IGNORE(lbt.DeleteTriggersL());
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
    Hybridareaarray1.Append(Cellarea1);
    Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateDisabled,wait,NULL);
    wait->Start();
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,NULL );
    TBool err=EFalse;
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    //Check area of the trigger
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
    CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
              RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 ==CLbtTriggerEntry::EStateEnabled)
    {
    err = ETrue;
    }
    }
    iLog->Log(_L("All checkings done "));    
    //free memory
    for ( TInt i = 0; i < count; i++ )
    {
    delete trigInfoList[i];
    }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    if(err)
    {
    iLog->Log(_L("State Not changed "));    
    return KErrGeneral;
    }
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState3L
// Settriggersstate  using Area filter synchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState3L(CStifItemParser& /*aItem*/)
    {
    return TC_TestSetTriggersState3L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState4L
// settriggersstate  using Area filter asynchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState4L(CStifItemParser& /*aItem*/)
    {
    return TC_TestSetTriggersState4L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState5L
// Settriggersstate  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState5L(CStifItemParser& /*aItem*/)
    {
    return TC_TestSetTriggersState5L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState6L
// Settriggersstate  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState6L(CStifItemParser& /*aItem*/)
    {
    return TC_TestSetTriggersState6L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState7L
// Settriggersstate  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState7L(CStifItemParser& /*aItem*/)
    {
    return TC_TestSetTriggersState7L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState8L
// Settriggersstate  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState8L(CStifItemParser& /*aItem*/)
    {
    return TC_TestSetTriggersState8L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState9L
// Settriggersstate using composite filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState9L(CStifItemParser& /*aItem*/)
    {
    return TC_TestSetTriggersState9L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState10L
// Settriggersstate using composite filter  constructed using Multiple CGI Area asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState10L(CStifItemParser& /*aItem*/)
    {
    return TC_TestSetTriggersState10L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState11L
// Settriggersstate using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState11L(CStifItemParser& /*aItem*/)
    {
    return TC_TestSetTriggersState11L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState12L
// Settriggersstate using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState12L(CStifItemParser& /*aItem*/)
    {
    return TC_TestSetTriggersState12L();
    }


    
    // -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggerState1L
// listing triggers using NULL filter synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggerState1L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
     
    TRAP_IGNORE(lbt.DeleteTriggersL());
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
        
    lbt.SetTriggerStateL(trigId1,CLbtTriggerEntry::EStateEnabled,ELbtTrue);
    lbt.SetTriggerStateL(trigId2,CLbtTriggerEntry::EStateEnabled,ELbtTrue);
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,NULL );
    TBool err=EFalse;
    TInt count = trigInfoList.Count();
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    //Check area of the trigger
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());

    CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
              RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));           
   
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=CLbtTriggerEntry::EStateEnabled)
    {
    err = ETrue;
    }
    }
    iLog->Log(_L("All checkings done "));    
    //free memory
    for ( TInt i = 0; i < count; i++ )
    {
    delete trigInfoList[i];
    }
    trigInfoList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    if(err)
    {
    return KErrGeneral;
    }
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds5L
// listing triggers using NULL filter synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds1L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
     
    TRAP_IGNORE(lbt.DeleteTriggersL());
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    RArray< TLbtTriggerId>  aTriggerIdList;
    lbt.ListTriggerIdsL (aTriggerIdList,NULL);
   
    TInt count = aTriggerIdList.Count();
    TInt Numoftriggersfound =0;
    for ( TInt i = 0; i < count; i++ )
    {
    if(aTriggerIdList[i]==trigId1 ||aTriggerIdList[i]==trigId2 )
        {
        Numoftriggersfound++;
        }
    
    }
    iLog->Log(_L("All checkings done "));    
    //free memory
   
    aTriggerIdList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    if(Numoftriggersfound<2)
        return KErrGeneral;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers2L
// Listing triggers using NULL filter asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds2L(CStifItemParser& /*aItem*/)
    {
    RLbtServer lbtserver;
    RLbt lbt;
    
    User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    User::LeaveIfError( lbt.Open( lbtserver ) );
    CleanupClosePushL( lbt );
     
    TRAP_IGNORE(lbt.DeleteTriggersL());
      //Construct triggers
    CLbtSessionTrigger* trig1 = CLbtSessionTrigger::NewL();
    CLbtStartupTrigger* trig2 = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig1 );
    CleanupStack::PushL( trig2 );
        
    trig1->SetNameL(_L("Trigger1"));
    trig2->SetNameL(_L("Trigger2"));
    trig1->SetState(CLbtTriggerEntry::EStateDisabled);
    trig2->SetState(CLbtTriggerEntry::EStateEnabled);
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray1);

    CLbtTriggerConditionArea* condn1=CLbtTriggerConditionArea::NewL(Hybridarea1,CLbtTriggerConditionArea::EFireOnEnter);
    CLbtTriggerConditionArea* condn2=CLbtTriggerConditionArea::NewL(Hybridarea2,CLbtTriggerConditionArea::EFireOnEnter);
    
    trig1->SetCondition(condn1);
    trig2->SetCondition(condn2);
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig1->SetRequestorL(ReqType,ReqFormat,ReqData);
    trig2->SetRequestorL(ReqType,ReqFormat,ReqData);
    
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    TLbtTriggerId trigId1,trigId2;
    notifier->CreateTriggers( lbt,*trig1,trigId1,ETrue,wait );
    wait->Start( );
    
    _LIT( KMyTriggerHandlingProcessName, "About.exe");
      
    TSecureId secureid;
    trig2->SetProcessId(KMyTriggerHandlingProcessName,secureid);
    notifier->CreateTriggers( lbt,*trig2,trigId2,ETrue,wait );
    wait->Start( );
    
    RArray< TLbtTriggerId>  aTriggerIdList;
    notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait,NULL);
    wait->Start();
   
    TInt count = aTriggerIdList.Count();
    TInt Numoftriggersfound =0;
    for ( TInt i = 0; i < count; i++ )
    {
    if(aTriggerIdList[i]==trigId1 ||aTriggerIdList[i]==trigId2 )
        {
        Numoftriggersfound++;
        }
    
    }
    iLog->Log(_L("All checkings done "));    
    //free memory
    
    aTriggerIdList.Close();
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    if(Numoftriggersfound<2)
            return KErrGeneral;
    return KErrNone;       
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds3L
// listing trigger ids  using Area filter synchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds3L(CStifItemParser& /*aItem*/)
    {
    return TC_TestListtriggerIds3L();
    }


// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds4L
// listing trigger ids  using Area filter asynchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds4L(CStifItemParser& /*aItem*/)
    {
    return TC_TestListtriggerIds4L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds5L
// listing trigger ids  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds5L(CStifItemParser& /*aItem*/)
    {
    return TC_TestListtriggerIds5L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds6L
// listing trigger ids  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds6L(CStifItemParser& /*aItem*/)
    {
    return TC_TestListtriggerIds6L();
    }


// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds7L
// listing trigger ids  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds7L(CStifItemParser& /*aItem*/)
    {
    return TC_TestListtriggerIds7L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds8L
// listing trigger ids  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds8L(CStifItemParser& /*aItem*/)
    {
    return TC_TestListtriggerIds8L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds9L
// listing trigger ids using composite filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds9L(CStifItemParser& /*aItem*/)
    {
    return TC_TestListtriggerIds9L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds10L
// listing trigger ids using composite filter  constructed using Multiple CGI Area asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds10L(CStifItemParser& /*aItem*/)
    {
    return TC_TestListtriggerIds10L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds11L
// listing trigger ids using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds11L(CStifItemParser& /*aItem*/)
    {
    return TC_TestListtriggerIds11L();
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds12L
// listing trigger ids using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds12L(CStifItemParser& /*aItem*/)
    {
    return TC_TestListtriggerIds12L();
    }


// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGettriggerLC1L
// Retrieving trigger attributes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
    //Retrieving informations about a Cellular based startup trigger
TInt CTestHybridTriggers::TC_TestGettriggerLC1L( CStifItemParser& /* aItem */ )
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
  //  CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "TrigHandlingProcess.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
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
      RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                        Hybridareaarray1.Append(Cellarea1);
                        Hybridareaarray1.Append(Cellarea2);
      CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Hybridarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId,trigId2;
     trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    //User::After(5000000);
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
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
    CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
              RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));        
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
/*  TLbtTriggerDynamicInfo::TLbtTriggerActivityStatus currstatus;
    currstatus = dyninfo -> iActivityStatus;
    if(currstatus!=TLbtTriggerDynamicInfo::EActive)
    {
        return -99;
    }*/
    CleanupStack::PopAndDestroy(1 );//trig info
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // //delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGettriggerLC2L
// Retrieving trigger attributes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
    //Retrieving informations about a Cellular based startup trigger
TInt CTestHybridTriggers::TC_TestGettriggerLC2L( CStifItemParser& /* aItem */ )
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
  //  CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    
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
   RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                     Hybridareaarray1.Append(Cellarea1);
                     Hybridareaarray1.Append(Cellarea2);
   CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Hybridarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId,trigId2;
     trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig->SetRequestorL(ReqType,ReqFormat,ReqData);   
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    //User::After(5000000);
    CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    iLog->Log(_L("Before GetTriggerLC "));
    
    Triginfo = lbt.GetTriggerLC(trigId);
    iLog->Log(_L("After GetTriggerLC "));
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
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
    CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
              RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));        
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
/*  TLbtTriggerDynamicInfo::TLbtTriggerActivityStatus currstatus;
    currstatus = dyninfo -> iActivityStatus;
    if(currstatus!=TLbtTriggerDynamicInfo::EActive)
    {
        return -99;
    }*/
    CleanupStack::PopAndDestroy(1 );//trig info
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
   // //delete wait;
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestUpdateTrigger1L
// Updating attributes of a Cellular based startup trigger synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestUpdateTrigger1L( CStifItemParser& /* aItem */ )
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
  
//   #if 0
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
     RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                       Hybridareaarray1.Append(Cellarea1);
                       Hybridareaarray1.Append(Cellarea2);
     CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
  
   TUid aManagerUi = TUid::Uid(0x87654321);     
   // ownership of Cellarea object transferred to the condition object
   CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                               Hybridarea,
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
   TLbtTriggerId trigId,trigId2;   
   notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
   wait->Start( );
  
   //**************New Trigger Entry**************/ 
   CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
   newtrig->SetNameL(_L("TriggerTest"));
   newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
   TBuf<4> Networkcountrycode3 = _L("3000");
    TBuf<8> Networkidentitycode3 = _L("30000000");
    TUint Locationareacode3 = 300;
    TUint CellId3 = 300;
        
    CLbtGeoCell* Cellarea3  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                        Networkcountrycode3,
                                                        Networkidentitycode3,
                                                        Locationareacode3,
                                                        CellId3
                                                            );
    TBuf<4> Networkcountrycode4 = _L("4000");
    TBuf<8> Networkidentitycode4 = _L("40000000");
    TUint Locationareacode4 = 400;
    TUint CellId4 = 400;
        
    CLbtGeoCell* Cellarea4  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                        Networkcountrycode4,
                                                        Networkidentitycode4,
                                                        Locationareacode4,
                                                        CellId4
                                                            );
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2 ;
                           Hybridareaarray2.Append(Cellarea3);
                           Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
   // ownership of Cellarea object transferred to the condition object
   CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                               Hybridarea2,
                                              CLbtTriggerConditionArea::EFireOnEnter);
      
      
   newtrig->SetCondition(newcondition); // ownership transferred to object
   //  trigId = 1;
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

   CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
        AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
        CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
                 RPointerArray<CLbtGeoAreaBase> Hybridareaarray3= area->HybridArea(); 
       CLbtGeoAreaBase* base=Hybridareaarray3[0];
               
       CLbtGeoCell* cell2=NULL;
       if(base->Type()==CLbtGeoAreaBase::ECellular)
       {
           cell2= static_cast<CLbtGeoCell*> (base);
       }    
       
       AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
       AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode3,_L("Wrong Network Country code"));   
       AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode3,_L("Wrong Network identity code"));            
       AssertTrueL(cell2->LocationAreaCode()==Locationareacode3,_L("Wrong Location Area code"));   
       AssertTrueL(cell2->CellId()==CellId3,_L("Wrong Cellid"));      
       
       base=Hybridareaarray3[1];
               
       cell2=NULL;
       if(base->Type()==CLbtGeoAreaBase::ECellular)
       {
           cell2= static_cast<CLbtGeoCell*> (base);
       }    
       
       AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
       AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode4,_L("Wrong Network Country code"));   
       AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode4,_L("Wrong Network identity code"));            
       AssertTrueL(cell2->LocationAreaCode()==Locationareacode4,_L("Wrong Location Area code"));   
       AssertTrueL(cell2->CellId()==CellId4,_L("Wrong Cellid"));        
  //}
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
 // //delete wait;
 
  iLog->Log(_L("Test passed "));
  return KErrNone; 
    
  }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestUpdateTrigger2L
// Updating attributes of a Cellular based session trigger synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestUpdateTrigger2L( CStifItemParser& /* aItem */ )
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
    
    //   #if 0
      //Construct a startup trigger
     CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
     CleanupStack::PushL( trig );
    
    // Set Name
     trig->SetNameL(_L("Trigger1"));
    
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
       RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                         Hybridareaarray1.Append(Cellarea1);
                         Hybridareaarray1.Append(Cellarea2);
       CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
     TUid aManagerUi = TUid::Uid(0x87654321);     
     // ownership of Cellarea object transferred to the condition object
     CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                 Hybridarea,
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
     TLbtTriggerId trigId,trigId2;   
     notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
     wait->Start( );
    
     //**************New Trigger Entry**************/ 
     CLbtSessionTrigger* newtrig = CLbtSessionTrigger::NewL();
     newtrig->SetNameL(_L("TriggerTest"));
     newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
     TBuf<4> Networkcountrycode3 = _L("3000");
      TBuf<8> Networkidentitycode3 = _L("30000000");
      TUint Locationareacode3 = 300;
      TUint CellId3 = 300;
          
      CLbtGeoCell* Cellarea3  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                          Networkcountrycode3,
                                                          Networkidentitycode3,
                                                          Locationareacode3,
                                                          CellId3
                                                              );
      TBuf<4> Networkcountrycode4 = _L("4000");
      TBuf<8> Networkidentitycode4 = _L("40000000");
      TUint Locationareacode4 = 400;
      TUint CellId4 = 400;
          
      CLbtGeoCell* Cellarea4  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                          Networkcountrycode4,
                                                          Networkidentitycode4,
                                                          Locationareacode4,
                                                          CellId4
                                                              );
      RPointerArray<CLbtGeoAreaBase> Hybridareaarray2 ;
                               Hybridareaarray2.Append(Cellarea3);
                               Hybridareaarray2.Append(Cellarea4);
      CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
     // ownership of Cellarea object transferred to the condition object
     CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                 Hybridarea2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
        
     newtrig->SetCondition(newcondition); // ownership transferred to object
     //  trigId = 1;
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
    
     CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
      AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
      CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
               RPointerArray<CLbtGeoAreaBase> Hybridareaarray3= area->HybridArea();
     CLbtGeoAreaBase* base=Hybridareaarray3[0];
             
     CLbtGeoCell* cell2=NULL;
     if(base->Type()==CLbtGeoAreaBase::ECellular)
     {
         cell2= static_cast<CLbtGeoCell*> (base);
     }    
     
     AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
     AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode3,_L("Wrong Network Country code"));   
     AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode3,_L("Wrong Network identity code"));            
     AssertTrueL(cell2->LocationAreaCode()==Locationareacode3,_L("Wrong Location Area code"));   
     AssertTrueL(cell2->CellId()==CellId3,_L("Wrong Cellid"));      
     
     base=Hybridareaarray3[1];
             
     cell2=NULL;
     if(base->Type()==CLbtGeoAreaBase::ECellular)
     {
         cell2= static_cast<CLbtGeoCell*> (base);
     }    
     
     AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
     AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode4,_L("Wrong Network Country code"));   
     AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode4,_L("Wrong Network identity code"));            
     AssertTrueL(cell2->LocationAreaCode()==Locationareacode4,_L("Wrong Location Area code"));   
     AssertTrueL(cell2->CellId()==CellId4,_L("Wrong Cellid"));        
    //}
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
    // //delete wait;
    
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    
  }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestUpdateTrigger3L
// Updating attributes of a Cellular based startup trigger asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestUpdateTrigger3L( CStifItemParser& /* aItem */ )
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
    
  //   #if 0
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
       RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
       Hybridareaarray1.Append(Cellarea1);
       Hybridareaarray1.Append(Cellarea2);
       CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
     TUid aManagerUi = TUid::Uid(0x87654321);     
     // ownership of Cellarea object transferred to the condition object
     CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                 Hybridarea,
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
     TLbtTriggerId trigId,trigId2;   
     notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
     wait->Start( );
    
     //**************New Trigger Entry**************/ 
     CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
     newtrig->SetNameL(_L("TriggerTest"));
     newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
     TBuf<4> Networkcountrycode3 = _L("3000");
      TBuf<8> Networkidentitycode3 = _L("30000000");
      TUint Locationareacode3 = 300;
      TUint CellId3 = 300;
          
      CLbtGeoCell* Cellarea3  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                          Networkcountrycode3,
                                                          Networkidentitycode3,
                                                          Locationareacode3,
                                                          CellId3
                                                              );
      TBuf<4> Networkcountrycode4 = _L("4000");
      TBuf<8> Networkidentitycode4 = _L("40000000");
      TUint Locationareacode4 = 400;
      TUint CellId4 = 400;
          
      CLbtGeoCell* Cellarea4  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                          Networkcountrycode4,
                                                          Networkidentitycode4,
                                                          Locationareacode4,
                                                          CellId4
                                                              );
      RPointerArray<CLbtGeoAreaBase> Hybridareaarray2 ;
                               Hybridareaarray2.Append(Cellarea3);
                               Hybridareaarray2.Append(Cellarea4);
      CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
     // ownership of Cellarea object transferred to the condition object
     CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                 Hybridarea2,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
        
     newtrig->SetCondition(newcondition); // ownership transferred to object
     //  trigId = 1;
     newtrig->SetId(trigId);
   
     /***************************End*******************/
     iLog->Log(_L("Before UpdateTriggerL "));
     notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
            |CLbtTriggerEntry::EAttributeState
            |CLbtTriggerEntry::EAttributeCondition ,wait,ELbtTrue
            );
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

     CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
      AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
      CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
               RPointerArray<CLbtGeoAreaBase> Hybridareaarray3= area->HybridArea();
     CLbtGeoAreaBase* base=Hybridareaarray3[0];
             
     CLbtGeoCell* cell2=NULL;
     if(base->Type()==CLbtGeoAreaBase::ECellular)
     {
         cell2= static_cast<CLbtGeoCell*> (base);
     }    
     
     AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
     AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode3,_L("Wrong Network Country code"));   
     AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode3,_L("Wrong Network identity code"));            
     AssertTrueL(cell2->LocationAreaCode()==Locationareacode3,_L("Wrong Location Area code"));   
     AssertTrueL(cell2->CellId()==CellId3,_L("Wrong Cellid"));      
     
     base=Hybridareaarray3[1];
             
     cell2=NULL;
     if(base->Type()==CLbtGeoAreaBase::ECellular)
     {
         cell2= static_cast<CLbtGeoCell*> (base);
     }    
     
     AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
     AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode4,_L("Wrong Network Country code"));   
     AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode4,_L("Wrong Network identity code"));            
     AssertTrueL(cell2->LocationAreaCode()==Locationareacode4,_L("Wrong Location Area code"));   
     AssertTrueL(cell2->CellId()==CellId4,_L("Wrong Cellid"));        
    //}
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
   // //delete wait;
   
    iLog->Log(_L("Test passed "));
    return KErrNone; 
    
  }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestUpdateTrigger4L
// Updating attributes of a Cellular based startup trigger synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers::TC_TestUpdateTrigger4L( CStifItemParser& /* aItem */ )
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
  
  //   #if 0
    //Construct a startup trigger
   CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
  
  //Push to cleanup stack
   CleanupStack::PushL( trig );
  
  // Set Name
   trig->SetNameL(_L("Trigger1"));
  
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
     RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                       Hybridareaarray1.Append(Cellarea1);
                       Hybridareaarray1.Append(Cellarea2);
     CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
  
   TUid aManagerUi = TUid::Uid(0x87654321);     
   // ownership of Cellarea object transferred to the condition object
   CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                               Hybridarea,
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
   TLbtTriggerId trigId,trigId2;   
   notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
   wait->Start( );
  
   //**************New Trigger Entry**************/ 
   CLbtSessionTrigger* newtrig = CLbtSessionTrigger::NewL();
   newtrig->SetNameL(_L("TriggerTest"));
   newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
   TBuf<4> Networkcountrycode3 = _L("3000");
    TBuf<8> Networkidentitycode3 = _L("30000000");
    TUint Locationareacode3 = 300;
    TUint CellId3 = 300;
        
    CLbtGeoCell* Cellarea3  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                        Networkcountrycode3,
                                                        Networkidentitycode3,
                                                        Locationareacode3,
                                                        CellId3
                                                            );
    TBuf<4> Networkcountrycode4 = _L("4000");
    TBuf<8> Networkidentitycode4 = _L("40000000");
    TUint Locationareacode4 = 400;
    TUint CellId4 = 400;
        
    CLbtGeoCell* Cellarea4  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                        Networkcountrycode4,
                                                        Networkidentitycode4,
                                                        Locationareacode4,
                                                        CellId4
                                                            );
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray2 ;
                           Hybridareaarray2.Append(Cellarea3);
                           Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
   // ownership of Cellarea object transferred to the condition object
   CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                               Hybridarea2,
                                              CLbtTriggerConditionArea::EFireOnEnter);
      
      
   newtrig->SetCondition(newcondition); // ownership transferred to object
   //  trigId = 1;
   newtrig->SetId(trigId);
  
   /***************************End*******************/
   iLog->Log(_L("Before UpdateTriggerL "));
   notifier->UpdateTrigger(lbt,*newtrig,CLbtTriggerEntry::EAttributeName
               |CLbtTriggerEntry::EAttributeState
               |CLbtTriggerEntry::EAttributeCondition ,wait,ELbtTrue
               );
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
  
   CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
    AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
    CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
             RPointerArray<CLbtGeoAreaBase> Hybridareaarray3= area->HybridArea();
   CLbtGeoAreaBase* base=Hybridareaarray3[0];
           
   CLbtGeoCell* cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode3,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode3,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode3,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId3,_L("Wrong Cellid"));      
   
   base=Hybridareaarray3[1];
           
   cell2=NULL;
   if(base->Type()==CLbtGeoAreaBase::ECellular)
   {
       cell2= static_cast<CLbtGeoCell*> (base);
   }    
   
   AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
   AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode4,_L("Wrong Network Country code"));   
   AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode4,_L("Wrong Network identity code"));            
   AssertTrueL(cell2->LocationAreaCode()==Locationareacode4,_L("Wrong Location Area code"));   
   AssertTrueL(cell2->CellId()==CellId4,_L("Wrong Cellid"));        
  //}
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
  // //delete wait;
  
  iLog->Log(_L("Test passed "));
  return KErrNone; 
    
  }
// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestUpdateTrigger5L
// Updating Hybrid startup trigger to a coordinate based startup trigger.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
 
    TInt CTestHybridTriggers::TC_TestUpdateTrigger5L( CStifItemParser& /* aItem */ )
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
    
 //   #if 0
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
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                Hybridarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    trig->SetCondition(condition); // ownership transferred to object

    
    trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig->SetRequestorL(ReqType,ReqFormat,ReqData);      
    TLbtTriggerId trigId,trigId2;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    
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
//  trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
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
        iLog->Log(_L("wrong name "));
        return -99;
    }
    //Check the state of the trigger
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=newtrig->State())
    {
        iLog->Log(_L("wrong state "));
        return -99;
    }
    //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 != newtrig->Type())
    {
        iLog->Log(_L("wrong type "));
        return -99;
    }
    //Check Condition of the trigger
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
    CLbtTriggerConditionArea::TDirection direction;
    direction = Condition2->Direction();
    if(direction!=CLbtTriggerConditionArea::EFireOnEnter)
    {
        iLog->Log(_L("wrong Direction "));
        return -99;
    }
    CLbtGeoAreaBase* area = Condition2->TriggerArea();
    if(area ->Type()!=CLbtGeoAreaBase::ECircle )
    {
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
    iLog->Log(_L("wrong area type "));
        return -99;
    }
    
    CLbtGeoCircle* circle2 = static_cast <CLbtGeoCircle*>(area);
    TCoordinate center = newcircle->Center();
    TCoordinate center2 = circle2->Center();
    TReal32 distance;
    center2.Distance(center,distance);
    if(distance!=0)
    {
        iLog->Log(_L("wrong center "));
        return -99;
    }
    TReal radius = circle2->Radius();
    if(radius != newcircle->Radius())
    {
        iLog->Log(_L("wrong radius "));
        return -99;
    }
    
    

    //Check trigger Id
    trigId2 = TrigEntry->Id();
    if(trigId !=trigId2)
    {
        iLog->Log(_L("wrong trigger id "));
        return -99;
    }
    
    CleanupStack::PopAndDestroy( 1 );//Triginfo
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
   
    CleanupStack::Pop( &lbt );
    lbt.Close();
    
    CleanupStack::PopAndDestroy( &lbtserver );
   
   
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
    
    // -----------------------------------------------------------------------------
    // CTestHybridTriggers::TC_TestUpdateTrigger4L
    // Updating coordinate based to a Wlan based startup trigger
    // (other items were commented in a header).
    // -----------------------------------------------------------------------------
     //
    
      TInt CTestHybridTriggers::TC_TestUpdateTrigger6L( CStifItemParser& /* aItem */ )
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
    
    TBuf<4> Networkcountrycode = _L("404");
    TBuf<8> Networkidentitycode = _L("45");
//    TUint Locationareacode = 1627;
 //   TUint CellId = 40331;
 
 //   #if 0
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
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
  //  TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    
    TUid aManagerUi = TUid::Uid(0x87654321);   
    trig->SetManagerUi(aManagerUi);   
     //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
    CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
    TBuf<KLbtMaxNameLength> ReqData=_L("");
    trig->SetRequestorL(ReqType,ReqFormat,ReqData);      
    TLbtTriggerId trigId,trigId2;   
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );
    User::After(5000000);
    
   //**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    // CLbtGeoCell* Cellarea = NULL; 
   /* CLbtGeoAreaBase * base = NULL;//static_cast<CLbtGeoAreaBase*>(Cellarea);
    CLBTOperation *LBTOperation = notifier->CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::ECellular, 
                                                                             &base,
                                                                             wait);
    wait->Start( );
    delete LBTOperation;
    
      
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                base,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
       
    newtrig->SetCondition(newcondition); // ownership transferred to object
*/
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
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                      Hybridareaarray1.Append(Cellarea1);
                      Hybridareaarray1.Append(Cellarea2);
    CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    // ownership of Cellarea object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                Hybridarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
        
    newtrig->SetCondition(newcondition); // ownership transferred to object
//  trigId = 1;
    newtrig->SetId(trigId);
   
    /***************************End*******************/
    iLog->Log(_L("Before UpdateTriggerL "));
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition,ELbtTrue);
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
    iLog->Log(_L("Name OK "));
    //Check the state of the trigger
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=newtrig->State())
    {
        return -99;
    }
     iLog->Log(_L("State OK "));
    //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 != newtrig->Type())
    {
        return -99;
    }
    
    //Check Condition of the trigger
    
    CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
    iLog->Log(_L("condition OK "));
    CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
              RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));      
    
    base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }    
    
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));        
    //}
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
   // //delete wait;
   
    iLog->Log(_L("Test passed "));
    return KErrNone; 
      
    }
      
  // -----------------------------------------------------------------------------
  // CTestHybridTriggers::TC_TestHybridTriggerFiring1L
  // Creation of hybrid Entry type startup trigger
  // (other items were commented in a header).
  // -----------------------------------------------------------------------------
  //
  TInt CTestHybridTriggers ::TC_TestHybridTriggerFiring1L(CStifItemParser& /*aItem*/)
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
    
      
       TSecureId secureid;
       trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
         
       //set Requestor     
       CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
       CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
       TBuf<KLbtMaxNameLength> ReqData=_L("");
       trig->SetRequestorL(ReqType,ReqFormat,ReqData);
      // set condition
       CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
       RFileLogger aLog;
       CLbtAppCgiRequestor* CGIRequest=CLbtAppCgiRequestor::NewL(aLog);
       CleanupStack::PushL( CGIRequest );  
       CGIRequest->Start(wait);
       wait->Start( );
       iLog->Log(_L("First CGI Info received"));
       CTelephony::TNetworkInfoV1Pckg currcgi1=CGIRequest->CurrentCGI();
      

       //CLbtGeoCell* Cellarea2;
       CTelephony::TNetworkMode Networkmode = currcgi1().iMode;
       TUint cellid1=currcgi1().iCellId;
       CLbtGeoCell*  Cellarea1=NULL;  
       RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Current Cell Id= %d"),cellid1);
       if(Networkmode ==CTelephony::ENetworkModeGsm)
       {
       Cellarea1= CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
               currcgi1().iCountryCode,
               currcgi1().iNetworkId,
               currcgi1().iLocationAreaCode,
               currcgi1().iCellId
                                     );  
       }
       /*
       //wait for CID change notification
        CLbtEventObserver* notifier2= CLbtEventObserver::NewL( lbt);
        CleanupStack::PushL( notifier2 );
        CActiveSchedulerWait* wait2=new(ELeave)CActiveSchedulerWait;
        notifier2->StartCidNotification(wait2);
        iLog->Log(_L("Cell id change notification requested"));
        wait2->Start();
        iLog->Log(_L("Cell id change Notification received"));
        //CID changed
        CleanupStack::PopAndDestroy( notifier2 );
       CGIRequest->Start(wait);
       wait->Start( );
       iLog->Log(_L("Second CGI Received"));
       CTelephony::TNetworkInfoV1Pckg currcgi2 = CGIRequest->CurrentCGI();
       Networkmode = currcgi2().iMode;
       TUint cellid2=currcgi2().iCellId;
       CLbtGeoCell* Cellarea2;
       if(Networkmode ==CTelephony::ENetworkModeGsm)
          {
          Cellarea2= CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                  currcgi2().iCountryCode,
                  currcgi2().iNetworkId,
                  currcgi2().iLocationAreaCode,
                  currcgi2().iCellId
                                        );  
          }*/
       
       CLbtGeoCell* Cellarea2=NULL;
       TUint CellId1 = 100;
          if(Networkmode ==CTelephony::ENetworkModeGsm)
             {
             Cellarea2= CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                     currcgi1().iCountryCode,
                     currcgi1().iNetworkId,
                     currcgi1().iLocationAreaCode,
                     CellId1
                                           );  
             }
       RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                             Hybridareaarray1.Append(Cellarea1);
                             Hybridareaarray1.Append(Cellarea2);
         CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
      
      
      // ownership of Cellarea object transferred to the condition object
       CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                   Hybridarea,
                                                  CLbtTriggerConditionArea::EFireOnEnter);
          
       trig->SetCondition(condition); // ownership transferred to object
      iLog->Log(_L("Trigger condition created"));
       TLbtTriggerId trigId;
          
          
       CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
       CleanupStack::PushL( notifier );
      
                 
       notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
       wait->Start( );
       iLog->Log(_L("Trigger Created"));
       notifier->StartNotification( wait );
       wait->Start( );
       iLog->Log(_L("Trigger Fired"));
       
       TLbtTriggerFireInfo FireInfo;
//       TReal32 trigDistance;
       TPosition firePosition;
       FireInfo = notifier->GetFiredTrigger();
        
       CLbtTriggerInfo *Triginfo ;
       iLog->Log(_L("Before GetTriggerLC "));
      
       Triginfo= lbt.GetTriggerLC(FireInfo.iTriggerId);
       
       CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
       CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
       CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
       AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
       CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
                     RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
      
      //CLbtGeoAreaBase* base=Hybridareaarray2[0];
              
      /*CLbtGeoCell* cell2=NULL;
      if(base->Type()==CLbtGeoAreaBase::ECellular)
      {
          cell2= static_cast<CLbtGeoCell*> (base);
      }    */
      
      /*AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
      AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
      AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
      AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
      AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));   */   
      
     /* base=Hybridareaarray2[1];
              
      cell2=NULL;
      if(base->Type()==CLbtGeoAreaBase::ECellular)
      {
          cell2= static_cast<CLbtGeoCell*> (base);
      }  */  
      
     /* AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
      AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
      AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
      AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
      AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));     */      
       CleanupStack::Pop(Triginfo);
       //CleanupStack::PopAndDestroy( notifier2 );
       CleanupStack::PopAndDestroy( notifier );
       CleanupStack::PopAndDestroy( CGIRequest );
       CleanupStack::PopAndDestroy( trig );
       CleanupStack::Pop( &lbt );
       CleanupStack::PopAndDestroy( &lbtserver );
      //delete wait;
       return KErrNone;
      
      }

  // -----------------------------------------------------------------------------
  // CTestHybridTriggers::TC_TestCreatetrigger2L
  // Creation of hybrid Entry type session trigger
  // (other items were commented in a header).
  // -----------------------------------------------------------------------------
  //
  TInt CTestHybridTriggers ::TC_TestHybridTriggerFiring2L(CStifItemParser& /*aItem*/)
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
     CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
     CleanupStack::PushL( trig );
     iLog->Log(_L("Startup Trigger Entry Created "));
    
    // Set Name
     trig->SetNameL(_L("Trigger1"));
   
    
     //set Requestor     
     CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
     CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
     TBuf<KLbtMaxNameLength> ReqData=_L("");
     trig->SetRequestorL(ReqType,ReqFormat,ReqData);
    // set condition
     CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
     RFileLogger aLog;
     CLbtAppCgiRequestor* CGIRequest=CLbtAppCgiRequestor::NewL(aLog);
     CleanupStack::PushL( CGIRequest );  
     CGIRequest->Start(wait);
     wait->Start( );
     CTelephony::TNetworkInfoV1Pckg currcgi1=CGIRequest->CurrentCGI();
    

     //CLbtGeoCell* Cellarea2;
     CTelephony::TNetworkMode Networkmode = currcgi1().iMode;
     TUint cellid1=currcgi1().iCellId;
     RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Current Cell Id= %d"),cellid1);
     CLbtGeoCell*  Cellarea1=NULL;                                              
     if(Networkmode ==CTelephony::ENetworkModeGsm)
     {
     Cellarea1= CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
             currcgi1().iCountryCode,
             currcgi1().iNetworkId,
             currcgi1().iLocationAreaCode,
             currcgi1().iCellId
                                   );  
     }
     /*
     //wait for CID change notification
      CLbtEventObserver* notifier2= CLbtEventObserver::NewL( lbt);
      CleanupStack::PushL( notifier2 );
       
      notifier2->StartCidNotification(wait);
      iLog->Log(_L("Cell id change notification requested"));
      wait->Start();
      //CID changed
      CleanupStack::PopAndDestroy( notifier2 );
     CGIRequest->Start(wait);
     wait->Start( );
     CTelephony::TNetworkInfoV1Pckg currcgi2 = CGIRequest->CurrentCGI();
     Networkmode = currcgi2().iMode;
     TUint cellid2=currcgi2().iCellId;
     CLbtGeoCell* Cellarea2;
     if(Networkmode ==CTelephony::ENetworkModeGsm)
        {
        Cellarea2= CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                currcgi2().iCountryCode,
                currcgi2().iNetworkId,
                currcgi2().iLocationAreaCode,
                currcgi2().iCellId
                                      );  
        }*/
     CLbtGeoCell* Cellarea2=NULL;
    TUint CellId1 = 100;
       if(Networkmode ==CTelephony::ENetworkModeGsm)
          {
          Cellarea2= CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                  currcgi1().iCountryCode,
                  currcgi1().iNetworkId,
                  currcgi1().iLocationAreaCode,
                  CellId1
                                        );  
          }
     RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ;
                           Hybridareaarray1.Append(Cellarea1);
                           Hybridareaarray1.Append(Cellarea2);
       CLbtGeoHybrid* Hybridarea  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    
    // ownership of Cellarea object transferred to the condition object
     CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                 Hybridarea,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
     trig->SetCondition(condition); // ownership transferred to object
    
     TLbtTriggerId trigId;
        
        
     CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
     CleanupStack::PushL( notifier );
    
               
     notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
     wait->Start( );
     iLog->Log(_L("Trigger Created"));
     notifier->StartNotification( wait );
     wait->Start( );
     iLog->Log(_L("Trigger Fired"));
     
     TLbtTriggerFireInfo FireInfo;
    // TReal32 trigDistance;
     TPosition firePosition;
     FireInfo = notifier->GetFiredTrigger();
      
     CLbtTriggerInfo *Triginfo ;
     iLog->Log(_L("Before GetTriggerLC "));
    
     Triginfo= lbt.GetTriggerLC(FireInfo.iTriggerId);
     
     CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
     CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
     CLbtGeoAreaBase* TrigArea = Condition2->TriggerArea();
     AssertTrueL(TrigArea->Type()==CLbtGeoAreaBase::EHybrid,_L("Wrong Type")); 
     CLbtGeoHybrid *area =static_cast<CLbtGeoHybrid*>(TrigArea); 
                   RPointerArray<CLbtGeoAreaBase> Hybridareaarray2= area->HybridArea();
    
    /*CLbtGeoAreaBase* base=Hybridareaarray2[0];
            
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }   */ 
    
    /*AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode1,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode1,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode1,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId1,_L("Wrong Cellid"));   */   
    
   /* base=Hybridareaarray2[1];
            
    cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
        cell2= static_cast<CLbtGeoCell*> (base);
    }   */ 
    
   /* AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode2,_L("Wrong Network Country code"));   
    AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode2,_L("Wrong Network identity code"));            
    AssertTrueL(cell2->LocationAreaCode()==Locationareacode2,_L("Wrong Location Area code"));   
    AssertTrueL(cell2->CellId()==CellId2,_L("Wrong Cellid"));     */      
     CleanupStack::Pop(Triginfo);
     //CleanupStack::PopAndDestroy( notifier2 );
     CleanupStack::PopAndDestroy( notifier );
     CleanupStack::PopAndDestroy( CGIRequest );
     CleanupStack::PopAndDestroy( trig );
     CleanupStack::Pop( &lbt );
     CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
     return KErrNone;
     
      }
      
    TInt CTestHybridTriggers::StartCellChangeNotifierL( CStifItemParser& /* aItem */ )
    {
   	RLbtServer lbtserver;
    RLbt lbt;
	 iLog->Log(_L("Before connecting"));
	 User::LeaveIfError( lbtserver.Connect() );
	 CleanupClosePushL( lbtserver );
	 iLog->Log(_L("Connection to RLbtServer Passed "));
	 User::LeaveIfError( lbt.Open(lbtserver));
	 CleanupClosePushL( lbt );
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
 	
 	CLbtEventObserver* notifier= CLbtEventObserver::NewL( lbt);
	CleanupStack::PushL( notifier );
  
    notifier->StartCidNotification(wait);
    iLog->Log(_L("Cell id change notification requested"));
    wait->Start();
    CleanupStack::PopAndDestroy( notifier );
		
	CleanupStack::PopAndDestroy( &lbt );
	CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone;
    }  



// -----------------------------------------------------------------------------
// CTestHybridTriggers::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CTestHybridTriggers::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
