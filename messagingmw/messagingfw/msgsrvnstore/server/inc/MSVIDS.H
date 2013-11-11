// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

/**
 @file
 @publishedAll
 @released
*/

#if !defined(__MSVIDS_H__)
#define __MSVIDS_H__
#if !defined(__MSVSTD_HRH__)
#include <msvstd.hrh>
#endif
#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

// Predefined entry ids - values #defined in MSVSTD.HRH

/**
A NULL ID. 
This is typically used to indicate that the ID has not yet been set. It can 
also be used to "park" a CMsvServerEntry so that it doesn't lock an index 
entry.
@publishedAll
@released
@see TMsvId
@see KMsvNullIndexEntryIdValue
*/
const TMsvId KMsvNullIndexEntryId=KMsvNullIndexEntryIdValue;//0



/**
ID of root entry of entire index
@publishedAll
@released
@see TMsvId
@see KMsvRootIndexEntryIdValue
*/
const TMsvId KMsvRootIndexEntryId=KMsvRootIndexEntryIdValue;//0x1000

/**
ID of local service (containing the standard folders)
@publishedAll
@released
@see TMsvId
@see KMsvLocalServiceIndexEntryIdValue
*/
const TMsvId KMsvLocalServiceIndexEntryId=KMsvLocalServiceIndexEntryIdValue;//0x1001

/**
ID of Inbox folder 
@publishedAll
@released
@see TMsvId
@see KMsvGlobalInBoxIndexEntryIdValue
*/
const TMsvId KMsvGlobalInBoxIndexEntryId=KMsvGlobalInBoxIndexEntryIdValue;//0x1002

/**
ID of Outbox folder
@publishedAll
@released
@see TMsvId
@see KMsvGlobalOutBoxIndexEntryIdValue
*/
const TMsvId KMsvGlobalOutBoxIndexEntryId=KMsvGlobalOutBoxIndexEntryIdValue;//0x1003

/**
ID of Draft folder
@publishedAll
@released
@see TMsvId
@see KMsvDraftEntryIdValue
*/
const TMsvId KMsvDraftEntryId=KMsvDraftEntryIdValue; //0x1004

/**
ID of Sent folder
@publishedAll
@released
@see TMsvId
@see KMsvSentEntryIdValue
*/
const TMsvId KMsvSentEntryId=KMsvSentEntryIdValue; //0x1005

/**
ID of Deleted folder - Invisible folder
@publishedAll
@released
@see TMsvId
@see KMsvDeletedEntryFolderEntryIdValue
*/
const TMsvId KMsvDeletedEntryFolderEntryId=KMsvDeletedEntryFolderEntryIdValue;//0x1006

/**
ID used to indicate Unknown/Invalid Service entry folder
@publishedAll
@released
@see TMsvId
@see KMsvUnkownServiceIndexEntryIdValue
*/
const TMsvId KMsvUnknownServiceIndexEntryId=KMsvUnkownServiceIndexEntryIdValue; //0x1007

/**
Uid for POPService
@publishedAll
@released
*/
const TUid KUidPOPService = {0x2000BB68};

/**
Uid for IMAPService
@publishedAll
@released
*/
const TUid KUidIMAPService = {0x2000BB69};

/**
Uid for SMTPService
@publishedAll
@released
*/
const TUid KUidSMTPService = {0x2000BB6A};

/**
Uid for SendAs2Service
@publishedAll
@released
*/
const TUid KUidSendAs2Service = {0x2000BB6B};

/**
Uid for SMSService
@publishedAll
@released
*/
const TUid KUidSMSService = {0x2000BB6C};

#endif
