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

#if !defined(__SVRMTM_H__)
#define __SVRMTM_H__
#if !defined(__MTSR_H__)
#include <mtsr.h>
#endif
#if !defined(__MSVENTRY_H__)
#include <msventry.h>
#endif

const TUid KDummySrvMtmVersion1Uid={268440313};
const TUid KDummySrvMtmTypeUid={268440314};


class CMySvrMtm : public CBaseServerMtm
	{
public:
	IMPORT_C static CMySvrMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry);
	~CMySvrMtm();

	void CopyToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void CopyFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void CopyWithinServiceL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void DeleteL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);
	void DeleteAllL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);
	void CreateL(TMsvEntry aNewEntry, TRequestStatus& aStatus);
	void ChangeL(TMsvEntry aNewEntry, TRequestStatus& aStatus);
	void MoveToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void MoveFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void MoveWithinServiceL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	//
	void StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus);
	//
	TBool CommandExpected();
	//
	const TDesC8& Progress();

protected:
	void DoCancel();
	void DoRunL();
	void DoComplete(TInt aError);

private:
	CMySvrMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry);
	void ConstructL();

private:
	TBuf8<10> iProgress;
	};

#endif
