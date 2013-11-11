// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined(__MSGFILT_H__)
#define __MSGFILT_H__

#include <mentact.h>

class CMsvSession;
class CMsvEntryFilter;
class CMsvEntrySelection;

class CMessageQuery : public CMsgActive
	{
public:
	static CMessageQuery* NewLC(CMsvSession& aSession, TInt aPriority = CActive::EPriorityStandard);
	static CMessageQuery* NewL(CMsvSession& aSession, TInt aPriority = CActive::EPriorityStandard);
	~CMessageQuery();
	//
	void GetItemIdsL(const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection, TRequestStatus& aStatus);
	//
private:
	CMessageQuery(CMsvSession& aSession, TInt aPriority);
	void ConstructL();
	void DoRunL();
	//
private:
	CMsvSession& iSession;
	CMsvEntrySelection* iResultSelection;
	const CMsvEntryFilter* iFilter;
	//
	CMsvEntrySelection* iSelection;
	CMsvEntrySelection* iChildSelection;
	CMsvEntryFilter* iBlankFilter;
	};

#endif
