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

#include "t_schedulesend.h"	

#include <e32test.h>
#include <msvuids.h>
#include "pigeonservermtm.h"
#include <msvsysagentaction.h>
#include <msvschedulesettings.h>
#include <tmsvschedulesettingsutils.h>
#include <centralrepository.h>

#include "t_schsendshare.h"

#include <e32property.h>
TBool definedPhonePwrValue = EFalse;
TBool definedSIMStatusValue = EFalse;
TBool definedNetworkStatusValue = EFalse;

_LIT(KPigeonDataComponentFileName, "z:\\resource\\messaging\\mtm\\pigeon.rsc");
_LIT(KFailed,"Failed to complete tests.");
_LIT(KScheduleSendTestPanic, "SchSendTst");

_LIT(KTestScheduleSendScheduleMultiple,			"Schedule multiple messages");
_LIT(KTestScheduleSendReScheduleTime_1,			"ReSechedule multiple messages, for same time");
_LIT(KTestScheduleSendReScheduleTime_2,			"ReSechedule multiple messages, for differnt times");
_LIT(KTestScheduleSendReScheduleConditions_1,	"ReSechedule multiple messages, for conditions, no timeout - conditions met");
_LIT(KTestScheduleSendReScheduleConditions_2,	"ReSechedule multiple messages, for conditions, with timeout - conditions met");
_LIT(KTestScheduleSendReScheduleConditions_3,	"ReSechedule multiple messages, for conditions, with timeout - timeout will occur");
_LIT(KTestScheduleSendReScheduleMixed_1,		"ReSechedule multiple messages, for time and conditions, with timeout - conditions met");
_LIT(KTestScheduleSendFailed_1,					"Fail multiple messages");
_LIT(KTestScheduleSendFailed_2,					"Fail multiple messages after second re-schedule");

_LIT(KStateUnknown,			"KMsvSendStateUnknown");
_LIT(KStateUponRequest, 	"KMsvSendStateUponRequest");
_LIT(KStateWaiting,			"KMsvSendStateWaiting");
_LIT(KStateSending,			"KMsvSendStateSending");
_LIT(KStateScheduled,		"KMsvSendStateScheduled");
_LIT(KStateResend,			"KMsvSendStateResend");
_LIT(KStateSuspended,		"KMsvSendStateSuspended");
_LIT(KStateFailed,			"KMsvSendStateFailed");
_LIT(KStateSent,			"KMsvSendStateSent");
_LIT(KStateNotApplicable,	"KMsvSendStateNotApplicable");
_LIT(KBoolTrue, 			"ETrue");
_LIT(KBoolFalse, 			"EFalse");

RTest test(_L("T_ScheduleSend Test Harness"));

LOCAL_C void CreateVariablesL()
	{
	TInt testState;
	if(KErrNone != RProperty::Get(KUidSystemCategory, KMsvTestUidPhonePwrValue, testState))
		{
		User::LeaveIfError(RProperty::Define(KUidSystemCategory, KMsvTestUidPhonePwrValue, RProperty::EInt));	// Qualified: No leavescan error
		definedPhonePwrValue = ETrue;
		}
	if(KErrNone != RProperty::Get(KUidSystemCategory, KMsvTestUidSIMStatusValue, testState))
		{
		User::LeaveIfError(RProperty::Define(KUidSystemCategory, KMsvTestUidSIMStatusValue, RProperty::EInt));	// Qualified: No leavescan error
		definedSIMStatusValue = ETrue;
		}
	if(KErrNone != RProperty::Get(KUidSystemCategory, KMsvTestUidNetworkStatusValue, testState))
		{
		User::LeaveIfError(RProperty::Define(KUidSystemCategory, KMsvTestUidNetworkStatusValue, RProperty::EInt));	// Qualified: No leavescan error
		definedNetworkStatusValue = ETrue;
		}
	}
	
LOCAL_C void DeleteVariablesL()
	{
	if(definedPhonePwrValue)
		{
		User::LeaveIfError(RProperty::Delete(KUidSystemCategory, KMsvTestUidPhonePwrValue));
		}
	if(definedSIMStatusValue)
		{
		User::LeaveIfError(RProperty::Delete(KUidSystemCategory, KMsvTestUidSIMStatusValue));
		}
	if(definedNetworkStatusValue)
		{
		User::LeaveIfError(RProperty::Delete(KUidSystemCategory, KMsvTestUidNetworkStatusValue));
		}
	}	

LOCAL_C void DoTestsL()
	{
	// Create and install the active scheduler
 	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	
	// Create the test engine
	CTestEngine* testEngine = CTestEngine::NewL(test);
	CleanupStack::PushL(testEngine);

	CreateVariablesL();

	// Do tests...
	testEngine->DoTestsL();

	DeleteVariablesL();

	CleanupStack::PopAndDestroy(2, scheduler);
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	CTrapCleanup* tc=CTrapCleanup::New();
	TRAPD(err,DoTestsL());
	if (err!=KErrNone)
		User::Panic(KFailed, err);
	delete tc;
	test.Close();
	__UHEAP_MARKEND;
	return KErrNone;
	}
	
/*
 * CScheduleSendTestUtils
 */

CScheduleSendTestUtils* CScheduleSendTestUtils::NewL(RTest& aTest, TUint aCreationFlags)
	{
	CScheduleSendTestUtils* self = new (ELeave) CScheduleSendTestUtils(aTest);
	CleanupStack::PushL(self);
	self->ConstructL(aCreationFlags);
	CleanupStack::Pop(self);
	return self;	
	}

CScheduleSendTestUtils::~CScheduleSendTestUtils()
	{
	CMsvTestUtils::Reset();
	
	}
	
