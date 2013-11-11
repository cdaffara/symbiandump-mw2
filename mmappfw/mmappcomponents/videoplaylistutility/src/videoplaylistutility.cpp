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
* Description:  Implementation of CVideoPlaylistUtility
*
*/

// Version : %version: 2 %



// INCLUDE FILES

#include <videoplaylistutility.h>
#include <mediarecognizer.h>
#include <streaminglinkmodel.h>
#include <mpxmediaarray.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxmediageneralextdefs.h>

#include "videoplaylistutility_log.h"


// CONSTANTS
const TInt KSingleLink(1);
_LIT( KDATATYPEVIDEOHELIX, "video/videohelix" );


// ============================ MEMBER FUNCTIONS ===================================================

// -------------------------------------------------------------------------------------------------
// CVideoPlaylistUtility::CVideoPlaylistUtility
// C++ default constructor can NOT contain any code, that
// might leave.
// -------------------------------------------------------------------------------------------------
//
CVideoPlaylistUtility::CVideoPlaylistUtility()
: iAsx( EFalse )
{

}

// -------------------------------------------------------------------------------------------------
// CVideoPlaylistUtility::ConstructL
// Symbian 2nd phase constructor can leave.
// -------------------------------------------------------------------------------------------------
//
void CVideoPlaylistUtility::ConstructL()
{

}

// -------------------------------------------------------------------------------------------------
// CVideoPlaylistUtility::NewL
// Two-phased constructor.
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CVideoPlaylistUtility* CVideoPlaylistUtility::NewL()
{
    VIDEOPLAYLISTUTILITY_ENTER_EXIT(_L("CVideoPlaylistUtility::NewL()"));

    CVideoPlaylistUtility* self = new( ELeave ) CVideoPlaylistUtility();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

// -------------------------------------------------------------------------------------------------
// CVideoPlaylistUtility::~CVideoPlaylistUtility
// Destructor
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CVideoPlaylistUtility::~CVideoPlaylistUtility()
{
    VIDEOPLAYLISTUTILITY_ENTER_EXIT(_L("CVideoPlaylistUtility::~CVideoPlaylistUtility()"));
     
    if(iPlaylistArray)
    {
        delete iPlaylistArray;
        iPlaylistArray = NULL;
    }

    if(iStreamingLinkModel)
    {
        iStreamingLinkModel->ResetLinkCount();
        iStreamingLinkModel->FreeFilehandle();

        delete iStreamingLinkModel;
        iStreamingLinkModel = NULL;
    }
}

// -------------------------------------------------------------------------------------------------
//   CVideoPlaylistUtility::GetItemArrayL
// -------------------------------------------------------------------------------------------------
//
void CVideoPlaylistUtility::GetItemArrayL( TInt aAccessPointId )
{
    VIDEOPLAYLISTUTILITY_ENTER_EXIT(_L("CVideoPlaylistUtility::GetItemArrayL()" ));
        
    HBufC* tempBuffer = HBufC::NewLC( iStreamingLinkModel->MaxLinkLength() );
    TPtr tempurl(tempBuffer->Des());
    
    TInt linkError = KErrNone; 
    TInt localUrl;
    TBool seek = ETrue;

    iPlaylistArray = CMPXMediaArray::NewL();
    
    while(linkError == KErrNone)
    {
        linkError = iStreamingLinkModel->GetNextLinkL( tempurl, localUrl ); 
        
        if (linkError == KErrNone)
        {
            CMPXMedia* lItem = CMPXMedia::NewL();
            CleanupStack::PushL( lItem );

            lItem->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
            lItem->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXVideo);
            // Bump up the score during plugin resolution in the FW
            lItem->SetTextValueL(KMPXMediaGeneralMimeType, KDATATYPEVIDEOHELIX);
            lItem->SetTextValueL(KMPXMediaGeneralUri, *tempBuffer);
            if (!localUrl)
            {
                // embed the access point into the playlist
                lItem->SetTObjectValueL(KMPXMediaGeneralExtAccessPoint, aAccessPointId); 
            }
            
            if ( iAsx )
            {
                // Get asx seek attribute
                seek = iStreamingLinkModel->IsSeekable();

                lItem->SetTObjectValueL<TBool>(KMPXMediaGeneralExtVideoSeekable, seek); 
            }
            
            CleanupStack::Pop( lItem );
            
            iPlaylistArray->AppendL(lItem);
            lItem = NULL;   
        }
    }
        

    CleanupStack::Pop();  // tempBuffer
    delete tempBuffer;    
}

