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
* Description:  Implementation of the audio buffer write request active object.
*
*/



// INCLUDE FILES
#ifdef _DEBUG
#include 	<e32svr.h>
#endif

#include "S60StreamingSourceCustomCommands.h"
#include "WriteRequest.h"
#include "S60SourceEventDispatcher.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CWriteRequest::CWriteRequest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CWriteRequest::CWriteRequest(
	TMMFMessageDestinationPckg aMessageHandler,
	RMMFController& aController,
	CS60SourceEventDispatcher& aCallback )
    :   CActive(CActive::EPriorityStandard),
    	iController(aController),
    	iMessageHandler(aMessageHandler),
    	iCallback(aCallback),
    	iClientBuffer(NULL),
    	iProcessed(EFalse),
    	iFree(ETrue)
    {
    }

// -----------------------------------------------------------------------------
// CWriteRequest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CWriteRequest::ConstructL()
    {
    CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CWriteRequest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CWriteRequest* CWriteRequest::NewL(
	TMMFMessageDestinationPckg aMessageHandler,
	RMMFController& aController,
	CS60SourceEventDispatcher& aCallback )
    {
    CWriteRequest* self = new(ELeave) CWriteRequest(aMessageHandler, aController, aCallback);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
// CWriteRequest::~CWriteRequest
// Destructor
// -----------------------------------------------------------------------------
//
CWriteRequest::~CWriteRequest()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CWriteRequest::~CWriteRequest()\n"));
#endif
    Cancel();
    }

// -----------------------------------------------------------------------------
// CWriteRequest::WriteAudioData
// -----------------------------------------------------------------------------
//
void CWriteRequest::WriteAudioData(CClientAudioBuffer& aBuffer)
    {
	if( !IsActive() )
		{
		if( aBuffer.IsLastBuffer() )
			iLastBufferPckg() = 1;
		else
			iLastBufferPckg() = 0;

		iClientBuffer = &aBuffer;
    	iController.CustomCommandAsync(iMessageHandler, (TInt)EProcessBuffer, iClientBuffer->GetBufferPtr(), iLastBufferPckg, iStatus);
    	iProcessed = EFalse;
    	iFree = EFalse;
    	SetActive();
		}
    }

// -----------------------------------------------------------------------------
// CWriteRequest::IsFree
// Destructor
// -----------------------------------------------------------------------------
//
TBool CWriteRequest::IsFree()
    {
 	return iFree;
    }

// -----------------------------------------------------------------------------
// CWriteRequest::RunL
// Invoke by the active scheduler when a request completes -- the buffer has been
// processed.
// -----------------------------------------------------------------------------
//
void CWriteRequest::RunL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CWriteRequest::RunL()\n"));
#endif

	if( iStatus == KErrNone )
		{
		iCallback.BufferProcessed(*iClientBuffer, KErrNone);
		iClientBuffer = NULL;
		iFree = ETrue;
		}
	if( iStatus == KErrCancel )
		{
		iCallback.BufferProcessed(*iClientBuffer, KErrCancel);
		iClientBuffer = NULL;
		iFree = ETrue;
		}

    }

// -----------------------------------------------------------------------------
// CWriteRequest::DoCancel
// Cancels the current and any on going requests/tasks.
// -----------------------------------------------------------------------------
//
void CWriteRequest::DoCancel()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CWriteRequest::DoCancel()\n"));
#endif
	TRequestStatus* s = &iStatus;

	User::RequestComplete(s, KErrCancel);
    }

// End of file

