/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Test module to implement Test cases for BC
*
*/




// System Includes


// User Includes
#include "testlbtclasses.h"
#include <e32std.h>
#include <s32mem.h>
#include "AsynOperationAO.h"
#include <centralrepository.h>
#include <SimulationPSYInternalCRKeys.h>
#include <EPos_MPosModulesObserver.h> 
#include <EPos_CPosModuleIdList.h>
#include <EPos_CPosModules.h>
#include <EPos_CPosModuleUpdate.h> // CPosModuleUpdate
// Constant Declarations

    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
TTestlbtclasses ::TTestlbtclasses (CStifLogger* aLog):iLog(aLog)
    {
        
    }

// ---------------------------------------------------------------------------
// C++ Destructor
// ---------------------------------------------------------------------------
//
TTestlbtclasses ::~TTestlbtclasses ()
    {
        
    }
void TTestlbtclasses::AssertTrueL( TBool aCondition,const TDesC& aMessage )
    {
           
         if(!aCondition)
         {
          
    	   iLog->Log( aMessage );
           User::LeaveIfError(KErrGeneral);
         }
    
    }
void TTestlbtclasses::EnableSimPSYL() 
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
    
// ---------------------------------------------------------------------------
// Test case for setting & getting requestor
// ---------------------------------------------------------------------------
TInt TTestlbtclasses ::TestCLbtGeoCell1L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea );
    
    Cellarea->SetNetworkType(RMobilePhone :: ENetworkModeGsm);
    
    TBuf<4> Networkcountrycode = _L("1000");
    Cellarea->SetNetworkCountryCode(Networkcountrycode);
    
    TBuf<8> Networkidentitycode = _L("10000000");
    Cellarea->SetNetworkIdentityCode(Networkidentitycode);
    
    TUint Locationareacode = 100;
    Cellarea->SetLocationAreaCode(Locationareacode);
    
    TUint CellId = 100;
    Cellarea->SetCellId(CellId);
    
        
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(Cellarea->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(Cellarea->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(Cellarea->CellId()==CellId,_L("Wrong Cellid"));         
     
    CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;
    }
    
       
    
    TInt TTestlbtclasses ::TestCLbtGeoCell2L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewLC();
    
    Cellarea->SetNetworkType(RMobilePhone :: ENetworkModeGsm);
    
    TBuf<4> Networkcountrycode = _L("1000");
    Cellarea->SetNetworkCountryCode(Networkcountrycode);
    
    TBuf<8> Networkidentitycode = _L("10000000");
    Cellarea->SetNetworkIdentityCode(Networkidentitycode);
    
    TUint Locationareacode = 100;
    Cellarea->SetLocationAreaCode(Locationareacode);
    
    TUint CellId = 100;
    Cellarea->SetCellId(CellId);
    
     
    
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(Cellarea->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(Cellarea->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(Cellarea->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));   
        
    CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;

    }
    
    
    TInt TTestlbtclasses ::TestCLbtGeoCell3L(CStifItemParser& /*aItem*/)
    {
    
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
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea );
       
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(Cellarea->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(Cellarea->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(Cellarea->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));   
        
    CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;

    }
    
    TInt TTestlbtclasses ::TestCLbtGeoCell4L(CStifItemParser& /*aItem*/)
    {
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
     
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewLC(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
           
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(Cellarea->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(Cellarea->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(Cellarea->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));   
        
    CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;


    }
    
     TInt TTestlbtclasses ::TestCLbtGeoCell5L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea );
    
    Cellarea->SetNetworkType(RMobilePhone :: ENetworkModeGsm);
    TBuf<4> Networkcountrycode = _L("1000");
    Cellarea->SetNetworkCountryCode(Networkcountrycode);
    
    TBuf<8> Networkidentitycode = _L("10000000");
    Cellarea->SetNetworkIdentityCode(Networkidentitycode);
    
    TUint Locationareacode = 100;
    Cellarea->SetLocationAreaCode(Locationareacode);
    
    TUint CellId = 100;
    Cellarea->SetCellId(CellId);
    
   
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    Cellarea->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtGeoCell* Cellarea2 = CLbtGeoCell::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea2 ); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    Cellarea2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
	AssertTrueL(Cellarea2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cellarea2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(Cellarea2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(Cellarea2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(Cellarea2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(Cellarea2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));  
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtGeoCell6L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( Cellarea );
    
    Cellarea->SetNetworkType(RMobilePhone :: ENetworkModeWcdma);
    
    TBuf<4> Networkcountrycode = _L("1000");
    Cellarea->SetNetworkCountryCode(Networkcountrycode);
    
    TBuf<8> Networkidentitycode = _L("10000000");
    Cellarea->SetNetworkIdentityCode(Networkidentitycode);
    
    TUint Locationareacode = 100;
    Cellarea->SetLocationAreaCode(Locationareacode);
    
    TUint CellId = 100;
    Cellarea->SetCellId(CellId);
    
        
    AssertTrueL(Cellarea->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cellarea->NetworkType()==RMobilePhone::ENetworkModeWcdma,_L("Wrong network Type")); 
    AssertTrueL(Cellarea->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(Cellarea->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(Cellarea->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(Cellarea->CellId()==CellId,_L("Wrong Cellid"));         
   
        
    CleanupStack::PopAndDestroy( Cellarea ); 
    
    return KErrNone;
    }
    
    
    TInt TTestlbtclasses ::TestCLbtGeoCell7L(CStifItemParser& /*aItem*/)
    {
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
    
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
        
    cond1->SetTriggerArea( Cellarea );
    
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
    
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (base);
    }
    
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
	AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
	AssertTrueL(cell2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
	AssertTrueL(cell2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
        
        
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }
    
    
    TInt TTestlbtclasses ::TestCLbtGeoCircle1L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoCircle* circle = CLbtGeoCircle::NewL();
    
    TReal radius=circle->Radius();
    TCoordinate center1=circle->Center();
    CleanupStack::PushL( circle );
    TCoordinate center2(5,10,15);
    circle->SetCenter(center2);
    circle->SetRadius(100);
    
    TCoordinate center3=circle->Center();
    TReal radius2=circle->Radius();
    AssertTrueL(center3.Latitude()==5 && center3.Longitude()==10 && center3.Altitude()==15,_L("wrong centre"));
    AssertTrueL( radius2==100,_L("Wrong Radius"));
    
        
    CleanupStack::Pop( circle ); 
    delete circle;
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtGeoCircle2L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoCircle* circle = CLbtGeoCircle::NewLC();
    
    TReal radius=circle->Radius();
    TCoordinate center=circle->Center();
    
    TCoordinate center2(5,10,15);
    circle->SetCenter(center2);
    circle->SetRadius(100);
    
    TCoordinate center3=circle->Center();
    TReal radius2=circle->Radius();
    AssertTrueL(center3.Latitude()==5 && center3.Longitude()==10 && center3.Altitude()==15,_L("wrong centre"));
    AssertTrueL( radius2==100,_L("Wrong Radius"));
    
        
    CleanupStack::PopAndDestroy( circle ); 
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtGeoCircle3L(CStifItemParser& /*aItem*/)
    {
    TCoordinate center(5,10,15);
    
    CLbtGeoCircle* circle = CLbtGeoCircle::NewL(center,100);
    CleanupStack::PushL( circle );
            
    TReal radius=circle->Radius();
    TCoordinate center2=circle->Center();
    
    AssertTrueL(center2.Latitude()==5 && center2.Longitude()==10 && center2.Altitude()==15,_L("wrong centre"));
    AssertTrueL( radius==100,_L("Wrong Radius"));
        
    CleanupStack::PopAndDestroy( circle ); 
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtGeoCircle4L(CStifItemParser& /*aItem*/)
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
    
    TInt TTestlbtclasses ::TestCLbtGeoCircle5L(CStifItemParser& /*aItem*/)
    {
    TCoordinate center(5,10,15);
    
    CLbtGeoCircle* circle = CLbtGeoCircle::NewLC(center,100);
    
    TReal radius=circle->Radius();
    TCoordinate center2=circle->Center();
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
     
    CLbtGeoCell* Cellarea  = CLbtGeoCell::NewLC(RMobilePhone :: ENetworkModeGsm,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
   														
    circle->SetAdditionalGeoAreaInfo(Cellarea);
    
    RPointerArray< CLbtGeoAreaBase > addnlgeoinfo;
    addnlgeoinfo = circle->GetAdditionalGeoAreaInfo();
    
   /* CLbtGeoCell* Cell;
    Cell = static_cast<CLbtGeoCell*>(addnlgeoinfo[0]);
    
    AssertTrueL(Cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(Cell->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(Cell->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(Cell->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(Cell->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(Cell->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(Cell->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));  */
    
    CleanupStack::PopAndDestroy( Cellarea );     
    CleanupStack::PopAndDestroy( circle ); 
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtGeoCircle6L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoCircle* circle = CLbtGeoCircle::NewL();
    
    TReal radius=circle->Radius();
    TCoordinate center1=circle->Center();
    CleanupStack::PushL( circle );
    TCoordinate center2(5,10,15);
    circle->SetCenter(center2);
    circle->SetRadius(100);
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    circle->ExternalizeL(wr);
    
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
    
    TCoordinate center3=circle2->Center();
    TReal radius2=circle2->Radius();
    AssertTrueL(center3.Latitude()==5 && center3.Longitude()==10 && center3.Altitude()==15,_L("wrong centre"));
    AssertTrueL( radius2==100,_L("Wrong Radius"));
       
    CleanupStack::PopAndDestroy( 3 ); 
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtGeoRect1L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( rect );  
    
    rect->SetBorder(-90,90,-180,180);
    
    TReal aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude ; 
    rect->GetBorder(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude);
    
    AssertTrueL( aSouthLatitude==-90 && aNorthLatitude==90 &&
                 aWestLongitude==-180 && aEastLongitude==180,_L("Wrong Border values"));
    
    CleanupStack::Pop(1);
    
    delete rect;
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtGeoRect2L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoRect* rect = CLbtGeoRect::NewLC();
    
    rect->SetBorder(-90,90,-180,180);
    
    TReal aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude ; 
    rect->GetBorder(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude);
    
    AssertTrueL( aSouthLatitude==-90 && aNorthLatitude==90 &&
                 aWestLongitude==-180 && aEastLongitude==180,_L("Wrong Border values"));
    
    CleanupStack::PopAndDestroy(1);
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtGeoRect3L(CStifItemParser& /*aItem*/)
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
    
    TInt TTestlbtclasses ::TestCLbtGeoRect4L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoRect* rect = CLbtGeoRect::NewLC(10,20,30,40);
       
    TReal aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude ; 
    
    AssertTrueL(rect->Type()==CLbtGeoAreaBase::ERectangular,_L("Wrong Type"));   
    
    rect->GetBorder(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude );

    AssertTrueL(aSouthLatitude==10 && aNorthLatitude==20 && aWestLongitude==30 && aEastLongitude==40,_L("Borders are incorrect"));   
    
    CleanupStack::PopAndDestroy( rect ); 
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtGeoRect5L(CStifItemParser& /*aItem*/)
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
    
    TInt TTestlbtclasses ::TestCLbtListTriggerOptions1L(CStifItemParser& /*aItem*/)
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
                                     TLbtTriggerDynamicInfo::EDistanceToLatestLocation
                                     );
                                     

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
    
    TInt TTestlbtclasses ::TestCLbtListTriggerOptions2L(CStifItemParser& /*aItem*/)
    {
    TCoordinate center;
    TLbtTriggerAttributeFieldsMask aEntryMask;
    TLbtTriggerDynamicInfoFieldsMask aDynInfoMask;
    
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewLC();
    
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
    
    
    TInt TTestlbtclasses ::TestCLbtListTriggerOptions3L(CStifItemParser& /*aItem*/)
    {
    TCoordinate center;
    TLbtTriggerAttributeFieldsMask aEntryMask;
    TLbtTriggerDynamicInfoFieldsMask aDynInfoMask;
    
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    listoptions->SetRetrievedFields( CLbtTriggerEntry::EAttributeState |  
                                     CLbtTriggerEntry::EAttributeName | 
                                     CLbtTriggerEntry::EAttributeManagerUi,
                                     TLbtTriggerDynamicInfo::EValidityStatus |
                                     TLbtTriggerDynamicInfo::EDistanceToLatestLocation|
                                     TLbtTriggerDynamicInfo::EFiredLocality);
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    listoptions->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtListTriggerOptions* listoptions2 = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions2 );
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    listoptions2->InternalizeL(rd);
    listoptions2->GetRetrievedFields( aEntryMask,aDynInfoMask );
    
    AssertTrueL(aEntryMask==0x16,_L("wrong entry field mask"));
    AssertTrueL(aDynInfoMask==0x7,_L("wrong dyn. info field mask"));
    CleanupStack::PopAndDestroy();
    buffer->Reset();
    CleanupStack::PopAndDestroy( 3 ); 
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtListTriggerOptions4L(CStifItemParser& /*aItem*/)
    {
    TCoordinate center;
    //TLbtTriggerAttributeFieldsMask aEntryMask;
    //TLbtTriggerDynamicInfoFieldsMask aDynInfoMask;
    CLbtTriggerFilterByAttribute* filter1=CLbtTriggerFilterByAttribute::NewL();
    filter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    filter1->AddTriggerManagerUiL( TUid::Uid(5) );
    
    CLbtTriggerFilterComposite* compfilter1=CLbtTriggerFilterComposite::NewL();
    
    compfilter1->AddFilterL( filter1 );
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    listoptions->SetFilter( compfilter1 );
    
    listoptions->SetSortingOption(CLbtListTriggerOptions::ELbtTriggerNameAscending);
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    listoptions->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtListTriggerOptions* listoptions2 = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions2 );
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    listoptions2->InternalizeL(rd);
    AssertTrueL( listoptions2->SortingOption()==CLbtListTriggerOptions::ELbtTriggerNameAscending,_L("wrong sorting option"));
    CleanupStack::PopAndDestroy();
    buffer->Reset();
    CleanupStack::PopAndDestroy( 3 ); 
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtListTriggerOptions5L(CStifItemParser& /*aItem*/)
    {
    TCoordinate center;
//    TLbtTriggerAttributeFieldsMask aEntryMask;
//    TLbtTriggerDynamicInfoFieldsMask aDynInfoMask;
    CLbtTriggerFilterByAttribute* filter1=CLbtTriggerFilterByAttribute::NewL();
    filter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    filter1->AddTriggerManagerUiL( TUid::Uid(5) );
    
    CLbtTriggerFilterComposite* compfilter1=CLbtTriggerFilterComposite::NewL();
           
    compfilter1->AddFilterL( filter1 );
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    listoptions->SetFilter( compfilter1 );
    
    listoptions->SetSortingOption(CLbtListTriggerOptions::ELbtTriggerNameDescending );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    listoptions->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtListTriggerOptions* listoptions2 = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions2 );
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    listoptions2->InternalizeL(rd);
    AssertTrueL( listoptions2->SortingOption()==CLbtListTriggerOptions::ELbtTriggerNameDescending ,_L("wrong sorting option"));
    CleanupStack::PopAndDestroy();
    buffer->Reset();
    CleanupStack::PopAndDestroy( 3 ); 
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtListTriggerOptions6L(CStifItemParser& /*aItem*/)
    {
    TCoordinate center;
//    TLbtTriggerAttributeFieldsMask aEntryMask;
//    TLbtTriggerDynamicInfoFieldsMask aDynInfoMask;
    CLbtTriggerFilterByAttribute* filter1=CLbtTriggerFilterByAttribute::NewL();
    filter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    filter1->AddTriggerManagerUiL( TUid::Uid(5) );
    
    CLbtTriggerFilterComposite* compfilter1=CLbtTriggerFilterComposite::NewL();
           
    compfilter1->AddFilterL( filter1 );
    CLbtListTriggerOptions* listoptions=CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions );
    listoptions->SetFilter( compfilter1 );
    
    listoptions->SetSortingOption(CLbtListTriggerOptions::ELbtDistanceToLatestLocationAscending  );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    listoptions->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtListTriggerOptions* listoptions2 = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL( listoptions2 );
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    listoptions2->InternalizeL(rd);
    AssertTrueL( listoptions2->SortingOption()==CLbtListTriggerOptions::ELbtDistanceToLatestLocationAscending  ,_L("wrong sorting option"));
    CleanupStack::PopAndDestroy();
    buffer->Reset();
    CleanupStack::PopAndDestroy( 3 ); 
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestRLbtServerL(CStifItemParser& /*aItem*/)
    {
    RLbtServer	lbtServ;
           
    TVersion LBTServerVersion= lbtServ.Version(); 	
    
    iLog->Log(_L("VersionMajor =%d VersionMinor =%d VersionBuild =%d"),
    			LBTServerVersion.iBuild,LBTServerVersion.iMajor,LBTServerVersion.iMinor); 
    
    //Close connection to RLbtServer
    lbtServ.Close();    
    
    //Return value			
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtSessionTrigger1L(CStifItemParser& /*aItem*/)
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
    CleanupStack::Pop(trig);
    delete trig;
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtSessionTrigger2L(CStifItemParser& /*aItem*/)
    {
    //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewLC();
        
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
    
    TInt TTestlbtclasses ::TestCLbtSessionTrigger3L(CStifItemParser& /*aItem*/)
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
    
    TInt TTestlbtclasses ::TestCLbtSessionTrigger4L(CStifItemParser& /*aItem*/)
    {
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
   
    //Close connection to Location Triggering Server.
        
    lbt.Close();
    CleanupStack::PopAndDestroy(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;

    }
    
    
    
    TInt TTestlbtclasses ::TestCLbtSessionTrigger5L(CStifItemParser& /*aItem*/)
    {
    //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
         
    // Set Name
    trig->SetNameL(_L("Name"));
    
    // Set trigger state
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    
    RRequestorStack reqStack;
    CleanupClosePushL( reqStack );
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
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    trig->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
    CLbtSessionTrigger* trig2 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 ); 
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    trig2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset();
    
    const TDesC name2= trig2->Name();
    //AssertTrueL( name2.Compare(_L("Name"))==0,_L("Wrong Name"));
    
    AssertTrueL( trig2->State()== CLbtTriggerEntry::EStateDisabled ,_L("Wrong state"));
    RRequestorStack reqStack2;
    trig2->GetRequestorsL( reqStack2 );
    TInt count=reqStack2.Count();
    AssertTrueL( count==1 ,_L("Wrong requestors count"));
    
     
    TDesC data=reqStack2[0]->RequestorData();
    
    AssertTrueL( reqStack2[0]->RequestorType()==CRequestorBase::ERequestorService && 
    reqStack2[0]->RequestorFormat()==CRequestorBase::EFormatApplication && data.Compare(KMyRequestorName),
    _L("Wrong Requestors")); 
    
    AssertTrueL( trig2->ManagerUi()==TUid::Uid(4),_L("Wrong Manager Ui"));

    CLbtTriggerConditionBase* cond= trig2->GetCondition();
    
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
       
    
    CleanupStack::PopAndDestroy( 4 ); 
    CleanupStack::PopAndDestroy(trig);
    
    return KErrNone;
    }
    
     TInt TTestlbtclasses ::TestCLbtStartupTrigger1L(CStifItemParser& /*aItem*/)
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
    
   
    CleanupStack::Pop( 1 ); 
    
    delete trig;
    
    return KErrNone;

    }
    
     TInt TTestlbtclasses ::TestCLbtStartupTrigger2L(CStifItemParser& /*aItem*/)
    {
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewLC();
    
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
    
     TInt TTestlbtclasses ::TestCLbtStartupTrigger3L(CStifItemParser& /*aItem*/)
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
    
    
    TInt TTestlbtclasses ::TestCLbtStartupTrigger4L(CStifItemParser& /*aItem*/)
    {
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
    lbt.Close();
    CleanupStack::PopAndDestroy(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);  
    return KErrNone;
    }
    
    
     TInt TTestlbtclasses ::TestCLbtStartupTrigger5L(CStifItemParser& /*aItem*/)
    {
    //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
         
    // Check for trigger type
    AssertTrueL( trig->Type()== CLbtTriggerEntry::ETypeStartup ,_L("Wrong type") );
    
    TBuf<256> filename;
    TSecureId secureid;
   
    // Set Process Id & command Line string
    _LIT( KFileName, "HandlingProcess.exe" );
//    _LIT( KCommandLine, "CommandLine" );
    filename.Format(KFileName);
    TUint32 sid=0x1333;
    secureid=sid;
   
    trig->SetProcessId(filename,secureid);
    
    trig->SetCommandLineL( _L("CommandLine") );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    trig->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
     
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
    AssertTrueL( trig2->CommandLine().Compare(_L("CommandLine"))==0 ,_L("Wrong CommandLine"));
    
   
    CleanupStack::PopAndDestroy( 3 ); 
    
    return KErrNone;

    }
    
    
    TInt TTestlbtclasses ::TestCLbtTriggerChangeEventNotifierL(CStifItemParser& /*aItem*/)
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
    CleanupStack::PopAndDestroy(&lbt2);
    CleanupStack::PopAndDestroy(&lbt1);
    CleanupStack::PopAndDestroy(&server);
 
    return KErrNone;
    }
    
    
    TInt TTestlbtclasses ::TestCLbtTriggerConditionArea1L(CStifItemParser& /*aItem*/)
    {
 	CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
    
    TCoordinate center(5,10,15);
    
    CLbtGeoCircle* circle1= CLbtGeoCircle::NewL( center,10 );
        
    cond1->SetTriggerArea( circle1 );
    
    CLbtGeoAreaBase* base=cond1->TriggerArea();
    
    CLbtGeoCircle* circle2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECircle)
    {
    	circle2= static_cast<CLbtGeoCircle*> (base);
    }
    
    center= circle2->Center();
    
    AssertTrueL( center.Latitude()==5 && center.Longitude()==10 && center.Altitude()==15,_L("Wrong Center"));
    
    AssertTrueL( circle2->Radius()==10,_L("Wrong Radius"));
    
    AssertTrueL( cond1->Direction()== CLbtTriggerConditionArea::EFireOnExit ,_L("Wrong Direction"));
        
        
    CleanupStack::Pop( 1 );
    
    delete cond1;
    
    return KErrNone;
    
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerConditionArea2L(CStifItemParser& /*aItem*/)
    {
 		CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewLC( );
        
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnExit );
    
    TCoordinate center(5,10,15);
    
    CLbtGeoCircle* circle1= CLbtGeoCircle::NewL( center,10 );
        
    cond1->SetTriggerArea( circle1 );
    
    CLbtGeoAreaBase* base=cond1->TriggerArea();
    
    CLbtGeoCircle* circle2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECircle)
    {
    	circle2= static_cast<CLbtGeoCircle*> (base);
    }
    
    center= circle2->Center();
    
    AssertTrueL( center.Latitude()==5 && center.Longitude()==10 && center.Altitude()==15,_L("Wrong Center"));
    
    AssertTrueL( circle2->Radius()==10,_L("Wrong Radius"));
    
    AssertTrueL( cond1->Direction()== CLbtTriggerConditionArea::EFireOnExit ,_L("Wrong Direction"));
        
        
    CleanupStack::PopAndDestroy( 1 );
    
    return KErrNone;
    
    }
    
    
    TInt TTestlbtclasses ::TestCLbtTriggerConditionArea3L(CStifItemParser& /*aItem*/)
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
    
    
     TInt TTestlbtclasses ::TestCLbtTriggerConditionArea4L(CStifItemParser& /*aItem*/)
    {
 	
    TCoordinate coordinate(5,10,15);
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,10);
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnExit);
    
    CleanupStack::PushL( condition );                                             
    CLbtGeoAreaBase* base=condition->TriggerArea();
    
    CLbtGeoCircle* circle2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECircle)
    {
    	circle2= static_cast<CLbtGeoCircle*> (base);
    }
    
    coordinate= circle2->Center();
    
    AssertTrueL( coordinate.Latitude()==5 && coordinate.Longitude()==10 && coordinate.Altitude()==15,_L("Wrong Center"));
    
    AssertTrueL( circle2->Radius()==10,_L("Wrong Radius"));
    
    AssertTrueL( condition->Direction()== CLbtTriggerConditionArea::EFireOnExit ,_L("Wrong Direction"));
        
        
    CleanupStack::PopAndDestroy( 1 );
    
    return KErrNone;
        
    
    }
    
     TInt TTestlbtclasses ::TestCLbtTriggerConditionArea5L(CStifItemParser& /*aItem*/)
    {
 	
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnEnter );
    
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
        
    cond1->SetTriggerArea( Cellarea );
    
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
    
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (base);
    }
    
     AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(cell2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(cell2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
        
        
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }
    
      TInt TTestlbtclasses ::TestCLbtTriggerConditionArea6L(CStifItemParser& /*aItem*/)
    {
 	
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnEnter );
    
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeWcdma,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
        
    cond1->SetTriggerArea( Cellarea );
    
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
    
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (base);
    }
    
     AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeWcdma,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(cell2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(cell2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
        
        
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }
    
    
     TInt TTestlbtclasses ::TestCLbtTriggerConditionArea7L(CStifItemParser& /*aItem*/)
    {
 	
    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    rect->SetBorder(10,90,-180,180);
    
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( rect,CLbtTriggerConditionArea::EFireOnEnter);
    CleanupStack::PushL( cond1 ); 
    
  //  cond1->SetDirection( CLbtTriggerConditionArea::EFireOnEnter );
    
  //  cond1->SetTriggerArea( rect );
    
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
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }
     
     TInt TTestlbtclasses ::TestCLbtTriggerConditionArea8L(CStifItemParser& /*aItem*/)
    {
 	
    CLbtTriggerConditionArea* cond1= CLbtTriggerConditionArea::NewL( );
    CleanupStack::PushL( cond1 ); 
    
    cond1->SetDirection( CLbtTriggerConditionArea::EFireOnEnter );
       	        
    cond1->SetTriggerArea( NULL );
    
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
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerEntry1L(CStifItemParser& /*aItem*/)
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
    
    //SetId
    trig->SetId(10);
    
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
    
    //Set ReArm time
    trig->SetTimeToRearm(KLbtDefaultTimeToRearm);
    
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
    
    TInt TimetoReArm = trig->TimeToRearm();
    AssertTrueL( TimetoReArm==KLbtDefaultTimeToRearm,_L("Wrong ReArm time"));   
    TLbtTriggerId trigid = trig->Id();
    AssertTrueL( trigid==10,_L("Wrong id"));   
    CleanupStack::PopAndDestroy( 2 ); 
    CleanupStack::Pop(trig);
    delete trig;
    
    return KErrNone;
        
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerEntry2L(CStifItemParser& /*aItem*/)
    {
 	 //Construct a session trigger
    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
        
    // Set Name
    trig->SetNameL(_L("Name"));
    
    // Set trigger state
    trig->SetState(CLbtTriggerEntry::EStateDisabled);
    
        
    //Set ReArm time
    trig->SetTimeToRearm(KLbtMaxTimeToRearm);
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);    
    trig->ExternalizeL(wr);
    
    wr.CommitL();
    CleanupStack::PopAndDestroy();
    
    CLbtSessionTrigger* trig2 = CLbtSessionTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig2 );
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);    
    trig2->InternalizeL(rd);
    
    CleanupStack::PopAndDestroy();
    
    buffer->Reset(); 
    
    const TDesC name2= trig2->Name();
    //AssertTrueL( name2.Compare(_L("Name"))==0,_L("Wrong Name"));
    
    AssertTrueL( trig2->State()== CLbtTriggerEntry::EStateDisabled ,_L("Wrong state"));
    
       
    TInt TimetoReArm = trig2->TimeToRearm();
    AssertTrueL( TimetoReArm==KLbtMaxTimeToRearm,_L("Wrong ReArm time"));   
    
    CleanupStack::PopAndDestroy( 2 ); 
    CleanupStack::PopAndDestroy(trig);
    
    return KErrNone;
        
    }
    
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterByArea1L(CStifItemParser& /*aItem*/)
    {
 	  TCoordinate coordinate(5,10,15);
    
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,10);
    
    CLbtTriggerFilterByArea* filterarea= CLbtTriggerFilterByArea::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( filterarea); 
    filterarea->SetArea(circle1);
    
    
    CLbtGeoAreaBase* geobase=filterarea->Area();
    
    CLbtGeoCircle* circle2=NULL;
    
    if( geobase->Type()==CLbtGeoAreaBase::ECircle)
    {
    	circle2= static_cast<CLbtGeoCircle*> (geobase); 
    }
    	
    TCoordinate center=circle2->Center();
    
    AssertTrueL( center.Latitude()==5 && center.Longitude()==10 && center.Altitude()==15,_L("Wrong Center"));
    
    AssertTrueL( circle2->Radius()==10,_L("Wrong Radius"));
    
    CleanupStack::Pop( 1 );
    delete filterarea;
    return KErrNone;
        
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterByArea2L(CStifItemParser& /*aItem*/)
    {
 	  TCoordinate coordinate(5,10,15);
    
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,10);
    
    CLbtTriggerFilterByArea* filterarea= CLbtTriggerFilterByArea::NewLC();
        
    filterarea->SetArea(circle1);
        
    CLbtGeoAreaBase* geobase=filterarea->Area();
    
    CLbtGeoCircle* circle2=NULL;
    
    if( geobase->Type()==CLbtGeoAreaBase::ECircle)
    {
    	circle2= static_cast<CLbtGeoCircle*> (geobase); 
    }
    	
    TCoordinate center=circle2->Center();
    
    AssertTrueL( center.Latitude()==5 && center.Longitude()==10 && center.Altitude()==15,_L("Wrong Center"));
    
    AssertTrueL( circle2->Radius()==10,_L("Wrong Radius"));
    
    CleanupStack::PopAndDestroy( 1 );
    
    return KErrNone;
        
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterByArea3L(CStifItemParser& /*aItem*/)
    {
 	  TCoordinate coordinate(5,10,15);
    
    CLbtGeoCircle* circle1=CLbtGeoCircle::NewL(coordinate,10);
    
    CLbtTriggerFilterByArea* filterarea= CLbtTriggerFilterByArea::NewL(circle1);
    CleanupStack::PushL( filterarea);    
    CLbtGeoAreaBase* geobase=filterarea->Area();
    
    CLbtGeoCircle* circle2=NULL;
    
    if( geobase->Type()==CLbtGeoAreaBase::ECircle)
    {
    	circle2= static_cast<CLbtGeoCircle*> (geobase); 
    }
    	
    TCoordinate center=circle2->Center();
    
    AssertTrueL( center.Latitude()==5 && center.Longitude()==10 && center.Altitude()==15,_L("Wrong Center"));
    
    AssertTrueL( circle2->Radius()==10,_L("Wrong Radius"));
    
    CleanupStack::PopAndDestroy( 1 );
    
    return KErrNone;
        
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterByArea4L(CStifItemParser& /*aItem*/)
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
    
    
     TInt TTestlbtclasses ::TestCLbtTriggerFilterByArea5L(CStifItemParser& /*aItem*/)
    {
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
    
    CLbtTriggerFilterByArea* filterarea1= CLbtTriggerFilterByArea::NewL( Cellarea );
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
    
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (base);
    }
    
     AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeGsm,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(cell2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(cell2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
    CleanupStack::Pop(filterarea2);
    CleanupStack::Pop(buffer);
    CleanupStack::Pop(filterarea1);
    return KErrNone;
    
        
    }
    
     TInt TTestlbtclasses ::TestCLbtTriggerFilterByArea6L(CStifItemParser& /*aItem*/)
    {
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
   CLbtGeoCell* Cellarea  = CLbtGeoCell::NewL(RMobilePhone :: ENetworkModeWcdma,
   														Networkcountrycode,
   														Networkidentitycode,
   														Locationareacode,
   														CellId
   														);
    
    CLbtTriggerFilterByArea* filterarea1= CLbtTriggerFilterByArea::NewL( Cellarea );
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
    
    CLbtGeoCell* cell2=NULL;
    if(base->Type()==CLbtGeoAreaBase::ECellular)
    {
    	cell2= static_cast<CLbtGeoCell*> (base);
    }
    
     AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type")); 
    AssertTrueL(cell2->NetworkType()==RMobilePhone::ENetworkModeWcdma,_L("Wrong network Type")); 
    AssertTrueL(cell2->NetworkCountryCode()==Networkcountrycode,_L("Wrong Network Country code"));   
		AssertTrueL(cell2->NetworkIdentityCode()==Networkidentitycode,_L("Wrong Network identity code"));            
		AssertTrueL(cell2->LocationAreaCode()==Locationareacode,_L("Wrong Location Area code"));   
		AssertTrueL(cell2->CellId()==CellId,_L("Wrong Cellid"));         
    AssertTrueL(cell2->Type()==CLbtGeoAreaBase::ECellular,_L("Wrong Type"));     
    CleanupStack::Pop(filterarea2);
    CleanupStack::Pop(buffer);
    CleanupStack::Pop(filterarea1);
    return KErrNone;
    
        
    }
     
     TInt TTestlbtclasses ::TestCLbtTriggerFilterByArea7L(CStifItemParser& /*aItem*/)
    {
    CLbtGeoRect* rect = CLbtGeoRect::NewL();
    
    rect->SetBorder(-90,90,-180,180);
    
    CLbtTriggerFilterByArea* filterarea1= CLbtTriggerFilterByArea::NewL( rect );
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
    
    CLbtGeoRect* rect2=NULL;
    if( base->Type()== CLbtGeoAreaBase::ERectangular  )
    {
    rect2= static_cast<CLbtGeoRect*> (base);
    }
        
    TReal aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude ; 
    rect2->GetBorder(aSouthLatitude,aNorthLatitude,aWestLongitude,aEastLongitude);
    
    AssertTrueL( aSouthLatitude==-90 && aNorthLatitude==90 &&
                 aWestLongitude==-180 && aEastLongitude==180,_L("Wrong Border values"));
    
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
        
    }
     
     TInt TTestlbtclasses ::TestCLbtTriggerFilterByArea8L(CStifItemParser& /*aItem*/)
    {
        	    
    CLbtTriggerFilterByArea* filterarea1= CLbtTriggerFilterByArea::NewL( NULL );
    CleanupStack::PushL( filterarea1 );
    
    
    CLbtGeoAreaBase* base=filterarea1->Area();
    
    CleanupStack::PopAndDestroy( 1 );
    
    return KErrNone;
        
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterByAttribute1L(CStifItemParser& /*aItem*/)
    {
 	  CLbtTriggerFilterByAttribute* filter1=CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( filter1 );
    TBool result =filter1->IsTriggerIdInFilter ( 2);
    if(result)
		{
		return -99;
		}
    result =filter1->IsTriggerManagerUiInFilter ( TUid::Uid(5));
    if(result)
		{
		return -99;
		}
    
    result =filter1->IsTriggerValidityStatusInFilter ( TLbtTriggerDynamicInfo::EInvalid);
    if(result)
		{
		return -99;
		}
    
    result =filter1->IsTriggerStateInFilter ( CLbtTriggerEntry::EStateDisabled);
    if(result)
		{
		return -99;
		}
    filter1->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
    filter1->AddTriggerStateL( CLbtTriggerEntry::EStateDisabled );
    filter1->AddTriggerManagerUiL( TUid::Uid(5) );
    filter1->AddTriggerValidityL(TLbtTriggerDynamicInfo::EInvalid) ;
		filter1->AddTriggerIdL (1) ;   
		filter1->AddTriggerIdL (2) ;   
	result =filter1->IsTriggerIdInFilter ( 3); 
	if(result)
		{
		return -99;
		}
	
	result =filter1->IsTriggerManagerUiInFilter (TUid::Uid(7)); 
	if(result)
		{
		return -99;
		}
	
	result =filter1->IsTriggerValidityStatusInFilter ( TLbtTriggerDynamicInfo::EValid);
    if(result)
		{
		return -99;
		}
    
    result =filter1->IsTriggerStateInFilter ( CLbtTriggerEntry::EStateEnabled);
    if(result)
		{
		return -99;
		}
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
    
    result=filter2->IsTriggerTypeInFilter(CLbtTriggerEntry::ETypeSession);
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
    
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterByAttribute2L(CStifItemParser& /*aItem*/)
    {
 	  CLbtTriggerFilterByAttribute* filter1=CLbtTriggerFilterByAttribute::NewLC();
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
    filter2->ResetFilter();
        
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterByAttribute3L(CStifItemParser& /*aItem*/)
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
    
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterComposite1L(CStifItemParser& /*aItem*/)
    {
 	   TCoordinate center(5,10,15);
    CLbtTriggerFilterComposite * composite=CLbtTriggerFilterComposite::NewL(CLbtTriggerFilterComposite::ECompositionTypeAnd);
    CleanupStack::PushL( composite );
   
    CLbtGeoCircle* circle1 = CLbtGeoCircle::NewL(center,5);
        
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(circle1);   
    composite->AddFilterL(areafilter1);
    composite->AddFilterL(areafilter1);
    composite->RemoveFilter(1); 
    TInt count=composite->NumberOfFilters();
    AssertTrueL(count==1,_L("wrong count"));
    if(composite->Type()!=CLbtTriggerFilterBase::EFilterComposite)
    {
    	User::LeaveIfError(KErrGeneral);
    }
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
    
    composite->Reset();    
    CleanupStack::Pop( 1 ); 
    delete composite;
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterComposite2L(CStifItemParser& /*aItem*/)
    {
 	   TCoordinate center(5,10,15);
    CLbtTriggerFilterComposite * composite=CLbtTriggerFilterComposite::NewLC(CLbtTriggerFilterComposite::ECompositionTypeAnd);
   
    CLbtGeoCircle* circle1 = CLbtGeoCircle::NewL(center,5);
        
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(circle1);   
    composite->AddFilterL(areafilter1);
    CLbtTriggerFilterBase::TFilterType filttype= composite->Type() ;
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
    
    composite->Reset();    
    CleanupStack::PopAndDestroy( 1 ); 
    return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterComposite3L(CStifItemParser& /*aItem*/)
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
    
    TInt TTestlbtclasses ::TestCLbtTriggerFilterComposite4L(CStifItemParser& /*aItem*/)
    {
    TCoordinate center(5,10,15);
    CLbtTriggerFilterComposite * composite=CLbtTriggerFilterComposite::NewLC(CLbtTriggerFilterComposite::ECompositionTypeAnd);
   
    CLbtGeoCircle* circle1 = CLbtGeoCircle::NewL(center,5);
        
    CLbtTriggerFilterByArea* areafilter1=CLbtTriggerFilterByArea::NewL(circle1);   
    composite->AddFilterL(areafilter1);
    CLbtTriggerFilterBase::TFilterType filttype= composite->Type() ;
    TInt count=composite->NumberOfFilters();
    AssertTrueL(count==1,_L("wrong count"));
            
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    RBufWriteStream wr;
    wr.Open(*buffer);
    CleanupClosePushL(wr);
    composite->ExternalizeL(wr);
    
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
    
    CLbtTriggerFilterByArea* filter2=NULL;
    if( base->Type()== CLbtTriggerFilterBase::EFilterByArea )
    {
    	filter2= static_cast<CLbtTriggerFilterByArea*> (base);
    } 
    CLbtGeoAreaBase* geobase=filter2->Area();
    
    CLbtGeoCircle* circle2=NULL;
    
    if( geobase->Type()==CLbtGeoAreaBase::ECircle)
    {
    	circle2= static_cast<CLbtGeoCircle*> (geobase); 
    }
    
    AssertTrueL( circle2->Radius()==5,_L("wrong Radius"));
   
    CleanupStack::PopAndDestroy( 3 );
    
    return KErrNone;
    
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerFiringEventNotifierL(CStifItemParser& /*aItem*/)
    {
  	iLog->Log(_L("Entering Test1"));
 
 		EnableSimPSYL();
//    _LIT( KSimulationFile,"c:\\system\\data\\simu_move2.sps" );
	
	 	RLbtServer lbtserver;
	 	RLbt lbt;
	 	iLog->Log(_L("Before connecting"));
	 	User::LeaveIfError( lbtserver.Connect() );
    CleanupClosePushL( lbtserver );
    iLog->Log(_L("Connection to RLbtServer Passed "));
	 	User::LeaveIfError( lbt.Open(lbtserver));
	 	iLog->Log(_L("Subsession opened "));
	 	CleanupClosePushL( lbt );
	 	 
	 	/*CRepository* repository = CRepository::NewLC(KCRUidSimulationPSY);
	 	iLog->Log(_L("Simulation PSY Repository object created"));
	//	 User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile0));
		User::LeaveIfError(repository->Set(KCRKeySimPSYSimulationFile, KSimulationFile));
		iLog->Log(_L("Simulation input file set "));
		CleanupStack::PopAndDestroy(repository);*/
	 
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

    TCoordinate coordinate(62.5285,23.9385);
   //	coordinate.Move(90,2000);
   	CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
         
    	// ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt);
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
    notifier->CreateTriggers( lbt,*trig,trigId,ETrue,wait );
    wait->Start( );
    iLog->Log(_L("Trigger Created"));
    notifier->StartNotification( wait );
    notifier->After(10000000);
  	wait->Start( );
    iLog->Log(_L("Trigger Fired"));
    TLbtTriggerFireInfo FireInfo;
    TReal32 trigDistance;
    TPosition firePosition;
    FireInfo = notifier->GetFiredTrigger();
    FireInfo.iFiredPositionInfo.GetPosition(firePosition);
    firePosition.Distance(coordinate,trigDistance);
   

    CleanupStack::PopAndDestroy( notifier );
    CleanupStack::PopAndDestroy( trig );
    CleanupStack::Pop( &lbt );
    CleanupStack::PopAndDestroy( &lbtserver );
    return KErrNone;
   
    }
 TInt TTestlbtclasses ::TestCLbtTriggerInfo1L(CStifItemParser& /*aItem*/)
 {
 	_LIT( KMyRequestorName, "Requestor" );
	CLbtTriggerInfo* triggerinfo=CLbtTriggerInfo::NewL();
	CleanupStack::PushL( triggerinfo );

	TLbtTriggerDynamicInfo* dyninfo = new(ELeave) TLbtTriggerDynamicInfo;
	dyninfo->iValidity= TLbtTriggerDynamicInfo::EInvalid;

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
  
    CleanupStack::PopAndDestroy( triggerinfo ); 
    
    return KErrNone;                
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerInfo2L(CStifItemParser& /*aItem*/)
    {
 	  _LIT( KMyRequestorName, "Requestor" );
	  CLbtTriggerInfo* triggerinfo=CLbtTriggerInfo::NewLC();
	  
	  TLbtTriggerDynamicInfo* dyninfo = new(ELeave) TLbtTriggerDynamicInfo;
	  dyninfo->iValidity= TLbtTriggerDynamicInfo::EInvalid;

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
  
    CleanupStack::PopAndDestroy( triggerinfo ); 
    
    return KErrNone;                
    }
    
 TInt TTestlbtclasses ::TestCLbtTriggerInfo3L(CStifItemParser& /*aItem*/)
  {
 	_LIT( KMyRequestorName, "Requestor" );


	TLbtTriggerDynamicInfo* dyninfo = new(ELeave) TLbtTriggerDynamicInfo;
	dyninfo->iValidity= TLbtTriggerDynamicInfo::EInvalid;

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
   
    CleanupStack::PopAndDestroy( triggerinfo ); 
    
    return KErrNone;                

    }
    
    TInt TTestlbtclasses ::TestCLbtTriggerInfo4L(CStifItemParser& /*aItem*/)
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
    
    TInt TTestlbtclasses ::TestTLbtTriggeringSystemSettingsL(CStifItemParser& /*aItem*/)
    {
 	  RLbtServer lbtserver;
 	  RLbt lbt;
 	 
 	  User::LeaveIfError( lbtserver.Connect() );
      CleanupClosePushL( lbtserver );
 	  User::LeaveIfError( lbt.Open( lbtserver ) );
 	  CleanupClosePushL( lbt );
 	 
 	  TLbtTriggeringSystemSettings settings;
 	  lbt.GetTriggeringSystemSettingsL( settings );
	  settings.SetMinimumTriggerAreaSize(200);
		
	  settings.SetTriggeringMechanismState(ETriggeringMechanismOff);
		
	  TLbtTriggeringSystemSettings::TType type =settings.Type();
	  if( type != TLbtTriggeringSystemSettings::ETypeSystemSettings )
	  {
	  	User::LeaveIfError(KErrGeneral);
	  }
 	  AssertTrueL( settings.MinimumTriggerAreaSize( )==200,_L(" Wrong Minimum trigger area" ) );
 	  AssertTrueL( settings.TriggeringMechanismState ( )==ETriggeringMechanismOff,_L(" Wrong triggering state" ) );
		settings.SetTriggeringMechanismState(ETriggeringMechanismOn);
 		AssertTrueL( settings.TriggeringMechanismState ( )==ETriggeringMechanismOn,_L(" Wrong triggering state" ) );
 	  CleanupStack::PopAndDestroy( &lbt );
      CleanupStack::PopAndDestroy( &lbtserver );
      return KErrNone;
    }
    
    TInt TTestlbtclasses ::TestCLbtTriggeringSystemSettingsChangeEventNotifier1L(CStifItemParser& /*aItem*/)
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
	    
	    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
	  
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
	    notifier2->After(10000);
	    wait2->Start( );
	    //Set triggering machanism off from managementUI
	   	lbt2.CancelNotifyTriggeringSystemSettingChange ();
	   	notifier1->Cancel();   
	   	lbt1.CancelAll(); 
	    CleanupStack::PopAndDestroy(notifier2);
	   
    	CleanupStack::PopAndDestroy(trig);
    	CleanupStack::PopAndDestroy(notifier1);
	    
	    CleanupStack::PopAndDestroy(&lbt2);
	    CleanupStack::PopAndDestroy(&lbt1);
	    CleanupStack::PopAndDestroy(&server);
	 
	    return KErrNone;
    }
    
    
    
 