CScheduleSendTestUtils::CScheduleSendTestUtils(RTest& aTest)
: CMsvTestUtils(aTest)
	{
	}
	
TMsvId CScheduleSendTestUtils::ServiceId()
	{
	return iServiceId;
	}

void CScheduleSendTestUtils::ConstructL(TUint aCreationFlags)
	{
	CMsvTestUtils::ConstructL(aCreationFlags);
	}
	
void CScheduleSendTestUtils::InstallPigeonMtmGroupL()
	{
	InstallMtmGroupL(KPigeonDataComponentFileName);
	}

void CScheduleSendTestUtils::CreatePigeonServerMtmRegL()
	{
	// Don't bother - use resource file!
	}
	
void CScheduleSendTestUtils::DeletePigeonMessagesL(TMsvId aFolder)
	{
	SetEntryL(aFolder);
	CMsvEntrySelection* sel = ChildrenWithMtmLC(KUidMsgTypePigeon);

	TInt count = sel->Count();

	while( count-- )
		{
		DeleteEntryL(sel->At(count));
		}

	CleanupStack::PopAndDestroy(sel);
	}
	
void CScheduleSendTestUtils::NotifySaPhoneOnL()
	{
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidPhonePwrValue, EMsvTestPhoneOn));
	}

void CScheduleSendTestUtils::NotifySaPhoneOffL()
	{
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidPhonePwrValue, EMsvTestPhoneOff));
	}

void CScheduleSendTestUtils::NotifySaSimOkL()
	{
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidSIMStatusValue, EMsvTestSimOk));
	}

void CScheduleSendTestUtils::NotifySaSimNotPresentL()
	{
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidSIMStatusValue, EMsvTestSimNotPresent));
	}

void CScheduleSendTestUtils::NotifySaNetworkAvailableL()
	{
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidNetworkStatusValue, EMsvTestNetworkAvailable));
	}

void CScheduleSendTestUtils::NotifySaNetworkUnAvailableL()
	{
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidNetworkStatusValue, EMsvTestNetworkUnAvailable));
	}
	
void CScheduleSendTestUtils::UpdateSysAgentConditionsL()
	{
	CMsvSysAgentActions* actions = new (ELeave) CMsvSysAgentActions();
	CleanupStack::PushL(actions);

	CRepository* repository = CRepository::NewLC(KUidMsgTypePigeon);
	// load existing settings
	TRAPD(error, TMsvScheduleSettingsUtils::LoadSysAgentSettingsL(*actions, *repository));
	if( error==KErrNone )
		{
		// Reset the conditions - leave the default action.
		actions->Reset();
		}
	
	// Add the conditions
	TMsvSysAgentConditionAction condAction;
	condAction.iUseDefaultSysAgentAction = ETrue;

	condAction.iCondition.iVariable = KMsvTestUidSIMStatus;
	condAction.iCondition.iState = EMsvTestSimOk;

	condAction.iCondition.iType = TMsvCondition::EMsvSchSendEquals;
	actions->AppendL(condAction);

	condAction.iCondition.iVariable = KMsvTestUidNetworkStatus;
	condAction.iCondition.iState = EMsvTestNetworkAvailable;

	condAction.iCondition.iType = TMsvCondition::EMsvSchSendEquals;
	actions->AppendL(condAction);

	// save these settings
	TMsvScheduleSettingsUtils::SaveSysAgentSettingsL(*actions, *repository);	
	CleanupStack::PopAndDestroy(2, actions); 
	}
	
void CScheduleSendTestUtils::UpdatePendingConditionsTimeoutL(TTimeIntervalMinutes aTimeout)
	{
	CMsvScheduleSettings* settings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(settings);

	// access sms account settings
	CRepository* repository = CRepository::NewLC(KUidMsgTypePigeon);
	TMsvScheduleSettingsUtils::LoadScheduleSettingsL(*settings, *repository);

	// Store the pending conditions timeout
	settings->SetPendingConditionsTimeout(aTimeout);

	// save these settings
	TMsvScheduleSettingsUtils::SaveScheduleSettingsL(*settings, *repository);
	CleanupStack::PopAndDestroy(2, settings);
	}

//	Methods from CMsvTestUtils

void CScheduleSendTestUtils::CreateServicesL()
	{
	iServiceId = CreateServiceL(KUidMsgTypePigeon, EFalse, ETrue);
	}
	
void CScheduleSendTestUtils::DeleteServicesL()
	{
	DeleteServiceL(KUidMsgTypePigeon);
	}
	
void CScheduleSendTestUtils::InstallMtmGroupsL()
	{
	InstallPigeonMtmGroupL();
	}

void CScheduleSendTestUtils::CreateServerMtmRegsL()
	{
	CreatePigeonServerMtmRegL();
	}
	
//	Methods from CTestUtils
 
void CScheduleSendTestUtils::Panic(TInt aPanic)
	{
	User::Panic(KScheduleSendTestPanic, aPanic);
	}
	
/*
 *	CTestEngine
 */
 
CTestEngine* CTestEngine::NewL(RTest& aTest)
	{
	CTestEngine* self = new (ELeave) CTestEngine();
	CleanupStack::PushL(self);
	self->ConstructL(aTest);
	CleanupStack::Pop(self);
	return self;
	}
	
CTestEngine::~CTestEngine()
	{
	delete iTestUtils;
	}
	
CTestEngine::CTestEngine()
:CBase()
	{
	}
	
void CTestEngine::ConstructL(RTest& aTest)
	{
	iTestUtils = CScheduleSendTestUtils::NewL(aTest);
	
	// Install the pigeon MTM.
	iTestUtils->InstallMtmGroupsL();
	}
	
