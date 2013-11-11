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
// Te_LbsSuplSmsTriggerSender.h
// A class sending SUPL INIT messages via UDP
// 
//

/**
 @file
 @internalComponent
 @prototype
*/

#include <e32base.h>
#include <in_sock.h>

#include "Te_LbsSuplSmsTriggerSender.h"

/**
Static factory method for creating an instance of the CTe_LbsSuplSmsTriggerSender class.

@param aPort	 [In] The port the message must be sent to.
@param aObserver [In] The observer to receive message delivery notifications.

@return An instance of the class. The calling application becomes the
owner of the returned instance and is responsible its disposal.

@leave If a error happens, it leaves with one of the system error codes.
*/
CTe_LbsSuplSmsTriggerSender* CTe_LbsSuplSmsTriggerSender::NewL(TInt aPort, MTe_LbsSuplSmsTriggerSenderObserver& aObserver)
	{
	CTe_LbsSuplSmsTriggerSender* self = new(ELeave) CTe_LbsSuplSmsTriggerSender(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aPort);
	CleanupStack::Pop(self);
	return self;
	}

/**
Static factory method for creating an instance of the CTe_LbsSuplSmsTriggerSender class and
leaving it on the cleanup stack.

@param aPort	 [In] The port messages must be sent to.
@param aObserver [In] The observer to receive message delivery notifications.

@return An instance of the class. The calling application becomes the
owner of the returned instance and is responsible its disposal.

@leave If a error happens, it leaves with one of the system error codes.
*/
CTe_LbsSuplSmsTriggerSender* CTe_LbsSuplSmsTriggerSender::NewLC(TInt aPort, MTe_LbsSuplSmsTriggerSenderObserver& aObserver)
	{
	CTe_LbsSuplSmsTriggerSender* self = new(ELeave) CTe_LbsSuplSmsTriggerSender(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aPort);
	return self;
	}

/**
Destructor. Cancels the async request and closes the sockets.
*/
CTe_LbsSuplSmsTriggerSender::~CTe_LbsSuplSmsTriggerSender()
	{
	Cancel();
	iSocket.Close();
	iSocketServ.Close();
	}

/**
Sends the given message via UDP on the port specified on the construction.

@param aMessage [In] A message to be sent.

@see MTe_LbsSuplSmsTriggerSenderObserver::OnMessageSent
*/
void CTe_LbsSuplSmsTriggerSender::SendMessage(const TDesC8& aMessage)
	{
	__ASSERT_ALWAYS(!IsActive(), User::Invariant());
	
	iSocket.SendTo(aMessage, iAddr, 0, iStatus);
	SetActive();
	}

/**
Constructor. Adds this active object to the active scheduler's list.
*/
CTe_LbsSuplSmsTriggerSender::CTe_LbsSuplSmsTriggerSender(MTe_LbsSuplSmsTriggerSenderObserver& aObserver) : CActive(EPriorityStandard), 
iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

/**
2nd phase constructor. Opens the socket and preinitializes the address.

@param aPort	 [In] The port messages must be sent to.

@leave If a error happens, it leaves with one of the system error codes.
*/
void CTe_LbsSuplSmsTriggerSender::ConstructL(TInt aPort)
	{
	// Open channel to Socket Server
	User::LeaveIfError(iSocketServ.Connect());
	
	//Open the socket to listen for messages
	User::LeaveIfError(iSocket.Open(iSocketServ, KAfInet, KSockDatagram, KProtocolInetUdp));
	
	iAddr.SetPort(aPort);
	iAddr.SetAddress(INET_ADDR(127,0,0,1));
	}

/**
Called when the message delivery report is received.

@see CActive::RunL
@see SendTo::SendTo
*/
void CTe_LbsSuplSmsTriggerSender::RunL()
	{
	iObserver.OnMessageSent(iStatus.Int());
	}

/**
Cancels the outstanding send async request.

@see CActive::DoCancel
@see SendTo::SendTo
*/
void CTe_LbsSuplSmsTriggerSender::DoCancel()
	{
	iSocket.CancelRecv();
	}

	
