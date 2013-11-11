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
// CASYNCWAITER.H
// 
//

#include "CAsyncWaiter.h"

CAsyncWaiter* CAsyncWaiter::NewL()
	{
	CAsyncWaiter* self = new(ELeave) CAsyncWaiter();
	self->ConstructL();
	return self;
	}

CAsyncWaiter::CAsyncWaiter()
	: CActive(EPriorityStandard)
	{
	
	}	

CAsyncWaiter::~CAsyncWaiter()
	{
	if(IsActive())
		{
		Cancel();
		}
	delete iWaiter;	
	}
	
void CAsyncWaiter::ConstructL()
	{
	iWaiter = new(ELeave) CActiveSchedulerWait();
	CActiveScheduler::Add(this);
	}

void CAsyncWaiter::StartAndWait()
	{
	SetActive();
	iWaiter->Start();
	}

TInt CAsyncWaiter::Result() const
	{
	return iError;
	}
	
void CAsyncWaiter::RunL()
	{
	iError = iStatus.Int();
	iWaiter->AsyncStop();
	}
	
void CAsyncWaiter::DoCancel()
	{
	}


