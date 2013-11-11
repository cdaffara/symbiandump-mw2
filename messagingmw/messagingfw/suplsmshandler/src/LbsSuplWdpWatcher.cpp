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
// The main SUPL SMS Trigger Plug-in class. It implements the ECOM interface, listens for the incoming messages,
// and notifies the SPM via the SUPL Push API.
// 
//

/**
 @file
 @internalComponent
 @prototype
*/

#include <ecom/implementationproxy.h>
#include <watcher.h>
#include "lbsdevloggermacros.h"

#include "LbsSuplWdpWatcher.h"


/** The port on which the SMS messages arrives*/
const TInt KSuplSmsTriggerWdpPort = 7275;

/** The timeout in seconds after which the restart attempts are maid if the connection fails */
const TInt KRecoveryInterval = 10;

/** The maximal number of messages on reaching which the IP datagrams are not delivered to the Supl Push API any more*/
const TUint KMaxMsgLimitForIp = 10;

/** ECOM implementation table */
const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(0x10283757, CLbsSuplWdpWatcher::NewL)
	};

/** The only exported ECOM entry point function */
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}

/**
Static factory method for creating an instance of the CLbsSuplWdpWatcher class.

@param aWatcherParams [In] A pointer on the params the Watcher Framework passes when the object is created. 
A reference on the CWatcherLog is passed here.

@return An instance of the class. The calling application becomes the
owner of the returned instance and is responsible its disposal.

@leave If a error happens, it leaves with one of the system error codes.

*/
CLbsSuplWdpWatcher* CLbsSuplWdpWatcher::NewL(TAny* aWatcherParams)
	{
	TWatcherParams* params = reinterpret_cast<TWatcherParams*>(aWatcherParams);
	CLbsSuplWdpWatcher* self= new (ELeave) CLbsSuplWdpWatcher(CActive::EPriorityStandard, params->iLog);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

/**
Constructor.

@param aPriority [In] The active object's priority
@param aLog [In] A refernce on the Watcher logger class

@see CLbsSuplWdpWatcher::NewL
*/
CLbsSuplWdpWatcher::CLbsSuplWdpWatcher(TInt aPriority, CWatcherLog& aLog)
: CActive(aPriority), iState(EDisconnected), iDataSizePckg(iDataSize),
iLocalPort(KSuplSmsTriggerWdpPort), iLog(aLog)
	{
	CActiveScheduler::Add(this);
	}


/**
Destructor. Cancels the outstanding request for new messages and releases all resources.
*/
CLbsSuplWdpWatcher::~CLbsSuplWdpWatcher()
	{
	Cancel();
	
	iData.Close();
	
	delete iWdpConn;
	delete iSuplPush;
	delete iTimer;
	}

/**
2nd phase constructor. Creates and assigns all the required internal resources.

@leave If a error happens, it leaves with one of the system error codes.

@see CLbsSuplWdpWatcher::NewL
*/
void CLbsSuplWdpWatcher::ConstructL()
	{
	iLog.Printf(_L8("SuplWatcher : ConstructL()"));
	LBSLOG(ELogP2, "SuplWatcher : ConstructL()");
	iTimer = CLbsCallbackTimer::NewL(*this);
	
	iSuplPush = CLbsSuplPush::NewL(ELbsSuplPushChannelSMS, *this);
	
	Restart();
	}

/**
Handles the completition of the AwaitRecvDataSize and RecvFrom asynchronous calls.

@leave If a error happens, it leaves with one of the system error codes.

@see CActive::RunL
@see CWapBoundDatagramService::AwaitRecvDataSize
@see CWapBoundDatagramService::RecvFrom
*/
void CLbsSuplWdpWatcher::RunL()
	{
	__ASSERT_DEBUG(iState==EAwaitingDataSize || iState==EAwaitingData, User::Invariant());
	
	if(iStatus!=KErrNone)
		{
		ScheduleRestart(iStatus.Int());
		return;
		}
	
	if(iState==EAwaitingDataSize)
		{
		LBSLOG2(ELogP2,"SuplWatcher : Message size received %d", iDataSize);
		iData.CreateL(iDataSize);
				
		LBSLOG(ELogP2,"SuplWatcher : Receiving message...");
		TInt err = iWdpConn->RecvFrom(iRemoteHost, iRemotePort, iData, iTruncated, iStatus, 0);
		if(err==KErrNone)
			{
			iState = EAwaitingData;
			SetActive();
			}
		else
			{
			ScheduleRestart(err);
			}
		}
	else //iState==EAwaitingData
		{
		TInt err = KErrNone;
						
		Wap::TBearer bearer;
		err = iWdpConn->GetBearer(bearer);
		//If there are KMaxMsgLimitForIp or more messages in the queue, we ignore IP datagrams for security reasons
		if(err!=KErrNone || (bearer==Wap::EIP && iPendingMsg>=KMaxMsgLimitForIp)) 
			{
			return;
			}
	
		
		TLbsSuplPushRequestId reqId;
		err=iSuplPush->SuplInit(reqId, iData, 0);
		iData.Close();
		iLog.Printf(_L8("SuplWatcher : Message %d received, resending it to SPM"), reqId);
		LBSLOG2(ELogP3,"SuplWatcher : Message %d received, resending it to SPM", reqId);
		if(err!=KErrNone)
			{
			iLog.Printf(_L8("SuplWatcher : Message %d delivery failed, error %d"), reqId, err);
			LBSLOG3(ELogP3,"SuplWatcher : Message %d delivery failed, error %d", reqId, err);
			}
		else
			{
			iPendingMsg++;
			}

		LBSLOG(ELogP2,"SuplWatcher : Awaiting next message size...");
		err = iWdpConn->AwaitRecvDataSize(iDataSizePckg, iStatus);
		if(err==KErrNone)
			{
			iState = EAwaitingDataSize;
			SetActive();
			}
		else
			{
			ScheduleRestart(iStatus.Int());
			}
		}
	}

/**
Called when CActive::Cancel method is called. The Cancel method is not supposed to be called by the Watcher 
Framework directly.

@see CActive::DoCancel
@see CLbsSuplWdpWatcher::~CLbsSuplWdpWatcher
*/
void CLbsSuplWdpWatcher::DoCancel()
	{
	//This method is not called by the Active Scheduler Framework if there is no request outstanding.
	//To have an outsatnding request, we must have non-zero iWdpConn. So, there is no check for null of 
	//iWdpConn here.
	__ASSERT_DEBUG(iWdpConn, User::Invariant());
	iWdpConn->CancelRecv();
	}

/**
Called if the CLbsSuplWdpWatcher::RunL leaves. Restarts the connection.

@return KErrNone to inform the ActiveScheduler Framework that the error was hndled by the object itself.

@see CActive::RunError
*/
TInt CLbsSuplWdpWatcher::RunError(TInt aError)
	{
	ScheduleRestart(aError);
	return KErrNone;
	}


/**
 	From MLbsSuplPushObserver.
	Receive notification that the SUPL Init message has been sent to the LBS sub-system.
	The call-back is invoked immediately after delivery of the SUPL INIT request and 
	does not provide any information about it's outcome, e.g. conflict control results,
	host validation results, connection results etc.
	
	@param aChannel  [In] The channel the call-back is related to.
	@param aReqId    [In] An Id of the request the call-back is related to.
	@param aError    [In] KErrNone if successful, KErrTimeout if it was not possible to deliver
	                      the request before the timeout period, KErrArgument if the structure 
	                      or content of the SUPL INIT message was incorrect. 
	                      Any system wide error code otherwise.
	@param aReserved [In] Reserved for future use.
	
	@see CLbsSuplPush::SuplInit
	@see MLbsSuplPushObserver::OnSuplInitComplete
*/
void CLbsSuplWdpWatcher::OnSuplInitComplete(TLbsSuplPushChannel /*aChannel*/, TLbsSuplPushRequestId aReqId, 
			TInt aError, TInt /*aReserved*/)
	{
	iPendingMsg--;
	
	if(aError==KErrNone)
		{
		iLog.Printf(_L8("SuplWatcher : Message %d delivered successfully"), aReqId);
		LBSLOG2(ELogP3,"SuplWatcher : Message %d delivered successfully", aReqId);
		}
	else
		{
		iLog.Printf(_L8("SuplWatcher : Message %d delivery failed, error %d"), aReqId, aError);
		LBSLOG3(ELogP3,"SuplWatcher : Message %d delivery failed, error %d", aReqId, aError);
		}
	}

/**
From MLbsSuplWdpConnTimerObserver. Receive notification when the restart event must happen.
	
@see MLbsSuplWdpConnTimerObserver::Restart
*/
void CLbsSuplWdpWatcher::Restart()
	{
	__ASSERT_DEBUG(iState==EDisconnected,  User::Invariant());
	
	LBSLOG(ELogP2,"SuplWatcher : Starting receiving WDP datagrams...");
	TRAPD(err,iWdpConn = CWapBoundDatagramService::NewL());
	
	if(err==KErrNone)
		{
		LBSLOG(ELogP2,"SuplWatcher : Openning connection...");
		//We connect it with Wap::EAll to receive all types of SMS
		//The IP bearer is filtered in the release version in the RunL function
		err = iWdpConn->Connect(Wap::EAll, iLocalPort);
		if(err==KErrNone)
			{
			LBSLOG(ELogP2,"SuplWatcher : Connected, awaiting message size...");
			err = iWdpConn->AwaitRecvDataSize(iDataSizePckg, iStatus);
			if(err==KErrNone)
				{
				iState = EAwaitingDataSize;
				SetActive();
				}
			}
		}
	
	if(err!=KErrNone)
		{
		ScheduleRestart(err);
		}
	}

/**
Schedules the next restart attempt.

@param aError [In] The error that causes the restart attempt 
*/
void CLbsSuplWdpWatcher::ScheduleRestart(TInt aError)
	{
	(void)aError;
	
	delete iWdpConn;
	iWdpConn = 0;
	iTimer->Cancel();
	iTimer->EventAfter(TTimeIntervalSeconds(KRecoveryInterval), 0);
	iData.Close();
	iState = EDisconnected;
	
	LBSLOG3(ELogP3,"SuplWatcher : Error %d, recovery attempt in %d sec", aError, KRecoveryInterval);
	}


/**
From MLbsCallbackTimerObserver. Receive notification when the restart event must happen.

@see CLbsSuplWdpWatcher::Restart
*/
void CLbsSuplWdpWatcher::OnTimerEventL(TInt /*aTimerId*/)
	{
	Restart();
	}

/**
From MLbsCallbackTimerObserver. Normally it is called when OnTimerEventL leaves. Our implementation 
of OnTimerEventL does not leave, so this function must not be called.

@see MLbsCallbackTimerObserver
@see CLbsCallbackTimer
*/
TInt CLbsSuplWdpWatcher::OnTimerError(TInt /*aTimerId*/, TInt /*aError*/)
	{
	__ASSERT_DEBUG(0, User::Invariant());
	return KErrNone;
	}

