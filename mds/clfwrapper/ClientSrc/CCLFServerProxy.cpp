/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*
*/


// INCLUDE FILES
#include <pathinfo.h>
#include <driveinfo.h>
#include <s32mem.h>
#include <bautils.h>
#include <mdeobjectquery.h>
#include <mdeconstants.h>
#include <collate.h>
#include "CLFConsts.h"
#include "CLFPanics.h"
#include "CCLFServerProxy.h"
#include "mdsutils.h"

// CONSTANTS
const TInt KCLFDefaultBufferLength( 64 );
const TInt KCLFDefaultArrayGranularity( 4 );
const TInt KCLFExtensionArrayGranularity( 51 );

_LIT( KCLFDriveLetterFormatString, ":\\" );
const TInt KCLFDriveC( 'C' );

const TInt KDelayNotityCLFGetUpdateEventMaxTimes = 30;
const TInt KCLFWrapperHarvesterEventInterval = 60;

_LIT( KExtensionMp3,    "mp3" );
_LIT( KExtensionAac,    "aac" );
_LIT( KExtensionAmr,    "amr" );
_LIT( KExtensionAwb,    "awb" );
_LIT( KExtensionMid,    "mid" );
_LIT( KExtensionMidi,   "midi" );
_LIT( KExtensionSpMid,  "spmid" );
_LIT( KExtensionRng,    "rng" );
_LIT( KExtensionMxmf,   "mxmf" );
_LIT( KExtensionWav,    "wav" );
_LIT( KExtensionAu,     "au" );
_LIT( KExtensionWma,    "wma" );
_LIT( KExtensionNrt,    "nrt" );
_LIT( KExtensionRa,    "ra" );

_LIT( KExtJpg, "JPG" );
_LIT( KExtJpeg, "JPEG" );
_LIT( KExtJp2, "JP2" );
_LIT( KExtJ2k, "J2K" ); 
_LIT( KExtJpx, "JPX" ); 
_LIT( KExtJpf, "JPF" );
_LIT( KExtMbm, "MBM" ); 
_LIT( KExtPng, "PNG" );
_LIT( KExtGif, "GIF" );
_LIT( KExtBmp, "BMP" );
_LIT( KExtTif, "TIF" );
_LIT( KExtTiff, "TIFF" );
_LIT( KExtOta, "OTA" );
_LIT( KExtWbmp, "WBMP" );
_LIT( KExtWmf, "WMF" ); 
_LIT( KExtOtb, "OTB" ); 

_LIT(KExtOma1, "dcf");
_LIT(KExtOma2, "odf");
_LIT(KExtOma3, "dm");
_LIT(KExtOma4, "o4a");
_LIT(KExtOma5, "o4v");

