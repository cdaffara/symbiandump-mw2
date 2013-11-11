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

inline TMsvTestDllInfo::TMsvTestDllInfo(const TDesC& aFileName, TInt aOrdinal, TVersion aVersion)
: iFileName(aFileName), iOrdinal(aOrdinal), iVersion(aVersion)
	{
	}

inline void CMsvTestUtils::CreateAndSetEntryL(TMsvEntry& aEntry)
	{
	CreateEntryL(aEntry);
	SetEntryL(aEntry.Id());
	}

inline void CMsvTestUtils::CreateEntryL(TMsvEntry& aEntry)
	{
	if (iClientServer == EServerSide)
		User::LeaveIfError(iServerEntry->CreateEntry(aEntry));
	else if (iClientServer == EClientSide)
		iMsvEntry->CreateL(aEntry);
	else
		User::Leave(KErrNotSupported);
	}

inline void CMsvTestUtils::DeleteEntryL(TMsvId aId)
	{
	if (iClientServer == EServerSide)
		User::LeaveIfError(iServerEntry->DeleteEntry(aId));
	else if (iClientServer == EClientSide)
		iMsvEntry->DeleteL(aId);
	else
		User::Leave(KErrNotSupported);
	}

inline void CMsvTestUtils::ChangeEntryL(TMsvEntry& aEntry)
	{
	if (iClientServer == EServerSide)
		User::LeaveIfError(iServerEntry->ChangeEntry(aEntry));
	else if (iClientServer == EClientSide)
		iMsvEntry->ChangeL(aEntry);
	else
		User::Leave(KErrNotSupported);
	}

inline void CMsvTestUtils::SetEntryL(TMsvId aId)
	{
	if (iClientServer == EServerSide)
		User::LeaveIfError(iServerEntry->SetEntry(aId));
	else if (iClientServer == EClientSide)
		iMsvEntry->SetEntryL(aId);
	else
		User::Leave(KErrNotSupported);
	}

inline CMsvStore* CMsvTestUtils::EditStoreL()
	{
	CMsvStore* store = NULL ;

	if (iClientServer == EServerSide)
		store = iServerEntry->EditStoreL();
	else if (iClientServer == EClientSide)
		store = iMsvEntry->EditStoreL();
	else
		User::Leave(KErrNotSupported);

	return store;
	}

inline CMsvStore* CMsvTestUtils::ReadStoreL()
	{
	CMsvStore* store = NULL;

	if (iClientServer == EServerSide)
		store = iServerEntry->ReadStoreL();
	else if (iClientServer == EClientSide)
		store = iMsvEntry->ReadStoreL();
	else
		User::Leave(KErrNotSupported);

	return store;
	}

inline TMsvEntry CMsvTestUtils::Entry()
	{
	TMsvEntry entry;
	
	if (iClientServer == EServerSide)
		{
		entry = iServerEntry->Entry();
		}
	else if (iClientServer == EClientSide)
		{
		entry = iMsvEntry->Entry();
		}
	else
		{
		Panic(KErrNotSupported);
		}

	return entry;
	}

inline CMsvEntrySelection* CMsvTestUtils::ChildrenWithTypeLC(TUid aType)
	{
	CMsvEntrySelection* sel = NULL;

	if (iClientServer == EServerSide)
		{
		sel = new (ELeave) CMsvEntrySelection();
		CleanupStack::PushL(sel);
		iServerEntry->GetChildrenWithType(aType, *sel);
		}
	else if (iClientServer == EClientSide)
		{
		sel = iMsvEntry->ChildrenWithTypeL(aType);
		CleanupStack::PushL(sel);
		}
	else
		{
		Panic(KErrNotSupported);
		}

	return sel;
	}

inline CMsvEntrySelection* CMsvTestUtils::ChildrenWithMtmLC(TUid aMtm)
	{
	CMsvEntrySelection* sel = NULL;

	if (iClientServer == EServerSide)
		{
		sel = new (ELeave) CMsvEntrySelection();
		CleanupStack::PushL(sel);
		iServerEntry->GetChildrenWithMtm(aMtm, *sel);
		}
	else if (iClientServer == EClientSide)
		{
		sel = iMsvEntry->ChildrenWithMtmL(aMtm);
		CleanupStack::PushL(sel);
		}
	else
		{
		Panic(KErrNotSupported);
		}

	return sel;
	}


inline void CMsvTestUtils::SetSortTypeL(TMsvSelectionOrdering aOrder)
	{
	if (iClientServer == EServerSide)
		{
		iServerEntry->SetSort(aOrder);
		}
	else if (iClientServer == EClientSide)
		{
		iMsvEntry->SetSortTypeL(aOrder);
		}
	else
		{
		User::Leave(KErrNotSupported);
		}
	}


