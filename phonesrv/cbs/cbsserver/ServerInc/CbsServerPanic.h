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
* Description:  Declares the CbsServerPanic() function which panics the server.
*
*/


#ifndef CBSSERVERPANIC_H
#define CBSSERVERPANIC_H

// INCLUDES
#include <e32base.h>

//  DATA TYPES  

/**
*   Different reasons for panics in CbsServer.
*
*   Enumeration defined manually because of convience
*   => Release build panic dialog shows only the panic
*   number.
*/
enum TCbsServerPanic
    {
    ECbsSchedulerCreationFailed                 = 0,
    ECbsServerCreationFailed                    = 1,
    ECbsCollectorArrayEmpty                     = 2,
    ECbsServerStartFailed                       = 3,
    ECbsMsgRepUnprepared                        = 4,
    ECbsInvalidMessage                          = 5,
    ECbsDatabaseNull                            = 6,
    ECbsReceiverNull                            = 7,
    ECbsCbmiListNull                            = 8,
    ECbsCbmiListNotEmpty                        = 9,
    ECbsCbmiListMismatch                        = 10,
    ECbsErrorFromNonClientObject                = 11,
    ECbsSettingsInitFailed                      = 12,
    ECbsObserverNull                            = 13,
    ECbsUnrecognizedFactoryDefaultsTag          = 14,
    ECbsFactoryDefaultsFileCorrupt              = 15,
    ECbsFactoryDefaultsTopicsCorrupt            = 16,
    ECbsTopicListProcessPanic                   = 17,
    ECbsMessagesSubsessionInitFailed            = 18,
    ECbsMessageAlreadyPermanent                 = 19,
    ECbsMcnSessionInitFailed                    = 20,
    ECbsObserverNotFound                        = 21,
    ECbsTopicMessagesNull                       = 22,
    EMcnProviderNull                            = 23,
    EMcnSubscriptionsExceedTotalSubscriptions   = 24,
    ESubscriptionTableCorrupt                   = 25,
    EMcnProviderNotFound                        = 26,
    ECbsRecShouldBeOn                           = 27,
    ECbsMessageNullAtCopyConstructor            = 28,
    ECbsRetrievedNullMessageContent             = 29,
    ECbsFSSOpenFailed                           = 30,
    ECbsInvalidSharedDataEvent                  = 31,
    ECbsGeneratorInvalidMsgHeader               = 32,
    ECbsStoreNull                               = 33,
    ECbsNullTopic                               = 34,
    ECbsCacheNotValid                           = 35,
    EInvalidSessionObject                       = 36,
    ECbsCollectorMergeFailed                    = 37,
    ECbsInvalidLockHandle                       = 38,
    ECbsNotIndexMessage                         = 39,
    ECbsLanguageNotResolved                     = 40,
    ECbsInvalidLanguage                         = 41,
    ECbsTopicDuplicate                          = 42,
    ECbsGetTopicFailed                          = 43,
    ECbsFindTopicUnknownLeave                   = 44,
    ECbsInvalidFilenameDescriptor               = 45,
    ECbsTopicListArrayNull                      = 46
    };

//  FUNCTION PROTOTYPES  

/**
*   Panics the server.
*
*   @param aPanic Reason for the panic.
*/
GLDEF_C void CbsServerPanic( TCbsServerPanic aPanic );

#endif      //  CBSSERVERPANIC_H   
            
// End of File


