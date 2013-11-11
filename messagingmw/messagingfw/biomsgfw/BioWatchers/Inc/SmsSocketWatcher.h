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

#ifndef __SMSSOCKETWATCHER_H_
#define __SMSSOCKETWATCHER_H_

#include <e32base.h>
#include <msvstd.h>
#include <mtsr.h>
#include <msvapi.h>
#include <es_sock.h>
#include <bif.h>
#include <bsp.h>
#include <csmsgetdetdescinterface.h>

enum TBioWatcherPanic
	{
	EMainSchedulerError1 = 0,
	EMainSchedulerError2 = 1,
	ECreateTrapCleanup = 2,
	ESocketWatcherAlreadyActive = 3,
	EAllSocketWatchersComplete = 4,
	EHandleBifChangeError = 5,
	EReportHandlingInvalid = 6,
	EUnexpectedErrorCode = 7,
	EBufferNotNull = 8,
	EObjectNotConstructed = 9,
	EAddrFamilyNotSupported1 = 10,
	EAddrFamilyNotSupported2 = 11,
	EAddrFamilyNotSupported3 = 12
	};

#ifdef _BIOMSG_NO_LOGGING
#define BIOWATCHERLOG(a)
#else
#define BIOWATCHERLOG(a) a
#endif

_LIT(KSocketWatcherPanicString, "BioWatcher");
inline void PanicWatcher(TBioWatcherPanic aPanic)
	{
	User::Panic(KSocketWatcherPanicString, aPanic);
	}

class CSmsMessage;
class CBaseSmsActiveSocketWatcher;
class CWatcherLog;
class CBIODatabase;
class CSmsSettings;

class MBioWatcherObserver
	{
	public:
		virtual void HandleWatcherComplete(CBaseSmsActiveSocketWatcher& aWatcher, TInt aError) = 0;
	};

class CBaseSmsActiveSocketWatcher : public CActive, public MMsvSessionObserver
	{
public:
	IMPORT_C virtual ~CBaseSmsActiveSocketWatcher();

	// Setup gets called synchronously to set up sockets and
	// mail server before we get going asynchy
	IMPORT_C void SetupL();

	// over-ride setup code here
	virtual void DoSetupL() = 0;
	
	// StartL gets called to kick off the AO putting 
	IMPORT_C void StartL();

	// Sets up the Sockets to wait
	virtual void WaitForMessageL() = 0;
	// Receives the message from the Socket

	virtual void ReceiveL() = 0;

	// Store the message in the mail server
	IMPORT_C void StoreMsgL(CSmsMessage* aSmsMsg);
	IMPORT_C void StoreMsgL(CSmsMessage* aSmsMsg, TBool aCheckForSID);

	inline void SetObserver(MBioWatcherObserver* aObserver);
	inline TUid BioMsgUID() const;

	virtual void DoComplete(TInt&) = 0;
	virtual void DoRunL() = 0;

private:
	void ReplaceTypeMessageL(CMsvEntry&, CSmsMessage& aMessage, TBool& aRetainOriginalMessage);
	void DeleteReplaceTypeMessagesL(CMsvEntry& aEntry, TUint8 aPid, CSmsMessage& aMessage, TBool& aRetainOriginalMessage);

	void CreateMessageL(CMsvEntry& aEntry, CSmsMessage* aMessage, TBool aCheckForSecureId);

	void SetBioServiceId(CMsvSession& aSession);
	void SetBioServiceId(TMsvId aBioServiceId, TMsvId aSmsServiceId);
	TBool CheckMessageExistsL(CMsvSession& aSession, TMsvId aId) const;

	TBool CheckForSID(TPtr& aMessage, TSecureId& aSecureId);
	
private:	// methods from MMsvSessionObserver
	virtual void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};

private:	// methods from CActive
	IMPORT_C virtual void RunL();
	IMPORT_C virtual TInt RunError(TInt aError);
	
private:

#ifndef _BIOMSG_NO_LOGGING
	void LogMessageL(const CSmsMessage& aMessage); //will only leave with KErrNoMemory
	void LogEachLine(const TDesC& aInput);
	void Log(const TDesC& aInput);
	void Log(const TDesC8& aInput);
#endif

protected:
	// Constructor
	IMPORT_C CBaseSmsActiveSocketWatcher(CWatcherLog& iWatcherLog, TInt aPriority, TUid aBioID, RFs& aFs);
	IMPORT_C void Complete(TInt aStatus);
	inline void WatcherComplete(TInt aError);

	inline virtual TBool CanStoreMessage() const;
	inline virtual TBool Visible() const;
	IMPORT_C virtual void RestoreSettingsL(CMsvSession&);
	IMPORT_C virtual void RestoreSettingsL(TMsvId aBioServiceId, TMsvId aSmsServiceId);
	IMPORT_C virtual void ConstructL(CBIODatabase& aBioDb, TMsvId aBioServiceId, TMsvId aSmsServiceId);
	IMPORT_C virtual void SetBioMsgText(CBIODatabase& aBioDb);
	IMPORT_C virtual void PreStoreActionL(CMsvSession& aSession, CSmsMessage& aMessage);

public:
	enum TSmsWatcherState
		{
		ESmsWWaitForMsg,
		ESmsWReadMsg,
		ESmsWRetryError,
		ESmsIsClass0Msg,
		ESmsSupportClass0Msg
		} iState;

protected:

	TUid					iBioMsgUID;
	TMsvId					iBioServiceId;
	TMsvId					iSmsServiceId;
	TBioMsgIdText			iBioMsgText;
	CWatcherLog&			iWatcherLog;
	RTimer					iTimer;
	RFs&					iFs;
	CSmsSettings*			iSettings;

private:

	MBioWatcherObserver*	iObserver;
	CSmsGetDetDescInterface* iGetDetDesc;
	
	};

#include "SmsSocketWatcher.inl"

#endif
