/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Timer implementation
*
*/
/*
* ============================================================================
*  Name:        cdelay.cpp
* 
*  Description: Timer implementation
*
* ============================================================================
*/

// Includes

#include <e32base.h>
#include <e32debug.h>

#include "cdelay.h"


CDelay::CDelay()
      : CTimer(EPriorityStandard)
	{
	}
	
CDelay::~CDelay()
	{
	}
	
void CDelay::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add(this);
	
	iWaitLoop = new (ELeave) CActiveSchedulerWait();
	}
	
CDelay* CDelay::NewL()
	{
	CDelay* self = new (ELeave) CDelay();
	
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	
	return self;
	}
	
void CDelay::Cancel()
	{
	CTimer::Cancel();
	
	if ( iWaitLoop->IsStarted() )
		{
		iWaitLoop->AsyncStop();
		}
	}

void CDelay::Start( TInt aDelay )
	{
	RDebug::Print(_L("[CCETEST] CDelay::Start()"));
	
	CTimer::After( aDelay * 1000 );
	
	if ( !IsActive() )
        {
        SetActive();
        }
     
    if ( !iWaitLoop->IsStarted() )
    	{
	    iWaitLoop->Start();
    	}
    	
    RDebug::Print(_L("[CCETEST] CDelay::Start() Timer completed"));
	}
	
void CDelay::RunL()
	{
	iWaitLoop->AsyncStop();
	}