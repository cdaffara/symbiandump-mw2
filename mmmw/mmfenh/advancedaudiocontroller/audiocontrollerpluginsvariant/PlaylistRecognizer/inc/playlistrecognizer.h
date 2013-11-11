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
* Description:  Playlist Recognizer for RAM, ASX, and M3U files
*
*/

// Version : %version: bh1mmcf#3 %


#ifndef _PLAYLISTRECOGNIZER_H_
#define _PLAYLISTRECOGNIZER_H_

//  INCLUDES
#if !defined(__APMREC_H__)
#include <apmrec.h>
#endif

//  CONSTANTS
const TInt KPlaylistRecognizerImplementationUid = 0x101F8825;


// CLASS DECLARATION

/**
*  CPlaylistRecognizer
*  Recognizer plug-in for RAM and M3U files
*
*  @lib MediaPlayerRecognizer.lib
*  @since 2.0
*/
class CPlaylistRecognizer : public CApaDataRecognizerType
{
    public:  // Constructor and destructor

        /**
        * C++ default constructor.
        */
        CPlaylistRecognizer();

        /**
        * Destructor.
        */
        virtual ~CPlaylistRecognizer();

    public: // Functions from CApaDataRecognizerType

        /** @see CApaDataRecognizerType::PreferredBufSize */
        TUint PreferredBufSize();

        /** @see CApaDataRecognizerType::SupportedDataTypeL */
        TDataType SupportedDataTypeL(TInt aIndex) const;

        static CApaDataRecognizerType* CreateRecognizerL();

    private: // Functions from CApaDataRecognizerType

        /** @see CApaDataRecognizerType::DoRecognizeL */
        void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);


		/**
		* Do the recognition of the Matroska file using the name of the
		* the file or the buffer to scan
		* @since 9.2
		* @param aName    Name of the file
		* @param aBuffer  Buffer that shall be scanned for recognition
		* @return void
		*/
        
        void DoRecognizeMatroskaL( const TDesC& aName, const TDesC8& aBuffer );

};

#endif  // _PLAYLISTRECOGNIZER_H_

// End of File
