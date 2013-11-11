/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CVoiceMailboxImpl class
*
*/


// INCLUDE FILES
#include <featmgr.h>
#include <cvoicemailboxentry.h>
#include "voicemailboxdefsinternal.h"

#include "vmbxenginebase.h"
#include "vmbxuiutilities.h"
#include "vmbxenginefactory.h"
#include "vmbxobserver.h"
#include "vmbxlogger.h"
#include "vmbxutilities.h"
#include "vmbxcenrephandler.h"
#include "voicemailboximpl.h"

// phone application uid
// In TB9.2 phone application use 0x100058B3
// In TB10.1 we get this value 101F4CD5 by testing.
// So use the value by testing
const TUint KPhoneApplicationUid = 0x101F4CD5;

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVoiceMailboxImpl* CVoiceMailboxImpl::NewL()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::NewL =>" );
    CVoiceMailboxImpl* vmbx = CVoiceMailboxImpl::NewLC();
    CleanupStack::Pop( vmbx );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::NewL <=" );
    return vmbx;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVoiceMailboxImpl* CVoiceMailboxImpl::NewLC()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::NewLC =>" );
    CVoiceMailboxImpl* vmbx = new( ELeave ) CVoiceMailboxImpl();
    CleanupStack::PushL( vmbx );
    vmbx->ConstructL();
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::NewLC <=" );
    return vmbx;
    }
// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::~CVoiceMailboxImpl
// Destructor
// ---------------------------------------------------------------------------
//
CVoiceMailboxImpl::~CVoiceMailboxImpl()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::~CVoiceMailboxImpl =>" );
    FeatureManager::UnInitializeLib();
    delete iUiUtilities;
    delete iVmbxFactory;
    delete iVmbxObserver;
    delete iCenRepHandler;
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::~CVoiceMailboxImpl <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::GetStoredEntry
// 
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxImpl::GetStoredEntry( const TVoiceMailboxParams& aParams,
                                            CVoiceMailboxEntry*& aEntry ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetStoredEntry =>" );
    TRAPD( err, GetStoredEntryL( aParams, aEntry ) );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetStoredEntry: err%I <=", err );
    return err;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::SaveEntry
// 
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxImpl::SaveEntry( const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SaveEntry =>" );
    TRAPD( err, SaveEntryL( aEntry ) );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::SaveEntry: err%I <=", err );
    return err;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::QueryNewEntry
// 
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxImpl::QueryNewEntry( const TVoiceMailboxParams& aParams,
                                           CVoiceMailboxEntry*& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryNewEntry =>" );
    TRAPD( err, QueryNewEntryL( aParams, aEntry ) );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::QueryNewEntry err%I <=",
        err );
    return err;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::QueryChangeEntry
// 
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxImpl::QueryChangeEntry( const TVoiceMailboxParams& aParams,
                                             CVoiceMailboxEntry*& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryChangeEntry =>" );
    TRAPD( err, QueryChangeEntryL( aParams, aEntry ) );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::QueryChangeEntry: err%I <=",
        err );
    return err;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::QueryVmbxType
// 
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxImpl::QueryVmbxType( TVoiceMailboxParams& aParams )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryVmbxType =>" );
    TRAPD( err, QueryVmbxTypeL( aParams ) );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::QueryVmbxType: err%I<=", err );
    return err;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::NotifyVmbxNumberChangeL
//
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::NotifyVmbxNumberChangeL(
    MVoiceMailboxObserver& aObserver,
    const TBool /*aNotifyOnActiveLineOnly*/ )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::NotifyVmbxNumberChangeL =>" );
    if ( !iNotifyCallBack )
        {
        iNotifyCallBack = &aObserver;
        iVmbxObserver = CVmbxObserver::NewL( *this );
        iVmbxObserver->SetVmbxObserver( *this );
        }
    else
        {
        User::Leave( KErrInUse );
        }
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::NotifyVmbxNumberChangeL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::NotifyVmbxNumberChangeCancel
//
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::NotifyVmbxNumberChangeCancel()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::NotifyVmbxNumberChangeCancel =>" );
    iNotifyCallBack = NULL;
    delete iVmbxObserver;
    iVmbxObserver = NULL;
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::NotifyVmbxNumberChangeCancel <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::CheckConfiguration
//
// ---------------------------------------------------------------------------
//
TBool CVoiceMailboxImpl::CheckConfiguration( const TVoiceMailboxParams& aParams,
                             const TInt aFlags )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::CheckConfiguration =>" );
    TBool configuration( EFalse );
    CVmbxEngineBase* vmbx = NULL;
    TRAPD( res, iVmbxFactory->CreateEngineL( vmbx, aParams.iType ) );
    if ( KErrNone == res && vmbx )
        {
        configuration = vmbx->CheckConfiguration( aParams, aFlags );
        }
    delete vmbx;
    vmbx = NULL;
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::CheckConfiguration <=" );
    return configuration;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::GetServiceIds
//
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxImpl::GetServiceIds( RIdArray& aProfileIds )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetServiceIds =>" );
    TRAPD( err, GetServiceIdsL( aProfileIds ) );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetServiceIds: err%I <=", err );
    return err;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::GetVmbxImage
//
// ---------------------------------------------------------------------------
/* CGulIcon* CVoiceMailboxImpl::GetVmbxImage(
                        const TVoiceMailboxParams& aParams )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetVmbxImage =>" );
    CGulIcon* guiIcon( NULL );
    TRAPD( err, guiIcon = GetVmbxImageL( aParams ) );
    if ( KErrNone != err )
        {
        delete guiIcon;
        guiIcon = NULL;
        }
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetVmbxImage: err%I <=", err );
    return guiIcon;
    } */

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::GetVmbxImages
//
// ---------------------------------------------------------------------------
//
/* CArrayPtr<CGulIcon>* CVoiceMailboxImpl::GetVmbxImages(
                            RArray<TVoiceMailboxParams>& aParams )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetVmbxImages =>" );
    CArrayPtr<CGulIcon>* guiIcons( NULL );
    TRAPD( err, guiIcons = GetVmbxImagesL( aParams ) );
    if ( KErrNone != err )
        {
        if ( guiIcons )
            {
            guiIcons->ResetAndDestroy();
            }
        }
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetVmbxImages: err%I <=", err );
    return guiIcons;
    } */

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::SaveProvisionedEntry
//
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxImpl::SaveProvisionedEntry( 
                                    const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SaveProvisionedEntry =>" );
    TRAPD( err, SaveProvisionedEntryL( aEntry ) );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::SaveProvisionedEntry: err%I <=",
    err );
    return err;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::SatRefreshL
// 
// notify Sat Refresh
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::SatRefreshL()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SatRefreshL =>" );
    // notify sim file's entry changed
    NotifyChangedL( EVmbxVoice );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SatRefreshL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::CenRepChangedL
// 
// notify CenRep Changed
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::CenRepChangedL( TVmbxCenRepKey aId )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::CenRepChangedL =>" );
    TVmbxAlsLineType alsLine = VmbxUtilities::AlsLine();
    if ( ( ( EVmbxVoiceLinePrimaryKey == aId 
            || EVmbxVideoLinePrimaryKey == aId ) // primary value changed
            && EVmbxAlsLine2 != alsLine )// line 1 active or using default line
        || ( ( EVmbxVoiceLineAlternateKey == aId 
            || EVmbxVideoLineAlternateKey == aId )// alternate value changed
            && EVmbxAlsLine2 == alsLine ) )// line 2 active
        {
        if ( VmbxUtilities::VideoSupported() 
            && ( ( EVmbxVideoLinePrimaryKey == aId 
            || EVmbxVideoLineAlternateKey == aId ) ) )
            {
            NotifyChangedL( EVmbxVideo );
            }
        else
            {
            NotifyChangedL( EVmbxVoice );
            }
        }
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::CenRepChangedL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::SimStoreChangedL
// notify sim store changed
// 
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::SimStoreChangedL()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SimStoreChangedL =>" );
    // notify sim file changed
    NotifyChangedL( EVmbxVoice );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SimStoreChangedL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::CVoiceMailboxImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVoiceMailboxImpl::CVoiceMailboxImpl()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::CVoiceMailboxImpl <=>" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::ConstructL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::ConstructL =>" );
    FeatureManager::InitializeLibL();

    // Create Ui utilities for dialog and images
    iUiUtilities = CVmbxUiUtilities::NewL();
    // create CenRep
    iCenRepHandler = CVmbxCenRepHandler::NewL();

    // Create factory for voice/video/voip engine
    iVmbxFactory = CVmbxEngineFactory::NewL( *this );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::GetStoredEntryL
// 
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::GetStoredEntryL( const TVoiceMailboxParams& aParams,
                                    CVoiceMailboxEntry*& aEntry ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetStoredEntryL =>" );
    CVoiceMailboxEntry* entry( NULL );
    CVmbxEngineBase* vmbxBox( NULL );
    iVmbxFactory->CreateEngineL( vmbxBox, aParams.iType );
    CleanupStack::PushL( vmbxBox );
    vmbxBox->GetL( entry );
    CleanupStack::PopAndDestroy( vmbxBox );
    aEntry = entry;
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetStoredEntryL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::SaveEntryL
// 
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::SaveEntryL( const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SaveEntryL =>" );
    TVoiceMailboxParams params( aEntry );
    CVmbxEngineBase* vmbxBox( NULL );
    iVmbxFactory->CreateEngineL( vmbxBox, aEntry.VoiceMailboxType() );
    CleanupStack::PushL( vmbxBox );
    if ( vmbxBox->CheckConfiguration( params, EVmbxChangeNbrAllowedOnUi ) )
        {
        vmbxBox->SaveL( aEntry, ETrue );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    CleanupStack::PopAndDestroy( vmbxBox );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SaveEntryL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::QueryNewEntryL
// 
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::QueryNewEntryL( const TVoiceMailboxParams& aParams,
                                   CVoiceMailboxEntry*& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryNewEntryL =>" );
    CVmbxEngineBase* vmbxBox( NULL );
    iVmbxFactory->CreateEngineL( vmbxBox, aParams.iType );
    CleanupStack::PushL( vmbxBox );
    if ( vmbxBox->CheckConfiguration( aParams, EVmbxChangeNbrAllowedOnUi ) )
        {
        CVoiceMailboxEntry* entry = CVoiceMailboxEntry::NewLC();
        entry->SetVoiceMailboxType( aParams.iType );
        entry->SetServiceId( aParams.iServiceId );
        vmbxBox->QueryDefineNumberL( *entry );
        CleanupStack::Pop( entry );
        aEntry = entry;
        }
    else
        {
        CheckNumberProvisionedL( aParams );
        }
    CleanupStack::PopAndDestroy( vmbxBox );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryNewEntryL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::QueryChangeEntryL
// 
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::QueryChangeEntryL( const TVoiceMailboxParams& aParams,
                                            CVoiceMailboxEntry*& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryChangeEntryL =>" );
    CVmbxEngineBase* vmbxBox( NULL );
    iVmbxFactory->CreateEngineL( vmbxBox, aParams.iType );
    CleanupStack::PushL( vmbxBox );
    if ( vmbxBox->CheckConfiguration( aParams, EVmbxChangeNbrAllowedOnUi ) )
        {
        CVoiceMailboxEntry* entry( NULL );
        GetStoredEntryL( aParams, entry );
        CleanupStack::PushL( entry );
        vmbxBox->QueryChangeNumberL( *entry );
        CleanupStack::Pop( entry );
        aEntry = entry;
        }
    else
        {
        CheckNumberProvisionedL( aParams );
        }
    CleanupStack::PopAndDestroy( vmbxBox );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryChangeEntryL <=" );
    }

// ---------------------------------------------------------------------------
// CleanupRPointerArray
// avoid memory leak when using RPointerArray
// ---------------------------------------------------------------------------
//
LOCAL_C void CleanupRPointerArray( TAny* aParam )
    {
    if ( aParam )
        {
        static_cast< RPointerArray<CVoiceMailboxEntry>* >( aParam )
                                                    ->ResetAndDestroy();
        }
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::QueryVmbxTypeL
// Query vmbx type
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::QueryVmbxTypeL( TVoiceMailboxParams& aParams )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryVmbxTypeL =>" );
    aParams.iType = EVmbxNone;
    aParams.iServiceId = KVmbxServiceIdNone;

    TInt result ( KErrNotFound );
    RPointerArray<CVoiceMailboxEntry> array;
    TCleanupItem item( CleanupRPointerArray, &array );
    CleanupStack::PushL( item );
    GetDefinedEntriesL( array );

    TInt definedCount = array.Count();
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::QueryVmbxTypeL:\
        definedCount%I", definedCount );
    // more than one number defined
    if ( definedCount > 1 )
        {
        VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryVmbxTypeL:\
        more than one number Denfined" );
        // query call type
        RArray<TVoiceMailboxParams> paramsArray;
        CleanupClosePushL( paramsArray );
        TVoiceMailboxParams entryParams;
        for ( TInt i = 0; i < array.Count(); i++ )
            {
            entryParams.iType = array[i]->VoiceMailboxType();
            entryParams.iServiceId = array[i]->ServiceId();
            paramsArray.AppendL( entryParams );
            }
        //CArrayPtr<CGulIcon>* dialogIcons = GetVmbxImagesL( paramsArray );
        //CleanupStack::PushL( dialogIcons );
        iUiUtilities->ShowCallSelectionDialogL( 
        array, /* dialogIcons, */ aParams, result );
        //CleanupStack::Pop( dialogIcons );
        CleanupStack::PopAndDestroy( &paramsArray );
        }
    // only one number defined
    else if ( 1 == definedCount )
        {
        // return the onle defined number
        VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryVmbxTypeL:\
        One number Denfined" );
        aParams.iType = array[0]->VoiceMailboxType();
        result = KErrNone;
        }
    // there is no number defined
    else
        {
        VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryVmbxTypeL:\
             no number Denfined" );
        // define number
        if ( VmbxUtilities::VideoSupported() )
            {
            // query to be defined type
            iUiUtilities->ShowDefineSelectionDialogL( aParams.iType, result );
            // if result is KErrNone(but result should be also KErrNotFound ),
            // it means user have seclected the defined type;
            // else user cancel to select the type, so should return result value
            if ( KErrNone == result )
                {
                result = KErrNotFound;
                }
            }
        else
            {
            aParams.iType = EVmbxVoice;
            }
        }

    CleanupStack::PopAndDestroy( &array ); //item 
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::QueryVmbxTypeL: result%I",
    result );
    User::LeaveIfError( result );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::QueryVmbxTypeL <=" );
    }

// ----------------------------------------------------------------------------
// CVoiceMailboxImpl::GetDefinedEntriesL
//
// ----------------------------------------------------------------------------
//
void CVoiceMailboxImpl::GetDefinedEntriesL( 
                            RPointerArray<CVoiceMailboxEntry>& aArray )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetDefinedEntriesL =>" );
    TRAPD( voiceErr,GetDefinedVoiceEntryL( aArray ));
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetDefinedEntriesL: voiceErr%I",
            voiceErr );
    // It should be ignored when video/voip unsupported or no number defined
    if ( KErrNotFound != voiceErr )
        {
        User::LeaveIfError( voiceErr );
        }
    
    TRAPD( videoErr, GetDefinedVideoEntryL( aArray ) );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetDefinedEntriesL: videoErr%I",
            videoErr );
    if ( KErrNotSupported != videoErr && KErrNotFound != videoErr )
        {
        User::LeaveIfError( videoErr );
        }
 
    TRAPD( voIperr, GetDefinedVoipEntriesL( aArray ));

    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetDefinedEntriesL: voIperr%I",
            voIperr );
    // It should be ignored when video/voip unsupported or no number defined
    if ( KErrNotSupported != voIperr && KErrNotFound != voIperr )
        {
        User::LeaveIfError( voIperr );
        }
 
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetDefinedEntriesL <=" );
    }

