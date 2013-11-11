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
* Description:  Implementation of CStreamingLinkModel
*
*/

// Version : %version: 4.1.1 %



// INCLUDE FILES
#include <utf.h>
#include <e32std.h>
#include <bautils.h>
#include <sysutil.h>
#include <pathinfo.h>
#include <mediarecognizer.h>
#include <streaminglinkmodel.h>

#ifdef RD_MULTIPLE_DRIVE
#include <driveinfo.h>
#endif  //RD_MULTIPLE_DRIVE

#include "playbackhelper_log.h"

// CONSTANTS
const TInt KLocalFileIdLength(5);
const TInt KMaxLinkFileSize(5120); //5kB
const TInt KBothSlashes(2);
#ifndef RD_MULTIPLE_DRIVE
const TInt KDriveLetter(2);
#endif  //RD_MULTIPLE_DRIVE

_LIT( KLocalFileId, "file:");
_LIT( KSlash, "/" );
_LIT( KDoubleSlash, "//" );
_LIT( KBackSlash, "\\" );

// ============================ MEMBER FUNCTIONS ===================================================

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::CStreamingLinkModel
// C++ default constructor can NOT contain any code, that
// might leave.
// -------------------------------------------------------------------------------------------------
//
CStreamingLinkModel::CStreamingLinkModel()
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::CStreamingLinkModel()"));
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::ConstructL
// Symbian 2nd phase constructor can leave.
// -------------------------------------------------------------------------------------------------
//
void CStreamingLinkModel::ConstructL()
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::ConstructL()"));

    User::LeaveIfError( iFs.Connect() );
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::NewL
// Two-phased constructor.
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CStreamingLinkModel* CStreamingLinkModel::NewL()
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::NewL()"));

    CStreamingLinkModel* self = new( ELeave ) CStreamingLinkModel();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::~CStreamingLinkModel
