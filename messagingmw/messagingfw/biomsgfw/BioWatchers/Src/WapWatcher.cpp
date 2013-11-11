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
// BioSMSWatcher.CPP
// 
//

#include "WapWatcher.h"
#include <c32comm.h>

#include <biodb.h>
#include "WapSocketWatcher.h"

#include <e32test.h>



#include <ecom/implementationproxy.h>

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x10008C67, CWapWatcher::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

//
// CWapWatcher
//


//
// Construction
//	protected
//
CWapWatcher::CWapWatcher(RFs& aFs, CWatcherLog& aWatcherLog)
: CSmsBaseWatcher(aFs, aWatcherLog, EPriorityStandard + 2)
	{
	}

CWapWatcher::~CWapWatcher()
	{
	}

//
CWapWatcher* CWapWatcher::NewL(TAny* aWatcherParams)
	{
	TWatcherParams* params = reinterpret_cast<TWatcherParams*>(aWatcherParams);
	CWapWatcher* self= new (ELeave) CWapWatcher(params->iFs, params->iLog);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

//
// Construction/destruction

void CWapWatcher::StartL()
	{
	CSmsBaseWatcher::StartL();
	
	GetBioServiceId(*iSession, iBioServiceId, iSmsServiceId);
	
	CreateSocketWatchersFromBioDbL(EBioMsgIdNbs);
	CreateSocketWatchersFromBioDbL(EBioMsgIdWap);
	CreateSocketWatchersFromBioDbL(EBioMsgIdWapSecure);
	CreateSocketWatchersFromBioDbL(EBioMsgIdWsp);
	CreateSocketWatchersFromBioDbL(EBioMsgIdWspSecure);
	StartSocketWatchersL();
	ResetDb(); //these members are only required during construction
	
	delete iSession;
	iSession = NULL;
	}

CBaseSmsActiveSocketWatcher* CWapWatcher::CreateSocketWatcherLC(const TUid aBioUid, const TBioMsgId& aBioMsg)
	{
	if (!SupportBioMsgId(aBioMsg))
		User::Leave(KErrNotSupported);

	CBaseSmsActiveSocketWatcher* portWatcher = CWapPortWatcher::NewL(iBioServiceId, iSmsServiceId, *iBioDb, iWatcherLog, Priority(), aBioUid, iFs, aBioMsg);
	CleanupStack::PushL(portWatcher);
	return portWatcher;
	}

TBool CWapWatcher::SupportBioMsgId(const TBioMsgId& aBioMsg) const
	{
	TBool retVal = aBioMsg.iType == EBioMsgIdWap || aBioMsg.iType == EBioMsgIdWapSecure || aBioMsg.iType == EBioMsgIdWsp || aBioMsg.iType == EBioMsgIdWspSecure;

	if (!retVal)
		{
		retVal = (aBioMsg.iType == EBioMsgIdNbs && !aBioMsg.iText.Length());
		}

	retVal = retVal && (aBioMsg.iPort > 0);

	return retVal;
	}

void CWapWatcher::AddBifL(TUid aBioID)
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioWap: AddBifL(BioUid: %d)"), aBioID.iUid));

	AddBifWithTypeL(EBioMsgIdWap, aBioID);
	AddBifWithTypeL(EBioMsgIdWapSecure, aBioID);
	AddBifWithTypeL(EBioMsgIdWsp, aBioID);
	AddBifWithTypeL(EBioMsgIdWspSecure, aBioID);
	AddBifWithTypeL(EBioMsgIdNbs, aBioID);

	ResetDb();
	}
