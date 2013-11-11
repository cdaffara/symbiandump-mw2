/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Extended collection helper with an internal caching array
*  Version     : %version: e003sa33#27.1.12.3.7 % 
*
*/


#include <e32base.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>

#include <mpxmediageneraldefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediaaudiodefs.h>
#include <mpxmediadrmdefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>
#include <mpxmediamtpdefs.h>

#include <e32property.h>    // for RProperty
#include <UsbWatcherInternalPSKeys.h>
#include <usbpersonalityids.h>

#include <mpxcollectionutility.h>
#include <mpxharvesterutility.h>

#include <mpxlog.h>

#include "mpxcollectioncachedhelper.h"
#include "mpxcollectionhelpercommon.h"
#include <mpxmetadataextractor.h>

#include <MetaDataUtility.h>        // for SetMissingMetadataL
#include <MetaDataFieldContainer.h> // for SetMissingMetadataL

// CONSTANTS
const TInt KCacheCount = 10;

_LIT( K3GPFileExt, ".3gp" );
_LIT( K3G2FileExt, ".3g2" );
_LIT( KODFFileExt, ".odf" );
_LIT( KO4AFileExt, ".o4a" );
_LIT( KAACFileExt, ".aac" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionCachedHelper::CMPXCollectionCachedHelper() :
#ifdef RD_MPX_COLLECTION_CACHE
    CMPXCollectionHelperImp(EFalse),iNotInCache(ETrue), iHitFoundMedia(0),
    iNotHitInCache(0), iLookingInCache(0),iLookingInUnknowCache(0),
#endif //RD_MPX_COLLECTION_CACHE
    iMetadataExtractor(NULL)
    {
    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXCollectionCachedHelper::ConstructL()
    {
    MPX_DEBUG1("CMPXCollectionCachedHelper::ConstructL <--");
    iCache = CMPXMediaArray::NewL();
    CMPXCollectionHelperImp::ConstructL();

#ifdef RD_MPX_COLLECTION_CACHE
    MPX_DEBUG1("CMPXCollectionCachedHelper::ConstructL Todd advancedcache");
    // cache for unknown artist
    TMPXItemId artistId = NULL;
    TRAPD(err, artistId = GetArtistIdL( KNullDesC(), iMusicCollectionId ));
    if ( err || artistId == (TMPXItemId)0 )
        {
        MPX_DEBUG2("CMPXCollectionCachedHelper::ConstructL no known artist id %d", err);
        }
    else
        {
        // Get songs that belong to the unknown artist
        GetSongsL( artistId, iMusicCollectionId, ETrue );
        }
#endif //RD_MPX_COLLECTION_CACHE
    User::LeaveIfError( iFs.Connect() );
    User::LeaveIfError( iAppArc.Connect() );
    iMetadataExtractor = CMPXMetadataExtractor::NewL( iFs, iAppArc, iSupportedTypes);
    MPX_DEBUG1("CMPXCollectionCachedHelper::ConstructL -->");
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionCachedHelper* CMPXCollectionCachedHelper::NewL()
    {
    CMPXCollectionCachedHelper* self = CMPXCollectionCachedHelper::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionCachedHelper* CMPXCollectionCachedHelper::NewLC()
    {
    CMPXCollectionCachedHelper* self = new( ELeave ) CMPXCollectionCachedHelper();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXCollectionCachedHelper::~CMPXCollectionCachedHelper()
    {
    Commit();
    delete iFoundMedia;
#ifdef RD_MPX_COLLECTION_CACHE
    if ( iCachedArtistArray )
        {
        delete iCachedArtistArray;
        }
    if ( iCachedUnknownArtistArray )
        {
        delete iCachedUnknownArtistArray;
        }
#endif //RD_MPX_COLLECTION_CACHE
    delete iCache;
    iOp.Close();
    if (iMetadataExtractor)
        {
        delete iMetadataExtractor;
        }
    iFs.Close();
    iAppArc.Close();
    iSupportedTypes.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// Add a media object
// ---------------------------------------------------------------------------
//
void CMPXCollectionCachedHelper::AddL( CMPXMedia* aMedia )
    {
    MPX_FUNC("CMPXCollectionCachedHelper::::AddL");    
    // Commit when we have cached more than a set amount
    //
    
    TInt cacheCount = KCacheCount;
    TInt usbStatus;
    RProperty::Get( KPSUidUsbWatcher, KUsbWatcherSelectedPersonality, usbStatus );
    
    // if MTP is not connected via USB, disable caching mechanism and flush immediately
    // in case UI is not blocked and need to reflect the change in real time
    // this logic is hardly invoked during MTP, because ::FindAllL (which flush) is always called before AddL
    if ((usbStatus != KUsbPersonalityIdMTP) && (usbStatus != KUsbPersonalityIdPCSuiteMTP))
        cacheCount = 1;            
    
    if( iCache->Count() >= cacheCount)
        Commit();

#ifdef ABSTRACTAUDIOALBUM_INCLUDED
    TBool extract = ETrue;
    if( aMedia->IsSupported( KMPXMediaMTPSampleDataFlag ) )
        {
        
        extract = aMedia->ValueTObjectL<TBool>( KMPXMediaMTPSampleDataFlag );
        MPX_DEBUG2("CMPXCollectionCachedHelper::AddL KMPXMediaMTPSampleDataFlag is set. extract=%d", extract );
        }
    
    if( extract )
        {
        // Extract album art from the file
        iMetadataExtractor->ExtractAlbumArtL( aMedia );
        }
#else
    // Extract album art from the file
    iMetadataExtractor->ExtractAlbumArtL( aMedia );
#endif
    
    CMPXMedia* copy = CMPXMedia::NewL( *aMedia );
    CleanupStack::PushL( copy );
    iCache->AppendL( copy );  // ownership x-fer
    CleanupStack::Pop( copy );
    iOp.AppendL( EAdd );
    }

// ---------------------------------------------------------------------------
// Remove a media object
// ---------------------------------------------------------------------------
//
void CMPXCollectionCachedHelper::RemoveL( const TDesC& aFile, 
                                          TMPXGeneralCategory aItemCat )
    {
    MPX_FUNC("CMPXCollectionCachedHelper::RemoveL");
    MPX_DEBUG3("aFile %S, category %d", &aFile, aItemCat);
    
    Commit();
    
    ::InitializeCollectionPluginsL( *iCollectionUtil );
    
    //
    // ask harvester to remove the file if any of the following is true:
    // 1) removing a song
    // 2) removing a playlist scanned through file system.
    //
    //    There are 3 types of playlists. The ones created from the device
    //    are virtual playlists which have file extension .vir. There are
    //    no physical playlist files associated with them; hence not
    //    registered with harvester. For virtual playlists, IsPlaylistL
    //    will return EFalse because there is not a playlist plugin that
    //    deals with .vir playlist files.
    //
    //    The ones synced from PC through MTP have file extension .pla.
    //    There are 0-byte .pla files associated with them but they
    //    are not registered with harvester either. IsPlaylistL will also
    //    return EFalse because there is not a playlist plugin that deals
    //    with .pla.
    //
    //    The 3rd type of playlists is .m3u on the file system. They are
    //    added to the collection through file scan and registered with
    //    harvester. IsPlaylistL will return ETrue.
    //
    //    For now virtual playlists and synced playlists are assumed to
    //    be in the music collection for now until there is a generic way
    //    resolving collections aside from using file extension or UID.
    //
    TInt collection(iMusicCollectionId.iUid);
    if (aItemCat == EMPXSong || iHvsUtility->IsPlaylistL(aFile))
        {
        // Remove from the harvester
        collection = iHvsUtility->RemoveFileL( aFile );
        }
    
    // Remove from the collection
    // Construct a CMPXMedia object with URI and collection
    //
    RArray<TInt> contID;
    CleanupClosePushL( contID );
    contID.AppendL( KMPXMediaIdGeneral );
    CMPXMedia* media = CMPXMedia::NewL( contID.Array() );
    CleanupStack::PopAndDestroy(&contID);
    CleanupStack::PushL( media );
    
    media->SetTextValueL( KMPXMediaGeneralUri, aFile );
    media->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId, TUid::Uid( collection ) );
    media->SetTObjectValueL( KMPXMediaGeneralType, EMPXItem );
    media->SetTObjectValueL( KMPXMediaGeneralCategory, aItemCat );
    
    // set up remove media command
    CMPXCommand* command = CMPXMedia::NewL();
    CleanupStack::PushL(command);
    
    command->SetTObjectValueL<TMPXCommandId>(
        KMPXCommandGeneralId, KMPXCommandIdCollectionRemoveMedia);
    command->SetTObjectValueL<TBool>(
        KMPXCommandGeneralDoSync, ETrue);
    command->SetTObjectValueL<TInt>(
        KMPXCommandGeneralCollectionId, collection);
    command->SetCObjectValueL(
        TMPXAttribute(KMPXCommandIdCollectionRemoveMedia, EMPXCommandCollectionRemoveMedia), 
        media);   
    command->SetTObjectValueL<TBool>(
        TMPXAttribute(KMPXCommandIdCollectionRemoveMedia, EMPXCommandCollectionRemoveMediaDeleteRecord), ETrue);         
    
    // send sync remove media command    
    iCollectionUtil->Collection().CommandL(*command);

    //
    // return command should contain error attribute
    //
    if (!command->IsSupported(TMPXAttribute(KMPXCommandIdCollectionRemoveMedia, EMPXCommandCollectionRemoveMediaError)))
        {
        User::Leave(KErrAbort);
        }
    
    //
    // abandon operation if an error occured cleaning up deleted medias from the collection
    //
    TInt error =
         command->ValueTObjectL<TInt>(
            TMPXAttribute(KMPXCommandIdCollectionRemoveMedia, EMPXCommandCollectionRemoveMediaError));
    User::LeaveIfError(error);
    
    //
    // reset found media if it's been deleted
    //
    if (iFoundMedia &&
        iFoundMedia->ValueText(KMPXMediaGeneralUri).CompareF(aFile) == 0)
        {
        delete iFoundMedia;
        iFoundMedia = NULL;
        }    

    CleanupStack::PopAndDestroy(2, media ); // command and media
    }

// ---------------------------------------------------------------------------
// Remove a media object
// ---------------------------------------------------------------------------
//
void CMPXCollectionCachedHelper::CleanupDeletedMediasL()
    {
    MPX_FUNC("CMPXCollectionCachedHelper::CleanupDeletedMediasL");
    
    Commit();
    
    ::InitializeCollectionPluginsL( *iCollectionUtil );

    //
    // set up the command to send to the collection
    //
    CMPXCommand* command = CMPXMedia::NewL();
    CleanupStack::PushL(command);
    
    command->SetTObjectValueL<TMPXCommandId>(
        KMPXCommandGeneralId, KMPXCommandIdCollectionCleanupDeletedMedias);
    command->SetTObjectValueL<TBool>(
        KMPXCommandGeneralDoSync, ETrue);
    command->SetTObjectValueL<TInt>(
        KMPXCommandGeneralCollectionId, iMusicCollectionId.iUid);
    
    // send sync cleanup command    
    iCollectionUtil->Collection().CommandL(*command);
    
    //
    // return command should contain error attribute
    //
    if (!command->IsSupported(TMPXAttribute(KMPXCommandIdCollectionCleanupDeletedMedias, EMPXCommandCollectionCleanupError)))
        {
        User::Leave(KErrAbort);
        }
    
    //
    // abandon operation if an error occured cleaning up deleted medias from the collection
    //
    TInt error =
        command->ValueTObjectL<TInt>(
            TMPXAttribute(KMPXCommandIdCollectionCleanupDeletedMedias, EMPXCommandCollectionCleanupError));
    User::LeaveIfError(error); 
    
    CleanupStack::PopAndDestroy(command);
    }

// ---------------------------------------------------------------------------
// Update a media object
// ---------------------------------------------------------------------------
//    
void CMPXCollectionCachedHelper::SetL( CMPXMedia*& aMedia )
    {
    MPX_DEBUG1("CMPXCollectionCachedHelper::::SetL <--");     

#ifdef ABSTRACTAUDIOALBUM_INCLUDED    
    if( aMedia->IsSupported( KMPXMediaMTPSampleDataFlag ) )
        {
        TBool flag = aMedia->ValueTObjectL<TBool>( KMPXMediaMTPSampleDataFlag );            
        MPX_DEBUG2("CMPXCollectionCachedHelper::SetL KMPXMediaMTPSampleDataFlag is set. flag=%d", flag );
        if( flag )
            {        
            iMetadataExtractor->ExtractAlbumArtL( aMedia );
            }
        return;
        }
#endif
            
    const TDesC& newUri = aMedia->ValueText( KMPXMediaGeneralUri );
    TInt count( iCache->Count() );
    
    // Take advantage that MTP always try to access the most recent item
    //
    CMPXMedia* media( NULL );
    TBool anotherAbstractMediaFound = EFalse;
    for( TInt i=count-1; i>=0; --i )
        {
        const TDesC& uri = iCache->AtL(i)->ValueText( KMPXMediaGeneralUri );
        TMPXGeneralCategory category = iCache->AtL(i)->ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory);
        MPX_DEBUG3("CMPXCollectionCachedHelper::SetL iCache[i].category = %d, uri = %S", category, &uri);
        if( newUri.CompareF( uri ) == 0 )
            {
            MPX_DEBUG2("Found existing media, index %i", i);
            media = iCache->AtL(i);
            break;
            }
        else if ( category == EMPXPlaylist || category == EMPXAbstractAlbum )
            anotherAbstractMediaFound = ETrue;
        }
    
    // Not found in the array
    if( !media )
        {
        // If the catched count is more than KCacheCount, commit it to database.
		// could be song or alb coming, not in the cache, and there is at least one .alb on the cache
        // commit all if another AbstractMedia is found (anotherAbstractMediaFound is on ETrue if not the current one)
        if( ( count >= KCacheCount ) || anotherAbstractMediaFound )
            {
            Commit();
            }
        CMPXMedia* copy = CMPXMedia::NewL( *aMedia );
        CleanupStack::PushL( copy );
        iCache->AppendL( copy );  // ownership x-fer
        CleanupStack::Pop( copy );
        iOp.AppendL( ESet );
        }
    else
        {
        // Update existing media object already in the array
        //
        DoAppendGeneralL( *aMedia, *media );
        DoAppendMusicL( *aMedia, *media );
        DoAppendAudioL( *aMedia, *media );
        DoAppendDRML( *aMedia, *media ); 
        DoAppendContainerL( *aMedia, *media );
        DoAppendMTPL( *aMedia, *media ); 
        }
    MPX_DEBUG1("CMPXCollectionCachedHelper::::SetL -->");     
    }

// ---------------------------------------------------------------------------
// Renames a file
// ---------------------------------------------------------------------------
//    
void CMPXCollectionCachedHelper::RenameL( const TDesC& aOldUri,
                                          const TDesC& aNewUri,
                                          TMPXGeneralCategory aItemCat )
    {
    TInt count = iCache->Count();
    
    // Take advantage that MTP always try to access the most recent item
    //
    CMPXMedia* media( NULL );
    for( TInt i=count-1; i>=0; --i )
        {
        const TDesC& uri = iCache->AtL(i)->ValueText( KMPXMediaGeneralUri );
        if( aOldUri.CompareF( uri ) == 0 )
            {
            MPX_DEBUG2("Found existing media, index %i", i);
            media = iCache->AtL(i);
            break;
            }
        }
    
    // Not found in the array
    if( !media )
        {
        Commit();
        CMPXCollectionHelperImp::RenameL(aOldUri, aNewUri, aItemCat);
        }
    else
        {
        // Update existing media object already in the array
        //
        media->SetTextValueL(KMPXMediaGeneralUri, aNewUri);
        }
        
    if (iFoundMedia &&
        iFoundMedia->ValueText(KMPXMediaGeneralUri).CompareF(aOldUri) == 0)
        {
        iFoundMedia->SetTextValueL(KMPXMediaGeneralUri, aNewUri);
        }        
    }


#ifdef RD_MPX_COLLECTION_CACHE
// ---------------------------------------------------------------------------
// Gets the media object of the supplied attributes
// ---------------------------------------------------------------------------
//    
void CMPXCollectionCachedHelper::GetSongL( const TDesC& aFile, 
    TMPXGeneralCategory aItemCat, TUid aCollectionUId )
    {
    MPX_DEBUG1("CMPXCollectionCachedHelper::GetSongL <--");
    RArray<TMPXAttribute> attributes;
    CleanupClosePushL(attributes);

    attributes.AppendL(
        TMPXAttribute(KMPXMediaIdGeneral,
                      EMPXMediaGeneralTitle | EMPXMediaGeneralDate |
                      EMPXMediaGeneralDuration | EMPXMediaGeneralComment |
                      EMPXMediaGeneralUri ));
    attributes.AppendL(
        TMPXAttribute(KMPXMediaIdMusic,
                      EMPXMediaMusicArtist | EMPXMediaMusicAlbum |
                      EMPXMediaMusicAlbumTrack | EMPXMediaMusicComposer |
                      EMPXMediaMusicYear | EMPXMediaMusicGenre));

    attributes.AppendL(KMPXMediaAudioAudioAll);
    attributes.AppendL(KMPXMediaMTPAll);

    delete iFoundMedia;
    iFoundMedia = NULL;
    iFoundMedia = CMPXCollectionHelperImp::GetL( aFile, attributes.Array(), aItemCat );
    if ( !iFoundMedia )
        {
        User::Leave(KErrNotFound);
        }
    iFoundMedia->SetTObjectValueL<TUid>(
        KMPXMediaGeneralCollectionId, aCollectionUId );

    CleanupStack::PopAndDestroy(&attributes);
    //MPX_DEBUG1("Artist name =");
    //RDebug::RawPrint(iFoundMedia->ValueText( KMPXMediaMusicArtist));
    MPX_DEBUG1("CMPXCollectionCachedHelper::GetSongL -->");
    }



        
// ---------------------------------------------------------------------------
// Gets the artist id for the media object
// ---------------------------------------------------------------------------
//    
TMPXItemId CMPXCollectionCachedHelper::GetArtistIdL( const TDesC& aArtist, 
    TUid aCollectionUId )
    {
    MPX_DEBUG1("CMPXCollectionCachedHelper::GetArtistIdL <--");
    CMPXMedia* search = CMPXMedia::NewL();
    CleanupStack::PushL(search);
    search->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);        
    search->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXArtist);
    search->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId, aCollectionUId );
    search->SetTextValueL( KMPXMediaGeneralTitle, aArtist );
           
    RArray<TMPXAttribute> attributes;
    CleanupClosePushL(attributes);
    attributes.AppendL(KMPXMediaGeneralId);

    CMPXMedia* result =
        CMPXCollectionHelperImp::FindAllL(*search, attributes.Array());
    CleanupStack::PopAndDestroy(&attributes);
    CleanupStack::PopAndDestroy(search);
    CleanupStack::PushL(result);
    const CMPXMediaArray* results =
        result->Value<CMPXMediaArray>(KMPXMediaArrayContents);
    if( !results )
        {
        User::Leave( KErrNoMemory );
        }
    
    TMPXItemId artistId = 0;
    if ( !results->Count() || aArtist == KNullDesC )
    	{
	    CleanupStack::PopAndDestroy(result);
    	return TMPXItemId(0);
    	}

    artistId = results->AtL(0)->ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId);
    CleanupStack::PopAndDestroy(result);

    
    MPX_DEBUG1("CMPXCollectionCachedHelper::GetArtistIdL -->");
    return artistId;
    }



