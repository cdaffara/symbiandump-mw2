/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer of the GPRS connection status change
*
*/



// INCLUDE FILES
#include <etelpckt.h>
#include <pcktcs.h> // for EPacketNotifyStatusChange in DoCancel
#include "csatbipconnectionobserver.h"
#include "CSatBIPUtils.h"
#include "SatLog.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatBIPConnectionObserver::CSatBIPConnectionObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatBIPConnectionObserver::CSatBIPConnectionObserver(
    CSatBIPUtils& aBipUtils ) :
    CActive( EPriorityNormal ),
    iBipUtils( aBipUtils )
    {
    LOG( NORMAL, "SATENGINE: CSatBIPConnectionObserver::\
        CSatBIPConnectionObserver calling" )
    CActiveScheduler::Add( this );
    // Get current status
    iBipUtils.PacketService().GetStatus( iConnectionStatus );
    // Set last status
    iPrevConnectionStatus = iConnectionStatus;
    LOG2( NORMAL, "SATENGINE: CSatBIPConnectionObserver::\
        CSatBIPConnectionObserver exiting: %i", iConnectionStatus )
    }

// -----------------------------------------------------------------------------
// CSatBIPConnectionObserver::~CSatBIPConnectionObserver
// Destructor
// -----------------------------------------------------------------------------
//
CSatBIPConnectionObserver::~CSatBIPConnectionObserver()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPConnectionObserver::~CSatBIPConnectionObserver \
        calling" )
    Cancel();
    LOG( SIMPLE,
        "SATENGINE: CSatBIPConnectionObserver::~CSatBIPConnectionObserver \
        exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPConnectionObserver::RunL
// -----------------------------------------------------------------------------
//
void CSatBIPConnectionObserver::RunL()
    {
    LOG( NORMAL, "SATENGINE: CSatBIPConnectionObserver::RunL calling" )

    const TInt errStatus( iStatus.Int() );
    LOG2( SIMPLE, " RunL (%i)", errStatus )
    LOG2( SIMPLE, " ConnStatus (%i)", iConnectionStatus )
    if ( KErrNone == errStatus )
        {
        //if previous status is active packet data connection and
        //current status is inactive packet data connection, drop links.
        if( RPacketService::EStatusActive == iPrevConnectionStatus && 
            ( RPacketService::EStatusAttached == iConnectionStatus || 
            RPacketService::EStatusUnattached == iConnectionStatus ))
            {
            LOG( NORMAL, "CSatBIPConnectionObserver::RunL stop UDP link" )
            iBipUtils.StopUdpLink();
            }
        
        // Restart request
        iPrevConnectionStatus = iConnectionStatus;
        iBipUtils.PacketService().NotifyStatusChange( iStatus, iConnectionStatus );
        SetActive();
        }

    LOG( NORMAL, "SATENGINE: CSatBIPConnectionObserver::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPConnectionObserver::DoCancel
// -----------------------------------------------------------------------------
//
void CSatBIPConnectionObserver::DoCancel()
    {
    LOG( NORMAL, "SATENGINE: CSatBIPConnectionObserver::DoCancel calling" )

    iBipUtils.PacketService().CancelAsyncRequest( EPacketNotifyStatusChange );

    LOG( NORMAL, "SATENGINE: CSatBIPConnectionObserver::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPConnectionObserver::StartObserver
// Starts to observe connection status
// -----------------------------------------------------------------------------
//
void CSatBIPConnectionObserver::StartObserver()
    {
    LOG( NORMAL, "SATENGINE: CSatBIPConnectionObserver::StartObserver calling" )

    if ( !IsActive() )
        {
        LOG( SIMPLE, "  Activating observer" )
        iBipUtils.PacketService().NotifyStatusChange( iStatus, iConnectionStatus );
        SetActive();
        }

    LOG( NORMAL, "SATENGINE: CSatBIPConnectionObserver::StartObserver exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPConnectionObserver::Status
// Returns current connection status
// -----------------------------------------------------------------------------
//
RPacketService::TStatus CSatBIPConnectionObserver::Status() const
    {
    LOG( NORMAL, "SATENGINE: CSatBIPConnectionObserver::Status \
    calling-exiting" )
    LOG2( NORMAL, " ConnectionStatus: %i", iConnectionStatus )
    return iConnectionStatus;
    }

// End of file
