// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CSENDASSERVER_H__
#define __CSENDASSERVER_H__

#include <e32base.h>
#include <msvapi.h>

// forward declarations
class CSendAsSession;
class CSendAsServerTimer;

class CSendAsActiveContainer;
class CSendAsMtmManager;

/** 
Server class containing an object container index, in order to provide subsessions.

Using a server-global containder index guarantees unique object containers for the
whole server.

@internalComponent
@released
*/
class CSendAsServer : public CServer2, public MMsvSessionObserver
	{
public:
	static CSendAsServer* NewLC();
	virtual ~CSendAsServer();
	//
	CMsvSession& GetMsvSessionL();
	CSendAsMtmManager* GetMtmManager();
	//
	CSendAsActiveContainer& ActiveContainer();
	void ContainerEmpty();
	//
	void SessionClosed(CObjectCon* aSessionContainer);
	CObjectCon* NewContainerL();
	//
	const TUid& NotifierUid() const;
	const TUid& EditUtilsPluginUid() const;
private:
	CSendAsServer();
	void ConstructL();
	void ReadResourceFileL();
	//
	CSession2* DoNewSessionL(const TVersion& aVersion, const RMessage2& aMsg);
	// from CServer2
	virtual CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMsg) const;
	// from MMsvSessionObserver
	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	//
	void HandleMtmChangeL(TMsvSessionEvent& aEvent);
private:
	CSendAsMtmManager* 		iMtmManager;
	CMsvSession* 			iMsvSession;
	CObjectConIx* 			iContainerIndex;
	CSendAsServerTimer* 	iCloseTimer;
	TInt  					iOpenSessions;
	CSendAsActiveContainer* iActiveContainer;
	TUid                    iEditUtilsPluginUid;
	TUid                    iNotifierUid;
	};

#endif	// __CSENDASSERVER_H__
