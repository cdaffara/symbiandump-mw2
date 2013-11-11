/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  GPRS Data channel for BIP
*
*/


#include <etelqos.h>
#include <cmmanager.h>
#include <cmconnectionmethod.h>
#include <cmpluginpacketdatadef.h>
#include <cs_subconevents.h>                    // For subconnection events
#include <networking/qos3gpp_subconparams.h>    // For extension parameters
#include <e32std.h>                             // TRealFormat
#include <e32cmn.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <in_sock_internal.h>                   // For KSoUdpRecvBuf contant
#endif
#include "CSatBIPUtils.h"
#include "CSatBIPGPRSDataChannel.h"
#include "SatLog.h"
#include "EnginePanic.h"
#include "CSatBIPDataSender.h"
#include "CSatBIPDataReceiver.h"
#include "MSatSendDataObserver.h"
#include "MSatConnectionObserver.h"
#include "TSatChannelIDInfo.h"
#include "TSatQoSParser.h"
#include "csatbipsubconneventobserver.h"
#include "csatsactivewrapper.h"
 
// Required GPRS parameters
const TUint8 KReqGprsQoSParams = 6;
// Required packet protocol type is always 02 for GPRS -> IP Protocol
const TUint8 KReqPacketProtType = 0x02;
// Position of the protocol type in the array
const TUint8 KProtocolPos = 5;
// Lengths of IP address
const TUint8 KIPv4Length = 4;
const TUint8 KIPv6Length = 16;

#ifdef SAT_USE_DUMMY_TSY
const TUint8 KIpAddressSize( 15 );
const TUint8 KIpv4Length( 4 );
const TUint8 KIpv6Length( 16 );
const TUint8 KIpAddressA( 0 );
const TUint KIpAddressAValue( 127 );
const TUint8 KIpAddressB( 1 );
const TUint KIpAddressBValue( 0 );
const TUint8 KIpAddressC( 2 );
const TUint KIpAddressCValue( 0 );
const TUint8 KIpAddressD( 3 );
const TUint KIpAddressDValueDestination( 2 );
const TUint KIpAddressDValueLocal( 3 );
#endif

#ifdef ENABLE_SAT_LOGGING
const TUint8 KIpAddressLogSize( 40 );
#endif

const TInt KReCheckStatusDelay = 100000; // 100ms
const TInt KMaxAttempts = 20;            // 20*100ms= 2s 
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSatBIPGPRSDataChannel* CSatBIPGPRSDataChannel::NewL(
    CSatBIPUtils& aUtils,
    RSocketServ& aSocketServer,
    TSatChannelIdInfo& aChannelId,
    RConnection& aConnection,
    RSubConnection& aSubConnection )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::NewL calling" )

    CSatBIPGPRSDataChannel* self = new( ELeave ) CSatBIPGPRSDataChannel( 
        aUtils, aSocketServer, aChannelId, aConnection, aSubConnection );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::NewL exiting" )
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSatBIPGPRSDataChannel* CSatBIPGPRSDataChannel::NewLC(
    CSatBIPUtils& aUtils,
    RSocketServ& aSocketServer,
    TSatChannelIdInfo& aChannelId,
    RConnection& aConnection,
    RSubConnection& aSubConnection )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::NewLC calling" )

    CSatBIPGPRSDataChannel* self = new ( ELeave ) CSatBIPGPRSDataChannel( 
        aUtils, aSocketServer, aChannelId, aConnection, aSubConnection );

    CleanupStack::PushL( self );
    self->ConstructL();

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::NewLC exiting" )
    return self;
    }

// ---------------------------------------------------------------------------
// destructor.
// ---------------------------------------------------------------------------
//
CSatBIPGPRSDataChannel::~CSatBIPGPRSDataChannel()
    {
    LOG( SIMPLE,
         "SATENGINE: CSatBIPGPRSDataChannel::~CSatBIPGPRSDataChannel calling" )

    // Close sender
    delete iSender;
    
    // Close receiver
    delete iReceiver;
    
    // Close receiver
    delete iSubConnEventObserver;
    
    // Close Socket
    iSocket.Close();
    
    // Delete buffers
    delete iSendStore;
    iRcvBuffer.Zero();
    
    LOG( SIMPLE,
         "SATENGINE: CSatBIPGPRSDataChannel::~CSatBIPGPRSDataChannel exiting" )
    }

// ---------------------------------------------------------------------------
// Sets up connection
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::SetupConnectionL( 
    const TSatBipConnectionInfo& aConnParams )
    {
    LOG( SIMPLE, 
        "SATENGINE: CSatBIPGPRSDataChannel::SetupConnectionL calling" )

    // Allocate Send buffer
    iSendStore = HBufC8::NewL( aConnParams.iBufferSize );

    // Store the rest of connection information
    iConnInfo = aConnParams;

    // Define used protocol
    iUseTcpProtocol = ( RSat::ETcp == iConnInfo.iProtocol.iTransportProto );

    // Set connection stage
    iConnStage = ESatBIPClosed;

    LOG( SIMPLE, 
        "SATENGINE: CSatBIPGPRSDataChannel::SetupConnectionL exiting" )
    }

