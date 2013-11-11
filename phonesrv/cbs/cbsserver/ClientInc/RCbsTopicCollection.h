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
* Description:  This file contains the header file of the RCbsTopicCollection class.
*                This is the client-side subsession which handles requests regarding
*                topic identities stored on CBS Server.
*
*
*/



#ifndef     RCBSTOPICCOLLECTION_H
#define     RCBSTOPICCOLLECTION_H

//  INCLUDES

#include <e32base.h>
#include "CbsCommon.h"

//  FORWARD DECLARATIONS
class RCbs;

//  CLASS DECLARATION 

/**
*   This represents the client-side subsession to retrieve information 
*   from topic collections.
*/
class RCbsTopicCollection 
        : public RSubSessionBase
    {
    public:     // New functions
        /**
        *   Constructor.
        */
        RCbsTopicCollection();

        /**
        *   Creates a subsession to the server.
        *
        *   Note that the method must be called before any other method 
        *   can be used.
        *
        *   @param  aServer             The base session.
        *   @return                     Error code.
        */
        TInt Open( RCbs& aServer );

        /**
        *   Closes the subsession.
        *
        *   @return                     Error code.
        */
        TInt Close();

        /**
        *   Returns the total amount of topic information the topic collection
        *   contains.
        *
        *   @param  aCount              It will contain the amount.
        */
        void GetTopicCount( TInt& aCount );

        /**
        *   Returns a topic information structure.
        *
        *   @param  aIndex              It is index to the topic info.
        *   @param  aInfo               It will contain the information.
        *   @return                     Error code.
        */
        TInt GetTopicInfo( TInt aIndex, TCbsTopicInfo& aInfo );

        /**
        *   Notifies the client next time when topic collection changes.
        *   
        *   @param  aStatus             It will be changed when topic collection
        *                               changes.
        */
        void NotifyOnChange( TRequestStatus& aStatus );

        /**
        *   Cancels the pending request to notify.
        */
        void NotifyOnChangeCancel();

        /**
        *   Resets the iterator. Must be called prior any call to HasNextTopic() 
        *   or NextTopic()!
        */
        void Start();

        /**
        *   Returns ETrue, if there is a topic not accessed with NextTopic()
        *   
        *   @return     ETrue, if there is a topic. EFalse if the end of the
        *               collection has been reached.
        */
        TBool HasNextTopic();                          

        /**
        *   Returns the next topic in the topic collection skipping all topics with 
        *   a topic number matching a topic already listed in the current topic list.
        *   This function will return KErrNotFound if the collection is tried to 
        *   access beyond the end of the list. Otherwise the error code will be 
        *   the same returned by GetTopicInfo().
        *
        *   @param aInfo        The topic information will be stored here.
        *   @return             The error code. KErrNotFound if there are no topics
        *                       left.
        */
        TInt NextTopic( TCbsTopicInfo& aInfo );

    private:
        
        /**
        *   Returns information whether the topic was in topic list.
        *   
        *   @param 	aIndex      Topic index
        *   @param  aInList     Was the topic in the list
        *   @return             Error code. KErrNotFound, if topic
        *                       didn't exist.
        */
        TInt TopicInList( TInt aIndex, TBool& aInList );

    private:// prohibited functions
        // Prohibited copy constructor
        RCbsTopicCollection( const RCbsTopicCollection& );

        // Prohibited assignment operator
        RCbsTopicCollection& operator=( const RCbsTopicCollection& );

    private:    // Data
        //Iterator is used to point to the next unlisted topic in topic collection. 
        //Unlisted topic is a topic which is not listed in the current topic list but 
        //is awailable in the topic collection
        TInt iIterator;     
    };

#endif      //  RCBSTOPICCOLLECTION_H
            
// End of File


