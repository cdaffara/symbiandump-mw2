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
* Description: Implementation of CVmbxObserver class.
 *
*/

// INCLUDE FILES
#include "vmbxlogger.h"
#include "vmbxcenrepobserver.h"
#include "vmbxsimstoreobserver.h"
#include "vmbxsatrefreshobserver.h"
#include "vmbxsimhandler.h"

#include "vmbxobserver.h"

// CONSTANTS


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxObserver::CVmbxObserver
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CVmbxObserver::CVmbxObserver( MVmbxResourceProvider& aProvider )
                            : iProvider( aProvider )
    {
    VMBLOGSTRING( "VMBX: CVmbxObserver::CVmbxObserver <=>" );
    }

// ---------------------------------------------------------------------------
// CVmbxObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVmbxObserver::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxObserver::ConstructL =>" );

    iCenRepObserver = CVmbxCenRepObserver::NewL( *this );

    TVmbxMemoryLocation storeType = 
                    iProvider.VmbxCenRepHandler().StoreType();
    if ( EVmbxSimMemory == storeType )
        {
        TRAPD( err, iSimHandler = CVmbxSimHandler::NewL() );
        if ( KErrNone == err )
            {
            iSatObserver = CVmbxSatRefreshObserver::NewL( *this );
            iSimObserver = CVmbxSimStoreObserver::NewL( *iSimHandler, *this );
            }
        VMBLOGSTRING2( "VMBX: CVmbxObserver::ConstructL: err=%I", err );
        }
    VMBLOGSTRING( "VMBX: CVmbxObserver::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxObserver::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxObserver* CVmbxObserver::NewL( MVmbxResourceProvider& aProvider )
    {
    VMBLOGSTRING( "VMBX: CVmbxObserver::NewL =>" );
    CVmbxObserver* self = CVmbxObserver::NewLC( aProvider );
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxObserver::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxObserver::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxObserver* CVmbxObserver::NewLC( MVmbxResourceProvider& aProvider )
    {
    VMBLOGSTRING( "VMBX: CVmbxObserver::NewLC =>" );
    CVmbxObserver* self = new (ELeave) CVmbxObserver( aProvider );
    CleanupStack::PushL( self );
    VMBLOGSTRING( "VMBX: CVmbxObserver::NewLC <=" );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxObserver::SetVmbxObserver
// Set observer
// ---------------------------------------------------------------------------
//
void CVmbxObserver::SetVmbxObserver( MVmbxChangeObserver& aObserver )
    {
    VMBLOGSTRING( "VMBX: CVmbxObserver::SetVmbxObserver =>" );
    iObserver = &aObserver;
    VMBLOGSTRING( "VMBX: CVmbxObserver::SetVmbxObserver <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxObserver::HandleSimStoreChanged
// observe Sim store changed.
// ---------------------------------------------------------------------------
//
void CVmbxObserver::HandleSimStoreChanged( )
    {
    VMBLOGSTRING( "VMBX: CVmbxObserver::HandleSimStoreChanged =>" );
    if ( iObserver )
        {
        TRAPD( err, iObserver->SimStoreChangedL() );
        // avoid warning
        err = err;
        VMBLOGSTRING2( "VMBX: CVmbxObserver::HandleSimStoreChanged, err=%I",
         err );
        }
    
    VMBLOGSTRING( "VMBX: CVmbxObserver::HandleSimStoreChanged <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxObserver::HandleSatRefresh
// observe sat refresh.
// ---------------------------------------------------------------------------
//
void CVmbxObserver::HandleSatRefresh( )
    {
    VMBLOGSTRING( "VMBX: CVmbxObserver::HandleSatRefreshL =>" );
    if ( iObserver )
        {
        TRAPD( err, iObserver->SatRefreshL() );
        // avoid warning
        err = err;
        VMBLOGSTRING2( "VMBX: CVmbxObserver::HandleSatRefresh, err=%I",
         err );
        }
    VMBLOGSTRING( "VMBX: CVmbxObserver::HandleSatRefreshL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxObserver::HandleCenRepChange
// observe central Repository changed.
// ---------------------------------------------------------------------------
//
void CVmbxObserver::HandleCenRepChange( TVmbxCenRepKey aId )
    {
    VMBLOGSTRING( "VMBX: CVmbxObserver::HandleCenRepChange =>" );
    if ( iObserver )
        {
        TRAPD( err, iObserver->CenRepChangedL( aId ) );
        // avoid warning
        err = err;
        VMBLOGSTRING2( "VMBX: CVmbxObserver::HandleCenRepChange, err=%I",
         err );
        }
    VMBLOGSTRING( "VMBX: CVmbxObserver::HandleCenRepChange <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxObserver::~CVmbxObserve
// Destructor.
// ---------------------------------------------------------------------------
//
CVmbxObserver::~CVmbxObserver()
    {
    VMBLOGSTRING( "VMBX: CVmbxObserver::~CVmbxObserver =>" );
    delete iSatObserver;
    delete iSimObserver;
    delete iCenRepObserver;
    delete iSimHandler;
    VMBLOGSTRING( "VMBX: CVmbxObserver::~CVmbxObserver <=" );
    }

//  End of File
