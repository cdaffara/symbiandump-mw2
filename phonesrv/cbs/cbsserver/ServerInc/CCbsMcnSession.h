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
* Description:  This file contains the header file of the CCbsMcnSession class which 
*                responds to client MCN-related requests.
*
*/




#ifndef     CCBSMCNSESSION_H
#define     CCBSMCNSESSION_H

//  INCLUDES

#include <e32base.h>
#include "CCbsObject.h"
#include "MCbsMcnSubscriptionsProvider.h"
#include <cbsmcncommon.h>

//  FORWARD DECLARATIONS

class CCbsDbImpSettings;
class CCbsRecEtel;
class CCbsReceiverHelper;
class CCbsRecMessage;
class CCbsSession;

//  CLASS DECLARATION 

/**
*   CCbsMcnSession is the server-side subsession object, which takes care
*   of client-side MCN requests.
*/
class CCbsMcnSession 
        : public CCbsObject, public MCbsMcnSubscriptionsProvider
    {
    public:     // New functions
        /**
        *   Creates a new instance of the class.
        *
        *   @param  aSession            Main session instance.
        *   @param  aReceiver           Receiver instance.
        *   @return                     New instance of CCbsMcnSession.
        */
        static CCbsMcnSession* NewL( CCbsSession& aSession, 
            CCbsRecEtel& aReceiver );

        /**
        *   Destructor.
        */
        ~CCbsMcnSession();

    public:     // From CCbsObject
        /**
        *   Handle the requests for the object.
        *
        *   @param  aMessage            Message to be handled.
        *   @return                     Indicates whether the message was 
        *                               handled.
        */
        TBool HandleRequestsL( const RMessage2& aMessage );

    public:     // From MCbsMcnSubscriptionsProvider
        /**
        *   Returns the number of topic subscriptions of this MCN client.
        *
        *   @return                     Number of subscribed CB topics
        */
        TUint NumberOfSubscriptions() const;

        /**
        *   If aMessage is of topic subscribed by the client,
        *   aMessage is copied to client-side. 
        *
        *   @param  aMessage            CB message.
        */
        void RouteMessageL( const CCbsMessage& aMessage );

    private:
        /**
        *   Constructor.
        *
        *   @param  aSession            Main session object.
        *   @param  aReceiver           Receiver instance.
        */
        CCbsMcnSession( CCbsSession& aSession, CCbsRecEtel& aReceiver );

        /**
        *   Finalizes construction.
        */
        void ConstructL();

        /**
        *   Close the subsession.
        */
        void CloseMcnSession();

        /**
        *   Handles client request for message routing.
        */
        void NotifyOnChange();

        /**
        *   Handles client request to cancel an outstanding 
        *   routing request.
        */
        void NotifyOnChangeCancel();

        /**
        *   Handles client request to subscribe a topic.
        *
        *   Note: ApplySubscriptionL has to be called in order to
        *   apply receiver changes.
        */
        void SubscribeTopicL();

        /**
        *   Called by RouteMessageL() to actually copy the message content
        *   to client side.
        *
        *   @param aMcnPckg         Package containing the MCN message
        *                           and it's parameters.
        */        
        void NotifyClientL( const TPckg<TCbsMcnMessage>& aMcnPckg );                

        /**
        *   Clears all topic subscriptions of this MCN session.
        *
        *   Leaves only if CCbsRecEtel::ApplyStateChangesL() leaves.
        */
        void ClearSubscriptionsL();

        /**
        *   Forces receiver to reload CB topic subscriptions.
        *
        *   This function has to be called after MCN client has subscribed
        *   topics with SubscribeTopicL().
        */
        void ApplySubscriptionsL();
  
    private:    // Prohibited operators and functions

        // Copy constructor
        CCbsMcnSession( const CCbsMcnSession& );

        // Assignment operator
        CCbsMcnSession& operator=( const CCbsMcnSession& );


    private:    // Data
        
        /// Receiver object.
        CCbsRecEtel& iReceiver;

        /**
        *   Indicates whether iMessage is a valid message; that is, there
        *   is a pending notify request.
        */
        TBool iIsMessage;

        /// Message for the pending notify request 
        RMessage2 iMessage;

        /// Array of topic numbers subscribed by the client.
        CArrayFixFlat< TUint16 >* iSubscribedTopics;

        // MCN message
        TCbsMcnMessage iMcnMessage;

        // MCN message package for IPC
        TPckg<TCbsMcnMessage> iMcnPckg;

    };

#endif      //  CCBSMCNSESSION_H   
            
// End of File