// ----------------------------------------------------------------------------
// CVoiceMailboxImpl::GetDefinedVoiceEntryL
//
// ----------------------------------------------------------------------------
//
void CVoiceMailboxImpl::GetDefinedVoiceEntryL( 
                            RPointerArray<CVoiceMailboxEntry>& aArray )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetDefinedVoiceEntryL =>" );
    // check voice mailbox
    CVmbxEngineBase* voiceBox( NULL );
    iVmbxFactory->CreateEngineL( voiceBox, EVmbxVoice );
    CleanupStack::PushL( voiceBox );
    // get voice entry
    CVoiceMailboxEntry* voiceEntry( NULL);
    voiceBox->GetL( voiceEntry );
    CleanupStack::PushL( voiceEntry );
    // get voice number
    TPtrC vmbxVoiceNumber( KNullDesC );
    TInt resVoice = voiceEntry->GetVmbxNumber( vmbxVoiceNumber );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetDefinedVoiceEntryL:\
                    VoiceNumber = %S ", &vmbxVoiceNumber );
    if ( KErrNone == resVoice && vmbxVoiceNumber.Length() )
        {
        aArray.AppendL( voiceEntry );
        }
    CleanupStack::Pop( voiceEntry );
    CleanupStack::PopAndDestroy( voiceBox );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetDefinedVoiceEntryL <=" );
    }

