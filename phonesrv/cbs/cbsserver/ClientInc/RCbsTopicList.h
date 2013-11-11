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
* Description:  This file contains the header file of the RCbsTopicList class
*    
*                RCbsTopicList provides an interface to CBS clients which
*                allows accessing the Cell Broadcast topics stored
*                in the CBS Server. Note: This class should not be directly 
*                accessed by the client application.
*
*                Services include accessing and modifying topic information,
*                deleting a single or all topics, and a notification service,
*                that enables the client to receive notifications on 
*                events modifying the server's topic list.
*
*
*/



#ifndef     RCBSTOPICLIST_H
#define     RCBSTOPICLIST_H

// INCLUDES
#include <e32base.h>
#include "CbsCommon.h"

// FORWARD DECLARATIONS
class RCbs;

// CLASS DECLARATION 

/**
*   This class represents the client-side subsession to handle topic lists.
*/

class RCbsTopicList 
        : public RSubSessionBase
    {
    public:     // New functions
        /**
        *   Constructor.
        */
        RCbsTopicList();

        /**
        *   Creates a subsession to the server. Before using any other method it
        *   must be called.

        *   @param aServer      It is the cbs server.
        *   @return             Error code.
        */
        TInt Open( RCbs& aServer );

        /**
        *   Closes the subsession and connection to the server.
        *
        *   @return             Error code.
        */
        TInt Close();

        /**
        *   Returns the total amount of topics the topic list contains.
        *
        *   @param aCount       It will contain the total amount of topics.
        */
        void GetTopicCount( TInt& aCount );

        /**
        *   Returns information about a topic from the topic list.
        *
        *   @param aIndex       It is the index to the topic.
        *   @param aTopic       It will contain the topic information.
        *   @return             Error code.
        */
        TInt GetTopic( const TInt aIndex, TCbsTopic& aTopic );

        /**
        *   Finds the topic by given number.
        *
        *   @param aNumber      It is the number of the topic.
        *   @param aTopic       It will contain the topic information.
        *   @return             Error code.
        */
        TInt FindTopicByNumber( TCbsTopicNumber aNumber, TCbsTopic& aTopic );

        /** 
        *   Deletes an existing topic.
        *
        *   Note that the topic must exists.
        *
        *   @param aNumber      It is the handle to the topic to be deleted.
        *   @return             Error code.
        */
        TInt DeleteTopic( TCbsTopicNumber aNumber );

        /**
        *   Delete all topics.
        *
        *   @return             Error code.
        */
        TInt DeleteAllTopics();

        /**
        *   Adds a new topic.
        *
        *   Note that the number of the new topic must be unused.
        *
        *   @param aTopic       It contains the information of the new topic.
        *   @return             Error code.
        */
        TInt AddTopic( TCbsTopic& aTopic );

        /**
        *   Changes the name and number of the existing topic.
        *
        *   Note that the changing fails in case there is another topic with the new topic number.
        *   It also fails if the topic is protected.
        *
        *   @param  aOldNumber     Old topic number
        *   @param  aNewNumber     New topic number
        *   @param  aName       It is the new name of the topic.
        *   @return             Error code.
        */  
        TInt ChangeTopicNameAndNumber( 
             TCbsTopicNumber aOldNumber,
             TCbsTopicNumber aNewNumber, 
             const TCbsTopicName& aName );

        /**
        *   Changes topic subscription status.
        *
        *   @param  aNumber     Number of the topic
        *   @param  aNewStatus  It is the new subscription status.
        *   @return             Error code.
        */
        TInt ChangeTopicSubscriptionStatus( TCbsTopicNumber aNumber, 
             TBool aNewStatus );

        /**
        *   Changes topic hotmark status.
        *
        *   @param  aNumber     Number of the topic
        *   @param  aNewStatus  It is the new hotmark status.
        *   @return             Error code.
        */
        TInt ChangeTopicHotmarkStatus( TCbsTopicNumber aNumber, TBool aNewStatus );

        /**
        *   Requests the server to notify the client whenever an event occurs that changes
        *   the information of the topics. 
        *   
        *   Note that the client may select what kind of events it is interested in. Note also that
        *   there can be at most one pending request per instance of the class.
        *
        *   @param aStatus      It is the variable that the server will modify whenever an event occurs.
        *   @param aRequested   It contains the events the client is listening to.
        *   @param aEvent       It will contain the event that occurred.
        *   @param aNumber      Number of topic
        */
        void NotifyOnEvent( 
             TRequestStatus& aStatus, 
             const TInt aRequested, 
             TCbsTopicListEvent& aEvent, 
             TCbsTopicNumber& aNumber );

        /**
        *   Cancels the pending notify request.
        */
         void NotifyOnEventCancel();

        /**
        *   Returns the number of session-specific new topics and resets the counter.
        *   New topics are added by the topic detection feature in absence of
        *   CBS UI Application.
        *
        *   @param aCount       On return contains the amount of new topics.
        *   @return Result code.
        */
        TInt GetNewTopicsCount( TInt& aCount );

        /**
        *   Returns the latest topic's number.
        *
        *   @param aNumber       Number of the topic
        *   @return              Result code.
        */
        TInt GetLatestTopicNumber( TCbsTopicNumber& aNumber );

        /**
        *   Returns the total amount of unread messages.
        *
        *   @param aCount       It will contain the amount of unread messages.
        */
        void GetUnreadMessageCount( TInt& aCount );

        /**
        *   Returns the handle to the latest hotmarked message that has been
        *   received after the system has started up.
        *   
        *   @param aMessage     It is handle to the message.
        */
        void GetHotmarkedMessageHandle( TCbsMessageHandle& aMessage );
        
        /**
        *   Returns the number of unread messages in hotmarked topics.
        *
        *   This function is to used by the client when deciding whether
        *   the message or topic list view should be opened to display
        *   a hotmarked message(s).
        *
        *   @return                 Number of unread hotmarked messages
        */
        TInt NumberOfUnreadHotmarkedMessages();

        /**
        *   Returns the numbers of topics that precede and succeed the given 
        *   topic in server-side topic list.
        *
        *   If the given topic is the first topic in list, aPosition has 
        *   ECbsHead bit up. If the given topic is the last topic in list,
        *   aPosition has ECbsTail bit up.
        *    
        *   Return code values:
        *   KErrNone		aPrevTopicNumber, aNextTopicNumber and aPosition 
        *		            contain valid values.
        *   KErrNotFound	aCurrentTopicNumber specified a topic that was not
        *                   on topic list.
        *
        *   @param  aCurrentTopicNumber	    Number that specifies the topic 
        *                                   whose surroundings are returned
        *   @param  aPrevTopicNumber		Returns: number of topic preceding 
        *                                   the given topic
        *   @param  aNextTopicNumber		Returns: number of topic succeeding
        *                                   the given topic
        *   @param  aPosition			    Returns: position of current 
        *                                   topic in list.
        *   @return 				        Result code
        */
        TInt GetNextAndPrevTopicNumber( 
	        const TCbsTopicNumber& aCurrentTopicNumber,
	        TCbsTopicNumber& aPrevTopicNumber,
	        TCbsTopicNumber& aNextTopicNumber,
	        TInt& aPosition );

    private: // prohibited functions
        // Prohibited copy constructor
        RCbsTopicList( const RCbsTopicList& );

        // Prohibited assignment operator
        RCbsTopicList& operator=( const RCbsTopicList& );

    private:    // Data

        // A pointer descriptor used to transfer information 
        // during a notify request. 
        TPtr8 iNotifyEventPtr;

        // A pointer descriptor used to transfer information 
        // during a notify request.
        TPtr8 iNotifyHandlePtr;        
    };

#endif      //  RCBSTOPICLIST_H   
            
// End of File


