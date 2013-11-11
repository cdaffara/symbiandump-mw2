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
* Description:  Monitors MMC insertions and removals.*
*/


#include "mmcmonitorplugin.h"
#include "harvesterlog.h"
#include "mdsfileserverpluginclient.h"
#include "fsutil.h"
#include "harvestercenreputil.h"
#include <driveinfo.h>

#ifdef __WINSCW__
#include <pathinfo.h>
#endif

#include <e32cmn.h>

_LIT( KColon, ":" );

// construct/destruct
CMMCMonitorPlugin* CMMCMonitorPlugin::NewL()
    {
    CMMCMonitorPlugin* self = new (ELeave) CMMCMonitorPlugin();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

void CMMCMonitorPlugin::ConstructL() // second-phase constructor
    {
    WRITELOG( "CMMCMonitorPlugin::ConstructL" );

    User::LeaveIfError( iFs.Connect() );
    
    iMMCMonitor = CMMCMonitorAO::NewL();
    iMountTask = CMMCMountTaskAO::NewL();
    iUsbMonitor = CMMCUsbAO::NewL();
    iMdEClient = NULL;
    }
    
CMMCMonitorPlugin::~CMMCMonitorPlugin() // destruct
    {
    WRITELOG( "CMMCMonitorPlugin::~CMMCMonitorPlugin" );
    
    if (iMMCMonitor)
    	{
    	iMMCMonitor->StopMonitoring();
    	delete iMMCMonitor;
    	iMMCMonitor = NULL;
    	}
    
	if (iUsbMonitor)
    	{
    	iUsbMonitor->StopMonitoring();
    	delete iUsbMonitor;
    	iUsbMonitor = NULL;
    	}
    
	if (iMountTask)
    	{
    	delete iMountTask;
    	iMountTask = NULL;
    	}
	
	delete iMmcScanner;
	iMmcScanner = NULL;
	delete iHddScanner;
	iHddScanner = NULL;
	
	iFs.Close();
    }

TBool CMMCMonitorPlugin::StartMonitoring( MMonitorPluginObserver& aObserver,
    CMdESession* aMdEClient, CContextEngine* /*aCtxEngine*/, 
    CHarvesterPluginFactory* aHarvesterPluginFactory )
    {
    WRITELOG( "CMMCMonitorPlugin::StartMonitoring" );

    iObserver = &aObserver;
    TRAPD( err, iMdEClient = CMdEHarvesterSession::NewL( *aMdEClient ) );
    if( err != KErrNone )
    	{
    	return EFalse;
    	}
    
    iMountTask->SetMonitorObserver( aObserver );
    iMountTask->SetMdeSession( iMdEClient );
    iMountTask->SetHarvesterPluginFactory( aHarvesterPluginFactory );
    
    // get present media IDs before last shutdown
    RArray<TMdEMediaInfo> medias;
    TRAP_IGNORE( iMdEClient->GetPresentMediasL( medias ) );
	
    // update db present status 
    TRAP( err, StartMonitoringAllMMCsL( medias ) );
    if ( err != KErrNone )
        {
        return EFalse;
        }
    
    TChar driveLetter( 0 );
	TBool presentState( EFalse );
    
	TUint32 hdMediaId( 0 );
	TBool hdPresent( EFalse );
    hdMediaId = iMountTask->GetInternalDriveMediaId( hdPresent );
    
    for( TInt i = medias.Count() - 1; i >=0; i-- )
    	{
    	TRAP_IGNORE( iMdEClient->GetMediaL( medias[i].iMediaId, driveLetter, presentState ) );
    	
    	if ( presentState && medias[i].iMediaId != hdMediaId )
    		{
    		// scan MMC if card was in phone
    	
    	    if( iMmcScanner )
    	        {
    	        iMmcScanner->Cancel();
    	        delete iMmcScanner;
    	        iMmcScanner = NULL;
    	        }
    		TRAP_IGNORE( iMmcScanner = CMmcScannerAO::NewL( medias[i].iMediaId, iMdEClient, iObserver,
    		    				aHarvesterPluginFactory, CActive::EPriorityUserInput ) );
    		}
    	}
 
    if( hdMediaId == 0 || !hdPresent )
        {
        // Try to fetch internall mass storage media id again if it was not mounted
        hdMediaId = iMountTask->GetInternalDriveMediaId( hdPresent );
        }
    
    // scan mass storage to catch all chances even if battery dies during operation that should  be catched
    if( hdMediaId )
		{
		TBool exists( EFalse );
		TRAP_IGNORE( exists= iMdEClient->GetMediaL( hdMediaId, driveLetter, presentState ) );
		
		if ( exists && hdPresent )
			{
			WRITELOG("CMMCMonitorPlugin::StartMonitoring - start mass storage scan");
			
			TMdEMediaInfo hdInfo;
			hdInfo.iMediaId = hdMediaId;
			hdInfo.iDrive = driveLetter;
			medias.Append( hdInfo );
			
			TRAP_IGNORE( iHddScanner = CMmcScannerAO::NewL( hdMediaId, iMdEClient, iObserver,
			    				aHarvesterPluginFactory, KHarvesterCustomImportantPriority ));
			}
		}

    iMMCMonitor->StartMonitoring( *this, medias );
    
    medias.Close();
 
   	return iUsbMonitor->StartMonitoring( *this );
    }

TBool CMMCMonitorPlugin::StopMonitoring()
    {
    WRITELOG( "CMMCMonitorPlugin::StopMonitoring" );
    
    iMMCMonitor->StopMonitoring();
    return iUsbMonitor->StopMonitoring();
    }

TBool CMMCMonitorPlugin::ResumeMonitoring( MMonitorPluginObserver& /*aObserver*/,
    CMdESession* /*aMdEClient*/, CContextEngine* /*aCtxEngine*/,
    CHarvesterPluginFactory* /*aHarvesterPluginFactory*/ )
    {
    WRITELOG( "CMMCMonitorPlugin::ResumeMonitoring" );
    iMountTask->SetCachingStatus( EFalse );
    return ETrue;
    }

TBool CMMCMonitorPlugin::PauseMonitoring()
    {
    WRITELOG( "CMMCMonitorPlugin::PauseMonitoring" ); // DEBUG INFO
    iMountTask->SetCachingStatus( ETrue );
    return ETrue;
    }

// constructor support
// don't export these, because used only by functions in this DLL, eg our NewLC()
CMMCMonitorPlugin::CMMCMonitorPlugin() // first-phase C++ constructor
    {
    // No implementation required
    }

void CMMCMonitorPlugin::AddNotificationPathL( TChar aDrive )
    {
    WRITELOG( "CMMCMonitorPlugin::AddNotificationPath" );

    // 1 in length is for aDrive
    HBufC* path = HBufC::NewLC( 1 + KColon.iTypeLength );
    TPtr pathPtr = path->Des();
    pathPtr.Append( aDrive );
    pathPtr.Append( KColon );    
    
	CHarvesterCenRepUtil* cenRepoUtil = CHarvesterCenRepUtil::NewLC();
	cenRepoUtil->AddIgnorePathsToFspL( pathPtr );
	cenRepoUtil->FspEngine().AddNotificationPath( pathPtr );
	CleanupStack::PopAndDestroy( cenRepoUtil );    
    CleanupStack::PopAndDestroy( path );
    }

void CMMCMonitorPlugin::MountEvent( TChar aDriveChar, TUint32 aMediaID, TMMCEventType aEventType )
    {
    WRITELOG( "CMMCMonitorPlugin::MountEvent" );

    TMountData* mountData = NULL;
    mountData = new TMountData;
    
    if ( !mountData )
        {
        return;
        }
    if( aMediaID != 0 && aEventType == EMounted)
    	{
	    TUint status;
	    TInt drive;
	    iFs.CharToDrive( aDriveChar, drive );
		if( DriveInfo::GetDriveStatus( iFs, drive, status ) == KErrNone )
			{
			//The "Out of disk space" mde query uses the MdE_Preferences table
			if( !(status & DriveInfo::EDriveInternal) )
				{
				iMdEClient->AddMemoryCard( aMediaID );
				}
			}
    	}

    mountData->iDrivePath.Append( aDriveChar );
    mountData->iDrivePath.Append( KColon );
    mountData->iMediaID = aMediaID;
    
    switch ( aEventType )
        {
        case EMounted:
            {
            WRITELOG( "CMMCMonitorPlugin::MountEvent with parameter EMounted" );
            mountData->iMountType = TMountData::EMount;
            
            // If mass storage mounting was delayed in boot so that mount event
            // occures in mmc monitor, update the mass storage media id in the
            // db in case factory settings were reseted and mass storage formatted
            TInt drive( -1 );
            TInt internalMassStorageError( DriveInfo::GetDefaultDrive( DriveInfo::EDefaultMassStorage, drive ) );
            if( internalMassStorageError == KErrNone )
                {
                TVolumeInfo internalMassStorageVolumeInfo;
                internalMassStorageError = iFs.Volume( internalMassStorageVolumeInfo, drive );
                if( internalMassStorageError == KErrNone )
                    {
                    const TUint32 massStorageMediaId( internalMassStorageVolumeInfo.iUniqueID );
                    if( massStorageMediaId == aMediaID &&
                        massStorageMediaId != 0 )
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
                            iMdEClient->CheckMassStorageMediaId( massStorageMediaId );              
                            }
                        else if( massStorageMediaId != mmcMediaId )
                            {
                            iMdEClient->CheckMassStorageMediaId( massStorageMediaId );
                            }          
                        }                    
                    }
                }
            
            TRAPD(err, iMountTask->StartMountL( *mountData ))
			
            if(err != KErrNone )
                {
                delete mountData;
                mountData = NULL;
                }
            }
        break;
        
        case EDismounted:
            {
            if( aMediaID == 0 )
            	{
            	TRAP_IGNORE( mountData->iMediaID = FSUtil::GetPreviousMediaIDL( iMdEClient, aDriveChar ) );
            	}
            if( mountData->iMediaID )
            	{
	            WRITELOG( "CMMCMonitorPlugin::MountEvent with parameter EDismounted" );
	            mountData->iMountType = TMountData::EUnmount;
	            TRAPD(err, iMountTask->StartUnmountL( *mountData ));
	            
	            if(err != KErrNone )
	                {
	                delete mountData;
	                mountData = NULL;
	                }
            	}
            else
                {
                delete mountData;
                mountData = NULL;
                }
            }
        break;
        
        case EFormatted:
            {
            WRITELOG( "CMMCMonitorPlugin::MountEvent with parameter EFormatted" );
            mountData->iMountType = TMountData::EFormat;
            TRAPD(err, iMountTask->StartUnmountL( *mountData ));
			
            if(err != KErrNone)
                {
                delete mountData;
                mountData = NULL;
                }
            }
        break;
        
        default:
            {
#ifdef _DEBUG
            _LIT( KLogPanic, "unknown state" );
            User::Panic( KLogPanic, KErrArgument );
#endif
            }
        break;
        }
    }

