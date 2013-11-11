/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of RingToneRecognizer
*
*/


// INCLUDE FILES
#include <apmrec.h>
#include <apmstd.h>
#include <implementationproxy.h>

#include "recrt.h"

// CONSTANTS
const TUid KUidRingingToneRecognizer={0x100059EA};
const TInt KMinBufferLength=3;					// minimum amount of file needed to determine a ringing tone IF it's not called .rng
const TInt KMaxBufferLength=4;					// maximum amount of buffer space we will ever use
const TInt KRecRtCommandByte=2;					// 00000010 = <command length>
const TInt KRecRtUnicodeCommandByte=3;			// 00000011 = <command length> if unicode
const TInt KRecRtProgrammingByte=74;			// 01001010 = <ringing-tone programming> + 0
const TInt KRecRtSoundByte=58;					// 00111010 = <sound> + 0
const TInt KRecRtUnicodeByte=68;				// 01000100 = <unicode> + 0
const TInt KRecRtFileExtensionsMightBeValid=4;	// If the file name length > 4, the file extension might be valid

_LIT(KRingToneExtension1, ".rng");
_LIT(KRingToneExtension2, ".nrt");
_LIT(KRingToneExtension3, ".ota");
_LIT8(KDataTypeRingingTone,"application/vnd.nokia.ringing-tone");



CApaRingingToneRecognizer::CApaRingingToneRecognizer()
    : CApaDataRecognizerType(KUidRingingToneRecognizer, CApaDataRecognizerType::ENormal)
    {
    iCountDataTypes=1;
    }

CApaRingingToneRecognizer::~CApaRingingToneRecognizer()
    {
    // Do nothing
    }

TUint CApaRingingToneRecognizer::PreferredBufSize()
    {
    return KMaxBufferLength;
    }

TDataType CApaRingingToneRecognizer::SupportedDataTypeL(TInt /*aIndex*/) const
	{
//	__ASSERT_DEBUG(aIndex==0,User::Invariant());
	return TDataType(KDataTypeRingingTone);
	}

void CApaRingingToneRecognizer::DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer)
{
	TBool nameRecognized=EFalse;
	if (aName.Length()>KRecRtFileExtensionsMightBeValid)
	{
		if ((aName.Right(KRecRtFileExtensionsMightBeValid).CompareF(KRingToneExtension1)==0) ||
			(aName.Right(KRecRtFileExtensionsMightBeValid).CompareF(KRingToneExtension2)==0) ||
			(aName.Right(KRecRtFileExtensionsMightBeValid).CompareF(KRingToneExtension3)==0))
			nameRecognized=ETrue;
	}

	TInt length=aBuffer.Length();
	if (length<KMinBufferLength)
		return;		// too short to be a ring tone
	TBool validChars=EFalse;

	if(aBuffer[0] == KRecRtCommandByte )	//if it's a ring tone, it's not in unicode format
	{
		if (aBuffer[1] == KRecRtProgrammingByte && aBuffer[2] == KRecRtSoundByte)
			validChars = ETrue;
	}
	else if (aBuffer[0] == KRecRtUnicodeCommandByte)	// if it's a ring tone, it is in unicode format	
	{	
		if (aBuffer[1] == KRecRtProgrammingByte && aBuffer[2] == KRecRtUnicodeByte && aBuffer[3] == KRecRtSoundByte)
			validChars = ETrue;
	}
	else if(aBuffer[0] == 0x00 && aBuffer[1] == 0x11 && aBuffer[2] == 0x06)
	{
		validChars = ETrue;
	}

	if (!validChars)
		return;		// not a ringing tone
	else 
	{
		if (!nameRecognized)
			iConfidence=EProbable;	// is probably a ringing tone
		else
			iConfidence=ECertain;	// is certainly a ringing tone
		iDataType=TDataType(KDataTypeRingingTone);
	}
}



CApaDataRecognizerType* CApaRingingToneRecognizer::CreateRecognizerL()
    {
    return new (ELeave) CApaRingingToneRecognizer();
    }

const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(KRingingToneRecognizerImplementationUid, CApaRingingToneRecognizer::CreateRecognizerL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


// End of file
