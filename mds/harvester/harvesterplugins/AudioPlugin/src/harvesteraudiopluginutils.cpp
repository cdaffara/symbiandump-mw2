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
#include <apgcli.h>  // for RApaLsSession

#include "harvesteraudiopluginutils.h"

#include "mdsutils.h"
#include "harvesterlog.h"

_LIT( KMimeTypeMpeg,   "audio/mpeg" );
_LIT( KMimeTypeAac,    "audio/aac" );
_LIT( KMimeTypeAmr,    "audio/amr" );
_LIT( KMimeTypeAwb,    "audio/amr-wb" );
_LIT( KMimeTypeMidi,   "audio/midi" );
_LIT( KMimeTypeSpMidi, "audio/sp-midi" );
_LIT( KMimeTypeRng,    "application/vnd.nokia.ringing-tone" );
_LIT( KMimeTypeMxmf,   "audio/vnd.nokia.mobile-xmf" );
_LIT( KMimeTypeWav,    "audio/wav" );
_LIT( KMimeTypeAu,     "audio/au" );
_LIT( KMimeTypeAudioMatroska, "audio/x-matroska");
_LIT( KMimeTypeAudioMp4,  "audio/mp4" );

_LIT( KExtensionMp3,    "mp3" );
_LIT( KExtensionAac,    "aac" );
_LIT( KExtensionAmr,    "amr" );
_LIT( KExtensionAwb,    "awb" );
_LIT( KExtensionMid,    "mid" );
_LIT( KExtensionMidi,   "midi" );
_LIT( KExtensionSpMid,  "spmid" );
_LIT( KExtensionRng,    "rng" );
_LIT( KExtensionMxmf,   "mxmf" );
_LIT( KExtensionWav,    "wav" );
_LIT( KExtensionAu,     "au" );
_LIT( KExtensionNrt,    "nrt" );
_LIT( KExtensionMka,    "mka" );
_LIT( KExtensionM4a,    "m4a" );

// -----------------------------------------------------------------------------
// CAudioMDParser::NewL
// 
// -----------------------------------------------------------------------------
//
CAudioMDParser* CAudioMDParser::NewL( const TBool aAlbumArt )
    {
    CAudioMDParser* self = new ( ELeave ) CAudioMDParser();
	CleanupStack::PushL( self );
	self->ConstructL( aAlbumArt );
	CleanupStack::Pop( self );

	return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CAudioMDParser::~CAudioMDParser()
    {
    delete iMetaDataUtility;
    iMetaDataUtility = NULL;
    
    iMimeTypeMappings.Close();
    iWantedMetadataFields.Close();
    }


// -----------------------------------------------------------------------------
// CAudioMDParser::CAudioMDParser
// 
// -----------------------------------------------------------------------------
//
CAudioMDParser::CAudioMDParser() :
	iMetaDataUtility( NULL ), iLastParsedFileMimeType( 0 )
    {
    }


// -----------------------------------------------------------------------------
// CAudioMDParser::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CAudioMDParser::ConstructL( const TBool aAlbumArt )
    {
    iMetaDataUtility = CMetaDataUtility::NewL();

    // initialize metadata field list for CMetaDataUtility (iMetaDataUtility)
    iWantedMetadataFields.AppendL( EMetaDataSongTitle );
    iWantedMetadataFields.AppendL( EMetaDataArtist );
    iWantedMetadataFields.AppendL( EMetaDataAlbum );
    iWantedMetadataFields.AppendL( EMetaDataGenre );
    iWantedMetadataFields.AppendL( EMetaDataComposer );
    iWantedMetadataFields.AppendL( EMetaDataRating );
    iWantedMetadataFields.AppendL( EMetaDataAlbumTrack );
    iWantedMetadataFields.AppendL( EMetaDataOriginalArtist );
    iWantedMetadataFields.AppendL( EMetaDataDuration );
    iWantedMetadataFields.AppendL( EMetaDataCopyright );
    iWantedMetadataFields.AppendL( EMetaDataDate );
    iWantedMetadataFields.AppendL( EMetaDataProtected );
    iWantedMetadataFields.AppendL( EMetaDataSampleRate );
    iWantedMetadataFields.AppendL( EMetaDataBitRate );
    iWantedMetadataFields.AppendL( EMetaDataAlbumArtist );
    
    if( aAlbumArt )
        {
        iWantedMetadataFields.AppendL( EMetaDataJpeg );
        }
    
    TLinearOrder< TMimeTypeMapping<TAudioMetadataHandling> > cmp(
    		TMimeTypeMapping<TAudioMetadataHandling>::CompareFunction);

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionMp3(), KMimeTypeMpeg(), 
    		EMetaDataUtilityHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionAac(), KMimeTypeAac(), 
    		EMetaDataUtilityHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionAmr(), KMimeTypeAmr(), 
    		EMetaDataUtilityHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionAwb(), KMimeTypeAwb(), 
    		EMetaDataUtilityHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
        	KExtensionWma(), KMimeTypeWma(), 
        	EMetaDataUtilityHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionMid(), KMimeTypeMidi(), 
    		EFileSystemHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionMidi(), KMimeTypeMidi(), 
    		EFileSystemHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionSpMid(), KMimeTypeSpMidi(), 
    		EFileSystemHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionRng(), KMimeTypeRng(), 
    		EFileSystemHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionMxmf(), KMimeTypeMxmf(), 
    		EFileSystemHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionWav(), KMimeTypeWav(), 
    		EFileSystemHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionAu(), KMimeTypeAu(), 
    		EFileSystemHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
    		KExtensionNrt(), KMimeTypeRng(), 
    		EFileSystemHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
            KExtensionMka(), KMimeTypeAudioMatroska(), 
            EMetaDataUtilityHandling ), cmp ) );
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TAudioMetadataHandling>(
            KExtensionM4a(), KMimeTypeAudioMp4(), 
            EMetaDataUtilityHandling ), cmp ) );
    }


