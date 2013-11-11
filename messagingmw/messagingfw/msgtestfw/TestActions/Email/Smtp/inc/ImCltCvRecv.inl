// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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


inline void CImCltRecvConvert::SetMsvId(TMsvId aId)
	{
	iClientEntry->SetEntryL(aId);
	iPopulateMessage = (iClientEntry->Entry().iType == KUidMsvMessageEntry);
	iRootEntryId = aId;
	}

inline CMsvEntry& CImCltRecvConvert::ClientEntry()
	{
	return *iClientEntry;
	}

inline const TMsvId CImCltRecvConvert::EntryId() const
	{
	if (iClientEntry->Entry().Id() == KMsvRootIndexEntryId)
		return iSavedEntryId;
	else
		return iClientEntry->Entry().Id();
	}

