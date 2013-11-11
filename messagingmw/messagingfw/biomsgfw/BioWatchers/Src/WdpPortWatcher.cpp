// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "WapSocketWatcher.h"
#include <watcher.h>

#include <wapmessage.h>
#include <wapmsgerr.h>

//
//CWdpPortWatcher
//

CWdpPortWatcher::CWdpPortWatcher(CWatcherLog& aWatcherLog, TInt aPriority, TUid aBioID, RFs& aFs, TUint16 aWapPort)
	: CWapPortWatcher(aWatcherLog, aPriority, aBioID, aFs, aWapPort), iRecvPtr(0,0), iDataSize(0), iDataSizePckg(iDataSize)
	{
	}

CWdpPortWatcher::~CWdpPortWatcher()
	{
	Cancel();
	CloseConn();
	}

void CWdpPortWatcher::CloseConn()
	{
	//Close the RWDPConn connection
	delete iWdpConn;
	iWdpConn=NULL;
	iRecvNext = ERecvSize;

	CWapPortWatcher::CloseConn(); //deletes iRecvBuf
	}

void CWdpPortWatcher::DoCancel()
	{
	//Cancel outstanding requests
	if (iWdpConn!=NULL)
		iWdpConn->CancelRecv();

	Complete(KErrCancel);
	}

void CWdpPortWatcher::DoSetupL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioWdp: DoSetupL: %S, port %d"), &iBioMsgText, iWapPort));

	//Open the RWDPConn connection on port iWapPort
	iWdpConn = CWapBoundDatagramService::NewL();
	User::LeaveIfError(iWdpConn->Connect(Wap::ESMS,iWapPort));
	iRecvNext = ERecvSize;
	}

void CWdpPortWatcher::WaitForMessageL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioWdp: WaitForMessageL: %S, port %d"), &iBioMsgText, iWapPort));

	User::LeaveIfError(iStatus.Int());

	//Wait to receive a WDP message on port iWapPort
	if (iRecvNext == ERecvSize)
		{
		User::LeaveIfError(iWdpConn->AwaitRecvDataSize(iDataSizePckg, iStatus));
		}
	else //iRecvNext == ERecv
		{
		iRecvPtr.Set(iRecvBuf->Des());
		iRecvPtr.Zero();
		iRemoteAddress.Zero();
		// 0 is an infinate time out, as we just got the recieved size we know a message is waiting
		User::LeaveIfError(iWdpConn->RecvFrom(iRemoteAddress,iRemotePort,iRecvPtr,iTruncated,iStatus,0));
		}

	//SetActive() is called by CBaseSmsActiveSocketWatcher::DoRunL()
	}

void CWdpPortWatcher::ReceiveL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioWdp: ReceiveL: %S, port %d"), &iBioMsgText, iWapPort));

	//Receive the WAP Message.
	User::LeaveIfError(iStatus.Int());
	if(iTruncated) // should never happen as we ask for the size and allocate the required buffer
		User::Leave(KErrOverflow);

	if (iRecvNext == ERecvSize)
		{
		// iRecvBuf should not currently exist because it is deleted after a message is received (below)
		// or if there is an error (it is deleted by CloseConn())
		__ASSERT_DEBUG(iRecvBuf == NULL, PanicWatcher(EBufferNotNull));
		iRecvBuf = HBufC8::NewL(iDataSizePckg());
		iRecvNext = ERecv;

		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
		}
	else //iRecvNext == ERecv
		{
		DoReceiveL(iRemoteAddress);
		delete iRecvBuf; // buffer is no longer required.
		iRecvBuf = NULL;
		iRecvNext = ERecvSize;
		}
	}