void CTestEngine::DoTestsL()
	{
	CTestScheduleSendBase* testCase = NULL;

	testCase = CTestSchedule::NewL(*iTestUtils);
	testCase->Start();
	CActiveScheduler::Start();
	delete testCase;	

	testCase = CTestReScheduleTime_1::NewL(*iTestUtils);
	testCase->Start();
	CActiveScheduler::Start();
	delete testCase;
			
	testCase = CTestReScheduleTime_2::NewL(*iTestUtils);
	testCase->Start();
	CActiveScheduler::Start();
	delete testCase;	

	testCase = CTestRescheduleConditions_1::NewL(*iTestUtils);
	testCase->Start();
	CActiveScheduler::Start();
	delete testCase;

	testCase = CTestRescheduleConditions_2::NewL(*iTestUtils);
	testCase->Start();
	CActiveScheduler::Start();
	delete testCase;
	testCase = CTestRescheduleConditions_3::NewL(*iTestUtils);
	testCase->Start();
	CActiveScheduler::Start();
	delete testCase;

	testCase = CTestRescheduleMixed_1::NewL(*iTestUtils);
	testCase->Start();
	CActiveScheduler::Start();
	delete testCase;

	testCase = CTestFailed_1::NewL(*iTestUtils);
	testCase->Start();
	CActiveScheduler::Start();
	delete testCase;		

	
	iTestUtils->TestHarnessCompleted();	
	}
	
/*
 * CTestFolderObserver
 */
	
CTestFolderObserver* CTestFolderObserver::NewL(MTestFolderObserverCallback& aCallback, TMsvId aFolderId)
	{
	CTestFolderObserver* self = new (ELeave) CTestFolderObserver(aCallback, aFolderId);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestFolderObserver::~CTestFolderObserver()
	{
	delete iEntryForObserver;
	}

CTestFolderObserver::CTestFolderObserver(MTestFolderObserverCallback& aCallback, TMsvId aFolderId)
: CBase(), iCallback(aCallback), iFolderId(aFolderId)
	{
	}

void CTestFolderObserver::ConstructL()
	{
	iEntryForObserver = iCallback.CreateEntryL(iFolderId);
	}

void CTestFolderObserver::StartObserverL()
	{
	iEntryForObserver->AddObserverL(*this);
	}
	
void CTestFolderObserver::StopObserver()
	{
	iEntryForObserver->RemoveObserver(*this);
	}	

// methods from MMsvEntryObserver

void CTestFolderObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	CMsvEntrySelection* sel = (CMsvEntrySelection*)(aArg1);
	
	// The messages in the outbox have been updated - check them.
	if(  aEvent == EMsvNewChildren )
		{
		TInt count = sel->Count();

		for( TInt i=0; i < count; ++i )
			{
			// Notify the callback
			iCallback.UpdatedMessageL(iFolderId, sel->At(i));
			}
		}	
	}
	
/*
 * CTestScheduleSendBase
 */
 
CTestScheduleSendBase::~CTestScheduleSendBase()
	{
	delete iSelection;
	delete iPendingSent;
	delete iOutboxObserver;
	delete iSentFolderObserver;
	delete iOperation;
	}	
	
void CTestScheduleSendBase::Start()
	{
	iTestUtils.Printf(_L("Starting %S\n"), &TestName());
	iState = EClearFolders;
	CompleteSelf();
	}
	
CTestScheduleSendBase::CTestScheduleSendBase(CScheduleSendTestUtils& aTestUtils)
: CActive(CActive::EPriorityStandard), iTestUtils(aTestUtils)
	{
	CActiveScheduler::Add(this);
	}
	
void CTestScheduleSendBase::ConstructL()
	{
	iSelection = new (ELeave) CMsvEntrySelection();
	iPendingSent = new (ELeave) CMsvEntrySelection();
	iOutboxObserver = CTestFolderObserver::NewL(*this, KMsvGlobalOutBoxIndexEntryId);
	iSentFolderObserver = CTestFolderObserver::NewL(*this, KMsvSentEntryId);
	}
	
void CTestScheduleSendBase::CompleteSelf()
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}
	
CMsvSession& CTestScheduleSendBase::Session()
	{
	return *iTestUtils.iMsvSession;
	}

CMsvEntry& CTestScheduleSendBase::MsvEntry()
	{
	return *iTestUtils.iMsvEntry;
	}

void CTestScheduleSendBase::ClearFoldersL()
	{
	iTestUtils.DeletePigeonMessagesL(KMsvGlobalOutBoxIndexEntryId);
	iTestUtils.DeletePigeonMessagesL(KMsvGlobalInBoxIndexEntryId);
	iTestUtils.DeletePigeonMessagesL(KMsvSentEntryId);		
	}
	
void CTestScheduleSendBase::TestComplete()
	{
	iTestUtils.Printf(_L("Test Complete\n"));
	CActiveScheduler::Stop();
	}
	
TMsvId CTestScheduleSendBase::CreateMessageL(TTime& aTime, TInt aError)
	{
	iTestUtils.Printf(_L("Creating message...\n"));
	
	TMsvEntry entry;
	entry.SetVisible(ETrue); 
	entry.SetInPreparation(EFalse); 
	entry.iServiceId = KMsvLocalServiceIndexEntryId; 
	entry.iType = KUidMsvMessageEntry; 
	entry.iMtm = KUidMsgTypePigeon; 
	entry.iDate = aTime; 
	entry.iSize = 0; 
	entry.iDescription.Set(KNullDesC); 
	entry.iDetails.Set(KNullDesC); 
	entry.iError = aError;
	entry.SetOffPeak(EFalse);
	entry.SetScheduled(EFalse);	
	entry.SetSendingState(KMsvSendStateWaiting);
	
	// Create the entry - set context to the global outbox.
	MsvEntry().SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	MsvEntry().CreateL(entry);
	
	return entry.Id();
	}
	
