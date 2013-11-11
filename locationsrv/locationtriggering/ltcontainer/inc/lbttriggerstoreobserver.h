/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This file defines the interface which the trigger stores use
*                to notify store observers
*
*/



#ifndef MLBTTRIGGERSTOREOBSERVER_H
#define MLBTTRIGGERSTOREOBSERVER_H

/**
 *  This M class defines the interface for the trigger stores observer
 *
 *  The class that uses trigger store observer must implement this class
 */
class MLbtTriggerStoreObserver
	{
	public:
		/**
		 * This method is used by trigger stores to inform its obervers
		 * that the trigger store is ready to accept requests
		 *
		 */
		virtual void NotifyTriggerStoreReadyToServe() = 0;
	};

#endif // MLBTTRIGGERSTOREOBSERVER_H

