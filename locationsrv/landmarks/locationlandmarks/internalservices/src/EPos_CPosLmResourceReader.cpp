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
* Description: Simplified interface to resource files
*
*
*/


// INCLUDE FILES
#include "EPos_CPosLmResourceReader.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmResourceReader::CPosLmResourceReader
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmResourceReader::CPosLmResourceReader()
    :iFileSession(),
    iResourceFile(),
    iResourceBuffer(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmResourceReader::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmResourceReader::ConstructL(const TDesC& aPath)
    {
    User::LeaveIfError(iFileSession.Connect());

    TFindFile* filefinder = new (ELeave) TFindFile(iFileSession);
    CleanupStack::PushL(filefinder);

    TParse* fileparser = new (ELeave) TParse;
    CleanupStack::PushL(fileparser);
    fileparser->Set(aPath, NULL, NULL);

    User::LeaveIfError(filefinder->FindByDir(fileparser->NameAndExt(),
        fileparser->DriveAndPath()));

    iResourceFile.OpenL(iFileSession, filefinder->File());
    iResourceFile.ConfirmSignatureL(0);

    CleanupStack::PopAndDestroy(2, filefinder); // fileparser
    }

// -----------------------------------------------------------------------------
// CPosLmResourceReader::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmResourceReader* CPosLmResourceReader::NewLC(const TDesC& aPath)
    {
    CPosLmResourceReader* self = new (ELeave) CPosLmResourceReader;
    CleanupStack::PushL( self );
    self->ConstructL(aPath);
    return self;
    }

// Destructor
EXPORT_C CPosLmResourceReader::~CPosLmResourceReader()
    {
    delete iResourceBuffer;
    iResourceFile.Close();
    iFileSession.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmResourceReader::ReadHBufCL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC* CPosLmResourceReader::ReadHBufCL(TInt aResourceId)
    {
    LoadResourceL(aResourceId);
    return iResourceReader.ReadHBufCL();
    }

// -----------------------------------------------------------------------------
// CPosLmResourceReader::ReadInt16L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmResourceReader::ReadInt16L(TInt aResourceId)
    {
    LoadResourceL(aResourceId);
    return iResourceReader.ReadInt16();
    }

// -----------------------------------------------------------------------------
// CPosLmResourceReader::ReadInt32L
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmResourceReader::ReadInt32L(TInt aResourceId)
    {
    LoadResourceL(aResourceId);
    return iResourceReader.ReadInt32();
    }

// -----------------------------------------------------------------------------
// CPosLmResourceReader::LoadResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmResourceReader::LoadResourceL(TInt aResourceId)
   {
   if (iResourceBuffer)
       {
       delete iResourceBuffer;
       iResourceBuffer = NULL;
       }
   iResourceBuffer = iResourceFile.AllocReadL(aResourceId);
   iResourceReader.SetBuffer(iResourceBuffer);
   }

//  End of File
