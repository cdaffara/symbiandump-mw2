/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header file for the CCbsMessageFactory class.
*
*/


#ifndef     CCBSMESSAGEFACTORY_H
#define     CCBSMESSAGEFACTORY_H

//  INCLUDES

#include <e32base.h>
#include <etelmm.h>
#include "CCbsMessage.h"

//  FORWARD DECLARATIONS
class CCbsRecEtel;

//  CLASS DECLARATION 

/**
*   CCbsMessageFactory constructs CB messages based on a page
*   received through ETel.
*
*   The selection between GSM and WCDMA messages is made when
*   constructing the message.
*/
class CCbsMessageFactory : public CBase
    {
    public:     // New functions        

        /**
        *   Constructs a new CCbsMessage based on TDes8&
        *   (which is a descriptor) received from EPOC Telephony Server.
        *   
        *   @param      aRecEtel        CCbsRecEtel reference
        *   @return                     A new instance of CCbsMessageFactory
        */
        static CCbsMessageFactory* NewL( CCbsRecEtel& aRecEtel );

        /**
        *   Destructor
        */
        ~CCbsMessageFactory();

        /**
        *   Creates a new message.
        *
        *   @param  aMessagePage        Source message descriptor
        *   @param  aAttributes         Message attributes
        *   @return aMsgType            Message type (GSM/WCDMA/Livecast)
        *   @return                     Current instance of CCbsMessage
        */
        CCbsMessage* CreateMessageL( const TDes8& aMessagePage,
            const RMobileBroadcastMessaging::TMobileBroadcastAttributesV2& aAttributes,
            TCbsMessageType& aMsgType );

        /**
        *   Creates a new message.
        *        
        *   @param  aMessage            Message from which to create the new msg
        *   @return                     Current instance of CCbsMessage
        */
        CCbsMessage* CreateMessageL( const CCbsMessage& aMessage );
        
    private:

        /**
        *   Default constructor               
        */  
        CCbsMessageFactory( CCbsRecEtel& aRecEtel );

        /**
        *   2nd-phase constructor               
        */
        void ConstructL();


    private: // Data

        // Message attributes, mainly used with WCDMA
        RMobileBroadcastMessaging::TMobileBroadcastAttributesV2 iAttributes;

        // CCbsRecEtel instance to which CB messages are passed.
        CCbsRecEtel& iRecEtel;

    };

#endif      //  CCBSMESSAGEFACTORY_H   
            
// End of File