// ---------------------------------------------------------------------------
// Gets the artist id for the media object
// ---------------------------------------------------------------------------
//    
void CMPXCollectionCachedHelper::GetSongsL( TMPXItemId aArtistId, 
    TUid aCollectionUId, TBool aUnknownArtist )
    {
    MPX_DEBUG1("CMPXCollectionCachedHelper::GetSongsLL <--");

    RArray<TMPXAttribute> attributes;
    CleanupClosePushL(attributes);
    
    if ((aArtistId == (TMPXItemId)0) || aUnknownArtist )
	    {
	    CleanupStack::PopAndDestroy(&attributes);	    
	    return;
	    }

    attributes.AppendL(
        TMPXAttribute(KMPXMediaIdGeneral,
                      EMPXMediaGeneralTitle | EMPXMediaGeneralDate |
                      EMPXMediaGeneralDuration | EMPXMediaGeneralComment |
                      EMPXMediaGeneralUri ));
    attributes.AppendL(
        TMPXAttribute(KMPXMediaIdMusic,
                      EMPXMediaMusicArtist | EMPXMediaMusicAlbum |
                      EMPXMediaMusicAlbumTrack | EMPXMediaMusicComposer |
                      EMPXMediaMusicYear | EMPXMediaMusicGenre));

    attributes.AppendL(KMPXMediaAudioAudioAll);
    attributes.AppendL(KMPXMediaMTPAll);
    
    CMPXMedia* search = CMPXMedia::NewL();
    CleanupStack::PushL( search );
    search->SetTObjectValueL( KMPXMediaGeneralType, EMPXGroup);
    search->SetTObjectValueL( KMPXMediaGeneralCategory, EMPXSong );
    search->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId, aCollectionUId );
    search->SetTObjectValueL<TMPXItemId>( KMPXMediaGeneralId, aArtistId );

    // Search in synch mode
    CMPXMedia* result = CMPXCollectionHelperImp::FindAllL( *search, attributes.Array() );
    CleanupStack::PopAndDestroy( search );
    CleanupStack::PopAndDestroy(&attributes);

    CleanupStack::PushL(result);    
    if( result->IsSupported( KMPXMediaArrayContents ) )
        {
        if ( !aUnknownArtist )
            { // cache for known artist
            if ( iCachedArtistArray )
                {
                delete iCachedArtistArray;
                iCachedArtistArray = NULL;
                }          
            CMPXMediaArray* ary = result->Value<CMPXMediaArray>(KMPXMediaArrayContents);
            iCachedArtistArray = CMPXMediaArray::NewL(*ary);
            MPX_DEBUG2("Known Artist Array count()=%d", iCachedArtistArray->Count());
            }
        else // cache for unknown artist
            {
            if ( iCachedUnknownArtistArray )
                {
                delete iCachedUnknownArtistArray;
                iCachedUnknownArtistArray = NULL;
                }          
            CMPXMediaArray* ary = result->Value<CMPXMediaArray>(KMPXMediaArrayContents);
            iCachedUnknownArtistArray = CMPXMediaArray::NewL(*ary);
            MPX_DEBUG2("Unknown Artist Array count()=%d", iCachedUnknownArtistArray->Count());
            }
      }


    CleanupStack::PopAndDestroy(result);

    MPX_DEBUG1("CMPXCollectionCachedHelper::GetSongsLL -->");
    }

