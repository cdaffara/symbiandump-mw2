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

#include <s32strm.h>

#include "CMsvAttachmentRename.h"

CMsvAttachmentRename* CMsvAttachmentRename::NewLC(const TDesC& aOldName, const TDesC& aNewName)
	{
	CMsvAttachmentRename* self = new(ELeave) CMsvAttachmentRename();
	CleanupStack::PushL(self);
	self->ConstructL(aOldName, aNewName);
	return self;
	}
	
CMsvAttachmentRename* CMsvAttachmentRename::NewLC()
	{
	CMsvAttachmentRename* self = new(ELeave) CMsvAttachmentRename();
	CleanupStack::PushL(self);
	return self;
	}

CMsvAttachmentRename::CMsvAttachmentRename()
	{
	}
	
void CMsvAttachmentRename::ConstructL(const TDesC& aOldName, const TDesC& aNewName)
	{
	iOldName = aOldName.AllocL();
	iNewName = aNewName.AllocL();
	}
	
CMsvAttachmentRename::~CMsvAttachmentRename()
	{
	delete iOldName;
	delete iNewName;
	}

const TDesC& CMsvAttachmentRename::OldName() const
	{
	__ASSERT_DEBUG(iOldName!=NULL, User::Invariant());
	
	return *iOldName;
	}
	
const TDesC& CMsvAttachmentRename::NewName() const
	{
	__ASSERT_DEBUG(iNewName!=NULL, User::Invariant());
	
	return *iNewName;
	}

void CMsvAttachmentRename::InternalizeL(RReadStream& aStream)
	{
	delete iOldName;
	iOldName = NULL;
	iOldName = HBufC::NewL(aStream, KMaxTInt);
	delete iNewName;
	iNewName = NULL;
	iNewName = HBufC::NewL(aStream, KMaxTInt);
	}
	
void CMsvAttachmentRename::ExternalizeL(RWriteStream& aStream) const
	{
	__ASSERT_DEBUG(iOldName!=NULL, User::Invariant());
	aStream << *iOldName;
	__ASSERT_DEBUG(iNewName!=NULL, User::Invariant());
	aStream << *iNewName;
	}

