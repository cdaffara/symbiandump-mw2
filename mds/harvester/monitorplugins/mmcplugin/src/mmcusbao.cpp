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
* Description:  Monitors Usb insertions and removals.*
*/


#include <usbmsshared.h>

#include "harvesterlog.h"
#include "harvestercommon.h"
#include "mmcusbao.h"
#include "mmcmonitorplugin.h"

CMMCUsbAO* CMMCUsbAO::NewL()
    {
    WRITELOG( "CMMCUsbAO::NewL" );
    
    CMMCUsbAO* self = new (ELeave) CMMCUsbAO();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CMMCUsbAO::CMMCUsbAO() : CActive( KHarvesterPriorityMonitorPlugin ), iObserver( NULL )
    {
    iDrive = 0;
    iMassStorageMode = EFalse;
    }

void CMMCUsbAO::ConstructL()
    {
    WRITELOG( "CMMCUsbAO::ConstructL" );
    
    CActiveScheduler::Add( this );
    
    User::LeaveIfError( iFs.Connect() );

    User::LeaveIfError( iDriveState.Attach( 
    		KUsbMsDriveState_Category, EUsbMsDriveState_DriveStatus ) );
    }

CMMCUsbAO::~CMMCUsbAO()
    {
    WRITELOG( "CMMCUsbAO::~CMMCUsbAO" );
    StopMonitoring();
    }

TBool CMMCUsbAO::StartMonitoring( MMMCMonitorObserver& aObserver )
    {
    WRITELOG( "CMMCUsbAO::StartMonitoring" );
    
    iObserver = &aObserver;
    
    return Resume();
    }

void CMMCUsbAO::StartNotify()
    {
    WRITELOG( "CMMCUsbAO::StartNotify" );
    iDriveState.Subscribe( iStatus );
    SetActive();
    }

TBool CMMCUsbAO::StopMonitoring()
    {
    WRITELOG( "CMMCUsbAO::StopNotify" );
    
    return Pause();
    }

TBool CMMCUsbAO::Resume()
    {
    WRITELOG( "CMMCUsbAO::Resume" );
        
    StartNotify();
    
    return ETrue;
    }

TBool CMMCUsbAO::Pause()
    {
    WRITELOG( "CMMCUsbAO::Pause" );
    
    Cancel();
    iDriveState.Close();
    iFs.Close();
    
    return ETrue;
    }

void CMMCUsbAO::RunL()
    {
    WRITELOG1( "CMMCUsbAO::RunL iStatus: %d", iStatus.Int() );
    
    // check if change to pc-suite mode
    if ( iStatus.Int() == KErrNotFound )
        {
        TChar chr;
        RFs::DriveToChar( iDrive, chr );
        iObserver->MountEvent( chr, MediaID(iDrive), EMounted );
        iMassStorageMode = EFalse;
        StartNotify();
        return;
        }
    else
        {
        User::LeaveIfError( iStatus.Int() );
        }
        
    TUsbMsDrivesStatus allDrivesStatus;
    const TInt err = iDriveState.Get( allDrivesStatus );
    
    if ( err == KErrNone )
        {
        WRITELOG1( "CMMCUsbAO::RunL - allDrivesStatus.Length(): %d", allDrivesStatus.Length() );
        const TInt count = allDrivesStatus.Length()/2;
        for ( TInt i = 0; i < count; i++ )
            {
            TInt driveNumber = allDrivesStatus[2*i];
            TInt driveStatus = allDrivesStatus[2*i+1];
            TChar driveChar;
            RFs::DriveToChar( driveNumber, driveChar );
#ifdef _DEBUG
            TFileName* fn = new (ELeave) TFileName( driveChar );
            _LIT( KIndicator, ":" );
            fn->Append( KIndicator );
            WRITELOG1( "CMMCUsbAO::RunL - drive letter: %S", &(*fn) );
            PrintDriveStatus( driveStatus );
            delete fn;
            fn = NULL;
#endif
            switch( driveStatus )
                {
                case EUsbMsDriveState_Connected:
                    {
                    if ( iMassStorageMode )
                        {
                        break;
                        }
                    iObserver->MountEvent( driveChar, MediaID(driveNumber), EDismounted );
                    iMassStorageMode = ETrue;
                    iDrive = driveNumber;
                    }
                break;
                
                case EUsbMsDriveState_Removed:
                    {
                    iObserver->MountEvent( driveChar, MediaID(driveNumber), EMounted );
                    iMassStorageMode = EFalse;
                    }
                break;
                
                default:
                	break;
                }
            }
        }
    else
        {
        WRITELOG1( "CMMCUsbAO::RunL - drive state error %d", err );
        }
    
    StartNotify();
    }

#ifdef _DEBUG
TInt CMMCUsbAO::RunError( TInt aError )
#else
TInt CMMCUsbAO::RunError( TInt )
#endif
    {
    WRITELOG1( "CMMCUsbAO::RunError with error code: %d", aError );
    
    StartNotify();
    return KErrNone;
    }

void CMMCUsbAO::DoCancel()
    {
    WRITELOG( "CMMCUsbAO::DoCancel" );
    
    iDriveState.Cancel();
    }

TUint32 CMMCUsbAO::MediaID( TInt aDrive )
    {
    TUint32 uid = 0;
    TVolumeInfo* vi = NULL;
    vi = new TVolumeInfo;
    if ( !vi )
        {
        return uid;
        }
    
    const TInt err = iFs.Volume( *vi, aDrive );
    if ( err == KErrNone )
        {
        uid = vi->iUniqueID;
        }

    delete vi;
    vi = NULL;
    
    return uid;
    }

#ifdef _DEBUG
void CMMCUsbAO::PrintDriveStatus( TInt aStatus )
    {
    switch( aStatus )
        {
        case EUsbMsDriveState_Disconnected:
        WRITELOG( "CMMCUsbAO::RunL - drive status: EUsbMsDriveState_Disconnected" );
        break;
        
        case EUsbMsDriveState_Connecting:
        WRITELOG( "CMMCUsbAO::RunL - drive status: EUsbMsDriveState_Connecting" );
        break;
        
        case EUsbMsDriveState_Connected:
        WRITELOG( "CMMCUsbAO::RunL - drive status: EUsbMsDriveState_Connected" );
        break;
        
        case EUsbMsDriveState_Disconnecting:
        WRITELOG( "CMMCUsbAO::RunL - drive status: EUsbMsDriveState_Disconnecting" );
        break;
        
        case EUsbMsDriveState_Active:
        WRITELOG( "CMMCUsbAO::RunL - drive status: EUsbMsDriveState_Active" );
        break;
        
        case EUsbMsDriveState_Locked:
        WRITELOG( "CMMCUsbAO::RunL - drive status: EUsbMsDriveState_Locked" );
        break;
        
        case EUsbMsDriveState_MediaNotPresent:
        WRITELOG( "CMMCUsbAO::RunL - drive status: EUsbMsDriveState_MediaNotPresent" );
        break;
        
        case EUsbMsDriveState_Removed:
        WRITELOG( "CMMCUsbAO::RunL - drive status: EUsbMsDriveState_Removed" );
        break;
        
        case EUsbMsDriveState_Error:
        WRITELOG( "CMMCUsbAO::RunL - drive status: EUsbMsDriveState_Error" );
        break;
        
        default:
        WRITELOG( "CMMCUsbAO::RunL - drive status: Unknown" );
        break;
        }
    }

#endif
