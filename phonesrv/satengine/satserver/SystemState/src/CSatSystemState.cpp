/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides system state information to SAT Server
*
*/



#include    <mtclreg.h>
#include    <smsclnt.h>
#include    <smutset.h>
#include    <PSVariables.h>             // Publish & Suscribe keys
#include    <centralrepository.h>       // CRepository
#include    <CommonEngineDomainCRKeys.h>
#include    <ProfileEngineSDKCRKeys.h>
#include    <settingsinternalcrkeys.h>
#include    <satdomainpskeys.h>
#define Q_OS_SYMBIAN // needed to activate homescreendomainpskeys.h
#include    <homescreendomainpskeys.h>

// ******************************************************************
// TODO: ScreensaverInternalPSKeys.h does no longer exist.
// Must find an alternative way to check screen locked state.
// #include    <ScreensaverInternalPSKeys.h>
// ******************************************************************
#include    <ctsydomainpskeys.h>
#include    <sbdefs.h>
#include    <BTSapDomainPSKeys.h>

#include    "SATInternalPSKeys.h"
#include    "CSatSystemState.h"
#include    "msatmultimodeapi.h"
#include    "SatLog.h"


// -----------------------------------------------------------------------------
// Helper function to define a property and log result
// -----------------------------------------------------------------------------
//
TInt DefineProperty(TUid aCategory, TUint aKey, TInt aAttr,
    const TSecurityPolicy & aReadPolicy, const TSecurityPolicy& aWritePolicy)
    {
    TInt err = RProperty::Define( aCategory, aKey, aAttr, aReadPolicy, aWritePolicy );
    LOG3( SIMPLE, 
        "SATSYSTEMSTATE: CSatSystemState::ConstructL: property %d defined, err=%d",
        aKey, err);
    return err;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSystemState::CSatSystemState( MSatMultiModeApi& aPhone ) : iPhone( aPhone )
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatSystemState::CSatSystemState calling-exiting" )
    }


// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatSystemState::ConstructL( MSatMultiModeApi& aPhone )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::ConstructL calling" )

    // Define policies
    TSecurityPolicy readPolicy( ECapabilityReadUserData );
    TSecurityPolicy writePolicy( ECapabilityWriteUserData );

    DefineProperty( KPSUidSatServerInternal, 
        KSatIdleModeText,
        RProperty::ELargeText, 
        readPolicy, 
        writePolicy );

    DefineProperty( KPSUidSatServerInternal, 
        KSatIdleModeTextIconId,
        RProperty::EInt, 
        readPolicy, 
        writePolicy );

    DefineProperty( KPSUidSatServerInternal, 
        KSatIdleModeTextIconQualifier,
        RProperty::EInt, 
        readPolicy, 
        writePolicy );

    // Set default (empty) values
    WriteSetIdleModeText( KNullDesC, KErrNotFound, RSat::ENoIconId );

    DefineProperty( KPSUidSatServer, 
        KSatLanguageSelectionTimeBeforeReboot,
        RProperty::EInt, 
        readPolicy, 
        writePolicy );
    
    // Create Network Registration Status Monitor.
    iNetworkRegStatusMonitor = CSatNetworkRegStatusMonitor::NewL( aPhone );

    // Start Network Registration Status Monitor.
    iNetworkRegStatusMonitor->Start();

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
MSatSystemState* CSatSystemState::NewL( MSatMultiModeApi& aPhone )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::NewL calling" )

    CSatSystemState* self = new( ELeave ) CSatSystemState( aPhone );
    CleanupStack::PushL( self );
    self->ConstructL( aPhone );
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::NewL exiting" )
    return self;
    }


// Destructor
CSatSystemState::~CSatSystemState()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::~CSatSystemState calling" )

    delete iNetworkRegStatusMonitor;
    iNetworkRegStatusMonitor = NULL;

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::~CSatSystemState exiting" )
    }

