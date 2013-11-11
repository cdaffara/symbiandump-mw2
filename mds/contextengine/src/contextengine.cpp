/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  An engine to collect context related metadata.
*
*/


// this class contains use of memory allocation and release not
// conforming to RAII principles

#include <e32uid.h>
#include <ecom.h>
#include <e32svr.h>

#include "contextengine.h"
#include "contextengineao.h"
#include "contextsnapshotitem.h"
#include "mdsutils.h"
#include "harvesterlog.h"
#include "harvesterdata.h"

/// Key for thread-local storage used by this singleton.
const TInt KContextEngineTLSKey = 0x200009f6;

// construct/destruct

// ---------------------------------------------------------------------------
// 1st phase C++ construction.
// Don't export these, because used only by our NewLC() and the like.
// ---------------------------------------------------------------------------
//
CContextEngine::CContextEngine( ) :
    iContextEngineAO( NULL ), iArrayCount( 0 ), iProcessedArrayCount( 0 )
    {
    WRITELOG( "CContextEngine::CContextEngine" ); // DEBUG INFO
    }

// ---------------------------------------------------------------------------
// 2nd phase construction
// ---------------------------------------------------------------------------
//
void CContextEngine::ConstructL( MContextInitializationObserver* aObserver )
    {
    WRITELOG( "CContextEngine::ConstructL" ); // DEBUG INFO
    iContextEngineAO = CContextEngineAO::NewL( aObserver, this );
    }

// ---------------------------------------------------------------------------
// Get a reference to this singleton. A new object is created if needed.
// If an observer object to notify is given, context plugins are initialized
// asyncronously and observer is notified when ready.
// ---------------------------------------------------------------------------
//
EXPORT_C CContextEngine* CContextEngine::GetInstanceL(
	MContextInitializationObserver* aObserver )
    {
    WRITELOG( "CContextEngine::GetInstanceL" ); // DEBUG INFO
    CContextEngineStaticData* data = static_cast<CContextEngineStaticData*>(
        UserSvr::DllTls(KContextEngineTLSKey) );
    CContextEngine* instance = NULL;
    if ( !data )
        {
        instance = new (ELeave) CContextEngine();
        CleanupStack::PushL( instance );
        instance->ConstructL( aObserver );

        UserSvr::DllSetTls( KContextEngineTLSKey,
        	new (ELeave) CContextEngineStaticData(instance) );

        CleanupStack::Pop( instance );
        }
    else
        {
        instance = data->iContextEngine;
        data->iRefCount++;
        if ( aObserver )
            {
            aObserver->ContextInitializationStatus( KErrNone );
            }
        }

    return instance;
    }

// ---------------------------------------------------------------------------
// Release a reference to this singleton.
// ---------------------------------------------------------------------------
//
EXPORT_C void CContextEngine::ReleaseInstance()
    {
    WRITELOG( "CContextEngine::ReleaseInstance" ); // DEBUG INFO
    CContextEngineStaticData* data =
        static_cast<CContextEngineStaticData*>( UserSvr::DllTls(KContextEngineTLSKey) );
    if ( data )
        {
        data->iRefCount--;
        if ( data->iRefCount <= 0 )
            {
            // destroy the singleton and free TLS
            delete data;
            data = NULL;
            UserSvr::DllFreeTls( KContextEngineTLSKey );
            }
        }
    }