_LIT( KExtensionMp4,   "mp4" );
_LIT( KExtensionMpg4,  "mpg4" );
_LIT( KExtensionMpeg4, "mpeg4" );
_LIT( KExtensionM4v,   "m4v" );
_LIT( KExtensionM4a,   "m4a" );
_LIT( KExtension3gp,   "3gp" );
_LIT( KExtension3gpp,  "3gpp" );
_LIT( KExtension3g2,   "3g2" );
_LIT( KExtensionRm,    "rm" );
_LIT( KExtensionRmvb,  "rmvb" );
_LIT( KExtensionRam,   "ram" );
_LIT( KExtensionRv,    "rv" );
_LIT( KExtensionWmv,    "wmv" );
_LIT( KExtensionAvi,    "avi" );
_LIT( KExtensionDivx,    "divx" );
_LIT( KExtensionAsf,    "asf" );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CItemsDeletedHandler::CUpdateItemsHandler
// -----------------------------------------------------------------------------
//
inline CUpdateItemsHandler::CUpdateItemsHandler()
    : iWasNotificationHandled( EFalse ), iParentObserver( NULL ),
      iParentPresentObserver( NULL ), iRemoveObserver( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CUpdateIDsHandler::Start
// -----------------------------------------------------------------------------
//
void CUpdateItemsHandler::StartScheduler()
    {
    if( !iScheduler.IsStarted() )
        {
        iScheduler.Start();
        }
    }

// -----------------------------------------------------------------------------
// CUpdateIDsHandler::AsyncStop
// -----------------------------------------------------------------------------
//
void CUpdateItemsHandler::AsyncStopScheduler()
    {
    iScheduler.AsyncStop();
    }

// -----------------------------------------------------------------------------
// CUpdateIDsHandler::WasNotificationHandled
// -----------------------------------------------------------------------------
//
TBool CUpdateItemsHandler::WasNotificationHandled() const
    {
    return iWasNotificationHandled;
    }

// -----------------------------------------------------------------------------
// CUpdateIDsHandler::WasNotificationHandled
// -----------------------------------------------------------------------------
//
void CUpdateItemsHandler::SetRemoveObserverFlag( TBool aRemove )
    {
    iRemoveObserver = aRemove;
    }

// -----------------------------------------------------------------------------
// CUpdateIDsHandler::NewL
// -----------------------------------------------------------------------------
//
CUpdateIDsHandler* CUpdateIDsHandler::NewL( const TArray< TCLFItemId >& aItemIDArray )
    {
    CUpdateIDsHandler* self = new( ELeave ) CUpdateIDsHandler( aItemIDArray );

    return self;
    }

// -----------------------------------------------------------------------------
// CUpdateIDsHandler::~CUpdateIDsHandler
// -----------------------------------------------------------------------------
//
CUpdateIDsHandler::~CUpdateIDsHandler()
    {
    iIdsPendingUpdate.Reset();

    if ( iScheduler.IsStarted() )
        {
        iScheduler.AsyncStop();
        }
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::~CUpdateFoldersHandler
// -----------------------------------------------------------------------------
//
void CUpdateIDsHandler::StartHandlingL()
    {
    // Pass through.
    }

// -----------------------------------------------------------------------------
// CUpdateIDsHandler::HandleObjectNotification
// -----------------------------------------------------------------------------
//
void CUpdateIDsHandler::HandleObjectNotification( const TItemId aId, TObserverNotificationType /* aType */ )
    {
    iWasNotificationHandled = EFalse;

    const TInt index = iIdsPendingUpdate.FindInOrder( aId );
    if ( index != KErrNotFound )
        {
        iWasNotificationHandled = ETrue;
        iIdsPendingUpdate.Remove( index );
        }
    }

// -----------------------------------------------------------------------------
// CUpdateIDsHandler::AllDone
// -----------------------------------------------------------------------------
//
TBool CUpdateIDsHandler::AllDone() const
    {
    return iIdsPendingUpdate.Count() == 0 ? ETrue : EFalse;
    }

// -----------------------------------------------------------------------------
// CUpdateIDsHandler::CUpdateIDsHandler
// -----------------------------------------------------------------------------
//
inline CUpdateIDsHandler::CUpdateIDsHandler( const TArray< TCLFItemId >& aItemIDArray )
    {
    const TInt idCount( aItemIDArray.Count() );
    for ( TInt i = 0; i < idCount; ++i )
        {
        if ( iIdsPendingUpdate.FindInOrder( aItemIDArray[ i ] ) == KErrNotFound )
            {
            iIdsPendingUpdate.InsertInOrder( aItemIDArray[ i ] );
            }
        }
    }

// -----------------------------------------------------------------------------
// CItemsDeletedHandler::NewL
// -----------------------------------------------------------------------------
//
CItemsDeletedHandler* CItemsDeletedHandler::NewL( CMdESession& aMdESession, 
                                                                                    const CDesCArray& aUriArray,
                                                                                    MMdEObjectObserver* aParentObserver,
                                                                                    MMdEObjectPresentObserver* aParentPresentObserver )
    {
    CItemsDeletedHandler* self = new( ELeave ) CItemsDeletedHandler( aMdESession );
    CleanupStack::PushL( self );
    self->ConstructL( aUriArray, aParentObserver, aParentPresentObserver );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CItemsDeletedHandler::~CUpdateItemsHandlerWithIDs
// -----------------------------------------------------------------------------
//
CItemsDeletedHandler::~CItemsDeletedHandler()
    {
    if( iRemoveObserver )
        {
        TRAP_IGNORE( iMdESession.RemoveObjectObserverL( *iParentObserver ) );
        TRAP_IGNORE( iMdESession.RemoveObjectPresentObserverL( *iParentPresentObserver ) );
        }
    iIdsPendingRemoval.Reset();
    iObjectsPendingRemoval.ResetAndDestroy();
    
    if ( iScheduler.IsStarted() )
        {
        iScheduler.AsyncStop();
        }
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::StartHandlingL
// -----------------------------------------------------------------------------
//
void CItemsDeletedHandler::StartHandlingL()
    {
    const TInt count( iObjectsPendingRemoval.Count() );
    for ( TInt i = 0; i < count; ++i )
        {
        // If object for URI to be removed is in MDS, then remove it from MDS. Obviates
        // the need to rely on unreliable filemonitorplugin.
        const TItemId id = iMdESession.RemoveObjectL( iObjectsPendingRemoval[ i ]->Id() );
        
        if ( id != KNoId )
            {
            iIdsPendingRemoval.InsertInOrder( id );
            }
        }
    iObjectsPendingRemoval.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CItemsDeletedHandler::HandleObjectNotification
// -----------------------------------------------------------------------------
//
void CItemsDeletedHandler::HandleObjectNotification( const TItemId aId, TObserverNotificationType aType )
    {
    iWasNotificationHandled = EFalse;

    if ( aType == ENotifyRemove )
        {
        const TInt index = iIdsPendingRemoval.FindInOrder( aId );
        if ( index != KErrNotFound )
            {
            iWasNotificationHandled = ETrue;
            iIdsPendingRemoval.Remove( index );
            }
        }
    }

// -----------------------------------------------------------------------------
// CUpdateIDsHandler::AllDone
// -----------------------------------------------------------------------------
//
TBool CItemsDeletedHandler::AllDone() const
    {
    return iIdsPendingRemoval.Count() == 0 ? ETrue : EFalse;
    }

// -----------------------------------------------------------------------------
// CItemsDeletedHandler::CItemsDeletedHandler
// -----------------------------------------------------------------------------
//
inline CItemsDeletedHandler::CItemsDeletedHandler( CMdESession& aMdESession )
      : iMdESession( aMdESession )
    {
    }

// -----------------------------------------------------------------------------
// CItemsDeletedHandler::ConstructL
// -----------------------------------------------------------------------------
//
inline void CItemsDeletedHandler::ConstructL( const CDesCArray& aUriArray,
                                                                            MMdEObjectObserver* aParentObserver,
                                                                            MMdEObjectPresentObserver* aParentPresentObserver)
    {
    iParentObserver = aParentObserver;
    iParentPresentObserver = aParentPresentObserver;
    // This leave is needed, if the handler was deleted earlier and the parent observer was removed 
    TRAP_IGNORE( iMdESession.AddObjectObserverL( *iParentObserver ) );
    TRAP_IGNORE( iMdESession.AddObjectPresentObserverL( *iParentPresentObserver ) );
    const TInt uriCount( aUriArray.Count() );
    for ( TInt i = 0; i < uriCount; ++i )
        {
        CMdEObject* object = NULL;

        object = iMdESession.GetObjectL( aUriArray.MdcaPoint( i ));
        CleanupStack::PushL( object );
        if ( object )
            {
            iObjectsPendingRemoval.AppendL( object );
            }
        CleanupStack::Pop( object );
        }
    iRemoveObserver = ETrue;
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::NewL
// -----------------------------------------------------------------------------
//
CUpdateFoldersHandler* CUpdateFoldersHandler::NewL( CMdESession& aMdESession, const CDesCArray& aUriArray,
                                                                                       MMdEObjectObserver* aParentObserver, 
                                                                                       MMdEObjectPresentObserver* aParentPresentObserver, 
																					   RFs& aFs,
																					   RHarvesterClient& aHarvester,
																					   CDesCArray* aSupportedTypes )
    {
    CUpdateFoldersHandler* self = new( ELeave ) CUpdateFoldersHandler( aMdESession, 
                                                                            aFs, aHarvester, aSupportedTypes );
    CleanupStack::PushL( self );
    self->ConstructL( aUriArray, aParentObserver, aParentPresentObserver );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::~CUpdateFoldersHandler
// -----------------------------------------------------------------------------
//
CUpdateFoldersHandler::~CUpdateFoldersHandler()
    {
    if( iRemoveObserver )
        {
        TRAP_IGNORE( iMdESession.RemoveObjectObserverL( *iParentObserver ) );
        TRAP_IGNORE( iMdESession.RemoveObjectPresentObserverL( *iParentPresentObserver ) );
        }
    iUrisFound.Reset();
    iFoldersFound.Reset();
    iIdsPendingRemoval.Reset();
    iIdsPendingUpdate.Reset();
    iIdsHandled.Reset();
    
    if ( iScheduler.IsStarted() )
        {
        iScheduler.AsyncStop();
        }
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::StartHandlingL
// -----------------------------------------------------------------------------
//
void CUpdateFoldersHandler::StartHandlingL()
    {
    CMdEObjectQuery* query = NULL;

    iIdsPendingRemoval.Reset();
    iIdsPendingUpdate.Reset();
    iIdsHandled.Reset();
    
    iStartingHandling = ETrue;
    DoQueryL( query, iFoldersFound );
    CleanupStack::PushL( query );
    if( !iScheduler.IsStarted() )
        {
        iScheduler.Start();
        }
    TRAP_IGNORE( DetermineIdsToRemoveL( *query ) );
    TRAP_IGNORE( DetermineIdsToUpdateL( iUrisFound ) );
    CleanupStack::PopAndDestroy( query );
    iStartingHandling = EFalse;
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::HandleObjectNotification
// -----------------------------------------------------------------------------
//
void CUpdateFoldersHandler::HandleObjectNotification( const TItemId aId, TObserverNotificationType aType )
    {
    iWasNotificationHandled = EFalse;

    if ( aType == ENotifyRemove )
        {
        const TInt index = iIdsPendingRemoval.FindInOrder( aId );
        
        if ( index != KErrNotFound )
            {
            iIdsPendingRemoval.Remove( index );
            iWasNotificationHandled = ETrue;
            }
        }
    else if ( aType == ENotifyAdd || aType == ENotifyModify )
        {
        const TInt index = iIdsPendingUpdate.FindInOrder( aId );
        
        if ( index != KErrNotFound )
            {
            iIdsPendingUpdate.Remove( index );
            const TInt handledIndex = iIdsHandled.FindInOrder( aId );
            if( handledIndex != KErrNotFound )
                {
                iIdsHandled.Remove( handledIndex );
                }
            iWasNotificationHandled = ETrue;
            }
        else if( iHarvestingOngoing )
            { 
            iIdsHandled.InsertInOrder( aId );
            iWasNotificationHandled = ETrue;
            }
        }
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::AllDone
// -----------------------------------------------------------------------------
//
TBool CUpdateFoldersHandler::AllDone() const
    {
    return ( iIdsPendingRemoval.Count() == 0 &&
             iIdsPendingUpdate.Count() == 0 &&
             !iStartingHandling ) ? ETrue : EFalse;
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::HandleQueryNewResults
// -----------------------------------------------------------------------------
//
void CUpdateFoldersHandler::HandleQueryNewResults( CMdEQuery& /* aQuery */, TInt /* aFirstNewItemIndex */, TInt /* aNewItemCount */ )
    {
    // Pass through.
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::HandleQueryCompleted
// -----------------------------------------------------------------------------
//
void CUpdateFoldersHandler::HandleQueryCompleted( CMdEQuery& /* aQuery */, TInt /* aError */ )
    {
    iScheduler.AsyncStop();
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::CUpdateFoldersHandler
// -----------------------------------------------------------------------------
//
inline CUpdateFoldersHandler::CUpdateFoldersHandler( CMdESession& aMdESession, 
                                                                                         RFs& aFs,
                                                                                         RHarvesterClient& aHarvester,
                                                                                         CDesCArray* aSupportedTypes )
    : iFs( aFs ),
      iUrisFound( KCLFDefaultArrayGranularity ),
      iFoldersFound( KCLFDefaultArrayGranularity ),
      iMdESession( aMdESession ),
      iHarvester( aHarvester ),
      iHarvestError( KErrNone ),
      iStartingHandling( EFalse ),
      iExtensionArray( aSupportedTypes ),
      iHarvestingOngoing( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::ConstructL
// -----------------------------------------------------------------------------
//
inline void CUpdateFoldersHandler::ConstructL( const CDesCArray& aUriArray,
                                                                             MMdEObjectObserver* aParentObserver,
                                                                             MMdEObjectPresentObserver* aParentPresentObserver )
    {
    iParentObserver = aParentObserver;
    iParentPresentObserver = aParentPresentObserver;
    // This leave is needed, if the handler was deleted earlier and the parent observer was removed 
    TRAP_IGNORE( iMdESession.AddObjectObserverL( *iParentObserver ) );
    TRAP_IGNORE( iMdESession.AddObjectPresentObserverL( *iParentPresentObserver ) );

    for ( TInt i = 0; i < aUriArray.MdcaCount(); ++i )
        {
        ScanNodeForUrisL( aUriArray.MdcaPoint( i ), iUrisFound, iFoldersFound );
        }
 
    iRemoveObserver = ETrue ;
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::ScanNodeForUrisL
// -----------------------------------------------------------------------------
//
void CUpdateFoldersHandler::ScanNodeForUrisL( const TDesC16& aNodeName, CDesCArray& aUriArray, CDesCArray& aFolderArray )
    {
    TBool isFolder = EFalse;

    BaflUtils::IsFolder( iFs, aNodeName, isFolder );
    if ( isFolder )
        {
        if ( !BaflUtils::FolderExists( iFs, aNodeName ) )
            {
            return;
            }
        CDirScan* dirscan = CDirScan::NewL( iFs );
        CDir* dir = NULL;

        CleanupStack::PushL( dirscan );
        dirscan->SetScanDataL( aNodeName, KEntryAttNormal, EDirsAnyOrder );

        for ( dirscan->NextL( dir ); dir; dirscan->NextL( dir ) )
            {
            CleanupStack::PushL( dir );

            for ( TInt i = 0 ; i < dir->Count() ; ++i )
                {
                const TEntry& entry = ( *dir )[ i ];
                const TInt totalLength = entry.iName.Length() + dirscan->FullPath().Length() + 1;

                if (( !entry.IsDir() ) && ( totalLength <= KMaxFileName ))
                    {
                    TFileName currentPath( dirscan->FullPath() );
                    currentPath.Append( ( *dir )[ i ].iName );
                    aUriArray.AppendL( currentPath );
                    }
                }
            CleanupStack::PopAndDestroy( dir );

            }
        aFolderArray.AppendL( aNodeName );

        CleanupStack::PopAndDestroy( dirscan );
        }
    else
        {
        aUriArray.AppendL( aNodeName );
        }
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::DetermineIdsToRemove
// -----------------------------------------------------------------------------
//
void CUpdateFoldersHandler::DetermineIdsToRemoveL ( const CMdEObjectQuery& aQuery )
    {
    RProcess process( KCurrentProcessHandle );
    process.Id().Id(); 
    if( process.HasCapability( ECapabilityWriteDeviceData ) )
        {
        const TInt queryCount( aQuery.Count() );
        for ( TInt i = 0; i < queryCount; ++i )
            {
            CMdEObject& object = aQuery.Result( i );
            TEntry entry;
            const TInt err = iFs.Entry( object.Uri(), entry );

            if ( err == KErrNotFound )
                {
                // Queried metadata object has a URI that does not match a filesystem URI.
                // It must therefore be removed.
                TItemId id( KNoId ); 
                id = iMdESession.RemoveObjectL( object.Id() );
                if ( id != KNoId )
                    {
                    iIdsPendingRemoval.InsertInOrder( id );
                    }
                }
            }
        }
    process.Close();
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::DetermineIdsToRemove
// -----------------------------------------------------------------------------
//
void CUpdateFoldersHandler::DetermineIdsToUpdateL ( const CDesCArray& aUriArray )
    {
    const TInt uriCount( aUriArray.Count() );
    for ( TInt i = 0; i < uriCount; ++i )
        {
        CMdEObject* object = NULL;
        const TDesC& uri = aUriArray.MdcaPoint( i );
        TPtrC ext;
        const TBool exists( MdsUtils::GetExt( uri, ext ) );
        if( !exists || !IsSupportedType( ext ) )
            {
            return;
            }

        object = iMdESession.GetObjectL( uri ); 
        CleanupStack::PushL( object );

        if ( !object )
            {
            // If object for given URI is not in MDS, then determine if an ENotifyAdd
            // or ENotifyModify  is pending by attempting to harvest the file associated
            // with the URI.
            DoHarvestL( uri );
            }
        CleanupStack::PopAndDestroy( object );
        }
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::DoQueryL
// -----------------------------------------------------------------------------
//
void CUpdateFoldersHandler::DoQueryL( CMdEObjectQuery*& aQuery, const CDesCArray& aFolderArray )
    {
    CMdENamespaceDef& defaultNamespaceDef = iMdESession.GetDefaultNamespaceDefL();
    CMdEObjectDef& objDef = defaultNamespaceDef.GetObjectDefL( MdeConstants::Object::KBaseObject );
    aQuery = iMdESession.NewObjectQueryL( iMdESession.GetDefaultNamespaceDefL(), objDef, this );
    CleanupStack::PushL( aQuery );
    CMdELogicCondition& uriLogicCond = aQuery->Conditions().AddLogicConditionL( ELogicConditionOperatorOr );

    const TInt folderCount( aFolderArray.Count() );
    for ( TInt i = 0; i < folderCount; ++i )
        {
        uriLogicCond.AddObjectConditionL( EObjectConditionCompareUriBeginsWith, aFolderArray.MdcaPoint( i ) );
        }
    aQuery->FindL();

    CleanupStack::Pop( aQuery ); // Caller takes ownership.
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::DoHarvest
// -----------------------------------------------------------------------------
//
void CUpdateFoldersHandler::DoHarvestL( const TDesC& aUri )
    {
    RArray< TItemId > albumIds;

    TUid uid( KNullUid );
    iHarvester.SetObserver( this );
    iHarvestingFile = TFileName( aUri );
    iHarvestingOngoing = ETrue;
    iHarvester.HarvestFileWithUID( aUri, albumIds, EFalse, uid );
    if( !iScheduler.IsStarted() )
        {
        iScheduler.Start();
        }

    albumIds.Close();
    
    // Wait until harvest complete before trying again.
    iHarvester.RemoveObserver( this );
    if ( iHarvestError == KErrNone )
        {
        CMdEObject* object = NULL;

        object = iMdESession.GetObjectL( aUri );
        if( object )
            {
            const TInt index = iIdsHandled.FindInOrder( object->Id() );
            if( index != KErrNotFound )
                {
                iIdsHandled.Remove( index );
                }
            else
                {
                iIdsPendingUpdate.InsertInOrder( object->Id() ); 
                }
            }
        }

    iHarvestError = KErrNone;
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::HarvestingComplete
// Callback from harvester client after harvesting
// -----------------------------------------------------------------------------
//
void CUpdateFoldersHandler::HarvestingComplete( TDesC& aURI, const TInt aError )
    {
    TBool match( EFalse );
    TCollationMethod m = *Mem::CollationMethodByIndex( 0 );

    iHarvestError = aError;
    m.iFlags = ( TCollationMethod::EIgnoreNone | TCollationMethod::EFoldCase );   

    const TFileName uri( aURI );
    
    if ( uri.CompareC( iHarvestingFile, 3, &m ) == 0 )
        {
        match = ETrue;
        iHarvestingOngoing = EFalse;
        }
    if ( iScheduler.IsStarted() && match )
        {
        iScheduler.AsyncStop();
        }
    }

// -----------------------------------------------------------------------------
// CUpdateFoldersHandler::IsSupportedType
// -----------------------------------------------------------------------------
//
TBool CUpdateFoldersHandler::IsSupportedType( const TDesC& aExtension )
    {
    TInt pos( 0 );
    if ( iExtensionArray->FindIsq( aExtension, pos ) == 0 ) // found
        {
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::~CCLFServerProxy
// Destructor.
// -----------------------------------------------------------------------------
//
CCLFServerProxy::~CCLFServerProxy()
    {
    // Reset the arrays this class owns.
    iStatusArray.Reset();
    iOpCodeArray.Reset();
    iUpdatedIds.Reset();
    TRAP_IGNORE( iMdESession.RemoveObjectObserverL( *this ));
    TRAP_IGNORE( iMdESession.RemoveObjectPresentObserverL( *this ));
    iUpdateItemsHandlerArray.ResetAndDestroy();
    delete iUriArray;
    iUriArray = NULL;
    delete iExtensionArray;
    iExtensionArray = NULL;
    iFs.Close();

    iHC.RemoveHarvesterEventObserver( *this );
    iHC.Close();
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::NewL
// -----------------------------------------------------------------------------
//
CCLFServerProxy* CCLFServerProxy::NewL( CMdESession& aMdESession )
    {
    CCLFServerProxy* self = new( ELeave ) CCLFServerProxy( aMdESession );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::GetUpdateEndEvent
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::GetUpdateEndEvent( TRequestStatus& aRequestStatus )
    {
    GetEvent( aRequestStatus, ECLFGetUpdateEvent );
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::CancelEventGets
// -----------------------------------------------------------------------------
//
TInt CCLFServerProxy::CancelGetEvent()
    {
    CancelGetEvent( ECLFGetUpdateEvent );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::GetUpdateStartEvent
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::GetUpdateStartEvent( TRequestStatus& aRequestStatus )
    {
    GetEvent( aRequestStatus, ECLFProcessStartEvent );
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::CancelGetUpdateStartEvent
// -----------------------------------------------------------------------------
//
TInt CCLFServerProxy::CancelGetUpdateStartEvent()
    {
    CancelGetEvent( ECLFProcessStartEvent );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::FetchItemListData
// -----------------------------------------------------------------------------
//
TInt CCLFServerProxy::FetchItemListData( RArray< TCLFItemId >& aItemIDArray )
    {
    const TInt count( iUpdatedIds.Count() );
    for ( TInt i = 0; i < count; ++i )
        {
        aItemIDArray.Append( iUpdatedIds[ i ] );
        }

    iUpdatedIds.Reset();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::UpdateItems
// -----------------------------------------------------------------------------
//
TInt CCLFServerProxy::UpdateItemsL( const TArray< TCLFItemId >& aItemIDArray )
    {
    CUpdateItemsHandler* handler = CUpdateIDsHandler::NewL( aItemIDArray );

    // Ownership tranferred
    StartHandlingL( handler );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::UpdateItems
// -----------------------------------------------------------------------------
//
TInt CCLFServerProxy::UpdateItemsL( const TInt aSemanticId, const TDesC8& aOpaqueData )
    {
    CDesCArray* uriArray = new ( ELeave ) CDesCArraySeg( KCLFDefaultArrayGranularity );
    CleanupStack::PushL( uriArray );
    DeSerializeL( aOpaqueData, *uriArray );

    if ( uriArray->Count() )
        {
        CUpdateItemsHandler* handler = NULL;
        
        if ( aSemanticId == KCLFItemsDeletedSemanticId )
            {
            handler = CItemsDeletedHandler::NewL( iMdESession, *uriArray, this, this );
            }
        else if ( aSemanticId == KCLFUpdateFoldersSemanticId )
            {
            handler = CUpdateFoldersHandler::NewL( iMdESession, *uriArray, this, this, 
                              iFs, iHC, iExtensionArray );
            }
        else if ( aSemanticId == KCLFUpdateCollectionsSemanticId )
            {
            // Pass through; collections are not supported.
            CleanupStack::PopAndDestroy( uriArray );
            return KErrNone;
            }

        // Ownership tranferred
        StartHandlingL( handler );
        }
    CleanupStack::PopAndDestroy( uriArray );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::UpdateAllItems
// -----------------------------------------------------------------------------
//
TInt CCLFServerProxy::UpdateAllItemsL()
    {
    CUpdateItemsHandler* handler = CUpdateFoldersHandler::NewL( iMdESession, 
                                                         *iUriArray, this, this, iFs, iHC, iExtensionArray );
    // Ownership tranferred
    StartHandlingL( handler );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
inline CCLFServerProxy::CCLFServerProxy( CMdESession& aMdESession )
    : iMdESession( aMdESession ),
    iUriArray( NULL ),
    iExtensionArray( NULL ),
    iHEStateFinished( ETrue ),
    iNeedNotifyCLFGetUpdateEvent( EFalse ),
    iDelayNotifyCLFGetUpdateEventCount( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::ConstructL
// -----------------------------------------------------------------------------
//
inline void CCLFServerProxy::ConstructL()
    {
    iUriArray = new ( ELeave ) CDesCArraySeg( KCLFDefaultArrayGranularity );
    User::LeaveIfError( iFs.Connect() );
    
    TDriveList driveList;
    TChar driveLetter; 
    TInt numOfElements( 0 );
    User::LeaveIfError( DriveInfo::GetUserVisibleDrives( iFs, 
                                                         driveList, 
                                                         numOfElements, 
                                                         KDriveAttExclude | KDriveAttRemote | KDriveAttRom ) );
    
    for( TInt driveNumber = EDriveA ; driveNumber <= EDriveZ ; driveNumber++ ) 
        { 
        if ( driveList[driveNumber] ) 
            {
            User::LeaveIfError( iFs.DriveToChar( driveNumber, driveLetter ) );
            HBufC* letter = HBufC::NewLC( KMaxPath );
            TPtr letterPtr( letter->Des() );

            if( driveLetter == KCLFDriveC )
                {
                letterPtr = PathInfo::PhoneMemoryRootPath();
                }
            else
                {
                letterPtr.Append( driveLetter );
                letterPtr.Append( KCLFDriveLetterFormatString ); 
                }   
                        
            if( BaflUtils::PathExists( iFs, letterPtr ) )
                {
                iUriArray->AppendL( letterPtr );
                }
            CleanupStack::PopAndDestroy( letter );
            }
        }
    
    PopulateSupportedExtensionL();
    
    iMdESession.AddObjectObserverL( *this );
    iMdESession.AddObjectPresentObserverL( *this );

    User::LeaveIfError( iHC.Connect() );
    iHC.AddHarvesterEventObserver( *this, EHEObserverTypeOverall, KCLFWrapperHarvesterEventInterval );

    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::HarvestingUpdated
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::HarvestingUpdated( 
			HarvesterEventObserverType /*aHEObserverType*/, 
			HarvesterEventState aHarvesterEventState,
			TInt /*aItemsLeft*/ )
    {
    if( aHarvesterEventState == EHEStateFinished ||
        aHarvesterEventState == EHEStatePaused )
        {
        iHEStateFinished = ETrue;
        if ( iNeedNotifyCLFGetUpdateEvent )
            {
            NotifyUpdateEvent( ECLFGetUpdateEvent );
            }
        }
    else
        {
        iHEStateFinished = EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::StartHandlingL
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::StartHandlingL( CUpdateItemsHandler* aHandler )
    {
    if ( aHandler )
        {
        CleanupStack::PushL( aHandler );
        // Notify pending active objects about the event.
        NotifyUpdateEvent( ECLFProcessStartEvent );

        // Now wait until the update is over, then return.
        iUpdateItemsHandlerArray.Append( aHandler );
        TRAPD( err, aHandler->StartHandlingL(); );
        if( err != KErrNone )
            {
            // Remove aHandler, because it is in clean up stack.
            iUpdateItemsHandlerArray.Remove( iUpdateItemsHandlerArray.Count() - 1 );
            CleanupStack::PopAndDestroy( aHandler );
            User::LeaveIfError( err );
            }
        if ( aHandler->AllDone() )
            {
            CleanupStack::PopAndDestroy( aHandler );
            iUpdateItemsHandlerArray.Remove( iUpdateItemsHandlerArray.Count() - 1 );
            }
        else
            {
            CleanupStack::Pop( aHandler );
            aHandler->StartScheduler();
            }
        }
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::HandleObjectNotification
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::HandleObjectNotification( CMdESession& /*aSession*/,
                                                TObserverNotificationType aType,
                                                const RArray< TItemId >& aObjectIdArray )
    {
    TBool unhandledIds = EFalse;

    const TInt objectCount( aObjectIdArray.Count() );
    for ( TInt i = 0; i < objectCount; ++i )
        {
        TBool handled = EFalse;

        iUpdatedIds.Append( aObjectIdArray[ i ] );
        for ( TInt j = 0; j < iUpdateItemsHandlerArray.Count(); ++j )
            {
            iUpdateItemsHandlerArray[ j ]->HandleObjectNotification( aObjectIdArray[ i ], aType );
            handled = handled ? ETrue : iUpdateItemsHandlerArray[ j ]->WasNotificationHandled();

            if ( iUpdateItemsHandlerArray[ j ]->AllDone() )
                {
                // If we have a pending update, we can now let it continue.
                iDelayNotifyCLFGetUpdateEventCount = KDelayNotityCLFGetUpdateEventMaxTimes;
                NotifyUpdateEvent( ECLFGetUpdateEvent );
                iUpdateItemsHandlerArray[ j ]->AsyncStopScheduler();
                iUpdateItemsHandlerArray[j]->SetRemoveObserverFlag( EFalse );
                delete iUpdateItemsHandlerArray[ j ]; 
                iUpdateItemsHandlerArray[ j ] = NULL;
                iUpdateItemsHandlerArray.Remove( j );
                j--; // Compensate for the removed handler
                }
            }
        unhandledIds = ( unhandledIds || !handled ) ? ETrue : EFalse;
        }

    if ( unhandledIds )
        {
        // If we have an ID that doesn't match a pending update, we assume
        // that the object notification was generated from outside the current
        // application, and we therefore must simulate a process end event.
        if( aType == ENotifyRemove )
            {
            // Increase the update triggering value more rapidly to provide remove events faster
            iDelayNotifyCLFGetUpdateEventCount += 12;
            }
        NotifyUpdateEvent( ECLFGetUpdateEvent );
        }
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::MMdEObjectPresentObserver
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::HandleObjectPresentNotification( CMdESession& /*aSession*/,
                                                TBool /*aPresent*/,
                                                const RArray< TItemId >& aObjectIdArray )
    {
    TBool unhandledIds = EFalse;

    const TInt objectCount( aObjectIdArray.Count() );
    for ( TInt i = 0; i < objectCount; ++i )
        {
        TBool handled = EFalse;

        iUpdatedIds.Append( aObjectIdArray[ i ] );
        for ( TInt j = 0; j < iUpdateItemsHandlerArray.Count(); ++j )
            {
            iUpdateItemsHandlerArray[ j ]->HandleObjectNotification( aObjectIdArray[ i ], ENotifyModify );
            handled = handled ? ETrue : iUpdateItemsHandlerArray[ j ]->WasNotificationHandled();

            if ( iUpdateItemsHandlerArray[ j ]->AllDone() )
                {
                // If we have a pending update, we can now let it continue.
                NotifyUpdateEvent( ECLFGetUpdateEvent );
                iUpdateItemsHandlerArray[ j ]->AsyncStopScheduler();
                iUpdateItemsHandlerArray[j]->SetRemoveObserverFlag( EFalse );
                delete iUpdateItemsHandlerArray[ j ]; 
                iUpdateItemsHandlerArray[ j ] = NULL;
                iUpdateItemsHandlerArray.Remove( j );
                j--; // Compensate for the removed handler
                }
            }
        unhandledIds = ( unhandledIds || !handled ) ? ETrue : EFalse;
        }

    if ( unhandledIds )
        {
        // If we have an ID that doesn't match a pending update, we assume
        // that the object notification was generated from outside the current
        // application, and we therefore must simulate a process end event.
        NotifyUpdateEvent( ECLFGetUpdateEvent );
        }
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::NotifyUpdateEvent
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::NotifyUpdateEvent( const TCLFServerOpCodes aOpCode )
    {

    if ( ECLFGetUpdateEvent == aOpCode
            && !iHEStateFinished
            && iDelayNotifyCLFGetUpdateEventCount < KDelayNotityCLFGetUpdateEventMaxTimes )
        {
        iNeedNotifyCLFGetUpdateEvent = ETrue;
        iDelayNotifyCLFGetUpdateEventCount++;
        return;
        }

    for ( TInt i = 0; i < iOpCodeArray.Count(); ++i )
        {
        if ( iOpCodeArray[ i ] == aOpCode )
            {
            User::RequestComplete( iStatusArray[ i ], KErrNone );
            iStatusArray.Remove( i );
            iOpCodeArray.Remove( i );
            }
        }
    
    if ( ECLFGetUpdateEvent == aOpCode )
        {
        iNeedNotifyCLFGetUpdateEvent = EFalse;
        iDelayNotifyCLFGetUpdateEventCount = 0;
        }
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::DeSerializeL
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::DeSerializeL( const TDesC8& aData, CDesCArray& aDataArray )
    {
    // Check if KNullDesC
    if ( aData.Length() > 0 )
        {
        RDesReadStream readStream( aData );
        CleanupClosePushL( readStream );
        const TInt count( readStream.ReadInt32L() );

        HBufC* buffer = HBufC::NewLC( KCLFDefaultBufferLength );
        TPtr ptr( buffer->Des() );
        for ( TInt i = 0 ; i < count ; ++i )
            {
            const TInt length( readStream.ReadInt32L() );
            const TUint uintLength( length );
            const TInt maxLength( KMaxTInt / 2 );
            if (( length < 0 ) || ( uintLength > maxLength ))
                {
                User::Leave( KErrNotSupported );
                }
            else if ( length > ptr.MaxLength() )
                {
                // create new buffer
                CleanupStack::PopAndDestroy( buffer );
                buffer = HBufC::NewLC( length );
                ptr.Set( buffer->Des() );
                }
            else
                {
                ptr.Zero();
                }
            readStream.ReadL( ptr, length );
            aDataArray.AppendL( ptr );
            }
        CleanupStack::PopAndDestroy( 2, &readStream );
        }
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::GetEvent
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::GetEvent( TRequestStatus& aRequestStatus, TCLFServerOpCodes aOpcode )
    {
    iStatusArray.Append( &aRequestStatus );
    iOpCodeArray.Append( aOpcode );
    aRequestStatus = KRequestPending;
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::CancelGetEvent
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::CancelGetEvent( TCLFServerOpCodes aOpcode )
    {
    for ( TInt i = 0; i < iStatusArray.Count(); ++i )
        {
        if ( iOpCodeArray[ i ] == aOpcode )
            {
            User::RequestComplete( iStatusArray[ i ], KErrCancel );
            iStatusArray.Remove( i );
            iOpCodeArray.Remove( i );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCLFServerProxy::PopulateSupportedExtensionL
// -----------------------------------------------------------------------------
//
void CCLFServerProxy::PopulateSupportedExtensionL()
    {
    iExtensionArray = new ( ELeave ) CDesCArraySeg( KCLFExtensionArrayGranularity );
    iExtensionArray->InsertIsqL( KExtensionMp3 );
    iExtensionArray->InsertIsqL( KExtensionAac );
    iExtensionArray->InsertIsqL( KExtensionAmr );
    iExtensionArray->InsertIsqL( KExtensionAwb );
    iExtensionArray->InsertIsqL( KExtensionMid );
    iExtensionArray->InsertIsqL( KExtensionMidi );
    iExtensionArray->InsertIsqL( KExtensionSpMid );
    iExtensionArray->InsertIsqL( KExtensionRng );
    iExtensionArray->InsertIsqL( KExtensionMxmf );
    iExtensionArray->InsertIsqL( KExtensionWav );
    iExtensionArray->InsertIsqL( KExtensionAu );
    iExtensionArray->InsertIsqL( KExtensionNrt );
    iExtensionArray->InsertIsqL( KExtensionWma );
    iExtensionArray->InsertIsqL( KExtensionRa );
    
    iExtensionArray->InsertIsqL( KExtJpg );
    iExtensionArray->InsertIsqL( KExtJpeg );
    iExtensionArray->InsertIsqL( KExtJp2 );
    iExtensionArray->InsertIsqL( KExtJ2k );
    iExtensionArray->InsertIsqL( KExtJpx );
    iExtensionArray->InsertIsqL( KExtJpf );
    iExtensionArray->InsertIsqL( KExtMbm );
    iExtensionArray->InsertIsqL( KExtPng );
    iExtensionArray->InsertIsqL( KExtGif );
    iExtensionArray->InsertIsqL( KExtBmp );
    iExtensionArray->InsertIsqL( KExtTif );
    iExtensionArray->InsertIsqL( KExtTiff );
    iExtensionArray->InsertIsqL( KExtOta );
    iExtensionArray->InsertIsqL( KExtWbmp );
    iExtensionArray->InsertIsqL( KExtWmf );
    iExtensionArray->InsertIsqL( KExtOtb );
    
    iExtensionArray->InsertIsqL( KExtOma1 );
    iExtensionArray->InsertIsqL( KExtOma2 );
    iExtensionArray->InsertIsqL( KExtOma3 );
    iExtensionArray->InsertIsqL( KExtOma4 );
    iExtensionArray->InsertIsqL( KExtOma5 );
    
    iExtensionArray->InsertIsqL( KExtensionMp4 );
    iExtensionArray->InsertIsqL( KExtensionMpg4 );
    iExtensionArray->InsertIsqL( KExtensionMpeg4 );
    iExtensionArray->InsertIsqL( KExtensionM4v );
    iExtensionArray->InsertIsqL( KExtensionM4a );
    iExtensionArray->InsertIsqL( KExtension3gp );
    iExtensionArray->InsertIsqL( KExtension3gpp );
    iExtensionArray->InsertIsqL( KExtension3g2 );
    iExtensionArray->InsertIsqL( KExtensionRm );
    iExtensionArray->InsertIsqL( KExtensionRmvb );
    iExtensionArray->InsertIsqL( KExtensionRam );
    iExtensionArray->InsertIsqL( KExtensionRv );
    iExtensionArray->InsertIsqL( KExtensionWmv );
    iExtensionArray->InsertIsqL( KExtensionAvi );
    iExtensionArray->InsertIsqL( KExtensionDivx );
    iExtensionArray->InsertIsqL( KExtensionAsf );
    }

//  End of File
