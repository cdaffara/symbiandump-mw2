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
* Description:  Implementations of methods of CMdSGarbageCollector class*
*/

#include "mdsgarbagecollector.h"

#include <e32svr.h> // RDebug

CMdSGarbageCollector::CMdSGarbageCollector(MMdSGarbageCollectorObserver& aObserver)
	: CActive( CActive::EPriorityLow ), iDelay(0), iNewDelay(0), iObserver( aObserver )
	{
	CActiveScheduler::Add( this );
	}

CMdSGarbageCollector::~CMdSGarbageCollector()
	{
	Cancel();
	iTimer.Close();
	}
	
void CMdSGarbageCollector::ConstructL()
	{
	User::LeaveIfError( iTimer.CreateLocal() );
	}

CMdSGarbageCollector* CMdSGarbageCollector::NewL(MMdSGarbageCollectorObserver& aObserver)
	{
    CMdSGarbageCollector* self = CMdSGarbageCollector::NewLC(aObserver);
    CleanupStack::Pop( self );
    return self;
	}

CMdSGarbageCollector* CMdSGarbageCollector::NewLC(MMdSGarbageCollectorObserver& aObserver)
	{
	CMdSGarbageCollector* self = new ( ELeave ) CMdSGarbageCollector(aObserver);
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CMdSGarbageCollector::Start( TInt aDelay )
	{
	#ifdef _DEBUG
	RDebug::Print( _L("CMdSGarbageCollector::Start - delay: %d"), aDelay );
	#endif
	if ( aDelay < 0 )
		{
		return;
		}

	const TInt KSecondsToMicroseconds = 1000000;
	const TInt KMaxInterval = KMaxTInt / KSecondsToMicroseconds;

	if( IsActive() )
		{
		if ( iNewDelay.Int() == 0 )
			{
			// check if interval is less or equal to 2147 seconds (35 minutes and 47 seconds)
			if( aDelay <= KMaxInterval )	
				iNewDelay = aDelay * KSecondsToMicroseconds;
			else
				iNewDelay = KMaxTInt;
			}
		return;
		}

	#ifdef _DEBUG
	RDebug::Print( _L("CMdSGarbageCollector::Start(%d)"), aDelay ); // test
	#endif

	// check if interval is less or equal to 2147 seconds (35 minutes and 47 seconds)
	if( aDelay <= KMaxInterval )	
		iDelay = aDelay * KSecondsToMicroseconds;
	else
		iDelay = KMaxTInt;

	iTimer.After( iStatus, iDelay ); // start timer

	SetActive();
	}

void CMdSGarbageCollector::RunL()
	{
	if( iStatus == KErrNone )
		{
		const TBool startAgain = iObserver.StartGarbageCollectionL();

		if ( startAgain )
			{
		    if( iNewDelay.Int() > 0 )
		        {
                iDelay = iNewDelay;
                iNewDelay = 0;
		        }
            iTimer.After( iStatus, iDelay ); // start timer
			SetActive();
			}
		}
	}

void CMdSGarbageCollector::DoCancel()
	{
	#ifdef _DEBUG
	RDebug::Print( _L("CMdSGarbageCollector::DoCancel") );
	#endif
	iTimer.Cancel();
	}

#ifdef _DEBUG
TInt CMdSGarbageCollector::RunError( TInt aError )
	{
#else
TInt CMdSGarbageCollector::RunError( TInt /*aError*/ )
	{
#endif
	#ifdef _DEBUG
	RDebug::Print( _L("CMdSGarbageCollector::RunError %d"), aError );
	#endif
	return KErrNone;
	}

void CMdSGarbageCollector::Pause()
	{
	#ifdef _DEBUG
	RDebug::Print( _L("CMdSGarbageCollector::Pause") );
	#endif
	DoCancel();
	}

void CMdSGarbageCollector::Resume()
	{
	#ifdef _DEBUG
	RDebug::Print( _L("CMdSGarbageCollector::Resume") );
	#endif
	
	if( !IsActive() )
	    {
	    if ( iDelay.Int() > 0 )
	         {
	         iTimer.After( iStatus, iDelay ); // start timer
	         SetActive();
	         }
	    }
	}
