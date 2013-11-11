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
* Description:  This file contains a common header file between CBS Server and
*                CBS Message Client packages.
*
*/



#ifndef CBSCOMMONTYPES_H
#define CBSCOMMONTYPES_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS

// The length of the topic name (79 characters plus zero).
const TInt KCbsDbTopicNameLength = 80;

// DATA TYPES

/**
*   Type for representing handles to the messages.
*
*   Each message has an unique handle. The handle is constructed so that it is
*   possible to determine the topic to which the message belongs. This is done
*   so that it is possible to extract the topic handle from the message handle.
*
*   As for topic handles, when the message is deleted, then the same handle can
*   be used again.
*/
typedef TUint32 TCbsDbMessageHandle;

/**
*   Type for representing message keys.
*
*   Each message has also message key. It is constructed so that by comparing
*   the message keys it is possible to determine whether two messages are
*   indeed the same.
*
*   You should refer to GSM specifications (03.41) to find out what
*   information needs to be found from the key.
*
*   The message key is called Serial Number in GSM specifications. It is
*   divided into Geographical Scope, Message Code and Update Number fields.
*/
typedef TUint16 TCbsDbMessageKey;

// Type for message length.
typedef TInt TCbsDbMessageLength;

/**
*   Type for representing topic numbers.
*
*   Topic numbers are integers in range from 0 to 999. In GSM specifications
*   they are called message identifiers.
*/
typedef TUint16 TCbsDbTopicNumber;

// Type for the name of the topic.
typedef TBuf< KCbsDbTopicNameLength > TCbsDbTopicName;

// The structure that represents a single message header.
struct TCbsDbMessage
    {
    // The handle of the message
    TCbsDbMessageHandle iHandle;

    // The message key (that is, the serial number)
    TCbsDbMessageKey iKey;

    // Determines the language in which the message is written.
    TInt iLanguage;

    // Time when the message was received.
    TInt64 iDateTime;

    // Indicates whether the message is saved or only received.
    //lint -e806
    TBool iPermanent:2;

    // Indicates whether the message has been read.
    TBool iRead:2;

    // Tells the length of the message contents.
    TCbsDbMessageLength iLength;
    };

// The structure that represents a single topic.
struct TCbsDbTopic
    {
    // The topic number.
    TCbsDbTopicNumber iNumber;

    // The name of the topic.
    TCbsDbTopicName iName;

    // Indicates whether the topic is protected.
    // If a topic is protected, it is not possible to edit
    // the topic number or name.
    // It is not possible to delete a topic, too.
    TBool iProtected:2;

    // Indicates whether the user has subscribed to the topic.
    // Only messages that belong to subscribed topics are received.
    TBool iSubscribed:2;

    // Indicates whether the topic is hotmarked.
    TBool iHotmarked:2; //lint +e806

    // Tells the total amount of unread messages in the topic.
    TInt iUnreadMessages;

    // Tells the total amount of saved messages in the topic.
    TInt iSavedMessages;
    };

// A handle to a message.
typedef TCbsDbMessageHandle TCbsMessageHandle;

// The structure that represents a single message header.
typedef TCbsDbMessage TCbsMessage;

// A topic number.
typedef TCbsDbTopicNumber TCbsTopicNumber;

// A single topic information structure
typedef TCbsDbTopic TCbsTopic;

#endif      // CBSCOMMONTYPES_H

// End of File