#endif //RD_MPX_COLLECTION_CACHE

    
// ---------------------------------------------------------------------------
// Gets the attributes of some media object
// ---------------------------------------------------------------------------
//    
const CMPXMedia& CMPXCollectionCachedHelper::GetL( const TDesC& aFile, 
                                                   TMPXGeneralCategory aItemCat )
    {
    
#ifdef RD_MPX_COLLECTION_CACHE
    Commit();

    if (aItemCat != EMPXSong && aItemCat != EMPXPlaylist)
        {
        User::Leave(KErrArgument);
        }

    MPX_DEBUG1("CMPXCollectionCachedHelper::GetL (advanced cache version) <--");
    MPX_DEBUG2("aFile %S", &aFile);

    // If the iFoundMedia is cached
    if ( iFoundMedia )
        {
        // and if asking for the same file, just return.
        if ( iFoundMedia->ValueText(KMPXMediaGeneralUri).CompareF(aFile) == 0 )
            {
            iHitFoundMedia++;
            return *iFoundMedia;
            }
        // and if not asking for the same file, seach through cacahed array.
        // It probably cached in the cached artist array
        else if ( iCachedArtistArray && iCachedArtistArray->Count() )
            {
            TInt count( iCachedArtistArray->Count() );
            iNotInCache = ETrue;
            iLookingInCache++;            
            MPX_DEBUG1("CMPXCollectionCachedHelper::GetL looking into cache <--");
            for( TInt i=0; i<count; i++ )
                {
                CMPXMedia& m = *iCachedArtistArray->AtL(i);
                //RDebug::Printf("Checking... begin");
                //RDebug::RawPrint(m.ValueText(KMPXMediaGeneralUri));
                if ( m.ValueText(KMPXMediaGeneralUri).CompareF(aFile) == 0 )
                    {
                    delete iFoundMedia;
                    iFoundMedia = NULL;
                    iFoundMedia = CMPXMedia::NewL( m );
                    iNotInCache = EFalse;
                    MPX_DEBUG1("CMPXCollectionCachedHelper::GetL found in cache <--");
                    break;
                    }
                }
            // if not in cached artist array, try with unknown array 
            if ( iNotInCache && iCachedUnknownArtistArray && iCachedUnknownArtistArray->Count() )
                {
                MPX_DEBUG1("CMPXCollectionCachedHelper::GetL looking into unknown cache <--");
                iLookingInUnknowCache++;                    
                count = iCachedUnknownArtistArray->Count();
                for( TInt i=0; i<count; i++ )
                    {
                    CMPXMedia& m = *iCachedUnknownArtistArray->AtL(i);
                    if ( m.ValueText(KMPXMediaGeneralUri).CompareF(aFile) == 0 )
                        {
                        MPX_DEBUG1("CMPXCollectionCachedHelper::GetL found unknown cache <--");
                        delete iFoundMedia;
                        iFoundMedia = NULL;
                        iFoundMedia = CMPXMedia::NewL( m );
                        iNotInCache = EFalse;
                        break;
                        }
                    }
                }
            }
        }


        
    // No cache or not finding anything in the cached artist arraies
    // it got to search the song in db and also create cache for the new artist
    if ( !iFoundMedia || !iCachedArtistArray || iNotInCache )
        {
        MPX_DEBUG4("CMPXCollectionCachedHelper::GetL looking in db <-- iFoundMedia=%x iCachedArtistArray=%x, iNotInCache=%d", iFoundMedia, iCachedArtistArray, iNotInCache);
        TInt col(iMusicCollectionId.iUid);
        if (aItemCat == EMPXSong || iHvsUtility->IsPlaylistL(aFile))    
            {
            TRAPD(err, col = iHvsUtility->FindCollectionIdL( aFile ));
            if (err != KErrNone)
            	{
            	MPX_DEBUG2("CMPXCollectionCachedHelper::GetL error $", err);
            	}
            }
        // to get single song that matches the supplied URI

        GetSongL(aFile, aItemCat, TUid::Uid(col)); 

        if ( aItemCat == EMPXPlaylist )
            {
            return *iFoundMedia; // no need to cache for playlist request
            }

        iNotHitInCache++;

        // Get Artist Id
        TMPXItemId artistId = (TMPXItemId)0;
        if ( iFoundMedia->IsSupported(KMPXMediaMusicArtist) 
        	&& iFoundMedia->ValueText(KMPXMediaMusicArtist).Length() )
	        {
	        TRAPD(err, artistId = GetArtistIdL( 
	            iFoundMedia->ValueText(KMPXMediaMusicArtist), TUid::Uid(col) ));
	        if ( err )
	            {
	            artistId = GetArtistIdL( KNullDesC(), TUid::Uid(col) );
	            }
	        }
	    else 
		    {
	        artistId = GetArtistIdL( KNullDesC(), TUid::Uid(col) );
		    }
        // Get songs that belongs to the same artist id
        GetSongsL( artistId, TUid::Uid(col), EFalse );
        }

    MPX_DEBUG1("CMPXCollectionCachedHelper::GetL -->");
    return *iFoundMedia;
    
#else  //RD_MPX_COLLECTION_CACHE

    MPX_DEBUG1("CMPXCollectionCachedHelper::GetL <--");
    
    TInt usbStatus;
    RProperty::Get( KPSUidUsbWatcher, KUsbWatcherSelectedPersonality, usbStatus );
    
    // if MTP is not connected via USB, flush immediately
    // in case UI is not blocked and need to reflect the change in real time
    if ((usbStatus != KUsbPersonalityIdMTP) && (usbStatus != KUsbPersonalityIdPCSuiteMTP))
        Commit();

#ifdef ABSTRACTAUDIOALBUM_INCLUDED
    if (aItemCat != EMPXSong && aItemCat != EMPXPlaylist && aItemCat != EMPXAbstractAlbum)
#else
    if (aItemCat != EMPXSong && aItemCat != EMPXPlaylist)
#endif
        {
        User::Leave(KErrArgument);
        }

    // Do reverse search on cache 
    for( TInt i = iCache->Count()-1; i >= 0; --i )
        {
        CMPXMedia* media = iCache->AtL(i);
        if( media &&
                media->ValueText(KMPXMediaGeneralUri).CompareF(aFile) == 0 )
            {
            return *media;
            }
        }
    
    if (!iFoundMedia ||
        iFoundMedia->ValueText(KMPXMediaGeneralUri).CompareF(aFile) != 0)
        {
        delete iFoundMedia;
        iFoundMedia = NULL;
        
		MPX_DEBUG1("CMPXCollectionCachedHelper::GetL, not found on the cache");
        Commit( ETrue );    // leave AbstractAlbum in cache

        RArray<TMPXAttribute> attributes;
        CleanupClosePushL(attributes);

        attributes.AppendL(
            TMPXAttribute(KMPXMediaIdGeneral,
                          EMPXMediaGeneralTitle | EMPXMediaGeneralDate |
                          EMPXMediaGeneralDuration | EMPXMediaGeneralComment |
                          EMPXMediaGeneralUri ));
#ifdef ABSTRACTAUDIOALBUM_INCLUDED
        attributes.AppendL(
            TMPXAttribute(KMPXMediaIdMusic,
                          EMPXMediaMusicArtist | EMPXMediaMusicAlbum |
                          EMPXMediaMusicAlbumTrack | EMPXMediaMusicComposer |
                          EMPXMediaMusicYear | EMPXMediaMusicGenre | 
                          EMPXMediaMusicAlbumArtist));
#else
	    attributes.AppendL(
            TMPXAttribute(KMPXMediaIdMusic,
	                      EMPXMediaMusicArtist | EMPXMediaMusicAlbum |
	                      EMPXMediaMusicAlbumTrack | EMPXMediaMusicComposer |
	                      EMPXMediaMusicYear | EMPXMediaMusicGenre));
#endif
        attributes.AppendL(KMPXMediaAudioAudioAll);
        attributes.AppendL(KMPXMediaMTPAll);

        iFoundMedia = CMPXCollectionHelperImp::GetL( aFile, attributes.Array(), aItemCat );
        
        CleanupStack::PopAndDestroy(&attributes);
        }
    MPX_DEBUG1("CMPXCollectionCachedHelper::GetL -->");
    return *iFoundMedia;

#endif //RD_MPX_COLLECTION_CACHE
        
    }

