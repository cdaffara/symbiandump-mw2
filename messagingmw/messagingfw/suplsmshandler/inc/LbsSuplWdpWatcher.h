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

#ifndef __LBSSUPLWDPWATCHER_H__
#define __LBSSUPLWDPWATCHER_H__

/**
@file
@internalComponent
@prototype
*/


#include <e32base.h>
#include <lbs/lbssuplpush.h>
#include <wapmessage.h>
#include "lbstimer.h"

/** 
A watcher interface conformant class which has one NewL export called by 
the watcher framework to create the watcher. 
*/
NONSHARABLE_CLASS(CLbsSuplWdpWatcher) : public CActive, public MLbsSuplPushObserver, 
public MLbsCallbackTimerObserver
	{
public:
	/**The states of the class*/ 
	enum TState
		{
		EDisconnected = 1,
		EAwaitingDataSize = 2,
		EAwaitingData = 4
		};
		
public:

#ifdef TE_UNITTESTMODE_ON
	static CLbsSuplWdpWatcher* NewLC(CTe_WatcherLog* aWatcherLog);
	void CheckStateL(TInt aPossibleStates);
#endif //TE_UNITTESTMODE_ON

	static CLbsSuplWdpWatcher* NewL(TAny* aWatcherParams);
	~CLbsSuplWdpWatcher();
	
	virtual void OnSuplInitComplete(TLbsSuplPushChannel aChannel, TLbsSuplPushRequestId aReqId, 
			TInt aError, TInt aReserved);	
	virtual void OnTimerEventL(TInt aTimerId);
	virtual TInt OnTimerError(TInt aTimerId, TInt aError);

	
	
private:
#ifdef TE_UNITTESTMODE_ON
protected:	
#endif	
	CLbsSuplWdpWatcher(TInt aPriority, CWatcherLog& aLog);
	
private:	
	void ConstructL();
	
	virtual void Restart();

	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	
	void ScheduleRestart(TInt aError);
	
private:
	/** The state of the object */
	TState iState;
	/** The WAP datagram conection*/
	CWapBoundDatagramService *iWdpConn;
	/** The size of the received data */
	TUint16 iDataSize;
	/** The packaging descriptor of the received data size*/
	TPckg<TUint16> iDataSizePckg;
	/** The remote host name */
	TBuf8<32> iRemoteHost;
	/** The local port */ 
	Wap::TPort iLocalPort;
	/** The remote port */
	Wap::TPort iRemotePort;
	/** The received data buffer */
	RBuf8	iData;
	/** The flag if the received data is truncated */
	TBool iTruncated;
	/** The watcher framework logger */
	CWatcherLog& iLog;
	/** The SUPL Push API */
	CLbsSuplPush* iSuplPush;
	/** The timer to generate the restart events */
	CLbsCallbackTimer* iTimer;
	/** Number of pending messages*/
	TUint iPendingMsg;
	
	}; 
	
#endif //__LBSSUPLWDPWATCHER_H__

