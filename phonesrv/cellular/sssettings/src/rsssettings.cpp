/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class for getting information about the Clir, Cug, Als and 
*                Als blocking status in the phone. 
*
*/



// INCLUDE FILES
#include    <bldvariant.hrh> // for feature definitions
#include    <e32base.h>
#include    <rmmcustomapi.h>
#include    <mmtsy_names.h>
#include    <PSVariables.h> 
#include    <startupdomainpskeys.h>
#include    <centralrepository.h> // central repository 
#include    <sssettingsprivatecrkeys.h> // settings central repository keys. 
#include    <telservicesinternalcrkeys.h>
#include    <telservicesvariant.hrh>

#include    "rsssettings.h" 
#include    "msssettingsobserver.h" 
#include    "csssettingsnotifier.h" 
#include    "csssettingsalsnotifier.h" 
#include    "sssettingslogger.h" 

// CONSTANTS
_LIT( KPanicCat, "SSSettings" );
#define KSSSettingsTSYName KMmTsyModuleName
const TInt KSSSettingsTSYNameMaxLength( 80 );
const TInt KSSSettingsLastSetting(ESSSettingsDefaultCug);
const TInt KSSSettingsMaxETelRetryCount(7);
const TInt KSSSettingsETelRetryCountTimeout(100000); 

// ================= LOCAL FUNCTIONS =======================
TInt SSSettingsDoOpenEtelConnection( RTelServer& aEtel, RPhone& aPhone )
    {
    TInt phones(0);
    if ( aEtel.EnumeratePhones( phones) == KErrNone )
        {
        for ( TInt p = 0 ; p < phones ; p++ )
            {
            TBuf<KSSSettingsTSYNameMaxLength> tsyName;
            if ( aEtel.GetTsyName( p,tsyName ) == KErrNone )
                {
                if ( tsyName.CompareF( KSSSettingsTSYName ) == 0 )
                    {
                    RTelServer::TPhoneInfo info;
                    if ( aEtel.GetPhoneInfo( p,info ) == KErrNone )
                        {
                        if ( aPhone.Open( aEtel, info.iName ) == KErrNone )
                            {
                            return KErrNone;
                            }
                        }
                    }
                }
            }
        }
    return KErrGeneral;
    }


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C RSSSettings::RSSSettings()
    {
    iData = NULL;
    }


// ---------------------------------------------------------
// RSSSettings::Close
// ---------------------------------------------------------
//
EXPORT_C void RSSSettings::Close()
   {
   __SSSLOGSTRING("[SSS]--> RSSSettings::Close");
   DeleteSettingsData();
   __SSSLOGSTRING("[SSS] <--RSSSettings::Close");
   }

// ---------------------------------------------------------
// RSSSettings::Open#1
// ---------------------------------------------------------
//
EXPORT_C TInt RSSSettings::Open()
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::Open");
    TInt ret(KErrNone);
    ret = Open( NULL );
    __SSSLOGSTRING("[SSS] <--RSSSettings::Open");
    return ret;
    }

// ---------------------------------------------------------
// RSSSettings::Open#3
// ---------------------------------------------------------
//
EXPORT_C TInt RSSSettings::Open( RTelServer* aTelServer )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::Open3");
    TRAPD( error, OpenL( aTelServer ) );
    if ( error != KErrNone )
        {
        DeleteSettingsData();
        }
    __SSSLOGSTRING("[SSS] <--RSSSettings::Open3");
    return error;

    }

