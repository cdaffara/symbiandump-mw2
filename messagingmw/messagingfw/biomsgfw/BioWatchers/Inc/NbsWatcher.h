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

#ifndef NBS_WATCHER_H_
#define NBS_WATCHER_H_

#include <e32base.h>
#include <es_sock.h>
#include <smsuaddr.h>
#include "SmsWatcher.h"

class CWatcherLog;

// Constants
const TUid KUidNbsSmsWatcher = {0x10005543};

// Export this First!
//IMPORT_C TInt WinsMain(TAny* /*aParam*/);

//
// Data types
class CNbsWatcher : public CSmsBaseWatcher
	{
public:
	static CNbsWatcher* NewL(TAny* aWatcherParams);
	virtual ~CNbsWatcher();
private:
	void StartL();
	CNbsWatcher(RFs& aFs, CWatcherLog& aLog);
	CBaseSmsActiveSocketWatcher* CreateSocketWatcherLC(const TUid aBioUid, const TBioMsgId& aBioMsg);
	void ConstructL();
	TBool SupportBioMsgId(const TBioMsgId& aBioMsg) const;
	void SetupSpecialWatcherL(TSmsAddrFamily aAddrFamily);
	void AddBifL(TUid aBioID);
	
private:
	CBaseSmsActiveSocketWatcher* iReceiveAnySocket;
		
	};

#endif
