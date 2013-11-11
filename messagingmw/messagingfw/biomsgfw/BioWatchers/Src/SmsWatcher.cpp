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
// SmsWatcher.CPP
//

#include <e32std.h>
#include <biouids.h>
#include <c32comm.h>
#include <smut.h>

#include "SmsWatcher.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "cwatcher.h"
#endif

const TInt KAllBioUids = 0;

EXPORT_C CSmsBaseWatcher::CSmsBaseWatcher(RFs& aFs, CWatcherLog& aLog, TInt aPriority)
: CActive(aPriority), iFs(aFs), iWatcherLog(aLog)
	{
	CActiveScheduler::Add(this);
	}

//
// Construction/destruction

EXPORT_C void CSmsBaseWatcher::ConstructL()
	{
	// The session to the message server is created in the ConstructL because at this 
	// moment the message server still runs because of the open session in CWatcherLauncherArray
	// If we would open the session in StartL, the message server will be opened and closed  
	// several times, which has a negative effect on performance.
	iSession = CMsvSession::OpenSyncL(*this);
	
	User::LeaveIfError(iTimer.CreateLocal());
	ConstructDbL();
	CompleteMyself(KErrNone);
	}

EXPORT_C void CSmsBaseWatcher::StartL()
	{
	// put the real construction here
	iSocketWatchers = new (ELeave) CArrayPtrFlat<CBaseSmsActiveSocketWatcher>(10);

	// Hook into the Bif Change Observer
	iBifObserver = CBifChangeObserver::NewL(*this, iFs);
	iBifObserver->Start();

	}

//
// Construction/destruction
EXPORT_C CSmsBaseWatcher::~CSmsBaseWatcher()
	{
	Cancel();
	Reset();
	ResetDb();
	iTimer.Close();
	
	delete iSession;
	}

EXPORT_C void CSmsBaseWatcher::Reset()
	{
	__ASSERT_DEBUG(!IsActive(), PanicWatcher(ESocketWatcherAlreadyActive));

	if (iSocketWatchers)
		iSocketWatchers->ResetAndDestroy();

	delete iSocketWatchers;
	iSocketWatchers = NULL;

	delete iBifObserver;
	iBifObserver = NULL;

	//Do not close iTimer. Leave that for the destructor.
	}

//
EXPORT_C void CSmsBaseWatcher::HandleBifChangeL(TBifChangeEvent aEvent, TUid aBioID)
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L8("Bio: Watcher HandleBifChangeL(Event: %d, BioUid: %d)"), aEvent, aBioID.iUid));
	TInt err = KErrNone;

	switch(aEvent)
		{
		case EBifAdded:
			TRAP(err, AddBifL(aBioID));
			break;
		case EBifDeleted:
			TRAP(err, DeleteBifL(aBioID));
			break;
		case EBifChanged:
			TRAP(err, ReloadBifL(aBioID));
			break;
		default:
			break;
		} 

	if (err)
		{
		BIOWATCHERLOG(iWatcherLog.Printf(_L8("Bio: Watcher HandleBifChangeL(Event: %d, BioUid: %d) error: %d"), aEvent, aBioID.iUid, err));
		}
	}

EXPORT_C void CSmsBaseWatcher::RunL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L8("Bio: Attempting restart [iStatus=%d]"), iStatus.Int()));

	//Restart the watcher
	Reset();
	StartL();
	}

EXPORT_C void CSmsBaseWatcher::DoCancel()
	{
	iTimer.Cancel();
	}

