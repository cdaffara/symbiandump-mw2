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

#ifndef _EMS_MSG_TEST_UTILS_H
#define _EMS_MSG_TEST_UTILS_H

#include <e32base.h>
#include <emsinformationelement.h>

// The following descriptors are the tags for EMS objects. Note that not all
// object types are supported - in particular, picture types and user defined
// animations
//
// The syntax for adding EMS objects is as follows:
// <TAG>= <numObjects> [<startPos> <specific parameters>] (n times)
//
// Specific parameters are:
// EmsFormat: <formatLength> <formatValue>
// EmsUserSound: <sound, where a = 0x01>, eg abcdefghijklmn
// EmsPredefSound: <soundValue>
// EmsPic: <picture-type> where 0 is small, 1 is large, 2 is user defined
// EmsAnim: <anim-type> where 0 is small, 1 is large
// EmsPredefAnim: <animValue>
// EmsUserPrompt: <numObjects>
//
// eg, to create two user sounds, at position 10 and 20, you do:
// EmsUserSound= 2 10 abcde 20 fghijk
_LIT(KEmsItemFormat, "EmsFormat");
_LIT(KEmsItemUserSound, "EmsUserSound");
_LIT(KEmsItemPredefSound, "EmsPredefSound");
_LIT(KEmsItemPic, "EmsPicture");
_LIT(KEmsItemPredefAnim, "EmsPredefAnim");
_LIT(KEmsItemAnim, "EmsAnimation");
_LIT(KEmsItemUserPrompt, "EmsUserPrompt");

class CScriptSection;
class EmsParser;
class CSmsMessage;

// This class extracts all ems objects from a given section when the class
// is constructed
class CEmsExtractor : public CBase
	{
public:
	// Constructs an EmsExtractor object from a given script section and
	// processes it
	static CEmsExtractor* NewL(CScriptSection& aSection);
	static CEmsExtractor* NewLC(CScriptSection& aSection);

	virtual ~CEmsExtractor();

	// This method adds all the extracted EMS objects into the actual message
	void AddToMessageL(CSmsMessage& msg) const;

private:
	CEmsExtractor() : iEmsArray(10) {}

	// Constructs the EmsExtractor object and sets up the array from the 
	// specified section
	void ConstructL(CScriptSection& aSection);

private:
	// array of CEmsInformationElement pointers
	RPointerArray<CEmsInformationElement> iEmsArray;
	RPointerArray<EmsParser> iParsers;
	};


// abstract base class which encapsulates the parsers for each object.
// EmsParsers will parse the parameters of an EMS tag. Each specialised
// EmsParser will implement the GetElementL method to extract the
// specific parameters section above.
class EmsParser
	{
public:
	EmsParser(const TDesC& aTagName) : iTagName(aTagName) {}
	virtual ~EmsParser() {}

	// For a given section, parse my EMS object type and put the newly
	// created objects into the array
	void ParseL(CScriptSection& aSection, RPointerArray<CEmsInformationElement>& aEmsArray) const;

protected:
	virtual CEmsInformationElement* GetElementL(TLex& aInput, TInt aStartPos) const = 0;

	// utility function for extracting the first integer after the marked
	// position
	static TInt ExtractIntegerL(TLex& aInput);

private:
	const TDesC& iTagName;
	};

// Parser for format object
class EmsFormatParser : public EmsParser
	{
public:
	EmsFormatParser() : EmsParser(KEmsItemFormat) {}

protected:
	virtual CEmsInformationElement* GetElementL(TLex& aInput, TInt aStartPos) const;
	};

// Parser for user sound object
class EmsUserSoundParser : public EmsParser
	{
public:
	EmsUserSoundParser() : EmsParser(KEmsItemUserSound) {}

protected:
	virtual CEmsInformationElement* GetElementL(TLex& aInput, TInt aStartPos) const;
	};

// Parser for predefined sound object
class EmsPredefSoundParser : public EmsParser
	{
public:
	EmsPredefSoundParser() : EmsParser(KEmsItemPredefSound) {}

protected:
	virtual CEmsInformationElement* GetElementL(TLex& aInput, TInt aStartPos) const;
	};

// Parser for picture object
class EmsPicParser : public EmsParser
	{
public:
	EmsPicParser() : EmsParser(KEmsItemPic) {}

protected:
	virtual CEmsInformationElement* GetElementL(TLex& aInput, TInt aStartPos) const;
	};

// Parser for animation object
class EmsAnimParser : public EmsParser
	{
public:
	EmsAnimParser() : EmsParser(KEmsItemAnim) {}

protected:
	virtual CEmsInformationElement* GetElementL(TLex& aInput, TInt aStartPos) const;
	};

// Parser for predefined animation object
class EmsPredefAnimParser : public EmsParser
	{
public:
	EmsPredefAnimParser() : EmsParser(KEmsItemPredefAnim) {}

protected:
	virtual CEmsInformationElement* GetElementL(TLex& aInput, TInt aStartPos) const;
	};

// Parser for user prompt object
class EmsUserPromptParser : public EmsParser
	{
public:
	EmsUserPromptParser() : EmsParser(KEmsItemUserPrompt) {}

protected:
	virtual CEmsInformationElement* GetElementL(TLex& aInput, TInt aStartPos) const;
	};


#endif