// ---------------------------------------------------------
// RSSSettings::OpenL
// ---------------------------------------------------------
//
void RSSSettings::OpenL( RTelServer* aTelServer )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::OpenL");
    if ( iData )
        {
        DeleteSettingsData();
        }

    iData = new ( ELeave ) TSSSettingsData;

    iData->iOwnTelServer = ETrue;
    iData->iTelServer = NULL;
    iData->iCallBack = NULL;
    iData->iCallBackSetting = KErrBadDescriptor;
    iData->iCallBackValue = KErrBadDescriptor;
    iData->iNotifier = NULL;
    iData->iCustomPhone = NULL;
    iData->iCUGDefault = NULL;
    iData->iRepository = NULL;

    // Create Notifier
    // ConstructL is called later when the sessions are opened.
    iData->iNotifier = new ( ELeave ) CSSSettingsNotifier( *this );

    // Crate call back  for async inform
    TCallBack callBack( InformCallBack, this );
    iData->iCallBack = new ( ELeave ) 
        CAsyncCallBack( callBack, CActive::EPriorityStandard );

    // Check central repository connection.
    iData->iRepository = CRepository::NewL(
        KCRUidSupplementaryServiceSettings );

    // Check ETel connecion
    if ( aTelServer )
        {
        iData->iOwnTelServer = EFalse;
        iData->iTelServer = aTelServer;
        // Given ETel connection must be open.
        if ( iData->iTelServer->Handle() == 0 )
            {
            User::Leave( KErrBadHandle );
            }
        }
    else
        {
        iData->iOwnTelServer = ETrue;
        iData->iTelServer = new ( ELeave ) RTelServer();

        // Retry ETel connection:
        TInt err(KErrNone);
        for ( TInt i = 0 ; i < KSSSettingsMaxETelRetryCount ; i++ )
            {
            err = iData->iTelServer->Connect();
            if ( err == KErrNone )
                {
                break;
                }
            User::After( KSSSettingsETelRetryCountTimeout );
            }

        User::LeaveIfError( err );
        }

    // Open phones
    TInt error = iData->iTelServer->LoadPhoneModule( KSSSettingsTSYName );
    if (( error != KErrNone ) && ( error != KErrAlreadyExists ))
        {
        User::Leave( error );
        }

    User::LeaveIfError( SSSettingsDoOpenEtelConnection( 
        *iData->iTelServer, iData->iMobilePhone ) );

    iData->iCustomPhone = new ( ELeave ) RMmCustomAPI;
    iData->iCustomPhone->Open( iData->iMobilePhone );


    // Finish off the notifier construct.
    iData->iNotifier->ConstructL();

    // Create arrays:
    for ( TInt i = 0 ; i <= KSSSettingsLastSetting ; i++ )
        {
        RSSSettingsObserverArray newArray;
        User::LeaveIfError( iData->iObservers.Append( newArray ) );
        }

    //Get default CUG
    error = GetDefaultCugL( iData->iCUGDefault );
    if ( error != KErrNone )
        {
        //Support legacy, if error occurs
        iData->iCUGDefault = ESSSettingsCugDefault; 
        }
    __SSSLOGSTRING("[SSS] <--RSSSettings::OpenL");
    }

// ---------------------------------------------------------
// RSSSettings::DeleteSettingsData
// ---------------------------------------------------------
//
void RSSSettings::DeleteSettingsData()
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::DeleteSettingsData");
    if ( iData == NULL )
        {
        return;
        }

    // if some event is waiting for the observer calls..
    if (( iData->iCallBackSetting != KErrBadDescriptor ||
          iData->iCallBackValue != KErrBadDescriptor ) &&
          iData->iCallBack )
        {
        iData->iCallBack->Cancel();
        InformCallBack( this );
        }

    // close array
    MSSSettingsObserver* observer = FindOneObserver();
    while ( observer )
        {
        CancelAll( *observer );
        observer = FindOneObserver();
        }

    iData->iObservers.Reset();

    // delete callback
    if ( iData->iCallBack )
        {
        iData->iCallBack->Cancel();
        delete iData->iCallBack;
        iData->iCallBack = NULL;
        }

    // Delete notifier
    delete iData->iNotifier;
    iData->iNotifier = NULL;

    delete iData->iRepository;
    iData->iRepository = NULL;
    
    // close phones
    if ( iData->iCustomPhone )
        {
        iData->iCustomPhone->Close();
        delete iData->iCustomPhone;
        iData->iCustomPhone = NULL;
        }

    iData->iMobilePhone.Close();

    // close ETel session
    if (( iData->iTelServer ) && ( iData->iOwnTelServer ))
        {
        if ( iData->iTelServer->Handle() )
            {
            iData->iTelServer->Close();
            }
        delete iData->iTelServer;
        iData->iTelServer = NULL;
        }

    // delete data object
    delete iData;
    iData = NULL;
    __SSSLOGSTRING("[SSS] <--RSSSettings::DeleteSettingsData");
    }

// ---------------------------------------------------------
// RSSSettings::InformCallBack
// KErrBadDescriptor is used as not defined.
// ---------------------------------------------------------
//
TInt RSSSettings::InformCallBack( TAny* aPtr )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::InformCallBack");
    RSSSettings* settings = static_cast <RSSSettings*> ( aPtr );

    if ( !settings->iData )
        {
        return KErrNone;
        }

    // We need to have both setting and value to make observer call.
    if ( settings->iData->iCallBackSetting == KErrBadDescriptor ||
         settings->iData->iCallBackValue == KErrBadDescriptor )
        {
        settings->iData->iCallBackSetting = KErrBadDescriptor;
        settings->iData->iCallBackValue = KErrBadDescriptor;
        return KErrNone;
        }

    settings->InformChange(
        (TSSSettingsSetting)settings->iData->iCallBackSetting , 
        settings->iData->iCallBackValue );

    settings->iData->iCallBackSetting = KErrBadDescriptor;
    settings->iData->iCallBackValue = KErrBadDescriptor;
    
    __SSSLOGSTRING("[SSS] <--RSSSettings::InformCallBack");
    return KErrNone;
    }

