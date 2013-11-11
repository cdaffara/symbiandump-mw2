// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __PUSHCLIENTMTM_H__
#define __PUSHCLIENTMTM_H__

#include <e32base.h>
#include <mtclbase.h>

/**
 * Skeleton Client MTM for the WAP Push Service.  This is required by the MTM architecture, so that 
 * Push Messages can be saved under a single WAP Push Service Entry. 
 * The Push MTM neither sends nor recieves via a remote server, none of the MTM feature are supported
 * except for Find()
 */

//##ModelId=3B659E3F02B7
class CWapPushClientMtm : public CBaseMtm
/**
@internalTechnology
@released
*/
	{
public:
	//##ModelId=3B659E3F02F6
	IMPORT_C static CWapPushClientMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	//##ModelId=3B659E3F02F5
	~CWapPushClientMtm();
	
    // from MMsvEntryObserver
	//##ModelId=3B659E3F02F0
	void HandleEntryEvent(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	//
	// from CBaseMtm
	//##ModelId=3B659E3F02EF
	void SaveMessageL(); 
	//##ModelId=3B659E3F02EE
	void LoadMessageL();
	//##ModelId=3B659E3F02EA
	CMsvOperation* ReplyL  (TMsvId aDestination, TMsvPartList aPartlist, TRequestStatus& aCompletionStatus);
	//##ModelId=3B659E3F02E2
	CMsvOperation* ForwardL(TMsvId aDestination, TMsvPartList aPartList, TRequestStatus& aCompletionStatus);
	//##ModelId=3B659E3F02E0
	TMsvPartList ValidateMessage(TMsvPartList aPartList);
	//##ModelId=3B659E3F02DD
	TMsvPartList Find(const TDesC& aTextToFind, TMsvPartList aPartList);
	//##ModelId=3B659E3F02DC
#ifndef __MESSAGING_API_V2__
	const CDesCArray& AddresseeList();
#endif	// __MESSAGING_API_V2__
	//##ModelId=3B659E3F02D7
	void AddAddresseeL(const TDesC& aRealAddress);
	//##ModelId=3B659E3F02D9
	void AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias);
	//##ModelId=3B659E3F02D5
	void RemoveAddressee(TInt aIndex);
	// --- RTTI functions ---
	//##ModelId=3B659E3F02D1
	void InvokeSyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter);
	//##ModelId=3B659E3F02CC
	CMsvOperation*  InvokeAsyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter, TRequestStatus& aCompletionStatus);

protected:
	// from CBaseMtm
	//##ModelId=3B659E3F02CB
	void ContextEntrySwitched();

private:
	//##ModelId=3B659E3F02C0
	CWapPushClientMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);

	TMsvPartList DoFindL(const TDesC& aTextToFind, TMsvPartList aPartList);
	};


#endif	// __PUSHCLIENTMTM_H__
