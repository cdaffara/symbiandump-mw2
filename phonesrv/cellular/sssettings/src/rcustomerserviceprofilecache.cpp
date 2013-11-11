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
* Description:  Customer service profile cache
*
*/


// INCLUDE FILES
#include    <bldvariant.hrh> // for feature definitions
#include    <etel.h>  
#include    <sssettingsprivatepskeys.h> 
#include    <centralrepository.h>
#include    <mmtsy_names.h>
#include    <featmgr.h>
      
#include    "rcustomerserviceprofilecache.h" 
#include    "sssettingslogger.h" 

// CONSTANTS
#define KSSSettingsTSYName KMmTsyModuleName
const TInt KCspCacheTSYNameMaxLength( 80 );
const TInt KCspCacheETelRetryCount(8);
const TInt KCspCacheETelRetryTimeout(100000);
const TInt KCspCacheNoFile(99999);
_LIT_SECURITY_POLICY_C1(KSSSettingReadPolicy, ECapabilityReadDeviceData);
_LIT_SECURITY_POLICY_C1(KSSSettingWritePolicy, ECapabilityWriteDeviceData);


// ==================== LOCAL FUNCTIONS ====================


static TInt CspCacheDoOpenEtelConnection( RTelServer& aEtel, RPhone& aPhone )
    {
    TInt phones(0);
    if ( aEtel.EnumeratePhones( phones) == KErrNone )
        {
        for ( TInt p = 0 ; p < phones ; p++ )
            {
            TBuf<KCspCacheTSYNameMaxLength> tsyName;
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


RCustomerServiceProfileCache::TCspCacheData::TCspCacheData():iCspFilePckg( 
    iCspFile )
    { 
    }


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
EXPORT_C RCustomerServiceProfileCache::RCustomerServiceProfileCache()
    {
    iData = NULL;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
EXPORT_C RCustomerServiceProfileCache::~RCustomerServiceProfileCache()
    {
    Close();
    }

// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::Open
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCustomerServiceProfileCache::Open()
    {    
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::Open");
    TRAPD( error, DoOpenL() );
    if ( error != KErrNone )
        {
        Close();
        }

    __SSSLOGSTRING1("[SSS]     RCustomerServiceProfileCache::Open: error: %d", error );
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::Open");
    return error;
    }

// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::Close
// -----------------------------------------------------------------------------
//
EXPORT_C void RCustomerServiceProfileCache::Close()
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::Close");
    if ( iData == NULL )
        {
        return;
        }

    iData->iPhone.Close();
    iData->iEtel.Close();
    
    delete iData;
    iData = NULL;

    delete iOldCspFile;
    iOldCspFile = NULL;

    FeatureManager::UnInitializeLib();
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::Close");
    }

// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::CspCallCompletion
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCustomerServiceProfileCache::CspCallCompletion( 
            RMobilePhone::TCspCallCompletion& aParams )
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::CspCallCompletion");
    TInt value(0);
    TInt error = DoGetValues( value, ESsCspGroup1 );
    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspCallCompletion: error: %d", error);

    if ( error == KErrNone )
        {
        //Read 8 bits are moved to correct place within a TInt.
        aParams = ( RMobilePhone::TCspCallCompletion ) 
            ( ( value >> 24 ) & 0xff );
        }

    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspCallCompletion: value: %d", value);
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::CspCallCompletion");
    return error;
    }

// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::CspCallOffering
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCustomerServiceProfileCache::CspCallOffering( 
            RMobilePhone::TCspCallOffering& aParams )
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::CspCallOffering");
    TInt value(0);
    TInt error = DoGetValues( value, ESsCspGroup1 );
    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspCallOffering: error: %d", error);

    if ( error == KErrNone )
        {
        //Read 8 bits are moved to correct place within a TInt.
        aParams = ( RMobilePhone::TCspCallOffering ) 
            ( ( value >> 16 ) & 0xff );
        }

    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspCallOffering: value: %d", value);
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::CspCallOffering");
    return error;
    }

// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::CspCallRestriction
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCustomerServiceProfileCache::CspCallRestriction( 
            RMobilePhone::TCspCallRestriction& aParams )
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::CspCallRestriction");
    TInt value(0);
    TInt error = DoGetValues( value, ESsCspGroup1 );
    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspCallRestriction: error: %d", error);

    if ( error == KErrNone )
        {
        //Read 8 bits are moved to correct place within a TInt.
        aParams = ( RMobilePhone::TCspCallRestriction )
            ( ( value >> 8 ) & 0xff );
        }

    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspCallRestriction: value: %d", value);
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::CspCallRestriction");
    return error;
    }

// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::CspCPHSTeleservices
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCustomerServiceProfileCache::CspCPHSTeleservices( 
            RMobilePhone::TCspCPHSTeleservices& aParams )
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::CspCPHSTeleservices");
    TInt value(0);
    TInt error = DoGetValues( value, ESsCspGroup1 );
    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspCPHSTeleservices: error: %d", error);

    if ( error == KErrNone )
        {
        //Read 8 bits are moved to correct place within a TInt.
        aParams = ( RMobilePhone::TCspCPHSTeleservices )
            ( value & 0xff );
        }

    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspCPHSTeleservices: value: %d", value);
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::CspCPHSTeleservices");
    return error;
    }

// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::HandleRefresh
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCustomerServiceProfileCache::HandleRefresh()
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::HandleRefresh");
    // Save the old CSP information.
    *iOldCspFile = iData->iCspFile;

    TRequestStatus status;
    TInt value(0);
    TInt valueExt(0);
    iData->iPhone.GetCustomerServiceProfile( status, iData->iCspFilePckg );
    User::WaitForRequest( status );
    TInt error = status.Int();
    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::HandleRefresh: GetCustomerServiceProfile: error: %d", error);

    if ( error == KErrNone )
        {
        // Read 8 bit groups are saved into one TInt. To make room for
        // all groups, the first read groups are shifted left.
        value = iData->iCspFile.iCallCompletionServices << 24;
        value += iData->iCspFile.iCallOfferingServices << 16;
        value += iData->iCspFile.iCallRestrictionServices << 8;
        value += iData->iCspFile.iCphsTeleservices;

        // Read 8 bit groups are saved into one TInt. To make room for
        // all groups, the first read groups are shifted left.
        valueExt = iData->iCspFile.iTeleservices << 8;        
        __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::HandleRefresh: valueExt1: %d", valueExt);
        valueExt += iData->iCspFile.iValueAddedServices;
        __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::HandleRefresh: valueExt2: %d", valueExt);
        }
    else
        {        
        value = KErrNotFound;
        valueExt = KErrNotFound;
        error = KErrNotSupported;
        }

    if ( error == KErrNone )
        {
        // Cache 2.
        error = DefineAndSetValue(
                KPSUidSSConfig,
                KSettingsCspCache2,
                valueExt );

        if ( error == KErrNone )
            {
            // Cache 1.
            error = DefineAndSetValue(
                    KPSUidSSConfig,
                    KSettingsCspCache1,
                    value );
            }
        }
    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::HandleRefresh: error: %d", error);
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::HandleRefresh");
    return error;
    }

// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::CspCPHSValueAddedServices
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCustomerServiceProfileCache::CspCPHSValueAddedServices( 
    RMobilePhone::TCspValueAdded& aParams )
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::CspCPHSValueAddedServices");
    TInt value(0);
    TInt error = DoGetValues( value, ESsCspGroup2 );
    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspCPHSValueAddedServices: error: %d", error);

    if ( error == KErrNone )
        {
        aParams = ( RMobilePhone::TCspValueAdded )( value & 0xff );
        }

    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspCPHSValueAddedServices: value: %d", value);
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::CspCPHSValueAddedServices");
    return error;
    }


// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::CspTeleServices

// Include  SM/CB, Reply path, Del. Conf, Prot. ID, Validity Period.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCustomerServiceProfileCache::CspTeleServices( 
    RMobilePhone::TCspTeleservices& aParams )
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::CspTeleServices");
    TInt value(0);
    TInt error = DoGetValues( value, ESsCspGroup2 );
    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspTeleServices: error: %d", error);

    if ( error == KErrNone )
        {
        // Read 8 bits are moved to correct place within a TInt.
        aParams = ( RMobilePhone::TCspTeleservices )
            ( ( value >> 8 ) & 0xff );
        }

    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::CspTeleServices: value: %d", value);
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::CspTeleServices");
    return error;
    }


// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::ChangedCspTable
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCustomerServiceProfileCache::ChangedCspTable( 
    TDes8& aChangedCsps,
    TDes8& aNewValues )
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::ChangedCspTable");
    TInt error(KErrNone);
    if ( !iData || !iOldCspFile )
        {
        // CspFiles did not exist.
        error = KErrCorrupt;
        }
    else 
        {
        // Set the changed CSP file, done with XORing the old and new CSP file.
        RMobilePhone::TMobilePhoneCspFileV1Pckg& cspFilePckg = 
            *reinterpret_cast< RMobilePhone::TMobilePhoneCspFileV1Pckg* >( 
            &aChangedCsps );
        RMobilePhone::TMobilePhoneCspFileV1& cspFile = cspFilePckg();

        cspFile.iCallOfferingServices    = ( TUint8 ) ( 
            iData->iCspFile.iCallOfferingServices ^ 
            iOldCspFile->iCallOfferingServices );

        cspFile.iCallRestrictionServices = ( TUint8 ) ( 
            iData->iCspFile.iCallRestrictionServices ^ 
            iOldCspFile->iCallRestrictionServices );

        cspFile.iOtherSuppServices       = ( TUint8 ) ( 
            iData->iCspFile.iOtherSuppServices ^ 
            iOldCspFile->iOtherSuppServices );

        cspFile.iCallCompletionServices  = ( TUint8 ) ( 
            iData->iCspFile.iCallCompletionServices ^ 
            iOldCspFile->iCallCompletionServices );

        cspFile.iTeleservices            = ( TUint8 ) ( 
            iData->iCspFile.iTeleservices ^ 
            iOldCspFile->iTeleservices );

        cspFile.iCphsTeleservices        = ( TUint8 ) ( 
            iData->iCspFile.iCphsTeleservices ^ 
            iOldCspFile->iCphsTeleservices );

        cspFile.iCphsFeatures            = ( TUint8 ) ( 
            iData->iCspFile.iCphsFeatures ^ 
            iOldCspFile->iCphsFeatures );

        cspFile.iNumberIdentServices     = ( TUint8 ) ( 
            iData->iCspFile.iNumberIdentServices ^ 
            iOldCspFile->iNumberIdentServices );

        cspFile.iPhase2PlusServices      = ( TUint8 ) ( 
            iData->iCspFile.iPhase2PlusServices ^ 
            iOldCspFile->iPhase2PlusServices );

        cspFile.iValueAddedServices      = ( TUint8 ) ( 
            iData->iCspFile.iValueAddedServices ^ 
            iOldCspFile->iValueAddedServices );


        // Set the new values.
        RMobilePhone::TMobilePhoneCspFileV1Pckg& newCspFilePckg = 
            *reinterpret_cast< RMobilePhone::TMobilePhoneCspFileV1Pckg* >( 
            &aNewValues );
        RMobilePhone::TMobilePhoneCspFileV1& newCpFile = newCspFilePckg();

        newCpFile = iData->iCspFile;
        }

    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::ChangedCspTable");
    return error;
    }


// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::DoOpenL
// -----------------------------------------------------------------------------
//
void RCustomerServiceProfileCache::DoOpenL()
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::DoOpenL");
    if ( iData )
        {
        Close();
        iData = NULL;
        }

    FeatureManager::InitializeLibL();

    iData = new ( ELeave ) TCspCacheData;
    
    // Retry ETel connection for a few times:
    TInt error(KErrNone);
    for ( TInt i = 0 ; i < KCspCacheETelRetryCount ; i++ )
        {
        error = iData->iEtel.Connect();
        if ( error == KErrNone )
            {
            break;
            }
        User::After( KCspCacheETelRetryTimeout );
        }
    User::LeaveIfError( error );


    error = iData->iEtel.LoadPhoneModule( KSSSettingsTSYName );
    if (( error != KErrNone ) && ( error != KErrAlreadyExists ))
        {
        User::Leave( error );
        }
    User::LeaveIfError( 
        CspCacheDoOpenEtelConnection( iData->iEtel, iData->iPhone ) );

    // Create and reset old CSP file variable.
    iOldCspFile = new ( ELeave ) RMobilePhone::TMobilePhoneCspFileV1;
    DoResetOldCspFile();
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::DoOpenL");
    }


// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::DoGetValues
// -----------------------------------------------------------------------------
//
TInt RCustomerServiceProfileCache::DoGetValues( TInt& aValue, TSsGroups aGroup )
    {
    __SSSLOGSTRING("[SSS]--> RCustomerServiceProfileCache::DoGetValues");
    TInt value(0);
    TInt error(KErrNone);
    if ( aGroup == ESsCspGroup1 )
        {
        error = RProperty::Get( KPSUidSSConfig, KSettingsCspCache1, value );
        __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::DoGetValues: ESsCspGroup1 value: %d", value);
        __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::DoGetValues: error: %d", error);
        }
    else
        {
        error = RProperty::Get( KPSUidSSConfig, KSettingsCspCache2, value );
        __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::DoGetValues: ESsCspGroup2 value: %d", value);
        __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::DoGetValues: error: %d", error);
        }
    // KErrNotFound if Property has not been defined.
    if ( error == KErrNotFound && aGroup == ESsCspGroup1 )
        {
        error = DefineAndSetValue( KPSUidSSConfig, KSettingsCspCache1, value );
        }
    else if ( error == KErrNotFound && aGroup == ESsCspGroup2 )
        {
        error = DefineAndSetValue( KPSUidSSConfig, KSettingsCspCache2, value ); 
        }

    if ( error == KErrNone )
        {
        if ( value != KCspCacheNoFile ) 
            {   
            __SSSLOGSTRING("[SSS] ---> GetCustomerServiceProfile");             
            TRequestStatus status;
            // GetCustomerServiceProfile status returns KErrNotFound if CSP product profile is OFF.
            iData->iPhone.GetCustomerServiceProfile(
               status ,
               iData->iCspFilePckg );
            User::WaitForRequest( status );
            error = status.Int();
            __SSSLOGSTRING1("[SSS] <--- GetCustomerServiceProfile: error: %d", error); 
             
            if (( error == KErrNotReady ) || ( error == KErrServerBusy ))
                {
                return error;
                }  
            }
        else // CSP has been read return value, if value is KCspCacheNoFile then return KErrNotSupported.
            {
            __SSSLOGSTRING("[SSS] CSP file cannot be found from SIM"); 
            return KErrNotSupported;    
            }

        if ( error == KErrNone )
            {
            if ( aGroup == ESsCspGroup1 )
                {
                // Read 8 bit groups are saved into one TInt. To make room for
                // all groups, the first read groups are shifted left.
                // All the 8 bit data is available in the below keys.

                // currently only CW supported on the UI level
                value = iData->iCspFile.iCallCompletionServices << 24;
                // currently CFU, CFB, CFNRy, CFNRc are supported
                value += iData->iCspFile.iCallOfferingServices << 16;
                // complete barring is supported
                value += iData->iCspFile.iCallRestrictionServices << 8;
                // contains ALS, 1-bit
                value += iData->iCspFile.iCphsTeleservices;

                __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::DoGetValues: ESsCspGroup1; value: %d", value);
                }
            else
                {
                // Read 8 bit groups are saved into one TInt. To make room for
                // all groups, the first read groups are shifted left.

                // supports SM/CB, Reply path, Del.Conf, Prot.ID, Validity Period
                value = iData->iCspFile.iTeleservices << 8;
                // currently PLMN MODE is supported in UI
                value += iData->iCspFile.iValueAddedServices;
                __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::DoGetValues: ESsCspGroup2; value: %d", value);
                }
            aValue = value;
            }
        else
            {
            value = KCspCacheNoFile;
            error = KErrNotSupported;
            }                        
        }
        
        if (( error == KErrNone ) || ( error == KErrNotSupported ))
            {
            TInt defError(0);
            if ( aGroup == ESsCspGroup1 )
                {
                __SSSLOGSTRING("[SSS]   DefineAndSetValue -> KSettingsCspCache1");
                defError = DefineAndSetValue( KPSUidSSConfig, KSettingsCspCache1, value );
                }
            else
                {
                __SSSLOGSTRING("[SSS]   DefineAndSetValue -> KSettingsCspCache2");
                defError = DefineAndSetValue( KPSUidSSConfig, KSettingsCspCache2, value );
                }
                
            // Do not set error if defError is KErrNone. If Define/Set failed then error value is set.
            if  ( defError != KErrNone )
                {
                error = defError;
                }
            }
        
    __SSSLOGSTRING1("[SSS]    RCustomerServiceProfileCache::DoGetValues: error: %d", error);
    __SSSLOGSTRING("[SSS] <-- RCustomerServiceProfileCache::DoGetValues");
    return error;
    }

// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::DoResetOldCspFile
// -----------------------------------------------------------------------------
//
void RCustomerServiceProfileCache::DoResetOldCspFile()
    {
    if ( iOldCspFile )
        {
        iOldCspFile->iCallOfferingServices = 0;
        iOldCspFile->iCallRestrictionServices = 0;
        iOldCspFile->iOtherSuppServices = 0;
        iOldCspFile->iCallCompletionServices = 0;
        iOldCspFile->iTeleservices = 0;
        iOldCspFile->iCphsTeleservices = 0;
        iOldCspFile->iCphsFeatures = 0;
        iOldCspFile->iNumberIdentServices = 0;
        iOldCspFile->iPhase2PlusServices = 0;
        iOldCspFile->iValueAddedServices = 0;
        }
    }

// -----------------------------------------------------------------------------
// CustomerServiceProfileCache::DefineAndSetValue
// -----------------------------------------------------------------------------
//
TInt RCustomerServiceProfileCache::DefineAndSetValue( TUid aCategory, 
        TUint aKey, TInt aValue )
    {
    TInt err = RProperty::Define( 
            aCategory, aKey,
            RProperty::EInt, KSSSettingReadPolicy,
            KSSSettingWritePolicy );
        
    if ( err == KErrNone || err == KErrAlreadyExists)
        {
        err = RProperty::Set( aCategory, aKey, aValue );        
        }
    return err;
    }

//  End of File  