// ---------------------------------------------------------
// RSSSettings::InformChange
// ---------------------------------------------------------
//
void RSSSettings::InformChange( TSSSettingsSetting aSetting, TInt aNewValue )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::InformChange");
    __ASSERT_DEBUG( iData, Panic( SSSettingsPanicOpenNotDone ) );

    RSSSettingsObserverArray& array = iData->iObservers[aSetting];
    for ( TInt i = 0 ; i < array.Count() ; i++ )
        {
        array[i]->PhoneSettingChanged( aSetting, aNewValue );
        }

    __SSSLOGSTRING("[SSS] <--RSSSettings::InformChange");
    }

// ---------------------------------------------------------
// RSSSettings::RegisterAll
// ---------------------------------------------------------
//
EXPORT_C TInt RSSSettings::RegisterAll( MSSSettingsObserver& aObserver )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::RegisterAll");
    __ASSERT_DEBUG( iData, Panic( SSSettingsPanicOpenNotDone ) );
    if ( !iData )
        {
        return KErrBadHandle;
        }

    TInt error(KErrNone);
    for ( TInt i = 0 ; i <= KSSSettingsLastSetting ; i++ )
        {
        error = Register( static_cast <TSSSettingsSetting >( i ), aObserver );
        // KErrAlreadyExists: it's already registered.
        // KErrNotSupported: ALS and ALS blocking may return this.
        if (( error != KErrNone ) && 
            ( error != KErrAlreadyExists ) && 
            ( error != KErrNotSupported ))
            {
            CancelAll( aObserver );
            break;
            }
        error = KErrNone;
        }
    __SSSLOGSTRING("[SSS] <--RSSSettings::RegisterAll");
    return error;
    }

// ---------------------------------------------------------
// RSSSettings::Register
// ---------------------------------------------------------
//
EXPORT_C TInt RSSSettings::Register( 
    TSSSettingsSetting aSetting, 
    MSSSettingsObserver& aObserver )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::Register");
    __ASSERT_DEBUG( iData, Panic( SSSettingsPanicOpenNotDone ) );
    if ( !iData )
        {
        return KErrBadHandle;
        }
    __ASSERT_DEBUG( aSetting <= KSSSettingsLastSetting, 
                    Panic( SSSettingsPanicInvalidRequest ) );

    if ( ExistsObserver( aSetting, aObserver ) )
        {
        return KErrAlreadyExists;
        }

    RSSSettingsObserverArray& array = iData->iObservers[aSetting];

    // When the first observer is set, the notifier is activated.
    TInt error(KErrNone);
    if ( array.Count() == 0 )
        {
        error = iData->iNotifier->AddNotifier( aSetting );
        // if BT Sim Access Profile(SAP) is turned on then ETEL returns KErrNotReady.
        if ( error != KErrNone )
            {
            __SSSLOGSTRING1("[SSS]--> RSSSettings::Register -> Error code: %d", error );
            return error;
            }
        }
    error = array.Append( &aObserver );

    __SSSLOGSTRING("[SSS] <--RSSSettings::Register");
    return error;
    }

// ---------------------------------------------------------
// RSSSettings::CancelAll
// ---------------------------------------------------------
//
EXPORT_C void RSSSettings::CancelAll( 
    MSSSettingsObserver& aObserver )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::CancelAll");
    if ( !iData )
        {
        return;
        }

    for ( TInt i = 0 ; i <= KSSSettingsLastSetting ; i++ )
        {
        Cancel( static_cast <TSSSettingsSetting> ( i ), aObserver );
        }
    __SSSLOGSTRING("[SSS] <--RSSSettings::CancelAll");
    }

// ---------------------------------------------------------
// RSSSettings::Cancel
// ---------------------------------------------------------
//
EXPORT_C void RSSSettings::Cancel( 
    TSSSettingsSetting aSetting,
    MSSSettingsObserver& aObserver )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::Cancel");
    if ( !iData )
        {
        return;
        }
    __ASSERT_ALWAYS( aSetting <= KSSSettingsLastSetting, 
                     Panic( SSSettingsPanicInvalidRequest ) );

    RSSSettingsObserverArray& array = iData->iObservers[aSetting];

    TInt index = array.Find( &aObserver );
    if ( index == KErrNotFound )
        {
        return;
        }

    array.Remove( index );
    array.Compress();

    // When the last observer is removed the notifier is cancelled.
    if ( array.Count() == 0 )
        {
        iData->iNotifier->RemoveNotifier( aSetting );
        }
    __SSSLOGSTRING("[SSS] <--RSSSettings::Cancel");
    }

