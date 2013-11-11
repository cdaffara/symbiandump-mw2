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
* Description:  This class implements resource file handling for AMR PlayControllerPlugin.
*
*/



// INCLUDE FILES
#include	<amraudioplaycontrollerresource.rsg>
#include	"AMRAudioPlayControllerResource.h"
#include "DebugMacros.h"
#include 	<data_caging_path_literals.hrh> 

// LOCAL CONSTANTS AND MACROS

_LIT(KDirAndFile,"z:AMRAudioPlayControllerResource.rsc"); 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerResource::CAMRAudioPlayControllerResource
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAMRAudioPlayControllerResource::CAMRAudioPlayControllerResource()
    {
    }

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerResource::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAMRAudioPlayControllerResource::ConstructL()
	{
	DP0(_L("CAMRAudioPlayControllerResource::ConstructL"));
	TParse* fp = new(ELeave) TParse();
	CleanupStack::PushL(fp);
	fp->Set(KDirAndFile, &KDC_RESOURCE_FILES_DIR, NULL);
	ReadResourceFileL(fp->FullName(), AMRDECODER);
	CleanupStack::Pop(fp);
	delete fp;
	}

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerResource::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAMRAudioPlayControllerResource* CAMRAudioPlayControllerResource::NewL()
	{
	DP0(_L("CAMRAudioPlayControllerResource::NewL"));
	CAMRAudioPlayControllerResource* self = new(ELeave) CAMRAudioPlayControllerResource();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
CAMRAudioPlayControllerResource::~CAMRAudioPlayControllerResource()
	{
	}

// -----------------------------------------------------------------------------
// CAMRAudioPlayControllerResource::ReadCodecConfigParametersL
// Reads the configuration parameters for AMR Decoder.
// -----------------------------------------------------------------------------
//
void CAMRAudioPlayControllerResource::ReadCodecConfigParametersL()
	{
	DP0(_L("CAMRAudioPlayControllerResource::ReadCodecConfigParametersL"));
	iCodecConfigParams.Append( iResourceReader.ReadInt32() ); // [0] Concealment Level
    iCodecConfigParams.Append(NULL);                          // [1] frame table pointer
    iCodecConfigParams.Append(NULL);						  // [2] render control
    iCodecConfigParams.Append(NULL);						  // [3] mark play end control
    iCodecConfigParams.Append(NULL);						  // [4] enable / disable
	}

//  End of File