void CTestScheduleSendBase::TransferCommandL(TInt aCommand)
	{
	delete iOperation;
	iOperation = NULL;
	
	iOperation = Session().TransferCommandL(*iSelection, aCommand, _L8("aaaa"), iStatus);
	SetActive();
	}
	
void CTestScheduleSendBase::UpdateSysAgentConditionsL()
	{
	CompleteSelf();
	}
	
/**
Notifies the test case that the messages have been scheduled successfully.

The test case can indicate whether this is the end of the test.

@return
A value of ETrue if this is the end of the test.
*/
TBool CTestScheduleSendBase::NotifyMessagesScheduledL()
	{
	return EFalse;
	}
	
const TDesC& CTestScheduleSendBase::GetStateText(TInt aState) const 
	{
	switch( aState )
		{
	case KMsvSendStateUnknown:
		return KStateUnknown();
	case KMsvSendStateUponRequest:
		return KStateUponRequest();
	case KMsvSendStateWaiting:
		return KStateWaiting();
	case KMsvSendStateSending:
		return KStateSending();
	case KMsvSendStateScheduled:
		return KStateScheduled();
	case KMsvSendStateResend:
		return KStateResend();
	case KMsvSendStateSuspended:
		return KStateSuspended();
	case KMsvSendStateFailed:
		return KStateFailed();
	case KMsvSendStateSent:
		return KStateSent();
	case KMsvSendStateNotApplicable:
		return KStateNotApplicable();
	default:
		break;
		}
	return KNullDesC();
	}
	
const TDesC& CTestScheduleSendBase::GetBoolText(TBool aBool) const
	{
	if( aBool )
		return KBoolTrue();
	return KBoolFalse();
	}

void CTestScheduleSendBase::CheckMessageDetailsL(TMsvId aId, TInt aState, TBool aScheduled, TBool aCheckPendingConditions)
	{
	// Need to check the following details on the message - 
	// 1. Correct sending state.
	// 2. Scheduled flag.
	// 3. Pending conditions flag
	iTestUtils.Printf(_L("!!Checking message %d...\n"), aId);
	
	MsvEntry().SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	MsvEntry().SetEntryL(aId);
	TMsvEntry entry = MsvEntry().Entry();
			
	iTestUtils.Printf(_L("->Sending state is %S (%d) - should be %S (%d)\n"), &GetStateText(entry.SendingState()), entry.SendingState(), &GetStateText(aState), aState);
	iTestUtils.Test()(entry.SendingState() == (TUint) aState);
	
	if( aScheduled )
		{
		iTestUtils.Printf(_L("->Scheduled flag is %S (%d) - should be %S\n"), &GetBoolText(entry.Scheduled()), entry.Scheduled(), &GetBoolText(ETrue));
		iTestUtils.Test()(entry.Scheduled() != EFalse);
		}
	else
		{
		iTestUtils.Printf(_L("->Scheduled flag is %S (%d) - should be %S\n"), &GetBoolText(entry.Scheduled()), entry.Scheduled(), &GetBoolText(EFalse));
		iTestUtils.Test()(entry.Scheduled() == EFalse);
		}

	// The pending conditons flag should be true if aCheckPendingConditions is 
	// true and the entry error is KErrorActionConditions
	if( aCheckPendingConditions && entry.iError == KErrorActionConditions && entry.SendingState()!= KMsvSendStateSent && entry.SendingState()!=KMsvSendStateResend)
		{
		iTestUtils.Printf(_L("->Pending conditions flag is %S (%d) - should be %S\n"), &GetBoolText(entry.PendingConditions()), entry.PendingConditions(), &GetBoolText(ETrue));
		iTestUtils.Test()(entry.PendingConditions() != EFalse);
		}
	else
		{
		iTestUtils.Printf(_L("->Pending conditions flag is %S (%d) - should be %S\n"), &GetBoolText(entry.PendingConditions()), entry.PendingConditions(), &GetBoolText(EFalse));
		iTestUtils.Test()(entry.PendingConditions() == EFalse);
		}
	
	// Check the scheduled time - do this if sending state is KMsvSendStateSent
	if( aState == KMsvSendStateSent )
		{
		TTime now;
		now.HomeTime();
		
		_LIT(KTimeFormat, "%-B%:0%J%:1%T%:2%S%:3%+B");
		
		TBuf<32> scheduledTime;
		entry.iDate.FormatL(scheduledTime, KTimeFormat);

		TBuf<32> currentTime;
		now.FormatL(currentTime, KTimeFormat);

		if( entry.PendingConditions() )
			{
			if( ConditionsMet() )
				{
				iTestUtils.Printf(_L("->Message scheduled for conditions -> met, timeout is %S - should be after current time %S\n"), &scheduledTime, &currentTime);
				iTestUtils.Test()(entry.iDate > now);
				}
			else
				{
				iTestUtils.Printf(_L("->Message scheduled for conditions -> not met, timeout is %S - should be same or before current time %S\n"), &scheduledTime, &currentTime);
				iTestUtils.Test()(entry.iDate <= now);
				}
			}
		else
			{
			iTestUtils.Printf(_L("->Message scheduled for %S - should be same or before current time %S\n"), &scheduledTime, &currentTime);
			iTestUtils.Test()(entry.iDate <= now);			
			}
		}
	}
	