// ---------------------------------------------------------------------------
// Find some media
// ---------------------------------------------------------------------------
//
CMPXMedia* CMPXCollectionCachedHelper::FindAllL( CMPXMedia& aCriteria, 
                     const TArray<TMPXAttribute>& aAttrs )
    {
    TBool cacheAbstractAlbum = EFalse;
    TMPXGeneralCategory category = aCriteria.ValueTObjectL<TMPXGeneralCategory>( KMPXMediaGeneralCategory );
    
    // leave AbstractAlbum in cache, unless the new criteria is another abstract media
    if ( ( category != EMPXPlaylist ) && ( category != EMPXAbstractAlbum ) )
        cacheAbstractAlbum = ETrue;
    
    Commit( cacheAbstractAlbum );

    return CMPXCollectionHelperImp::FindAllL( aCriteria, aAttrs );
    }

// ---------------------------------------------------------------------------
// Close function to destroy this object
// ---------------------------------------------------------------------------
//                                 
void CMPXCollectionCachedHelper::Close()
    {
#ifdef RD_MPX_COLLECTION_CACHE
    MPX_DEBUG2("mpx perf result iHitFoundMedia=%d", iHitFoundMedia);
    MPX_DEBUG2("mpx pref result iNotHitInCache=%d", iNotHitInCache);
    MPX_DEBUG2("mpx perf result iLookingInCache=%d", iLookingInCache);
    MPX_DEBUG2("mpx perf result iLookingInUnknowCache=%d", iLookingInUnknowCache);
#endif //RD_MPX_COLLECTION_CACHE
    delete this;  
    }

