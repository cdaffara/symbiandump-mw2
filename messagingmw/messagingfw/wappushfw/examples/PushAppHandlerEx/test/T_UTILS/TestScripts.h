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
// API for reading from a script file
// 
//

/**
 @file TestScripts.h
*/

#ifndef __TESTSCRIPTS_H__
#define __TESTSCRIPTS_H__

#include <e32base.h>
#include <f32file.h>

class CScriptSection;
class CScriptSectionItem;

_LIT(KScriptPanic, "MMSTestScript");
_LIT(KScriptSectionStart, "[");
_LIT(KScriptCRLF, "\r\n"); // Carriage Return and Left Feed
_LIT(KScriptLF, "\n");
_LIT(KScriptCR, "\r");
_LIT(KScriptItemEnd, "=");
_LIT(KDefaults, "Defaults");
_LIT(KDefault1, "Def");
_LIT(KDefault2, "Default");



/**
 * Class to read from a script file and creates the script sections.
 */
class CScriptFile : public CBase
	{
	public:
	// Methods
/**
	Standard static factory constructor.
	Error Condition	:	Memory limitations
	@param			const TDesC& aComponent Component name to be scripted
	@return			CScriptFile* for a reference to all the sections being scripted
 */
		IMPORT_C static CScriptFile* NewL(const TDesC& aComponent);

/**
	Standard static factory constructor.
	Error Condition	:	Memory limitations
	@param			const TDesC& aComponent Component name to be scripted
	@return			CScriptFile* for a reference to all the sections being scripted
 */
		IMPORT_C static CScriptFile* NewLC(const TDesC& aComponent);

/**
	Default destructor

 */
		IMPORT_C ~CScriptFile();

/**
	To return an Item Value of a given section as TDesC&
	@param			const TDesC& aSection A given Section in the script file
	@param			const TDesC& aItem A given Item in a Section
	@param			const TDesC& aDefault Identifies a [Default] value
	@return			TDesC& Item Value under a default section in TDesC&
 */
		IMPORT_C const TDesC& ItemValue(const TDesC& aSection, const TDesC& aItem , const TDesC& aDefault);

/**
	To return an Item Value of a given section as TInt
	@param			const TDesC& aSection A given Section in the script file
	@param			const TDesC& aItem A given Item in a Section
	@param			const TInt aDefault Identifies a [Default] value
	@return			TInt Item Value under a default section in TDesC&
 */
		IMPORT_C TInt ItemValue(const TDesC& aSection, const TDesC& aItem, const TInt aDefault);

/**
	To return an Item Value of a default Section (aSection) that has item name aItem
	Error Condition	:	Memory Limitations
	@param			const TDesC& aComponent The Title of the component test cases being scripted
	@param			const TDesC& aSection The non-default section name to check the value of aItem
	@param			const TDesC& aItem Item name of a aSection that we need its value
	@param			const TInt aDefault A default section to check the item aItem
	@return			aDefault The value of item name aItem
 */
		IMPORT_C static TInt ItemValueL(const TDesC& aComponent, const TDesC& aSection, const TDesC& aItem, const TInt aDefault);

/**
	Read the contents of a file specified in aScript
	Error Condition	:	Memory Limitation
	@param			const TDesC& aScript The path name for the script file
	@pre 			Fully constructed CScriptFile object and connection with the file server is established
	@post			Read the contents of aFile with no error
 */
		IMPORT_C TInt ReadScriptL(const TDesC& aScript);

/**
	To return reference to our sections in 'this' Scriptfile' object
 */
		RPointerArray<CScriptSection>& Sections();

/**
	Get the CScriptSection with the section name aSectionName
	@param			const TDesC& aSectionName Section name to be used for getting its CScriptSection Object
	@return			CScriptSection* The script section object with aSectionName
 */
		IMPORT_C CScriptSection* Section(const TDesC& aSectionName);

/**
	Get the CScriptSection with the section name aSectionName
 */
		inline CScriptSection& SectionL(const TDesC& aSectionName);

/**
	Get the Section at index aIndex the sections in 'this' script object
 */
		inline CScriptSection& Section(TInt aIndex) {return *(iSections[aIndex]);}

/**
	Replace old string (aOldString) with a new string
	Error Condition	:	Memory Limitation
	@param			const TDesC& aOld Reference to the old string
	@param			const TDesC& aNew Reference to the new string to replace the old one
	@param			HBufC*& rString The actual old string
 */
		IMPORT_C static void ReplaceL(const TDesC& aSource, const TDesC& aNew, HBufC*& aDestination);

/**
	Replace old string (aOldString) with a new string
	Error Condition	:	Memory Limitation
	@param			const TDesC& aOld Reference to the old string
	@param			const TDesC& aNew Reference to the new string to replace the old one
	@param			const TDesC& aOldString The actual old string
	@return			HBufC* Buffer containing the new string
 */
		IMPORT_C static HBufC* ReplaceLC(const TDesC& aSource, const TDesC& aNew, const TDesC& aDestination);

	protected:

/**
	Initialize the array with the sections and connect with the file server
	Error Condition	:	Memory Limitation
	@param			const TDesC& aComponent The component name to be tested
	@pre 			Test script file to be stored in epoc32\wins\c. RFs file declared as a data member in 'this' object
	@post			Connection with the file server is established and ready to read the file contents
 */
		void ConstructL(const TDesC& aComponent);

		RPointerArray<CScriptSection> iSections; // To store the sections
		CScriptSection* iLastSection; // To mark the last section

		HBufC* iComponent; // The component name to be scripted

/**
	Parses the text from a token
	@param			const TDesC& aText Text used to set the value being parsed
	@param			const TLex& aInput Token value
	@param			TInt aCurrentItemStart Index to mark the start of the current item
	@return			TPtrC The value being parsed
	@pre 			aInput is a valid token
	@post			New value being parsed after each call of the function
 */
		TPtrC ParseValue(const TDesC& aText, const TLex& aInput, TInt aCurrentItemStart) const;

/**
	Parses the text from a token
	Error Condition	:	Memory Limitation
	@param			const TDesC& aText Text used to set the value being parsed
	@param			const TLex& aInput Token Value
	@param			TInt aCurrentItemStart Index to mark the start of the current item
	@param			CScriptSectionItem*& arCurrentItem Index to mark the start of the current
	@pre 			aInput is a valid token
	@post			New value being parsed and set after each call of the function
 */
		void ParseAndSetItemValueL(const TDesC& aText, const TLex& aInput, TInt aCurrentItemStart, CScriptSectionItem*& arCurrentItem);

/**
	To Find a new script section item and initialize arCurrentItem
	Error Condition	:	Memory Limitation
	@param			const TDesC& aText The script section item
	@param			TLex& arInput Provides access to string-parsing functions applied to the sections stored as token
	@param			TInt& arCurrentItemStart Index to start looking for the new item
	@param			CScriptSection& aSection The section to find the new item
	@param			CScriptSectionItem*& arCurrentItem The new item found
	@pre 			The script contents stored in a buffer are converted into tokens
	@post			All new Items will have been found.  Those items pre-fixed with 'Comment:' are ignored
 */
		void FoundNewItemL(const TDesC& aText, TLex& aInput, TInt& aCurrentItemStart, CScriptSection& aSection, CScriptSectionItem*& aCurrentItem);

/**
	To copy items of aSection to a default section
	Error Condition	:	Memory Limitation
	@param			CScriptSection& aSection Default section to copy its items to 'this' script object
	@pre 			Check if default section exist
	@post			To confirm if all the items have been copied
 */
		void CopyInDefaultsL(CScriptSection& aSection);

/**
	To read from a file and return the script contents
	Error Condition	:	Memory Limitation
	@param			const TDesC& aScript The file name to be scripted
	@return			HBufC* The buffer to store the script contents
	@pre 			File exist and the connection with the file server is established
	@post			Script contents are being read till end of file and file server to be closed in the destructor
 */
		HBufC* ReadFileLC(const TDesC& aScript);

/**
	To read from a file and return the script contents
	Error Condition	:	Memory Limitations
	@param			const TDesC& aFile The file name with data contents to be scripted
	@return			HBufC8* The buffer to store the script contents
	@pre 			File exist and the connection with the file server is established
	@post			All the script contents have been read till the end of file with no error
					File server to be closed at 'this' script object's destructor
 */
		HBufC8* ReadFileL(const TDesC& aFile);

	private:
		RFs iFs; // For connecting to the file server
	};