EXPORT_C void CSmsBaseWatcher::HandleWatcherComplete(CBaseSmsActiveSocketWatcher& aWatcher, TInt aError)
	{
	//Only try to re-start the watcher if it wasn't cancelled.
	if (!iSocketWatchers || aError == KErrCancel)
		return;

	TInt count = iSocketWatchers->Count();
	TInt err = KErrNone;
	
	while (count--)
		{
		CBaseSmsActiveSocketWatcher* watcher = iSocketWatchers->At(count);
		
		if (watcher == &aWatcher)
			{
			//Found the same watcher
			TRAP(err, watcher->SetupL());
				
			if (!err)
				{
				TRAP(err, watcher->StartL());
				}
				
			if (err)
				{
				iSocketWatchers->Delete(count);
				delete watcher;
				}
			
			break;
			}
		}
	
	if (err)
		{
		//This will call RunL, which will restart the watcher.
		BIOWATCHERLOG(iWatcherLog.Printf(_L8("Bio: Unable to restart watcher [err=%d, IsActive=%d]"), err, IsActive()));
		Cancel();
		iTimer.After(iStatus, KWatcherDelay);
		SetActive();
		}
	}

EXPORT_C void CSmsBaseWatcher::DeleteSocketWatchersWithUidL(TUid aUid)
	{
	TInt count = iSocketWatchers->Count();

	while (count--)
		{
		CBaseSmsActiveSocketWatcher* socketWatcher = iSocketWatchers->At(count);
		
		if (socketWatcher->BioMsgUID() == aUid)
			{
			socketWatcher->Cancel();
			delete socketWatcher;
			iSocketWatchers->Delete(count);
			}
		}
	}

EXPORT_C void CSmsBaseWatcher::CreateSocketWatchersFromBioDbL(TBioMsgIdType aType)
	{
	CreateSocketWatchersFromBioDbL(aType, *iSocketWatchers);
	}

void CSmsBaseWatcher::CreateSocketWatchersFromBioDbL(TBioMsgIdType aType, CArrayPtrFlat<CBaseSmsActiveSocketWatcher>& rSocketWatchers)
	{
	TUid uid;
	uid.iUid = KAllBioUids;
	CreateSocketWatchersFromBioDbL(aType, uid, rSocketWatchers);
	}

EXPORT_C void CSmsBaseWatcher::CreateSocketWatchersFromBioDbL(TBioMsgIdType aType, TUid aBioID)
	{
	CreateSocketWatchersFromBioDbL(aType, aBioID, *iSocketWatchers);
	}

void CSmsBaseWatcher::CreateSocketWatchersFromBioDbL(TBioMsgIdType aType, TUid aUid, CArrayPtrFlat<CBaseSmsActiveSocketWatcher>& rSocketWatchers)
	{
	ConstructDbL();

	TInt pos;
	TUid bioMsgID(KNullUid);
	CBaseSmsActiveSocketWatcher* socketWatcher = NULL;

	const CArrayFix<TBioMsgId>* bioIDs = iBioDb->BioEntryByTypeLC(CBIODatabase::EStart, aType, pos);

	if (bioIDs)
		iBioDb->GetBioMsgID(pos, bioMsgID);

	TInt bioDBCount = iBioDb->BIOCount();
	while( bioIDs != 0 || pos < bioDBCount )
		{
		if (aUid.iUid == KAllBioUids || bioMsgID == aUid)
			{
			TInt index = 0;
			if (bioIDs)
				{
				index = bioIDs->Count();
				}

			while (index--)
				{
				// Create the Socket Watcher and Add it to the list
				TBioMsgId msgId = bioIDs->At(index);

				if (msgId.iType == aType && SupportBioMsgId(msgId))
					{
					socketWatcher = CreateSocketWatcherLC(bioMsgID, msgId);
					TInt error = SetupAndAppendL(*socketWatcher, rSocketWatchers);

					if (error)
						CleanupStack::PopAndDestroy();
					else
						CleanupStack::Pop();
					}
				}
			}
		
		CleanupStack::PopAndDestroy();	// bioIDs
		bioIDs = NULL;
		
		bioIDs = iBioDb->BioEntryByTypeLC(CBIODatabase::ENext, aType, pos);
		if (bioIDs)
			iBioDb->GetBioMsgID(pos, bioMsgID);
		}
	}

