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
* Description:  This file contains The only common header file between CBS UI and 
*                CbsClient packages.
*
*/



#ifndef CBSCOMMON_H
#define CBSCOMMON_H

//  INCLUDES
#include <e32base.h>
#include <s32std.h>
#include <cbscommontypes.h>

// CONSTANTS

// Max count of topic lists in the root
const TInt KCbsRootItemsSize = 1;

// DATA TYPES  

/**
*   Type for representing handles to the topics.
*
*   For each topic there is an unique handle that does not change. 
*   When the topic is deleted, the same handle can be used again.
*/
typedef TUint16 TCbsDbTopicHandle;

// A handle to a topic.
typedef TCbsDbTopicHandle TCbsTopicHandle;

// A topic name.
typedef TCbsDbTopicName TCbsTopicName;

/**
*   The structure that represents information about single topic. 
*
*   When an index message is received, then a new topic collection is created.
*   In deed, records of this type are created.
*/
struct TCbsDbTopicIdentity
    {
    // Name of the topic
    TCbsDbTopicName iName;

    // The topic number (that is, the message identifier)
    TCbsDbTopicNumber iNumber;
    };

// Type returned by RCbs::GetNextAndPrevTopicNumber and 
// RCbs::GetNextAndPrevMessageHandle to specify whether the item in question 
// is the first or the last (or both) in list.
enum TCbsPositionInList 
    {
    ECbsHead = 0x01,
    ECbsTail = 0x02
    };

// Enumerates the different language settings.
enum TCbsDbLanguage
    {
    ECbsGerman,                 // German             
    ECbsEnglish,                // English      
    ECbsItalian,                // Italian      
    ECbsFrench,                 // French       
    ECbsSpanish,                // Spanish      
    ECbsDutch,                  // Dutch        
    ECbsSwedish,                // Swedish      
    ECbsDanish,                 // Danish       
    ECbsPortuguese,             // Portuguese   
    ECbsFinnish,                // Finnish      
    ECbsNorwegian,              // Norwegian    
    ECbsGreek,                  // Greek        
    ECbsTurkish,                // Turkish      
    ECbsHungarian,              // Hungarian
    ECbsPolish,                 // Polish
    ECbsCzech,                  // Czech
    ECbsHebrew,                 // Hebrew
    ECbsArabic,                 // Arabic
    ECbsRussian,                // Russian
    ECbsIcelandic,              // Icelandic
    ECbsOther,                  // Other 
    ECbsAll,                    // All
    ECbsCount                   // The amount of languages, 
                                // this must be the last element
    };


// Structure to contain user-selected languages
struct TCbsDbLanguages
    {
    TBool iLanguages[ ECbsCount ];
    };

// Structure containing cached information on topic
struct TCbsDbImpTopic
    {
    // The stream identifier to the topic information.
    TStreamId iTopicId;

    // The stream identifier to the messages of the topic.
    TStreamId iTopicMessagesId;

    // The rest of the topic information
    TCbsDbTopic iTopicData;
    };

// Structure that contains information about a topic list
struct TCbsDbTopicList
    {    
    // Name of the topic list
    TBuf< KCbsDbTopicNameLength > iTopicListName;

    // Is this topic list the default topic list
    TBool iIsDefaultTopicList;
    
    // The integer identifier of the list
    TInt iNumber;

    // NUmber of topic lists in this list
    TInt iTopicCount;
    };


// The structure that contains (root) stream information.
struct TCbsDbImpTopicList
    {    
    // Name of the topic list
    TBuf< KCbsDbTopicNameLength > iTopicListName;    

    // Is this topic list the default topic list
    TBool iIsDefaultTopicList;

    // The integer identifier of the list
    TInt iNumber;

    // NUmber of topic lists in this list
    TInt iTopicCount;
        
	// The ID to topic list stream of this topic list
	TStreamId iTopicListId;
    };

// Enumerates the reasons for the client panic.
enum TCbsSessionPanic
    {
    ECbsBadRequest,
    ECbsBadDescriptor
    };

// Enumerates the different types of settings-session related events.
enum TCbsSettingsEvent
    {
    ECbsModifiedReceptionStatus,        // Reception status was modified
    ECbsModifiedTopicDetectionStatus,   // Topic detection status was modified
    ECbsModifiedLanguages,              // Preferred languages were modified
    ECbsModifiedLimitedReceptionStatus, // Time-limited reception status modified
    ECbsModifiedCleanupTime             // Clean up time of read messages modified
    };

// Enumerates the different types of topic list events that can occur.
enum TCbsTopicListEvent
    {
    ECbsTopicListInitialized = 0x01,
    ECbsTopicAdded = 0x02,
    ECbsTopicModified = 0x04,
    ECbsTopicDeleted = 0x08,
    ECbsTopicReceivedNewMessage = 0x10,
    ECbsTopicAllEvents = ( 0x01 | 0x02 | 0x04 | 0x08 | 0x10 )
    };

// Structure to contain user-selected languages
typedef TCbsDbLanguages TCbsSettingsLanguages;

// A structure that contains information about a topic list
typedef TCbsDbTopicList TCbsTopicList;

// A structure that contains information about one topic (information
// that is retrieved from the index topic).
typedef TCbsDbTopicIdentity TCbsTopicInfo;

#endif      // CBSCOMMON_H   
            
// End of File

