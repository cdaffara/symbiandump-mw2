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
#include "msgservertestutils.h"
#include <miutset.h>

_LIT(KTestName,"Message Server Smoke test");
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
TInt CreateServiceTest(TTestSessionObserver &aObserver, CMsvEntry &aEntry, TMsvId &aServiceId)
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
		{
		err=aObserver.CheckEvents(1,MMsvSessionObserver::EMsvEntriesCreated);
		aServiceId = McliUtils::GetLocalProgressL(*anop).iId;
		}

	CleanupStack::PopAndDestroy(anop);
	return err ;
	}


// create a service
TInt CreateMessageTest(TTestSessionObserver &aObserver, CMsvEntry &aEntry, TMsvId aServiceId, TMsvId& aMessageId)
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
		{
		aMessageId = McliUtils::GetLocalProgressL(*anop).iId;
		err=aObserver.CheckEvents(1,MMsvSessionObserver::EMsvEntriesCreated);
		}
	if(err==KErrNone)
		err=eObs.CheckEvents(1,MMsvEntryObserver::EMsvNewChildren);

	CleanupStack::PopAndDestroy(anop);
	CleanupStack::PopAndDestroy(&clean);
	return(err);
	}


TInt CopyMessageTest(TTestSessionObserver &aObserver, CMsvEntry &aEntry, TMsvId aMessageId)
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
		err=aObserver.CheckEvents(1,MMsvSessionObserver::EMsvEntriesCreated);
	if(err==KErrNone)
		err=eObs.CheckEvents(0);

	CleanupStack::PopAndDestroy(anop);
	CleanupStack::PopAndDestroy(&clean);
	return err ;
	}

TInt MoveMessageTest(TTestSessionObserver &aObserver, CMsvEntry &aEntry, TMsvId aMessageId)
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

	if(err==KErrNone)
		err=aObserver.CheckEvents(1,MMsvSessionObserver::EMsvEntriesMoved);

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



TInt DoTestsL(TTestSessionObserver &aSessionObs, CMsvEntry &aEntry)
	{
	TInt testno=1;
	TInt err=KErrNone;

	TMsvId serviceId; 

	theUtils->TestStart(testno,_L("Creating a Service"));
	err=CreateServiceTest(aSessionObs,aEntry,serviceId);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) return(err);

	TMsvId messageId;
	theUtils->TestStart(testno,_L("Creating a Message"));
	err=CreateMessageTest(aSessionObs,aEntry,serviceId,messageId);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) return(err);

	theUtils->TestStart(testno,_L("Copying message to Outbox"));
	err=CopyMessageTest(aSessionObs,aEntry,messageId);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) return(err);

	theUtils->TestStart(testno,_L("Creating another Message"));
	err=CreateMessageTest(aSessionObs,aEntry,serviceId,messageId);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) return(err);

	theUtils->TestStart(testno,_L("Moving message to Outbox"));
	err=MoveMessageTest(aSessionObs,aEntry,messageId);
	theUtils->TestFinish(testno++,err);
	if(err!=KErrNone) return(err);


	return KErrNone;
	}




void DoMainL()
	{
	theUtils=CMsgServerTestUtils::NewL(tests);
	theUtils->SetupL();
	TTestSessionObserver sObs;
	CMsvSession *session=CMsvSession::OpenSyncL(sObs);
	CleanupStack::PushL(session);
	CMsvEntry *cEntry=session->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(cEntry);

	TInt err=DoTestsL(sObs,*cEntry);
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(session);
	theUtils->Close(err);
	delete theUtils;
	}

GLDEF_C TInt E32Main()
	{       
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TRAPD(ret, DoMainL());
	tests(ret==KErrNone);
    tests.Getch();
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



