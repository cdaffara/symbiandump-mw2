/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of Stream Source Reader active object.
*
*/



// INCLUDE FILES
#ifdef _DEBUG
#include 	<e32svr.h>
#endif

#include "ReadRequest.h"
#include <mmfdatasource.h>
#include <mmfdatasink.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CReadRequest::CReadRequest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CReadRequest::CReadRequest(
	CS60StreamingSource& aCallback,
	CMMFBuffer* aBuffer,
	MDataSink* aConsumer )
    :   CActive(CActive::EPriorityStandard),
    	iCallback(aCallback),
    	iBuffer(aBuffer),
    	iConsumer(aConsumer)
    {
    }

// -----------------------------------------------------------------------------
// CReadRequest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CReadRequest::ConstructL()
    {
    CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CReadRequest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CReadRequest* CReadRequest::NewL(
	CS60StreamingSource& aCallback,
	CMMFBuffer* aBuffer,
	MDataSink* aConsumer )
    {
    CReadRequest* self = new(ELeave) CReadRequest(aCallback, aBuffer, aConsumer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
// CReadRequest::~CReadRequest
// Destructor
// -----------------------------------------------------------------------------
//
CReadRequest::~CReadRequest()
    {
    // We should not have to cancel the outstanding request because the message
    // handler will complete our request with KErrCancel in its destructor.
    Cancel();
    }

// -----------------------------------------------------------------------------
// CReadRequest::HandleRequest
// -----------------------------------------------------------------------------
//
void CReadRequest::HandleRequest()
    {
	if (!IsActive())
		{
		TRequestStatus* s = &iStatus;
		SetActive();
		User::RequestComplete(s, KErrNone);
		}
    }

// -----------------------------------------------------------------------------
// CReadRequest::RunL
// Invoke by the active scheduler when a request completes -- the buffer has been
// processed.
// -----------------------------------------------------------------------------
//
void CReadRequest::RunL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CReadRequest::RunL()\n"));
#endif

	iCallback.HandleFillBuffer(iBuffer, iConsumer);
    }

// -----------------------------------------------------------------------------
// CReadRequest::DoCancel
// Cancels the current and any on going requests/tasks.
// -----------------------------------------------------------------------------
//
void CReadRequest::DoCancel()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CReadRequest::DoCancel()\n"));
#endif
    }

// End of file

