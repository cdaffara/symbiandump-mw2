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
* Description: 
*
*/


#include <e32base.h>
#include <harvesterdata.h>

#include <mderelation.h>
#include <mdeobjectdef.h>

#include "harvesteraudioplaylistplugin.h"
#include "mdeobjectwrapper.h"
#include "mdsutils.h"
#include "harvestercommon.h"

#include "harvesterlog.h"

// for CleanupResetAndDestroyPushL
#include <mmf/common/mmfcontrollerpluginresolver.h>

CHarvesterAudioPlaylistPluginPropertyDefs::CHarvesterAudioPlaylistPluginPropertyDefs() : CBase()
    {
    }

void CHarvesterAudioPlaylistPluginPropertyDefs::ConstructL(CMdEObjectDef& aObjectDef)
    {
    CMdENamespaceDef& nsDef = aObjectDef.NamespaceDef();
    
    // Property definitions
    CMdEObjectDef& objectDef = nsDef.GetObjectDefL( MdeConstants::Object::KBaseObject );
    iCreationDatePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KCreationDateProperty );
    iLastModifiedDatePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KLastModifiedDateProperty );
    iSizePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KSizeProperty );
    iItemTypePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
    iTitlePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
    
    CMdEObjectDef& albumObjectDef = nsDef.GetObjectDefL( MdeConstants::Album::KAlbumObject );
    iAlbumTypeDef = &albumObjectDef.GetPropertyDefL( MdeConstants::Album::KTypeProperty );
    
    CMdEObjectDef& playlistObjectDef = nsDef.GetObjectDefL( MdeConstants::AudioPlaylistItem::KAudioPlaylistItem );
    iAlbumIDDef = &playlistObjectDef.GetPropertyDefL( MdeConstants::AudioPlaylistItem::KPlaylistIDProperty );
    iAudioObjectIDDef = &playlistObjectDef.GetPropertyDefL( MdeConstants::AudioPlaylistItem::KAudioObjectIDProperty );
    iPositionDef = &playlistObjectDef.GetPropertyDefL( MdeConstants::AudioPlaylistItem::KPositionProperty );
    }

CHarvesterAudioPlaylistPluginPropertyDefs* CHarvesterAudioPlaylistPluginPropertyDefs::NewL(CMdEObjectDef& aObjectDef)
    {
    CHarvesterAudioPlaylistPluginPropertyDefs* self = 
        new (ELeave) CHarvesterAudioPlaylistPluginPropertyDefs();
    CleanupStack::PushL( self );
    self->ConstructL( aObjectDef );
    CleanupStack::Pop( self );
    return self;
    }

using namespace MdeConstants;

// ---------------------------------------------------------------------------
// CHarvesterAudioPlugin::CHarvesterAudioPlugin
// ---------------------------------------------------------------------------
//    
CHarvesterAudioPlaylistPlugin::CHarvesterAudioPlaylistPlugin() : 
	CHarvesterPlugin()
	{
	}

// ---------------------------------------------------------------------------
// CHarvesterAudioPlaylistPlugin::NewL
// ---------------------------------------------------------------------------
//    
CHarvesterAudioPlaylistPlugin* CHarvesterAudioPlaylistPlugin::NewL()
	{
	WRITELOG( "CHarvesterAudioPlaylistPlugin::NewL()" );
	CHarvesterAudioPlaylistPlugin* self = new (ELeave) CHarvesterAudioPlaylistPlugin();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	
	return self;
	}

// ---------------------------------------------------------------------------
// CHarvesterAudioPlaylistPlugin::~CHarvesterAudioPlaylistPlugin
// ---------------------------------------------------------------------------
//    
CHarvesterAudioPlaylistPlugin::~CHarvesterAudioPlaylistPlugin()
	{
	WRITELOG( "CHarvesterAudioPlaylistPlugin::~CHarvesterAudioPlaylistPlugin()" );
	
	delete iPlaylistParser;
	delete iPropDefs;
	delete iMime;
	}

// ---------------------------------------------------------------------------
// CHarvesterAudioPlaylistPlugin::ConstructL
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlaylistPlugin::ConstructL()
	{
	WRITELOG( "CHarvesterAudioPlaylistPlugin::ConstructL()" );
	SetPriority( KHarvesterPriorityHarvestingPlugin - 1 );
	
	iPlaylistParser =  CHarvesterAudioPlaylistParser::NewL();
	}

