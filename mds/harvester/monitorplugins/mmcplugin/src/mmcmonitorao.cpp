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
* Description:
*
*/

#include <driveinfo.h>

#ifdef __WINSCW__
#include <pathinfo.h>
#endif

#include "harvesterlog.h"
#include "harvestercommon.h"
#include "mmcmonitorao.h"
#include "mmcmonitorplugin.h"
#include "fsutil.h"

CMMCMonitorAO* CMMCMonitorAO::NewL()
	{
	WRITELOG( "CMMCMonitorAO::NewL" ); // DEBUG INFO
	
	CMMCMonitorAO* self = new (ELeave) CMMCMonitorAO();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

void CMMCMonitorAO::ConstructL()
	{
	WRITELOG( "CMMCMonitorAO::ConstructL" ); // DEBUG INFO
	
	User::LeaveIfError( iFs.Connect() );
	CActiveScheduler::Add( this );
	for( TInt i = KMaxDrives - 1; i >=0; i-- )
	    {
	    iMediaIdList[i] = 0;
	    }
	}

CMMCMonitorAO::~CMMCMonitorAO()
	{
	WRITELOG( "CMMCMonitorAO::~CMMCMonitorAO" ); // DEBUG INFO
	
	StopMonitoring();
    iFs.Close();
	}

TBool CMMCMonitorAO::StartMonitoring( MMMCMonitorObserver& aObserver, RArray<TMdEMediaInfo>& aMedias )
	{
	WRITELOG( "CMMCMonitorAO::StartMonitoring" ); // DEBUG INFO

	iObserver = &aObserver;
	
    BuildDriveList();
    
    const TInt count( aMedias.Count() );
    for ( TInt i = 0; i < count; i++ )
    	{
    	TInt drive(0);
    	const TInt err = iFs.CharToDrive( aMedias[i].iDrive, drive );
    	if ( err == KErrNone )
    		{
    		WRITELOG2( "CMMCMonitorAO::StartMonitoring - set drive %d mediaid to %d ", drive, aMedias[i].iMediaId );
    		iDriveList[drive] = 1; // fake value for startup (not KDriveAbsent)
        	iMediaIdList[drive] = aMedias[i].iMediaId;
    		}
    	}
    
	iPreviousDriveList.Zero();
	iPreviousDriveList.Copy( iDriveList );

#ifdef _DEBUG
	PrintAllLists();
#endif

	CompareDriveLists();

	StartNotify();
	
	return ETrue;
	}

void CMMCMonitorAO::StartNotify()
	{
	WRITELOG( "CMMCMonitorAO::StartNotify with ENotifyEntry" ); // DEBUG INFO
	
	if( !IsActive() )
		{
		iFs.NotifyChange( ENotifyEntry, iStatus ); 
		SetActive();                               
		}
	}

TBool CMMCMonitorAO::StopMonitoring()
	{
	WRITELOG( "CMMCMonitorAO::StopNotify" ); // DEBUG INFO
	
	Cancel();
	return ETrue;
	}

void CMMCMonitorAO::RunL()
	{
#ifdef _DEBUG
	WRITELOG1( "CMMCMonitorAO::RunL iStatus: %d", iStatus.Int() ); // DEBUG INFO
#endif

	if ( iStatus.Int() != KErrNone )
		{
		StartNotify();
		return;
		}

	StartNotify();
	
	iPreviousDriveList.Zero();
	iPreviousDriveList.Copy( iDriveList );

	BuildDriveList();

#ifdef _DEBUG
	PrintAllLists();
#endif

	CompareDriveLists();

#ifdef _DEBUG
	PrintAllLists();
#endif
	}

#ifdef _DEBUG
TInt CMMCMonitorAO::RunError( TInt aError )
#else
TInt CMMCMonitorAO::RunError( TInt )
#endif
	{
	WRITELOG1( "CMMCMonitorAO::RunError with error code: %d", aError ); // DEBUG INFO
	
	StartNotify();
	return KErrNone;
	}

void CMMCMonitorAO::DoCancel()
	{
	WRITELOG( "CMMCMonitorAO::DoCancel" ); // DEBUG INFO
	
	iFs.NotifyChangeCancel( iStatus );
	}

CMMCMonitorAO::CMMCMonitorAO() : CActive( KHarvesterServerHighPriority ),
    iObserver( NULL )
	{
	}

void CMMCMonitorAO::BuildDriveList()
	{
	WRITELOG( "CMMCMonitorAO::BuildDriveList" ); // DEBUG INFO

	iDriveList.Zero();
    TInt numOfElements( 0 );
    TInt err = DriveInfo::GetUserVisibleDrives( 
    		iFs, iDriveList, numOfElements, 
			KDriveAttExclude | KDriveAttRemote | KDriveAttRom );

#ifdef __WINSCW__
    TFileName systemPath = PathInfo::GetPath( PathInfo::EPhoneMemoryRootPath );
    TInt systemDriveNum( -1 );
    iFs.CharToDrive( systemPath[0], systemDriveNum );
#endif
    
	if ( err != KErrNone )
		{
		WRITELOG1( "CMMCMonitorAO::BuildDriveList with error code: %d", err ); // DEBUG INFO
		
		return;
		}

	TDriveInfo driveInfo;
	for ( TInt i=0; i < KMaxDrives; i++ )
		{
	
#ifdef __WINSCW__
        if ( i == systemDriveNum )
            {
            iDriveList[i] = 0;
            continue;
            }
#endif
	
		if ( iDriveList[i] == KDriveAbsent )
			{
			continue;
			}
		err = iFs.Drive( driveInfo, i );
		if ( err != KErrNone )
			{
			WRITELOG1( "CMMCMonitorAO::BuildDriveList with error code: %d", err ); // DEBUG INFO
			}

		TUint driveStatus;
		err = DriveInfo::GetDriveStatus( iFs, i, driveStatus );
		if ( err != KErrNone )
			{
			WRITELOG1( "CMMCMonitorAO::BuildDriveList with error code2: %d", err ); // DEBUG INFO
			}

#ifdef _DEBUG
		if ( driveStatus & DriveInfo::EDriveInternal )
			{
			WRITELOG1("Drive %d is internal",i);
			}		
		
		WRITELOG2("Drive %d type %d",i,driveInfo.iType);
#endif
		
		if ( !( driveStatus & DriveInfo::EDriveRemovable ) )
			{
			WRITELOG1("Drive %d is not removable",i);
			// check if driver is internal hard disk
			if ( driveInfo.iType != EMediaHardDisk )
				{
				iDriveList[i] = 0;
				continue;
				}
			else
				{
				WRITELOG1("Drive %d is hard disk",i);
				}
			}
		
		if( driveStatus & DriveInfo::EDriveUsbMemory )
		    {
            iDriveList[i] = 0;
            continue;
		    }

		if ( driveInfo.iType == EMediaNotPresent )
			{
			WRITELOG1("Drive %d is not present",i);
			iDriveList[i] = KDriveAbsent;
			continue;
			}

		const TUint32 mediaId = FSUtil::MediaID(iFs, i);
		if ( mediaId == 0 )
			{
			WRITELOG1("Drive %d: MediaId is 0",i);
			iDriveList[i] = KDriveAbsent;
			}
		}
	}

void CMMCMonitorAO::CompareDriveLists()
	{
	WRITELOG( "CMMCMonitorAO::CompareDriveLists" ); // DEBUG INFO

	for ( TInt i = 0; i < KMaxDrives; i++ )
		{
		if ( iDriveList[i] == KDriveAbsent && iPreviousDriveList[i] == KDriveAbsent )
			{	
			continue;
			}

		TChar chr;
		iFs.DriveToChar( i, chr );
		const TUint32 mediaId = FSUtil::MediaID(iFs, i);

		if ( iDriveList[i] > KDriveAbsent && iPreviousDriveList[i] > KDriveAbsent )
			{
			if ( iMediaIdList[i] != mediaId )
				{
				WRITELOG3( "CMMCMonitorAO::CompareDriveLists media changed %d, old=%u, new=%u", i, iMediaIdList[i], mediaId ); // DEBUG INFO
				// skip mount events if mediaId is 0
				if ( iMediaIdList[i] != 0 )
					{
					iObserver->MountEvent( chr, iMediaIdList[i], EDismounted );
					}
				if ( mediaId != 0 )
					{
					iObserver->MountEvent( chr, mediaId, EMounted );
					}
				iMediaIdList[i] = mediaId;

				}
			continue;
			}

		if ( iDriveList[i] == KDriveAbsent )
			{
			WRITELOG1( "CMMCMonitorAO::CompareDriveLists Dismounted %d", i ); // DEBUG INFO
			if ( mediaId )
				{
				iObserver->MountEvent( chr, mediaId, EDismounted );
				}
			else if ( iMediaIdList[i] )
				{
				iObserver->MountEvent( chr, iMediaIdList[i], EDismounted );
				}
			iMediaIdList[i] = 0;
			}

		if ( iPreviousDriveList[i] == KDriveAbsent )
			{
			WRITELOG1( "CMMCMonitorAO::CompareDriveLists Mounted %d", i ); // DEBUG INFO
			iObserver->MountEvent( chr, mediaId, EMounted );
			iMediaIdList[i] = mediaId;
			}
		}
	}
	
#ifdef _DEBUG
void CMMCMonitorAO::PrintAllLists()
	{
    WRITELOG( "CMMCMonitorAO::PrintAllLists()" );

	for ( TInt i=0; i < KMaxDrives; i++ )
		{
		if ( !(iPreviousDriveList[i] == 0 && iDriveList[i] == 0 && iMediaIdList[i] == 0) )
			{
		    TInt32 drive;
		    TUint32 mediaId;
			drive = iPreviousDriveList[i];
		    WRITELOG1( "CMMCMonitorAO::PrintAllLists() - Previous drive element: %d", drive );
			drive = iDriveList[i];
		    WRITELOG1( "CMMCMonitorAO::PrintAllLists() - Current drive element : %d", drive );
		    mediaId = iMediaIdList[i];
		    WRITELOG1( "CMMCMonitorAO::PrintAllLists() - Current MediaId       : %u", mediaId );
			}
		}
	}
#endif
