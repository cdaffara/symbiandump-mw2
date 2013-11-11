/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvests meta data from video file.*
*/


#include <e32base.h>
#include <e32std.h>
#include <3gplibrary/mp4lib.h>
#include <hxmetadatautil.h>
#include <hxmetadatakeys.h>
#include <caf/caf.h>
#include <pathinfo.h>

#include "mdsutils.h"
#include "harvestervideoplugin.h"
#include "harvesterlog.h"
#include "harvesterblacklist.h"
#include "mdeobjectwrapper.h"
#include "mdscommoninternal.h"

#include <mdenamespacedef.h>
#include <mdeobjectdef.h>
#include <mdepropertydef.h>
#include <mdeobject.h>
#include <harvesterdata.h>
#include <mdeconstants.h>

using namespace MdeConstants;

_LIT( KMimeTypeVideoMp4,  "video/mp4" );
_LIT( KMimeTypeAudioMp4,  "audio/mp4" );
_LIT( KMimeTypeVideo3gpp, "video/3gpp" );
_LIT( KMimeTypeAudio3gpp, "audio/3gpp" );
_LIT( KMimeTypeRm,        "application/vnd.rn-realmedia" ); // can be audio or video
_LIT( KMimeTypeRmvb,      "application/vnd.rn-realmedia-vbr" ); // can be audio or video
_LIT( KMimeTypeRv,        "video/vnd.rn-realvideo" );
_LIT( KMimeTypeRa,        "audio/vnd.rn-realaudio" );
_LIT( KMimeTypeAvi,        "video/avi");
_LIT( KMimeTypeVideoMatroska, "video/x-matroska");
_LIT( KMimeTypeAudioMatroska, "audio/x-matroska");
_LIT( KMimeTypeWmv, "video/x-ms-wmv");
_LIT( KMimeTypeDivx, "video/x-hx-divx");
_LIT( KMimeTypeAsf, "video/x-ms-asf");

_LIT( KExtensionMp4,   "mp4" );
_LIT( KExtensionMpg4,  "mpg4" );
_LIT( KExtensionMpeg4, "mpeg4" );
_LIT( KExtensionM4v,   "m4v" );
_LIT( KExtensionM4a,   "m4a" );
_LIT( KExtension3gp,   "3gp" );
_LIT( KExtension3gpp,  "3gpp" );
_LIT( KExtension3g2,   "3g2" );
_LIT( KExtensionRm,    "rm" );
_LIT( KExtensionRmvb,  "rmvb" );
_LIT( KExtensionRv,    "rv" );
_LIT( KExtensionAvi,    "avi" );
_LIT( KExtensionMkv,    "mkv" );
_LIT( KExtensionRa,     "ra" );
_LIT( KExtensionWmv,     "wmv" );
_LIT( KExtensionDivx,     "divx" );
_LIT( KExtensionAsf,     "asf" );

_LIT(KVideo, "Video");
_LIT(KAudio, "Audio");

_LIT(KAudioAC3, "audio/AC3");
_LIT(KAudioEAC3, "audio/EAC3");
const TUint32 KMDSFourCCCodeAC3 = 0x33434120;       //{' ', 'A', 'C', '3'}
const TUint32 KMDSFourCCCodeEAC3 = 0x33434145;      //{'E', 'A', 'C', '3'}

_LIT(KInUse, "InUse");

const TInt KKiloBytes = 1024;
const TReal32 KThousandReal = 1000.0;

CHarvesterVideoPluginPropertyDefs::CHarvesterVideoPluginPropertyDefs() : CBase(),
    iCreationDatePropertyDef( NULL )
	{
	}

void CHarvesterVideoPluginPropertyDefs::ConstructL(CMdEObjectDef& aObjectDef)
	{
    SetByObjectDefL( aObjectDef );
	}

CHarvesterVideoPluginPropertyDefs* CHarvesterVideoPluginPropertyDefs::NewL()
    {
    CHarvesterVideoPluginPropertyDefs* self = 
        new (ELeave) CHarvesterVideoPluginPropertyDefs();
    return self;
    }

void CHarvesterVideoPluginPropertyDefs::SetByObjectDefL( CMdEObjectDef& aObjectDef )
    {
    CMdENamespaceDef& nsDef = aObjectDef.NamespaceDef();
    
    // Common property definitions
    CMdEObjectDef& objectDef = nsDef.GetObjectDefL( Object::KBaseObject );
    iCreationDatePropertyDef = &objectDef.GetPropertyDefL( Object::KCreationDateProperty );
    iLastModifiedDatePropertyDef = &objectDef.GetPropertyDefL( Object::KLastModifiedDateProperty );
    iSizePropertyDef = &objectDef.GetPropertyDefL( Object::KSizeProperty );
    iTimeOffsetPropertyDef = &objectDef.GetPropertyDefL( Object::KTimeOffsetProperty );
    iItemTypePropertyDef = &objectDef.GetPropertyDefL( Object::KItemTypeProperty );
    iTitlePropertyDef = &objectDef.GetPropertyDefL( Object::KTitleProperty );
    iDefaultFolderPropertyDef = &objectDef.GetPropertyDefL( Object::KInDefaultFolder );

    CMdEObjectDef& mediaDef = nsDef.GetObjectDefL( MediaObject::KMediaObject );
    iReleaseDatePropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KReleaseDateProperty );
    iCaptureDatePropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KCaptureDateProperty );
    iDurationPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KDurationProperty );
    iWidthPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KWidthProperty );
    iHeightPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KHeightProperty );
    iBitratePropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KBitrateProperty );
    iCopyrightPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KCopyrightProperty );
    iAuthorPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KAuthorProperty );
    iGenrePropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KGenreProperty );
    iArtistPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KArtistProperty );
    iDescriptionPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KDescriptionProperty );
    iDrmPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KDRMProperty );
    
    iAudioFourCCDef = &mediaDef.GetPropertyDefL( MediaObject::KAudioFourCCProperty );

    // Video property definitions
    CMdEObjectDef& videoDef = nsDef.GetObjectDefL( Video::KVideoObject );
    iFrameratePropertyDef = &videoDef.GetPropertyDefL( Video::KFramerateProperty );

    // Audio property definitions
    CMdEObjectDef& audioDef = nsDef.GetObjectDefL( Audio::KAudioObject );
    iSamplingFrequencyPropertyDef = &audioDef.GetPropertyDefL( Audio::KSamplingFrequencyProperty );
    }