// ---------------------------------------------------------
// RSSSettings::Get
// ---------------------------------------------------------
//
EXPORT_C TInt RSSSettings::Get( TSSSettingsSetting aSetting, TInt& aValue )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::Get");
    __ASSERT_DEBUG( iData, Panic( SSSettingsPanicOpenNotDone ) );
    if ( !iData )
        {
        return KErrBadHandle;
        }

    __ASSERT_DEBUG( aSetting <= KSSSettingsLastSetting, 
                    Panic( SSSettingsPanicInvalidRequest ) );

    TInt error(KErrNone);
    TInt value(0);

    // If Get fails, we try to save the default data.
    switch ( aSetting )
        {
        case ESSSettingsCug:
            error = GetCug( value );
            if (( error != KErrNone ) && ( error != KErrNotFound ))
                {
                break;
                }
            if ( error == KErrNotFound )
                {
                error = iData->iRepository->Set(
                    KSettingsCUG,
                    iData->iCUGDefault );
                    
                if ( error != KErrNone )
                    {
                    break;
                    }
                value = iData->iCUGDefault;
                }
            //If value outside of 0-32767 and not 65535, value is not accepted.
            if ( iData->iCUGDefault == ESSSettingsAltCugDefault )
                {
                if ((( value < ESSSettingsCugSuppress ) || 
                     ( value > ESSSettingsCugMax )) &&
                     ( value != ESSSettingsAltCugDefault ))
                    {
                    error = KErrCorrupt;
                    break;
                    }
                }
            //If value outside of 1-32767, it is not accepted.
            else
                {
                if (( value < ESSSettingsCugSuppress ) || 
                    ( value > ESSSettingsCugMax ))
                    {
                    error = KErrCorrupt;
                    break;
                    }
                }
            aValue = value;
            break;
        case ESSSettingsClir:
            error = iData->iRepository->Get(
                KSettingsCLIR,
                value );
            if ( ( error != KErrNone ) && ( error != KErrNotFound ) )
                {
                break;
                }
            if ( error == KErrNotFound )
                {
                error = iData->iRepository->Set( 
                    KSettingsCLIR,
                    ESSSettingsClirNetworkDefault );
                if ( error != KErrNone )
                    {
                    break;
                    }
                value = ESSSettingsClirNetworkDefault;
                }

            if (( value < ESSSettingsClirNetworkDefault ) || 
                ( value > ESSSettingsClirExplicitSuppress ))
                {
                error = KErrCorrupt;
                break;
                }
            aValue = value;
            break;
            
        case ESSSettingsAls:
            {
            TInt simState(0);
            error = RProperty::Get( KPSUidStartup, KPSSimStatus, simState );
            
            __SSSLOGSTRING2("[SSS] ESSSettingsAls: SIM State: %d Error: %d", simState, error );
           if ( error == KErrNone )
                {
                error = GetALSSettingValue( value, simState );
                aValue = value;
                __SSSLOGSTRING1("[SSS]     RSSSettings::Get: ALS Value: %d", aValue );
                }
            break;
            }
        case ESSSettingsAlsBlocking:
            {
            RMmCustomAPI::TGetAlsBlockStatus alsBlockStatus = 
                RMmCustomAPI::EBlockStatusUnknown;
            error = iData->iCustomPhone->GetAlsBlocked( alsBlockStatus );

            if ( error != KErrNone )
                {
                aValue = ESSSettingsAlsBlockingNotSupported;
                return KErrNone;
                }
            switch ( alsBlockStatus )
                {
                case RMmCustomAPI::EBlockStatusNotSupported: 
                    aValue = ESSSettingsAlsBlockingNotSupported;
                    break;
                case RMmCustomAPI::EBlockStatusActive: 
                    aValue = ESSSettingsAlsBlockingOn;
                    break;
                case RMmCustomAPI::EBlockStatusInactive: 
                    aValue = ESSSettingsAlsBlockingOff;
                    break;
                case RMmCustomAPI::EBlockStatusUnknown: 
                    error = KErrNotSupported;
                    break;
                default:
                    error = KErrNotSupported;
                    break;
                }
            break;
            }
        case ESSSettingsDefaultCug:
            aValue = iData->iCUGDefault;
            break;
        default:
            error = KErrNotSupported;
            break;
        }

    __SSSLOGSTRING("[SSS] <--RSSSettings::Get");
    __SSSLOGSTRING1("[SSS] <--RSSSettings::Get Error code: %d", error );
    return error;
    }

