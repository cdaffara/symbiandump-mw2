/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   General purpose class for timeout notification.
*
*/



#include "epos_comasupldialogtimer.h"
#include "epos_momasupldialogtimeoutnotifier.h"

#if defined(_DEBUG)
	_LIT( KPanicMsg, "COMASuplDialogTimer");
#endif	
    
// COMASuplDialogTimer::NewL
// Creates instance of COMASuplDialogTimer

COMASuplDialogTimer* COMASuplDialogTimer::NewL(MOMASuplDialogTimeOutNotifier& aTimeOutNotify)
    {
    COMASuplDialogTimer* self = COMASuplDialogTimer::NewLC(aTimeOutNotify);
    CleanupStack::Pop(self);
    return self;
    }

// COMASuplTimeoutTimer::NewLC
// Creates instance of COMASuplTimeoutTimer & pushes into cleanup stack    
COMASuplDialogTimer* COMASuplDialogTimer::NewLC(MOMASuplDialogTimeOutNotifier& aTimeOutNotify)
    {
    COMASuplDialogTimer* self = new (ELeave) COMASuplDialogTimer(aTimeOutNotify);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// COMASuplDialogTimer::COMASuplDialogTimer
// Constructor
COMASuplDialogTimer::COMASuplDialogTimer(MOMASuplDialogTimeOutNotifier& aTimeOutNotify)
: CTimer(CActive::EPriorityStandard), iNotify(aTimeOutNotify),iTimerExpired(EFalse)
    {
    }

// COMASuplDialogTimer::~COMASuplDialogTimer()
// Destuctor
COMASuplDialogTimer::~COMASuplDialogTimer()
    {
    }

// COMASuplDialogTimer::ConstructL()
// 2nd phase constuction
void COMASuplDialogTimer::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add(this);
    }

// COMASuplDialogTimer::StartTimer()
// Starts timer
void COMASuplDialogTimer::StartTimer( const TInt aTimeOut)
    {
	    if (!IsActive())
	    	{
	    		iTimerExpired = EFalse;
	    		After(aTimeOut);
	    	}
    }

// COMASuplDialogTimer::StopTimer()
// Stops timer
void COMASuplDialogTimer::StopTimer()
    {
    		iTimerExpired = EFalse;
			Cancel();
    }

// COMASuplDialogTimer::RunL()
// Gets called when time out occurs
void COMASuplDialogTimer::RunL()
    {
    // Timer request has completed, so notify the timer's owner
    if (iStatus == KErrNone)
        {
        	iTimerExpired = ETrue;
        	iNotify.DialogTimerExpiredL();
        }
    else
        {
        #if defined(_DEBUG)
        	User::Panic(KPanicMsg, ESuplDialogTimeoutTimerFailed);
        #endif	
        }
    }

TBool COMASuplDialogTimer::IsTimerExpired()
	{
		return iTimerExpired;
	}
TBool COMASuplDialogTimer::IsTimerStarted()
	{
		if(IsActive())
			{
				return ETrue;
			}
		else
			{
				return EFalse;
			}	
	}