// -----------------------------------------------------------------------------
// Returns network registration status.
// -----------------------------------------------------------------------------
//
RMobilePhone::TMobilePhoneRegistrationStatus
    CSatSystemState::GetNetworkRegistrationStatus()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::\
        GetNetworkRegistrationStatus calling-exiting" )
    return iNetworkRegStatusMonitor->CurrentValue();
    }

// -----------------------------------------------------------------------------
// Checks current call status and returns true only if cuurent call status is
// voice call active.
// -----------------------------------------------------------------------------
//
TBool CSatSystemState::IsCallActive()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::IsCallActive calling" )
    TInt callActiveState = EPSCTsyCallTypeNone;

    // Get current call status from P&S
    const TInt errCode( RProperty::Get(
        KPSUidCtsyCallInformation,
        KCTsyCallType,
        callActiveState ) );

    // Check current call type
    TBool result(
        ( EPSCTsyCallTypeCSVoice == callActiveState ) &&
        ( KErrNone == errCode ) );

    LOG2(
        SIMPLE,
        "SATSYSTEMSTATE: CSatSystemState::IsCallActive exiting with value: %d",
        result )
    return result;
    }

// -----------------------------------------------------------------------------
// Checks current call state and returns true if phone is alerting or ringing.
// -----------------------------------------------------------------------------
//
TBool CSatSystemState::IsCallIncoming()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::IsCallIncoming calling" )

    // Using MSatMultiModeApi::IsCallIncoming to check call status instead of
    // checking P&S key so that we can get the status on time
    TBool result( iPhone.IsCallIncoming() );

    LOG2( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::IsCallIncoming exiting \
        with value: %d", result )
    return result;
    }

// -----------------------------------------------------------------------------
// Gets the SMS Service center number and returns it.
// -----------------------------------------------------------------------------
//
const TDesC& CSatSystemState::SCANumber()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::SCANumber calling" )

    // Null possible previous SCA number
    iScaAddress = KNullDesC;
    TBuf<KMaxSCANumberSize> address;

    TRAPD( err, ReadCurrentSMSCL( address ) );
    LOG2( SIMPLE,
    "SATSYSTEMSTATE: CSatSystemState::SCANumber err: %d", err )
    // If any error occurs, return null string.
    if ( KErrNone != err )
        {
        address = KNullDesC;
        }

    iScaAddress = address;

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::SCANumber exiting" )
    return iScaAddress;
    }

// -----------------------------------------------------------------------------
// Method is used when the information of phones silent mode is needed.
// Returns true if silent mode is on and false otherwise.
// -----------------------------------------------------------------------------
//
TBool CSatSystemState::IsSilentMode()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::IsSilentMode calling" )
    TBool result( EFalse );
    TInt silenceMode( -1 );
    CRepository* centRep( NULL );

    TRAPD( err, centRep = CRepository::NewL( KCRUidProfileEngine ) );
    LOG2( SIMPLE, "SATSYSTEMSTATE:CSatSystemState::IsSilentMode\
                   CRepository::NewL result: %i", err )
    if ( ( KErrNone == err ) && centRep )
        {
        // Get the needed information from CentRep
        centRep->Get( KProEngActiveMessageAlert, silenceMode );

        // Delete CentRep
        delete centRep;

        if ( 0 == silenceMode )
            {
            LOG( SIMPLE, "SATSYSTEMSTATE:CSatSystemState::IsSilentMode\
                          Silent mode" )
            result = ETrue;
            }
        }
    
    LOG2(
        SIMPLE,
        "SATSYSTEMSTATE: CSatSystemState::IsSilentMode exiting with value: %d",
        result )
    return result;
    }

// -----------------------------------------------------------------------------
// Checks is phone in idle state.
// -----------------------------------------------------------------------------
//
TBool CSatSystemState::IsPhoneInIdleStateL()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::IsPhoneInIdleState calling" )
    TInt idleStatus( EHomeScreenWidgetViewForeground );

    // Get the idle status from P&S
    User::LeaveIfError( RProperty::Get(
        KHsCategoryUid, 
        KHsCategoryStateKey, 
        idleStatus));
    // Returns true if phone in idle state. 
    const TBool result( EHomeScreenWidgetViewForeground == idleStatus );

    LOG2( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::IsPhoneInIdleState exiting \
        with value: %d", result )
    return result;
    }
    