// ---------------------------------------------------------
// RSSSettings::GetALSSettingValue
// ---------------------------------------------------------
//
TInt RSSSettings::GetALSSettingValue( TInt& aValue, const TInt aSimState )
    {
    __SSSLOGSTRING1("[SSS]--> RSSSettings::GetALSSettingValue SIMSTATE: %d", aSimState );
    TInt value(0);
    TBool simAlsValue(EFalse);
    TBool ppAlsValue(EFalse);
    TBool alsCSPSupport(EFalse);
    TInt error(KErrNone);
    /* aValue is set to ESSSettingsAlsNotSupported value because if SimState isnt 
    ESimUsable or ALS handler doesnt exist then value should always be Not Supported.*/ 
    aValue = ESSSettingsAlsNotSupported;
    
    // als is NULL when ALS is not supported by PP als, SIM als and SIM csp als bits.
    CSSSettingsAlsNotifier* als = iData->iNotifier->GetAlsHandler();
    __SSSLOGSTRING1("[SSS] AlsHandler value: %d", als );
    
     // Make sure that SIM state is OK otherwise parameters coming from SIM are mess.
    if ( ( aSimState == ESimUsable ) && als )    
        {
        error = als->GetAlsValue( (TSSSettingsAlsValue&) value );
        if ( error == KErrNone )
            {
            if (( value < ESSSettingsAlsNotSupported ) || 
                ( value > ESSSettingsAlsAlternate ))
                {
                error = KErrCorrupt;
                return error;
                }
            
            TInt alsCSPError(KErrNone);
            als->GetAlsSupport( ppAlsValue, simAlsValue, alsCSPSupport, alsCSPError );
            if ( alsCSPError == KErrNotReady || alsCSPError == KErrServerBusy )
                {
                __SSSLOGSTRING1("[SSS] ALS - CSP specific Error code: %d", alsCSPError );
                return alsCSPError;                             
                }
                                        
            // Decides if ALS is wanted to be shown.
            if ( KErrNotSupported == IsALSSupported( ppAlsValue, simAlsValue, alsCSPSupport, error ) )
                {
                value = ESSSettingsAlsNotSupported;
                }
            aValue = value;
            }
        }
    __SSSLOGSTRING1("[SSS]<-- RSSSettings::GetALSSettingValue Error: %d", error );
    return error;
    }

// ---------------------------------------------------------------------------
// RSSSettings::IsALSSupported
// List all cases when als is shown. If there is some special SIM that doesn't
// support ALS then add case below and give error code KErrNotSupported.
// ---------------------------------------------------------------------------
// 
TInt RSSSettings::IsALSSupported( TInt aPpAlsValue, TInt aSimAlsValue, 
    TBool aAlsCspSupport, TInt aAlsCspError ) const
    {
    TInt error(KErrNotSupported);
    
    __SSSLOGSTRING1("[SSS]    RSSSettings::IsALSSupported ppAlsValue value: %d", aPpAlsValue );
    __SSSLOGSTRING1("[SSS]    RSSSettings::IsALSSupported simAlsValue value:  %d", aSimAlsValue );
    __SSSLOGSTRING1("[SSS]    RSSSettings::IsALSSupported alsCSPSupport value: %d", aAlsCspSupport );
    __SSSLOGSTRING1("[SSS]    RSSSettings::IsALSSupported alsCSPError value: %d", aAlsCspError );
    
    // When PP ALS is on and used SIM card suppports CSP ALS then alsCSPSupport returns EFALSE and alsCSPError KErrNotSupported but ALS functionality
    // needs to be shown therefore KErrNone is returned.
    if ( aPpAlsValue && !aSimAlsValue && aAlsCspSupport == FALSE && aAlsCspError == KErrNotSupported  )
        {
        error = KErrNone;
        }          
    else if ( aPpAlsValue && aSimAlsValue && aAlsCspSupport == FALSE )
        {
        error = KErrNone;
        }
    else if ( aPpAlsValue && aSimAlsValue && aAlsCspSupport == TRUE  )
        {
        error = KErrNone;
        }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    else if ( aPpAlsValue && !aSimAlsValue && aAlsCspSupport == FALSE  )
        {
        error = KErrNone;
        }
    else if ( aPpAlsValue && !aSimAlsValue && aAlsCspSupport == TRUE  )
        {
        error = KErrNone;
        }
    else if ( !aPpAlsValue && aSimAlsValue && aAlsCspSupport == TRUE  )
        {
        error = KErrNone;
        }
    else if ( !aPpAlsValue && aSimAlsValue && aAlsCspSupport == FALSE  )
        {
        error = KErrNotSupported;
        }
    else if ( !aPpAlsValue && !aSimAlsValue && aAlsCspSupport == TRUE  )
        {
        error = KErrNone;
        }
                            
    return error;   
    }
    
