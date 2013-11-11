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

#include <mtmuids.h>
#include <mtclbase.h>


class CRegMtm : public CBaseMtm
	{
public:
	IMPORT_C static CRegMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	~CRegMtm();
	//
	// from MMsvEntryObserver
	void HandleEntryEvent(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	//
	// from CBaseMtm
	void SaveMessageL(); 
	void LoadMessageL();
	CMsvOperation* ReplyL  (TMsvId aDestination,   TMsvPartList aPartlist, TRequestStatus& aCompletionStatus);
	CMsvOperation* ForwardL(TMsvId aDestination, TMsvPartList aPartList, TRequestStatus& aCompletionStatus);
	TMsvPartList ValidateMessage(TMsvPartList aPartList);
	TMsvPartList Find(const TDesC& aTextToFind, TMsvPartList aPartList);
	const CMsvRecipientList& AddresseeList();
	void AddAddresseeL(const TDesC& aRealAddress);
	void AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias);
	void RemoveAddressee(TInt aIndex);
	void InvokeSyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter);
	CMsvOperation*  InvokeAsyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter, TRequestStatus& aCompletionStatus);
	//
	TInt QueryCapability(TUid aCapability, TInt& aResponse); 
	virtual void SetSubjectL(const TDesC& aSubject); 
protected:
	//
	// from CBaseMtm
	void ContextEntrySwitched();
	//
private:
	CRegMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	//
private:
	};



EXPORT_C CRegMtm* CRegMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
	{
	CRegMtm* myclntmtm=new(ELeave) CRegMtm(aRegisteredMtmDll, aSession);
	return myclntmtm;
	}

CRegMtm::~CRegMtm()
	{
	}

void CRegMtm::HandleEntryEvent(TMsvEntryEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	}

void CRegMtm::SaveMessageL()
	{
	CMsvStore* store = iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	StoreBodyL(*store);
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store
	}

void CRegMtm::LoadMessageL()
	{
	CMsvStore* store = iMsvEntry->ReadStoreL();
	CleanupStack::PushL(store);
	RestoreBodyL(*store);
	CleanupStack::PopAndDestroy(); // store
	}

CMsvOperation* CRegMtm::ReplyL  (TMsvId /*aDestination*/,   TMsvPartList /*aPartlist*/, TRequestStatus& /*aCompletionStatus*/)
	{
	return NULL;
	}

CMsvOperation* CRegMtm::ForwardL(TMsvId /*aDestination*/, TMsvPartList /*aPartList*/, TRequestStatus& /*aCompletionStatus*/)
	{
	return NULL;
	}

TMsvPartList CRegMtm::Find(const TDesC& /*aTextToFind*/, TMsvPartList /*aPartList*/)
	{
	return 0;
	}

const CMsvRecipientList& CRegMtm::AddresseeList()
	{
	return *iAddresseeList;
	}

void CRegMtm::AddAddresseeL(const TDesC& aRealAddress)
	{
	iAddresseeList->AppendL(aRealAddress);
	}

void CRegMtm::AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias)
	{
	TBuf<256> buf;
	buf.Copy(aAlias);
	buf.Append(_L(" <"));
	buf.Append(aRealAddress);
	buf.Append(_L(">"));
	iAddresseeList->AppendL(buf);
	}

void CRegMtm::RemoveAddressee(TInt aIndex)
	{
	iAddresseeList->Delete(aIndex);
	}

void CRegMtm::ContextEntrySwitched()
	{
	}

void CRegMtm::InvokeSyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/)
	{
	User::Leave(KErrNotSupported);
	}

CMsvOperation* CRegMtm::InvokeAsyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/, TRequestStatus& aCompletionStatus)
	{
	TRequestStatus* status=&aCompletionStatus;
	User::RequestComplete(status,KErrNotSupported);
	return NULL;;
	}

TInt CRegMtm::QueryCapability(TUid /*aCapability*/, TInt& /*aResponse*/)
	{
	return KErrNotSupported;
	}

void CRegMtm::SetSubjectL(const TDesC& /*aSubject*/)
	{
	}

TUint CRegMtm::ValidateMessage(TMsvPartList /*aPartList*/)
	{
	return 0;
	}

CRegMtm::CRegMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
: CBaseMtm(aRegisteredMtmDll, aSession)
	{
	__DECLARE_NAME(_S("CSendMtm"));
	}