/**
* Default constructor
*/
CHarvesterVideoPlugin::CHarvesterVideoPlugin() : CHarvesterPlugin(), iPropDefs( NULL )
	{
	}

/**
* Construction
* @return Harvester image plugin
*/
CHarvesterVideoPlugin* CHarvesterVideoPlugin::NewL()
	{
	WRITELOG("CHarvesterVideoPlugin::NewL()");
	CHarvesterVideoPlugin* self = new(ELeave) CHarvesterVideoPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
* Destruction
*/
CHarvesterVideoPlugin::~CHarvesterVideoPlugin()
	{
	delete iPropDefs;
	iPropDefs = NULL;
	iMimeTypeMappings.Close();
    RMediaIdUtil::ReleaseInstance();

    delete iPhoneVideosPath;
    iPhoneVideosPath = NULL;
    delete iMmcVideosPath;
    iMmcVideosPath = NULL;
    
	WRITELOG("CHarvesterVideoPlugin::CHarvesterVideoPlugin()");
	}

/**
* 2nd phase constructor
*/
void CHarvesterVideoPlugin::ConstructL()
	{
	WRITELOG( "CHarvesterVideoPlugin::ConstructL() - begin" );
	
	iPropDefs = CHarvesterVideoPluginPropertyDefs::NewL();
	
	TLinearOrder< THarvestingHandling > cmp( THarvestingHandling::CompareFunction );

	// MPEG4
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtensionMp4(), KNullDesC(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KNullDesC(),
    				KMimeTypeVideoMp4(), KMimeTypeAudioMp4() ) ), 
    		cmp ) );
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtensionMpg4(), KNullDesC(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KNullDesC(),
    				KMimeTypeVideoMp4(), KMimeTypeAudioMp4() ) ), 
    		cmp ) );
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtensionMpeg4(), KNullDesC(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KNullDesC(),
    				KMimeTypeVideoMp4(), KMimeTypeAudioMp4() ) ), 
    		cmp ) );
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtensionM4v(), KMimeTypeVideoMp4(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KVideo(),
    				KMimeTypeVideoMp4(), KMimeTypeAudioMp4() ) ), 
    		cmp ) );
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtensionM4a(), KMimeTypeAudioMp4(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KAudio(),
    				KMimeTypeVideoMp4(), KMimeTypeAudioMp4() ) ), 
    		cmp ) );

	// 3GP
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtension3gp(), KNullDesC(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KNullDesC(),
    				KMimeTypeVideo3gpp(), KMimeTypeAudio3gpp() ) ), 
    		cmp ) );
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtension3gpp(), KNullDesC(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KNullDesC(),
    				KMimeTypeVideo3gpp(), KMimeTypeAudio3gpp() ) ), 
    		cmp ) );
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtension3g2(), KNullDesC(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KNullDesC(),
    				KMimeTypeVideo3gpp(), KMimeTypeAudio3gpp() ) ), 
    		cmp ) );

	// RealMedia
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtensionRm(), KNullDesC(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KNullDesC(),
    				KMimeTypeRm(), KMimeTypeRm() ) ), 
    		cmp ) );
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtensionRmvb(), KNullDesC(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KNullDesC(),
    				KMimeTypeRmvb(), KMimeTypeRmvb() ) ), 
    		cmp ) );
	User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
    		KExtensionRv(), KMimeTypeRv(), 
    		TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KVideo(),
    				KMimeTypeRv(), KMimeTypeRa() ) ), 
    		cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
            KExtensionRa(), KMimeTypeRa(), 
            TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KAudio(),
                    KMimeTypeRv(), KMimeTypeRa() ) ), 
            cmp ) );
    
    // Avi
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
            KExtensionAvi(), KMimeTypeAvi(), 
            TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KVideo(),
                    KMimeTypeAvi(), KMimeTypeAvi() ) ), 
            cmp ) );
    
    // Divx
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
            KExtensionDivx(), KMimeTypeDivx(), 
            TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KVideo(),
                    KMimeTypeDivx(), KMimeTypeDivx() ) ), 
            cmp ) );
    
    // Matroska
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
            KExtensionMkv(), KNullDesC(), 
            TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KNullDesC(),
                    KMimeTypeVideoMatroska(), KMimeTypeAudioMatroska() ) ), 
            cmp ) );
    
    // Wmv
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
            KExtensionWmv(), KMimeTypeWmv(), 
            TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KVideo(),
                    KMimeTypeWmv(), KMimeTypeWmv() ) ), 
            cmp ) );

    // Asf
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( THarvestingHandling(
            KExtensionAsf(), KMimeTypeAsf(), 
            TVideoMetadataHandling( TVideoMetadataHandling::EHexilMetadataHandling, KVideo(),
                    KMimeTypeAsf(), KMimeTypeAsf() ) ), 
            cmp ) );

    TFileName videos = PathInfo::VideosPath();
    
    TFileName phonePath = PathInfo::PhoneMemoryRootPath();
    phonePath.Append( videos );
    iPhoneVideosPath = phonePath.AllocL();

    TFileName mmcPath = PathInfo::MemoryCardRootPath();
    mmcPath.Append( videos );
    iMmcVideosPath = mmcPath.Right( mmcPath.Length() - 1 ).AllocL();
    
    iMediaIdUtil = &RMediaIdUtil::GetInstanceL();
    }


