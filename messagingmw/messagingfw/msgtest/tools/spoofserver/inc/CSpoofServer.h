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

#ifndef __CSPOOFSERVER_H__
#define __CSPOOFSERVER_H__

#include <in_sock.h>
#include "cscriptfileprocessor.h"

// Definitions
const TInt KNextLineBufferSize = 2048;
const TInt KCurrentBufferSize = KNextLineBufferSize + 2;

class MImapTestEventHandler
	{
public:
	virtual void TestComplete(TInt aErrorCode)=0;
	};

class CSpoofServer : public CActive 
	{

public:
	IMPORT_C static CSpoofServer* NewL(MImapTestEventHandler& aObserver,const TDesC& aScriptFile);
	IMPORT_C static CSpoofServer* NewLC(MImapTestEventHandler& aObserver,const TDesC& aScriptFile);
	IMPORT_C void StartL(const TInt aPortNumber);
	~CSpoofServer();
	
private:
	CSpoofServer(MImapTestEventHandler& aObserver);
	void ConstructL(const TDesC& aScriptFile);
	void IssueWrite();
	void IssueRead();
	//implemented functions from CActive
	virtual void DoCancel();
	virtual void RunL();
	virtual TInt RunError(TInt aError);
	
private:
	enum TIMAPServerState 
		{
		EIdle=0, 
		EWriting, 
		EReading
		};
	TIMAPServerState iServerStatus;
	RSocket iListeningSocket;
	RSocket iServiceSocket;
	RSocketServ iSocketServer;
	TBuf8<KCurrentBufferSize> iBuffer; 
	TBuf8<KNextLineBufferSize> iNextLine;
	TSockXfrLength iReadLength;
	CScriptFileProcessor* iScriptFileProcessor;
	MImapTestEventHandler& iObserver;
	};

#endif  //__CSPOOFSERVER_H__

