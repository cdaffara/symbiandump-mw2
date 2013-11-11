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
* Description:  This is a builder class for Encoder object.
*
*/



// INCLUDE FILES
#include	"3GPAudioRecordControllerEncoderBuilder.h"
#include	"3GPAACAudioRecordControllerEncoder.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerEncoderBuilder::C3GPAudioRecordControllerEncoderBuilder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
C3GPAudioRecordControllerEncoderBuilder::C3GPAudioRecordControllerEncoderBuilder()
    {
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerEncoderBuilder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerEncoderBuilder::ConstructL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPAudioRecordControllerEncoderBuilder::ConstructL"));
#endif
	}

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerEncoderBuilder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
C3GPAudioRecordControllerEncoderBuilder* C3GPAudioRecordControllerEncoderBuilder::NewL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPAudioRecordControllerEncoderBuilder::NewL"));
#endif
	C3GPAudioRecordControllerEncoderBuilder* self = new(ELeave) C3GPAudioRecordControllerEncoderBuilder();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
C3GPAudioRecordControllerEncoderBuilder::~C3GPAudioRecordControllerEncoderBuilder()
	{
	}

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerEncoderBuilder::BuildEncoderL
// Creates a Encoder of specified type.
// -----------------------------------------------------------------------------
//
CAdvancedAudioEncoder* C3GPAudioRecordControllerEncoderBuilder::BuildEncoderL(
	TInt /*aType*/,
	const TFourCC& aSinkDatatype )
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPAudioRecordControllerEncoderBuilder::BuildEncoderL"));
#endif
	// For CMMFCodec version, we use same Encoder for Record and conversion

		C3GPAACAudioRecordControllerEncoder* encoder = C3GPAACAudioRecordControllerEncoder::NewL(aSinkDatatype);
		return encoder;
	}

//  End of File
