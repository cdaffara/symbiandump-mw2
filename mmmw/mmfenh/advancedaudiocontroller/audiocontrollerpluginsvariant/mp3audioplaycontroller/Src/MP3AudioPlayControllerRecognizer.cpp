/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Recognizer for MP3 files.
*
*/



// INCLUDES
#include	<f32file.h>
#include	<barsread.h>
#include        <implementationproxy.h>
#include	"MP3AudioPlayControllerRecognizer.h"

// LOCAL CONSTANTS AND MACROS
const TUid KUidMimeMp3Recognizer = {0x101FAF95};
const TInt KExtLength = 4;

_LIT(KDotMp3, ".mp3");
_LIT8(KMp3MimeType, "audio/mpeg");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerRecognizer::CMP3AudioPlayControllerRecognizer()
// C++ default constructor can NOT contain any code, that
// might leave.
// Call base constructor with the recognizer's UID and confidence level
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerRecognizer::CMP3AudioPlayControllerRecognizer()
	:	CApaDataRecognizerType(KUidMimeMp3Recognizer, CApaDataRecognizerType::EHigh)
	{
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerRecognizer::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------

void CMP3AudioPlayControllerRecognizer::ConstructL()
	{
	// Set the number of data (MIME) types supported by this recognizer.
	iCountDataTypes = 1;
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerRecognizer::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
CMP3AudioPlayControllerRecognizer* CMP3AudioPlayControllerRecognizer::NewL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMP3AudioPlayControllerRecognizer::NewL"));
#endif
	CMP3AudioPlayControllerRecognizer* self = new (ELeave) CMP3AudioPlayControllerRecognizer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// Destructor
CMP3AudioPlayControllerRecognizer::~CMP3AudioPlayControllerRecognizer()
	{
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerRecognizer::SupportedDataTypeL
// Return the MimeType of the DataType depending on
// mime type of the file
// -----------------------------------------------------------------------------

TDataType CMP3AudioPlayControllerRecognizer::SupportedDataTypeL(
	TInt aIndex ) const
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMP3AudioPlayControllerRecognizer::SupportedDataTypeL [%d]"), aIndex);
#endif
	TDataType dataType;
	switch ( aIndex )
		{
		case 0:
			dataType = TDataType(KMp3MimeType);
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}
	return dataType;
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerRecognizer::PreferredBufSize()
// Return the supposed minimum buffer size we need to
// successfully recognize the data
// -----------------------------------------------------------------------------

TUint CMP3AudioPlayControllerRecognizer::PreferredBufSize()
	{
	return 0;
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerRecognizer::DoRecognizeL
// Attempt to recognize the data
// this recognizer only attempts to match the data and/or file suffix
//
// NB if the file is not recognized, this function should NOT leave :
// it should instead set iConfidence = ENotRecognized and return
// the function should only leave if there is an out-of-memory condition
// -----------------------------------------------------------------------------

void CMP3AudioPlayControllerRecognizer::DoRecognizeL(
	const TDesC& aName,
	const TDesC8& /*aBuffer*/ )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMP3AudioPlayControllerRecognizer::DoRecognizeL: "));
	RDebug::RawPrint(aName);
	RDebug::Print(_L(" "));
#endif
	// assume match will fail
	iConfidence = CApaDataRecognizerType::ENotRecognized;
	if ( aName.Length() < KExtLength )
		{
#ifdef _DEBUG
	RDebug::Print(_L("CMP3AudioPlayControllerRecognizer::DoRecognizeL - Ext not present"));
#endif
		return;
		}

	if ( aName.Right( KDotMp3.iTypeLength ).CompareF(KDotMp3) == 0 )
		{
#ifdef _DEBUG
	RDebug::Print(_L("CMP3AudioPlayControllerRecognizer::DoRecognizeL - Possible match"));
#endif
		iDataType = TDataType(KMp3MimeType);
		iConfidence = EProbable;
		}
  	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// CApaDataRecognizerType* CreateRecognizerL()
// Used to create an instance of a CApaDataRecognizerType-derived class
// -----------------------------------------------------------------------------
CApaDataRecognizerType* CMP3AudioPlayControllerRecognizer::CreateRecognizerL()
	{
	return new (ELeave) CMP3AudioPlayControllerRecognizer ();
	}

const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(0x101FAFD8, CMP3AudioPlayControllerRecognizer::CreateRecognizerL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

// End of file


