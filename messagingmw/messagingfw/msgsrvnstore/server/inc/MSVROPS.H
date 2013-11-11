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

#if !defined(__MSVROPS_H__)
#define __MSVROPS_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVOP_H__)
#include "MSVOP.H"
#endif

class CBaseServerMtm;

//**********************************
// CMsvMtmOperation
//**********************************
//
//
//
class CMsvMtmOperation : public CMsvServerOperation
/**
@internalComponent
@released
*/
	{
private:
	enum TMtmOpType {	EMtmOpNone,
						EMtmOpCopyToLocal,
						EMtmOpCopyFromLocal,
						EMtmOpCopyWithinService,
						EMtmOpMoveToLocal,
						EMtmOpMoveFromLocal,
						EMtmOpMoveWithinService,
						EMtmOpDeleteAll,
						EMtmOpCreate,
						EMtmOpChange,
						EMtmOpCommand,
						EMtmOpChangeEntries};
public:
	static CMsvMtmOperation* NewL(const RMessage2& aMessage, TMsvOp aId, TUid aMtmUid, TMsvId aServiceId, TInt aSessionId, MMsvOperationObserver& aOpObserver);
	~CMsvMtmOperation();
	// from CMsvServerOperation
	const TDesC8& Progress();
	TInt SystemProgress(TMsvSystemProgress& aSysProgress);
	// general start function
	void Start(CBaseServerMtm& aMtm, TBool aCapabilityCheck);
	void Failed(TInt aError);
	// 
	// various operation that can be done
	inline void CopyToLocal(CMsvEntrySelection* aSelection, TMsvId aDestination);
	inline void CopyFromLocal(CMsvEntrySelection* aSelection, TMsvId aDestination);
	inline void CopyWithinService(CMsvEntrySelection* aSelection, TMsvId aDestination);
	inline void MoveToLocal(CMsvEntrySelection* aSelection, TMsvId aDestination);
	inline void MoveFromLocal(CMsvEntrySelection* aSelection, TMsvId aDestination);
	inline void MoveWithinService(CMsvEntrySelection* aSelection, TMsvId aDestination);
	inline void DeleteAll(CMsvEntrySelection* aSelection);
	inline void CreateL(const TMsvEntry& aNewEntry);
	inline void ChangeL(const TMsvEntry& aNewEntry);
	inline void ChangeEntriesL(CMsvEntrySelection* aSelection, TInt aMark);
	inline void StartCommand(CMsvEntrySelection* aSelection, TInt aCommand, HBufC8* aParameter);
#if (defined SYMBIAN_USER_PROMPT_SERVICE)	
 	inline void SetThreadId(TThreadId aThreadId);
 	inline void SetCapability(TBool aCap);
 	inline TBool Capability();
#endif
	//
protected:
	void DoCancel();
	void RunL();
	//
private:
	CMsvMtmOperation(const RMessage2& aMessage, TMsvOp aId, TUid aMtmUid, TMsvId aServiceId, TInt aSessionId, MMsvOperationObserver& aOpObserver);
	void ConstructL();
	//
	void StoreParametersL(TMtmOpType aOpType, const TMsvEntry& aNewEntry);
	void StoreParameters(TMtmOpType aOpType, CMsvEntrySelection* aSelection, TInt aIntParam, HBufC8* aDesParam);
	void DoStartL(TBool aCapabilityCheck);
	//
private:
	TMtmOpType iOpType;
	CBaseServerMtm* iServerMtm;
	MMsvOperationObserver& iOpObserver;
	//
	TMsvEntry* iEntry;
	CMsvEntrySelection* iSelection;
	TInt iIntParam;
	HBufC* iBuf1;
	HBufC* iBuf2;
	HBufC8* iDesParam;
#if (defined SYMBIAN_USER_PROMPT_SERVICE)	
 	TThreadId iThreadId;
 	TBool iHasCapability;
#endif
	};

#include "MSVROPS.INL"

#endif