// ---------------------------------------------------------
// RSSSettings::Set
// ---------------------------------------------------------
//
EXPORT_C TInt RSSSettings::Set( TSSSettingsSetting aSetting, TInt aValue )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::Set");
    __ASSERT_DEBUG( iData, Panic( SSSettingsPanicOpenNotDone ) );
    if ( !iData )
        {
        return KErrBadHandle;
        }

    __ASSERT_DEBUG( aSetting <= KSSSettingsLastSetting, 
                    Panic( SSSettingsPanicInvalidRequest ) );

    TInt error(KErrNone);

    switch ( aSetting )
        {
        case ESSSettingsCug:
            {
            if ( iData->iCUGDefault == ESSSettingsAltCugDefault )
                {
                if ((( aValue < ESSSettingsCugSuppress ) || 
                    ( aValue > ESSSettingsCugMax )) &&
                    ( aValue != iData->iCUGDefault ))
                    {
                    error = KErrArgument;
                    break;
                    }
                }
            else
                {
                if (( aValue < ESSSettingsCugSuppress ) || 
                    ( aValue > ESSSettingsCugMax ))
                    {
                    error = KErrArgument;
                    break;
                    }
                }
            if ( error == KErrNone )
                {
                error = SetCug( aValue, iData->iCUGDefault );
                }
            }
            break;
        case ESSSettingsClir:
            if (( aValue < ESSSettingsClirNetworkDefault ) || 
                ( aValue > ESSSettingsClirExplicitSuppress ))
                {
                error = KErrArgument;
                break;
                }
            error = iData->iRepository->Set(
                KSettingsCLIR,
                aValue );
            break;
        case ESSSettingsAls:
            {
            // ESSSettingsAlsBlockingNotSupported is not allowed to write
            if (( aValue < ESSSettingsAlsBlockingOff ) || 
                ( aValue > ESSSettingsAlsAlternate ))
                {
                error = KErrArgument;
                break;
                }

            CSSSettingsAlsNotifier* als = iData->iNotifier->GetAlsHandler();
            if ( als )
                {
                error = als->SetAlsValue( (TSSSettingsAlsValue)aValue );   
                }
            else
                {
                error = KErrNotSupported;
                }
            break;
            }
        case ESSSettingsAlsBlocking:
            switch ( aValue )
                {
                case ESSSettingsAlsBlockingOn:
                    error = iData->iCustomPhone->SetAlsBlocked( 
                        RMmCustomAPI::EActivateBlock );
                    break;
                case ESSSettingsAlsBlockingOff:
                    error = iData->iCustomPhone->SetAlsBlocked( 
                        RMmCustomAPI::EDeactivateBlock );
                    break;
                default:
                    error = KErrArgument;
                    break;
                }
            break;
        case ESSSettingsDefaultCug:
            return KErrNotSupported;
        default:
            error = KErrArgument;
            break;
        }

    if ( error == KErrNone )
        {
        // If old request is still waiting, force it to end.
        if (( iData->iCallBackSetting != KErrBadDescriptor ) ||
            ( iData->iCallBackValue != KErrBadDescriptor ))
            {
            iData->iCallBack->Cancel();
            InformCallBack( this );
            }

        // activate call back
        iData->iCallBackSetting = aSetting;
        iData->iCallBackValue = aValue;
        iData->iCallBack->CallBack();
        }
    __SSSLOGSTRING("[SSS] <--RSSSettings::Set");
    return error;
    }

// ---------------------------------------------------------
// RSSSettings::HandleSIMChanged
// ---------------------------------------------------------
//
EXPORT_C TInt RSSSettings::HandleSIMChanged()
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::HandleSIMChanged");
    __ASSERT_DEBUG( iData, Panic( SSSettingsPanicOpenNotDone ) );
    if ( !iData )
        {
        return KErrBadHandle;
        }
    TInt error = Set( ESSSettingsCug, iData->iCUGDefault );
    if ( error != KErrNone )
        {
        return error;
        }
    else
        {
        // Reset also the previous Cug value.
        error = ResetPreviousCugValue();
        if ( error != KErrNone )
            {
            return error;
            }
        }

    error = Set( ESSSettingsClir, ESSSettingsClirNetworkDefault );
    if ( error != KErrNone )
        {
        return error;
        }

    // Set als to primary only if it's stored in Shared Data.
    CSSSettingsAlsNotifier* als = iData->iNotifier->GetAlsHandler();
    if ( als )
        {
        TBool simAls(EFalse);
        TBool ppAls(EFalse);
        TBool cspAls(EFalse);
        TInt cspError(KErrNone);
        als->GetAlsSupport( ppAls, simAls, cspAls, cspError );
        __SSSLOGSTRING2("[SSS]     RSSSettings::HandleSIMChanged: ppAls: %d, simAls: %d", ppAls, simAls );
        if ( !simAls )
            {
            error = Set( ESSSettingsAls, ESSSettingsAlsPrimary );
            }
        }
    __SSSLOGSTRING("[SSS] <--RSSSettings::HandleSIMChanged");
    return error;
    }

