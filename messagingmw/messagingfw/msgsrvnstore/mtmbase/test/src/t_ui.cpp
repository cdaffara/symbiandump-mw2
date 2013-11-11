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
// T_EDITOR.CPP
// 
//

#include <e32std.h>
#include <f32file.h>
#include <e32test.h>
#include <msvapi.h>
#include <msvuids.h>
#include <msvids.h>
#include <mtclreg.h>
#include <mtuireg.h>
#include <mtclbase.h>
#include <mtmuibas.h>
#include <mturutils.h>
#include <mtudcbas.h>
#include <mtudreg.h>

RTest test(_L("Mtm Ui Test Harness"));
RFs theFs;

const TUid KUidMsvPopMtm = { 0x10001029 };
const TUid KUidMsvImapMtm = { 0x1000102a };
const TUid KUidMsvSmtpMtm = { 0x10001028 };
const TUid KUidMsvFaxMtm = { 0x1000102b };
const TUid KUidMsvSmsMtm = { 0x1000102c };

//

class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) { };
	};

//

LOCAL_C void TestUiDataL(CMsvSession& aSession, const TMsvEntry& aEntry)
	{
	CMtmUiDataRegistry* reg = CMtmUiDataRegistry::NewL(aSession);
	CleanupStack::PushL(reg);

	CBaseMtmUiData* uiData = reg->NewMtmUiDataLayerL(aEntry.iMtm);
	test(uiData != NULL);
	delete uiData;

	CleanupStack::PopAndDestroy(); // reg
	}

LOCAL_C void DoTestsL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	test.Start(_L("POP"));
	entry.iMtm = KUidMsvPopMtm;
	cEntry->CreateL(entry);
	MturUtils::LaunchEditorL(entry.Id());
	TestUiDataL(*session, entry);

	test.Printf(_L("Press a key to continue\n"));
	test.Getch();

	test.Next(_L("IMAP"));
	entry.iMtm = KUidMsvImapMtm;
	cEntry->CreateL(entry);
	MturUtils::LaunchEditorL(entry.Id());
	TestUiDataL(*session, entry);

	test.Printf(_L("Press a key to continue\n"));
	test.Getch();

	test.Next(_L("SMTP"));
	entry.iMtm = KUidMsvSmtpMtm;
	cEntry->CreateL(entry);
	MturUtils::LaunchEditorL(entry.Id());
	TestUiDataL(*session, entry);

	test.Printf(_L("Press a key to continue\n"));
	test.Getch();

	test.Next(_L("FAX"));
	entry.iMtm = KUidMsvFaxMtm;
	cEntry->CreateL(entry);
	MturUtils::LaunchEditorL(entry.Id());
	TestUiDataL(*session, entry);

	test.Printf(_L("Press a key to continue\n"));
	test.Getch();

	test.Next(_L("SMS"));
	entry.iMtm = KUidMsvSmsMtm;
	cEntry->CreateL(entry);
	MturUtils::LaunchEditorL(entry.Id());
	TestUiDataL(*session, entry);

	CleanupStack::PopAndDestroy(3); // cEntry, session, ob
	}

LOCAL_C void doMainL()
	{
	CActiveScheduler::Install(new(ELeave)CActiveScheduler);
	TRAPD(error, DoTestsL());
	delete CActiveScheduler::Current(); 
	User::LeaveIfError(error);
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	test(cleanup != NULL);
	TRAPD(error, doMainL());
	test(error == KErrNone);
	delete cleanup;
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return KErrNone;
	}

EXPORT_C TInt WinsMain()
	{
#if defined(__WINS__)
	E32Main();
#endif
	return KErrNone;
	}

