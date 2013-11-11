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

#include "EmsMsgTestUtils.h"
#include <msgtestscripts.h>
#include <emsformatie.h>
#include <emsanimationie.h>
#include <emssoundie.h>
#include <emsuserpromptie.h>
#include <gsmumsg.h>
#include "emstestutils.h"


CEmsExtractor* CEmsExtractor::NewL(CScriptSection& aSection)
	{
	CEmsExtractor* retVal = NewLC(aSection);
	CleanupStack::Pop(retVal);

	return retVal;
	}
CEmsExtractor* CEmsExtractor::NewLC(CScriptSection& aSection)
	{
	CEmsExtractor* retVal = new (ELeave) CEmsExtractor;
	CleanupStack::PushL(retVal);
	retVal->ConstructL(aSection);

	return retVal;
	}

void CEmsExtractor::ConstructL(CScriptSection& aSection)
	{
	// create the vector of parsers
	iParsers.Append(new (ELeave) EmsFormatParser);
	iParsers.Append(new (ELeave) EmsUserSoundParser);
	iParsers.Append(new (ELeave) EmsPredefSoundParser);
	iParsers.Append(new (ELeave) EmsPicParser);
	iParsers.Append(new (ELeave) EmsAnimParser);
	iParsers.Append(new (ELeave) EmsPredefAnimParser);
	iParsers.Append(new (ELeave) EmsUserPromptParser);

	// For each of the different types of parsers, attempt
	// to extract the object information from the section
	for (TInt i = 0; i < iParsers.Count(); ++i)
		{
		iParsers[i]->ParseL(aSection, iEmsArray);
		}
	}
	
CEmsExtractor::~CEmsExtractor()
	{ 
	iEmsArray.ResetAndDestroy(); 
	iParsers.ResetAndDestroy();
	}

void CEmsExtractor::AddToMessageL(CSmsMessage& msg) const
	{
	for (TInt i = 0; i < iEmsArray.Count(); ++i)
		{
		msg.AddEMSInformationElementL(*iEmsArray[i]);
		}
	}

void EmsParser::ParseL(CScriptSection& aSection, 
					   RPointerArray<CEmsInformationElement>& aEmsArray) const
	{
	// Get the value for the specific tag
	TPtrC tagPtr(aSection.ItemValue(iTagName, KNullDesC));

	// Only proceed if our string is non-empty
	if (tagPtr.Length() > 0)
		{
		// Firstly, wrap the characters in a TLex object
		TLex input(tagPtr);

		// Now, extract the number of objects we are expecting
		TInt num = ExtractIntegerL(input);

		// Loop around 'n' times getting the start position and then calling
		// the specific processing function
		for (TInt i = 0; i < num; ++i)
			{
			TInt startPos = ExtractIntegerL(input);

			// call the virtual function to append a new ems object into the
			// array
			aEmsArray.Append(GetElementL(input, startPos));
			}
		}
	}

TInt EmsParser::ExtractIntegerL(TLex& aInput)
	{
	aInput.Mark();

	// Now, get the value
	TInt retVal;
	User::LeaveIfError(aInput.Val(retVal));

	// Skip any whitespaces
	aInput.SkipSpace();

	return retVal;
	}


CEmsInformationElement* EmsFormatParser::GetElementL(TLex& aInput, TInt aStartPos) const
	{
	// Firstly, extract all the necessary parameters
	TInt formatLength = ExtractIntegerL(aInput);
	TInt formatValue = ExtractIntegerL(aInput);

	// now, create the formatting object
	CEmsFormatIE* object = CEmsFormatIE::NewL();
	object->SetStartPosition(aStartPos);
	object->SetFormatLength(formatLength);
	object->SetAlignment(STATIC_CAST(CEmsFormatIE::TAlignment, formatValue & 0x03));
	object->SetFontSize(STATIC_CAST(CEmsFormatIE::TFontSize, (formatValue & 0x0C) >> 2));
	object->SetBold(formatValue & 0x10);
	object->SetItalic(formatValue & 0x20);
	object->SetUnderline(formatValue & 0x40);
	object->SetStrikethrough(formatValue & 0x80);

	// Return this object
	return object;
	}

CEmsInformationElement* EmsUserSoundParser::GetElementL(TLex& aInput, TInt aStartPos) const
	{
	TPtrC soundAlpha;

	// Extract the sound but leave there is none.
	User::LeaveIfError(CScriptFile::GetNextWord(aInput, ' ', soundAlpha));

	TInt length = soundAlpha.Length();

	// Otherwise, create a new HBuf with the sound length
	HBufC8* sound = HBufC8::NewLC(soundAlpha.Length());
	TPtr8 ptr(sound->Des());

	// Go through the soundAlpha and extract the sound bytes
	// (removing offset of 'a')
	for (TInt i = 0; i < length; ++i)
		{
		ptr.Append(STATIC_CAST(TUint8, soundAlpha[i]) - 'a');
		}

	// Create the sound object
	CEmsSoundIE* object = CEmsSoundIE::NewL(*sound);
	object->SetStartPosition(aStartPos);
	CleanupStack::PopAndDestroy(sound);

	return object;
	}

CEmsInformationElement* EmsPredefSoundParser::GetElementL(TLex& aInput, TInt aStartPos) const
	{
	TInt sndValue = ExtractIntegerL(aInput);

	CEmsPreDefSoundIE* object = CEmsPreDefSoundIE::NewL(
		STATIC_CAST(CEmsPreDefSoundIE::TPredefinedSound, sndValue));
	object->SetStartPosition(aStartPos);

	return object;
	}

CEmsInformationElement* EmsPicParser::GetElementL(TLex& aInput, TInt aStartPos) const
	{
	TInt picValue = ExtractIntegerL(aInput);

	// construct a picture from the index which was input
	CEmsPictureIE* object = EmsTestUtils::CreatePictureL(picValue);
	object->SetStartPosition(aStartPos);

	return object;
	}

CEmsInformationElement* EmsAnimParser::GetElementL(TLex& aInput, TInt aStartPos) const
	{
	TInt animValue = ExtractIntegerL(aInput);

	// construct a picture from the index which was input
	CEmsAnimationIE* object = EmsTestUtils::CreateAnimationL(animValue);
	object->SetStartPosition(aStartPos);

	return object;
	}

CEmsInformationElement* EmsPredefAnimParser::GetElementL(TLex& aInput, TInt aStartPos) const
	{
	TInt animValue = ExtractIntegerL(aInput);

	CEmsPreDefAnimationIE* object = CEmsPreDefAnimationIE::NewL(
		STATIC_CAST(CEmsPreDefAnimationIE::TAnimType, animValue));
	object->SetStartPosition(aStartPos);

	return object;
	}

CEmsInformationElement* EmsUserPromptParser::GetElementL(TLex& aInput, TInt aStartPos) const
	{	
	TInt objCount = ExtractIntegerL(aInput);

	CEmsUserPrompt* object = CEmsUserPrompt::NewL(objCount);
	object->SetStartPosition(aStartPos);
	return object;
	}

