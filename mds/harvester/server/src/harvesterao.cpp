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
* Description:  Thread which actually performs the harvesting of the files*
*/


#include <e32svr.h>
#include <caf/caf.h>
#include <driveinfo.h>

#include <placeholderdata.h>
#include <harvesterclientdata.h>
#include <pathinfo.h>

using namespace ContentAccess;

#include "harvesterao.h"
#include "harvesterlog.h"
#include "harvesterblacklist.h"
#include "mdeobject.h"
#include "mdsutils.h"
#include "mdeconstants.h"
#include "harvesterdata.h"
#include "ondemandao.h"
#include "harvestercommon.h"
#include "mdscommoninternal.h"
#include "processoriginmapperinterface.h"
#include "mdeobjecthandler.h"
#include "harvestereventmanager.h"
#include "harvestercenreputil.h"
#include "restorewatcher.h"
#include "backupsubscriber.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "harvesteraoTraces.h"
#endif


// constants
const TInt32 KFileMonitorPluginUid = 0x20007186;  // file monitor plugin implementation uid

const TInt KPlaceholderQueueSize = 99;
const TInt KContainerPlaceholderQueueSize = 10;
const TInt KReadyPlaceholderQueueSize = 10;
const TInt KObjectDefStrSize = 20;

_LIT( KTAGDaemonName, "ThumbAGDaemon" );
_LIT( KTAGDaemonExe, "thumbagdaemon.exe" );

_LIT(KVideo, "Video");
_LIT(KInUse, "InUse");

_LIT(KUndefined, " ");

_LIT( KExtensionMp4,   "mp4" );
_LIT( KExtensionMpg4,  "mpg4" );
_LIT( KExtensionMpeg4, "mpeg4" );
_LIT( KExtension3gp,   "3gp" );
_LIT( KExtension3gpp,  "3gpp" );
_LIT( KExtension3g2,   "3g2" );

CHarvesterAoPropertyDefs::CHarvesterAoPropertyDefs() : CBase()
	{
	}

void CHarvesterAoPropertyDefs::ConstructL(CMdEObjectDef& aObjectDef)
	{
    SetByObjectDefL( aObjectDef );
	}

CHarvesterAoPropertyDefs* CHarvesterAoPropertyDefs::NewL()
    {
    CHarvesterAoPropertyDefs* self = 
        new (ELeave) CHarvesterAoPropertyDefs();
    return self;
    }

void CHarvesterAoPropertyDefs::SetByObjectDefL(CMdEObjectDef& aObjectDef)
    {
    CMdENamespaceDef& nsDef = aObjectDef.NamespaceDef();

    // Common property definitions
    CMdEObjectDef& objectDef = nsDef.GetObjectDefL( MdeConstants::Object::KBaseObject );
    iCreationDatePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KCreationDateProperty );
    iLastModifiedDatePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KLastModifiedDateProperty );
    iSizePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KSizeProperty );
    iOriginPropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KOriginProperty );
    iItemTypePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
    iTitlePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
    iDefaultFolderPropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KInDefaultFolder );
    
    CMdEObjectDef& mediaDef = nsDef.GetObjectDefL( MdeConstants::MediaObject::KMediaObject );
    iPreinstalledPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KPreinstalledProperty );
    }
	
// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CHarvesterAO* CHarvesterAO::NewLC()
    {
    WRITELOG( "CHarvesterAO::NewLC() - begin" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_NEWLC, "CHarvesterAO::NewLC" );    
    
    CHarvesterAO* self = new (ELeave) CHarvesterAO();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CHarvesterAO* CHarvesterAO::NewL()
    {
    WRITELOG( "CHarvesterAO::NewL() - begin" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_NEWL, "CHarvesterAO::NewL" );
    
    CHarvesterAO* self = CHarvesterAO::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CHarvesterAO
// ---------------------------------------------------------------------------
//
CHarvesterAO::CHarvesterAO() : CActive( KHarvesterPriorityHarvestingPlugin )
    {
    WRITELOG( "CHarvesterAO::CHarvesterAO() - begin" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_CHARVESTERAO, "CHarvesterAO::CHarvesterAO" );
    
    iServerPaused = ETrue;
    iNextRequest = ERequestIdle;
    
    iContextEngineInitialized = EFalse;
    iMdeSessionInitialized = EFalse;
    
    iRamFull = EFalse;
    iDiskFull = EFalse;
    iManualPauseEnabled = EFalse;
    iFastHarvestNeeded = EFalse;
    iHarvestingPlaceholders = EFalse;
    
    iUnmountDetected = EFalse;
    iUnmountHandlingOngoing = EFalse;
    iPriorityInterruptDetected = EFalse;
    iLocManipulatorConnected = EFalse;
    }
     
// ---------------------------------------------------------------------------
// ~CHarvesterAO
// ---------------------------------------------------------------------------
//
CHarvesterAO::~CHarvesterAO()
    {
    WRITELOG( "CHarvesterAO::~CHarvesterAO()" );
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_CHARVESTERAO, "CHarvesterAO::~CHarvesterAO" );
    
    Cancel();

	iFs.Close();
	
	iLocManipulator.Close();
	
	if (iCtxEngine)
		{
		iCtxEngine->ReleaseInstance();
		}
			
   	if (iHarvesterEventManager)
		{
		iHarvesterEventManager->ReleaseInstance();
		}

    StopMonitoring();
    DeleteMonitorPlugins();        
    
    StopComposers();
    DeleteComposers();

    delete iDiskFullNotifier;
    iDiskFullNotifier = NULL;
	delete iBackupSubscriber;
	iBackupSubscriber = NULL;
    
    if (iBlacklist)
		{
		iBlacklist->CloseDatabase();
		delete iBlacklist;
		iBlacklist = NULL;
		}
	delete iReHarvester;
	iReHarvester = NULL;

    if ( iHarvestFileMessages.Count() > 0 )
        {
        for ( TInt i = iHarvestFileMessages.Count()-1; i >= 0; --i )
            {
            RMessage2& msg = iHarvestFileMessages[i].iMessage;
            if (!msg.IsNull())
            	{
            	msg.Complete( KErrCancel );
            	}
            iHarvestFileMessages.Remove( i );
            }
        }
    iHarvestFileMessages.Close();
  	
    iPHArray.ResetAndDestroy();
    iPHArray.Close();
	
   	iReadyPHArray.ResetAndDestroy();
    iReadyPHArray.Close();
    
    iContainerPHArray.ResetAndDestroy();
    iContainerPHArray.Close();
    
    iTempReadyPHArray.ResetAndDestroy();
    iTempReadyPHArray.Close();
    
	delete iHarvesterOomAO;
	iHarvesterOomAO = NULL;
    delete iRestoreWatcher;
    iRestoreWatcher = NULL;
	delete iOnDemandAO;
	iOnDemandAO = NULL;
	delete iMdEHarvesterSession;
	iMdEHarvesterSession = NULL;
	delete iMdESession;
	iMdESession = NULL;
	delete iQueue;
	iQueue = NULL;
	delete iHarvesterPluginFactory;
	iHarvesterPluginFactory = NULL;
	delete iMdeObjectHandler;
	iMdeObjectHandler = NULL;
	delete iUnmountHandlerAO;
	iUnmountHandlerAO = NULL;
	
	delete iPropDefs;
	iPropDefs = NULL;
	delete iCameraExtensionArray;
	iCameraExtensionArray = NULL;
	
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
	
	RMediaIdUtil::ReleaseInstance();
    
    REComSession::FinalClose();
    }

 // ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CHarvesterAO::ConstructL()
    {
    WRITELOG( "CHarvesterAO::ConstructL() - begin" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_CONSTRUCTL, "CHarvesterAO::ConstructL - begin" );
     
    CActiveScheduler::Add( this );

	User::LeaveIfError( iFs.Connect() );
	
    // Setting up MdE Session
	iMdESession = CMdESession::NewL( *this );

    iBackupSubscriber = CBackupSubscriber::NewL( *this );
	
	iUnmountHandlerAO = CUnmountHandlerAO::NewL( *this );
	iUnmountHandlerAO->WaitForUnmountL();
	
	iHarvesterEventManager = CHarvesterEventManager::GetInstanceL();
	
	iRestoreWatcher = CRestoreWatcher::NewL();
	
	iHarvesterOomAO = CHarvesterOomAO::NewL( *this );
	
	iMediaIdUtil = &RMediaIdUtil::GetInstanceL();
	
    iBlacklist = CHarvesterBlacklist::NewL();
    iBlacklist->OpenDatabase();
    
    // Setting up Harvester queue
    iQueue = CHarvesterQueue::NewL( this, iBlacklist );
    
    // Setting up reharvester
    iReHarvester = CReHarvesterAO::NewL();
    iReHarvester->SetHarvesterQueue( iQueue );
    
    iHarvesterPluginFactory = CHarvesterPluginFactory::NewL();
    iHarvesterPluginFactory->SetBlacklist( *iBlacklist );
    
    // Reset harvesting status for clients in case blacklisted file was handled
    iHarvesterPluginFactory->SendHarvestingStatusEventL( EFalse );
    
    iPropDefs = CHarvesterAoPropertyDefs::NewL();
	
    iCameraExtensionArray = new ( ELeave ) CDesCArraySeg( 6 );
    iCameraExtensionArray->InsertIsqL( KExtensionMp4 );
    iCameraExtensionArray->InsertIsqL( KExtensionMpg4 );
    iCameraExtensionArray->InsertIsqL( KExtensionMpeg4 );
    iCameraExtensionArray->InsertIsqL( KExtension3gp );
    iCameraExtensionArray->InsertIsqL( KExtension3gpp );
    iCameraExtensionArray->InsertIsqL( KExtension3g2 );

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
   		
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_CONSTRUCTL, "CHarvesterAO::ConstructL - end" );    
	
    WRITELOG( "CHarvesterAO::ConstructL() - end" );
    }

// ---------------------------------------------------------------------------
// LoadMonitorPluginsL
// ---------------------------------------------------------------------------
//
void CHarvesterAO::LoadMonitorPluginsL()
    {
    WRITELOG( "CHarvesterAO::LoadMonitorPluginsL()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_LOADMONITORPLUGINSL, "CHarvesterAO::LoadMonitorPluginsL" );
    
    RImplInfoPtrArray infoArray;
    
    TCleanupItem cleanupItem( MdsUtils::CleanupEComArray, &infoArray );
    CleanupStack::PushL( cleanupItem );
    
    CMonitorPlugin::ListImplementationsL( infoArray );
    const TInt count( infoArray.Count() );
    CMonitorPlugin* plugin = NULL;
    
    for ( TInt i = 0; i < count; i++ )
        {
        TUid uid = infoArray[i]->ImplementationUid();    // Create the plug-ins
        plugin = NULL;
        TRAPD( err, plugin = CMonitorPlugin::NewL( uid ) );
        if ( err == KErrNone && plugin )
            {
            CleanupStack::PushL( plugin );
            iMonitorPluginArray.AppendL( plugin ); // and add them to array
            CleanupStack::Pop( plugin );
            if ( uid.iUid == KFileMonitorPluginUid )
                {
                void* ptr = plugin;
                iProcessOriginMapper = STATIC_CAST( MProcessOriginMapperInterface*, ptr );
                }
            }
        else
            {
            WRITELOG( "CHarvesterAO::LoadMonitorPlugins() - Failed to load a monitor plugin!" );
            OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_LOADMONITORPLUGINSL, "CHarvesterAO::LoadMonitorPluginsL  - Failed to load a monitor plugin!" );
            
            delete plugin;
            plugin = NULL;
            }
        }
    
    CleanupStack::PopAndDestroy( &infoArray ); // infoArray, results in a call to CleanupEComArray
    }

// ---------------------------------------------------------------------------
// DeleteMonitorPlugins
// ---------------------------------------------------------------------------
//
void CHarvesterAO::DeleteMonitorPlugins()
    {
    WRITELOG( "CHarvesterAO::DeleteMonitorPlugins()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_DELETEMONITORPLUGINS, "CHarvesterAO::DeleteMonitorPlugins" );
    
    iMonitorPluginArray.ResetAndDestroy();
    iMonitorPluginArray.Close();
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_DELETEMONITORPLUGINS, "CHarvesterAO::DeleteMonitorPlugins - end" );    
    WRITELOG( "CHarvesterAO::DeleteMonitorPlugins() - end" );
    }

// ---------------------------------------------------------------------------
// StartMonitoring
// ---------------------------------------------------------------------------
//
void CHarvesterAO::StartMonitoring()
    {
    WRITELOG( "CHarvesterAO::StartMonitoring()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_STARTMONITORING, "CHarvesterAO::StartMonitoring" );    
    const TInt count( iMonitorPluginArray.Count() );  
    
    for ( TInt i = 0; i < count; i++ )
        {
        iMonitorPluginArray[i]->StartMonitoring( *iQueue, iMdESession, NULL, 
        		iHarvesterPluginFactory );
        }
    }

// ---------------------------------------------------------------------------
// StopMonitoring
// ---------------------------------------------------------------------------
//
void CHarvesterAO::StopMonitoring()
    {
    WRITELOG( "CHarvesterAO::StopMonitoring()" );
    
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_STOPMONITORING, "CHarvesterAO::StopMonitoring" );
    
    for( TInt i = iMonitorPluginArray.Count() - 1; i >=0; i-- )
        {
        iMonitorPluginArray[i]->StopMonitoring();
        }
    }

// ---------------------------------------------------------------------------
// PauseMonitoring
// ---------------------------------------------------------------------------
//
void CHarvesterAO::PauseMonitoring()
    {
    WRITELOG( "CHarvesterAO::PauseMonitoring()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_PAUSEMONITORING, "CHarvesterAO::PauseMonitoring" );
    
    for( TInt i = iMonitorPluginArray.Count() - 1; i >=0; i-- )
        {
        iMonitorPluginArray[i]->PauseMonitoring();
        }
    
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_PAUSEMONITORING, "CHarvesterAO::PauseMonitoring - end" );    
    WRITELOG( "CHarvesterAO::PauseMonitoring() - end" );
    }

