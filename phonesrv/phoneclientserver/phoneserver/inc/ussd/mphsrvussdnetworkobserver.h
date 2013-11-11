/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Abstract interface for observing network message events.
*
*/

#ifndef MPHSRVUSSDNETWORKOBSERVER_H
#define MPHSRVUSSDNETWORKOBSERVER_H


// CLASS DECLARATION

/**
*  Abstract interface for observing network message events.
*
*  @since 1.0
*/
class MPhSrvUssdNetworkObserver
    {
    public: // New functions

        /**
        * Called when a message has been sent.
        * 
        * @param aError Complete code.
        */
        virtual void UssdNetworkObserverHandleSendEventL( TInt aError ) = 0;
        
        /**
        * Called when a message has been received.
        * 
        * @param aMessage Received message.
        * @param aIsRequest ETrue if is a request.
        * @param aError Error code of the operation.
        */
        virtual void UssdNetworkObserverHandleReceivedEventL(
            const TDes8& aMsgData, 
            const RMobileUssdMessaging::TMobileUssdAttributesV1& aMsgAttributes,
            TInt aError ) = 0;
            
       virtual void UssdNetworkObserverHandleNotifyNWReleaseL(
            const RMobilePhone::TMobilePhoneSendSSRequestV3& aReturnResult, 
            TInt aError ) = 0;
    };

#endif // MPHSRVUSSDNETWORKOBSERVER_H


// End of File
