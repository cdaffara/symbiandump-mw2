// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CLNTMTM.H"

#include <msvreg.h>


EXPORT_C CMyClntMtm* CMyClntMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
	{
	CMyClntMtm* myclntmtm=new(ELeave) CMyClntMtm(aRegisteredMtmDll, aSession);
	return myclntmtm;
	}

CMyClntMtm::~CMyClntMtm()
	{
	}

void CMyClntMtm::SaveMessageL()
	{
	CMsvStore* store = iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	StoreBodyL(*store);
	store->CommitL();
	CleanupStack::PopAndDestroy();
	}

void CMyClntMtm::LoadMessageL()
	{
	CMsvStore* store = iMsvEntry->ReadStoreL();
	CleanupStack::PushL(store);
	if (store->HasBodyTextL())
		RestoreBodyL(*store);
	CleanupStack::PopAndDestroy();
	}

CMsvOperation* CMyClntMtm::ReplyL  (TMsvId /*aDestination*/, TMsvPartList /*aPartlist*/, TRequestStatus& /*aCompletionStatus*/)
	{
	return NULL;
	}

CMsvOperation* CMyClntMtm::ForwardL(TMsvId /*aDestination*/, TMsvPartList /*aPartList*/, TRequestStatus& /*aCompletionStatus*/)
	{
	return NULL;
	}

TMsvPartList CMyClntMtm::ValidateMessage(TMsvPartList /*aPartList*/)
	{
	return 0;
	}

TMsvPartList CMyClntMtm::Find(const TDesC& /*aTextToFind*/, TMsvPartList /*aPartList*/)
	{
	return 0;
	}

const CDesCArray& CMyClntMtm::AddresseeList()
	{
	return iDesCArray;
	}

void CMyClntMtm::AddAddresseeL(const TDesC& /*aRealAddress*/)
	{
	}

void CMyClntMtm::AddAddresseeL(const TDesC& /*aRealAddress*/, const TDesC& /*aAlias*/)
	{
	}

void CMyClntMtm::RemoveAddressee(TInt /*aIndex*/)
	{
	}

void CMyClntMtm::ContextEntrySwitched()
	{
	}

void CMyClntMtm::InvokeSyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/)
	{
	User::Leave(KErrNotSupported);
	}

CMsvOperation* CMyClntMtm::InvokeAsyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/, TRequestStatus& aCompletionStatus)
	{
	TRequestStatus* status=&aCompletionStatus;
	User::RequestComplete(status,KErrNotSupported);
	return NULL;;
	}

CMyClntMtm::CMyClntMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
: CBaseMtm(aRegisteredMtmDll, aSession), iDesCArray(8)
	{
	__DECLARE_NAME(_S("CMyClntMtm"));
	}