void CTestScheduleSendBase::CheckCreatedMessagesL()
	{
	// Need to check the following on all pigeon messages in the outbox - 
	// 1. Correct sending state - KMsvSendStateWaiting.
	// 2. Scheduled flag is false.
	// 3. Pending conditions is false.
	
	iTestUtils.Printf(_L("Checking for new pigeon messages in the outbox...\n"));
	
	MsvEntry().SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	
	CMsvEntrySelection* sel = iTestUtils.ChildrenWithMtmLC(KUidMsgTypePigeon);
	
	iTestUtils.Printf(_L("->Number of pigeon messages is %d - should be %d\n"), sel->Count(), iSelection->Count());
	iTestUtils.Test()(sel->Count() == iSelection->Count());
	
	TInt count = sel->Count();
	
	for( TInt i=0; i < count; ++i )
		{
		CheckMessageDetailsL(sel->At(i), KMsvSendStateWaiting, EFalse, EFalse);
		}
	
	CleanupStack::PopAndDestroy(sel);
	}
	
void CTestScheduleSendBase::CheckScheduledMessagesL()
	{
	// Need to check the following on all pigeon messages in the outbox - 
	// 1. Correct sending state - ask derived class
	// 2. Scheduled flag is not false.
	// 3. Pending conditions is not false if the message error code is KErrorActionConditions.
	
	iTestUtils.Printf(_L("Checking for scheduled pigeon messages in the outbox...\n"));
	
	MsvEntry().SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	
	CMsvEntrySelection* sel = iTestUtils.ChildrenWithMtmLC(KUidMsgTypePigeon);
	
	iTestUtils.Printf(_L("->Number of pigeon messages is %d - should be %d\n"), sel->Count(), iSelection->Count());
	iTestUtils.Test()(sel->Count() == iSelection->Count());
	
	TInt count = sel->Count();
	
	for( TInt i=0; i < count; ++i )
		{
		TInt state = ScheduledState();
		TBool scheduled = state == KMsvSendStateResend || state == KMsvSendStateScheduled;
		CheckMessageDetailsL(sel->At(i), state, scheduled, ETrue);
		}
	
	CleanupStack::PopAndDestroy(sel);	
	}
		
void CTestScheduleSendBase::CheckPreSendMessageL(TMsvId aId)
	{
	// Need to check the following on all pigeon messages in the outbox - 
	// 1. Correct sending state - ask derived class (noramlly KMsvSendStateWaiting).
	// 2. Scheduled flag is not false.
	// 3. Pending conditions is not false if the message error code is KErrorActionConditions.

	CheckMessageDetailsL(aId, PreSendState(), ETrue, ETrue);
	}

void CTestScheduleSendBase::CheckSentMessageL(TMsvId aId)
	{
	// Need to check the following on the supplied message - 
	// 1. Correct sending state - ask derived class (noramlly KMsvSendStateSent).
	// 2. Scheduled flag is not false.
	// 3. Pending conditions is not false if the message error code is KErrorActionConditions.
	
	CheckMessageDetailsL(aId, SentState(), ETrue, ETrue);
	}
	
TInt CTestScheduleSendBase::PreSendState()
	{
	return KMsvSendStateWaiting;
	}
	
TInt CTestScheduleSendBase::SentState()
	{
	return KMsvSendStateSent;
	}

TBool CTestScheduleSendBase::ConditionsMet() const
	{
	return iConditionsMet;
	}

// methods from CActive

void CTestScheduleSendBase::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	switch( iState )
		{
	case EClearFolders:
		{
		ClearFoldersL();
		iState = EUpdateSysAgentConditions;
		CompleteSelf();
		} break;
	case EUpdateSysAgentConditions:
		{
		UpdateSysAgentConditionsL();
		iState = ECreateMessages;
		} break;			
	case ECreateMessages:
		{
		CreateMessagesL();
		iState = ECheckCreatedMessages;
		CompleteSelf();
		} break;
	case ECheckCreatedMessages:
		{
		CheckCreatedMessagesL();
		iState = EScheduleMessages;
		CompleteSelf();
		} break;
	case EScheduleMessages:
		{
		ScheduleMessagesL();
		iState = ECheckScheduledMessages;
		} break;
	case ECheckScheduledMessages:
		{
		CheckScheduledMessagesL();

		// Get outbox observer to start observing...
		iOutboxObserver->StartObserverL();
		iSentFolderObserver->StartObserverL();
		
		TBool done = NotifyMessagesScheduledL();
		
		if( done )
			{
			// No more to do - end the test
			TestComplete();
			}
		else
			{
			// Waiting for the messages to be scheduled and sent.
			iState = EPendingMessagesSent;
			}
		} break;
	case EPendingMessagesSent:
		{
		// End the test...
		TestComplete();
		} break;			
	default:
		User::Invariant();
		break;
		}
	}
	
void CTestScheduleSendBase::DoCancel()
	{
	}
	
TInt CTestScheduleSendBase::RunError(TInt aError)
	{
	iTestUtils.Printf(_L("!!!RunL leave with error code : %d...\n"), aError);
	
	User::Invariant();
	
	return aError;
	}
	
//	methods from MTestFolderObserverCallback	
	
void CTestScheduleSendBase::UpdatedMessageL(TMsvId aFolderId, TMsvId aMessageId)
	{
	__ASSERT_DEBUG( iState == EPendingMessagesSent, User::Invariant() );
	
	if( aFolderId == KMsvGlobalOutBoxIndexEntryId )
		{
		iTestUtils.Printf(_L("Outbox has been updated...\n"));
		
		// This message has just been scheduled to be sent - check it has been
		// updated correctly..
		CheckPreSendMessageL(aMessageId);
		}
	else if( aFolderId == KMsvSentEntryId )
		{
		iTestUtils.Printf(_L("Sent folder has been updated...\n"));

		// This message has just been sent - check it has been updated correctly.
		CheckSentMessageL(aMessageId);
		++iMessagesDone;
		}
		
	if( iMessagesDone == iSelection->Count() )
		{
		// Test is ended as all messges have been sent - complete-self to end.
		CompleteSelf();
		}
	}
	
