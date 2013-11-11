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

#ifndef __MSVTESTCONFIG_H__
#define __MSVTESTCONFIG_H__

#include <e32base.h>
#include <f32file.h>

const TInt KConfigSelectionGranularity = 8;

class TMsvConfigEntry
	{
public:
	IMPORT_C TInt Set(const TDesC& aLine);
	IMPORT_C TVersion Version() const;
	IMPORT_C TInt Int() const;
	//
	inline const TDesC& Name() const;
	inline const TDesC& String() const;
	inline TUid Uid() const;
	//
private:
	TInt Find(const TDesC& aString, const TDesC& aFind);
	TInt Int(TPtrC aPtr) const;
	//
	TBuf<128> iName;
	TBuf<128> iValue;
	};

class CMsvConfigEntrySelection : public CArrayFixFlat<TMsvConfigEntry>
	{ 
public: 
	IMPORT_C static CMsvConfigEntrySelection* NewL(RFs& aFs, const TDesC& aFileName);
	IMPORT_C TInt FindPos(const TDesC& aName) const;
	IMPORT_C const TMsvConfigEntry* Find(const TDesC& aName) const;
	//
private:
	CMsvConfigEntrySelection();
	};

#include <msvtestconfig.inl>

#endif