// ----------------------------------------------------------------------------
// CVoiceMailboxImpl::GetDefinedVideoEntryL
//
// ----------------------------------------------------------------------------
//
void CVoiceMailboxImpl::GetDefinedVideoEntryL( 
                            RPointerArray<CVoiceMailboxEntry>& aArray )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetDefinedVideoEntryL =>" );
    // check video mailbox
    CVmbxEngineBase* videoBox = NULL;
    iVmbxFactory->CreateEngineL( videoBox, EVmbxVideo );
    CleanupStack::PushL( videoBox );

    CVoiceMailboxEntry* videoEntry( NULL );
    // get video entry
    videoBox->GetL( videoEntry );
    CleanupStack::PushL( videoEntry );
    TPtrC vmbxVideoNumber( KNullDesC );

    TInt resVideo = videoEntry->GetVmbxNumber( vmbxVideoNumber );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetDefinedVideoEntryL:\
                            VideoNumber = %S ", &vmbxVideoNumber );

    if ( KErrNone == resVideo && vmbxVideoNumber.Length() )
        {
        aArray.AppendL( videoEntry );
        }
    CleanupStack::Pop( videoEntry );
    CleanupStack::PopAndDestroy( videoBox );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetDefinedVideoEntryL <=" );
    }

// ----------------------------------------------------------------------------
// CVoiceMailboxImpl::GetDefinedVoipEntriesL
//
// ----------------------------------------------------------------------------
//
void CVoiceMailboxImpl::GetDefinedVoipEntriesL( 
                            RPointerArray<CVoiceMailboxEntry>& aArray )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetDefinedVoipEntriesL =>" );
    // check VoIP mailbox
    CVmbxEngineBase* voIPBox = NULL;
    iVmbxFactory->CreateEngineL( voIPBox, EVmbxVoip );
    CleanupStack::PushL( voIPBox );

    RIdArray profileIds;
    CleanupClosePushL( profileIds );

    voIPBox->GetServiceIdsL( profileIds );
    TInt profileCounts = profileIds.Count();
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetDefinedVoipEntriesL:\
                                   VoIP profileCounts%I ", profileCounts);
    if ( 0 < profileCounts )
        {
        for ( TInt i( 0 ); i < profileCounts; i++ )
            {
            voIPBox->SetCurrentServiceId( profileIds[i] );
            CVoiceMailboxEntry* voIPEntry( NULL );
            // get voip entry
            voIPBox->GetL( voIPEntry );
            CleanupStack::PushL( voIPEntry );
            TPtrC vmbxVoIPName( KNullDesC );
            TInt resVoIP = voIPEntry->GetVmbxName( vmbxVoIPName );

            TPtrC vmbxVoIPAddress( KNullDesC );
            if ( KErrNone == resVoIP )
                {
                VMBLOGSTRING2( 
                "VMBX: CVoiceMailboxImpl::GetDefinedVoipEntriesL:\
                        vmbxVoIPName = %S ", &vmbxVoIPName );
                resVoIP = voIPEntry->GetVmbxNumber( vmbxVoIPAddress );
                }
            if ( KErrNone == resVoIP && vmbxVoIPAddress.Length() 
                 && vmbxVoIPName.Length() )
                {
                VMBLOGSTRING2( 
                "VMBX: CVoiceMailboxImpl::GetDefinedVoipEntriesL:\
                    vmbxVoIPAddress = %S ", &vmbxVoIPAddress );
                aArray.AppendL( voIPEntry );
                }
            CleanupStack::Pop( voIPEntry );
            }
        }
    CleanupStack::PopAndDestroy( &profileIds );
    CleanupStack::PopAndDestroy( voIPBox );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetDefinedVoipEntriesL <=" );
    }

