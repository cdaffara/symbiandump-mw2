// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "csendastestedittimer.h"
#include "csendastesteditutils.h"

#include <mturutils.h>
#include <msvids.h>

CSendAsTestEditTimer* CSendAsTestEditTimer::NewL(CSendAsTestEditUtils& aEditUtils)
	{
	CSendAsTestEditTimer* self=new (ELeave) CSendAsTestEditTimer(aEditUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CSendAsTestEditTimer::CSendAsTestEditTimer(CSendAsTestEditUtils& aEditUtils)
: CTimer(EPriorityStandard), iEditUtils(aEditUtils)
	{
	CActiveScheduler::Add(this);
	}
	
CSendAsTestEditTimer::~CSendAsTestEditTimer()
	{
	Cancel();
	}
	
// methods from CActive	
void CSendAsTestEditTimer::RunL()
	{
	// inform the edit utils that the timer is complete
	iEditUtils.TimerDone();
	}
