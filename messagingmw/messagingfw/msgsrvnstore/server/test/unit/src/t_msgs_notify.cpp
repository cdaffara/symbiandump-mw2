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

#include <e32test.h>
#include <msvapi.h>
#include <msvids.h>
#include "msvtestutils.h"
#include "msvtestconfig.h"
#include <miutset.h>
#include "msgservertestutils.h"

_LIT(KTestName,"Message Server Notify test");
RTest tests(KTestName);
CMsgServerTestUtils* theUtils = NULL;


class TTestSessionObserver: public MMsvSessionObserver
	{
public:
	TTestSessionObserver() : iNumberOfEvents(0) {};
	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
		{
		iLastEvent=aEvent;
		iNumberOfEvents++;
		};

	TInt CheckEvents(TInt aNumber)
		{
		_LIT(KNote3,"Checking session observer got the correct number of events\n");
		theUtils->Printf(KNote3);
		TInt err=KErrNone;
		if(iNumberOfEvents!=aNumber)
			{
			_LIT(KErrMsg,"Wrong Number of Events, Wanted %d Got %d, last event %d\n");
			theUtils->Printf(KErrMsg,aNumber,iNumberOfEvents,iLastEvent);
			err=KErrUnknown;
			}
		iNumberOfEvents=0;
		return err;
		};

	TInt CheckEvents(TInt aNumber, TMsvSessionEvent aLastEvent)
		{
		_LIT(KNote3,"Checking session observer got the correct event\n");
		theUtils->Printf(KNote3);

		TInt err=KErrNone;
		if(aNumber !=0 && iLastEvent!=aLastEvent)
			{
			_LIT(KErrMsg,"Wrong Event Wanted %d Got %d\n");		
			theUtils->Printf(KErrMsg,aLastEvent,iLastEvent);
			err=KErrUnknown;
			}
		if(iNumberOfEvents!=aNumber)
			{
			_LIT(KErrMsg,"Wrong Number of Events Wanted %d Got %d\n");		
			theUtils->Printf(KErrMsg,aNumber,iNumberOfEvents);
			err=KErrUnknown;
			}
		iNumberOfEvents=0;
		return err;
		};
	
private:

	TMsvSessionEvent iLastEvent;
	TInt iNumberOfEvents;
	};	


class TTestEntryObserver: public MMsvEntryObserver
	{
public:
	TTestEntryObserver() : iNumberOfEvents(0) {};
	virtual void HandleEntryEventL(TMsvEntryEvent aEvent, TAny*, TAny*, TAny*)
		{
		iLastEvent=aEvent;
		iNumberOfEvents++;
		}


	TInt CheckEvents(TInt aNumber)
		{
		_LIT(KNote3,"Checking entry observer got the correct number of events\n");
		theUtils->Printf(KNote3);
		TInt err=KErrNone;
		if(iNumberOfEvents!=aNumber)
			{
			_LIT(KErrMsg,"Wrong Number of Events, Wanted %d Got %d, last event %d\n");
			theUtils->Printf(KErrMsg,aNumber,iNumberOfEvents,iLastEvent);
			err=KErrUnknown;
			}
		iNumberOfEvents=0;
		return err;
		};

	TInt CheckEvents(TInt aNumber, TMsvEntryEvent aLastEvent)
		{
		_LIT(KNote3,"Checking entry observer got the correct event\n");
		theUtils->Printf(KNote3);

		TInt err=KErrNone;
		if(iNumberOfEvents!=aNumber)
			{
			_LIT(KErrMsg,"Wrong Number of Events, Wanted %d Got %d\n");
			theUtils->Printf(KErrMsg,aNumber,iNumberOfEvents);
			err=KErrUnknown;
			}
		if(iLastEvent!=aLastEvent)
			{
			_LIT(KErrMsg,"Wrong Entry Event, Wanted %d Got %d\n");		
			theUtils->Printf(KErrMsg,aLastEvent,iLastEvent);
			err=KErrUnknown;
			}
		iNumberOfEvents=0;
		return err;
		};


private:
	TMsvEntryEvent iLastEvent;
	TInt iNumberOfEvents;
	};



class TEntryObserverCleanup
	{
	public:
		TEntryObserverCleanup(CMsvEntry &aEntry,TTestEntryObserver &aObserver)
			: iEntry(aEntry), iObserver(aObserver) {};
	static void CleanupEntryObserver(TAny* aPtr)
		{
		TEntryObserverCleanup* me = STATIC_CAST(TEntryObserverCleanup*, aPtr);
		me->Cleanup();
		};
	private:
		void Cleanup()
		{
		iEntry.RemoveObserver(iObserver);
		};

	private:
		CMsvEntry &iEntry;
		TTestEntryObserver &iObserver;
	};


