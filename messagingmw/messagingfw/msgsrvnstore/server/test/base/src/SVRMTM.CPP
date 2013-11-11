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

#include "SVRMTM.H"

EXPORT_C CMySvrMtm* CMySvrMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry)
	{
	CleanupStack::PushL(aInitialEntry); // Take ownership of aInitialEntry
	CMySvrMtm* mysvrmtm=new(ELeave) CMySvrMtm(aRegisteredMtmDll, aInitialEntry);
	CleanupStack::Pop(); // aInitialEntry - Entry now safely stored in member

	CleanupStack::PushL(mysvrmtm);
	mysvrmtm->ConstructL();
	CleanupStack::Pop();
	return mysvrmtm;
	}

CMySvrMtm::~CMySvrMtm()
	{
	}

void CMySvrMtm::CopyToLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CMySvrMtm::CopyFromLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CMySvrMtm::CopyWithinServiceL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CMySvrMtm::MoveToLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CMySvrMtm::MoveFromLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CMySvrMtm::MoveWithinServiceL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CMySvrMtm::DeleteL(const CMsvEntrySelection& /*aSelection*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CMySvrMtm::DeleteAllL(const CMsvEntrySelection& /*aSelection*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CMySvrMtm::CreateL(TMsvEntry /*aNewEntry*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CMySvrMtm::ChangeL(TMsvEntry /*aNewEntry*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CMySvrMtm::StartCommandL(CMsvEntrySelection& /*aSelection*/, TInt /*aCommand*/, const TDesC8& /*aParameter*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

TBool CMySvrMtm::CommandExpected()
	{
	return EFalse;
	}

const TDesC8& CMySvrMtm::Progress()
	{
	return iProgress;
	}

void CMySvrMtm::DoCancel()
	{
	}

void CMySvrMtm::DoRunL()
	{
	}

void CMySvrMtm::DoComplete(TInt /*aError*/)
	{
	}


CMySvrMtm::CMySvrMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry):
	CBaseServerMtm(aRegisteredMtmDll, aInitialEntry)
	{
	__DECLARE_NAME(_S("CMyMtm"));
	}
		
void CMySvrMtm::ConstructL()
	{
	}

