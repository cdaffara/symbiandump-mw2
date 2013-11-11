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

#include "cbifentry.h"

#include <f32file.h>

EXPORT_C CBifEntry* CBifEntry::NewLC(const TEntry& aEntry)
	{
	CBifEntry* self = new(ELeave) CBifEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aEntry);
	return self;
	}
	
EXPORT_C CBifEntry* CBifEntry::NewL(const TEntry& aEntry)
	{
	CBifEntry* self = CBifEntry::NewLC(aEntry);
	CleanupStack::Pop(self);
	return self;
	}
	
CBifEntry::~CBifEntry()
	{
	delete iName;
	}
	
CBifEntry::CBifEntry()
: CBase()
	{
	}

void CBifEntry::ConstructL(const TEntry& aEntry)
	{
	iSize = aEntry.iSize;
	iModified = aEntry.iModified;
	iName = aEntry.iName.AllocL();
	}

/**
Equality operator.

@param CBifEntry
Specifies CBifEntry object to compare with.

@return
ETrue if the entries are the same.
*/
EXPORT_C TBool CBifEntry::operator==(const CBifEntry& aEntry) const
	{
	return ((iModified == aEntry.iModified)
		 && (iName->Compare(*aEntry.iName) == 0)
		 && (iSize == aEntry.iSize));
	}
