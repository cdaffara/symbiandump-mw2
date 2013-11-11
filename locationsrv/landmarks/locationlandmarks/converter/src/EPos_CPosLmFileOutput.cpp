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
* Description: File output for encoder.
*
*
*/


// INCLUDE FILES
#include    <epos_cposlmdiskutilities.h>
#include    "EPos_CPosLmFileOutput.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmFileOutput::CPosLmFileOutput
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmFileOutput::CPosLmFileOutput()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmFileOutput::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmFileOutput::ConstructL(const TDesC& aOutputFile)
    {
    BaseConstructL(iFileWriteStream);

    User::LeaveIfError(iFs.Connect());

    User::LeaveIfError(iFileWriteStream.Create(
        iFs, aOutputFile, EFileShareExclusive | EFileStreamText | EFileWrite));

    iDiskUtilities = CPosLmDiskUtilities::NewL();

    // Get the drive letter (used by disk utilities)
    TParse filenameParser;
    iFs.Parse(aOutputFile, filenameParser);
    // Drive() always returns [drive]: even if the drive is not a part of the
    // path, see RFs::Parse for further info.
    iDbDrive = filenameParser.Drive()[0];
    }

// -----------------------------------------------------------------------------
// CPosLmFileOutput::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmFileOutput* CPosLmFileOutput::NewL(const TDesC& aOutputFile)
    {
    CPosLmFileOutput* self = new (ELeave) CPosLmFileOutput;
    CleanupStack::PushL(self);
    self->ConstructL(aOutputFile);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmFileOutput::~CPosLmFileOutput()
    {
    iFileWriteStream.Close();
    iFs.Close();
    delete iDiskUtilities;
    }

// -----------------------------------------------------------------------------
// CPosLmFileOutput::CheckLowDiskL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmFileOutput::CheckLowDiskL(
    const TDesC16& aDesc)
    {
    iDiskUtilities->DiskSpaceBelowCriticalLevelL(aDesc.Size(), iDbDrive);
    }

//  End of File
