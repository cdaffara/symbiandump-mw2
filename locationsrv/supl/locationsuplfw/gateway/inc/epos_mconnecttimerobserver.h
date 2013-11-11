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


#ifndef __MConnectTimerObserver_H__
#define __MConnectTimerObserver_H__


/**
*  MConnectTimerObserver is a an observer  
*  
*  *  @since Series 60 3.1M //e.g. Series 60 3.0
*/
class MConnectTimerObserver
    {
    public:  // Interface methods
    
    	/**
        * HandlePacket.
        * @since Series 60 3.1M
        * @param aPacket packet received from the network
        * @return void *
        */
        
        virtual void CompleteConnectTimerL()=0;
        
       
        
        
    };

#endif      // __MConnectTimerObserver_H__
            
// End of File
