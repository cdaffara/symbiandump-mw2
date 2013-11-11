// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32std.h>
#include "MENTACT.H"
#include "MSVPANIC.H"


//
// CMsgActive framework
//


EXPORT_C CMsgActive::CMsgActive(TInt aPriority)
	:CActive(aPriority)
	{}


EXPORT_C CMsgActive::~CMsgActive()
	{
	Cancel();		// framework requirement
	}


EXPORT_C void CMsgActive::Queue(TRequestStatus& aStatus)
//
// call this last when an asynch operation has been requested
//
	{
	__ASSERT_DEBUG(iReport==NULL, PanicServer(EMsvCMsgActiveAlreadyActive));
//
	aStatus=KRequestPending;
	iReport=&aStatus;
	}


EXPORT_C void CMsgActive::DoCancel()
//
// must be called at end of derived classes DoCancel()
//
	{
	TInt result=KErrCancel;
	Complete(result);	// can be done safely as asynch reporting
	}


EXPORT_C void CMsgActive::RunL()
//
// When the AO is state driven, this form of Run() is very effective
// DoRunL() takes the AO through the states, queuing another asynch step as required
// if DoRunL() detects the end of the cycle it returns without queuing another cycle.
//
// If Run() would exit without queuing another cycle it reports completion to the client.
// This is true if the asynch step or DoRunL fails, or the state cycle is complete
//
	{
	TInt status=iStatus.Int();
	if (status>=KErrNone)
		{
		TRAPD(error,DoRunL());		// continue operations, may re-queue
		__ASSERT_DEBUG(error==KErrNone || !IsActive(),User::Invariant());	// must not requeue in error situations
		if (IsActive())				// requeud
			return;
		status=error;
		}
	Complete(status);
	}


EXPORT_C void CMsgActive::Complete(TInt aStatus)
	{
	if (iReport)
		{
		// only complete properly once.
		// this allows a derived class to complete and then cancel, reporting the desired state instead of KErrCancel
		DoComplete(aStatus);
		User::RequestComplete(iReport,aStatus);
		}
	}


EXPORT_C void CMsgActive::DoComplete(TInt&)
//
// Default implementation does nothing
//
	{}