// ---------------------------------------------------------------------------
// GetObjectType
// ---------------------------------------------------------------------------
//
void CHarvesterVideoPlugin::GetObjectType( const TDesC& aUri, TDes& aObjectType )
	{
	aObjectType.Zero();
	
	const THarvestingHandling* mapping = FindHandler( aUri );

	// no matching extension found
	if( !mapping )
		{
		return;
		}

	const TVideoMetadataHandling& handler = mapping->iHandler;
	
    // object def is defined by extension
    if( handler.iObjectDef.Length() > 0 )
    	{
    	aObjectType.Copy( handler.iObjectDef );
    	return;
    	}

    RFile64 file;
    // File must not be allowed to be written to here. If file passes through 
    // here while it is still open for writing, it will fail in actual harvesting
    // and will not be processed again until the file is modified
    const TInt error = file.Open( iFs, aUri, EFileRead | EFileShareReadersOnly  );
    if ( error != KErrNone )
        {
        WRITELOG1( "CHarvesterVideoPlugin::GetObjectType - File open error: %d", error );
        if( error == KErrInUse || KErrLocked )
            {
#ifdef _DEBUG
            TPtrC fileName( aUri.Mid(2) );
            WRITELOG1( "CHarvesterVideoPlugin :: Checking open file handles to %S", &fileName );

            CFileList* fileList = 0;
            TOpenFileScan fileScan( iFs );

            TRAP_IGNORE( fileScan.NextL( fileList ) );   
  
            while ( fileList )   
                {
                const TInt count( fileList->Count() ); 
                for (TInt i = 0; i < count; i++ )   
                    {   
                    if ( (*fileList)[i].iName == aUri.Mid(2) )
                        {
                        TFullName processName;
                        TFindThread find(_L("*"));
                        while( find.Next( processName ) == KErrNone )
                            {
                            RThread thread;
                            TInt err = thread.Open( processName );
     
                            if ( err == KErrNone )
                                {
                                if ( thread.Id().Id() ==  fileScan.ThreadId() )
                                    {
                                    processName = thread.Name();
                                    thread.Close();
                                    WRITELOG1( "CHarvesterVideoPlugin:: %S has a file handle open", &processName );
                                    break;
                                    }
                                thread.Close();
                                }
                            }
                        }
                    }
                fileList = NULL;
                TRAP_IGNORE( fileScan.NextL( fileList ) );   
                } 
#endif
            aObjectType.Copy( KInUse() );
            }
        return;
        }

    if( handler.iLibrary == TVideoMetadataHandling::EMp4LibHandling )
    	{
    	GetMp4Type( file, aObjectType );
    	}
    else if( handler.iLibrary == TVideoMetadataHandling::EHexilMetadataHandling )
    	{
    	TRAP_IGNORE( GetRmTypeL( file, aObjectType ) );
    	}
    else
    	{
    	WRITELOG1( "CHarvesterVideoPlugin::GetObjectType - ERROR: %S. No handling library found", &aUri );
    	}

    file.Close();
	}

void CHarvesterVideoPlugin::HarvestL( CHarvesterData* aHD )
	{
    CMdEObject& mdeObject = aHD->MdeObject();
    CVideoHarvestData* fileData = new (ELeave) CVideoHarvestData;
    CleanupStack::PushL( fileData );
    
#ifdef _DEBUG    
    WRITELOG1("CHarvesterVideoPlugin::HarvestL - aHD->Uri() %S", &aHD->Uri() );
    WRITELOG1("CHarvesterVideoPlugin::HarvestL - mdeObject.Uri() %S", &mdeObject.Uri() );
#endif
    
    TRAPD( error, GatherDataL( mdeObject, *fileData ) );
    if ( error == KErrNone || error == KErrCompletion )
    	{
        TBool isNewObject( mdeObject.Id() == 0 );
        
        if ( isNewObject || mdeObject.Placeholder() )
            {
            TRAP_IGNORE( HandleObjectPropertiesL( *aHD, *fileData, ETrue ) );
            mdeObject.SetPlaceholder( EFalse );
            }
        else
            {
            TRAP_IGNORE( HandleObjectPropertiesL( *aHD, *fileData, EFalse ) );
            }
    	}
    else
        {
        WRITELOG1( "CHarvesterVideoPlugin::HarvestL() - TRAP error: %d", error );
        TInt convertedError = KErrNone;
        MdsUtils::ConvertTrapError( error, convertedError );
        aHD->SetErrorCode( convertedError );
        }

    CleanupStack::PopAndDestroy( fileData );
	}

// ---------------------------------------------------------------------------
// CHarvesterVideoPlugin::GetMimeType (from CHarvesterPlugin)
// ---------------------------------------------------------------------------
//    
void CHarvesterVideoPlugin::GetMimeType( const TDesC& aUri, TDes& aMimeType )
    {
    aMimeType.Zero();
    
    const THarvestingHandling* mapping = FindHandler( aUri );

    // no matching extension found
    if( !mapping )
        {
        return;
        }

    aMimeType = mapping->iMimeType;
    }