// -----------------------------------------------------------------------------
// Checks is screen saver actived from idle.
// -----------------------------------------------------------------------------
//
TBool CSatSystemState::IsScreenSaverActivedFromIdle()
    {
    LOG( SIMPLE, 
    "SATSYSTEMSTATE: CSatSystemState::IsScreenSaverActivedFromIdle calling" )
    TBool result( EFalse );

    /*
    // TODO: This method can no longer be used for checking screen saver status.
    // Must find an alternative method.

    TInt screenSaverActive( -1 );

    // Get the idle status from P&S
    TInt err = RProperty::Get( KPSUidScreenSaver, 
        KScreenSaverActivatedFromIdle, screenSaverActive );
        
    if ( KErrNone == err )
        {
        // Returns true if screen saver is actived from idle.    
        if ( screenSaverActive )
            {
            result = ETrue;
            }
        }

    LOG2( SIMPLE, 
        "SATSYSTEMSTATE: CSatSystemState:: IsScreenSaverActivedFromIdle \
         err: %d", err )
    */

    LOG2( SIMPLE, 
        "SATSYSTEMSTATE: CSatSystemState:: \
        IsScreenSaverActivedFromIdle exiting with value: %d", result )
    return result;    
    }

// -----------------------------------------------------------------------------
// Sets time between language selection and reboot.
// -----------------------------------------------------------------------------
//
TInt CSatSystemState::WriteLanguageSelectionTime(
    TInt aTime )
    {
    LOG2( NORMAL,
    "SATSYSTEMSTATE: CSatSystemState::WriteLanguageSelectionTime calling, \
    aTime: %d", aTime )
    TInt errorCode( KErrNone );

    // Set SetUpIdleModeText string
    errorCode = RProperty::Set(
        KPSUidSatServer,
        KSatLanguageSelectionTimeBeforeReboot,
        aTime );
    LOG2( NORMAL,
    "SATSYSTEMSTATE: CSatSystemState::WriteLanguageSelectionTime exiting, \
    errorCode: %d", errorCode )
    return errorCode;
    }

// -----------------------------------------------------------------------------
// Writes idle mode text and icon ID to P&S.
// -----------------------------------------------------------------------------
//
TInt CSatSystemState::WriteSetIdleModeText(
    const TDesC& aString,
    TInt aIconId,
    RSat::TIconQualifier aIconQualifier )
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatSystemState::WriteSetIdleModeText calling" )
    LOG2(
        NORMAL,
        "SATSYSTEMSTATE: CSatSystemState::WriteSetIdleModeText ID: %d",
        aIconId )
    TInt errorCode( KErrNone );

    // Set SetUpIdleModeText string
    errorCode = RProperty::Set(
        KPSUidSatServerInternal,
        KSatIdleModeText,
        aString );
    LOG2( NORMAL,
    "SATSYSTEMSTATE: CSatSystemState::WriteSetIdleModeText errorCode: %d",
     errorCode )
    if ( KErrNone == errorCode )
        {
        // Set SetUpIdleModeText icon
        errorCode = RProperty::Set(
            KPSUidSatServerInternal,
            KSatIdleModeTextIconId,
            aIconId );
        if ( KErrNone == errorCode )
            {
            // Select qualifier value for icon.
            TSatIdleModeTextIconQualifierValue qualifier( KSatIdleIconQInit );
            LOG2( NORMAL,
            "SATSYSTEMSTATE: CSatSystemState::WriteSetIdleModeText \
            aIconQualifier: %d", aIconQualifier )
            switch ( aIconQualifier )
                {
                // Icon not present.
                case RSat::ENoIconId:
                    {
                    qualifier = KSatIdleIconQNoIcon;
                    break;
                    }
                // Icon is self explanatory. Displayed instead text string.
                case RSat::ESelfExplanatory:
                    {
                    qualifier = KSatIdleIconQSelfExplanatory;
                    break;
                    }
                // Icon is not self explanatory. Displayed with text string.
                case RSat::ENotSelfExplanatory:
                    {
                    qualifier = KSatIdleIconQNotSelfExplanatory;
                    break;
                    }
                // Icon qualifier is not set.
                case RSat::EIconQualifierNotSet:
                default:
                    {
                    qualifier = KSatIdleIconQInit;
                    break;
                    }
                }

                LOG2( NORMAL,
                "SATSYSTEMSTATE: CSatSystemState::WriteSetIdleModeText \
                IconQualifier: %d", qualifier )

            // Set qualifier for icon.
            errorCode = RProperty::Set(
                KPSUidSatServerInternal,
                KSatIdleModeTextIconQualifier,
                qualifier );
            }
        }

    LOG2( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::WriteSetIdleModeText \
        exiting with value: %d", errorCode )
    return errorCode;
    }

