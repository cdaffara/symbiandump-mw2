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

#if !defined(__CLNTMTM_H__)
#define __CLNTMTM_H__
#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif
#if !defined(__MTCLBASE_H__)
#include <mtclbase.h>
#endif

const TUid KDummySrvMtmVersion1Uid={268440313};
const TUid KDummySrvMtmTypeUid={268440314};


class CMyClntMtm : public CBaseMtm
	{
public:
	IMPORT_C static CMyClntMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	~CMyClntMtm();
	// from CBaseMtm
	void SaveMessageL(); 
	void LoadMessageL();
	CMsvOperation* ReplyL  (TMsvId aDestination, TMsvPartList aPartlist, TRequestStatus& aCompletionStatus);
	CMsvOperation* ForwardL(TMsvId aDestination, TMsvPartList aPartList, TRequestStatus& aCompletionStatus);
	TMsvPartList ValidateMessage(TMsvPartList aPartList);
	TMsvPartList Find(const TDesC& aTextToFind, TMsvPartList aPartList);
	const CDesCArray& AddresseeList();
	void AddAddresseeL(const TDesC& aRealAddress);
	void AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias);
	void RemoveAddressee(TInt aIndex);
	void InvokeSyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter);
	CMsvOperation*  InvokeAsyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter, TRequestStatus& aCompletionStatus);
protected:
	//
	// from CBaseMtm
	void ContextEntrySwitched();
	//
private:
	CMyClntMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	//
private:
	CDesCArrayFlat iDesCArray;
	};

#endif
