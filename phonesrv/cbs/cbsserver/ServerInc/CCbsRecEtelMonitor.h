/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CCbsRecEtelMonitor class
*    
*                This class receives CBS message pages from the EPOC Telephony server and
*                sends them to a CCbsRecEtel instance.
*
*/



#ifndef     CCBSRECETELMONITOR_H
#define     CCBSRECETELMONITOR_H

//  INCLUDES
        /*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
#include <etelmm.h>

//  FORWARD DECLARATIONS
class TGsmBroadcastMessageData;
class CCbsRecEtel;
class MCbsEtelMessaging;
class CCbsMessageFactory;
class CCbsLivecastHandler;

//  CLASS DECLARATION 

/**
*   CCbsRecEtelMonitor is an active object handling the reception of 
*   CB message pages from the EPOC Telephony server.
*    
*   CCbsRecMessages are constructed from the received CBS message pages and
*   forwarded to an instance of CCbsRecEtel for decoding.
*   Constructing a whole message, in case of multipage messages, is also left 
*   to CCbsRecEtel and CCbsRecCollector.
*/
class CCbsRecEtelMonitor : public CActive
    {
    public:     // New functions

        /**
        *   Creates an instance of the class.
        *
        *   @param      aEtel               A reference to the receiver
        *   @param      aSmsMessaging       A reference to the messaging object
        *   @param      aFactory            A reference to the message factory
        *   @param      aLivecastHandler    A reference to the Livecast handler
        *   @return                         A new instance of CCbsRecEtelMonitor
        */
        static CCbsRecEtelMonitor* NewL( CCbsRecEtel& aEtel, 
            MCbsEtelMessaging& aSmsMessaging,  
            CCbsMessageFactory& aFactory,
            CCbsLivecastHandler& aLivecastHandler );

        /**
        *   Destructor.
        */
        ~CCbsRecEtelMonitor();

        /** 
        *   Requests EPOC Telephony server to forward next received CB message to 
        *   this object.
        */
        void IssueRequest();

    private:    // From CActive

        /**
        *   Becomes active when EPOC Telephony server has copied a CB message to 
        *   iCbsMessagePage. The page is then given to CCbsRecEtel for
        *   processing.
        *
        *   Finally this function reissues the ReceiveMessage request.
        */
        void RunL();

        /**
        *   Cancels an outstanding ReceiveMessage request.
        */
        void DoCancel();

    private:

        /**
        *   The constructor.
        *
        *   @param  aEtel               CCbsRecEtel object.
        *   @param  aMessaging          ETel messaging object
        *   @param  aFactory            A reference to the message factory
        *   @param  aLivecastHandler    A reference to the Livecast handler
        */
        CCbsRecEtelMonitor( CCbsRecEtel& aEtel, MCbsEtelMessaging& aMessaging,
            CCbsMessageFactory& aFactory, CCbsLivecastHandler& aLivecastHandler );

        /**
        *   Finalizes the construction.
        */
        void ConstructL();
      
    private:    // Data

        // CCbsRecEtel instance to which CB messages are passed.
        CCbsRecEtel& iEtel;
                
        // Messaging subsession.
        MCbsEtelMessaging& iSmsMessaging;       

        /**
        *   Message page is stored here when the monitor becomes active
        *   due the arrival of a CBS message.
        */        
        // Message data
        RMobileBroadcastMessaging::TBroadcastPageData iMsgData;
        
        // Attributedata of an incoming broadcast message
        RMobileBroadcastMessaging::TMobileBroadcastAttributesV2 iAttributes;

        // Attributepackage of an incoming broadcast message
        RMobileBroadcastMessaging::TMobileBroadcastAttributesV2Pckg iMsgAttributesPckg;
        
        // Message factory
        CCbsMessageFactory& iFactory;

        // LC message handler
        CCbsLivecastHandler& iLivecastHandler;

        // Is Newsticker feature supported
        TBool iNewstickerSupported;

    };

#endif      //  CCBSRECETELMONITOR_H   
            
// End of File