// ---------------------------------------------------------------------------
// Opens the connection
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::ActivateConnectionL(
    MSatConnectionObserver* aObserver )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL calling" )
    
    TInt errCode( KErrNone );               // errCode
    RPacketQoS::TQoSGPRSNegotiated negQoS;  // The negotiated QoS
    
    // If the connection has not been setup or already connecting, leave
    if ( ESatBIPClosed != iConnStage )
        {
        LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL \
        ESatBIPClosed != iConnStage" )
        User::Leave( KErrCancel );
        }

    iConnectionObserver = aObserver;

    if ( RSat::EIPv4Address == iConnInfo.iDestination.iType )
        {
        LOG( NORMAL, 
             "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL\
             Using IPv4" )
        }
    else if ( RSat::EIPv6Address == iConnInfo.iDestination.iType )
        {
        LOG( NORMAL, 
             "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL\
             Using IPv6" )
        }
    else
        {
        LOG( NORMAL, "  Unknown IP type" )
        User::Leave( KErrUnknown );
        }
    
    // Open connection if it is inactive
    if ( !iUtils.IsConnectionActivated() )
        {
        LOG( NORMAL, 
             "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL\
             Connection is inactive, open connection " )
        // Write the primary PDP Context QoS Parameters to CMManager
        // before activating the primary PDP Context.  
        WritePrimaryPDPContextQoSParamsL();
        
        // Open sub-session to SocketServer    
        errCode = iConnection.Open( iSocketServ );
        LOG2( NORMAL, 
              "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL\
              RConnection opened with code: %i", errCode )
        User::LeaveIfError( errCode );
        
        // Start connection. If there are no override settings, 
        // use default settings
        if ( iConnInfo.iOverrideSet )
            {
            LOG( NORMAL, "Starting connection with override settings" )
            TConnPrefList prefList;
            TExtendedConnPref extPrefs;
            
            extPrefs.SetNoteBehaviour( 
                    iConnInfo.iOverrideSet->NoteBehaviour() );
            extPrefs.SetIapId( iConnInfo.iOverrideSet->IapId() );
            prefList.AppendL( &extPrefs );
            
            errCode = iConnection.Start( prefList );
            }
        else
            {
            LOG( NORMAL, "Starting connection with default settings" )
            errCode = iConnection.Start();
            }
        
        LOG2( NORMAL, 
              "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL\
              RConnection started with code: %i", errCode )
        User::LeaveIfError( errCode );
        
        // Erase the primary PDP Context QoS Parameters from CMManager 
        WriteDefaultQoSParamsL();
        
        // Set Connection activated
        iUtils.SetConnectionActivated( ETrue );
        // Make sure the subconnection is inactive.
        iUtils.SetSubConnectionActivated( EFalse );
        }
    else    
        {
        LOG( NORMAL, 
             "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL\
             Connection is active " )
        // Connection is active, open SubConnection if it is inactive
        if ( !iUtils.IsSubConnectionActivated() ) 
            {
            LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::\
            ActivateConnectionL SubConnection is inactive, open \
            subconnection" )
            // Open SubConnection 
            OpenSubConnection();
            }
        }
    
    // Set up QoS values
    SetConnQoSParamsL();
    
    // Open socket
    OpenSocketL();
    
    // UDP Socket doesn't need ActiveObject, so we don't have to wait.
    if ( !iUseTcpProtocol )
        {
        LOG( NORMAL, 
             "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL\
             UDP protocol " )
        StartReceiveL();
        iConnStage = ESatBIPConnected;
        
        // Notify ConnectionObserver
        iConnectionObserver->ConnectionNotification( KErrNone );
        }
    
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL exiting" )
    }

// ---------------------------------------------------------------------------
// ends or stores the data.
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::GetNegotiatedQoSParams( 
    RSat::TBearerParams& aResult ) const
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::NegotiatedQoSParams\
        calling - exiting" )
    aResult = iConnInfo.iBearerParams;
    }

// ---------------------------------------------------------------------------
// Sends or stores the data.
// ---------------------------------------------------------------------------
//
TInt CSatBIPGPRSDataChannel::SendDataL( const TDes8& aData,
    const TBool aSendImmediately,
    TInt& aFreeBufferSize,
    MSatSendDataObserver* aObserver )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::SendDataL calling" )

    TInt errCode( MSatBIPUtils::ESatBIPSuccess );
    CSatSActiveWrapper* delay = NULL;
    TInt numberOfTries( 1 );
    // Check status of connection
    TBool suspended( RPacketService::EStatusSuspended == 
        iUtils.ConnectionStatus() );

    // Check that does the data fit in buffer
    const TInt bufsize( iSendStore->Length() );

    LOG2( NORMAL,
        "SATENGINE: CSatBIPGPRSDataChannel::SendDataL iSendBuffer size: %i",
        bufsize )
    LOG2( NORMAL,
        "SATENGINE: CSatBIPGPRSDataChannel::SendDataL aData size: %i",
        aData.Length() )
    if ( suspended )
        {
        // for async wait
        delay = new ( ELeave ) CSatSActiveWrapper();
        }
    
    while ( suspended && KMaxAttempts >= numberOfTries ) 
        {
        // In cases of SMS or Call suspend the pdp context, we need to 
        // evaluate the status of context after a small delay
        LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::SendDataL, wait, %d "\
            , numberOfTries )
        delay->After( KReCheckStatusDelay );
        suspended = RPacketService::EStatusSuspended == iUtils.ConnectionStatus();
        numberOfTries++;
        }
    
    delete delay;
    delay = NULL;
            
    if ( suspended && aSendImmediately )
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::SendDataL \
            Connection suspended" )
        errCode = MSatBIPUtils::ESatBIPServiceError;
        }
    else if ( iConnInfo.iBufferSize < ( aData.Length() + bufsize ) )
        {
        LOG( NORMAL,
            "SATENGINE: CSatBIPGPRSDataChannel::SendDataL Buffer overflow" )
        errCode = MSatBIPUtils::ESatBIPReqBufSizeNotAvail;
        }
    else
        {

        // Append data to send buffer
        TPtr8 ptr = iSendStore->Des();
        ptr.Append( aData );

        // Send immediately, if required
        if ( aSendImmediately )
            {
            LOG( NORMAL,
                "SATENGINE: CSatBIPGPRSDataChannel::SendDataL Sending data" )

            // If Sender object is NULL, create it
            if ( !iSender && aObserver )
                {
                LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::SendDataL \
                create iSender" )
                iSender = new ( ELeave ) CSatBIPDataSender(
                        *aObserver, iSocket, iUseTcpProtocol,
                        iConnInfo.iBufferSize );
                }

            if ( iSender )
                {
                LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::SendDataL \
                iSender true" )
                // Send data
                iSender->SendData( ptr, iDestAddr );

                // Remove data from buffer.
                iSendStore = HBufC8::NewL( iConnInfo.iBufferSize );
                }
            }
        }

    // Place the number of bytes of empty space in the buffer to aFreeBufferSize
    aFreeBufferSize = ( iConnInfo.iBufferSize - iSendStore->Length() );

    LOG2( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::SendDataL exiting, \
          errCode: %d", errCode )
    return errCode;
    }

