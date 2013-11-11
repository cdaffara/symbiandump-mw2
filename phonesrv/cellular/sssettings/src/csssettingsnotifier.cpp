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
* Description:  Notifier for changes.
*
*/


// INCLUDE FILES
#include    <rmmcustomapi.h>
#include    <centralrepository.h>
#include    <sssettingsprivatecrkeys.h> 

#include    "csssettingsnotifier.h" 
#include    "msssettingsobserver.h" 
#include    "csssettingsalsnotifier.h" 
#include    "sssettingslogger.h" 


// DATA TYPES

/**
* It enumerates all keys to be notified from Central Repository under 
* KCRUidSupplementaryServiceSettings.
*
*   ESSSettingsKeyNone    - No key change to be notified.
*   ESSSettingsKeyAlsLine - Als line change to be notified.
*   ESSSettingsKeyClir    - Clir change to be notified.
*   ESSSettingsKeyCug     - Cug change to be notified.
*   ESSSettingsKeyAll     - All keys to be notified.
*/
enum TSSSettingsCenRepNotifyKey
    {
    ESSSettingsKeyNone    = 0x00000000,
    ESSSettingsKeyAlsLine = 0x00000001,
    ESSSettingsKeyClir    = 0x00000002,
    ESSSettingsKeyCug     = 0x00000004,

    // Keep as last.
    ESSSettingsKeyAll     = 0xFFFFFFFF
    };


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CSSSettingsNotifier::CSSSettingsNotifier
// -----------------------------------------------------------------------------
//
CSSSettingsNotifier::CSSSettingsNotifier( RSSSettings& aSettings ) 
: CActive( EPriorityStandard ), 
  iSettings( aSettings ),
  iNotifyKeys ( ESSSettingsKeyNone )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CSSSettingsNotifier::ConstructL
// -----------------------------------------------------------------------------
//
void CSSSettingsNotifier::ConstructL()
    {
    iAls = CSSSettingsAlsNotifier::NewL( 
        iSettings.iData->iMobilePhone,
        *iSettings.iData->iCustomPhone,
        *this );

    iRepository = CRepository::NewL( KCRUidSupplementaryServiceSettings );
    
    iCenRepAlsNotifyHandler = CCenRepNotifyHandler::NewL(
        *this, 
        *iRepository,
        CCenRepNotifyHandler::EIntKey,
        KSettingsAlsLine );
    
    iCenRepClirNotifyHandler = CCenRepNotifyHandler::NewL(
        *this, 
        *iRepository,
        CCenRepNotifyHandler::EIntKey,
        KSettingsCLIR );
    
    iCenRepCugNotifyHandler = CCenRepNotifyHandler::NewL(
        *this, 
        *iRepository,
        CCenRepNotifyHandler::EIntKey,
        KSettingsCUG );
    
    // Start listening all the keys, the notify is handled only if there is
    // notify request made.
    iCenRepAlsNotifyHandler->StartListeningL();
    iCenRepClirNotifyHandler->StartListeningL();
    iCenRepCugNotifyHandler->StartListeningL();
    }

// -----------------------------------------------------------------------------
// CSSSettingsNotifier::~CSSSettingsNotifier
// -----------------------------------------------------------------------------
//
CSSSettingsNotifier::~CSSSettingsNotifier()
    {
    Cancel();
    if( iCenRepAlsNotifyHandler )
        {
        iCenRepAlsNotifyHandler->StopListening();
        }

    if( iCenRepClirNotifyHandler )
        {
        iCenRepClirNotifyHandler->StopListening();
        }

    if( iCenRepCugNotifyHandler )
        {
        iCenRepCugNotifyHandler->StopListening();
        }
    
    delete iCenRepAlsNotifyHandler;
    delete iCenRepClirNotifyHandler;
    delete iCenRepCugNotifyHandler;
    delete iRepository;
    delete iAls;

    }

