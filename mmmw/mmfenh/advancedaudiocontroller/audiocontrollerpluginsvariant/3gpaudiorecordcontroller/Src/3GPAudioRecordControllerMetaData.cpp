/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation file for 3GPAudioRecordControllerMetaData
*
*/


// INCLUDES
#include "MetaDataWrite.h"
#include "3GPAudioRecordControllerMetaData.h"
#include "3GPAudioMetaDataComposer.h"
#include "3GPAudioRecordControllerPlugin.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerMetaData::NewL
// Two-phased constructor
// -----------------------------------------------------------------------------
//
C3GPAudioRecordControllerMetaData* C3GPAudioRecordControllerMetaData::NewL()
    {
    C3GPAudioRecordControllerMetaData* self = new(ELeave) C3GPAudioRecordControllerMetaData();
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerMetaData::~C3GPAudioRecordControllerMetaData
// Destructor
// -----------------------------------------------------------------------------
//
C3GPAudioRecordControllerMetaData::~C3GPAudioRecordControllerMetaData()
    {
    delete iMetaDataComposer;
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerMetaData
// C++ default constructor
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerMetaData::ConstructL()
    {
    iMetaDataComposer = C3GPAudioMetaDataComposer::NewL();
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerMetaData::AddMetaDataWriteParserL
// Add a MetaDataWrite Parser to the Parent.
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerMetaData::AddMetaDataWriteParserL(C3GPAudioRecordControllerPlugin* aParent)
{
    aParent->AddMetaDataWriteParserL();
}

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerMetaData::AddMetaDataEntryL
// Add and remember meta data entry to be written later
// -----------------------------------------------------------------------------
//
void C3GPAudioRecordControllerMetaData::AddMetaDataEntryL(const CMMFMetaDataEntry& aNewEntry)
    {
    iMetaDataComposer->AddMetaDataEntryL(aNewEntry);
    }

// -----------------------------------------------------------------------------
// C3GPAudioRecordControllerMetaData::GetUdtaAtomL
// Return a buffer that contain UDTA atom
// -----------------------------------------------------------------------------
//
HBufC8* C3GPAudioRecordControllerMetaData::GetUdtaAtomL()
    {
    return iMetaDataComposer->GetUdtaAtomL();
    }

// End of File
