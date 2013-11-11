// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef SMS_BASE_WATCHER_H
#define SMS_BASE_WATCHER_H

#include "SmsSocketWatcher.h"
#include <watcher.h>
#include <bifchangeobserver.h>

class CActive;

// Data types
class CSmsBaseWatcher : public CActive, public MBioWatcherObserver, public MBifChangeObserver, public MMsvSessionObserver
	{
public:
	//--------------------------------------
	//     Construction & Destruction
	//--------------------------------------
	IMPORT_C ~CSmsBaseWatcher();

protected:
	IMPORT_C CSmsBaseWatcher(RFs& aFs, CWatcherLog& aLog, TInt aPriority = EPriorityStandard);
	IMPORT_C void ConstructL();
	IMPORT_C virtual void StartL();
	IMPORT_C virtual void Reset();

	IMPORT_C void RunL();
	IMPORT_C TInt RunError(TInt aError);
	IMPORT_C void DoCancel();
	IMPORT_C void CompleteMyself(TInt aError);

	//MBioWatcherObserver
	IMPORT_C void HandleWatcherComplete(CBaseSmsActiveSocketWatcher& aWatcher, TInt aError);

	IMPORT_C void CreateSocketWatchersFromBioDbL(TBioMsgIdType aType);
	IMPORT_C void CreateSocketWatchersFromBioDbL(TBioMsgIdType aType, TUid aBioID);

	IMPORT_C void DeleteSocketWatchersWithUidL(TUid aUid);

	IMPORT_C void StartSocketWatchersL();

	virtual CBaseSmsActiveSocketWatcher* CreateSocketWatcherLC(const TUid aBioUid, const TBioMsgId& aBioMsg) = 0;
	virtual TBool SupportBioMsgId(const TBioMsgId& aBioMsg) const = 0;

	//MBifChangeObserver
	IMPORT_C void HandleBifChangeL(TBifChangeEvent aEvent, TUid aBioID);

	virtual void AddBifL(TUid aBioID) = 0;
	IMPORT_C void AddBifWithTypeL(TBioMsgIdType aType, TUid aBioID);

	IMPORT_C virtual void DeleteBifL(TUid aBioID);
	IMPORT_C virtual void ReloadBifL(TUid aBioID);

	//Returns the error returned by SetupL()
	//Leaves if:
	//	Setup() returns other than KErrNone, KErrNotFound, KErrAlreadyExists; or
	//	AppendL() leaves
	IMPORT_C TInt SetupAndAppendL(CBaseSmsActiveSocketWatcher& aSocketWatcher, CArrayPtrFlat<CBaseSmsActiveSocketWatcher>& rSocketWatchers);
	IMPORT_C void ResetDb();

	IMPORT_C void GetBioServiceId(CMsvSession& aSession, TMsvId& aBioServiceId, TMsvId& aSmsServiceId);
private:
	void CreateSocketWatchersFromBioDbL(TBioMsgIdType aType, CArrayPtrFlat<CBaseSmsActiveSocketWatcher>& rSocketWatchers);
	void CreateSocketWatchersFromBioDbL(TBioMsgIdType aType, TUid aBioID, CArrayPtrFlat<CBaseSmsActiveSocketWatcher>& rSocketWatchers);
	void StartSocketWatchersL(const CArrayPtrFlat<CBaseSmsActiveSocketWatcher>& aSocketWatchers);
	void ConstructDbL();
	
	//MMsvSessionObserver
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
protected:
	CArrayPtrFlat<CBaseSmsActiveSocketWatcher>* iSocketWatchers;

	RFs& iFs;
	CWatcherLog& iWatcherLog;

	CBifChangeObserver* iBifObserver;

	CBIODatabase* iBioDb;	//used during construction only
	RTimer iTimer;
	
	CMsvSession* iSession;
	
	// member variables used to pass settings
	TMsvId iBioServiceId;
	TMsvId iSmsServiceId;
	};

#endif