// ---------------------------------------------------------------------------
// GatherDataL
// ---------------------------------------------------------------------------
//
void CHarvesterVideoPlugin::GatherDataL( CMdEObject& aMetadataObject,
		CVideoHarvestData& aVHD )
    {
#ifdef _DEBUG
    TTime dStart, dStop;
    dStart.UniversalTime();
    dStop.UniversalTime();
    WRITELOG1( "CHarvesterVideoPlugin::GatherDataL start %d us", (TInt)dStop.MicroSecondsFrom(dStart).Int64() );
#endif
    const TDesC& uri = aMetadataObject.Uri();
    
    WRITELOG1( "CHarvesterVideoPlugin - Gather data from file %S", &uri );
    
    RFile64 file;
    TInt error = file.Open( iFs, uri, EFileRead | EFileShareReadersOnly );
    CleanupClosePushL( file );
    if ( error == KErrInUse ||
         error == KErrLocked )
        {
        WRITELOG( "CHarvesterVideoPlugin - File is open!" );
#ifdef _DEBUG
            TPtrC fileName( uri.Mid(2) );
            WRITELOG1( "CHarvesterVideoPlugin :: Checking open file handles to %S", &fileName );

            CFileList* fileList = 0;
            TOpenFileScan fileScan( iFs );

            fileScan.NextL( fileList );   
  
            while ( fileList )   
                {
                const TInt count( fileList->Count() ); 
                for (TInt i = 0; i < count; i++ )   
                    {   
                    if ( (*fileList)[i].iName == uri.Mid(2) )
                        {
                        TFullName processName;
                        TFindThread find(_L("*"));
                        while( find.Next( processName ) == KErrNone )
                            {
                            RThread thread;
                            TInt err = thread.Open( processName );
     
                            if ( err == KErrNone )
                                {
                                if ( thread.Id().Id() ==  fileScan.ThreadId() )
                                    {
                                    processName = thread.Name();
                                    thread.Close();
                                    WRITELOG1( "CHarvesterVideoPlugin:: %S has a file handle open", &processName );
                                    break;
                                    }
                                thread.Close();
                                }
                            }
                        }
                    }
                fileScan.NextL( fileList );   
                } 
#endif
        CleanupStack::PopAndDestroy( &file );
        User::Leave( KErrInUse );
        }
    else if( error == KErrNotFound ||
                error == KErrPathNotFound ||
                error == KErrDisMounted ||
                error == KErrBadDescriptor )
        {
        WRITELOG1( "CHarvesterVideoPlugin - File open error: %d", error );
        CleanupStack::PopAndDestroy( &file );
        User::Leave( error );
        }
    
    TBool dataExtracted( aMetadataObject.Id() == 0 || aMetadataObject.Placeholder() );
    
    if( dataExtracted )
        {
        CMdEProperty* prop = NULL;
        CMdEObjectDef& objectDef = aMetadataObject.Def();
        CMdEPropertyDef& modifiedDef = objectDef.GetPropertyDefL( MdeConstants::Object::KLastModifiedDateProperty );
        aMetadataObject.Property( modifiedDef, prop );
        if( prop )
            {
            aVHD.iModified  = prop->TimeValueL();
            }
        else
            {
            dataExtracted = EFalse;
            }
        }
    
    if( !dataExtracted )
        {
        // If file could be opened, use file handle to fetch base data, otherwise
        // attempt to fetch the data from TEntry 
        if( error == KErrNone )
            {
            User::LeaveIfError( file.Modified( aVHD.iModified ) );
            User::LeaveIfError( file.Size( aVHD.iFileSize ) );            
            }
        else
            {
            TEntry entry;
            const TInt errorcode = iFs.Entry( uri, entry );
        
            if ( errorcode != KErrNone )
                {
                WRITELOG1( "CHarvesterVideoPlugin - Error getting entry: %d", errorcode );
                CleanupStack::PopAndDestroy( &file );
                User::Leave( errorcode );
                }
        
            aVHD.iModified = entry.iModified;
            aVHD.iFileSize = (TUint)entry.iSize;        
            }
        
        WRITELOG1( "CHarvesterVideoPlugin - File size: %d", aVHD.iFileSize );
        }

    aVHD.iVideoObject = aMetadataObject.Def().Name().Compare( KVideo ) == 0;

    if( error != KErrNone )
        {
        WRITELOG1( "CHarvesterVideoPlugin - File open error: %d", error );
        CleanupStack::PopAndDestroy( &file );
        User::Leave( KErrCompletion );
        }
    
    // now the minimum information has been harvested
    // from now on the harvested data should always be stored

    const THarvestingHandling* mapping = FindHandler( uri );
    
    if( !mapping )
    	{
    	CleanupStack::PopAndDestroy( &file );
    	User::Leave( KErrNotFound );
    	}
    
    if ( mapping->iHandler.iLibrary == TVideoMetadataHandling::EHexilMetadataHandling )
    	{
    	// doesn't own pointers to MIME types
    	RPointerArray<HBufC> mimes;
    	CleanupClosePushL( mimes );

        TPtrC ext;
        const TBool exists = MdsUtils::GetExt( uri, ext );
        
        // Check for possibly protected content
        if( exists && ext.CompareF( KExtensionWmv ) == 0 )
            {
            ContentAccess::CContent* content = ContentAccess::CContent::NewLC( uri );
            ContentAccess::CData* data = content->OpenContentLC( ContentAccess::EPeek );
            
            data->GetAttribute( ContentAccess::EIsProtected, aVHD.iDrmProtected );
            CleanupStack::PopAndDestroy( 2 ); // content, data
            }
    	
    	CHXMetaDataUtility* helixMetadata = CHXMetaDataUtility::NewL();
        CleanupStack::PushL( helixMetadata );
        
        TRAP( error, helixMetadata->OpenFileL( file ) );        

        // No need for the file handle anymore so closing it
        WRITELOG( "CHarvesterVideoPlugin - Parsing done, file handle can be closed" );   
        file.Close();
        
        if ( error == KErrNone )
        	{
        	HBufC *buf = NULL;
        	HXMetaDataKeys::EHXMetaDataId metaid;        	
        	TUint metacount = 0;
        	helixMetadata->GetMetaDataCount( metacount );
        	TLex lex;
        	for ( TUint i = 0; i < metacount; i++ )
        		{        		
        		helixMetadata->GetMetaDataAt( i, metaid, buf );
        		switch (metaid)
        			{
                    case HXMetaDataKeys::EHXTitle:
                        {
                        aVHD.iTitle = buf->Alloc();
                        break;
                        }
        			case HXMetaDataKeys::EHXVideoBitRate:
	        			{
        				WRITELOG( "CHarvesterVideoPlugin - found videobitrate" );
	        			if( aVHD.iVideoObject )
	        				{
	        				lex.Assign( *buf );
	        				if( KErrNone == lex.Val( aVHD.iVideoBitrate ) )
	        					{
	        					aVHD.iVideoBitrate /= KKiloBytes;
	        					}
	        				}
	        			break;
	        			}
        			case HXMetaDataKeys::EHXAudioBitRate:
	        			{
	        			WRITELOG( "CHarvesterVideoPlugin - found audiobitrate" );
	        			lex.Assign( *buf );
	        			if( KErrNone == lex.Val( aVHD.iAudioBitrate ) )
	        				{
	        				aVHD.iAudioBitrate /= KKiloBytes;
	        				}
	        			break;
	        			}
        			case HXMetaDataKeys::EHXClipBitRate:
        				{
        				WRITELOG( "CHarvesterVideoPlugin - found clipbitrate" );
	        			lex.Assign( *buf );
	        			if( KErrNone == lex.Val( aVHD.iClipBitrate ) )
							{
							aVHD.iClipBitrate /= KKiloBytes;
							}
        				break;
        				}
        			case HXMetaDataKeys::EHXDuration:
	        			{
	        			WRITELOG( "CHarvesterVideoPlugin - found duration" );
	        			lex.Assign(*buf);
	        			if( KErrNone == lex.Val( aVHD.iDuration ) )
							{
							aVHD.iDuration /= KThousandReal;
							}
						break;
	        			}
        			case HXMetaDataKeys::EHXFramesPerSecond:
	        			{
	        			WRITELOG( "CHarvesterVideoPlugin - found framerate" );
	        			lex.Assign( *buf );
	        			lex.Val( aVHD.iFrameRate );
	        			break;
	        			}
        			case HXMetaDataKeys::EHXCopyright:
	        			{
	        			aVHD.iCopyright = buf->Alloc();
	        			break;
	        			}
        			case HXMetaDataKeys::EHXAuthor:
	        			{
	        			aVHD.iAuthor = buf->Alloc();
	        			break;
	        			}
        			case HXMetaDataKeys::EHXGenre:
	        			{
	        			aVHD.iGenre = buf->Alloc();
	        			break;
	        			}
        			case HXMetaDataKeys::EHXPerformer:
	        			{
	        			aVHD.iPerformer = buf->Alloc();
	        			break;
	        			}
        			case HXMetaDataKeys::EHXDescription:
	        			{
	        			aVHD.iDescription = buf->Alloc();
	        			break;
	        			}
        			case HXMetaDataKeys::EHXMimeType:
	        			{
	        			mimes.AppendL( buf );
	        			if( aVHD.iCodec == 0 )
	        			    {
	        			    CheckForCodecSupport( buf, aVHD );
	        			    }
	        			break;
	        			}
                    case HXMetaDataKeys::EHXFrameSize:
                        {
                        const TChar separator = 'x';    // as in e.g."177x144"
                        const TInt separatorLocation = buf->Locate( separator );
                        TLex input( buf->Left( separatorLocation ) );

                        input.Val( aVHD.iFrameWidth );
                        input = buf->Right(buf->Length() - separatorLocation - 1);
                        input.Val( aVHD.iFrameHeight );
	        			break;
	        			}
        			default:
        				break;
        			}
        		}
        	}

        const TInt mimeCount = mimes.Count();
        
        TPtrC mime( NULL, 0 );

        // if metadata didn't contain MIME, get it from extension mapping
        if( mimeCount == 0 )
        	{
        	if( aVHD.iVideoObject )
        		{
        		mime.Set( mapping->iHandler.iVideoMime.Ptr(),
        				mapping->iHandler.iVideoMime.Length() );
        		}
        	else
        		{
        		mime.Set( mapping->iHandler.iAudioMime.Ptr(),
        				mapping->iHandler.iAudioMime.Length() );
        		}
        	}
        // otherwise search from MIME type array
        else
        	{
	        for( TInt i = 0; i < mimeCount; i++ )
	        	{
	        	HBufC* mimeTmp = mimes[i];
	        	
	        	if( !mimeTmp )
	        		{
	        		continue;
	        		}
	        	
	        	mime.Set( mimeTmp->Des().Ptr(), mimeTmp->Des().Length() );

	        	// MIME contains substring "application/vnd.rn-realmedia".
	        	// That case MIME matches also with 
	        	// string "application/vnd.rn-realmedia-vbr".
	        	if( MdsUtils::Find( mime, KMimeTypeRm() ) != KErrNotFound )
	        		{
	        		break;
	        		}
	        	// Match MIME type, for video object with "video" substring
	        	else if( aVHD.iVideoObject )
	        		{
	        		if( MdsUtils::Find( mime, KVideo() ) != KErrNotFound )
		        		{
		        		break;
		        		}
	        		}
	        	// Match MIME type for audio object with "audio" substring
	        	else if( MdsUtils::Find( mime, KAudio() ) != KErrNotFound )
	        		{
	        	    if( !aVHD.iVideoObject )
	        	        {
	        	        break;
	        	        }
	        		}
	        	}
	        }
        
        if( mime.Ptr() && ( mime.Length() > 0 ) )
        	{
        	aVHD.iMimeBuf = mime.Alloc();
        	}
        
        helixMetadata->ResetL();
        CleanupStack::PopAndDestroy( helixMetadata );
        
        // don't destory mime type pointers just clean array
        CleanupStack::PopAndDestroy( &mimes );
        
        // If parsing failed, check for possible protected content
        if( error == KErrNotSupported || 
            error == KErrAccessDenied ||
            error == KErrPermissionDenied )
            {
            ContentAccess::CContent* content = ContentAccess::CContent::NewLC( uri );
            ContentAccess::CData* data = content->OpenContentLC( ContentAccess::EPeek );
        
            if( !aVHD.iDrmProtected )
                {
                data->GetAttribute( ContentAccess::EIsProtected, aVHD.iDrmProtected );
                }
            if( aVHD.iDrmProtected )
                {
                ContentAccess::RStringAttributeSet attrSet;
                CleanupClosePushL( attrSet );
                
                attrSet.AddL( ContentAccess::EDescription );
                attrSet.AddL( ContentAccess::ETitle );
                attrSet.AddL( ContentAccess::EAuthor );
                attrSet.AddL( ContentAccess::EGenre );

                if( data->GetStringAttributeSet(attrSet) == KErrNone )
                    {
                    TBuf<KMaxDataTypeLength> value;
                    
                    TInt err = attrSet.GetValue( ContentAccess::EDescription, value );
                    if ( err != KErrNone)
                        {
                        WRITELOG1( "CHarvesterVideoPlugin::GatherDataL - ERROR: getting description failed %d", err );
                        }
                    else if( value.Length() > 0 )
                        {
                        aVHD.iDescription = value.Alloc();
                        }
                
                    err = attrSet.GetValue( ContentAccess::ETitle, value );
                    if ( err != KErrNone)
                        {
                        WRITELOG1( "CHarvesterVideoPlugin::GatherDataL - ERROR: getting title failed %d", err );
                        }
                    else if( value.Length() > 0 )
                        {
                        aVHD.iTitle = value.Alloc();
                        }
                
                    err = attrSet.GetValue( ContentAccess::EAuthor, value );
                    if ( err != KErrNone)
                        {
                        WRITELOG1( "CHarvesterVideoPlugin::GatherDataL - ERROR: getting author failed %d", err );
                        }
                    else if( value.Length() > 0 )
                        {
                        aVHD.iAuthor = value.Alloc();
                        }

                    err = attrSet.GetValue( ContentAccess::EGenre, value );
                    if ( err != KErrNone)
                        {
                        WRITELOG1( "CHarvesterVideoPlugin::GatherDataL - ERROR: getting genre failed %d", err );
                        }
                    else if( value.Length() > 0 )
                        {
                        aVHD.iGenre = value.Alloc();
                        }
                    }
                CleanupStack::PopAndDestroy(); // attrSet
                }
            CleanupStack::PopAndDestroy( 2 ); // content, data
            }
        }
    else if( mapping->iHandler.iLibrary == TVideoMetadataHandling::EMp4LibHandling )
        {
#ifdef MDS_MP4LIB_USAGE
        MP4Handle handle( 0 );
        MP4Err mp4err = MP4_OK;
        
        WRITELOG( "CHarvesterVideoPlugin - Before open file handle to parse" );
        
        WRITELOG( "CHarvesterVideoPlugin - GatherDataL - MP4ParseOpenFileHandle - start" ); 
		
        mp4err = MP4ParseOpenFileHandle64( &handle, &file );
       
        WRITELOG( "CHarvesterVideoPlugin - GatherDataL - MP4ParseOpenFileHandle - ready" );            
        
        if ( mp4err != MP4_OK )
            {
            WRITELOG( "CHarvesterVideoPlugin - Error opening file handle" );
            
            CleanupStack::PopAndDestroy( &file );     
            User::Leave( KErrCompletion );
            }
        
        WRITELOG( "CHarvesterVideoPlugin - Start gathering" );
        
        if( aVHD.iVideoObject )
        	{
            mp4_u32 videolength( 0 );
            mp4_double framerate( 0 );
            mp4_u32 videotype( 0 );
            mp4_u32 videowidth( 0 );
            mp4_u32 videoheight( 0 );
            mp4_u32 timescale( 0 );

	        WRITELOG( "CHarvesterVideoPlugin - GatherDataL - MP4ParseRequestVideoDescription - start" );
	        mp4err = MP4ParseRequestVideoDescription( handle, &videolength, &framerate, 
	        		&videotype, &videowidth, &videoheight, &timescale );
	        WRITELOG( "CHarvesterVideoPlugin - GatherDataL - MP4ParseRequestVideoDescription - ready" );

	        if ( mp4err == MP4_OK )
	            {
	            aVHD.iDuration = videolength / KThousandReal;
		        aVHD.iFrameRate = framerate;
		        aVHD.iFrameWidth = videowidth;
		        aVHD.iFrameHeight = videoheight;
	            }

	        mp4_u32 streamSize( 0 );
	        mp4_u32 avgBitrate( 0 );

	        WRITELOG( "CHarvesterVideoPlugin - GatherDataL - MP4ParseRequestStreamDescription - start" );            
	        mp4err = MP4ParseRequestStreamDescription( handle, &streamSize, &avgBitrate );
	        WRITELOG( "CHarvesterVideoPlugin - GatherDataL - MP4ParseRequestStreamDescription - ready" );            

	        if ( mp4err == MP4_OK )
	            {
	            aVHD.iClipBitrate = avgBitrate / KKiloBytes;
	            }

       		aVHD.iMimeBuf = mapping->iHandler.iVideoMime.Alloc();
        	}
        else
        	{
        	mp4_u32 audiolength;
        	mp4_u32 audiotype;
        	mp4_u8 framespersample;
        	mp4_u32 timescale;
        	mp4_u32 averagebitrate;

        	WRITELOG( "CHarvesterVideoPlugin - GatherDataL - MP4ParseRequestVideoDescription - start" );
        	mp4err = MP4ParseRequestAudioDescription( handle, &audiolength, &audiotype, 
        			&framespersample, &timescale, &averagebitrate );
        	WRITELOG( "CHarvesterVideoPlugin - GatherDataL - MP4ParseRequestAudioDescription - ready" );

        	if ( mp4err == MP4_OK )
	            {
	            aVHD.iDuration = audiolength / KThousandReal;
	        	aVHD.iSamplingFrequency = timescale;
	        	aVHD.iAudioBitrate = averagebitrate / KKiloBytes;
	            }

       		aVHD.iMimeBuf = mapping->iHandler.iAudioMime.Alloc();
        	}

        WRITELOG( "CHarvesterVideoPlugin - Stop gathering" );

        WRITELOG( "CHarvesterVideoPlugin - GatherDataL - MP4ParseClose - start" );            
        mp4err = MP4ParseClose( handle );
        WRITELOG( "CHarvesterVideoPlugin - GatherDataL - MP4ParseClose - ready" );            
        if ( mp4err != MP4_OK )
            {
            WRITELOG( "CHarvesterVideoPlugin - Error closing file handle" );
            }
#endif
        }
    WRITELOG( "CHarvesterVideoPlugin - Closing file, if still open" );        
    CleanupStack::PopAndDestroy( &file );        

#ifdef _DEBUG
    dStop.UniversalTime();
    WRITELOG1( "CHarvesterVideoPlugin::GatherDataL end %d us", (TInt)dStop.MicroSecondsFrom(dStart).Int64() );
#endif  
    
    WRITELOG( "CHarvesterVideoPlugin - Start adding data to object" );
    }

