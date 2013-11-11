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

/**
 @file
 @internalComponent
 @prototype
*/

#include <e32property.h>
#include <push/pushmessage.h>
#include <ecom/implementationproxy.h>

#include "LbsSuplWapPush.h"

#include "Te_LbsSuplWapPushStep.h"
#include "Te_LbsSuplPush.h"
#include "Te_LbsSuplWapPushCommon.h"
#include "Te_ErrorPattern.h"


const TUint KTestMsgLength = 128;

const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount);

/**
 * Constructor. It must	call the SetTestStepName as the controlling
 * framework uses the test step name immediately following construction to set
 * up the step's unique logging ID. 
 */
CTe_LbsSuplWapPushStep::CTe_LbsSuplWapPushStep()
	{
	SetTestStepName(KLbsSuplWapPushStep);
	CTe_LbsSuplPush::SetTestObserver(this);
	}

/**
 * Destructor.
 */
CTe_LbsSuplWapPushStep::~CTe_LbsSuplWapPushStep()
	{
	delete iReceiver;
	}

/**
Creates the test step environment - the receiver part of the SUPL Push API.

@return - always returns EPass.

@leave If a error happens, it leaves with one of the system error codes.

@see CTe_BaseStep::doTestStepPreambleL
 */
TVerdict CTe_LbsSuplWapPushStep::doTestStepPreambleL()
	{
	CTe_BaseStep::doTestStepPreambleL();
	iReceiver = CLbsSuplPushRec::NewL(*this);
	return TestStepResult();
	}

/**
Overrides pure virtual CTe_BaseStep::ReallyDoTestStepL. The test code is run here.

@leave If a error happens, it leaves with one of the system error codes or EFail.
*/
void CTe_LbsSuplWapPushStep::ReallyDoTestStepL()
	{
	//Successfuly send/receive a single message
	CLbsSuplWapPush* wapPush = MakePluginLC();
	CPushMessage* msg = MakeMessageLC(iSendFillDigit++);
	iReceived = EFalse;
	iSenderError = KErrNone;
	wapPush->HandleMessageL(msg);
	CleanupStack::Pop(msg);
	iSchedulerWait->Start();
	if(iUnexpectedError!=KErrNone || iSenderError!=KErrNone || !iReceived)
		{
		User::Leave(EFail);
		}
	
	
	//Dummy calls just to improve the BullsEye code coverage statistics
	TRequestStatus dummyStatus;
	wapPush->HandleMessageL(NULL, dummyStatus);
	wapPush->CancelHandleMessage();
	wapPush->CPushHandlerBase_Reserved1();
	wapPush->CPushHandlerBase_Reserved2();
	wapPush->RunL();
	wapPush->DoCancel();
	
	TInt dummyInt;
	ImplementationGroupProxy(dummyInt);
	CleanupStack::PopAndDestroy(wapPush);
	
	
	//Passing empty message
	wapPush = MakePluginLC();
	iReceived = EFalse;
	wapPush->HandleMessageL(NULL);
	CleanupStack::PopAndDestroy(wapPush);
	if(iUnexpectedError!=KErrNone || iReceived)
		{
		User::Leave(EFail);
		}
	
	
	//Passing a message with an empty body
	wapPush = MakePluginLC();
	HBufC8* hdr = HBufC8::NewLC(1);
	msg = CPushMessage::NewL(hdr, NULL);
	CleanupStack::Pop(hdr);
	CleanupStack::PushL(msg);
	iReceived = EFalse;
	wapPush->HandleMessageL(msg);
	CleanupStack::Pop(msg);
	CleanupStack::PopAndDestroy(wapPush);
	if(iUnexpectedError!=KErrNone || iReceived)
		{
		User::Leave(EFail);
		}
	
	//Memory allocation failure test - creation of CLbsSuplWapPush	
	TInt* marker = new TInt(0);
	delete marker;
	for(TInt i=1; marker; i++)
		{			
		__UHEAP_FAILNEXT(i);
		
		wapPush = 0;
		TRAPD(err, wapPush = CLbsSuplWapPush::NewL());
		delete wapPush;
		
		marker = new TInt(0);
		delete marker;
		
		if(!marker || (marker && err==KErrNone)) 
			{
			if(wapPush==0)
				{
				User::Leave(KErrGeneral);
				}
			User::LeaveIfError(err);
			}
		}
	
	//Memory allocation failure test - handling of a message	
	marker = new TInt(0);
	delete marker;
	for(TInt i=1; marker; i++)
		{			
		wapPush = MakePluginLC();
		iRecvFillDigit = iSendFillDigit;
		CPushMessage* msg = MakeMessageLC(iSendFillDigit++);
		
		__UHEAP_FAILNEXT(i);
		
		iReceived = EFalse;
		iSenderError = KErrNone;
		TRAPD(err, wapPush->HandleMessageL(msg));
		CleanupStack::Pop(msg);
		iSchedulerWait->Start(12000000);
		CleanupStack::PopAndDestroy(wapPush);
		User::LeaveIfError(err); //this function does not leave
		User::LeaveIfError(iUnexpectedError);
		
		marker = new TInt(0);
		delete marker;
		
		if(!marker) //that is there were no errors in the code tested 
			{
			if(iSenderError!=KErrNone || !iReceived)
				{
				User::Leave(EFail);
				}
			}
		else //there was an allocation error in the code tested
			{
			if(iSenderError!=KErrNone && iSenderError!=KErrTimedOut)
				{
				User::Leave(EFail);
				}
			}
		}	
	}


