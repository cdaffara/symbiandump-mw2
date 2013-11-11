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
* This contains the class CT_MsgActive
* 
*
*/




#ifndef __T_MSG_ACTIVE_H__
#define __T_MSG_ACTIVE_H__


/* Epoc includes */
#include <e32base.h>


/**
 * This implements an interface for the Test Timer Cmpletion Callback
 */
class MT_MsgActiveCallback
	{
public:
	/*
	* Method from which CT_MsgActive informs the user with RunL call
    * To be implemented by the derived class
	*/
	virtual void Completed() = 0;

	/*
	* Method from which CT_MsgActive informs the user with DoCancel call
    * To be implemented by the derived class
	*/
	virtual void CancelStep() = 0;
	};



/**
 * This implements a Test Active Notification class
 *
 */
class CT_MsgActive : public CActive
	{
public:
	/**
	* Destructor
	*/
	virtual ~CT_MsgActive();

	/**
	* Two phase constructor that allocates and constructs
	* a new Active object whose actions are performed by a callback
	*
	* \param aCallback object to inform on RunL.
	* \param aPriority priority of active object.
	* \return New Callback active object.
	*/
	static CT_MsgActive* NewL(MT_MsgActiveCallback& aCallback, TInt aPriority=EPriorityStandard);

	/**
	* Two phase constructor that allocates and constructs
	* a new Active object whose actions are performed by a callback
	*
	* \param aCallback object to inform on RunL.
	* \param aPriority priority of active object.
	* \return New Callback active object.
	*/
	static CT_MsgActive*	NewLC(MT_MsgActiveCallback& aCallback, TInt aPriority=EPriorityStandard);

	/**
	* Activate the object
	*/
	void Activate()	{ SetActive(); }

	/**
	* Active the object RunL implementation.
	*
	* Calls the MT_MsgActiveCallback::RunL to inform user that the RunL has been reached.
	*/
	void RunL()		{ iCallback.Completed(); }

	/**
	* Active object DoCancel implementation.
	*
	* Calls the MT_MsgActiveCallback::DoCancel to inform user that the DoCancel has been reached.
	*/
	void DoCancel()	{ iCallback.CancelStep(); }

	/**
	* Gets the status as TInt.
	*/
	TInt Result()	{ return iStatus.Int(); }

protected:
	/**
	* Protected constructor with timer completion callback and priority.
	*
	* Called by two phase constructor.
	*
	* \param aCallback object to inform on timer completion.
	* \param aPriority priority of active object.
	*/
	CT_MsgActive(MT_MsgActiveCallback& aCallback, TInt aPriority);

private:
	/**
	* This is internal and not intended for use.
	*
	* Second phase of two phase constructor.
	*/
	void ConstructL();

private:
	/**
	* This is internal and not intended for use.
	*/
	MT_MsgActiveCallback& iCallback;
	};

#endif /* __T_MSG_ACTIVE_H__ */