// ---------------------------------------------------------------------------
// Returns the received data in param aData
// ---------------------------------------------------------------------------
//
TInt CSatBIPGPRSDataChannel::ReceiveDataL( TDes8& aData,
    const TInt aBytesToRead, TInt& aAvailableBytes )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::ReceiveDataL calling" )

    TInt errCode( MSatBIPUtils::ESatBIPSuccess );

    // Number of bytes to return, may not be the same as requested
    TInt bytesToReturn( aBytesToRead );

    const TInt bytesAvail( iRcvBuffer.Length() );

    // Compare the number of bytes received and the number of bytes requested.
    if ( bytesToReturn > bytesAvail )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::ReceiveDataL \
        bytesToReturn > bytesAvail" )
        // There are no requested bytes availabe, we return all we have.
        errCode = MSatBIPUtils::ESatBIPReqBufSizeNotAvail;
        bytesToReturn = bytesAvail;
        }

    LOG2( NORMAL, "  ReceiveDataL: Data available for receive: %i", bytesAvail )

    // Put the data into param aData.
    aData = iRcvBuffer.Left( bytesToReturn );
    // Remove read data from receive store
    iRcvBuffer.Delete( 0, bytesToReturn );
    // Put the number of unread bytes into param aAvailableBytes.
    aAvailableBytes = iRcvBuffer.Length();
    
    LOG2( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::ReceiveDataL \
          aAvailableBytes: %d", aAvailableBytes )
    // Notify receiver if store is empty to get the next datagram in UDP sockets
    if ( ( 0 == aAvailableBytes ) && iReceiver )
        {
        iReceiver->ReceiveStoreEmptyNotificationL();
        }

    LOG2( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::ReceiveDataL exiting. \
          errCode: %d", errCode )
    return errCode;
    }

// ---------------------------------------------------------------------------
// Returns the identifier of this channel
// ---------------------------------------------------------------------------
//
TInt CSatBIPGPRSDataChannel::ChannelId() const
    {
    LOG2( SIMPLE,
    "SATENGINE: CSatBIPGPRSDataChannel::ChannelId calling - exiting,\
    ChannelId: %i", iChannelId.ChannelId() )
    return iChannelId.ChannelId();
    }

// ---------------------------------------------------------------------------
// Returns the identifier of this channel. This function is used only when
// generating ChannelStatus TLV object
// ---------------------------------------------------------------------------
//
TUint8 CSatBIPGPRSDataChannel::ChannelStatusChannelId() const
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::ChannelStatusChannelId calling" )

    TUint8 channelId( iChannelId.ShortChannelId() );

    LOG2( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::ChannelStatusChannelId exiting \
        with channel id: %i", channelId )
    return channelId;
    }

// ---------------------------------------------------------------------------
// Returns the status of this data channel
// ---------------------------------------------------------------------------
//
TInt CSatBIPGPRSDataChannel::Status() const
    {
    LOG2( SIMPLE, 
    "SATENGINE: CSatBIPGPRSDataChannel::Status calling - exiting,\
    iChannelStatus: %i", iChannelStatus )
    return iChannelStatus;
    }

// ---------------------------------------------------------------------------
// Closes this data channel
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::CloseChannel()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::CloseChannel calling" )

    if ( ESatBIPConnected == iConnStage ||
         ESatBIPCancelled == iConnStage )
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::CloseChannel \
            Channel is connected" )
        CancelAll();
        // Close socket
        iSocket.Close();

        iConnStage = ESatBIPClosed;
        }
    else if ( iConnStage == ESatBIPConnect &&
              iUseTcpProtocol )
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::CloseChannel \
            Channel is connecting" )
        // If Channel is connecting and protocol is TCP,
        // have to call Cancel instead of CancelAll since this is an active
        // object in this situation
        Cancel();
        // Close socket
        iSocket.Close();
        
        iConnStage = ESatBIPClosed;
        }
    else
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::CloseChannel \
            Channel already closed" )
        }

    // Release channel ID
    iChannelId.ReleaseChannel();

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::CloseChannel exiting" )
    }

// ---------------------------------------------------------------------------
// Performs cancel actions.
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::CancelAll()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::CancelAll calling" )

    // Cancel all activity in sockets
    if ( iConnStage == ESatBIPConnected )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::CancelAll \
        iConnStage == ESatBIPConnected" )
        // Cancel sender
        if ( iSender )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::CancelAll \
            cancel iSender" )
            iSender->Cancel();
            }

        // Cancel receiver
        if ( iReceiver )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::CancelAll \
            cancel iReceiver" )
            iReceiver->Cancel();
            }

        // Cancel all actions
        iSocket.CancelAll();
        // Link dropped, when socket cancels all
        iChannelStatus = MSatBIPUtils::ESatLinkDropped;
        iConnStage = ESatBIPCancelled;
        }
    else if ( iConnStage == ESatBIPConnect )
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::CancelAll \
            Cancelling connect" )
        // Cancel connection negotiations
        iSocket.CancelConnect();
        iConnStage = ESatBIPCancelled;
        }
    else
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::CancelAll \
            Channel already cancelled or closed" )
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::CancelAll exiting" )
    }

// ---------------------------------------------------------------------------
// Returns the status of this data channel
// ---------------------------------------------------------------------------
//
TBool CSatBIPGPRSDataChannel::IsContextActive() const
    {
    LOG2( SIMPLE,
    "SATENGINE: CSatBIPGPRSDataChannel::IsContextActive calling - exiting,\
    return: %d", ESatBIPConnected == iConnStage )
    return ESatBIPConnected == iConnStage;
    }

// ---------------------------------------------------------------------------
// Returns the status of this data channel
// ---------------------------------------------------------------------------
//
const MSatBIPDataChannel::TSatBipConnectionInfo& CSatBIPGPRSDataChannel::ConnInfo() const
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::ConnInfo calling - exiting" )
    return iConnInfo;
    }

// -----------------------------------------------------------------------------
// CSatBIPGPRSDataChannel::StopUdpSocket
// -----------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::StopUdpSocket()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::StopUdpSocket calling" )
    
    if ( !iUseTcpProtocol )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::StopUdpSocket UDP" )
    iSocket.Close();
    }
        
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::StopUdpSocket exiting" )
    }

// ---------------------------------------------------------------------------
// From CActive
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::RunL()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::RunL calling " )

    if ( KErrNone == iStatus.Int() )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::RunL \
        KErrNone == iStatus.Int() " )
        // Check connection stage
        switch ( iConnStage )
            {
            case ESatBIPConnect:
                {
                LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::RunL \
                    Connect OK" )
                    
                // Start receiving data
                StartReceiveL();
                iConnStage = ESatBIPConnected;
                // Notify ConnectionObserver
                iConnectionObserver->ConnectionNotification( KErrNone );
                break;
                }

            case ESatBIPCancelled:
                {
                LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::RunL \
                    Connection Cancelled" )
                // Notify ConnectionObserver
                iConnectionObserver->ConnectionNotification( KErrCancel );
                break;
                }

            default:
                {
                LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::RunL default" )
                PanicSatEngine( ESatBIPAccessViolation );
                }
            }
        }
    else
        {
        // Notify ConnectionObserver with error
        iConnectionObserver->ConnectionNotification( iStatus.Int() );
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::RunL exiting " )
    }

// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::DoCancel()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::DoCancel calling" )

    CancelAll();

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::DoCancel calling" )
    }

// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//
TInt CSatBIPGPRSDataChannel::RunError( TInt aError )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::RunError calling" )

    if ( iConnectionObserver )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::RunError \
        iConnectionObserver true" )
        iConnectionObserver->ConnectionNotification( aError );
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::RunError calling" )
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Stores received data into buffer.
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::DataReceivedNotificationL( const TDesC8& aData )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::DataReceivedNotification calling" )

    // Check do we have to report data receive
    const TInt bufsize( iRcvBuffer.Length() );
#ifdef ENABLE_SAT_LOGGING
    TBuf<KIpAddressLogSize> addressForLog;
