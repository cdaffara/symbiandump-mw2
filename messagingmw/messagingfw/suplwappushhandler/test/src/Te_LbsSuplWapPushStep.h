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
// This is an integration unit test class for the SUPL WAP Push Plug-in
// 
//

#if (!defined __TE_LBSSUPLWAPPUSH_STEP__)
#define __TE_LBSSUPLWAPPUSH_STEP__

/**
@file
@internalComponent
@prototype
*/

#include <test/testexecutestepbase.h>
#include <lbs/lbssuplpushreceiver.h>
#include <lbs/lbssuplpush.h>

#include "Te_BaseStep.h"

/** The string name of the test step */
_LIT(KLbsSuplWapPushStep, "LbsSuplWapPushStep");

class CPushMessage;
class CLbsSuplWapPush;

/**
This is an integration unit test class for the SUPL WAP Push Plug-in.
*/
class CTe_LbsSuplWapPushStep : public CTe_BaseStep, public MLbsSuplPushRecObserver, 
	public MLbsSuplPushObserver
	{
public:
	CTe_LbsSuplWapPushStep();
	virtual ~CTe_LbsSuplWapPushStep();
	
	virtual TVerdict doTestStepPreambleL();
	
	virtual void OnSuplInit(TLbsSuplPushChannel aChannel, TLbsSuplPushRequestId aReqId, TDesC8& aMsg);
	virtual void OnSuplInitComplete(TLbsSuplPushChannel aChannel, TLbsSuplPushRequestId aReqId, 
			TInt aError, TInt aReserved);
	
private:
	virtual void ReallyDoTestStepL();
	CLbsSuplWapPush* MakePluginLC();
	CPushMessage* MakeMessageLC(TUint aNum);
	TBool CompareMessage(TUint aNum, TDesC8& aMsg);
	
		
private:
	/** Stores the error code if a error happens in an async function*/
	TInt iUnexpectedError;
	/** The receiver part of the SUPL Push API */
	CLbsSuplPushRec* iReceiver;
	/** Counter of sent/received messages */
	TInt iMsgCount;
	/** The total number of messages sent for a test */
	TInt iMsgMaxCount;
	/** The number used to fill a sent message */
	TUint iSendFillDigit;
	/** The number used to check the received message */
	TUint iRecvFillDigit;
	/** The flag that message was received by the receiver */
	TBool iReceived;
	/** The error code returned to the sender */
	TInt iSenderError;
	};
		

#endif //__TE_LBSSUPLWAPPUSH_STEP__
