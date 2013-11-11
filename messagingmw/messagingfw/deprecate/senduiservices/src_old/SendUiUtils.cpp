/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Provides private utilities for SendUi.
*
*/




// INCLUDE FILES
#include <apmstd.h>
#include <apgcli.h>
#include <caf/caf.h>
#include <MsgMimeTypes.h>

#include "SendUiUtils.h"

// CONSTANTS
_LIT8( KMmsSymbianInternalPattern, "x-epoc/x-app0");

const TInt KSenduiUrlMaxLen = 1024;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// SendUiUtils::OpenFileHandleL
// -----------------------------------------------------------------------------
//
RFile SendUiUtils::OpenFileHandleL( const TDesC& aFilePath, RFs& aFs )
    {
    RFile fileHandle;
    TInt err = fileHandle.Open( aFs, aFilePath, EFileShareReadersOnly );
    if ( err )
        {
        User::LeaveIfError( fileHandle.Open( aFs, aFilePath, EFileShareAny ) );
        }

    return fileHandle;
    }

// -----------------------------------------------------------------------------
// SendUiUtils::DrmContentUrlL
// -----------------------------------------------------------------------------
//
TBool SendUiUtils::HasContentUrlL( RFile& aFile )
    {
    TBuf<KSenduiUrlMaxLen> contentUrl;
    ContentAccess::CData* content = ContentAccess::CData::NewLC(
        aFile,
        ContentAccess::KDefaultContentObject,
        ContentAccess::EPeek );

    TInt error = content->GetStringAttribute( ContentAccess::EContentURI, contentUrl );
    CleanupStack::PopAndDestroy( content );
    
    return !error;
    }

// -----------------------------------------------------------------------------
// SendUiUtils::ResolveMimeTypeL
// -----------------------------------------------------------------------------
//
void SendUiUtils::ResolveMimeTypeL( const RFile& aFile, TDataType& aMimeType )
    {
    RApaLsSession lsSession;
    CleanupClosePushL( lsSession );
    User::LeaveIfError( lsSession.Connect() );

    TDataRecognitionResult dataType;

    TInt err = lsSession.RecognizeData( aFile, dataType );
    if ( err
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
    TInt pos( 0 );
    aFile.Seek( ESeekStart, pos );
    CleanupStack::PopAndDestroy( &lsSession );
    }

//  End of File  
