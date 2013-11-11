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
// [Test Step] :
// AsyncStep
// Ownes a Active Object for async operations.
// 
//

/* User includes */
#include "T_MsgAsyncStep.h"


/* Literals Used */
_LIT(KProgressTime,	"ProgressTime");
_LIT(KCancelTime,	"CancelTime");


/**
  Function : CT_MsgAsyncStep
  Description : Constructor
  @return : N/A
*/
CT_MsgAsyncStep::CT_MsgAsyncStep()
:	CT_MsgStep()
,	iActive(NULL)
,	iTimerCancel(NULL)
,	iTimerProgress(NULL)
,	iTimeProgress(0)
	{
	}



/**
  Function : ConstructL
  Description : Creates an object of the active object and timer
  @return : N/A
*/
void CT_MsgAsyncStep::ConstructL()
	{
	iActive = CT_MsgActive::NewL(*this);
	iTimerCancel = CT_MsgTimer::NewL(*this);
	iTimerProgress= CT_MsgTimer::NewL(*this);
	}



/**
  Function : ~CT_MsgAsyncStep
  Description : Destructor
  @return : N/A
*/
CT_MsgAsyncStep::~CT_MsgAsyncStep()
	{
	delete iTimerProgress;
	iTimerProgress=NULL;

	delete iTimerCancel;
	iTimerCancel=NULL;

	delete iActive;
	iActive=NULL;
	}



/**
  Function : Completed
  Description : Cancels the wait for completion of an outstanding request
  @return : N/A
*/
void CT_MsgAsyncStep::Completed()
	{
	iTimerProgress->Cancel();
	iTimerCancel->Cancel();
	ProgressL(ETrue);
	CActiveScheduler::Stop();
	}


/**
  Function : TimerCompletedL
  Description : Sets the progress information and cancel information accordingly
  @return : none
*/
void CT_MsgAsyncStep::TimerCompletedL(CTimer* aTimer)
	{
	if ( aTimer==iTimerProgress )
		{
		INFO_PRINTF1(_L("Calling ProgressL(EFalse)"));
		ProgressL(EFalse);
		iTimerProgress->After(iTimeProgress);
		}
	else if ( aTimer==iTimerCancel )
		{
		INFO_PRINTF1(_L("Calling Cancel()"));
		iTimerProgress->Cancel();
		CancelStep();
		CActiveScheduler::Stop();
		}
	}



/**
  Function : doTestStepPreambleL
  Description : Call the doTestStepPreambleL of the CTestStep class
				Reads the porgress time and cancel time from the ini
  @return : TVerdict result
*/
enum TVerdict CT_MsgAsyncStep::doTestStepPreambleL()
	{
	TVerdict ret=CTestStep::doTestStepPreambleL();

	if( GetIntFromConfig( ConfigSection(), KProgressTime, iTimeProgress) )
		{
		iTimerProgress->After(iTimeProgress);
		}

	TInt time = 0;
	if( GetIntFromConfig( ConfigSection(), KCancelTime, time) )
		{
		iTimerCancel->After(time);
		}
	return ret;
	}
