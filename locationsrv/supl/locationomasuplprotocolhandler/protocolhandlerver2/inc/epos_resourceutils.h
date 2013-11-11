/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Resource utilities 
*
*/

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void FindResourceFileL( 
    RFs& aFsSession,
    const TDesC& aFilePathAndName,
    TFileName& aRscFile )
    {
    TParse parse;
    parse.Set( aFilePathAndName, NULL, NULL );
    
    TFindFile finder( aFsSession );
    if ( finder.FindByDir( parse.NameAndExt(), parse.Path() ) == KErrNone )
        {
        aRscFile.Copy( finder.File() );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    }
    
    
// -----------------------------------------------------------------------------
// COMASuplSession::ShowOfflineNoteL
//  
// -----------------------------------------------------------------------------
void FindLocalizedResourceFileL( 
    RFs& aFsSession, 
    const TDesC& aRscFile,
    TFileName& aFileName )
    {
    TFileName fileName( aRscFile );

    TParse parse;
    parse.Set( aRscFile, NULL, NULL );

    // look on c: drives first
    BaflUtils::NearestLanguageFile( aFsSession, fileName );
    
    const TInt KLangPartOfExtensionLen = 2;
    
    TBool found = ( fileName.Right( KLangPartOfExtensionLen ) != 
                    aRscFile.Right( KLangPartOfExtensionLen ) );
    
    if ( !found )
        {
        // look on other drives
        TDriveList drives;
        aFsSession.DriveList( drives );

        for ( TInt drive = EDriveZ; drive >= EDriveA; drive-- )
            {
            if ( drive != EDriveC ) // already checked
                {
                TDriveInfo drvInfo;
                aFsSession.Drive( drvInfo, drive );
                
                if ( drvInfo.iType != EMediaNotPresent )
                    {
                    TChar drv;
                    aFsSession.DriveToChar( drive, drv );

                    fileName.Zero();
                    fileName.Append( drv );
                    fileName.Append( KDriveDelimiter );
                    fileName.Append( parse.Path() );
                    fileName.Append( parse.NameAndExt() );

                    // look on c: and this drive
                    BaflUtils::NearestLanguageFile( aFsSession, fileName );

                    found = ( fileName.Right( KLangPartOfExtensionLen ) != 
                              aRscFile.Right( KLangPartOfExtensionLen ) );
                    if ( found ) break;
                    }
                }
            }
        }
        
    if ( found )
        {
        aFileName.Copy( fileName );
        }
    else
        {
        // localized file not found, try using SC
        FindResourceFileL( aFsSession, aRscFile, aFileName );
        }
    }

//////////////////////////////////////////////////////////////
HBufC* LoadResourceTextL( 
    RFs& aFsSession, 
    const TDesC& aRscFile, 
    TInt aResourceId )
    {
 
    TFileName fileName;
    FindLocalizedResourceFileL( aFsSession, aRscFile, fileName );

    CResourceFile* resFile = CResourceFile::NewL( aFsSession, fileName, 0, 0 );
    CleanupStack::PushL( resFile );
    resFile->ConfirmSignatureL( 0 );
    
    RResourceReader reader;
    reader.OpenL( resFile, aResourceId );
    CleanupClosePushL( reader );
    
    HBufC* text = reader.ReadHBufC16L();
    
    CleanupStack::PopAndDestroy( &reader );
    CleanupStack::PopAndDestroy( resFile );
    return text;  

    }