// ---------------------------------------------------------------------------
// ResumeMonitoring
// ---------------------------------------------------------------------------
//
void CHarvesterAO::ResumeMonitoring()
    {
    WRITELOG( "CHarvesterAO::ResumeMonitoring()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_RESUMEMONITORING, "CHarvesterAO::ResumeMonitoring" );
    
    const TInt count( iMonitorPluginArray.Count() );
    
    for ( TInt i=0; i < count; i++ )
        {
        iMonitorPluginArray[i]->ResumeMonitoring( *iQueue, iMdESession, NULL,
        		iHarvesterPluginFactory );
        }
    WRITELOG( "CHarvesterAO::ResumeMonitoring() - end" );
    }

// ---------------------------------------------------------------------------
// HandleUnmount
// ---------------------------------------------------------------------------
//
void CHarvesterAO::HandleUnmount( TUint32 aMediaId )
	{
    WRITELOG1( "CHarvesterAO::HandleUnmount(%u)", aMediaId );    
	OstTrace1( TRACE_NORMAL, CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount;aMediaId=%d", aMediaId );
    
    iUnmountDetected = ETrue;
    iUnmountHandlingOngoing = ETrue;
    
    if( !iServerPaused )
        {
        // Stop harvesting for unmount
        PauseMonitoring();
        PauseHarvester();
        }
	
    TUint32 mediaId( 0 );
    TUint removed( 0 );
    CHarvesterData* hd = NULL;

    TInt err( KErrNone );
	
#ifdef _DEBUG
	WRITELOG1( "CHarvesterAO::HandleUnmount() iReadyPHArray.Count() = %d", iReadyPHArray.Count() );
	OstTrace1( TRACE_NORMAL, DUP1_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount;iReadyPHArray.Count()=%d", iReadyPHArray.Count() );
	
#endif
	TInt arrayCount( iReadyPHArray.Count() );
	if( arrayCount > 0 )
        {
        for( TInt i=arrayCount-1; i>= 0; i--)
            {
            hd = iReadyPHArray[i];
            err = iMediaIdUtil->GetMediaId( hd->Uri(), mediaId );
            
            if( err == KErrNone && mediaId == aMediaId )
                {
                WRITELOG1( "CHarvesterAO::HandleUnmount() remove iReadyPHArray %d", i);
                OstTrace1( TRACE_NORMAL, DUP2_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount remove iReadyPHArray %d", i );
                
                HarvestCompleted( hd->ClientId(), hd->Uri(), KErrCancel );
                delete hd;
                hd = NULL;
                iReadyPHArray.Remove( i );
                removed++;
                arrayCount--;
                }
            }
        if( iReadyPHArray.Count() == 0 )
            {
            iReadyPHArray.Compress();
            }
        WRITELOG1( "CHarvesterAO::HandleUnmount() DecreaseItemCountL iReadyPHArray %d", removed);
        OstTrace1( TRACE_NORMAL, DUP3_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount DecreaseItemCountL iReadyPHArray %d", removed );
        
        TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypePlaceholder, removed) );
        }
   
   removed = 0;
	
#ifdef _DEBUG
	WRITELOG1( "CHarvesterAO::HandleUnmount() iPHArray.Count() = %d", iPHArray.Count() );
	OstTrace1( TRACE_NORMAL, DUP4_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount iPHArray.Count()=%d", iPHArray.Count() );
	
#endif
	arrayCount = iPHArray.Count();
   if( arrayCount > 0 )
        {
        for( TInt i=arrayCount-1; i>= 0; i--)
            {
            hd = iPHArray[i];
            err = iMediaIdUtil->GetMediaId( hd->Uri(), mediaId );
            
            if( err == KErrNone && mediaId == aMediaId )
                {
                WRITELOG1( "CHarvesterAO::HandleUnmount() remove iPHArray %d", i);
                OstTrace1( TRACE_NORMAL, DUP5_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount remove iPHArray %d", i );
                
                HarvestCompleted( hd->ClientId(), hd->Uri(), KErrCancel );
                delete hd;
				hd = NULL;
                iPHArray.Remove( i );
                removed++;
                arrayCount--;
                }
            }
        if( iPHArray.Count() == 0 )
            {
            iPHArray.Compress();
            }
        WRITELOG1( "CHarvesterAO::HandleUnmount() DecreaseItemCountL iPHArray %d", removed);
        OstTrace1( TRACE_NORMAL, DUP6_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount DecreaseItemCountL iPHArray %d", removed );        
        TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypePlaceholder, removed) );
        }
   
   removed = 0;
   
#ifdef _DEBUG
   WRITELOG1( "CHarvesterAO::HandleUnmount() iContainerPHArray.Count() = %d", iContainerPHArray.Count() );
   OstTrace1( TRACE_NORMAL, DUP7_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount iContainerPHArray.Count()=%d", iContainerPHArray.Count() );
   
#endif
   arrayCount = iContainerPHArray.Count();
   if( arrayCount > 0 )
        {
        for( TInt i=arrayCount-1; i>= 0; i--)
            {
            hd = iContainerPHArray[i];
            err = iMediaIdUtil->GetMediaId( hd->Uri(), mediaId );
            
            if( err == KErrNone && mediaId == aMediaId )
                {
                WRITELOG1( "CHarvesterAO::HandleUnmount() remove iContainerPHArray %d", i);
                OstTrace1( TRACE_NORMAL, DUP8_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount remove iContainerPHArray %d", i );
                
                HarvestCompleted( hd->ClientId(), hd->Uri(), KErrCancel );
                delete hd;
				hd = NULL;
                iContainerPHArray.Remove( i );
                removed++;
                arrayCount--;
                }
            }
        if( iContainerPHArray.Count() == 0 )
            {
            iContainerPHArray.Compress();
            }
        WRITELOG1( "CHarvesterAO::HandleUnmount() DecreaseItemCountL iContainerPHArray %d", removed);
        TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypePlaceholder, removed) );
        }

   removed = 0;
   
#ifdef _DEBUG
   WRITELOG1( "CHarvesterAO::HandleUnmount() iTempReadyPHArray.Count() = %d", iTempReadyPHArray.Count() );
#endif
   arrayCount = iTempReadyPHArray.Count();
   if( arrayCount > 0 )
        {
        for( TInt i=arrayCount-1; i>= 0; i--)
            {
            hd = iTempReadyPHArray[i];
            err = iMediaIdUtil->GetMediaId( hd->Uri(), mediaId );
            
            if( err == KErrNone && mediaId == aMediaId )
                {
                WRITELOG1( "CHarvesterAO::HandleUnmount() remove iTempReadyPHArray %d", i);
                HarvestCompleted( hd->ClientId(), hd->Uri(), KErrCancel );
                delete hd;
                hd = NULL;
                iTempReadyPHArray.Remove( i );
                removed++;
                arrayCount--;
                }
            }
        if( iTempReadyPHArray.Count() == 0 )
            {
            iTempReadyPHArray.Compress();
            }
        WRITELOG1( "CHarvesterAO::HandleUnmount() DecreaseItemCountL iTempReadyPHArray %d", removed);
        TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypePlaceholder, removed) );
        }
   
   removed = 0;
   
	const TUint count = iQueue->ItemsInQueue();
	WRITELOG1( "CHarvesterAO::HandleUnmount() iQueue.Count() = %d", count );
	OstTrace1( TRACE_NORMAL, DUP10_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount iQueue.Count() = %d", count );
	
	if( count > 0 )
	    {
	    WRITELOG( "CHarvesterAO::HandleUnmount() remove iQueue" );
	    OstTrace0( TRACE_NORMAL, DUP11_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount remove iQueue" );	    
	    removed = iQueue->RemoveItems( aMediaId );
	    WRITELOG1( "CHarvesterAO::HandleUnmount() removed iQueue = %d", removed );
	    OstTrace1( TRACE_NORMAL, DUP12_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount removed iQueue = %d", removed );
	    
	    TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypePlaceholder, removed ) );
        TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, removed ) );
	    }
	
	removed = 0;
	
	RPointerArray<CHarvesterPluginInfo>& hpiArray = iHarvesterPluginFactory->GetPluginInfos();
	const TInt hpiArrayCount( hpiArray.Count() );
	if( hpiArrayCount > 0 )
		{
		TUint32 mediaId( 0 );
		TInt err( KErrNone );
		
		for( TInt i = hpiArrayCount; --i >= 0; )
			{
			CHarvesterPluginInfo* hpi = hpiArray[i];
			for( TInt j = hpi->iQueue.Count(); --j >= 0; )
				{
				CHarvesterData* hd = hpi->iQueue[j];
				CMdEObject& mdeobj = hd->MdeObject();
				
				err = iMediaIdUtil->GetMediaId( mdeobj.Uri(), mediaId );
	
				if( aMediaId == mediaId || err != KErrNone )
					{
                    hpi->iQueue.Remove(j);
					removed++;

					TRAP_IGNORE( iMdESession->CancelObjectL( mdeobj ) );
					
					HarvestCompleted( hd->ClientId(), hd->Uri(), KErrCancel );
					delete hd;
					hd = NULL;
					}
				}
			
            if( hpi->iQueue.Count() == 0 )
                {
                hpi->iQueue.Compress();
                }
			}
		
		if( removed )
		    {
            WRITELOG1( "CHarvesterAO::HandleUnmount() remove from plugins = %d", removed);
            OstTrace1( TRACE_NORMAL, DUP13_CHARVESTERAO_HANDLEUNMOUNT, "CHarvesterAO::HandleUnmount remove from plugins = %d", removed );
            
            TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypePlaceholder, removed ) );
            TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, removed ) );
		    }
		}
	
    iMediaIdUtil->RemoveMediaId( aMediaId );
	
	// resume harvesting from last state
    iUnmountHandlingOngoing = EFalse;
    // resume monitoring
    ResumeMonitoring();
    TRAP_IGNORE( ResumeHarvesterL() );    
	}

