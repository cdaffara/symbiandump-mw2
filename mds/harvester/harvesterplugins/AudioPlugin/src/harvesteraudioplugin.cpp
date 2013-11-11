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
* Description:  Harvester audio plugin
*
*/


#include <e32base.h>
#include <harvesterdata.h>
#include <mdenamespacedef.h>
#include <mdeobjectdef.h>
#include <mdeobject.h>
#include <centralrepository.h>
#include <pathinfo.h>

#include "harvestercommon.h"
#include "harvesteraudioplugin.h"
#include "harvesteraudiopluginutils.h"
#include "mdsutils.h"
#include "mdscommoninternal.h"

#include "harvesterlog.h"

const TInt KMimeLength( 10 );
const TUid KHarvesterRepoUid = { 0x200009FE };
const TUint32 KEnableAlbumArtHarvest = 0x00090001;
const TInt KKiloBytes = 1024;

CHarvesterAudioPluginPropertyDefs::CHarvesterAudioPluginPropertyDefs() : CBase(),
    iCreationDatePropertyDef( NULL )
	{
	}

void CHarvesterAudioPluginPropertyDefs::ConstructL(CMdEObjectDef& aObjectDef)
	{
    SetByObjectDefL( aObjectDef );
	}

CHarvesterAudioPluginPropertyDefs* CHarvesterAudioPluginPropertyDefs::NewL()
    {
    CHarvesterAudioPluginPropertyDefs* self = 
        new (ELeave) CHarvesterAudioPluginPropertyDefs();
    return self;
    }

void CHarvesterAudioPluginPropertyDefs::SetByObjectDefL(CMdEObjectDef& aObjectDef)
    {
    CMdENamespaceDef& nsDef = aObjectDef.NamespaceDef();

    // Common property definitions
    CMdEObjectDef& objectDef = nsDef.GetObjectDefL( MdeConstants::Object::KBaseObject );
    iCreationDatePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KCreationDateProperty );
    iLastModifiedDatePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KLastModifiedDateProperty );
    iSizePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KSizeProperty );
    iItemTypePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
    iTitlePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
    iTimeOffsetPropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KTimeOffsetProperty );
    iDefaultFolderPropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KInDefaultFolder );

    // Media property definitions
    CMdEObjectDef& mediaDef = nsDef.GetObjectDefL( MdeConstants::MediaObject::KMediaObject );
    iRatingPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KRatingProperty );
    iGenrePropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KGenreProperty );
    iArtistPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KArtistProperty );
    iDurationPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KDurationProperty );
    iCopyrightPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KCopyrightProperty );
    iTrackPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KTrackProperty );
    iThumbnailPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KThumbnailPresentProperty );
    iDatePropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KReleaseDateProperty );
    iDrmPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KDRMProperty );
    iBitratePropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KBitrateProperty );

    // Audio property definitions
    CMdEObjectDef& audioDef = nsDef.GetObjectDefL( MdeConstants::Audio::KAudioObject );
    iAlbumPropertyDef = &audioDef.GetPropertyDefL( MdeConstants::Audio::KAlbumProperty );
    iComposerPropertyDef = &audioDef.GetPropertyDefL( MdeConstants::Audio::KComposerProperty );
    iOriginalArtistPropertyDef = &audioDef.GetPropertyDefL( MdeConstants::Audio::KOriginalArtistProperty );
    iSampleRatePropertyDef = &audioDef.GetPropertyDefL( MdeConstants::Audio::KSamplingFrequencyProperty );
    iAlbumArtistPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::Audio::KAlbumArtistProperty );
    }

