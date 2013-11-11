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



// INCLUDE FILES
#include    "SendUiFileRightsEngine.h"
#include    <SendUiConsts.h>
#include    "SendUiDataUtils.h"

#include    <MsgMedia.hrh>
#include    <MsgMediaResolver.h>            // Msg Media Resolver
#include    <MmsConformance.h>              // MMS Conformance

#include    <sendnorm.rsg>
#include    <AknQueryDialog.h>              // Query dialog
#include    <AknNoteWrappers.h>             // Note dialog
#include    <stringresourcereader.h>        // String Resource Reader
#include    <stringloader.h>                // Stringloader
#include    <data_caging_path_literals.hrh>
#include    <mmsconst.h>
#include    <fileprotectionresolver.h>
#include    <VideoConversionApi.h>
#include    <MsgMimeTypes.h>
#include    "CSendUiAttachment.h"
// CONSTANTS
const TInt KFREKiloByte                     = 1024;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::CSendUiFileRightsEngine
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSendUiFileRightsEngine::CSendUiFileRightsEngine( RFs& aFS )
    : iFileSession( aFS )
    {
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendUiFileRightsEngine::ConstructL()
    {
    iProtectionResolver = CFileProtectionResolver::NewL( iFileSession );
    iMediaResolver = CMsgMediaResolver::NewL();
    iMmsConformance = CMmsConformance::NewL();
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CSendUiFileRightsEngine* CSendUiFileRightsEngine::NewL( RFs& aFs)
    {
    CSendUiFileRightsEngine* self = NewLC( aFs );
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CSendUiFileRightsEngine* CSendUiFileRightsEngine::NewLC( RFs& aFs )
    {
    CSendUiFileRightsEngine* self =
        new( ELeave ) CSendUiFileRightsEngine( aFs );

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// Destructor
CSendUiFileRightsEngine::~CSendUiFileRightsEngine()
    {
    delete iProtectionResolver;
    delete iMediaResolver;
    delete iMmsConformance;
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ConfirmDrmFileRightsL

// Closed content list checking is also done here.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUiFileRightsEngine::ConfirmDrmFileRightsL(
    CArrayPtrFlat<CSendUiAttachment>* aAttachments )
    {
	if ( !aAttachments )
		{
		return;
		}
    TInt fileCount = aAttachments->Count();
    if ( fileCount )
        {
        TInt index = 0;
        TInt protection = 0;
        TDataType mimeType;

        CSendUiDataUtils* dataUtils = CSendUiDataUtils::NewL( iFileSession );
        CleanupStack::PushL( dataUtils );

        while ( index < fileCount)
            {
            iValidationResults[ESenduiIndexTotalCount]++;
            CSendUiAttachment& attachment = *(aAttachments->At(index));
            RFile file = *(attachment.Handle());
            dataUtils->ResolveFileMimeTypeL( file, mimeType );
        	protection = iProtectionResolver->ProtectionStatusL( file, mimeType );
     		attachment.SetMimeType( mimeType );
            	
            if ( protection & EFileProtSuperDistributable )
                {
                iValidationResults[ESenduiIndexDrmSD]++;
                }
            else if ( ( protection & EFileProtForwardLocked ) ||
                      ( protection & EFileProtClosedContent ) )
                {
                iInvalidationReason[ESenduiIndexDrmFLOrCCL]++;
                iValidationResults[ESenduiIndexDeleted]++;
                delete aAttachments->At(index);
                aAttachments->Delete( index );
                
                fileCount--;
                index--;
                }
            index++;
            }

        CleanupStack::PopAndDestroy( dataUtils );
        }
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ConfirmMmsValidityL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUiFileRightsEngine::ConfirmMmsValidityL(
    CArrayPtrFlat<CSendUiAttachment>* aAttachments,
    TUid                aServiceId )
    {
	if ( !aAttachments )
		{
		return;
		}
    TBool removeFilePath = EFalse;
    TInt index = 0;
    TInt fileCount = aAttachments->Count();
    TInt attachmentsSize = 0;
    // MMS image size is set to original for MMS upload service,
    // otherwise oversize images are not filtered away from array
    if( aServiceId == KMmsDirectUpload || aServiceId == KMmsIndirectUpload )
        {
        // Image size setting not used since only MIME type conformance is checked.
        iMmsConformance->SetCreationMode( ETrue );
        }
    if ( fileCount )
        {
        while ( index < fileCount)
            {
            
            // Validate file for MMS
            CSendUiAttachment& attachment = *(aAttachments->At(index));
            removeFilePath = AnalyzeMmsValidationResultL( *(aAttachments->At(index) ), attachmentsSize );
	  
            if ( removeFilePath )
                {
                delete aAttachments->At(index);
                aAttachments->Delete( index );
                iValidationResults[ESenduiIndexDeleted]++;
                fileCount--;
                index--;
                }

            index++;
            }
        }
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ShowDrmAndMmsInfoL
// Shows DRM confirmation query and/or error note, if needed.
// Returns EFalse if user cancels sending.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSendUiFileRightsEngine::ShowDrmAndMmsInfoL( TUid aServiceId )
    {
    TBool showDrmQuery = EFalse;
    TBool showErrorNotification = EFalse;
    TUint drmQueryTextId = 0;
    TUint noteTextId = 0;
    TUint valueForNote = 0;
    TFREErrorNoteType noteType = EInformationNote;

    // Information query and/or error note is shown if needed
    if ( iValidationResults[ESenduiIndexDeleted] )
        {
        ResolveErrorNoteL( aServiceId, noteTextId, noteType, valueForNote );
        showErrorNotification = ETrue;
        }

    // Possible DRM query is shown only if all files are not deleted.
    if ( iValidationResults[ESenduiIndexDeleted] <
         iValidationResults[ESenduiIndexTotalCount] )
        {
        if ( iValidationResults[ESenduiIndexDrmSD] )
            {
            ResolveDrmQuery( aServiceId, drmQueryTextId );
            showDrmQuery = ETrue;
            }
        }

    // Show error note if needed
    if ( showErrorNotification )
        {
        ShowErrorNotificationL( noteTextId, noteType, valueForNote );
        }

    // Show query if needed
    if ( showDrmQuery )
        {
        return ShowConfirmationQueryL(
            drmQueryTextId, R_SENDUI_DRM_FORWARDLOCKED_CONTENT_QUERY );
        }

    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::CalculateTotalSizeOfFiles
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSendUiFileRightsEngine::CalculateTotalSizeOfFiles(
    CArrayPtrFlat<CSendUiAttachment>* aAttachments,
    TBool aLinksSupported )
{
	if ( !aAttachments )
		{
		return 0;
		}
    TUint totalSize = 0;
  
    TInt index = 0;
    for ( index = 0; index < aAttachments->Count(); index++ )
        {
        CSendUiAttachment& attachment = *(aAttachments->At(index));
        
        if ( attachment.Type() == CSendUiAttachment::EAttachmentHandle )
        	{
        	totalSize += attachment.Size();
        	}
    	else
    		{
    		if ( !aLinksSupported )
    			{
	    		totalSize += attachment.Size();
    			}
    		}
        
        }
    return totalSize;
}

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ClearValidationCounters
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUiFileRightsEngine::ClearValidationCounters()
    {
    TInt index = 0;
    // Go through the lists
    for( index = 0; index < ESenduiIndexLastResult; index++ )
        {
        iValidationResults[index] = 0;
        }

    for( index = 0; index < ESenduiIndexLastInvalid; index++ )
        {
        iInvalidationReason[index] = 0;
        }
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::AnalyzeMmsValidationResult
// -----------------------------------------------------------------------------
// 
TBool CSendUiFileRightsEngine::AnalyzeMmsValidationResultL( CSendUiAttachment& aAttachment, TInt& aAttachmentsSize )
    {
    TBool attachmentInvalid = ETrue;
    
    RImplInfoPtrArray implArray;
    CleanupStack::PushL( TCleanupItem( CleanupImplArray, &implArray ) );
    CVideoConversionPlugin::ListImplementationsL( implArray );
    
    if ( aAttachment.MimeType() == KMsgMimeVideoMp4 && implArray.Count() != 0 )
        {
        attachmentInvalid = EFalse;
        }
    else if ( !iMmsConformance->CreationMode() &&
         !iMmsConformance->IsConformantMime( aAttachment.MimeType() ) )
        {
        // Restricted mode and non-conformant MIME
        iInvalidationReason[ESenduiIndexMmsNotSupported]++;
        }
    else if ( aAttachmentsSize + aAttachment.Size() > iMmsConformance->MaxSendSize() &&
              iMediaResolver->MediaType( aAttachment.MimeType() ) != EMsgMediaImage )
        {
        iInvalidationReason[ESenduiIndexMmsTooBig]++;
        }
    else
        {
        attachmentInvalid = EFalse;
        }

	if ( !attachmentInvalid
			&& iMediaResolver->MediaType( aAttachment.MimeType() ) != EMsgMediaImage )
	    {
	    aAttachmentsSize += aAttachment.Size();
	    }
    
    CleanupStack::PopAndDestroy( );    //implArray
    return attachmentInvalid;
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ResolveErrorNoteL
// -----------------------------------------------------------------------------
//
void CSendUiFileRightsEngine::ResolveErrorNoteL(
    TUid                    aServiceId,
    TUint&                  aResourceId,
    TFREErrorNoteType&      aNoteType,
    TUint&                  aValueForNote )
    {
    TInt i = 0;
    TInt errorTypeCount = 0;
    TUint errorType = 0;
    TUint totalFileCount = iValidationResults[ESenduiIndexTotalCount];
    TUint deletedFiles = iValidationResults[ESenduiIndexDeleted];

    // Own notes for MMS direct upload
    if ( aServiceId == KMmsDirectUpload )
        {
        ResolveMmsUploadErrorNoteL(
            aResourceId, aNoteType, aValueForNote );
        return;
        }

    // Count amount of different error types, if different errors exists
    // general error note is shown.
    for( i = 0; i < ESenduiIndexLastInvalid; i++ )
        {
        if ( iInvalidationReason[i] > 0 )
            {
            errorType = i;
            errorTypeCount++;
            }
        }

    if ( errorTypeCount == 0 )
        { // Should never end up here.
        aResourceId = R_SENDUI_MME_NONE_CANNOT_INS;
        return;
        }
    else if ( errorTypeCount == 1 )
        {  
        switch ( errorType )
            {
            case ESenduiIndexDrmFLOrCCL:
                {
                TUint protectedFiles = iInvalidationReason[ ESenduiIndexDrmFLOrCCL ];

                if ( deletedFiles < totalFileCount )
                    { // Some files are protected
                    aResourceId = R_SENDUI_DRM_SEND_FORBID_SOME;
                    }
                else
                    { // All files are protected
                    if ( protectedFiles == 1 )
                        { // One protected file
                        aResourceId = R_SENDUI_DRM_SEND_FORBID_ONE;
                        }
                    else
                        { // Many protected files
                        aResourceId = R_SENDUI_DRM_ALL_FILES_DELETED;
                        }
                    }
                }            
                break;
            case ESenduiIndexAlreadyInUse:
                {
                if ( deletedFiles == totalFileCount )
                    { // All files are in use
                    User::Leave( KErrInUse );
                    }
                else
                    { // Some files are in use
                    aResourceId = R_SENDUI_MME_SOME_INSERTED;
                    }
                }
                break;
            case ESenduiIndexMmsTooBig:
                {
                if ( deletedFiles == totalFileCount )
                    { // All files are too big
                    aResourceId = R_SENDUI_MME_ONE_TOO_BIG;
                    }
                else
                    { // Some files are too big
                    aResourceId = R_SENDUI_MME_MANY_TOO_BIG;
                    }

                aNoteType = EConfirmationQuery;
                aValueForNote = iMmsConformance->MaxSendSize();
                aValueForNote /= KFREKiloByte ;
                }
                break;
            case ESenduiIndexMmsNotSupported:
                {
                // Different notes when creation mode is locked
                if ( iMmsConformance->CreationModeUserChangeable() &&
                     !iMmsConformance->CreationMode() )
                    {
                    if ( deletedFiles == totalFileCount )
                        { // All files are not supported
                        aResourceId = deletedFiles > 1 ?
                            R_SENDUI_MME_RMODE_FILES_NOT_SUPPORTED :
                            R_SENDUI_MME_RMODE_FILE_NOT_SUPPORTED;
                        }
                    else
                        { // Some files are not supported
                        aResourceId = R_SENDUI_MME_RMODE_SOME_NOT_SUPPORTED;
                        }
                    }
                else
                    {
                    if ( deletedFiles == totalFileCount )
                        { // All files are not supported
                        aResourceId = deletedFiles > 1 ?
                            R_SENDUI_MME_FILES_NOT_SUPPORTED :
                            R_SENDUI_MME_FILE_NOT_SUPPORTED;
                        }
                    else
                        { // Some files are not supported
                        aResourceId = R_SENDUI_MME_SOME_FILES_DELETED;
                        }
                    }
                }
                break;
            case ESenduiIndexBadHandle:
            default:
                {
                aResourceId = R_SENDUI_MME_NONE_CANNOT_INS;
                }
                break;
            }
        }
    else // Different kind of errors exist, show general error note
        {
        if ( totalFileCount == deletedFiles )
            { // Can't send any file
            aResourceId = R_SENDUI_MME_NONE_CANNOT_INS;
            }
        else
            { // Some files can be send
            aResourceId = R_SENDUI_MME_SOME_INSERTED;
            }
        }
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ResolveMmsUploadErrorNoteL
// -----------------------------------------------------------------------------
//
void CSendUiFileRightsEngine::ResolveMmsUploadErrorNoteL(
    TUint&                      aResourceId,
    TFREErrorNoteType&          aNoteType,
    TUint&                      aValueForNote )
    {
    TInt i = 0;
    TInt errorTypeCount = 0;
    TUint errorType = 0;
    TUint totalFileCount = iValidationResults[ESenduiIndexTotalCount];
    TUint deletedFiles = iValidationResults[ESenduiIndexDeleted];

    // Count amount of different error types, if different errors exists
    // general error note is shown.
    for( i = 0; i < ESenduiIndexLastInvalid; i++ )
        {
        if ( iInvalidationReason[i] > 0 )
            {
            errorType = i;
            errorTypeCount++;
            }
        }

    if ( errorTypeCount == 0 )
        { // Should never end up here.
        return;
        }
    else if ( errorTypeCount == 1 )
        {  
        switch ( errorType )
            {
            case ESenduiIndexDrmFLOrCCL:
                {
                TUint protectedFiles = iInvalidationReason[ ESenduiIndexDrmFLOrCCL ];

                if ( deletedFiles < totalFileCount )
                    { // Some files are protected
                    aResourceId = R_SENDUI_DRM_SEND_FORBID_SOME;
                    }
                else
                    { // All files are protected
                    if ( protectedFiles == 1 )
                        { // One protected file
                        aResourceId = R_SENDUI_DRM_SEND_FORBID_ONE;
                        }
                    else
                        { // Many protected files
                        aResourceId = R_SENDUI_DRM_ALL_FILES_DELETED;
                        }
                    }
                }
                break;
            case ESenduiIndexAlreadyInUse:
                {
                if ( deletedFiles == totalFileCount )
                    { // All files are in use
                    User::Leave( KErrInUse );
                    }
                else
                    { // Some files are in use
                    aResourceId = R_SENDUI_MMS_UPLOAD_SOME_FILES_DELETED;
                    }
                }
                break;
            case ESenduiIndexMmsTooBig:
                {
                aResourceId = deletedFiles > 1 ?
                    R_SENDUI_MMS_UPLOAD_MANY_TOO_BIG : 
                    R_SENDUI_MMS_UPLOAD_ONE_TOO_BIG;

                aNoteType = EConfirmationQuery;
                aValueForNote = iMmsConformance->MaxSendSize();
                aValueForNote /= KFREKiloByte ;
                }
                break;
            case ESenduiIndexMmsNotSupported:
            default:
                {
                if ( deletedFiles == totalFileCount )
                    { // All files are not supported
                    aResourceId = deletedFiles > 1 ?
                        R_SENDUI_MMS_UPLOAD_FILES_NOT_SUPPORTED :
                        R_SENDUI_MMS_UPLOAD_FILE_NOT_SUPPORTED;
                    }
                else
                    { // Some files are not supported
                    aResourceId = R_SENDUI_MMS_UPLOAD_SOME_FILES_DELETED;
                    }
                }
                break;
            }
        }
    else // Different kind of errors exist, show general error note
        {
        if ( totalFileCount == deletedFiles )
            { // Can't send any file
            aResourceId = R_SENDUI_MMS_UPLOAD_NONE_CANNOT_INS;
            }
        else
            { // Some files can be send
            aResourceId = R_SENDUI_MMS_UPLOAD_SOME_INSERTED;
            }
        }   
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ResolveDrmQuery
// -----------------------------------------------------------------------------
//
void CSendUiFileRightsEngine::ResolveDrmQuery(
    TUid        aServiceId,
    TUint&      aResourceId )
    {
    TUint totalFileCount = iValidationResults[ESenduiIndexTotalCount];
    TUint drmProtectedFiles = iInvalidationReason[ ESenduiIndexDrmFLOrCCL ];
    TUint drmSdFiles = iValidationResults[ ESenduiIndexDrmSD ];
    TUint unprotectedFiles = totalFileCount - drmProtectedFiles - drmSdFiles;
    
    if ( unprotectedFiles > 0 )
        { // Some DRM SD protected files and unprotected files
        // Different query for direct MMS upload service
        if ( aServiceId == KMmsDirectUpload )
            {
            aResourceId = R_SENDUI_MMS_UPLOAD_DRM_SOME_SD;
            }
        else
            {
            aResourceId = R_SENDUI_DRM_SEVERAL_DISTRUTABLE;
            }
        }
    else
        { // Only DRM protected files
        // Different query for direct MMS upload service
        if ( aServiceId == KMmsDirectUpload || 
             aServiceId == KMmsIndirectUpload )
            {
            aResourceId = drmSdFiles > 1 ? 
                R_SENDUI_MMS_UPLOAD_DRM_MANY_SD : 
                R_SENDUI_MMS_UPLOAD_DRM_ONE_SD;
            }
        else
            {
            aResourceId = drmSdFiles > 1 ? 
                R_SENDUI_DRM_MANY_SD_ITEMS : 
                R_SENDUI_DRM_SINGLE_DISTRUTABLE;
            }
        }
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ShowErrorNotificationL
// -----------------------------------------------------------------------------
//
void CSendUiFileRightsEngine::ShowErrorNotificationL(
    TUint                   aResourceId,
    TFREErrorNoteType       aErrorNoteType, 
    TUint                   aValueForNote ) const
    {
    switch ( aErrorNoteType )
        {
        case EErrorNote:
            {
            ShowErrorNoteL( aResourceId );
            break;
            }
        case EInformationNote:
            {
            ShowInformationNoteL( aResourceId );
            break;
            }
        case EConfirmationQuery:
            {
            ShowConfirmationQueryL(
                aResourceId,
                R_SENDUI_CONFIRMABLE_INFO,
                aValueForNote );
            break;
            }
        default: // Shouldn't end up here
            break;
        }
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ShowErrorNoteL
// -----------------------------------------------------------------------------
//
void CSendUiFileRightsEngine::ShowErrorNoteL( TUint aTextResourceId ) const
    {
    // Get text to be shown
    HBufC* text = StringLoader::LoadLC( aTextResourceId );

    // Create the note and show it
    CAknErrorNote* note = new (ELeave) CAknErrorNote(ETrue);
    note->ExecuteLD( text->Des() );

    CleanupStack::PopAndDestroy( text ); // text
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ShowConfirmationQueryL
// -----------------------------------------------------------------------------
//
TBool CSendUiFileRightsEngine::ShowConfirmationQueryL( 
    TUint    aTextResourceId,
    TUint    aNoteResourceId,
    TUint    aValueForNote ) const
    {
    HBufC* text = NULL;
    
    if ( aValueForNote == 0 )
        {
        text = StringLoader::LoadLC( aTextResourceId );
        }
    else
        {
        text = StringLoader::LoadLC( aTextResourceId, aValueForNote );
        }

    CAknQueryDialog* dlg = CAknQueryDialog::NewL();

    TInt result = dlg->ExecuteLD( aNoteResourceId, *text );
    
    CleanupStack::PopAndDestroy( text );

    // ETrue, if Yes or Selection key were pressed
    // Otherwise EFalse
    return result; 
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::ShowInformationNoteL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUiFileRightsEngine::ShowInformationNoteL(
    TUint aTextResourceId ) const
    {
    // Get text to be shown
    HBufC* text = StringLoader::LoadLC( aTextResourceId );

    // Create the note and show it
    CAknInformationNote* note = new (ELeave) CAknInformationNote( ETrue );
    note->ExecuteLD( text->Des() );    

    CleanupStack::PopAndDestroy( text ); // text
    }

// -----------------------------------------------------------------------------
// CSendUiFileRightsEngine::CleanupImplArray
// -----------------------------------------------------------------------------
//
void CSendUiFileRightsEngine::CleanupImplArray( TAny* aAny )
    {
    RImplInfoPtrArray* implArray = 
        reinterpret_cast<RImplInfoPtrArray*>( aAny );
    implArray->ResetAndDestroy();
    implArray->Close();
    }


//  End of File  
