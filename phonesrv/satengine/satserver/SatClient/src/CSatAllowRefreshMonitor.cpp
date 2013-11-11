/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Active object for waiting the refresh query,
*
*/



// INCLUDE FILES
#include    "CSatAllowRefreshMonitor.h"
#include    "msatrefreshobserver.h"
#include    "rsatrefresh.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatAllowRefreshMonitor::CSatAllowRefreshMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatAllowRefreshMonitor::CSatAllowRefreshMonitor(
    MSatRefreshObserver& aObserver,
    RSatRefresh& aRefresh ) :
    CActive( CActive::EPriorityStandard ),
    iObserver( aObserver ),
    iRefresh( aRefresh ),
    iChangingFiles(),
    iChangingFilesPckg( iChangingFiles ),
    iObservedFiles(),
    iType( ERefreshTypeNotSet ),
    iTypePckg( iType )
    {
    LOG( SIMPLE,
        "SATCLIENT: CSatAllowRefreshMonitor::CSatAllowRefreshMonitor calling" )

    CActiveScheduler::Add( this );
    iObservedFiles.Zero();

    LOG( SIMPLE,
        "SATCLIENT: CSatAllowRefreshMonitor::CSatAllowRefreshMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatAllowRefreshMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatAllowRefreshMonitor* CSatAllowRefreshMonitor::NewL(
    MSatRefreshObserver& aObserver,
    RSatRefresh& aRefresh )
    {
    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::NewL calling" )

    CSatAllowRefreshMonitor* self =
        new ( ELeave ) CSatAllowRefreshMonitor( aObserver, aRefresh );

    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::NewL exiting" )
    return self;
    }

// Destructor
CSatAllowRefreshMonitor::~CSatAllowRefreshMonitor()
    {
    LOG( SIMPLE,
        "SATCLIENT: CSatAllowRefreshMonitor::~CSatAllowRefreshMonitor calling" )

    Cancel();
    iObservedFiles.Zero();

    LOG( SIMPLE,
        "SATCLIENT: CSatAllowRefreshMonitor::~CSatAllowRefreshMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatAllowRefreshMonitor::ObserveFiles
// Set files to be observed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatAllowRefreshMonitor::ObserveFiles(
    const TSatRefreshFiles& aObservedFiles )
    {
    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::ObserveFiles calling" )

    iObservedFiles.Zero();

    // Add list of observed files if given.
    TInt observeFileCount( aObservedFiles.Length() );
    LOG2( SIMPLE, 
    "SATCLIENT: CSatAllowRefreshMonitor::ObserveFiles observeFileCount: %d",
    observeFileCount )
    for ( TInt fileIndex = 0; fileIndex < observeFileCount; fileIndex++ )
        {
        iObservedFiles.Append( aObservedFiles[ fileIndex ] );
        }

    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::ObserveFiles exiting" )
    }

// -----------------------------------------------------------------------------
// CSatAllowRefreshMonitor::Start
// Starts the waiting of notification.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatAllowRefreshMonitor::Start()
    {
    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::Start calling" )

    if ( !IsActive() )
        {
        LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::Start not Active" )
        iChangingFiles.Zero();

        // Add list of observed files if given.
        TInt observeFileCount( iObservedFiles.Length() );
        LOG2( SIMPLE, 
        "SATCLIENT: CSatAllowRefreshMonitor::Start observeFileCount: %d",
        observeFileCount )
        for ( TInt fileIndex = 0; fileIndex < observeFileCount; fileIndex++ )
            {
            iChangingFiles.Append( iObservedFiles[ fileIndex ] );
            }

        iRefresh.NotifyAllowRefresh( iTypePckg, iChangingFilesPckg, iStatus );
        SetActive();
        }

    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatAllowRefreshMonitor::RunL
// Notifies the observer for the refresh query.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatAllowRefreshMonitor::RunL()
    {
    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::RunL calling" )
    const TInt err( iStatus.Int() );
    
    LOG2( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::RunL, error: %i", err )

    if ( KErrNone == err )
        {
        const TBool response = iObserver.AllowRefresh( iType, iChangingFiles );
        iRefresh.AllowRefreshL( response );
        Start();
        }
    else if ( KErrInUse != err && 
              KErrServerTerminated != err && 
              KErrCancel != err )
        {
        Start();
        }

    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatAllowRefreshMonitor::RunError
// Handles refresh monitor error.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatAllowRefreshMonitor::RunError( TInt aError )
    {
    LOG2( SIMPLE,
          "SATCLIENT: CSatAllowRefreshMonitor::RunError calling, error: %i",
          aError )

    iRefresh.CancelNotifyAllowRefresh( aError );

    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::RunError exiting" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatAllowRefreshMonitor::DoCancels
// Cancels the notfication.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatAllowRefreshMonitor::DoCancel()
    {
    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::DoCancel calling" )

    iRefresh.CancelNotifyAllowRefresh();

    LOG( SIMPLE, "SATCLIENT: CSatAllowRefreshMonitor::DoCancel exiting" )
    }

//  End of File
