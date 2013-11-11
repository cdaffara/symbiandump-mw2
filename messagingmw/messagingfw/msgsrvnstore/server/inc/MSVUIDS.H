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
#if !defined(__MSVUIDS_H__)
#define __MSVUIDS_H__
#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif
#if !defined(__MSVSTD_HRH__)
#include <msvstd.hrh>
#endif


/**
* Entry type Uids
* @publishedAll
* @released
*/
const TUid KUidMsvNullEntry={0};
/**
* Root entry of entire index 
* @publishedAll
* @released
*/
const TUid KUidMsvRootEntry={KUidMsvRootEntryValue};
// Service 
/**
* @publishedAll
* @released
*/
const TUid KUidMsvServiceEntry={KUidMsvServiceEntryValue};
// Folder 
/**
* @publishedAll
* @released
*/
const TUid KUidMsvFolderEntry={KUidMsvFolderEntryValue};
// Message
/**
* @publishedAll
* @released
*/
const TUid KUidMsvMessageEntry={KUidMsvMessageEntryValue};
// Entry 
/**
* @publishedAll
* @released
*/
const TUid KUidMsvAttachmentEntry={KUidMsvAttachmentEntryValue};

// Local Service Uid
// An operation that affects only local entries. 
/**
* @publishedAll
* @released
*/
const TUid KUidMsvLocalServiceMtm={KUidMsvLocalServiceMtmValue};
//An operation performed by the Message Server. 
/**
* @publishedAll
* @released
*/
const TUid KUidMsvServerMtm={KUidMsvServerMtmValue};
// Key for Publish & Subscribe Disk Space Monitor  
/**
* @publishedAll
* @released
*/
const TInt KUidPSDiskSpaceMonitorKeyType={0x2001FC30};
  	  	 	 
#endif