// -----------------------------------------------------------------------------
// Checks is backup process ongoing.
// -----------------------------------------------------------------------------
//
TBool CSatSystemState::IsBackupProcessOngoing()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatSystemState::IsBackupProcessOngoing calling" )
    TInt backupState( conn::ENoBackup );

    // Get the backup state from P&S
    const TInt errCode( RProperty::Get(
        KUidSystemCategory,
        conn::KUidBackupRestoreKey,
        backupState ) );

    TBool result(
        ( conn::ENoBackup != ( backupState & conn::KBackupIncTypeMask ) )
        && ( KErrNone == errCode ) );

    LOG2( SIMPLE,
        "SATSYSTEMSTATE: CSatSystemState::IsBackupProcessOngoing exiting \
        with value: %d", result )
    return result;
    }

// -----------------------------------------------------------------------------
// CSatSystemState::IsConfirmSatOperationsOn
// -----------------------------------------------------------------------------
//
TBool CSatSystemState::IsConfirmSatOperationsOn()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatSystemState::IsConfirmSatOperationsOn calling" )
    TBool result( EFalse );
    TInt confirmSatOperations( 0 );
    CRepository* centRep( NULL );

    TRAPD( err, centRep = CRepository::NewL( KCRUidPersonalizationSettings ) );
    LOG2( SIMPLE, "SATSYSTEMSTATE:   CentRep failed: %i", err )
    if ( ( KErrNone == err ) && centRep )
        {
        // Get the needed information from CentRep
        centRep->Get( KSettingsConfirmSatOperations, confirmSatOperations );

        // Delete CentRep
        delete centRep;

        if ( 1 == confirmSatOperations )
            {
            LOG( SIMPLE,
            "SATSYSTEMSTATE: CSatSystemState::IsConfirmSatOperationsOn \
            1 == confirmSatOperations" )
            result = ETrue;
            }
        }
    
    LOG2( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::IsConfirmSatOperationsOn \
        exiting with value: %d", result )
    return result;
    }

// -----------------------------------------------------------------------------
// CSatSystemState::DisplayTextLanguage
// -----------------------------------------------------------------------------
//
TBool CSatSystemState::DisplayTextLanguage()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatSystemState::DisplayTextLanguage calling" )
    TInt selLanguage( 0 );
    CRepository* centRep( NULL );

    // Create instance to CentRep
    TRAPD( err, centRep = CRepository::NewL( KCRUidCommonEngineKeys ) );
    LOG2( SIMPLE, "SATSYSTEMSTATE:   CentRep failed: %i", err )
    if ( ( KErrNone == err ) && centRep )
        {
        // Get the needed information from CentRep
        centRep->Get( KGSDisplayTxtLang, selLanguage );

        // Delete CentRep
        delete centRep;
        }
    
    LOG2( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::DisplayTextLanguage \
        exiting with value: %d", selLanguage )
    return selLanguage;
    }

