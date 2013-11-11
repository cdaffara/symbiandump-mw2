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
// BIO-message SMS Port Watcher
// 
//

#ifndef WAPWATCHER_H__
#define WAPWATCHER_H__

#include <e32base.h>

#include "SmsWatcher.h"

class CWatcherLog;

class CWapWatcher : public CSmsBaseWatcher
	{
	public:
		static CWapWatcher* NewL(TAny* aWatcherParams);
		
	public:
		
	private:
		CWapWatcher(RFs& aFs, CWatcherLog& aWatcherLog);
		~CWapWatcher();
		void StartL();
		CBaseSmsActiveSocketWatcher* CreateSocketWatcherLC(const TUid aBioUid, const TBioMsgId& aBioMsg);
		TBool SupportBioMsgId(const TBioMsgId& aBioMsg) const;
		
		void AddBifL(TUid aBioID);
		void CreateNewWapServerL();
	};

#endif
