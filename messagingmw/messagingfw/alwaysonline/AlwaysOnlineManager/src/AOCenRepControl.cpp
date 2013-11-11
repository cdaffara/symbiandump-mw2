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
* Description: AlwaysOnline server Central Repository controller
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include <CoreApplicationUIsSDKCRKeys.h>   // Offline key

#include "AOCenRepControl.h"
#include "AOInternalCRKeys.h"
#include "AlwaysOnlineManagerLogging.h"

// EXTERNAL DATA STRUCTURES
// EXTERNAL FUNCTION PROTOTYPES
// CONSTANTS
const TUint32 KDisabledPluginsBaseKey = 0x00000100;
const TUint32 KDisabledPluginsLastKey = 0x000001FF;
const TUint32 KCoreAppUiRepository = 0x101F876C;

#ifdef _DEBUG   // Just to make compiler happy!
_LIT( KAOSetPanic, "AO cenrep set" );
_LIT( KAOGetPanic, "AO cenrep get" );
_LIT( KAODelPanic, "AO cenrep del" );
#endif

// MACROS
// LOCAL CONSTANTS AND MACROS
// MODULE DATA STRUCTURES
// LOCAL FUNCTION PROTOTYPES
// FORWARD DECLARATIONS

// ============================ MEMBER FUNCTIONS ===============================

