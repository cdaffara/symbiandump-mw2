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
* Description:  Receiver class for TCP / UDP sockets
*
*/



// INCLUDE FILES
#include <es_sock.h>
#include "CSatBIPDataReceiver.h"
#include "CSatBIPGPRSDataChannel.h"
#include "SatLog.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatBIPDataReceiver::CSatBIPDataReceiver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatBIPDataReceiver::CSatBIPDataReceiver( CSatBIPGPRSDataChannel& aObserver,  
    RSocket& aSocket,
    TBool aIsTcpSocket ) : CActive( EPriorityNormal ),
    iObserver( aObserver ),
    iSocket( aSocket ),
    iReceiveDataLen( 0 ),
    iRecvPckg( iReceiveDataLen )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::CSatBIPDataReceiver\
         calling-exiting" )
    
    iIsTcpSocket = aIsTcpSocket;
    
    CActiveScheduler::Add( this );
    }
    
// -----------------------------------------------------------------------------
// CSatBIPDataReceiver::~CSatBIPDataReceiver
// Destructor
// -----------------------------------------------------------------------------
//
CSatBIPDataReceiver::~CSatBIPDataReceiver()
    {
    LOG( SIMPLE, 
    "SATENGINE: CSatBIPDataReceiver::~CSatBIPDataReceiver calling" )
    // Don't have our own pointers...
    LOG( SIMPLE, 
    "SATENGINE: CSatBIPDataReceiver::~CSatBIPDataReceiver exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPDataReceiver::RunL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CSatBIPDataReceiver::RunL()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::RunL calling" )

    LOG2( NORMAL, "  RunL with error: %i", iStatus.Int() )
    LOG2( NORMAL, "  RunL Data received <buffer>: %i", iRecvBuffer.Length() )

    // Notify observer
    if ( KErrNone == iStatus.Int() )
        {
        iObserver.DataReceivedNotificationL( iRecvBuffer );
        }    
    else
        {
        iObserver.DataReceiveError( iStatus.Int() );
        }
    LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPDataReceiver::DoCancel
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CSatBIPDataReceiver::DoCancel()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::DoCancel calling" )
    iSocket.CancelRecv();
    LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::DoCancel exiting" )
    }
    
// -----------------------------------------------------------------------------
// CSatBIPDataReceiver::ReceiveStoreEmptyNotification
// -----------------------------------------------------------------------------
//
void CSatBIPDataReceiver::ReceiveStoreEmptyNotificationL()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::ReceiveStoreEmptyNotification\
         calling" )
    // It doesn't matter, if this buffer is empty
    iObserver.DataReceivedNotificationL( iRecvBuffer );
    LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::ReceiveStoreEmptyNotification\
         exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPDataReceiver::StartReceive
// Starts to receive data
// -----------------------------------------------------------------------------
//
void CSatBIPDataReceiver::StartReceive( TInetAddr& aAddr )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::StartReceive calling" )

    // It may already be active
    if ( !IsActive() )
        {
        // This is used only with TCP sockets
        iReceiveDataLen = 0;
        // Remove previously received data
        iRecvBuffer.Zero();
        // Check the socket type
        if ( iIsTcpSocket )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::StartReceive TCP" )
            // Only TCP / Stream socket may receive one or more bytes
            iSocket.RecvOneOrMore( iRecvBuffer, 0, iStatus, iRecvPckg );
            }
        else
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::StartReceive UDP" )
            // Use different receive method with UPD / Datagram socket
            // Destination has already been stored in iSocket during 
            // activation, 
            // so we don't have to tell it here.        
            iSocket.RecvFrom( iRecvBuffer, aAddr, 0, iStatus );
            }
        SetActive();
        }
    LOG( SIMPLE, "SATENGINE: CSatBIPDataReceiver::StartReceive exiting" )
    }

// End of file
