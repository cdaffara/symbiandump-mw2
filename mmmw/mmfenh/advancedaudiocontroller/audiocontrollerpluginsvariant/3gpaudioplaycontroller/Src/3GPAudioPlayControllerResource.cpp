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
* Description:  This class provides the main API for the specific codec resources.
*
*/



// INCLUDE FILES
#include	<3gpaudioplaycontrollerresource.rsg>
#include	"3GPAudioPlayControllerResource.h"
#include 	<data_caging_path_literals.hrh>

// LOCAL CONSTANTS AND MACROS
_LIT(KDirAndFile, "z:3GPAudioPlayControllerResource.rsc");


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerResource::C3GPAudioPlayControllerResource
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
C3GPAudioPlayControllerResource::C3GPAudioPlayControllerResource()
	:	iResourceType(0)
    {
    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerResource::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3GPAudioPlayControllerResource::ConstructL(
	TInt aResourceType )
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPAudioPlayControllerResource::ConstructL(%d)\n"), aResourceType);
#endif
	TInt resourceId = 0;
	switch ( aResourceType )
		{
		case K3GPResourceAAC:
			resourceId = AACDECODER;
			break;
		case K3GPResourceAMR:
			resourceId = AMRDECODER;
			break;
		case K3GPResourceAWB:
			resourceId = AWBDECODER;
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}
	iResourceType = aResourceType;

//	ReadResourceFileL(KResourceFile, resourceId);
	TParse* fp = new(ELeave) TParse();
	fp->Set(KDirAndFile, &KDC_RESOURCE_FILES_DIR, NULL);

	ReadResourceFileL(fp->FullName(), resourceId);
	delete fp;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioControllerResource::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
C3GPAudioPlayControllerResource* C3GPAudioPlayControllerResource::NewL(
	TInt aResourceType )
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPAudioPlayControllerResource::NewL\n"));
#endif
	C3GPAudioPlayControllerResource* self = new(ELeave) C3GPAudioPlayControllerResource();
	CleanupStack::PushL(self);
	self->ConstructL(aResourceType);
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
C3GPAudioPlayControllerResource::~C3GPAudioPlayControllerResource()
	{
	}

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerResource::ReadCodecConfigParametersL
// Reads the configuration parameters for the specified codec.
// -----------------------------------------------------------------------------
//
void C3GPAudioPlayControllerResource::ReadCodecConfigParametersL()
	{
	switch ( iResourceType )
		{
		case K3GPResourceAAC:

			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [0] Input Sampling Frequency
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [1] Input Channels
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [2] Input Profile
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [3] Input Frame Length
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [4] Down Mixing
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [5] Output Channel Selection
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [6] Decimation Factor
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [7] Downsample mode
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [8] Sample Size
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [9] Output Sampling Frequency
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [10] Extension Object Type
		    iCodecConfigParams.Append(NULL);                        // [11] frame table pointer
		    iCodecConfigParams.Append(NULL);                        // [12] format type - TEAacPlusFormat
		    iCodecConfigParams.Append(NULL);						// [13] render control
		    iCodecConfigParams.Append(NULL);						// [14] mark play end control
		    iCodecConfigParams.Append(NULL);						// [15] enable / disable

			break;

		case K3GPResourceAMR:
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [0] Concealment Level
		    iCodecConfigParams.Append(NULL);                        // [1] frame table pointer
		    iCodecConfigParams.Append(NULL);						// [2] render control
		    iCodecConfigParams.Append(NULL);						// [3] mark play end control
		    iCodecConfigParams.Append(NULL);						// [4] enable / disable
			break;

		case K3GPResourceAWB:
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [0] Decimation Factor
			iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [1] Concealment Level
		    iCodecConfigParams.Append(NULL);                        // [2] frame table pointer
		    iCodecConfigParams.Append(NULL);						// [3] render control
		    iCodecConfigParams.Append(NULL);						// [4] mark play end control
		    iCodecConfigParams.Append(NULL);						// [5] enable / disable
			break;

		default:
			User::Leave(KErrArgument);
			break;
		}
	}

//  End of File