// ---------------------------------------------------------------------------
// Commits all transactions left in the helper
// ---------------------------------------------------------------------------
//   
void CMPXCollectionCachedHelper::Commit( TBool aCacheAbstractAlbum )
    {
    MPX_DEBUG2("CMPXCollectionCachedHelper::Commit %d <--", iCache->Count());
    TInt error( KErrNotFound );
    TInt count( iCache->Count() );
    TInt currentIndex = 0;
    
    for( TInt i = 0; i < count; ++i )
        {
        CMPXMedia* media = (*iCache)[currentIndex];
        TMPXGeneralCategory category = media->ValueTObjectL<TMPXGeneralCategory>( KMPXMediaGeneralCategory );
        TBool needCache = EFalse;
        
        if ( ( category == EMPXAbstractAlbum ) && aCacheAbstractAlbum )
            needCache = ETrue;
        
        if( media )
            {
            switch( iOp[currentIndex] )
                {
                case EAdd:
                    {
                    if ( !needCache )
                        {
                        TRAP( error, SetMissingMetadataL( media ) );    // doing this only before initial add
                        MPX_DEBUG2( "CMPXCollectionCachedHelper::Commit, SetMissingMetadataL, err = %d", error );
                        TRAP( error, CMPXCollectionHelperImp::AddL( media ) );
                        }
                    break;
                    }
                case ESet:
                    {
                    TRAP( error,
                        CMPXCollectionHelperImp::SetL( media );
                        );
                    break;
                    }
                default:
                    {
                    ASSERT(0);
                    break;
                    }
                }
        
            }
        else
            {
            error = KErrNoMemory;
            }
        
        if (!error &&
            iFoundMedia &&
            iFoundMedia->ValueText(KMPXMediaGeneralUri).CompareF(media->ValueText(KMPXMediaGeneralUri)) == 0)
            {
            TRAP_IGNORE( 
                    DoAppendGeneralL( *media, *iFoundMedia );
                    DoAppendMusicL( *media, *iFoundMedia );
                    DoAppendAudioL( *media, *iFoundMedia );
                    DoAppendDRML( *media, *iFoundMedia ); 
                    DoAppendContainerL( *media, *iFoundMedia );
                    DoAppendMTPL( *media, *iFoundMedia ); 
                    );
            }

        if ( needCache )
            {
            const TDesC& uri = media->ValueText(KMPXMediaGeneralUri);
			MPX_DEBUG2("need cache uri %S", &uri);
            currentIndex++;
            }
        else
            {
            iCache->Remove( currentIndex );
            iOp.Remove( currentIndex );
            }        
        }
    
    MPX_DEBUG1("CMPXCollectionCachedHelper::Commit -->");        
    }

