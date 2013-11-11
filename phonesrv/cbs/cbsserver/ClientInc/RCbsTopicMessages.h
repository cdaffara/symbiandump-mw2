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
* Description:  This file contains the header file of the RCbsTopicMessages class
*    
*                Client-side subsession handling requests which regard 
*                CB messages.
*
*
*/



#ifndef     RCBSTOPICMESSAGES_H
#define     RCBSTOPICMESSAGES_H

// INCLUDES
#include <e32base.h>
#include "CbsCommon.h"

// FORWARD DECLARATIONS
class RCbs;

// CLASS DECLARATION 

// Client-side subsession handling requests regarding 
// CB messages.
//
class RCbsTopicMessages 
        : public RSubSessionBase
    {
    public:     // New functions
        /**
        *   Constructor.
        */
        RCbsTopicMessages();

        /**
        *   Creates a subsession to the server. Before using any other method 
        *   it must be called.
        *
        *   @param aServer      It is the cbs server.
        *   @return             Return code.
        */
        TInt Open( RCbs& aServer );
        
        /**
        *   Closes the subsession.
        *
        *   @return             Return code.
        */
        TInt Close();

        /**
        *   Returns the total amount of messages the topic contains.    
        *
        *   @param  aNumber     Topic number.
        *   @param  aCount      It will contain the total amount of 
        *                       messages.
        */
        TInt GetMessageCount( TCbsTopicNumber aNumber, TInt& aCount );
        
        /**
        *   Returns message information.
        *
        *   @param  aNumber     Topic number
        *   @param  aIndex      It is index to the message in topic. 
        *   @param  aMessage    It will contain the message information.
        *   @return             Return code.
        */
        TInt GetMessage( TCbsTopicNumber aNumber, TInt aIndex, 
            TCbsMessage& aMessage );

        /**
        *   Finds a message by given handle.
        *
        *   @param  aHandle     It is handle to the message.
        *   @param  aMessage    It will contain the message information.
        *   @return             Return code.
        */
        TInt FindMessageByHandle( const TCbsMessageHandle& aHandle, 
            TCbsMessage& aMessage );

        /**
        *   Returns the index of a message with given handle in topic.
        *
        *   Result code KErrNotFound indicates that no message was found with
        *   the given handle.
        *
        *   @param  aHandle     Handle of the message
        *   @param  aIndex      Return: index of the message in message topic
        *   @return             Result code
        */
        TInt GetMessageIndexByHandle( 
            const TCbsMessageHandle& aHandle, TInt& aIndex );

        /**
        *   Deletes an existing message.
        *
        *   Note that it does not care a lot about the status of the message.
        *   
        *   Check also the description of LockMessage method.
        *
        *   @param  aHandle     It is handle to the message.
        *   @return             Return code.
        */
        TInt DeleteMessage( const TCbsMessageHandle& aHandle );
        
        /**
        *   Saves a message (i.e., the saved message won't be deleted to make 
        *   room for new messages).
        *
        *   Return codes:
        *   KErrNone            Message is saved.
        *   KErrGeneral         Message not saved -- total maximum of saved 
        *                       messages reached.
        *
        *   @param  aHandle     Handle to the message to be saved.
        *   @return             Return code.
        */
        TInt SaveMessage( const TCbsMessageHandle& aHandle );

        /**
        *   Locks the message.
        *
        *   Note that a single topic messages subsession can have at most one 
        *   locked message. 
        *
        *   Message can be unlocked by trying to lock a null message. Locked 
        *   message will also be automatically unlocked when subsession is 
        *   closed. If a message is locked, then it will not be deleted from
        *   the database. Thus, deleting a message or trying to delete a 
        *   topic that contains such a message will fail.
        *
        *   Locking a message does not prevent to save the message nor read 
        *   the message.
        *
        *   @param  aHandle     It is handle to the message to be locked.
        *   @return             Return code.
        */
        TInt LockMessage( const TCbsMessageHandle& aHandle );
        
        /**
        *   Sets the message as read.
        *
        *   @param  aHandle     It is handle to the message to be set read.
        */
        TInt ReadMessage( const TCbsMessageHandle& aHandle );
        
        /**
        *   Returns the message contents.
        *
        *   @param  aHandle     It is handle to the message.
        *   @param  aBuffer     It will contain the contents (as much as it fits).
        *   @return             Return code.
        */
        TInt GetMessageContents( const TCbsMessageHandle& aHandle, 
            TDes& aBuffer );

        /**
        *   Returns the handles of messages that precede and succeed the 
        *   given message in server-side list of topic messages.
        *
        *   Topic is resolved from the given message handle
        *
        *   If the given handle specifies the first message in topic,
        *   aPosition has ECbsHead bit up. If the given handle specifies 
        *   the last message in topic, aPosition has ECbsTail bit up.
        *    
        *    Return code values:
        *    KErrNone       aPrevMsgHandle, aNextMsgHandle and aPosition
        *                   contain valid values.
        *    KErrNotFound	aCurrentMsgHandle specified a message that was not
        *			        found.
        *
        *    @param  aCurrentMsgHandle		Handle that specifies the message 
        *                                   whose surroundings are returned
        *    @param  aPrevMsgHandle		    Returns: handle of message 
        *                                   preceding the given message
        *    @param  aNextMsgHandle		    Returns: handle of message 
        *                                   succeeding the given topic
        *    @param  aPosition			    Returns: position of current topic 
        *                                   in list
        *    @return 				        Result code
        */
        TInt GetNextAndPrevMessageHandle(
	        const TCbsMessageHandle& aCurrentMsgHandle,
	        TCbsMessageHandle& aPrevMsgHandle,
	        TCbsMessageHandle& aNextMsgHandle,
	        TInt& aPosition );

    };

#endif      //  RCBSTOPICMESSAGES_H   
            
// End of File


