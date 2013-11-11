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
* Description:  Refresh notification sub-session.
*
*/



// INCLUDE FILES
#include    <rsatrefresh.h>
#include    <rsatsession.h>
#include    "SatSOpcodes.h"
#include    "CSatAllowRefreshMonitor.h"
#include    "CSatRefreshMonitor.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RSatRefresh::RSatRefresh
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C RSatRefresh::RSatRefresh( MSatRefreshObserver& aObserver ) :
    RSubSessionBase(),
    iObserver( aObserver ),
    iAllowMonitor( NULL ),
    iRefreshMonitor( NULL )
    {
    }

// -----------------------------------------------------------------------------
// RSatRefresh::OpenL
// Connects a sub-session to SatServer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatRefresh::OpenL( const RSatSession& aSession )
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::OpenL calling" )
    
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatRefresh::OpenL UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif

    TIpcArgs arguments( 0, 0, 0, 0 );
    User::LeaveIfError(
        CreateSubSession( aSession, ESatSOpenRefreshSubSession, arguments ) );

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::OpenL exiting" )
    }

// -----------------------------------------------------------------------------
// RSatRefresh::Close
// Closes the sub-session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatRefresh::Close()
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::Close calling" )
    
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatRefresh::Close UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif

    delete iRefreshMonitor;
    delete iAllowMonitor;

    iRefreshMonitor = NULL;
    iAllowMonitor = NULL;

    CloseSubSession( ESatSCloseRefreshSubSession );

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::Close exiting" )
    }

// -----------------------------------------------------------------------------
// RSatRefresh::NotifyFileChangeL
// Starts monitoring refresh events.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatRefresh::NotifyFileChangeL()
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyFileChangeL calling" )
    
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatRefresh::NotifyFileChangeL UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif
    
    if ( !iAllowMonitor )
        {
        LOG( SIMPLE, 
        "SATCLIENT: RSatRefresh::NotifyFileChangeL iAllowMonitor false" )
        iAllowMonitor = CSatAllowRefreshMonitor::NewL( iObserver, *this );
        iAllowMonitor->Start();
        }
    else
        {
        LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyFileChangeL, \
            re-start allowMonitor" )
        TSatRefreshFiles empty;
        iAllowMonitor->ObserveFiles( empty );
        iAllowMonitor->Start();
        }

    if ( !iRefreshMonitor )
        {
        LOG( SIMPLE, 
        "SATCLIENT: RSatRefresh::NotifyFileChangeL iRefreshMonitor false" )
        iRefreshMonitor = CSatRefreshMonitor::NewL( *this, iObserver );
        iRefreshMonitor->Start();
        }
    else
        {
        LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyFileChangeL, \
            re-start RefreshMonitor" )
        iRefreshMonitor->Start();
        }

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyFileChangeL exiting" )
    }

// -----------------------------------------------------------------------------
// RSatRefresh::NotifyFileChangeL
// Starts monitoring refresh events.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatRefresh::NotifyFileChangeL(
    const TSatRefreshFiles& aObservedFiles )
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyFileChangeL(1) calling" )
    
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatRefresh::NotifyFileChangeL(1) UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif
    
    if ( !iAllowMonitor )
        {
        LOG( SIMPLE, 
        "SATCLIENT: RSatRefresh::NotifyFileChangeL(1) iAllowMonitor false" )
        iAllowMonitor = CSatAllowRefreshMonitor::NewL( iObserver, *this );
        iAllowMonitor->ObserveFiles( aObservedFiles );
        iAllowMonitor->Start();
        }
    else
        {
        LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyFileChangeL(1), \
            re-start allowMonitor" )
        iAllowMonitor->ObserveFiles( aObservedFiles );
        iAllowMonitor->Start();
        }

    if ( !iRefreshMonitor )
        {
        LOG( SIMPLE, 
        "SATCLIENT: RSatRefresh::NotifyFileChangeL(1) iRefreshMonitor false" )
        iRefreshMonitor = CSatRefreshMonitor::NewL( *this, iObserver );
        iRefreshMonitor->Start();
        }
    else
        {
        LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyFileChangeL(1), \
            re-start RefreshMonitor" )
        iRefreshMonitor->Start();
        }

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyFileChangeL(1) exiting" )
    }

