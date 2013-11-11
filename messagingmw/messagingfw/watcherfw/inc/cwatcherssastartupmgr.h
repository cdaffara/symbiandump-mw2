// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __CWATCHERSSASTARTUPMGR_H__
#define __CWATCHERSSASTARTUPMGR_H__

#include <e32base.h>
#include <startup.hrh>          // For start-up states
#include <startupdomaindefs.h>  // For start-up domains
#include <domaindefs.h>
#include <domainmember.h>       // For CDmDomain
#include <e32cmn.h>             // For RArray

#include "cwatcherlauncher.h"   // For CWatcherLauncher
#include "watcher.h"            // For CWatcherLog

	
/**
This class interacts with the Domain Manager to be kept aware of the current
system startup state. 

@internalComponent
@released
*/

class CWatcherSSAStartupMgr: public CDmDomain
	{

public:
	static CWatcherSSAStartupMgr* NewL();
	virtual ~CWatcherSSAStartupMgr();

private:
	CWatcherSSAStartupMgr(TDmHierarchyId aHierarchyId, TDmDomainId aDomainId);
	void ConstructL();
	virtual void RunL();                   // from CActive
	virtual TInt RunError(TInt aError);    // from CActive
	void DoRunErrorL();   // trapped by RunError
	// DoCancel is implemented by CDmDomain and cancels any
	// outstanding transition notification request
	TBool ProcessSSAEventL(TStartupStateIdentifier aKnownState);
	void  PerformFullInitL();
	void  PerformCriticalInitL();
	void  PerformNonCriticalInitL();
	void  StartWatchersL(TUid aEComInterface);
	
private:
	/** Current startup state */
	TStartupStateIdentifier iCurrentStartupState;
	/** List of watcher launchers */
	RPointerArray<CWatcherLauncher> iWatcherList;
		
	CWatcherLog* iLog;
	RFs iFs;
					
	};

#endif // __CWATCHERSSASTARTUPMGR_H__