TInt CheckEvents(TTestSessionObserver &aObserver,TInt aN1,
				 TTestSessionObserver &aObserver2,TInt aN2,
				 TTestSessionObserver &aObserver3,TInt aN3,
				 TTestSessionObserver &aObserver4,TInt aN4)
	{
	CTestTimer *timer=CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

	TInt err=aObserver.CheckEvents(aN1);
	if(err==KErrNone)
		err=aObserver2.CheckEvents(aN2);
	if(err==KErrNone)
		err=aObserver3.CheckEvents(aN3);
	if(err==KErrNone)
		err=aObserver4.CheckEvents(aN4);
	return(err);
	}



TInt RunWhilePending(const TRequestStatus &aStatus)
	{
	_LIT(KNote2,"Running scheduler till we complete\n");
	theUtils->Printf(KNote2);

	TInt err=KErrNone;
	while(aStatus==KRequestPending)
		{
		CActiveScheduler::Current()->RunIfReady(err,0);
		CActiveScheduler::Current()->WaitForAnyRequest();
		}

	_LIT(KNote3,"Completed\n");
	theUtils->Printf(KNote3);
	return err;
	}


// create a service
TInt CreateServiceTest(CMsvEntry &aEntry, TMsvId &aServiceId)
	{

	TMsvEntry tEntry;

	tEntry.iMtm = KUidMsgTypePOP3;
	tEntry.iType = KUidMsvServiceEntry;
	tEntry.SetReadOnly(EFalse);
	tEntry.SetVisible(ETrue);
	_LIT(KADescription,"A Fake Service");
	tEntry.iDescription.Set(KADescription);

	_LIT(KNote0,"Setting entry to root\n");
	theUtils->Printf(KNote0);
	aEntry.SetEntryL(KMsvRootIndexEntryIdValue);

	_LIT(KNote1,"Starting async create service\n");
	theUtils->Printf(KNote1);
	TRequestStatus status;
	CMsvOperation *anop=aEntry.CreateL(tEntry,status);
	CleanupStack::PushL(anop);

	TInt err=RunWhilePending(status);
	if(err==KErrNone)
		aServiceId = McliUtils::GetLocalProgressL(*anop).iId;


	CleanupStack::PopAndDestroy(anop);
	return err ;
	}


// create a service
TInt CreateMessageTest(CMsvEntry &aEntry, TMsvId aServiceId, TMsvId& aMessageId)
	{
	_LIT(KNote0,"Setting entry to drafts entry\n");
	theUtils->Printf(KNote0);
	aEntry.SetEntryL(KMsvDraftEntryIdValue);
	_LIT(KNote1,"Observing draft entry\n");
	theUtils->Printf(KNote1);
	TTestEntryObserver eObs;
	aEntry.AddObserverL(eObs);
	TEntryObserverCleanup clean(aEntry,eObs);
	CleanupStack::PushL(TCleanupItem(TEntryObserverCleanup::CleanupEntryObserver, &clean));


	_LIT(KAnAddress,"Test@psion7.demon.co.uk");
	_LIT(KADescription,"A Message");

	TMsvEntry message;
	message.iDetails.Set(KAnAddress);
	message.iDescription.Set(KADescription);
	message.iMtm=KUidMsgTypePOP3;
	
	message.iServiceId=aServiceId;
	message.iType=KUidMsvMessageEntry;

	_LIT(KNote2,"Starting async create message\n");
	theUtils->Printf(KNote2);

	TRequestStatus status;
	CMsvOperation *anop=aEntry.CreateL(message,status);
	CleanupStack::PushL(anop);

	TInt err=RunWhilePending(status);

	if(err==KErrNone)
		aMessageId = McliUtils::GetLocalProgressL(*anop).iId;
	if(err==KErrNone)
		err=eObs.CheckEvents(1,MMsvEntryObserver::EMsvNewChildren);

	CleanupStack::PopAndDestroy(anop);
	CleanupStack::PopAndDestroy(&clean);
	return(err);
	}