// ----------------------------------------------------------------------------
// CVoiceMailboxImpl::GetServiceIdsL
//
// ----------------------------------------------------------------------------
void CVoiceMailboxImpl::GetServiceIdsL( RIdArray& aProfileIds )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetServiceIdsL =>" );
    // Get all Service Ids
    RIdArray profileIds;
    CleanupClosePushL( profileIds );
    CVmbxEngineBase* vmbx = NULL;
    iVmbxFactory->CreateEngineL( vmbx, EVmbxVoip );
    CleanupStack::PushL( vmbx );
    vmbx->GetServiceIdsL( profileIds );
    for ( TInt i( 0 ); i < profileIds.Count(); i++ )
        {
        aProfileIds.AppendL( profileIds[i]);
        }
    CleanupStack::PopAndDestroy( vmbx );
    CleanupStack::Pop( &profileIds );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetServiceIdsL <=");
    }

// ----------------------------------------------------------------------------
// CVoiceMailboxImpl::GetVmbxImageL
//
// ----------------------------------------------------------------------------
/* CGulIcon* CVoiceMailboxImpl::GetVmbxImageL( 
                    const TVoiceMailboxParams& aParams )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetVmbxImageL =>" );
    CGulIcon* guiIcon( NULL );
    CVmbxEngineBase* vmbxEngine = NULL;
    iVmbxFactory->CreateEngineL( vmbxEngine, aParams.iType );
    CleanupStack::PushL( vmbxEngine );
    if ( vmbxEngine )
        {
        guiIcon = vmbxEngine->GetVmbxImageL( aParams );
        }
    CleanupStack::PopAndDestroy( vmbxEngine ); 
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetVmbxImageL <=" );
    return guiIcon;
    } */