// ---------------------------------------------------------------------------
// HandleNewObjectL
// ---------------------------------------------------------------------------
//
void CHarvesterVideoPlugin::HandleObjectPropertiesL( 
		CHarvesterData& aHD,
		CVideoHarvestData& aVHD,
		TBool aIsAdd )
    {
    WRITELOG("CHarvesterVideoPlugin::HandleObjectPropertiesL ");

    CMdEObject& mdeObject = aHD.MdeObject();
    
    InitPropDefsL( mdeObject.Def() );

    TTimeIntervalSeconds timeOffsetSeconds = User::UTCOffset();
    TTime localModifiedDate = aVHD.iModified + timeOffsetSeconds;

    if( !mdeObject.Placeholder() )
    	{
    	// Creation date
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iCreationDatePropertyDef, &localModifiedDate, aIsAdd );
    
    	// Last modified date
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iLastModifiedDatePropertyDef, &aVHD.iModified, aIsAdd );
    
    	// File size
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iSizePropertyDef, &aVHD.iFileSize, aIsAdd );
    	
    	// Default folder
        const TDesC& uri = mdeObject.Uri();
        if( uri.FindF( iMmcVideosPath->Des()) != KErrNotFound ||
            uri.FindF( iPhoneVideosPath->Des()) != KErrNotFound ||
            uri.FindF( KDCIMFolder ) != KErrNotFound ) 
            {
            TBool inDefaultFolder( ETrue );
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDefaultFolderPropertyDef, &inDefaultFolder, aIsAdd );
            }
        else
            {
            TBool inDefaultFolder( EFalse );
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDefaultFolderPropertyDef, &inDefaultFolder, aIsAdd );    
            }
    	}

    // Item Type
    if( aVHD.iMimeBuf )
        {
        TBool isAdd( EFalse );
        CMdEProperty* prop = NULL;
        TInt index = mdeObject.Property( *iPropDefs->iItemTypePropertyDef, prop );
        if( index < 0 )
            {
            isAdd = ETrue;
            }
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iItemTypePropertyDef, aVHD.iMimeBuf, isAdd );
        }
    
    // Release date
	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iReleaseDatePropertyDef, &localModifiedDate, aIsAdd );

	// Capture date
	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iCaptureDatePropertyDef, &localModifiedDate, aIsAdd );

	// Time offset
	TInt16 timeOffsetMinutes = timeOffsetSeconds.Int() / 60;
	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iTimeOffsetPropertyDef, &timeOffsetMinutes, aIsAdd );

    // Duration
	if( aVHD.iDuration != 0.0f )
		{
		CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDurationPropertyDef, &aVHD.iDuration, aIsAdd );
		}

	if( aVHD.iVideoObject )
		{
	    // Width
	    if (aVHD.iFrameWidth != 0)
	        {
	    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iWidthPropertyDef, &aVHD.iFrameWidth, aIsAdd );
	        }

	    // Height
	    if (aVHD.iFrameHeight != 0)
	        {
	    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iHeightPropertyDef, &aVHD.iFrameHeight, aIsAdd );
	        }
	
	    // Framerate
	    if (aVHD.iFrameRate != 0)
	    	{
	    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iFrameratePropertyDef, &aVHD.iFrameRate, aIsAdd );
	    	}
		}
	else // audio object
		{
		// Sampling frequency
	    if (aVHD.iSamplingFrequency != 0.0f)
	        {
	    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iSamplingFrequencyPropertyDef, &aVHD.iSamplingFrequency, aIsAdd );
	        }
		}

	// Bitrate
    if (aVHD.iClipBitrate != 0)
    	{
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iBitratePropertyDef, &aVHD.iClipBitrate, aIsAdd );
    	}
    else
    	{
    	if( aVHD.iVideoObject )
    		{
    	    if( aVHD.iVideoBitrate != 0 )
    	        {
    		    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iBitratePropertyDef, &aVHD.iVideoBitrate, aIsAdd );
    	        }
    		}
    	else // audio object
    		{
    	    if( aVHD.iAudioBitrate != 0 )
    	        {
    		    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iBitratePropertyDef, &aVHD.iAudioBitrate, aIsAdd );
    	        }
    		}
    	}

    // Copyright
    if( aVHD.iCopyright && aVHD.iCopyright->Length() < iMaxTextLength )
    	{
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iCopyrightPropertyDef, aVHD.iCopyright, aIsAdd );
    	}

    // Author
    if( aVHD.iAuthor && aVHD.iAuthor->Length() < iMaxTextLength )
    	{
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iAuthorPropertyDef, aVHD.iAuthor, aIsAdd );
    	}

    // Genre
    if( aVHD.iGenre && aVHD.iGenre->Length() < iMaxTextLength )
    	{
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iGenrePropertyDef, aVHD.iGenre, aIsAdd );
    	}

    // Artist
    if( aVHD.iPerformer && aVHD.iPerformer->Length() < iMaxTextLength )
    	{
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iArtistPropertyDef, aVHD.iPerformer, aIsAdd );
    	}

    // Description
    if( aVHD.iDescription && aVHD.iDescription->Length() < iMaxTextLength )
    	{
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDescriptionPropertyDef, aVHD.iDescription, aIsAdd );
    	}
    
    // Codec
    if( aVHD.iCodec != 0 )
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iAudioFourCCDef, &aVHD.iCodec, aIsAdd );
        }
    
    // Title
    if( aVHD.iTitle && aVHD.iTitle->Length() < KMaxTitleFieldLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iTitlePropertyDef, aVHD.iTitle, EFalse );
        }
    
    // DRM protection
    if( aVHD.iDrmProtected )
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDrmPropertyDef, &aVHD.iDrmProtected, aIsAdd );
        }
    }

