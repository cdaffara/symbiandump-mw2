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

//
// CWapPortWatcher
//

//
// Global Defines
//
const TInt KSmsWRetryTimer = 1000000 * 15; // 15 Secs

//
// Construction
//	private
//
CWapPortWatcher::CWapPortWatcher(CWatcherLog& aWatcherLog, TInt aPriority, TUid aBioID, RFs& aFs, TUint16 aWapPort)
:	CBaseSmsActiveSocketWatcher(aWatcherLog, aPriority, aBioID, aFs), iWapPort(aWapPort)
	{
	}

CWapPortWatcher* CWapPortWatcher::NewL(TMsvId aBioServiceId, 
									   TMsvId aSmsServiceId, 
									   CBIODatabase& aBioDb, 
									   CWatcherLog& aWatcherLog, 
									   TInt aPriority, 
									   TUid aBioID, 
									   RFs& aFs, 
									   const TBioMsgId& aBioMsg)
	{
	CWapPortWatcher* self = NULL;

	//Create either a WDP or WSP watcher, depending on the BioMsgId type
	switch (aBioMsg.iType)
		{
		case EBioMsgIdNbs:
		case EBioMsgIdWap:
		case EBioMsgIdWapSecure:
			{
			self = new (ELeave) CWdpPortWatcher(aWatcherLog, aPriority, aBioID, aFs, STATIC_CAST(Wap::TPort,aBioMsg.iPort));
			break;
			}
		case EBioMsgIdWsp:
		case EBioMsgIdWspSecure:
			{
			self = new (ELeave) CWspPortWatcher(aWatcherLog, aPriority, aBioID, aFs, STATIC_CAST(Wap::TPort,aBioMsg.iPort));
			break;
			}
		default:
			User::Leave(KErrNotSupported);
		}

	CleanupStack::PushL(self);
	self->ConstructL(aBioDb, aBioServiceId, aSmsServiceId);
	CleanupStack::Pop(self);
	return self;
	}

CWapPortWatcher::~CWapPortWatcher()
	{
	delete iRecvBuf;
	iRecvBuf = NULL;
	}

void CWapPortWatcher::DoComplete(TInt& rStatus)
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioWap: DoComplete: %S, port %d, status %d"), &iBioMsgText, iWapPort, rStatus));

	CloseConn();
	}

void CWapPortWatcher::DoReceiveL(const TDesC8& aRecvFrom)
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioWap: DoReceiveL: %S, port %d"),&iBioMsgText, iWapPort));
	
	// Code for collecting message goes here...
	BIOWATCHERLOG(iWatcherLog.Printf(_L8("BioWap: Recv datagram length: %d on socket OK"), iRecvBuf->Length()));
	
	// Create a CSmsMessage from the recieved buffer
	CSmsBuffer* buffer=CSmsBuffer::NewL();
	
	
	CSmsMessage* smsmessage = CSmsMessage::NewL(iFs, CSmsPDU::ESmsDeliver, buffer);
	CleanupStack::PushL(smsmessage);

	smsmessage->SmsPDU().SetAlphabet(TSmsDataCodingScheme::ESmsAlphabet8Bit);

	if (aRecvFrom.Length())
		{
		//Convert aRecvFrom to 16-bit
		HBufC* tempRemoteAddr = HBufC::NewLC(aRecvFrom.Length());
		tempRemoteAddr->Des().Copy(aRecvFrom);

		//Set the from address in the CSmsMessage
		smsmessage->SmsPDU().SetToFromAddressL(*tempRemoteAddr);
		CleanupStack::PopAndDestroy(tempRemoteAddr);
		}
	
	//Convert iRecvTotalMessage to 16-bit
	HBufC* tempRecvBuf = HBufC::NewLC(iRecvBuf->Length());
	tempRecvBuf->Des().Copy(*iRecvBuf);
	buffer->InsertL(0, *tempRecvBuf);
	CleanupStack::PopAndDestroy(tempRecvBuf);

	//Pop buffer and smsmessage off the cleanup stack,
	//because StoreMsgL() will destroy the CSmsMessage (which contains the buffer).
	CleanupStack::Pop();// smsmessag

	StoreMsgL(smsmessage);
	
	// Not Async so signal here...
	TRequestStatus* pS=&iStatus;
	User::RequestComplete(pS, KErrNone);
	}

void CWapPortWatcher::AppendToHBufC8L(HBufC8*& aBuf, const TDesC8& aAppend)
	{
	//Appends aAppend to aBuf. If aBuf is NULL, then copies aAppend into aBuf.

	if (!aBuf || aBuf->Length() == 0)
		{
		delete aBuf;
		aBuf = NULL;
		aBuf = aAppend.AllocL();
		}
	else
		{
		//Append the new message in iWspRecvBuf to the iTempRecvBuf
		aBuf = aBuf->ReAllocL(aBuf->Length() + aAppend.Length());
		aBuf->Des().Append(aAppend);
		}
	}

void CWapPortWatcher::CloseConn()
	{
	delete iRecvBuf;
	iRecvBuf = NULL;
	}

void CWapPortWatcher::DoRunL()
	{
	switch(iState)
		{
		case ESmsWWaitForMsg:
			iState = ESmsWReadMsg;
			WaitForMessageL();
			break;
		
		case ESmsWReadMsg:
			iState = ESmsWWaitForMsg;
			ReceiveL();
			break;

		case ESmsWRetryError:
			if (!iTimer.Handle())
				{
				User::LeaveIfError(iTimer.CreateLocal());
				}
			iState=ESmsWWaitForMsg;
			iTimer.After(iStatus, KSmsWRetryTimer);
			break;
		
		case ESmsIsClass0Msg:
			iState = ESmsWWaitForMsg;
			CompleteSelf();
			break;
		}

	SetActive();

	}

// WAP messages that use Class 0 SMS messages should be treated as non Class 0 messages,
// so need to go back to ESmsWWaitForMsg state.
void CWapPortWatcher::CompleteSelf()
	{
	// Complete self.
	TRequestStatus* status = &iStatus;
	iStatus = KRequestPending;
	User::RequestComplete(status, KErrNone);
	}
