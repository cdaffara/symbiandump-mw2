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

#include <mtmuids.h>
#include <mtclbase.h>

_LIT(KTestSendas2MtmDefaultsFolder, "c:\\test\\sendas2\\%08X.default");

class CSendMtm : public CBaseMtm
	{
public:
	IMPORT_C static CSendMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	~CSendMtm();
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
	//const CDesCArray& AddresseeList();
	const CMsvRecipientList& AddresseeList();
	void AddAddresseeL(const TDesC& aRealAddress);
	void AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias);
	void RemoveAddressee(TInt aIndex);
	void InvokeSyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter);
	CMsvOperation*  InvokeAsyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter, TRequestStatus& aCompletionStatus);
	//
	TInt QueryCapability(TUid aCapability, TInt& aResponse); 
	virtual void SetSubjectL(const TDesC& aSubject); 
	//
 	virtual TMsvId DefaultServiceL() const;
 	virtual void RemoveDefaultServiceL();
 	virtual void ChangeDefaultServiceL(const TMsvId& aService);
 	//
	virtual void AddAddresseeL(TMsvRecipientType aType, const TDesC& aRealAddress);
	virtual void AddAddresseeL(TMsvRecipientType aType, const TDesC& aRealAddress, const TDesC& aAlias);
protected:
	//
	// from CBaseMtm
	void ContextEntrySwitched();
	//
private:
	CSendMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	//
private:
	RFs& iFs;
	};



EXPORT_C CSendMtm* CSendMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
	{
	CSendMtm* myclntmtm=new(ELeave) CSendMtm(aRegisteredMtmDll, aSession);
	return myclntmtm;
	}

CSendMtm::~CSendMtm()
	{
	}

void CSendMtm::HandleEntryEvent(TMsvEntryEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	}

void CSendMtm::SaveMessageL()
	{
	CMsvStore* store = iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	StoreBodyL(*store);
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store
	}

void CSendMtm::LoadMessageL()
	{
	CMsvStore* store = iMsvEntry->ReadStoreL();
	CleanupStack::PushL(store);
	RestoreBodyL(*store);
	CleanupStack::PopAndDestroy(); // store
	}

CMsvOperation* CSendMtm::ReplyL(TMsvId /*aDestination*/, TMsvPartList /*aPartlist*/, TRequestStatus& /*aCompletionStatus*/)
	{
	return NULL;
	}

CMsvOperation* CSendMtm::ForwardL(TMsvId /*aDestination*/, TMsvPartList /*aPartList*/, TRequestStatus& /*aCompletionStatus*/)
	{
	return NULL;
	}

TMsvPartList CSendMtm::Find(const TDesC& /*aTextToFind*/, TMsvPartList /*aPartList*/)
	{
	return 0;
	}

const CMsvRecipientList& CSendMtm::AddresseeList()
	{
	return *iAddresseeList;
	}

void CSendMtm::AddAddresseeL(const TDesC& aRealAddress)
	{
	iAddresseeList->AppendL(aRealAddress);
	}

void CSendMtm::AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias)
	{
	TBuf<256> buf;
	buf.Copy(aAlias);
	buf.Append(_L(" <"));
	buf.Append(aRealAddress);
	buf.Append(_L(">"));
	iAddresseeList->AppendL(buf);
	}

void CSendMtm::RemoveAddressee(TInt aIndex)
	{
	iAddresseeList->Delete(aIndex);
	}

void CSendMtm::ContextEntrySwitched()
	{
	}

void CSendMtm::InvokeSyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/)
	{
	User::Leave(KErrNotSupported);
	}

CMsvOperation* CSendMtm::InvokeAsyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/, TRequestStatus& aCompletionStatus)
	{
	TRequestStatus* status=&aCompletionStatus;
	User::RequestComplete(status,KErrNotSupported);
	return NULL;;
	}

TMsvId CSendMtm::DefaultServiceL() const
	{
	TFileName filename;
	filename.Format(KTestSendas2MtmDefaultsFolder, Type().iUid);
	TPckgBuf<TMsvId> msvbuf;
	RFile file;
	CleanupClosePushL(file);
	
	if (file.Open(iFs, filename, EFileRead) == KErrNone)
		{
		User::LeaveIfError(file.Read(msvbuf));
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	CleanupStack::PopAndDestroy(&file);
	return msvbuf();		
	}

void CSendMtm::RemoveDefaultServiceL()
	{
	TFileName filename;
	filename.Format(KTestSendas2MtmDefaultsFolder, Type().iUid);
	iFs.Delete(filename);
	}

void CSendMtm::ChangeDefaultServiceL(const TMsvId& aService)
	{
	TFileName filename;
	filename.Format(KTestSendas2MtmDefaultsFolder, Type().iUid);
	TPckgBuf<TMsvId> msvbuf;
	msvbuf() = aService;
	RFile file;
	CleanupClosePushL(file);
	iFs.MkDirAll(filename);
	User::LeaveIfError(file.Replace(iFs, filename, EFileRead) == KErrNone);
	User::LeaveIfError(file.Write(msvbuf));
	CleanupStack::PopAndDestroy(&file);
	}
	
CSendMtm::CSendMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession) :
	CBaseMtm(aRegisteredMtmDll, aSession), iFs(aSession.FileSession())
	{
	__DECLARE_NAME(_S("CSendAs2Mtm"));
	}