#ifdef MDS_MP4LIB_USAGE 
void CHarvesterVideoPlugin::GetMp4Type( RFile64& aFile, TDes& aType )
    {
    WRITELOG( "CHarvesterVideoPlugin::GetMp4Mime - MP4ParseOpenFileHandle - start" );
    TFileName tempName;
    TUint32 mediaId( 0 );
    TInt blackListError( KErrNone );

    blackListError = GetFileFullNameAndMediaId( aFile, tempName, mediaId );
    if( blackListError == KErrNone )
        {
        blackListError == AddFileToBlackList( tempName, mediaId );
        }

    MP4Handle handle;

    MP4Err mp4err = MP4ParseOpenFileHandle64( &handle, &aFile );
    
    WRITELOG( "CHarvesterVideoPlugin::GetMp4Mime - MP4ParseOpenFileHandle - ready" );

    if ( mp4err != MP4_OK )
        {
        WRITELOG( "CHarvesterVideoPlugin::GetMp4Mime - Error opening file handle" ); 
        return;
        }

    WRITELOG( "CHarvesterVideoPlugin::GetMp4Mime - MP4ParseRequestVideoDescription - start" );
    mp4_u32 videolength;
    mp4_double framerate;
    mp4_u32 videowidth;
    mp4_u32 videoheight;
    mp4_u32 timescale;
    mp4_u32 videotype;
    mp4err = MP4ParseRequestVideoDescription( handle, &videolength,
    		&framerate, &videotype, &videowidth, &videoheight, &timescale );
    WRITELOG( "CHarvesterVideoPlugin::GetMp4Mime - MP4ParseRequestVideoDescription - ready" );

    // If no video stream found, set to Audio
    if ( mp4err == MP4_NO_VIDEO )
        {
        WRITELOG( "CHarvesterVideoPlugin::GetMp4Mime - No video stream found, set to Audio" );
        aType.Copy( KAudio() );
        }
    // Otherwise set to Video, regardless how badly file is corrupted
    else
    	{
    	aType.Copy( KVideo() );
    	}

    if( blackListError == KErrNone )
        {
        RemoveFileFromBlackList( tempName, mediaId );
        }

    MP4ParseClose( handle );
	}
