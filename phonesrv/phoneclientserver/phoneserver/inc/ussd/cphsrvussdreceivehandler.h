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
* Description:  Get network message.
*
*/


#ifndef CPHSRVUSSDRECEIVEHANDLER_H
#define CPHSRVUSSDRECEIVEHANDLER_H


// INCLUDES
#include <e32base.h>
#include <etelmm.h>            // ETel


// FORWARD DECLARATIONS
class MPhSrvUssdNetworkObserver;


// CLASS DECLARATION

/**
*  Get network message.
*
*  @since 1.0
*/
class CPhSrvUssdReceiveHandler : public CActive
    {
    public:
        
        /**
        * C++ constructor.
        *
        * @param aObserver Observer for receive actions.
        * @param aUssdMessaging The actual sending class.
        */
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        CPhSrvUssdReceiveHandler( 
            MPhSrvUssdNetworkObserver& aObserver,
            RMobileUssdMessaging& aUssdMessaging );

        /**
        * Destructor.
        */
        ~CPhSrvUssdReceiveHandler();
        
        /**
        * Symbian OS second phase constructor.
        */
        void ConstructL();


    public:  // New functions
        
        /**
        * Request that this object attempts to receive a message.
        *
        * @return Error code
        */
        TInt StartReceiving();


    private:  // Functions from base classes
        
        /**
        * From CActive. 
        * Called when Ussd message is received.
        */
        void RunL();
        
        /**
        * From CActive. 
        * Called when receive is cancelled.
        */
        void DoCancel();
        
        /**
        * From CActive. 
        * Called when RunL leaves.
        */
        TInt RunError( TInt aError );


    private:    // Data
        
        // If this is true, receiving starts automatically.
        TBool iAutoReceive;
        
        // The observer for actions.
        MPhSrvUssdNetworkObserver& iObserver;

        // The actual sending class.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RMobileUssdMessaging& iUssdMessaging;

        // The USSD message data.
        RMobileUssdMessaging::TGsmUssdMessageData iMsgData;

        // USSD message attributes.
        RMobileUssdMessaging::TMobileUssdAttributesV1 iMsgAttributes;

        // USSD message attributes as package.
        RMobileUssdMessaging::TMobileUssdAttributesV1Pckg iMsgAttributesPckg;

    private:
        friend class CPhSrvUssdTester; // For test purposes only.
    };

#endif // CPHSRVUSSDRECEIVEHANDLER_H


// End of File
