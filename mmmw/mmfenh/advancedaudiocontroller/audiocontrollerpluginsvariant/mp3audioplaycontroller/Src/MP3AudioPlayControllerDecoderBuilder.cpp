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
#include	"MP3AudioPlayControllerDecoderBuilder.h"
#include	"MP3AudioPlayControllerDecoder.h"
#include "DebugMacros.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoderBuilder::CMP3AudioPlayControllerDecoderBuilder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerDecoderBuilder::CMP3AudioPlayControllerDecoderBuilder()
    {
    }

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoderBuilder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMP3AudioPlayControllerDecoderBuilder::ConstructL()
	{
	DP0(_L("CMP3AudioPlayControllerDecoderBuilder::ConstructL"));
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoderBuilder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerDecoderBuilder* CMP3AudioPlayControllerDecoderBuilder::NewL()
	{
	DP0(_L("CMP3AudioPlayControllerDecoderBuilder::NewL"));
	CMP3AudioPlayControllerDecoderBuilder* self = new(ELeave) CMP3AudioPlayControllerDecoderBuilder();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
CMP3AudioPlayControllerDecoderBuilder::~CMP3AudioPlayControllerDecoderBuilder()
	{
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerDecoderBuilder::BuildDecoderL
// Creates a decoder of specified type.
// -----------------------------------------------------------------------------
//
CAdvancedAudioDecoder* CMP3AudioPlayControllerDecoderBuilder::BuildDecoderL(
	TInt /*aType*/ )
	{
	DP0(_L("CMP3AudioPlayControllerDecoderBuilder::BuildDecoderL"));
	// For CMMFCodec version, we use same decoder for play and conversion
	CMP3AudioPlayControllerDecoder* decoder = CMP3AudioPlayControllerDecoder::NewL();
	return decoder;
	}

//  End of File