#else
void CHarvesterVideoPlugin::GetMp4Type( RFile64& /*aFile*/, TDes& /*aType*/ )
    {
    }
#endif

void CHarvesterVideoPlugin::GetRmTypeL( RFile64& aFile, TDes& aType )
	{
	TBool possibleVideo = EFalse;

    TFileName tempName;
    TUint32 mediaId( 0 );
    TInt blackListError( KErrNone );
    
    blackListError = GetFileFullNameAndMediaId( aFile, tempName, mediaId );
    if( blackListError == KErrNone )
        {
        blackListError = AddFileToBlackList( tempName, mediaId );
        }
	
    CHXMetaDataUtility* helixMetadata = CHXMetaDataUtility::NewL();
    CleanupStack::PushL( helixMetadata );
    
	TRAPD( err, helixMetadata->OpenFileL( aFile ) );
	aFile.Close();

	if( err == KErrNone )
		{
		// doesn't own pointers to MIME types
		RPointerArray<HBufC> mimes;
		CleanupClosePushL( mimes );
		
		TUint metacount = 0;
		helixMetadata->GetMetaDataCount(metacount);
		for (TUint i = 0; i < metacount; i++)
			{
			HBufC *buf = NULL;
			HXMetaDataKeys::EHXMetaDataId metaid;
			helixMetadata->GetMetaDataAt( i, metaid, buf );
			if( metaid == HXMetaDataKeys::EHXMimeType )
				{
				mimes.AppendL( buf );
				}
			else if( metaid == HXMetaDataKeys::EHXVideoBitRate )
				{
				possibleVideo = ETrue;
				}
			}
	
		const TInt mimeCount = mimes.Count();
		
		// Set to Video, regardless how badly file is corrupted
		if( mimeCount == 0 )
			{
		    aType.Copy( KVideo );
			}
	
		for( TInt i = 0; i < mimeCount; i++ )
			{
			HBufC* mime = mimes[i];
	
			// "application/vnd.rn-realmedia" or "application/vnd.rn-realmedia-vbr"
			if( MdsUtils::Find( *mime, KMimeTypeRm() ) != KErrNotFound )
				{
				WRITELOG1( "CHarvesterVideoPlugin::GetRmTypeL - mimetype %S. Object type Rm", mime );
				if( possibleVideo )
					{
					aType.Copy( KVideo );
					}
				else
					{
					aType.Copy( KAudio );
					}
				
				break;
				}
			else if( MdsUtils::Find( *mime, KVideo() ) != KErrNotFound )
				{
				WRITELOG1( "CHarvesterVideoPlugin::GetRmTypeL - mimetype %S. Object type Video", mime );
				aType.Copy( KVideo );
	
				// use MIME with "video" substring, if file might be video
				if( possibleVideo )
					{
					break;
					}
				}
			else if( MdsUtils::Find( *mime, KAudio() ) != KErrNotFound )
				{
				WRITELOG1( "CHarvesterVideoPlugin::GetRmTypeL - mimetype %S. Object type Audio", mime );
				aType.Copy( KAudio );
				}
			// Set to Video, regardless how badly file is corrupted
			else
				{
				aType.Copy( KVideo );
				}
			}
		
	    // don't destory mime type pointers just clean array
	    CleanupStack::PopAndDestroy( &mimes );
		}
	// Set to Video, regardless how badly file is corrupted
	else
		{
		aType.Copy( KVideo );
		}

	helixMetadata->ResetL();
    CleanupStack::PopAndDestroy( helixMetadata );
	
    if( blackListError == KErrNone )
        {
        RemoveFileFromBlackList( tempName, mediaId );
        }  
	}

