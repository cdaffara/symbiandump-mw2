/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definitions for podcast type
*
*/



#ifndef CMPXPODCASTDEFS_H
#define CMPXPODCASTDEFS_H

#include <e32base.h>
#include <mpxattribute.h>

/**
*  Content ID identifying Podcast category of content provided
*  in the media object and associated attributes. 
*/
const TInt KMPXMediaIdPodcast = 0x101FFC46;


enum TMPXPodcastCategory
    {
    EMPXAll,                // 0
    EMPXTitle,              // 1
    EMPXPubDate,            // 2
    EMPXRecentlyAdded,      // 3
    EMPXNotYetPlayed,       // 4
    EMPXNew,                // 5
    EMPXPartlyPlayed,       // 6
    EMPXCompletelyPlayed,   // 7
    EMPXEpisode,            // 8
    EMPXPodcastFile,        // 9
    EMPXPodcastCollection,  // 10
    EMPXUnknown
    };
    
enum TMPXPodcastType
    {
    EMPXPodcastNoType,
    EMPXPodcastGroup,
    EMPXPodcastItem
    }; 

/**
*  TMPXPodcastType: see above enum definition
*/
const TMPXAttributeData KMPXMediaPodcastType={KMPXMediaIdPodcast,0x01};
 
/**
*  Descriptor
*/
const TMPXAttributeData KMPXMediaPodcastTitle={KMPXMediaIdPodcast,0x02};

/**
*  Descriptor
*/
const TMPXAttributeData KMPXMediaPodcastCategory={KMPXMediaIdPodcast,0x04}; 

/**
*  TTime
*/
const TMPXAttributeData KMPXMediaPodcastPubDate={KMPXMediaIdPodcast,0x08};

/**
*  TMPXPodcastCategory: See above for enum definition
*/
const TMPXAttributeData KMPXMediaPodcastCategoryGroup={KMPXMediaIdPodcast,0x10};

/**
*  TUint
*/
const TMPXAttributeData KMPXMediaPodcastPlayCount={KMPXMediaIdPodcast,0x20};

/**
*  TMPXItemId
*/
const TMPXAttributeData KMPXMediaPodcastPubDateCategoryId={KMPXMediaIdPodcast,0x40};

/**
* TBool
*/
const TMPXAttributeData KMPXMediaPodcastIsPlaying={KMPXMediaIdPodcast,0x80};

/**
* TBool
*/
const TMPXAttributeData KMPXMediaPodcastSetIsPlayingTrue={KMPXMediaIdPodcast,0x100};

/**
*  TInt
*/
const TMPXAttributeData KMPXMediaPodcastCurrentlyPlayingIndex = {KMPXMediaIdPodcast,0x200};
         
/**
*  Attributes supported for KMPXMediaIdPodcast
*/
enum 
    {
    EMPXMediaPodcastType = 0x01,
    EMPXMediaPodcastTitle = 0x02,       // Text
    EMPXMediaPodcastCategory = 0x04,    // Text, episode category
    EMPXMediaPodcastPubDate=0x08,        // TTime    
    EMPXMediaPodcastCategoryGroup = 0x10,     // See enum TMPXPodcastCategory
    EMPXMediaPodcastPlayCount = 0x20,   // TBool
    EMPXMediaPodcastPubDateCategoryId = 0x40, // TMPXItemId
    EMPXMediaPodcastIsPlaying = 0x80, // TBool
    EMPXMediaPodcastSetIsPlayingTrue = 0x100, // TBool
    EMPXMediaPodcastCurrentlyPlayingIndex = 0x200, //TInt
    EMPXMediaPodcastAll=0xffffffff & ~(EMPXMediaPodcastSetIsPlayingTrue)
    };

/**
*  Used to indicate the current podcast collection view
*/   
enum TMPXPodcastView
    {
    EMPXMainPodcastMenuView,
    EMPXTitlesView,
    EMPXPublishDateView,
    EMPXEpisodesView    
    };
     
#endif

 // CMPXPODCASTDEFS_H
