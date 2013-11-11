// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MSVSYSPROGRESSTESTMTM_H__
#define __MSVSYSPROGRESSTESTMTM_H__

#include <tmsvsystemprogress.h>
#include <mtsr.h>

const TUid KUidSysProgressTestMtm = {0x1028308F}; //KUidRajaSekharChowdary
	
class CMsvSysProgressTestMtm : public CBaseServerMtm
	{
public:
	
	IMPORT_C static CMsvSysProgressTestMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry& aServerEntry);
	~CMsvSysProgressTestMtm();

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
protected:
	IMPORT_C TInt Extension_(TUint aExtensionId, TAny *&a0, TAny *a1);
private:
	CMsvSysProgressTestMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry);
	void			ConstructL();	
	void			DoStartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus);						
	void			CreateBulkEntryWithMultipleParentsL(TUint aEntryCount);
	void			CreateBulkEntries(TUint aEntryCount, TMsvId aServiceId);
	void			GetSystemProgress(TMsvSystemProgress& aOutSysProg);
private:
	TMsvId			iServiceId;
	TMsvSystemProgress	iSysProgress;
	TPckgBuf<TMsvSystemProgress> iProgress;
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

#endif // __MSVSYSPROGRESSTESTMTM_H__