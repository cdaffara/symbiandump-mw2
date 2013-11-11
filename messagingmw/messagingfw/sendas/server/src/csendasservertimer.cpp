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

#include "csendasservertimer.h"

/** Utility AO factory function.

This AO is used by the server to implement its shutdown timers.

If the timer expires the active scheduler will be stopped, killing the server.
*/
CSendAsServerTimer* CSendAsServerTimer::NewL()
	{
	CSendAsServerTimer *self = new (ELeave) CSendAsServerTimer();
	CleanupStack::PushL(self);
	self->ConstructL();	// from CTimer
	CleanupStack::Pop(self);
	return self;
	}
	
void CSendAsServerTimer::RunL()
	{
	// stopping the scheduler stops the server.
	CActiveScheduler::Stop(); 
	}
	
CSendAsServerTimer::CSendAsServerTimer()
: CTimer(CActive::EPriorityLow)
	{
	CActiveScheduler::Add(this);
	}