// Destructor
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CStreamingLinkModel::~CStreamingLinkModel()
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::~CStreamingLinkModel()"));

    for ( TInt i = 0 ; i < iLinkArray.Count() ; i++ )
    {
        delete iLinkArray[i]->link;
    }

    iLinkArray.ResetAndDestroy();
        
    iFileHandle.Close();
    iFs.Close();
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::OpenLinkFileL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TInt CStreamingLinkModel::OpenLinkFileL( const TDesC& aLinkFileName,
                                                  TBool aEnableFiltering )
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::OpenLinkFileL()"));

    //
    // exit if file is not found
    //
    if ( !BaflUtils::FileExists( iFs, aLinkFileName ) )
    {
        User::Leave( KErrNotFound );
    }

    //
    // open via file name
    //
    RFile ramFile;
    TInt err = ramFile.Open( iFs, aLinkFileName, EFileRead | EFileShareReadersOnly );

    if( err )
    {
        err = ramFile.Open( iFs, aLinkFileName, EFileRead | EFileShareAny );
    }

    CleanupClosePushL( ramFile );

    //
    // populate the links
    //
    TInt result = PopulateRamLinksL( ramFile, aEnableFiltering );

    CleanupStack::PopAndDestroy(); //ramFile

    return result;
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::OpenLinkFileL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TInt CStreamingLinkModel::OpenLinkFileL( RFile& aFile,
                                                  TBool aEnableFiltering )
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::OpenLinkFileL()"));

    RFile ramFile;

    if ( FileHandleExists(aFile) )
    {
        iFileHandle.Close();
        User::LeaveIfError( iFileHandle.Duplicate( aFile ) );
        User::LeaveIfError( ramFile.Duplicate( iFileHandle ) );
        CleanupClosePushL( ramFile );
    }
    else
    {
        User::Leave( KErrNotFound );
    }

    //
    // populate the links
    //
    TInt result = PopulateRamLinksL( ramFile, aEnableFiltering );

    CleanupStack::PopAndDestroy(); //ramFile

    return result;
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::MaxLinkLength
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TInt CStreamingLinkModel::MaxLinkLength()
{
    TInt length = 0;

    for ( TInt index = 0; index < iLinkArray.Count(); index++)
    {
        length = length + iLinkArray[index]->link->Length();
    }

    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::MaxLinkLength() max length = [%d] "), length );

    return length;
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::GetNextLinkL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TInt CStreamingLinkModel::GetNextLinkL( TDes& aLink, TBool& aLocalFile, TBool aParse )
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::GetNextLinkL()") );

    aLocalFile = EFalse;
    TInt ret = KErrNone;

    if ( iLinkCount < iLinkArray.Count() )
    {
        aLink = iLinkArray[iLinkCount]->link->Des();
        iLinkCount++;

        // local file --> parse it.
        if ( aParse && !aLink.Left( KLocalFileIdLength ).CompareF( KLocalFileId ) )
        {
            // remove "file:"
            aLocalFile = ETrue;
            aLink = aLink.Mid( KLocalFileIdLength );

            // remove extra '/' from begin
            while ( !aLink.Left( 1 ).CompareF( KSlash ) )
            {
                aLink = aLink.Mid( 1 );
            }

            // change "//" and "/" inside body to "\"
            TInt index = aLink.FindF( KDoubleSlash );
            while ( index != KErrNotFound )
            {
                aLink.Replace( index, KBothSlashes, KBackSlash );
                index = aLink.FindF( KDoubleSlash );
            }

            index = aLink.FindF( KSlash );

            while ( index != KErrNotFound )
            {
                aLink.Replace( index, 1, KBackSlash );
                index = aLink.FindF( KSlash );
            }
        }
    }
    else
    {
        ret = KErrNotFound;
    }

    return ret;
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::IsSeekable
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TBool CStreamingLinkModel::IsSeekable()
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::IsSeekable()") );

    TBool ret = ETrue;

    // link count has been incremented in the GetNextlink
    if ( iLinkCount <= iLinkArray.Count() )
    {
        ret = iLinkArray[iLinkCount-1]->seek;
    }
    
    return ( ret );
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::ReadNextLine
// -------------------------------------------------------------------------------------------------
//
TInt CStreamingLinkModel::ReadNextLine( TDes& aLine )
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::ReadNextLine()"));

    TInt ret = KErrNone;
    TInt lineChange = iBufferPtr.LocateF( EKeyLineFeed );

    if ( lineChange == KErrNotFound )
    {
        // No line change was found --> last line had no line change
        // Copy last line to (unicode) aLink
        CnvUtfConverter::ConvertToUnicodeFromUtf8( aLine, iBufferPtr );
        ret = KErrEof;
    }
    else
    {
        //Found line change
        TInt length = lineChange;
        if ( ( length > 0 ) && ( iBufferPtr[length-1] == EKeyEnter ) )
        {
            length--;
        }

        // Copy line to (unicode) aLink
        CnvUtfConverter::ConvertToUnicodeFromUtf8( aLine, iBufferPtr.Left( length ) );

        // Move past the line feed
        iBufferPtr.Set( iBufferPtr.Mid( ++lineChange ) );

        if ( !iBufferPtr.Length() )
        {
            // File end reached.
            ret = KErrEof;
        }
    }

    return ret;
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::CreateNewLinkFileL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TInt CStreamingLinkModel::CreateNewLinkFileL( const TDesC& aNewLinkFileName,
                                                       MDesCArray* aLinkArray,
                                                       TBool aOverWrite )
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::CreateNewLinkFileL()"));

    TInt result = KErrNone;

    // check if file exists already
    if ( !aOverWrite && BaflUtils::FileExists( iFs, aNewLinkFileName ) )
    {
        result = KErrAlreadyExists;
    }
    else
    {
        //check disk space
        TInt bytesRequired = 0;
        TInt index = 0;

        for ( index = 0; index < aLinkArray->MdcaCount(); index++ )
        {
            bytesRequired = bytesRequired + aLinkArray->MdcaPoint( index ).Size();
        }

        TParsePtrC parse(aNewLinkFileName);

#ifdef RD_MULTIPLE_DRIVE
        DriveInfo::TDriveArray driveArray;
        TInt drive = 0;

        RFs::CharToDrive( parse.Drive()[0], drive );

        //check disk space
        if( SysUtil::DiskSpaceBelowCriticalLevelL( &iFs, bytesRequired, drive ) )
        {
            result = KErrDiskFull;
        }
#else  //RD_MULTIPLE_DRIVE
        if( parse.Drive().CompareF( PathInfo::MemoryCardRootPath().Left( KDriveLetter ) ) == 0 )
        {
            if ( SysUtil::MMCSpaceBelowCriticalLevelL( &iFs, bytesRequired ) )
            {
                result = KErrDiskFull;
            }
        }
        else if ( SysUtil::FFSSpaceBelowCriticalLevelL( &iFs, bytesRequired ) )
        {
            User::Leave( KErrDiskFull );
        }
#endif  //RD_MULTIPLE_DRIVE

        if ( result == KErrNone )
        {
            // save
            RFile file;
            User::LeaveIfError( file.Replace( iFs, aNewLinkFileName, EFileWrite ) );
            CleanupClosePushL( file );

            for ( index = 0; result == KErrNone && index < aLinkArray->MdcaCount(); index++ )
            {
                // Convert to ascii. Some special marks like '? might take 1-3 letters.
                HBufC8* link = HBufC8::NewLC( aLinkArray->MdcaPoint( index ).Length() * 3 + 1 );
                TPtr8 ptr = link->Des();
                CnvUtfConverter::ConvertFromUnicodeToUtf8( ptr, aLinkArray->MdcaPoint( index ) );
                ptr.Append( EKeyLineFeed );

                // write to file
                result = file.Write( ptr );

                CleanupStack::PopAndDestroy( link );
            }

            file.Flush();
            CleanupStack::PopAndDestroy();  // file

        }
    }

    return result;
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::FreeFilehandle
// -------------------------------------------------------------------------------------------------
//
EXPORT_C void CStreamingLinkModel::FreeFilehandle()
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::FreeFilehandle()"));

    iFileHandle.Close();
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::FileHandleExists
// -------------------------------------------------------------------------------------------------
//
TBool CStreamingLinkModel::FileHandleExists( RFile& aFile )
{
    TInt size = 0;
    TInt err = KErrNone;
    TBool exist = EFalse;

    if ( aFile.SubSessionHandle() )
    {
        err = aFile.Size( size );
    }

    if ( !err && size )
    {
        exist = ETrue;
    }

    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::FileHandleExists() exist = [%d]"), exist);

    return exist;
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::MaxLinkLength
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TInt CStreamingLinkModel::MultiLinksCount()
{
    TInt cnt = iLinkArray.Count();

    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::MultiLinksCount() count = [%d]"), cnt);

    return cnt;
}

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::ResetLinkCount
// -------------------------------------------------------------------------------------------------
//
EXPORT_C void CStreamingLinkModel::ResetLinkCount()
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::ResetLinkCount()"));

    iLinkCount = 0;
}

// -----------------------------------------------------------------------------
// CStreamingLinkModel::OpenAsxFileL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CStreamingLinkModel::OpenAsxFileL( const TDesC& aLinkFileName,
                                                 TBool aEnableFiltering )
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::OpenAsxFileL(%S,%d)"),
                                        &aLinkFileName,aEnableFiltering);
    TInt result = KErrNone;

