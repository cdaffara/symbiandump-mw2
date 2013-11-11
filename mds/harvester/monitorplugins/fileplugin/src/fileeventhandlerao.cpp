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
* Description:  Handles monitored file creations, modifications and deletions.*
*/

#include <collate.h>
#include <mdeobject.h>
#include <pathinfo.h>

#include "fileeventhandlerao.h"
#include "harvesterlog.h"
#include "fsutil.h"
#include "mdeconstants.h"
#include "mdeharvestersession.h"
#include "processoriginmapper.h"
#include "mdenamespacedef.h"
#include "mdeobjectdef.h"
#include "FolderRenamer.h"
#include "mdsutils.h"
#include "harvesterpluginfactory.h"
#include "mdsfspqueue.h"
#include "harvestercommon.h"
#include <centralrepository.h>

using namespace MdeConstants;

const TUid KRepositoryUid = { 0x20007182 };
const TUint32 KCacheSizeKey = 0x00000001;

const TInt KMaxEventsAtTime = 25;
const TInt KMaxEventsGranularity = 20;

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::NewL()
// ---------------------------------------------------------------------------
//
CFileEventHandlerAO* CFileEventHandlerAO::NewL( MMonitorPluginObserver& aObserver,
		CMdEHarvesterSession* aMdeSession, CHarvesterPluginFactory* aHarvesterPluginFactory )
    {
    WRITELOG( "CFileEventHandlerAO::NewL" );

    CFileEventHandlerAO* self = new (ELeave) CFileEventHandlerAO;
    CleanupStack::PushL( self );
    self->ConstructL( aObserver, aMdeSession, aHarvesterPluginFactory );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::ConstructL()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::ConstructL( MMonitorPluginObserver& aObserver,
		CMdEHarvesterSession* aMdeSession, CHarvesterPluginFactory* aHarvesterPluginFactory )
    {
    WRITELOG( "CFileEventHandlerAO::ConstructL" );
    iObserver = &aObserver;
    iHarvesterPluginFactory = aHarvesterPluginFactory;
    iMdeSession = aMdeSession;
    iNextRequest = ERequestIdle;
    iCacheEvents = EFalse;
    
    CActiveScheduler::Add( this );
    iMapper = CProcessOriginMapper::NewL();
    iMoveTimer = CMoveTimer::NewL(this);
    iFolderRenamer = CFolderRenamer::NewL(*this);
    TRAPD( error, iMapper->ReadFileL() );
    if ( error != KErrNone )
    	{
    	WRITELOG1( "CFileEventHandlerAO::ConstructL - iMapper->ReadFileL() error %d", error );
    	}
    error = iFs.Connect();
    User::LeaveIfError( error );
    
    TRAP( error, ReadCacheSizeFromCenrepL() );
	
    if ( error == KErrNone )
    	{
    	iQueue.Reserve( iCacheSize );
    	}
    
    iEventArray = new (ELeave) CArrayFixSeg< TMdsFSPStatus >( KMaxEventsGranularity );
    
    TFileName phoneRoot = PathInfo::PhoneMemoryRootPath();
    TFileName mmcRoot = PathInfo::MemoryCardRootPath();
    
    TFileName images = PathInfo::ImagesPath();
    
    TFileName phoneImagePath( phoneRoot );
    phoneImagePath.Append( images );
    iPhoneImagesPath = phoneImagePath.AllocL();

    TFileName mmcImagePath( mmcRoot );
    mmcImagePath.Append( images );
    iMmcImagesPath = mmcImagePath.Right( mmcImagePath.Length() - 1 ).AllocL();
    
    TFileName videos = PathInfo::VideosPath();
    
    TFileName phoneVideoPath( phoneRoot );
    phoneVideoPath.Append( videos );
    iPhoneVideosPath = phoneVideoPath.AllocL();

    TFileName mmcVideoPath( mmcRoot );
    mmcVideoPath.Append( videos );
    iMmcVideosPath = mmcVideoPath.Right( mmcVideoPath.Length() - 1 ).AllocL();
    
    TFileName sounds = PathInfo::SoundsPath();
    
    TFileName phoneSoundPath( phoneRoot );
    phoneSoundPath.Append( sounds );
    iPhoneSoundsPath = phoneSoundPath.AllocL();

    TFileName mmcSoundPath( mmcRoot );
    mmcSoundPath.Append( sounds );
    iMmcSoundsPath = mmcSoundPath.Right( mmcSoundPath.Length() - 1 ).AllocL();
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::~CFileEventHandlerAO()
// ---------------------------------------------------------------------------
//
CFileEventHandlerAO::~CFileEventHandlerAO()
    {
    WRITELOG( "CFileEventHandlerAO::~CFileEventHandlerAO" );
    
    Cancel();
    
    iFs.Close();
    
    delete iMapper;
    iMapper = NULL;
    
    delete iMoveTimer;
    iMoveTimer = NULL;
    
    delete iFolderRenamer;
    iFolderRenamer = NULL;
    
    delete iEventArray;
    iEventArray = NULL;
    
    iQueue.ResetAndDestroy();
    iQueue.Close();
	
    iUriArray.ResetAndDestroy();
    iUriArray.Close();
    
    delete iPhoneImagesPath;
	iPhoneImagesPath = NULL;
    delete iMmcImagesPath;
	iMmcImagesPath = NULL;
    
    delete iPhoneVideosPath;
	iPhoneVideosPath = NULL;
    delete iMmcVideosPath;
	iMmcVideosPath = NULL;
    
    delete iPhoneSoundsPath;
	iPhoneSoundsPath = NULL;
    delete iMmcSoundsPath;
	iMmcSoundsPath = NULL;
    }



// ---------------------------------------------------------------------------
// CFileEventHandlerAO::RunL()
// From CActive
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::RunL()
    {
    WRITELOG( "CFileEventHandlerAO::RunL" );
    switch( iNextRequest )
    	{
    	case ( ERequestIdle ):
    		{
    	    iQueue.Compress();
    	    iEventArray->Compress();
    	    iUriArray.Compress();
        	break;
    		}
    	
    	case ( ERequestProcessing ):
    		{
    		if( iQueue.Count() > 0 && !iCacheEvents )
    			{
    			CMdsFSPQueueItem* item = iQueue[0];
    			iQueue.Remove( 0 );

    			ResetEvent();
    			item->GetAsFspStatus(iEvent);
    			delete item;
    			item = NULL;
    			
    			if( iEvent.iFileEventType == EMdsFileDeleted )
    			    {
    			    iEventArray->AppendL( iEvent );
    			    TInt limit( KMaxEventsAtTime );
    			    const TInt count( iQueue.Count() );
    			    if( count >= KMaxEventsAtTime )
    			        {
    			        limit = KMaxEventsAtTime;
    			        }
    			    else
    			        {
    			        limit = count;
    			        }
    			    
    	            for( TInt i( 0 ); i < limit; i++ )
    	                {
    	                CMdsFSPQueueItem* tempItem = iQueue[0];
    	                TMdsFSPStatus status;
    	                tempItem->GetAsFspStatus( status );
    	                if( status.iFileEventType == EMdsFileDeleted  )
    	                    {
    	                    iQueue.Remove( 0 );
    	                    delete tempItem;
    	                    tempItem = NULL;
    	                    iEventArray->AppendL( status );
    	                    }
    	                else
    	                    {
    	                    break;
    	                    }
    	                }
    			    }
    			
    			if( iEventArray->Count() > 1 )
    			    {
    			    iFolderRenamer->HandleFileEventsL(iEventArray);
    			    }
    			else
    			    {
    			    iFolderRenamer->HandleFileEventL(iEvent);
    			    }
    			
    			SetNextRequest( ERequestProcessing );
    			}
    		else
    			{
    			SetNextRequest( ERequestIdle );
    			}
        	break;
    		}
    	default:
    		break;
    	}
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::HandleNotificationL()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::HandleNotificationL(TMdsFSPStatus &aEvent)
    {
    WRITELOG( "CFileEventHandlerAO::HandleNotificationL" );
    
    TMdsFSPStatus& status = aEvent;
    
    if ( !iObserver )
        {
        WRITELOG( "-- ERROR -- CFileEventHandlerAO::HandleNotificationL - no observer" );
        return;
        }
    
    TBool fastHarvest( EFalse );
    
    TOrigin origin = MdeConstants::Object::EOther;
    if ( iMapper )
        {
        origin = iMapper->OriginL( status.iProcessId );
        if ( origin == KOriginIgnored )
            {
            WRITELOG1( "CFileEventHandlerAO::HandleNotificationL - ignored origin for %S", &status.iFileName );
            return;
            }
        else if( origin == KOriginFastHarvest )
            {
            WRITELOG( "CFileEventHandlerAO::HandleNotificationL - potential fast harvest file detected" );
            fastHarvest = ETrue;
            }
    
        _LIT(KCameraTemp,"camcordertmp");
        if ( origin == MdeConstants::Object::ECamera &&
        		(aEvent.iFileEventType == EMdsFileCreated || 
				aEvent.iFileName.FindF(KCameraTemp) != KErrNotFound) )        		
            {
            WRITELOG1( "CFileEventHandlerAO::HandleNotificationL - ignored camera origin for %S", &status.iFileName );
            return;
            }
        
        if( (origin == KOriginIgnoreAttribsChanged ||
             origin == MdeConstants::Object::ECamera ) &&
            aEvent.iFileEventType == EMdsFileAttribsChanged )
            {
            WRITELOG1( "CFileEventHandlerAO::HandleNotificationL - ignored attribs changed event for %S", &status.iFileName );
            return;        
            }
        }

    // If internal origin value is used for evaluation, set it default value
    if( origin == KOriginIgnoreAttribsChanged ||
        origin == KOriginFastHarvest )
        {
        origin = MdeConstants::Object::EOther;
        }
    
    // ignore created file event if extension is not supported by any harverter plugin
    if( EMdsFileCreated == status.iFileEventType && 
    		status.iFileName.Length() > 0 )
    	{
    	if( !iHarvesterPluginFactory->IsSupportedFileExtension( 
    			status.iFileName ) )
    		{
    		WRITELOG1( "CFileEventHandlerAO::HandleNotificationL - file extension not supported: %S", &status.iFileName );
    		return;
    		}
    	}

    switch( status.iFileEventType )
        {
        case EMdsFileCreated:
            {
            WRITELOG1( "CFileEventHandlerAO::HandleNotificationL - EmdsFileCreated: %S", &status.iFileName );
            HBufC* fn = status.iFileName.AllocLC();
            CHarvesterData* hd = CHarvesterData::NewL( fn );
            CleanupStack::Pop( fn );
            hd->SetEventType( EHarvesterAdd );
            hd->SetOrigin( origin );
            CleanupStack::PushL( hd );
            iMoveTimer->AddHarvesterDataL( hd );
            CleanupStack::Pop( hd );
            }
        break;
        
        case EMdsFileRenamed:
            {
            WRITELOG2( "CFileEventHandlerAO::HandleNotificationL - EMdsFileRenamed: %S to %S", &status.iFileName, &status.iNewFileName );
            RenameToMDEL( status.iFileName, status.iNewFileName, origin );
            }
        break;
        
        case EMdsFileModified:
        case EMdsFileAttribsChanged:
            {
            WRITELOG1( "CFileEventHandlerAO::HandleNotificationL - EmdsFileModified: %S", &status.iFileName );
            ModifyL( status.iFileName, origin, fastHarvest );
            }
        break;
        
        case EMdsFileReplaced:
            {
            WRITELOG2( "CFileEventHandlerAO::HandleNotificationL - EMdsFileReplaced: %S to %S", &status.iFileName, &status.iNewFileName );
            ReplaceL( status.iFileName, status.iNewFileName, origin, fastHarvest );
            }
        break;
        
        case EMdsFileDeleted:
            {
            WRITELOG1( "CFileEventHandlerAO::HandleNotificationL - EmdsFileDeleted: %S", &status.iFileName );
            CHarvesterData* hd = iMoveTimer->CheckUriL( status.iFileName );
            if ( hd )
            	{
            	// move event
            	CleanupStack::PushL( hd );
            	RenameToMDEL( status.iFileName, hd->Uri(), hd->Origin() );
            	CleanupStack::PopAndDestroy( hd );
            	}
            else
                {
                DeleteFromMDEL( status.iFileName );
                }
            }
        break;

        case EMdsDriveFormatted:
        	{
        	WRITELOG1( "CFileEventHandlerAO::HandleNotificationL - EMdsDriveFormatted: %d", status.iDriveNumber );
        	// format drive
        	FormatL( status.iDriveMediaId, status.iProcessId.iUid != 0 );
        	}
        break;
        
        default:
            WRITELOG1( "CFileEventHandlerAO::HandleNotificationL - KErrNotSupported %d", status.iFileEventType );
            User::Leave( KErrNotSupported );
        break;
        }
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::HandleMultideletionL()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::HandleMultideletionL( CArrayFixSeg< TMdsFSPStatus >* aEvents )
    {
    WRITELOG( "CFileEventHandlerAO::HandleMultideletionL" );
    
    if ( !iObserver )
        {
        WRITELOG( "-- ERROR -- CFileEventHandlerAO::HandleMultideletionL - no observer" );
        return;
        }
    
    const TInt count( aEvents->Count() );
    for( TInt i( 0 ); i < count; i++ )
        {
        TMdsFSPStatus& status = (*aEvents)[i];
        
        TOrigin origin = MdeConstants::Object::EOther;
        if ( iMapper )
            {
            origin = iMapper->OriginL( status.iProcessId );
            // Used only for delete events so fast harvest origin is not used
            if ( origin == KOriginIgnored )
                {
                WRITELOG1( "CFileEventHandlerAO::HandleMultideletionL - ignored origin for %S", &status.iFileName );
                continue;
                }
            }
        
        WRITELOG1( "CFileEventHandlerAO::HandleMultideletionL - EmdsFileDeleted: %S", &status.iFileName );
        CHarvesterData* hd = iMoveTimer->CheckUriL( status.iFileName );
        if ( hd )
            {
            // move event
            CleanupStack::PushL( hd );
            RenameToMDEL( status.iFileName, hd->Uri(), hd->Origin() );
            CleanupStack::PopAndDestroy( hd );
            }
        else
            {
            iUriArray.Append( &(status.iFileName) );
            }
        }
    MultiDeleteFromMDEL( iUriArray );
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::ResetMdsFSPStatus()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::ResetEvent()
    {
    WRITELOG( "CFileEventHandlerAO::ResetEvent" );
    
    iEvent.iDriveNumber = 0;
    iEvent.iFileEventType = EMdsFileUnknown;
    iEvent.iFileName.Zero();
    iEvent.iNewFileName.Zero();
    iEvent.iProcessId = TUid::Null();
    
    iEventArray->Reset();
    iUriArray.Reset();
    }
    
// ---------------------------------------------------------------------------
// CFileEventHandlerAO::RenameToMDEL()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::RenameToMDEL( const TDesC& aOldUrl, 
		const TDesC& aNewUrl, TOrigin aOrigin )
    {
    WRITELOG( "CFileEventHandlerAO::RenameToMDEL" );

    CMdEObject* oldObject = iMdeSession->Session()->OpenObjectL( aOldUrl );
    if ( oldObject )
        {
        CleanupStack::PushL( oldObject );
        }

    CMdEObject* newObject = iMdeSession->Session()->GetObjectL( aNewUrl );
    if ( newObject )
        {
        CleanupStack::PushL( newObject );
        }

    // check if not in mde, harvest
    if ( !oldObject && !newObject && iObserver )
        {
        HBufC* fn = aNewUrl.AllocLC();
        CHarvesterData* hd = CHarvesterData::NewL( fn );
        CleanupStack::Pop( fn );
        hd->SetEventType( EHarvesterAdd );
        hd->SetOrigin( aOrigin );
        iObserver->MonitorEvent( hd );
        }

    // set old url to new url
    if ( oldObject && !newObject )
        {
        SetTitleL( oldObject , aNewUrl );
        oldObject->SetUriL( aNewUrl );
        TUint32 mediaId = FSUtil::MediaID( iFs, aNewUrl );
        oldObject->SetMediaId( mediaId );
        }

    // if old and new then destroy new and change old
    if ( oldObject && newObject && ( oldObject->Id() != newObject->Id()) )
        {
        const TItemId removedId = iMdeSession->Session()->RemoveObjectL( 
        		newObject->Id() );
        if ( removedId != KNoId )
        	{
            SetTitleL( oldObject , aNewUrl );
	        oldObject->SetUriL( aNewUrl );
	        TUint32 mediaId = FSUtil::MediaID( iFs, aNewUrl );
	        oldObject->SetMediaId( mediaId );
        	}
        }

    if ( newObject )
        {
        CleanupStack::PopAndDestroy( newObject );
        }

    if ( oldObject )
        {
        CheckDefaultFolderL( oldObject );
        SetModifiedTimeL( oldObject, aNewUrl );
        TOrigin origin = OriginFromMdEObjectL( *oldObject );
        if( origin == MdeConstants::Object::EOther)
        	{
        	OriginToMdEObjectL( *oldObject, aOrigin );
        	}
        iMdeSession->Session()->CommitObjectL( *oldObject );
        CleanupStack::PopAndDestroy( oldObject );
        }
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::OriginFromMdEObject()
// ---------------------------------------------------------------------------
//
TOrigin CFileEventHandlerAO::OriginFromMdEObjectL( CMdEObject& aObject )
	{
	TOrigin retVal = MdeConstants::Object::EOther;

	if( !iOriginPropertyDef )
		{
		CMdEObjectDef& objDef = aObject.Def();
		iOriginPropertyDef = &objDef.GetPropertyDefL( Object::KOriginProperty );
		}

	CMdEProperty* property = NULL;
	TInt err = aObject.Property( *iOriginPropertyDef, property );
	if( err >= KErrNone && property )
		{
		CMdEUint8Property* uint8Property = (CMdEUint8Property*)property;
		retVal = (TOrigin) uint8Property->Value();
		}
	 
	return retVal;
	}

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::OriginFromMdEObject()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::OriginToMdEObjectL( CMdEObject& aObject, 
		TOrigin aOrigin )
	{
	if( !iOriginPropertyDef )
		{
		CMdEObjectDef& objDef = aObject.Def();
		iOriginPropertyDef = &objDef.GetPropertyDefL( 
				Object::KOriginProperty );
		}

	CMdEProperty* property = NULL;
	TInt err = aObject.Property( *iOriginPropertyDef, property );
	if(err < KErrNone)
		{
		return;
		}

	if( property )
		{
		CMdEUint8Property* uint8Property = (CMdEUint8Property*)property;
		TRAP_IGNORE( uint8Property->SetValueL( aOrigin ) );
		}
	else
		{
		TRAP_IGNORE( aObject.AddUint8PropertyL( *iOriginPropertyDef, 
				(TUint8) aOrigin ) );
		}
	}
    
// ---------------------------------------------------------------------------
// CFileEventHandlerAO::ReplaceL()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::ReplaceL( const TDesC& aOldUrl, const TDesC& aNewUrl,
		TOrigin aOrigin,  TBool aFastHarvest )
    {
    WRITELOG( "CFileEventHandlerAO::ReplaceL" );

    if ( !iObserver )
        {
        User::Leave( KErrGeneral );
        }

    CMdEObject* newObject = NULL;
    CMdEObject* oldObject = NULL;
    
    if ( aNewUrl.Length() > 0 )
    	{
    	newObject = iMdeSession->Session()->GetObjectL( aNewUrl );
    	if ( newObject )
    		{
    		CleanupStack::PushL( newObject );
    		}
    	}
    
    if ( aOldUrl.Length() > 0 )
    	{
    	oldObject = iMdeSession->Session()->GetObjectL( aOldUrl );
    	if ( oldObject )
    		{
    		CleanupStack::PushL( oldObject );
    		}
    	}
    
    // check if not in mde, harvest
    if ( !oldObject && !newObject )
        {
        HBufC* fn = NULL; 
    
        // ignore created file event if extension is not supported by any harverter plugin
        if( aNewUrl.Length() > 0 )
            {
            if( iHarvesterPluginFactory->IsSupportedFileExtension( aNewUrl ) )
                {
                fn = aNewUrl.AllocLC();
                }
            else
                {
                WRITELOG1( "CFileEventHandlerAO::ReplaceL - file extension not supported: %S", &aNewUrl );
                return;           
                }
            }
        else
            {
            if( iHarvesterPluginFactory->IsSupportedFileExtension( aOldUrl ) )
                {
                fn = aOldUrl.AllocLC();
                }
            else
                {
                WRITELOG1( "CFileEventHandlerAO::ReplaceL - file extension not supported: %S", &aOldUrl );
                return;           
                }        
            }

        CHarvesterData* hd = CHarvesterData::NewL( fn );
        CleanupStack::Pop( fn );
        hd->SetEventType( EHarvesterAdd );
        hd->SetOrigin( aOrigin );
        if( aFastHarvest )
            {
            hd->SetObjectType( EFastHarvest );
            }
        iObserver->MonitorEvent( hd );
        }

    if ( oldObject )
        {
        // check if same then harvest modify
        if ( aNewUrl.Length() == 0 || MdsUtils::Compare( aNewUrl, aOldUrl ) == 0 )
            {
            HBufC* fn = aOldUrl.AllocLC();
            CHarvesterData* hd = CHarvesterData::NewL( fn );
            CleanupStack::Pop( fn );
            hd->SetEventType(EHarvesterEdit);
            hd->SetOrigin( aOrigin );
            if( aFastHarvest )
                {
                hd->SetObjectType( EFastHarvest );
                }
            iObserver->MonitorEvent( hd );
            }
        else if ( newObject )
        	{
        	// both are existing so delete new and change uri of old one
            const TItemId removedId = iMdeSession->Session()->RemoveObjectL( 
            		newObject->Id() );
            if ( removedId != KNoId )
            	{
            	const TItemId oldObjectId = oldObject->Id();
            	CleanupStack::PopAndDestroy( oldObject );
            	oldObject = iMdeSession->Session()->OpenObjectL( oldObjectId );
                if ( oldObject )
                    {
                    CleanupStack::PushL( oldObject );
                    SetTitleL( oldObject , aNewUrl );
        	        oldObject->SetUriL( aNewUrl );
        	        CheckDefaultFolderL( oldObject );
        	        TUint32 mediaId = FSUtil::MediaID( iFs, aNewUrl );
        	        oldObject->SetMediaId( mediaId );
        	        SetModifiedTimeL( oldObject, aNewUrl );
        	        TOrigin origin = OriginFromMdEObjectL( *oldObject );
        	        if( origin == MdeConstants::Object::EOther)
        	        	{
        	        	OriginToMdEObjectL( *oldObject, aOrigin );
        	        	}
                    iMdeSession->Session()->CommitObjectL( *oldObject );
                    }
            	}
        	}
        else
        	{
        	// case is actually rename
        	const TItemId oldObjectId = oldObject->Id();
        	CleanupStack::PopAndDestroy( oldObject );
        	oldObject = iMdeSession->Session()->OpenObjectL( oldObjectId );
            if ( oldObject )
                {
                CleanupStack::PushL( oldObject );
                SetTitleL( oldObject , aNewUrl );
    	        oldObject->SetUriL( aNewUrl );
    	        CheckDefaultFolderL( oldObject );
    	        TUint32 mediaId = FSUtil::MediaID( iFs, aNewUrl );
    	        oldObject->SetMediaId( mediaId );
    	        SetModifiedTimeL( oldObject, aNewUrl );
    	        TOrigin origin = OriginFromMdEObjectL( *oldObject );
    	        if( origin == MdeConstants::Object::EOther)
    	        	{
    	        	OriginToMdEObjectL( *oldObject, aOrigin );
    	        	}
                iMdeSession->Session()->CommitObjectL( *oldObject );
                }
        	}
        }
    else if ( newObject )
    	{
    	// if we replace file from not phone location (e.g. from PC)
    	// where it create some temporary file and replace aNewUrl file
       	WRITELOG( "CFileEventHandlerAO::ReplaceL - Different filenames... still try to harvest new one as modify" );
       	HBufC* fn = aNewUrl.AllocLC();
       	CHarvesterData* hd = CHarvesterData::NewL( fn );
       	CleanupStack::Pop( fn );
        hd->SetEventType( EHarvesterEdit );
        hd->SetOrigin( aOrigin );
        if( aFastHarvest )
            {
            hd->SetObjectType( EFastHarvest );
            }
       	iObserver->MonitorEvent( hd );
    	}

    if ( oldObject )
        {
        CleanupStack::PopAndDestroy( oldObject );
        }

    if ( newObject )
        {
        CleanupStack::PopAndDestroy( newObject );
        }
    }
    
// ---------------------------------------------------------------------------
// CFileEventHandlerAO::ModifyL()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::ModifyL( const TDesC& aUrl, TOrigin aOrigin, TBool aFastHarvest )
    {
    WRITELOG( "CFileEventHandlerAO::ModifyL" );
    
    CMdEObject* oldObject = iMdeSession->Session()->GetObjectL( aUrl );
    
    // check if not in mde, harvest
    if ( !oldObject && iObserver )
        {
        HBufC* fn = aUrl.AllocLC();
        CHarvesterData* hd = CHarvesterData::NewL( fn );
        CleanupStack::Pop( fn );
        hd->SetEventType( EHarvesterAdd );
        hd->SetOrigin( aOrigin );
        if( aFastHarvest )
            {
            hd->SetObjectType( EFastHarvest );
            }
        iObserver->MonitorEvent( hd );
        }
    else if ( iObserver )
        {
        HBufC* fn = aUrl.AllocLC();
        CHarvesterData* hd = CHarvesterData::NewL( fn );
        CleanupStack::Pop( fn );
        hd->SetEventType( EHarvesterEdit );
        hd->SetOrigin( aOrigin );
        if( aFastHarvest )
            {
            hd->SetObjectType( EFastHarvest );
            }
        iObserver->MonitorEvent( hd );
        }
        
    if ( oldObject )
        {
        delete oldObject;
        oldObject = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::DeleteFromMDEL()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::DeleteFromMDEL( const TDesC& aUrl )
    {
    WRITELOG( "CFileEventHandlerAO::DeleteFromMDEL" );

   	iMdeSession->Session()->RemoveObjectL( aUrl );
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::MultiDeleteFromMDEL()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::MultiDeleteFromMDEL( const RPointerArray<TDesC>& aUrls )
    {
    WRITELOG( "CFileEventHandlerAO::MultiDeleteFromMDEL" );

    RArray<TItemId> results;
    CleanupClosePushL( results );
    iMdeSession->Session()->RemoveObjectsL( aUrls, results );
    CleanupStack::PopAndDestroy( &results );
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::FormatL()
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::FormatL( TUint32 aOldMediaId, TBool aSubClose )
	{
	WRITELOG2( "CFileEventHandlerAO::FormatL - old media ID %u subclose %d", 
			aOldMediaId, aSubClose );

	if ( aOldMediaId )
		{
		iMdeSession->SetFilesToNotPresent( aOldMediaId );
		if ( aSubClose )
			{
			iMdeSession->RemoveFilesNotPresent( aOldMediaId );
			}
		}
	}

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::RunError()
// From CActive
// ---------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CFileEventHandlerAO::RunError( TInt aError )
#else
TInt CFileEventHandlerAO::RunError( TInt /*aError*/ )
#endif
    {
    WRITELOG1( "CFileEventHandlerAO::RunError %d", aError );

    SetNextRequest( ERequestProcessing );

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::DoCancel()
// From CActive
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::DoCancel()
    {
    WRITELOG( "CFileEventHandlerAO::DoCancel" );

    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::CFileEventHandlerAO()
// Constructor
// ---------------------------------------------------------------------------
//
CFileEventHandlerAO::CFileEventHandlerAO() : 
		CActive( KHarvesterPriorityMonitorPlugin )
    {
    }

// ---------------------------------------------------------------------------
// CFileEventHandlerAO::Mapper()
// Returns a handle to CProcessOriginMapper.
// ---------------------------------------------------------------------------
//
CProcessOriginMapper& CFileEventHandlerAO::Mapper()
    {
    return *iMapper;
    }


// ---------------------------------------------------------------------------
// CFileEventHandlerAO::NotMoveEvent()
// Handles other than move events in move case (rename). If the case is not
// move, then new file must be created.
// ---------------------------------------------------------------------------
//
void CFileEventHandlerAO::NotMoveEvent(RPointerArray<CHarvesterData>& aHDArray)
	{
	TInt count = aHDArray.Count();
	for (TInt i = 0; i < count; ++i)
		{
		CHarvesterData* hd = aHDArray[i];
        iObserver->MonitorEvent( hd );
		}
	}

    
CMdEHarvesterSession * CFileEventHandlerAO::MdeHarvesterSession()
    {
    return iMdeSession;
    }


void CFileEventHandlerAO::AddToQueueL( TMdsFSPStatus& aEvent )
	{
	CMdsFSPQueueItem* item = CMdsFSPQueueItem::NewL(aEvent);
	iQueue.AppendL(item);
	if( iNextRequest == ERequestIdle && !iCacheEvents )
		{
		SetNextRequest( ERequestProcessing );
		}
	}

void CFileEventHandlerAO::SetNextRequest( TRequest aRequest )
    {
    WRITELOG( "CFileEventHandlerAO::SetNextRequest" );
    iNextRequest = aRequest;
            
    if ( !IsActive() )
        {
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* ptrStatus = &iStatus;
        User::RequestComplete( ptrStatus, KErrNone );
        }
    }


void CFileEventHandlerAO::SetCachingStatus( TBool aCachingStatus )
	{
	iCacheEvents = aCachingStatus;
	if ( iCacheEvents )
		{
		SetNextRequest( ERequestIdle );
		}
	else if ( iQueue.Count() > 0 )
		{
		SetNextRequest( ERequestProcessing );
		}
	}

void CFileEventHandlerAO::ReadCacheSizeFromCenrepL()
	{
	CRepository* repo = CRepository::NewLC( KRepositoryUid );
	User::LeaveIfError( repo->Get( KCacheSizeKey, iCacheSize ) );
	CleanupStack::PopAndDestroy( repo );
	}

//---------------------------------------------------------------------------
// CFileEventHandlerAO::SetTitle()
// Set MdE object's title with aNewUrl
// ---------------------------------------------------------------------------
//   
void CFileEventHandlerAO::SetTitleL( CMdEObject* aOldObject, const TDesC& aNewUrl )
    {
    TPtrC newName;
    if( MdsUtils::GetName( aNewUrl, newName ) )
        {
        if( !iTitlePropertyDef )
        	{
        	iTitlePropertyDef = &aOldObject->Def().GetPropertyDefL( 
        			MdeConstants::Object::KTitleProperty );
        	}
        
        CMdEProperty* titleProp = NULL;
        aOldObject->Property( *iTitlePropertyDef, titleProp );
        if ( titleProp )
            {
            TPtrC oldName;
            if( MdsUtils::GetName( aOldObject->Uri(), oldName ) )
            	{
            	// update title property only, 
            	// if title property is same as the old name and
            	// the new name is not same as the old name
            	if( MdsUtils::Compare( oldName, titleProp->TextValueL() ) == 0 && 
            		MdsUtils::Compare( newName, oldName ) != 0 )
            		{
            		titleProp->SetTextValueL( newName );
            		}
				}
            }
        else
            {
            // add title property, if it doesn't exist 
            aOldObject->AddTextPropertyL( *iTitlePropertyDef , newName );
            }
        }
    }

//---------------------------------------------------------------------------
// CFileEventHandlerAO::SetModifiedTimeL()
// ---------------------------------------------------------------------------
//   
void CFileEventHandlerAO::SetModifiedTimeL( CMdEObject* aOldObject, const TDesC& aNewUrl )
    {
    TTime time;
    
    const TInt errorcode = iFs.Modified( aNewUrl, time );
    if ( errorcode != KErrNone )
        {
        return;
        }
    
    if( !iTimePropertyDef )
        {
        iTimePropertyDef = &aOldObject->Def().GetPropertyDefL( 
                       MdeConstants::Object::KLastModifiedDateProperty );
        }
       
    CMdEProperty* timeProp = NULL;
    aOldObject->Property( *iTimePropertyDef, timeProp );
    if( timeProp )
        {
        timeProp->SetTimeValueL( time );
        }
    else
        {
        aOldObject->AddTimePropertyL( *iTimePropertyDef , time );
        }
    }

//---------------------------------------------------------------------------
// CFileEventHandlerAO::CheckDefaultFolderL()
// ---------------------------------------------------------------------------
//   
void CFileEventHandlerAO::CheckDefaultFolderL( CMdEObject* aOldObject )
    {
    if( !iDefaultFolderPropertyDef )
        {
        iDefaultFolderPropertyDef = &aOldObject->Def().GetPropertyDefL( 
                       MdeConstants::Object::KInDefaultFolder );
        }
    
    CMdEProperty* folderProp = NULL;
    aOldObject->Property( *iDefaultFolderPropertyDef, folderProp );
    
    TBool inDefaultFolder( EFalse );
    TPtrC objectDefName( aOldObject->Def().Name() );
    if( objectDefName == MdeConstants::Image::KImageObject )
        {
        const TDesC& uri = aOldObject->Uri();
        if( uri.FindF( iMmcImagesPath->Des()) != KErrNotFound ||
            uri.FindF( iPhoneImagesPath->Des()) != KErrNotFound ||
            uri.FindF( KDCIMFolder ) != KErrNotFound )
            {
            inDefaultFolder = ETrue; 
            }    
        }
    else if( objectDefName == MdeConstants::Video::KVideoObject )
        {
        const TDesC& uri = aOldObject->Uri();
        if( uri.FindF( iMmcVideosPath->Des()) != KErrNotFound ||
            uri.FindF( iPhoneVideosPath->Des()) != KErrNotFound ||
            uri.FindF( KDCIMFolder ) != KErrNotFound )
            {
            inDefaultFolder = ETrue; 
            }    
        }
    else if( objectDefName == MdeConstants::Audio::KAudioObject )
        {
        const TDesC& uri = aOldObject->Uri();
        if( uri.FindF( iMmcSoundsPath->Des()) != KErrNotFound ||
            uri.FindF( iPhoneSoundsPath->Des()) != KErrNotFound )
            {
            inDefaultFolder = ETrue;
            } 
        }

    if( folderProp )
        {
        folderProp->SetBoolValueL( inDefaultFolder );
        }
    else
        {
        aOldObject->AddBoolPropertyL( *iDefaultFolderPropertyDef, inDefaultFolder );
        }
    }

