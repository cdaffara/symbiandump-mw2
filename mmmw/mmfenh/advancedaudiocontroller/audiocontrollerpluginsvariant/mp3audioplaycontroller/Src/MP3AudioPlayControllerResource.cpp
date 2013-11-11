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
* Description:  This class implements resource file handling for MP3 PlayControllerPlugin.
*
*/



// INCLUDE FILES
#include	<mp3audioplaycontrollerresource.rsg>
#include	"MP3AudioPlayControllerResource.h"
#include "DebugMacros.h"

#include 	<data_caging_path_literals.hrh> 

// LOCAL CONSTANTS AND MACROS


_LIT(KDirAndFile, "z:MP3AudioPlayControllerResource.rsc");


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerResource::CMP3AudioPlayControllerResource
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerResource::CMP3AudioPlayControllerResource()
    {
    }

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerResource::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMP3AudioPlayControllerResource::ConstructL()
	{
	DP0(_L("CMP3AudioPlayControllerResource::ConstructL"));
//	ReadResourceFileL(KResourceFile, MP3DECODER);
	
	TParse* fp = new(ELeave) TParse(); 
	fp->Set(KDirAndFile, &KDC_RESOURCE_FILES_DIR, NULL); 

	ReadResourceFileL(fp->FullName(), MP3DECODER);
	delete fp;
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerResource::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMP3AudioPlayControllerResource* CMP3AudioPlayControllerResource::NewL()
	{
	DP0(_L("CMP3AudioPlayControllerResource::NewL"));
	CMP3AudioPlayControllerResource* self = new(ELeave) CMP3AudioPlayControllerResource();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
CMP3AudioPlayControllerResource::~CMP3AudioPlayControllerResource()
	{
	}

// -----------------------------------------------------------------------------
// CMP3AudioPlayControllerResource::ReadCodecConfigParametersL
// Reads the configuration parameters for MP3 Decoder.
// -----------------------------------------------------------------------------
//
void CMP3AudioPlayControllerResource::ReadCodecConfigParametersL()
	{
	DP0(_L("CMP3AudioPlayControllerResource::ReadCodecConfigParametersL"));
	iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// [0] Stereo to Mono Switch
	iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// [1] Decode Left or Right Channel
	iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// [2] Decimation Factor
	iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// [3] Concealment Level
	iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// [4] Sample Length
	iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// [5] Sample Frequency
    iCodecConfigParams.Append(NULL);                        	// [6] frame table pointer
    iCodecConfigParams.Append(NULL);							// [7] render control
    iCodecConfigParams.Append(NULL);							// [8] samples per frame
    iCodecConfigParams.Append(NULL);							// [9] mark play end control
    iCodecConfigParams.Append(NULL);							// [10] enable / disable
	}

//  End of File