/**
Overrides the pure virtual MLbsSuplPushRecObserver::OnSuplInit. Receives notifications about 
incoming SUPL INIT messages.

@param aChannel  [In] The channel the call-back is related to.
@param aReqId    [In] An Id of the request the call-back is related to.
@param aMsg      [In] A buffer containing a SUPL INIT message.

@see MLbsSuplPushRecObserver::OnSuplInit
@see CLbsSuplPushRec
*/
void CTe_LbsSuplWapPushStep::OnSuplInit(TLbsSuplPushChannel aChannel, 
		TLbsSuplPushRequestId /*aReqId*/, TDesC8& aMsg)
	{
	iReceived = ETrue;
	if(aChannel!=ELbsSuplPushChannelWAP || !CompareMessage(iRecvFillDigit++, aMsg))
		{
		if(aChannel!=ELbsSuplPushChannelWAP)
			{
			ERR_PRINTF1(_L("CTe_LbsSuplWapPushStep::OnSuplInit: aChannel!=ELbsSuplPushChannelWAP"));
			}
		iUnexpectedError = KErrGeneral;;
		iSchedulerWait->AsyncStop();
		return;
		}
	}

/**
Overrides the pure virtual MLbsSuplPushObserver::OnSuplInitComplete. Receives message delivery 
notifications.

@param aChannel  [In] The channel the call-back is related to.
@param aReqId    [In] An Id of the request the call-back is related to.
@param aError    [In] KErrNone if successful, KErrTimeout if it was not possible to deliver
	                      the request before the timeout period, KErrArgument if the structure 
	                      or content of the SUPL INIT message was incorrect. 
	                      Any system wide error code otherwise.
@param aReserved [In] Reserved for future use.

@see MLbsSuplPushObserver::OnSuplInitComplete
@see CLbsSuplPush
*/
void CTe_LbsSuplWapPushStep::OnSuplInitComplete(TLbsSuplPushChannel aChannel, 
		TLbsSuplPushRequestId /*aReqId*/, TInt aError, TInt /*aReserved*/)
	{
	iSenderError = aError;
	if(aChannel!=ELbsSuplPushChannelWAP)
		{
		ERR_PRINTF1(_L("CTe_LbsSuplSmsTriggerIntegStep::OnSuplInit: aChannel!=ELbsSuplPushChannelSMS"));
		iUnexpectedError = KErrGeneral;;
		}
	iSchedulerWait->AsyncStop();
	}

CLbsSuplWapPush* CTe_LbsSuplWapPushStep::MakePluginLC()
	{
	CLbsSuplWapPush* wapPush = CLbsSuplWapPush::NewL();
	CleanupStack::PushL(wapPush);
	return wapPush;
	}

CPushMessage* CTe_LbsSuplWapPushStep::MakeMessageLC(TUint aNum)
	{
	HBufC8* body = HBufC8::NewMaxLC(KTestMsgLength);
	HBufC8* hdr = HBufC8::NewLC(1);
	TPtr8 des = body->Des();
	aNum = aNum%10;
	for(TUint i=0;i<KTestMsgLength;i++)
		{
		des[i]=aNum;
		}
	
	des[0] = 0;
	des[1] = 0;

	// set the encoded length field
	des[0] |= (TUint8)( KTestMsgLength >> 8 );
	des[1] |= (TUint8)( KTestMsgLength );
	
	CPushMessage* msg = CPushMessage::NewL(hdr, body);
	CleanupStack::Pop(hdr);
	CleanupStack::Pop(body);
	CleanupStack::PushL(msg);
	return msg;
	}

/**
Generates a message using the pattern specified and compares it with the given message.

@param aNum [In] The number used to fill the generated message.

@return ETrue if the messages are identical, EFalse otherwise.

@see CTe_LbsSuplWapPushStep::OnSuplInit
@see CTe_LbsSuplWapPushStep::SendMessage
*/
TBool CTe_LbsSuplWapPushStep::CompareMessage(TUint aNum, TDesC8& aMsg)
	{
	aNum = aNum%10;
	
	if ( aMsg[0] != (TUint8)( KTestMsgLength >> 8 )
	  || aMsg[1] != (TUint8)( KTestMsgLength ) )
		{
		ERR_PRINTF1(_L("CTe_LbsSuplWapPushStep::CompareMessage: wrong message length"));
		return EFalse;
		}
	
	for(TUint i=2;i<KTestMsgLength;i++)
		{
		if(aMsg[i]!=aNum)
			{
			ERR_PRINTF1(_L("CTe_LbsSuplWapPushStep::CompareMessage: wrong message"));
			return EFalse;
			}
		}
	return ETrue;
	}