#endif
    // LOG values
    LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::RunL\
        ReceiveStore length: %i", bufsize )

    // In UDP context, we can store only one datagram at a time. If there is
    // previous datagram in store, we'll have to wait for the UICC to get all
    // before storing the netx datagram and to start receive again. If there are
    // more than one datagrams pending, they are stored in RSockets queue.
    if ( bufsize == 0 || RSat::ETcp == iConnInfo.iProtocol.iTransportProto )
        {
#ifdef ENABLE_SAT_LOGGING
        iSourceAddr.Output( addressForLog );
        LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::RunL\
            Received data length: %i", aData.Length() )
        LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::RunL\
            Received from address: %S", &addressForLog )
        LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::RunL\
            Received from port: %i", iSourceAddr.Port() )
#endif
        // Store data, if it fits into store, else, do not receive until store
        // is empty.
        if ( KSatBIPMinReceiveBufferSize >= ( aData.Length() + bufsize ) )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
                 DataReceivedNotification store data" )
            iRcvBuffer.Append( aData );

            // It is possible that aData is empty, because this function 
            // is called
            // every time when SIM gets the final bytes from ReceiveStore.
            if ( 0 == bufsize && aData.Length() > 0 )
                {
                LOG( SIMPLE,
                "SATENGINE: CSatBIPGPRSDataChannel::DataReceivedNotification \
                aData.Length() > 0" )
                // If the store is empty, send event download
                iUtils.DataAvailable( ChannelId(), aData.Length() );
                }

            // Start to receive again. This should never leave, because Receiver
            // object is already created.
            StartReceiveL();
            }
        }

    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::DataReceivedNotification exiting" )
    }

// ---------------------------------------------------------------------------
// Error while receiving data
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::DataReceiveError( TInt aError )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::DataReceiveError calling" )

    if ( KErrCancel != aError ) // Don't do anything if cancelled
        {
        LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::DataReceiveError\
             Error: %i", aError )
        // Report error
        iConnStage = ESatBIPCancelled; 
        iChannelStatus = MSatBIPUtils::ESatLinkDropped;
        iUtils.ChannelStatus( ChannelId(), iChannelStatus );
        }
    else
        {
        // Cancelled
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::DataReceiveError\
            Receive has been cancelled cancelled" )
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::DataReceiveError exiting" )
    }

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CSatBIPGPRSDataChannel::CSatBIPGPRSDataChannel( CSatBIPUtils& aUtils,
    RSocketServ& aSocketServer,
    TSatChannelIdInfo& aChannelId,
    RConnection& aConnection,
    RSubConnection& aSubConnection) :
    CActive( EPriorityStandard ),
    iUtils( aUtils ),
    iSocketServ( aSocketServer ),
    iChannelId( aChannelId ),
    iConnection( aConnection ),
    iSubConnection( aSubConnection )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::CSatBIPGPRSDataChannel calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::CSatBIPGPRSDataChannel exiting" )
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::ConstructL calling" )

    iChannelStatus = MSatBIPUtils::ESatNoFurtherInformation;
    
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::ConstructL exiting" )
    }

// ---------------------------------------------------------------------------
// Opens and connects / binds Socket
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::OpenSocketL()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL calling" )

    TInt errCode( KErrNone );

    // Define Destination address
    // Create InetAddress to hold remote device information
    LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
         Port from SIM: %d", iConnInfo.iProtocol.iPrtNumber )

    // Remote server's IP Address
#ifdef SAT_USE_DUMMY_TSY
    TBuf8<RSat::KPcktAddressMaxSize> destAddr( RSat::KPcktAddressMaxSize );
    
    // KAfInet or KAfInet6
    if ( RSat::EIPv6Address == iConnInfo.iDestination.iType )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL IPv6" )
        // 0:0:0:0:0:0:0:2 is equivalent to 127, 0, 0, 2
        for ( TInt ndx = 0 ; ndx < KIpAddressSize ; ndx++ )
            {
            destAddr[ndx] = 0;
            }
        destAddr[KIpAddressSize] = KIpAddressDValueDestination;
        destAddr.SetLength( KIpv6Length );
        }
    else //ipv4
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL IPv4" )
        destAddr[KIpAddressA] = KIpAddressAValue;
        destAddr[KIpAddressB] = KIpAddressBValue;
        destAddr[KIpAddressC] = KIpAddressCValue;
        destAddr[KIpAddressD] = KIpAddressDValueDestination;
        destAddr.SetLength( KIpv4Length );
        }
    iDestAddr = DestAddress( destAddr );
#else
    iDestAddr = DestAddress( iConnInfo.iDestination.iAddress );
#endif // SAT_USE_DUMMY_TSY
    
    LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
         Complete defining destination address" )
    
    iSourceAddr = iDestAddr;
    
    TUint socketType( KSockStream );
    TUint protocol( KProtocolInetTcp );
    
    // Check is it UDP or TCP
    if ( !iUseTcpProtocol )
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
            Opening UDP datagram socket" )
        socketType = KSockDatagram;
        protocol = KProtocolInetUdp;
        }
    else
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
            Opening TCP stream socket" )
        }
        
    // Open socket
    if ( iUtils.IsSubConnectionActivated() )
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
        using subconnection" )
        errCode = iSocket.Open( iSocketServ, KAfInet, socketType,
            protocol, iSubConnection );
        }
    else
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
        using connection" )
        errCode = iSocket.Open( iSocketServ, KAfInet, socketType,
            protocol, iConnection );
        }
        
    // Check socket activation
    LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
        Socket opened with code: %i", errCode )
    User::LeaveIfError( errCode );
    
    // Define local address. Local port is obtained automatically
    TInetAddr localAddress( KInetPortAny );
    
#ifdef SAT_USE_DUMMY_TSY
        // win2000 doesn't support IPv6, so force to IPv4
        localAddress.SetFamily( KAfInet );
        // KAfInet or KAfInet6
        if ( RSat::EIPv6Address == iConnInfo.iDestination.iType )
            {
            LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
                IPv6" )
            // 0:0:0:0:0:0:0:3 is equivalent to 127, 0, 0, 3
            TIp6Addr ip6Adrr;
            
            for ( TInt index = 0 ; index < KIpAddressSize ; index++ )
                {
                ip6Adrr.u.iAddr8[index] = 0;
                }
                
            ip6Adrr.u.iAddr8[KIpAddressSize] = KIpAddressDValueLocal;
            localAddress.SetAddress( ip6Adrr );
            }
        else //ipv4
            {
            LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
                IPv4" )
            localAddress.SetAddress( INET_ADDR(
                KIpAddressAValue,
                KIpAddressBValue,
                KIpAddressCValue,
                KIpAddressDValueLocal ) );
            }
#else
    // Check does the SIM provide a local address
    if ( 0 < iConnInfo.iSource.iAddress.Length() )
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
            Setting pre-defined local address" )
        localAddress = DestAddress( iConnInfo.iSource.iAddress );
        localAddress.SetPort( KInetPortAny );
        }
    else
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
            Setting automatic local address" )
        // Unspecified. Local address is obtained automatically.
        localAddress.SetFamily( KAFUnspec );
        }
