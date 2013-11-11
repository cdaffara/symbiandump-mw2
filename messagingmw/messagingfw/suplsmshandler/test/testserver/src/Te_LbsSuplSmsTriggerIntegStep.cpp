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

/**
 @file
 @internalComponent
 @prototype
*/

#include <e32property.h>
#include <ecom/ecom.h>
#include <centralrepository.h>

#include <wapmessage.h>

#include "lbssuplpushprops.h"
#include "lbsrootcenrepdefs.h"

#include "Te_LbsSuplSmsTriggerIntegStep.h"
#include "Te_SuplWdpWatcher.h"
#include "Te_LbsSuplPush.h"
#include "Te_LbsSuplSmsTriggerCommon.h"
#include "Te_ErrorPattern.h"


/** The port on which test SMS messages sent*/
const TInt KSuplSmsTriggerTestPort = 7276;

/**
 * Constructor. It must	call the SetTestStepName as the controlling
 * framework uses the test step name immediately following construction to set
 * up the step's unique logging ID. 
 */
CTe_LbsSuplSmsTriggerIntegStep::CTe_LbsSuplSmsTriggerIntegStep()
	{
	SetTestStepName(KLbsSuplSmsTriggerIntegStep);
	CTe_LbsSuplPush::SetTestObserver(this);
	}

/**
 * Destructor.
 */
CTe_LbsSuplSmsTriggerIntegStep::~CTe_LbsSuplSmsTriggerIntegStep()
	{
	delete iSender;
	delete iReceiver;
	delete iWatcherLog;
	}

/**
Creates the test step environment: the UDP message sender, the receiver part of the SUPL Push API,
and the Watcher Framework logger.

@return - always returns EPass.

@leave If a error happens, it leaves with one of the system error codes.

@see CTe_BaseStep::doTestStepPreambleL
 */
TVerdict CTe_LbsSuplSmsTriggerIntegStep::doTestStepPreambleL()
	{
	CTe_BaseStep::doTestStepPreambleL();
	iSender = CTe_LbsSuplSmsTriggerSender::NewL(KSuplSmsTriggerTestPort , *this);
	iReceiver = CLbsSuplPushRec::NewL(*this);
	iWatcherLog = CTe_WatcherLog::NewL();
	return TestStepResult();
	}