// -----------------------------------------------------------------------------
// Checks is BT SAP active
// -----------------------------------------------------------------------------
//
TBool CSatSystemState::IsBtSapActive()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatSystemState::IsBtSapActive calling" )
    TInt sapActive( EBTSapNotConnected );

    // Get the backup state from P&S
    const TInt errCode( RProperty::Get(
        KPSUidBluetoothSapConnectionState,
        KBTSapConnectionState,
        sapActive ) );

    LOG2(
        SIMPLE,
        "SATSYSTEMSTATE:   RProperty::Get returned value: %i", errCode )

    TBool result( ( ( EBTSapConnecting == sapActive ) ||
                    ( EBTSapConnected == sapActive ) ) &&
                    ( KErrNone == errCode ) );

    LOG2(
        SIMPLE,
        "SATSYSTEMSTATE: CSatSystemState::IsBtSapActive exiting with value: %i",
        result )
    return result;
    }

// -----------------------------------------------------------------------------
// Reads current SMS Center number.
// -----------------------------------------------------------------------------
//
void CSatSystemState::ReadCurrentSMSCL( TDes& aAddress )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::ReadCurrentSMSCL calling" )

    aAddress = KNullDesC;
    TInt status( KErrNone );

    // Open message server connection.
    CMsvSession* msvSession = CMsvSession::OpenSyncL( *this );
    CleanupStack::PushL( msvSession );

    CClientMtmRegistry* clientRegistry =
        CClientMtmRegistry::NewL( *msvSession );

    CleanupStack::PushL( clientRegistry );

    // Get the account.
    TInt serviceId( ServiceForMTML( *msvSession, KUidMsgTypeSMS ) );
    if ( KMsvUnknownServiceIndexEntryId != serviceId )
        {
        LOG( SIMPLE, 
        "SATSYSTEMSTATE: CSatSystemState::ReadCurrentSMSCL \
        KMsvUnknownServiceIndexEntryId != serviceId" )
        // Read the setting.
        CSmsClientMtm* mtm =
            static_cast<CSmsClientMtm*>( clientRegistry->NewMtmL( KUidMsgTypeSMS ) );
        CleanupStack::PushL( mtm );
        mtm->SwitchCurrentEntryL( serviceId );
        mtm->LoadMessageL();
        // Findout the number.
        const CSmsSettings& settings = mtm->ServiceSettings();

#ifndef __HIDE_MESSAGING_API_V1__
        if ( settings.NumSCAddresses() != 0 )
            {
            if ( settings.DefaultSC() < settings.NumSCAddresses() )
                {
                LOG( SIMPLE, 
                "SATSYSTEMSTATE: CSatSystemState::ReadCurrentSMSCL \
                settings.DefaultSC() < settings.NumSCAddresses()" )
                TPtrC addr = settings.SCAddress( settings.DefaultSC() ).Address();
                aAddress = addr;
                }
            else
                {
                LOG( SIMPLE, 
                "SATSYSTEMSTATE: CSatSystemState::ReadCurrentSMSCL \
                others" )
                status = KErrOverflow;
                }
            }
#else

        if ( settings.ServiceCenterCount() != 0 )
            {
            LOG( SIMPLE, 
            "SATSYSTEMSTATE: CSatSystemState::ReadCurrentSMSCL \
            settings.ServiceCenterCount() != 0" )
            if ( settings.DefaultServiceCenter() < settings.ServiceCenterCount() )
                {
                LOG( SIMPLE, 
                "SATSYSTEMSTATE: CSatSystemState::ReadCurrentSMSCL \
                settings.DefaultServiceCenter() < \
                settings.ServiceCenterCount()" )
                TPtrC addr = settings.GetServiceCenter(
                    settings.DefaultServiceCenter() ).Address();
                aAddress = addr;
                }
            else
                {
                status = KErrOverflow;
                }
            }

#endif
        else
            {
            LOG( SIMPLE, 
            "SATSYSTEMSTATE: CSatSystemState::ReadCurrentSMSCL \
            settings.ServiceCenterCount() == 0" )
            status = KErrNotFound;
            }

        // Cleanup.
        CleanupStack::PopAndDestroy( mtm );
        }
    else
        {
        LOG( SIMPLE, 
        "SATSYSTEMSTATE: CSatSystemState::ReadCurrentSMSCL \
        KMsvUnknownServiceIndexEntryId == serviceId" )
        status = KErrNotFound;
        }

    CleanupStack::PopAndDestroy( clientRegistry );
    CleanupStack::PopAndDestroy( msvSession );

    if ( KErrNone != status )
        {
        LOG2( NORMAL,
            "SATSYSTEMSTATE: CSatSystemState::ReadCurrentSMSCL leave: %d",
            status )
        User::Leave( status );
        }

    LOG( NORMAL, "SATSYSTEMSTATE: CSatSystemState::ReadCurrentSMSCL exiting" )
    }