#endif       
    LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
        Bind local address" )
    errCode = iSocket.Bind( localAddress );  
        
    // Check what was the response from "Bind"
    if ( KErrNone == errCode )
        {
        // Set status to indicate that connection attempt ongoing
        iConnStage = ESatBIPConnect;
        
        if ( iUseTcpProtocol )
            {
            // Connect the socket
            iSocket.Connect( iDestAddr, iStatus );
            SetActive();
            }
        else
            {
            // This option redefines the receive buffer size
            iSocket.SetOpt( KSoUdpRecvBuf, KSolInetUdp, 
                KSatBIPMinReceiveBufferSize );
            // Connect the socket
            iSocket.Connect( iDestAddr, iStatus );
            // UDP sockets dont need active object
            User::WaitForRequest( iStatus );
            LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
                UDP Socket connect status  %i", iStatus.Int() )
            }
        }
    else
        {
        // Send Terminal Response indicating that static address allocation
        // could not be done (KErrNotFound) or Bind failed for some other 
        // reason
        LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL\
            Bind error:  %i", errCode )
        iConnectionObserver->ConnectionNotification( errCode );
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::OpenSocketL exiting" )
    }

// ---------------------------------------------------------------------------
// Opens SubConnection and attaches socket into it
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::OpenSubConnection()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::OpenSubConnection calling" )
    
    TInt errCode( KErrNone );
    
    // Create SubConnection. This is mandatory for multiple PDP Context support
    LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSubConnection\
        Opening Sub-connection" )
    
    errCode = iSubConnection.Open( iSocketServ, RSubConnection::ECreateNew, 
                                   iConnection );
    
    LOG2( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSubConnection\
        RSubConnection opened with code: %i", errCode )
    
    if ( KErrNone != errCode )
        {
        // No subconnections, Affects on sockets
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSubConnection\
                RSubConnection is opened incorrectly" )
        iSubConnection.Close();
        iUtils.SetSubConnectionActivated( EFalse );
        }
    else 
        {
        LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::OpenSubConnection\
                RSubConnection is opened correctly" )
        iUtils.SetSubConnectionActivated( ETrue );
        }
        
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::OpenSubConnection exiting" )
    }

// ---------------------------------------------------------------------------
// Sets QoS parameters to active SubConnection
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::SetSubConQoSParamsL()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::SetSubConQoSParamsL calling" )
    
    const TInt paramLength( iConnInfo.iBearerParams.Length() );
    TInt errCode( KErrNone );

    LOG2( NORMAL, "  QoS Parameters length: %i", paramLength )

    // GPRS requires 6 parameters from SIM
    if ( KReqGprsQoSParams == paramLength )
        {
        // Check that is packet protocol valid, ie. IP protocol
        if ( KReqPacketProtType != iConnInfo.iBearerParams[KProtocolPos] )
            {
            LOG( NORMAL, 
                "SATENGINE: CSatBIPGPRSDataChannel::SetSubConQoSParamsL\
                Requested packet protocol is not valid" )
            User::Leave( KErrNotSupported );
            }
        }
    else
        {
        LOG( NORMAL, 
            "SATENGINE: CSatBIPGPRSDataChannel::SetSubConQoSParamsL\
            Requested packet protocol is valid, not support" )
        User::Leave( KErrNotSupported );
        }

    // Creating and setting properties for a sub-connection
    // Create a parameter bundle and add to the cleanup stack
    RSubConParameterBundle parameterBundle;
    CleanupClosePushL( parameterBundle );
        
    // Create a family owned by parameterBundle
    CSubConParameterFamily* parameterFamily = 
        CSubConParameterFamily::NewL( parameterBundle, KSubConQoSFamily );
    
    // Create extension paramset(Rel5) owned by parameterFamily
    CSubConQosR5ParamSet* requestedQoSRel5 = 
        CSubConQosR5ParamSet::NewL( *parameterFamily, 
                                    CSubConParameterFamily::ERequested );
    
    if( requestedQoSRel5 )
        {
        RPacketQoS::TQoSGPRSRequested reqQoS;
        TSatQoSParser::GetRequestedQoSValues( iConnInfo.iBearerParams, 
            reqQoS );
        TSatQoSParser::TQoSRel5 qosRel5 = TSatQoSParser::ConvertQoS( reqQoS );
        // Set some requested QoS values
        requestedQoSRel5->SetTrafficClass( 
            static_cast<RPacketQoS::TTrafficClass>( qosRel5.iTrafficClass ) );
        requestedQoSRel5->SetTrafficHandlingPriority( 
            static_cast<RPacketQoS::TTrafficHandlingPriority>( 
                qosRel5.iTrafficHandlingPriority ) );
        requestedQoSRel5->SetSDUErrorRatio( 
            static_cast<RPacketQoS::TSDUErrorRatio>( qosRel5.iSduErrorRatio ) );
        requestedQoSRel5->SetResidualBitErrorRatio( 
            static_cast<RPacketQoS::TBitErrorRatio>( qosRel5.iResidualBer ) );
        requestedQoSRel5->SetErroneousSDUDelivery( 
            static_cast<RPacketQoS::TErroneousSDUDelivery>( 
                qosRel5.iDeliveryErroneousSdu ) );
        requestedQoSRel5->SetMaxBitrateUplink( qosRel5.iMaxBitRate );
        requestedQoSRel5->SetMaxBitrateDownlink( qosRel5.iMaxBitRate );
        requestedQoSRel5->SetMaxSduSize( qosRel5.iMaximumSDUSize );
        requestedQoSRel5->SetDeliveryOrder( 
            static_cast<RPacketQoS::TDeliveryOrder>( qosRel5.iDeliveryOrder ) );
        LOG( SIMPLE, 
            "SATENGINE: CSatBIPGPRSDataChannel::SetSubConQoSParamsL\
            Setting requested QoS values for subconn" )
        
        // Start observing granted event.
        if( !iSubConnEventObserver )
            {
            LOG( SIMPLE, 
            "SATENGINE: CSatBIPGPRSDataChannel::SetSubConQoSParamsL\
            iSubConnEventObserver false" )
            iSubConnEventObserver = 
                new ( ELeave ) CSatBIPSubConnEventObserver( *this );
            }
        iSubConnEventObserver->StartObservSubConnEvent();
        
        // Set parameters
        errCode = iSubConnection.SetParameters( parameterBundle );
        LOG2( NORMAL, 
            "SATENGINE: CSatBIPGPRSDataChannel::SetSubConQoSParamsL\
	        calling iSubConnection.SetParameters (parameterBundle) = %d", 
	        errCode )
        }
    else
        {
        LOG( SIMPLE,
            "SATENGINE: CSatBIPGPRSDataChannel::SetSubConQoSParamsL\
            Can't create CSubConQosR5ParamSet" )
        errCode = KErrNotSupported;
        }
    
    // Pop and close parameterBundle
    CleanupStack::PopAndDestroy();
    
    User::LeaveIfError( errCode );
        
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::SetSubConQoSParamsL exiting" )
    }

