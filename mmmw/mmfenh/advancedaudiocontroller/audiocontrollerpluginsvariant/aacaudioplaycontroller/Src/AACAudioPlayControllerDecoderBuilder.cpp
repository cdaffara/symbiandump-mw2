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
#include	"AACAudioPlayControllerDecoderBuilder.h"
#include	"AACAudioPlayControllerDecoder.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoderBuilder::CAACAudioPlayControllerDecoderBuilder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerDecoderBuilder::CAACAudioPlayControllerDecoderBuilder()
    {
    }

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoderBuilder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerDecoderBuilder::ConstructL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerDecoderBuilder::ConstructL"));
#endif
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoderBuilder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerDecoderBuilder* CAACAudioPlayControllerDecoderBuilder::NewL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerDecoderBuilder::NewL"));
#endif
	CAACAudioPlayControllerDecoderBuilder* self = new(ELeave) CAACAudioPlayControllerDecoderBuilder();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
CAACAudioPlayControllerDecoderBuilder::~CAACAudioPlayControllerDecoderBuilder()
	{
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerDecoderBuilder::BuildDecoderL
// Creates a decoder of specified type.
// -----------------------------------------------------------------------------
//
CAdvancedAudioDecoder* CAACAudioPlayControllerDecoderBuilder::BuildDecoderL(
	TInt /*aType*/ )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerDecoderBuilder::BuildDecoderL"));
#endif
	// For CMMFCodec version, we use same decoder for play and conversion
	CAACAudioPlayControllerDecoder* decoder = CAACAudioPlayControllerDecoder::NewL();
	return decoder;
	}

//  End of File
