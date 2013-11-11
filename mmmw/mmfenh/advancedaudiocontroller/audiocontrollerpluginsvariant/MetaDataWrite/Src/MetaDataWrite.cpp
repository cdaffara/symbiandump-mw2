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
* Description:  Implementation file for MetaDataWrite
*
*/


// INCLUDES
#include "MetaDataWrite.h"
#include "MetaDataWrite.hrh"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataWrite::NewL
// Two-phased constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CMetaDataWrite* CMetaDataWrite::NewL(CMdaAudioRecorderUtility& aUtility)
{
    CMetaDataWrite* self = new(ELeave) CMetaDataWrite(aUtility);
    return self;
}

// -----------------------------------------------------------------------------
// CMetaDataWrite::EnableDelayWrite
// Enable delay write
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMetaDataWrite::EnableDelayWrite()
{
    return iUtility.RecordControllerCustomCommandSync(iDestinationPckg, KMetaDataWriteEnableDelayWrite, KNullDesC8, KNullDesC8);
}

// -----------------------------------------------------------------------------
// CMetaDataWrite::CommitMetaData
// Commit meta data
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMetaDataWrite::CommitMetaData()
{
    return iUtility.RecordControllerCustomCommandSync(iDestinationPckg, KMetaDataWriteCommitMetaData, KNullDesC8, KNullDesC8);
}

// -----------------------------------------------------------------------------
// CMetaDataWrite::CommitMetaData
// C++ default constructor can NOT contain any code that might leave
// -----------------------------------------------------------------------------
//
CMetaDataWrite::CMetaDataWrite(CMdaAudioRecorderUtility& aUtility) :
    iUtility(aUtility)
{
    TMMFMessageDestination destination(KUidMetaDataWriteCustomCommand);
	iDestinationPckg = TMMFMessageDestinationPckg(destination);
}

// End of file
