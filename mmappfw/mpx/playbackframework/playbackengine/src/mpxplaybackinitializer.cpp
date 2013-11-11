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
* Description:  Initialize a file before playback
*
*/

#include <mpxcollectionpath.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediadrmdefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxmedia.h>
#include <mpxcollectionplaylist.h>
#include <mpxplaybackplugin.h>
#include <mpxlog.h>
#include <mpxmediageneralextdefs.h>
#include <mpxplaybackpluginversion2.h>

#include "mpxplaybackinitializer.h"
#include "mpxplaybackpluginhandler.h"
#include "mpxplaybackengine.h"
#include "mpxplaybackmediahelper.h"

const TInt KMPXInitTimer = 3000000; // 3 seconds

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer* NewL
// ----------------------------------------------------------------------------
//
CMPXPlaybackInitializer* CMPXPlaybackInitializer::NewL(
    CMPXPlaybackEngine& aEngine,
    CMPXPlaybackMediaHelper& aMediaHelper )
    {
    CMPXPlaybackInitializer* self = new (ELeave)
        CMPXPlaybackInitializer( aEngine, aMediaHelper );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackInitializer::CMPXPlaybackInitializer(
    CMPXPlaybackEngine& aEngine,
    CMPXPlaybackMediaHelper& aMediaHelper )
    : iState( EPbStateNotInitialised ),
      iEngine( aEngine ),
      iMediaHelper( aMediaHelper )
    {
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::ConstructL
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::ConstructL()
    {
    iMedia = CMPXMedia::NewL();
    iTimer = CPeriodic::NewL( CActive::EPriorityIdle );
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackInitializer::~CMPXPlaybackInitializer()
    {
    delete iMedia;
    delete iMediaMessage;
    Close();
    CancelPreInitTimer();
    delete iTimer;
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::Init
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::Init( CMPXCollectionPlaylist& aPlaylist )
    {
    MPX_FUNC_EX("CMPXPlaybackInitializer::Init aPlaylist");

    // Check if we already initialized the song in this playlist,
    // if so then do nothing
    if ( !IsInitializing( aPlaylist ))
        {
        Reset(); // clean up all resources (start again)
        delete iHandler;
        iHandler = NULL;

        // Save the path index and count.  This is just in case the
        // engine calls InitL() again before we make a call to
        // Request media, because these are used to check if we're
        // initializing the same song in the IsInitializing() method.
        iPathIndex = aPlaylist.Path().Index();
        iInitPlaylistCount = iEngine.iPlaylist->Count();
        }
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::Next
// Advance the playlist and start initializing (return ETrue) or if there's
// no next song, does nothing (return EFalse)
// ----------------------------------------------------------------------------
//
TBool CMPXPlaybackInitializer::Next()
    {
    MPX_FUNC("CMPXPlaybackInitializer::Next");

    ASSERT( iEngine.iPlaylist );

    TInt nextPlIndex( KErrNotFound );
    TBool playNext( iEngine.iPlaylist->NextIndex( EFalse, nextPlIndex ));
    Reset(); // Reset state

    // Only pre-init if there is a next song
    if ( playNext )
        {
        MPX_DEBUG1("CMPXPlaybackInitializer::Next(): Starting timer");
        iPathIndex = iEngine.iPlaylist->PathIndex( nextPlIndex );
        iInitPlaylistCount = iEngine.iPlaylist->Count();
        CancelPreInitTimer();
        // Start timer
        iTimer->Start(
            KMPXInitTimer,
            KMPXInitTimer,
            TCallBack(PreInitTimerCallback, this ));
        }
    return playNext;
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::SetActiveL. Swaps the item being handled in the
// initializer to the engine, which takes over in whatever state it currently
// is in
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::SetActiveL()
    {
    ASSERT(iEngine.iPlaylist);
    MPX_DEBUG3("-->CMPXPlaybackInitializer::SetActiveL state %d, error %d",
                iState, iError);
    CancelPreInitTimer();
    switch(iState)
        {
        case EPbStateNotInitialised:
        //
        // From NewL()/InitL()/NextL()/PreviousL()
        // up until the requested media URI is returned.
        // Need to swap the playlist only (no plugin has
        // yet been resolved
        //
            {
            if (iEngine.iPlaylist)
                {
                //
                // Get the URI from the collection - the callback triggers
                // the initialization
                //
                RequestMediaL();
                UpdateMediaForwarding();
                }
            break;
            }

        case EPbStateInitialising:
        //
        // From NewL()/InitL()/NextL()/PreviousL() up
        // until the URI has been returned, plug-in is
        // being intialized OR an error has been returned
        // from playlist
        //
            {
            UpdateMediaForwarding();
            SwapUriL();
            SwapFlag();
            if (iError==KErrNone)
                //
                // Plug-in must be initialising
                //
                {
                SwapPluginL();
                }
            else
                {
                //
                // Hasn't got as far as initialising a plug-in
                //
                iEngine.HandleCollectionMediaL(*iMedia,iError);
                }
            break;
            }

        case EPbStateStopped:
        //
        // From NewL()/InitL()/NextL()/PreviousL() up
        // until the plug-in has been initialised, either
        // successfully or with an error
        //
            {
            UpdateMediaForwarding();
            SwapPluginL();
            SwapUriL();
            SwapFlag();
            if (iError==KErrNone)
                {
                iEngine.HandlePluginEvent(EPInitialised,iDuration,KErrNone);
                }
            else
                {
                iEngine.HandlePluginEvent(EPInitialised,0,iError);
                }
            break;
            }
        default:
            ASSERT(0) ;
        }
    MPX_DEBUG1("<--CMPXPlaybackInitializer::SetActiveL");
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::Close. Resets and destroys all resources owned by
// initializer
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::Close()
    {
    MPX_FUNC("CMPXPlaybackInitializer::Close");
    CancelPreInitTimer();
    Reset(); // must come after handler deletion
    delete iHandler;
    iHandler=NULL;
    }

// ----------------------------------------------------------------------------
// Gets the current item id in use by the initializer
// ----------------------------------------------------------------------------
//
TMPXItemId CMPXPlaybackInitializer::CurrentInitItemId()
    {
    TMPXItemId id( KMPXInvalidItemId );
    if( iState != EPbStateNotInitialised && iEngine.iPlaylist )
        {
        id = iEngine.iPlaylist->Path().Id();
        }
    return id;
    }

// ----------------------------------------------------------------------------
// Implementation uid of the plugin for pre-initialzer
// ----------------------------------------------------------------------------
//
TUid CMPXPlaybackInitializer::PluginUid()
    {
    TUid uid(KNullUid);
    if (iHandler)
        {
        if (iHandler->Plugin())
            {
            uid = iHandler->Plugin()->Uid();
            }
        }
    return uid;
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::Reset. Resets such that next media can be initialised
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::Reset()
    {
    MPX_FUNC("CMPXPlaybackInitializer::Reset");
    //
    // Cancel any activity with current plug-in
    //
    if (iHandler)
        {
        if (iHandler->Plugin())
            {
            TRAP_IGNORE(iHandler->Plugin()->CommandL(EPbCmdStop));
            TRAP_IGNORE(iHandler->Plugin()->CommandL(EPbCmdClose));
            }
        }
    //
    // Reset data associated with previous initialization
    //
    iState = EPbStateNotInitialised;
    iError = KErrNone;
    iDuration = 0;
    iPathIndex = KErrNotFound;
    iInitPlaylistCount = 0;
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::RequestMediaL
// Get the Media from the collection - the callback triggers
// the initialization
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::RequestMediaL( )
    {
    ASSERT(iEngine.iPlaylist);

    MPX_DEBUG3("-->CMPXPlaybackInitializer::RequestMediaL 0x%08x item index %d",
               this, iEngine.iPlaylist->Index());
    // Reset media message
    delete iMediaMessage;
    iMediaMessage = NULL;
    iErrorOfMediaMessage = KErrNone;

    // Setup attribute array
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL(attrs);
    attrs.Append(KMPXMediaGeneralId);
    attrs.Append(KMPXMediaGeneralUri);
    attrs.Append(KMPXMediaGeneralMimeType);
    attrs.Append(KMPXMediaGeneralLastPlaybackPosition);
    attrs.Append(KMPXMediaGeneralFlags);
    attrs.Append(KMPXMediaGeneralDuration);
    attrs.Append(KMPXMediaGeneralTitle);
    attrs.Append( KMPXMediaMusicArtist );

    // Create object to hold attributes and specs
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXCommandContentIdMedia);
    CBufBase* buf = CBufFlat::NewL( KMPXBufGranularity );
    CleanupStack::PushL( buf );

    RBufWriteStream writeStream( *buf );
    CleanupClosePushL( writeStream );
    // externalize attributes array
    ::ExternalizeL(attrs.Array(), writeStream);
    // Close and compress buffer
    writeStream.CommitL();
    buf->Compress();
    CleanupStack::PopAndDestroy( &writeStream );
    TPtrC ptr( MPXUser::Ptr( buf->Ptr( 0 )));
    cmd->SetTextValueL( KMPXCommandMediaAttribute, ptr );

    CMPXAttributeSpecs* attrSpecs( CMPXAttributeSpecs::NewL() );
    CleanupStack::PushL( attrSpecs );
    cmd->SetCObjectValueL<CMPXAttributeSpecs>(
        KMPXCommandMediaAttributeSpecs, attrSpecs );
    CleanupStack::PopAndDestroy( attrSpecs );

    // Instead of copying the whole path object, which may be large due
    // to number of songs in the list, just get the container path and
    // append the item ID to that.  This saves memory and time.
    const CMPXCollectionPath& path( iEngine.iPlaylist->Path() );
    CMPXCollectionPath* initPath( path.ContainerPathL() );
    CleanupStack::PushL( initPath );
    initPath->AppendL( path.IdOfIndex( iPathIndex ) );

    iMediaHelper.MediaL( *initPath,
                         cmd,   // ownership passed
                         this );
    CleanupStack::PopAndDestroy( initPath );
    CleanupStack::PopAndDestroy( buf );
    CleanupStack::Pop( cmd );
    CleanupStack::PopAndDestroy( &attrs );
    iMediaRequested = ETrue;
    MPX_DEBUG2("<--CMPXPlaybackInitializer::RequestMediaL 0x%08x", this);
    }    

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::UpdateMediaForwarding. Updates the forwarding variable
// to send the media to the engine if a media has been requested.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::UpdateMediaForwarding()
    {
    MPX_FUNC("CMPXPlaybackInitializer::UpdateMediaForwarding");

    // Forward the media to the engine if it has been requested.

    // The difference between the iMediaToEngine and iMediaRequested flags,
    // is that if the engine calls this method first (through SetActive()),
    // but the media has not been requested yet, then we want to make sure
    // in that case to not forward the subsequent media requests
    iMediaToEngine = iMediaRequested ? ETrue : EFalse;
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::SwapPluginL. Swaps the plugin being handled in the
// initializer to the engine, which takes over in whatever state it currently
// is in
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::SwapPluginL()
    {
    MPX_DEBUG1("==>CMPXPlaybackInitializer::SwapPluginL");
    // unfinished business with old playback plug-in
    if (iEngine.iPluginHandler->Plugin())
        {
        iEngine.iPluginHandler->Plugin()->CancelRequest();
        }
    TInt index( iEngine.iPlaylist->Index() );
    TMPXPlaybackMessage msg( TMPXPlaybackMessage::EStateChanged,
                             EPbStateInitialising,
                             index );
    iEngine.SetStateL( msg );
    TRAPD(err, iEngine.DoStopL());
    if (err)
        { // play is not available. e.g. power off
        MPX_DEBUG2("CMPXPlaybackInitializer::SwapPluginL stop leave %d", err);
        iError = err; // save error
        }
    else
        {
        if ( iMedia )
            {
            iEngine.RestorePlaybackPositionL( *iMedia );
            iEngine.RestorePlaybackStateL(); 
            }
        //
        // Set plug-in handler
        //
        if ( iHandler )
            {
            CMPXPlaybackPluginHandler*& h=iEngine.iPluginHandler;
            delete h; // delete engine's handler

            // plug-in handler now reports to engine
            iHandler->SetObserver(iEngine); 

            if (iHandler->Plugin())
                {
                iHandler->Plugin()->SetObserver(iEngine);
                }

            h=iHandler; // replace engine's handler
            iHandler=NULL; // reset handler in initializer
            }

        if (iMediaMessage)
            {
            iEngine.HandlePlaybackMessage(iMediaMessage, iErrorOfMediaMessage);
            }
        }
    MPX_DEBUG1("<==CMPXPlaybackInitializer::SwapPluginL");
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::SwapUriL. Swaps the item uri in the engine
// Only in the EPbStateInitialising and EPbStateStopped states
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::SwapUriL()
    {
    // Update the engine's uri to the item that was in the initializer
    //
    delete iEngine.iUri ;
    iEngine.iUri = NULL;
    if ( iMedia->IsSupported( KMPXMediaGeneralUri ))
        {
        iEngine.iUri = iMedia->ValueText(KMPXMediaGeneralUri).AllocL();
        }
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::SwapFlag. Swaps the item db flag in the engine
// Only in the EPbStateInitialising and EPbStateStopped states
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::SwapFlag()
    {
    // Update the engine's flag to the item that was in the initializer
    //
    if ( iMedia->IsSupported( KMPXMediaGeneralFlags ) )
        {
        iEngine.iDbFlag = iMedia->ValueTObjectL<TUint>( KMPXMediaGeneralFlags );
        }
    else // not supported, reset to 0
        {
        iEngine.iDbFlag = 0;
        }

    // Also swap item id
    if ( iMedia->IsSupported( KMPXMediaGeneralId ))
        {
        iEngine.iItemId = iMedia->ValueTObjectL<TMPXItemId>( KMPXMediaGeneralId );
        }
    else
        {
        iEngine.iItemId = KMPXInvalidItemId;
        }

    // Also swap duration value
    if ( iMedia->IsSupported( KMPXMediaGeneralDuration ))
        {
        iEngine.iMediaDuration = iMedia->ValueTObjectL<TInt>( KMPXMediaGeneralDuration );
        }
    else
        {
        iEngine.iMediaDuration = 0;
        }
    iEngine.iAccessPointSet = EFalse;
    if ( iMedia->IsSupported(KMPXMediaGeneralExtAccessPoint) )
        {
        TInt accessPoint = iMedia->ValueTObjectL<TInt>( KMPXMediaGeneralExtAccessPoint );
        iEngine.iAccessPoint = accessPoint;
        if ( accessPoint )
            {
            iEngine.iAccessPointSet = ETrue;
            }
        }
    else
        {
        iEngine.iAccessPoint = 0;
        }
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::IsInitializing
// ----------------------------------------------------------------------------
//
TBool CMPXPlaybackInitializer::IsInitializing(
    const CMPXCollectionPlaylist& aPlaylist)
    {
    MPX_FUNC("CMPXPlaybackInitializer::IsInitializing aPlaylist");
    TBool ret=ETrue;

    if ( !iEngine.iPlaylist )
        {
        ret=EFalse;
        }
    else if ( iPathIndex != aPlaylist.Path().Index() ||
              iInitPlaylistCount != aPlaylist.Count() ||
              aPlaylist.Levels() == 0 )
        {
        ret=EFalse;
        }
    else if ( iHandler )
    //
    // Check that PI obeys the same selection criteria as that of the
    // engine, else we may have the wrong plug-in
    //
        {
        if (iHandler->Plugin())
            {
            TMPXPlaybackPlayerType t1;
            TUid u1;
            TInt i1;
            TPtrC n1;
            iHandler->GetSelection(t1,u1,i1,n1);

            TMPXPlaybackPlayerType t2;
            TUid u2;
            TInt i2;
            TPtrC n2;
            iEngine.iPluginHandler->GetSelection(t2,u2,i2,n2);

            ret=(t1==t2 && u1==u2 && i1==i2);
            }
        }

    MPX_DEBUG3("CMPXPlaybackInitializer::IsInitializing plugin=%08x, ret=%d",
               iHandler ? iHandler->Plugin() : NULL, ret);
    return ret;
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::InitL
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::InitL(const CMPXMedia& aMedia,TInt aError)
    {
    MPX_FUNC("CMPXPlaybackInitializer::InitL(const CMPXMedia& aMedia,TInt aError)");
    iState=EPbStateInitialising;
    if (aError==KErrNone)
        {
        *iMedia = aMedia; // Copy and save media(Uri) from collection

        //
        // No error, get the URI
        //
        const TDesC& uri=aMedia.ValueText(KMPXMediaGeneralUri);
        if (uri.Length())
            {
            //
            // Got the URI, so get a plug-in and go
            //
            if (!iHandler)
                {
                iHandler=CMPXPlaybackPluginHandler::NewL(iEngine.iPluginMonitor,
                                                         *this, *this);
                                                        // arg=plug-in observer
                //
                // Copy selection criteria
                //
                TMPXPlaybackPlayerType t;
                TUid u;
                TInt i;
                TPtrC n;
                iEngine.iPluginHandler->GetSelection(t,u,i,n);

                if (i!=KErrNotFound)
                    {
                    iHandler->SelectSubPlayerL(u,i);
                    }
                else if (u!=KNullUid)
                    {
                    iHandler->SelectPlayerL(u);
                    }
                else
                    {
                    iHandler->SelectPlayersL(t);
                    }
                }           
            
            HBufC8* mimeType =
                    MPXUser::Alloc8L(aMedia.ValueText(KMPXMediaGeneralMimeType));
            CleanupStack::PushL(mimeType);
            iHandler->SelectPlayerL(uri,*mimeType);
            MPX_DEBUG2("CMPXPlaybackInitializer::InitL plugin=%08x",iHandler->Plugin());
            // Plugin handler guarantee to choose a plugin
            // In case there is file open in the audio controller
            iHandler->Plugin()->CommandL(EPbCmdClose);
            InitL(uri, *mimeType); 
            CleanupStack::PopAndDestroy(mimeType);
            }
        else  // No URI
            {
            iError=KErrNotFound;
            }
        }
    else // aError not equal to KErrNone
        {
        iError=aError;

        // Reset media since if there's an error, the media
        // object passed in is unreliable
        delete iMedia;
        iMedia = NULL;
        iMedia = CMPXMedia::NewL();
        }
    }

// ----------------------------------------------------------------------------
// Cancels the pre-init timer
// ----------------------------------------------------------------------------
void CMPXPlaybackInitializer::CancelPreInitTimer()
    {
    MPX_FUNC("CMPXPlaybackInitializer::CancelPreInitTimer()");
    if ( iTimer && iTimer->IsActive() )
        {
        MPX_DEBUG1("CMPXPlaybackInitializer::CancelPreInitTimer(): Timer active, cancelling");
        iTimer->Cancel();
        }
    }

// ----------------------------------------------------------------------------
// Handle pre-init timer callback
// ----------------------------------------------------------------------------
void CMPXPlaybackInitializer::HandlePreInitTimerCallback()
    {
    MPX_FUNC("CMPXPlaybackInitializer::HandlePreInitTimerCallback()");

    CancelPreInitTimer();

    TRAP_IGNORE(RequestMediaL());
    }

// ----------------------------------------------------------------------------
// Callback for pre-init timer.
// ----------------------------------------------------------------------------
TInt CMPXPlaybackInitializer::PreInitTimerCallback(TAny* aPtr)
    {
    MPX_FUNC("CMPXPlaybackInitializer::PreInitTimerCallback()");

    CMPXPlaybackInitializer* ptr =
        static_cast<CMPXPlaybackInitializer*>(aPtr);
    ptr->HandlePreInitTimerCallback();

    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::HandlePluginEventL. Must be non leaving since client has
// no context as this is a callback
// ----------------------------------------------------------------------------

void CMPXPlaybackInitializer::HandlePluginEvent(
    TEvent aEvent,TInt aData,TInt aError)
    {
    TRAP_IGNORE( HandlePluginEventL( aEvent, aData, aError ) );
    }

// ----------------------------------------------------------------------------
// Leaving version of CMPXPlaybackInitializer::HandlePluginEvent
// ----------------------------------------------------------------------------
void CMPXPlaybackInitializer::HandlePluginEventL(
    TEvent aEvent,TInt aData,TInt aError)
    {
    MPX_DEBUG3("CMPXPlaybackInitializer::HandlePluginEventL %d, error %d",
               aEvent, aError);
    if (aEvent==EPInitialised)
        {
        if (aError!=KErrNone)
            {
            iError=aError;
            }
        else
            {
            iDuration=aData;
            if ( iMedia->IsSupported( KMPXMediaGeneralDuration ))
                {
                TInt dur( iMedia->ValueTObjectL<TInt>( KMPXMediaGeneralDuration ));
                if ( dur != iDuration )
                    {
                    // Set duration if it's different than what collection has
                    if ( iEngine.iPlaylist )
                        {
                        if ( iEngine.iPlaylist->Count() && !iEngine.iPlaylist->EmbeddedPlaylist() )
                            {
                            const CMPXCollectionPath& path( iEngine.iPlaylist->Path() );
                            CMPXMedia* media = CMPXMedia::NewL();
                            CleanupStack::PushL(media);
                            media->SetTObjectValueL( KMPXMediaGeneralType,
                                                     EMPXItem );
                            media->SetTObjectValueL( KMPXMediaGeneralCategory,
                                                     EMPXSong );
                            media->SetTObjectValueL( KMPXMediaGeneralCollectionId,
                                                     path.Id(
                                                     CMPXCollectionPath::ECollectionUid ));
                            media->SetTObjectValueL<TMPXItemId>(
                                KMPXMediaGeneralId,
                                iMedia->ValueTObjectL<TMPXItemId>( KMPXMediaGeneralId ) );
                            media->SetTObjectValueL( KMPXMediaGeneralDuration,
                                                     iDuration );
                            iEngine.iPlaylist->SetL( *media );
                            CleanupStack::PopAndDestroy( media );
                            }
                        }
                    }
                }
            }
        iState=EPbStateStopped;
        }
    }

// ----------------------------------------------------------------------------
// Callback from plug-in, handle plugin message
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::HandlePlaybackMessage(
    CMPXMessage* aMsg,
    TInt aErr)
    {
    // Save the media update message
    TRAP_IGNORE(iMediaMessage = CMPXMessage::NewL(*aMsg));
    iErrorOfMediaMessage = aErr;
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::HandleProperty
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::HandleProperty(
    TMPXPlaybackProperty /*aProperty*/,
    TInt /*aValue*/,
    TInt /*aError*/)
    {
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::HandleSubPlayerNames
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::HandleSubPlayerNames(
    TUid /*aPlayer*/,
    const MDesCArray* /*aSubPlayers*/,
    TBool /*aComplete*/,
    TInt /*aError*/)
    {
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::HandleMedia
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::HandleMedia(
    const CMPXMedia& aMedia,
    TInt aError)
    {
    MPX_FUNC("CMPXPlaybackInitializer::HandleMedia()");

    iMediaRequested = EFalse;
    if ( iMediaToEngine )
        {
        TRAP_IGNORE( iEngine.HandleCollectionMediaL( aMedia,iError) );
        iMediaToEngine = EFalse;
        }
    else
        {
        TRAPD( err, InitL( aMedia,aError ));
        if ( err )
            { // Set the error so that not switch to un-initialized plugin
            MPX_DEBUG2("CMPXPlaybackInitializer::HandleMedia TRAP InitL %d",
                       err);
            iError = err; // reset error code to new error
            }
        }
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::HandlePluginHandlerEvent
// Rely on the playback engine to handle the plugin handler events.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::HandlePluginHandlerEvent(
    TPluginHandlerEvents /* aEvent */,
    const TUid& /* aPluginUid */,
    TBool /* aLoaded */,
    TInt /* aData */)
    {
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackInitializer::InitL
// Initialize.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackInitializer::InitL(const TDesC& aUri, const TDesC8& aType ) 
    {
    MPX_FUNC("CMPXPlaybackInitializer::InitL");
    CMPXPlaybackPlugin* p = iHandler->Plugin();
    if ( !p )
        {
        MPX_DEBUG1("CMPXPlaybackInitializer::InitL(): *** Init failed -- No plugin selected");                
        return;
        }
    
    // Check if version2 interface is supported
    CDesCArray* interfaces = iHandler->SupportedInterfacesL( p->Uid() );
    TBool version2InterfaceSupported = EFalse;
    if (interfaces->MdcaCount())
        {
        TInt pos(0);            
        version2InterfaceSupported = !interfaces->FindIsq(KMPXPlaybackPluginVersion2, pos);
        }
    delete interfaces;
    
    if (version2InterfaceSupported)
        {
        // cast the plugin to use our interface
        CMPXPlaybackPluginVersion2* plugin = NULL;
        plugin = static_cast<CMPXPlaybackPluginVersion2*>(p);
        
        // if cast was successful, then init streaming with access point
        if (plugin)
            {            
            TInt accessPoint = 0;
            if ( iMedia->IsSupported(KMPXMediaGeneralExtAccessPoint) ) 
                {
                accessPoint = iMedia->ValueTObjectL<TInt>( KMPXMediaGeneralExtAccessPoint ); 
                MPX_DEBUG2("CMPXPlaybackInitializer::InitL accessPoint %d", accessPoint );
                }            
            
            TInt position = 0;            
            if (iMedia->IsSupported(KMPXMediaGeneralLastPlaybackPosition))
                {
                position = iMedia->ValueTObjectL<TInt>( KMPXMediaGeneralLastPlaybackPosition );
                MPX_DEBUG2("CMPXPlaybackInitializer::InitL position %d", position );
                }
            
                        
            if ( accessPoint ) 
                {
                plugin->InitStreamingL( aUri, aType, accessPoint, position ); 
                }
            else
                {
                plugin->InitialiseWithPositionL(aUri, position); 
                }
            }
        else
            {
            MPX_DEBUG1("CMPXPlaybackInitializer::InitL(): *** Init failed -- failure to convert to expected interface");                
            }
        }
    else
        {
        p->InitialiseL(aUri);
        }
    }

// end of file
