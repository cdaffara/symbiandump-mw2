/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sends USSD message to the network.
*
*/

#ifndef CPHSRVUSSDSENDHANDLER_H
#define CPHSRVUSSDSENDHANDLER_H

// INCLUDES
#include    <etelmm.h>            

// FORWARD DECLARATIONS
class MPhSrvUssdNetworkObserver;
class MPhSrvPhoneInterface;
class CPhSrvUssdSessionCancelWaiter;

// CLASS DECLARATION

/**
*  Sends message to the network.
*
*  @since Series60_1.0
*/
class CPhSrvUssdSendHandler : public CActive
    {
    
    public:  // Constructors and destructor     

        /**
        * C++ constructor.
        *
        * @param aObserver Observer for send events.
        * @param aUssdMessaging The actual sender instance.
        * @param aPhoneInterface Phone interfaces.
        */
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        CPhSrvUssdSendHandler( 
            MPhSrvUssdNetworkObserver& aObserver,
            RMobileUssdMessaging& aUssdMessaging,
            MPhSrvPhoneInterface& aPhoneInterface );

        /**
        * Destructor
        */
        ~CPhSrvUssdSendHandler();


    public: // New functions
        
        /**
        * Send USSD message.
        *
        * @param aMsgData USSD message to be sent.
        * @param aMsgAttribute USSD message attributes.
        */
        void SendUssdL( 
            const TDesC8& aMsgData, 
            const TDesC8& aMsgAttribute );

        /**
        * Cancel send USSD message.
        */
        void SendUssdCancel();
        
        /**
        * Terminates USSD Session.
        */
        void SendReleaseSession();

    private: // Functions from base classes
        
        /**
        * From CActive. 
        * Called when sending is cancelled.
        */
        void DoCancel();
        
        /**
        * From CActive. 
        * Called when send request is completed.
        */
        void RunL();

    private:    // Data
        
        // Observer for send events.
        MPhSrvUssdNetworkObserver& iObserver;

        // The actual sending instance.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RMobileUssdMessaging& iUssdMessaging;

        // References to phone interfaces.
        MPhSrvPhoneInterface& iPhoneInterface;

        // Message data storage.
        HBufC8* iMsgData;

        // Message data attributes.
        HBufC8* iMsgAttribute;

        // For cancelling the USSD session.
        CPhSrvUssdSessionCancelWaiter* iSessionCancelWaiter;
    };

#endif // CPHSRVUSSDSENDHANDLER_H


// End of File