/**
 * Class with functions to make changes to Items in a Section
 */

class CScriptSection : public CBase
	{
	friend class CScriptFile;

	public:

/**
	Standard static factory constructor.
	Error Condition	:	Memory limitations
	@param			const TDesC& aSectionName The section name
	@return			CScriptSection* 
 */
		IMPORT_C static CScriptSection* NewLC(const TDesC& aSectionName);

/**
	Standard static factory constructor.
	Error Condition	:	Memory limitations
	@param			const TDesC& aSectionName The section name
	@param			CScriptSection& aDefaults A reference to the section marked as [Defaults]
	@return			CScriptSection* 
 */
		IMPORT_C static CScriptSection* NewLC(const TDesC& aSectionName, CScriptSection& aDefaults);

/**
	Default destructor

 */
		IMPORT_C ~CScriptSection();
		
/**
	Get the section name of 'this' script section object
 */
		inline const TDesC& SectionName() const;

/**
	To return the CScriptSectionItem with a item name aItem 
	@param			const TDesC& aItem To store the item name
	@return			CScriptSectionItem* 
 */
		IMPORT_C CScriptSectionItem* Item(const TDesC& aItem);

/**
	Returns a CScriptSectionItem with item name aItem
	Error Condition	:	leaves with KErrNotFound if the item does not exist
	@param			const TDesC& aItem The item name
	@return			CScriptSectionItem& A reference to CScriptSectionItem object with the name aItem
 */
		CScriptSectionItem& ItemL(const TDesC& aItem);

/**
	To return the Item value (as descriptor) of a default section with the name aItem
	@param			const TDesC& aItem The reference to item name
	@param			const TDesC& aDefault Section name marked as [Default]
	@return			TDesC& The Item name 
 */
		IMPORT_C const TDesC& ItemValue(const TDesC& aItem, const TDesC& aDefault);

/**
	To return the Item value of a default section
	@param			const TDesC& aItem The item name to check its value
	@param			TInt aDefault Index of item at default section
	@return			TInt Item The item value of aItem
 */
		IMPORT_C TInt ItemValue(const TDesC& aItem, TInt aDefault);

/**
	Adds the CScriptSectionItem with the name aItem and the value aValue
	Error Condition	:	Memory limitations
	@param			const TDesC& aItem The item name to be added
	@param			const TDesC& aValue The item value to be added
	@return			CScriptSectionItem& New script section item to be added is returned
 */
		IMPORT_C CScriptSectionItem& AddItemL(const TDesC& aItem, const TDesC& aValue);

/**
	Adds a section item with a name aItem if it doesn't exist in 'this' object
	Error Condition	:	Memory limitations
	@param			const TDesC& aItem The item name to check it exists in our section items
	@param			const TDesC& aValue The item value
 */
		IMPORT_C void AddItemIfNotExistL(const TDesC& aItem, const TDesC& aValue);

/**
	Replace the value of aItem with aValue
	Error Condition	:	Memory limitations
	@param			const TDesC& aItem The item name to be replaced
	@param			const TDesC& aValue The item value to replace the section item with a name aItem
	@return			CScriptSectionItem& The section item of the value aValue
 */
		IMPORT_C CScriptSectionItem& ReplaceItemL(const TDesC& aItem, const TDesC& aValue);

/**
	Deletes CScriptSectionItem with a name aItem
	@param			const TDesC& aItem The item name to be removed
 */
		IMPORT_C void DeleteItems(const TDesC& aItem);

/**
	Get CScriptSectionItems from 'this' script section
 */
		inline RPointerArray<CScriptSectionItem> Items() const {return iItems;}

/**
	Get the CScriptSectionItem at index aIndex from 'this' script section
 */
		inline CScriptSectionItem& Item(TInt aIndex) {return *(iItems[aIndex]);}

/**
	Sets the default section with aDefaults
 */
		inline void SetDefaultsL(CScriptSection& aDefaults);

/**
	Gets default section from 'this' object
 */
		inline CScriptSection* Defaults() const {return iDefaults;} 

/**
	To return a copy of 'this' script section
	Error Condition	:	Memory Limitation
	@return			CScriptSection* 
 */
	IMPORT_C CScriptSection* CopyLC();

	private:

/**
	To initialize 'this' CScriptSection name with aSectionName
	Error Condition	:	Memory Limitation
	@param			const TDesC& aSectionName The section name to initialize 'this' ScriptSection
 */
		void ConstructL(const TDesC& aSectionName);

/**
	Default constructor
 */
		CScriptSection();

		RPointerArray<CScriptSectionItem> iItems;

		HBufC* iSectionName; // To store the section names

		CScriptSectionItem* iLastItem; // To store the last section item of 'this' script object

		CScriptSection* iDefaults; // To store the default sections of 'this' script object
	};