// ---------------------------------------------------------------------------
// Starts to receive data from Socket
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::StartReceiveL()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::StartReceiveL calling" )

    // If receiver is NULL, create it
    if ( NULL == iReceiver )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::StartReceiveL \
        create iReceiver" )
        iReceiver = new ( ELeave ) CSatBIPDataReceiver(
            *this, iSocket, iUseTcpProtocol );
        }

    // Tell receiver to start receive
    iReceiver->StartReceive( iSourceAddr );

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::StartReceiveL exiting" )
    }

// ---------------------------------------------------------------------------
// Defines address
// ---------------------------------------------------------------------------
//
TInetAddr CSatBIPGPRSDataChannel::DestAddress( const TPtrC8& aAddr )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::DestAddress calling" )

    TInetAddr destinationAddress;

    const TInt addLength( aAddr.Length() );
    LOG2( NORMAL, "  Address length is %i", addLength )

#ifdef ENABLE_SAT_LOGGING
    TBuf<KIpAddressLogSize> addressForLog;
#endif
    // Check IPv4 address
    if ( KIPv4Length == addLength )
        {
        TInt index = 0;
        TUint8 first( aAddr[index++] );
        TUint8 second( aAddr[index++] );
        TUint8 third( aAddr[index++] );
        TUint8 fourth( aAddr[index++] );

        // This macro creates IPAddress into TUint32 format.
        //lint -e{1924} Problem inside epoc macro, nothing to do.
        destinationAddress = TInetAddr( INET_ADDR( first, second, third, fourth ),
            iConnInfo.iProtocol.iPrtNumber );
        destinationAddress.SetFamily( KAfInet );
#ifdef ENABLE_SAT_LOGGING
        destinationAddress.Output( addressForLog );
        LOG2( NORMAL, "  Using IPv4, Address is %S", &addressForLog )
#endif
        }
    // Check IPv6 address
    else if ( KIPv6Length == addLength )
        {
        TIp6Addr ip6Adrr;
        for ( TInt index = 0 ; index < addLength ; index++ )
            {
            ip6Adrr.u.iAddr8[index] = aAddr[index];
            }

        destinationAddress = TInetAddr( ip6Adrr, iConnInfo.iProtocol.iPrtNumber );
        destinationAddress.SetFamily( KAfInet6 );
#ifdef ENABLE_SAT_LOGGING
        destinationAddress.Output( addressForLog );
        LOG2( NORMAL, "  Using IPv6, Address is %S", &addressForLog )
#endif
#ifdef SAT_USE_DUMMY_TSY
        // Using Dummy TSY, overwriting IPv6 address to IPv4 since
        // Windows 2000 doesn't support IPv6
        destinationAddress.ConvertToV4();
        destinationAddress.SetFamily( KAfInet );
#ifdef ENABLE_SAT_LOGGING
        destinationAddress.Output( addressForLog );
        LOG2( NORMAL, "  Converted IPv6 address to IPv4,\
             Address is %S", &addressForLog )
#endif
        destinationAddress.SetAddress( INET_ADDR(
            KIpAddressAValue,
            KIpAddressBValue,
            KIpAddressCValue,
            KIpAddressDValueDestination ) );
#ifdef ENABLE_SAT_LOGGING
        destinationAddress.Output( addressForLog );
        LOG2( NORMAL, "  Overwrited IPv4 localhost,\
             Address is %S", &addressForLog )
#endif
#endif //SAT_USE_DUMMY_TSY
        }
    else // Invalid IP address length
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::DestAddress \
        invalid IP address" )
        destinationAddress = TInetAddr( 0 , 0 );
        destinationAddress.SetFamily( KAFUnspec );
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::DestAddress exiting" )
    return destinationAddress;
    }

// ---------------------------------------------------------------------------
// Write Primary PDP Context QoS parameters to RCmManager
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::WritePrimaryPDPContextQoSParamsL()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::WritePrimaryPDPContextQoSParamsL \
        calling" )

    if ( iConnInfo.iOverrideSet )
        {
        LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::WritePrimaryPDPContextQoSParamsL \
        iConnInfo.iOverrideSet true" )
        
        TUint32 iapId( iConnInfo.iOverrideSet->IapId() );
        LOG2( SIMPLE, 
              "SATENGINE: CSatBIPGPRSDataChannel::WritePrimaryPDPContextQoSParamsL \
              IAP id = %d", iapId )
        
        RPacketQoS::TQoSGPRSRequested reqQoS;
        TSatQoSParser::GetRequestedQoSValues( iConnInfo.iBearerParams, 
            reqQoS );
        TSatQoSParser::TQoSRel5 qosRel5 = TSatQoSParser::ConvertQoS( reqQoS );    
        
        RCmManager cmManager;
        cmManager.OpenLC();

        RCmConnectionMethod cm = cmManager.ConnectionMethodL( iapId );
        CleanupClosePushL( cm );
        
        // Requested traffic class.
        // Type of application for which the UMTS bearer service is optimised
        cm.SetIntAttributeL( CMManager::EGPRSReqTrafficClass, 
                             qosRel5.iTrafficClass );
        LOG2( SIMPLE, "  EGPRSReqTrafficClass:    0x%x", 
              qosRel5.iTrafficClass )
        
        // Requested traffic handling priority.                     
        // Specifies the relative importance for handling of all SDUs belonging
        // to the UMTS bearer compared to the SDUs of other bearers.
        cm.SetIntAttributeL( CMManager::EGPRSReqTrafficHandlingPriority, 
                             qosRel5.iTrafficHandlingPriority );
        LOG2( SIMPLE, "  EGPRSReqTrafficHandlingPriority:    0x%x", 
              qosRel5.iTrafficHandlingPriority )                
        
        // Requested target SDU error ratio.
        // Indicates the fraction of SDUs lost or detected as erroneous.               
        cm.SetIntAttributeL( CMManager::EGPRSReqSDUErrorRatio, 
                             qosRel5.iSduErrorRatio );
        LOG2( SIMPLE, "  EGPRSReqSDUErrorRatio:    0x%x", 
              qosRel5.iSduErrorRatio )                         
                             
        // Requested target Bit error ratio.
        // Indicates the undetected bit error ratio in the delivered SDU.
        cm.SetIntAttributeL( CMManager::EGPRSReqBER, 
                             qosRel5.iResidualBer);
        LOG2( SIMPLE, "  EGPRSReqBER:    0x%x", 
              qosRel5.iResidualBer )                          
              
        // Requested value for erroneous SDU delivery. 
        // Indicates whether SDUs detected as erroneous shall be
        // delivered or discarded
        cm.SetIntAttributeL( CMManager::EGPRSReqDeliverErroneousSDU, 
                             qosRel5.iDeliveryErroneousSdu );
        LOG2( SIMPLE, "  EGPRSReqDeliverErroneousSDU:    0x%x", 
              qosRel5.iDeliveryErroneousSdu )
        
        // Requested maximum bit rates on downlink.                     
        cm.SetIntAttributeL( CMManager::EGPRSReqMaxDownlinkRate, 
                             qosRel5.iMaxBitRate );
        LOG2( SIMPLE, "  EGPRSReqMaxDownlinkRate:    0x%x", 
              qosRel5.iMaxBitRate )
        
        // Requested maximum bit rates on uplink      
        cm.SetIntAttributeL( CMManager::EGPRSReqMaxUplinkRate, 
                             qosRel5.iMaxBitRate );
        LOG2( SIMPLE, "  EGPRSReqMaxUplinkRate:    0x%x", 
              qosRel5.iMaxBitRate )       
        
        // Request maximum SDU size.
        // The maximum SDU size for which the network shall 
        // satisfy the negotiated QoS
        cm.SetIntAttributeL( CMManager::EGPRSReqMaxSDUSize, 
                             qosRel5.iMaximumSDUSize );
        LOG2( SIMPLE, "  EGPRSReqMaxSDUSize:    0x%x", 
              qosRel5.iMaximumSDUSize )
        
        // Requested value for sequential SDU delivery.
        // Indicates whether the UMTS bearer shall provide 
        // in-sequence SDU delivery or not.
        cm.SetIntAttributeL( CMManager::EGPRSReqDeliveryOrder, 
                             qosRel5.iDeliveryOrder );
        LOG2( SIMPLE, "  EGPRSReqDeliveryOrder:    0x%x", 
              qosRel5.iDeliveryOrder )

        cm.UpdateL();
        
        CleanupStack::PopAndDestroy( &cm );
        CleanupStack::PopAndDestroy( &cmManager );
        }
   
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::WritePrimaryPDPContextQoSParamsL \
        exiting" )
    }

