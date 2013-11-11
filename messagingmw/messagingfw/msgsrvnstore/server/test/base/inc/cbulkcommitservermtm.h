// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CBULKCOMMITSERVERMTM_H__
#define __CBULKCOMMITSERVERMTM_H__

#include "tbulkcommitservermtmprogress.h"
#include <mtsr.h>

const TUid KUidBulkCommitTestMtm = {0x10273EDD}; //KUidRajaSekharChowdary
	
class CBulkCommitServerMtm : public CBaseServerMtm
	{
public:
	
	IMPORT_C static CBulkCommitServerMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry& aServerEntry);
	~CBulkCommitServerMtm();

	void			StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus);
	void			CopyToLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/);
	void			CopyFromLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/);
	void			CopyWithinServiceL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/);
	void			DeleteAllL(const CMsvEntrySelection& /*aSelection*/, TRequestStatus& /*aStatus*/);
	void			CreateL(TMsvEntry /*aEntry*/, TRequestStatus& /*aStatus*/);
	void			ChangeL(TMsvEntry /*aEntry*/, TRequestStatus& /*aStatus*/);
	TBool			CommandExpected();
	const TDesC8&	Progress();
	void			MoveToLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/);
	void			MoveFromLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/);
	void			MoveWithinServiceL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/);
	void			DoCancel();
	void			DoRunL();
	void			DoComplete(int /*i*/);

private:
	CBulkCommitServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry);
	void			ConstructL();	
	void			DoStartCommandL(CMsvEntrySelection& aSelection, TBulkCommitTestOperation aCommand, const TDesC8& aParameter, TRequestStatus& aStatus);						
	void			CreateBulkEntryWithMultipleParentsL(TUint aEntryCount);
	void			CreateBulkEntries(TUint aEntryCount, TMsvId aServiceId);
private:
	TMsvId			iServiceId;
	TBulkCommitServerMtmProgressBuf	iProgress;
	};

class TTestMsvEntry : public TMsvEntry
	{
public:
	void SetOwner(TBool aFlag) {TMsvEntry::SetOwner(aFlag);};
	void SetId(TMsvId aId) {TMsvEntry::SetId(aId);};
	void SetParent(TMsvId aId) {TMsvEntry::SetParent(aId);};
	void SetDeleted(TBool aFlag) {TMsvEntry::SetDeleted(aFlag);};
	TInt PcSyncCount() {return TMsvEntry::PcSyncCount();};
	TInt Reserved() {TInt* ptr = (TInt*)&iServiceId; return *--ptr;};
	void SetReserved(TInt aValue) {TInt* ptr = (TInt*)&iServiceId; *--ptr=aValue;};
	void SetData(TInt aData) {*(((TInt*)this)+2) = aData;};
	};
	
#endif // __CBULKCOMMITSERVERMTM_H__
