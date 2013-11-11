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
* Description:  This class implements resource file handling for AWB PlayControllerPlugin.
*
*/



// INCLUDE FILES
#include	<awbaudioplaycontrollerresource.rsg>
#include	"AWBAudioPlayControllerResource.h"
#include 	<data_caging_path_literals.hrh> 
#include "DebugMacros.h"

// LOCAL CONSTANTS AND MACROS


_LIT(KDirAndFile,"z:AWBAudioPlayControllerResource.rsc"); 


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerResource::CAWBAudioPlayControllerResource
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAWBAudioPlayControllerResource::CAWBAudioPlayControllerResource()
    {
    }

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerResource::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAWBAudioPlayControllerResource::ConstructL()
	{
    DP0(_L("CAWBAudioPlayControllerResource::ConstructL"));
	TParse* fp = new(ELeave) TParse(); 
	CleanupStack::PushL(fp);
	fp->Set(KDirAndFile, &KDC_RESOURCE_FILES_DIR, NULL); 
	ReadResourceFileL(fp->FullName(), AWBDECODER);
	CleanupStack::Pop(fp);
	delete fp;
	}

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerResource::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAWBAudioPlayControllerResource* CAWBAudioPlayControllerResource::NewL()
	{
    DP0(_L("CAWBAudioPlayControllerResource::NewL"));
	CAWBAudioPlayControllerResource* self = new(ELeave) CAWBAudioPlayControllerResource();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
CAWBAudioPlayControllerResource::~CAWBAudioPlayControllerResource()
	{
	}

// -----------------------------------------------------------------------------
// CAWBAudioPlayControllerResource::ReadCodecConfigParametersL
// Reads the configuration parameters for AWB Decoder.
// -----------------------------------------------------------------------------
//
void CAWBAudioPlayControllerResource::ReadCodecConfigParametersL()
	{
    DP0(_L("CAWBAudioPlayControllerResource::ReadCodecConfigParametersL"));
	iCodecConfigParams.Append( iResourceReader.ReadInt32() ); // [0] Decimation Factor
	iCodecConfigParams.Append( iResourceReader.ReadInt32() ); // [1] Concealment Level
    iCodecConfigParams.Append(NULL);                          // [2] frame table pointer
    iCodecConfigParams.Append(NULL);						  // [3] render control
    iCodecConfigParams.Append(NULL);						  // [4] mark play end control
    iCodecConfigParams.Append(NULL);						  // [5] enable / disable

	}

//  End of File
