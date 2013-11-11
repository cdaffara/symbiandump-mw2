/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Active object for an asynchronous harvesrt request
 *
*/

#include <e32base.h>

#include "harvesterrequestactive.h"
#include "harvesterrequestqueue.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CHarvesterRequestActive::~CHarvesterRequestActive()
// Destructor.
// ---------------------------------------------------------------------------
//
CHarvesterRequestActive::~CHarvesterRequestActive()
    {
    if( IsActive() )
        {
        Cancel();
        }
    
    iRequestCompleted = ETrue;
    delete iAlbumIds;
    iAlbumIds = NULL;
    }

// ---------------------------------------------------------------------------
// CHarvesterRequestActive::NewL
// Two-phased constructor.
// --------------------------------------------------------------------------- 
//
CHarvesterRequestActive* CHarvesterRequestActive::NewL(
        RHarvesterClient& aClient, MHarvestObserver* aObserver,
        TInt aService, const TDesC& aUri, 
        HBufC8* aAlbumIds, TBool aAddLocation,
        CHarvesterRequestQueue* aQueue )
    {
    CHarvesterRequestActive* self = new( ELeave )CHarvesterRequestActive( aClient, aObserver,
            aService, aUri, aAlbumIds, aAddLocation, aQueue );
    return self;
    }

// ---------------------------------------------------------------------------
// CHarvesterRequestActive::CHarvesterRequestActive()
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CHarvesterRequestActive::CHarvesterRequestActive( RHarvesterClient& aClient,
    MHarvestObserver* aObserver, TInt aService, const TDesC& aUri, 
    HBufC8* aAlbumIds, TBool aAddLocation, CHarvesterRequestQueue* aQueue )
    : CActive( CActive::EPriorityStandard ), iClient( aClient ), iObserver( aObserver ), 
    iService( aService ), iUri( aUri ), iAlbumIds( aAlbumIds ), iAddLocation( aAddLocation ),
    iRequestQueue( aQueue ), iLocation( EFalse ), iCancelled( EFalse )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CHarvesterRequestActive::RunL()
// ---------------------------------------------------------------------------
//
void CHarvesterRequestActive::RunL()
    {
    if( iStatus.Int() == KErrCancel )
        {
        return;
        }           
    
    if( iStatus.Int() && iObserver )
        {
        iObserver->HarvestingComplete( iUri, iStatus.Int() );
        }       
    
    iRequestCompleted = ETrue;
    if( iRequestQueue )
        {
        iRequestQueue->RequestComplete();
        }
    }

// ---------------------------------------------------------------------------
// CHarvesterRequestActive::RunError()
// ---------------------------------------------------------------------------
//
TInt CHarvesterRequestActive::RunError( TInt aError )
    {
    if( aError == KErrCancel )
        {
        return KErrNone;
        }
    
    if( iObserver )
        {
        iObserver->HarvestingComplete( iUri, aError );
        }
    
    iRequestCompleted = ETrue;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CHarvesterRequestActive::DoCancel()
// ---------------------------------------------------------------------------
//
void CHarvesterRequestActive::DoCancel()
    {
    iCancelled = ETrue;
	iObserver = NULL;
    }

// ---------------------------------------------------------------------------
// CHarvesterRequestActive::Start()
// ---------------------------------------------------------------------------
//
void CHarvesterRequestActive::Start()
    {
    TPckg<TBool> location( iAddLocation );
    iLocation.Set( location );
    
    TIpcArgs ipcArgs( &iUri, iAlbumIds, &iLocation );
    iPersistentArgs = ipcArgs;
    
    if( !iCancelled )
        {
        iClient.HarvestFile( iService, iPersistentArgs, iStatus );
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// CHarvesterRequestActive::ForceHarvest()
// ---------------------------------------------------------------------------
//
void CHarvesterRequestActive::ForceHarvest()
    {
    iObserver = NULL;
    
    TPckg<TBool> location( iAddLocation );
    iLocation.Set( location );
    
    TIpcArgs ipcArgs( &iUri, iAlbumIds, &iLocation );
    iPersistentArgs = ipcArgs;
    
    iClient.ForceHarvestFile( iService, iPersistentArgs );
    }

// End of file