// ----------------------------------------------------------------------------
// CVoiceMailboxImpl::GetVmbxImagesL
//
// ----------------------------------------------------------------------------
//
/* CArrayPtr<CGulIcon>* CVoiceMailboxImpl::GetVmbxImagesL(
                            RArray<TVoiceMailboxParams>& aParams )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetVmbxImagesL =>" );
    CAknIconArray* icons =
                new( ELeave ) CAknIconArray( KVmLbxItemsArraySize );
    CleanupStack::PushL( icons );

    const TInt count = aParams.Count();
    VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::GetVmbxImagesL count %I",
        count);
    for( TInt i = 0; i < count; i++ )
        {
        CGulIcon *icon = GetVmbxImageL( aParams[i] );
        icons->AppendL( icon );
        }
    CleanupStack::Pop( icons );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::GetVmbxImagesL <=" );
    return icons;
    } */

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::SaveProvisionedEntryL
//
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::SaveProvisionedEntryL( 
                                    const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SaveProvisionedEntryL =>" );
    CVmbxEngineBase* vmbxEngine = NULL;
    iVmbxFactory->CreateEngineL( vmbxEngine, aEntry.VoiceMailboxType() );
    CleanupStack::PushL( vmbxEngine );
    TVoiceMailboxParams params( aEntry );
    vmbxEngine->SaveProvisionedEntryL( aEntry );
    CleanupStack::PopAndDestroy( vmbxEngine );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SaveProvisionedEntryL <=");
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::NotifyChangedL
// notify changed and tell the change to observer
// 
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::NotifyChangedL( TVmbxType aType )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::NotifyChangedL =>" );
    iUiUtilities->DismissDialogL();
    if ( iNotifyCallBack )
        {
        CVmbxEngineBase* vmbxEngine = NULL;
        iVmbxFactory->CreateEngineL( vmbxEngine, aType );
        CleanupStack::PushL( vmbxEngine );
        CVoiceMailboxEntry* entry( NULL );
        TRAPD(err,vmbxEngine->GetL( entry ));
        VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::NotifyChangedL err=%d",err );
        if(err == KErrNotFound)
            {
            delete entry;
            entry = NULL;
            entry = CVoiceMailboxEntry::NewL();
            // get als line info
            entry->SetVmbxAlsLineType( VmbxUtilities::AlsLine() );
            entry->SetVoiceMailboxType( aType );
            entry->SetVmbxNumber( KNullDesC());
            }
        else
            {
            User::LeaveIfError(err);
            }
        CleanupStack::PushL( entry );
        iNotifyCallBack->HandleNotifyL( *entry );
        CleanupStack::PopAndDestroy( entry );           
        CleanupStack::PopAndDestroy( vmbxEngine );
        }
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::NotifyChangedL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::VmbxUiUtilities
// notify changed and tell the change to observer
// 
// ---------------------------------------------------------------------------
//
MVmbxUiUtilities& CVoiceMailboxImpl::VmbxUiUtilities()
    {
    return *iUiUtilities;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::VmbxCenRepHandler
// notify changed and tell the change to observer
// 
// ---------------------------------------------------------------------------
//
MVmbxCenrepHandler& CVoiceMailboxImpl::VmbxCenRepHandler()
    {
    return *iCenRepHandler;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::CheckNumberProvisionedL
// 
// ---------------------------------------------------------------------------
//
void CVoiceMailboxImpl::CheckNumberProvisionedL( 
        const TVoiceMailboxParams& aParams )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::CheckNumberProvisionedL =>" );
    if ( ( EVmbxVoice == aParams.iType )
         || ( EVmbxVideo == aParams.iType ) )
        {
        // get current active process
        RProcess curProcess;
        TUint curProcessId( curProcess.SecureId().iId );
        VMBLOGSTRING2( "VMBX: CVoiceMailboxImpl::CheckNumberProvisionedL \
            Get cur process id: curProcessId = %I", curProcessId );
        // User press 1+send key or long press 1
        if ( KPhoneApplicationUid == curProcessId )
            {
            RPointerArray<CVoiceMailboxEntry> array;
            TCleanupItem item( CleanupRPointerArray, &array );
            CleanupStack::PushL( item );
            GetDefinedEntriesL( array );
            // Only operator has the possibility to configure device, 
            // user is not allowed to edit the voice mail numbers
            if ( array.Count() < 1 )
                {
                // show not allowed user editing dialog
                iUiUtilities->ShowNotAllowedEditingDialogL();
                }
            CleanupStack::PopAndDestroy( &array ); //item 
            } 
        }
    // User don't have access to edit cs voice or video number
    User::Leave( KErrAccessDenied );
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::CheckNumberProvisionedL <=" );
    }

// End of file
