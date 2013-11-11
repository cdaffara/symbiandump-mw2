/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*/

#ifndef __ONDEMANDAO_H__
#define __ONDEMANDAO_H__

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib

class CMdESession;
class CMdEHarvesterSession;
class MMonitorPluginObserver;
class CHarvesterPluginFactory;
class CHarvesterData;

class COnDemandAO : public CActive
{
public:
	// Cancel and destroy
	~COnDemandAO();

	// Two-phased constructor.
	static COnDemandAO* NewL( CMdESession& aSession, MMonitorPluginObserver& aObserver,
			CHarvesterPluginFactory& aPluginFactory, RPointerArray<CHarvesterData>* aPhArray);

	// Two-phased constructor.
	static COnDemandAO* NewLC( CMdESession& aSession, MMonitorPluginObserver& aObserver,
			CHarvesterPluginFactory& aPluginFactory, RPointerArray<CHarvesterData>* aPhArray);

public: // New functions
	void StartL();

private:
	// C++ constructor
	COnDemandAO();
	
	// Second-phase constructor
	void ConstructL( CMdESession& aSession, MMonitorPluginObserver& aObserver,
			CHarvesterPluginFactory& aPluginFactory, RPointerArray<CHarvesterData>* aPhArray);
	void WaitHarvestingRequest();
	
private: // From CActive
	// Handle completion
	void RunL();
	
	// How to cancel me
	void DoCancel();
	
	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError( TInt aError );

private:

private:
	CMdEHarvesterSession* iMdEHarvesterSession;
	MMonitorPluginObserver* iObserver;
	CHarvesterPluginFactory* iPluginFactory;
	
	RPointerArray<CHarvesterData> *iReadyPhArray;
};

#endif // __ONDEMANDAO_H__
