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
//

#ifndef __PIGEONSERVERMTM_H__
#define __PIGEONSERVERMTM_H__

#include <schedulebaseservermtm.h>
#include <msvschedulesend.h>
#include <msvoffpeaktime.h>
#include "t_schsendshare.h"



const TUid KUidMsgTypePigeon = {0x1000127E};
	
class CPigeonScheduledSend;
class MPigeonObserver;

class CPigeonServerMtm : public CScheduleBaseServerMtm
	{
public:
	
	IMPORT_C static CPigeonServerMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry& aServerEntry);

	~CPigeonServerMtm();

	void			ScheduleL(CMsvEntrySelection& aSelection, const TBool aMove, const TDesC8& aParameter, TRequestStatus& /*aStatus*/);
	void			RestoreScheduleSettingsL();
	void			SendScheduledL(CMsvEntrySelection& aSelection, const TBool aMove, const TDesC8& aParameter, TRequestStatus& aStatus);
	void			SetFailFirstSendL(CMsvEntrySelection& aSelection);
	void			StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus);

	void			DoCancel(){};
	void			CopyToLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/){};
	void			CopyFromLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/){};
	void			CopyWithinServiceL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/){};
	void			DeleteAllL(const CMsvEntrySelection& /*aSelection*/, TRequestStatus& /*aStatus*/){};
	void			CreateL(TMsvEntry /*aEntry*/, TRequestStatus& /*aStatus*/){};
	void			ChangeL(TMsvEntry /*aEntry*/, TRequestStatus& /*aStatus*/){};
	TBool			CommandExpected(){return ETrue;};
	const TDesC8&	Progress();
	void			MoveToLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/){};
	void			MoveFromLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/){};
	void			MoveWithinServiceL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/){};
	void			DoRunL(){};
	void			DoComplete(int /*i*/){};
	void			LoadResourceFileL(){};
	void			PopulateSchedulePackage(const TDesC8& /*aDes*/, const int /*i*/, TMsvSchedulePackage& /*aPackage*/) const{};




private:
	void					ConstructL();
	void					RestoreSettingsL();
	void					DoStartCommandL(CMsvEntrySelection& aSelection, TSchSendTestOperation aCommand, const TDesC8& aParameter, TRequestStatus& aStatus);
							CPigeonServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry);// 
	void					UpdateProgressL(const CMsvEntrySelection& aSelection);
	CMsvScheduledEntry*		ScheduledEntryLC(TMsvId aId);
	CMsvStore*				GetServiceEntryEditStoreLC();
private:
	TMsvId					iServiceId;
	CPigeonScheduledSend*	iScheduleSend;
	
	TSchSendTestProgressBuf	iProgress;

	TBool					iHeapFailure;
	TInt					iNextFailure;
	};

class CPigeonScheduledEntry : public CMsvScheduledEntry 
	{
public:
	IMPORT_C static CPigeonScheduledEntry*	NewL(const TMsvEntry& aMsvEntry);
	TBool					CanSendToAnyRecipients(const TMsvSendErrorAction&);
	TBool					CanSendToAnyRecipients(const CMsvSendErrorActions&, TMsvSendErrorAction&);
	void					RecipientsResetRetries(){};
	void					RecipientsIncreaseRetries(){};
	void					RecipientsSetFailed(){};
	TBool					RecipientsAllSent() const;
	void					RecipientsStoreL(CMsvStore&) const{};
	void					RecipientsRestoreL(CMsvStore&) {};
	
private:
	void					ConstructL();
	CPigeonScheduledEntry(const TMsvEntry& aMsvEntry) : CMsvScheduledEntry(aMsvEntry){};
	};

class CPigeonScheduledSend : public CMsvScheduleSend
	{
public:
	IMPORT_C static CPigeonScheduledSend* NewL(CMsvServerEntry& aServerEntry);
							~CPigeonScheduledSend();
	CMsvScheduledEntry*		GetMessageL(const TMsvId aId) const;

	void					SetupL(TSchSendTestOperation aOption);
	void 					SaveSysAgentActionsL(CRepository& aRepository);	
	void 					LoadSysAgentActionsL(CRepository& aRepository);
	void 					LoadPendingConditionsTimeoutL(CRepository& aRepository);
	void					PopulateErrorListL();

private:

	//Two arrays of possible values for iSettings and iErrorActions
	//This class takes ownership of _all_ of them including the ones installed in 
	//iSettings and iErrorActions at destructing time.

	CArrayFixFlat<TMsvSendErrorAction>*			iActionsList;
	CArrayFixFlat<TMsvOffPeakTime>*				iOffPeakList;

private:
							CPigeonScheduledSend(CMsvServerEntry& aServerEntry)
								: CMsvScheduleSend(aServerEntry){};
	void					ConstructL();

	enum					TActions
								{
								EActionDefault = 0,
								EActionImmediately = 1,
								EActionLater = 2,
								EActionVariable = 3,
								EActionFail = 4
								};
	enum					TSettings
								{
								ESettingsDefault = 0,
								ESettingsCustom = 1
								};
	enum					TOffPeak
								{
								EOffPeakCurrent = 0,
								EOffPeakFinished = 1,
								EOffPeakFuture = 2
								};

	// Add all the different possible settings/actions to the lists
	void					PopulateSettingsL();
	void					PopulateActionsListL();
	void					PopulateOffPeakListL();
	
	//Set the current settings/actions from the lists
	void					SetActionL(TActions aAction);
	void					SetOffPeakL(TOffPeak aSetting);
	};
	
#endif
