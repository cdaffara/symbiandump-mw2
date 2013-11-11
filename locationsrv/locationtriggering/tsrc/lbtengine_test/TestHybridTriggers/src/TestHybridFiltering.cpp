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
* Description:  Test file to test Filtering testcases for Multiple cgi triggering
*
*/


#include <StifParser.h>
#include <Stiftestinterface.h>
#include "TestHybridTriggers.h"

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers3L
// Deleting triggers using Area filter  constructed using Multiple CGI Area synchronously(when both the cells are matching)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers3L()
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
    //construct filter
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    lbt.DeleteTriggersL(areafilter);
    CleanupStack::Pop( areafilter );
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ););
    TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ););
    if(error==KErrNotFound && error2==KErrNotFound)
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    }
    else
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrGeneral;
    }
    }
// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers4L
// Deleting triggers in given Hybrid area using Area filter asynchronously(when both the cells are matching)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers4L()
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
    //construct filter
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    notifier->DeleteFilteredTriggers(lbt,areafilter,wait);
    wait->Start();
    CleanupStack::Pop( areafilter );
    
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ););
        TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ););
    if(error==KErrNotFound && error2==KErrNotFound)
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    }
    else
    {
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrGeneral;
    }
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestDeleteTriggers5L
// Deleting triggers using Area filter  constructed using Multiple CGI Area synchronously(when both the cells are not matching)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers5L()
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
    //construct filter
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    lbt.DeleteTriggersL(areafilter);
    CleanupStack::Pop( areafilter );
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ););
        TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ););
    if(error==KErrNone && error2==KErrNone)
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
// CTestHybridTriggers::TC_TestDeleteTriggers6L
// Deleting triggers in given Hybrid area using Area filter asynchronously(when both the cells are not matching)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers6L()
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
    //construct filter
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    notifier->DeleteFilteredTriggers(lbt,areafilter,wait);
    wait->Start();
    CleanupStack::Pop( areafilter );
    
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ););
        TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ););
    if(error==KErrNone && error2==KErrNone)
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
// Deleting triggers using Area filter  constructed using Multiple CGI Area synchronously(when one of the cells are not matching)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers7L()
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
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    lbt.DeleteTriggersL(areafilter);
    CleanupStack::Pop( areafilter );
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ););
        TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ););
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
// CTestHybridTriggers::TC_TestDeleteTriggers8L
// Deleting triggers in given Hybrid using Area filter asynchronously(when one of the cells are not matching)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers8L()
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
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    notifier->DeleteFilteredTriggers(lbt,areafilter,wait);
    wait->Start();
    CleanupStack::Pop( areafilter );
    
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ););
        TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ););
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
// CTestHybridTriggers::TC_TestDeleteTriggers9L
// Deleting triggers using composite filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers9L()
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
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    CLbtTriggerFilterByAttribute* attribFilter = 
        CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter ); 
    
    
    compositeFilter->AddFilterL( areafilter ); 
    
    lbt.DeleteTriggersL(compositeFilter);
    CleanupStack::PopAndDestroy( compositeFilter );
    CleanupStack::Pop( attribFilter );
    CleanupStack::Pop( areafilter );
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ););
    TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ););
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
// CTestHybridTriggers::TC_TestDeleteTriggers10L
// Deleting triggers in given Hybrid using composite filter asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers10L()
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
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    CLbtTriggerFilterByAttribute* attribFilter = 
        CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter ); 
    
    
    compositeFilter->AddFilterL( areafilter ); 
    notifier->DeleteFilteredTriggers(lbt,compositeFilter,wait);
    wait->Start();
    CleanupStack::PopAndDestroy( compositeFilter );
    CleanupStack::Pop( attribFilter );
    CleanupStack::Pop( areafilter );
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ););
        TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ););
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
// CTestHybridTriggers::TC_TestDeleteTriggers11L
// Deleting triggers using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers11L()
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ,Hybridareaarray2;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      Hybridareaarray2.Append(Cellarea3);
      Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
        
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
    //construct filter
    
       
    CLbtGeoCell* Cellarea5  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode1,
                                                       Networkidentitycode1,
                                                       Locationareacode1,
                                                       CellId1
                                                           );
    
       
    CLbtGeoCell* Cellarea6  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode4,
                                                       Networkidentitycode4,
                                                       Locationareacode4,
                                                       CellId4
                                                           );
    
    CLbtGeoCell* Cellarea7  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode2,
                                                           Networkidentitycode2,
                                                           Locationareacode2,
                                                           CellId2
                                                               );
        
           
    CLbtGeoCell* Cellarea8  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode3,
                                                           Networkidentitycode3,
                                                           Locationareacode3,
                                                           CellId3
                                                               );
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3,Hybridareaarray4 ;
      Hybridareaarray3.Append(Cellarea5);
      Hybridareaarray3.Append(Cellarea6);
      Hybridareaarray4.Append(Cellarea7);
      Hybridareaarray4.Append(Cellarea8);
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray3);
    CLbtGeoHybrid* Hybridarea4  = CLbtGeoHybrid::NewL(Hybridareaarray4);
    
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter1 );  
    CLbtTriggerFilterByArea* areafilter2=CLbtTriggerFilterByArea::NewL(Hybridarea4); 
        CleanupStack::PushL( areafilter2 );  
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areafilter1 ); 
    
    
    compositeFilter->AddFilterL( areafilter2 ); 
    
    lbt.DeleteTriggersL(compositeFilter);
    CleanupStack::PopAndDestroy( compositeFilter );
    CleanupStack::Pop( areafilter2 );
    CleanupStack::Pop( areafilter1 );
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ););
    TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ););
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
// CTestHybridTriggers::TC_TestDeleteTriggers12L
// Deleting triggers using composite filter  constructed using Two Multiple CGI Area filters asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestDeleteTriggers12L()
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ,Hybridareaarray2;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      Hybridareaarray2.Append(Cellarea3);
      Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
        
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
    //construct filter
    
       
    CLbtGeoCell* Cellarea5  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode1,
                                                       Networkidentitycode1,
                                                       Locationareacode1,
                                                       CellId1
                                                           );
    
       
    CLbtGeoCell* Cellarea6  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode4,
                                                       Networkidentitycode4,
                                                       Locationareacode4,
                                                       CellId4
                                                           );
    
    CLbtGeoCell* Cellarea7  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode2,
                                                           Networkidentitycode2,
                                                           Locationareacode2,
                                                           CellId2
                                                               );
        
           
    CLbtGeoCell* Cellarea8  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode3,
                                                           Networkidentitycode3,
                                                           Locationareacode3,
                                                           CellId3
                                                               );
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3,Hybridareaarray4 ;
      Hybridareaarray3.Append(Cellarea5);
      Hybridareaarray3.Append(Cellarea6);
      Hybridareaarray4.Append(Cellarea7);
      Hybridareaarray4.Append(Cellarea8);
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray3);
    CLbtGeoHybrid* Hybridarea4  = CLbtGeoHybrid::NewL(Hybridareaarray4);
    
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter1 );  
    CLbtTriggerFilterByArea* areafilter2=CLbtTriggerFilterByArea::NewL(Hybridarea4); 
        CleanupStack::PushL( areafilter2 );  
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areafilter1 ); 
    
    
    compositeFilter->AddFilterL( areafilter2 ); 
    
    notifier->DeleteFilteredTriggers(lbt,compositeFilter,wait);
    wait->Start();
    CleanupStack::PopAndDestroy( compositeFilter );
    CleanupStack::Pop( areafilter2 );
    CleanupStack::Pop( areafilter1 );
