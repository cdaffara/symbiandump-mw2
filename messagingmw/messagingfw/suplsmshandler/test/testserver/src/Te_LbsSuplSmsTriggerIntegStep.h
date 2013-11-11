// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is an integration unit test class for the SUPL Sms Trigger Plug-in
// 
//

#if (!defined __TE_LBSSUPLSMSTRIGGERINTEG_STEP__)
#define __TE_LBSSUPLSMSTRIGGERINTEG_STEP__

/**
@file
@internalComponent
@prototype
*/

#include <testexecutestepbase.h>
#include <lbs/lbssuplpushreceiver.h>
#include <lbs/lbssuplpush.h>

#include "Te_LbsSuplSmsTriggerSender.h"
#include "Te_BaseStep.h"
#include "Te_WatcherLog.h"

/** The string name of the test step */
_LIT(KLbsSuplSmsTriggerIntegStep, "LbsSuplSmsTriggerIntegStep");

/**
This is an integration unit test class for the SUPL Sms Trigger Plug-in.
*/
class CTe_LbsSuplSmsTriggerIntegStep : public CTe_BaseStep, public MTe_LbsSuplSmsTriggerSenderObserver, 
		public MLbsSuplPushRecObserver, public MLbsSuplPushObserver
	{
public:
	CTe_LbsSuplSmsTriggerIntegStep();
	virtual ~CTe_LbsSuplSmsTriggerIntegStep();
	
	virtual TVerdict doTestStepPreambleL();
	
	virtual void OnMessageSent(TInt aError);
	virtual void OnSuplInit(TLbsSuplPushChannel aChannel, TLbsSuplPushRequestId aReqId, TDesC8& aMsg);
	virtual void OnSuplInitComplete(TLbsSuplPushChannel aChannel, TLbsSuplPushRequestId aReqId, 
			TInt aError, TInt aReserved);
	
protected:
	virtual void ReallyDoTestStepL();
	virtual void SendMessage(TUint aNum);
	virtual TBool CompareMessage(TUint aNum, TDesC8& aMsg);
		
protected:
	/** Stores the error code if a error happens in an async function*/
	TInt iUnexpectedError;
	/** Sends SUPL INIT message via UDP */
	CTe_LbsSuplSmsTriggerSender* iSender;
	/** The receiver part of the SUPL Push API */
	CLbsSuplPushRec* iReceiver;
	/** The simulation of the Watcher Framework logger */
	CTe_WatcherLog* iWatcherLog;
	/** Counter of sent/received messages */
	TInt iMsgCount;
	/** The total number of messages sent for a test */
	TInt iMsgMaxCount;
	/** The message buffer used to send/compare messages */
	TBuf8<32> iMessage;
	/** The number used to fill a sent message */
	TUint iSendFillDigit;
	/** The number used to check the received message */
	TUint iRecvFillDigit;
	};
		

#endif //__TE_LBSSUPLSMSTRIGGERINTEG_STEP__
