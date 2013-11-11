/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef M_LBTAOOBSERVER_H
#define M_LBTAOOBSERVER_H

// FORWARD DECLARATION
class CLbtAOOperationBase;

/**
 * Class that needs to implement this interface to listen
 * to events that come from the AO Operation class
 *
 * @since S60 v4.0
 */
class MLbtAOObserver
	{
	public:
		/**
		 * Notifies the observer of any notifications from 
		 * Container or stratergy manager. This method will be
		 * invoked by the operation object when it completes
		 * its operation for which the object would have been
		 * created. After the operation is complete the operation
		 * object can be deleted. The operation object sends the
		 * pointer itself for identification
		 *
		 * @param[in] aOperation pointer to the operation object
		 * which invokes this method
		 * @param aStatus the completion status of the messsage
		 */
		virtual void HandleOperationClosureL( CLbtAOOperationBase* aOperation, 											  
											  TInt aStatus ) = 0;
	};
	
#endif //M_LBTAOOBSERVER_H