EXPORT_C void CSmsBaseWatcher::StartSocketWatchersL()
	{
	StartSocketWatchersL(*iSocketWatchers);
	}

void CSmsBaseWatcher::StartSocketWatchersL(const CArrayPtrFlat<CBaseSmsActiveSocketWatcher>& aSocketWatchers)
	{
	TInt count = aSocketWatchers.Count();

	//Order Important
	for (TInt i = 0; i < count; i++)
		{
		CBaseSmsActiveSocketWatcher* socketWatcher = aSocketWatchers[i];
		socketWatcher->SetObserver(this);
		socketWatcher->StartL();
		}
	}

EXPORT_C void CSmsBaseWatcher::DeleteBifL(TUid aBioID)
	{
	DeleteSocketWatchersWithUidL(aBioID);
	}

EXPORT_C void CSmsBaseWatcher::ReloadBifL(TUid aBioID)
	{
	DeleteBifL(aBioID);
	AddBifL(aBioID);
	}

EXPORT_C void CSmsBaseWatcher::AddBifWithTypeL(TBioMsgIdType aType, TUid aBioID)
	{
	CArrayPtrFlat<CBaseSmsActiveSocketWatcher>* socketWatchers = new (ELeave) CArrayPtrFlat<CBaseSmsActiveSocketWatcher>(1);
	CleanupStack::PushL(socketWatchers);

	CreateSocketWatchersFromBioDbL(aType, aBioID, *socketWatchers);
	StartSocketWatchersL(*socketWatchers);

	TInt count = socketWatchers->Count();
	while (count--)
		{
		iSocketWatchers->AppendL(socketWatchers->At(count));
		}

	CleanupStack::PopAndDestroy(); //socketWatchers
	}

EXPORT_C TInt CSmsBaseWatcher::SetupAndAppendL(CBaseSmsActiveSocketWatcher& aSocketWatcher, CArrayPtrFlat<CBaseSmsActiveSocketWatcher>& rSocketWatchers)
	{
	TRAPD(error, aSocketWatcher.SetupL());

	if (!error)
		{
		rSocketWatchers.AppendL(&aSocketWatcher);
		}
	else 
		{
		BIOWATCHERLOG(iWatcherLog.Printf(_L8("Bio: Watcher setup error - %d"), error));

		if (error != KErrAlreadyExists && error != KErrNotFound)
			User::Leave(error);
		else
			BIOWATCHERLOG(iWatcherLog.Printf(_L8("Bio: Ignoring watcher error - Config problem?")));
		}

	return error;
	}

void CSmsBaseWatcher::ConstructDbL()
	{
	if (iBioDb == NULL)
		iBioDb = CBIODatabase::NewL(iFs);
	}

EXPORT_C void CSmsBaseWatcher::ResetDb()
	{
	delete iBioDb;
	iBioDb = NULL;
	}

EXPORT_C void CSmsBaseWatcher::CompleteMyself(TInt aError)
	{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, aError);
	SetActive();
	}

EXPORT_C TInt CSmsBaseWatcher::RunError(TInt aError)
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L8("Bio: RunL left with %d. Restarting in %d sec"), aError, KWatcherDelay / 1000000));

	iTimer.After(iStatus, KWatcherDelay);
	SetActive();

	return KErrNone;
	}

EXPORT_C void CSmsBaseWatcher::GetBioServiceId(CMsvSession& aSession, TMsvId& aBioServiceId, TMsvId& aSmsServiceId)
	{
	TInt err = KErrNone;
	TRAP(err, TSmsUtilities::ServiceIdL(aSession, aBioServiceId, KUidBIOMessageTypeMtm));
	if (err == KErrNotFound)
		{
		aBioServiceId = KMsvLocalServiceIndexEntryId;
		}
	TRAP(err, TSmsUtilities::ServiceIdL(aSession, aSmsServiceId));
	if (err == KErrNotFound)
		{
		aSmsServiceId = KMsvLocalServiceIndexEntryId;
		}
	}


