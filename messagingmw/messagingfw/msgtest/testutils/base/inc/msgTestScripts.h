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

#ifndef TEST_SCRIPTS_H_
#define TEST_SCRIPTS_H_

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include "msvtestutilsbase.h"

class CScriptSection;
class CScriptSectionItem;

_LIT(KScriptPanic, "MSG-TEST-SCRIPT");
_LIT(KScriptSectionStart, "[");
_LIT(KScriptCRLF, "\r\n");
_LIT(KScriptLF, "\n");
_LIT(KScriptCR, "\r");
_LIT(KScriptItemEnd, "=");
_LIT(KDefaults, "Defaults");
_LIT(KDefault1, "Def");
_LIT(KDefault2, "Default");

class CScriptFile : public CBase
	{
	public:
		IMPORT_C static CScriptFile* NewLC(CTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScriptFile);
		IMPORT_C static CScriptFile* NewLC(CTestUtils& aTestUtils, const TDesC& aComponent);
		IMPORT_C ~CScriptFile();

		IMPORT_C const TDesC& ItemValue(const TDesC& aSection, const TDesC& aItem, const TDesC& aDefault);
		IMPORT_C TInt ItemValue(const TDesC& aSection, const TDesC& aItem, const TInt aDefault);
		
		IMPORT_C static HBufC* ItemValueLC(CTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScript, const TDesC& aSection, const TDesC& aItem, const TDesC& aDefault);
		IMPORT_C static TInt ItemValueL(CTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScript, const TDesC& aSection, const TDesC& aItem, const TInt aDefault);

		IMPORT_C void ReadScriptL(const TDesC& aScript);

		inline CArrayPtrFlat<CScriptSection>& Sections() const;

		IMPORT_C CScriptSection* Section(const TDesC& aSectionName); //return NULL if section not found
		inline CScriptSection& SectionL(const TDesC& aSectionName); //leaves with KErrNotFound if section not found
		inline CScriptSection& Section(TInt aIndex) const {return *iSections->At(aIndex);}

		IMPORT_C static TInt GetNextWord(TLex& aInput, TChar aDelimiter, TInt& aOutput);
		IMPORT_C static TInt GetNextWord(TLex& aInput, TChar aDelimiter, TPtrC& aOutput);
		IMPORT_C static void ReplaceL(const TDesC& aOld, const TDesC& aNew, HBufC*& rString);
		IMPORT_C static HBufC* ReplaceLC(const TDesC& aOld, const TDesC& aNew, const TDesC& aOldString);

	protected:
		CScriptFile(CTestUtils& aTestUtils);
		void ConstructL(const TDesC& aComponent);

		CArrayPtrFlat<CScriptSection>* iSections;
		CScriptSection* iLastSection;
		CTestUtils& iTestUtils;
		HBufC* iComponent;

		TPtrC ParseValue(const TDesC& aText, const TLex& aInput, TInt aCurrentItemStart) const;
		void ParseAndSetItemValueL(const TDesC& aText, const TLex& aInput, TInt aCurrentItemStart, CScriptSectionItem*& arCurrentItem);
		void FoundNewItemL(const TDesC& aText, TLex& arInput, TInt& arCurrentItemStart, CScriptSection& aSection, CScriptSectionItem*& arCurrentItem);
		void CopyInDefaultsL(CScriptSection& aSection, const TDesC& aDefaultFile);

		HBufC* ReadFileLC(const TDesC& aScript) const;
		HBufC8* ReadFileL(const TDesC& aFile) const;
	};

class CScriptSection : public CBase
	{
	friend class CScriptFile;

	public:
		IMPORT_C static CScriptSection* NewLC(const TDesC& aSectionName);
		IMPORT_C static CScriptSection* NewLC(const TDesC& aSectionName, CScriptSection& aDefaults);
		IMPORT_C ~CScriptSection();
		
		inline const TDesC& SectionName() const;

		IMPORT_C CScriptSectionItem* Item(const TDesC& aItem); //return NULL if the item does not exist
		inline CScriptSectionItem& ItemL(const TDesC& aItem); //leaves with KErrNotFound if the item does not exist

		IMPORT_C const TDesC& ItemValue(const TDesC& aItem, const TDesC& aDefault);
		IMPORT_C TInt ItemValue(const TDesC& aItem, TInt aDefault);

		IMPORT_C CScriptSectionItem& AddItemL(const TDesC& aItem, const TDesC& aValue);
		IMPORT_C void AddItemIfNotExistL(const TDesC& aItem, const TDesC& aValue);
		IMPORT_C CScriptSectionItem& ReplaceItemL(const TDesC& aItem, const TDesC& aValue);
		IMPORT_C void DeleteItemsL(const TDesC& aItem);

		inline CArrayPtrFlat<CScriptSectionItem>& Items() const {return *iItems;}
		inline CScriptSectionItem& Item(TInt aIndex) const {return *iItems->At(aIndex);}

		inline void SetDefaultsL(CScriptSection& aDefaults);
		inline CScriptSection* Defaults() const {return iDefaults;}

		IMPORT_C CScriptSection* CopyLC();

	private:
		void ConstructL(const TDesC& aSectionName);
		CScriptSection();
		CArrayPtrFlat<CScriptSectionItem>* iItems;
		HBufC* iSectionName;
		CScriptSectionItem* iLastItem;
		CScriptSection* iDefaults;
	};

class CScriptSectionItem : public CBase
	{
	friend class CScriptSection;
	friend class CScriptItem;

	public:
		IMPORT_C static CScriptSectionItem* NewLC(CScriptSection& aParent, const TDesC& aItem, const TDesC& aValue);
		inline CScriptSectionItem* CopyLC();

		IMPORT_C ~CScriptSectionItem();
		HBufC* iValue;
		inline const TDesC& Item() const;
		inline const TDesC& Value() const;

		CScriptSection& iParent;
		
	private:
		CScriptSectionItem(CScriptSection& aParent);
		void ConstructL(const TDesC& aItem, const TDesC& aValue);
		HBufC* iItem;
	};

#include "msgtestscripts.inl"

#endif
