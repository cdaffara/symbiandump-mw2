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
* Description:  This file contains the header file of the CCbsTopicMessages class
*    
*                This class represents the server-side topic messages subsession class.
*
*/



#ifndef     CCBSTOPICMESSAGES_H
#define     CCBSTOPICMESSAGES_H

// INCLUDES
#include "CCbsObject.h"

// FORWARD DECLARATIONS
class CCbsDbImpTopicMessages;
class CCbsRecEtel;

// CLASS DECLARATION 
/**
    It represents the server side topic messages subsession.
*/
class CCbsTopicMessages : public CCbsObject
    {
    public:     // New functions
        /**
        *   Creates a new instance of the class.
        *
        *   @param  aSession    Pointer to the session.
        *   @param  aMessages   Pointer to the topic messages.
        *   @param  aReceiver   Receiver.
        *   @return             Pointer to a new instance of 
        *                       CCbsTopicMessages.
        */
        static CCbsTopicMessages* NewL( CCbsSession& aSession, 
                                        CCbsDbImpTopicMessages& aMessages,
                                        CCbsRecEtel& aReceiver );

        /**
        *   Destructor.
        */
        ~CCbsTopicMessages();

    public:     // From CCbsObject      
        /**
        *   Handle the requests for the object.
        *
        *   @param aMessage Message to be handled.
        *   @return         It indicates whether the message was handled.
        */
        TBool HandleRequestsL( const RMessage2& aMessage );

    private:
        /**
        *   Constructor.
        *
        *   @param  aSession    Pointer to the session.
        *   @param  aMessages   Pointer to the topic messages.
        *   @param  aReceiver   Receiver
        */
        CCbsTopicMessages( CCbsSession& aSession, 
                           CCbsDbImpTopicMessages& aMessages,
                           CCbsRecEtel& aReceiver );

        /**
        *   Finalizes construction.
        */
        void ConstructL();

        /**
        *   Closes the subsession.
        */
        void CloseTopicMessages();

        /**
        *   Returns the total amount of messages to the client.
        */
        void GetMessageCountL();

        /**
        *   Returns the requested message to the client.
        */
        void GetMessageL();

        /**
        *   Finds a message by its handle.
        */
        void FindMessageByHandleL();

        /**
        *   Returns message index in topic by message handle.
        */
        void GetMessageIndexByHandleL();

        /**
        *   Deletes a message.
        *
        *   @param  aReceiver   Receiver.
        */    
        void DeleteMessageL( const CCbsRecEtel& aReceiver );

        /**
        *   Saves a message.
        *
        *   @param  aReceiver   Receiver.
        */
        void SaveMessageL( const CCbsRecEtel& aReceiver );

        /**
        *   Locks a message.
        */
        void LockMessageL();

        /**
        *   Reads a message.
        *
        *   @param  aReceiver   Receiver.
        */
        void ReadMessageL( const CCbsRecEtel& aReceiver );

        /**
        *   Returns the content of the message to the client.
        */
        void GetMessageContentsL();

        /**
        *   Retrieves and returns the handles of messages that
        *   precede and succeed the given message in topic.
        *   
        *   Also returns two flags indicating whether the
        *   given message is the first and/or the last message in topic.
        */
        void GetNextAndPrevMsgHandleL();

    private:    // Prohibited operators and functions

        // Copy constructor
        CCbsTopicMessages( const CCbsTopicMessages& );

        // Assignment operator
        CCbsTopicMessages& operator=( const CCbsTopicMessages& );

    private:    // Data

        // Pointer to the topic messages manager
        CCbsDbImpTopicMessages& iMessages;
        
        // It is the locked message handle, or null message handle
        // in case no message is locked.        
        TCbsDbMessageHandle iLocked;
        
        // Receiver reference        
        CCbsRecEtel& iReceiver;
    };

#endif      //  CCBSTOPICMESSAGES_H   
            
// End of File


