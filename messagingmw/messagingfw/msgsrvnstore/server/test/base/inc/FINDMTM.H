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

#ifndef __FINDMTM_H__
#define __FINDMTM_H__

#include <mtclbase.h>

class CMsvFindText;

class CFindMtm : public CBaseMtm
	{
public:
	IMPORT_C static CFindMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	~CFindMtm();
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
	CFindMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	void ConstructL();
	//
	TMsvPartList DoFindSyncL(const TDesC& aTextToFind, TMsvPartList aPartList);
	TBool FindRecipientL(const TDesC& aTextToFind, TMsvPartList aFlags);
	//
	void StoreRecipientsL(CMsvStore& aStore);
	void RestoreRecipientsL(CMsvStore& aStore);
	//
private:
	CMsvFindText* iFindText;
	};

#endif
