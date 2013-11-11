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
* Description:  Implementation of CVideoPlaylistUtility
*
*/

// Version : %version: 2 %



#ifndef __VIDEOPLAYLISTUTILITY_H
#define __VIDEOPLAYLISTUTILITY_H

//  INCLUDES
#include <e32base.h>
#include <f32file.h>
#include <badesca.h>
#include <e32def.h>
#include <mediarecognizer.h>
#include <mpxmedia.h>

// CLASS DECLARATION
class CMPXMediaArray;
class CStreamingLinkModel;

NONSHARABLE_CLASS(CVideoPlaylistUtility) : public CBase
{
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CVideoPlaylistUtility* NewL();
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CVideoPlaylistUtility();

    public: // New functions
        
        IMPORT_C CMPXMedia*  GetPlayListL( TInt aAccessPointId);
        IMPORT_C TPtrC GetLinkLC();

        IMPORT_C void GetFileInfoL( const TDesC& aFileName, 
                                          CMediaRecognizer::TMediaType aMediaType, 
                                          TBool& aSingleLink, TBool& aLocalFile );
        IMPORT_C void GetFileInfoL( RFile& aFile, 
                                          CMediaRecognizer::TMediaType aMediaType, 
                                          TBool& aSingleLink, TBool& aLocalFile );
        
        IMPORT_C TBool IsSeekable();

    private:

        /**
        * C++ default constructor.
        */
        CVideoPlaylistUtility();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:

        /**
        * Reads next line from buffer
        * @since 2.0
        * @param aLine Whole line which was read.
        * @return KErrNone or one of the system wide error codes.
        */
        void GetItemArrayL( TInt aAccessPointId);
        
        void ComposePlaylistL();

    private:    // Data
        CMPXMediaArray*                     iPlaylistArray;        
        CMPXMedia*                          iPlaylist;
        CStreamingLinkModel*                iStreamingLinkModel;
        TBool                               iAsx;
};

#endif      // __VIDEOPLAYLISTUTILITY_H
            
// End of File
