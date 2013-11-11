/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
#include <f32file.h>
#include "movetimer.h"
#include "mdsutils.h"
#include "harvestercommon.h"

#include <harvesterdata.h>

const TInt KTimeout = 1000000;

CMoveTimer::CMoveTimer( MMoveTimerObserver* aObserver ) : 
		CActive( KHarvesterPriorityMonitorPlugin )
	{
	iObserver = aObserver;
	}

CMoveTimer* CMoveTimer::NewLC( MMoveTimerObserver* aObserver )
	{
	CMoveTimer* self = new ( ELeave ) CMoveTimer( aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

CMoveTimer* CMoveTimer::NewL( MMoveTimerObserver* aObserver )
	{
	CMoveTimer* self = CMoveTimer::NewLC( aObserver );
	CleanupStack::Pop( self ); 
	return self;
	}

void CMoveTimer::AddHarvesterDataL( CHarvesterData* aHD )
	{
	iHDArray.AppendL( aHD );
	StartL();
	}

CHarvesterData* CMoveTimer::CheckUriL( const TDesC& aUri )
	{
	TPtrC nameExt;
	
	if( MdsUtils::GetNameExt( aUri, nameExt ) )
		{		
		// check if harvester queue contains harvesting request 
		// with same file name and extension 
		for (TInt i = iHDArray.Count(); --i >= 0; )
			{
			CHarvesterData* hd = iHDArray[i];
			
			TPtrC tempNameExt;
			
			if ( MdsUtils::GetNameExt( hd->Uri(), tempNameExt ) && 
				 MdsUtils::Compare( nameExt, tempNameExt ) == 0 )
				{
				iHDArray.Remove( i );
				
				if( iHDArray.Count() == 0 )
					{
					iHDArray.Compress();
					}
				
				return hd;
				}
			}
		}
	
	return NULL;
	}

void CMoveTimer::ConstructL()
	{
	User::LeaveIfError( iTimer.CreateLocal() );	// Initialize timer
	CActiveScheduler::Add( this );				// Add to scheduler
	}

CMoveTimer::~CMoveTimer()
	{
	Cancel(); // Cancel any request, if outstanding
	iTimer.Close(); // Destroy the RTimer object
	// Delete instance variables if any
	iHDArray.ResetAndDestroy();
	iHDArray.Close();
	}

void CMoveTimer::DoCancel()
	{
	iTimer.Cancel();
	}

void CMoveTimer::StartL()
	{
	Cancel();							// Cancel any request, just to be sure
	iTimer.After( iStatus, KTimeout );	// Set for later
	SetActive();						// Tell scheduler a request is active
	}

void CMoveTimer::RunL()
	{
	User::LeaveIfError( iStatus.Int() );
	if( iObserver && iHDArray.Count() > 0)
		{
		iObserver->NotMoveEvent( iHDArray );
		}
	iHDArray.Reset();
	iHDArray.Compress();
	}

TInt CMoveTimer::RunError( TInt /*aError*/ )
	{
	if( iObserver && iHDArray.Count() > 0)
		{
		iObserver->NotMoveEvent( iHDArray );
		}
	iHDArray.Reset();
	return KErrNone;
	}