// ---------------------------------------------------------
// RSSSettings::ExistsObserver
// ---------------------------------------------------------
//
TBool RSSSettings::ExistsObserver( TSSSettingsSetting aSetting, 
                    MSSSettingsObserver& aObserver )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::ExistsObserver");
    if ( !iData )
        {
        return EFalse;
        }

    __ASSERT_ALWAYS( aSetting <= KSSSettingsLastSetting, 
                    Panic( SSSettingsPanicInvalidRequest ) );

    TInt index = iData->iObservers[aSetting].Find( &aObserver );
    if ( index == KErrNotFound )
        {
        return EFalse;
        }

    __SSSLOGSTRING("[SSS] <--RSSSettings::ExistsObserver");
    return ETrue;
    }

// ---------------------------------------------------------
// RSSSettings::FindOneObserver
// ---------------------------------------------------------
//
MSSSettingsObserver* RSSSettings::FindOneObserver() const
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::FindOneObserver");
    if ( iData == NULL )
        {
        return NULL;
        }

    for ( TInt settingIndex = 0 ; 
          settingIndex < iData->iObservers.Count() ; 
          settingIndex++ )
        {
        RSSSettingsObserverArray& array = iData->iObservers[settingIndex];
        if ( array.Count() )
            {
            return array[0];
            }
        }

    __SSSLOGSTRING("[SSS] <--RSSSettings::FindOneObserver");
    return NULL;
    }

// ---------------------------------------------------------
// RSSSettings::GetCug
// ---------------------------------------------------------
//
TInt RSSSettings::GetCug( TInt& aValue ) const
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::GetCug");
    TInt value(KErrNone);
    TInt error(KErrNone);
    error = iData->iRepository->Get(
        KSettingsCUG,
        value );

    if ( error == KErrNone )
        {
        aValue = value;
        }

    __SSSLOGSTRING1("[SSS]     RSSSettings::GetCug: value: %d", value );
    __SSSLOGSTRING("[SSS] <--RSSSettings::GetCug");
    return error;
    }

// ---------------------------------------------------------
// RSSSettings::GetDefaultCugL
// ---------------------------------------------------------
//
TInt RSSSettings::GetDefaultCugL( TInt& aValue ) const
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::GetDefaultCugL");
    TInt value(0);
    TBool useDefault(EFalse);
    TInt error(KErrNone);
    CRepository* repository = CRepository::NewL( KCRUidTelSrvVariation );
    error = repository->Get(
            KTelSrvVariationFlags,
            value );
    delete repository;
    repository = NULL;
    __SSSLOGSTRING1("[SSS]--> RSSSettings::GetDefaultCugL: value: %d", value);
    
    if ( error == KErrNotFound )
        {
        useDefault = ETrue;
        }
    else if ( error != KErrNone )
        {
        return error;
        }

    if (( useDefault ) || 
       !( value & KTelephonyLVFlagStandardCugIndexing ))
        {
        value = ESSSettingsCugDefault;
        }
    else
        {
        value = ESSSettingsAltCugDefault;
        }
    //Return directly (i.e. no shifting).
    aValue = value;

    __SSSLOGSTRING1("[SSS]--> RSSSettings::GetDefaultCugL: value: %d", value);
    __SSSLOGSTRING("[SSS] <--RSSSettings::GetDefaultCugL");
    return error;
    }

