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
* Description:  This class implements resource file handling for AMR RecordControllerPlugin.
*
*/



// INCLUDE FILES
#include	<amraudiorecordcontrollerresource.rsg>
#include	"AMRAudioRecordControllerResource.h"
#include 	<data_caging_path_literals.hrh>

// LOCAL CONSTANTS AND MACROS

_LIT(KDirAndFile,"z:AMRAudioRecordControllerResource.rsc");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerResource::CAMRAudioRecordControllerResource
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAMRAudioRecordControllerResource::CAMRAudioRecordControllerResource()
    {
    }

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerResource::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAMRAudioRecordControllerResource* CAMRAudioRecordControllerResource::NewL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAMRAudioRecordControllerResource::NewL"));
#endif
	CAMRAudioRecordControllerResource* self = new(ELeave) CAMRAudioRecordControllerResource();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
	}

// Destructor
CAMRAudioRecordControllerResource::~CAMRAudioRecordControllerResource()
	{
	iSupportedBitRates.Close();
	iSupportedSampleRates.Close();
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerResource::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerResource::ConstructL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAMRAudioRecordControllerResource::ConstructL"));
#endif

	TParse* fp = new(ELeave) TParse();
	fp->Set(KDirAndFile, &KDC_RESOURCE_FILES_DIR, NULL);
	ReadResourceFileL(fp->FullName(), AMRENCODER);
	delete fp;
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerResource::SupportedSampleRatesL
// Returns the supported sample rates.
// -----------------------------------------------------------------------------
//
const RArray<TUint>& CAMRAudioRecordControllerResource::SupportedSampleRatesL()
	{
	return iSupportedSampleRates;
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerResource::SupportedBitRatesL
// Returns the supported bit rates.
// -----------------------------------------------------------------------------
//
const RArray<TUint>& CAMRAudioRecordControllerResource::SupportedBitRatesL()
	{
	return iSupportedBitRates;
	}

// -----------------------------------------------------------------------------
// CAMRAudioRecordControllerResource::ReadCodecConfigParametersL
// Reads the configuration parameters for AMR Decoder.
// -----------------------------------------------------------------------------
//
void CAMRAudioRecordControllerResource::ReadCodecConfigParametersL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CAMRAudioRecordControllerResource::ReadCodecConfigParametersL"));
#endif
	iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// Encoding Mode
	iCodecConfigParams.Append( iResourceReader.ReadInt32() );	// DTX Flag

	TInt number = iResourceReader.ReadInt16();
	for ( TInt i = 0; i < number; i++ )
		{
		iSupportedBitRates.Append( iResourceReader.ReadInt32() );	// Bit Rate
		}
	number = iResourceReader.ReadInt16();
	for ( TInt i = 0; i < number; i++ )
		{
		iSupportedSampleRates.Append( iResourceReader.ReadInt32() );	// Sample Rate
		}
	}

//  End of File
