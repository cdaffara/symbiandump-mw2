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

// Version : %version: 5 %



#ifndef __STREAMINGLINKMODEL_H
#define __STREAMINGLINKMODEL_H

//  INCLUDES
#include <e32base.h>
#include <f32file.h>
#include <badesca.h>
#include <e32def.h>

#ifdef __WINDOWS_MEDIA
#include <asxparser.h>
#endif

// CLASS DECLARATION
class MCLFContentListingEngine;

struct LinkStruct {
    HBufC*  link;
    TBool   seek;
};

NONSHARABLE_CLASS(CStreamingLinkModel) : public CBase
{
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CStreamingLinkModel* NewL();
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CStreamingLinkModel();

    public: // New functions
        
        /**
        * Opens and reads RAM file to buffer.
        * @since 2.0
        * @param aRamFileName RAM file
        * @param aEnableFiltering, Is ram file content filtered or not
        * @return KErrNone or KErrNotFound if no valid links in file
        */
        IMPORT_C TInt OpenLinkFileL( const TDesC& aLinkFileName, TBool aEnableFiltering = ETrue );

        /**
        * Opens and reads RAM file to buffer.
        * @since 9.2
        * @param aFile file handle
        * @param aEnableFiltering, Is ram file content filtered or not
        * @return KErrNone or KErrNotFound if no valid links in file
        */
        IMPORT_C TInt OpenLinkFileL( RFile& aFile, TBool aEnableFiltering = ETrue );

        /**
        * Gets next link from opened RAM file.
        * @since 2.0
        * @param aLink, New link.
        * @param aLocalFile, Is link to local file or not.
        * @param aParse, Parse the link if not forbidden.
        * @return KErrNone if there are links left in RAM file, 
        *       KErrNotFound if not. 
        */
        IMPORT_C TInt GetNextLinkL( TDes& aLink, TBool& aLocalFile, TBool aParse = ETrue );
        IMPORT_C TBool IsSeekable();
        /**
        * Returns max link length in opened RAM file
        * @since 2.0
        * @return link length
        */
        IMPORT_C TInt MaxLinkLength();

        /**
        * Creates new RAM file
        * @since 2.0
        * @param aNewRamFileName, New RAM file.
        * @param aLinkArray, Array which contains links which are 
        *       saved to RAM file.
        * @param aOverWrite, Overwrite existing file or not.
        * @return KErrNone or one of the system wide error codes.         
        */
        IMPORT_C TInt CreateNewLinkFileL( const TDesC& aNewLinkFileName, 
                                          MDesCArray* aLinkArray, 
                                          TBool aOverWrite = EFalse );

		/**
        * Frees model's filehandle. 
        * @since 3.0
        */
        IMPORT_C void FreeFilehandle(); 

        /**
        * Returns number of links in opened RAM file
        * @since 3.2
        * @return number of links
        */
        IMPORT_C TInt MultiLinksCount(); 

        /**
        * Reset number of links in opened RAM file for Repeat playlist
        * @since 3.2
        */
        IMPORT_C void ResetLinkCount(); 
     
        /**
        * Opens and reads ASX file to buffer.
        * @since 9.2
        * @param aLinkFileName Asx file
        * @param aEnableFiltering, Is ASX file content filtered or not
        * @return KErrNone or KErrNotFound if no valid links in file
        */
        IMPORT_C TInt OpenAsxFileL( const TDesC& aLinkFileName,  
                           			TBool aEnableFiltering = ETrue );                

        /**
        * Opens and reads ASX file to buffer.
        * @since 9.2
        * @param aFile file handle
        * @param aEnableFiltering, Is ASX file content filtered or not
        * @return KErrNone or KErrNotFound if no valid links in file
        */
        IMPORT_C TInt OpenAsxFileL( RFile& aFile,    
                           			TBool aEnableFiltering = ETrue ); 

         /**
        * Returns if all the links in the file are local
        * @
        * @return ETrue if all the links are local
        */       
        IMPORT_C TBool AreAllLinksLocal();
		
		private:

        /**
        * C++ default constructor.
        */
        CStreamingLinkModel();

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
    	TInt ReadNextLine( TDes& aLine );

        /**
        * Determines presence of file handle.
        * @since 9.2
        * @param aFile file handle
        * @return ETrue if file handle exists.
        */
        TBool FileHandleExists( RFile& aFile );
        
        /**
        * Parse the ram file and add the link(s) to buffer.
        * @since 9.2
        * @param aRamFile a Ram file
        * @param aEnableFiltering, is Ram file content filtered or not
        * @return KErrNone or KErrNotFound if no valid links in file
        */
		TInt PopulateRamLinksL( RFile aRamFile, TBool aEnableFiltering );

        /**
        * Parse the Asx file and add the link(s) to buffer.
        * @since 9.2
        * @param aAsxParser 
        * @param aEnableFiltering, is Asx file content filtered or not
        * @return KErrNone or KErrNotFound if no valid links in file
        */
		TInt PopulateAsxLinksL( CAsxParser* aAsxParser, TBool aEnableFiltering );

    private:    // Data
        RFs                         iFs;
        RFile                       iFileHandle;

        RPointerArray<LinkStruct>   iLinkArray;
        TPtrC8                      iBufferPtr;

        TInt                        iLinkCount;

        MCLFContentListingEngine*   iCLFEngine;
};

#endif      // __STREAMINGLINKMODEL_H
            
// End of File
