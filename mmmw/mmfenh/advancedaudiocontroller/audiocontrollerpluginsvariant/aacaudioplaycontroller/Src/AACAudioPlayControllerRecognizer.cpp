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
* Description:  Recognizer for AAC files.
*
*/



// INCLUDES
#include	<f32file.h>
#include	<barsread.h>
#include	<implementationproxy.h>
#include	"AACAudioPlayControllerRecognizer.h"

// LOCAL CONSTANTS AND MACROS
const TUid KUidMimeAacRecognizer = {0x101FAF96};
const TInt KExtLength = 4;

_LIT(KDotAac, ".aac");
_LIT8(KAacMimeType, "audio/aac");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerRecognizer::CAACAudioPlayControllerRecognizer()
// C++ default constructor can NOT contain any code, that
// might leave.
// Call base constructor with the recognizer's UID and confidence level
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerRecognizer::CAACAudioPlayControllerRecognizer()
	:	CApaDataRecognizerType(KUidMimeAacRecognizer, CApaDataRecognizerType::EHigh)
	{
	// Set the number of data (MIME) types supported by this recognizer.
	iCountDataTypes = 1;
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerRecognizer::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerRecognizer::ConstructL()
	{
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerRecognizer::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerRecognizer* CAACAudioPlayControllerRecognizer::NewL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerRecognizer::NewL"));
#endif
	CAACAudioPlayControllerRecognizer* self = new (ELeave) CAACAudioPlayControllerRecognizer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// Destructor
CAACAudioPlayControllerRecognizer::~CAACAudioPlayControllerRecognizer()
	{
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerRecognizer::SupportedDataTypeL
// Return the MimeType of the DataType depending on
// mime type of the file
// -----------------------------------------------------------------------------
//
TDataType CAACAudioPlayControllerRecognizer::SupportedDataTypeL(
	TInt aIndex ) const
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerRecognizer::SupportedDataTypeL [%d]"), aIndex);
#endif
	TDataType dataType;
	switch ( aIndex )
		{
		case 0:
			dataType = TDataType(KAacMimeType);
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}
	return dataType;
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerRecognizer::PreferredBufSize()
// Return the supposed minimum buffer size we need to
// successfully recognize the data
// -----------------------------------------------------------------------------
//
TUint CAACAudioPlayControllerRecognizer::PreferredBufSize()
	{
	return 0;
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerRecognizer::DoRecognizeL
// Attempt to recognize the data
// this recognizer only attempts to match the data and/or file suffix
//
// NB if the file is not recognized, this function should NOT leave :
// it should instead set iConfidence = ENotRecognized and return
// the function should only leave if there is an out-of-memory condition
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerRecognizer::DoRecognizeL(
	const TDesC& aName,
	const TDesC8& /*aBuffer*/ )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerRecognizer::DoRecognizeL: "));
	RDebug::RawPrint(aName);
	RDebug::Print(_L(" "));
#endif
	// assume match will fail
	iConfidence = CApaDataRecognizerType::ENotRecognized;
	if ( aName.Length() < KExtLength )
		{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerRecognizer::DoRecognizeL - Ext not present"));
#endif
		return;
		}

	if ( aName.Right( KDotAac.iTypeLength ).CompareF(KDotAac) == 0 )
		{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerRecognizer::DoRecognizeL - Possible match!"));
#endif
		iDataType = TDataType(KAacMimeType);
		iConfidence = EProbable;
		}
  	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// CApaDataRecognizerType* CreateRecognizerL()
// Used to create an instance of a CApaDataRecognizerType-derived class
// -----------------------------------------------------------------------------
//
CApaDataRecognizerType* CAACAudioPlayControllerRecognizer::CreateRecognizerL()
	{
	return new (ELeave) CAACAudioPlayControllerRecognizer ();
	}

const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(0x101FAFD7, CAACAudioPlayControllerRecognizer::CreateRecognizerL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

// End of file
