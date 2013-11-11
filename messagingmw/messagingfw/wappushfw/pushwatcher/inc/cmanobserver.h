// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains the declaration of the MConnManObserver which is abstract interface to the
// connectino Manager
// 
//

/**
 @file CMANOBSERVER.H
*/

#ifndef __CMANOBSERVER_H__
#define __CMANOBSERVER_H__

class CCOWatcherBase;
class TPushConnPoint;

/**
 * Abstract interface to the connection manager for callbacks from the COWatchers.
 * Functions are implemented in Connection Manager. 
 */
//##ModelId=3B659EE10376
class MConnManObserver
/**
@internalComponent
@released
*/
	{
public:
	
	/**
	 * This method is defined to allow the SIA content type handler
	 * to create a new connection without owning a handle to the 
	 * connection manager.  
	 * @param TPushConnPoint& aConnPoint
	 *					in: Details of the security level and address of the connection 
	 * @return None
	 */

	//##ModelId=3B659EE1038A
	virtual void CMOpenConnectionL(TPushConnPoint& aConnPoint)=0;
	/**
	 * This method is a mechanism for the connection-oriented watchers 
	 * to report back their status.  The connection manager can delete them 
	 * when they complete
	 * @param CCOWatcherBase& aCOWatcher
	 *					in: reference to the COWatcher that has completed
	 * @param TInt aError
	 *					in: Error code of completing CO Watcher
	 * @return None
	 */
	//##ModelId=3B659EE10380
	virtual void CMWatcherComplete(CCOWatcherBase& aCOWatcher, TInt aError)=0;
	};


#endif
