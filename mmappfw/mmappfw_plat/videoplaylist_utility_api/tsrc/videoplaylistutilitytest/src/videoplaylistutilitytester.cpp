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
* Description:  videoplaylistutility test class for STIF testing
*
*/

// Version : %version: 2 %

#include <f32file.h>
#include <mediarecognizer.h>
#include <mpxmediaarray.h>
#include <mpxmediacontainerdefs.h>
#include <mpxmediageneralextdefs.h>

#include "videoplaylistutilitytester.h"
#include "videoplaylistutility.h"


// -------------------------------------------------------------------------------------------------
//   CVideoPlaylistUtility_Tester::NewL
// -------------------------------------------------------------------------------------------------
//
CVideoPlaylistUtility_Tester* CVideoPlaylistUtility_Tester::NewL()
{
    CVideoPlaylistUtility_Tester* self = new (ELeave) CVideoPlaylistUtility_Tester;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
// CVideoPlaylistUtility_Tester::~CVideoPlaylistUtility_Tester
//
// -----------------------------------------------------------------------------
//
CVideoPlaylistUtility_Tester::~CVideoPlaylistUtility_Tester()
{
    delete iPlaylistUtility;
    iPlaylistUtility = NULL;

}

// -------------------------------------------------------------------------------------------------
//   CVideoPlaylistUtility_Tester::ConstructL
// -------------------------------------------------------------------------------------------------
//
void CVideoPlaylistUtility_Tester::ConstructL()
{
}

// -----------------------------------------------------------------------------
// CVideoPlaylistUtility_Tester::CVideoPlaylistUtility_Tester
//
// -----------------------------------------------------------------------------
//
CVideoPlaylistUtility_Tester::CVideoPlaylistUtility_Tester()
{
}

// -------------------------------------------------------------------------------------------------
//   CVideoPlaylistUtility_Tester::CreateEngineL
// -------------------------------------------------------------------------------------------------
//
TInt CVideoPlaylistUtility_Tester::CreatePlaylistUtilityL( CStifItemParser& /*aItem*/ )
{
    iPlaylistUtility = CVideoPlaylistUtility::NewL();

    return KErrNone;
}


// -------------------------------------------------------------------------------------------------
//   CVideoPlaylistUtility_Tester::GetFileInfoL
// -------------------------------------------------------------------------------------------------
//
TInt CVideoPlaylistUtility_Tester::GetFileInfoL( CStifItemParser& aItem )
{
    CMediaRecognizer::TMediaType mediaType = CMediaRecognizer::ELocalRamFile;
    TInt error = KErrNone;

    TBool useFileHandle;
    TInt  media;
    TBool expectedSingleLink;
    TBool expectedLocalFile;
    TBool singleLink;
    TBool localFile;

    User::LeaveIfError( aItem.GetNextInt( useFileHandle ) );

    TFileName filename = GetFileNameAndPathL( aItem );

    User::LeaveIfError( aItem.GetNextInt( media ) );


    // Get the mediaType
    switch ( media )
    {
        case ERamFile:
        {
            mediaType = CMediaRecognizer::ELocalRamFile;
            break;
        }
        case EAsxFile:
        {
            mediaType = CMediaRecognizer::ELocalAsxFile;
            break;
        }
    }

    if ( useFileHandle )
    {
        RFs fs;
        RFile file;

        User::LeaveIfError( fs.Connect() );

        fs.ShareProtected();

        User::LeaveIfError( file.Open( fs, filename, EFileRead ) );

        iPlaylistUtility->GetFileInfoL( file, mediaType, singleLink, localFile );

        file.Close();
        fs.Close();
    }
    else
    {
        iPlaylistUtility->GetFileInfoL( filename, mediaType, singleLink, localFile );
    }

    // Read the expected result
    User::LeaveIfError( aItem.GetNextInt( expectedSingleLink ) );
    User::LeaveIfError( aItem.GetNextInt( expectedLocalFile ) );

    if ( expectedSingleLink != singleLink || expectedLocalFile != localFile )
    {
        error = KErrGeneral;
    }

    return error;
}


// -------------------------------------------------------------------------------------------------
//   CVideoPlaylistUtility_Tester::GetLinkL
// -------------------------------------------------------------------------------------------------
//
TInt CVideoPlaylistUtility_Tester::GetLinkL( CStifItemParser& aItem )
{
    TInt error = KErrNone;

    _LIT( KRamStreamingLink, "rtsp://stream.3gp" );
    _LIT( KAsxStreamingLink, "http://10.48.2.56:443/stream.wmv" );

#ifdef __WINSCW__
    _LIT( KRamLocalLink, "c:\\data\\videos\\local.3gp" );
#else
    _LIT( KRamLocalLink, "e:\\testing\\data\\local.3gp" );
#endif

    TInt linkType;
    TPtrC link;

    User::LeaveIfError( aItem.GetNextInt( linkType ) );

    link.Set(iPlaylistUtility->GetLinkLC());

    // Get the linkType
    switch ( linkType )
    {
        case ERamFileStreaming:
        {
            if ( link.CompareF(KRamStreamingLink) )
            {
                error = KErrArgument;
            }
            break;
        }
        case ERamFileLocal:
        {
            if ( link.CompareF(KRamLocalLink) )
            {
                error = KErrArgument;
            }
            break;
        }
        case EAsxFileStreaming:
        {
            if ( link.CompareF(KAsxStreamingLink) )
            {
                error = KErrArgument;
            }
            break;
        }

    }

    CleanupStack::PopAndDestroy();
    return error;
}


// -------------------------------------------------------------------------------------------------
//   CVideoPlaylistUtility_Tester::GetPlaylistL
// -------------------------------------------------------------------------------------------------
//
TInt CVideoPlaylistUtility_Tester::GetPlaylistL( CStifItemParser& aItem )
{
    TInt error = KErrNone;
    TInt linkCount;
    TInt expectedCount;

    TInt accessPointId;
    TInt embeddedInfo;

    User::LeaveIfError( aItem.GetNextInt( accessPointId ) );
    User::LeaveIfError( aItem.GetNextInt( expectedCount ) );
    User::LeaveIfError( aItem.GetNextInt( embeddedInfo ) );

    CMPXMedia* playlist = iPlaylistUtility->GetPlayListL(accessPointId);

    linkCount = playlist->ValueTObjectL<TInt>(KMPXMediaArrayCount);

    if ( linkCount != expectedCount )
    {
       error = KErrArgument;
    }

    // Get the mediaType
    switch ( embeddedInfo )
    {
        case EEmbeddedSeekInfo:
        {
            CMPXMediaArray* mediaArray = 
                    playlist->Value<CMPXMediaArray>(KMPXMediaArrayContents);
            
            for ( TInt i = 0; i < mediaArray->Count(); i++ )
            {
                CMPXMedia* media = mediaArray->AtL( i );
                if ( !(media->IsSupported( KMPXMediaGeneralExtVideoSeekable )) )
                 {
                      // The test clip has all the links set to non seek                     }
                      error = KErrArgument;  
                      break;                      
                 }
            }
            break;
        }
        case ENoEmbeddedInfo:
        {
            CMPXMediaArray* mediaArray = 
                    playlist->Value<CMPXMediaArray>(KMPXMediaArrayContents);
            
            for ( TInt i = 0; i < mediaArray->Count(); i++ )
            {
                CMPXMedia* media = mediaArray->AtL( i );
                if ((media->IsSupported( KMPXMediaGeneralExtVideoSeekable )) )
                 {
                      // The test clip has all the links set to non seek                     }
                      error = KErrArgument;  
                      break;                      
                 }
            }
            break;
        }
        default:
        {
            break;
        }
    }
        
        
    delete playlist;
    return error;
}

// -------------------------------------------------------------------------------------------------
//   CVideoPlaylistUtility_Tester::IsSeekableL
// -------------------------------------------------------------------------------------------------
//
TInt CVideoPlaylistUtility_Tester::IsSeekableL( CStifItemParser& aItem )
{
    TInt error = KErrNone;
    TBool seekable;

    User::LeaveIfError( aItem.GetNextInt( seekable ) );

    if ( seekable != iPlaylistUtility->IsSeekable())
    {
        error = KErrArgument;
    }

    return error;
}


TFileName CVideoPlaylistUtility_Tester::GetFileNameAndPathL( CStifItemParser& aItem )
{
    TPtrC filename;

#ifdef __WINSCW__
    _LIT( KVideoTestPath, "c:\\data\\Videos\\" );
#else
    _LIT( KVideoTestPath, "e:\\testing\\data\\" );
#endif

    User::LeaveIfError( aItem.GetNextString( filename ) );

    TFileName fullPath;

    fullPath.Append( KVideoTestPath );

    fullPath.Append( filename );

    return fullPath;
}

// EOF
