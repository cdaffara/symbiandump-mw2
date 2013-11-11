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
* Description:  Scans MMC after phone reboot for file changes*
*/

#include "mmcscannerao.h"
#include "harvesterlog.h"
#include "fsutil.h"
#include <placeholderdata.h>
#include <centralrepository.h>

_LIT( KColon, ":" );

const TInt KDefaultDelay = 3;
const TInt KMillion = 1000000;

const TUid KRepositoryUid = { 0x20007183 };
const TUint32 KScanDelayKey = 0x00000001;

CMmcScannerAO::CMmcScannerAO( TUint32 aMediaId, 
		CMdEHarvesterSession* aMdEClient, MMonitorPluginObserver* aObserver, 
		CHarvesterPluginFactory* aHarvesterPluginFactory, const TInt aPriority ) : 
		CActive( aPriority ), iState( EUninitialized ), iMmcFileList( NULL )   
	{
	iMediaId = aMediaId;
	iMdEClient = aMdEClient;
	iObserver = aObserver;
	iHarvesterPluginFactory = aHarvesterPluginFactory;
	}

CMmcScannerAO* CMmcScannerAO::NewL( TUint32 aMediaId, CMdEHarvesterSession* aMdEClient,
		MMonitorPluginObserver* aObserver, CHarvesterPluginFactory* aHarvesterPluginFactory, 
		const TInt aPriority )
	{
	CMmcScannerAO* self = new ( ELeave ) CMmcScannerAO( aMediaId, aMdEClient, aObserver, 
			aHarvesterPluginFactory, aPriority );
	
	CleanupStack::PushL( self );
	self->ConstructL( );
	CleanupStack::Pop( self );
	return self;
	}

void CMmcScannerAO::ConstructL()
	{
    CActiveScheduler::Add( this ); // Add to scheduler
	iTimer.CreateLocal();
	iState = EUninitialized;
	User::LeaveIfError( iFs.Connect() );
	iMmcFileList = CMmcFileList::NewL();
	
    TInt tmpDelay( KDefaultDelay );
    TTimeIntervalMicroSeconds32 delay( tmpDelay * KMillion ); 
    CRepository* repo = CRepository::NewLC( KRepositoryUid );
    const TInt err = repo->Get( KScanDelayKey, tmpDelay );
    if ( err == KErrNone )
        {
        delay = tmpDelay * KMillion;
        }
    CleanupStack::PopAndDestroy( repo );
	
    iHEM = CHarvesterEventManager::GetInstanceL();
    
    iTimer.After( iStatus, delay );
    SetActive();
	}

CMmcScannerAO::~CMmcScannerAO()
	{
	Cancel(); // Cancel any request, if outstanding
	// Delete instance variables if any
	
	delete iMmcFileList;
	iMmcFileList = NULL;
	
	iEntryArray.ResetAndDestroy();
	iEntryArray.Close();

	iHarvestEntryArray.ResetAndDestroy();
	iHarvestEntryArray.Close();
	
    iHdArray.ResetAndDestroy();
	iHdArray.Close();

    if (iHEM)
        {
        iHEM->ReleaseInstance();
        }
	
	iFs.Close();
	}


void CMmcScannerAO::RunL()
	{
	switch( iState )
		{
		case( EUninitialized ):
			{
		    WRITELOG("CMmcScannerAO::RunL - Starting processing");
		    if( iMediaId == 0 )
		        {
		        WRITELOG("CMmcScannerAO::RunL - MediaId == 0 -> end");
                SetState( EDone );
                break;		    
		        }
		    
			WRITELOG("CMmcScannerAO::RunL - Setting files to not present");
			iMdEClient->SetFilesToNotPresent( iMediaId, ETrue );
			SetState( EReadFiles );
			break;
			}
		
		case( EReadFiles ):
			{
			for ( TInt i=0; i < KMaxDrives; i++ )
				{
				const TUint32 mediaId = FSUtil::MediaID(iFs, i);
				if( mediaId == iMediaId )
					{
					TChar chr;
					iFs.DriveToChar( i, chr );
					i = KMaxDrives;
					iDrive.Zero();
					iDrive.Append( chr );
					iDrive.Append( KColon );
					}
				}
			// drive not found (unmount before scanning delay)
			if ( iDrive.Length() == 0 )
				{
				SetState( EDone );
				break;
				}
			
			WRITELOG("CMmcScannerAO::RunL - build file list");
			iMmcFileList->BuildFileListL( iFs, iDrive, iEntryArray );
			
            // send start event
            const TInt entryCount = iEntryArray.Count();
            if( entryCount > 0 )
                {
                iHEM->IncreaseItemCount( EHEObserverTypeMMC, entryCount );
                iHEM->SendEventL( EHEObserverTypeMMC, EHEStateStarted, iHEM->ItemCount( EHEObserverTypeMMC ) );
                }
			
			SetState( EProcessFiles );
			break;
			}
		
		case( EProcessFiles ):
			{
			if( iEntryArray.Count() > 0 )
				{
				WRITELOG("CMmcScannerAO::RunL - handling file list");
				iMmcFileList->HandleFileEntryL( *iMdEClient, iEntryArray, 
						iHarvestEntryArray, iMediaId, iHarvesterPluginFactory );
				SetState( EProcessFiles );
                }
            else 
                {
                SetState( EHarvestFiles );
                }
			break;
			}
		
		case( EHarvestFiles ):
			{
			if ( iHarvestEntryArray.Count() > 0 )
				{
				WRITELOG("CMmcScannerAO::RunL - adding new files to harvester queue");
				HandleReharvestL();
				SetState( EHarvestFiles );
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
				SetState( ERemoveNPFiles );
				}
			break;
			}
		
		case( ERemoveNPFiles ):
			{
			WRITELOG("CMmcScannerAO::RunL - Removing not present files");
			iMdEClient->RemoveFilesNotPresent( iMediaId, ETrue );
			SetState( EDone );
			break;
			}
		
		case( EDone ):
			{
			iFs.Close();
			iTimer.Close();
		    iHdArray.Reset();
		    iHdArray.Compress();
			iEntryArray.Compress();
			iHarvestEntryArray.Compress();
		    if (iHEM)
		        {
		        iHEM->ReleaseInstance();
		        iHEM = NULL;
		        }
			break;
			}
		
		default: 
			break;
		
		}
	}

void CMmcScannerAO::HandleReharvestL()
	{
	WRITELOG("CMMCMountTaskAO::HandleReharvestL");

    const TInt count( iHarvestEntryArray.Count() );
    TInt batchSize( KMmcEntryBufferSize );
    if ( count < KMmcEntryBufferSize )
        {
        batchSize = count;
        }

    const TInt endIndex( count - batchSize );
        
    for ( TInt i = count; --i >= endIndex; )
        {
        CPlaceholderData* ei = iHarvestEntryArray[i];

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
        iHarvestEntryArray.Remove( i );
        }

    if( iHarvestEntryArray.Count() == 0 )
    	{
    	iHarvestEntryArray.Compress();
    	}
	}
	

TInt CMmcScannerAO::RunError( TInt /*aError*/ )
	{
	return KErrNone;
	}

void CMmcScannerAO::SetState( TCMmcScannerAOState aState )
	{
	WRITELOG("CMmcScannerAO::SetNextRequest" );
	if ( !IsActive() )
		{
		iState = aState;
		TRequestStatus* ptrStatus = &iStatus;
		User::RequestComplete( ptrStatus, KErrNone );
		SetActive();
		}
	}

void CMmcScannerAO::DoCancel()
    {
    iTimer.Cancel();
    iTimer.Close();
    }

