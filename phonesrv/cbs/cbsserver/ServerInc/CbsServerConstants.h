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
* Description:  Contains constants (version number, request codes) used by the server.*
*/


#ifndef CBSSERVERCONSTANTS_H
#define CBSSERVERCONSTANTS_H

//  INCLUDES

#include <e32base.h>
#include "CbsCommon.h"

//  CONSTANTS

// How long to wait if the server was locked by backup. Time in microseconds. 
const TInt KTimeToWaitForBackup = 100000;

// How many times to retry if the server is locked.
const TInt KMaxRetryTimes = 50;

// Name of CbsServer thread
_LIT( KCbsServerName, "CbsServer" );

// Server major version number
const TInt KCbsServerVersionMajor  =   1;

// Server minor version number
const TInt KCbsServerVersionMinor  =   2;

// Server build number
const TInt KCbsServerVersionBuild  =   0;

/**
*   Enumeration of client requests.
*/
enum TCbsServerRequest
    {
    // RCbs requests
    ECbsCloseSession,
    ECbsCreateSettingsSubsession,
    ECbsCreateTopicListSubsession,
    ECbsCreateTopicMessagesSubsession,
    ECbsCreateTopicCollectionSubsession,
    ECbsShutdown,
    
    // RCbsSettings subsession requests
    ECbsCloseSettingsSubsession,
    ECbsSetReceptionStatus,
    ECbsGetReceptionStatus,
    ECbsSetTopicDetectionStatus,
    ECbsGetTopicDetectionStatus,
    ECbsGetLanguages,
    ECbsSetLanguages,
    ECbsNotifySettingsChanged,
    ECbsNotifySettingsChangedCancel,

    // RCbsTopicList subsession requests
    ECbsCloseTopicListSubsession,
    ECbsGetTopicCount,
    ECbsGetTopic,
    ECbsFindTopicByNumber,
    ECbsDeleteTopic,
    ECbsAddTopic,
    ECbsChangeTopicNameAndNumber,
    ECbsChangeTopicSubscriptionStatus,
    ECbsChangeTopicHotmarkStatus,
    ECbsNotifyOnEvent,
    ECbsNotifyOnEventCancel,
    ECbsGetNewTopicsCount,
    ECbsGetLatestTopicNumber,
    ECbsGetUnreadMessageCount,
    ECbsGetHotmarkedMessageHandle,
    ECbsGetUnreadHotmarkedMessageCount,
    ECbsDeleteAll,
    ECbsGetNextAndPrevTopicNumber,

    // RCbsTopicMessages subsession requests
    ECbsCloseTopicMessagesSubsession,
    ECbsGetMessageCount,
    ECbsGetMessage,
    ECbsFindMessageByHandle,
    ECbsGetMessageIndexByHandle,
    ECbsDeleteMessage,
    ECbsSaveMessage,
    ECbsReadMessage,
    ECbsLockMessage,
    ECbsGetMessageContents,
    ECbsGetNextAndPrevMsgHandle,
        
    // RCbsTopicCollection subsession requests
    ECbsCloseTopicCollectionSubsession,
    ECbsGetTopicInfoCount,
    ECbsGetTopicInfo,
    ECbsGetTopicInfoCountNotListed,
    ECbsTopicInList,
    ECbsNotifyOnChange,
    ECbsNotifyOnChangeCancel,
    ECbsGetUnlistedTopicIndexies,

    // RCbsMcnSession requests
    EMcnCreateSubsession,
    EMcnCloseSession,
    EMcnNotifyOnChange,
    EMcnNotifyOnChangeCancel,
    EMcnGetCellInfo,
    EMcnSubscribeTopic,
    EMcnClearSubscriptions,
    EMcnNoMoreSubscriptions,
    EMcnCloseSubsession,
    EMcnGetInfoMessage
    };

// Number of message slots used in server-client ITC.
const TInt KCbsServerDefaultSlots = 4;

// Header size of a CB message page in bytes.
const TInt KCbsCbMsgHeaderSize = 6;

// Maximum characters in page
const TInt KCbsMaxCharsInPage = 93;

//  DATA TYPES  

// Used by RCbs::GetNextAndPrevTopicNumber to transfer request results
// from server to client.
struct TCbsNextPrevTopicAndPosition
    {
    TCbsTopicNumber iPrevTopic;
    TCbsTopicNumber iNextTopic;
    TInt iPosition;
    };

// Used by RCbs::GetNextAndPrevMsgHandle to transfer request results
// from server to client.
struct TCbsNextPrevMsgAndPosition
    {
    TCbsMessageHandle iPrevMsg;
    TCbsMessageHandle iNextMsg;
    TInt iPosition;
    };

// Used by active objects to determine the command type.
enum TCbsServerEtelCommand
    {
    ECbsReceiveMessage,
    ECbsSetFilterSetting,
    ECbsGetLanguageFilter,
    ECbsSetLanguageFilter,
    ECbsRetrieveBroadcastIdList,
    ECbsStoreBroadcastIdList,
    ECbsBroadcastListCancelAll,
    ECbsCommandTypeNotSet
    };

// District info message topic.
const TInt KCellInfoTopic = 50;

// HomeZone message topic.
const TInt KHomeZoneTopic = 221;

#endif      //  CBSSERVERCONSTANTS_H   
            
// End of File