// ---------------------------------------------------------------------------
// CHarvesterAudioPlaylistPlugin::HarvestL (from CHarvesterPlugin)
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlaylistPlugin::HarvestL( CHarvesterData* aHD )
	{
	WRITELOG( "CHarvesterAudioPlaylistPlugin::HarvestL()" );
	
	TRAPD( err, DoHarvestL( aHD ) );
	
	if ( err != KErrNone )
	    {
	    aHD->SetErrorCode( err );
	    }
	}

// ---------------------------------------------------------------------------
// CHarvesterAudioPlaylistPlugin::DoHarvestL
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlaylistPlugin::DoHarvestL( CHarvesterData* aHD )
	{
	WRITELOG( "CHarvesterAudioPlaylistPlugin::DoHarvestL()" );
    CMdEObject& mdeObject = aHD->MdeObject();
        
    TBool isAdd = EFalse;
    if ( mdeObject.Placeholder() || mdeObject.Id() == KNoId ) // is a new object or placeholder
        {
        isAdd = ETrue;
        }
    else
        {
        return; // Once playlist has been imported, changes made to it by Music Player
                     // will not get written back to original playlist file. Thus changes to playlist
                     // should be done within Music Player, and MDS will prevent playlist that
                     // have already been imported to be changes so that there will not be 
                     // several versions of same playlist imported by accident.
        }

    GetPropertiesL( aHD, isAdd );
	}


// ---------------------------------------------------------------------------
// CHarvesterAudioPlaylistPlugin::GetPropertiesL
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlaylistPlugin::GetPropertiesL( CHarvesterData* aHD,
                                            TBool aIsAdd )
    {
    CMdEObject& mdeObject = aHD->MdeObject();
    
    // get creation time, modified time and file size
    if( !mdeObject.Placeholder() )
        {
        GetPlaceHolderPropertiesL( aHD, aIsAdd );
        }
    
    const TDesC& uri = mdeObject.Uri();
    
    TBool isSupported( GetMimeTypePropertyL( aHD, aIsAdd ) );

    if( isSupported )
    	{
    	GetMusicPropertiesL( aHD, aIsAdd );
    	}
    }

// ---------------------------------------------------------------------------
// CHarvesterAudioPlaylistPlugin::GetPlaceHolderPropertiesL
// Get placeholder properties (creation time, modify time and file size).
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlaylistPlugin::GetPlaceHolderPropertiesL( CHarvesterData* aHD,
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
    
	TTime now;
	now.HomeTime();
	
    if( !iPropDefs )
        {
        CMdEObjectDef& objectDef = mdeObject.Def();
        iPropDefs = CHarvesterAudioPlaylistPluginPropertyDefs::NewL( objectDef );
        }
    
	CMdeObjectWrapper::HandleObjectPropertyL(
                 mdeObject, *iPropDefs->iCreationDatePropertyDef, &now, aIsAdd );

	CMdeObjectWrapper::HandleObjectPropertyL(
             mdeObject, *iPropDefs->iLastModifiedDatePropertyDef, &entry.iModified, aIsAdd );

	CMdeObjectWrapper::HandleObjectPropertyL(
              mdeObject, *iPropDefs->iSizePropertyDef, &entry.iSize, aIsAdd );
    }

// ---------------------------------------------------------------------------
// CHarvesterAudioPlaylistPlugin::GetMimeTypePropertyL
// Get mime type.
// ---------------------------------------------------------------------------
//    
TBool CHarvesterAudioPlaylistPlugin::GetMimeTypePropertyL( 
		CHarvesterData* aHD, TBool aIsAdd )
    {
    CMdEObject& mdeObject = aHD->MdeObject();
 
    TBuf<40> mimeType( KNullDesC );
    TBool supported( EFalse );
    supported = iPlaylistParser->ParseMimeType( mdeObject.Uri(), mimeType );
    delete iMime;
    iMime = NULL;
    iMime = mimeType.AllocL();
        
    if( !iPropDefs )
        {
        CMdEObjectDef& objectDef = mdeObject.Def();
        iPropDefs = CHarvesterAudioPlaylistPluginPropertyDefs::NewL( objectDef );
        }
    
    CMdeObjectWrapper::HandleObjectPropertyL( mdeObject, 
            *iPropDefs->iItemTypePropertyDef, (TAny*)&(mimeType), 
        	aIsAdd );
    
    return supported;
    }