using namespace MdeConstants;

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::CHarvesterAudioPlugin
// ---------------------------------------------------------------------------
//    
CHarvesterAudioPlugin::CHarvesterAudioPlugin() : CHarvesterPlugin(),
    iAudioParser( NULL ), iPropDefs( NULL ), iTNM( NULL ), iHarvestAlbumArt( EFalse )
	{
	}

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::NewL
// ---------------------------------------------------------------------------
//    
CHarvesterAudioPlugin* CHarvesterAudioPlugin::NewL()
	{
	WRITELOG( "CHarvesterAudioPlugin::NewL()" );
	CHarvesterAudioPlugin* self = new (ELeave) CHarvesterAudioPlugin();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	
	return self;
	}

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::~CHarvesterAudioPlugin
// ---------------------------------------------------------------------------
//    
CHarvesterAudioPlugin::~CHarvesterAudioPlugin()
	{
	WRITELOG( "CHarvesterAudioPlugin::~CHarvesterAudioPlugin()" );
	
	delete iAudioParser;
	iAudioParser = NULL;
	delete iPropDefs;
	iPropDefs = NULL;
	delete iTNM;
	iTNM = NULL;
	
	delete iPhoneSoundsPath;
	iPhoneSoundsPath = NULL;
	delete iMmcSoundsPath;
	iMmcSoundsPath = NULL;
	}

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::ConstructL
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlugin::ConstructL()
	{
	WRITELOG( "CHarvesterAudioPlugin::ConstructL()" );
	
    CRepository* rep = CRepository::NewLC( KHarvesterRepoUid );
    // If data could not be fetched, default to non-album art mode
    rep->Get( KEnableAlbumArtHarvest, iHarvestAlbumArt );
    CleanupStack::PopAndDestroy( rep );   
	
	iAudioParser = CAudioMDParser::NewL( iHarvestAlbumArt );
    
    if( iHarvestAlbumArt )
        {
        TRAP_IGNORE( iTNM = CThumbnailManager::NewL( *this ) );
        }
		
    iPropDefs = CHarvesterAudioPluginPropertyDefs::NewL();
    
    SetPriority( KHarvesterPriorityHarvestingPlugin - 2 );
	
    TFileName sounds = PathInfo::SoundsPath();
    
    TFileName phonePath = PathInfo::PhoneMemoryRootPath();
    phonePath.Append( sounds );
    iPhoneSoundsPath = phonePath.AllocL();

    TFileName mmcPath = PathInfo::MemoryCardRootPath();
    mmcPath.Append( sounds );
    iMmcSoundsPath = mmcPath.Right( mmcPath.Length() - 1 ).AllocL();
	}

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::HarvestL (from CHarvesterPlugin)
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlugin::HarvestL( CHarvesterData* aHD )
	{
	WRITELOG( "CHarvesterAudioPlugin::HarvestL()" );
	
	TRAPD( error, DoHarvestL( aHD ) );
	if ( error != KErrNone )
	    {
        WRITELOG1( "CHarvesterAudioPlugin::HarvestL() - error: %d", error );
        TInt convertedError = KErrNone;
        MdsUtils::ConvertTrapError( error, convertedError );
        aHD->SetErrorCode( convertedError );
        WRITELOG1( "CHarvesterAudioPlugin::HarvestL() - returning: %d", convertedError );
	    
	    }
	}

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::ThumbnailPreviewReady 
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlugin::ThumbnailPreviewReady( MThumbnailData& /*aThumbnail*/,
    TThumbnailRequestId /*aId*/ )
    {
    // Pass through, nothing to do
    }

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::ThumbnailReady
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlugin::ThumbnailReady( TInt /*aError*/, 
    MThumbnailData& /*aThumbnail*/,
    TThumbnailRequestId /*aId*/ )
    {
    // Pass through, nothing to do
    }

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::GetMimeType (from CHarvesterPlugin)
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlugin::GetMimeType( const TDesC& aUri, TDes& aMimeType )
    {
    aMimeType.Zero();
    
    const TMimeTypeMapping<TAudioMetadataHandling>* mapping = 
        iAudioParser->ParseMimeType( aUri );

    if ( !mapping )
        {
        return;
        }
    
    aMimeType = mapping->iMimeType;
    }

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::DoHarvestL
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlugin::DoHarvestL( CHarvesterData* aHD )
	{
	WRITELOG( "CHarvesterAudioPlugin::DoHarvestL()" );
    CMdEObject& mdeObject = aHD->MdeObject();
        
    TBool isAdd = EFalse;
    if ( mdeObject.Placeholder() || mdeObject.Id() == KNoId ) // is a new object or placeholder
        {
        isAdd = ETrue;
        }

    GetPropertiesL( aHD, isAdd );
	}


// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::GetPropertiesL
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlugin::GetPropertiesL( CHarvesterData* aHD,
                                            TBool aIsAdd )
    {
    CMdEObject& mdeObject = aHD->MdeObject();
    
    // get creation time, modified time and file size
    if( !mdeObject.Placeholder() )
        {
        GetPlaceHolderPropertiesL( aHD, aIsAdd );
        }
    
    const TMimeTypeMapping<TAudioMetadataHandling>* mapping = 
    	GetMimeTypePropertyL( aHD, aIsAdd );

    if( mapping )
    	{
		// get properties for file types supported by CMetaDataUtility.
    	if( mapping->iHandler == EMetaDataUtilityHandling )
    		{
    		GetMusicPropertiesL( aHD, aIsAdd, mapping->iMimeType );
    		}
    	}
    }

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::GetPlaceHolderPropertiesL
// Get placeholder properties (creation time, modify time and file size).
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlugin::GetPlaceHolderPropertiesL( CHarvesterData* aHD,
                               TBool aIsAdd )
    {
    CMdEObject& mdeObject = aHD->MdeObject();
    
    const TDesC& uri = mdeObject.Uri();
    
    TEntry entry;
    TInt err = iFs.Entry( uri, entry );
    
    if ( err!= KErrNone )
        {
        User::Leave( err ); // metadata cannot be gathered!
        }
    
    TTimeIntervalSeconds timeOffsetSeconds = User::UTCOffset();
    TTime localModifiedDate = entry.iModified + timeOffsetSeconds;
	
	InitPropDefsL( mdeObject.Def() );
	
	CMdeObjectWrapper::HandleObjectPropertyL(
                 mdeObject, *iPropDefs->iCreationDatePropertyDef, &localModifiedDate, aIsAdd );

	CMdeObjectWrapper::HandleObjectPropertyL(
             mdeObject, *iPropDefs->iLastModifiedDatePropertyDef, &entry.iModified, aIsAdd );

	CMdeObjectWrapper::HandleObjectPropertyL(
              mdeObject, *iPropDefs->iSizePropertyDef, &entry.iSize, aIsAdd );
	
	mdeObject.SetPlaceholder( EFalse );
    }

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::GetMimeTypePropertyL
// Get mime type.
// ---------------------------------------------------------------------------
//    
const TMimeTypeMapping<TAudioMetadataHandling>* CHarvesterAudioPlugin::GetMimeTypePropertyL( 
		CHarvesterData* aHD, TBool aIsAdd )
    {
    CMdEObject& mdeObject = aHD->MdeObject();
 
    const TMimeTypeMapping<TAudioMetadataHandling>* mapping = 
    	iAudioParser->ParseMimeType( mdeObject.Uri() );
    
    if ( mapping && !mdeObject.Placeholder() )
        {
        InitPropDefsL( mdeObject.Def() );
        
    	CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
    			*iPropDefs->iItemTypePropertyDef, (TAny*)&(mapping->iMimeType), aIsAdd );
        }
    
    return mapping;
    }

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::GetMusicPropertiesL
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlugin::GetMusicPropertiesL( CHarvesterData* aHD,
                                      TBool aIsAdd, TPtrC /*aMimeType*/ )
    {
#ifdef _DEBUG
    TTime dStart, dStop;
    dStart.UniversalTime();
    dStop.UniversalTime();
    WRITELOG1( "CHarvesterAudioPlugin::GetMusicPropertiesL start %d us", (TInt)dStop.MicroSecondsFrom(dStart).Int64() );
#endif
    
    CMdEObject& mdeObject = aHD->MdeObject();
    const TDesC& uri = mdeObject.Uri();

    TBool parsed( EFalse );
    TRAPD( parseError, parsed = iAudioParser->ParseL( uri ) );

    if( !parsed || (parseError != KErrNone) )
    	{
    	iAudioParser->ResetL();
    	return;
    	}

    InitPropDefsL( mdeObject.Def() );
    
    // We do not want to get all long text fields at this time
    TPtrC song        = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldSong );
    TPtrC artist      = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldArtist );
    TPtrC album       = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldAlbum );
    TPtrC genre       = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldGenre );
    TPtrC composer    = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldComposer );
    TPtrC rating      = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldRating );
    TPtrC orgArtist   = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldOriginalArtist );
    TPtrC track       = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldTrack );
    TPtrC duration    = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldDuration );
    TPtrC copyright   = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldCopyright );
    TPtrC date        = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldDate );   
    TPtrC prot        = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldProtected );
    TPtrC samplerate  = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldSampleRate );
    TPtrC bitrate     = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldBitRate );
    TPtrC albumartist = iAudioParser->MetaDataFieldL( CAudioMDParser::EAudioMDFieldAlbumArtist );
    
    TPtrC8 jpeg = iAudioParser->MetaDataField8L( CAudioMDParser::EAudioMDFieldJpeg );
 
    // Time offset
    TTimeIntervalSeconds timeOffsetSeconds = User::UTCOffset();
    TInt16 timeOffsetMinutes = timeOffsetSeconds.Int() / 60;
    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iTimeOffsetPropertyDef, &timeOffsetMinutes, aIsAdd );
    
    if( !mdeObject.Placeholder() )
        {
        if( uri.FindF( iMmcSoundsPath->Des()) != KErrNotFound ||
            uri.FindF( iPhoneSoundsPath->Des()) != KErrNotFound )
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
    
    if ( song.Length() > 0
        && song.Length() < KMaxTitleFieldLength )
        {    
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
        		*iPropDefs->iTitlePropertyDef, &song, EFalse );
        }

    if ( artist.Length() > 0
        && artist.Length() < iMaxTextLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
        		*iPropDefs->iArtistPropertyDef, &artist, aIsAdd );
        }

    if ( album.Length() > 0
        && album.Length() < iMaxTextLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
        		*iPropDefs->iAlbumPropertyDef, &album, aIsAdd );
        }
 
    if ( genre.Length() > 0
        && genre.Length() < iMaxTextLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
        		*iPropDefs->iGenrePropertyDef, &genre, aIsAdd );
        }

    if ( composer.Length() > 0
        && composer.Length() < iMaxTextLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
        		*iPropDefs->iComposerPropertyDef, &composer, aIsAdd );
        }

    if ( rating.Length() > 0 )
        {
        TLex ratingLex( rating );
        TUint8 ratingValue( 0 );
        const TInt error( ratingLex.Val( ratingValue, EDecimal ) );
        if( error == KErrNone )
            {
            CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
                *iPropDefs->iRatingPropertyDef, &ratingValue, aIsAdd );        
            }
        }
    
    if ( orgArtist.Length() > 0
        && orgArtist.Length() < iMaxTextLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
        		*iPropDefs->iOriginalArtistPropertyDef, &orgArtist, aIsAdd );
        }
    
    if ( track.Length() > 0 )
        {
        TLex trackLex( track );
        TUint16 trackValue( 0 );
        trackLex.Val( trackValue, EDecimal );
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
        		*iPropDefs->iTrackPropertyDef, &trackValue, aIsAdd );
        }
    
    if ( duration.Length() > 0 )
        {
        TLex durationLex( duration );
        TReal32 durationValue( 0 );
        const TInt error( durationLex.Val( durationValue, EDecimal ) );
        if( error == KErrNone )
            {
            if ( durationValue < iPropDefs->iDurationPropertyDef->MaxRealValueL() )
                {
                CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
                    *iPropDefs->iDurationPropertyDef, &durationValue, aIsAdd );
                }
            }
        }
    
    if ( copyright.Length() > 0
        && copyright.Length() < iMaxTextLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
        		*iPropDefs->iCopyrightPropertyDef, &copyright, aIsAdd );
        }
    
    if ( date.Length() > 0
        && date.Length() < iPropDefs->iDatePropertyDef->MaxTextLengthL() )
        {
        TTime releaseDate( date );
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
                *iPropDefs->iDatePropertyDef, &releaseDate, aIsAdd );
        }

    TLex lex;
    
    if ( prot.Length() > 0 )
        {
        lex.Assign( prot );
        TInt protValue( 0 );
        if( KErrNone == lex.Val( protValue ) && protValue == 1 )
            {
            CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
                    *iPropDefs->iDrmPropertyDef, &protValue, aIsAdd );
            }
        }
    
    if ( samplerate.Length() )
        {
        lex.Assign( samplerate );
        TReal32 value( 0 );
        if( KErrNone == lex.Val( value ) && value != 0 )
            {
            CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
                    *iPropDefs->iSampleRatePropertyDef, &value, aIsAdd );
            }
        }
    
    if ( bitrate.Length() > 0 )
        {
        lex.Assign( bitrate );
        TInt value( 0 );
        if( KErrNone == lex.Val( value ) && value != 0 )
            {
            value /= KKiloBytes;
            CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
                    *iPropDefs->iBitratePropertyDef, &value, aIsAdd );
            }
        }
    
    if ( albumartist.Length() > 0
        && albumartist.Length() < iPropDefs->iDatePropertyDef->MaxTextLengthL() )
        {
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
                *iPropDefs->iAlbumArtistPropertyDef, &albumartist, aIsAdd );
        }

    if( iHarvestAlbumArt && iTNM && jpeg.Length() > 0 )
        {
        HBufC8* jpegBuf = jpeg.AllocLC();  
        _LIT( KAlbumArtMimeType, "image/jpeg" );
        TBuf<KMimeLength> mimeType;
        mimeType.Copy( KAlbumArtMimeType );
        CThumbnailObjectSource* tnmSource = CThumbnailObjectSource::NewL( jpegBuf, mimeType, uri );
        // Ownership of buffer is transferred to Thumbnail Manager
        CleanupStack::Pop(); // jpegBuf
        iTNM->CreateThumbnails( *tnmSource );
        delete tnmSource;
        tnmSource = NULL;
        TBool thumbnailPresent( ETrue );
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
                          *iPropDefs->iThumbnailPropertyDef, &thumbnailPresent, aIsAdd );
        }
    else if( iHarvestAlbumArt && iTNM )
        {
        TBool thumbnailNotPresent( EFalse );
        CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
                          *iPropDefs->iThumbnailPropertyDef, &thumbnailNotPresent, aIsAdd );
        }
    
    iAudioParser->ResetL();
    
#ifdef _DEBUG
    dStop.UniversalTime();
    WRITELOG1( "CHarvesterAudioPlugin::GetMusicPropertiesL start %d us", (TInt)dStop.MicroSecondsFrom(dStart).Int64() );
#endif   
    }

void CHarvesterAudioPlugin::InitPropDefsL(CMdEObjectDef& aObjectDef)
    {
    if( !iPropDefs->iCreationDatePropertyDef )
        {
        iPropDefs->SetByObjectDefL( aObjectDef );
        // Prefetch max text lengt for validity checking
        iMaxTextLength = iPropDefs->iCopyrightPropertyDef->MaxTextLengthL();
        }
    }

// End of file