#ifdef __WINDOWS_MEDIA

    CAsxParser* asxParser = NULL;
    TRAPD( err,
    {
        asxParser = CAsxParser::NewL( aLinkFileName );
    } ); //TRAPD

    if ( err )
    {
        result = KErrNotSupported;
    }
    else
    {
        CleanupStack::PushL( asxParser );
        result = PopulateAsxLinksL( asxParser, aEnableFiltering );
        CleanupStack::PopAndDestroy();  // asxParser
    }


#else   // __WINDOWS_MEDIA

    result = KErrNotSupported;

#endif  // __WINDOWS_MEDIA

    return result;
}

// -----------------------------------------------------------------------------
// CStreamingLinkModel::OpenAsxFileL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CStreamingLinkModel::OpenAsxFileL( RFile& aFile,
                                                 TBool aEnableFiltering )
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::OpenAsxFileL(aFile,%d)"),aEnableFiltering);

    TInt result = KErrNone;

#ifdef __WINDOWS_MEDIA

    CAsxParser* asxParser = NULL;
    TRAPD( err,
    {
        asxParser = CAsxParser::NewL( aFile );
    } ); //TRAPD

    if ( err )
    {
        result = KErrNotSupported;
    }
    else
    {
        CleanupStack::PushL( asxParser );
        result = PopulateAsxLinksL( asxParser, aEnableFiltering );
        CleanupStack::PopAndDestroy();  // asxParser
    }


#else   // __WINDOWS_MEDIA

    result = KErrNotSupported;

#endif  // __WINDOWS_MEDIA

    return result;
}

// -----------------------------------------------------------------------------
// CStreamingLinkModel::PopulateRamLinksL
// -----------------------------------------------------------------------------
//
TInt CStreamingLinkModel::PopulateRamLinksL( RFile aRamFile,
                                             TBool aEnableFiltering )
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::PopulateRamLinksL()"));

    for ( TInt i = 0 ; i < iLinkArray.Count() ; i++ )
    {
        delete iLinkArray[i]->link;
    }

    iLinkArray.ResetAndDestroy();
    iLinkCount = 0;

    TInt size = 0;
    TInt result = KErrNone;
    
    TBool linkadded = EFalse;

    if ( aRamFile.Size( size ) == KErrNone && size <= KMaxLinkFileSize )
    {
        HBufC8* buffer = HBufC8::NewLC( size );
        TPtr8 ptr = buffer->Des();
        TInt ret = KErrNone;

        // read file to buffer
        User::LeaveIfError( aRamFile.Read( ptr ) );
        iBufferPtr.Set( ptr );

        CMediaRecognizer* recognizer = CMediaRecognizer::NewL();
        CleanupStack::PushL( recognizer );

        // Get links from buffer
        while ( ret == KErrNone )
        {
            linkadded = EFalse;
        
            // Create a linkitem ptr
            LinkStruct* linkItem = new( ELeave ) LinkStruct;   
            CleanupStack::PushL( linkItem );

            linkItem->seek = ETrue;

            linkItem->link = HBufC::NewLC( buffer->Length() );
            TPtr ptr2 = linkItem->link->Des();
            ret = ReadNextLine( ptr2 );

            if ( aEnableFiltering )
            {
                // check if line is link or not
                if ( recognizer->IsValidUrlPrefix( ptr2 ) )
                {
                    ptr2.TrimRight();
                    iLinkArray.Append( linkItem );
                    linkadded = ETrue;
                }
            }
            else
            {
                iLinkArray.Append( linkItem );
                linkadded = ETrue;
            }
            
            if ( linkadded )
            {
                CleanupStack::Pop(2); // pop the linkItem and the HbufC created for linkItem->link               
            }                              
            else 
            {
                CleanupStack::PopAndDestroy(2); //pop and destroy linkItem and the HbufC created for linkItem->link
            }
        }

        CleanupStack::PopAndDestroy(2);  // buffer, recognizer

        if ( iLinkArray.Count() == 0 )
        {
            result = KErrNotFound;
        }
    }
    else
    {
        result = KErrNotSupported;
    }

    return result;
}

