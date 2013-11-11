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
* Description:  Handles mount tasks*
*/

#include <driveinfo.h>

#ifdef __WINSCW__
#include <pathinfo.h>
#endif

#include <placeholderdata.h>
#include "mmcmounttaskao.h"
#include "mmcmonitorplugin.h"
#include "harvesterlog.h"
#include "mdsfileserverpluginclient.h"
#include "mdeharvestersession.h"
#include "harvesterdata.h"
#include "mdsutils.h"
#include "harvestercenreputil.h"
#include "fsutil.h"
#include "harvesterplugininfo.h"
#include "harvesterpluginfactory.h"

//-----------------------------------------------------------------------------
// CMMCMountTaskAO
//-----------------------------------------------------------------------------
CMMCMountTaskAO* CMMCMountTaskAO::NewL()
	{
	WRITELOG( "CMMCMountTaskAO::NewL" );
	
	CMMCMountTaskAO* self = new (ELeave) CMMCMountTaskAO();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

void CMMCMountTaskAO::ConstructL()
	{
	WRITELOG("CMMCMountTaskAO::ConstructL");
	
	CActiveScheduler::Add( this );
	User::LeaveIfError( iFs.Connect() );
	iNextRequest = ERequestIdle;
	iMmcFileList = CMmcFileList::NewL();
	iCacheEvents = EFalse;
	iHEM = CHarvesterEventManager::GetInstanceL();
	}

CMMCMountTaskAO::CMMCMountTaskAO() :
		CActive( KHarvesterPriorityMonitorPlugin )
	{
	WRITELOG( "CMMCMountTaskAO::CMMCMountTaskAO" );
	}
	
CMMCMountTaskAO::~CMMCMountTaskAO()
	{
	WRITELOG( "CMMCMountTaskAO::~CMMCMountTaskAO" );
	Cancel();
	iFs.Close();
	
	iMountDataQueue.ResetAndDestroy();
	iMountDataQueue.Close();
	iEntryArray.ResetAndDestroy();
	iEntryArray.Close();
	iHarvestEntryArray.ResetAndDestroy();
	iHarvestEntryArray.Close();
	
	delete iMdeSession;
	iMdeSession = NULL;
  
	Deinitialize();
	
	if (iHEM)
		{
		iHEM->ReleaseInstance();
		}
	
	delete iMmcFileList;
	iMmcFileList = NULL;
	iHdArray.ResetAndDestroy();
	iHdArray.Close();
	}
	
void CMMCMountTaskAO::SetMonitorObserver( MMonitorPluginObserver& aObserver )
	{
	WRITELOG( "CMMCMountTaskAO::SetMonitorObserver" );
	iObserver = &aObserver;
	}

void CMMCMountTaskAO::SetMdeSession( CMdEHarvesterSession* aMdeSession )
	{
	iMdeSession = aMdeSession;
	}

void CMMCMountTaskAO::SetHarvesterPluginFactory( CHarvesterPluginFactory* aPluginFactory )
	{
	iHarvesterPluginFactory = aPluginFactory;
	}
	
void CMMCMountTaskAO::StartMountL( TMountData& aMountData )
	{
	WRITELOG("CMMCMountTaskAO::StartMount");
	
	// Remove pending mount request for the same drive
	// if for example USB cable is pluged and unpluged
	// several times in a row
	for( TInt i = iMountDataQueue.Count() - 1; i >=0; i-- )
	    {
	    WRITELOG( "CMMCMountTaskAO::StartUnmountL - checking for pending mount notifications" );
	    TMountData* tempData = iMountDataQueue[i];
	    if( tempData->iMediaID == aMountData.iMediaID &&
	        tempData->iMountType == TMountData::EMount &&
	        aMountData.iMountType == TMountData::EMount &&
	        tempData->iDrivePath == aMountData.iDrivePath )
	        {
	        WRITELOG( "CMMCMountTaskAO::StartUnmountL - removing obsolite mount notifications" );
	        iMountDataQueue.Remove(i);
	        delete tempData;
	        tempData = NULL;
	        }
	    }

	User::LeaveIfError( iMountDataQueue.Append( &aMountData ));
	
	if ( iNextRequest == ERequestIdle )
		{
		SetNextRequest( ERequestStartTask );
		}
	}
	
void CMMCMountTaskAO::StartUnmountL(TMountData& aMountData)
	{
	WRITELOG("CMMCMountTaskAO::StartUnmount");
	
	// make sure that drive is not currently mounting
	if ( iMountData )
		{
		if ( iMountData->iDrivePath.Compare( aMountData.iDrivePath ) == 0 )
			{
			Cancel();
			Deinitialize();
			}
		}

    // Remove pending unmount request for the same drive
    // if for example USB cable is pluged and unpluged
    // several times in a row
    for( TInt i = iMountDataQueue.Count() - 1; i >=0; i-- )
        {
        WRITELOG( "CMMCMountTaskAO::StartUnmountL - checking for pending unmount notifications" );
        TMountData* tempData = iMountDataQueue[i];
        if( tempData->iMediaID == aMountData.iMediaID &&
            tempData->iMountType == TMountData::EUnmount &&
            aMountData.iMountType == TMountData::EUnmount &&
            tempData->iDrivePath == aMountData.iDrivePath )
            {
            WRITELOG( "CMMCMountTaskAO::StartUnmountL - removing obsolite unmount notifications" );
            iMountDataQueue.Remove(i);
            delete tempData;
            tempData = NULL;
            }
        }
	
	User::LeaveIfError( iMountDataQueue.Append( &aMountData ));
		
	SetNextRequest( ERequestStartTask );
	}
	
void CMMCMountTaskAO::RunL()
	{
	WRITELOG1( "CMMCMountTaskAO::RunL iStatus: %d", iStatus.Int() );
	
	User::LeaveIfError( iStatus.Int() );
	
	if ( iCacheEvents )
		{
		if ( iMountData )
			{
			if( iMountDataQueue.Insert( iMountData, 0 ) != KErrNone)
			    {
                delete iMountData;
			    }
			iMountData = NULL;
			}
		Deinitialize();
		return;
		}
	
	switch( iNextRequest )
		{
		case ERequestStartTask:
			{
			WRITELOG( "CMMCMountTaskAO::RunL - ERequestStartTask" );
			if ( iMountData )
				{
				delete iMountData;
				iMountData = NULL;
				}
			
			iHdArray.ResetAndDestroy();
			
			if( iMountDataQueue.Count() > 0 )
				{
				iMountData = iMountDataQueue[0];
				iMountDataQueue.Remove(0);

				WRITELOG1( "iMountData.iMountType: %d", iMountData->iMountType );
				WRITELOG1( "iMountData.iDrivePath: %S", &iMountData->iDrivePath );
				WRITELOG1( "iMountData.iMediaID: %u", iMountData->iMediaID );

				if ( iMountData->iMountType == TMountData::EMount )
					{
					SetNextRequest( ERequestMount );
					}
				else if ( iMountData->iMountType == TMountData::EUnmount )
					{
					SetNextRequest( ERequestUnmount );
					}
				else if ( iMountData->iMountType == TMountData::EFormat )
					{
					SetNextRequest( ERequestFormat );
					}
				}
			else
				{
				SetNextRequest( ERequestIdle );
				iMountDataQueue.Compress();
				}
			}
		break;
		
		case ERequestMount:
			{
			WRITELOG( "CMMCMountTaskAO::RunL - ERequestMount" );
			Initialize();
			StartNotifyL();
			SetNotPresentToMDE();
			TRAPD( err, iMmcFileList->BuildFileListL( iFs, iMountData->iDrivePath, iEntryArray ));
			if ( err == KErrNoMemory )
				{
				if( iMountDataQueue.Insert( iMountData, 0 ) != KErrNone)
				    {
                    delete iMountData;
				    }
					
				iMountData = NULL;
				Deinitialize();
				SetNextRequest( ERequestStartTask );
				break;
				}
			
			// send start event
			const TInt entryCount = iEntryArray.Count();
			if( entryCount > 0 )
				{
				iHEM->IncreaseItemCount( EHEObserverTypeMMC, entryCount );
				iHEM->SendEventL( EHEObserverTypeMMC, EHEStateStarted, iHEM->ItemCount( EHEObserverTypeMMC ) );
				}

			SetNextRequest( ERequestHandleFileEntry );
			}
		break;
		
		case ERequestUnmount:
			{
			WRITELOG( "CMMCMountTaskAO::RunL - ERequestUnmount" );
			
			const TUint entryCount = iEntryArray.Count();
			WRITELOG1( "CMMCMountTaskAO::RunL - ERequestUnmount entryCount = %d", entryCount );
            if( entryCount )
        		{
        		iHEM->DecreaseItemCountL( EHEObserverTypeMMC, entryCount );
		        }
            
            const TUint harvestEntryCount = iHarvestEntryArray.Count();
            WRITELOG1( "CMMCMountTaskAO::RunL - ERequestUnmount harvestEntryCount = %d", harvestEntryCount );
            if( harvestEntryCount )
                {
                iHEM->DecreaseItemCountL( EHEObserverTypeMMC, harvestEntryCount );
                }
			
			RMsgQueue<TInt> unmountQueue;
			_LIT( KUnmountHandlerAOString, "unmounthandlerao" );
			const TInt err = unmountQueue.OpenGlobal( KUnmountHandlerAOString );
			if( err == KErrNone )
				{
				unmountQueue.Send( iMountData->iMediaID );
				}
			
			SetNotPresentToMDE();
			StopNotifyL();
			SetNextRequest( ERequestCleanup );
			}
		break;
		
		case ERequestFormat:
			{
			WRITELOG( "CMMCMountTaskAO::RunL - ERequestFormat" );
			SetNotPresentToMDE();
			RemoveNotPresentFromMDE();
			StopNotifyL();
			SetNextRequest( ERequestCleanup );
			}
		break;
		
		case ERequestHandleFileEntry:
			{
			WRITELOG( "CMMCMountTaskAO::RunL - ERequestHandleFileEntry" );
			
			if( iNextRequest == ERequestStartTask )
                {
                WRITELOG( "CMMCMountTaskAO::RunL - ERequestHandleFileEntry stop processing media is unmounted");
                SetNextRequest( ERequestStartTask );
				return;
                }
            else if ( iEntryArray.Count() > 0 )
				{
				TRAPD( err, iMmcFileList->HandleFileEntryL( *iMdeSession, iEntryArray, 
						iHarvestEntryArray, iMountData->iMediaID, iHarvesterPluginFactory ));
				if ( err != KErrNone )
					{
					if( err == KErrNoMemory )
						{
						if(iMountDataQueue.Insert( iMountData, 0 ) != KErrNone)
						    {
                            delete iMountData;
						    }
							
						iMountData = NULL;
						}
						
					Deinitialize();
					SetNextRequest( ERequestStartTask );
					break;
					}
				
	            if ( iEntryArray.Count() > 0 )
	                {
	                SetNextRequest( ERequestHandleFileEntry );
	                break;
	                }
				}
			
		    if ( iHarvestEntryArray.Count() > 0 )
				{
				SetNextRequest( ERequestHandleReharvest );
				break;
				}
			else
				{
                // All detected entries are set as present at this stage, clear not present entries
                RemoveNotPresentFromMDE();
                SetNextRequest( ERequestCleanup );
                break;
				}
			}
		
		case ERequestHandleReharvest:
			{
			WRITELOG( "CMMCMountTaskAO::RunL - ERequestHandleReharvest" );
			
			if( iNextRequest == ERequestStartTask )
                {
                WRITELOG( "CMMCMountTaskAO::RunL - ERequestHandleReharvest stop processing media is unmounted");
                SetNextRequest( ERequestStartTask );
				return;
                }
         	else if ( iHarvestEntryArray.Count() > 0 )
				{
				HandleReharvestL( iHarvestEntryArray );
				SetNextRequest( ERequestHandleReharvest );
				break;
				}
          	else
				{
                if ( iObserver )
                    {
                    if( iHdArray.Count() > 0)
                        {
                        iObserver->MonitorEvent( iHdArray );
                        }
                    }
                iHdArray.Reset();
                iHdArray.Compress();
				SetNextRequest( ERequestHandleFileEntry );
				break;
				}
			}
		
		case ERequestCleanup:
			{
			WRITELOG( "CMMCMountTaskAO::RunL - ERequestCleanup" );
			TBool present = (iMountData->iMountType == TMountData::EMount);
			iMdeSession->SetMediaL( iMountData->iMediaID, iMountData->iDrivePath[0], present );
			Deinitialize();
			SetNextRequest( ERequestStartTask );
			}
		break;
		
		case ERequestIdle:
			{
			WRITELOG( "CMMCMountTaskAO::RunL - ERequestIdle" );
			// all done
			}
		break;
		
		default:
            User::Leave( KErrNotSupported );
		break;
		}
	}

#ifdef _DEBUG
TInt CMMCMountTaskAO::RunError( TInt aError )
#else
TInt CMMCMountTaskAO::RunError( TInt )
#endif
	{
	WRITELOG1( "CMMCMountTaskAO::RunError with error code: %d", aError );
	Deinitialize();
	SetNextRequest( ERequestStartTask );
	return KErrNone;
	}

void CMMCMountTaskAO::DoCancel()
	{
	WRITELOG( "CMMCMountTaskAO::DoCancel" );
	}

void CMMCMountTaskAO::SetNextRequest( TRequest aRequest )
	{
	WRITELOG("CMMCMountTaskAO::SetNextRequest" );
	if ( !IsActive() )
		{
		iNextRequest = aRequest;
		TRequestStatus* ptrStatus = &iStatus;
		User::RequestComplete( ptrStatus, KErrNone );
		SetActive();
		}
	}

void CMMCMountTaskAO::SetNotPresentToMDE()
	{
	WRITELOG1("CMMCMountTaskAO::SetNotPresentToMDE - MediaID %u", iMountData->iMediaID);
	if ( iMountData->iMediaID )
		{
		iMdeSession->SetFilesToNotPresent( iMountData->iMediaID );
		}
	else
		{
		WRITELOG("CMMCMountTaskAO::SetNotPresentToMDE - MediaID 0, not setting");
		}
	}
	
void CMMCMountTaskAO::HandleReharvestL( RPointerArray<CPlaceholderData>& aArray )
	{
	WRITELOG("CMMCMountTaskAO::HandleReharvestL");
	
	const TInt count( aArray.Count() );
	TInt batchSize( KMmcEntryBufferSize );
	if ( count < KMmcEntryBufferSize )
		{
	    batchSize = count;
		}

	const TInt endIndex( count - batchSize );
	
	for ( TInt i = count; --i >= endIndex; )
		{
		CPlaceholderData* ei = aArray[i];
		
		HBufC* fileName = ei->Uri().AllocLC();
		CHarvesterData* hd = CHarvesterData::NewL( fileName );
		hd->SetOrigin( MdeConstants::Object::EOther );
		CleanupStack::Pop( fileName );
		
		if ( ei->PresentState() == EMdsPlaceholder || 
			 ei->PresentState() == EMdsModified )
			{
			hd->SetEventType( EHarvesterEdit );
			hd->SetObjectType( ENormal );
			delete ei;
			ei = NULL;
			}
		else
			{
			hd->SetEventType( EHarvesterAdd );
			hd->SetObjectType( EPlaceholder );
			hd->SetClientData( ei );
			}
		
		if(iHdArray.Append( hd ) != KErrNone )
		    {
            delete hd;
            hd = NULL;
		    }
			
		aArray.Remove( i );
		}
	
    if( aArray.Count() == 0 )
        {
        aArray.Compress();
        }
	}
	
void CMMCMountTaskAO::RemoveNotPresentFromMDE()
	{
	WRITELOG( "CMMCMountTaskAO::RemoveNotPresentFromMDE" );
	
	iMdeSession->RemoveFilesNotPresent( iMountData->iMediaID );
	}
	
void CMMCMountTaskAO::StartNotifyL()
	{
	WRITELOG( "CMMCMountTaskAO::StartNotify" );

	CHarvesterCenRepUtil* cenRepoUtil = CHarvesterCenRepUtil::NewLC();
	cenRepoUtil->AddIgnorePathsToFspL( iMountData->iDrivePath );
	cenRepoUtil->FspEngine().AddNotificationPath( iMountData->iDrivePath );
	CleanupStack::PopAndDestroy( cenRepoUtil );
	}
	
void CMMCMountTaskAO::StopNotifyL()
	{
	WRITELOG( "CMMCMountTaskAO::StopNotify" );
	
	CHarvesterCenRepUtil* cenRepoUtil = CHarvesterCenRepUtil::NewLC();
	cenRepoUtil->RemoveIgnorePathsFromFspL( iMountData->iDrivePath );
	cenRepoUtil->FspEngine().RemoveNotificationPath( iMountData->iDrivePath );
	CleanupStack::PopAndDestroy( cenRepoUtil );
	}
	
void CMMCMountTaskAO::Initialize()
	{
	WRITELOG( "CMMCMountTaskAO::Initialize" );
    iEntryArray.ResetAndDestroy();
    iHarvestEntryArray.ResetAndDestroy();
	}
	
void CMMCMountTaskAO::Deinitialize()
	{
	WRITELOG( "CMMCMountTaskAO::Deinitialize" );
    
    WRITELOG1( "CMMCMountTaskAO::Deinitialize - iEntryArray.Count() = %d", iEntryArray.Count() );
    if( iEntryArray.Count() > 0)
        {
        TRAP_IGNORE( iHEM->DecreaseItemCountL( EHEObserverTypeMMC, iEntryArray.Count() ) );
        }
    
    WRITELOG1( "CMMCMountTaskAO::Deinitialize - iHarvestEntryArray.Count() = %d", iHarvestEntryArray.Count() );
    if( iHarvestEntryArray.Count() > 0)
        {
        TRAP_IGNORE( iHEM->DecreaseItemCountL( EHEObserverTypeMMC, iHarvestEntryArray.Count() ) );
        }
	
	iEntryArray.ResetAndDestroy();
	iHarvestEntryArray.ResetAndDestroy();

	if ( iMountData )
		{
		delete iMountData;
		iMountData = NULL;
		}
	}

TUint32 CMMCMountTaskAO::GetInternalDriveMediaId( TBool& aPresent )
	{
    WRITELOG( "CMMCMountTaskAO::GetInternalDriveMediaId" );
	    
	TDriveInfo driveInfo;
	TDriveList driveList;
    TInt numOfElements( 0 );

    const TInt err = DriveInfo::GetUserVisibleDrives( 
    		iFs, driveList, numOfElements, 
    		KDriveAttExclude | KDriveAttRemote | KDriveAttRom );
    if( err != KErrNone )
    	{
    	return 0;
    	}
 
#ifdef __WINSCW__
    TFileName systemPath = PathInfo::GetPath( PathInfo::EPhoneMemoryRootPath );
    TInt systemDriveNum( -1 );
    iFs.CharToDrive( systemPath[0], systemDriveNum );
#endif
    
	TUint32 hdMediaId = 0;
	TInt i( 0 );

	const TInt acount = driveList.Length();
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
	    	iFs.Drive( driveInfo, i );
	        if ( driveInfo.iType == EMediaHardDisk )
	        	{
	        	// check if disk is internal
	        	TUint driveStatus;
	        	const TInt err = DriveInfo::GetDriveStatus( iFs, i, driveStatus );
	        	if ( ( err == KErrNone ) && 
	        	     ( driveStatus & DriveInfo::EDriveInternal ))
	        		{
	        		// get media id
	        		hdMediaId = FSUtil::MediaID( iFs, i );
	        		if( driveStatus & DriveInfo::EDrivePresent )
	        		    {
	        		    aPresent = ETrue;
	        		    }
	        		break;
	        		}
	        	}
	    	}
		}

	return hdMediaId;
	}

void CMMCMountTaskAO::SetCachingStatus( TBool aCachingStatus )
	{
	iCacheEvents = aCachingStatus;
	if( !iCacheEvents )
		{
		SetNextRequest( ERequestStartTask );
		}
	}