//    CLbtTriggerInfo* Triginfo=NULL;
    TRAPD(error, lbt.GetTriggerLC(trigId1);CleanupStack::PopAndDestroy(  ););
        TRAPD(error2, lbt.GetTriggerLC(trigId2);CleanupStack::PopAndDestroy(  ););
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
// CTestHybridTriggers::TC_TestGetTriggers3L
// listing triggers  using Area filter synchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers3L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,listOptions );
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
// CTestHybridTriggers::TC_TestGetTriggers4L
// listing triggers  using Area filter asynchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers4L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
       
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
// CTestHybridTriggers::TC_TestGetTriggers5L
// listing triggers  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers5L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    TRAPD(error,lbt.GetTriggersL(trigInfoList,listOptions ));
    
    if(error==KErrNotFound)
    {
    trigInfoList.Close();
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    trigInfoList.Close();
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
// CTestHybridTriggers::TC_TestGetTriggers6L
// listing triggers  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers6L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    
    if(count==0)
    {
    trigInfoList.Close();
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
    trigInfoList.Close();
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
// CTestHybridTriggers::TC_TestGetTriggers7L
// listing triggers  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers7L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,listOptions );
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
// CTestHybridTriggers::TC_TestGetTriggers8L
// listing triggers  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers8L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
// CTestHybridTriggers::TC_TestDeleteTriggers3L
// Deleting triggers using composite filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers9L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    CLbtTriggerFilterByAttribute* attribFilter = 
        CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter ); 
    
    
    compositeFilter->AddFilterL( areafilter ); 
    listOptions->SetFilter( compositeFilter ); 
    
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,listOptions );
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( attribFilter );
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
// CTestHybridTriggers::TC_TestGetTriggers10L
// listing triggers using composite filter  constructed using Multiple CGI Area asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers10L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    CLbtTriggerFilterByAttribute* attribFilter = 
        CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter ); 
    
    
    compositeFilter->AddFilterL( areafilter ); 
    listOptions->SetFilter( compositeFilter ); 
    
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( attribFilter );
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
// CTestHybridTriggers::TC_TestGetTriggers11L
// Listing triggers using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers11L()
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ,Hybridareaarray2;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      Hybridareaarray2.Append(Cellarea3);
      Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
        
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    
       
    CLbtGeoCell* Cellarea5  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode1,
                                                       Networkidentitycode1,
                                                       Locationareacode1,
                                                       CellId1
                                                           );
    
       
    CLbtGeoCell* Cellarea6  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode4,
                                                       Networkidentitycode4,
                                                       Locationareacode4,
                                                       CellId4
                                                           );
    
    CLbtGeoCell* Cellarea7  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode2,
                                                           Networkidentitycode2,
                                                           Locationareacode2,
                                                           CellId2
                                                               );
        
           
    CLbtGeoCell* Cellarea8  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode3,
                                                           Networkidentitycode3,
                                                           Locationareacode3,
                                                           CellId3
                                                               );
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3,Hybridareaarray4 ;
      Hybridareaarray3.Append(Cellarea5);
      Hybridareaarray3.Append(Cellarea6);
      Hybridareaarray4.Append(Cellarea7);
      Hybridareaarray4.Append(Cellarea8);
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray3);
    CLbtGeoHybrid* Hybridarea4  = CLbtGeoHybrid::NewL(Hybridareaarray4);
    
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter1 );  
    CLbtTriggerFilterByArea* areafilter2=CLbtTriggerFilterByArea::NewL(Hybridarea4); 
        CleanupStack::PushL( areafilter2 );  
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areafilter1 ); 
    
    
    compositeFilter->AddFilterL( areafilter2 ); 
    listOptions->SetFilter( compositeFilter ); 
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,listOptions );
    CleanupStack::Pop( compositeFilter );
        CleanupStack::Pop( areafilter2 );
        CleanupStack::Pop( areafilter1 );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    TLbtTriggerId triggerid = TrigEntry->Id();
    if(triggerid!=trigId1 && triggerid!=trigId2)
        {
        trigInfoList.Close();
        CleanupStack::PopAndDestroy( notifier );
        CleanupStack::Pop( trig2 );
        CleanupStack::PopAndDestroy( trig1 );
        CleanupStack::PopAndDestroy( &lbt );
        CleanupStack::PopAndDestroy( &lbtserver );
        return KErrGeneral;
        }
    }
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
// CTestHybridTriggers::TC_TestGetTriggers12L
// Listing triggers using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers12L()
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ,Hybridareaarray2;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      Hybridareaarray2.Append(Cellarea3);
      Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
        
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    
       
    CLbtGeoCell* Cellarea5  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode1,
                                                       Networkidentitycode1,
                                                       Locationareacode1,
                                                       CellId1
                                                           );
    
       
    CLbtGeoCell* Cellarea6  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode4,
                                                       Networkidentitycode4,
                                                       Locationareacode4,
                                                       CellId4
                                                           );
    
    CLbtGeoCell* Cellarea7  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode2,
                                                           Networkidentitycode2,
                                                           Locationareacode2,
                                                           CellId2
                                                               );
        
           
    CLbtGeoCell* Cellarea8  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode3,
                                                           Networkidentitycode3,
                                                           Locationareacode3,
                                                           CellId3
                                                               );
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3,Hybridareaarray4 ;
      Hybridareaarray3.Append(Cellarea5);
      Hybridareaarray3.Append(Cellarea6);
      Hybridareaarray4.Append(Cellarea7);
      Hybridareaarray4.Append(Cellarea8);
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray3);
    CLbtGeoHybrid* Hybridarea4  = CLbtGeoHybrid::NewL(Hybridareaarray4);
    
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter1 );  
    CLbtTriggerFilterByArea* areafilter2=CLbtTriggerFilterByArea::NewL(Hybridarea4); 
        CleanupStack::PushL( areafilter2 );  
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areafilter1 ); 
    
    
    compositeFilter->AddFilterL( areafilter2 ); 
    listOptions->SetFilter( compositeFilter ); 
   
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();
    CleanupStack::Pop( compositeFilter );
       CleanupStack::Pop( areafilter2 );
       CleanupStack::Pop( areafilter1 );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    TLbtTriggerId triggerid = TrigEntry->Id();
    if(triggerid!=trigId1 &&triggerid!=trigId2)
        {
        trigInfoList.Close();
        CleanupStack::PopAndDestroy( notifier );
        CleanupStack::Pop( trig2 );
        CleanupStack::PopAndDestroy( trig1 );
        CleanupStack::PopAndDestroy( &lbt );
        CleanupStack::PopAndDestroy( &lbtserver );
        return KErrGeneral;
        }
    }
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
// CTestHybridTriggers::TC_TestGetTriggers_Iterator3L
// listing triggers using NULL filter synchronously using iterator
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator3L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray<CLbtTriggerInfo> trigInfoList;
    lbt.CreateGetTriggerIteratorL(listOptions);
    
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
    CleanupStack::Pop(trigInfo);
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator4L
// listing triggers using NULL filter asynchronously using iterator
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator4L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray<CLbtTriggerInfo> trigInfoList;
    notifier->CreateIterator(lbt,wait,NULL);
    wait->Start( );
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
    CleanupStack::Pop(trigInfo);
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
   
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator5L
// listing triggers  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator5L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray<CLbtTriggerInfo> trigInfoList;
    TRAPD(error,lbt.CreateGetTriggerIteratorL(listOptions));
    
    if(error==KErrNotFound)
    {
    trigInfoList.Close();
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    trigInfoList.Close();
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
// CTestHybridTriggers::TC_TestGetTriggers_Iterator6L
// listing triggers  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator6L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray<CLbtTriggerInfo> trigInfoList;
    notifier->CreateIterator(lbt,wait,listOptions);
    wait->Start( );
    TInt error = notifier->iStatus.Int();
        
        if(error==KErrNotFound)
        {
        trigInfoList.Close();
        CleanupStack::Pop( areafilter );
        CleanupStack::PopAndDestroy( listOptions );
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
        trigInfoList.Close();
        CleanupStack::Pop( areafilter );
        CleanupStack::PopAndDestroy( listOptions );
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
// CTestHybridTriggers::TC_TestGetTriggers_Iterator7L
// listing triggers  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator7L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray<CLbtTriggerInfo> trigInfoList;
    lbt.CreateGetTriggerIteratorL(listOptions);
    
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
    CleanupStack::Pop(trigInfo);
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator8L
// listing triggers  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator8L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray<CLbtTriggerInfo> trigInfoList;
    notifier->CreateIterator(lbt,wait,NULL);
    wait->Start( );
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
    CleanupStack::Pop(trigInfo);
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator9L
// Deleting triggers using composite filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator9L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    CLbtTriggerFilterByAttribute* attribFilter = 
        CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter ); 
    
    
    compositeFilter->AddFilterL( areafilter ); 
    listOptions->SetFilter( compositeFilter ); 
    
    
    RArray<CLbtTriggerInfo> trigInfoList;
    lbt.CreateGetTriggerIteratorL(listOptions);
    
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
    CleanupStack::Pop(trigInfo);
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( attribFilter );
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator10L
// listing triggers using composite filter  constructed using Multiple CGI Area asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator10L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    CLbtTriggerFilterByAttribute* attribFilter = 
        CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter ); 
    
    
    compositeFilter->AddFilterL( areafilter ); 
    listOptions->SetFilter( compositeFilter ); 
    
    
    RArray<CLbtTriggerInfo> trigInfoList;
    notifier->CreateIterator(lbt,wait,NULL);
    wait->Start( );
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
    CleanupStack::Pop(trigInfo);
    CleanupStack::Pop( compositeFilter );
        CleanupStack::Pop( attribFilter );
        CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    
    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::Pop( trig2 );
    CleanupStack::PopAndDestroy( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestGetTriggers_Iterator11L
// Listing triggers using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator11L()
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ,Hybridareaarray2;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      Hybridareaarray2.Append(Cellarea3);
      Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
        
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    
       
    CLbtGeoCell* Cellarea5  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode1,
                                                       Networkidentitycode1,
                                                       Locationareacode1,
                                                       CellId1
                                                           );
    
       
    CLbtGeoCell* Cellarea6  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode4,
                                                       Networkidentitycode4,
                                                       Locationareacode4,
                                                       CellId4
                                                           );
    
    CLbtGeoCell* Cellarea7  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode2,
                                                           Networkidentitycode2,
                                                           Locationareacode2,
                                                           CellId2
                                                               );
        
           
    CLbtGeoCell* Cellarea8  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode3,
                                                           Networkidentitycode3,
                                                           Locationareacode3,
                                                           CellId3
                                                               );
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3,Hybridareaarray4 ;
      Hybridareaarray3.Append(Cellarea5);
      Hybridareaarray3.Append(Cellarea6);
      Hybridareaarray4.Append(Cellarea7);
      Hybridareaarray4.Append(Cellarea8);
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray3);
    CLbtGeoHybrid* Hybridarea4  = CLbtGeoHybrid::NewL(Hybridareaarray4);
    
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter1 );  
    CLbtTriggerFilterByArea* areafilter2=CLbtTriggerFilterByArea::NewL(Hybridarea4); 
        CleanupStack::PushL( areafilter2 );  
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areafilter1 ); 
    
    
    compositeFilter->AddFilterL( areafilter2 ); 
    listOptions->SetFilter( compositeFilter ); 
    
    RArray<CLbtTriggerInfo> trigInfoList;
    lbt.CreateGetTriggerIteratorL(listOptions);
    
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
    CleanupStack::Pop(trigInfo);
    CleanupStack::Pop( compositeFilter );
        CleanupStack::Pop( areafilter2 );
        CleanupStack::Pop( areafilter1 );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i].TriggerEntry();
    TLbtTriggerId triggerid = TrigEntry->Id();
    if(triggerid!=trigId1 && triggerid!=trigId2)
        {
        trigInfoList.Close();
        
        CleanupStack::PopAndDestroy( notifier );
        CleanupStack::Pop( trig2 );
        CleanupStack::PopAndDestroy( trig1 );
        CleanupStack::PopAndDestroy( &lbt );
        CleanupStack::PopAndDestroy( &lbtserver );
        return KErrGeneral;
        }
    }
    //free memory
    
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
// CTestHybridTriggers::TC_TestGetTriggers12L
// Listing triggers using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestGetTriggers_Iterator12L()
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ,Hybridareaarray2;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      Hybridareaarray2.Append(Cellarea3);
      Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
        
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    
       
    CLbtGeoCell* Cellarea5  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode1,
                                                       Networkidentitycode1,
                                                       Locationareacode1,
                                                       CellId1
                                                           );
    
       
    CLbtGeoCell* Cellarea6  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode4,
                                                       Networkidentitycode4,
                                                       Locationareacode4,
                                                       CellId4
                                                           );
    
    CLbtGeoCell* Cellarea7  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode2,
                                                           Networkidentitycode2,
                                                           Locationareacode2,
                                                           CellId2
                                                               );
        
           
    CLbtGeoCell* Cellarea8  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode3,
                                                           Networkidentitycode3,
                                                           Locationareacode3,
                                                           CellId3
                                                               );
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3,Hybridareaarray4 ;
      Hybridareaarray3.Append(Cellarea5);
      Hybridareaarray3.Append(Cellarea6);
      Hybridareaarray4.Append(Cellarea7);
      Hybridareaarray4.Append(Cellarea8);
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray3);
    CLbtGeoHybrid* Hybridarea4  = CLbtGeoHybrid::NewL(Hybridareaarray4);
    
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter1 );  
    CLbtTriggerFilterByArea* areafilter2=CLbtTriggerFilterByArea::NewL(Hybridarea4); 
        CleanupStack::PushL( areafilter2 );  
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areafilter1 ); 
    
    
    compositeFilter->AddFilterL( areafilter2 ); 
    listOptions->SetFilter( compositeFilter ); 
    
    RArray<CLbtTriggerInfo> trigInfoList;
    notifier->CreateIterator(lbt,wait,listOptions);
    wait->Start( );
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
    CleanupStack::Pop(trigInfo);
    CleanupStack::Pop( compositeFilter );
        CleanupStack::Pop( areafilter2 );
        CleanupStack::Pop( areafilter1 );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i].TriggerEntry();
    TLbtTriggerId triggerid = TrigEntry->Id();
    if(triggerid!=trigId1 && triggerid!=trigId2)
        {
        trigInfoList.Close();
        
        CleanupStack::PopAndDestroy( notifier );
        CleanupStack::Pop( trig2 );
        CleanupStack::PopAndDestroy( trig1 );
        CleanupStack::PopAndDestroy( &lbt );
        CleanupStack::PopAndDestroy( &lbtserver );
        return KErrGeneral;
        }
    }
    //free memory
    
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
// CTestHybridTriggers::TC_TestSetTriggersState3L
// Settriggersstate  using Area filter synchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState3L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,areafilter);   
    listOptions->SetFilter( areafilter ); 
        CleanupStack::Pop( areafilter );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,listOptions );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    TBool err=EFalse;
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
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(err)
        {
        iLog->Log(_L("State not changed "));    
        return KErrGeneral;
        }
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState4L
// settriggersstate  using Area filter asynchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState4L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateDisabled,wait,areafilter);
        wait->Start();
    listOptions->SetFilter( areafilter ); 
        CleanupStack::Pop( areafilter );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    TBool err=EFalse;
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
        if(State2 !=CLbtTriggerEntry::EStateDisabled)
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
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    if(err)
    {
    iLog->Log(_L("State not changed "));    
    return KErrGeneral;
    }
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState5L
// Settriggersstate  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState5L()
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
    trig1->SetState(CLbtTriggerEntry::EStateEnabled);
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateDisabled,ELbtTrue,areafilter);   
    CleanupStack::Pop( areafilter );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,NULL );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    TBool err=EFalse;
    for(TInt i=0; i<count; i++)
        {
        CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
        CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
                if(State2 !=CLbtTriggerEntry::EStateEnabled)
                {
                err = ETrue;
                }
        }
    if(!err)
    {
    
    trigInfoList.Close();
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
    trigInfoList.Close();
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
// CTestHybridTriggers::TC_TestSetTriggersState6L
// Settriggersstate  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState6L()
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
    trig2->SetState(CLbtTriggerEntry::EStateDisabled);
    
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateEnabled,wait,areafilter);
    wait->Start();
    CleanupStack::Pop( areafilter );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    
    TBool err=EFalse;
    for(TInt i=0; i<count; i++)
        {
        CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
        CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
                if(State2 !=CLbtTriggerEntry::EStateDisabled)
                {
                err = ETrue;
                }
        }
    if(!err)
    {
    
    trigInfoList.Close();
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
    trigInfoList.Close();
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
// CTestHybridTriggers::TC_TestSetTriggersState7L
// Settriggersstate  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState7L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,areafilter);   
        CleanupStack::Pop( areafilter );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,listOptions );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    TBool err=EFalse;
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
    if(err)
    {
    iLog->Log(_L("State not changed "));    
    return KErrGeneral;
    }
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState8L
// Settriggersstate  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState8L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateDisabled,wait,areafilter);
    wait->Start();
        CleanupStack::Pop( areafilter );
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    TBool err=EFalse;
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
    if(State2 !=CLbtTriggerEntry::EStateDisabled)
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
    if(err)
    {
    iLog->Log(_L("State not changed "));    
    return KErrGeneral;
    }
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState9L
// Settriggersstate using composite filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState9L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    CLbtTriggerFilterByAttribute* attribFilter = 
        CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeStartup );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter ); 
    
    
    compositeFilter->AddFilterL( areafilter ); 
    listOptions->SetFilter( compositeFilter ); 
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,compositeFilter);   
    
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,listOptions );
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( attribFilter );
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    TBool err = EFalse;
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
    if(err)
    {
    iLog->Log(_L("State not changed "));    
    return KErrGeneral;
    }
    //delete wait;
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState10L
// Settriggersstate using composite filter  constructed using Multiple CGI Area asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState10L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    CLbtTriggerFilterByAttribute* attribFilter = 
        CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeStartup );
    attribFilter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter ); 
    
    
    compositeFilter->AddFilterL( areafilter ); 
    listOptions->SetFilter( compositeFilter ); 
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateDisabled,wait,compositeFilter);
    wait->Start();
    
        
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( attribFilter );
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    TBool err=EFalse;
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
    if(State2 !=CLbtTriggerEntry::EStateDisabled)
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
    if(err)
    {
    iLog->Log(_L("State not changed "));    
    return KErrGeneral;
    }
    //delete wait;
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestSetTriggersState11L
// Settriggersstate using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState11L()
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ,Hybridareaarray2;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      Hybridareaarray2.Append(Cellarea3);
      Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
        
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    
       
    CLbtGeoCell* Cellarea5  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode1,
                                                       Networkidentitycode1,
                                                       Locationareacode1,
                                                       CellId1
                                                           );
    
       
    CLbtGeoCell* Cellarea6  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode4,
                                                       Networkidentitycode4,
                                                       Locationareacode4,
                                                       CellId4
                                                           );
    
    CLbtGeoCell* Cellarea7  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode2,
                                                           Networkidentitycode2,
                                                           Locationareacode2,
                                                           CellId2
                                                               );
        
           
    CLbtGeoCell* Cellarea8  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode3,
                                                           Networkidentitycode3,
                                                           Locationareacode3,
                                                           CellId3
                                                               );
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3,Hybridareaarray4 ;
      Hybridareaarray3.Append(Cellarea5);
      Hybridareaarray3.Append(Cellarea6);
      Hybridareaarray4.Append(Cellarea7);
      Hybridareaarray4.Append(Cellarea8);
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray3);
    CLbtGeoHybrid* Hybridarea4  = CLbtGeoHybrid::NewL(Hybridareaarray4);
    
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter1 );  
    CLbtTriggerFilterByArea* areafilter2=CLbtTriggerFilterByArea::NewL(Hybridarea4); 
        CleanupStack::PushL( areafilter2 );  
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areafilter1 ); 
    
    
    compositeFilter->AddFilterL( areafilter2 ); 
    listOptions->SetFilter( compositeFilter ); 
    lbt.SetTriggersStateL(CLbtTriggerEntry::EStateEnabled,ELbtTrue,compositeFilter);  
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    lbt.GetTriggersL(trigInfoList,listOptions );
    CleanupStack::Pop( compositeFilter );
        CleanupStack::Pop( areafilter2 );
        CleanupStack::Pop( areafilter1 );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    TBool err=EFalse;
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=CLbtTriggerEntry::EStateEnabled)
    {
    err = ETrue;
    }
    if(err)
        {
        trigInfoList.Close();
        CleanupStack::PopAndDestroy( notifier );
        CleanupStack::Pop( trig2 );
        CleanupStack::PopAndDestroy( trig1 );
        CleanupStack::PopAndDestroy( &lbt );
        CleanupStack::PopAndDestroy( &lbtserver );
        return KErrGeneral;
        }
    }
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
// CTestHybridTriggers::TC_TestSetTriggersState12L
// Settriggersstate using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestSetTriggersState12L()
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ,Hybridareaarray2;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      Hybridareaarray2.Append(Cellarea3);
      Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
        
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    
       
    CLbtGeoCell* Cellarea5  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode1,
                                                       Networkidentitycode1,
                                                       Locationareacode1,
                                                       CellId1
                                                           );
    
       
    CLbtGeoCell* Cellarea6  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode4,
                                                       Networkidentitycode4,
                                                       Locationareacode4,
                                                       CellId4
                                                           );
    
    CLbtGeoCell* Cellarea7  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode2,
                                                           Networkidentitycode2,
                                                           Locationareacode2,
                                                           CellId2
                                                               );
        
           
    CLbtGeoCell* Cellarea8  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode3,
                                                           Networkidentitycode3,
                                                           Locationareacode3,
                                                           CellId3
                                                               );
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3,Hybridareaarray4 ;
      Hybridareaarray3.Append(Cellarea5);
      Hybridareaarray3.Append(Cellarea6);
      Hybridareaarray4.Append(Cellarea7);
      Hybridareaarray4.Append(Cellarea8);
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray3);
    CLbtGeoHybrid* Hybridarea4  = CLbtGeoHybrid::NewL(Hybridareaarray4);
    
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter1 );  
    CLbtTriggerFilterByArea* areafilter2=CLbtTriggerFilterByArea::NewL(Hybridarea4); 
        CleanupStack::PushL( areafilter2 );  
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areafilter1 ); 
    
    
    compositeFilter->AddFilterL( areafilter2 ); 
    listOptions->SetFilter( compositeFilter ); 
    notifier->UpdateTriggersState(lbt,CLbtTriggerEntry::EStateDisabled,wait,compositeFilter);
    wait->Start();
    
    RPointerArray < CLbtTriggerInfo > trigInfoList;
    notifier->GetTriggersList(lbt,trigInfoList,wait,listOptions);
    wait->Start();
    CleanupStack::Pop( compositeFilter );
        CleanupStack::Pop( areafilter2 );
        CleanupStack::Pop( areafilter1 );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = trigInfoList.Count();
    TBool err=EFalse;
    for ( TInt i = 0; i < count; i++ )
    {
    CLbtTriggerEntry *TrigEntry =trigInfoList[i]->TriggerEntry();
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ; 
    if(State2 !=CLbtTriggerEntry::EStateDisabled)
    {
    err = ETrue;
    }
    if(err)
        {
        trigInfoList.Close();
        CleanupStack::PopAndDestroy( notifier );
        CleanupStack::Pop( trig2 );
        CleanupStack::PopAndDestroy( trig1 );
        CleanupStack::PopAndDestroy( &lbt );
        CleanupStack::PopAndDestroy( &lbtserver );
        return KErrGeneral;
        }
    }
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
// CTestHybridTriggers::TC_TestListtriggerIds3L
// listing trigger ids  using Area filter synchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds3L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray< TLbtTriggerId>  aTriggerIdList;
    lbt.ListTriggerIdsL (aTriggerIdList,listOptions);
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds4L
// listing trigger ids  using Area filter asynchronously when both the cells are matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds4L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray< TLbtTriggerId>  aTriggerIdList;
        notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait,listOptions);
        wait->Start();
        CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    CleanupStack::Pop( trig1 );
    CleanupStack::PopAndDestroy( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    //delete wait;
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds5L
// listing trigger ids  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds5L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray< TLbtTriggerId>  aTriggerIdList;
        lbt.ListTriggerIdsL (aTriggerIdList,listOptions);
        CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = aTriggerIdList.Count();
    
    if(count==0)
    {
    aTriggerIdList.Close();
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
    aTriggerIdList.Close();
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
// CTestHybridTriggers::TC_TestListtriggerIds6L
// listing trigger ids  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds6L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray< TLbtTriggerId>  aTriggerIdList;
        notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait,listOptions);
        wait->Start();
        CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
    TInt count = aTriggerIdList.Count();
    
    if(count==0)
    {
    aTriggerIdList.Close();
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
    aTriggerIdList.Close();
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
// CTestHybridTriggers::TC_TestListtriggerIds7L
// listing trigger ids  using Area filter synchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds7L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
        
        RArray< TLbtTriggerId>  aTriggerIdList;
            lbt.ListTriggerIdsL (aTriggerIdList,listOptions);
            CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds8L
// listing trigger ids  using Area filter asynchronously when both the cells are not matching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds8L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    
    listOptions->SetFilter( areafilter ); 
    
    RArray< TLbtTriggerId>  aTriggerIdList;
        notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait,listOptions);
        wait->Start();
        CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    return KErrNone;    
    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds9L
// listing trigger ids using composite filter  constructed using Multiple CGI Area synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds9L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    CLbtTriggerFilterByAttribute* attribFilter = 
        CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter ); 
    
    
    compositeFilter->AddFilterL( areafilter ); 
    listOptions->SetFilter( compositeFilter ); 
    
    
    RArray< TLbtTriggerId>  aTriggerIdList;
    lbt.ListTriggerIdsL (aTriggerIdList,listOptions);
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( attribFilter );
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds10L
// listing trigger ids using composite filter  constructed using Multiple CGI Area asynchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds10L()
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    TBuf<4> Networkcountrycode3 = _L("1000");
    TBuf<8> Networkidentitycode3 = _L("10000000");
    TUint Locationareacode3 = 100;
    TUint CellId3 = 100;
       
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
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray2);
    
    CLbtTriggerFilterByArea* areafilter=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter );  
    CLbtTriggerFilterByAttribute* attribFilter = 
        CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL( attribFilter );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( attribFilter ); 
    
    
    compositeFilter->AddFilterL( areafilter ); 
    listOptions->SetFilter( compositeFilter ); 
    
    
    RArray< TLbtTriggerId>  aTriggerIdList;
    notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait,listOptions);
    wait->Start();
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( attribFilter );
    CleanupStack::Pop( areafilter );
    CleanupStack::PopAndDestroy( listOptions );
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
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds11L
// listing trigger ids using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds11L()
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ,Hybridareaarray2;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      Hybridareaarray2.Append(Cellarea3);
      Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
        
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    
       
    CLbtGeoCell* Cellarea5  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode1,
                                                       Networkidentitycode1,
                                                       Locationareacode1,
                                                       CellId1
                                                           );
    
       
    CLbtGeoCell* Cellarea6  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode4,
                                                       Networkidentitycode4,
                                                       Locationareacode4,
                                                       CellId4
                                                           );
    
    CLbtGeoCell* Cellarea7  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode2,
                                                           Networkidentitycode2,
                                                           Locationareacode2,
                                                           CellId2
                                                               );
        
           
    CLbtGeoCell* Cellarea8  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode3,
                                                           Networkidentitycode3,
                                                           Locationareacode3,
                                                           CellId3
                                                               );
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3,Hybridareaarray4 ;
      Hybridareaarray3.Append(Cellarea5);
      Hybridareaarray3.Append(Cellarea6);
      Hybridareaarray4.Append(Cellarea7);
      Hybridareaarray4.Append(Cellarea8);
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray3);
    CLbtGeoHybrid* Hybridarea4  = CLbtGeoHybrid::NewL(Hybridareaarray4);
    
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter1 );  
    CLbtTriggerFilterByArea* areafilter2=CLbtTriggerFilterByArea::NewL(Hybridarea4); 
        CleanupStack::PushL( areafilter2 );  
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areafilter1 ); 
    
    
    compositeFilter->AddFilterL( areafilter2 ); 
    listOptions->SetFilter( compositeFilter ); 
    
    RArray< TLbtTriggerId>  aTriggerIdList;
    lbt.ListTriggerIdsL (aTriggerIdList,listOptions);
    CleanupStack::Pop( compositeFilter );
    CleanupStack::Pop( areafilter2 );
    CleanupStack::Pop( areafilter1 );
    CleanupStack::PopAndDestroy( listOptions );
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
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CTestHybridTriggers::TC_TestListtriggerIds12L
// listing trigger ids using composite filter  constructed using Two Multiple CGI Area filters synchronously
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestHybridTriggers ::TC_TestListtriggerIds12L()
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
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray1 ,Hybridareaarray2;
      Hybridareaarray1.Append(Cellarea1);
      Hybridareaarray1.Append(Cellarea2);
      Hybridareaarray2.Append(Cellarea3);
      Hybridareaarray2.Append(Cellarea4);
    CLbtGeoHybrid* Hybridarea1  = CLbtGeoHybrid::NewL(Hybridareaarray1);
    CLbtGeoHybrid* Hybridarea2  = CLbtGeoHybrid::NewL(Hybridareaarray2);
        
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
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listOptions );
    //construct filter
    
       
    CLbtGeoCell* Cellarea5  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode1,
                                                       Networkidentitycode1,
                                                       Locationareacode1,
                                                       CellId1
                                                           );
    
       
    CLbtGeoCell* Cellarea6  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                       Networkcountrycode4,
                                                       Networkidentitycode4,
                                                       Locationareacode4,
                                                       CellId4
                                                           );
    
    CLbtGeoCell* Cellarea7  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode2,
                                                           Networkidentitycode2,
                                                           Locationareacode2,
                                                           CellId2
                                                               );
        
           
    CLbtGeoCell* Cellarea8  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeGsm,
                                                           Networkcountrycode3,
                                                           Networkidentitycode3,
                                                           Locationareacode3,
                                                           CellId3
                                                               );
                                                           
    RPointerArray<CLbtGeoAreaBase> Hybridareaarray3,Hybridareaarray4 ;
      Hybridareaarray3.Append(Cellarea5);
      Hybridareaarray3.Append(Cellarea6);
      Hybridareaarray4.Append(Cellarea7);
      Hybridareaarray4.Append(Cellarea8);
    
    CLbtGeoHybrid* Hybridarea3  = CLbtGeoHybrid::NewL(Hybridareaarray3);
    CLbtGeoHybrid* Hybridarea4  = CLbtGeoHybrid::NewL(Hybridareaarray4);
    
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(Hybridarea3); 
    CleanupStack::PushL( areafilter1 );  
    CLbtTriggerFilterByArea* areafilter2=CLbtTriggerFilterByArea::NewL(Hybridarea4); 
        CleanupStack::PushL( areafilter2 );  
    
    //Construct a composite filter
    CLbtTriggerFilterComposite* compositeFilter = 
        CLbtTriggerFilterComposite::NewL( 
            CLbtTriggerFilterComposite::ECompositionTypeAnd );
    
    CleanupStack::PushL( compositeFilter );
    
    //Add filters to the list option object
    compositeFilter->AddFilterL( areafilter1 ); 
    
    
    compositeFilter->AddFilterL( areafilter2 ); 
    listOptions->SetFilter( compositeFilter ); 
    
    RArray< TLbtTriggerId>  aTriggerIdList;
       notifier->ListTriggerIdsList(lbt,aTriggerIdList,wait,listOptions);
       wait->Start();
       CleanupStack::Pop( compositeFilter );
           CleanupStack::Pop( areafilter2 );
           CleanupStack::Pop( areafilter1 );
    CleanupStack::PopAndDestroy( listOptions );
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
    return KErrNone;    
    }



