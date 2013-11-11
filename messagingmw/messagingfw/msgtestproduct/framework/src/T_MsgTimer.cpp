// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file 
// 
//


/* User Includes */
#include "T_MsgTimer.h"


/**
  Function : NewL
  Description : Creates an object of CT_MsgTimer
  @return : N/A
  @leave  :	KErrNoMemory	There is no sufficient memory	
*/
CT_MsgTimer* CT_MsgTimer::NewL(MT_MsgTimerCallback& aCallback, TInt aPriority)
	{
	CT_MsgTimer*	ret=new (ELeave) CT_MsgTimer(aCallback, aPriority);
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}




/**
  Function : CT_MsgTimer
  Description : Constructor
  @return : N/A
*/
CT_MsgTimer::CT_MsgTimer(MT_MsgTimerCallback& aCallback, TInt aPriority)
:	CTimer(aPriority)
,	iCallback(aCallback)
	{
	}



/**
  Function : ConstructL
  Description : Calls the ConstrutL of CTimer class
				and adds the object to Active Scheduler
  @return : N/A
*/
void CT_MsgTimer::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add(this);
	}


/**
  Function : RunL
  Description : Signals the completion of the operation
  @return : N/A
*/
void CT_MsgTimer::RunL()
	{
	iCallback.TimerCompletedL(this);
	}