// ---------------------------------------------------------------------------
// Write default QoS Parameters to RCmManager
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::WriteDefaultQoSParamsL()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::WriteDefaultQoSParamsL \
        calling" )
        
    if ( iConnInfo.iOverrideSet )
        {
        LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::WriteDefaultQoSParamsL \
        iConnInfo.iOverrideSet true" );
        TUint32 iapId( iConnInfo.iOverrideSet->IapId() ); 
        LOG2( SIMPLE, 
              "SATENGINE: CSatBIPGPRSDataChannel::WriteDefaultQoSParamsL \
              IAP id = %d", iapId )        
        
        RCmManager cmManager;
        cmManager.OpenLC();

        RCmConnectionMethod cm = cmManager.ConnectionMethodL( iapId );
        CleanupClosePushL( cm );
        
        cm.SetIntAttributeL( CMManager::EGPRSReqTrafficClass, 
                             RPacketQoS::ETrafficClassUnspecified );
                             
        cm.SetIntAttributeL( CMManager::EGPRSReqTrafficHandlingPriority, 
                             RPacketQoS::ETrafficPriorityUnspecified );
                             
        cm.SetIntAttributeL( CMManager::EGPRSReqSDUErrorRatio, 
                             RPacketQoS::ESDUErrorRatioUnspecified );
                             
        cm.SetIntAttributeL( CMManager::EGPRSReqBER, 
                             RPacketQoS::EBERUnspecified );
        
        cm.SetIntAttributeL( CMManager::EGPRSReqDeliverErroneousSDU, 
                             RPacketQoS::EErroneousSDUDeliveryUnspecified );
                             
        cm.SetIntAttributeL( CMManager::EGPRSReqMaxDownlinkRate, 0 );
        
        cm.SetIntAttributeL( CMManager::EGPRSReqMaxUplinkRate, 0 );
        
        cm.SetIntAttributeL( CMManager::EGPRSReqMaxSDUSize, 0 );
        
        cm.SetIntAttributeL( CMManager::EGPRSReqDeliveryOrder, 
                             RPacketQoS::EDeliveryOrderUnspecified );

        LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::WriteDefaultQoSParamsL \
        set default QoS params" );

        cm.UpdateL();
        
        CleanupStack::PopAndDestroy( &cm );
        CleanupStack::PopAndDestroy( &cmManager );
        }
    
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::WriteDefaultQoSParamsL \
        exiting" )
    }

// ---------------------------------------------------------------------------
// Set QoS for connection or subconnection
// ---------------------------------------------------------------------------
//
void CSatBIPGPRSDataChannel::SetConnQoSParamsL()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::SetConnQoSParamsL calling" )
    
    RPacketQoS::TQoSGPRSNegotiated negQoS;  // The negotiated QoS
    
    // If using Dummy TSY, set the negotiated QoS directly by hard-codes,
    // otherwise, fetch negotiated QoS for primary context(by ETel API), set QoS
    // for secondary context. 
#ifdef SAT_USE_DUMMY_TSY    
    // Set the negotiated QoS directly if using Dummy TSY
    LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL\
        don't need to set qos for dummy tsy" )
    negQoS.iPrecedence = RPacketQoS::EPriorityHighPrecedence;
    negQoS.iDelay = RPacketQoS::EDelayClass4;
    negQoS.iReliability = RPacketQoS::EReliabilityClass3;
    negQoS.iPeakThroughput = RPacketQoS::EPeakThroughput256000;
    negQoS.iMeanThroughput = RPacketQoS::EUnspecifiedMeanThroughput;
    TSatQoSParser::GetNegotiatedQoSValues( negQoS, iConnInfo.iBearerParams );
    // Set last bearer param to IP Protocol
    iConnInfo.iBearerParams[KProtocolPos] = KReqPacketProtType;     
#else   // Set QoS for channels
    if ( iUtils.IsSubConnectionActivated() ) 
        {
        // Set QoS params, granted QoS will be fetched after connected
         LOG( NORMAL, "SATENGINE: CSatBIPGPRSDataChannel::ActivateConnectionL\
                Set QoS values for SubConnection" )
        SetSubConQoSParamsL();
        }
    else
        {
        // Primary PDP Context need to be fetched by ETel API
         RPacketQoS::TQoSGPRSRequested reqQoS;
         TSatQoSParser::GetRequestedQoSValues( iConnInfo.iBearerParams, 
            reqQoS );
        negQoS = iUtils.ProposeQoSParametersL( reqQoS );
        TSatQoSParser::GetNegotiatedQoSValues( negQoS, 
            iConnInfo.iBearerParams );
        // Set last bearer param to IP Protocol
        iConnInfo.iBearerParams[KProtocolPos] = KReqPacketProtType;      
        }
#endif
        
    LOG( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::SetConnQoSParamsL exiting" )
    }