/**
Overrides pure virtual CTe_BaseStep::ReallyDoTestStepL. The test code is run here.

@leave If a error happens, it leaves with one of the system error codes or EFail.
*/
void CTe_LbsSuplSmsTriggerIntegStep::ReallyDoTestStepL()
	{
	// Get the CategoryUid from the cenrep file owned by LbsRoot.
	TInt category;
	CRepository* rep = CRepository::NewLC(KLbsCenRepUid);
	User::LeaveIfError(rep->Get(KSuplPushAPIKey, category));
	CleanupStack::PopAndDestroy(rep);
	TUid propOwnerSecureId = TUid::Uid(category);
	
	//Reset the values set by previous tests
	User::LeaveIfError(RProperty::Set(propOwnerSecureId, KLbsSuplPushSmsBusyKey, 1));
	User::LeaveIfError(RProperty::Set(propOwnerSecureId, KLbsSuplPushSmsAckKey, 0));
	User::LeaveIfError(RProperty::Set(propOwnerSecureId, KLbsSuplPushSmsInitKey, KNullDesC8));
	
	iMsgMaxCount = 1;

	//Successfuly send/receive a single message
	CLbsSuplWdpWatcher* wdpWatcher = CLbsSuplWdpWatcher::NewLC(iWatcherLog);
	
	CTe_LbsSuplWdpWatcher::DisableAsserts();
	wdpWatcher->OnTimerError(0, KErrGeneral); //Just to improve code coverage
	CTe_LbsSuplWdpWatcher::EnableAsserts();
	
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	SendMessage(iSendFillDigit++);
	iSchedulerWait->Start();
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	User::LeaveIfError(iUnexpectedError);
	CleanupStack::PopAndDestroy(wdpWatcher);
	
	//Successfuly send/receive several messages
	wdpWatcher = CLbsSuplWdpWatcher::NewLC(iWatcherLog);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	for(TInt i=0;i<10;i++)
		{
		SendMessage(iSendFillDigit++);
		iSchedulerWait->Start();
		wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
		User::LeaveIfError(iUnexpectedError);
		}
	CleanupStack::PopAndDestroy(wdpWatcher);
	
	
	//Memory allocation failure test - creation of WdpWatcher
	
	User::__DbgSetAllocFail(EFalse ,RAllocator::EFailNext, 1);
	
	TInt* marker = new TInt(0);
	delete marker;
	
	if(!marker) //We only do memory failure tests for the udeb build of the OS
		{
		marker = new TInt(0);
		delete marker;
			
		for(TInt i=1; marker; i++)
			{			
			User::__DbgSetAllocFail(EFalse ,RAllocator::EFailNext, i);
		
			TRAPD(err, wdpWatcher = CLbsSuplWdpWatcher::NewLC(iWatcherLog); CleanupStack::Pop(wdpWatcher));
		
			marker = new TInt(0);
			delete marker;
		
			if(!marker || (marker && err==KErrNone)) 
				{
				if(wdpWatcher==0)
					{
					User::Leave(KErrGeneral);
					}
				CleanupStack::PushL(wdpWatcher);
				User::LeaveIfError(err);
				wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EDisconnected | CLbsSuplWdpWatcher::EAwaitingDataSize);
				CleanupStack::PopAndDestroy(wdpWatcher);
				}
			}
	
		//Memory allocation failure test - CLbsSuplWdpWatcher::RunL method
		marker = new TInt(0);
		delete marker;
		for(TInt i=1; marker; i++)
			{
			//reset the previous errors and reset the message counter
			iUnexpectedError = KErrNone;
			iRecvFillDigit=iSendFillDigit;
		
			wdpWatcher = CLbsSuplWdpWatcher::NewLC(iWatcherLog);
			wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
			SendMessage(iSendFillDigit++);
		
			User::__DbgSetAllocFail(EFalse ,RAllocator::EFailNext, i);
		
			//we have to set time limitation here if leave occurs and the msg is not processed
			iSchedulerWait->Start(15000000);	
		
			marker = new TInt(0);
			delete marker;
		
			if(marker) //memory leak occurs in our code or in the ActiveScheduler
				{
				TRAPD(err, wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize));
				if(err!=KErrNone)
					{
					iSchedulerWait->Start(11000000);	
					wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
					}
				}
			else //memory leak does not occur in our code
				{
				wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
				User::LeaveIfError(iUnexpectedError);
				}
			CleanupStack::PopAndDestroy(wdpWatcher);
			}
		}
	
	//Error on starting listening for new messages and restoring the connection afterwards
	wdpWatcher = CLbsSuplWdpWatcher::NewLC(iWatcherLog);
	//The port is occupied here
	CLbsSuplWdpWatcher* wdpWatcher2 = CLbsSuplWdpWatcher::NewLC(iWatcherLog);
	CleanupStack::Pop(wdpWatcher2);
	CleanupStack::PopAndDestroy(wdpWatcher);
	CleanupStack::PushL(wdpWatcher2);
	wdpWatcher2->CheckStateL(CLbsSuplWdpWatcher::EDisconnected);
	wdpWatcher2->Cancel();  //This is added to test CLbsSuplWdpWatcher::DoCancel method
	iSchedulerWait->Start(5000000);
	wdpWatcher2->CheckStateL(CLbsSuplWdpWatcher::EDisconnected);
	iSchedulerWait->Start(10000000);
	wdpWatcher2->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	SendMessage(iSendFillDigit++);
	iSchedulerWait->Start();
	User::LeaveIfError(iUnexpectedError);
	wdpWatcher2->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	CleanupStack::PopAndDestroy(wdpWatcher2);
	
	
	//Sync error while receiving message size
	RTe_ErrorPattern::AppendErrorL(KModuleUid, ECWapBoundDatagramService_AwaitRecvDataSize, 
			1, KErrGeneral, EFalse);
	wdpWatcher = CLbsSuplWdpWatcher::NewLC(iWatcherLog);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EDisconnected);
	iSchedulerWait->Start(5000000);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EDisconnected);
	iSchedulerWait->Start(10000000);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	SendMessage(iSendFillDigit++);
	iSchedulerWait->Start();
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	User::LeaveIfError(iUnexpectedError);
	CleanupStack::PopAndDestroy(wdpWatcher);
	RTe_ErrorPattern::Reset();
	
	//Sync error while receiving message size - on the second message
	RTe_ErrorPattern::AppendErrorL(KModuleUid, ECWapBoundDatagramService_AwaitRecvDataSize, 
			2, KErrGeneral, EFalse);
	wdpWatcher = CLbsSuplWdpWatcher::NewLC(iWatcherLog);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	SendMessage(iSendFillDigit++);
	iSchedulerWait->Start();
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EDisconnected);
	User::LeaveIfError(iUnexpectedError);
	iSchedulerWait->Start(5000000);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EDisconnected);
	iSchedulerWait->Start(10000000);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	SendMessage(iSendFillDigit++);
	iSchedulerWait->Start();
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	User::LeaveIfError(iUnexpectedError);
	CleanupStack::PopAndDestroy(wdpWatcher);
	RTe_ErrorPattern::Reset();
	
	
	//Async error while receiving message size
	RTe_ErrorPattern::AppendAsyncErrorL(KModuleUid, ECWapBoundDatagramService_AwaitRecvDataSize, 
			1, KErrGeneral, EFalse);
	wdpWatcher = CLbsSuplWdpWatcher::NewLC(iWatcherLog);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	iSchedulerWait->Start(5000000);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EDisconnected);
	iSchedulerWait->Start(10000000);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	SendMessage(iSendFillDigit++);
	iSchedulerWait->Start();
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	User::LeaveIfError(iUnexpectedError);
	CleanupStack::PopAndDestroy(wdpWatcher);
	RTe_ErrorPattern::Reset();
	
	//Sync error while receiving message 
	RTe_ErrorPattern::AppendErrorL(KModuleUid, ECWapBoundDatagramService_RecvFrom, 
			1, KErrGeneral, EFalse);
	wdpWatcher = CLbsSuplWdpWatcher::NewLC(iWatcherLog);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	SendMessage(iSendFillDigit++);
	iSchedulerWait->Start(5000000);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EDisconnected);
	iSchedulerWait->Start(10000000);
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	iRecvFillDigit=iSendFillDigit;
	SendMessage(iSendFillDigit++);
	iSchedulerWait->Start();
	wdpWatcher->CheckStateL(CLbsSuplWdpWatcher::EAwaitingDataSize);
	User::LeaveIfError(iUnexpectedError);
	CleanupStack::PopAndDestroy(wdpWatcher);
	RTe_ErrorPattern::Reset();
	
	}

