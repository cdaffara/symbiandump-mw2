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
#include	<3gpaudiorecordcontrollerresource.rsg>
#include	"3GPAudioRecordControllerResource.h"
#include 	<data_caging_path_literals.hrh>

// LOCAL CONSTANTS AND MACROS
_LIT(KDirAndFile, "z:3GPAudioRecordControllerResource.rsc");


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerResource::C3GPAudioRecordControllerResource
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
C3GPAudioRecordControllerResource::C3GPAudioRecordControllerResource()
	:	iResourceType(0)
    {
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerResource::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerResource::ConstructL(
	TInt aResourceType )
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPAudioRecordControllerResource::ConstructL(%d)\n"), aResourceType);
#endif
	TInt resourceId = 0;
	switch ( aResourceType )
		{
		case K3GPResourceAAC:
			resourceId = AACENCODER;
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}
	iResourceType = aResourceType;

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
C3GPAudioRecordControllerResource* C3GPAudioRecordControllerResource::NewL(
	TInt aResourceType )
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPAudioRecordControllerResource::NewL\n"));
#endif
	C3GPAudioRecordControllerResource* self = new(ELeave) C3GPAudioRecordControllerResource();
	CleanupStack::PushL(self);
	self->ConstructL(aResourceType);
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
C3GPAudioRecordControllerResource::~C3GPAudioRecordControllerResource()
	{
	iSupportedBitRates.Close();
	iSupportedSampleRates.Close();
	}

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerResource::SupportedSampleRatesL
// Returns the supported sample rates.
// -----------------------------------------------------------------------------
//
const RArray<TUint>& C3GPAudioRecordControllerResource::SupportedSampleRatesL()
	{
	return iSupportedSampleRates;
	}

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerResource::SupportedBitRatesL
// Returns the supported bit rates.
// -----------------------------------------------------------------------------
//
const RArray<TUint>& C3GPAudioRecordControllerResource::SupportedBitRatesL()
	{
	return iSupportedBitRates;
	}

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerResource::ReadCodecConfigParametersL
// Reads the configuration parameters for the specified codec.
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerResource::ReadCodecConfigParametersL()
	{
	switch ( iResourceType )
		{
		case K3GPResourceAAC:
			{
			iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// bitrate
			iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// sampling_frequency
			iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// tool_flag
			iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// num_of_channels
			iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// use_format
			iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// fill_buffer
			TInt number = iResourceReader.ReadInt16();
			for ( TInt i = 0; i < number; i++ )	// supported bit rates
				{
				iSupportedBitRates.Append( iResourceReader.ReadInt32() );	// Bit Rate
				}
		    number = iResourceReader.ReadInt16();

			for ( TInt i = 0; i < number; i++ )	// supported sample rates
				{
				iSupportedSampleRates.Append( iResourceReader.ReadInt32() );	// Sample Rate
				}
			}
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}
	}

//  End of File
