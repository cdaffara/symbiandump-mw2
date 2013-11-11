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
* Description:  Implementation of CMediaRecognizer
*
*/

// Version : %version: 6 %



#ifndef __MEDIARECOGNIZER_H
#define __MEDIARECOGNIZER_H

//  INCLUDES
#include <e32base.h>
#include <apgcli.h>
#include <apmstd.h> 
#include <mmf/common/mmfcontrollerpluginresolver.h> 


NONSHARABLE_CLASS(CMediaRecognizer) : public CBase
{

    public:
        // Media types
        enum TMediaType
        {
            ELocalVideoFile,
            ELocalAudioFile,
            ELocalRamFile,
            ELocalSdpFile,
            EUrl,
            ELocalAudioPlaylist,
            EProgressiveDownload,
            EFlashVideoFile,
            ELocalAsxFile,            
            EUnidentified 
        };

    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMediaRecognizer* NewL();
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CMediaRecognizer();

    public: // New functions
        
        /**
        * Recognizes media type. 
        * @since 2.0
        * @param aMediaName Media file name or Url
        * @param aIncludeUrls Are Urls included or not
        * @return TMediaType
        */
        IMPORT_C TMediaType IdentifyMediaTypeL( const TDesC& aMediaName, 
                                                TBool aIncludeUrls = EFalse );

        /**
        * Recognizes media type. 
        * @since 3.0
        * @param aMediaName Media file name or Url
        * @param aFile file handle 
        * @param aIncludeUrls Are Urls included or not
        * @return TMediaType
        */
        IMPORT_C TMediaType IdentifyMediaTypeL( const TDesC& aMediaName, 
                                                RFile& aFile,  
                                                TBool aIncludeUrls = EFalse );

        /**
        * Recognizes Media files MimeType. 
        * @since 2.0
        * @param aLocalFile File name
        * @return TBuf<KMaxDataTypeLength>, Mime type.
        */
        IMPORT_C TBuf<KMaxDataTypeLength> MimeTypeL( const TDesC& aLocalFile );

        /**
        * Recognizes Media files MimeType. 
        * @since 3.0
        * @param aFile File handle
        * @return TBuf<KMaxDataTypeLength>, Mime type.
        */
        IMPORT_C TBuf<KMaxDataTypeLength> MimeTypeL( RFile& aFile ); 

        /*
        *  IsValidStreamingPrefix
        *  Check for a valid streaming prefix given a URL
        *  @param aUrl       URL
        *  @return           ETrue is prefix is valid
        */
        IMPORT_C TBool IsValidStreamingPrefix( const TDesC& aUrl );

        /*
        *  IsValidUrlPrefix
        *  Check for a valid prefix given a URL
        *  @param aUrl       URL
        *  @return           ETrue is prefix is valid
        */
        IMPORT_C TBool IsValidUrlPrefix( const TDesC& aUrl );
        IMPORT_C TBool IsValidUrlPrefix( const TDesC8& aUrl );

		/**
        * Frees recognizer's filehandle. 
        * @since 3.0
        */
        IMPORT_C void FreeFilehandle(); 
                    
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

        /**
        * Recognizes media type. 
        * @since 9.2
        * @param aMediaName Media file name or Url
        * @param aFile RFile64 file handle
        * @param aIncludeUrls Are Urls included or not
        * @return TMediaType
        */
        IMPORT_C TMediaType IdentifyMediaTypeL( const TDesC& aMediaName, 
                                                RFile64& aFile,  
                                                TBool aIncludeUrls = EFalse );        

        /**
        * Recognizes Media files MimeType. 
        * @since 9.2
        * @param aFile RFile64 file handle
        * @return TBuf<KMaxDataTypeLength>, Mime type.
        */
        IMPORT_C TBuf<KMaxDataTypeLength> MimeTypeL( RFile64& aFile ); 
        
#endif

    private:  // New functions
              
        /**
        * Checks if there is video playback support.
        * @since 2.0
        * @param aFileName
        * @param aFileRecognitionResult Recognition result
        * @return ETrue:    Supported
        *         EFalse:   Not supported
        */
        TBool FileHasVideoSupportL( const TDesC& aFileName,
                                    TDataRecognitionResult& aFileRecognitionResult,
                                    RMMFControllerImplInfoArray& aVideoControllers );

        /**
        * Checks if there is audio playback support.
        * @since 2.0
        * @param aFileName
        * @param aFileRecognitionResult Recognition result
        * @return ETrue:    Supported
        *         EFalse:   Not supported
        */
        TBool FileHasAudioSupport( const TDesC& aFileName,
                                   TDataRecognitionResult& aFileRecognitionResult,
                                   RMMFControllerImplInfoArray& aAudioControllers,
                                   RMMFControllerImplInfoArray& aVideoControllers );
        
        /**
        * Creates array to hold all the controller plugins that support video.
        * @since 2.0
        * @return void
        */
        void CreateVideoFormatsArrayL( RMMFControllerImplInfoArray& aVideoController );

        /**
        * Creates array to hold all the controller plugins that support audio.
        * @since 2.0
        * @return void
        */
        void CreateAudioFormatsArrayL( RMMFControllerImplInfoArray& aAudioController );

        /**
        * Recognizes local file
        * @since 2.0
        * @param aFileName. 
        * This parameter is ignored if the recognizer has a valid filehandle.
        * Filehandle can be set via IdentifyMediaTypeL or MimeTypeL methods.
        * @param aFileRecognitionResult Recognition result
        * @return void
        */
        void RecognizeFileL( const TDesC& aFileName, TDataRecognitionResult& aResult );
        
        /**
        * Reads local file to buffer
        * @since 2.0
        * @param aFileName
        * @param aBuf, buffer
        * @return KErrNone or one of the system wide error codes.
        */
        TInt ReadFile( const TDesC& aFileName, TDes8& aBuf, TInt aBufSize );

        /**
        * Determines presence of file handle.
        * @since 3.0
        * @return ETrue if file handle exists.
        */
        TBool FileHandleExists();

        /**
        * Recognizes media type for non-url type
        * @return TMediaType
        */
        TMediaType MediaTypeL( const TDesC& aMediaName );

        /**
        * Reads local file to buffer using new RFile handle
        * @since 9.2
        * @param aFileName
        * @param aBuf, buffer
        * @return KErrNone or one of the system wide error codes.
        */
        TInt ReadFileViaNewFileHandle( const TDesC& aFileName, 
                                       TDes8& aBuf, TInt aBufSize );
        
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

        /**
        * Determines presence of file handle.
        * @since 9.2
        * @return ETrue if file handle exists.
        */
        TBool FileHandle64Exists();

        /**
        * Reads local file to buffer using new RFile64 handle
        * @since 9.2
        * @param aFileName
        * @param aBuf, buffer
        * @return KErrNone or one of the system wide error codes.
        */
        TInt ReadFileViaNewFileHandle64( const TDesC& aFileName, 
                                         TDes8& aBuf, TInt aBufSize  );

#endif

    private:

        /**
        * C++ default constructor.
        */
        CMediaRecognizer();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data
        // Array to hold all the controller plugins that support video
        // Array to hold all the controller plugins that support audio

        // bufsize

        // Mime type recognizer
        RFs            iFs;
        RFile          iFileHandle; 
        
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
        RFile64        iFileHandle64;        
#endif

};

#endif      // __MEDIARECOGNIZER_H
            
// End of File
