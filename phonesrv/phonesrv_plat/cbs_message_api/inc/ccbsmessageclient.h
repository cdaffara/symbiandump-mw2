/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the 
                 CCbsMessageClient class.
*
*                CCbsMessageClient provides the interface e.g for Cover UI for
*                accessing the CBS server.
*
*
*/



#ifndef     CCBSMESSAGECLIENT_H
#define     CCBSMESSAGECLIENT_H

// INCLUDES
#include "cbscommontypes.h"

//  CLASS DECLARATION

/**
*   CBS Message API e.g for Cover UI's needs. This API is a smaller snapshot
*   of CBS Client API.
*
*   @lib CbsClient.lib
*   @since 3.2
*/
class CCbsMessageClient : public CBase
    {
    public:     // New functions

        /**
        *   Instantiates new CCbsMessageClient object. During instantaion
        *   connection to CBS Server is created.
        *
        *   @return     A new instance of CCbsMessageClient
        */
        IMPORT_C static CCbsMessageClient* NewL();

        /**
        *   Finds a message by given handle.
        *
        *   @param  aHandle     Handle to the message.
        *   @param  aMessage    Return: contains the message information.
        *   @return             Error code.
        */
        IMPORT_C virtual TInt FindMessageByHandle(
            const TCbsMessageHandle& aHandle,
            TCbsMessage& aMessage ) = 0;

        /**
        *   Finds the topic by the given number.
        *
        *   Return values:        
        *
        *   @param  aNumber         Number of the topic
        *   @param  aTopic          Return: contains the topic information
        *   @return KErrNone        Topic returned as parameter
        *   @return KErrNotFound    Topic was not found
        */
        IMPORT_C virtual TInt FindTopicByNumber( const TCbsTopicNumber aNumber,
            TCbsTopic& aTopic ) = 0;

        /**
        *   Returns the total amount of topics the topic list contains.
        *
        *   @return                 Contains the total amount of topics.
        */
        IMPORT_C virtual TInt TopicCount() = 0;

        /**
        *   Returns information about a topic from the topic list.        
        *
        *   @param  aIndex       Index to the topic.
        *   @param  aTopic       It will contain the topic information.
        *   @return KErrArgument Topic was not found. Rest of return values
        *                        indicate a file error.
        */
        IMPORT_C virtual TInt GetTopic( const TInt aIndex, TCbsTopic& aTopic ) = 0;

        /**
        *   Returns the total amount of unread messages.
        *
        *   @return             Number of unread messages
        */
        IMPORT_C virtual TInt UnreadMessageCount() = 0;

        /**
        *   Returns the total amount of messages the topic contains.
        *
        *   Return codes:
        *               
        *   
        *
        *   @param  aNumber         Number of the topic.
        *   @param  aCount          Number of messages in given topic.
        *   @return KErrNotFound    Invalid handle
        *   @return KErrNone        aCount contains the number of messages 
        *                           in topic
        */
        IMPORT_C virtual TInt GetMessageCount( const TCbsTopicNumber aNumber,
            TInt& aCount ) = 0;

        /**
        *   Returns message information.
        *
        *   @param  aNumber         Number of the topic
        *   @param  aIndex          Index to the message in topic.
        *   @param  aMessage        Returns: the message information
        *   @return KErrNotFound    Topic or message not found
        *   @return KErrNone        aMessage contains the message information
        */
        IMPORT_C virtual TInt GetMessage( const TCbsTopicNumber aNumber, const TInt aIndex,
            TCbsMessage& aMessage ) = 0;

        /**
        *   Returns the message contents.
        *
        *   @param  aHandle     Handle to the message.
        *   @param  aBuffer     It will contain the contents (as much as it fits).
        *   @return             Error code.
        */
        IMPORT_C virtual TInt GetMessageContents(
            const TCbsMessageHandle& aHandle,
            TDes& aBuffer ) = 0;

        /**
        *   Sets the message as read.
        *
        *   @param  aHandle     Handle to the message to be set read.
        *   @return             Error code.
        */
        IMPORT_C virtual TInt SetMessageAsRead( const TCbsMessageHandle& aHandle ) = 0;

    protected:

        /**
        *   Default constructor.
        */
        CCbsMessageClient();
    };



#endif      //  CCBSMESSAGECLIENT_H

// End of File