// -----------------------------------------------------------------------------
// CSSSettingsNotifier::AddNotifier
// -----------------------------------------------------------------------------
//
TInt CSSSettingsNotifier::AddNotifier( TSSSettingsSetting aSetting )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsNotifier::AddNotifier");
    TInt error(KErrNone);
    switch ( aSetting )
        {
        case ESSSettingsCug:
            iNotifyKeys = ( iNotifyKeys | ESSSettingsKeyCug );
            break;

        case ESSSettingsClir:
            iNotifyKeys = ( iNotifyKeys | ESSSettingsKeyClir );
            break;

        case ESSSettingsAls:
            error = iAls->NotifyAlsChange();
            if ( error == KErrNone || error == KErrAlreadyExists )
                {
                iNotifyKeys = ( iNotifyKeys | ESSSettingsKeyAlsLine );
                }
            break;

        case ESSSettingsAlsBlocking:
            // Handled via active object
            if ( !IsActive() )
                {
                iSettings.iData->iCustomPhone->NotifyAlsBlockedChanged( 
                    iStatus, iAlsBlockStatus );
                SetActive();
                }
            else
                {
                error = KErrAlreadyExists;
                }
            break;

        case ESSSettingsDefaultCug:
            //CUG default cannot be changed
            break;

        default:
            error = KErrNotSupported;
        }   

    __SSSLOGSTRING1("[SSS]     CSSSettingsNotifier::AddNotifier: error: %d", error );
    __SSSLOGSTRING("[SSS] <--CSSSettingsNotifier::AddNotifier");
    return error;
    }

// -----------------------------------------------------------------------------
// CSSSettingsNotifier::RemoveNotifier
// -----------------------------------------------------------------------------
//
void CSSSettingsNotifier::RemoveNotifier( TSSSettingsSetting aSetting )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsNotifier::RemoveNotifier");
    switch ( aSetting )
        {
        case ESSSettingsCug:
            iNotifyKeys = ( iNotifyKeys & ( ~ESSSettingsKeyCug ) );
            break;

        case ESSSettingsClir:
            iNotifyKeys = ( iNotifyKeys & ( ~ESSSettingsKeyClir ) );
            break;

        case ESSSettingsAls:
            if ( iAls )
                {
                iAls->CancelNotify();
                }
            iNotifyKeys = ( iNotifyKeys  & ( ~ESSSettingsKeyAlsLine ) );
            break;

        case ESSSettingsAlsBlocking:
            Cancel();
            break;

        default:
            break;
        } 
    __SSSLOGSTRING("[SSS] <--CSSSettingsNotifier::RemoveNotifier");
    }

// -----------------------------------------------------------------------------
// CSSSettingsNotifier::RunL
// -----------------------------------------------------------------------------
//
void CSSSettingsNotifier::RunL()
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsNotifier::RunL");
    switch ( iStatus.Int() )
        {
        case KErrCancel:
        case KErrNotSupported:
            return;
        case KErrNone:
            // Inform observers
            switch ( iAlsBlockStatus )
                {
                case RMmCustomAPI::EBlockStatusNotSupported: 
                    iSettings.InformChange( 
                        ESSSettingsAlsBlocking, 
                        static_cast <TInt>( 
                        ESSSettingsAlsBlockingNotSupported ) );
                    break;
                case RMmCustomAPI::EBlockStatusActive: 
                    iSettings.InformChange( 
                        ESSSettingsAlsBlocking, 
                        static_cast <TInt>( ESSSettingsAlsBlockingOn ) );
                    break;
                case RMmCustomAPI::EBlockStatusInactive: 
                    iSettings.InformChange( ESSSettingsAlsBlocking,
                        static_cast <TInt>( ESSSettingsAlsBlockingOff ) );
                    break;
                case RMmCustomAPI::EBlockStatusUnknown: 
                    break;
                default:
                    break;
                }
            break;
        default:
            break;
        }

    // re-activate request.
    iSettings.iData->iCustomPhone->NotifyAlsBlockedChanged( 
        iStatus, iAlsBlockStatus );
    SetActive();

    __SSSLOGSTRING("[SSS] <--CSSSettingsNotifier::RunL");
    }

// -----------------------------------------------------------------------------
// CSSSettingsNotifier::DoCancel
// -----------------------------------------------------------------------------
//
void CSSSettingsNotifier::DoCancel()
    {
    iSettings.iData->iCustomPhone->CancelAsyncRequest( 
        ECustomNotifyAlsBlockedChangedIPC );
    }

