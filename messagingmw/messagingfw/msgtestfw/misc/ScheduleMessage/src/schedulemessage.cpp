// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "pigeonservermtm.h"
#include <msvapi.h>
#include <msvids.h>
#include <msvuids.h>
#include <msvschedulepackage.h>
#include <msvschedulesend.h>
#include <msvschedulesettings.h>


class CSessionObserver : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};

LOCAL_C TBool FindOrCreateScheduleL(RScheduler& aScheduler, const TTime& aTime, const CMsvScheduleSettings& aSettings, TSchedulerItemRef& aRef)
	{
	TRAPD(err, CMsvScheduleSend::FindScheduleL(aScheduler, aTime, aRef));

	if( err != KErrNone )
		CMsvScheduleSend::CreateScheduleL(aScheduler, aSettings, aTime, aSettings.ValidityPeriod(), aRef);

	return (err == KErrNone);
	}
	
LOCAL_C void DoExecuteL()
	{
	CActiveScheduler* activeScheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(activeScheduler);
	CActiveScheduler::Install(activeScheduler);
	
	// Create Session
	CSessionObserver* sessionObserver;
	sessionObserver = new (ELeave) CSessionObserver;
	CleanupStack::PushL(sessionObserver);
		
	CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
	CleanupStack::PushL(session);
	
	// Set the message entry to global Outbox folder
	CMsvEntry* msvEntry = session->GetEntryL(KMsvGlobalOutBoxIndexEntryId);
	CleanupStack::PushL(msvEntry);
	
	// Get the selection of Pigeon messages present in Outbox
	CMsvEntrySelection* selection = msvEntry->ChildrenWithMtmL(KUidMsgTypePigeon);
	CleanupStack::PushL(selection);
	
	// Schedule it to send immediately - use default settings.
	CMsvScheduleSettings* settings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(settings);

	RScheduler scheduler;
	TSchedulerItemRef ref;
	
	CMsvScheduleSend::ConnectAndRegisterL(scheduler, *settings);
	CleanupClosePushL(scheduler);
	
	TInt selCount = selection->Count();
	
	for( TInt index = 0; index < selCount; ++index )
		{
		msvEntry->SetEntryL(selection->At(index));
			
		if(msvEntry != NULL)
			{
			TMsvEntry entry = msvEntry->Entry();
					
			ref.iHandle = KErrNotFound;
			
			FindOrCreateScheduleL(scheduler, entry.iDate, *settings, ref);

			User::LeaveIfError(scheduler.DisableSchedule(ref.iHandle));
		
			TMsvSchedulePackage package;
			package.iPollProgress = 3000000;
			package.iCommandId = ESendScheduledL;
			package.iId = selection->At(index);
				
			TTaskInfo info;

			CMsvScheduleSend::ScheduleEntryL(scheduler, ref, package, info);
		
			User::LeaveIfError(scheduler.EnableSchedule(ref.iHandle));
			}
		}
	CleanupStack::PopAndDestroy(7, activeScheduler);
	}
	
LOCAL_C TInt Execute()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	TRAPD(err, DoExecuteL());
	
	delete cleanup;
	__UHEAP_MARKEND;

	return err;
	}
	
GLDEF_C TInt E32Main()
	{
	return Execute();
	}

