/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
*     Messaging progress watching classes
*
*/




// INCLUDE FILES
#include <msvapi.h>
#include "muiudomainpan.h"
#include "MuiuMsvSingleOpWatcher.h"

// CONSTANTS
const TInt KSingleOpWatcherPriority = KMaxTInt;


// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------
// CMsvSingleOpWatcher::NewLC
//
// ---------------------------------------------------------
//
EXPORT_C CMsvSingleOpWatcher* CMsvSingleOpWatcher::NewLC( MMsvSingleOpWatcher& aObserver )
    {   // static 
    CMsvSingleOpWatcher* self = new( ELeave ) CMsvSingleOpWatcher( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------
// CMsvSingleOpWatcher::NewL
//
// ---------------------------------------------------------
//
EXPORT_C CMsvSingleOpWatcher* CMsvSingleOpWatcher::NewL( MMsvSingleOpWatcher& aObserver )
    {   // static 
    CMsvSingleOpWatcher* self = NewLC(aObserver);
    CleanupStack::Pop( self ); // self
    return self;
    }


// ---------------------------------------------------------
// CMsvSingleOpWatcher::CMsvSingleOpWatcher
//
// ---------------------------------------------------------
//
CMsvSingleOpWatcher::CMsvSingleOpWatcher( MMsvSingleOpWatcher& aObserver )
    : CActive( KSingleOpWatcherPriority ), iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------
// CMsvSingleOpWatcher::ConstructL
//
// ---------------------------------------------------------
//
void CMsvSingleOpWatcher::ConstructL()
    {
    // Nothing to do, reserved for future use...
    }


// ---------------------------------------------------------
// CMsvSingleOpWatcher::~CMsvSingleOpWatcher
//
// ---------------------------------------------------------
//
EXPORT_C CMsvSingleOpWatcher::~CMsvSingleOpWatcher()
    {
    Cancel();   // Cancel any request, if outstanding. Calls Cancel on iOperation.
    delete iOperation;      
    }


// ---------------------------------------------------------
// CMsvSingleOpWatcher::SetOperation
// --- Setter ---
// Must only be called once during the lifetime of a CMsvSingleOpWatcher object.
// ---------------------------------------------------------
//
EXPORT_C void CMsvSingleOpWatcher::SetOperation(CMsvOperation* aOperation)
    {
    __ASSERT_DEBUG( !IsActive(), Panic( EMuiuSingleOpWatcherAlreadyActive ) );
    __ASSERT_DEBUG( !iOperation, Panic( EMuiuSingleOpWatcherOperationAlreadySet ) );
    __ASSERT_DEBUG( aOperation, Panic( EMuiuSingleOpWatcherOperationNull ) );

    //
    // Take ownership of operation and set our active status so we're handled 
    // by the active scheduler.
    iOperation = aOperation;
    iStatus = KRequestPending;
    SetActive();
    }


// ---------------------------------------------------------
// CMsvSingleOpWatcher::Operation
// --- Accessor ---
// ---------------------------------------------------------
//
EXPORT_C CMsvOperation& CMsvSingleOpWatcher::Operation() const
    {
    __ASSERT_DEBUG( iOperation, Panic( EMuiuSingleOpWatcherOperationNull ) );
    return *iOperation;
    }


// ---------------------------------------------------------
// CMsvSingleOpWatcher::DoCancel
// --- From CActive ---
// ---------------------------------------------------------
//
void CMsvSingleOpWatcher::DoCancel()        
    {

    // Pass on cancel 
    iOperation->Cancel();
    }


//------------------------------------
// CMsvSingleOpWatcher::RunL
// --- From CActive ---
// ---------------------------------------------------------
//
void CMsvSingleOpWatcher::RunL()
    {
    //
    // Inform client of watcher that the operation has completed
    iObserver.OpCompleted( *this, iStatus.Int() );
    }

// End of file
