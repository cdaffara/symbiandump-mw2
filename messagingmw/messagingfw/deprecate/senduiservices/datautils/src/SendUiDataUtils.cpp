/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Utility class for SendUI.
*
*/




// INCLUDE FILES
#include    "SendUiDataUtils.h"
#include    <eikenv.h>
#include    <apgcli.h>

_LIT8( KMmsSymbianInternalPattern, "x-epoc/x-app0");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendUiDataUtils:CSendUiDataUtils
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSendUiDataUtils::CSendUiDataUtils( RFs& aFs ) : iFs( aFs )
    {
    }
// -----------------------------------------------------------------------------
// CSendUiDataUtils::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CSendUiDataUtils* CSendUiDataUtils::NewL( RFs& aFs )
    {
    CSendUiDataUtils* self = new(ELeave)CSendUiDataUtils( aFs );
    return self;
    }

// Destructor
CSendUiDataUtils::~CSendUiDataUtils()
    {
    if ( iApaLsSession )
        {
        iApaLsSession->Close();
        delete iApaLsSession;
        }
    }


// -----------------------------------------------------------------------------
// CSendUiDataUtils::ResolveFileMimeTypeL
// 
// (other items were commented in a header).
//
// Used by CSendUiImpl
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUiDataUtils::ResolveFileMimeTypeL(
    const TDesC& aFilePath,
    TDataType& aMimeType )
    {
    RFile file;

    TInt err = file.Open( iFs, aFilePath, EFileShareReadersOnly );

    if ( err )
        {
        User::LeaveIfError( file.Open( iFs, aFilePath, EFileShareAny ) );
        }

    CleanupClosePushL( file );

    ResolveFileMimeTypeL( file, aMimeType );

    CleanupStack::PopAndDestroy( &file );
    }

// -----------------------------------------------------------------------------
// CSendUiDataUtils::ResolveFileMimeTypeL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUiDataUtils::ResolveFileMimeTypeL(
    const RFile& aFile,
    TDataType& aMimeType )
    {
    if ( !iApaLsSession )
        {
        iApaLsSession = new(ELeave)RApaLsSession();
        User::LeaveIfError( iApaLsSession->Connect() );
        }

    TDataRecognitionResult dataType;

    TInt err = iApaLsSession->RecognizeData( aFile, dataType );
    if (    err
        ||  dataType.iDataType.Des8().Length() == 0
        ||  !dataType.iDataType.Des8().CompareF( KMmsSymbianInternalPattern ) )
        {
        // Not recognized
        aMimeType = KMsgMimeUnknown();
        }
    else
        {
        aMimeType = dataType.iDataType;
        }
    // The attachment file pointer needs to be reset to the start of the file.
    TInt pos(0);
    aFile.Seek( ESeekStart, pos );
    }

//  End of File
