/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*/

#include "unmounthandlerao.h"

CUnmountHandlerAO::CUnmountHandlerAO( MUnmountObserver& aObserver ) : 
		CActive( CActive::EPriorityHigh )
    {
	iUnmountObserver = &aObserver;
    }

CUnmountHandlerAO* CUnmountHandlerAO::NewLC( MUnmountObserver& aObserver )
    {
	CUnmountHandlerAO* self = new ( ELeave ) CUnmountHandlerAO( aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
    }

CUnmountHandlerAO* CUnmountHandlerAO::NewL( MUnmountObserver& aObserver )
    {
	CUnmountHandlerAO* self = CUnmountHandlerAO::NewLC( aObserver );
	CleanupStack::Pop(); // self;
	return self;
    }

void CUnmountHandlerAO::ConstructL()
    {
	CActiveScheduler::Add( this );				// Add to scheduler
	_LIT( KNote, "unmounthandlerao" );
	User::LeaveIfError( iMsgQueue.CreateGlobal( KNote, 32) );
    }

CUnmountHandlerAO::~CUnmountHandlerAO()
    {
	Cancel(); // Cancel any request, if outstanding
	iMsgQueue.Close();
    }

void CUnmountHandlerAO::DoCancel()
    {
	iMsgQueue.CancelDataAvailable();
    }

void CUnmountHandlerAO::WaitForUnmountL()
    {
	Cancel();							// Cancel any request, just to be sure
	iMsgQueue.NotifyDataAvailable( iStatus );
	SetActive();						// Tell scheduler a request is active
    }

void CUnmountHandlerAO::RunL()
    {
	User::LeaveIfError( iStatus.Int() );
	if( iUnmountObserver )
		{
		TUint32 mediaId;
		const TInt err = iMsgQueue.Receive( mediaId );
		if( err == KErrNone )
			{
			iUnmountObserver->HandleUnmount( mediaId );
			}
		}
	WaitForUnmountL();
    }

TInt CUnmountHandlerAO::RunError( TInt /*aError*/ )
    {
	return KErrNone;
    }
