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
// CMsvEntry
// 
//

inline const TMsvEntry& CMsvClientEntry::Entry() const
	{
	return iEntry;
	}

inline TMsvClientEntryType CMsvClientEntry::Type() const
	{
	return iType;
	}

inline void CMsvClientEntry::SetType(TMsvClientEntryType aType)
	{
	iType = aType;
	}

inline void CMsvClientEntry::SetOwnerFlag(TBool aState)
	{
	iEntry.SetOwner(aState);
	}

inline void CMsvClientEntry::SetId(TMsvId id)
	{
	iEntry.SetId(id);
	}