/**
 * Class representing a section item as Item name and a value
 */

class CScriptSectionItem : public CBase
	{
	friend class CScriptSection;
	friend class CScriptItem;

	public:

/**
	Standard static factory constructor.
	Error Condition	:	Memory limitations
	@param			CScriptSection& aParent A reference to our script sections
	@param			const TDesC& aItem The item name
	@param			const TDesC& aValue The item value
	@return			CScriptSectionItem* The script section item storing the aItem and aValue
 */
		IMPORT_C static CScriptSectionItem* NewLC(CScriptSection& aParent, const TDesC& aItem, const TDesC& aValue);

/**
	Make a copy of our 'this' CScriptSectionItem
	Error Condition	:	Memory limitations
	@return			CScriptSectionItem* The script section item storing the item and value of 'this' object
 */
		inline CScriptSectionItem* CopyLC();

/**
	Default destructor
 */
		IMPORT_C ~CScriptSectionItem();

		HBufC* iValue; // The value for 'this' CScriptSectionItem

/**
	To return the Item name of 'this' CScriptSectionItem object
	@return			TDesC& Descriptor with a reference to the Item name
 */
		inline const TDesC& Item() const;

/**
	To return the value of 'this' CScriptSectionItem object
	@return			TDesC&  Descriptor with a reference to the Item name
 */
		const TDesC& Value() const;

		CScriptSection& iParent; // Reference to the sections in 'this' object
		
	private:

/**
	To initialize our reference to CScriptSection
	@param			CScriptSection& aParent A reference to CScriptSection to intialize to 'this' script sections
 */
		CScriptSectionItem(CScriptSection& aParent);

/**
	To initialize 'this' CScriptSectionItem to store aItem and aValue
	@param			const TDesC& aItem The item name
	@param			const TDesC& aValue The item value
 */
		void ConstructL(const TDesC& aItem, const TDesC& aValue);

		HBufC* iItem; // To store the item name for 'this' CScriptSectionItem
	};

#include "TestScripts.inl"

#endif // __TESTSCRIPTS_H__

