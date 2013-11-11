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
//CWspPortWatcher
//

CWspPortWatcher::CWspPortWatcher(CWatcherLog& aWatcherLog, TInt aPriority, TUid aBioID, RFs& aFs, Wap::TPort aWapPort)
	: CWapPortWatcher(aWatcherLog, aPriority, aBioID, aFs, aWapPort),iPushPckgId(0)
	{
	}

CWspPortWatcher::~CWspPortWatcher()
	{
	Cancel();
	CloseConn();
	delete iWspConn;
	delete iRecvHeader;
	}

void CWspPortWatcher::CloseConn()
	{
	//Close the WSP connection
	delete iWspConn;
	iWspConn=NULL;

	delete iRecvHeader;
	iRecvHeader = NULL;

	CWapPortWatcher::CloseConn(); //deletes iRecvBuf
	}

void CWspPortWatcher::DoCancel()
	{
	if (iWspConn!=NULL)
		iWspConn->CancelAwaitPush();
	}

void CWspPortWatcher::DoSetupL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioWsp: DoSetupL: %S, port %d"), &iBioMsgText, iWapPort));

	//Open Connectionless WSP connection on port iWapPort
	if(iWspConn==NULL)
		iWspConn=CWapBoundCLPushService::NewL();
	User::LeaveIfError(iWspConn->Connect(Wap::ESMS,iWapPort,EFalse));
	}

void CWspPortWatcher::WaitForMessageL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioWsp: WaitForMessageL: %S, port %d"), &iBioMsgText, iWapPort));

	User::LeaveIfError(iStatus.Int());

	//Wait for WSP push message on port iWapPort	
	iTempRecvBuf.Zero();
	iTempRecvHeader.Zero();
	User::LeaveIfError(iWspConn->AwaitPush(iTempRecvHeader,iTempRecvBuf,iPushPckgId,iStatus));

	//SetActive() is called by CBaseSmsActiveSocketWatcher::DoRunL()
	}

void CWspPortWatcher::ReceiveL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioWsp: ReceiveL: %S, port %d"), &iBioMsgText, iWapPort));

	//Receive the WAP Message.

	switch (iStatus.Int())
		{
		case Wap::EMoreData:
			{
			//if iStatus == EMoreData, then must wait for the rest of the message
			AppendToHBufC8L(iRecvHeader, iTempRecvHeader);
			AppendToHBufC8L(iRecvBuf, iTempRecvBuf);

			TRequestStatus* pS=&iStatus;
			User::RequestComplete(pS, KErrNone);
			break;
			}
		case KErrNone:
			{
			AppendToHBufC8L(iRecvHeader, iTempRecvHeader);
			AppendToHBufC8L(iRecvBuf, iTempRecvBuf);
			
			// DEF053845 Provide a NULL pointer for GetServerAddress().
			// GetServerAddress() Allocs and sets up the pointer.
			HBufC8* serverAddress = NULL;
			// Pick up and interrogate the return rather than User::LeaveIfError.
			// Leaving here could result in us receiving the message again
			// and again.
			TInt err = iWspConn->GetServerAddress(serverAddress);
			if(err == KErrNone && serverAddress->Length())
				{
				CleanupStack::PushL(serverAddress);
				DoReceiveL(*serverAddress);
				CleanupStack::PopAndDestroy(serverAddress);
				}
			else
				{
				delete serverAddress; // Could have been alloc'd but it won't hurt to delete if NULL
				DoReceiveL(KNullDesC8);
				}							
			delete iRecvBuf;
			iRecvBuf = NULL;
			delete iRecvHeader;
			iRecvHeader = NULL;
			}
			break;
		default:
			User::Leave(iStatus.Int());
			break;
		}
	}
