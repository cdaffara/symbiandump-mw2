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
//

#include <e32base.h>
#include <msvapi.h>
#include <msvids.h>
#include <msvuids.h>
#include "msvutils.h"
#include "msvtestutilsbase.h"


_LIT(KMsvServerPattern, "!MsvServer*");
const TInt KMsvStoreInitLongDelayBeforeExit = 10 * 1000000;		// 10 secs

_LIT(KMsvStoreInitEntryDescription, "MessagingMailInitTestEntry");
_LIT(KMsvStoreInitEntryDetails, "Entry created by test MailInit");

CActiveScheduler* Scheduler;
CTrapCleanup* Cleanup;

class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};

void DeleteStoreInitFlagFileL()
	{
	}

LOCAL_C void doMainL()
	{
	_LIT(KMailInitExeInfoPrint, "Test MailInit exe has been run");
	User::InfoPrint(KMailInitExeInfoPrint);

	// Only create session if server started through correct mechanism
	// Test code breaks the rules - this will stop a crash
	TBool create = EFalse;

	TFullName name;
	TFindProcess find(KMsvServerPattern);
	if (find.Next(name) == KErrNone)
		create = ETrue;

	Scheduler = new(ELeave)CActiveScheduler;
	CleanupStack::PushL(Scheduler);
	CActiveScheduler::Install(Scheduler);

	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

/*   AVOID creating entry in mailinit.
	This is causing failure in certain unit testcase.
	
	if (create)
		{
		CTestActive* active = new (ELeave) CTestActive; 
		CleanupStack::PushL(active);

		CMsvEntry* msvEntry = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
		CleanupStack::PushL(msvEntry);
		msvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);

		TMsvEntry testEntry;
		testEntry.iType=KUidMsvMessageEntry;
		testEntry.iMtm = KUidMsvLocalServiceMtm;
		testEntry.iServiceId = KMsvLocalServiceIndexEntryId;
		testEntry.iDescription.Set(KMsvStoreInitEntryDescription);
		testEntry.iDetails.Set(KMsvStoreInitEntryDetails);
		CMsvOperation* opert = msvEntry->CreateL(testEntry, active->iStatus);
		active->StartL();
		CActiveScheduler::Start(); // operation complete
		delete opert; 

		CleanupStack::PopAndDestroy(msvEntry);
		CleanupStack::PopAndDestroy(active);
		}
*/
	CleanupStack::PopAndDestroy(session);
	CleanupStack::PopAndDestroy(Scheduler);

	// Long delay here before deleting flag file. This simulates the "real" behaviour.
	User::After(KMsvStoreInitLongDelayBeforeExit);
	// Delete flag file if present.
	DeleteStoreInitFlagFileL();
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	Cleanup = CTrapCleanup::New();
	__ASSERT_ALWAYS(Cleanup, User::Invariant());
	TRAPD(ret, doMainL());		
	__ASSERT_ALWAYS(!ret, User::Invariant());
	delete Cleanup;
	__UHEAP_MARKEND;
	return(KErrNone);
	}

EXPORT_C TInt WinsMain()
	{
	return KErrNone;
	}

