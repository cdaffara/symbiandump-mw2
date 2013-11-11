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

#if !defined(__CIOCMTM_H__)
#define __BIOCMTM_H__
#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif
#if !defined(__MTCLBASE_H__)
#include <mtclbase.h>
#endif

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include <bsp.h>
#endif



// forward declarations
class CBIODatabase;


class CBIOClientMtm : public CBaseMtm
/** BIO client MTM interface.

The BIO client MTM is simply responsible for requesting BIO message parser 
operations to the BIO Server MTM through InvokeAsyncFunctionL(). Many of the 
other base class functions are implemented to do nothing. 
@publishedPartner
@released
*/
	{
public:
	IMPORT_C static CBIOClientMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	~CBIOClientMtm();
	// from MMsvEntryObserver
	void HandleEntryEvent(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	//
	// from CBaseMtm
	void SaveMessageL(); 
	void LoadMessageL();
	CMsvOperation* ReplyL  (TMsvId aDestination, TMsvPartList aPartlist, TRequestStatus& aCompletionStatus);
	CMsvOperation* ForwardL(TMsvId aDestination, TMsvPartList aPartList, TRequestStatus& aCompletionStatus);
	TMsvPartList ValidateMessage(TMsvPartList aPartList);
	TMsvPartList Find(const TDesC& aTextToFind, TMsvPartList aPartList);
	void AddAddresseeL(const TDesC& aRealAddress);
	void AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias);
	void RemoveAddressee(TInt aIndex);
	// --- RTTI functions ---
	TInt QueryCapability(TUid aCapability, TInt& aResponse);
	void InvokeSyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter);
	CMsvOperation*  InvokeAsyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter, TRequestStatus& aCompletionStatus);
protected:
	//
	// from CBaseMtm
	void ContextEntrySwitched();
	//
private:
	CBIOClientMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	void ConstructL();
	//
	void FindInDescriptionL(const TDesC& aTextToFind, TMsvPartList aPartList,TMsvPartList& aFoundList);
	void FindInOriginatorL(const TDesC& aTextToFind, TMsvPartList aPartList,TMsvPartList& aFoundList);
	void FindInBodyL(const TDesC& aTextToFind, TMsvPartList aPartList,TMsvPartList& aFoundList);
private:
	RFs					iFs;
	CBIODatabase*		iBioDatabase;
	};


#endif
