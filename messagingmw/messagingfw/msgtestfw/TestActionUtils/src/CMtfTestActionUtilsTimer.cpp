// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/


#include "CMtfTestActionUtilsTimer.h"


CMtfTestActionUtilsTimer* CMtfTestActionUtilsTimer::NewL()
	{
	CMtfTestActionUtilsTimer* self = new (ELeave) CMtfTestActionUtilsTimer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CActiveScheduler::Add(self);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionUtilsTimer::CMtfTestActionUtilsTimer() : CTimer(EPriorityHigh)
	{
	}


CMtfTestActionUtilsTimer::~CMtfTestActionUtilsTimer()
	{
	}


void CMtfTestActionUtilsTimer::After(TInt aSec,TRequestStatus& aStatus)
	{
	iRequestStatus = &aStatus;
	aStatus = KRequestPending;
	TTimeIntervalMicroSeconds32 interval = aSec*1000000;
	CTimer::After(interval);
	}


void CMtfTestActionUtilsTimer::RunL()
	{
	User::RequestComplete(iRequestStatus,iStatus.Int());
	}

