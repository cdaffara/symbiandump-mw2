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
* This contains CT_MsgTimer class
* 
*
*/





#ifndef __T_MSG_TIMER_H__
#define __T_MSG_TIMER_H__

/* User Includes */
#include <test/testexecutestepbase.h>




/* Interface to be implemented by the dervied classes for asynchronous operation */
class MT_MsgTimerCallback
	{
public:
	/* To be implemented by the derived class */
	virtual void	TimerCompletedL(CTimer* aTimer) = 0;
	};




/* Implementes a class that dervies from the CTimer, for an active timer object */
class CT_MsgTimer : public CTimer
	{
public:
	/**
	* Two phase constructor that allocates and constructs
	* a new CT_MsgTimer object 
	*
	* \param aCallback object to inform on timer completion.
	* \param aPriority priority of CT_MsgTimer object.
	* \return New Callback CT_MsgTimer object.
	*/
	static CT_MsgTimer*	NewL(MT_MsgTimerCallback& aCallback, TInt aPriority = EPriorityStandard);

protected:
	CT_MsgTimer(MT_MsgTimerCallback& aCallback, TInt aPriority);

	/* 
	 * Constructs a new asynchronous timer 
	 * And adds the object to the active scheduler
	 */
	void ConstructL();

	/* Signals on the complettion of the Timer */
	virtual void RunL();

private:
	/*
	 * This is internal and not intended for use.
	 */
	MT_MsgTimerCallback&	iCallback;
	};

#endif /* __T_MSG_TIMER_H__ */