/**
Overrides the pure virtual MTe_LbsSuplSmsTriggerSenderObserver::OnMessageSent. Receives the result
of the message sending.
	
@param aError [In] The error code or KErrNone if successful.

@see MTe_LbsSuplSmsTriggerSenderObserver::OnMessageSent
@see CTe_LbsSuplSmsTriggerSender::SendMessage
*/
void CTe_LbsSuplSmsTriggerIntegStep::OnMessageSent(TInt aError)
	{
	if(aError!=KErrNone)
		{
		iUnexpectedError = aError;
		iSchedulerWait->AsyncStop();
		return;
		}
	iMsgCount++;
	
	if(iMsgCount<iMsgMaxCount) //continue to send messages
		{
		SendMessage(iMsgCount);
		}
	else //create the receiver and start to process messages
		{
		if(iMsgMaxCount>1)
			{
			User::After(5000000);
			}
		iMsgCount = 0;
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
void CTe_LbsSuplSmsTriggerIntegStep::OnSuplInit(TLbsSuplPushChannel aChannel, 
		TLbsSuplPushRequestId /*aReqId*/, TDesC8& aMsg)
	{
	if(aChannel!=ELbsSuplPushChannelSMS || !CompareMessage(iRecvFillDigit++, aMsg))
		{
		if(aChannel!=ELbsSuplPushChannelSMS)
			{
			ERR_PRINTF1(_L("CTe_LbsSuplSmsTriggerIntegStep::OnSuplInit: aChannel!=ELbsSuplPushChannelSMS"));
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
void CTe_LbsSuplSmsTriggerIntegStep::OnSuplInitComplete(TLbsSuplPushChannel aChannel, 
		TLbsSuplPushRequestId /*aReqId*/, TInt /*aError*/, TInt /*aReserved*/)
	{
	
	if(aChannel!=ELbsSuplPushChannelSMS)
		{
		ERR_PRINTF1(_L("CTe_LbsSuplSmsTriggerIntegStep::OnSuplInit: aChannel!=ELbsSuplPushChannelSMS"));
		iUnexpectedError = KErrGeneral;;
		iSchedulerWait->AsyncStop();
		return;
		}
	
	iMsgCount++;
	
	if(iMsgCount==iMsgMaxCount)
		{
		iSchedulerWait->AsyncStop();
		}
	}

/**
Generates and sends a UDP message using the CTe_LbsSuplSmsTriggerSender object.

@param aNum [In] The number used to fill the message.

@see CTe_LbsSuplSmsTriggerIntegStep::OnMessageSent
@see CTe_LbsSuplSmsTriggerIntegStep::CompareMessage
*/
void CTe_LbsSuplSmsTriggerIntegStep::SendMessage(TUint aNum)
	{
	INFO_PRINTF2(_L("CTe_LbsSuplSmsTriggerIntegStep::SendMessage: msg %d"), aNum);
	aNum = aNum%10;
	iMessage.Zero();
	TInt maxLength = iMessage.MaxLength();
	for(TInt i=0;i<maxLength;i++)
		{
		iMessage.AppendNum(aNum);
		}
		
	iMessage[0] = 0;
	iMessage[1] = 0;

	// set the encoded length field
	iMessage[0] |= (TUint8)( maxLength >> 8 );
	iMessage[1] |= (TUint8)( maxLength );
		
	iSender->SendMessage(iMessage);
	}


/**
Generates a message using the pattern specified and compares it with the given message.

@param aNum [In] The number used to fill the generated message.

@return ETrue if the messages are identical, EFalse otherwise.

@see CTe_LbsSuplSmsTriggerIntegStep::OnSuplInit
@see CTe_LbsSuplSmsTriggerIntegStep::SendMessage
*/
TBool CTe_LbsSuplSmsTriggerIntegStep::CompareMessage(TUint aNum, TDesC8& aMsg)
	{
	aNum = aNum%10;
	iMessage.Zero();
	TInt maxLength = iMessage.MaxLength();
	for(TInt i=0;i<maxLength;i++)
		{
		iMessage.AppendNum(aNum);
		}
	
	iMessage[0] = 0;
	iMessage[1] = 0;

	// set the encoded length field
	iMessage[0] |= (TUint8)( maxLength >> 8 );
	iMessage[1] |= (TUint8)( maxLength );	
	
	if(aMsg.Compare(iMessage)==0)
		{
		return ETrue;
		}
	ERR_PRINTF1(_L("CTe_LbsSuplSmsTriggerIntegStep::CompareMessage: aMsg!=iMessage"));
	return EFalse;
	}