TInt CopyMessageTest(CMsvEntry &aEntry, TMsvId aMessageId)
	{
	_LIT(KNote0,"Setting entry to drafts\n");
	theUtils->Printf(KNote0);
	aEntry.SetEntryL(KMsvDraftEntryId);
	_LIT(KNote1,"Observing drafts entry\n");
	theUtils->Printf(KNote1);
	TTestEntryObserver eObs;
	aEntry.AddObserverL(eObs);
	TEntryObserverCleanup clean(aEntry,eObs);
	CleanupStack::PushL(TCleanupItem(TEntryObserverCleanup::CleanupEntryObserver, &clean));


	_LIT(KNote3,"Starting async copy message on draft entry/aMessageId to Outbox\n");
	theUtils->Printf(KNote3);

	TRequestStatus status;
	CMsvOperation *anop=aEntry.CopyL(aMessageId,KMsvGlobalOutBoxIndexEntryId,status);
	CleanupStack::PushL(anop);

	TInt err=RunWhilePending(status);

	if(err==KErrNone)
		err=eObs.CheckEvents(0);

	CleanupStack::PopAndDestroy(anop);
	CleanupStack::PopAndDestroy(&clean);
	return err ;
	}

TInt MoveMessageTest(CMsvEntry &aEntry, TMsvId aMessageId)
	{
	_LIT(KNote0,"Setting entry to drafts\n");
	theUtils->Printf(KNote0);
	aEntry.SetEntryL(KMsvDraftEntryId);
	_LIT(KNote1,"Observing drafts entry\n");
	theUtils->Printf(KNote1);
	TTestEntryObserver eObs;
	aEntry.AddObserverL(eObs);
	TEntryObserverCleanup clean(aEntry,eObs);
	CleanupStack::PushL(TCleanupItem(TEntryObserverCleanup::CleanupEntryObserver, &clean));


	_LIT(KNote2,"Getting outbox entry\n");
	theUtils->Printf(KNote2);
	CMsvEntry* outboxEntry=aEntry.Session().GetEntryL(KMsvGlobalOutBoxIndexEntryId);
	CleanupStack::PushL(outboxEntry);
	_LIT(KNote3,"Observing outbox entry\n");
	theUtils->Printf(KNote3);
	TTestEntryObserver outboxObs;
	outboxEntry->AddObserverL(outboxObs);
	TEntryObserverCleanup clean2(*outboxEntry,outboxObs);
	CleanupStack::PushL(TCleanupItem(TEntryObserverCleanup::CleanupEntryObserver, &clean2));

	_LIT(KNote5,"Starting async move message on draft entry/aMessageId to Outbox\n");
	theUtils->Printf(KNote5);

	TRequestStatus status;
	CMsvOperation *anOp=aEntry.MoveL(aMessageId,KMsvGlobalOutBoxIndexEntryId,status);
	CleanupStack::PushL(anOp);

	TInt err=RunWhilePending(status);

	_LIT(KNote6,"Checking drafts entry events\n");
	theUtils->Printf(KNote6);
	if(err==KErrNone)
		err=eObs.CheckEvents(1,MMsvEntryObserver::EMsvDeletedChildren);

	_LIT(KNote7,"Checking outbox entry events\n");
	theUtils->Printf(KNote7);
	if(err==KErrNone)
		err=outboxObs.CheckEvents(1,MMsvEntryObserver::EMsvNewChildren);

	CleanupStack::PopAndDestroy(anOp);
	CleanupStack::PopAndDestroy(&clean2);
	CleanupStack::PopAndDestroy(outboxEntry);
	CleanupStack::PopAndDestroy(&clean);
	return err ;
	}



