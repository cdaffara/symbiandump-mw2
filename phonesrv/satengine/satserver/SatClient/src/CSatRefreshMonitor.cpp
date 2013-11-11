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
* Description:  Active object for monitoring the refresh event.
*
*/



// INCLUDE FILES
#include    <rsatsession.h>
#include    <rsatrefresh.h>

#include    "CSatRefreshMonitor.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatRefreshMonitor::CSatRefreshMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatRefreshMonitor::CSatRefreshMonitor(
    RSatRefresh& aRefresh,
    MSatRefreshObserver& aObserver ) :
    CActive( CActive::EPriorityStandard ),
    iObserver( aObserver ),
    iRefresh( aRefresh ),
    iChangedFiles(),
    iChangedFilesPckg( iChangedFiles ),
    iType( ERefreshTypeNotSet ),
    iTypePckg( iType )
    {
    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::CSatRefreshMonitor calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::CSatRefreshMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatRefreshMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatRefreshMonitor* CSatRefreshMonitor::NewL(
    RSatRefresh& aRefresh,
    MSatRefreshObserver& aObserver )
    {
    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::NewL calling" )

    CSatRefreshMonitor* self =
        new ( ELeave ) CSatRefreshMonitor( aRefresh, aObserver );

    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::NewL exiting" )
    return self;
    }

// Destructor
CSatRefreshMonitor::~CSatRefreshMonitor()
    {
    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::~CSatRefreshMonitor calling" )

    Cancel();

    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::~CSatRefreshMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatRefreshMonitor::Start
// Starts monitoring the refresh event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatRefreshMonitor::Start()
    {
    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::Start calling" )

    if ( !IsActive() )
        {
        LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::Start set active" )
        iRefresh.NotifyRefresh( iTypePckg, iChangedFilesPckg, iStatus );
        SetActive();
        }

    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatRefreshMonitor::RunL
// Notifies the observer about refresh event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatRefreshMonitor::RunL()
    {
    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::RunL calling" )
    const TInt err( iStatus.Int() );

    if ( KErrNone == err )
        {
        iObserver.Refresh( iType, iChangedFiles );
        Start();
        }
    else if ( KErrInUse == err )
        {
        LOG( SIMPLE, "SATCLIENT:   Attempt to restart, cancelled" )
        }
    else if ( KErrServerTerminated == err )
        {
        LOG( SIMPLE, "SATCLIENT:   Server terminated" )
        }
    else if ( KErrCancel != err )
        {
        LOG2( SIMPLE, "SATCLIENT:   RunL error: %i", err )
        Start();
        }
    else
        {
        LOG( SIMPLE, "SATCLIENT:   Cancelled" )
        }

    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatRefreshMonitor::DoCancel
// Cancels the request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatRefreshMonitor::DoCancel()
    {
    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::DoCancel calling" )

    iRefresh.CancelNotifyRefresh();

    LOG( SIMPLE, "SATCLIENT: CSatRefreshMonitor::DoCancel exiting" )
    }

//  End of File