/* ---------------------------------------------------------------------------
 * Ignores the reference count and destroys this singleton.            .   .
 * Should be used only if client code is not using ReleaseInstance()  /|\ /|\
 * to release their handles to this singleton.                         |   |
 *                                                                     |   |
 *                                                        			  -------
 *                                                                	  -------
 * ---------------------------------------------------------------------------
*/
EXPORT_C void CContextEngine::Destroy()
    {
    WRITELOG( "CContextEngine::Destroy" ); // DEBUG INFO
    CContextEngineStaticData* data =
        static_cast<CContextEngineStaticData*>( UserSvr::DllTls(KContextEngineTLSKey) );
    if ( data )
        {
        // destroy the singleton and free TLS
        delete data;
        data = NULL;
        UserSvr::DllFreeTls( KContextEngineTLSKey );
        }
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CContextEngine::~CContextEngine()
    {
    WRITELOG( "CContextEngine::~CContextEngine" ); // DEBUG INFO
    delete iContextEngineAO;
    iContextEngineAO = NULL;

    MdsUtils::CleanupPtrArray<CContextSnapshotItem>( &iSnapshotQueue );
    REComSession::FinalClose(); // we are done
    }

// ---------------------------------------------------------------------------
// Set MdeSession to context engine and it's plugins.
// Session must be set in order to successfully harvest context data.
// ---------------------------------------------------------------------------
//
EXPORT_C void CContextEngine::SetMdeSession( CMdESession* aSession )
    {
    iContextEngineAO->SetMdeSession( aSession );
    }

// ---------------------------------------------------------------------------
// Takes a context snapshot including all initialized plugins.
// ---------------------------------------------------------------------------
//
EXPORT_C void CContextEngine::ContextSnapshot( MContextSnapshotObserver& aObserver,
    CHarvesterData& aHD )
    {
    WRITELOG( "CContextEngine::ContextSnapshot (single)" ); // DEBUG INFO
    
    // if any plug-in hasn't been found...
    if ( iContextEngineAO->PluginCount() <= 0 )
        {
        aHD.SetErrorCode( KErrNone );
        aObserver.ContextSnapshotStatus( &aHD );
        return;
        }

    // queue item for taking a snapshot
    CContextSnapshotItem* item = NULL;
    TRAPD( err, item = CContextSnapshotItem::NewL( &aObserver, &aHD ) );
    iArrayCount = 0;
    iProcessedArrayCount = 0;
    if ( err == KErrNone ) QueueSnapshotItem( item );
    }

// ---------------------------------------------------------------------------
// Takes a context snapshot including all initialized plugins
// for multiple objects.
// ---------------------------------------------------------------------------
//
EXPORT_C void CContextEngine::ContextSnapshot( MContextSnapshotObserver& aObserver,
    RPointerArray<CHarvesterData>& aHDArray )
    {
    WRITELOG( "CContextEngine::ContextSnapshot (array)" ); // DEBUG INFO
    
    // if any plug-in hasn't been found...
    if ( iContextEngineAO->PluginCount() <= 0 )
        {
        aObserver.ContextSnapshotStatus( NULL );
        return;
        }

    // queue items for taking a snapshot
    CContextSnapshotItem* item = NULL;
    TRAPD( err, item = CContextSnapshotItem::NewL( &aObserver, &aHDArray ) );
    iArrayCount = aHDArray.Count();
    iProcessedArrayCount = 0;
    if ( err == KErrNone ) QueueSnapshotItem( item );
    }

// ---------------------------------------------------------------------------
// Number of context plug-ins loaded. For testing purposes only.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CContextEngine::PluginCount()
    {
    return iContextEngineAO->PluginCount();
    }

// ---------------------------------------------------------------------------
// From MContextEngineObserver.
// Method is called by CContextEngineAO when a context snapshot is finished
// or an error has occured.
// ---------------------------------------------------------------------------
//
void CContextEngine::ContextSnapshotStatus( CHarvesterData* aHD )
    {
    if( iSnapshotQueue.Count() == 0 )
        {
        return;
        }
    
    if( iArrayCount > 0 )
        {
        iProcessedArrayCount++;
        }
    
    // notify the client application by using MContextSnapshotObserver's
    // method ContextSnapshotStatus()
    iSnapshotQueue[0]->GetObserver()->ContextSnapshotStatus( aHD );

    // remove handled item from queue
    if( iProcessedArrayCount == iArrayCount )
        {
        delete iSnapshotQueue[0];
        iSnapshotQueue[0] = NULL;
        iSnapshotQueue.Remove( 0 );
        iArrayCount = 0;
        iProcessedArrayCount = 0;
        }

    // ask a new snapshot if there is stuff in the queue
    if( iArrayCount == 0 && iSnapshotQueue.Count() > 0 )
        {
        RPointerArray<CHarvesterData>* tempArray = iSnapshotQueue[0]->GetItemArray();
        if( tempArray )
            {
            iArrayCount = tempArray->Count();
            }
        iContextEngineAO->StartSnapshot( iSnapshotQueue[0] );
        }
    else if( iSnapshotQueue.Count() == 0 )
    	{
    	iSnapshotQueue.Compress();
    	}
    }

// ---------------------------------------------------------------------------
// Add a new item to the snapshot item queue.
// Item contains an observer that needs to be informed when a snapshot is ready
// and items that need a context snapshot.
// If the queue was empty before adding the item, it's handling is started.
// ---------------------------------------------------------------------------
//
void CContextEngine::QueueSnapshotItem( CContextSnapshotItem* aItem )
    {
    // add a new object
    if ( iSnapshotQueue.Append( aItem ) != KErrNone )
        {
        delete aItem;
        aItem = NULL;
        return;
        }

    // the queue was empty, start ContextEngineAO
    if ( iSnapshotQueue.Count() == 1 )  // if the queue _was_ empty
        {
        iContextEngineAO->StartSnapshot( iSnapshotQueue[0] );
        }
    }

