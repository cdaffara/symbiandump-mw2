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

#ifndef __CSENDASSESSION_H__
#define __CSENDASSESSION_H__

#include <e32base.h>
#include <msvapi.h>
#include <tsendasclientpanic.h>
#include <csendasmessagetypes.h>
#include <tsendasmessagetypefilter.h>

// forward declarations
class CSendAsServer;
class CSendAsMessage;
class CSendAsMtmManager;
class CSendAsAccounts;
class CSendAsActiveContainer;

/**
Server-side session object representing a client-side session.

SendAs messages are represented as subsessions, one per message.
Subsessions are stored in an object container index.

@internalComponent
@released
*/
class CSendAsSession : public CSession2
	{
public:
	static CSendAsSession* NewL(const TVersion& aVersion, CSendAsServer& aServer);
	virtual ~CSendAsSession();
	
	CMsvSession& GetMsvSessionL();
	CSendAsActiveContainer& ActiveContainer();
	void PanicClient(const RMessage2& aMessage, TSendAsClientPanic aPanic) const;

	CBaseMtm* GetClientMtmL(TUid aMtmUid);
	const TUid& NotifierUid() const;
	const TUid& EditUtilsPluginUid() const;
	//
	void HandleMtmChange();
private:
	CSendAsSession(CSendAsServer& aServer);	
	void ConstructL();

	void RemoveMessage(TUint aHandle);
	CSendAsMessage* MessageFromHandle(TUint aHandle);

	// IPC servicing methods
	TBool DoServiceL(const RMessage2& aMessage);
	void CreateSubsessionL(const RMessage2& aMessage);
	void DoSetMessageTypeFilterL(const RMessage2& aMessage);
	void DoClearMessageTypeFilterL();
	void DoGetMessageTypeListLengthL(const RMessage2& aMessage);
	void DoGetMessageTypeListL(const RMessage2& aMessage);
	void DoGetAccountListLengthL(const RMessage2& aMessage);
	void DoGetAccountListL(const RMessage2& aMessage);

	// 
	void AddTypeFilterL(const TSendAsMessageTypeFilter& aFilter);
	void ResetTypeFilterL();

	// from CSession2
	virtual void ServiceL(const RMessage2& aMessage);
	virtual void Disconnect(const RMessage2& aMessage);
private:
	TInt				iMessageCount;	// total num of messages created

	// session object tracking 
	CSendAsServer& 		iServer;
	CObjectCon* 		iContainer;	// object container for this session
	CObjectIx*  		iMessages;	// object index storing CSendAsMessage's for this session

	// session data
	CSendAsMessageTypes*	iAvailableMessageTypes;
	CMsvEntry*	 			iMsvEntry;
	CSendAsAccounts* 		iSendAsAccounts;
	};
	
#endif	// __CSENDASSESSION_H__
