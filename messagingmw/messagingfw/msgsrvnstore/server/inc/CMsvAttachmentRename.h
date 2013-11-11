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

#ifndef __CMSVATTACHMENTRENAME_H__
#define __CMSVATTACHMENTRENAME_H__

#include <e32base.h>

class RReadStream;
class RWriteStream;

/**
Structure for containing information to enable safe attachment
renaming.

This structure contains data about the renaming of an attachment
so that if an error occurs the renaming operation is safe and
always maintains data integrity.

@see CMsvAttachmentManager
@internalComponent
*/
class CMsvAttachmentRename : public CBase
	{
public:
	static CMsvAttachmentRename* NewLC(const TDesC& aOldName, const TDesC& aNewName);
	static CMsvAttachmentRename* NewLC();
	~CMsvAttachmentRename();
	
	const TDesC& OldName() const;
	const TDesC& NewName() const;
	
	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;
	
private:
	CMsvAttachmentRename();
	void ConstructL(const TDesC& aOldName, const TDesC& aNewName);
	
private:
	HBufC* iOldName;
	HBufC* iNewName;
	};

#endif // __CMSVATTACHMENTRENAME_H__