// ----------------------------------------------------------------------------
// CAOCenRepControl::CAOCenRepControl()
// ----------------------------------------------------------------------------
//
CAOCenRepControl::CAOCenRepControl( CAlwaysOnlineManager& aCallBack ) : 
    iCallBack( aCallBack )
    {
    KAOMANAGER_LOGGER_FN1("CAOCenRepControl::CAOCenRepControl()");
    KAOMANAGER_LOGGER_FN2("CAOCenRepControl::CAOCenRepControl()");
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::~CAOCenRepControl()
// ----------------------------------------------------------------------------
//
CAOCenRepControl::~CAOCenRepControl()
    {
    if ( iCoreAppUiCenRepNotifier )
        {
        iCoreAppUiCenRepNotifier->StopListening();
        }
    
    delete iCoreAppUiCenRepNotifier;
    iCoreAppUiCenRepNotifier = NULL;
    
    delete iCoreAppUiCenRepSession;    
    iCoreAppUiCenRepSession = NULL;
    
    delete iAoServerCenRepSession;
    iAoServerCenRepSession = NULL;
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::ConstructL()
// ----------------------------------------------------------------------------
//
void CAOCenRepControl::ConstructL()
    {
    // Create Core App Ui Central Repository session and notifier
    CreateCoreAppUiCenRepSessionL();
    
    // Create AoServer Central Repository session
    CreateAoServerCenRepSessionL();
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::NewL()
// ----------------------------------------------------------------------------
//
CAOCenRepControl* CAOCenRepControl::NewL( CAlwaysOnlineManager& aCallBack )
    {
    CAOCenRepControl* self = NewLC( aCallBack );
    CleanupStack::Pop( self );

    return self;
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::NewLC()
// ----------------------------------------------------------------------------
//
CAOCenRepControl* CAOCenRepControl::NewLC( CAlwaysOnlineManager& aCallBack )
    {
    CAOCenRepControl* self = new ( ELeave ) CAOCenRepControl( aCallBack );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }
    
// ----------------------------------------------------------------------------
// CAOCenRepControl::CreateAoServerCenRepSessionL()
// ----------------------------------------------------------------------------    
//   
void CAOCenRepControl::CreateAoServerCenRepSessionL()
    {
    KAOMANAGER_LOGGER_FN1("CAOCenRepControl::CreateAoServerCenRepSessionL()");
    
    // Create the session if not yet exist.
    if ( !iAoServerCenRepSession )
        {
        iAoServerCenRepSession = CRepository::NewL( KCRUidAOSettings );
        }
    
    KAOMANAGER_LOGGER_FN2("CAOCenRepControl::CreateAoServerCenRepSessionL()");
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::SetCenRepKey()
// ----------------------------------------------------------------------------    
//   
TInt CAOCenRepControl::SetCenRepKey( 
    const TUid& aRepository, const TUint32 aKeyId, TInt aValue )
    {
    TInt result = KErrNotFound;
    
    switch ( aRepository.iUid )
        {
        case KCRAOSettings:
            result = iAoServerCenRepSession->Set( aKeyId, aValue );
            break;
        case KCoreAppUiRepository:
            result = iCoreAppUiCenRepSession->Set( aKeyId, aValue );
            break;
        default:
            __ASSERT_DEBUG( EFalse, User::Panic( KAOSetPanic, KErrNotSupported ) );
            break;
        }
        
    return result;
    }
    
// ----------------------------------------------------------------------------
// CAOCenRepControl::GetCenRepKey()
// ----------------------------------------------------------------------------    
//   
TInt CAOCenRepControl::GetCenRepKey(
    const TUid& aRepository, const TUint32 aKeyId, TInt& aValue )
    {
    TInt result = KErrNotFound;
    
    switch ( aRepository.iUid )
        {
        case KCRAOSettings:
            result = iAoServerCenRepSession->Get( aKeyId, aValue );
            break;
        case KCoreAppUiRepository:
            result = iCoreAppUiCenRepSession->Get( aKeyId, aValue );
            break;
        default:
            __ASSERT_DEBUG( EFalse, User::Panic( KAOGetPanic, KErrNotSupported ) );
            break;
        }
    
    return result;
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::DelCenRepKey()
// ----------------------------------------------------------------------------    
//   
TInt CAOCenRepControl::DelCenRepKey( 
    const TUid& aRepository, 
    const TUint32 aKeyId )
    {
    TInt result = KErrNotFound;
    
    switch ( aRepository.iUid )
        {
        case KCRAOSettings:
            result = iAoServerCenRepSession->Delete( aKeyId );
            break;
        case KCoreAppUiRepository:
            result = iCoreAppUiCenRepSession->Delete( aKeyId );
            break;
        default:
            __ASSERT_DEBUG( EFalse, User::Panic( KAODelPanic, KErrNotSupported ) );
            break;
        }
    
    return result;
    }


// ----------------------------------------------------------------------------
// CAOCenRepControl::CreateCoreAppUiCenRepSessionL()
// ----------------------------------------------------------------------------    
//   
void CAOCenRepControl::CreateCoreAppUiCenRepSessionL()
    {
    KAOMANAGER_LOGGER_FN1("CAOCenRepControl::CreateCoreAppUiCenRepSessionL()");
    // Create the session
    iCoreAppUiCenRepSession = CRepository::NewL( KCRUidCoreApplicationUIs );
    
    // Create the notifer
    iCoreAppUiCenRepNotifier = 
        CCenRepNotifyHandler::NewL( 
            iCallBack, *iCoreAppUiCenRepSession, CCenRepNotifyHandler::EIntKey,
            KCoreAppUIsNetworkConnectionAllowed  );
            
    // Start listening
    iCoreAppUiCenRepNotifier->StartListeningL();
    
    KAOMANAGER_LOGGER_FN2("CAOCenRepControl::CreateCoreAppUiCenRepSessionL()");        
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::ReCreateCoreAppUiCenRepSessionL()
// ----------------------------------------------------------------------------    
//   
void CAOCenRepControl::ReCreateCoreAppUiCenRepSessionL()
    {
    KAOMANAGER_LOGGER_FN1("CAOCenRepControl::ReCreateCoreAppUiCenRepSessionL()");
    // Remove session and notifier
    if ( iCoreAppUiCenRepNotifier )
        {
        iCoreAppUiCenRepNotifier->StopListening();
        }
    
    delete iCoreAppUiCenRepNotifier;
    iCoreAppUiCenRepNotifier = NULL;
    
    delete iCoreAppUiCenRepSession;    
    iCoreAppUiCenRepSession = NULL;
    
    // Create new session
    CreateCoreAppUiCenRepSessionL();
    
    KAOMANAGER_LOGGER_FN2("CAOCenRepControl::ReCreateCoreAppUiCenRepSessionL()");
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::UpdateDisabledPluginsUids()
// ----------------------------------------------------------------------------    
//   
void CAOCenRepControl::UpdateDisabledPluginsUids( 
    CArrayFixFlat<TUid>& aDisabledPluginUidsArray )
    {
    KAOMANAGER_LOGGER_FN1("CAOCenRepControl::UpdateDisabledPluginsUids()");
    // Reset to original state
    aDisabledPluginUidsArray.Reset();

    // Key #1 indicates the number of disabled plugin
    TInt count = 0;
    TInt result = GetCenRepKey(
        KCRUidAOSettings,
        KAODisabledPluginsCount, 
        count );
    
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAOCenRepControl::UpdateDisabledPluginsUids() Disabled plugins count: %d", count);
    
    // Is there any disabled plugins?
    if ( count > 0 )
        {
        TInt value = 0;
        TInt error = KErrNone;
        TUint32 key = KDisabledPluginsBaseKey;
        
        for ( ; key <= KDisabledPluginsLastKey; key++ )
            {
            error = GetCenRepKey(
                KCRUidAOSettings,
                key,
                value );
                
            // If no errors, then index have an UID and it can be appended
            // to disabled plugins array.
            if ( error == KErrNone && value != KErrNotFound )
                {
                TRAPD( err, aDisabledPluginUidsArray.AppendL( TUid::Uid( value ) ) )
                // If AppendL returns error (most propably memory full) then we
                // don't set plugin as disabled. Delete the key from cenrep.
                if ( err != KErrNone )
                    {
                    KAOMANAGER_LOGGER_WRITE_FORMAT("CAOCenRepControl::UpdateDisabledPluginsUids() AppendL failed, err =  %d", err);
                    // Enable plugin
                    SetPluginStatus( TUid::Uid( value ), EFalse );
                    }
                KAOMANAGER_LOGGER_WRITE_FORMAT("CAOCenRepControl::UpdateDisabledPluginsUids() found disabled plugin, UID: 0x%x", TUid::Uid( value ));
                }
            // Have we found all the disabled plugins?
            if ( aDisabledPluginUidsArray.Count() == count )
                {
                // We already found all the UIDs of the disabled plugins,
                // so we can jump out from the loop.
                break;
                }
            }
        }
        
    KAOMANAGER_LOGGER_FN2("CAOCenRepControl::UpdateDisabledPluginsUids()");
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::SetPluginStatus()
// ----------------------------------------------------------------------------    
//  
void CAOCenRepControl::SetPluginStatus( const TUid& aPlugin, TBool aDisable )
    {
    KAOMANAGER_LOGGER_FN1("CAOCenRepControl::SetPluginStatus()");
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAOCenRepControl::SetPluginStatus() aPlugin: 0x%x", aPlugin);
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAOCenRepControl::SetPluginStatus() aDisable: %d", aDisable);
    
    TInt result = KErrNone;
    TUint32 key;
    
    if ( aDisable )
        {
        // Get first free key for disabled plugins UIDs
        if ( GetFirstFreeKey( key ) )
            {
            // Set aPlugin to first free key
            result = SetCenRepKey( KCRUidAOSettings, key, aPlugin.iUid );
            }
            
        // Update disabled plugins count
        if ( result == KErrNone )
            {
            UpdateDisabledPluginsCount( ETrue );
            }
        }
    else
        {
        // Find correct cenrep key, which have value aPlugin as a value.
        if ( GetKeyWithValue( aPlugin, key ) )
            {
            // Delete the found cenrep key
            result = DelCenRepKey( KCRUidAOSettings, key );
            }
            
        // Update disabled plugins count
        if ( result == KErrNone )
            {
            UpdateDisabledPluginsCount( EFalse );
            }
        }
    
    KAOMANAGER_LOGGER_FN2("CAOCenRepControl::SetPluginStatus()");
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::GetFirstFreeKey()
// ----------------------------------------------------------------------------    
//  
TBool CAOCenRepControl::GetFirstFreeKey( TUint32& aFreeKey )
    {
    TInt error = KErrNone;
    TInt value = 0;
    TBool found = EFalse;
    TInt key = KDisabledPluginsBaseKey;
    
    while( !found && key <= KDisabledPluginsLastKey )
        {
        error = GetCenRepKey( 
            KCRUidAOSettings, 
            key, 
            value );
            
        if ( error == KErrNotFound )
            {
            aFreeKey = key;
            found = ETrue;
            KAOMANAGER_LOGGER_WRITE_FORMAT("CAOCenRepControl::GetFirstFreeKey() aFreeKey: 0x%x", aFreeKey);
            }
            
        key++;
        }
    
    return found;
    }

// ----------------------------------------------------------------------------
// CAOCenRepControl::GetKeyWithValue()
// ----------------------------------------------------------------------------    
// 
TBool CAOCenRepControl::GetKeyWithValue( 
    const TUid& aValue, 
    TUint32& aKey )
    {
    TInt error = KErrNone;
    TInt value = 0;
    TBool found = EFalse;
    TInt loopKey = KDisabledPluginsBaseKey;
    
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAOCenRepControl::GetKeyWithValue() Searching value: 0x%x", aValue);
    
    while ( !found && loopKey <= KDisabledPluginsLastKey )
        {
        error = GetCenRepKey( 
            KCRUidAOSettings, 
            loopKey, 
            value );
            
        if ( error == KErrNone && aValue.iUid == value )
            {
            aKey = loopKey;
            found = ETrue;
            KAOMANAGER_LOGGER_WRITE_FORMAT("CAOCenRepControl::GetKeyWithValue() Found in key: 0x%x", aKey);
            }
        loopKey++;
        value = 0;
        }

    return found;
    }
 
 // ----------------------------------------------------------------------------
// CAOCenRepControl::UpdateDisabledPluginsCount()
// ----------------------------------------------------------------------------    
//    
TInt CAOCenRepControl::UpdateDisabledPluginsCount( TBool aIncrease )
    {
    KAOMANAGER_LOGGER_FN1("CAOCenRepControl::UpdateDisabledPluginsCount()");
    TInt count = 0;
    TInt result = GetCenRepKey(
        KCRUidAOSettings,
        KAODisabledPluginsCount, 
        count );

    if ( result == KErrNone )
        {
        aIncrease ? count++ : count--;
        
        result = SetCenRepKey( 
            KCRUidAOSettings, 
            KAODisabledPluginsCount, 
            count );
            
        KAOMANAGER_LOGGER_WRITE_FORMAT("CAOCenRepControl::UpdateDisabledPluginsCount() count: %d", count);
        }

    KAOMANAGER_LOGGER_FN2("CAOCenRepControl::UpdateDisabledPluginsCount()");
    
    return result;
    }
// End of File