// -----------------------------------------------------------------------------
// CSSSettingsNotifier::HandleNotifyInt
// -----------------------------------------------------------------------------
//
void CSSSettingsNotifier::HandleNotifyInt( TUint32 aId, TInt aNewValue )
    {
    __SSSLOGSTRING2("[SSS] -->CSSSettingsNotifier::HandleNotifyInt aId: %d  aNewValue: %d", aId, aNewValue);
    // First check CUG mode
    if ( aId == KSettingsCUG )
        {
        // Inform change only if Cug notify request is on.
        if ( iNotifyKeys & ESSSettingsKeyCug )
            {
            iSettings.InformChange(
                ESSSettingsCug,
                aNewValue );
            }
        }

    // Then CLIR
    else if ( aId == KSettingsCLIR )
        {
        // Inform change only if Clir notify request is on.
        if ( iNotifyKeys & ESSSettingsKeyClir )
            {
            iSettings.InformChange(
                ESSSettingsClir,
                aNewValue );
            }
        }

    // ALS - not forwarded if not supported
    else if ( aId == KSettingsAlsLine )
        {
        // Inform change only if Als line notify request is on.
        if ( iNotifyKeys & ESSSettingsKeyAlsLine )
            {
            TBool ppSupported(EFalse);
            TBool simSupported(EFalse);
            TBool cspSupported(EFalse);
            TInt cspError(KErrNone);
            iAls->GetAlsSupport( ppSupported, 
                                simSupported, 
                                cspSupported, 
                                cspError );
            __SSSLOGSTRING1("[SSS]    ppSupported:  %d", ppSupported);
            __SSSLOGSTRING1("[SSS]    simSupported: %d", simSupported);
            __SSSLOGSTRING1("[SSS]    cspSupported: %d", cspSupported);
            __SSSLOGSTRING1("[SSS]    cspError:     %d", cspError);
            if ( AlsSupport( ppSupported, cspSupported, cspError ) )
                {
                __SSSLOGSTRING1("[SSS] ->InformChange aNewValue:%d", aNewValue);
                iSettings.InformChange(
                    ESSSettingsAls,
                    aNewValue );
                }
            else
                {
                __SSSLOGSTRING("[SSS] ->InformChange AlsNotSupported ");
                iSettings.InformChange( 
                    ESSSettingsAls,
                    ESSSettingsAlsNotSupported );
                }
            }
        }
    // Other
    else
        {
        __ASSERT_DEBUG( EFalse, User::Invariant() );
        }
    __SSSLOGSTRING("[SSS] <--CSSSettingsNotifier::HandleNotifyInt");
    }

// -----------------------------------------------------------------------------
// CSSSettingsNotifier::AlsSupport
// -----------------------------------------------------------------------------
//
TBool CSSSettingsNotifier::AlsSupport( TBool aPPSupport, TBool aCSPSupport,
    TInt aCSPError )
    {
    TBool send(EFalse);
    // If aCSPSupport is false and CSP doesnt return error then it is not
    // needed to send notification to clients.
    if ( aPPSupport || ( aCSPSupport && aCSPError == KErrNone ) ) 
        {
        send = ETrue;
        }
    
    return send;
    }

// -----------------------------------------------------------------------------
// CSSSettingsNotifier::HandleNotifyError
// -----------------------------------------------------------------------------
//
void CSSSettingsNotifier::HandleNotifyError( TUint32 /*aId*/, TInt /*aError*/,
    CCenRepNotifyHandler* /*aHandler*/ )
    {
    // Here you could call the below, if you are sure that it do not cause 
    // eternal loop
    // TRAPD( error, aHandler->StartListeningL() );
    }


// -----------------------------------------------------------------------------
// CSSSettingsNotifier::GetAlsHandler
// -----------------------------------------------------------------------------
//
CSSSettingsAlsNotifier* CSSSettingsNotifier::GetAlsHandler()
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsNotifier::GetAlsHandler");
    TBool ppSupported(EFalse);
    TBool simSupported(EFalse);
    TBool cspSupported(EFalse);
    TInt cspError(KErrNone);
    iAls->GetAlsSupport( ppSupported, simSupported, cspSupported, cspError );
    __SSSLOGSTRING1("[SSS]     CSSSettingsNotifier::GetAlsHandler: ppSupported: %d", ppSupported );
    __SSSLOGSTRING1("[SSS]     CSSSettingsNotifier::GetAlsHandler: simSupported: %d", simSupported );
    __SSSLOGSTRING1("[SSS]     CSSSettingsNotifier::GetAlsHandler: cspSupported: %d", cspSupported );
    if ( ppSupported || simSupported || cspSupported )
        {
        __SSSLOGSTRING("[SSS] <--CSSSettingsNotifier::GetAlsHandler: ALS supported");
        return iAls; // returned only if supported
        }

    __SSSLOGSTRING("[SSS] <--CSSSettingsNotifier::GetAlsHandler: NULL returned");
    return NULL;
    }

// -----------------------------------------------------------------------------
// CSSSettingsNotifier::HandleRefresh
// -----------------------------------------------------------------------------
//
TInt CSSSettingsNotifier::HandleRefresh()
    {
    return iAls->HandleRefresh();
    }

//  End of File  
