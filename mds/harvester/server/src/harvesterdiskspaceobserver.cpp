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
* Description:  Low disk space observer for harvester server
*
*/

#include "harvesterdiskspaceobserver.h"
#include "harvestercommon.h"

CMdSHarvesterDiskspaceObserverAO* CMdSHarvesterDiskspaceObserverAO::NewL(
    MMdSHarvesterDiskSpaceObserver& aObserver, TInt64 aThreshold, const TDesC& aFilename)
    {
    CMdSHarvesterDiskspaceObserverAO* self = 
        CMdSHarvesterDiskspaceObserverAO::NewLC( aObserver, aThreshold, aFilename );
    CleanupStack::Pop( self );
    return self;
    }

CMdSHarvesterDiskspaceObserverAO* CMdSHarvesterDiskspaceObserverAO::NewLC(
    MMdSHarvesterDiskSpaceObserver& aObserver, TInt64 aThreshold, const TDesC& aFilename)
    {
    TDriveNumber driveNumber = GetDriveNumberL( aFilename );

    CMdSHarvesterDiskspaceObserverAO* self = 
        new ( ELeave ) CMdSHarvesterDiskspaceObserverAO( aObserver, aThreshold, driveNumber );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CMdSHarvesterDiskspaceObserverAO::~CMdSHarvesterDiskspaceObserverAO()
    {
    Cancel();

    iFileServerSession.Close();
    }

void CMdSHarvesterDiskspaceObserverAO::RunL()
    {   
    TVolumeInfo volumeInfo;

    if ( iState == CMdSHarvesterDiskspaceObserverAO::ENormal )
        {
        TInt status = iStatus.Int();
        
        switch( status )
            {
            case KErrNone:
                {
                const TInt error = iFileServerSession.Volume( volumeInfo, iDrive );
                if( error != KErrNone )
                    {
                    // Continue harvester if some error in accessing volume occures so that harvester does not remain paused forever 
                    iObserver.HandleDiskSpaceNotificationL( MMdSHarvesterDiskSpaceObserver::EMore );
                    StartNotifier();
                    break;
                    }
                
                // Check if free space is less than threshold level
                if( volumeInfo.iFree < iThreshold )
                    {
                    iObserver.HandleDiskSpaceNotificationL( MMdSHarvesterDiskSpaceObserver::ELess );
                    iState = EIterate;
                    iIterationCount = 0;
                    SetActive();
                    TRequestStatus* status = &iStatus;
                    User::RequestComplete( status, KErrNone );
                    return;
                    }
                else
                    {
                    iObserver.HandleDiskSpaceNotificationL( MMdSHarvesterDiskSpaceObserver::EMore );
                    }
                StartNotifier();
                break;
                }

            default:
                StartNotifier();
                break;
            }
        }
    else if ( iState == CMdSHarvesterDiskspaceObserverAO::EIterate )
        {
        const TInt KMaxIterations = 5;
        TInt error = iFileServerSession.Volume( volumeInfo, iDrive );
        if( error != KErrNone )
            {
            // Continue harvester if some error in accessing volume occures so that harvester does not remain paused forever 
            iObserver.HandleDiskSpaceNotificationL( MMdSHarvesterDiskSpaceObserver::EMore );
            iState = ENormal;
            iIterationCount = 0;
            StartNotifier();
            return;
            }

        if ( volumeInfo.iFree < iThreshold )
            {
            ++iIterationCount;
            if ( iIterationCount < KMaxIterations )
                {
                SetActive();
                TRequestStatus* status = &iStatus;
                User::RequestComplete( status, KErrNone );
                return;
                }
            }
        else
            {
            iObserver.HandleDiskSpaceNotificationL( MMdSHarvesterDiskSpaceObserver::EMore );
            }
        iState = ENormal;
        iIterationCount = 0;
        StartNotifier();            
        }
    else
        {
        User::Leave( KErrGeneral );
        }
    }

TInt CMdSHarvesterDiskspaceObserverAO::RunError(TInt /*aError*/)
    {
    StartNotifier();     
    
    return KErrNone;
    }

void CMdSHarvesterDiskspaceObserverAO::DoCancel()
    {
    if( IsActive() )
        {   
        iFileServerSession.NotifyDiskSpaceCancel();
        }
    }

CMdSHarvesterDiskspaceObserverAO::CMdSHarvesterDiskspaceObserverAO(
    MMdSHarvesterDiskSpaceObserver& aObserver, TInt64 aThreshold, TDriveNumber aDrive)
    : CActive( KHarvesterPriorityMonitorPlugin), 
    iObserver( aObserver ), iThreshold( aThreshold ), iDrive( aDrive ), iState( CMdSHarvesterDiskspaceObserverAO::ENormal )
    {
    CActiveScheduler::Add( this );
    }

void CMdSHarvesterDiskspaceObserverAO::ConstructL()
    {   
    TInt KMessageSlotCount = 2; // slots for NotifyDiskSpace and NotifyDiskSpaceCancel

    User::LeaveIfError( iFileServerSession.Connect( KMessageSlotCount ) );

    StartNotifier();
    }

void CMdSHarvesterDiskspaceObserverAO::StartNotifier()
    {   
    iFileServerSession.NotifyDiskSpace( iThreshold, iDrive, iStatus );
    
    SetActive();
    }

TDriveNumber CMdSHarvesterDiskspaceObserverAO::GetDriveNumberL( const TDesC& aFilename )
    {
    TLex driveParser( aFilename );
    
    TChar driveChar = driveParser.Get();

    if( 0 == driveChar || TChar( ':' ) != driveParser.Peek() )
        {
        User::Leave( KErrArgument );
        }
        
    TInt driveNumber;
    
    RFs::CharToDrive( driveChar, driveNumber );
    
    return (TDriveNumber)driveNumber;
    }

