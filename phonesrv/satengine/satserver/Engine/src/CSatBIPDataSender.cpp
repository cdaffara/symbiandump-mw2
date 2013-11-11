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
* Description:  Sender class for TCP / UDP sockets
*
*/



// INCLUDE FILES
#include <es_sock.h>
#include "CSatBIPDataSender.h"
#include "MSatSendDataObserver.h"
#include "SatLog.h"
#include "MSatBIPUtils.h"

#ifdef ENABLE_SAT_LOGGING
const TUint8 KIpAddressLogSize( 40 );
#endif
 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatBIPDataSender::CSatBIPDataSender
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatBIPDataSender::CSatBIPDataSender( MSatSendDataObserver& aObserver, 
    RSocket& aSocket,
    TBool aIsTcpSocket,
    TInt aBufferSize ) : CActive( EPriorityNormal ),
    iObserver( aObserver ),
    iSocket( aSocket )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::CSatBIPDataSender\
         calling-exiting" )

    iIsTcpSocket = aIsTcpSocket;
    iBufferSize = aBufferSize;
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CSatBIPDataSender::~CSatBIPDataSender
// Destructor
// -----------------------------------------------------------------------------
//
CSatBIPDataSender::~CSatBIPDataSender()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::~CSatBIPDataSender calling" )
    // Don't have our own pointers...
    LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::~CSatBIPDataSender exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPDataSender::RunL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CSatBIPDataSender::RunL()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::RunL calling" )

    LOG2( NORMAL, "  RunL with error: %i", iStatus.Int() )
    LOG2( NORMAL, "  RunL <buffer_size>: %i", iBufferSize )
    TInt errCode( MSatBIPUtils::ESatBIPSuccess );

    if ( KErrNone != iStatus.Int() )
        {
        // Error, setting error code
        errCode = MSatBIPUtils::ESatBIPRemoteDeviceNotReachable;
        }

    // Notify observer and empty buffer
    iObserver.DataSentNotification( errCode, iBufferSize );
    iSendPtr.Zero();
    LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPDataSender::DoCancel
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CSatBIPDataSender::DoCancel()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::DoCancel calling" )
    iSocket.CancelSend();
    LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPDataSender::SendData
// Starts the send process using predefined protocol
// -----------------------------------------------------------------------------
//
void CSatBIPDataSender::SendData( const TDesC8& aData, TInetAddr& aDest )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::SendData calling" )
    // Copy data into local buffer
    iSendPtr.Copy( aData );
    
    #ifdef ENABLE_SAT_LOGGING
    TBuf<KIpAddressLogSize> addressForLog;
    #endif

    #ifdef ENABLE_SAT_LOGGING
    aDest.Output( addressForLog );
    LOG2( NORMAL, 
    "SATENGINE: CSatBIPDataSender::SendData Sending to %S", &addressForLog )
    #endif    
    
    LOG2( NORMAL, "  Port number: %i", aDest.Port() )
    LOG2( NORMAL, "  Sending %i bytes", aData.Length() )
    if ( !IsActive() )
        {
        // Check protocol
        if ( iIsTcpSocket )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::SendData TCP" )
            // When using TCP, this request completes when the data is actually
            // sent to remote device. 
            iSocket.Send( iSendPtr, 0, iStatus );
            }
        else
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::SendData UDP" )
            // UDP Socket. This request completes almost immediately. 
            // There is no
            // way to be sure that the remote device gets the datagram.
            iSocket.SendTo( iSendPtr, aDest, 0, iStatus );
            }
        SetActive();
        }
    LOG( SIMPLE, "SATENGINE: CSatBIPDataSender::SendData exiting" )
    }

// End of file
