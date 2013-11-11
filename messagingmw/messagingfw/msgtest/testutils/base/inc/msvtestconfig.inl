// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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


inline const TDesC& TMsvConfigEntry::String() const
	{
	return iValue;
	}

inline const TDesC& TMsvConfigEntry::Name() const
	{
	return iName;
	}

inline TUid TMsvConfigEntry::Uid() const
	{
	return TUid::Uid(Int());
	}