// -----------------------------------------------------------------------------
// CStreamingLinkModel::PopulateAsxLinksL
// -----------------------------------------------------------------------------
//
TInt CStreamingLinkModel::PopulateAsxLinksL( CAsxParser* aAsxParser,
                                             TBool aEnableFiltering )
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::PopulateAsxLinksL()"));

    for ( TInt i = 0 ; i < iLinkArray.Count() ; i++ )
    {
        delete iLinkArray[i]->link;
    }

    iLinkArray.ResetAndDestroy();
    iLinkCount = 0;

    TInt ret = KErrNone;
    
    TBool linkadded = EFalse;

    if ( aAsxParser )
    {
        TUint urlCount = 0;
        AsxStruct* asxItem;

        aAsxParser->GetUrlCount( urlCount );
        if ( urlCount )
        {
            CMediaRecognizer* recognizer = CMediaRecognizer::NewL();
            CleanupStack::PushL( recognizer );

            for (TInt i=1; i <= urlCount; i++)
            {

                linkadded = EFalse;
                
                // Get the asx struct from the parser
                asxItem = aAsxParser->GetUrl(i);
                // Set the url to the bufferptr
                iBufferPtr.Set(asxItem->url->Des());
                
                // Create a linkitem ptr
                LinkStruct* linkItem = new( ELeave ) LinkStruct;  
                // push onto the cleanup stack
                CleanupStack::PushL( linkItem );

                // Allocate heap mem for the link
                linkItem->link = HBufC::NewLC( asxItem->url->Length() );
                //Get the ptr to the link
                TPtr ptr2 = linkItem->link->Des();
                // read the asx url into the link ptr
                ReadNextLine( ptr2 );
                
                // Get the additional attibutes from the asx item
                linkItem->seek = asxItem->seek;
                
                if ( aEnableFiltering )
                {
                    // check if line is link or not
                    if ( recognizer->IsValidUrlPrefix( ptr2 ) )
                    {
                        ptr2.TrimRight();
                        iLinkArray.Append( linkItem );
                        linkadded = ETrue;
                    }
                }
                else
                {
                    iLinkArray.Append( linkItem );
                    linkadded = ETrue;
                }
                                              
                if ( linkadded )
                {
                    CleanupStack::Pop(2); // pop the linkItem and the HbufC created for linkItem->link                    
                }                              
                else 
                {
                    CleanupStack::PopAndDestroy(2); //pop and destroy linkItem and the HbufC created for linkItem->link
                }
            }

            CleanupStack::PopAndDestroy();  //recognizer
        }
        else
        {
            ret = KErrNotFound;
        }

        if ( iLinkArray.Count() == 0 )
        {
            ret = KErrNotFound;
        }
    }
    else
    {
        ret = KErrNotSupported;
    }

    return ret;
 }

// -------------------------------------------------------------------------------------------------
// CStreamingLinkModel::AreAllLinksLocal
// returns true if all the links are local
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TBool CStreamingLinkModel::AreAllLinksLocal()
{
    PLAYBACKHELPER_DEBUG(_L("CStreamingLinkModel::AreAllLinksLocal()") );

    TBool ret = ETrue;

    if ( iLinkArray.Count() )
    {    
        for ( TInt linkCount = 0 ; linkCount < iLinkArray.Count() ; linkCount++ )
        {            
            TPtr aLink = ( iLinkArray[linkCount]->link )->Des();
    
            if ( aLink.Left( KLocalFileIdLength ).CompareF( KLocalFileId ) )
            {
                ret = EFalse;
                break;
            }
        }
    }

    return ret;
}

//  End of File
