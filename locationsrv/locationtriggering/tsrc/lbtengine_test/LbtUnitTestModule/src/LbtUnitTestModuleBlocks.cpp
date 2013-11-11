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
* Description: This file contains testclass implementation.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include <etelmm.h>
#include <centralrepository.h>
#include "LbtUnitTestModule.h"
#include "lbtgeogsmcell.h"
#include "lbtgeowcdmacell.h"
#include "lbtsettingsmanager.h"
#include "lbtdiskspacelistner.h"
#include "lbttriggernode.h"
#include "lbtcontextsourceplugincommon.h"
#include "lbtcellinfo.h"
#include "lbtoperation.h"
#include "lbtlogger.h"


static const TInt KArrayIndex = 5;
const TInt KCRUidLocationTriggeringSettings(0x1028312F);

const TBuf<236> testFileName(_L("C:\\data\\testdata.log"));

TInt ThreadFunction( TAny* aParam );

enum TCenRepKeys
    {
    ELbtMinimumTriggerArea = 0x00000007,
    ELbtTrackModeInterval = 0x00000008,
    ELbtLocRequestTimeOut = 0x00000009,
    ELbtMinHysteresisArea = 0x0000000A,
    ELbtDaysForCleanup = 0x0000000B,
    ELbtSleepTime  = 0x0000000C,
    ELbtMaxActiveTriggers = 0x0000000D,
    };

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CLbtUnitTestModule::Delete() 
    {
    // Lbt logger is created by the first LOG call, it is destroyed 
    // only during termination. This has to be explicitly destroyed.
    STOPLOGGING
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CLbtUnitTestModule::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "TestCLbtGeoGsmCellNewL", CLbtUnitTestModule::TestCLbtGeoGsmCellNewL ),
        ENTRY( "TestCLbtGeoGsmCellNewLC", CLbtUnitTestModule::TestCLbtGeoGsmCellNewLC ),
        ENTRY( "TestCLbtGeoGsmCellNew1L", CLbtUnitTestModule::TestCLbtGeoGsmCellNew1L ),
        ENTRY( "TestCLbtGeoGsmCellNew1LC", CLbtUnitTestModule::TestCLbtGeoGsmCellNew1LC ),
        ENTRY( "TestSetAndGetBsicL", CLbtUnitTestModule::TestSetAndGetBsicL ),
        ENTRY( "TestGsmCGISpecifiedDuringCreationL", CLbtUnitTestModule::TestGsmCGISpecifiedDuringCreationL ),
        ENTRY( "TestGsmCellInternalizeExternalizeL", CLbtUnitTestModule::TestGsmCellInternalizeExternalizeL ),
        ENTRY( "TestCLbtGeoWcdmaCellL", CLbtUnitTestModule::TestCLbtGeoWcdmaCellL ),
        ENTRY( "TestCLbtGeoWcdmaCellLC", CLbtUnitTestModule::TestCLbtGeoWcdmaCellLC ),
        ENTRY( "TestCLbtGeoWcdmaCell1L", CLbtUnitTestModule::TestCLbtGeoWcdmaCell1L ),
        ENTRY( "TestCLbtGeoWcdmaCell1LC", CLbtUnitTestModule::TestCLbtGeoWcdmaCell1LC ),
        ENTRY( "TestSetAndGetWcdmaInfoL", CLbtUnitTestModule::TestSetAndGetWcdmaInfoL ),
        ENTRY( "TestWcdmaCGISpecifiedDuringCreationL", CLbtUnitTestModule::TestWcdmaCGISpecifiedDuringCreationL ),
        ENTRY( "TestWcdmaCellInternalizeExternalizeL", CLbtUnitTestModule::TestWcdmaCellInternalizeExternalizeL ),
        ENTRY( "TestSetAndGetOptimisationLevelL", CLbtUnitTestModule::TestSetAndGetOptimisationLevelL ),
        ENTRY( "TestSetAndGetNppModuleIdL", CLbtUnitTestModule::TestSetAndGetNppModuleIdL ),
        ENTRY( "TestSetAndGetMinUpdateIntOnGpsFailureL", CLbtUnitTestModule::TestSetAndGetMinUpdateIntOnGpsFailureL ),
        ENTRY( "TestSetAndGetMaximumUserSpeedL", CLbtUnitTestModule::TestSetAndGetMaximumUserSpeedL ),
        ENTRY( "TestSetAndGetTrigMechStateL", CLbtUnitTestModule::TestSetAndGetTrigMechStateL ),
        ENTRY( "TestTriggerConfigParamsL", CLbtUnitTestModule::TestTriggerConfigParamsL ),
        ENTRY( "TestCLbtDiskSpaceListnerStartToListenL", CLbtUnitTestModule::TestCLbtDiskSpaceListnerStartToListenL ),
        ENTRY( "TestCLbtDiskSpaceListnerCancelL", CLbtUnitTestModule::TestCLbtDiskSpaceListnerCancelL ),
        ENTRY( "TestTriggerNodeMethodsL", CLbtUnitTestModule::TestTriggerNodeMethodsL ),
        ENTRY( "TestGsmCellInfoMethodsL", CLbtUnitTestModule::TestGsmCellInfoMethodsL ),
        ENTRY( "TestWcdmaCellInfoMethodsL", CLbtUnitTestModule::TestWcdmaCellInfoMethodsL ),
        ENTRY( "TestLbtOperationsMethodL", CLbtUnitTestModule::TestLbtOperationsMethodL )
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestCLbtGeoGsmCellNewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestCLbtGeoGsmCellNewL( CStifItemParser& /*aItem*/ )
    {
    CLbtGeoGsmCell* gsmCell = CLbtGeoGsmCell::NewL();
    delete gsmCell;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestCLbtGeoGsmCellNewLC
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestCLbtGeoGsmCellNewLC( CStifItemParser& /*aItem*/ )
    {
    CLbtGeoGsmCell* gsmCell = CLbtGeoGsmCell::NewLC();
    CleanupStack::PopAndDestroy( gsmCell );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestCLbtGeoGsmCellNew1L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestCLbtGeoGsmCellNew1L( CStifItemParser& /*aItem*/ )
    {
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    CLbtGeoGsmCell* gsmCell = CLbtGeoGsmCell::NewL( 
                                   RMobilePhone::ENetworkModeGsm,
                                   Networkcountrycode,
                                   Networkidentitycode,
                                   Locationareacode,
                                   CellId);
    delete gsmCell;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestCLbtGeoGsmCellNew1LC
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestCLbtGeoGsmCellNew1LC( CStifItemParser& /*aItem*/ )
    {
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    CLbtGeoGsmCell* gsmCell = CLbtGeoGsmCell::NewLC( 
                                    RMobilePhone::ENetworkModeGsm,
                                    Networkcountrycode,
                                    Networkidentitycode,
                                    Locationareacode,
                                    CellId);
    CleanupStack::PopAndDestroy( gsmCell );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestSetAndGetBsicL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestSetAndGetBsicL( CStifItemParser& /*aItem*/ )
    {
    CLbtGeoGsmCell* gsmCell = CLbtGeoGsmCell::NewLC();
    RArray<TInt> bsicArray;
    CleanupClosePushL( bsicArray );
    for( TInt i=0;i<KArrayIndex;i++ )
        {
        bsicArray.AppendL( i+1 );
        }
    gsmCell->SetBsic( bsicArray );
    
    RArray<TInt> retBsicArray = gsmCell->Bsic();
    for( TInt i=0;i<bsicArray.Count();i++ )
        {
        if( bsicArray[i] != retBsicArray[i] )
            {
            User::Leave( KErrArgument );
            }
        }
    CleanupStack::PopAndDestroy( 2 );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestGsmCGISpecifiedDuringCreationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestGsmCGISpecifiedDuringCreationL( CStifItemParser& /*aItem*/ )
    {
    CLbtGeoGsmCell* gsmCell = CLbtGeoGsmCell::NewL();
    CleanupStack::PushL( gsmCell );
    TBool cgiSpecified = gsmCell->CGISpecifiedDuringCreation();
    if( cgiSpecified )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( gsmCell );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestGsmCellInternalizeExternalizeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestGsmCellInternalizeExternalizeL( CStifItemParser& /*aItem*/ )
    {
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoGsmCell* gsmCell = CLbtGeoGsmCell::NewL( 
                                    RMobilePhone::ENetworkModeGsm,
                                    Networkcountrycode,
                                    Networkidentitycode,
                                    Locationareacode,
                                    CellId);
    CleanupStack::PushL( gsmCell );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    // Externalize
    RBufWriteStream wr;
    CleanupClosePushL( wr );
    wr.Open(*buffer);
    gsmCell->ExternalizeL( wr );
    wr.CommitL();
    CleanupStack::PopAndDestroy( &wr );
    
    // Internalize
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    CLbtGeoGsmCell* newGsmCell = CLbtGeoGsmCell::NewLC();
    newGsmCell->InternalizeL(rd);
    
    if( newGsmCell->NetworkType() != RMobilePhone::ENetworkModeGsm ||
        newGsmCell->NetworkCountryCode() != Networkcountrycode ||
        newGsmCell->NetworkIdentityCode() != Networkidentitycode ||
        newGsmCell->LocationAreaCode() != Locationareacode ||
        newGsmCell->CellId() != CellId )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( 4 );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestCLbtGeoWcdmaCellL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestCLbtGeoWcdmaCellL( CStifItemParser& /*aItem*/ )
    {
    CLbtGeoWcdmaCell* wcdmaCell = CLbtGeoWcdmaCell::NewL();
    delete wcdmaCell;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestCLbtGeoWcdmaCellLC
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestCLbtGeoWcdmaCellLC( CStifItemParser& /*aItem*/ )
    {
    CLbtGeoWcdmaCell* wcdmaCell = CLbtGeoWcdmaCell::NewLC();
    CleanupStack::PopAndDestroy( wcdmaCell );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestCLbtGeoWcdmaCell1L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestCLbtGeoWcdmaCell1L( CStifItemParser& /*aItem*/ )
    {
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    CLbtGeoWcdmaCell* wcdmaCell = CLbtGeoWcdmaCell::NewL( 
                                        RMobilePhone::ENetworkModeWcdma,
                                        Networkcountrycode,
                                        Networkidentitycode,
                                        Locationareacode,
                                        CellId);
    delete wcdmaCell;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestCLbtGeoWcdmaCell1LC
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestCLbtGeoWcdmaCell1LC( CStifItemParser& /*aItem*/ )
    {
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoWcdmaCell* wcdmaCell = CLbtGeoWcdmaCell::NewLC(
                                        RMobilePhone::ENetworkModeWcdma,
                                        Networkcountrycode,
                                        Networkidentitycode,
                                        Locationareacode,
                                        CellId );
    CleanupStack::PopAndDestroy( wcdmaCell );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestSetAndGetWcdmaInfoL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestSetAndGetWcdmaInfoL( CStifItemParser& /*aItem*/ )
    {
    CLbtGeoWcdmaCell* wcdmaCell = CLbtGeoWcdmaCell::NewLC();
    
    TLbtWcdmaCellInfo wcdmaCellInfo;
    wcdmaCellInfo.iCell.iCarrierRssi = 10;
    wcdmaCellInfo.iCell.iEcNo = 20;
    wcdmaCellInfo.iCell.iPathLoss = 30;
    wcdmaCellInfo.iCell.iRscp = 40;
    wcdmaCell->SetWcdmaCellInfo( wcdmaCellInfo );
    
    TLbtWcdmaCellInfo retWcdmaCellInfo = wcdmaCell->WcdmaCellInfo();
    
    if( wcdmaCellInfo.iCell.iCarrierRssi != retWcdmaCellInfo.iCell.iCarrierRssi ||
        wcdmaCellInfo.iCell.iEcNo != retWcdmaCellInfo.iCell.iEcNo ||
        wcdmaCellInfo.iCell.iPathLoss != retWcdmaCellInfo.iCell.iPathLoss ||
        wcdmaCellInfo.iCell.iRscp != retWcdmaCellInfo.iCell.iRscp )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( wcdmaCell );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestWcdmaCGISpecifiedDuringCreationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestWcdmaCGISpecifiedDuringCreationL( CStifItemParser& /*aItem*/ )
    {
    CLbtGeoWcdmaCell* wcdmaCell = CLbtGeoWcdmaCell::NewL();
    CleanupStack::PushL( wcdmaCell );
    TBool cgiSpecified = wcdmaCell->CGISpecifiedDuringCreation();
    if( cgiSpecified )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( wcdmaCell );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestWcdmaCellInternalizeExternalizeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestWcdmaCellInternalizeExternalizeL( CStifItemParser& /*aItem*/ )
    {
    TBuf<4> Networkcountrycode = _L("1000");
    TBuf<8> Networkidentitycode = _L("10000000");
    TUint Locationareacode = 100;
    TUint CellId = 100;
    
    CLbtGeoWcdmaCell* wcdmaCell = CLbtGeoWcdmaCell::NewL( 
                                    RMobilePhone::ENetworkModeWcdma,
                                    Networkcountrycode,
                                    Networkidentitycode,
                                    Locationareacode,
                                    CellId);
    CleanupStack::PushL( wcdmaCell );
    
    CBufFlat* buffer= CBufFlat::NewL(256);
    CleanupStack::PushL( buffer );  
    
    // Externalize
    RBufWriteStream wr;
    CleanupClosePushL( wr );
    wr.Open(*buffer);
    wcdmaCell->ExternalizeL( wr );
    wr.CommitL();
    CleanupStack::PopAndDestroy( &wr );
    
    // Internalize
    
    RBufReadStream rd;
    rd.Open(*buffer);
    CleanupClosePushL(rd);
    
    CLbtGeoWcdmaCell* newWcdmaCell = CLbtGeoWcdmaCell::NewLC();
    newWcdmaCell->InternalizeL(rd);
    
    if( newWcdmaCell->NetworkType() != RMobilePhone::ENetworkModeWcdma ||
        newWcdmaCell->NetworkCountryCode() != Networkcountrycode ||
        newWcdmaCell->NetworkIdentityCode() != Networkidentitycode ||
        newWcdmaCell->LocationAreaCode() != Locationareacode ||
        newWcdmaCell->CellId() != CellId )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( 4 );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestSetAndGetOptimisationLevelL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestSetAndGetOptimisationLevelL( CStifItemParser& /*aItem*/ )
    {
    CLbtSettingsManager* settingsManager = CLbtSettingsManager::NewL();
    CleanupStack::PushL( settingsManager );
    settingsManager->SetOptimizationLevel( 10 );
    if( settingsManager->GetOptimizationLevel() != 10 )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( settingsManager );
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestSetAndGetModuleIdL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestSetAndGetModuleIdL( CStifItemParser& /*aItem*/ )
    {
    CLbtSettingsManager* settingsManager = CLbtSettingsManager::NewL();
    CleanupStack::PushL( settingsManager );
    TUid moduleId = TUid::Uid( 270526860 );
    settingsManager->SetModuleId( moduleId );
    if( settingsManager->GetModuleId() != moduleId )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( settingsManager );
    return KErrNone;    
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestSetAndGetNppModuleIdL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestSetAndGetNppModuleIdL( CStifItemParser& /*aItem*/ )
    {
    CLbtSettingsManager* settingsManager = CLbtSettingsManager::NewL();
    CleanupStack::PushL( settingsManager );
    TUid moduleId = TUid::Uid( 16777216 );
    settingsManager->SetModuleId( moduleId );
    if( settingsManager->GetModuleId() != moduleId )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( settingsManager );
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestSetAndGetMinUpdateIntOnGpsFailureL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestSetAndGetMinUpdateIntOnGpsFailureL( CStifItemParser& /*aItem*/ )
    {
    CLbtSettingsManager* settingsManager = CLbtSettingsManager::NewL();
    CleanupStack::PushL( settingsManager );
    
    settingsManager->SetMinimumUpdateIntervalOnGpsFailure( 10000000 );
    if( settingsManager->GetMinimumUpdateIntervalOnGpsFailure() != 10000000 )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( settingsManager );
    return KErrNone;    
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestSetAndGetMaximumUserSpeedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestSetAndGetMaximumUserSpeedL( CStifItemParser& /*aItem*/ )
    {
    CLbtSettingsManager* settingsManager = CLbtSettingsManager::NewL();
    CleanupStack::PushL( settingsManager );
    
    settingsManager->SetMaximumUserSpeed( 42 );
    if( settingsManager->GetMaximumUserSpeed() != 42 )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( settingsManager );
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestSetAndGetTrigMechStateL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestSetAndGetTrigMechStateL( CStifItemParser& /*aItem*/ )
    {
    CLbtSettingsManager* settingsManager = CLbtSettingsManager::NewL();
    CleanupStack::PushL( settingsManager );
    
    settingsManager->SetTriggeringMechanismState( ETriggeringMechanismOn );
    if( settingsManager->GetTriggeringMechanismState() != ETriggeringMechanismOn )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( settingsManager );
    return KErrNone;    
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestTriggerConfigParamsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestTriggerConfigParamsL( CStifItemParser& /*aItem*/ )
    {
    // Read values from cenrep.
    
    CRepository* repository = CRepository::NewL(TUid::Uid(KCRUidLocationTriggeringSettings));
    CleanupStack::PushL( repository );
    // Get all the values and store it in RAM
    TInt minTriggerArea;
    repository->Get(ELbtMinimumTriggerArea, minTriggerArea);
    TInt trackModeInt;
    repository->Get(ELbtTrackModeInterval, trackModeInt);
    TInt locRequestTimeOut;
    repository->Get(ELbtLocRequestTimeOut, locRequestTimeOut);
    TInt minHysteresisArea;
    repository->Get(ELbtMinHysteresisArea, minHysteresisArea);
    TInt daysForCleanup;
    repository->Get(ELbtDaysForCleanup, daysForCleanup);
    TInt enabledValidTriggerCount;
    repository->Get(ELbtMaxActiveTriggers, enabledValidTriggerCount);
    CLbtSettingsManager* settingsManager = CLbtSettingsManager::NewL();
    CleanupStack::PushL( settingsManager );
    
    if( settingsManager->MinimumTriggeringArea() != minTriggerArea )
        {
        User::Leave( KErrArgument );
        }
    
    if( settingsManager->TrackingModeInterval() != trackModeInt )
        {
        User::Leave( KErrArgument );
        }
    
    if( settingsManager->GetLocationRequestTimeOut() != locRequestTimeOut )
        {
        User::Leave( KErrArgument );
        }
    
    if( settingsManager->MinimumHysteresisArea() != minHysteresisArea )
        {
        User::Leave( KErrArgument );
        }
    
    if( settingsManager->NumberOfDaysForCleanup() != daysForCleanup )
        {
        User::Leave( KErrArgument );
        }
    
    if( settingsManager->EnabledValidTriggerCount() != enabledValidTriggerCount )
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( 2 );
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestCLbtDiskSpaceListnerStartToListenL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestCLbtDiskSpaceListnerStartToListenL( CStifItemParser& /*aItem*/ )
    {
    CLbtDiskSpaceListner* diskSpaceListner = CLbtDiskSpaceListner::NewL( *this );
    CleanupStack::PushL( diskSpaceListner );
    
    diskSpaceListner->StartToListen();
    
    RThread thread;
    TThreadFunction  funcPtr = ThreadFunction;
    thread.Create(_L("TestLbtThread"),funcPtr,KDefaultStackSize,NULL,NULL);
    thread.Resume();
    
    iWait->Start();
    thread.Kill( 0 );
    thread.Close();
    CleanupStack::PopAndDestroy( diskSpaceListner );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestCLbtDiskSpaceListnerCancelL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestCLbtDiskSpaceListnerCancelL( CStifItemParser& /*aItem*/ )
    {
    CLbtDiskSpaceListner* diskSpaceListner = CLbtDiskSpaceListner::NewL( *this );
    CleanupStack::PushL( diskSpaceListner );
    
    diskSpaceListner->StartToListen();
    
    // Cancel is called in the destructor.
    CleanupStack::PopAndDestroy( diskSpaceListner );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestTriggerNodeMethodsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestTriggerNodeMethodsL( CStifItemParser& /*aItem*/ )
    {
    TTriggerNode triggerNode;
    
    HBufC* buffer = HBufC::NewLC( 10 );
    TPtr ptr =buffer->Des();
    TPtrC retPtr;
    TReal value;
    
    // Test set and get trigger name.
    ptr.Copy(_L("Test")); 
    triggerNode.SetTriggerNameL( ptr );
    triggerNode.GetTriggerName( retPtr );
    if( ptr != retPtr )
        {
        User::Leave( KErrArgument );
        }
    
    // Test set and get latitude.
    ptr.Copy(_L("12.7"));
    triggerNode.SetLatitudeL( ptr );
    triggerNode.GetLatitude( value );
    if( value != 12.7 )
        {
        User::Leave( KErrArgument );
        }
    
    // Test set and get longitude.
    ptr.Copy(_L("72.7"));
    triggerNode.SetLongitudeL( ptr );
    triggerNode.GetLongitude( value );
    if( value != 72.7 )
        {
        User::Leave( KErrArgument );
        }
    
    
    // Test set and get region.
    ptr.Copy(KRegionValue); 
    triggerNode.SetRegionL( ptr );
    triggerNode.GetRegion( retPtr );
    if( ptr != retPtr )
        {
        User::Leave( KErrArgument );
        }
    
    // Test set and get radius.
    ptr.Copy(_L("10"));
    triggerNode.SetRadiusL( ptr );
    triggerNode.GetRadius( value );
    if( value != KMinRadius )
        {
        User::Leave( KErrArgument );
        }
    
    
    // Test set and get direction.
    ptr.Copy(KNullDesC); 
    triggerNode.SetDirectionL( ptr );
    if( triggerNode.GetDirection() != CLbtTriggerConditionArea::EFireOnEnter )
        {
        User::Leave( KErrArgument );
        }
    
    // Test set and get context source.
    ptr.Copy(KDefaultContextSourceName); 
    triggerNode.SetContextSource( ptr );
    if( triggerNode.GetContextSource() != KDefaultContextSourceName )
        {
        User::Leave( KErrArgument );
        }
    
    // Test set and get current state.
    triggerNode.SetCurrentStateL( ptr );
    if( triggerNode.GetCurrentState() != CLbtTriggerEntry::EStateEnabled )
        {
        User::Leave( KErrArgument );
        }
    
    CleanupStack::PopAndDestroy( buffer );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestGsmCellInfoMethodsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestGsmCellInfoMethodsL( CStifItemParser& /*aItem*/ )
    {
    // Test GSM cell info methods.
    RMmCustomAPI::TMmCellInfo info;
    
    //Construct dummy cell info with GSM params.
    info.iMode = RMmCustomAPI::TMmCellInfo::EGSM;
    
    info.iGsmCellInfo.iTA = 10;
    
    CLbtCellInfo* cellInfo = CLbtCellInfo::NewL( info );
    CleanupStack::PushL( cellInfo );
    
    // Get the GSM NMR array. 
    RArray<TGsmNMR> gsmArray = cellInfo->GetNMR();
    if( gsmArray.Count() != 0 )
        {
        User::Leave( KErrArgument );
        }
    
    // Check the count of cell info.
    if( cellInfo->Count() != 0 )
        {
        User::Leave( KErrArgument );
        }

    // Read the TA from the cell info.
    if( cellInfo->GetTA() != 10 )
        {
        User::Leave( KErrArgument );
        }
    //Get the cell id count for GSM network. It is not 
    // supported for GSM. Hence it should return 0.
    if( cellInfo->CellChangeCount() != 0 )
        {
        User::Leave( KErrArgument );
        }
    
    // Attempt to find GSM cell info at 0th index should fail.
    TGsmNMR gsmNmr;
    TInt index = 0;
    if( cellInfo->Find( index, gsmNmr ) )
        {
        User::Leave( KErrArgument );
        }
    
    // Add same cell info again
    cellInfo->AddNMRData( info );
    
    // Start the iterator
    CLbtCellInfo::TCellInfoArrayIterator itr = cellInfo->Begin();
    
    if( itr.Next( gsmNmr ) )
        {
        User::Leave( KErrArgument );
        }
    itr.Reset();
    
    // Reset the info
    cellInfo->Reset();
    
    CleanupStack::PopAndDestroy( cellInfo );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestWcdmaCellInfoMethodsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestWcdmaCellInfoMethodsL( CStifItemParser& /*aItem*/ )
    {
    // Test WCDMA cell info methods.
    RMmCustomAPI::TMmCellInfo info;
    
    //Construct dummy cell info with WCDMA params.
    info.iMode = RMmCustomAPI::TMmCellInfo::EWCDMA;
    
    //Set scrambling code and cell id.
    info.iWcdmaCellInfo.iPrimaryScrambilingCode = 10;
    info.iWcdmaCellInfo.iNwkMeasureReport[0].iCellMeasuredResult[0].iCID = 100;
    
    CLbtCellInfo* cellInfo = CLbtCellInfo::NewL( info );
    CleanupStack::PushL( cellInfo );
    
    // Get the WCDMA NMR array. 
    RArray<TWcdmaNMR> wcdmaArray = cellInfo->GetWcdmaNMR();
    if( wcdmaArray.Count() == 0 )
        {
        User::Leave( KErrArgument );
        }
    
    // Check the count of cell info.
    if( cellInfo->Count() == 0 )
        {
        User::Leave( KErrArgument );
        }

    //Get the cell id count for WCDMA network. It is not 
    // supported for GSM. Hence it should return 0.
    if( cellInfo->CellChangeCount() == 0 )
        {
        User::Leave( KErrArgument );
        }
    

    // Add same cell info again
    cellInfo->AddNMRData( info );
    
    // Start the iterator
    CLbtCellInfo::TCellInfoArrayIterator itr = cellInfo->Begin();
    
    TWcdmaNMR wcdmaNmr;
    if( !itr.Next( wcdmaNmr ) )
        {
        User::Leave( KErrArgument );
        }
    itr.Reset();
    
    // Reset the info
    cellInfo->Reset();
    
    CleanupStack::PopAndDestroy( cellInfo );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::TestLbtOperationsMethodL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CLbtUnitTestModule::TestLbtOperationsMethodL( CStifItemParser& /*aItem*/ )
    {
    CLbtGeoAreaBase* areaBase = NULL;
    TRequestStatus status;
    
    CLBTOperation* lbtOperation = CLBTOperation::CreateGeoAreaForCurrentLocationL( 
                                    status,
                                    CLbtGeoAreaBase::ECellular,
                                    &areaBase );
    delete lbtOperation;
    delete areaBase;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLbtUnitTestModule::HandleDiskSpaceChange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CLbtUnitTestModule::HandleDiskSpaceChange()
    {
    iWait->AsyncStop();
    }


// -----------------------------------------------------------------------------
// ThreadFunction
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt ThreadFunction( TAny* /*aParam*/)
    {
    TInt error;
    RFs fs;
    fs.Connect();
    RFile file;
    error = file.Create( fs,testFileName,EFileWrite);
    if(error == KErrAlreadyExists )
        {
        fs.Delete( testFileName );
        }   
    file.Close();
    fs.Close();
    return KErrNone;
    }



//  [End of File] 