void CMMCMonitorPlugin::StartMonitoringAllMMCsL( RArray<TMdEMediaInfo>& aMedias )
    {
    WRITELOG( "CMMCMonitorPlugin::StartMonitoringAllMMCs" );

    TDriveInfo driveInfo;
    TDriveList driveList;
    TInt numOfElements( 0 );
    DriveInfo::GetUserVisibleDrives( iFs, 
                                                        driveList, 
                                                        numOfElements, 
                                                        KDriveAttExclude | KDriveAttRemote | KDriveAttRom );

#ifdef __WINSCW__
    TFileName systemPath = PathInfo::GetPath( PathInfo::EPhoneMemoryRootPath );
    TInt systemDriveNum( -1 );
    iFs.CharToDrive( systemPath[0], systemDriveNum );
#endif
    
    TInt i( 0 );
    TChar drive;
    const TInt acount = driveList.Length();
    const TInt mediaCount = aMedias.Count();
    TInt count( 0 );
    
    // set removed medias to not present
    for ( i = 0; i < mediaCount; i++ )
    	{
    	TInt driveNum(0);
    	iFs.CharToDrive( aMedias[i].iDrive, driveNum );
    	TUint32 mediaId = FSUtil::MediaID( iFs, driveNum );
    	if ( mediaId != aMedias[i].iMediaId ) 
    		{
    		iMdEClient->SetMediaL( aMedias[i].iMediaId, aMedias[i].iDrive, EFalse );
    		}
    	}
    
    for ( i = 0; i < acount; i++ )
        {
#ifdef __WINSCW__
        if ( i == systemDriveNum )
            {
            continue;
            }
#endif
    
        if ( driveList[i] > 0 )
            {
            TUint driveStatus( 0 );
            DriveInfo::GetDriveStatus( iFs, i, driveStatus ); 

            if ( driveStatus & DriveInfo::EDriveUsbMemory )
                {
                driveList[i] = 0;
                continue;
                }
            
            iFs.Drive( driveInfo, i );
            if( ( (driveStatus & DriveInfo::EDriveRemovable) ||
                   (driveInfo.iType == EMediaHardDisk && driveStatus & DriveInfo::EDriveInternal) ||
                   (driveInfo.iDriveAtt & KDriveAttLogicallyRemovable) ||
                   (driveInfo.iDriveAtt & KDriveAttRemovable) ) &&
                (driveInfo.iType != EMediaNotPresent) )
                {
                count++; // DEBUG INFO
                
                iFs.DriveToChar( i, drive );
                
                // set media id to MdE
                TUint32 mediaId = FSUtil::MediaID( iFs, i );
                if ( mediaId != 0 )
                    {
                    iMdEClient->SetMediaL( mediaId, drive, ETrue );

                    AddNotificationPathL( drive );
                    }
                }
            }
        }
    
    WRITELOG1( "CMMCMonitorPlugin::StartMonitoringAllMMCs found %d MMCs", count );
    }