// ---------------------------------------------------------------------------
// Copies general media information
// ---------------------------------------------------------------------------
//  
void CMPXCollectionCachedHelper::DoAppendGeneralL( CMPXMedia& aSrc, 
                                                   CMPXMedia& aDestination )
    {
    MPX_DEBUG1("CMPXCollectionCachedHelper::DoAppendGeneralL <--");     
    
    // Note: Only a subset used by MTP...
    //
    TUint atts = aSrc.AttributesSet( KMPXMediaIdGeneral );
    if( atts&EMPXMediaGeneralUri )  // text
        {
        aDestination.SetTextValueL( KMPXMediaGeneralUri,
                     aSrc.ValueText(KMPXMediaGeneralUri )
                                  );
        }
    if( atts&EMPXMediaGeneralDrive )  // text
        {
        aDestination.SetTextValueL( KMPXMediaGeneralDrive,
                     aSrc.ValueText(KMPXMediaGeneralDrive )
                                  );
        }
    if( atts&EMPXMediaGeneralTitle ) // text
        {
        aDestination.SetTextValueL( KMPXMediaGeneralTitle,
                     aSrc.ValueText(KMPXMediaGeneralTitle )
                                  );
        }
    if( atts&EMPXMediaGeneralComment ) //text
        {
        aDestination.SetTextValueL( KMPXMediaGeneralComment,
                     aSrc.ValueText(KMPXMediaGeneralComment ) 
                                  );
        }
    if( atts&EMPXMediaGeneralMimeType ) //text
        {
        aDestination.SetTextValueL( KMPXMediaGeneralMimeType,
                     aSrc.ValueText(KMPXMediaGeneralMimeType ) 
                                  );
        }
    if( atts&EMPXMediaGeneralDate ) // TInt64
        {
        aDestination.SetTObjectValueL( KMPXMediaGeneralDate,
                    aSrc.ValueTObjectL<TInt64>(KMPXMediaGeneralDate ) 
                                     );
        }
    if( atts&EMPXMediaGeneralDuration ) // TInt
        {
        aDestination.SetTObjectValueL( KMPXMediaGeneralDuration,
                    aSrc.ValueTObjectL<TInt>( KMPXMediaGeneralDuration ) 
                                     );
        }
    if( atts&EMPXMediaGeneralSynchronized ) // TBool
        {
        aDestination.SetTObjectValueL( KMPXMediaGeneralSynchronized,
                    aSrc.ValueTObjectL<TBool>( KMPXMediaGeneralSynchronized ) 
                                     );
        }
    if( atts&EMPXMediaGeneralDeleted ) // TBool
        {
        aDestination.SetTObjectValueL( KMPXMediaGeneralDeleted,
                    aSrc.ValueTObjectL<TBool>( KMPXMediaGeneralDeleted ) 
                                     );
        }
    if( atts&EMPXMediaGeneralModified )  // TBool
        {
        aDestination.SetTObjectValueL( KMPXMediaGeneralModified,
                    aSrc.ValueTObjectL<TBool>( KMPXMediaGeneralModified ) 
                                     );
        }
    if( atts&KMPXMediaGeneralFlags.iAttributeId )  // TUint
        {
        aDestination.SetTObjectValueL( KMPXMediaGeneralFlags,
                    aSrc.ValueTObjectL<TUint>( KMPXMediaGeneralFlags ) 
                                     );
        }
    MPX_DEBUG1("CMPXCollectionCachedHelper::DoAppendGeneralL -->");     
    }

