/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/



/**
 @file
*/
 
#ifndef __MSGINGUTILS_H__
#define __MSGINGUTILS_H__
 
#include <msvstd.h>
#include <msvapi.h>

	_LIT(KInbox, "Inbox");
	_LIT(KOutbox, "Outbox");
	_LIT(KDraft, "Draft");
	_LIT(KSent, "Sent");
	_LIT(KDeletedEntryFolder, "DeletedEntryFolder");
	_LIT(KRoot, "Root");
	_LIT(KLocal, "Local");

class CMsvSession;
class VerifyIndicationTestStep;

class MsgingUtils 
	{
public:
	static TMsvId GetLocalFolderId(const TDesC& aFolderName);
	static TMsvId GetEntryByNameL(const TDesC& aSubject,TMsvId aFolderId, CMsvSession& aSession);
	};

class CEntryCmdHandler : public MMsvEntryObserver
	{
public:
	static CEntryCmdHandler* NewL();
	void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /* aArg2*/, TAny* /*aArg3*/);
	~CEntryCmdHandler();
public:
	TInt iCount;
private:
	void ConstructL();
	CEntryCmdHandler();

	};
#endif //__MSGINGUTILS_H__