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
* Description:  GPRS Data channel
*
*/



#ifndef CSATBIPGPRSDATACHANNEL_H
#define CSATBIPGPRSDATACHANNEL_H

#include  <e32base.h>
#include  <es_enum.h>           // TConnectionInfo
#include  <in_sock.h>
#include  <etelqos.h>

#include  "MSatBIPDataChannel.h"
#include  "MSatBIPUtils.h"
#include  "TSatQoSParser.h"

class CSatBIPUtils;
class MSatSendDataObserver;
class CSatBIPDataSender;
class CSatBIPDataReceiver;
class MSatConnectionObserver;
class TSatChannelIdInfo;
class TNotificationEventBuf;
class CSubConQosR5ParamSet;
class CSubConExtensionParameterSet;
class CSatSActiveWrapper;
class CSatBIPSubConnEventObserver;

/**  Maximum Address Length */
const TInt KMaxAddressLength = 50;

/**  Enum for connection stage */
enum TSatBIPConnectionStage
    {
    ESatBIPClosed,
    ESatBIPConnect,
    ESatBIPConnected,
    ESatBIPCancelled
    };

/**
 *  GPRS Data channel for Bearer Independent Protocol.
 *  
 *  @lib SatServer
 *  @since Series 60 3.0
 */
class CSatBIPGPRSDataChannel : public CActive,
                               public MSatBIPDataChannel
    {
public:  

// Constructors and destructor

    /**
     * Two-phased constructor.
     * @param aUtils Interface for EventDownload commands
     * @param aSocketServer Reference to Socket server.
     * @param aChannelId ID for this data channel
     * @param aConnection Reference to Connection
     * @param aSubConnection ID Reference to SubConnection
     */
    static CSatBIPGPRSDataChannel* NewL( CSatBIPUtils& aUtils,
        RSocketServ& aSocketServer,
        TSatChannelIdInfo& aChannelId,
        RConnection& aConnection,
        RSubConnection& aSubConnection
         );

    /**
     * Two-phased constructor.
     * @param aUtils Interface for EventDownload commands
     * @param aSocketServer Reference to Socket server.
     * @param aChannelId ID for this data channel needs to add to comments
     * @param aConnection Reference to Connection
     * @param aSubConnection ID Reference to SubConnection
     */
    static CSatBIPGPRSDataChannel* NewLC( CSatBIPUtils& aUtils,
        RSocketServ& aSocketServer,
        TSatChannelIdInfo& aChannelId,
        RConnection& aConnection,
        RSubConnection& aSubConnection );
    
    /**
     * Destructor.
     */
    virtual ~CSatBIPGPRSDataChannel();

// New functions

    /**
     * Notification that data has been received.
     * @param aData Holds received data.
     */
    void DataReceivedNotificationL( const TDesC8& aData );

    /**
     * Error notification while receiving
     * @param aError The error code
     */
    void DataReceiveError( const TInt aError );
        
    /**
     * Granted parameters event is received correctly.
     * @param aEventBuffer Holds the negotiated QoS parameters.
     */
    void GrantedEventReceivedL( const TNotificationEventBuf& aEventBuffer );
        
    /**
     * Return the subconnection.
     * @return Reference to SubConnection
     */
    RSubConnection& SubConnection() const;
        
// From base class MSatBIPDataChannel

    /**
     * From MSatBIPDataChannel Sets up connection. Does not activate it yet.
     * @param aConnParams includes all needed information to open the connection.
     */
    void SetupConnectionL( const TSatBipConnectionInfo& aConnParams );

    /**
     * Notification that data has been received.
     * @param aData Holds received data.
     */
    void ActivateConnectionL( MSatConnectionObserver* aObserver );

    /**
     * From MSatBIPDataChannel Returns Negotiated bearer parameters
     * @param aResult Output Negotiated bearer parameters
     */
    void GetNegotiatedQoSParams( RSat::TBearerParams& aResult ) const;

    /**
     * From MSatBIPDataChannel Sends the data or stores it into a
     * buffer for later send.
     * @param aData Data to be send.
     * @param aSendImmedeately Indicates whether to send immediately or
     *        store data to buffer for later send.
     * @param aFreeBufferSize The number of bytes of empty size in the buffer
     * @param aObserver Observer of the SendData procedure. When data is sent,
     *        observer gets notification. If data is stored for later send,
     *        observer is not notified.
     * @return Error code indicating the status of the command execution.
     */
    TInt SendDataL( const TDes8& aData, 
        const TBool aSendImmediately,
        TInt& aFreeBufferSize,
        MSatSendDataObserver* aObserver );

    /**
     * From MSatBIPDataChannel Receives the data from the buffer.
     * @param aData Data to receive.
     * @param aBytesToRead The number of requested bytes to return
     * @param aAvailableBytes Number of unread bytes left in the buffer.
     * @return Error code indicating the status of the command execution.
     */
    TInt ReceiveDataL( TDes8& aData, const TInt aBytesToRead,
        TInt& aAvailableBytes );

    /**
     * From MSatBIPDataChannel Returns the ID of this data channel.
     * @return ID of this channel.
     */
    TInt ChannelId() const;
    
    /**
     * From MSatBIPDataChannel Returns the ID of this data channel to
     * ChannelStatus.
     * @return ID that can be placed to ChannelStatus
     */
    TUint8 ChannelStatusChannelId() const;

    /**
     * From MSatBIPDataChannel Returns the status of this channel.
     * @return Status of this data channel
     */
    TInt Status() const;

    /**
     * From MSatBIPDataChannel Closes the connection and empties the buffers.
     */
    void CloseChannel();
    
    /**
     * From MSatBIPDataChannel Cancels all activities.
     */
    void CancelAll();

    /**
     * From MSatBIPDataChannel Indicates whether PDP contex is active or not.
     * @return TBool indicating context activation.
     */
    TBool IsContextActive() const;

    /**
     * From MSatBIPDataChannel Returns connection information
     * @return Connection information
     */
    const MSatBIPDataChannel::TSatBipConnectionInfo& ConnInfo() const;

    /**
     * From MSatBIPDataChannel Close socket if type is udp.
     */
     void StopUdpSocket();
   

 // From base class CActive
 
    /**
     * From CActive
     */
    void RunL();

    /**
     * From CActive
     */
    void DoCancel();

    /**
     * From CActive
     */
    TInt RunError( TInt aError );
        


private:

    /**
     * C++ default constructor.
     * @param aUtils Interface for EventDownload commands
     * @param aSocketServer Reference to Socket server.
     * @param aChannelId ID for this data channel
     * @param aConnection  Reference to RConnection
     * @param aSubConnection Reference to RSubConnection
     */
    CSatBIPGPRSDataChannel( CSatBIPUtils& aUtils,
        RSocketServ& aSocketServer,
        TSatChannelIdInfo& aChannelId,
        RConnection& aConnection,
        RSubConnection& aSubConnection );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * Opens and connects / binds socket
     * @param aAddrFamily Address family to use.
     */
    void OpenSocketL();

    /**
     * Opens sub-connection and attaches socket into it
     */
    void OpenSubConnection();

    /**
     * Sets QoS parameters to connection
     */
    void SetSubConQoSParamsL();
    
    /**
     * Starts to receive data from socket
     */
    void StartReceiveL();
    
    /**
     * Defines the IP address.
     * @param aAddr An 8-bit descriptor containing the IP address
     * @return IP address in TBuf descriptor, which may be 8- or 16-bit.
     */
    TInetAddr DestAddress( const TPtrC8& aAddr );
        
    /**
     * Write QoS Values used for requesting Primary PDP context to RCmManager
     */
    void WritePrimaryPDPContextQoSParamsL();
    
    /**
     * Write Default QoS values to RCmManager
     */
    void WriteDefaultQoSParamsL(); 
    
    /**
     * Sets up QoS parameters for connection or subconnection
     */
    void SetConnQoSParamsL();
    
    /**
     * Get the negotiated QoS parameters from granted event
     * @param aEvent SubConNotificationEvent
     * @param aNegQoSRel5 hold the negotiated QoS in Rel5 got from event
     * @return errCode if it fails to get the parameters from event.
     */
    TInt GetSubConQoSR5ParamsFromEvent(
        CSubConNotificationEvent& aEvent, 
        TSatQoSParser::TQoSRel5& aNegQoSRel5 );
        
private:    // Data
        
    /**
     * Receive Buffer
     */
    TBuf8<KSatBIPMinReceiveBufferSize> iRcvBuffer;
        
    /**
     * Status of this channel
     */
    TInt iChannelStatus;
        
    /**
     * Socket to send / receive data
     */
    RSocket iSocket;
       
    /**
     * Connection information
     */
    MSatBIPDataChannel::TSatBipConnectionInfo iConnInfo;

    /**
     * Destination address
     */
    TInetAddr iDestAddr;

    /**
     * Sourceaddress
     */
    TInetAddr iSourceAddr;

    /**
     * Reference to BIP Utils.
     * Not own.
     */
    CSatBIPUtils& iUtils;
        
    /**
     * Connection stage
     */
    TSatBIPConnectionStage iConnStage;
    
    /**
     * Indicates the protocol
     */
    TBool iUseTcpProtocol;
        
    /**
     * Reference to a socket server
     * Not own.
     */
    RSocketServ& iSocketServ;
        
    /**
     * Identifier for this channel
     * Not own.
     */
    TSatChannelIdInfo& iChannelId;
        
    /**
     * Connection information
     *  Not own
     */
    RConnection& iConnection;
    
    /**
     * Subconnection, represents a data channel
     *  Not own
     */
    RSubConnection& iSubConnection;
        
    /**
     * SendBuffer
     * Own.
     */
    HBufC8* iSendStore;
        
    /**
     * SendData observer
     * Own.
     */
    MSatSendDataObserver* iSendDataObserver;

    /**
     * Sender object
     * Own.
     */
    CSatBIPDataSender* iSender;

    /**
     * Receiver object
     * Own.
     */
    CSatBIPDataReceiver* iReceiver;

    /**
     * Connection observer
     * Own.
     */
    MSatConnectionObserver* iConnectionObserver;
        
    /**
     * Observer for SubConnection parameters event;
     * Own.
     */
    CSatBIPSubConnEventObserver* iSubConnEventObserver;
    
    };

#endif      // CSATBIPGPRSDATACHANNEL_H


