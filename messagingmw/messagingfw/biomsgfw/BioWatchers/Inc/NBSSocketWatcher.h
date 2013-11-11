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

#ifndef __NBSSOCKETWATCHER_H__
#define __NBSSOCKETWATCHER_H__

#include <s32mem.h>
#include <e32base.h>
#include <smsuaddr.h>
#include <gsmumsg.h>
#include <gsmubuf.h>
#include <smsustrm.h>
#include <smutset.h>
#include "SmsSocketWatcher.h"
#include "class0sms.h"

class CSmsHeader;

//UID for Class 0 SMS Implementation
const TUid KUidClass0SmsHandler = {0x2000BDF1};

#include "SmsSocketWatcher.h"

class CNbsSmsSocketWatcher : public CBaseSmsActiveSocketWatcher
	{
public:
	static CNbsSmsSocketWatcher* NewLC(TMsvId aBioServiceId, 
									   TMsvId aSmsServiceId, 
									   CBIODatabase& aBioDb, 
									   CWatcherLog& aLog, 
									   TInt aPriority, 
									   TUid aBioID, 
									   RFs& aFs, 
									   const TDesC& 
									   aNbsPattern, 
									   TSmsAddrFamily aAddrFamily = ESmsAddrMatchText);
	virtual ~CNbsSmsSocketWatcher();

public:	// methods from CBaseSmsActiveSocketWatcher
	virtual void DoSetupL();
	virtual void WaitForMessageL();
	virtual void ReceiveL();
	virtual void DoComplete(TInt&);
	virtual void DoRunL();

public:	// methods from CActive
	virtual void DoCancel();
	void CheckForClass0SmsSupport();
	void SupportForClass0SmsL();

protected:
	CNbsSmsSocketWatcher(CWatcherLog& aLog, TInt aPriority, TUid aBioID, RFs& aFs, const TDesC& aNbsPattern, TSmsAddrFamily aAddrFamily);
	void ConstructL(TMsvId aBioServiceId, TMsvId aSmsServiceId, CBIODatabase& aBioDb);
	void DoReceiveL();
	void ExternalizeL(RBufWriteStream& aWriteStream, TInt aStartPos, TInt endPos, TBool aIsLastMessage, TPtr aData);
		
private:
	TBool MessageHasPort(const CSmsMessage& aSmsMessage);
	TBool IsJavaTextMessage(const CSmsMessage& aSmsMessage);
	
protected:
	TSmsAddrFamily			iAddrFamily;
	TBioMsgIdText			iNbsPattern;
	TSmsAddr				iSmsAddr;
	TPckgBuf<TUint>			iOctlRes;
	RSocketServ				iSocketServer;
	RSocket					iSocket;
	TBool					iIoCtlActive;
	
	TUid					iDtor_ID_Key;
	CSmsClass0Base* 		iSmsClass0;
	TInt					iMsgRefNumber;
	};

/**
Handles Status Reports and Special (Message Waiting) Messages

@internalComponent
@released
*/
class CSpecialNbsSmsSocketWatcher : public CNbsSmsSocketWatcher
	{
public:
	static CSpecialNbsSmsSocketWatcher* NewL(TMsvId aBioServiceId, TMsvId aSmsServiceId, CBIODatabase& aBioDb, CWatcherLog& aLog, TInt aPriority, TUid aBioID, RFs& aFs, const TDesC& aNbsPattern, TSmsAddrFamily aAddrFamily);
	static CSpecialNbsSmsSocketWatcher* NewLC(TMsvId aBioServiceId, TMsvId aSmsServiceId, CBIODatabase& aBioDb, CWatcherLog& aLog, TInt aPriority, TUid aBioID, RFs& aFs, const TDesC& aNbsPattern, TSmsAddrFamily aAddrFamily);
	virtual ~CSpecialNbsSmsSocketWatcher();

protected:
	CSpecialNbsSmsSocketWatcher(CWatcherLog& aLog, TInt aPriority, TUid aBioID, RFs& aFs, const TBioMsgIdText& aNbsPattern, TSmsAddrFamily aAddrFamily);
	
protected:	// methods from CBaseSmsActiveSocketWatcher
	virtual inline TBool CanStoreMessage() const;
	virtual inline TBool Visible() const;
	virtual void RestoreSettingsL(CMsvSession&);
	virtual void RestoreSettingsL(TMsvId aBioServiceId, TMsvId aSmsServiceId);
	virtual void SetBioMsgText(CBIODatabase& aBioDb);
	virtual void PreStoreActionL(CMsvSession& aSession, CSmsMessage& aSmsMessage);

protected:
	void ConstructL(TMsvId aBioServiceId, TMsvId aSmsServiceId, CBIODatabase& aBioDb);
	void MatchStatusReportL(CMsvSession& aSession, TLogId aLogId, TSmsStatus::TSmsStatusValue aDeliveryStatus);
	TBool SearchForMessageL(CMsvEntry& aMsvEntry, CSmsHeader& aHeader, const CMsvEntrySelection& aSelection, TLogId aLogId, TMsvId& aMessageId);
	void UpdateMessageEntryL(CMsvEntry& aMsvEntry, CSmsHeader& aHeader, TMsvId aMessageId, TLogId aLogId, TSmsStatus::TSmsStatusValue aDeliveryStatus);

protected:
	CSmsSettings::TSmsReportHandling iReportHandling;

	};

#include "NBSSocketWatcher.inl"

#endif	// __NBSSOCKETWATCHER_H__