// ---------------------------------------------------------------------------
// CHarvesterAudioPlaylistPlugin::GetMusicPropertiesL
// ---------------------------------------------------------------------------
//    
void CHarvesterAudioPlaylistPlugin::GetMusicPropertiesL( CHarvesterData* aHD,
                                      TBool aIsAdd )
    {
    CMdEObject& mdeObject = aHD->MdeObject();
    
    if( !iPropDefs )
        {
        CMdEObjectDef& objectDef = mdeObject.Def();
        iPropDefs = CHarvesterAudioPlaylistPluginPropertyDefs::NewL( objectDef );
        }
    
    CMdESession& session = mdeObject.Session();

#ifdef MDS_PLAYLIST_HARVESTING_ENABLED
	TInt playlistType( MdeConstants::Album::EAlbumMusicPlaylist );
	CMdeObjectWrapper::HandleObjectPropertyL( 
              mdeObject, *iPropDefs->iAlbumTypeDef, &playlistType, aIsAdd );
#endif
    
    RPointerArray<HBufC> playlistItems;
    TCleanupItem cleanupItem( MdsUtils::CleanupPtrArray<HBufC>, &playlistItems );
    CleanupStack::PushL( cleanupItem );
    
    const TBool parsed( iPlaylistParser->ParseL( mdeObject.Uri(), playlistItems, iMime->Des() ) ); 

    if( parsed )
        {
        TTime curtime( 0 );
        curtime.UniversalTime();

        const TTimeIntervalSeconds timeOffsetSeconds = User::UTCOffset();
        const TTime localTime = curtime + timeOffsetSeconds;
        
        RPointerArray<CMdEObject> objectArray;
        
        CMdENamespaceDef& nsDef = session.GetDefaultNamespaceDefL();
        CMdEObjectDef& playlistObjectDef = nsDef.GetObjectDefL( MdeConstants::AudioPlaylistItem::KAudioPlaylistItem );
        
        const TInt count( playlistItems.Count() );
        for( TInt i = 0; i < count; i++)
            {
            CMdEObject* mdeAudioObject = session.GetObjectL( playlistItems[i]->Des() );
            if( mdeAudioObject )
                {
                CMdEObject* object = session.NewObjectLC( playlistObjectDef, Object::KAutomaticUri );
                
                object->AddTimePropertyL( *iPropDefs->iCreationDatePropertyDef, localTime );
                object->AddTimePropertyL( *iPropDefs->iLastModifiedDatePropertyDef, curtime );
                object->AddUint32PropertyL( *iPropDefs->iSizePropertyDef, 0 );
                object->AddTextPropertyL( *iPropDefs->iItemTypePropertyDef, AudioPlaylistItem::KAudioPlaylistItemType );
                
                object->AddInt32PropertyL( *iPropDefs->iAlbumIDDef, mdeObject.Id() );
                object->AddInt32PropertyL( *iPropDefs->iAudioObjectIDDef, mdeAudioObject->Id() );
                
                // position is considered to represent items relative position in the list
                // if item is removed from the middle of the list, position is still valid even though
                // the actual position value is not updated
                object->AddInt32PropertyL( *iPropDefs->iPositionDef, (i+1) ); // playlist position starts from 1
                objectArray.AppendL( object );
                CleanupStack::Pop(); // object
                }
            delete mdeAudioObject;
            }
        CleanupResetAndDestroyPushL( objectArray );
        TRAPD( addError, session.AddObjectsL( objectArray ) );
        if ( addError != KErrNone )
            {
            WRITELOG1( "CHarvesterAudioPlaylistPlugin::GetMusicPropertiesL - Add items to MDE error: %d", addError );
            }
        CleanupStack::PopAndDestroy( &objectArray );
        }

    CleanupStack::PopAndDestroy( &playlistItems );
    }

// End of file