TInt DoTestsL()
	{
	TInt testno=1;
	TInt err=KErrNone;
	TInt allerrors=KErrNone;

	TTestSessionObserver sObs;
	TTestSessionObserver sObs2;
	TTestSessionObserver sObs3;
	TTestSessionObserver sObs4;

	CMsvSession *session=CMsvSession::OpenSyncL(sObs);
	CleanupStack::PushL(session);
	CMsvEntry *cEntry=session->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(cEntry);

	CMsvSession *session2=CMsvSession::OpenSyncL(sObs2);
	User::LeaveIfError(session2->SetReceiveEntryEvents(EFalse));
	CleanupStack::PushL(session2);

	CMsvSession *session3=CMsvSession::OpenSyncL(sObs3);
	User::LeaveIfError(session3->SetReceiveEntryEvents(EFalse));
	CleanupStack::PushL(session3);
	CMsvEntry *cEntry3=session3->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(cEntry3);

	CMsvSession *session4=CMsvSession::OpenSyncL(sObs4);
	User::LeaveIfError(session4->SetReceiveEntryEvents(EFalse));
	CleanupStack::PushL(session4);

//	CheckEvents(sObs,0,sObs2,0,sObs3,0,sObs4,0);
	CheckEvents(sObs,1,sObs2,0,sObs3,1,sObs4,0); // 1 and 3 can't have 0 events as mailinit creating a test entry
												 // generates an event too.

	// we should get events for session1 and session3,
	// 3 because it has a CMsvEntry, 1 because it hasn't turned off events
	TMsvId serviceId; 
	theUtils->TestStart(testno,_L("Creating a Service"));
	err=CreateServiceTest(*cEntry,serviceId);
	if(err==KErrNone) err=CheckEvents(sObs,1,sObs2,0,sObs3,1,sObs4,0);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) allerrors=err;

	// we should get events for session1 and session3,
	// 3 because it has a CMsvEntry, 1 because it hasn't turned off events
	TMsvId messageId;
	theUtils->TestStart(testno,_L("Creating a Message"));
	err=CreateMessageTest(*cEntry,serviceId,messageId);
	if(err==KErrNone) err=CheckEvents(sObs,1,sObs2,0,sObs3,1,sObs4,0);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) allerrors=err;

	CMsvEntry *cEntry4=session4->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(cEntry4);

	// we should get events for session1, session3, and session4
	// 3 because it has a CMsvEntry, 1 because it hasn't turned off events
	// session 4 should now get events because we created a entry on it.
	theUtils->TestStart(testno,_L("Copying message to Outbox"));
	err=CopyMessageTest(*cEntry,messageId);
	if(err==KErrNone) err=CheckEvents(sObs,1,sObs2,0,sObs3,1,sObs4,1);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) allerrors=err;

	User::LeaveIfError(session2->SetReceiveEntryEvents(ETrue));
	User::LeaveIfError(session4->SetReceiveEntryEvents(EFalse));

	// now session two should get events because we turned them on
	// session 4 should still get events because although we turned them off we have an entry
	theUtils->TestStart(testno,_L("Copying message to Outbox"));
	err=CopyMessageTest(*cEntry,messageId);
	if(err==KErrNone) err=CheckEvents(sObs,1,sObs2,1,sObs3,1,sObs4,1);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) allerrors=err;

	User::LeaveIfError(session4->SetReceiveEntryEvents(ETrue));

	// everyone should get events
	theUtils->TestStart(testno,_L("Creating another Message"));
	err=CreateMessageTest(*cEntry,serviceId,messageId);
	if(err==KErrNone) err=CheckEvents(sObs,1,sObs2,1,sObs3,1,sObs4,1);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) allerrors=err;

	CleanupStack::PopAndDestroy(cEntry4);

	// although we have destroyed the enrty on session 4 we also turned
	// events on so it should still get an event.
	theUtils->TestStart(testno,_L("Moving message to Outbox"));
	err=MoveMessageTest(*cEntry,messageId);
	if(err==KErrNone) err=CheckEvents(sObs,1,sObs2,1,sObs3,1,sObs4,1);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) allerrors=err;


	User::LeaveIfError(session2->SetReceiveEntryEvents(EFalse));
	User::LeaveIfError(session4->SetReceiveEntryEvents(EFalse));

	// everyone should get a close server event even though 2 and 4 have
	// events truned off
	theUtils->TestStart(testno,_L("Closing server"));
	session->CloseMessageServer();
	if(err==KErrNone) err=CheckEvents(sObs,1,sObs2,1,sObs3,1,sObs4,1);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) allerrors=err;

	CleanupStack::PopAndDestroy(session4);
	CleanupStack::PopAndDestroy(cEntry3);
	CleanupStack::PopAndDestroy(session3);
	CleanupStack::PopAndDestroy(session2);
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(session);

	return allerrors;
	}




void DoMainL()
	{
	theUtils=CMsgServerTestUtils::NewL(tests);
	theUtils->SetupL();

	TInt err=DoTestsL();
	theUtils->Close(err);
	delete theUtils;
	}

GLDEF_C TInt E32Main()
	{       
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TRAPD(ret, DoMainL());
	tests(ret==KErrNone);
	tests.Close();
	delete cleanup;      
	__UHEAP_MARKEND;
	return(KErrNone);
	}

EXPORT_C TInt WinsMain()
	{
#if defined(__WINS__)
	E32Main();
#endif
	return KErrNone;
	}



