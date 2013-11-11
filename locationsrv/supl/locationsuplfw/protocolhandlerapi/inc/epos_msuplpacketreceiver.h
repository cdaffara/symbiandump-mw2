/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Header file for Socket Read
*
*/


#ifndef M_MSUPLPACKETRECEIVER_H
#define M_MSUPLPACKETRECEIVER_H

/**
*  MSuplPacketNotifer is a an observer for receiving packets
*  from the network. Whenever there is a packet, SUPL Connection
*  notifies this observer
*  
*  @lib EPosSuplProtocolHandlerPlugin.dll
*  @since Series 60 3.1M //e.g. Series 60 3.0
*/
class MSuplPacketReceiver
    {
    public:  // Interface methods
    
    	/**
        * HandlePacket.
        * @since Series 60 3.1M
        * @param aPacket packet received from the network
        * @return void *
        */
        
        virtual void HandlePacket(const TDesC8 &aPacket,TUint aPortNo) = 0;
        
        /**
        * HandleError.
        * @since Series 60 3.1M
        * @param Error value indicating the Error
        * @return void *
        */
        
        virtual void HandlePacketError(TInt aError) = 0;
        
        
    };

#endif      // M_MSUPLPACKETRECEIVER_H
            
// End of File
