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

#ifndef __CSENDASMTMMANAGER_H__
#define __CSENDASMTMMANAGER_H__

#include <e32base.h>
#include <tsendasmessagetypefilter.h>

// forward declarations
class CBaseMtm;
class CMsvSession;
class CClientMtmRegistry;
class CSendAsServer;
class CSendAsMessageTypes;

/**
This class implements most of the MTM instance management for the SendAs server 
sessions and subsessions.

The SendAs server maintains a single instance of this class, while each session
maintains its own array of available MTMs. The SendAs server also owns a 
CMsvSession object and inherits from MMsvSessionObserver. When notified of a message 
server event affecting MTM availability, the sendas server updates the MTM manager 
and each open sendas session of the change.

CSendAsSession uses this class to provide a default list of MTM's available for
sendas operations. Sessions then update their own available list based on subsequent
RSendAs filter operations.

CSendAsMessage uses this class to obtain instances of MTMs.

@internalComponent
@released
*/ 
class CSendAsMtmManager : public CBase
	{
public:
	static CSendAsMtmManager* NewL(CSendAsServer& aServer);
	virtual ~CSendAsMtmManager();
	//
	CBaseMtm* GetClientMtmL(TUid aMtmUid);
	CBaseMtm* FindStoredMtmL(TUid aMtmUid);
	RArray<TUid>& GetMtmUidArray();
	CClientMtmRegistry* GetClientMtmRegistry();
	void RefreshMtmUidArrayL();
private:
	CSendAsMtmManager(CSendAsServer& aServer);
	void ConstructL();
	//
	void PopulateL();
private:
	CSendAsServer& 				iServer;
	RArray<TUid> 				iMtmUidArray;
	RPointerArray<CBaseMtm> 	iClientMtmArray;
	CClientMtmRegistry* 		iClientRegistry;
	};
		
#endif	// __CSENDASMTMMANAGER_H__
