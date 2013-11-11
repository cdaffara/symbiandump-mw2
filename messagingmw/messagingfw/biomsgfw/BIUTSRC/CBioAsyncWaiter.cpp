// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <cbioasyncwaiter.h>

/**
Standard 2-phase contruction.

Allocates and initialises the object.

@return A pointer to the created object.
*/
EXPORT_C CBioAsyncWaiter* CBioAsyncWaiter::NewLC()
	{
	CBioAsyncWaiter* self = new(ELeave) CBioAsyncWaiter();
	CleanupStack::PushL(self);
	return self;
	}

CBioAsyncWaiter::CBioAsyncWaiter()
	: CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}	

/**
Destructor.

Also cancels any outstanding requests.
*/
EXPORT_C CBioAsyncWaiter::~CBioAsyncWaiter()
	{
	Cancel();
	}
	
/**
Starts the asynchronous request.

Starts the active scheduler and waits for the asynchronous request
to complete.
*/
EXPORT_C void CBioAsyncWaiter::StartAndWait()
	{
	SetActive();
	CActiveScheduler::Start();
	}

/**
Obtains the result of the asynchronous request.

Returns the error code of the last asynchronous request that
was made.

@return KErrNone is request was successful, otherwise the error
		code that the request completed with.
*/
EXPORT_C TInt CBioAsyncWaiter::Result() const
	{
	return iError;
	}
	
void CBioAsyncWaiter::RunL()
	{
	iError = iStatus.Int();
	CActiveScheduler::Stop();
	}
	
void CBioAsyncWaiter::DoCancel()
	{
	iError = KErrCancel;
	CActiveScheduler::Stop();
	}

