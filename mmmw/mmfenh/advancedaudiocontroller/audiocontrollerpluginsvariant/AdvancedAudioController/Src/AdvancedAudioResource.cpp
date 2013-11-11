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
* Description:  This class provides the main API for the Advanced Audio Controller
*				 Resource.  This class is the base class for the specific codec resource
*				 classes.
*
*/



// INCLUDE FILES
#include	"AdvancedAudioResource.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAdvancedAudioResource::CAdvancedAudioResource
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CAdvancedAudioResource::CAdvancedAudioResource()
	:	iResourceBuf(NULL),
		iResourceOpen(EFalse)
    {
    }

// Destructor
EXPORT_C CAdvancedAudioResource::~CAdvancedAudioResource()
	{
	iCodecConfigParams.Close();

	if ( iResourceOpen )
		{
		delete iResourceBuf;
		iResourceFile.Close();
		iFs.Close();
		}
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioResource::PropertiesL
// Returns the controller's properties for the current Codec.
// -----------------------------------------------------------------------------
//
EXPORT_C const TAapProperties& CAdvancedAudioResource::PropertiesL()
	{
	return iProperties;
	}

// -----------------------------------------------------------------------------
// CAapResourceAMRDecoder::ConfigurationParametersL
// Returns the configuration parameters for AMR Decoder.
// -----------------------------------------------------------------------------
//
EXPORT_C const RArray<TInt>& CAdvancedAudioResource::CodecConfigParametersL()
	{
	return iCodecConfigParams;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioResource::ReadResourceFileL
// Reads the data from the resource file for the specified codec.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioResource::ReadResourceFileL(
	const TDesC& aResourceFileName,
	TInt aResourceId )
	{
	iResourceOpen = ETrue;
	// Connect to the file server
	User::LeaveIfError(iFs.Connect());
	// Open the resource file
	iResourceFile.OpenL(iFs, aResourceFileName);

	iResourceBuf = iResourceFile.AllocReadL(aResourceId);
	iResourceReader.SetBuffer(iResourceBuf);

	iProperties.iSharedBufferMaxNum = iResourceReader.ReadInt32();
	iProperties.iSharedBufferMaxSize = iResourceReader.ReadInt32();
	iProperties.iSharedBufferMaxSizeForNonSeekableSrc = iResourceReader.ReadInt32();
	iProperties.iSamplingRate = iResourceReader.ReadInt32();
	iProperties.iStereoSupport = iResourceReader.ReadInt32();
	iProperties.iMetaDataSupport = iResourceReader.ReadInt32();

	ReadCodecConfigParametersL();

	// Cleanup
	delete iResourceBuf;
	iResourceBuf = NULL;
	iResourceFile.Close();
	iFs.Close();
	iResourceOpen = EFalse;
	}

//  End of File
