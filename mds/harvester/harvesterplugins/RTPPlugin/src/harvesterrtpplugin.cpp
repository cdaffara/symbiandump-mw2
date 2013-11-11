/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvests metadata from rtp video file 
*
*/

#include <e32std.h>
#include <apgcli.h> 

#include "harvesterdata.h"
#include "harvesterrtpplugin.h"
#include "harvesterlog.h"
#include "mdeobject.h"
#include "mdsutils.h"
#include "harvestercommon.h"
#include "mdeobjectwrapper.h"
#include "harvesterrtpmetadatareader.h"
#include <mdenamespacedef.h>
#include <mdeobjectdef.h>

// Supported mime type
_LIT( KRtpClipMimetype, "application/x-nokia-teh-rtp" );

// Enough size to recognize file and read metaheader
const TInt KFileBufferSize( KMaxMetaHeaderLength );

CHarvesterRtpPluginPropertyDefs::CHarvesterRtpPluginPropertyDefs() : CBase(),
    iCreationDatePropertyDef( NULL )
	{
	}

void CHarvesterRtpPluginPropertyDefs::ConstructL( CMdEObjectDef& aObjectDef )
    {
    SetByObjectDefL( aObjectDef );
    }

CHarvesterRtpPluginPropertyDefs* CHarvesterRtpPluginPropertyDefs::NewL()
    {
    CHarvesterRtpPluginPropertyDefs* self = 
        new (ELeave) CHarvesterRtpPluginPropertyDefs();
    return self;
    }

void CHarvesterRtpPluginPropertyDefs::SetByObjectDefL( CMdEObjectDef& aObjectDef )
	{
	CMdENamespaceDef& nsDef = aObjectDef.NamespaceDef();
	
	// Common property definitions
	CMdEObjectDef& objectDef = nsDef.GetObjectDefL( MdeConstants::Object::KBaseObject );
	iCreationDatePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KCreationDateProperty );
	iLastModifiedDatePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KLastModifiedDateProperty );
	iSizePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KSizeProperty );
	iItemTypePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
	iTitlePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
	iDefaultFolderPropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KInDefaultFolder );

	CMdEObjectDef& mediaDef = nsDef.GetObjectDefL( MdeConstants::MediaObject::KMediaObject );
	iCaptureDatePropertyDef =& mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KCaptureDateProperty );
	iDurationPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KDurationProperty );

	CMdEObjectDef& videoDef = nsDef.GetObjectDefL( MdeConstants::Video::KVideoObject );
	iAgeProfilePropertyDef = &videoDef.GetPropertyDefL( MdeConstants::Video::KAgeProfileProperty );
	iRecordingFlagsPropertyDef = &videoDef.GetPropertyDefL( MdeConstants::Video::KRecordingFlagsProperty );
	}

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CHarvesterRtpPlugin::CHarvesterRtpPlugin() : CHarvesterPlugin(), iPropDefs( NULL )
    {
    }

// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CHarvesterRtpPlugin::ConstructL()
    {
    WRITELOG( "CHarvesterRtpPlugin::ConstructL()" );
    SetPriority( KHarvesterPriorityHarvestingPlugin - 1 );
    iPropDefs = CHarvesterRtpPluginPropertyDefs::NewL();
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CHarvesterRtpPlugin* CHarvesterRtpPlugin::NewL()
    {
    CHarvesterRtpPlugin* self = new(ELeave) CHarvesterRtpPlugin();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CHarvesterRtpPlugin::~CHarvesterRtpPlugin()
    {
    WRITELOG( "CHarvesterRtpPlugin::~CHarvesterRtpPlugin()" );
    delete iPropDefs;
    iPropDefs = NULL;
    }

// ---------------------------------------------------------------------------
// Harvest file
// ---------------------------------------------------------------------------
//
void CHarvesterRtpPlugin::HarvestL( CHarvesterData* aHD )    
    {
    WRITELOG( "CHarvesterRtpPlugin::Harvest()" );
    CMdEObject& mdeObject = aHD->MdeObject();
    
    CHarvesterRtpClipDetails* clipDetails = CHarvesterRtpClipDetails::NewL();
    CleanupStack::PushL( clipDetails );
    
    TRAPD( error, GatherDataL( mdeObject, *clipDetails ) );
    if ( error == KErrNone || error == KErrCompletion )
    	{
    	TBool isNewObject( mdeObject.Id() == 0 );
        
        if ( isNewObject || mdeObject.Placeholder() )
            {
            TRAP( error, HandleObjectPropertiesL( *aHD, *clipDetails, ETrue ) );
            mdeObject.SetPlaceholder( EFalse );
            }
        else
            {
            TRAP( error, HandleObjectPropertiesL( *aHD, *clipDetails, EFalse ) );
            }

        if ( error != KErrNone )
            {
            WRITELOG1( "CHarvesterWMVPlugin::HarvestL() - Handling object failed: ", error );
            }
    	}
    else
        {
        TInt convertedError = KErrNone;
        MdsUtils::ConvertTrapError( error, convertedError );
        aHD->SetErrorCode( convertedError );
        }
    CleanupStack::PopAndDestroy( clipDetails );
    }

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::GetMimeType (from CHarvesterPlugin)
// ---------------------------------------------------------------------------
//    
void CHarvesterRtpPlugin::GetMimeType( const TDesC& /*aUri*/, TDes& aMimeType )
    {
    aMimeType = KRtpClipMimetype;
    }

// ---------------------------------------------------------------------------
// Gather metadata
// ---------------------------------------------------------------------------
//
void CHarvesterRtpPlugin::GatherDataL( CMdEObject& aMetadataObject, CHarvesterRtpClipDetails& aClipDetails )
    {
    WRITELOG( "CHarvesterRtpPlugin - GatherDataL()" );
    
    const TDesC& uri = aMetadataObject.Uri();

    HBufC8* buffer = HBufC8::NewLC( KFileBufferSize );
    TPtr8 bufferPtr( buffer->Des() );
    
    RFile64 file;
    CleanupClosePushL( file );

    User::LeaveIfError( file.Open( iFs, uri, EFileShareAny ) );
    
    TEntry* entry = new (ELeave) TEntry();
    CleanupStack::PushL( entry );
    User::LeaveIfError( iFs.Entry( uri, *entry ) );
    aClipDetails.iFileSize = (TUint)entry->iSize;
    aClipDetails.iModifiedDate = entry->iModified;
    CleanupStack::PopAndDestroy( entry ); // entry
    
    User::LeaveIfError( file.Read( bufferPtr, KFileBufferSize ) );
    User::LeaveIfError( RecognizeFile( uri, *buffer, aClipDetails ) );
    User::LeaveIfError( CheckIfMimeSupported( aClipDetails.iMimeType ) ); 

    CHarvesterRtpMetaDataReader* mdr = CHarvesterRtpMetaDataReader::NewL( buffer );
    CleanupStack::PushL( mdr );

    WRITELOG( "CHarvesterRtpPlugin - GatherDataL() ---> found metadata!" );
    mdr->GetClipDetailsL( aClipDetails );

    CleanupStack::PopAndDestroy( mdr ); // mdr
    CleanupStack::PopAndDestroy( &file );  // file
    CleanupStack::PopAndDestroy( buffer ); // buffer
    }

// -----------------------------------------------------------------------------
// Runs recognizers on the given file to determine its mime type
// -----------------------------------------------------------------------------
//
TInt CHarvesterRtpPlugin::RecognizeFile(
    const TDesC& aFilePath,
    const TDesC8& aFileBuffer,
    CHarvesterRtpClipDetails& aClipDetails )
    {
    WRITELOG( "CHarvesterRtpPlugin::RecognizeFile()" );
    TInt err( KErrNone );
    
    RApaLsSession apaSession;
    err = apaSession.Connect();
    if ( err == KErrNone )
        {
        TDataRecognitionResult result;
        err = apaSession.RecognizeData( aFilePath, aFileBuffer, result );
        if ( result.iConfidence >= CApaDataRecognizerType::EProbable )
            {
            aClipDetails.iMimeType = result.iDataType.Des();
            WRITELOG1( "CHarvesterRtpPlugin - Mime type: %S", &aClipDetails.iMimeType );
            }

        apaSession.Close();
        }
    
    return err;
    }

///---------------------------------------------------------------------------
/// Check if mime type is supported
///---------------------------------------------------------------------------
//
TInt CHarvesterRtpPlugin::CheckIfMimeSupported( const TDesC& aMimeBuf )
    {
    if ( MdsUtils::Compare( KRtpClipMimetype, aMimeBuf ) == 0 )
        {
        WRITELOG( "CHarvesterRtpPlugin - Mime type supported");
        return KErrNone;
        }
    
    WRITELOG( "CHarvesterRtpPlugin - Mime type not supported");
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// Add object properties
// ---------------------------------------------------------------------------
//
void CHarvesterRtpPlugin::HandleObjectPropertiesL( 
    CHarvesterData& aHD,
    CHarvesterRtpClipDetails& aClipDetails,
    TBool aIsAdd)
    {
    WRITELOG( "CHarvesterRtpPlugin::HandleObjectPropertiesL()" );

    CMdEObject& mdeObject = aHD.MdeObject();
     
    InitPropDefsL( mdeObject.Def() );

    TTimeIntervalSeconds timeOffset = User::UTCOffset();
    TTime localModifiedTime = aClipDetails.iModifiedDate + timeOffset;
    
    if( ! mdeObject.Placeholder() )
    	{
    	// Creation date of media file
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
    			*iPropDefs->iCreationDatePropertyDef, &localModifiedTime, aIsAdd );
    	// Last modified date
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
    			*iPropDefs->iLastModifiedDatePropertyDef, &aClipDetails.iModifiedDate, aIsAdd );
    	// File size
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
    			*iPropDefs->iSizePropertyDef, &aClipDetails.iFileSize, aIsAdd );
        // Mime Type
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
                *iPropDefs->iItemTypePropertyDef, &aClipDetails.iMimeType, aIsAdd );
        // Default folder
        TBool inDefaultFolder( ETrue );
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
                *iPropDefs->iDefaultFolderPropertyDef, &inDefaultFolder, aIsAdd );
    	}

    // Title (is set from URI by default)
    if ( aClipDetails.iTitle.Length() > 0 ) 
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
        		*iPropDefs->iTitlePropertyDef, &aClipDetails.iTitle, EFalse );
        }

    // Capture date
    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
    		*iPropDefs->iCaptureDatePropertyDef, &localModifiedTime, aIsAdd );    

   	// Duration
    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
    		*iPropDefs->iDurationPropertyDef, &aClipDetails.iDuration, aIsAdd );

    //Save these if video center videostorage available
    //AgeProfile
    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
    		*iPropDefs->iAgeProfilePropertyDef, &aClipDetails.iParental, aIsAdd );
    
    //VideoFlags    
    TUint32 flags = 0;

    //always set clip type as recording
    flags |= EIptvMyVideosVideoTypeRecording;

    if ( aClipDetails.iRecOngoing )
        {
        //Recording ongoing 
        //Otherwise assuming that recording has completed
        flags |= EIptvMyVideosVideoIsRecording;
        }        

    if ( aClipDetails.iRecFailed )
        {
        //Recording failed
        flags |= EIptvMyVideosVideoRecFailed;
        }

    if ( aClipDetails.iQuality <= KPartiallyFailedLimit )
        {
        //Clip partially failed
        flags |= EIptvMyVideosVideoRecPartiallyFailed;
        }

    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
    		*iPropDefs->iRecordingFlagsPropertyDef, &flags, aIsAdd );
    }

void CHarvesterRtpPlugin::InitPropDefsL( CMdEObjectDef& aObjectDef )
    {
    if( !iPropDefs->iCreationDatePropertyDef )
        {
        iPropDefs->SetByObjectDefL( aObjectDef );
        }
    }