// ---------------------------------------------------------------------------
// Copies music media information
// ---------------------------------------------------------------------------
//  
void CMPXCollectionCachedHelper::DoAppendMusicL( CMPXMedia& aSrc, 
                                                 CMPXMedia& aDestination )
    {
    MPX_DEBUG1("CMPXCollectionCachedHelper::DoAppendMusicL <--");     
    TUint atts = aSrc.AttributesSet( KMPXMediaIdMusic );
    
    // Note: Only a subset used by MTP...
    //
    if( atts&EMPXMediaMusicArtist ) // Text
        {
        aDestination.SetTextValueL( KMPXMediaMusicArtist,
                     aSrc.ValueText(KMPXMediaMusicArtist ) 
                                  );
        }
    if( atts&EMPXMediaMusicAlbum ) // Text
        {
        aDestination.SetTextValueL( KMPXMediaMusicAlbum,
                     aSrc.ValueText(KMPXMediaMusicAlbum ) 
                                  );
        }
    if( atts&EMPXMediaMusicAlbumTrack ) // Text
        {
        aDestination.SetTextValueL( KMPXMediaMusicAlbumTrack,
                     aSrc.ValueText(KMPXMediaMusicAlbumTrack ) 
                                  );
        }
    if( atts&EMPXMediaMusicComposer ) // Text
        {
        aDestination.SetTextValueL( KMPXMediaMusicComposer,
                     aSrc.ValueText(KMPXMediaMusicComposer ) 
                                  );
        }
    if( atts&EMPXMediaMusicYear ) // TInt64
        {
        aDestination.SetTObjectValueL<TInt64>( KMPXMediaMusicYear,
                          aSrc.ValueTObjectL<TInt64>( KMPXMediaMusicYear ) 
                                  );
        }
    if( atts&EMPXMediaMusicGenre ) // Text
        {
        aDestination.SetTextValueL( KMPXMediaMusicGenre,
                     aSrc.ValueText(KMPXMediaMusicGenre ) 
                                  );
        }
    if( atts&EMPXMediaMusicRating )  // TInt
        {
        aDestination.SetTObjectValueL( KMPXMediaMusicRating,
             aSrc.ValueTObjectL<TInt>( KMPXMediaMusicRating ) 
                                     );   
        }
#ifdef ABSTRACTAUDIOALBUM_INCLUDED 
    if( atts&EMPXMediaMusicAlbumArtist ) // Text
        {
        aDestination.SetTextValueL( KMPXMediaMusicAlbumArtist,
                     aSrc.ValueText(KMPXMediaMusicAlbumArtist ) 
                                  );
        }
#endif
    MPX_DEBUG1("CMPXCollectionCachedHelper::DoAppendMusicL -->");     
    }

// ---------------------------------------------------------------------------
// Copies Audio media information
// ---------------------------------------------------------------------------
//  
void CMPXCollectionCachedHelper::DoAppendAudioL( CMPXMedia& aSrc, 
                                                 CMPXMedia& aDestination )
    {
    MPX_DEBUG1("CMPXCollectionCachedHelper::DoAppendAudioL <--");     
    TUint atts = aSrc.AttributesSet( KMPXMediaIdAudio );
    if( atts&EMPXMediaAudioBitrate ) // TInt
        {
        aDestination.SetTObjectValueL( KMPXMediaAudioBitrate,
             aSrc.ValueTObjectL<TInt>( KMPXMediaAudioBitrate ) 
                                     );
        }
    if( atts&EMPXMediaAudioSamplerate ) // TInt
        {
        aDestination.SetTObjectValueL( KMPXMediaAudioSamplerate,
             aSrc.ValueTObjectL<TInt>( KMPXMediaAudioSamplerate ) 
                                     );
        }
    if( atts&EMPXMediaAudioNumberOfChannels ) // TInt
        {
        aDestination.SetTObjectValueL( KMPXMediaAudioNumberOfChannels,
             aSrc.ValueTObjectL<TInt>( KMPXMediaAudioNumberOfChannels ) 
                                     );
        }
    if( atts&EMPXMediaAudioCodec ) // TInt
        {
        aDestination.SetTObjectValueL( KMPXMediaAudioAudioCodec,
             aSrc.ValueTObjectL<TInt>( KMPXMediaAudioAudioCodec ) 
                                     );
        }
    MPX_DEBUG1("CMPXCollectionCachedHelper::DoAppendAudioL -->");     
    }

// ---------------------------------------------------------------------------
// Copies DRM media information
// ---------------------------------------------------------------------------
//  
void CMPXCollectionCachedHelper::DoAppendDRML( CMPXMedia& aSrc, 
                                               CMPXMedia& aDestination )
    {
    MPX_DEBUG1("CMPXCollectionCachedHelper::DoAppendDRML <--");     
    TUint atts = aSrc.AttributesSet( KMPXMediaIdDrm );
    
    // Note: Only a subset used by MTP...
    //
    if( atts&EMPXMediaDrmProtected )  // TBool
        {
        aDestination.SetTObjectValueL( KMPXMediaDrmProtected,
            aSrc.ValueTObjectL<TBool>( KMPXMediaDrmProtected ) 
                                     );
        }
    if( atts&EMPXMediaDrmRightsStatus )  // TInt
        {
        aDestination.SetTObjectValueL( KMPXMediaDrmRightsStatus,
             aSrc.ValueTObjectL<TInt>( KMPXMediaDrmRightsStatus ) 
                                     );
        }
    MPX_DEBUG1("CMPXCollectionCachedHelper::DoAppendDRML -->");     
    }