CMsvEntry* CTestScheduleSendBase::CreateEntryL(TMsvId aFolderId)
	{
	return 	Session().GetEntryL(aFolderId);
	}

/*
 * CTestSchedule
 */
 
CTestSchedule* CTestSchedule::NewL(CScheduleSendTestUtils& aTestUtils)
	{
	CTestSchedule* self = new (ELeave) CTestSchedule(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
 
CTestSchedule::~CTestSchedule()
	{
	}	
	
CTestSchedule::CTestSchedule(CScheduleSendTestUtils& aTestUtils)
: CTestScheduleSendBase(aTestUtils)
	{
	}
	
//	 methods from CTestScheduleSendBase
	
void CTestSchedule::CreateMessagesL()
	{
	// Create two pigeon messages in the inbox.
	iSelection->Reset();
	iSelection->SetReserveL(2);
	
	TTime now;
	now.HomeTime();
	now.RoundUpToNextMinute();
	
	iSelection->AppendL(CreateMessageL(now, KErrNone));
	iSelection->AppendL(CreateMessageL(now, KErrNone));	
	}

void CTestSchedule::ScheduleMessagesL()
	{
	iTestUtils.Printf(_L("Scheduling %d messages\n"), iSelection->Count());
	TransferCommandL(EScheduleAllL);
	}

TInt CTestSchedule::ScheduledState()
	{
	return KMsvSendStateScheduled;
	}

const TDesC& CTestSchedule::TestName()
	{
	return KTestScheduleSendScheduleMultiple();
	}
	
/*
 * CTestReScheduleTime_1
 */
 
CTestReScheduleTime_1* CTestReScheduleTime_1::NewL(CScheduleSendTestUtils& aTestUtils)
	{
	CTestReScheduleTime_1* self = new (ELeave) CTestReScheduleTime_1(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
 
CTestReScheduleTime_1::~CTestReScheduleTime_1()
	{
	}	
	
CTestReScheduleTime_1::CTestReScheduleTime_1(CScheduleSendTestUtils& aTestUtils)
: CTestScheduleSendBase(aTestUtils)
	{
	}
	
//	 methods from CTestScheduleSendBase
	
void CTestReScheduleTime_1::CreateMessagesL()
	{
	// Create two pigeon messages in the inbox.
	iSelection->Reset();
	iSelection->SetReserveL(2);
	
	TTime now;
	now.HomeTime();
	
	iSelection->AppendL(CreateMessageL(now, KErrorActionImmediately));
	iSelection->AppendL(CreateMessageL(now, KErrorActionImmediately));
	}

void CTestReScheduleTime_1::ScheduleMessagesL()
	{
	iTestUtils.Printf(_L("Rescheduling %d messages\n"), iSelection->Count());
	TransferCommandL(EReScheduleAllL);
	}

TInt CTestReScheduleTime_1::ScheduledState()
	{
	return KMsvSendStateResend;
	}

const TDesC& CTestReScheduleTime_1::TestName()
	{
	return KTestScheduleSendReScheduleTime_1();
	}
	
/*
 * CTestReScheduleTime_2
 */
 
CTestReScheduleTime_2* CTestReScheduleTime_2::NewL(CScheduleSendTestUtils& aTestUtils)
	{
	CTestReScheduleTime_2* self = new (ELeave) CTestReScheduleTime_2(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
 
CTestReScheduleTime_2::~CTestReScheduleTime_2()
	{
	}	
	
CTestReScheduleTime_2::CTestReScheduleTime_2(CScheduleSendTestUtils& aTestUtils)
: CTestScheduleSendBase(aTestUtils)
	{
	}
	
//	 methods from CTestScheduleSendBase
	
void CTestReScheduleTime_2::CreateMessagesL()
	{
	// Create two pigeon messages in the inbox.
	iSelection->Reset();
	iSelection->SetReserveL(2);
	
	TTime now;
	now.HomeTime();
	
	iSelection->AppendL(CreateMessageL(now, KErrorActionImmediately));
	iSelection->AppendL(CreateMessageL(now, KErrorActionLater));
	}

void CTestReScheduleTime_2::ScheduleMessagesL()
	{
	iTestUtils.Printf(_L("Rescheduling %d messages\n"), iSelection->Count());
	TransferCommandL(EReScheduleAllL);
	}

TInt CTestReScheduleTime_2::ScheduledState()
	{
	return KMsvSendStateResend;
	}

const TDesC& CTestReScheduleTime_2::TestName()
	{
	return KTestScheduleSendReScheduleTime_2();
	}
	
/*
 * CTestRescheduleConditions_1
 */
 
CTestRescheduleConditions_1* CTestRescheduleConditions_1::NewL(CScheduleSendTestUtils& aTestUtils)
	{
	CTestRescheduleConditions_1* self = new (ELeave) CTestRescheduleConditions_1(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}	
	
CTestRescheduleConditions_1::~CTestRescheduleConditions_1()
	{
	}
	
CTestRescheduleConditions_1::CTestRescheduleConditions_1(CScheduleSendTestUtils& aTestUtils)
: CTestScheduleSendBase(aTestUtils)
	{
	}
	
// methods from CTestScheduleSendBase

void CTestRescheduleConditions_1::UpdateSysAgentConditionsL()
	{
	iTestUtils.UpdateSysAgentConditionsL();
	iTestUtils.UpdatePendingConditionsTimeoutL(0);	
	CompleteSelf();
	}
	
TBool CTestRescheduleConditions_1::NotifyMessagesScheduledL()
	{
	// Messages have been scheduled - meet the conditions; SIM ok, network available.
	iTestUtils.NotifySaSimOkL();
	iTestUtils.NotifySaNetworkAvailableL();
	iConditionsMet = ETrue;
	return EFalse;
	}
	
void CTestRescheduleConditions_1::CreateMessagesL()
	{
	// Create two pigeon messages in the inbox.
	iSelection->Reset();
	iSelection->SetReserveL(2);
	
	TTime now;
	now.HomeTime();
	
	iSelection->AppendL(CreateMessageL(now, KErrorActionConditions));
	iSelection->AppendL(CreateMessageL(now, KErrorActionConditions));	
	}
	
void CTestRescheduleConditions_1::ScheduleMessagesL()
	{
	// First fail to meet conditions - SIM not present + network unavailable.
	iTestUtils.NotifySaSimNotPresentL();	
	iTestUtils.NotifySaNetworkUnAvailableL();	
	
	iTestUtils.Printf(_L("Rescheduling %d messages\n"), iSelection->Count());
	TransferCommandL(EReScheduleAllL);
	}
	
TInt CTestRescheduleConditions_1::ScheduledState()
	{
	return KMsvSendStateResend;
	}

const TDesC& CTestRescheduleConditions_1::TestName()
	{
	return KTestScheduleSendReScheduleConditions_1();
	}
	
/*
 * CTestRescheduleConditions_2
 */
 
CTestRescheduleConditions_2* CTestRescheduleConditions_2::NewL(CScheduleSendTestUtils& aTestUtils)
	{
	CTestRescheduleConditions_2* self = new (ELeave) CTestRescheduleConditions_2(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}	
	
CTestRescheduleConditions_2::~CTestRescheduleConditions_2()
	{
	}
	
CTestRescheduleConditions_2::CTestRescheduleConditions_2(CScheduleSendTestUtils& aTestUtils)
: CTestScheduleSendBase(aTestUtils)
	{
	}
	
// methods from CTestScheduleSendBase

void CTestRescheduleConditions_2::UpdateSysAgentConditionsL()
	{
	iTestUtils.UpdateSysAgentConditionsL();
	iTestUtils.UpdatePendingConditionsTimeoutL(1);
	CompleteSelf();
	}
	
TBool CTestRescheduleConditions_2::NotifyMessagesScheduledL()
	{
	// Messages have been scheduled - meet the conditions; SIM ok, network available.
	iTestUtils.NotifySaSimOkL();
	iTestUtils.NotifySaNetworkAvailableL();
	iConditionsMet = ETrue;
	return EFalse;
	}
	
void CTestRescheduleConditions_2::CreateMessagesL()
	{
	// Create two pigeon messages in the inbox.
	iSelection->Reset();
	iSelection->SetReserveL(2);
	
	TTime now;
	now.HomeTime();
	
	iSelection->AppendL(CreateMessageL(now, KErrorActionConditions));
	iSelection->AppendL(CreateMessageL(now, KErrorActionConditions));	
	}
	
void CTestRescheduleConditions_2::ScheduleMessagesL()
	{
	// First fail to meet conditions - SIM not present + network unavailable.
	iTestUtils.NotifySaSimNotPresentL();	
	iTestUtils.NotifySaNetworkUnAvailableL();	
	
	iTestUtils.Printf(_L("Rescheduling %d messages\n"), iSelection->Count());
	TransferCommandL(EReScheduleAllL);
	}
	
TInt CTestRescheduleConditions_2::ScheduledState()
	{
	return KMsvSendStateResend;
	}

const TDesC& CTestRescheduleConditions_2::TestName()
	{
	return KTestScheduleSendReScheduleConditions_2();
	}
		
/*
 * CTestRescheduleConditions_3
 */
 
CTestRescheduleConditions_3* CTestRescheduleConditions_3::NewL(CScheduleSendTestUtils& aTestUtils)
	{
	CTestRescheduleConditions_3* self = new (ELeave) CTestRescheduleConditions_3(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}	
	
CTestRescheduleConditions_3::~CTestRescheduleConditions_3()
	{
	}
	
CTestRescheduleConditions_3::CTestRescheduleConditions_3(CScheduleSendTestUtils& aTestUtils)
: CTestScheduleSendBase(aTestUtils)
	{
	}
	
// methods from CTestScheduleSendBase

void CTestRescheduleConditions_3::UpdateSysAgentConditionsL()
	{
	iTestUtils.UpdateSysAgentConditionsL();
	iTestUtils.UpdatePendingConditionsTimeoutL(1);
	CompleteSelf();
	}
		
TBool CTestRescheduleConditions_3::NotifyMessagesScheduledL()
	{
	// Messages have been scheduled - meet only one of the conditions; SIM ok.
	// This should imply that timeout should occur
	iTestUtils.NotifySaSimOkL();
	iConditionsMet = EFalse;
	return EFalse;
	}

void CTestRescheduleConditions_3::CreateMessagesL()
	{
	// Create two pigeon messages in the inbox.
	iSelection->Reset();
	iSelection->SetReserveL(2);
	
	TTime now;
	now.HomeTime();
	
	iSelection->AppendL(CreateMessageL(now, KErrorActionConditions));
	iSelection->AppendL(CreateMessageL(now, KErrorActionConditions));	
	}
	
void CTestRescheduleConditions_3::ScheduleMessagesL()
	{
	// First fail to meet conditions - phone off + network unavailable.
	// NOTE - no need to meet conditions, let timeout happen.	
	iTestUtils.NotifySaSimNotPresentL();	
	iTestUtils.NotifySaNetworkUnAvailableL();	
	
	iTestUtils.Printf(_L("Rescheduling %d messages\n"), iSelection->Count());
	TransferCommandL(EReScheduleAllL);
	}
	
TInt CTestRescheduleConditions_3::ScheduledState()
	{
	return KMsvSendStateResend;
	}

const TDesC& CTestRescheduleConditions_3::TestName()
	{
	return KTestScheduleSendReScheduleConditions_3();
	}
	
/*
 * CTestRescheduleMixed_1
 */
 
CTestRescheduleMixed_1* CTestRescheduleMixed_1::NewL(CScheduleSendTestUtils& aTestUtils)
	{
	CTestRescheduleMixed_1* self = new (ELeave) CTestRescheduleMixed_1(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}	
	
CTestRescheduleMixed_1::~CTestRescheduleMixed_1()
	{
	}
	
CTestRescheduleMixed_1::CTestRescheduleMixed_1(CScheduleSendTestUtils& aTestUtils)
: CTestScheduleSendBase(aTestUtils)
	{
	}
	
// methods from CTestScheduleSendBase

void CTestRescheduleMixed_1::UpdateSysAgentConditionsL()
	{
	iTestUtils.UpdateSysAgentConditionsL();
	iTestUtils.UpdatePendingConditionsTimeoutL(1);
	CompleteSelf();
	}
	
TBool CTestRescheduleMixed_1::NotifyMessagesScheduledL()
	{
	// Messages have been scheduled - meet the conditions; SIM ok, network available.
	iTestUtils.NotifySaSimOkL();
	iTestUtils.NotifySaNetworkAvailableL();
	iConditionsMet = ETrue;
	return EFalse;
	}
	
void CTestRescheduleMixed_1::CreateMessagesL()
	{
	// Create two pigeon messages in the inbox.
	iSelection->Reset();
	iSelection->SetReserveL(2);
	
	TTime now;
	now.HomeTime();
	
	iSelection->AppendL(CreateMessageL(now, KErrorActionImmediately));
	iSelection->AppendL(CreateMessageL(now, KErrorActionConditions));	
	}
	
void CTestRescheduleMixed_1::ScheduleMessagesL()
	{
	// First fail to meet conditions - SIM not present + network unavailable.
	iTestUtils.NotifySaSimNotPresentL();	
	iTestUtils.NotifySaNetworkUnAvailableL();	
	
	iTestUtils.Printf(_L("Rescheduling %d messages\n"), iSelection->Count());
	TransferCommandL(EReScheduleAllL);
	}
	
TInt CTestRescheduleMixed_1::ScheduledState()
	{
	return KMsvSendStateResend;
	}

const TDesC& CTestRescheduleMixed_1::TestName()
	{
	return KTestScheduleSendReScheduleMixed_1();
	}
	
/*
 * CTestFailed_1
 */
 
CTestFailed_1* CTestFailed_1::NewL(CScheduleSendTestUtils& aTestUtils)
	{
	CTestFailed_1* self = new (ELeave) CTestFailed_1(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}	
	
CTestFailed_1::~CTestFailed_1()
	{
	}
	
CTestFailed_1::CTestFailed_1(CScheduleSendTestUtils& aTestUtils)
: CTestScheduleSendBase(aTestUtils)
	{
	}
	
// methods from CTestScheduleSendBase
	
TBool CTestFailed_1::NotifyMessagesScheduledL()
	{
	// Messages are not scheduled - end test.
	return ETrue;
	}
	
void CTestFailed_1::CreateMessagesL()
	{
	// Create two pigeon messages in the inbox.
	iSelection->Reset();
	iSelection->SetReserveL(2);
	
	TTime now;
	now.HomeTime();
	
	iSelection->AppendL(CreateMessageL(now, KErrorActionFail));
	iSelection->AppendL(CreateMessageL(now, KErrorActionFail));	
	}
	
void CTestFailed_1::ScheduleMessagesL()
	{
	iTestUtils.Printf(_L("Rescheduling %d messages\n"), iSelection->Count());
	TransferCommandL(EReScheduleAllL);
	}
	
TInt CTestFailed_1::ScheduledState()
	{
	return  KMsvSendStateResend;
	}

const TDesC& CTestFailed_1::TestName()
	{
	return KTestScheduleSendFailed_1();
	}
	
/*
 * CTestFailed_2
 */
 
CTestFailed_2* CTestFailed_2::NewL(CScheduleSendTestUtils& aTestUtils)
	{
	CTestFailed_2* self = new (ELeave) CTestFailed_2(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}	
	
CTestFailed_2::~CTestFailed_2()
	{
	}
	
CTestFailed_2::CTestFailed_2(CScheduleSendTestUtils& aTestUtils)
: CTestScheduleSendBase(aTestUtils)
	{
	}
	
// methods from CTestScheduleSendBase
	
TBool CTestFailed_2::NotifyMessagesScheduledL()
	{
	// Messages are scheduled.
	return EFalse;
	}
	
void CTestFailed_2::CreateMessagesL()
	{
	// Create two pigeon messages in the inbox.
	iSelection->Reset();
	iSelection->SetReserveL(2);
	
	TTime now;
	now.HomeTime();
	
	iSelection->AppendL(CreateMessageL(now, KErrorActionRetryFail));
	iSelection->AppendL(CreateMessageL(now, KErrorActionRetryFail));	
	}
	
void CTestFailed_2::ScheduleMessagesL()
	{
	iTestUtils.Printf(_L("Rescheduling (for retries) %d messages\n"), iSelection->Count());
	TransferCommandL(EReScheduleRetryAllL);
	}
	
TInt CTestFailed_2::ScheduledState()
	{
	return KMsvSendStateResend;
	}

TInt CTestFailed_2::SentState()
	{
	return KMsvSendStateWaiting;
	}

const TDesC& CTestFailed_2::TestName()
	{
	return KTestScheduleSendFailed_2();
	}