// -----------------------------------------------------------------------------
// CMPlayerM3UPlaylistParser::ComposePlaylistL
// -----------------------------------------------------------------------------
//
void CVideoPlaylistUtility::ComposePlaylistL()
{
    VIDEOPLAYLISTUTILITY_ENTER_EXIT(_L("CVideoPlaylistUtility::ComposePlaylistL()"));
    
    // instantiate a CMPXMedia object to represent the playlist 
    // which will contain the CMPXMediaArray
    iPlaylist = CMPXMedia::NewL();
            
    // set type
    iPlaylist->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
        
    // set category
    iPlaylist->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXPlaylist);        

    // set iPlaylist array
    iPlaylist->SetCObjectValueL(KMPXMediaArrayContents, iPlaylistArray);
        
    // set array acount
    iPlaylist->SetTObjectValueL(KMPXMediaArrayCount, iPlaylistArray->Count());

    // the ownership has been transfered 
    // safe to delete 
    delete iPlaylistArray;
    iPlaylistArray = NULL;

}
    
// -------------------------------------------------------------------------------------------------
// CVideoPlaylistUtility::OpenLinkFileL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CMPXMedia* CVideoPlaylistUtility::GetPlayListL( TInt aAccessPointId )
{
    VIDEOPLAYLISTUTILITY_ENTER_EXIT(_L("CVideoPlaylistUtility::GetPlayListL()"));
    
    CMPXMedia* playlist = NULL;
    
    // Create the CMPXMediaItemArray that will have the links
    GetItemArrayL(aAccessPointId);
    
    // Embed the CMPXMediaItemArray into a CMPXMedia object 
    ComposePlaylistL();
    
    playlist = iPlaylist;
    iPlaylist = NULL;
    
    return playlist;
}


// -------------------------------------------------------------------------------------------------
// CVideoPlaylistUtility::GetFileInfoL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C void CVideoPlaylistUtility::GetFileInfoL( const TDesC& aFileName, 
                                                        CMediaRecognizer::TMediaType aMediaType,
                                                        TBool& aSingleLink, TBool& aLocalFile )
{
    VIDEOPLAYLISTUTILITY_ENTER_EXIT(_L("CVideoPlaylistUtility::GetFileInfoL() FileName "));

    TInt error = KErrNone;  
    aSingleLink = aLocalFile = EFalse;
    iStreamingLinkModel = CStreamingLinkModel::NewL();
    
        
    if ( aMediaType == CMediaRecognizer::ELocalRamFile )
    {
        error = iStreamingLinkModel->OpenLinkFileL( aFileName );
    }
    else 
    {
        error = iStreamingLinkModel->OpenAsxFileL( aFileName ); 
        iAsx = ETrue;
    }
    
    if(error == KErrNone)
    {
        // check to see if its a single link file 
        if (iStreamingLinkModel->MultiLinksCount() == KSingleLink)
        {
            aSingleLink = ETrue;
        }
        aLocalFile = iStreamingLinkModel->AreAllLinksLocal();
    }
    else
    {
        User::Leave( KErrBadName );
    }
}

// -------------------------------------------------------------------------------------------------
// CVideoPlaylistUtility::GetFileInfoL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C void CVideoPlaylistUtility::GetFileInfoL( RFile& aFile, 
                                                        CMediaRecognizer::TMediaType aMediaType,
                                                        TBool& aSingleLink, TBool& aLocalFile )
{
    VIDEOPLAYLISTUTILITY_ENTER_EXIT(_L("CVideoPlaylistUtility::GetFileInfoL() RFile"));

    TInt error = KErrNone;  
    aSingleLink = aLocalFile = EFalse;
    iStreamingLinkModel = CStreamingLinkModel::NewL();
            
    if ( aMediaType == CMediaRecognizer::ELocalRamFile )
    {
        error = iStreamingLinkModel->OpenLinkFileL( aFile );
    }
    else 
    {
        error = iStreamingLinkModel->OpenAsxFileL( aFile ); 
        iAsx = ETrue;
    }
    
    if(error == KErrNone)
    {
        // check to see if its a single link file 
        if (iStreamingLinkModel->MultiLinksCount() == KSingleLink)
        {
            aSingleLink = ETrue;
        }
        aLocalFile = iStreamingLinkModel->AreAllLinksLocal();
    }
    else
    {
        User::Leave( KErrBadName );
    }
}

// -------------------------------------------------------------------------------------------------
// CVideoPlaylistUtility::GetLinkLC
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TPtrC CVideoPlaylistUtility::GetLinkLC()
{
    VIDEOPLAYLISTUTILITY_ENTER_EXIT(_L("CVideoPlaylistUtility::GetLinkLC()"));

    TBool localUrl;
    HBufC* tempBuffer = HBufC::NewLC( iStreamingLinkModel->MaxLinkLength() );
  
    TPtr tempurl(tempBuffer->Des());
    
    iStreamingLinkModel->GetNextLinkL( tempurl, localUrl );
            
    // push tempBuffer to the cleanup stack again
    // caller of GetLinkLC will pop and destroy it
    return tempurl;
}

// -------------------------------------------------------------------------------------------------
// CVideoPlaylistUtility::IsSeekable
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TBool CVideoPlaylistUtility::IsSeekable()
{
    VIDEOPLAYLISTUTILITY_ENTER_EXIT(_L("CVideoPlaylistUtility::Seekable()"));

    return iStreamingLinkModel->IsSeekable();
}

//  End of File
