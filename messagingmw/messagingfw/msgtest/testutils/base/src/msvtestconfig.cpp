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

#include "msvtestconfig.h"

EXPORT_C TInt TMsvConfigEntry::Set(const TDesC& aLine)
	{
	TInt pos = Find(aLine, _L("\t= "));
	if (pos >= 0 && pos < aLine.Length() - 1)
		{
		iName.Copy(aLine.Left(pos));
		iName.Trim();

		iValue.Copy(aLine.Mid(pos + 1));
		iValue.Trim();
		}
	else
		return KErrArgument;

	return KErrNone;
	}

TInt TMsvConfigEntry::Find(const TDesC& aString, const TDesC& aFind)
	{
	TInt result = -1;
	for (TInt pos = 0; pos < aFind.Length(); pos++)
		{
		result = aString.Locate(aFind[pos]);
		if (result >= 0)
			break;
		}
	return result;
	}

EXPORT_C TInt TMsvConfigEntry::Int() const
	{
	return Int(iValue);
	}

TInt TMsvConfigEntry::Int(TPtrC aPtr) const
	{
	TRadix radix = EDecimal;

	if (aPtr.Length() > 2 && aPtr[0] == '0' && (aPtr[1] == 'x' || aPtr[1] == 'X'))
		{
		aPtr.Set(aPtr.Mid(2));
		radix = EHex;
		}

	TLex lex(aPtr);

	TUint val = 0;
	lex.Val(val, radix);

	return val;
	}

EXPORT_C TVersion TMsvConfigEntry::Version() const
	{
	TUint major = 0;
	TUint minor = 0;
	TUint build = 0;

	// Find first comma
	TInt pos1 = iValue.Find(_L(","));
	if (pos1 > 0)
		major = Int(iValue.Left(pos1));

	// Get position of second comma
	TInt pos2 = pos1 + 1;
	while(pos2 < iValue.Length() && iValue[pos2] != ',')
		pos2++;
	
	// Check there is any data to read
	if (pos1 + 1 < iValue.Length() - 1)
		minor = Int(iValue.Mid(pos1 + 1, pos2 - pos1));

	// Check there is any data to read
	if (pos2 < iValue.Length() - 1)
		build = Int(iValue.Mid(pos2 + 1));

	return TVersion(major, minor, build);
	}

CMsvConfigEntrySelection::CMsvConfigEntrySelection()
	: CArrayFixFlat<TMsvConfigEntry>(KConfigSelectionGranularity)
	{
	}

EXPORT_C CMsvConfigEntrySelection* CMsvConfigEntrySelection::NewL(RFs& aFs, const TDesC& aFileName)
	{
	// Create the configuration selection
	CMsvConfigEntrySelection* self = new(ELeave)CMsvConfigEntrySelection;
	CleanupStack::PushL(self);

	// Open the file
	RFile file;
	User::LeaveIfError(file.Open(aFs, aFileName, EFileShareReadersOnly | EFileRead));
	
	// Treat it as a text file (unicode)
	TFileText textFile;
	textFile.Set(file);

	TInt error = KErrNone;
	TBuf<256> buf;

	while(error == KErrNone)
		{
		// Read a line of text
		error = textFile.Read(buf);
		buf.TrimLeft();

		if (!error)
			{
			// Skip comments
			if (buf.Length() >= 2 && buf[0] == '/' && buf[1] == '/')
				continue;

			// Create a configuration entry and add to selection
			TMsvConfigEntry entry;
			if (!entry.Set(buf))
				self->AppendL(entry);
			}
		}

	// Did an error occur?
	if (error != KErrEof && !error)
		User::Leave(error);

	CleanupStack::Pop(); // self
	return self;
	}

EXPORT_C TInt CMsvConfigEntrySelection::FindPos(const TDesC& aName) const
	{
	TInt count = Count();
	while(count--)
		{
		if (At(count).Name().CompareF(aName) == 0)
			return count;
		}
	return KErrNotFound;
	}

EXPORT_C const TMsvConfigEntry* CMsvConfigEntrySelection::Find(const TDesC& aName) const
	{
	TInt pos = FindPos(aName);
	if (pos >= 0)
		return &At(pos);

	return NULL;
	}
