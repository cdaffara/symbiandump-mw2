// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// WAP Push - ConnectionManager Test Harness
// This file contains the declaration of the CWapPushTestMsgUtils which performs operations with
// the message server
// 
//

/**
 @file T_SERVICEUTILS.H
*/

#ifndef __T_SERVICEUTILS_H__
#define __T_SERVICEUTILS_H__

#include <e32std.h>
#include <msvapi.h>

#define __TEST_SERVICE_CREATION__

/*
 *Utility class that performs operations with with the Message Server 
 */
class CWapPushTestMsgUtils: public CBase, public MMsvSessionObserver
	{
public:
	static CWapPushTestMsgUtils* NewL();
	~ CWapPushTestMsgUtils();

	void PushServiceIdL(TMsvId& rFirstId, CMsvEntrySelection* rServiceIds);
	TMsvId CreateServiceL();
	TMsvId CreateMmsServiceL();
	TMsvId CreatePushMsgFolderL();
	void RemoveServiceEntryChildrenL(TMsvId aServiceId);
	void RemoveEntriesFromLocalServiceFolderL(TMsvId aPushFolderId, TUid aMtm);
	void PushFolderIdL(TMsvId& rFirstFolderId, CMsvEntrySelection* rFolderIds);
	void InstallMtmGroupL(const TDesC& aDatFile);
	void HandleSessionEventL(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/) {}

	// Only use the following if you want to remove the service or folder entries. Handle with care!
	void RemoveAllPushServiceEntriesL(); // Clears any existing Push Service entries - test if creation works
	void RemoveAllPushFolderEntriesL();      //Removes all existing Push Folder Entries.

private:
	void ConstructL();
	void DeleteEntriesSychronouslyL(CMsvEntrySelection* aSelection);
	void SetMmsSettings(TMsvId id);

	CMsvSession* iMsvSession;
	CMsvEntry*	 iMsvEntry;
	TMsvId		 iPushServiceId;

	};

#endif