// -----------------------------------------------------------------------------
// CAudioMDParser::ParseMimeType
// 
// -----------------------------------------------------------------------------
//
const TMimeTypeMapping<TAudioMetadataHandling>* CAudioMDParser::ParseMimeType( 
		const TDesC& aFileName )
    {
    TPtrC ext;
    if( MdsUtils::GetExt( aFileName, ext ) )
    	{
    	TMimeTypeMapping<TAudioMetadataHandling> finder(ext);
    	
    	TLinearOrder< TMimeTypeMapping<TAudioMetadataHandling> > cmp(
    			TMimeTypeMapping<TAudioMetadataHandling>::CompareFunction);
    	
    	TInt pos = iMimeTypeMappings.FindInOrder( finder, cmp );
    	
    	if( KErrNotFound != pos )
    		{
    		iLastParsedFileMimeType = pos;
    		return &iMimeTypeMappings[ pos ];
    		}
    	}

    return NULL;
    }


// -----------------------------------------------------------------------------
// CAudioMDParser::ParseL
// 
// -----------------------------------------------------------------------------
//
TBool CAudioMDParser::ParseL( const TDesC& aFileName )
    {
    TBuf8<KMaxDataTypeLength> mime;
    mime.Copy( iMimeTypeMappings[ iLastParsedFileMimeType ].iMimeType );
    iMetaDataUtility->OpenFileL( aFileName, iWantedMetadataFields, mime );
    
    return iMetaDataUtility->MetaDataCount() > 0;
    }


// -----------------------------------------------------------------------------
// CAudioMDParser::MetaDataFieldL
// 
// -----------------------------------------------------------------------------
//
TPtrC CAudioMDParser::MetaDataFieldL( TInt aFieldId )
    {
    const CMetaDataFieldContainer& metaCont = iMetaDataUtility->MetaDataFieldsL();
    
    TMetaDataFieldId id = MapFieldId( aFieldId );
    return metaCont.Field( id );  // returns KNullDesC if not found
    }

// -----------------------------------------------------------------------------
// CAudioMDParser::MetaDataFieldL
// 
// -----------------------------------------------------------------------------
//
TPtrC8 CAudioMDParser::MetaDataField8L( TInt aFieldId )
    {
    const CMetaDataFieldContainer& metaCont = iMetaDataUtility->MetaDataFieldsL();
    
    TMetaDataFieldId id = MapFieldId( aFieldId );
    return metaCont.Field8( id );  // returns KNullDesC if not found
    }

// -----------------------------------------------------------------------------
// CAudioMDParser::ResetL
// 
// -----------------------------------------------------------------------------
//
void CAudioMDParser::ResetL()
    {
    iMetaDataUtility->ResetL();
    }

// -----------------------------------------------------------------------------
// CAudioMDParser::MapFieldId
// 
// -----------------------------------------------------------------------------
//
TMetaDataFieldId CAudioMDParser::MapFieldId( TInt aFieldId )
    {
    TMetaDataFieldId id = EMetaDataSongTitle;
    
    switch ( aFieldId )
        {
        case EAudioMDFieldSong:
            {
            id = EMetaDataSongTitle;
            break;
            }
        case EAudioMDFieldArtist:
            {
            id = EMetaDataArtist;
            break;
            }
        case EAudioMDFieldAlbum:
            {
            id = EMetaDataAlbum;
            break;
            }
        case EAudioMDFieldGenre:
            {
            id = EMetaDataGenre;
            break;
            }
        case EAudioMDFieldComposer:
            {
            id = EMetaDataComposer;
            break;
            }
        case EAudioMDFieldRating:
            {
            id = EMetaDataRating;
            break;
            }
        case EAudioMDFieldTrack:
            {
            id = EMetaDataAlbumTrack;
            break;
            }
        case EAudioMDFieldOriginalArtist:
            {
            id = EMetaDataOriginalArtist;
            break;
            }
        case EAudioMDFieldDuration:
            {
            id = EMetaDataDuration;
            break;
            }
        case EAudioMDFieldCopyright:
            {
            id = EMetaDataCopyright;
            break;
            }
        case EAudioMDFieldDate:
            {
            id = EMetaDataDate;
            break;
            }
        case EAudioMDFieldJpeg:
            {
            id = EMetaDataJpeg;
            break;
            }
        case EAudioMDFieldProtected:
            {
            id = EMetaDataProtected;
            break;
            }
        case EAudioMDFieldSampleRate:
            {
            id = EMetaDataSampleRate;
            break;
            }
        case EAudioMDFieldBitRate:
            {
            id = EMetaDataBitRate;
            break;
            }
        case EAudioMDFieldAlbumArtist:
            {
            id = EMetaDataAlbumArtist;
            break;
            }
        default:
            {
#ifdef _DEBUG
            _LIT( KPanicCategory,"HarvesterAudioPlugin" );
            __ASSERT_DEBUG( EFalse, User::Panic( KPanicCategory,  KErrArgument ) );
#endif
            break;
            }
        }
        
    return id;
    }
