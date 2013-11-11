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
* Description:  This is a builder class for decoder object.
*
*/



// INCLUDE FILES
#include	"3GPAudioPlayControllerDecoderBuilder.h"
#include	"AACAudioPlayControllerDecoder.h"
#include	"AWBAudioPlayControllerDecoder.h"
#include	"AMRAudioPlayControllerDecoder.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerDecoderBuilder::C3GPAudioPlayControllerDecoderBuilder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
C3GPAudioPlayControllerDecoderBuilder::C3GPAudioPlayControllerDecoderBuilder()
    {
    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerDecoderBuilder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3GPAudioPlayControllerDecoderBuilder::ConstructL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPAudioPlayControllerDecoderBuilder::ConstructL"));
#endif
	}

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerDecoderBuilder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
C3GPAudioPlayControllerDecoderBuilder* C3GPAudioPlayControllerDecoderBuilder::NewL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPAudioPlayControllerDecoderBuilder::NewL"));
#endif
	C3GPAudioPlayControllerDecoderBuilder* self = new(ELeave) C3GPAudioPlayControllerDecoderBuilder();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
C3GPAudioPlayControllerDecoderBuilder::~C3GPAudioPlayControllerDecoderBuilder()
	{
	}

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerDecoderBuilder::BuildDecoderL
// Creates a decoder of specified type.
// -----------------------------------------------------------------------------
//
CAdvancedAudioDecoder* C3GPAudioPlayControllerDecoderBuilder::BuildDecoderL(
	TInt /*aType*/,
	const TFourCC& aSrcDatatype )
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPAudioPlayControllerDecoderBuilder::BuildDecoderL"));
#endif
	// For CMMFCodec version, we use same decoder for play and conversion
	if (aSrcDatatype == TFourCC(' ','A','M','R'))
		{
		CAMRAudioPlayControllerDecoder* decoder = CAMRAudioPlayControllerDecoder::NewL();
		return decoder;
		}
	else if (aSrcDatatype == TFourCC(' ','A','W','B'))
		{
		CAWBAudioPlayControllerDecoder* decoder = CAWBAudioPlayControllerDecoder::NewL();
		return decoder;
		}
	else if (aSrcDatatype == TFourCC(' ','E','A','C'))
		{
		CAACAudioPlayControllerDecoder* decoder = CAACAudioPlayControllerDecoder::NewL();
		return decoder;
		}
	else
		{
		User::Leave(KErrNotSupported);
		}
	return NULL; //to suppress warning
	}

//  End of File
