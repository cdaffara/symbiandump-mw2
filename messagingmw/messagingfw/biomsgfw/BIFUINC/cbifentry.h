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

#ifndef __CBIFENTRY_H__
#define __CBIFENTRY_H__

#include <e32base.h>

class TEntry;

/** 
Encapsulates information about installed BIF files.

@internalTechnology
*/
class CBifEntry : public CBase
	{
public:
	IMPORT_C static CBifEntry* NewLC(const TEntry& aEntry);
	IMPORT_C static CBifEntry* NewL(const TEntry& aEntry);
	virtual ~CBifEntry();

	IMPORT_C TBool operator==(const CBifEntry& aEntry) const; 

private:
	CBifEntry();
	void ConstructL(const TEntry& aEntry);
	
public:
	/** The size of the file in bytes. */
	TInt iSize;

	/** The system time of last modification, in universal time. */
	TTime iModified;
	
	/** The name of the file relative to the owning directory. */
	HBufC* iName;
	};

#endif //__CBIFENTRY_H__