// ---------------------------------------------------------------------------
// StartComposersL
// ---------------------------------------------------------------------------
//
void CHarvesterAO::StartComposersL()
    {
    WRITELOG( "CHarvesterAO::StartComposersL()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_STARTCOMPOSERSL, "CHarvesterAO::StartComposersL" );
        
    RImplInfoPtrArray infoArray;
    TCleanupItem cleanupItem( MdsUtils::CleanupEComArray, &infoArray );
    CleanupStack::PushL( cleanupItem );
    CComposerPlugin::ListImplementationsL( infoArray );
    const TInt count( infoArray.Count() );

    for ( TInt i=0; i < count; i++ )
        {
        TUid uid = infoArray[i]->ImplementationUid();
        CComposerPlugin* plugin = NULL;
        TRAPD(trapError, plugin = CComposerPlugin::NewL( uid ) );
        if ( trapError != KErrNone )
            {
            WRITELOG( "CHarvesterAO::StartComposersL() - failed to create new composer" );
            }
        else
            {
            plugin->SetSession( iMdEHarvesterSession->SessionRef() );
            iComposerPluginArray.AppendL( plugin );            
            }
        }
            
    CleanupStack::PopAndDestroy( &infoArray );    
    WRITELOG( "CHarvesterAO::StartComposersL() - end" );
    }

// ---------------------------------------------------------------------------
// StopComposers
// ---------------------------------------------------------------------------
//
void CHarvesterAO::StopComposers()
    {
    WRITELOG( "CHarvesterAO::StopComposers()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_STOPCOMPOSERS, "CHarvesterAO::StopComposers" );    
    
    const TInt count( iComposerPluginArray.Count() );
    for ( TInt i = count; --i >= 0; )
        {
        iComposerPluginArray[i]->RemoveSession();
        }

    WRITELOG( "CHarvesterAO::StopComposers() - end" );
    }

// ---------------------------------------------------------------------------
// DeleteComposers
// ---------------------------------------------------------------------------
//
void CHarvesterAO::DeleteComposers()
    {
    WRITELOG( "CHarvesterAO::DeleteComposers()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_DELETECOMPOSERS, "CHarvesterAO::DeleteComposers" );
    
    iComposerPluginArray.ResetAndDestroy();
    iComposerPluginArray.Close();

    WRITELOG( "CHarvesterAO::DeleteComposers() - end" );
    }

// ---------------------------------------------------------------------------
// IsComposingReady
// ---------------------------------------------------------------------------
//
TBool CHarvesterAO::IsComposingReady()
    {
    WRITELOG( "CHarvesterAO::IsComposingReady()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_ISCOMPOSINGREADY, "CHarvesterAO::IsComposingReady" );
    
    const TInt count( iComposerPluginArray.Count() );
    for ( TInt i = count; --i >= 0; )
        {
        if ( iComposerPluginArray[i]->IsComposingComplete() == EFalse )
            {
            return EFalse;
            }
        }

    WRITELOG( "CHarvesterAO::IsComposingReady() - end" );
    return ETrue;
    }

// ---------------------------------------------------------------------------
// ReadItemFromQueueL
// ---------------------------------------------------------------------------
//
void CHarvesterAO::ReadItemFromQueueL()
    {
    WRITELOG( "CHarvesterAO::ReadItemFromQueueL()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_READITEMFROMQUEUEL, "CHarvesterAO::ReadItemFromQueueL" );
    
    CHarvesterData* hd = iQueue->GetNextItem();
    
    if ( hd->ObjectType() == EPlaceholder )
    	{
        iHarvestingPlaceholders = ETrue;
        if( !iFastHarvestNeeded )
            {
            SetPriority( KHarvesterCustomImportantPriority );
            }
    	while( hd &&
			   iPHArray.Count() < KPlaceholderQueueSize &&
			   hd->ObjectType() == EPlaceholder )
    		{
        	if(iPHArray.Append( hd ) != KErrNone)
        	    {
        	    HarvestCompleted( hd->ClientId(), hd->Uri(), KErrNoMemory );
        	    iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
                delete hd;
                hd = NULL;
        	    }
        	else
        	    {
                if( hd->Origin() == MdeConstants::Object::ECamera || 
                    hd->ObjectType() == EFastHarvest  )
                    {
                    if( !iFastHarvestNeeded )
                        {
                        iFastHarvestNeeded = ETrue;
                        // Fast harvest event must be handled even if MMC handling would be ongoing
                        SetPriority( KHarvesterPriorityMonitorPlugin );
                        }
                    break;
                    }
                else if( iFastHarvestNeeded )
                    {
                    iFastHarvestNeeded = EFalse;
                    SetPriority( KHarvesterCustomImportantPriority );
        	    	}
        	    }
        	hd = iQueue->GetNextItem();
    		}
    	
    	if( iFastHarvestNeeded && iPHArray.Count() > 0 )
    	    {
            TRAPD( err, HandlePlaceholdersL( ETrue ) );

            // make sure that when HandlePlaceholdersL leaves unexpectedly, iPHArray is cleared
            if ( err != KErrNone )
                {
                if( err != KErrDiskFull )
                    {
                    iPHArray.ResetAndDestroy();
                    iTempReadyPHArray.ResetAndDestroy();
                    }
                User::Leave( err );
                }
            
            TInt count( iReadyPHArray.Count() );
            for( TInt i = 0; i < count; i++ )
                {
                CheckFileExtensionAndHarvestL( iReadyPHArray[i] );
                if( iUnmountDetected )
                    {
                    break;
                    }
                iReadyPHArray.Remove( i );
                // correct the index so harvesting order remains ok
                i--;
                count--;
                }
            return;
    	    }
    	
		if( hd )
    		{
	    	if( hd->ObjectType() == EPlaceholder )
	    		{
	        	if( iPHArray.Append( hd ) != KErrNone )
	        	    {
	        	    HarvestCompleted( hd->ClientId(), hd->Uri(), KErrNoMemory ); 
	        	    iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
                    delete hd;
                    hd = NULL;
	        	    }
	    		}
	    	else
	    		{
                if( iReadyPHArray.Append( hd ) != KErrNone)
                    {
                    delete hd;
                    hd = NULL;
                    }
	    		}
    		}
			
    	if( iPHArray.Count() > 0 )
    		{
	    	TRAPD( err, HandlePlaceholdersL( ETrue ) );

	    	// make sure that when HandlePlaceholdersL leaves unexpectedly, iPHArray is cleared
	    	if ( err != KErrNone )
	    		{
	    	    if( err != KErrDiskFull )
	    	        {
	    	        iPHArray.ResetAndDestroy();
	    	        iTempReadyPHArray.ResetAndDestroy();
	    	        }
	    		User::Leave( err );
	    		}
    		}
    	}
    else
    	{
        if( iHarvestingPlaceholders && !iFastHarvestNeeded )
            {
            SetPriority( KHarvesterPriorityHarvestingPlugin );
            }
        iHarvestingPlaceholders = EFalse;
        CheckFileExtensionAndHarvestL( hd );
        if( iUnmountDetected && hd )
            {
            iQueue->Append( hd );
            }
    	}
    }

// ---------------------------------------------------------------------------
// HandlePlaceholdersL
// ---------------------------------------------------------------------------
//
void CHarvesterAO::HandlePlaceholdersL( TBool aCheck )
	{
	WRITELOG( "CHarvesterAO::HandlePlaceholdersL()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_HANDLEPLACEHOLDERSL, "CHarvesterAO::HandlePlaceholdersL" );

	RPointerArray<CMdEObject> mdeObjectArray;
	CleanupClosePushL( mdeObjectArray );

	TTimeIntervalSeconds timeOffsetSeconds = User::UTCOffset();
	
	CMdENamespaceDef& defNS = iMdESession->GetDefaultNamespaceDefL();
	
	TInt endindex( iPHArray.Count() );
	for( TInt i = 0; i < endindex; i++ )
		{
		CHarvesterData* hd = iPHArray[i];
		
		if( aCheck && 
		    hd->Origin() != MdeConstants::Object::ECamera &&
		    iHarvesterPluginFactory->IsContainerFileL( hd->Uri() ) )
			{
			if( iContainerPHArray.Append( hd ) != KErrNone )
			    {
			    HarvestCompleted( hd->ClientId(), hd->Uri(), KErrNoMemory );
			    iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
                delete hd;
                hd = NULL;
			    }

			iPHArray.Remove( i );
			i--;
			endindex--;
			continue;
			}
		
		TBuf<KObjectDefStrSize> objDefStr;
		
		if( !CheckForCameraItem( hd, objDefStr ) )
		    {
		    iHarvesterPluginFactory->GetObjectDefL( hd, objDefStr );
		    }
		
		// GetObjectDef can cause context switch, and if unmount happens when this execution is 
		// interrupted, the ph arrays can be invalid. Thus, abort whole run, and start over to make sure 
		// the arrays are valid.
		if( iUnmountDetected )
		    {
		    WRITELOG( "CHarvesterAO::HandlePlaceholdersL() - Unmount detected during execution!" );
		    for( TInt y( iTempReadyPHArray.Count() -1 ); y >=0; y-- )
		        {
		        CHarvesterData* tempHd = iTempReadyPHArray[y];
				
		        if(iPHArray.Insert( tempHd, 0 ) != KErrNone)
		            {
		            HarvestCompleted( tempHd->ClientId(), tempHd->Uri(), KErrNoMemory );
		            iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
                    delete tempHd;
                    tempHd = NULL;
		            }
		        }
		    iTempReadyPHArray.Reset();
		    CleanupStack::PopAndDestroy( &mdeObjectArray );
		    return;
		    }
	    // Check for priority interrupt
	    // If priority interrupt occures, drop current operation to handle the priority item first,
	    // and continue from current situation after that
		else if( iPriorityInterruptDetected )
		    {
            WRITELOG( "CHarvesterAO::HandlePlaceholdersL() - Priority interrupt during execution!" );
            const TInt currentPHArrayCount( iPHArray.Count() );
            for( TInt y( iTempReadyPHArray.Count() -1 ); y >=0; y-- )
                {
                CHarvesterData* tempHd = iTempReadyPHArray[y];
                if(iPHArray.Insert( tempHd, 0 ) != KErrNone)
                    {
                    HarvestCompleted( tempHd->ClientId(), tempHd->Uri(), KErrNoMemory );
                    iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
                    delete tempHd;
                    tempHd = NULL;
                    }
                }
            iTempReadyPHArray.Reset();
            CleanupStack::PopAndDestroy( &mdeObjectArray );
            return;		
		    }
	    // Check if disk is full
	    // If disk is detected to be full, no items can be added to MDS db, thus abort the run, and start over 
		// when disk space is available to make sure the arrays are valid.
		else if( iDiskFull )
		    {
            WRITELOG( "CHarvesterAO::HandlePlaceholdersL() - No disk space available!" );
            for( TInt y( iTempReadyPHArray.Count() -1 ); y >=0; y-- )
                {
                CHarvesterData* tempHd = iTempReadyPHArray[y];
            
                if(iPHArray.Insert( tempHd, 0 ) != KErrNone)
                    {
                    HarvestCompleted( tempHd->ClientId(), tempHd->Uri(), KErrNoMemory );
                    iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
                    delete tempHd;
                    tempHd = NULL;
                    }
                }
            iTempReadyPHArray.Reset();
            CleanupStack::PopAndDestroy( &mdeObjectArray );
            User::Leave( KErrDiskFull );
		    }
		
		if( objDefStr.Length() == 0 ||
		    ( objDefStr == KInUse ) )
			{
#ifdef _DEBUG
		    if( objDefStr.Length() == 0 )
		        {
		        WRITELOG( "CHarvesterAO::HandlePlaceholdersL() - no objectDef failing harvesting" );
		        }
		    else
		        {
		        WRITELOG( "CHarvesterAO::HandlePlaceholdersL() - in use, failing harvesting" );
		        }
		    WRITELOG1( "CHarvesterAO::HandlePlaceholdersL() - harvesting failed, uri: %S", &(hd->Uri()) );
#endif
		    // If object has not been created in the device so that monitors would have
		    // picked up creation event, and the file is in use, subclose event will
		    // not trigger the file to be harvester when closed, thus it needs to be moved 
		    // to reharvesting queue
		    if( objDefStr == KInUse &&
		        !hd->TakeSnapshot() )
		        {
		        iPHArray.Remove( i );
		        i--;
		        endindex--;
		        iReHarvester->AddItem( hd );
		        continue;
		        }
			const TInt error( KErrUnknown );
            // notify observer, notification is needed even if file is not supported
            HarvestCompleted( hd->ClientId(), hd->Uri(), error );
			delete hd;
			hd = NULL;
			iPHArray.Remove( i );
            i--;
            endindex--;
			iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
			continue;
			}

		CMdEObjectDef& mdeObjectDef = defNS.GetObjectDefL( objDefStr );

		HBufC* hdUri = hd->Uri().AllocL();
		CleanupStack::PushL( hdUri );
		CMdEObject* mdeObject = iMdESession->NewObjectL( mdeObjectDef, *hdUri );
		CleanupStack::PopAndDestroy( hdUri );
		CleanupStack::PushL( mdeObject );
		
		CPlaceholderData* phData = NULL;

		if( hd->TakeSnapshot() )
			{
		    phData = CPlaceholderData::NewL();
		    CleanupStack::PushL( phData );
		    TEntry* entry = new (ELeave) TEntry();
		    CleanupStack::PushL( entry );
		    const TDesC& uri = hd->Uri();
		    const TInt err = iFs.Entry( uri, *entry );
		    if ( err != KErrNone )
		    	{
		    	WRITELOG( "CHarvesterAO::HandlePlaceholdersL() - cannot create placeholder data object for camera. file does not exists" );
		    	OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_HANDLEPLACEHOLDERSL, "CHarvesterAO::HandlePlaceholdersL- cannot create placeholder data object for camera. file does not exists" );
		    	
	    		// notify observer
	    	    HarvestCompleted( hd->ClientId(), hd->Uri(), err );
				delete hd;
				hd = NULL;
				iPHArray.Remove( i );
	            i--;
	            endindex--;
				iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
				CleanupStack::PopAndDestroy( entry );
				CleanupStack::PopAndDestroy( phData );
				CleanupStack::PopAndDestroy( mdeObject );
				continue;
		    	}
		    phData->SetUri( uri );
			phData->SetModified( entry->iModified );
			phData->SetFileSize( entry->iSize );
			CleanupStack::PopAndDestroy( entry );

			TUint32 mediaId( 0 );
			User::LeaveIfError( iMediaIdUtil->GetMediaId( uri, mediaId ) );
			phData->SetMediaId( mediaId );
			}
		else
			{
			phData = static_cast<CPlaceholderData*> ( hd->ClientData() );
		    if( !phData )
		    	{
		    	WRITELOG( "CHarvesterAO::HandlePlaceholdersL() - Placeholder data object NULL - abort" );
		    	OstTrace0( TRACE_NORMAL, DUP2_CHARVESTERAO_HANDLEPLACEHOLDERSL, "CHarvesterAO::HandlePlaceholdersL - Placeholder data object NULL - abort" );
		    	
		    	const TInt error( KErrUnknown );
	    		// notify observer
	    	    HarvestCompleted( hd->ClientId(), hd->Uri(), error );
				delete hd;
				hd = NULL;
				iPHArray.Remove( i );
	            i--;
	            endindex--;
				iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
				CleanupStack::PopAndDestroy( mdeObject );
				continue;
		    	}	
			CleanupStack::PushL( phData );
			}	
		
		// set media id
		mdeObject->SetMediaId( phData->MediaId() );
	    
	    // set placeholder
	    mdeObject->SetPlaceholder( ETrue );
	    
	    if( !iPropDefs->iCreationDatePropertyDef )
	        {
            iPropDefs->SetByObjectDefL( defNS.GetObjectDefL( MdeConstants::Object::KBaseObject ) );
	        }

	    // set file size
    	mdeObject->AddUint32PropertyL( *iPropDefs->iSizePropertyDef, phData->FileSize() );

	    // set creation date
    	const TTime localModifiedDate = phData->Modified() + timeOffsetSeconds;
    	mdeObject->AddTimePropertyL( *iPropDefs->iCreationDatePropertyDef, localModifiedDate );

	    // set modification date
    	mdeObject->AddTimePropertyL( *iPropDefs->iLastModifiedDatePropertyDef, phData->Modified() );
	    
    	// set origin
		mdeObject->AddUint8PropertyL( *iPropDefs->iOriginPropertyDef, hd->Origin() );

        TBuf<KMaxDataTypeLength> mimeType;
        iHarvesterPluginFactory->GetMimeType( hd->Uri(), mimeType );
        if( mimeType.Length() > 0 )
            {
            mdeObject->AddTextPropertyL( *iPropDefs->iItemTypePropertyDef, mimeType );
            }
        else
            {
            mdeObject->AddTextPropertyL( *iPropDefs->iItemTypePropertyDef, KUndefined );
            }
		
        if( hd->Origin() == MdeConstants::Object::ECamera )
            {
            TBool inDefaultFolder( ETrue );
            mdeObject->AddBoolPropertyL( *iPropDefs->iDefaultFolderPropertyDef, inDefaultFolder );
            }
        else
            {
            AddDefaultFolderDataL( mdeObject );
            }       
        
	    TPtrC name;
	    TBool nameFound = MdsUtils::GetName( hd->Uri(), name );

	    if ( nameFound )
	        {
	        mdeObject->AddTextPropertyL( *iPropDefs->iTitlePropertyDef, name );
	        }
	    else
	        {
	        mdeObject->AddTextPropertyL( *iPropDefs->iTitlePropertyDef, KUndefined );
	        }
	    
    	CPlaceholderData* ph = NULL;
    	if( hd->TakeSnapshot() )
    	    {
    	    ph = static_cast<CPlaceholderData*>( hd->ClientData() );
    	    }
    	else
    	    {
    	    ph = phData;
    	    }
    	
	    const TInt isPreinstalled = ph->Preinstalled();
	    if( isPreinstalled == MdeConstants::MediaObject::EPreinstalled )
	    	{
	    	WRITELOG("CHarvesterAO::HandlePlaceholdersL() - preinstalled");
	    	OstTrace0( TRACE_NORMAL, DUP3_CHARVESTERAO_HANDLEPLACEHOLDERSL, "CHarvesterAO::HandlePlaceholdersL - preinstalled" );
	    	
	    	mdeObject->AddInt32PropertyL( *iPropDefs->iPreinstalledPropertyDef, isPreinstalled );
	    	}
		
	    hd->SetEventType( EHarvesterEdit );
		
		// skip 
		if( hd->TakeSnapshot() )
			{
			hd->SetObjectType( EFastHarvest );
			}
		else
			{
			hd->SetClientData( NULL );
			hd->SetObjectType( ENormal );
			}
		
		hd->SetMdeObject( mdeObject );
		
		// Ownership of mdeObject transferred to the array
		mdeObjectArray.AppendL( mdeObject );
		
	    CleanupStack::PopAndDestroy( phData );
	    
	    CleanupStack::Pop( mdeObject );
		
	    if(iTempReadyPHArray.Append( hd ) != KErrNone)
	        {
	        HarvestCompleted( hd->ClientId(), hd->Uri(), KErrNoMemory );
            delete hd;
            hd = NULL;
	        }
		iPHArray.Remove( i );
        i--;
        endindex--;
		}
	
	const TInt objectCount = mdeObjectArray.Count();  
	
    if( objectCount > 0 )
		{
		// add object to mde
		iMdEHarvesterSession->AutoLockL( mdeObjectArray );
		TInt addError( KErrNone );
		TRAPD( addFailure, addError = iMdESession->AddObjectsL( mdeObjectArray ) );
		if( addFailure == KErrDiskFull )
		    {
            WRITELOG( "CHarvesterAO::HandlePlaceholdersL() - No disk space available!" );
            for( TInt y( iTempReadyPHArray.Count() -1 ); y >=0; y-- )
                {
                CHarvesterData* tempHd = iTempReadyPHArray[y];
        
                if(iPHArray.Insert( tempHd, 0 ) != KErrNone)
                    {
                    HarvestCompleted( tempHd->ClientId(), tempHd->Uri(), KErrNoMemory );
                    iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
                    delete tempHd;
                    tempHd = NULL;
                    }
                }
            iTempReadyPHArray.Reset();
            CleanupStack::PopAndDestroy( &mdeObjectArray );
            User::Leave( KErrDiskFull );		
		    }
		else if( addFailure != KErrNone )
		    {
		    User::Leave( addFailure );      
		    }
		
		if( addError != KErrNone )
		    {
		    // If some error occures, retry
		    iMdESession->AddObjectsL( mdeObjectArray );
		    }

	    const TInt tempArrayCount( iTempReadyPHArray.Count() );
	    for( TInt i( 0 ); i < tempArrayCount; i++ )
	        {
	        CHarvesterData* tempHd = iTempReadyPHArray[i];
            if(iReadyPHArray.Append( tempHd ) != KErrNone)
                 {
                 HarvestCompleted( tempHd->ClientId(), tempHd->Uri(), KErrNoMemory );
                 delete tempHd;
                 tempHd = NULL;
                 }
	        }
	    iTempReadyPHArray.Reset();
		
	    iHarvesterEventManager->IncreaseItemCount( EHEObserverTypePlaceholder, 
		        objectCount );
		iHarvesterEventManager->SendEventL( EHEObserverTypePlaceholder, EHEStateStarted, 
				iHarvesterEventManager->ItemCount( EHEObserverTypePlaceholder ) );
		
#ifdef _DEBUG
		for (TInt i = 0; i < objectCount; ++i)
			{
			CMdEObject* mdeObject = mdeObjectArray[i];
			if(mdeObject->Id() == 0)
				{
				WRITELOG1( "CHarvesterAO::HandlePlaceholdersL() - failed to add: %S", &mdeObject->Uri() );
				}
			}
#endif
		}

	iPHArray.ResetAndDestroy();
	
	CleanupStack::PopAndDestroy( &mdeObjectArray );
	
	WRITELOG( "CHarvesterAO::HandlePlaceholdersL() - end" );
	}

// ---------------------------------------------------------------------------
// CheckFileExtensionAndHarvestL
// ---------------------------------------------------------------------------
//
void CHarvesterAO::CheckFileExtensionAndHarvestL( CHarvesterData* aHD )
    {
    TBool isError = EFalse;
    CMdEObject* mdeObject = &aHD->MdeObject();
    const TDesC& uri = aHD->Uri();
    TBool objectExisted = ETrue;
    
    if( !mdeObject )
    	{
    	objectExisted = EFalse;
    	WRITELOG1( "CHarvesterAO::CheckFileExtensionAndHarvestL() - no mdeobject. URI: %S", &uri );
	    TBuf<KObjectDefStrSize> objDefStr;
		iHarvesterPluginFactory->GetObjectDefL( aHD, objDefStr );

        // GetObjectDef can cause context switch, and if unmount happens when this execution is 
        // interrupted, the ph data can be invalid. Thus, abort whole run, and start over to make sure 
        // the data is valid.
        if( !aHD )
            {
            return;
            }
		
		if( objDefStr.Length() == 0 )
			{
			WRITELOG( "CHarvesterAO::CheckFileExtensionAndHarvestL() - cannot get object definition" );
			OstTrace0( TRACE_NORMAL, CHARVESTERAO_CHECKFILEEXTENSIONANDHARVESTL, "CHarvesterAO::CheckFileExtensionAndHarvestL - cannot get object definition" );
			
			isError = ETrue;
			}
		else if( objDefStr == KInUse )
		    {
            aHD->SetErrorCode( KMdEErrHarvestingFailed );
            HarvestingCompleted( aHD );
            return;
		    }
		else
			{
			TInt mdeError( KErrNone );
			
			// Check if non-binary object (messages) already exists in db 
			if ( !aHD->IsBinary() )
				{
				CMdENamespaceDef& defNS = iMdESession->GetDefaultNamespaceDefL();
				CMdEObjectDef& mdeObjectDef = defNS.GetObjectDefL( objDefStr );
				TRAP( mdeError, mdeObject = iMdESession->OpenObjectL( aHD->Uri(), mdeObjectDef ));
				}
			
			if ( mdeObject )
				{
				aHD->SetTakeSnapshot( EFalse );
				aHD->SetEventType( EHarvesterEdit );
				}
			else
				{
				WRITELOG( "CHarvesterAO::CheckFileExtensionAndHarvestL() - getting mdeobject" );
				OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_CHECKFILEEXTENSIONANDHARVESTL, "CHarvesterAO::CheckFileExtensionAndHarvestL - getting mdeobject" );
				
				TRAP( mdeError, mdeObject = iMdeObjectHandler->GetMetadataObjectL( *aHD, objDefStr ) );
				}
			TInt harvesterError = KErrNone;
			if( mdeError != KErrNone)
				{
				WRITELOG1( "CHarvesterAO::CheckFileExtensionAndHarvestL() - cannot get mde object. error: %d", mdeError );
				OstTrace1( TRACE_NORMAL, DUP2_CHARVESTERAO_CHECKFILEEXTENSIONANDHARVESTL, "CHarvesterAO::CheckFileExtensionAndHarvestL - cannot get mde object. error: %d", mdeError );
				
				MdsUtils::ConvertTrapError( mdeError, harvesterError );
				if( harvesterError == KMdEErrHarvestingFailedPermanent )
					{
					WRITELOG( "CHarvesterAO::CheckFileExtensionAndHarvestL() - permanent fail" );
					OstTrace0( TRACE_NORMAL, DUP3_CHARVESTERAO_CHECKFILEEXTENSIONANDHARVESTL, "CHarvesterAO::CheckFileExtensionAndHarvestL - permanent fail" );
					
					isError = ETrue;
					}
				else if ( harvesterError == KMdEErrHarvestingFailed )
					{
	                WRITELOG( "CHarvesterAO::CheckFileExtensionAndHarvestL() - KMdEErrHarvestingFailed");
	                OstTrace0( TRACE_NORMAL, DUP4_CHARVESTERAO_CHECKFILEEXTENSIONANDHARVESTL, "CHarvesterAO::CheckFileExtensionAndHarvestL - KMdEErrHarvestingFailed" );
	                
	                aHD->SetErrorCode( KMdEErrHarvestingFailed );
	                HarvestingCompleted( aHD );
	                return;
					}
				}

			if( !mdeObject )
				{
				WRITELOG( "CHarvesterAO::CheckFileExtensionAndHarvestL() - mde object is null. stop harvesting" );
				OstTrace0( TRACE_NORMAL, DUP5_CHARVESTERAO_CHECKFILEEXTENSIONANDHARVESTL, "CHarvesterAO::CheckFileExtensionAndHarvestL - mde object is null. stop harvesting" );
				
				isError = ETrue;
				}
			}
		if( isError )
			{
            aHD->SetErrorCode( KMdEErrHarvestingFailedPermanent );
            HarvestingCompleted( aHD );
            return;
			}
		
		CleanupStack::PushL( aHD );
		
		TUint32 mediaId( 0 );
		if ( aHD->IsBinary() )
			{
			User::LeaveIfError( iMediaIdUtil->GetMediaId( uri, mediaId ) );
			}
		mdeObject->SetMediaId( mediaId );
		
		aHD->SetMdeObject( mdeObject );
		CleanupStack::Pop( aHD );
    	}
    
#ifdef _DEBUG
    WRITELOG1("CHarvesterAO::CheckFileExtensionAndHarvestL() - mdeobject URI: %S", &mdeObject->Uri() );
#endif
	
	aHD->SetPluginObserver( *this );
	
    if( objectExisted && aHD->EventType() == EHarvesterAdd )
    	{
    	iMdESession->RemoveObjectL( aHD->Uri() );
    	}
    
    // If context swich occures just right due to RemoveObjectL, check aHD for validity
    if( !aHD )
        {
        return;
        }
	
	TInt pluginErr = KErrNone;
    TRAPD( err, pluginErr = iHarvesterPluginFactory->HarvestL( aHD ));
    
    if ( err != KErrNone )
    	{
    	WRITELOG1( "CHarvesterAO::CheckFileExtensionAndHarvestL() - plugin error: %d", err );
    	OstTrace1( TRACE_NORMAL, DUP6_CHARVESTERAO_CHECKFILEEXTENSIONANDHARVESTL, "CHarvesterAO::CheckFileExtensionAndHarvestL - plugin error: %d", err );
    	
    	if ( err == KErrInUse )
    		{
            WRITELOG( "CHarvesterAO::CheckFileExtensionAndHarvestL() - item in use" );
            OstTrace0( TRACE_NORMAL, DUP7_CHARVESTERAO_CHECKFILEEXTENSIONANDHARVESTL, "CHarvesterAO::CheckFileExtensionAndHarvestL - item in use" );
            
            aHD->SetErrorCode( KMdEErrHarvestingFailed );
            HarvestingCompleted( aHD );
            return;
    		}
    	
    	aHD->SetErrorCode( KMdEErrHarvestingFailedUnknown );
    	HarvestingCompleted( aHD );
    	return;
    	}
    
    if( pluginErr != KErrNone )
    	{
    	aHD->SetErrorCode( KMdEErrHarvestingFailedUnknown );
    	HarvestingCompleted( aHD );
    	return;
    	}
    
    WRITELOG1("CHarvesterAO::CheckFileExtensionAndHarvestL() - ends with error %d", pluginErr );
    OstTrace1( TRACE_NORMAL, DUP8_CHARVESTERAO_CHECKFILEEXTENSIONANDHARVESTL, "CHarvesterAO::CheckFileExtensionAndHarvestL) - ends with error %d", pluginErr );
    }

// ---------------------------------------------------------------------------
// HarvestingCompleted
// ---------------------------------------------------------------------------
//
void CHarvesterAO::HarvestingCompleted( CHarvesterData* aHD )
    {
    WRITELOG( "CHarvesterAO::HarvestingCompleted()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted" );
        
    if ( aHD->ErrorCode() == KErrNone )
        {
        iReHarvester->CheckItem( *aHD );

        TInt mdeError = KErrNone;
        if( !aHD->TakeSnapshot() )
        	{
        	WRITELOG( "CHarvesterAO::HarvestingCompleted() origin is not camera or clf" );
        	OstTrace0( TRACE_NORMAL, DUP2_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted origin is not camera or clf" );
        	
            aHD->MdeObject().SetPlaceholder( EFalse );
            TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypePlaceholder ) );
        	TRAP( mdeError, iMdeObjectHandler->SetMetadataObjectL( *aHD ) );
        	}

        if(mdeError != KErrNone)
        	{
        	WRITELOG( "==============================ERROR===============================" );
            WRITELOG( "CHarvesterAO::HarvestingCompleted() - cannot set metadata object" );
            WRITELOG( "==============================ERROR done =========================" );
            OstTrace0( TRACE_NORMAL, DUP3_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted - cannot set metadata object" );
            
            delete aHD;
            aHD = NULL;

            TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 ) );
        	}
        else
        	{
        	WRITELOG( "CHarvesterAO::HarvestingCompleted() mdeError == KErrNone" );
        	OstTrace0( TRACE_NORMAL, DUP4_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted mdeError == KErrNone" );
        	
	        if ( aHD->TakeSnapshot() && iCtxEngine )
	            {
	            WRITELOG( "CHarvesterAO::HarvestingCompleted() - Taking a context snapshot." );
	            OstTrace0( TRACE_NORMAL, DUP5_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted - Taking a context snapshot." );
	            
	            iCtxEngine->ContextSnapshot( *this, *aHD );
	            }
	        else
	        	{
	        	TLocationData* locData = aHD->LocationData();
	        	if( locData )
	        		{
	        		WRITELOG( "CHarvesterAO::HarvestingCompleted() - Creating location object. " );
	        		OstTrace0( TRACE_NORMAL, DUP6_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted - Creating location object." );
	        		
	        		TInt loError( KErrNone ); 
	        		if( !iLocManipulatorConnected )
	        		    {
	        		    loError = iLocManipulator.Connect();
	        		    if( loError == KErrNone )
	        		        {
	        		        iLocManipulatorConnected = ETrue;
	        		        }
	        		    }    		
	        		
	        		if (loError == KErrNone)
	        			{
	        			TInt err = iLocManipulator.CreateLocationObject( *locData, aHD->MdeObject().Id() );
	        			if( err != KErrNone )
	        				{
	        				WRITELOG( "CHarvesterAO::HarvestingCompleted() - Location object creation failed!!!" );
	        				OstTrace0( TRACE_NORMAL, DUP7_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted - Location object creation failed!!!" );
	        				iLocManipulator.Close();
	        				iLocManipulatorConnected = EFalse;
	        				}
	        			}
	        		else
	        			{
	        			WRITELOG( "CHarvesterAO::HarvestingCompleted() - LocationObjectManipulator connect failed!!!" );
	        			OstTrace0( TRACE_NORMAL, DUP8_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted - LocationObjectManipulator connect failed!!" );	        			
	        			}
	        		}
	        	
	        	TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 ) );
	        	
				delete aHD;
				aHD = NULL;
	        	}
        	}        
        }
    else
        {
#ifdef _DEBUG
        WRITELOG( "==============================ERROR===============================" );
        WRITELOG1( "CHarvesterAO::HarvestingCompleted() - not OK! Error: %d", aHD->ErrorCode() );
        OstTrace1( TRACE_NORMAL, DUP9_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted - not OK! Error: %d", aHD->ErrorCode() );
        
#endif
				 
		const TInt errorCode( aHD->ErrorCode() );
        if ( errorCode== KMdEErrHarvestingFailed )
            {
#ifdef _DEBUG
            WRITELOG1("CHarvesterAO::HarvestingCompleted() - KMdEErrHarvestingFailed - %S - reharvesting", &aHD->Uri() );
            OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted - KMdEErrHarvestingFailed - reharvesting" );
#endif
            iReHarvester->AddItem( aHD );
            }
        else if ( errorCode == KMdEErrHarvestingFailedPermanent ||
                errorCode == KMdEErrHarvestingFailedUnknown )
            {
            WRITELOG( "CHarvesterAO::HarvestingCompleted() - KMdEErrHarvestingFailedPermanent - no need to re-harvest!" );
            OstTrace0( TRACE_NORMAL, DUP10_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted - KMdEErrHarvestingFailedPermanent - no need to re-harvest!" );            
            TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeMMC, 1 ) );
            
            delete aHD;
			aHD = NULL;
            }
        else
            {
            WRITELOG1( "CHarvesterAO::HarvestingCompleted() - unknown error: %d", errorCode );
            OstTrace1( TRACE_NORMAL, DUP11_CHARVESTERAO_HARVESTINGCOMPLETED, "CHarvesterAO::HarvestingCompleted - unknown error: %d", errorCode );
            
            delete aHD;
            aHD = NULL;
            }
        
        WRITELOG( "==============================ERROR done =========================" );
        OstTrace0( TRACE_NORMAL, DUP12_CHARVESTERAO_HARVESTINGCOMPLETED, "==============================ERROR done =========================" );
        
        }
    }

// ---------------------------------------------------------------------------
// HandleSessionOpened
// ---------------------------------------------------------------------------
//
void CHarvesterAO::HandleSessionOpened( CMdESession& aSession, TInt aError )
    {
    WRITELOG( "HarvesterThread::HandleSessionOpened()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened" );
        
    if ( KErrNone == aError )
        {
        TBool isTNMDaemonEnabled( EFalse );
        TRAP_IGNORE( CHarvesterCenRepUtil::IsThumbnailDaemonEnabledL( isTNMDaemonEnabled ) );

        if( isTNMDaemonEnabled )
            {
            StartThumbAGDaemon();
            }
        
        TRAPD( errorTrap, iMdEHarvesterSession = CMdEHarvesterSession::NewL( *iMdESession ) );
        if ( errorTrap == KErrNone )
            {
        	iMdeSessionInitialized = ETrue;
            }
        else
        	{
            WRITELOG( "CHarvesterAO::HandleSessionOpened() - error creating mde harvester session" );
            OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened) - error creating mde harvester session" );
        	}

        // Setting up context Engine (initialization is ready when ContextInitializationStatus -callback is called)
        TRAP( errorTrap, iCtxEngine = CContextEngine::GetInstanceL( this ) ); // Create the context engine 
        if ( errorTrap != KErrNone )
                {
                WRITELOG( "CHarvesterAO::HandleSessionOpened() - Context Engine creation failed" );
                OstTrace0( TRACE_NORMAL, DUP2_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - Context Engine creation failed" );                
                }
        
#ifdef _DEBUG        
        TRAP( errorTrap, iMdeObjectHandler = CMdeObjectHandler::NewL( *iMdESession ) );
        if ( errorTrap != KErrNone )
                {
                WRITELOG( "CHarvesterAO::HandleSessionOpened() - ObjectHandler creation failed" );
                OstTrace0( TRACE_NORMAL, DUP3_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - ObjectHandler creation failed" );
                }
        
        // Setting up monitor plugins
        TRAP( errorTrap, LoadMonitorPluginsL() );
        if ( errorTrap != KErrNone )
            {
            WRITELOG( "CHarvesterAO::HandleSessionOpened() - error loading monitor plugins" );
            OstTrace0( TRACE_NORMAL, DUP4_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - error loading monitor plugins" );            
            }

        // To check if the default namespace structure is in order
        TRAP( errorTrap, PreallocateNamespaceL( aSession.GetDefaultNamespaceDefL() ) );
        if ( errorTrap != KErrNone )
            {
            WRITELOG( "CHarvesterAO::HandleSessionOpened() - error loading default schema" );
            OstTrace0( TRACE_NORMAL, DUP5_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - error loading default schema" );            
            }
        
        TRAP( errorTrap, StartComposersL() );
        if ( errorTrap != KErrNone )
            {
            WRITELOG( "CHarvesterAO::HandleSessionOpened() - couldn't start composer plugins" );
            OstTrace0( TRACE_NORMAL, DUP6_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - couldn't start composer plugins" );
            
            }
#else
        // The idea here is that all of these  methods needs to be called,
        // even if some leave, thus the several TRAPs
        TRAP_IGNORE( iMdeObjectHandler = CMdeObjectHandler::NewL( *iMdESession ) );
        TRAP_IGNORE( PreallocateNamespaceL( aSession.GetDefaultNamespaceDefL() ) );
        TRAP_IGNORE( LoadMonitorPluginsL() );
        TRAP_IGNORE( StartComposersL() );        
#endif

            // Starting monitor plugins
        StartMonitoring();

        TRAP( errorTrap, iDiskFullNotifier = CMdSHarvesterDiskspaceObserverAO::NewL( *this, KDiskFullThreshold, KMdsSqlDbDefaultName ) );
        if ( errorTrap != KErrNone )
            {
            WRITELOG( "CHarvesterAO::HandleSessionOpened() - couldn't start diskspace observer" );
            OstTrace0( TRACE_NORMAL, DUP7_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - couldn't start diskspace observer" );            
            }
        
        TRAP( errorTrap, iOnDemandAO = COnDemandAO::NewL( *iMdESession, *iQueue, 
        		*iHarvesterPluginFactory, &iReadyPHArray ) );
        if ( errorTrap == KErrNone )
        	{
        	TRAP( errorTrap, iOnDemandAO->StartL() );
        	if ( errorTrap != KErrNone )
        		{
        		WRITELOG( "CHarvesterAO::HandleSessionOpened() - couldn't start on demand observer" );
        		OstTrace0( TRACE_NORMAL, DUP8_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - couldn't start on demand observer" );        		
        		}
        	}
        else
        	{
        	WRITELOG( "CHarvesterAO::HandleSessionOpened() - couldn't create on demand observer" );
        	OstTrace0( TRACE_NORMAL, DUP9_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - couldn't create on demand observer" );        	
        	}
    	
        // Initializing pause indicator
        iServerPaused = EFalse;
		
#ifdef _DEBUG
        WRITELOG( "HarvesterThread::HandleSessionOpened() - Succeeded!" );
        OstTrace0( TRACE_NORMAL, DUP10_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - Succeeded!" );
                
        TBool isRomScanEnabled( EFalse );
        TRAP_IGNORE( CHarvesterCenRepUtil::IsRomScanEnabledL( isRomScanEnabled ) );
        
        if( isRomScanEnabled )
            {
            TRAP( errorTrap, BootRomScanL() );
            if( errorTrap != KErrNone )
                {
                WRITELOG1( "CHarvesterAO::HandleSessionOpened() - BootRomScanL() returned error: %d", errorTrap );
                OstTrace1( TRACE_NORMAL, DUP11_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - BootRomScanL() returned error: %d", errorTrap );
                }
            }

        TRAP( errorTrap, BootPartialRestoreScanL() );
        if( errorTrap != KErrNone )
        	{
        	WRITELOG1( "CHarvesterAO::HandleSessionOpened() - BootPartialRestoreScanL() returned error: %d", errorTrap );
        	OstTrace1( TRACE_NORMAL, DUP12_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - BootPartialRestoreScanL() returned error: %d", errorTrap );        	
        	}
#else
        // The idea here is that all of these three methods needs to be called,
        // even if some leave, thus the two TRAPs
        TBool isRomScanEnabled( EFalse );
        TRAP_IGNORE( CHarvesterCenRepUtil::IsRomScanEnabledL( isRomScanEnabled ) );
        
        if( isRomScanEnabled )
            {
            TRAP_IGNORE( BootRomScanL() );
            }
        TRAP_IGNORE( BootPartialRestoreScanL() );
#endif
        
        // Store the internal mass memory media ID to DB, and update data if necessary
        TInt drive( -1 );
        TInt internalMassStorageError( DriveInfo::GetDefaultDrive( DriveInfo::EDefaultMassStorage, drive ) );
        if( internalMassStorageError == KErrNone )
            {
            TVolumeInfo internalMassStorageVolumeInfo;
            internalMassStorageError = iFs.Volume( internalMassStorageVolumeInfo, drive );
            if( internalMassStorageError == KErrNone )
                {
                const TUint32 massStorageMediaId( internalMassStorageVolumeInfo.iUniqueID );
                if( massStorageMediaId != 0 )
                    {
                    TUint32 mmcMediaId( 0 );
                    TInt mmcDrive( -1 );
                    TInt mmcError( DriveInfo::GetDefaultDrive( DriveInfo::EDefaultRemovableMassStorage, mmcDrive ) );
                    if( mmcError == KErrNone )
                        {
                        if( drive != mmcDrive )
                            {
                            TVolumeInfo mmcVolumeInfo;
                            mmcError = iFs.Volume( mmcVolumeInfo, mmcDrive );
                            if( mmcError == KErrNone )
                                {
                                mmcMediaId = mmcVolumeInfo.iUniqueID;
                                }                        
                            }
                        else
                            {
                            mmcMediaId = massStorageMediaId;
                            }
                        }
                
                    // If removable storage is not found, assume internal mass storage was mounted
                    if( mmcError )
                        {
                        iMdEHarvesterSession->CheckMassStorageMediaId( massStorageMediaId );                  
                        }
                    else if( massStorageMediaId != mmcMediaId )
                        {
                        iMdEHarvesterSession->CheckMassStorageMediaId( massStorageMediaId );
                        }          
                    }                    
                }
            }
        }
    else
        {
        iServerPaused = ETrue;    
        WRITELOG1( "HarvesterThread::HandleSessionOpened() - Failed: %d!", aError );
        OstTrace1( TRACE_NORMAL, DUP13_CHARVESTERAO_HANDLESESSIONOPENED, "CHarvesterAO::HandleSessionOpened - Failed: %d!", aError );
        }
    }

// ---------------------------------------------------------------------------
// HandleSessionError
// ---------------------------------------------------------------------------
//
void CHarvesterAO::HandleSessionError( CMdESession& /*aSession*/, TInt aError )
    {       
    if ( KErrNone != aError )
        {
        WRITELOG1( "HarvesterThread::HandleSessionError() - Error: %d!", aError );
        OstTrace1( TRACE_NORMAL, CHARVESTERAO_HANDLESESSIONERROR, "CHarvesterAO::HandleSessionError - Error: %d!", aError );
        }
    }

// ---------------------------------------------------------------------------
// ContextInitializationStatus
// ---------------------------------------------------------------------------
//
void CHarvesterAO::ContextInitializationStatus( TInt aErrorCode )
    {
    WRITELOG( "CHarvesterAO::ContextInitializationStatus()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_CONTEXTINITIALIZATIONSTATUS, "CHarvesterAO::ContextInitializationStatus" );    
    
    if ( KErrNone == aErrorCode )
        {
        WRITELOG( "HarvesterThread::ContextInitializationStatus() - Succeeded!" );
        OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_CONTEXTINITIALIZATIONSTATUS, "CHarvesterAO::ContextInitializationStatus - Succeeded!" );
        
        iContextEngineInitialized = ETrue;
        if ( iMdeSessionInitialized )
            {
            iCtxEngine->SetMdeSession( iMdESession );
            }
        }
    else
        {
        WRITELOG1( "HarvesterThread::ContextInitializationStatus() - Failed: %d!", aErrorCode );
        OstTrace1( TRACE_NORMAL, DUP2_CHARVESTERAO_CONTEXTINITIALIZATIONSTATUS, "CHarvesterAO::ContextInitializationStatus - Failed: %d!", aErrorCode );
        
        }
    }

// ---------------------------------------------------------------------------
// PauseHarvester
// ---------------------------------------------------------------------------
//
TInt CHarvesterAO::PauseHarvester()
    {
    WRITELOG( "CHarvesterAO::PauseHarvester()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_PAUSEHARVESTER, "CHarvesterAO::PauseHarvester" );
    
    iHarvesterPluginFactory->PauseHarvester( ETrue );
    iServerPaused = ETrue;
    
    // Everything is paused
    WRITELOG( "CHarvesterAO::PauseHarvester() - Moving paused state paused" );
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_PAUSEHARVESTER, "CHarvesterAO::PauseHarvester - Moving paused state paused" );
        
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// ResumeHarvester
// ---------------------------------------------------------------------------
//
void CHarvesterAO::ResumeHarvesterL()
    {
    WRITELOG( "CHarvesterAO::ResumeHarvesterL()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_RESUMEHARVESTERL, "CHarvesterAO::ResumeHarvesterL" );

    if( iRamFull || iDiskFull || iUnmountHandlingOngoing || iManualPauseEnabled )
        {
        return;
        }
    
    iHarvesterPluginFactory->PauseHarvester( EFalse );
    iServerPaused = EFalse;
    
    if( iNextRequest == ERequestIdle )
        {
        SetNextRequest( ERequestHarvest );
        }  
    }

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void CHarvesterAO::RunL()
    {
    WRITELOG( "CHarvesterAO::RunL" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_RUNL, "CHarvesterAO::RunL" );
    
    // check if pause is requested
    if ( this->iServerPaused && iNextRequest != ERequestPause && iNextRequest != ERequestResume)
    	{
    	iNextRequest = ERequestIdle;
    	}
    
    // Reset unmount flag, as unmount is handled before RunL is called again after aborted harvesting run
    iUnmountDetected = EFalse;
    // Reset priority flag, as interrupt is handled automatically first when RunL is called again 
    iPriorityInterruptDetected = EFalse;
    
    User::LeaveIfError( iStatus.Int() );
    switch( iNextRequest )
        {
        // no more items in queue
        case ERequestIdle:
            {
            WRITELOG( "CHarvesterAO::RunL - ERequestIdle" );
            OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_RUNL, "CHarvesterAO::RunL - ERequestIdle" );
            
            iReadyPHArray.Compress();
            iContainerPHArray.Compress();
            iPHArray.Compress();
            iTempReadyPHArray.Compress();
            
            if( iLocManipulatorConnected )
                {
                iLocManipulator.Close();
                iLocManipulatorConnected = EFalse;            
                }
            }
        break;

        // data added to harvester queue
        case ERequestHarvest:
            {
            WRITELOG( "CHarvesterAO::RunL - ERequestHarvest" );
            OstTrace0( TRACE_NORMAL, DUP2_CHARVESTERAO_RUNL, "CHarvesterAO::RunL - ERequestHarvest" );
            
            // harvest new items first
            if ( iQueue->ItemsInQueue() > 0 )
                {
                WRITELOG( "CHarvesterAO::RunL - Items in queue - calling ReadItemFromQueueL()" );
                ReadItemFromQueueL();
				SetNextRequest( ERequestHarvest );
				break;
                }

            // no more items to handle from main queue
            else
                {
                WRITELOG( "CHarvesterAO::RunL - No items in main queue" );
                // If interrupts occured, check the normal placeholder array for possible items to
                // be handled before moving on to container or ready placeholders
                if( iPHArray.Count() > 0 )
                    {
                    WRITELOG( "CHarvesterAO::RunL - Items found in placeholder array" );
                    TRAPD( err, HandlePlaceholdersL( ETrue ) );

                    // make sure that when HandlePlaceholdersL leaves unexpectedly, iPHArray is cleared
                    if ( err != KErrNone )
                        {
                        if( err != KErrDiskFull )
                            {
                            iPHArray.ResetAndDestroy();
                            iTempReadyPHArray.ResetAndDestroy();
                            }
                        User::Leave( err );
                        }
                    SetNextRequest( ERequestHarvest );
                    break;
                    }
                // All registered fast harvested items or placeholders handled at this point     
                // if container files to harvest, handle those next
                else if( iContainerPHArray.Count() > 0 )
                	{
                    WRITELOG( "CHarvesterAO::RunL - Items in iContainterPHArray - requesting ERequestContainerPlaceholder handling" );
                    iFastHarvestNeeded = EFalse;
                    iHarvestingPlaceholders = EFalse;
                    SetPriority( KHarvesterPriorityHarvestingPlugin );
                	SetNextRequest( ERequestContainerPlaceholder );
                	break;
                	}
                else if( iHarvestingPlaceholders || iFastHarvestNeeded )
                    {
                    WRITELOG( "CHarvesterAO::RunL - No items in iContainerPHArray" );
                    WRITELOG( "CHarvesterAO::RunL - PlaceholderHarvesting or FastHarvesting were enabled -> reset" );
                    WRITELOG( "CHarvesterAO::RunL - next request for ERequestHarvest handling" );
                    // reset to default priority       
                    iFastHarvestNeeded = EFalse;
                    iHarvestingPlaceholders = EFalse;
                    SetPriority( KHarvesterPriorityHarvestingPlugin );
                    SetNextRequest( ERequestHarvest );
                    break;
                    }
                
                WRITELOG( "CHarvesterAO::RunL - starting handling of iReadyPHArray items" );
                
                const TInt arrayCount( iReadyPHArray.Count() );
				if( arrayCount > 0 )
            		{
#ifdef _DEBUG
            		WRITELOG1("CHarvesterAO::RunL - items in ready pharray: %d", arrayCount );
            		OstTrace1( TRACE_NORMAL, DUP3_CHARVESTERAO_RUNL, "CHarvesterAO::RunL - items in ready pharray: %d", arrayCount );
#endif   		
            		TInt endIndex( KReadyPlaceholderQueueSize );
            		if( arrayCount < KReadyPlaceholderQueueSize )
            		    {
            		    endIndex = arrayCount;
            		    }
            		for( TInt i = 0; i < endIndex; i++ )
            			{
                		CheckFileExtensionAndHarvestL( iReadyPHArray[i] );
                		if( iUnmountDetected )
                		    {
                		    break;
                		    }
                		iReadyPHArray.Remove( i );
                        // correct the index so harvesting order remains ok
                        i--;
                        endIndex--;
            			}
            		if( iReadyPHArray.Count() )
            		    {
            		    SetNextRequest( ERequestHarvest );
            		    break;
            		    }
            		iReadyPHArray.Reset();
            		}

                SetNextRequest( ERequestIdle );
                }
            }
        break;
        
        case ERequestContainerPlaceholder:
        	{
#ifdef _DEBUG
        	WRITELOG( "CHarvesterAO::RunL - ERequestContainerPlaceholder" );
        	OstTrace0( TRACE_NORMAL, DUP4_CHARVESTERAO_RUNL, "CHarvesterAO::RunL - ERequestContainerPlaceholder" );        	
        	WRITELOG1( "CHarvesterAO::RunL - Items in container pharray: %d", iContainerPHArray.Count() );
        	OstTrace1( TRACE_NORMAL, DUP5_CHARVESTERAO_RUNL, "CHarvesterAO::RunL - Items in container pharray: %d", iContainerPHArray.Count() );
#endif
        	TInt count = iContainerPHArray.Count() > KContainerPlaceholderQueueSize ? KContainerPlaceholderQueueSize : iContainerPHArray.Count();
        	TInt i = 0;
        	while( i < count )
        		{
        		CHarvesterData* hd = iContainerPHArray[0];
        		if( iPHArray.Append( hd ) != KErrNone )
        		    {
        		    delete hd;
        		    hd = NULL;
        		    }
        		iContainerPHArray.Remove( 0 );
        		i++;
        		}
        	TRAPD( err, HandlePlaceholdersL( EFalse ) );

            // make sure that when HandlePlaceholdersL leaves unexpectedly, iPHArray is cleared
            if ( err != KErrNone )
                {
                if( err != KErrDiskFull )
                    {
                    iContainerPHArray.ResetAndDestroy();
                    iPHArray.ResetAndDestroy();
                    iTempReadyPHArray.ResetAndDestroy();
                    }
                User::Leave( err );
                }
	    	SetNextRequest( ERequestHarvest );
        	}
        break;
        
        // pause request
        case ERequestPause:
            {
            WRITELOG( "CHarvesterAO::RunL - ERequestPause" );
            OstTrace0( TRACE_NORMAL, DUP6_CHARVESTERAO_RUNL, "CHarvesterAO::RunL - ERequestPause" );
            User::LeaveIfError( PauseHarvester() );
            iManualPauseEnabled = ETrue;
            iHarvesterEventManager->SendEventL( EHEObserverTypeOverall, EHEStatePaused );
            if( iHarvesterStatusObserver )
            	{
            	iHarvesterStatusObserver->PauseReady( KErrNone );
            	}
            }
        break;

        // resume request
        case ERequestResume:
            {
            WRITELOG( "CHarvesterAO::RunL - ERequestResume" );
            OstTrace0( TRACE_NORMAL, DUP7_CHARVESTERAO_RUNL, "CHarvesterAO::RunL - ERequestResume" );
            iManualPauseEnabled = EFalse;
            // If for some reason, mds session is not (yet) ready, only inform that
            // server state is no longer paused, but do not restart actual harvesting
            // yet.
            if( !iMdeSessionInitialized )
                {
                iHarvesterEventManager->SendEventL( EHEObserverTypeOverall, EHEStateResumed );
                if( iHarvesterStatusObserver )
                    {
                    iHarvesterStatusObserver->ResumeReady( KErrNone );
                    }
                break;
                }
            ResumeHarvesterL();
            iHarvesterEventManager->SendEventL( EHEObserverTypeOverall, EHEStateResumed );
            if( iHarvesterStatusObserver )
            	{
                iHarvesterStatusObserver->ResumeReady( KErrNone );
            	}
            SetNextRequest( ERequestHarvest );
            }
        break;
        
        default:
            {
            WRITELOG( "CHarvesterAO::RunL - Not supported request" );
            OstTrace0( TRACE_NORMAL, DUP8_CHARVESTERAO_RUNL, "CHarvesterAO::RunL - Not supported request" );
            User::Leave( KErrNotSupported );
            }
        break;
        }
    }
    
// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CHarvesterAO::DoCancel()
    {
    WRITELOG( "CHarvesterAO::DoCancel()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_DOCANCEL, "CHarvesterAO::DoCancel" );
    }
    
// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//
TInt CHarvesterAO::RunError( TInt aError )
    {
    WRITELOG1( "CHarvesterAO::RunError, error: %d", aError );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_RUNERROR, "CHarvesterAO::RunError" );
    
    switch( iNextRequest )
        {
        case ERequestHarvest:
            {
            WRITELOG( "CHarvesterAO::RunError - state ERequestHarvest" );
            OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_RUNERROR, "CHarvesterAO::RunError - state ERequestHarvest" );
            if( aError == KErrDiskFull || aError == KErrNoMemory )
                {
                SetNextRequest( ERequestIdle );
                }
            }
        break;
        
        case ERequestContainerPlaceholder:
            {
            WRITELOG( "CHarvesterAO::RunError - state ERequestContainerPlaceholder" );
            OstTrace0( TRACE_NORMAL, DUP5_CHARVESTERAO_RUNERROR, "CHarvesterAO::RunError - state ERequestContainerPlaceholder" );
            if( aError == KErrDiskFull || aError == KErrNoMemory )
                {
                SetNextRequest( ERequestIdle );
                }
            }
        break;
        
        case ERequestPause:
            {
            WRITELOG( "CHarvesterAO::RunError - state ERequestPause" );
            OstTrace0( TRACE_NORMAL, DUP2_CHARVESTERAO_RUNERROR, "CHarvesterAO::RunError - state ERequestPause" );
            
            if ( aError == KErrNotReady )
                {
                SetNextRequest( ERequestPause );
                }
            else if( iHarvesterStatusObserver )
            	{
                iHarvesterStatusObserver->PauseReady( aError );
            	}
            }
        break;
        
        case ERequestResume:
            {
            WRITELOG( "CHarvesterAO::RunError - state ERequestResume" );
            OstTrace0( TRACE_NORMAL, DUP3_CHARVESTERAO_RUNERROR, "CHarvesterAO::RunError - state ERequestResume" );
            
            if( iHarvesterStatusObserver )
            	{
                iHarvesterStatusObserver->ResumeReady( aError );
            	}
            }
        break;
        
        default:
            {
            WRITELOG( "CHarvesterAO::RunError - unknown state" );
            OstTrace0( TRACE_NORMAL, DUP4_CHARVESTERAO_RUNERROR, "CHarvesterAO::RunError - unknown state" );
            }
        break;
        }
        
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// SetNextRequest
// ---------------------------------------------------------------------------
//
void CHarvesterAO::SetNextRequest( TRequest aRequest )
    {
    WRITELOG( "CHarvesterAO::SetNextRequest" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_SETNEXTREQUEST, "CHarvesterAO::SetNextRequest" );
    
    iNextRequest = aRequest;
            
    if ( !IsActive() )
        {
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* ptrStatus = &iStatus;
        User::RequestComplete( ptrStatus, KErrNone );
        }
    }

// ---------------------------------------------------------------------------
// IsServerPaused
// ---------------------------------------------------------------------------
//
TBool CHarvesterAO::IsServerPaused()
    {
    WRITELOG( "CHarvesterAO::IsServerPaused" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_ISSERVERPAUSED, "CHarvesterAO::IsServerPaused" );
    
    return iServerPaused;
    }

// ---------------------------------------------------------------------------
// From MBackupRestoreObserver.
// Called by CBlacklistBackupSubscriberAO when
// Backup&Restore is backing up or restoring.
// ---------------------------------------------------------------------------
//
void CHarvesterAO::BackupRestoreStart()
    {
    // close blacklist database connection
    WRITELOG( "CHarvesterAO::BackupRestoreStart" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_BACKUPRESTORESTART, "CHarvesterAO::BackupRestoreStart" );
    
    iBlacklist->CloseDatabase();
    }

// ---------------------------------------------------------------------------
// From MBackupRestoreObserver.
// Called by CBlacklistBackupSubscriberAO when
// Backup&Restore has finished backup or restore.
// ---------------------------------------------------------------------------
//
void CHarvesterAO::BackupRestoreReady()
    {
    // restart blacklist database connection
    WRITELOG( "CHarvesterAO::BackupRestoreReady" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_BACKUPRESTOREREADY, "CHarvesterAO::BackupRestoreReady" );
    
    iBlacklist->OpenDatabase();
    }

// ---------------------------------------------------------------------------
// From MMdSHarvesterDiskSpaceObserver.
// Called when change is disk space status occures
// ---------------------------------------------------------------------------
//
void CHarvesterAO::HandleDiskSpaceNotificationL( TDiskSpaceDirection aDiskSpaceDirection )
    {
    WRITELOG("CHarvesterAO::HandleDiskSpaceNotificationL()");
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_HANDLEDISKSPACENOTIFICATIONL, "CHarvesterAO::HandleDiskSpaceNotificationL" );
    
    if( MMdSHarvesterDiskSpaceObserver::EMore == aDiskSpaceDirection )
        {
        WRITELOG("CHarvesterAO::HandleDiskSpaceNotificationL() - disk space available");
        OstTrace0( TRACE_NORMAL, DUP2_CHARVESTERAO_HANDLEDISKSPACENOTIFICATIONL, "CHarvesterAO::HandleDiskSpaceNotificationL - disk space available" );
        iDiskFull = EFalse;
        }
    else
        {
        WRITELOG("CHarvesterAO::HandleDiskSpaceNotificationL() - disk full");
        OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_HANDLEDISKSPACENOTIFICATIONL, "CHarvesterAO::HandleDiskSpaceNotificationL - disk full" );        
        iDiskFull = ETrue;
        if( iServerPaused )
            {
            return;
            }
        }
    
    if( iDiskFull )
        {
        // cache monitored events
        PauseMonitoring();   
        PauseHarvester();    
        }
    else if( iServerPaused )
        {
        // resume monitoring
        ResumeMonitoring();
        TRAP_IGNORE( ResumeHarvesterL() );    
        }
    }

// ---------------------------------------------------------------------------
// HarvestFile
// ---------------------------------------------------------------------------
//
void CHarvesterAO::HarvestFile( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterAO::HarvestFile" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile" );
    
    const TInt KParamUri = 0;
    const TInt KParamAlbumIds = 1;
    const TInt KParamAddLocation = 2;
    
    // read uri
    HBufC* uri = HBufC::New( KMaxFileName );
    
    if ( ! uri )
        {
        WRITELOG( "CHarvesterAO::HarvestFile - out of memory creating uri container" );
        OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile - out of memory creating uri container" );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( KErrNoMemory );
            }
        return;
        }
    
    TPtr uriPtr( uri->Des() );
    TInt err = aMessage.Read( KParamUri, uriPtr );
    if ( err != KErrNone )
        {
        WRITELOG1( "CHarvesterAO::HarvestFile - error in reading aMessage (uri): %d", err );
        OstTrace1( TRACE_NORMAL, DUP2_CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile - error in reading aMessage (uri): %d", err );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( err );
            }
        delete uri;
        uri = NULL;
        return;
        }
    WRITELOG1( "CHarvesterAO::HarvestFile - uri: %S", uri );
    
    // read album ids
    RArray<TItemId> albumIds;
    const TInt KAlbumIdsLength = aMessage.GetDesLength( KParamAlbumIds );
    if ( KAlbumIdsLength > 0 )
        {
        HBufC8* albumIdBuf = HBufC8::New( KAlbumIdsLength );
        if ( !albumIdBuf )
            {
            WRITELOG( "CHarvesterAO::HarvestFile - error creating album id buffer." );
            OstTrace0( TRACE_NORMAL, DUP3_CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile - error creating album id buffer." );
            
            if (!aMessage.IsNull())
                {
                aMessage.Complete( KErrNoMemory );
                }
            delete uri;
            uri = NULL;
            return;
            }
        TPtr8 ptr( albumIdBuf->Des() );
        err = aMessage.Read( KParamAlbumIds, ptr );
        if ( err != KErrNone )
            {
            WRITELOG1( "CHarvesterAO::HarvestFile - error in reading aMessage (albumIds): %d", err );
            OstTrace1( TRACE_NORMAL, DUP4_CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile - error in reading aMessage (albumIds): %d", err );
            delete albumIdBuf;
            albumIdBuf = NULL;
            delete uri;
            uri = NULL;
            if (!aMessage.IsNull())
                {
                aMessage.Complete( err );
                }
            return;
            }

        TRAPD( err, DeserializeArrayL( ptr, albumIds ) );
        if ( err != KErrNone )
            {
            WRITELOG1( "CHarvesterAO::HarvestFile - error in reading album id array: %d", err );
            OstTrace1( TRACE_NORMAL, DUP5_CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile - error in reading album id array: %d", err );
            
            delete albumIdBuf;
            albumIdBuf = NULL;
            delete uri;
            uri = NULL;
            if (!aMessage.IsNull())
                {
                aMessage.Complete( err );
                }
            return;
            }

#ifdef _DEBUG
        const TInt count = albumIds.Count();
        for (TInt i = 0; i < count; ++i)
        	{
        	WRITELOG2( "RHarvesterClient::HarvestFile - album id[%d]: %d", i, albumIds[i] );
        	}
#endif
        
        delete albumIdBuf;
        albumIdBuf = NULL;
        
        WRITELOG1( "CHarvesterAO::HarvestFile - album id count: %d", albumIds.Count() );
        OstTrace1( TRACE_NORMAL, DUP6_CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile - album id count: %d", albumIds.Count() );
        
        }
    
    TBool addLocation;    
    TPckg<TBool> location( addLocation );	
		err = aMessage.Read(KParamAddLocation, location);
		if ( err != KErrNone )
			{
			WRITELOG1( "CHarvesterAO::HarvestFile - error in reading aMessage (addLocation): %d", err );
			OstTrace1( TRACE_NORMAL, DUP7_CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile - error in reading aMessage (addLocation): %d", err );
			
			delete uri;
			uri = NULL;
			if (!aMessage.IsNull())
				{
				aMessage.Complete( err );
				}
			return;
			}	
	
		WRITELOG1( "RHarvesterClient::HarvestFile - add location: %d", addLocation );
		OstTrace1( TRACE_NORMAL, DUP8_CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile - add location: %d", addLocation );
		
    
    CHarvesterData* hd = NULL;
    TRAP( err, hd = CHarvesterData::NewL( uri ) );
    if ( err != KErrNone || !hd )
        {
        WRITELOG( "CHarvesterAO::HarvestFile - creating harvUri failed" );
        OstTrace0( TRACE_NORMAL, DUP9_CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile - creating harvUri failed" );
        
        albumIds.Close();
        delete uri;
        uri = NULL;
        if (!aMessage.IsNull())
            {
            aMessage.Complete( err );
            }
        return;
        }

    hd->SetEventType( EHarvesterAdd );
    hd->SetOrigin( MdeConstants::Object::ECamera );
    hd->SetObjectType( EPlaceholder );
    hd->SetTakeSnapshot( ETrue );
    hd->SetClientId( aMessage.Identity() );
    hd->SetAddLocation( addLocation );

    CHarvestClientData* clientData = CHarvestClientData::New();
    if ( clientData )
        {
        clientData->SetAlbumIds( albumIds );
        hd->SetClientData( clientData ); // ownership is transferred
        }
    else
        {
        WRITELOG( "CHarvesterAO::HarvestFile - creating clientData failed" );
        OstTrace0( TRACE_NORMAL, DUP10_CHARVESTERAO_HARVESTFILE, "CHarvesterAO::HarvestFile - creating clientData failed" );        
        }

    if( iQueue )
        {
        iQueue->Append( hd );
        
        iPriorityInterruptDetected = ETrue;
        
        // signal to start harvest if harvester idles
        if ( !IsServerPaused() && iNextRequest == ERequestIdle )
            {
            SetNextRequest( CHarvesterAO::ERequestHarvest );
            }
        }
    else
        {
        delete hd;
        hd = NULL;
        err = KErrUnknown;
        }
    
    if (!aMessage.IsNull())
        {
        aMessage.Complete( err );
        }
    else if( err != KErrNone )
        {
        HarvestCompleted( aMessage.Identity(), uri->Des(), err );
        }
    
    albumIds.Close();
    }
  
// ---------------------------------------------------------------------------
// HarvestFileWithUID
// ---------------------------------------------------------------------------
//
void CHarvesterAO::HarvestFileWithUID( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterAO::HarvestFileWithUID" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID" );
    
    const TInt KParamUri = 0;
    const TInt KParamAlbumIds = 1;
    const TInt KParamAddLocation = 2;
    
    // read uri
    HBufC* uri = HBufC::New( KMaxFileName );
    
    if ( ! uri )
        {
        WRITELOG( "CHarvesterAO::HarvestFileWithUID - out of memory creating uri container" );
        OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID - out of memory creating uri container" );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( KErrNoMemory );
            }
        return;
        }
    
    TPtr uriPtr( uri->Des() );
    TInt err = aMessage.Read( KParamUri, uriPtr );
    if ( err != KErrNone )
        {
        WRITELOG1( "CHarvesterAO::HarvestFileWithUID - error in reading aMessage (uri): %d", err );
        OstTrace1( TRACE_NORMAL, DUP2_CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID - error in reading aMessage (uri): %d", err );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( err );
            }
        delete uri;
        uri = NULL;
        return;
        }
    WRITELOG1( "CHarvesterAO::HarvestFileWithUID - uri: %S", uri );
    
    // read album ids
    RArray<TItemId> albumIds;
    const TInt KAlbumIdsLength = aMessage.GetDesLength( KParamAlbumIds );
    if ( KAlbumIdsLength > 0 )
        {
        HBufC8* albumIdBuf = HBufC8::New( KAlbumIdsLength );
        if ( !albumIdBuf )
            {
            WRITELOG( "CHarvesterAO::HarvestFileWithUID - error creating album id buffer." );
            OstTrace0( TRACE_NORMAL, DUP3_CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID - error creating album id buffer." );
            
            if (!aMessage.IsNull())
                {
                aMessage.Complete( KErrNoMemory );
                }
            delete uri;
            uri = NULL;
            return;
            }
        TPtr8 ptr( albumIdBuf->Des() );
        err = aMessage.Read( KParamAlbumIds, ptr );
        if ( err != KErrNone )
            {
            WRITELOG1( "CHarvesterAO::HarvestFileWithUID - error in reading aMessage (albumIds): %d", err );
            OstTrace1( TRACE_NORMAL, DUP4_CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID - error in reading aMessage (albumIds): %d", err );
            
            delete albumIdBuf;
            albumIdBuf = NULL;
            delete uri;
            uri = NULL;
            if (!aMessage.IsNull())
                {
                aMessage.Complete( err );
                }
            return;
            }

        TRAPD( err, DeserializeArrayL( ptr, albumIds ) );
        if ( err != KErrNone )
            {
            WRITELOG1( "CHarvesterAO::HarvestFileWithUID - error in reading album id array: %d", err );
            OstTrace1( TRACE_NORMAL, DUP5_CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID - error in reading album id array: %d", err );
            
            delete albumIdBuf;
            albumIdBuf = NULL;
            delete uri;
            uri = NULL;
            if (!aMessage.IsNull())
                {
                aMessage.Complete( err );
                }
            return;
            }

#ifdef _DEBUG
        const TInt count = albumIds.Count();
        for (TInt i = 0; i < count; ++i)
            {
            WRITELOG2( "RHarvesterClient::HarvestFileWithUID - album id[%d]: %d", i, albumIds[i] );
            }
#endif
        
        delete albumIdBuf;
        albumIdBuf = NULL;
		
#ifdef _DEBUG        
        WRITELOG1( "CHarvesterAO::HarvestFileWithUID - album id count: %d", albumIds.Count() );
        OstTrace1( TRACE_NORMAL, DUP6_CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID - album id count: %d", albumIds.Count() );
#endif
        }
    
    TBool addLocation;    
    TPckg<TBool> location( addLocation );   
        err = aMessage.Read(KParamAddLocation, location);
        if ( err != KErrNone )
            {
            WRITELOG1( "CHarvesterAO::HarvestFileWithUID - error in reading aMessage (addLocation): %d", err );
            OstTrace1( TRACE_NORMAL, DUP7_CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID - error in reading aMessage (addLocation): %d", err );
            
            delete uri;
            uri = NULL;
            if (!aMessage.IsNull())
                {
                aMessage.Complete( err );
                }
            return;
            }   
    
        WRITELOG1( "RHarvesterClient::HarvestFileWithUID - add location: %d", addLocation );
        OstTrace1( TRACE_NORMAL, DUP8_CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID - add location: %d", addLocation );
        
    
    CHarvesterData* hd = NULL;
    TRAP( err, hd = CHarvesterData::NewL( uri ) );
    if ( err != KErrNone || !hd )
        {
        WRITELOG( "CHarvesterAO::HarvestFileWithUID - creating harvUri failed" );
        OstTrace0( TRACE_NORMAL, DUP9_CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID - creating harvUri failed" );
        
        albumIds.Close();
        delete uri;
        uri = NULL;
        if (!aMessage.IsNull())
            {
            aMessage.Complete( err );
            }
        return;
        }

    hd->SetEventType( EHarvesterAdd );
    hd->SetOrigin( MdeConstants::Object::EOther );
    hd->SetObjectType( EPlaceholder );
    hd->SetTakeSnapshot( ETrue );
    hd->SetClientId( aMessage.Identity() );
    hd->SetAddLocation( addLocation );

    CHarvestClientData* clientData = CHarvestClientData::New();
    if ( clientData )
        {
        clientData->SetAlbumIds( albumIds );
        hd->SetClientData( clientData ); // ownership is transferred
        }
    else
        {
        WRITELOG( "CHarvesterAO::HarvestFileWithUID - creating clientData failed" );
        OstTrace0( TRACE_NORMAL, DUP10_CHARVESTERAO_HARVESTFILEWITHUID, "CHarvesterAO::HarvestFileWithUID - creating clientData failed" );
        }

    if( iQueue )
    	{
    	iQueue->Append( hd );

    	// signal to start harvest if harvester idles
    	if ( !IsServerPaused() && iNextRequest == ERequestIdle )
    		{
    		SetNextRequest( CHarvesterAO::ERequestHarvest );
    		}
    	}
    else
        {
        delete hd;
        hd = NULL;
        err = KErrUnknown;
        }

    if (!aMessage.IsNull())
        {
        aMessage.Complete( err );
        }
    else if( err != KErrNone )
        {
        HarvestCompleted( aMessage.Identity(), uri->Des(), err );
        }
    
    albumIds.Close();
    }

// ---------------------------------------------------------------------------
// RegisterProcessOrigin()
// ---------------------------------------------------------------------------
//
void CHarvesterAO::RegisterProcessOrigin( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterAO::RegisterProcessOrigin" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_REGISTERPROCESSORIGIN, "CHarvesterAO::RegisterProcessOrigin" );
    
    if ( !iProcessOriginMapper )
        {
        WRITELOG( "CHarvesterAO::RegisterProcessOrigin - mapper not available." );
        OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_REGISTERPROCESSORIGIN, "CHarvesterAO::RegisterProcessOrigin - mapper not available." );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( KErrNotSupported );
            }
        return;
        }

    TUid processId = { 0 };
    processId.iUid = aMessage.Int0();
    if ( MdsUtils::IsValidProcessId( processId ) )
        {
        WRITELOG1( "CHarvesterAO::RegisterProcessOrigin - error reading processId. Read: %d", processId.iUid );
        OstTrace1( TRACE_NORMAL, DUP2_CHARVESTERAO_REGISTERPROCESSORIGIN, "CHarvesterAO::RegisterProcessOrigin - error reading processId. Read: %d", processId.iUid );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( KErrCorrupt );
            }
        return;
        }
    
    // read origin

    TOrigin origin = STATIC_CAST( TOrigin, aMessage.Int1() );
    WRITELOG1( "CHarvesterAO::RegisterProcessOrigin - origin: %d", origin );
    OstTrace1( TRACE_NORMAL, DUP3_CHARVESTERAO_REGISTERPROCESSORIGIN, "CHarvesterAO::RegisterProcessOrigin - origin: %d", origin );
    
    if ( origin < 0 )
        {
        WRITELOG( "CHarvesterAO::RegisterProcessOrigin - error reading origin from aMessage (negative)." );
        OstTrace0( TRACE_NORMAL, DUP4_CHARVESTERAO_REGISTERPROCESSORIGIN, "CHarvesterAO::RegisterProcessOrigin - error reading origin from aMessage (negative)." );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( KErrCorrupt );
            }
        return;
        }

    TRAPD( err, iProcessOriginMapper->RegisterProcessL( processId, origin ) );
    if ( err != KErrNone )
        {
        WRITELOG1( "CHarvesterAO::RegisterProcessOrigin - error registering mapping: %d", err );
        OstTrace1( TRACE_NORMAL, DUP5_CHARVESTERAO_REGISTERPROCESSORIGIN, "CHarvesterAO::RegisterProcessOrigin - error registering mapping: %d", err );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( err );
            }
        return;
        }
    if (!aMessage.IsNull())
        {
        aMessage.Complete( KErrNone );
        }
    }

// ---------------------------------------------------------------------------
// UnregisterProcessOrigin()
// ---------------------------------------------------------------------------
//
void CHarvesterAO::UnregisterProcessOrigin( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterAO::UnregisterProcessOrigin" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_UNREGISTERPROCESSORIGIN, "CHarvesterAO::UnregisterProcessOrigin" );
        
    if ( !iProcessOriginMapper )
        {
        WRITELOG( "CHarvesterAO::RegisterProcessOrigin - mapper not available." );
        OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_UNREGISTERPROCESSORIGIN, "CHarvesterAO::UnregisterProcessOrigin - mapper not available." );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( KErrNotSupported );
            }
        return;
        }

    TUid processId = { 0 };
    processId.iUid = aMessage.Int0();
    if ( MdsUtils::IsValidProcessId( processId ) )
        {
        WRITELOG1( "CHarvesterAO::UnregisterProcessOrigin - error reading processId. Read: %d", processId.iUid );
        OstTrace1( TRACE_NORMAL, DUP2_CHARVESTERAO_UNREGISTERPROCESSORIGIN, "CHarvesterAO::UnregisterProcessOrigin - error reading processId. Read: %d", processId.iUid );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( KErrCorrupt );
            }
        return;
        }
    
    TRAPD( err, iProcessOriginMapper->UnregisterProcessL( processId ) );
    if ( err != KErrNone )
        {
        WRITELOG1( "CHarvesterAO::UnregisterProcessOrigin - error unregistering mapping: %d", err );
        OstTrace1( TRACE_NORMAL, DUP3_CHARVESTERAO_UNREGISTERPROCESSORIGIN, "CHarvesterAO::UnregisterProcessOrigin - error unregistering mapping: %d", err );
        
        if (!aMessage.IsNull())
            {
            aMessage.Complete( err );
            }
        return;
        }
    if (!aMessage.IsNull())
        {
        aMessage.Complete( KErrNone );
        }
    }

// ---------------------------------------------------------------------------
// RegisterHarvestComplete()
// ---------------------------------------------------------------------------
//
TInt CHarvesterAO::RegisterHarvestComplete( const CHarvesterServerSession& aSession, const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterAO::RegisterHarvestComplete" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_REGISTERHARVESTCOMPLETE, "CHarvesterAO::RegisterHarvestComplete" );
        
    return iHarvestFileMessages.Append( 
    		THarvestFileRequest( aSession, aMessage ) );
    }

// ---------------------------------------------------------------------------
// UnregisterHarvestComplete()
// ---------------------------------------------------------------------------
//
TInt CHarvesterAO::UnregisterHarvestComplete( const CHarvesterServerSession& aSession )
    {    
    WRITELOG( "CHarvesterAO::UnregisterHarvestComplete" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_UNREGISTERHARVESTCOMPLETE, "CHarvesterAO::UnregisterHarvestComplete" );
    
    TInt err( KErrNotFound );
    if ( iHarvestFileMessages.Count() > 0 )
        {
        for ( TInt i = iHarvestFileMessages.Count()-1; i >= 0; --i )
            {
            THarvestFileRequest& req = iHarvestFileMessages[i];
            
            if ( req.iMessage.IsNull() )
            	{
            	iHarvestFileMessages.Remove( i );
            	continue;
            	}
            
            if( &req.iSession == &aSession )
            	{
            	err = KErrNone;
	            if (!req.iMessage.IsNull())
	            	{
	            	// cancels found request
	            	req.iMessage.Complete( KErrCancel );
	            	}
                iHarvestFileMessages.Remove( i );
                
                if( iHarvestFileMessages.Count() == 0 )
            		{
            		iHarvestFileMessages.Compress();
            		}
            	}
            }
        }

    return err;
    }

// ---------------------------------------------------------------------------
// RegisterHarvesterEvent()
// ---------------------------------------------------------------------------
//
void CHarvesterAO::RegisterHarvesterEvent( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterAO::RegisterHarvesterEvent" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_REGISTERHARVESTEREVENT, "CHarvesterAO::RegisterHarvesterEvent" );    

    TRAPD( err, iHarvesterEventManager->RegisterEventObserverL( aMessage ) );
    aMessage.Complete( err );
    }

// ---------------------------------------------------------------------------
// UnregisterHarvesterEvent()
// ---------------------------------------------------------------------------
//
void CHarvesterAO::UnregisterHarvesterEvent( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterAO::UnregisterHarvesterEvent" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_UNREGISTERHARVESTEREVENT, "CHarvesterAO::UnregisterHarvesterEvent" );    
    
    const TInt err = iHarvesterEventManager->UnregisterEventObserver( aMessage );
    aMessage.Complete( err );
    }

void CHarvesterAO::GetLastObserverId( const RMessage2& aMessage )
	{
	WRITELOG( "CHarvesterAO::GetLastObserverId" );
	OstTrace0( TRACE_NORMAL, CHARVESTERAO_GETLASTOBSERVERID, "CHarvesterAO::GetLastObserverId" );
	
	TUint observerId = iHarvesterEventManager->GetLastClientId();
	
	TPckg<TUint> pckgId( observerId );
	aMessage.Write( 0, pckgId );
	aMessage.Complete( KErrNone );
	}

// ---------------------------------------------------------------------------
// ContextSnapshotStatus
// ---------------------------------------------------------------------------
//
void CHarvesterAO::ContextSnapshotStatus( CHarvesterData* aHD )
    {
    WRITELOG( "CHarvesterAO::ContextSnapshotStatus()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_CONTEXTSNAPSHOTSTATUS, "CHarvesterAO::ContextSnapshotStatus" );
        
    HarvestCompleted( aHD->ClientId(), aHD->Uri(), aHD->ErrorCode() );

    const TInt errorCode = aHD->ErrorCode();
    if( errorCode != KErrNone && errorCode != KErrCompletion )
    	{
        WRITELOG1( "CHarvesterAO::ContextSnapshotStatus() - error occurred: %d", errorCode );
        OstTrace1( TRACE_NORMAL, DUP1_CHARVESTERAO_CONTEXTSNAPSHOTSTATUS, "CHarvesterAO::ContextSnapshotStatus - error occurred: %d", errorCode );        
    	}
    else
    	{
        if( errorCode == KErrCompletion )
            {
            WRITELOG( "CHarvesterAO::ContextSnapshotStatus() - snapshot could not be completed" );
            OstTrace0( TRACE_NORMAL, DUP2_CHARVESTERAO_CONTEXTSNAPSHOTSTATUS, "CHarvesterAO::ContextSnapshotStatus - snapshot could not be completed" );            
            WRITELOG( "CHarvesterAO::ContextSnapshotStatus() - processing non-context data anyway" );   
            OstTrace0( TRACE_NORMAL, DUP3_CHARVESTERAO_CONTEXTSNAPSHOTSTATUS, "CHarvesterAO::ContextSnapshotStatus - processing non-context data anyway" );
            }
        else
            {
            WRITELOG( "CHarvesterAO::ContextSnapshotStatus() - successfully completed" );
            OstTrace0( TRACE_NORMAL, DUP4_CHARVESTERAO_CONTEXTSNAPSHOTSTATUS, "CHarvesterAO::ContextSnapshotStatus - successfully completed" );
            }
        if( aHD->Origin() == MdeConstants::Object::ECamera )
        	{
            aHD->MdeObject().SetPlaceholder( EFalse );
            TRAP_IGNORE( iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypePlaceholder ) );
        	TRAPD(mdeError, iMdeObjectHandler->SetMetadataObjectL( *aHD ) );
        	if(mdeError != KErrNone)
            	{
            	WRITELOG( "==============================ERROR===============================" );
                WRITELOG( "CHarvesterAO::HarvestingCompleted() - cannot set metadata object" );
                WRITELOG( "==============================ERROR done =========================" );
                OstTrace0( TRACE_NORMAL, DUP5_CHARVESTERAO_CONTEXTSNAPSHOTSTATUS, "CHarvesterAO::ContextSnapshotStatus - cannot set metadata object" );                
            	}
        	}
    	}

    delete aHD;
    }

// ---------------------------------------------------------------------------
// IsConnectedToMde
// ---------------------------------------------------------------------------
//
TBool CHarvesterAO::IsConnectedToMde()
    {
    return iMdESession != NULL;
    }

// ---------------------------------------------------------------------------
// HarvestCompleted
// ---------------------------------------------------------------------------
//
void CHarvesterAO::HarvestCompleted( TUid aClientId, const TDesC& aUri, TInt aErr )
	{
	const TInt KParamUri = 0;
    // check if fast harvested file
    if ( iHarvestFileMessages.Count() > 0 )
        {
        for ( TInt i = iHarvestFileMessages.Count()-1; i >= 0; --i )
            {
            RMessage2& msg = iHarvestFileMessages[i].iMessage;
            if ( aClientId == msg.Identity() )
                {
                WRITELOG1( "CHarvesterAO::HarvestingCompleted() - Completing Fast Harvest request! Error code: %d", aErr );
                OstTrace1( TRACE_NORMAL, CHARVESTERAO_HARVESTCOMPLETED, "CHarvesterAO::HarvestCompleted - Completing Fast Harvest request! Error code: %d", aErr );
                
                if (!msg.IsNull())
                	{
                	msg.Write( KParamUri, aUri );
                	msg.Complete( aErr );
                	}
                else
                	{
                	WRITELOG("CHarvesterAO::HarvestingCompleted() NOT COMPLETING AS msg->iMessage->IsNull returns ETrue");
                	OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_HARVESTCOMPLETED, "CHarvesterAO::HarvestCompleted NOT COMPLETING AS msg->iMessage->IsNull returns ETrue" );
                	} 
                iHarvestFileMessages.Remove( i );
                if( iHarvestFileMessages.Count() == 0 )
                    {
                    iHarvestFileMessages.Compress();
                    }
                }
            }
        }
	}

void CHarvesterAO::BootRomScanL()
	{
	WRITELOG("CHarvesterAO::BootRomScanL()");
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_BOOTROMSCANL, "CHarvesterAO::BootRomScanL" );

    if( !iMdeSessionInitialized )
        {
        return;
        }
	
	RPointerArray<TScanItem> scanItems;
	TCleanupItem cleanupItem( MdsUtils::CleanupPtrArray<TScanItem>, &scanItems );
    CleanupStack::PushL( cleanupItem );

	CHarvesterCenRepUtil::GetScanItemsL( scanItems );

	RPointerArray<HBufC> ignorePaths;
	TCleanupItem cleanupItem2( MdsUtils::CleanupPtrArray<HBufC>, &ignorePaths );
    CleanupStack::PushL( cleanupItem2 );

	CHarvesterCenRepUtil::GetIgnoredScanPathsL( ignorePaths );

	BootScanL( scanItems, ignorePaths, ETrue );

	CleanupStack::PopAndDestroy( &ignorePaths );
	CleanupStack::PopAndDestroy( &scanItems );
	}

void CHarvesterAO::BootPartialRestoreScanL()
	{
	// check if partial restore was done before last boot
	TBool partialRestore = iRestoreWatcher->Register();
	
#if defined(__WINSCW__) || defined(FF_PLATFORM_SIMULATOR)
	partialRestore = ETrue;
#endif
	
	if( !iMdeSessionInitialized )
	    {
	    return;
	    }
	
    if( partialRestore )
        {
        iMdEHarvesterSession->ChangeCDriveMediaId();

        WRITELOG("CHarvesterAO::BootPartialRestoreScanL() - partial restore");  
        }
    else
        {
        WRITELOG("CHarvesterAO::BootPartialRestoreScanL() - preinstalled content bootup scan");
        }
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_BOOTPARTIALRESTORESCANL, "CHarvesterAO::BootPartialRestoreScanL - prepare bootup scan" );
    
	RPointerArray<TScanItem> scanItems;
	TCleanupItem cleanupItem( MdsUtils::CleanupPtrArray<TScanItem>, &scanItems );
    CleanupStack::PushL( cleanupItem );

    if( partialRestore )
        {
        CHarvesterCenRepUtil::GetPartialRestorePathsL( scanItems );
        }

    RPointerArray<TScanItem> preinstalledItems;
    TCleanupItem cleanupItem2( MdsUtils::CleanupPtrArray<TScanItem>, &preinstalledItems );
    CleanupStack::PushL( cleanupItem2 );
    
    // Scan for preinstalled content anyways
    CHarvesterCenRepUtil::GetAlwaysScanOnBootPathsL( preinstalledItems );

    for( TInt i = preinstalledItems.Count() - 1; i >=0; i-- )
        {
        TScanItem* preinstalledLocation = preinstalledItems[i];
        preinstalledLocation->iPreinstalled = MdeConstants::MediaObject::EPreinstalled;
        scanItems.Append( preinstalledLocation );
        }
    preinstalledItems.Reset(); 

	RPointerArray<HBufC> ignorePaths;
	TCleanupItem cleanupItem3( MdsUtils::CleanupPtrArray<HBufC>, &ignorePaths );
    CleanupStack::PushL( cleanupItem3 );

	CHarvesterCenRepUtil::GetIgnoredPartialRestorePathsL( ignorePaths );

	BootScanL( scanItems, ignorePaths, EFalse );
	
	WRITELOG("CHarvesterAO::BootPartialRestoreScanL() - iRestoreWatcher->UnregisterL()");
	OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERAO_BOOTPARTIALRESTORESCANL, "CHarvesterAO::BootPartialRestoreScanL - iRestoreWatcher->UnregisterL()" );
	
	iRestoreWatcher->UnregisterL();

	CleanupStack::PopAndDestroy( &ignorePaths );
	CleanupStack::PopAndDestroy( &preinstalledItems );
	CleanupStack::PopAndDestroy( &scanItems );
	}

TBool CHarvesterAO::IsDescInArray(const TPtrC& aSearch, const RPointerArray<HBufC>& aArray)
	{
	for( TInt i = aArray.Count() - 1; i >=0; i-- )
		{
		const TDesC& ignorePath = aArray[i]->Des();
		
		TInt result = MdsUtils::Compare( aSearch, ignorePath );
		
		if( result == 0 )
			{
			return ETrue;
			}
		}

	return EFalse;
	}

void CHarvesterAO::BootScanL( RPointerArray<TScanItem>& aScanItems, 
		const RPointerArray<HBufC>& aIgnorePaths,
        TBool aCheckDrive )
	{
	WRITELOG("CHarvesterAO::BootScanL() - begin");
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_BOOTSCANL, "CHarvesterAO::BootScanL - begin" );
	
	TVolumeInfo volumeInfo;
	iFs.Volume( volumeInfo, EDriveC );

	iMdEHarvesterSession->SetFilesToNotPresent( volumeInfo.iUniqueID, ETrue );
	
	_LIT( KDirectorySeparator, "\\" );

#ifdef _DEBUG
	WRITELOG1("CHarvesterAO::BootScanL() - item count: %d", aScanItems.Count() );
	OstTrace1( TRACE_NORMAL, DUP1_CHARVESTERAO_BOOTSCANL, "CHarvesterAO::BootScanL - item count: %d", aScanItems.Count() );
#endif
	
	RPointerArray<CHarvesterData> hdArray;
	CleanupClosePushL( hdArray );
	TBool rootDir( ETrue );
	
	while( aScanItems.Count() > 0 )
		{
		HBufC* folder = aScanItems[0]->iPath;
		TUint32 preinstalled = aScanItems[0]->iPreinstalled;

		CDir* directory = NULL;
		TInt error( KErrNone );
        if( rootDir )
            {
            error = iFs.GetDir( folder->Des(), KEntryAttDir, KHarvesterGetRootDirFlags, directory );
            rootDir = EFalse;
            }
        else
            {
            error = iFs.GetDir( folder->Des(), KEntryAttDir, KHarvesterGetDirFlags, directory );
            }

		if ( error == KErrNone )
			{
			CleanupStack::PushL( directory );

			TInt count = directory->Count();
			
			TUint32 mediaId( 0 );
			
			if( count > 0 )
				{
				TInt drive = 0;
				if( iFs.CharToDrive( (folder->Des())[0], drive ) == KErrNone )
					{
					TVolumeInfo volInfo;
					if( iFs.Volume( volInfo, drive ) == KErrNone )
						{
						mediaId = volInfo.iUniqueID;
						}
					}
				}			
			
			for ( TInt i = 0; i < count; i++ )
				{
				TEntry entry = (*directory)[i];
				
				TInt length = folder->Length() + entry.iName.Length() + KDirectorySeparator().Length();
				HBufC* name = HBufC::NewLC( length );
				name->Des().Append( *folder );
				TPtrC ptr = *folder;
				if( ptr[ ptr.Length() - 1 ] == TChar('\\') )
					{
					name->Des().Append( entry.iName );
					}
			
				if ( entry.IsDir() )
					{					
					name->Des().Append( KDirectorySeparator );
					TPtrC path = *name;
					if( !aCheckDrive )
						{
						path.Set( (*name).Mid( 2 ) );
						}
					if( !IsDescInArray( path, aIgnorePaths ) )
						{
						WRITELOG("CHarvesterAO::BootScanL() - scanFolders.AppendL");
						OstTrace0( TRACE_NORMAL, DUP2_CHARVESTERAO_BOOTSCANL, "CHarvesterAO::BootScanL - scanFolders.AppendL" );
						TScanItem* item = new (ELeave) TScanItem();
						CleanupStack::PushL( item );
						item->iPath = name->AllocL();
						item->iPreinstalled = preinstalled;
						CleanupStack::Pop( item );
						aScanItems.AppendL( item ); // ownership is transferred
						}
					}
				else
					{
					TPtrC filename = *name;
					if( !aCheckDrive )
						{
						filename.Set( (*name).Mid( 2 ) );
						}
					if( !IsDescInArray( filename, aIgnorePaths ) )
						{
						WRITELOG("CHarvesterAO::BootScanL() - check files");
						OstTrace0( TRACE_NORMAL, DUP3_CHARVESTERAO_BOOTSCANL, "CHarvesterAO::BootScanL - check files" );
						
					    RArray<TPtrC> uris;
					    RArray<TMdSFileInfo> fileInfos;
					    RArray<TFilePresentStates> results;
					    CleanupClosePushL( uris );
					    CleanupClosePushL( fileInfos );
					    CleanupClosePushL( results );
						
					    TMdSFileInfo fileInfo;
					    fileInfo.iModifiedTime = entry.iModified.Int64();
					    fileInfo.iSize = entry.iSize;
					    fileInfos.Append( fileInfo );
					    uris.Append( name->Des() );
						
					    TFilePresentStates found;
					    
					    if( mediaId == volumeInfo.iUniqueID )
					        {
					        iMdEHarvesterSession->SetFilesToPresentL( volumeInfo.iUniqueID, uris, fileInfos, results );
					        found = results[ 0 ];
					        }
					    else
					        {
					        found = EMdsNotFound;
					        }
						
						// scan file if it was not found from DB, or if it has been modified
						if( found == EMdsNotFound ||
						    found == EMdsPlaceholder ||
						    found == EMdsModified )
						    {
	                        CPlaceholderData* phData = CPlaceholderData::NewL();
	                        CleanupStack::PushL( phData );
	                        phData->SetUri( *name );
	                        phData->SetModified( entry.iModified );
	                        phData->SetFileSize( entry.iSize );
	                        phData->SetMediaId( mediaId );
	                        phData->SetPreinstalled( preinstalled );

	                        CHarvesterData* hd = CHarvesterData::NewL( name->AllocL() );
	                        hd->SetEventType( EHarvesterAdd );
	                        hd->SetObjectType( EPlaceholder );
	                        hd->SetOrigin( MdeConstants::Object::EOther );
	                        hd->SetClientData( phData );

	                        CleanupStack::Pop( phData );
							
	                        if(hdArray.Append( hd ) != KErrNone )
	                            {
                                delete hd;
                                hd = NULL;
	                            }
						    }
						CleanupStack::PopAndDestroy( &results );
						CleanupStack::PopAndDestroy( &fileInfos );
						CleanupStack::PopAndDestroy( &uris );
						}
					}
				CleanupStack::PopAndDestroy( name );
				}
			
			CleanupStack::PopAndDestroy( directory );
			}
		folder = NULL;
		delete aScanItems[0];
		aScanItems.Remove( 0 );
		}
	
	WRITELOG("CHarvesterAO::BootScanL() - iQueue->Append");
	OstTrace0( TRACE_NORMAL, DUP4_CHARVESTERAO_BOOTSCANL, "CHarvesterAO::BootScanL - iQueue->Append" );
	
	iQueue->MonitorEvent( hdArray );
	CleanupStack::PopAndDestroy( &hdArray ); 

	iMdEHarvesterSession->RemoveFilesNotPresent( volumeInfo.iUniqueID, ETrue );
	
	WRITELOG("CHarvesterAO::BootScanL() - end");
	OstTrace0( TRACE_NORMAL, DUP5_CHARVESTERAO_BOOTSCANL, "CHarvesterAO::BootScanL - end" );
	}

void CHarvesterAO::SetHarvesterStatusObserver( MHarvesterStatusObserver* aObserver )
	{
	iHarvesterStatusObserver = aObserver;
	}

TBool CHarvesterAO::UnharvestedItemsLeftInPlugins()
	{
	RPointerArray<CHarvesterPluginInfo>& infos = iHarvesterPluginFactory->GetPluginInfos();
	const TInt count = infos.Count();
	for ( TInt i = count; --i >= 0; )
		{
		if ( infos[i]->iQueue.Count() > 0 )
			{
			return ETrue;
			}
		}
	
	return EFalse;
	}

void CHarvesterAO::PreallocateNamespaceL( CMdENamespaceDef& aNamespaceDef )
	{
	for( TInt i = aNamespaceDef.ObjectDefCount() - 1; i >=0; i-- )
		{
		CMdEObjectDef& objectDef = aNamespaceDef.ObjectDefL( i );

		for( TInt j = objectDef.PropertyDefCount() - 1; j >=0; j-- )
			{
		    // No need for a local copy of the returned pointer
			objectDef.PropertyDefL( j );
			}
		}
	}

void CHarvesterAO::StartThumbAGDaemon()
    {
    TInt res( KErrNone );
    
    // create server - if one does not already exist
    TFindServer findServer( KTAGDaemonName );
    TFullName name;
    if ( findServer.Next( name ) != KErrNone )
        {
        RProcess server;
        // Create the server process
        // KNullDesC param causes server's E32Main() to be run
        res = server.Create( KTAGDaemonExe, KNullDesC );
        if ( res != KErrNone )
            {
            return;
            }

        // Process created successfully
        TRequestStatus status;
        server.Rendezvous( status );
        
        if ( status != KRequestPending )
            {
            server.Kill( 0 );     // abort startup
            }
        else
            {
            server.Resume();    // logon OK - start the server
            }       

        // Wait until the completion of the server creation
        User::WaitForRequest( status );

        server.Close(); // we're no longer interested in the other process
        }    
    }

void CHarvesterAO::MemoryLow()
	{
    WRITELOG("CHarvesterAO::MemoryLow()");
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_MEMORYLOW, "CHarvesterAO::MemoryLow" );
    
    iRamFull = ETrue;
    
    if( iServerPaused )
        {
        return;
        }

	// cache monitored events
	PauseMonitoring();
	PauseHarvester();
	
    iReadyPHArray.Compress();
    iContainerPHArray.Compress();
    iPHArray.Compress();
    iTempReadyPHArray.Compress();
	}

void CHarvesterAO::MemoryGood()
	{
    WRITELOG("CHarvesterAO::MemoryGood()");    
    OstTrace0( TRACE_NORMAL, CHARVESTERAO_MEMORYGOOD, "CHarvesterAO::MemoryGood" );
    
    iRamFull = EFalse;
    
    if( iServerPaused )
        {
        // resume monitoring
        ResumeMonitoring();
    
        TRAP_IGNORE( ResumeHarvesterL() );    
        }
	}

TBool CHarvesterAO::CheckForCameraItem( CHarvesterData* aHd, TDes& aObjectDef )
    {
    if( aHd->Origin() == MdeConstants::Object::ECamera )
        {
        TPtrC ext;
        if( !MdsUtils::GetExt( aHd->Uri(), ext ) )
            {
            return EFalse;
            }
        TInt pos( 0 );
        if( iCameraExtensionArray->FindIsq( ext, pos ) == 0 ) // video extension matched
            {
            aObjectDef.Copy( KVideo );
            return ETrue;
            }
        }
    return EFalse;
    }

void CHarvesterAO::RemoveBlacklistedFile( CHarvesterData* aItem )
    {
    if( iMdeSessionInitialized )
        {
        TRAP_IGNORE( iMdESession->RemoveObjectL( aItem->Uri() ) );
        }
    }

void CHarvesterAO::AddDefaultFolderDataL( CMdEObject* aObject )
    {
    TPtrC objectDefName( aObject->Def().Name() );
    if( objectDefName == MdeConstants::Image::KImageObject )
        {
        const TDesC& uri = aObject->Uri();
        if( uri.FindF( iMmcImagesPath->Des()) != KErrNotFound ||
            uri.FindF( iPhoneImagesPath->Des()) != KErrNotFound ||
            uri.FindF( KDCIMFolder ) != KErrNotFound )
            {
            TBool inDefaultFolder( ETrue );
            aObject->AddBoolPropertyL( *iPropDefs->iDefaultFolderPropertyDef, inDefaultFolder );
            }
        else
            {
            TBool inDefaultFolder( EFalse );
            aObject->AddBoolPropertyL( *iPropDefs->iDefaultFolderPropertyDef, inDefaultFolder );  
            }    
        }
    else if( objectDefName == MdeConstants::Video::KVideoObject )
        {
        const TDesC& uri = aObject->Uri();
        if( uri.FindF( iMmcVideosPath->Des()) != KErrNotFound ||
            uri.FindF( iPhoneVideosPath->Des()) != KErrNotFound ||
            uri.FindF( KDCIMFolder ) != KErrNotFound )
            {
            TBool inDefaultFolder( ETrue );
            aObject->AddBoolPropertyL( *iPropDefs->iDefaultFolderPropertyDef, inDefaultFolder );
            }
        else
            {
            TBool inDefaultFolder( EFalse );
            aObject->AddBoolPropertyL( *iPropDefs->iDefaultFolderPropertyDef, inDefaultFolder );   
            }    
        }
    else if( objectDefName == MdeConstants::Audio::KAudioObject )
        {
        const TDesC& uri = aObject->Uri();
        if( uri.FindF( iMmcSoundsPath->Des()) != KErrNotFound ||
            uri.FindF( iPhoneSoundsPath->Des()) != KErrNotFound )
            {
            TBool inDefaultFolder( ETrue );
            aObject->AddBoolPropertyL( *iPropDefs->iDefaultFolderPropertyDef, inDefaultFolder );
            }
        else
            {
            TBool inDefaultFolder( EFalse );
            aObject->AddBoolPropertyL( *iPropDefs->iDefaultFolderPropertyDef, inDefaultFolder );    
            }     
        }
    else
        {
        // Other objects are set to be in default location by default
        TBool inDefaultFolder( ETrue );
        aObject->AddBoolPropertyL( *iPropDefs->iDefaultFolderPropertyDef, inDefaultFolder );
        }
    }

CHarvesterAO::TRequest CHarvesterAO::GetHarvesterAORunState()
    {
    return iNextRequest;
    }

