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

#include "NbsWatcher.h"
#include <biodb.h>

#include "NBSSocketWatcher.h"

#include <e32test.h>
#include <watcher.h>

//
// DLL Global Methods
//


#include <ecom/implementationproxy.h>

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x10008C66, CNbsWatcher::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}


//
// NbsWatcher
//

//
// Construction
//
CNbsWatcher::CNbsWatcher(RFs& aFs, CWatcherLog& aLog)
: CSmsBaseWatcher(aFs, aLog, EPriorityStandard + 1)
	{
	}
	
CNbsWatcher::~CNbsWatcher()
	{
	}

//
CNbsWatcher* CNbsWatcher::NewL(TAny* aWatcherParams)
	{
	TWatcherParams* params = reinterpret_cast<TWatcherParams*>(aWatcherParams);
	CNbsWatcher* self= new (ELeave) CNbsWatcher(params->iFs, params->iLog);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
void CNbsWatcher::ConstructL()
	{
	// CSmsBaseWatcher::ConstructL calls SetActive
	CSmsBaseWatcher::ConstructL();
	}

//
// Construction/destruction

void CNbsWatcher::StartL()
	{
	CSmsBaseWatcher::StartL();

	GetBioServiceId(*iSession, iBioServiceId, iSmsServiceId);

	CreateSocketWatchersFromBioDbL(EBioMsgIdNbs);

	// Set up a SMS Watcher for Special Messages
	SetupSpecialWatcherL(ESmsAddrMessageIndication);

	// Set up a SMS Watcher for Status Reports
	SetupSpecialWatcherL(ESmsAddrStatusReport);

	// Set up the default NULL SMS message
	CBaseSmsActiveSocketWatcher* nbsWatcher = CNbsSmsSocketWatcher::NewLC(iBioServiceId, iSmsServiceId, *iBioDb, iWatcherLog, Priority(), KNullUid, iFs, KNullDesC, ESmsAddrRecvAny);
	TInt error = SetupAndAppendL(*nbsWatcher, *iSocketWatchers);
	User::LeaveIfError(error); //need to make sure the ReceiveAny watcher is working
	iReceiveAnySocket = nbsWatcher;
	CleanupStack::Pop(); //nbsWatcher

	StartSocketWatchersL();

	ResetDb(); //these members are only required during construction

	// delete the session with the message server
	delete iSession;
	iSession = NULL;	
	}

CBaseSmsActiveSocketWatcher* CNbsWatcher::CreateSocketWatcherLC(const TUid aBioUid, const TBioMsgId& aBioMsg)
	{
	if (!SupportBioMsgId(aBioMsg))
		User::Leave(KErrNotSupported);

	__ASSERT_DEBUG(iBioDb, PanicWatcher(EObjectNotConstructed));

	return CNbsSmsSocketWatcher::NewLC(iBioServiceId, iSmsServiceId, *iBioDb, iWatcherLog, Priority(), aBioUid, iFs, aBioMsg.iText);
	}

TBool CNbsWatcher::SupportBioMsgId(const TBioMsgId& aBioMsg) const
	{
	return aBioMsg.iType == EBioMsgIdNbs && aBioMsg.iPort <= 0 && aBioMsg.iText.Length();
	}

void CNbsWatcher::AddBifL(TUid aBioID)
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L8("BioNbs: AddBifL(BioUid: %d)"), aBioID.iUid));

	// Handle the case where the watchers are not working
	if (!iReceiveAnySocket)
		User::Leave(KErrNotSupported);

	iReceiveAnySocket->Cancel();

	TRAPD(err, AddBifWithTypeL(EBioMsgIdNbs, aBioID));

	iReceiveAnySocket->SetupL();
	iReceiveAnySocket->StartL();

	ResetDb();

	User::LeaveIfError(err);
	}

void CNbsWatcher::SetupSpecialWatcherL(TSmsAddrFamily aAddrFamily)
	{
	CBaseSmsActiveSocketWatcher* nbsWatcher = NULL;

	//The following function will leave with KErrNotSupported
	//if the CSmsSettings stored against the SMS Service indicates
	//that this message type should not be watched

	BIOWATCHERLOG(iWatcherLog.Printf(_L8("BioNbs: SetupSpecialWatcher [aAddrFamily=%d]"), aAddrFamily));
	__ASSERT_DEBUG(iBioDb, PanicWatcher(EObjectNotConstructed));

	TRAPD(error, nbsWatcher = CSpecialNbsSmsSocketWatcher::NewL(iBioServiceId, iSmsServiceId, *iBioDb, iWatcherLog, Priority(), KNullUid, iFs, KNullDesC, aAddrFamily));

	if (error == KErrNone)
		{
		CleanupStack::PushL(nbsWatcher);
		error = SetupAndAppendL(*nbsWatcher, *iSocketWatchers); //Returns either KErrNotFound or KErrAlreadyExists, otherwise leaves

		if (error != KErrNone)
			CleanupStack::PopAndDestroy(nbsWatcher);
		else
			CleanupStack::Pop(nbsWatcher);
		}

	if (error != KErrNone)
		{
		BIOWATCHERLOG(iWatcherLog.Printf(_L8("BioNbs: SetupSpecialWatcher Failed [error=%d]"), error));

		if (error == KErrNoMemory)
			User::Leave(error);
		//else ignore the error
		}
	}
