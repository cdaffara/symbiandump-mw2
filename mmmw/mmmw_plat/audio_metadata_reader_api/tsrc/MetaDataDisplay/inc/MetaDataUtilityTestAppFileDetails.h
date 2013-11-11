/*
* Copyright (c) 2002, 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for file details
*
*/



#ifndef METADATAUTILITYTESTAPPFILEDETAILS_H
#define METADATAUTILITYTESTAPPFILEDETAILS_H

//  INCLUDES
#include <f32file.h>
#include <e32base.h>
//#include <mplayerdrmhelper.h>

// FORWARD DECLARATIONS
//class CMetaDataUtilityTestAppVideoPlayerUIController;
//class CMetaDataUtilityTestAppFileDetailsDialog;

// CLASS DECLARATION

/**
*  CMetaDataUtilityTestAppFileDetails 
*  Container class for file details
*
*  @lib MetaDataUtilityTestApp.lib
*  @since 2.0
*/
NONSHARABLE_CLASS(CMetaDataUtilityTestAppFileDetails) : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        IMPORT_C CMetaDataUtilityTestAppFileDetails();
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CMetaDataUtilityTestAppFileDetails();

    public:    // Data

        HBufC* iTitle;
        HBufC* iFileName;
		HBufC* iFilePath;
        HBufC* iUrl;
        HBufC* iFormat;
        HBufC* iCopyright;      
        HBufC* iArtist;
        HBufC* iAlbum;
        HBufC* iYear;

        HBufC* iOriginalArtist; 
        HBufC* iAlbumTrack;
        HBufC* iGenre;
        HBufC* iComposer;
        HBufC* iAudioFileWebPage;
        HBufC* iComment;
        HBufC* iProvider;
        HBufC* iDescription;
        HBufC* iRating;
        HBufC* iVendor;
        HBufC8* iAlbumArt;
        HBufC* iLyrics;

        // For X-pcs Metadata
        HBufC* iArtistXpcs;
        HBufC* iTitleXpcs;
        HBufC* iAlbumXpcs;
        HBufC* iInfoXpcs;
        
        TInt   iResolutionWidth;
        TInt   iResolutionHeight;
        TInt   iBitrate;
        TInt   iSamplerate;
        TInt   iSize;
        TInt   iMaxVolume;
 
        TInt64 iDurationInSeconds;
    
        TTime  iTime;
        
        TBool  iLiveStream;
        TBool  iSeekable;
        TBool  iAudioTrack;
        TBool  iVideoTrack;

        RFile   iDetailsFileHandle; 
    };

#endif      // METADATAUTILITYTESTAPPFILEDETAILS_H  
            
// End of File