// ---------------------------------------------------------------------------
// Copies array information
// ---------------------------------------------------------------------------
//  
void CMPXCollectionCachedHelper::DoAppendContainerL( CMPXMedia& aSrc,
                                                     CMPXMedia& aDestination )
    {
    MPX_FUNC("CMPXCollectionCachedHelper::DoAppendContainerL");     
    
    TUint atts = aSrc.AttributesSet( KMPXMediaIdContainer );
    
    if( atts&EMPXMediaArrayContents )
        {
        const CMPXMediaArray* medias =
            aSrc.Value<CMPXMediaArray>( KMPXMediaArrayContents );
        if( !medias )
            {
            User::Leave( KErrNoMemory );
            }
        
        aDestination.SetCObjectValueL(
            KMPXMediaArrayContents, 
            const_cast<CMPXMediaArray*>(medias));
        }
    if( atts&EMPXMediaArrayCount )  // TInt
        {
        aDestination.SetTObjectValueL( KMPXMediaArrayCount,
             aSrc.ValueTObjectL<TInt>( KMPXMediaArrayCount ) 
                                     );
        }
    }

// ---------------------------------------------------------------------------
// Copies mtp information
// ---------------------------------------------------------------------------
// 
void CMPXCollectionCachedHelper::DoAppendMTPL( CMPXMedia& aSrc, 
                                               CMPXMedia& aDestination )
    {
    MPX_FUNC("CMPXCollectionCachedHelper::DoAppendMTPL");
    
    TUint atts = aSrc.AttributesSet( KMPXMediaIdMTP );
    
    if( atts&KMPXMediaMTPDrmStatus.iAttributeId)
        {
        TUint16 val = aSrc.ValueTObjectL<TUint16>(KMPXMediaMTPDrmStatus);
        aDestination.SetTObjectValueL( KMPXMediaMTPDrmStatus, val );
        }
    }

// ---------------------------------------------------------------------------
// Attempt to set missing Metadata info in specific case
// ---------------------------------------------------------------------------
// 
void CMPXCollectionCachedHelper::SetMissingMetadataL(CMPXMedia* aMedia)
    {
    MPX_FUNC("CMPXCollectionCachedHelper::SetMissingMetadataL");
    
    const TDesC& path = aMedia->ValueText(KMPXMediaGeneralUri);
    TParsePtrC parse( path );

    // only do this for file that might not be natively supported by PC
    if ( ( parse.Ext().CompareF(K3GPFileExt) == 0 )
        || ( parse.Ext().CompareF(K3G2FileExt) == 0 )
        || ( parse.Ext().CompareF(KAACFileExt) == 0 )
        || ( parse.Ext().CompareF(KODFFileExt) == 0 )
        || ( parse.Ext().CompareF(KO4AFileExt) == 0 ) )
        {
        TBool isTitleMissing = EFalse;
        TBool isArtistMissing = EFalse;
        TBool isAlbumMissing = EFalse;

        if ( aMedia->ValueText( KMPXMediaGeneralTitle ).Length() == 0 )
            isTitleMissing = ETrue;
    
        if ( aMedia->ValueText( KMPXMediaMusicArtist ).Length() == 0 )
            isArtistMissing = ETrue;
    
        if ( aMedia->ValueText( KMPXMediaMusicAlbum ).Length() == 0 )
            isAlbumMissing = ETrue;

        MPX_DEBUG4("CMPXCollectionCachedHelper::SetMissingMetadataL, isTitleMissing = %d, isArtistMissing = %d, isAlbumMissing = %d", isTitleMissing, isArtistMissing, isAlbumMissing);

        // only do this if one of the following is missing:
        // - Title
        // - Artist
        // - Album
        // but can easily be extended to support any category field like: composer, genre
        if ( isTitleMissing || isArtistMissing || isAlbumMissing )
            {
            // metadataextractor should be used instead, but not until CreateMediaL can be called without add/TN of the embedded art
            // CR is needed to add parameter (TBool aAlbumArtNeeded = ETrue)  // ETrue by default, for all existing caller
            // for now use MDU instead
            CMetaDataUtility* metadataUtility = CMetaDataUtility::NewL();
            
            CleanupStack::PushL( metadataUtility );
            metadataUtility->OpenFileL( path );
            
            MPX_DEBUG1( "CMPXCollectionCachedHelper::SetMissingMetadataL, CMetaDataUtility::OpenFileL succeed" );
            
            const CMetaDataFieldContainer& metaCont = metadataUtility->MetaDataFieldsL();
        
            for ( TInt i = 0; i < metaCont.Count(); i++ )
                {
                TMetaDataFieldId fieldType;
                TMPXAttributeData attribute;
            
                metaCont.FieldIdAt( i, fieldType );  // get the field type
                
                switch ( fieldType )
                    {
                    case EMetaDataSongTitle:    // fall through
                    case EMetaDataArtist:       // fall through
                    case EMetaDataAlbum:
                        {
                        if ( ( isTitleMissing && ( fieldType == EMetaDataSongTitle ) )
                            || ( isArtistMissing && ( fieldType == EMetaDataArtist ) )
                            || ( isAlbumMissing && ( fieldType == EMetaDataAlbum ) ) )
                            {
                            HBufC* value = metaCont.At( i, fieldType ).AllocL();
                            CleanupStack::PushL( value );
                            
                            TPtr valptr = value->Des();
                            valptr.Trim();

                            // replace '\t' as ' '
                            for ( TInt i = 0; i < value->Length(); i++ )
                                {
                                if ( valptr[i] == TText( '\t' ) )
                                    valptr[i] = TText( ' ' );
                                }
                            
                            if ( fieldType == EMetaDataSongTitle )
                                attribute = KMPXMediaGeneralTitle;
                            else if ( fieldType == EMetaDataArtist )
                                attribute = KMPXMediaMusicArtist;
                            else
                                attribute = KMPXMediaMusicAlbum;

                            MPX_DEBUG3( "fieldType = %d, value = %S", fieldType, &valptr );
                            aMedia->SetTextValueL( attribute , *value );
                            CleanupStack::PopAndDestroy( value );                                
                            }
                        }

                        break;

                    default:

                        break;
                    }
                }

            metadataUtility->ResetL();
            CleanupStack::PopAndDestroy( metadataUtility );
            }
        }
    }
            
// END OF FILE
