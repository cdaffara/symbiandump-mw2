/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Description
*
*/

// Version : %version: 6 %




// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <eikenv.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <mediarecognizer.h>
#include <streaminglinkmodel.h>
#include <devsoundif.h>

#include "playbackhelpertest.h"

#ifdef __WINSCW__
_LIT( KVideoTestPath, "c:\\data\\Videos\\" );
#else
_LIT( KVideoTestPath, "e:\\data\\Videos\\" );
#endif

// ============================ MEMBER FUNCTIONS ===================================================

// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::Delete
// Delete here all resources allocated and opened from test methods.
// Called from destructor.
// -------------------------------------------------------------------------------------------------
//
void CPlaybackHelperTest::Delete()
{
}

// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::RunMethodL( CStifItemParser& aItem )
{
    static TStifFunctionInfo const KFunctions[] =
    {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
        ENTRY( "IdentifyMediaTypeViaFileName", CPlaybackHelperTest::IdentifyMediaTypeViaFileName),
        ENTRY( "IdentifyMediaTypeViaFileHandle", CPlaybackHelperTest::IdentifyMediaTypeViaFileHandle),
        ENTRY( "MimeTypeViaFileName", CPlaybackHelperTest::MimeTypeViaFileName),
        ENTRY( "MimeTypeViaFileHandle", CPlaybackHelperTest::MimeTypeViaFileHandle),
        ENTRY( "IsValidStreamingPrefix", CPlaybackHelperTest::IsValidStreamingPrefix),
        ENTRY( "IsValidUrlPrefix1", CPlaybackHelperTest::IsValidUrlPrefix1),
        ENTRY( "IsValidUrlPrefix2", CPlaybackHelperTest::IsValidUrlPrefix2),
        ENTRY( "FreeFilehandleForRecognizer", CPlaybackHelperTest::FreeFilehandleForRecognizer),
        ENTRY( "OpenLinkFile1", CPlaybackHelperTest::OpenLinkFile1),
        ENTRY( "OpenLinkFile2", CPlaybackHelperTest::OpenLinkFile2),
        ENTRY( "GetNextLink", CPlaybackHelperTest::GetNextLink),
        ENTRY( "MaxLinkLength", CPlaybackHelperTest::MaxLinkLength ),
        ENTRY( "CreateNewLinkFile", CPlaybackHelperTest::CreateNewLinkFile),
        ENTRY( "MultiLinksCount",CPlaybackHelperTest::MultiLinksCount),
        ENTRY( "ResetLinkCount", CPlaybackHelperTest::ResetLinkCount),
        ENTRY( "FreeFilehandleForLink", CPlaybackHelperTest::FreeFilehandleForLink),
        ENTRY( "OpenAsxLinkViaFileName", CPlaybackHelperTest::OpenAsxLinkViaFileName),
        ENTRY( "OpenAsxLinkViaFileHandle", CPlaybackHelperTest::OpenAsxLinkViaFileHandle),
        ENTRY( "GetNumberOfVolumeSteps", CPlaybackHelperTest::GetNumberOfVolumeSteps),
	    ENTRY( "IsSeekable", CPlaybackHelperTest::IsSeekable)

    };
    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::IdentifyMediaTypeViaFileName
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::IdentifyMediaTypeViaFileName( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::IdentifyMediaTypeViaFileName"));

    TInt result = KErrNone;
    TInt err = KErrNone;

    TInt expectedMediaType;
    TInt mediaType = 0;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    if ( ( err = aItem.GetNextInt( expectedMediaType ) ) == KErrNone )
    {
        TPtrC filename;
    
        if ( ( err = aItem.GetNextString( filename ) ) == KErrNone )
        {
            TBool pathNeeded;

            if ( ( err = aItem.GetNextInt( pathNeeded ) ) == KErrNone )
            {
                TBuf<120> fullPath;

                if ( pathNeeded )
                {
                    fullPath.Append( KVideoTestPath );
                }

                fullPath.Append( filename );
            
                TRAP( result, mediaType = iMediaRecognizer->IdentifyMediaTypeL( fullPath ) );

                if ( result != KErrNone || expectedMediaType != mediaType )
                {
                    err = KErrGeneral;
                }
            }
        }
    }

    if ( err == KErrNone )
    {
        err = result;
    }

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::IdentifyMediaTypeViaFileHandle
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::IdentifyMediaTypeViaFileHandle( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::IdentifyMediaTypeViaFileHandle"));

    TInt result = KErrNone;
    TInt err = KErrNone;
    TInt expectedMediaType;
    TInt fileHandle32;
    TInt mediaType = 0;
    RFs fs;

    User::LeaveIfError( fs.Connect() );
    User::LeaveIfError( fs.ShareProtected() );

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    
    if ( ( err = aItem.GetNextInt( fileHandle32 ) ) == KErrNone )
    {   
    
#ifndef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
        //
        // set RFile as default if the 64-bit flag is not defined
        //
        fileHandle32 = ETrue;
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    
        if ( ( err = aItem.GetNextInt( expectedMediaType ) ) == KErrNone )
        {   
            TPtrC filename;
                    
            if ( ( err = aItem.GetNextString( filename ) ) == KErrNone )
            {
                TBuf<120> fullPath;
    
                fullPath.Append( KVideoTestPath );
                fullPath.Append( filename );
                                           
                if ( fileHandle32 )
                {
                    //
                    // using 32-bit file handle
                    //
                    RFile file;
                    err = file.Open( fs,
                                     fullPath,
                                     EFileShareAny | EFileStream | EFileRead );
                
                    if ( err == KErrNone )
                    {
                        TRAP( result, mediaType = iMediaRecognizer->IdentifyMediaTypeL( fullPath, file ) );
                    }
                    
                    file.Close();    
                }
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
                else
                {
                    //
                    // using 64-bit file handle
                    //
                    RFile64 file64;
                    err = file64.Open( fs,
                                       fullPath,
                                       EFileShareAny | EFileStream | EFileRead );
    
                    if ( err == KErrNone )
                    {
                        TRAP( result, mediaType = iMediaRecognizer->IdentifyMediaTypeL( fullPath, file64 ) );                        
                    }
                    
                    file64.Close();
                }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
                
                if ( result != KErrNone || expectedMediaType != mediaType )
                {
                    err = KErrGeneral;
                }                                        
            }        
        }
    }

    if ( err == KErrNone )
    {
        err = result;
    }

    fs.Close();

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::MimeTypeViaFileName
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::MimeTypeViaFileName( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::MimeTypeViaFileName"));

    TInt result = KErrNone;
    TInt err = KErrNone;
    TPtrC string;
    TPtrC expectedMimetype;
    TBuf<20> mimeType;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    if ( ( err = aItem.GetNextString( expectedMimetype ) ) == KErrNone )
    {
        TPtrC filename;

        if ( ( err = aItem.GetNextString( filename ) ) == KErrNone )
        {
            TBuf<120> fullPath;

            fullPath.Append( KVideoTestPath );
            fullPath.Append( filename );

            TRAP( result, mimeType = iMediaRecognizer->MimeTypeL( fullPath ) );

            if ( result != KErrNone || mimeType.CompareF( expectedMimetype ) )
            {
                err = KErrGeneral;
            }
        }
    }

    if ( err == KErrNone )
    {
        err = result;
    }

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::MimeTypeViaFileHandle
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::MimeTypeViaFileHandle( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::MimeTypeViaFileHandle"));

    TInt result = KErrNone;
    TInt err = KErrNone;
    TPtrC fileHandleType, expectedMimetype;
    TInt fileHandle32;
    TBuf<20> mimeType;
    
    RFs fs;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    User::LeaveIfError( fs.Connect() );
    User::LeaveIfError( fs.ShareProtected() );

    if ( ( err = aItem.GetNextInt( fileHandle32 ) ) == KErrNone )
    {  
    
#ifndef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
        //
        // set RFile as default if the 64-bit flag is not defined
        //
        fileHandle32 = ETrue;
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    
        if ( ( err = aItem.GetNextString( expectedMimetype ) ) == KErrNone )
        {        
            TPtrC filename;
            
            if ( ( err = aItem.GetNextString( filename ) ) == KErrNone )
            {
                TBuf<120> fullPath;
    
                fullPath.Append( KVideoTestPath );
                fullPath.Append( filename );
    
                if ( fileHandle32 )
                {
                    //
                    // using 32-bit file handle
                    //
                    RFile file;
                    err = file.Open( fs,
                                     fullPath,
                                     EFileShareAny | EFileStream | EFileRead );
        
                    if ( err == KErrNone )
                    {
                        TRAP( result, mimeType = iMediaRecognizer->MimeTypeL( file ) );
                    }
                    
                    file.Close();     
                }
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
                else
                {
                    //
                    // using 64-bit file handle
                    //
                    RFile64 file64;
                    err = file64.Open( fs,
                                       fullPath,
                                       EFileShareAny | EFileStream | EFileRead );
    
                    if ( err == KErrNone )
                    {
                        TRAP( result, mimeType = iMediaRecognizer->MimeTypeL( file64 ) );  
                    }
                    
                    file64.Close();
                }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
                
                if ( result != KErrNone || mimeType.CompareF( expectedMimetype ) )
                {
                    err = KErrGeneral;
                }                   
            }
        }
    }
    
    if ( err == KErrNone )
    {
        err = result;
    }

    fs.Close();

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::IsValidStreamingPrefix
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::IsValidStreamingPrefix( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::IsValidStreamingPrefix"));

    TInt err = KErrNone;
    TBool valid = EFalse;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    if ( ( err = aItem.GetNextInt( valid ) ) == KErrNone )
    {
        TPtrC string;

        if ( ( err = aItem.GetNextString( string ) ) == KErrNone )
        {
            if ( iMediaRecognizer->IsValidStreamingPrefix( string ) != valid )
            {
                err = KErrGeneral;
            }
        }
    }
    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::IsValidUrlPrefix1
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::IsValidUrlPrefix1( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::IsValidUrlPrefix1"));

    TInt err = KErrNone;
    TBool valid = EFalse;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    if ( ( err = aItem.GetNextInt( valid ) ) == KErrNone )
    {
        TPtrC string;
        
        if ( ( err = aItem.GetNextString( string ) ) == KErrNone )
        {
            if ( iMediaRecognizer->IsValidUrlPrefix( string ) != valid )
            {
                err = KErrGeneral;
            }
        }
    }
    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::IsValidUrlPrefix2
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::IsValidUrlPrefix2( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::IsValidUrlPrefix2"));

    TInt err = KErrNone;
    TBool valid = EFalse;
    TPtrC16 stringDes16;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    if ( ( err = aItem.GetNextInt( valid ) ) == KErrNone )
    {
        if ( ( err = aItem.GetNextString( stringDes16 ) ) == KErrNone )
        {
            HBufC8* string8 = NULL;

            TRAP( err, string8 = HBufC8::NewL( stringDes16.Length() ) );

            if ( err == KErrNone )
            {
                TPtr8 stringDes8( string8->Des() );
                stringDes8.Copy( stringDes16 );

                if ( iMediaRecognizer->IsValidUrlPrefix( stringDes8 ) != valid )
                {
                    err = KErrGeneral;
                }
            }

            delete string8;
        }
    }
    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::FreeFilehandleForRecognizer
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::FreeFilehandleForRecognizer()
{
    iLog->Log(_L("CPlaybackHelperTest::FreeFilehandleForRecognizer"));

    TInt err = KErrNone;

    iMediaRecognizer->FreeFilehandle();

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::OpenLinkFile1
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::OpenLinkFile1( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::OpenLinkFile1"));

    TInt err = KErrNone;
    TInt result = KErrNone;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    TPtrC filename;
    
    if ( ( err = aItem.GetNextString( filename ) ) == KErrNone )
    {
        TBuf<120> fullPath;

        fullPath.Append( KVideoTestPath );
        fullPath.Append( filename );

        TRAP( err, result = iStreamingLinkModel->OpenLinkFileL( fullPath ) );
    }

    if ( err == KErrNone )
    {
        err = result;
    }

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::OpenLinkFile2
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::OpenLinkFile2( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::OpenLinkFile2"));

    TInt err = KErrNone;
    TInt result = KErrNone;
    RFile file;
    RFs fs;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    TRAP( err, fs.Connect() );

    TPtrC filename;
    
    if ( err == KErrNone && ( err = aItem.GetNextString( filename ) ) == KErrNone )
    {
        TBuf<120> fullPath;

        fullPath.Append( KVideoTestPath );
        fullPath.Append( filename );

        err = file.Open( fs,
                         fullPath,
                         EFileShareAny | EFileStream | EFileRead );

        if ( err == KErrNone )
        {
            TRAP( err, result = iStreamingLinkModel->OpenLinkFileL( file ) );
        }
    }

    if ( err == KErrNone )
    {
        err = result;
    }

    file.Close();
    fs.Close();

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::GetNextLink
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::GetNextLink( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::GetNextLink"));

    TInt err = KErrNone;
    TInt result = KErrNone;

    TBool local = ETrue;

    HBufC* nextLink = HBufC::NewLC( 100 );
    TPtr nextLinkDes = nextLink->Des();

    HBufC* string = HBufC::NewLC( 100 );
    TPtr stringDes = string->Des();

    TPtrC stringC;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    if ( ( err = aItem.GetNextInt( local ) ) == KErrNone )
    {
        TRAP( err, result = iStreamingLinkModel->GetNextLinkL( nextLinkDes, local ) );

        if ( err == KErrNone &&
             result == KErrNone &&
             ( err = aItem.GetNextString( stringC ) ) == KErrNone )
        {
            nextLinkDes.Copy(stringC);

            if ( !stringDes.CompareF( nextLinkDes ) )
            {
                err = KErrNotFound;
            }
        }
        else if ( result != KErrNone )
        {
            err = result;
        }
    }

    CleanupStack::PopAndDestroy(2); //string, nextLink
    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::MaxLinkLength
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::MaxLinkLength( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::MaxLinkLength"));

    TInt err = KErrNone;
    TInt length = 0;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    if ( ( err = aItem.GetNextInt( length ) ) == KErrNone )
    {
        if ( iStreamingLinkModel->MaxLinkLength() != length )
        {
            err = KErrGeneral;
        }
    }

    return err;
}

//
// ---------------------------------------------------------------------------
// CPlaybackHelperTest::MultiLinksCount
// ---------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::MultiLinksCount( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::MultiLinksCount"));

    TInt err = KErrNone;
    TInt cnt = 0;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    if ( ( err = aItem.GetNextInt( cnt ) ) == KErrNone )
    {
        if ( iStreamingLinkModel->MultiLinksCount() != cnt )
        {
            err = KErrGeneral;
        }
    }
    return err;
}

// ---------------------------------------------------------------------------
// CPlaybackHelperTest::CreateNewLinkFile
// ---------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::CreateNewLinkFile( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::CreateNewLinkFileL"));

    TInt err = KErrNone;
    TInt result = KErrNone;

    TPtrC filename;
    TPtrC string;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    if ( ( err = aItem.GetNextString( filename ) ) == KErrNone )
    {
        TBuf<120> fullPath;

        fullPath.Append( KVideoTestPath );
        fullPath.Append( filename );

        CDesCArrayFlat* linkArray = new (ELeave) CDesCArrayFlat(10);

        while ( ( err = aItem.GetNextString( string ) ) == KErrNone )
        {
            linkArray->AppendL( string );
        }

        TRAP ( err,
               result = iStreamingLinkModel->CreateNewLinkFileL( fullPath, linkArray, ETrue ) );

        delete linkArray;
    }

    if ( err == KErrNone )
    {
        err = result;
    }

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::ResetLinkCount
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::ResetLinkCount()
{
    iLog->Log(_L("CPlaybackHelperTest::ResetLinkCount"));

    TInt err = KErrNone;

    iStreamingLinkModel->ResetLinkCount();

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::FreeFilehandleForLink
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::FreeFilehandleForLink()
{
    iLog->Log(_L("CPlaybackHelperTest::FreeFilehandleForLink"));

    TInt err = KErrNone;

    iStreamingLinkModel->FreeFilehandle();

    return err;
}

// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::OpenAsxLinkViaFileName
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::OpenAsxLinkViaFileName( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::OpenAsxLinkViaFileName"));

    TInt err = KErrNone;
    TInt result = KErrNone;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    TPtrC filename;
    
    if ( ( err = aItem.GetNextString( filename ) ) == KErrNone )
    {
        TBuf<120> fullPath;

        fullPath.Append( KVideoTestPath );
        fullPath.Append( filename );

        TRAP( err, result = iStreamingLinkModel->OpenAsxFileL( fullPath ) );
    }

    if ( err == KErrNone )
    {
        err = result;
    }

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::OpenAsxLinkViaFileHandle
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::OpenAsxLinkViaFileHandle( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::OpenAsxLinkViaFileHandle"));

    TInt err = KErrNone;
    TInt result = KErrNone;
    RFile file;
    RFs fs;

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    TRAP( err, fs.Connect() );

    TPtrC filename;
    
    if ( err == KErrNone && ( err = aItem.GetNextString( filename ) ) == KErrNone )
    {
        TBuf<120> fullPath;

        fullPath.Append( KVideoTestPath );
        fullPath.Append( filename );

        err = file.Open( fs,
                         fullPath,
                         EFileShareAny | EFileStream | EFileRead );

        if ( err == KErrNone )
        {
            TRAP( err, result = iStreamingLinkModel->OpenAsxFileL( file ) );
        }
    }

    if ( err == KErrNone )
    {
        err = result;
    }

    file.Close();
    fs.Close();

    return err;
}

//
// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::GetNumberOfVolumeSteps
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::GetNumberOfVolumeSteps()
{
    iLog->Log(_L("CPlaybackHelperTest::GetNumberOfVolumeSteps"));

	TInt result = KErrNone;

	if ( iDevSoundIf->GetNumberOfVolumeSteps() < 10 ) 
	{
        result = KErrArgument;
	}

    return result;
}

// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::IsSeekable
// -------------------------------------------------------------------------------------------------
//
TInt CPlaybackHelperTest::IsSeekable( CStifItemParser& aItem )
{
    iLog->Log(_L("CPlaybackHelperTest::IsSeekable"));

    TInt error = KErrNone;
    TBool seekable;
    
    aItem.GetNextInt( seekable );
    
    if ( seekable != iStreamingLinkModel->IsSeekable())
    {
        error = KErrArgument;
    }

    return error;
}
//  [End of File] - Do not remove
