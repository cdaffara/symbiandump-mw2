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
#include	"AMRAudioPlayControllerDecoderBuilder.h"
#include	"AMRAudioPlayControllerDecoder.h"
#include "DebugMacros.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoderBuilder::CAMRAudioPlayControllerDecoderBuilder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAMRAudioPlayControllerDecoderBuilder::CAMRAudioPlayControllerDecoderBuilder()
    {
    }

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoderBuilder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAMRAudioPlayControllerDecoderBuilder::ConstructL()
	{
	DP0(_L("CAMRAudioPlayControllerDecoderBuilder::ConstructL"));
	}

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoderBuilder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAMRAudioPlayControllerDecoderBuilder* CAMRAudioPlayControllerDecoderBuilder::NewL()
	{
	DP0(_L("CAMRAudioPlayControllerDecoderBuilder::NewL"));
	CAMRAudioPlayControllerDecoderBuilder* self = new(ELeave) CAMRAudioPlayControllerDecoderBuilder();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
CAMRAudioPlayControllerDecoderBuilder::~CAMRAudioPlayControllerDecoderBuilder()
	{
	}

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerDecoderBuilder::BuildDecoderL
// Creates a decoder of specified type.
// -----------------------------------------------------------------------------
//
CAdvancedAudioDecoder* CAMRAudioPlayControllerDecoderBuilder::BuildDecoderL(
	TInt /*aType*/ )
	{
	DP0(_L("CAMRAudioPlayControllerDecoderBuilder::BuildDecoderL"));
	// For CMMFCodec version, we use same decoder for play and conversion
	CAMRAudioPlayControllerDecoder* decoder = CAMRAudioPlayControllerDecoder::NewL();
	return decoder;
	}

//  End of File
