/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Provides functionality for DRM file protection analyzing
*                and content MMS compatibility resolving.
*
*/




#ifndef SENDUIFILERIGHTSENGINE_H
#define SENDUIFILERIGHTSENGINE_H

//  INCLUDES
#include    <e32base.h>
#include    <f32file.h>            // FileSession
#include    <badesca.h>


// DATA TYPES
enum TFREErrorNoteType
    {
    EErrorNote = 0,
    EInformationNote,
    EConfirmationQuery
    };
            
// FORWARD DECLARATIONS
class CMsgMediaResolver;
class CMmsConformance;
class CFileProtectionResolver;
class CSendUiAttachment;
// CLASS DECLARATION

/**
* Provides functionality for DRM file protection analyzing
* and content MMS compatibility resolving.
*
*  @lib SenduiDataUtils.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS (CSendUiFileRightsEngine) : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSendUiFileRightsEngine* NewL( RFs& aFs );

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSendUiFileRightsEngine* NewLC( RFs& aFs );

        /**
        * Destructor.
        */
        virtual ~CSendUiFileRightsEngine();

    public: // New functions

        /**
        * ValidateFiles
        *
        * This functions stores file sizes for all validated files.
        *
        * @since S60 3.0
        * @param aAttachments Attachents to be validated.
        *        Return value contains valid attachments,
        *        invalid attachments are filtered away from array.
        * @return None.
        */
        IMPORT_C void ValidateFiles( CArrayPtrFlat<CSendUiAttachment>* aAttachments );

        /**
        * Confirms DRM rights of files
        *
        * Note: files must be validated before DRM analyze. This functions also 
        * stores mimetypes of files for later use. Mimetypes are stored into
        * the given array.
        *
        * @since S60 3.0
        * @param aAttachments Attachments (Path and handle) to be validated.
        *        Return value contains valid attachments,
        *        invalid attachments are filtered away from array.
        * @return None.
        */
        IMPORT_C void ConfirmDrmFileRightsL( CArrayPtrFlat<CSendUiAttachment>* aAttachments );
        
        /**
        * Validates files for sending via MMS
        *
        * Note: files must be validated before MMS validation.
        *
        * @since S60 3.0
        * @param aAttachments Attachments to be validated.
        *        Return value contains valid attachments,
        *        invalid attachments are filtered away from array.
        * @param aServiceId Identifies the type of MMS service (MMS/MMS upload..)
        * @return None.
        */

        IMPORT_C void ConfirmMmsValidityL( CArrayPtrFlat<CSendUiAttachment>* aAttachments, TUid aServiceId = KNullUid );
        
        /**
        * Shows DRM confirmation query and/or error note, if needed.
        * Returns EFalse if user cancels sending.
        * @since S60 v.3.0
        * @param aServiceId Different services get different notes.
        * @return EFalse if user cancels sending, otherwise ETrue.
        */
        IMPORT_C TBool ShowDrmAndMmsInfoL( TUid aServiceId = KNullUid );

        /**
        * CalculateTotalSizeOfFiles
        *
        * File sizes saved by ValidateFiles are used. If cached file sizes 
        * do not exist, then file sizes are solved.
        *
        * @aAttachments Attachments
        * @aLinksSupported If ETrue, then only attachments type of file handles are calculated
        *                  total size of files. Otherwise filesize of all attachments is returned.
        * @return Size of files in aAttachments 
        */
        IMPORT_C TInt CalculateTotalSizeOfFiles(
            CArrayPtrFlat<CSendUiAttachment>* aAttachments,
            TBool aLinksSupported );

        /**
        * Clears all validation counter
        *
        * @since S60 v3.0
        * @return None.
        */
        IMPORT_C void ClearValidationCounters();

       /**
        * Shows an information note
        * @since S60 v.3.0
        * @param aTextResourceId Resource id of the note
        * @return None.
        */
        IMPORT_C void ShowInformationNoteL( TUint aTextResourceId ) const;
        
         /**
        * Cleanup ECOM Implementation info array
        * @param aAny The array
        * @return None.
        */
        static void CleanupImplArray( TAny* aAny );

    private:

        /**
        * C++ default constructor.
        */
        CSendUiFileRightsEngine( RFs& aFS );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // New functions

        /**
        * Validates that the given file usable
        *
        * @since S60 v3.0
        * @param aFileName file path
        * @param aFileSize file size
        * @return ETrue, if the file can be opened for reading. Otherwise EFalse
        */
        TBool ValidateFile( const TDesC& aFileName, TInt& aFileSize );

        /**
        * AnalyzeMmsValidationResult
        * @since S60 v.3.0
        * @param aAttachment Attachment to be analyzed
        * @return ETrue, if attachment is valid. Otherwise EFalse.
        */
        TBool AnalyzeMmsValidationResultL( CSendUiAttachment& aAttachment, TInt& aAttachmentsSize );

        /**
        * Resolves a proper DRM query for the given service
        * @since S60 v.3.0
        * @param aServiceId Service Uid
        * @param aResourceId Id for a proper query in resources.
        * @return None.
        */
        void ResolveDrmQuery( TUid aServiceId, TUint& aResourceId );

        /**
        * Resolves a proper error note for the given service
        * @since S60 v.3.0
        * @param aServiceId Service Uid
        * @param aResourceId Id for a proper note in resources.
        * @param aNoteType Type of the note (query/info)
        * @param aValueForNote Value for the variable in the note
        * @return None.
        */
        void ResolveErrorNoteL( 
            TUid aServiceId,
            TUint& aResourceId,
            TFREErrorNoteType& aNoteType,
            TUint& aValueForNote );

        /**
        * Resolves a proper error note for the MMS Upload services
        * @since S60 v.3.0
        * @param aResourceId Id for a proper note in resources.
        * @param aNoteType Type of the note (query/info)
        * @param aValueForNote Value for the variable in the note
        * @return None.
        */
        void ResolveMmsUploadErrorNoteL(
            TUint& aResourceId, 
            TFREErrorNoteType& aNoteType,
            TUint& aValueForNote );

        /**
        * Resolves a proper note
        * @since S60 v.3.0
        * @param aResourceId Id for a proper note in resources.
        * @param aNoteType Type of the note
        * @param aValueForNote Value for the variable in the note
        * @return None.
        */
        void ShowErrorNotificationL(
            TUint aResourceId,
            TFREErrorNoteType aNoteType,
            TUint aValueForNote ) const;

        /**
        * Shows an error note
        * @since S60 v.3.0
        * @param aTextResourceId Resource id of the note
        * @return None.
        */
        void ShowErrorNoteL( TUint aTextResourceId ) const;

        /**
        * Shows a confirmation query
        * @since S60 v.3.0
        * @param aTextResourceId Text id for the note
        * @param aNoteResourceId Resource id for the note
        * @param aValueForNote Variable value for the note
        * @return None.
        */
        TBool ShowConfirmationQueryL(
            TUint aTextResourceId,
            TUint aNoteResourceId,
            TUint aValueForNote = NULL ) const;

   
    private:    // Data     

        // Validation results.
        enum TFREValidationResults
            {
            ESenduiIndexTotalCount = 0,
            ESenduiIndexDeleted,
            ESenduiIndexDrmSD,          // SuperDistributable
            ESenduiIndexLastResult
            };

        // Reasons for invalidation.
        enum TFREInvalidTypes
            {
            ESenduiIndexDrmFLOrCCL = 0,
            ESenduiIndexAlreadyInUse,
            ESenduiIndexMmsTooBig,
            ESenduiIndexMmsNotSupported,
            ESenduiIndexBadHandle,
            ESenduiIndexLastInvalid
            };

        // File protection resolver. Owned.
        CFileProtectionResolver*    iProtectionResolver;

        CMsgMediaResolver*      iMediaResolver;
        CMmsConformance*        iMmsConformance;

        // File session.
        RFs                     iFileSession;

        // Results of file validations, DRM analyses and MMS validations.
        TUint                   iValidationResults[ESenduiIndexLastResult];

        TUint                   iInvalidationReason[ESenduiIndexLastInvalid];
    };

#endif      // SENDUIFILERIGHTSENGINE_H   

// End of File
