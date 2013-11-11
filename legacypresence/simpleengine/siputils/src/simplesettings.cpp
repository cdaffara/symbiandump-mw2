/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    SIMPLE engine settings
*
*/




// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include <centralrepository.h>
#include <pressettingsapi.h>
#include "simpleenginedomaincrkeys.h"
#include "simpleengineprivatecrkeys.h"

#ifdef _DEBUG
#include <bautils.h>
#include "simpledebugutils.h"
#endif

#include "simplesettings.h"

#ifdef _DEBUG
_LIT ( KSimpleIniFile, "\\system\\data\\simple.ini");
_LIT8 ( KSimpleExpiryPuhlish, "expiry_publish");
_LIT8 ( KSimpleExpiryWatcher, "expiry_watcher");
_LIT8 ( KSimpleExpiryWinfo, "expiry_winfo");
_LIT8 ( KSimpleExpiryAPI, "expiry_api");
_LIT8 ( KSimpleObjLimit, "obj_limit");
_LIT8 ( KSimpleThrottle, "throttle");
_LIT8 ( KSimpleMaxSubs, "max_subs");
#endif

/** default API expiry time */
const TInt KMyDefaultAPIExpiry = 60;
/** default SIP refresh expiry time */
const TInt KMyDefaultSIPExpiry = 43200;
/** default OTA settings value */
const TInt KOTASettingsDefault = 0;

// ================= MEMBER FUNCTIONS =======================




//**********************************
// TSimpleSettings
//**********************************

// ---------------------------------------------------------
// TSimpleSettings::TSimpleSettings
// ---------------------------------------------------------
//
TSimpleSettings::TSimpleSettings( )
    :
    iExpiryPuhlish( KMyDefaultSIPExpiry ),
    iExpiryWatcher( KMyDefaultSIPExpiry ),
    iExpiryWinfo( KMyDefaultSIPExpiry ),
    iExpiryApi( KMyDefaultAPIExpiry ),
    iObjLimit(KOTASettingsDefault),
    iThrottle(KOTASettingsDefault),
    iMaxSubscriptions(KOTASettingsDefault),
    iSipProfileId(KOTASettingsDefault)
    {}

TUint TSimpleSettings::ExpiryPuhlish()
    {
    return iExpiryPuhlish;
    }

TUint TSimpleSettings::ExpiryWatcher()
    {
    return iExpiryWatcher;
    }

TUint TSimpleSettings::ExpiryWinfo()
    {
    return iExpiryWinfo;
    }

TUint TSimpleSettings::ExpiryApi()
    {
    return iExpiryApi;
    }

TInt TSimpleSettings::ObjLimit()
    {
    return iObjLimit;
    }

TInt TSimpleSettings::Throttle()
    {
    return iThrottle;
    }

TInt TSimpleSettings::MaxSubscriptions()
    {
    return iMaxSubscriptions;
    }

TInt TSimpleSettings::SipProfileId()
    {
    return iSipProfileId;
    }

// ---------------------------------------------------------
// TSimpleSettings::ReadCentRepSettings
// ---------------------------------------------------------
//
void TSimpleSettings::ReadCentRepSettings( )
    {
    // Read Central Repository settings
    CRepository* repository = NULL;
    TRAP_IGNORE( repository = CRepository::NewL( KCRUIDSimpleVariation ));
    if ( repository )
        {
        TRAP_IGNORE( iExpiryPuhlish = GetCenRepIntValueL(
            repository, KSimplePublishRefresh ));
        TRAP_IGNORE( iExpiryWatcher = GetCenRepIntValueL(
            repository, KSimpleSubscribeRefresh ));
        TRAP_IGNORE( iExpiryWinfo = GetCenRepIntValueL(
            repository, KSimpleSubscribeWinfoRefresh ));
        TRAP_IGNORE( iExpiryApi = GetCenRepIntValueL(
            repository, KSimpleExpiryTime));
        delete repository;
        repository = NULL;
#ifdef _DEBUG
#ifdef __WINS__
        TSimpleLogger::Log(_L("CentRep iExpiryPuhlish=%d iExpiryWatcher=%d iExpiryWinfo=%d iExpiryAPI=%d"),
        iExpiryPuhlish, iExpiryWatcher, iExpiryWinfo, iExpiryApi);
#endif
#endif
        }
    }

// ---------------------------------------------------------
// TSimpleSettings::ReadOTASettings
// ---------------------------------------------------------
//
void TSimpleSettings::ReadOTASettingsL( TInt aSetId )
    {
    // Read OTA settings using PresSettingsApi

    TPresSettingsSet set;
    TInt error( KErrNone );
    error = PresSettingsApi::SettingsSetL( aSetId, set );
    User::LeaveIfError( error );

    // convert -1 values (undefined) to default values
    iObjLimit = set.iObjectSize > KOTASettingsDefault ?
        set.iObjectSize : KOTASettingsDefault;
    iMaxSubscriptions = set.iMaxSubscriptions > KOTASettingsDefault ?
        set.iMaxSubscriptions : KOTASettingsDefault;
    iThrottle = set.iPublicationInt > KOTASettingsDefault ?
        set.iPublicationInt : KOTASettingsDefault;
    iSipProfileId = set.iSipProfile > KOTASettingsDefault ?
        set.iSipProfile : KOTASettingsDefault;

#ifdef _DEBUG
#ifdef __WINS__
        TSimpleLogger::Log(_L("OTAsettings iObjLimit=%d iThrottle=%d iMaxSubs=%d iSip=%d"),
        iObjLimit, iThrottle, iMaxSubscriptions, iSipProfileId );
#endif
#endif

    }

