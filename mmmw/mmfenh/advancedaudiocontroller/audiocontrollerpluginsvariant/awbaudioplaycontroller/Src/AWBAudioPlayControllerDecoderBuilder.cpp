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
#include	"AWBAudioPlayControllerDecoderBuilder.h"
#include	"AWBAudioPlayControllerDecoder.h"
#include "DebugMacros.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoderBuilder::CAWBAudioPlayControllerDecoderBuilder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAWBAudioPlayControllerDecoderBuilder::CAWBAudioPlayControllerDecoderBuilder()
    {
    }

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoderBuilder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAWBAudioPlayControllerDecoderBuilder::ConstructL()
	{
    DP0(_L("CAWBAudioPlayControllerDecoderBuilder::ConstructL"));
	}

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoderBuilder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAWBAudioPlayControllerDecoderBuilder* CAWBAudioPlayControllerDecoderBuilder::NewL()
	{
    DP0(_L("CAWBAudioPlayControllerDecoderBuilder::NewL"));
	CAWBAudioPlayControllerDecoderBuilder* self = new(ELeave) CAWBAudioPlayControllerDecoderBuilder();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
CAWBAudioPlayControllerDecoderBuilder::~CAWBAudioPlayControllerDecoderBuilder()
	{
	}

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerDecoderBuilder::BuildDecoderL
// Creates a decoder of specified type.
// -----------------------------------------------------------------------------
//
CAdvancedAudioDecoder* CAWBAudioPlayControllerDecoderBuilder::BuildDecoderL(
	TInt /*aType*/ )
	{
    DP0(_L("CAWBAudioPlayControllerDecoderBuilder::BuildDecoderL"));
	// For CMMFCodec version, we use same decoder for play and conversion
	CAWBAudioPlayControllerDecoder* decoder = CAWBAudioPlayControllerDecoder::NewL();
	return decoder;
	}

//  End of File
