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
// CMTFASYNCWAITER.H
// 
//

#include "CMtfAsyncWaiter.h"

CMtfAsyncWaiter* CMtfAsyncWaiter::NewL()
	{
	CMtfAsyncWaiter* self = new(ELeave) CMtfAsyncWaiter();
	return self;
	}

CMtfAsyncWaiter::CMtfAsyncWaiter()
	: CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}	

CMtfAsyncWaiter::~CMtfAsyncWaiter()
	{
	Cancel();
	}
	
void CMtfAsyncWaiter::StartAndWait()
	{
	SetActive();
	CActiveScheduler::Start();
	}
	
TInt CMtfAsyncWaiter::Result() const
	{
	return iError;
	}
	
void CMtfAsyncWaiter::RunL()
	{
	iError = iStatus.Int();
	CActiveScheduler::Stop();
	}
	
void CMtfAsyncWaiter::DoCancel()
	{
	iError = KErrCancel;
	CActiveScheduler::Stop();
	}

