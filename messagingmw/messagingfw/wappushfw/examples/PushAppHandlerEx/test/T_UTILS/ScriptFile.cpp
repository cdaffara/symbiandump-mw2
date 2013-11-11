// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "testscripts.h"

EXPORT_C CScriptFile* CScriptFile::NewL(const TDesC& aComponent)
	{
	CScriptFile* self = NewLC(aComponent);
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CScriptFile* CScriptFile::NewLC(const TDesC& aComponent)
	{
	CScriptFile* self = new (ELeave) CScriptFile();
	CleanupStack::PushL(self);
	self->ConstructL(aComponent);
	return self;
	}

void CScriptFile::ConstructL(const TDesC& aComponent)
	{
	iComponent = aComponent.AllocL();

	// Connect to file server for test scripts, ini-files etc.
	User::LeaveIfError(iFs.Connect());
	}

EXPORT_C CScriptFile::~CScriptFile()
	{
	iSections.ResetAndDestroy();

	delete iComponent;

	// Close fileserver connection
	iFs.Close();
	}

EXPORT_C CScriptSection* CScriptFile::Section(const TDesC& aSectionName)
	{
	// Find a section that exist with a section name aSectionName
	CScriptSection* section = iLastSection;

	if (!iLastSection || iLastSection->SectionName().CompareF(aSectionName) != 0)
		{
		const TInt count = iSections.Count();
		
		for (TInt i = 0; i < count; ++i) //order important
			{
			section = iSections[i];
			iLastSection = section;

			if (iLastSection->SectionName().CompareF(aSectionName) == 0)
				break;
			else
				section = NULL;
			}
		}

	return section;
	}

EXPORT_C const TDesC& CScriptFile::ItemValue(const TDesC& aSection, const TDesC& aItem, const TDesC& aDefault)
	{
	// Find Item value of a section: aItem value is the item name
	CScriptSection* section = Section(aSection);

	if (section != NULL)
		return section->ItemValue(aItem , aDefault);

	return aDefault;
	}

EXPORT_C TInt CScriptFile::ItemValue(const TDesC& aSection, const TDesC& aItem, const TInt aDefault)
	{
	// Find Item value of a section: aItem value is a TInt
	TInt output = aDefault;
	CScriptSection* section = Section(aSection);

	if (section != NULL)
		output = section->ItemValue(aItem, aDefault);

	return output;
	}
		

EXPORT_C TInt CScriptFile::ItemValueL(const TDesC& aComponent, const TDesC& aSection, const TDesC& aItem, const TInt aDefault)
	{
	CScriptFile* self = NewLC(aComponent);
	TInt output = self->ItemValue(aSection, aItem, aDefault);
	CleanupStack::PopAndDestroy(self);
	return output;
	}

HBufC8* CScriptFile::ReadFileL(const TDesC& aFile)
	{
	RFile file;
	_LIT(KMMSDirectoryLocation,"push");
	TFindFile find(iFs);
	if (find.FindByDir(aFile, KMMSDirectoryLocation) == KErrNone)
		{
		User::LeaveIfError(file.Open(iFs, aFile, EFileShareAny | EFileRead));
		CleanupClosePushL(file);
		}
	
	else
		return NULL; // File not found

	TInt size = 0;
	TInt err = file.Size(size);

	if (err)
		User::LeaveIfError(err);

	HBufC8* contents = HBufC8::NewLC(size);
	TPtr8 contentsPtr(contents->Des());
	err = file.Read(0, contentsPtr);

	if (err)
		User::LeaveIfError(err);

	CleanupStack::Pop(contents);
	CleanupStack::PopAndDestroy(); //file

	return contents;
	}

HBufC* CScriptFile::ReadFileLC(const TDesC& aScript)
	{
	HBufC8* file = NULL;

	file = ReadFileL(aScript);

	if (file == NULL)
		return NULL;

	CleanupStack::PushL(file);

	HBufC* buf = HBufC::NewL(file->Length());
	buf->Des().Copy(*file);
	CleanupStack::PopAndDestroy(file);
	CleanupStack::PushL(buf);
	return buf;
	}

EXPORT_C TInt CScriptFile::ReadScriptL(const TDesC& aScript)
	{
	iSections.ResetAndDestroy();
	iLastSection = NULL;

	CScriptSection* section = NULL;
	CScriptSectionItem* currentItem = NULL;
	TInt currentItemStart = 0;
	CScriptSection* sectionAll = NULL;

	HBufC* scriptContents = ReadFileLC(aScript);
	if (scriptContents == NULL)
		return KErrNotFound;

	TLex input(*scriptContents);

	while (!input.Eos())
		{
	    input.SkipSpaceAndMark();

		input.SkipCharacters();
		if ( input.TokenLength() == 0 )    // if valid potential token
			break;

		TPtrC token = input.MarkedToken();
		_LIT(KEndScript, "endscript");
		if (token.CompareF(KEndScript) == 0)
			break;
		else if (token.FindF(KScriptSectionStart) == 0 && token.Length() > 2) // >2 implies a section with a name != NULL i.e. excluding the two brackets "[" "]"
			{
			ParseAndSetItemValueL(*scriptContents, input, currentItemStart, currentItem);

			TInt mid = 1;
			TInt len = token.Length() - 2; // Contents as from first bracket to the last bracket

			if (sectionAll)
				section = CScriptSection::NewLC(token.Mid(mid, len), *sectionAll);
			else
				section = CScriptSection::NewLC(token.Mid(mid, len));

			if (!sectionAll && section->SectionName().CompareF(_L("All")) == 0)
				sectionAll = section;

			User::LeaveIfError(iSections.Append(section));
			CleanupStack::Pop(); //section
			}
		else if (section)
			{
			TPtrC itemEnd(KScriptItemEnd);

			if (itemEnd.Length() < token.Length() && token.Right(itemEnd.Length()).CompareF(itemEnd) == 0)
				FoundNewItemL(*scriptContents, input, currentItemStart, *section, currentItem);
			}
		}

	ParseAndSetItemValueL(*scriptContents, input, currentItemStart, currentItem);
	CleanupStack::PopAndDestroy(scriptContents);
	return KErrNone;
	}

TPtrC CScriptFile::ParseValue(const TDesC& aText, const TLex& aInput, TInt aCurrentItemStart) const
	{
	TInt mid = aCurrentItemStart;
	TInt len = aInput.MarkedOffset() - mid;
	TPtrC ret(KNullDesC);

	if (len > 0)
		ret.Set(aText.Mid(mid, len));

	return ret;
	}

void CScriptFile::ParseAndSetItemValueL(const TDesC& aText, const TLex& aInput, TInt aCurrentItemStart, CScriptSectionItem*& arCurrentItem)
	{
	if (arCurrentItem)
		{
		delete arCurrentItem->iValue;
		arCurrentItem->iValue = NULL;
		arCurrentItem->iValue = ParseValue(aText, aInput, aCurrentItemStart).AllocL();
		arCurrentItem->iValue->Des().Trim();
		ReplaceL(KScriptCRLF, KScriptLF, arCurrentItem->iValue);

		if (arCurrentItem->Item().CompareF(KDefaults) == 0)
			CopyInDefaultsL(arCurrentItem->iParent);
		}

	arCurrentItem = NULL;
	}


void CScriptFile::FoundNewItemL(const TDesC& aText, TLex& aInput, TInt& aCurrentItemStart, CScriptSection& aSection, CScriptSectionItem*& aCurrentItem)
	{
	TPtrC token = aInput.MarkedToken();

	ParseAndSetItemValueL(aText, aInput, aCurrentItemStart, aCurrentItem);

	aInput.SkipSpaceAndMark();
	aCurrentItemStart = aInput.Offset();

	TPtrC itemEnd(KScriptItemEnd);
	const TInt length = token.Length() - itemEnd.Length();
	aCurrentItem = &aSection.ReplaceItemL(token.Left(length), KNullDesC);
	}

void CScriptFile::CopyInDefaultsL(CScriptSection& aSection)
	{
	CScriptFile* file = CScriptFile::NewLC(*iComponent);

	TInt count = file->Sections().Count();

	if (count > 0)
		{
		CScriptSection& def = *file->Sections()[0];
		aSection.SetDefaultsL(def);
		}
	CleanupStack::PopAndDestroy(file);
	}

EXPORT_C void CScriptFile::ReplaceL(const TDesC& aSource, const TDesC& aNew, HBufC*& aDestination)
	{
	HBufC* repl = ReplaceLC(aSource, aNew, *aDestination);
	CleanupStack::Pop(repl);
	delete aDestination;
	aDestination = repl;
	}

EXPORT_C HBufC* CScriptFile::ReplaceLC(const TDesC& aSource, const TDesC& aNew, const TDesC& aDestination)
	{
	HBufC* rString = aDestination.AllocLC();
	TInt oldLen = aSource.Length();
	TInt newLen = aNew.Length();

	if (!oldLen)
		return rString;

	for (TInt pos = 0; pos < rString->Length(); pos += newLen)
		{
		TPtrC ptrC = rString->Mid(pos);
		TInt find = ptrC.Find(aSource);

		if (find == KErrNotFound)
			return rString;
	
		pos += find;

		if (newLen > oldLen)
			{
			rString = rString->ReAllocL(rString->Length() + newLen - oldLen);
			CleanupStack::Pop();
			CleanupStack::PushL(rString);
			}

		TPtr ptr(rString->Des());
		ptr.Replace(pos, oldLen, aNew);
		}

	return rString;
	}

RPointerArray<CScriptSection>& CScriptFile::Sections()
	{
	return iSections;
	}

const TDesC& CScriptSectionItem::Value() const
	{
	return *iValue;
	}
