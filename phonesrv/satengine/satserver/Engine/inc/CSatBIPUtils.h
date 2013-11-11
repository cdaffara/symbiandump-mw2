/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSatBIPUtils is responsible for handling Bearer Independent
                 Protocol data channels.
*
*/


#ifndef CSATBIPUTILS_H
#define CSATBIPUTILS_H

#include    <e32base.h>
#include    <es_sock.h>
#include    <etelpckt.h>
#include    <etelmm.h>
#include    <etelqos.h>
#include    "MSatBIPUtils.h"
#include    "MSatBIPEventNotifier.h"
#include    "MSatEventObserver.h"
#include    "MSatBIPDataChannel.h"

class MSatBIPChannelStatusObserver;
class MSatBIPDataAvailableObserver;
class TSatChannelIdInfo;
class MSatUtils;
class CSatApnHandler;
class CSatSActiveWrapper;
class CSatBIPConnectionObserver;

/**
 *  CSatBIPUtils is responsible for handling data channels used by
 *
 *  Bearer Independent Protocol (BIP)
 *
 *  @lib SatEngine.lib
 *  @since S60 3.0
 */
class CSatBIPUtils : public CBase,
                     public MSatBIPUtils,
                     public MSatBIPEventNotifier,
                     public MSatEventObserver
    {
public:  
// Constructors and destructor
    /**
     * Two-phased constructor.
     * @param aUtils Reference to common Utils class
     */
    static CSatBIPUtils* NewL( MSatUtils& aUtils );
    
    /**
     * Destructor.
     */
    virtual ~CSatBIPUtils();
    
// from base class MSatEventObserver
    /**
     * From MSatEventObserver
     * Handles event
     *
     * @since S60 3.0
     * @param aEvent Event that has occured
     */       
    void Event( TInt aEvent );
    
// from base class MSatBIPUtils
    /**
     * From MSatBIPUtils 
     * Creates new data channel and returns reference to it
     *
     * @since S60 3.0
     * @param TSatBIPError code indicating the execution status.
     * @return Pointer to created data channel
     */
    MSatBIPDataChannel* CreateChannelL(
        const TSatBearerType aBearerType,
        TInt& aErrCode );
    
    /**
     * From MSatBIPUtils 
     * Request to close specified data channel.
     *
     * @since S60 3.0
     * @param aChannelId Identifies the channel to be closed.
     * @return TSatBIPError code indicating the execution status.
     */
    TInt CloseChannel( const TInt aChannelId );

    /**
     * From MSatBIPUtils 
     * Request to open new data channel
     *
     * @since S60 3.0
     * @param aChannelId Identifies the channel.
     * @param aErrCode TSatBIPError code indicating the execution status.
     * @return Pointer to requested data channel
     */
    MSatBIPDataChannel* DataChannel(
        const TInt aChannelId,
        TInt& aErrCode );
    
    /**
     * From MSatBIPUtils 
     * Generates channel status object from given 
     * parameters to be used in terminal responses.
     *
     * @since S60 3.0
     * @param aChannelId Data channel identifier.
     * @param aStatus Data channel's status.
     * @return channel status object that can be put into terminal responses.
     */
    RSat::TChannelStatus GenerateChannelStatusL(
        const TInt aChannelId,
        TInt aStatus );

    /**
     * From MSatBIPUtils 
     * Reference to APN handler
     *
     * @since S60 3.0
     * @return Interface reference to CSatApnHandler
     */
    MSatApnHandler& ApnHandler();
    
    /**
     * From MSatBIPUtils 
     * Status of the PDP Context
     *
     * @since S60 3.0
     * @return ETrue, if there is active PDP Context
     */
    TBool IsContextActive();
    
    /**
     * From MSatBIPUtils 
     * Request to close specified data channel.
     *
     * @since S60 3.0
     * @return Array containing all available data channels.
     */
    CArrayPtrFlat<MSatBIPDataChannel>* DataChannels();
    
// from base class MSatBIPUtils
                
    /**
     * From MSatBIPEventNotifier
     * Notification request for DataAvailable event.
     *
     * @since S60 3.0
     * @param aObserver An observer of the event.
     */
    void NotifyDataAvailable(
        MSatBIPDataAvailableObserver* aObserver );

    /**
     * From MSatBIPEventNotifier
     * Notification request for ChannelStatus event.
     *
     * @since S60 3.0
     * @param aObserver An observer of the event.
     */
    void NotifyChannelStatus(
        MSatBIPChannelStatusObserver* aObserver );
    
    /**
     * From MSatBIPEventNotifier 
     * Cancels DataAvailable event notification request.
     *
     * @since S60 3.0
     */
    void CancelDataAvailable();
    
    /**
     * From MSatBIPEventNotifier 
     * Cancels ChannelStatus event notification request.
     *
     * @since S60 3.0
     */
    void CancelChannelStatus();

// New Member Function
    /**
     * DataAvailable event from data channel.
     *
     * @since S60 S60 3.0
     * @param aChannelId ID of the channel that called this function.
     * @param aLength Number of bytes available.
     */
    void DataAvailable( const TInt aChannelId, const TInt aLength );
    
    /**
     * ChannelStatus event from data channel.
     *
     * @since S60 S60 3.0
     * @param aChannelId ID of the channel that called this function.
     * @param aStatus Status that caused this event.
     */
    void ChannelStatus(
        const TInt aChannelId,
        const TInt aStatus );

    
    /**
     * Closes all active data channels. This function is called
     * when SAP is enabled or SAT Server closes
     * @since S60 S60 3.0
     */
    void CloseAllChannels();

    /**
     * Proposes QoS parameters for the last created primary or secondary context
     * and returns negotioated QoS parameters as a result.
     *
     * @since S60 S60 3.0
     * @param aQoSParams Requested QoS parameters to propose for the context
     * @return Negotioated QoS parameters, which may differ from requested
     *         parameters.
     */
    RPacketQoS::TQoSGPRSNegotiated ProposeQoSParametersL(
        const RPacketQoS::TQoSGPRSRequested& aQoSParams );
       
    /**
     * Gives reference to RPacketService
     *
     * @since S60 S60 3.0
     */
    RPacketService& PacketService();
    
    
    /**
     * Current status of the GPRS service
     *
     * @since S60 S60 3.0
     * @return Current status of the GPRS service
     */     
    RPacketService::TStatus ConnectionStatus() const;
        
    /**
     * Stop all udp connection.
     *
     * @since S60 S60 3.2
     */  
    void StopUdpLink();
        
    /**
     * Set the state for subconnection
     *
     * @since S60 S60 3.2
     * @param aSet State to be set
     */  
    void SetConnectionActivated( TBool aSet );
        
    /**
     * Get the state of connection
     *
     * @since S60 S60 3.2
     * @param aSet State to be set
     */  
    TBool IsConnectionActivated();
        
    /**
     * Set the state for subconnection
     *
     * @since S60 S60 3.2
     * @param aSet State to be set
     */  
    void SetSubConnectionActivated( TBool aSet );
        
    /**
     * Get the state of subconnection
     *
     * @since S60 S60 3.2
     * @param aSet State to be set
     */  
    TBool IsSubConnectionActivated();  
        
private:

    /**
     * C++ default constructor.
     * @param aUtils Reference to common utils class
     */
    CSatBIPUtils( MSatUtils& aUtils );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * Defines next available channel ID
     * @return Pointer to next available channel id
     */
    TSatChannelIdInfo* AvailableChannelID();
    
    /**
     * Reference to RPacketContex handle
     * @return Error code indicating the success of function completion
     */
    TInt GprsService();
    
    /**
     * Number of contexts currently in Gprs service
     *
     * @since S60 S60 3.0
     * @return Number of active PDP contexts
     */  
    TInt ContextCount();

private: // Data

    /**
     * Array containing all data channels
     * Own
     */
    CArrayPtrFlat<MSatBIPDataChannel>* iDataChannels;

    /**
     * DataAvailable event observer
     * Own
     */
    MSatBIPDataAvailableObserver* iDataAvailableObserver;

    /**
     * ChannelStatus event observer
     * Own
     */
    MSatBIPChannelStatusObserver* iChannelStatusObserver;

    /**
     * Main session to Socket Server
     * 
     */
    RSocketServ iSocketServer;

    /**
     * Indicates is there channel active or not
     * 
     */
    TBool iDataChannelActivated;

    /**
     * Array containing datachannel IDs
     * Own
     */
    CArrayPtrFlat<TSatChannelIdInfo>* iChannelIds;

    /**
     * Temporary Channel ID pointer, used for ~CSatBIPUtils to 
     * cleanup when AppendL leave. Look into ConstructL and
     * ~CSatBIPUtils to see how it be used.
     * Not Own
     */
    TSatChannelIdInfo* iChannelId;
   
   /**
     * Utils class
     * Not Own
     */
    MSatUtils& iUtils;

    /**
     * Blocker for synchronous requests
     * Own
     */
    CSatSActiveWrapper* iWrapper;

    /**
     * Classes for GPRS context activation
     * 
     */
    RPacketService iPacketService;

    /**
     * Indicates is the Session to PacketService active
     * 
     */
    TBool iPacketServiceActive;

    /**
     * Pointer to APN handler
     * Own
     */
    MSatApnHandler* iApnHandler;

    /**
     * Pointer to connection status observer
     * Own
     */
    CSatBIPConnectionObserver* iConnObserver;
        
    /**
     * Represent the primary PDP context
     * Own
     */
    RConnection iConnection;
        
    /**
     * Represent the secondary PDP context
     * Own
     */
    RSubConnection iSubConnection;
        
    /**
     * iSubConnectionActivated
     */
    TBool iSubConnectionActivated;
        
    /**
     * iConnectionActivated
     */
    TBool iConnectionActivated;
    };

#endif      // CSATBIPUTILS_H

