/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MT_SSSettings
*
*/


//  CLASS HEADER
#include "MT_SSSettings.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <CSSSettingsRefreshContainer.h>
#include <rcustomerserviceprofilecache.h>
#include <rsssettings.h>
#include <mmtsy_names.h>

//  INTERNAL INCLUDES
#define KSSSettingsTSYName KMmTsyModuleName
// CONSTANTS
const TInt KCPETestWaitTime = 2000000; // Two second 
const TInt KSSSettingsMaxETelRetryCount(7);
const TInt KSSSettingsETelRetryCountTimeout(100000); 
    
//////////////////////////////////////////////////////////////////////////////////
////	MT_SSSettings CLASS
///////////////////////////////////////////////////////////////////////////////////
//  METHODS
// CONSTRUCTION
MT_SSSettings* MT_SSSettings::NewL()
    {
    MT_SSSettings* self = MT_SSSettings::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_SSSettings* MT_SSSettings::NewLC()
    {
    MT_SSSettings* self = new( ELeave ) MT_SSSettings();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_SSSettings::~MT_SSSettings()
    {
    }

// Default constructor
MT_SSSettings::MT_SSSettings()
    {
    }

// Second phase construct
void MT_SSSettings::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    
    iSettings = new(ELeave) RSSSettings;
    }
///////////////////////////////////////////////////////////////////////////////////
////	MSSSettingsObserver - PhoneSettingChanged
///////////////////////////////////////////////////////////////////////////////////
void MT_SSSettings::PhoneSettingChanged( TSSSettingsSetting /*aSetting*/, 
            TInt /*aNewValue*/ )
    {
    // Do nothing.
    }

///////////////////////////////////////////////////////////////////////////////////
////	EmptySetupL
///////////////////////////////////////////////////////////////////////////////////
void MT_SSSettings::EmptySetupL(  )
    {
    iCugValue = NULL;
    iClirValue = NULL;
    iAlsValue = NULL;
    iAlsBlockValue = NULL;
    iCugDefValue = NULL;
    iSettings = new(ELeave) RSSSettings;
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	EmptyTeardown
///////////////////////////////////////////////////////////////////////////////////    
void MT_SSSettings::EmptyTeardown(  )
    {
    delete iSettings;
    iSettings = NULL;
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	SetupL
///////////////////////////////////////////////////////////////////////////////////
void MT_SSSettings::SetupL(  )
    {
    iCugValue = NULL;
    iClirValue = NULL;
    if ( iAlsValue < NULL )
        {
        iAlsValue = NULL;
        }
    iAlsValue = NULL;
    iAlsBlockValue = NULL;
    iCugDefValue = NULL;
    iSettings = new(ELeave) RSSSettings;
    User::LeaveIfError(iSettings->Open());
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	Teardown
///////////////////////////////////////////////////////////////////////////////////
void MT_SSSettings::Teardown(  )
    {
    iSettings->Close();
    delete iSettings;
    iSettings = NULL;
    }

///////////////////////////////////////////////////////////////////////////////////
////	CSP SetupL
///////////////////////////////////////////////////////////////////////////////////
void MT_SSSettings::CSPSetupL(  )
    {
    iCugValue = NULL;
    iClirValue = NULL;
    iAlsValue = NULL;
    iAlsBlockValue = NULL;
    iCugDefValue = NULL;   
    
    iCSP = new (ELeave) RCustomerServiceProfileCache;
    User::LeaveIfError(iCSP->Open());
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	CSP Teardown
///////////////////////////////////////////////////////////////////////////////////
void MT_SSSettings::CSPTeardown(  )
    {
    delete iCSP;
    iCSP = NULL;
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_OpenCloseL
///////////////////////////////////////////////////////////////////////////////////
void MT_SSSettings::MT_RSSSettings_OpenCloseL(  )
    {
    // Open RSSSettings connection.
    iSettings->Open();
    // Close RSSSettings connection.
    iSettings->Close();
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_Open_1L
///////////////////////////////////////////////////////////////////////////////////
void MT_SSSettings::MT_RSSSettings_Open_1L(  )
    {
    RTelServer*  telServer = new ( ELeave ) RTelServer();
    // Retry ETel connection:
    TInt err(KErrNone);
    for ( TInt i = 0 ; i < KSSSettingsMaxETelRetryCount ; i++ )
        {
        err = telServer->Connect();
        if ( err == KErrNone )
            {
            break;
            }
        User::After( KSSSettingsETelRetryCountTimeout );
        }
   User::LeaveIfError( err );

    // Open phones
    TInt error = telServer->LoadPhoneModule( KSSSettingsTSYName );
    if (( error != KErrNone ) && ( error != KErrAlreadyExists ))
        {
        User::Leave( error );
        }
    // Open RSSSettings connection.
    User::LeaveIfError(iSettings->Open(telServer));
    // Close RSSSettings connection.
    iSettings->Close();
    
    // close ETel session
    if ( telServer )
        {
        if ( telServer->Handle() )
            {
            telServer->Close();
            }
        delete telServer;
        telServer = NULL;
        }
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_RegisterL
///////////////////////////////////////////////////////////////////////////////////    
void MT_SSSettings::MT_RSSSettings_RegisterL(  )
    {
    User::LeaveIfError(iSettings->Register( ESSSettingsCug, *this ));
    iSettings->Cancel( ESSSettingsCug, *this );
    
    User::LeaveIfError(iSettings->Register( ESSSettingsClir, *this ));
    iSettings->Cancel( ESSSettingsClir, *this );
    
    User::LeaveIfError(iSettings->Register( ESSSettingsAls, *this ));
    iSettings->Cancel( ESSSettingsAls, *this );
    
    User::LeaveIfError(iSettings->Register( ESSSettingsAlsBlocking, *this ));
    iSettings->Cancel( ESSSettingsAlsBlocking, *this );
    
    User::LeaveIfError(iSettings->Register( ESSSettingsDefaultCug, *this ));
    iSettings->Cancel( ESSSettingsDefaultCug, *this );
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_RegisterAllL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RSSSettings_RegisterAllL(  )
    {
    User::LeaveIfError(iSettings->RegisterAll( *this ));
    iSettings->CancelAll( *this );
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_CancelAllL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RSSSettings_CancelAllL(  )
    {
    iSettings->CancelAll( *this );
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_CancelL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RSSSettings_CancelL( )
    {
    User::LeaveIfError(iSettings->Register( ESSSettingsCug, *this ));
    iSettings->Cancel( ESSSettingsCug, *this );
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_GetL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RSSSettings_GetL(  )
    {
    User::LeaveIfError(iSettings->Get( ESSSettingsCug, iCugValue ));
    User::LeaveIfError(iSettings->Get( ESSSettingsClir, iClirValue ));
    User::LeaveIfError(iSettings->Get( ESSSettingsAls, iAlsValue ));
    User::LeaveIfError(iSettings->Get( ESSSettingsAlsBlocking, iAlsBlockValue ));
    User::LeaveIfError(iSettings->Get( ESSSettingsDefaultCug, iCugDefValue ));
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_SetL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RSSSettings_SetL(  )
    {
    TInt cug(32767);
    TInt defCug(0);
    
    // Set cug value.
    User::LeaveIfError(iSettings->Set( ESSSettingsCug, cug ));
    
    // Set clir value.
    if ( iClirValue == ESSSettingsClirExplicitInvoke )
        {
        User::LeaveIfError(iSettings->Set( ESSSettingsClir, ESSSettingsClirExplicitSuppress ));
        }
    else
        {
        User::LeaveIfError(iSettings->Set( ESSSettingsClir, ESSSettingsClirExplicitInvoke ));
        }
    // Set als value.
    // If timer is not used set als param will cause request count warning or fail 
    // because SSSettings does several ASync request during als set operation.
    if ( iAlsValue == ESSSettingsAlsPrimary )
        {
        User::LeaveIfError(iSettings->Set( ESSSettingsAls, ESSSettingsAlsAlternate ));
        WaitTimer( KCPETestWaitTime );
        }
    else
        {
        iSettings->Set( ESSSettingsAls, ESSSettingsAlsPrimary );
        WaitTimer( KCPETestWaitTime );
        }
    // Set als blcking value.
    if ( iAlsBlockValue == ESSSettingsAlsBlockingOff )
        {
        User::LeaveIfError(iSettings->Set( ESSSettingsAlsBlocking, ESSSettingsAlsBlockingOn ));    
        }
    else
        {
        User::LeaveIfError(iSettings->Set( ESSSettingsAlsBlocking, ESSSettingsAlsBlockingOff )); 
        }
    
    // Set default cug value.
    iSettings->Set( ESSSettingsDefaultCug, defCug ); // returns kerrnotsupported
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_WaitTimer
////    Start CActiveSchedulerWait, this wait last until Stop is called. This allows 
////    other parts of software to run. Expires after given delay.
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::WaitTimer( TInt aWaitTime )
    {
    CPeriodic* iTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    iTimer->Start( aWaitTime, aWaitTime, TCallBack( DoCallBackL, this ) );
    iWaitTimer.Start();

    delete iTimer;
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_DoCallBackL
/////////////////////////////////////////////////////////////////////////////////// 
TInt MT_SSSettings::DoCallBackL( TAny* aAny )
    {
    // can't call iWaitAny.AsyncStop() in static function
    REINTERPRET_CAST( MT_SSSettings*, aAny )->Stop();
    return KErrNone;
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_Stop
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::Stop()
    {
    iWaitTimer.AsyncStop();  
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_HandleSIMChangedL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RSSSettings_HandleSIMChangedL(  )
    {
    User::LeaveIfError(iSettings->HandleSIMChanged());
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_PreviousCugValueL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RSSSettings_PreviousCugValueL(  )
    {
    TInt previousValue(0);
    User::LeaveIfError(iSettings->PreviousCugValue(previousValue));
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_ResetPreviousCugValueL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RSSSettings_ResetPreviousCugValueL(  )
    {
    User::LeaveIfError(iSettings->ResetPreviousCugValue());    
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_HandleRefreshL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RSSSettings_HandleRefreshL(  )
    {
    // Emulator returns -6/KErrArgument.
    TInt error = iSettings->HandleRefresh();
    if ( error != KErrNone && error != KErrArgument )
        {
        User::Leave(KErrGeneral);
        }
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RSSSettings_IsValueValidCugIndexL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RSSSettings_IsValueValidCugIndexL(  )
    {
    TBool cugValue(0);
    TInt notCugValue(32768);
    TInt corCugValue(32767);
    cugValue = iSettings->IsValueValidCugIndex( notCugValue );
    if ( cugValue )
        {
        User::Leave(KErrGeneral);
        }
        
    cugValue = iSettings->IsValueValidCugIndex( corCugValue );
    if ( !cugValue )
        {
        User::Leave(KErrGeneral);
        }
    }

// From MSSSettingsRefreshHandler.
void MT_SSSettings::NotifyFileChangeL( 
            MSSSettingsRefreshObserver& /*aObserver*/,
            TSatElementaryFiles /*aObservedFile*/,
            TSatRefreshType /*aObservedRefreshType*/)
    {
    }
            
// From MSSSettingsRefreshHandler.
void  MT_SSSettings::CancelNotify()
    {
    // Do nothing
    }
    
// From MSSSettingsRefreshHandler.
TInt  MT_SSSettings::ChangedCspTable( TDes8& /*aChangedCsps*/,
            TDes8& /*aNewValues*/ )
    {
    return KErrNone;
    // Do nothing
    }
    
// From   MSSSettingsRefreshObserver.
TBool MT_SSSettings::AllowRefresh( const TSatRefreshType /*aType*/,
            const TSatElementaryFiles /*aFiles*/ )
    {
    return KErrNone;
    // Do nothing
    }
            
// From   MSSSettingsRefreshObserver.
void  MT_SSSettings::Refresh( const TSatRefreshType /*aType*/,
            const TSatElementaryFiles /*aFiles*/ )
    {
    // Do nothing
    }
///////////////////////////////////////////////////////////////////////////////////
////	MT_CSSSettingsRefreshContainer_NewLL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_CSSSettingsRefreshContainer_NewLL()
    {
    CSSSettingsRefreshContainer* refreshContainer = CSSSettingsRefreshContainer::NewL();
    MSSSettingsRefreshHandler* refreshHandler = &refreshContainer->RefreshHandler();
    
    //NotifyFileChangeL( *this, KCspEf, EFileChangeNotification );
    refreshHandler->NotifyFileChangeL( *this, KCspEf, EFileChangeNotification );
    
    refreshHandler = NULL;
    delete refreshContainer;
    refreshContainer = NULL;
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RCustomerServiceProfileCache_CspCallCompletionL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RCustomerServiceProfileCache_CspCallCompletionL()
    {
    RMobilePhone::TCspCallCompletion params;    
    TInt retVal = iCSP->CspCallCompletion(params);
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RCustomerServiceProfileCache_CspCallOfferingL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RCustomerServiceProfileCache_CspCallOfferingL()
    {
    RMobilePhone::TCspCallOffering params;    
    TInt retVal = iCSP->CspCallOffering(params);
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RCustomerServiceProfileCache_CspCallRestrictionL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RCustomerServiceProfileCache_CspCallRestrictionL()
    {
    RMobilePhone::TCspCallRestriction params;    
    TInt retVal = iCSP->CspCallRestriction(params);
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RCustomerServiceProfileCache_CspCPHSTeleservicesL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RCustomerServiceProfileCache_CspCPHSTeleservicesL()
    {
    RMobilePhone::TCspCPHSTeleservices params;    
    TInt retVal = iCSP->CspCPHSTeleservices(params);
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	MT_RCustomerServiceProfileCache_CspCPHSValueAddedServicesL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RCustomerServiceProfileCache_CspCPHSValueAddedServicesL()
    {
    RMobilePhone::TCspValueAdded params;    
    TInt retVal = iCSP->CspCPHSValueAddedServices(params);
    }   
     
///////////////////////////////////////////////////////////////////////////////////
////	MT_RCustomerServiceProfileCache_CspTeleServicesL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RCustomerServiceProfileCache_CspTeleServicesL()
    {
    RMobilePhone::TCspTeleservices params;    
    TInt retVal = iCSP->CspTeleServices(params);
    }
    
///////////////////////////////////////////////////////////////////////////////////
////	MT_RCustomerServiceProfileCache_HandleRefreshL
///////////////////////////////////////////////////////////////////////////////////     
void MT_SSSettings::MT_RCustomerServiceProfileCache_HandleRefreshL()
    {       
    TInt retVal = iCSP->HandleRefresh();
    }

///////////////////////////////////////////////////////////////////////////////////
////	MT_RCustomerServiceProfileCache_ChangedCspTableL
/////////////////////////////////////////////////////////////////////////////////// 
void MT_SSSettings::MT_RCustomerServiceProfileCache_ChangedCspTableL()
    {
    RMobilePhone::TMobilePhoneCspFileV1 cspSettings;
    RMobilePhone::TMobilePhoneCspFileV1Pckg cspSettingsPckg ( cspSettings );
    RMobilePhone::TMobilePhoneCspFileV1 cspValues;
    RMobilePhone::TMobilePhoneCspFileV1Pckg cspValuePckg ( cspValues );

    iCSP->ChangedCspTable( cspSettingsPckg, cspValuePckg );
    }

///////////////////////////////////////////////////////////////////////////////////
////  TEST TABLE
///////////////////////////////////////////////////////////////////////////////////
EUNIT_BEGIN_TEST_TABLE(
    MT_SSSettings,
    "Add test suite description here.",
    "MODULE" )

// RCustomerServiceProfileCache Class test.
    
EUNIT_TEST(
    "RCSP:CallCompletion",
    "RCustomerServiceProfileCache",
    "CspCallCompletion",
    "FUNCTIONALITY",
    CSPSetupL, MT_RCustomerServiceProfileCache_CspCallCompletionL, CSPTeardown)
    
EUNIT_TEST(
    "RCSP:CspCallOffering",
    "RCustomerServiceProfileCache",
    "CspCallOffering",
    "FUNCTIONALITY",
    CSPSetupL, MT_RCustomerServiceProfileCache_CspCallOfferingL, CSPTeardown)
    

EUNIT_TEST(
    "RCSP:CspCallRestriction",
    "RCustomerServiceProfileCache",
    "CspCallRestriction",
    "FUNCTIONALITY",
    CSPSetupL, MT_RCustomerServiceProfileCache_CspCallRestrictionL, CSPTeardown)
    
EUNIT_TEST(
    "RCSP:CspCPHSTeleservices",
    "RCustomerServiceProfileCache",
    "CspCPHSTeleservices",
    "FUNCTIONALITY",
    CSPSetupL, MT_RCustomerServiceProfileCache_CspCPHSTeleservicesL, CSPTeardown)

EUNIT_TEST(
    "RCSP:CspCPHSValueAddedServices",
    "RCustomerServiceProfileCache",
    "CspCPHSValueAddedServices",
    "FUNCTIONALITY",
    CSPSetupL, MT_RCustomerServiceProfileCache_CspCPHSValueAddedServicesL, CSPTeardown)

EUNIT_TEST(
    "RCSP:CspTeleServices",
    "RCustomerServiceProfileCache",
    "CspTeleServices",
    "FUNCTIONALITY",
    CSPSetupL, MT_RCustomerServiceProfileCache_CspTeleServicesL, CSPTeardown)

EUNIT_TEST(
    "RCSP:HandleRefresh",
    "RCustomerServiceProfileCache",
    "HandleRefresh",
    "FUNCTIONALITY",
    CSPSetupL, MT_RCustomerServiceProfileCache_HandleRefreshL, CSPTeardown)
    
EUNIT_TEST(
    "RCSP:ChangedCspTable",
    "RCustomerServiceProfileCache",
    "ChangedCspTable",
    "FUNCTIONALITY",
    CSPSetupL, MT_RCustomerServiceProfileCache_ChangedCspTableL, CSPTeardown)
        
// CSSSettingsRefreshContainer Class test.

EUNIT_TEST(
    "Ref:NewL",
    "CSSSettingsRefreshContainer",
    "NewL",
    "FUNCTIONALITY",
    EmptySetupL, MT_CSSSettingsRefreshContainer_NewLL, EmptyTeardown)

// RSSSettings Class test.

EUNIT_TEST(
    "RSS:Open Close",
    "RSSSettings",
    "Close",
    "FUNCTIONALITY",
    EmptySetupL, MT_RSSSettings_OpenCloseL, EmptyTeardown)

EUNIT_TEST(
    "RSS:Open with EtelParam",
    "RSSSettings",
    "Open",
    "FUNCTIONALITY",
    EmptySetupL, MT_RSSSettings_Open_1L, EmptyTeardown)

EUNIT_TEST(
    "RSS:RegisterAll",
    "RSSSettings",
    "RegisterAll",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_RegisterAllL, Teardown)

EUNIT_TEST(
    "RSS:Register",
    "RSSSettings",
    "Register",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_RegisterL, Teardown)

EUNIT_TEST(
    "RSS:CancelAll",
    "RSSSettings",
    "CancelAll",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_CancelAllL, Teardown)

EUNIT_TEST(
    "RSS:Cancel",
    "RSSSettings",
    "Cancel",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_CancelL, Teardown)

EUNIT_TEST(
    "RSS:Get",
    "RSSSettings",
    "Get",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_GetL, Teardown)

EUNIT_TEST(
    "RSS:Set",
    "RSSSettings",
    "Set",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_SetL, Teardown)

EUNIT_TEST(
    "RSS:HandleSIMChanged",
    "RSSSettings",
    "HandleSIMChanged",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_HandleSIMChangedL, Teardown)

EUNIT_TEST(
    "RSS:PreviousCugValue",
    "RSSSettings",
    "PreviousCugValue",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_PreviousCugValueL, Teardown)

EUNIT_TEST(
    "RSS:ResetPreviousCugValue",
    "RSSSettings",
    "ResetPreviousCugValue",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_ResetPreviousCugValueL, Teardown)

EUNIT_TEST(
    "RSS:HandleRefresh",
    "RSSSettings",
    "HandleRefresh",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_HandleRefreshL, Teardown)

EUNIT_TEST(
    "RSS:IsValueValidCugIndex",
    "RSSSettings",
    "IsValueValidCugIndex",
    "FUNCTIONALITY",
    SetupL, MT_RSSSettings_IsValueValidCugIndexL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