TInt CHarvesterVideoPlugin::AddFileToBlackList( const TFileName& aFullName, const TUint32& aMediaId )
    {
    TInt blackListError( KErrNone );

    TTime modified ( 0 );
    blackListError = iFs.Modified( aFullName, modified );
    if( blackListError == KErrNone )
        {
        WRITELOG( "CHarvesterVideoPlugin::AddFileToBlackList - Adding URI to blacklist" );
        blackListError = iBlacklist->AddFile( aFullName, aMediaId, modified );
        }

    return blackListError;
    }

TInt CHarvesterVideoPlugin::RemoveFileFromBlackList( const TFileName& aFullName, const TUint32& aMediaId )
    {
    TInt blackListError( KErrNone );

    if( iBlacklist )
        {
        WRITELOG( "CHarvesterVideoPlugin::RemoveFileFromBlackList - Removing URI from blacklist" );
        blackListError = iBlacklist->RemoveFile( aFullName, aMediaId );
        }

    return blackListError;
    }

TInt CHarvesterVideoPlugin::GetFileFullNameAndMediaId( const RFile64& aFile, TFileName& aFullName, TUint32& aMediaId)
    {
    TInt blackListError( KErrNone );

    if( iBlacklist )
        {
        WRITELOG( "CHarvesterVideoPlugin::GetFileFullNameAndMediaId" );
        blackListError = aFile.FullName( aFullName );
        if( blackListError == KErrNone )
            {
            blackListError = iMediaIdUtil->GetMediaId( aFullName, aMediaId );
            }
        }
    else
        {
        blackListError = KErrNotReady;
        }
 
    return blackListError;
    }

const THarvestingHandling* CHarvesterVideoPlugin::FindHandler( const TDesC& aUri )
	{
	TParsePtrC parse( aUri );
    TPtrC ext = parse.Ext();

    // remove '.' from extension
    if ( ext.Length() > 1 )
        {
        ext.Set( ext.Mid( 1 ) );
        }
    else
    	{
    	WRITELOG1( "CHarvesterVideoPlugin::GetObjectType - ERROR: %S. No any extension found", &aUri );
    	return NULL;
    	}

    THarvestingHandling finder( ext );

	TLinearOrder<THarvestingHandling> cmp(THarvestingHandling::CompareFunction);

	TInt pos = iMimeTypeMappings.FindInOrder( finder, cmp );

	// no matching extension found or extension is missing from filename
    if( KErrNotFound == pos || ext.Length() <= 0 )
    	{
    	WRITELOG1( "CHarvesterVideoPlugin::GetObjectType - ERROR: %S. No extension found", &aUri );
		return NULL;
    	}

    const THarvestingHandling* mapping = &iMimeTypeMappings[pos];
	
	return mapping;
	}

void CHarvesterVideoPlugin::CheckForCodecSupport( HBufC* aMimeBuffer, CVideoHarvestData& aVHD )
    {
    if( !aMimeBuffer )
        {
        return;
        }
    
    TPtrC mime( NULL, 0 );
    mime.Set( aMimeBuffer->Des().Ptr(), aMimeBuffer->Des().Length() );
    
    if( MdsUtils::Find( mime, KAudioAC3() ) != KErrNotFound )
        {
        aVHD.iCodec = KMDSFourCCCodeAC3;
        return;
        }
    
    if( MdsUtils::Find( mime, KAudioEAC3() ) != KErrNotFound )
        {
        aVHD.iCodec = KMDSFourCCCodeEAC3;
        return;
        }
    return;
    }

void CHarvesterVideoPlugin::InitPropDefsL(CMdEObjectDef& aObjectDef)
    {
    if( !iPropDefs->iCreationDatePropertyDef )
        {
        iPropDefs->SetByObjectDefL( aObjectDef );
        // Prefetch max text lengt for validity checking
        iMaxTextLength = iPropDefs->iCopyrightPropertyDef->MaxTextLengthL();
        }
    }

// End of file

