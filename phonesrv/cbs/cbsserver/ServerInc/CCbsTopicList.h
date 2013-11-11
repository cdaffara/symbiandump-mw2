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
* Description:  This file contains the header file of the CCbsTopicList class.
*    
*                The server-side topic list subsession class.
*
*/



#ifndef     CCBSTOPICLIST_H
#define     CCBSTOPICLIST_H

// INCLUDES
#include "CCbsObject.h"
#include "MCbsDbTopicListObserver.H"

// FORWARD DECLARATIONS
class CCbsSession;
class CCbsRecEtel;
class CCbsDbImpTopicList;

//  CLASS DECLARATION 

/**
*   Subsession for managing the topic list.
*/
class   CCbsTopicList
        : public CCbsObject, public MCbsDbTopicListObserver
    {
    public:     // New functions
        /**
        *   Creates a new instance of the class.
        *
        *   @param  aSession        Session
        *   @param  aTopicList      Topic list
        *   @param  aReceiver       Receiver.
        *   @return                 The new instance of CCbsSettings.
        */
        static CCbsTopicList* NewL( CCbsSession& aSession, 
                                    CCbsDbImpTopicList& aTopicList, 
                                    CCbsRecEtel& aReceiver );

        /**
        *   Destructor.
        */
        ~CCbsTopicList();

    public:     // From CCbsObject

        /**
        *   Handle the requests for the object. 
        *
        *   @param  aMessage        The message to be handled
        *   @return                 ETrue, if the request was handled here
        */
        TBool HandleRequestsL( const RMessage2& aMessage );

    private:    // From MCbsDbTopicListObserver

        /**
        *   Called when the topic list is initialized.
        */
        void TopicListInitializedIndL();

        /**
        *   Called whenever a topic is created. If necessary, 
        *   the client will be notified.
        *
        *   @param  aHandle         A handle to the topic.    
        */
        void TopicAddedIndL( const TCbsDbTopicNumber& aNumber );

        /**
        *   Called whenever a topic is deleted. If necessary, 
        *   the client will be notified.
        *
        *   @param  aHandle         A handle to the topic.
        */
        void TopicDeletedIndL( const TCbsDbTopicNumber& aNumber );

        /**
        *   Called whenever a topic is modified. If necessary, 
        *   the client will be notified.
        *
        *   @param  aHandle         A handle to the topic.
        */
        void TopicModifiedIndL( const TCbsDbTopicNumber& aNumber );

        /**
        *   Called whenever a new message has been received. 
        *   If necessary, the client will be notified.
        *
        *   @param  aHandle         A handle to the topic.
        */
        void TopicNewMessageReceivedIndL( const TCbsDbMessageHandle& aHandle );

    private:
        /**
        *   Constructor.
        *
        *   @param  aSession        Session
        *   @param  aTopicList      Topic list manager
        *   @param  aReceiver       Receiver
        */
        CCbsTopicList( CCbsSession& aSession, 
                       CCbsDbImpTopicList& aTopicList, 
                       CCbsRecEtel& aReceiver );

        /**
        *   Finalizes construction.
        */
        void ConstructL();
        
        /**
        *   Closes and destroys the subsession.
        *
        *   Note that after the call has completed, the object is no longer
        *   valid. It is also left for the callee's task to complete 
        *   the request.
        */
        void CloseTopicList();

        /**
        *   Returns the total number of topics to the client.
        */
        void GetTopicCountL();

        /**
        *   Returns the requested topic to the client.
        */
        void GetTopicL();

        /**
        *   Finds a topic by number and returns it to the client.
        */
        void FindTopicByNumberL();

        /**
        *   Deletes a topic by its number and returns it to the client.
        */
        void DeleteTopicL();

        /**
        *   Changes a topic name and number.
        */
        void ChangeTopicNameAndNumberL();
        
        /**
        *   Changes topic subscription status.
        */
        void ChangeTopicSubscriptionStatusL();
            
        /**
        *   Changes topic hotmark status.
        */
        void ChangeTopicHotmarkStatusL();

        /**
        *   Adds a topic to the list.
        */
        void AddTopicL();

        /**
        *   Sets up a pending notification request.
        */
        void NotifyOnEvent();

        /**
        *   Cancels a pending notification request.
        */
        void NotifyOnEventCancel();

        /**
        *   Returns the number of "new" topics to the client.
        *
        *   By new it is referred to topics that are created because of 
        *   topic detection since last power on.
        */
        void GetNewTopicsCountL();

        /**
        *   Retrieves the number of the topic that was added
        *   last to the topic list.
        */
        void GetLatestTopicNumberL();

        /**
        *   Returns the total amount of unread messages.
        */
        void GetUnreadMessageCountL();

        /**
        *   Returns a handle to the latest (that is not yet taken) 
        *   hotmarked message handle.
        */
        void GetHotmarkedMessageHandleL();

        /**
        *   Returns the number of hotmarked, unread messages.
        */
        void GetUnreadHotmarkedMessageCountL();

        /**
        *   Deletes all topics.
        */
        void DeleteAllL();

        /**
        *   If there is a pending notify request, notifies 
        *   the client.
        *
        *   @param  aEvent          The type of event
        *   @param  aNumber         The topic which was affected
        */
        void NotifyClientL( TCbsTopicListEvent aEvent, 
            TCbsTopicNumber aNumber );

        /**
        *   Retrieves and returns the numbers of topics that
        *   precede and succeed the given topic in topic list.
        *   
        *   Also returns two flags indicating whether the
        *   given topic is the first and/or the last topic in list.
        */
        void GetNextAndPrevTopicNumberL();

    private:    // Prohibited operators and functions

        // Copy constructor
        CCbsTopicList( const CCbsTopicList& );

        // Assignment operator
        CCbsTopicList& operator=( const CCbsTopicList& );

    private:    // Data

        // Referebce to the topic list db.
        CCbsDbImpTopicList& iTopicList;

        // Reference to the receiver.
        CCbsRecEtel& iReceiver;

        // Indicates whether there is an active notify request.
        TBool iIsMessage;

        // The message for pending notify request.
        RMessage2 iMessage;
        
    };

#endif      //  CCBSTOPICLIST_H   
            
// End of File


