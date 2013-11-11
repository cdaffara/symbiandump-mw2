/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
* @file 
* This contains the header file for CT_MsgAsyncStep.
* 
*
*/




#ifndef __T_MSG_ASYNCSTEP_H__
#define __T_MSG_ASYNCSTEP_H__


/* User includes */
#include "T_MsgStep.h"
#include "T_MsgActive.h"
#include "T_MsgTimer.h"


/* This implements a base class for the asynchronous test steps */
class CT_MsgAsyncStep : public CT_MsgStep, private MT_MsgActiveCallback, private MT_MsgTimerCallback
	{
public:
	/**
	* Destructor
	*/
	virtual ~CT_MsgAsyncStep();

	/* Derived from CTestStep
	 * Calls the doTestStepPreambleL of the base class CTestStep
	 * Reads in the ProgressTime and CancelTime for the asynchronous operation
	 */
	virtual enum TVerdict doTestStepPreambleL();

protected:

	CT_MsgAsyncStep();

	/* 
	 * Children must call this in their own NewL().
 	 */
	void ConstructL(); 				 

	/* Method to return the object of CT_MsgActive */
	CT_MsgActive& Active() { return *iActive; }

	/* Derived  from CT_MsgStep 
	 * Displays the progress of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal) = 0;

private:
	/*	Derived from MT_MsgActiveCallback */
	virtual void Completed();

	/*	MT_MsgTimerCallback implementation */
	virtual void	TimerCompletedL(CTimer* aTimer);

private:
	/* To be used in asynchronous operations */
	CT_MsgActive*		iActive;
	/* Timer object */
	CT_MsgTimer*		iTimerCancel;
	/* Gets the timer progress */
	CT_MsgTimer*		iTimerProgress;

	TInt iTimeProgress;
	};

#endif /* __T_MSG_ASYNCSTEP_H__ */
