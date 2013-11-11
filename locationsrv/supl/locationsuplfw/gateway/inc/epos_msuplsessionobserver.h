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


#ifndef __MSuplSessionObserver_H__
#define __MSuplSessionObserver_H__


/**
*  MSuplSessionObserver is a an observer  
*  
*  *  @since Series 60 3.1M //e.g. Series 60 3.0
*/
class MSuplSessionObserver
    {
    public:  // Interface methods
    
    	/**
        * HandlePacket.
        * @since Series 60 3.1M
        * @param aPacket packet received from the network
        * @return void *
        */        
        virtual void CompleteRunSession(TInt err) = 0;
        
    	/**
        * HandlePacket.
        * @since Series 60 9.2TB
        * @param aPacket packet received from the network
        * @return void *
        */        
        virtual void CompleteTriggerRunSession(TInt err) = 0;
        
        /**
        * Complete Request : Completes Get Position Request
        * @since Series 60 3.1M
        * @param 
        * @return void *
        */        
        virtual void CompleteGetPositionRequest(const RMessage2& aMessage, TInt err) = 0;
        
        /**
        * Complete Request : Completes Supl Trigger Fired Notification Request
        * @since Series 60 9.2TB
        * @param 
        * @return void *
        */        
        virtual void CompleteTriggerFiredNotifyRequest(TInt err) = 0;
        
        /** Packs the position Info 
        */        
        virtual TInt PackPositionData(const RMessage2& aMessage) = 0;        
        
    };

#endif      // __MSuplSessionObserver_H__   
            
// End of File
