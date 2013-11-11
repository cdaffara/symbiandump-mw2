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
* Description:  This is disk space notifier for Metadata server.*
*/

// INCLUDE FILES
#include "mdsdiskspacenotifier.h"


CMdSDiskSpaceNotifierAO* CMdSDiskSpaceNotifierAO::NewL(
	MMdSDiskSpaceNotifierObserver& aObserver, TInt64 aThreshold, const TDesC& aFilename)
	{
	CMdSDiskSpaceNotifierAO* self = 
		CMdSDiskSpaceNotifierAO::NewLC( aObserver, aThreshold, aFilename );
	CleanupStack::Pop( self );
	return self;
	}

CMdSDiskSpaceNotifierAO* CMdSDiskSpaceNotifierAO::NewLC(
	MMdSDiskSpaceNotifierObserver& aObserver, TInt64 aThreshold, const TDesC& aFilename)
	{
	TDriveNumber driveNumber = GetDriveNumberL( aFilename );

    CMdSDiskSpaceNotifierAO* self = 
    	new ( ELeave ) CMdSDiskSpaceNotifierAO( aObserver, aThreshold, driveNumber );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
	}

CMdSDiskSpaceNotifierAO::~CMdSDiskSpaceNotifierAO()
	{
	Cancel();

	iFileServerSession.Close();
	}

void CMdSDiskSpaceNotifierAO::RunL()
	{	
	TVolumeInfo volumeInfo;

	if ( iState == CMdSDiskSpaceNotifierAO::ENormal )
		{
		TInt status = iStatus.Int();
		
		switch( status )
			{
			case KErrNone:
			    {
				const TInt error = iFileServerSession.Volume( volumeInfo, iDrive );
				if( error != KErrNone )
				    {
				    StartNotifier();
				    break;
				    }
				
				// Check if free space is less than threshold level
				if( volumeInfo.iFree < iThreshold )
					{
					iObserver.HandleDiskSpaceNotificationL( MMdSDiskSpaceNotifierObserver::ELess );
					iDiskFull = ETrue;
					iState = EIterate;
					iIterationCount = 0;
					SetActive();
					TRequestStatus* status = &iStatus;
					User::RequestComplete( status, KErrNone );
					return;
					}
				else
					{
					iObserver.HandleDiskSpaceNotificationL( MMdSDiskSpaceNotifierObserver::EMore );
					iDiskFull = EFalse;
					}
				StartNotifier();
				break;
			    }

			case KErrArgument:
				User::Leave( status );
				break;
			default:
				break;
			}
		}
	else if ( iState == CMdSDiskSpaceNotifierAO::EIterate )
		{
		const TInt KMaxIterations = 10;
        TInt error = iFileServerSession.Volume( volumeInfo, iDrive );
        if( error != KErrNone )
            {
            iState = ENormal;
            iIterationCount = 0;
            StartNotifier();
            return;
            }

		if ( volumeInfo.iFree < iThreshold )
			{
			iObserver.HandleDiskSpaceNotificationL( MMdSDiskSpaceNotifierObserver::ELess );
			++iIterationCount;
			if ( iIterationCount < KMaxIterations )
				{
				SetActive();
				TRequestStatus* status = &iStatus;
				User::RequestComplete( status, KErrNone );
				return;
				}
			else
				{
		        error = iFileServerSession.Volume( volumeInfo, iDrive );
		        if( error != KErrNone )
		            {
		            iState = ENormal;
		            iIterationCount = 0;
		            StartNotifier();
		            return;
		            }
				if ( volumeInfo.iFree >= iThreshold )
					{
					iDiskFull = EFalse;
					}
				}
			}
		else
			{
			iDiskFull = EFalse;
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

TInt CMdSDiskSpaceNotifierAO::RunError(TInt aError)
	{
	iObserver.HandleDiskSpaceError( aError );
	
	return KErrNone;
	}

void CMdSDiskSpaceNotifierAO::DoCancel()
	{
	if( IsActive() )
		{	
		iFileServerSession.NotifyDiskSpaceCancel();
		}
	}

CMdSDiskSpaceNotifierAO::CMdSDiskSpaceNotifierAO(
	MMdSDiskSpaceNotifierObserver& aObserver, TInt64 aThreshold, TDriveNumber aDrive)
	: CActive( CActive::EPriorityStandard ), 
	iObserver( aObserver ), iThreshold( aThreshold ), iDrive( aDrive ), iState( CMdSDiskSpaceNotifierAO::ENormal ), iDiskFull( EFalse )
	{
	CActiveScheduler::Add( this );
	}

void CMdSDiskSpaceNotifierAO::ConstructL()
	{	
	TInt KMessageSlotCount = 2; // slots for NotifyDiskSpace and NotifyDiskSpaceCancel

	User::LeaveIfError( iFileServerSession.Connect( KMessageSlotCount ) );
	
	TVolumeInfo volumeInfo;
	const TInt error = iFileServerSession.Volume( volumeInfo, iDrive );	
	if ( (error != KErrNone) || volumeInfo.iFree < iThreshold )
		{
		iDiskFull = ETrue;
		}

	StartNotifier();
	}

void CMdSDiskSpaceNotifierAO::StartNotifier()
	{	
	iFileServerSession.NotifyDiskSpace( iThreshold, iDrive, iStatus );
	
	SetActive();
	}

TDriveNumber CMdSDiskSpaceNotifierAO::GetDriveNumberL( const TDesC& aFilename )
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

TBool CMdSDiskSpaceNotifierAO::DiskFull() const
	{
	return iDiskFull;
	}
