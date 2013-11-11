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

#ifndef __T_PERFORMANCE423CLIENT_H__
#define __T_PERFORMANCE423CLIENT_H__

#include "emailtestutils.h"
#include <iapprefs.h>
#include <cemailaccounts.h>
#include <imapconnectionobserver.h>
#include "T_PerformanceStepBase.h"

// For loading the serial comms device drivers 
#if defined (__WINS__)
	#define PDD_NAME		_L("ECDRV")
	#define LDD_NAME		_L("ECOMM")
#else
	#define PDD_NAME		_L("EUART1")
	#define LDD_NAME		_L("ECOMM")
#endif

class CPerformance423Client : public CActive , public MMsvImapConnectionObserver
	{

public:
	
	static CPerformance423Client* NewL(CPerformanceStepBase& aStepBase);
	static CPerformance423Client* NewLC(CPerformanceStepBase& aStepBase);
	void StartL();
	~CPerformance423Client();
private:
	CPerformance423Client(CPerformanceStepBase& aStepBase);
	void ConstructL();
	virtual void DoCancel();
	virtual void RunL();
	virtual TInt RunError(TInt aError);
	void CompleteSelf();
	//implementation of pure virtual inherited from MMsvImapConnectionObserver 
	virtual void HandleImapConnectionEvent(TImapConnectionEvent aConnectionState);
private:

	enum TIMAPClientState 
		{
		EConnectAndSync=0,
		EWait,
		EFetch,	
		EDisconnect, 
		EComplete
		};
		
	TImapAccount iImapAccount;
	TSmtpAccount iSmtpAccount;
	CMsvSession* iSession;
	CBaseMtm* iClientMtm;
	CClientMtmRegistry*	iClientRegistry;
	TDummySessionObserver*	iSessionObserver;
	CMsvOperation* iMsvOperation;
	TIMAPClientState iState;
	CMsvEntrySelection* iSelection;
	CPerformanceStepBase& iStepBase;
	TPckgBuf<TImImap4GetMailInfo> iPackage;
	CMsvEntry* iInboxEntry;
	CMsvEntrySelection* iChildren;
	CMsvOperation* iOperation;
	CMsvEntrySelection* iEntrySelectionToSend;
	};
	

#endif  //__T_PERFORMANCE423CLIENT_H__