// -----------------------------------------------------------------------------
// Specifies an entry in the Message Server index.
// -----------------------------------------------------------------------------
//
TMsvId CSatSystemState::ServiceForMTML(
    CMsvSession& aSession,
    const TUid& aMtm ) const
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatSystemState::ServiceForMTML calling" )

    CMsvEntry* entry = aSession.GetEntryL( KMsvRootIndexEntryIdValue );
    CleanupStack::PushL( entry );
    TMsvId serviceId = KMsvUnknownServiceIndexEntryId;
    LOG2( SIMPLE, 
    "SATSYSTEMSTATE: CSatSystemState::ServiceForMTML count: %d",
     entry->Count() )
    // Check are there any entries
    if ( entry->Count() != 0 )
        {        
        entry->SetSortTypeL(
            TMsvSelectionOrdering( KMsvGroupByType | KMsvGroupByStandardFolders,
            EMsvSortByDetailsReverse,
            ETrue ) );

        const TMsvEntry* tentry;
        for ( TInt cc( entry->Count() ); --cc >= 0; )
            {
            tentry = &( *entry )[cc];
            if ( tentry->iMtm == aMtm )
                {
                LOG( SIMPLE, 
                "SATSYSTEMSTATE: CSatSystemState::ServiceForMTML \
                tentry->iMtm == aMtm" )
                if ( ( tentry->iType.iUid == KUidMsvServiceEntryValue ) &&
                     ( tentry->Id() != KMsvLocalServiceIndexEntryIdValue ) )
                    {
                    LOG( SIMPLE, 
                    "SATSYSTEMSTATE: CSatSystemState::ServiceForMTML \
                    exit loop" )
                    serviceId = tentry->Id();
                    cc = 0; // To exit for-loop. Replaces break.
                    }
                }
            }
        }

    CleanupStack::PopAndDestroy( entry );

    LOG2( SIMPLE,
    "SATSYSTEMSTATE: CSatSystemState::ServiceForMTML exiting with value: %d",
    serviceId )
    return serviceId;
    }

// -----------------------------------------------------------------------------
// Method is used when the information of phones warnings and game tone
// mode is needed.
// Returns true if silent mode is on and false otherwise.
// -----------------------------------------------------------------------------
//
TBool CSatSystemState::IsWarningAndGameTonesOn()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: \
        CSatSystemState::IsWarningAndGameTonesOn calling" )

    TBool modeOn( ETrue );
    CRepository* centRep( NULL );

    TRAPD( err, centRep = CRepository::NewL( KCRUidProfileEngine ) );
    LOG2( SIMPLE, "SATSYSTEMSTATE:   CentRep failed: %i", err )
    if ( ( KErrNone == err ) && centRep )
        {
        // Get the needed information from CentRep
        centRep->Get( KProEngActiveWarningTones, modeOn );

        // Delete CentRep
        delete centRep;

        }
   
    LOG2(
        SIMPLE,
        "SATSYSTEMSTATE: \
        CSatSystemState::IsWarningAndGameTonesOn exiting with value: %d",
        modeOn )

    return modeOn;
    }
