/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class implements resource file handling for AAC PlayControllerPlugin.
*
*/


// INCLUDE FILES
#include "AACAudioPlayControllerResource.h"
#include <aacaudioplaycontrollerresource.rsg>
#include <data_caging_path_literals.hrh>

// LOCAL CONSTANTS AND MACROS
_LIT(KDirAndFile, "z:AACAudioPlayControllerResource.rsc"); 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerResource::CAACAudioPlayControllerResource
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerResource::CAACAudioPlayControllerResource()
    {
    }

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerResource::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerResource::ConstructL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerResource::ConstructL"));
#endif
//	ReadResourceFileL(KResourceFile, AACDECODER);
	TParse* fp = new(ELeave) TParse(); 
	fp->Set(KDirAndFile, &KDC_RESOURCE_FILES_DIR, NULL); 

	ReadResourceFileL(fp->FullName(), AACDECODER);
	delete fp;
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerResource::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAACAudioPlayControllerResource* CAACAudioPlayControllerResource::NewL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerResource::NewL"));
#endif
	CAACAudioPlayControllerResource* self = new(ELeave) CAACAudioPlayControllerResource();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
CAACAudioPlayControllerResource::~CAACAudioPlayControllerResource()
	{
	}

// -----------------------------------------------------------------------------
// CAACAudioPlayControllerResource::ReadCodecConfigParametersL
// Reads the configuration parameters for AAC Decoder.
// -----------------------------------------------------------------------------
//
void CAACAudioPlayControllerResource::ReadCodecConfigParametersL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAACAudioPlayControllerResource::ReadCodecConfigParametersL"));
#endif
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [0] Input Sampling Freq
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [1] Input Channels
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [2] Input Profile
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [3] Input Frame Length
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [4] Down Mixing
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [5] Output Channel Selection
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [6] Decimation Factor
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [7] Concealment Level
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [8] Sample Length
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [9] Output Sample Frequency
	iCodecConfigParams.Append(iResourceReader.ReadInt32());	// [10] Ext Object Type
    iCodecConfigParams.Append(NULL);                        // [11] frame table pointer
    iCodecConfigParams.Append(NULL);                        // [12] format type - TEAacPlusFormat
    iCodecConfigParams.Append(NULL);						// [13] render control
    iCodecConfigParams.Append(NULL);						// [14] mark play end control
    iCodecConfigParams.Append(NULL);						// [15] enable / disable
	}

//  End of File
