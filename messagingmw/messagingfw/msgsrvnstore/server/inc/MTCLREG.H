// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#if !defined __MTCLREG_H__
#define __MTCLREG_H__

#include <e32base.h>		// CActive, TFileName etc.
#include <msvreg.h>
#include <msvapi.h>			// MMsvSessionObserver

// CObserverRegistry mixes in session notification API
// this allows run-time update of the registries on the client-side.
// the server registry should inherit from CMtmDllRegistry
class CObserverRegistry : public CMtmDllRegistry, public MMsvSessionObserver
/** Base class for client, UI and UI data MTM registries.

It implements the session observer interface to update the registry information 
when MTM groups are installed or uninstalled.
@publishedAll
@released
*/
	{
public:
	IMPORT_C virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	IMPORT_C ~CObserverRegistry();

protected:
	IMPORT_C CObserverRegistry(CMsvSession& aMsvSession,TUid aDllTypeUid,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32);
	IMPORT_C void ConstructL();

protected:
	CMsvSession& iMsvSession;
	TBool iIsAdded;
	};

// Client side MTM registry
class CClientMtmRegistry : public CObserverRegistry
/** Accesses the Client-side MTM registry. 

This registry holds details of the all the Client-side MTMs currently available 
on the system. Message client applications use this class to get a CBaseMtm-derived 
object by which to access Client-side MTM functionality.

Note that the base class CMtmDllRegistry provides functions for discovering 
what MTMs are present in the registry.
 
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CClientMtmRegistry* NewL(CMsvSession& aMsvSession, TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32=TTimeIntervalMicroSeconds32(30000000));
	IMPORT_C ~CClientMtmRegistry();
	IMPORT_C CBaseMtm* NewMtmL(TUid aMtmTypeUid);
	//
protected:
	CClientMtmRegistry(CMsvSession& aMsvSession,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32);
	void ConstructL();
	//
private:
	CBaseMtm* DoNewMtmL(const RLibrary& aLib, CRegisteredMtmDll& aReg) const;
	};

#endif // __MTCLREG_H__
