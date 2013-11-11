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
* Description:  Low memory observer for havester server
*
*/


#include "harvesteroomao.h"
#include "harvestercommon.h"

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CHarvesterOomAO* CHarvesterOomAO::NewL( MHarvesterOomObserver& aObserver )
	{
	CHarvesterOomAO* self = new (ELeave) CHarvesterOomAO( aObserver );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// CHarvesterOomAO
// ---------------------------------------------------------------------------
//
CHarvesterOomAO::CHarvesterOomAO( MHarvesterOomObserver& aObserver ) : 
		CActive( CActive::EPriorityUserInput )
	{
	iObserver = &aObserver;
	}


// ---------------------------------------------------------------------------
// ~CHarvesterOomAO
// ---------------------------------------------------------------------------
//
CHarvesterOomAO::~CHarvesterOomAO()
	{
    Cancel();
	iOomMsgQueue.Close();
	}

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CHarvesterOomAO::ConstructL()
	{
	CActiveScheduler::Add( this);
	
	iOomMsgQueue.OpenGlobal( KHarvesterOomQueue );
	iOomMsgQueue.NotifyDataAvailable( iStatus );
	SetActive();
	}

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void CHarvesterOomAO::RunL()
	{
	User::LeaveIfError( iStatus.Int() );
	
	TInt oomStatus( 0 );
	const TInt err = iOomMsgQueue.Receive( oomStatus );
	
	if( oomStatus == EMdsOomFreeRam )
		{
		iObserver->MemoryLow();
		}
	else
		{
		iObserver->MemoryGood();
		}
	
	iOomMsgQueue.NotifyDataAvailable( iStatus );
	SetActive();
	}

// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CHarvesterOomAO::DoCancel()
	{
	iOomMsgQueue.CancelDataAvailable();
	}

// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//
TInt CHarvesterOomAO::RunError( TInt /*aError*/ )
	{
	return KErrNone;
	}