// ---------------------------------------------------------
// TSimpleSettings:GetCenRepIntValueL
// ---------------------------------------------------------
//
TInt TSimpleSettings::GetCenRepIntValueL(
    CRepository* aRepository,
    TUint32 aId )
    {
    TInt value( 0 );
    User::LeaveIfError( aRepository->Get( aId, value ) );
    return value;
    }

// ---------------------------------------------------------
// TSimpleSettings::ReadIniFileL
// ---------------------------------------------------------
//
#ifdef _DEBUG

void TSimpleSettings::ReadIniFileL( RFs&  aFs )
    {
    // Set few values without reading them from ini file
    CSimpleSettingFile* configFile = CSimpleSettingFile::NewL( aFs );
    CleanupStack::PushL( configFile );  // *****
    // Search current drive
    TFileName path;
    Dll::FileName( path );
    TDriveName drive( TParsePtrC( path ).Drive( ) );

    // In emulator we try to find from c drive
    // in hardware we try to find from current drive and then from RAM.
#ifdef __WINS__
    TFileName fileN( KSimpleIniFile);
#else
    TFileName fileN( drive );
    fileN.Append( KSimpleIniFile );
#endif

    TInt err = 0;

    // Open temporary config file
    TRAP( err, configFile->OpenL( fileN ));
    if ( err && drive.CompareF( _L("c:") ))
        {
        err = KErrNone;
        fileN = _L("c:");
        fileN.Append( KSimpleIniFile );
        TRAP( err, configFile->OpenL( fileN ));
        }
    if ( err )
        {
        err = KErrNone;
        TSimpleLogger::Log(_L("Config file not found ***"));
        User::Leave( KErrNotFound );
        }
    else
        {
        TSimpleLogger::Log(_L("Config file found ***"));
        }

    TUint32 myUint = 0;
    TPtrC8 myPtr;
    TBuf<8> myBuffer;

    // read expiry times
    TRAP( err, myPtr.Set( configFile->KeyValueL(KSimpleExpiryPuhlish)));
    if ( !err )
        {
        myBuffer.Copy( myPtr );
        TLex lex;
        lex.Assign( myBuffer );
        lex.Val(myUint, EDecimal);
        iExpiryPuhlish = (TInt) myUint;
        }

    TRAP( err, myPtr.Set( configFile->KeyValueL(KSimpleExpiryWatcher)));
    if ( !err )
        {
        myBuffer.Copy( myPtr );
        TLex lex;
        lex.Assign( myBuffer );
        lex.Val(myUint, EDecimal);
        iExpiryWatcher = (TInt) myUint;
        }

    TRAP( err, myPtr.Set( configFile->KeyValueL(KSimpleExpiryWinfo)));
    if ( !err )
        {
        myBuffer.Copy( myPtr );
        TLex lex;
        lex.Assign( myBuffer );
        lex.Val(myUint, EDecimal);
        iExpiryWinfo = (TInt) myUint;
        }

    TRAP( err, myPtr.Set( configFile->KeyValueL(KSimpleExpiryAPI)));
    if ( !err )
        {
        myBuffer.Copy( myPtr );
        TLex lex;
        lex.Assign( myBuffer );
        lex.Val(myUint, EDecimal);
        iExpiryApi = (TInt) myUint;
        }

    TRAP( err, myPtr.Set( configFile->KeyValueL(KSimpleObjLimit)));
    if ( !err )
        {
        myBuffer.Copy( myPtr );
        TLex lex;
        lex.Assign( myBuffer );
        lex.Val(myUint, EDecimal);
        iObjLimit = (TInt) myUint;
        }

    TRAP( err, myPtr.Set( configFile->KeyValueL(KSimpleThrottle)));
    if ( !err )
        {
        myBuffer.Copy( myPtr );
        TLex lex;
        lex.Assign( myBuffer );
        lex.Val(myUint, EDecimal);
        iThrottle = (TInt) myUint;
        }

    TRAP( err, myPtr.Set( configFile->KeyValueL(KSimpleMaxSubs)));
    if ( !err )
        {
        myBuffer.Copy( myPtr );
        TLex lex;
        lex.Assign( myBuffer );
        lex.Val(myUint, EDecimal);
        iMaxSubscriptions = (TInt) myUint;
        }
    CleanupStack::PopAndDestroy(configFile);

    // debug logging
    TSimpleLogger::Log(_L("ReadFile iExpiryPuhlish=%d iExpiryWatcher=%d iExpiryWinfo=%d iExpiryAPI=%d"),
    iExpiryPuhlish, iExpiryWatcher, iExpiryWinfo, iExpiryApi);
    TSimpleLogger::Log(_L("ReadFile iObjLimit=%d iThrottle=%d iMaxSubs=%d"),
    iObjLimit, iThrottle, iMaxSubscriptions );
    }

#endif // _DEBUG


//  End of File

