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

#ifndef __CSENDASTESTCLIENTMTM_H__
#define __CSENDASTESTCLIENTMTM_H__

#include <mtmuids.h>
#include <mtclbase.h>
#include "csendastestmessage.h"

const TUid KUidMsgTypeSendAsTest	= {0x10204287};

class CSendAsTestClientMtm : public CBaseMtm
	{
public:
	IMPORT_C static CSendAsTestClientMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	virtual ~CSendAsTestClientMtm();

	// from MMsvEntryObserver
	virtual void HandleEntryEvent(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

	// from CBaseMtm
	virtual void SaveMessageL(); 
	virtual void LoadMessageL();
	
	virtual TMsvPartList ValidateMessage(TMsvPartList aPartList);
	virtual TMsvPartList Find(const TDesC& aTextToFind, TMsvPartList aPartList);

	virtual CMsvOperation* ReplyL(TMsvId aDestination, TMsvPartList aPartlist, TRequestStatus& aCompletionStatus);
	virtual CMsvOperation* ForwardL(TMsvId aDestination, TMsvPartList aPartList, TRequestStatus& aCompletionStatus);

	virtual void AddAddresseeL(TMsvRecipientType aType, const TDesC& aRealAddress);
	virtual void AddAddresseeL(TMsvRecipientType aType, const TDesC& aRealAddress, const TDesC& aAlias);
	virtual void AddAddresseeL(const TDesC& aRealAddress);
	virtual void AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias);
	virtual void RemoveAddressee(TInt aIndex);

	virtual void SetSubjectL(const TDesC& aSubject);
	virtual void BioTypeChangedL(TUid aBioTypeUid);

	virtual TInt QueryCapability(TUid aCapability, TInt& aResponse);

	virtual void InvokeSyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter);
	virtual CMsvOperation* InvokeAsyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter, TRequestStatus& aCompletionStatus);

 	virtual TMsvId DefaultServiceL() const;
private:	// from CMtmBase
	virtual void ContextEntrySwitched();
private:
	CSendAsTestClientMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	void ConstructL();
	void DoAddAddresseeL(const TDesC& aAddress, const TDesC& aAlias, CSendAsTestMessage::TSendAsTestAddressType aType);
	void AddToAddresseeListL(const TDesC& aAddress, const TDesC& aAlias);
	void Reset();	
private:
	TMsvId					iServiceId;
	CSendAsTestMessage*		iSendAsTestMessage;
	};

#endif	//  __CSENDASTESTCLIENTMTM_H__