// -----------------------------------------------------------------------------
// RSatRefresh::RefreshEFRead
// Indicates client's refresh actions complete.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatRefresh::RefreshEFRead( TBool aAdditionEFRead ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::RefreshEFRead calling" )
    
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatRefresh::RefreshEFRead UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif
    
    TPckg<TBool> additionEFReadPckg( aAdditionEFRead );
    TIpcArgs arguments( &additionEFReadPckg );

    SendReceive( ESatSRefreshEFRead, arguments );

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::RefreshEFRead exiting" )
    }

// -----------------------------------------------------------------------------
// RSatRefresh::Cancel
// Cancels both requests.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatRefresh::Cancel() const
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::Cancel calling" )
    
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatRefresh::Cancel UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif
    
    CancelNotifyAllowRefresh();
    CancelNotifyRefresh();

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::Cancel exiting" )
    }

// -----------------------------------------------------------------------------
// RSatRefresh::NotifyAllowRefresh
// Issues the request to be notified when refresh query is made.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RSatRefresh::NotifyAllowRefresh(
    TPckg<TSatRefreshType>& aType,
    TPckg<TSatRefreshFiles>& aChangingFiles,
    TRequestStatus& aStatus ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyAllowRefresh calling" )

    TIpcArgs arguments( &aChangingFiles, &aType );
    SendReceive( ESatSNotifyAllowRefresh, arguments, aStatus );

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyAllowRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// RSatRefresh::AllowRefreshL
// Sends client's response.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RSatRefresh::AllowRefreshL( TBool aAllow ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::AllowRefreshL calling" )

    TPckg<TBool> allowPckg( aAllow );
    TIpcArgs arguments( &allowPckg );
    User::LeaveIfError( SendReceive( ESatSRefreshAllowed, arguments ) );

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::AllowRefreshL exiting" )
    }

// -----------------------------------------------------------------------------
// RSatRefresh::CancelNotifyAllowRefresh
// Cancels the request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RSatRefresh::CancelNotifyAllowRefresh( TInt aError ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::CancelNotifyAllowRefresh calling" )

    if ( iAllowMonitor )
        {
        LOG( SIMPLE, 
        "SATCLIENT: RSatRefresh::CancelNotifyAllowRefresh iAllowMonitor true" )
        TIpcArgs arguments( aError );
        SendReceive( ESatSNotityAllowRefreshCancel, arguments );
        }

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::CancelNotifyAllowRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// RSatRefresh::NotifyRefresh
// Issues the request to be notified when refresh is made.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RSatRefresh::NotifyRefresh(
    TPckg<TSatRefreshType>& aType,
    TPckg<TSatRefreshFiles>& aChangedFiles,
    TRequestStatus& aStatus ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyRefresh calling" )

    TIpcArgs arguments( &aChangedFiles, &aType );
    SendReceive( ESatSNotifyRefresh, arguments, aStatus );

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::NotifyRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// RSatRefresh::CancelNotifyRefresh
// Cancels the request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RSatRefresh::CancelNotifyRefresh() const
    {
    LOG( SIMPLE, "SATCLIENT: RSatRefresh::CancelNotifyRefresh calling" )

    if ( iRefreshMonitor )
        {
        LOG( SIMPLE, 
        "SATCLIENT: RSatRefresh::CancelNotifyAllowRefresh iRefreshMonitor \
        true" )
        TIpcArgs arguments( 0 );
        SendReceive( ESatSCancelNotifyRefresh, arguments );
        }

    LOG( SIMPLE, "SATCLIENT: RSatRefresh::CancelNotifyRefresh exiting" )
    }

//  End of File
