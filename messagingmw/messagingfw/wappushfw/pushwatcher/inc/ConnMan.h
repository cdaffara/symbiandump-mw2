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
// This file contains the declaration of the CConnectionManager which manages the Wap Push Connections
// to the WAP STACK
// WAP Push Message Header File - ConnectionManager
// 
//

/**
 @file ConnMan.H
*/

#ifndef __CONNMAN_H__
#define __CONNMAN_H__

#include <e32base.h>
#include "CLWatcher.h"
#include "cmanobserver.h"


/**
 * 
 *
 *		- Creates Connectionless (CL) Watchers that will *always* be running
 *
 *		- Connection Oriented (CO) connections are started via an exposed API and managed 
 *		  When a CO it opened, it will:
 *
 *			-  Opens a PPP Connection (if one is not already opened)
 *
 *			-  Start a Connection Oriented Watcher, and keeps a reference to it
 *
 *			- If the Connection Oriented Watcher already opened, it leaves it open
 *
 *			- Deletes the Connection Oriented Watcher when signaled EComplete
 *
 *			- Closes PPP Connection if no more Connection Oriented Watchers
 *
 *		Note: The Connection Manager is *always* running
 *
 */
class CConnectionManager : public CActive, public MConnManObserver
/**
@internalComponent
@released
*/
	{
public:
	static CConnectionManager* NewL(MWapPushLog& aLog);
	~CConnectionManager();

	// from MConnManObserver
	void CMOpenConnectionL(TPushConnPoint& aConnPoint);
	void CMWatcherComplete(CCOWatcherBase& aCOWatcher, TInt aError);


private:
	CConnectionManager(MWapPushLog& aLog); 
	void ConstructL();

	// from CActive 
	void RunL();

	void DoCancel();

	TInt RunError(TInt aError);

private:
	MWapPushLog&			iLog;
	CCLUnsecureWatcher*		iCLUnsecWatcher;
	CCLSecureWatcher*		iCLSecureWatcher;	
	TBool					iConnectionExists;
	};


#endif