// ---------------------------------------------------------
// RSSSettings::SetCug
// Reads existing CUG value from shared data. If new CUG
// value is between 1 and 32767 (or between 0 and 32767), 
// value is to shared data.
// Change also the previous cug to be the old current one.
// ---------------------------------------------------------
//
TInt RSSSettings::SetCug( const TInt aValue, const TInt aCugDefault ) const
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::SetCug");
    TInt value(KErrNone);
    TInt error(KErrNone);
    error = iData->iRepository->Get( KSettingsCUG, value );
    __SSSLOGSTRING1("[SSS]--> RSSSettings::SetCug: value: %d", value);
    if ( error == KErrNone )
        {
        TInt previousCug(value);

        // CUG default must be saved as shared data default, to support
        // different platform versions. Do not write default Cug again to 
        // Central Repository, if already exists.
        if ( ( aValue != aCugDefault ) ||
             ( ( aValue == aCugDefault ) && ( previousCug != aCugDefault ) ) )
            {
            // Write the current cug value to Central Repository.
            error = iData->iRepository->Set( KSettingsCUG, aValue );
            __SSSLOGSTRING1("[SSS]--> RSSSettings::SetCug: aValue: %d", aValue);
            if ( error == KErrNone )
                {
                if ( ( previousCug != ESSSettingsCugSuppress ) &&
                     ( previousCug != aCugDefault ) )
                    {
                    // Write previous cug value to Central Repository.
                    // Default or Suppress value can not be previousCug.
                    error = iData->iRepository->Set( 
                        KSettingsPreviousCUGIndex,
                        previousCug );
                    __SSSLOGSTRING1("[SSS]--> RSSSettings::SetCug: previousCug: %d", previousCug);
                    }
                }
            }
        }
    __SSSLOGSTRING("[SSS] <--RSSSettings::SetCug");
    return error;
    }

// ---------------------------------------------------------
// RSSSettings::PreviousCugValue
// ---------------------------------------------------------
//
EXPORT_C TInt RSSSettings::PreviousCugValue( TInt& aPreviousValue )
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::PreviousCugValue");
    __ASSERT_DEBUG( iData, Panic( SSSettingsPanicOpenNotDone ) );
    if ( !iData )
        {
        return KErrBadHandle;
        }
    TInt value(KErrNone);
    TInt error = iData->iRepository->Get( 
        KSettingsPreviousCUGIndex,
        value );

    if ( error == KErrNone )
        {
        aPreviousValue = value;
        }

    __SSSLOGSTRING1("[SSS]--> RSSSettings::PreviousCugValue: value: %d", value);
    __SSSLOGSTRING("[SSS] <--RSSSettings::PreviousCugValue");
    return error;
    }

// ---------------------------------------------------------
// RSSSettings::ResetPreviousCugValue
// Reset PreviousCugValue
// ---------------------------------------------------------
//
EXPORT_C TInt RSSSettings::ResetPreviousCugValue()
    {
    __SSSLOGSTRING("[SSS]--> RSSSettings::ResetPreviousCugValue");
    __ASSERT_DEBUG( iData, Panic( SSSettingsPanicOpenNotDone ) );
    if ( !iData )
        {
        return KErrBadHandle;
        }

    TInt value(KErrNone);
    TInt error(KErrNone);
    error = iData->iRepository->Get(
        KSettingsPreviousCUGIndex,
        value );
    __SSSLOGSTRING1("[SSS]--> RSSSettings::PreviousCugValue; before setting: value: %d", value);
    if ( error == KErrNone )
        {
        // Set cug to default one.
        value = iData->iCUGDefault;
        error = iData->iRepository->Set(
            KSettingsPreviousCUGIndex,
            value );
        __SSSLOGSTRING1("[SSS]--> RSSSettings::PreviousCugValue; after setting: value: %d", value);
        }
    __SSSLOGSTRING("[SSS] <--RSSSettings::ResetPreviousCugValue");
    return error;
    }

// ---------------------------------------------------------
// RSSSettings::HandleRefresh
// ---------------------------------------------------------
//
EXPORT_C TInt RSSSettings::HandleRefresh()
    {
    __ASSERT_DEBUG( iData, Panic( SSSettingsPanicOpenNotDone ) );
    if ( !iData )
        {
        return KErrBadHandle;
        }

    return iData->iNotifier->HandleRefresh();
    }

// ---------------------------------------------------------
// RSSSettings::IsValueValidCugIndex
// ---------------------------------------------------------
//
EXPORT_C TBool RSSSettings::IsValueValidCugIndex( const TInt& aValue ) const
    {
    __ASSERT_DEBUG( iData, Panic( SSSettingsPanicOpenNotDone ) );
    if ( !iData )
        {
        return KErrBadHandle;
        }

    TBool validCugIndex = !(( aValue > ESSSettingsCugMax ) ||
                            ( aValue < ESSSettingsCugMin ));

    if ( iData->iCUGDefault != ESSSettingsCugDefault )
        {
        validCugIndex = validCugIndex || 
                        aValue == ESSSettingsAltCugMin;
        }
    return validCugIndex;
    }

// ================= OTHER EXPORTED FUNCTIONS ==============
// Panic
GLREF_C void Panic( TSSSettingsPanic aPanic )
    {
    User::Panic( KPanicCat, aPanic );
    }
    
//  End of File 