// ---------------------------------------------------------------------------
// GetSubConQoSR5ParamsFromEvent
// ---------------------------------------------------------------------------
//
TInt CSatBIPGPRSDataChannel::GetSubConQoSR5ParamsFromEvent(
    CSubConNotificationEvent& aEvent, 
    TSatQoSParser::TQoSRel5& aNegQoSRel5 )
    {
    LOG( SIMPLE, 
        "SATENGINE: CSatBIPGPRSDataChannel::GetSubConQoSR5ParamsFromEvent \
        calling" )
    TInt errCode( KErrNone );
    
    CSubConGenEventParamsGranted* grantedEvent = 
        static_cast<CSubConGenEventParamsGranted*>( &aEvent );
    
    TInt extSetNum = grantedEvent->GetNumExtensionSets();
    LOG2( NORMAL, 
        "SATENGINE: GetSubConQoSR5ParamsFromEvent extSetNum :%d ", extSetNum )
                 
    const CSubConExtensionParameterSet* grantedExtParams = 
        grantedEvent->GetExtensionSet( extSetNum-1 );
    
    if ( grantedExtParams )
        {
        CSubConQosR5ParamSet* paramSet;
        paramSet = ( CSubConQosR5ParamSet* )grantedExtParams;
        
        if ( paramSet )
            {
            aNegQoSRel5.iTrafficClass = paramSet->GetTrafficClass();
            aNegQoSRel5.iTrafficHandlingPriority =
                paramSet->GetTrafficHandlingPriority();
            aNegQoSRel5.iSduErrorRatio = paramSet->GetSDUErrorRatio();
            aNegQoSRel5.iResidualBer = paramSet->GetResidualBitErrorRatio();
            aNegQoSRel5.iDeliveryErroneousSdu = 
                paramSet->GetErroneousSDUDelivery();
            aNegQoSRel5.iMaxBitRate = Max( paramSet->GetMaxBitrateUplink(),
                                        paramSet->GetMaxBitrateDownlink() );
            aNegQoSRel5.iMaximumSDUSize = paramSet->GetMaxSduSize();
            aNegQoSRel5.iDeliveryOrder = paramSet->GetDeliveryOrder();
                
            LOG2( SIMPLE, "Negotiated TrafficClass: %d", 
                aNegQoSRel5.iTrafficClass )
            LOG2( SIMPLE, "Negotiated TrafficHandlingPriority: %d", 
                aNegQoSRel5.iTrafficHandlingPriority )
            LOG2( SIMPLE, "Negotiated SDUErrorRatio: %d", 
                aNegQoSRel5.iSduErrorRatio )
            LOG2( SIMPLE, "Negotiated ResidualBitErrorRatio: %d", 
                aNegQoSRel5.iResidualBer )
            LOG2( SIMPLE, "Negotiated ErroneousSDUDelivery: %d", 
                aNegQoSRel5.iDeliveryErroneousSdu )
            LOG2( SIMPLE, "Negotiated Maxbitrate: %d", 
                aNegQoSRel5.iMaxBitRate )
            LOG2( SIMPLE, "Negotiated MaximumSDUSize: %d", 
                aNegQoSRel5.iMaximumSDUSize )
            LOG2( SIMPLE, "Negotiated DeliveryOrder: %d", 
                aNegQoSRel5.iDeliveryOrder )
            }
        else
            {
            LOG( NORMAL, "SATENGINE: GetSubConQoSR5ParamsFromEvent\
                Can't get CSubConQosR5ParamSet " )
            errCode = KErrNotSupported;
            }
        }
    else
        {
        LOG( NORMAL, "SATENGINE: GetSubConQoSR5ParamsFromEvent\
                Can't get granted CSubConExtensionParameterSet " )
        errCode = KErrNotSupported;
        }
     
    LOG2( SIMPLE,
        "SATENGINE: CSatBIPGPRSDataChannel::GetSubConQoSR5ParamsFromEvent \
        exiting, errCode: %d", errCode )
    return errCode;
    }
    
// ---------------------------------------------------------------------------
// GrantedEventReceivedL
// ---------------------------------------------------------------------------
//   
void CSatBIPGPRSDataChannel::GrantedEventReceivedL( 
    const TNotificationEventBuf& aEventBuffer )
    {
    LOG( SIMPLE, 
        "SATENGINE: CSatBIPGPRSDataChannel::GrantedEventReceivedL calling" )
    TInt errCode( KErrNone );
    RPacketQoS::TQoSGPRSNegotiated negQoS;
    
    // Exacting information from receieved events
	CSubConNotificationEvent* event = 
        CSubConNotificationEvent::NewL( aEventBuffer );
        
    if ( KSubConnGenericEventsImplUid == event->GroupId() )
        {
        LOG( NORMAL, "SATENGINE: GrantedEventReceivedL \
            event->GroupId() == KSubConnGenericEventsImplUid" )
        TSatQoSParser::TQoSRel5 negQoSRel5;
                
        switch ( event->Id() )
            {
            case KSubConGenericEventParamsGranted:
                {
                LOG( NORMAL, 
                    "SATENGINE: GrantedEventReceivedL Request granted" )
                      
                // Get the granted param set from event
                errCode = GetSubConQoSR5ParamsFromEvent( *event, negQoSRel5 );
                LOG2( NORMAL, 
                       "SATENGINE: GetSubConQoSR5ParamsFromEvent errCode %d ", 
                       errCode )
                User::LeaveIfError( errCode );
                break;
                }
            case KSubConGenericEventParamsRejected:
                {
                LOG( NORMAL, 
                        "SATENGINE: GrantedEventReceivedL Request Rejected" )
                        
                CSubConGenEventParamsRejected* rejectedEvent = 
                     static_cast<CSubConGenEventParamsRejected*>( event );
                errCode = rejectedEvent->Error();
                     
                LOG2( NORMAL, 
                        "SATENGINE: GrantedEventReceivedL\
	                        rejectedEvent->Error() = %d", errCode )
	            User::LeaveIfError( errCode );
	            break;
                }
            default:
                {
                LOG( NORMAL, 
                    "SATENGINE: GrantedEventReceivedL. EventId \
                    not in KSubConGenericEventParamsRejected/Granted " )
                // Do nothing
                break;
                }
            }                
        // convert negQoSRel5 to negQoS
        negQoS = TSatQoSParser::ConvertNegotiatedQoS( negQoSRel5 );
                             
        TSatQoSParser::GetNegotiatedQoSValues( negQoS, 
            iConnInfo.iBearerParams );
                
        // Set last bearer param to IP Protocol
        iConnInfo.iBearerParams[KProtocolPos] = KReqPacketProtType;
        }

    LOG( SIMPLE, 
        "SATENGINE: CSatBIPGPRSDataChannel::GrantedEventReceivedL exiting" )
    }
    
// ---------------------------------------------------------------------------
// Returns sub connection
// ---------------------------------------------------------------------------
//
RSubConnection& CSatBIPGPRSDataChannel::SubConnection() const
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::SubConnection\
        calling - exiting" )
    return iSubConnection;
    }

