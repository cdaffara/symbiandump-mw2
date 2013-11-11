/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provides an observer interface for server logic AO operation
*
*/



#ifndef M_LBTDELETESESSIONTRIGGEROBSERVER_H
#define M_LBTDELETESESSIONTRIGGEROBSERVER_H

// FORWARD DECLARATION
class CLbtDeleteSessionTriggers;

/**
 * Class that needs to implement this interface to listen
 * to events that come from the delete session triggers AO 
 * Operation class.
 *
 * @since S60 v4.0
 */
class MLbtDeleteSessionTriggerObserver
	{
	public:
	/**
	 * This method will be invoked by the delete session trigger AO
	 * object when it completes deletion of all the session triggers of 
	 * a particular client.After the operation is complete the operation
	 * object can be deleted. The operation object sends the
	 * pointer itself for identification
	 *
	 * @param[in] aDeleteSessionTriggers pointer to the operation object
	 * which invokes this method
	 */
	virtual void HandleDeleteSessionTriggersClosureL(CLbtDeleteSessionTriggers*
												    aDeleteSessionTriggers)=0;
	};
#endif // M_LBTDELETESESSIONTRIGGEROBSERVER_H
