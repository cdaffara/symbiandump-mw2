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
// A class sending SUPL INIT messages via UDP
// 
//

#ifndef __TE_LBSSUPLSMSTRIGGERSENDER_H__
#define __TE_LBSSUPLSMSTRIGGERSENDER_H__

/**
@file
@internalComponent
@prototype
*/

#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>


/** 
An observer for getting notifications about the UDP messages sent.
 */
class MTe_LbsSuplSmsTriggerSenderObserver
	{
public:	
	/**
	Reports the result of the message sending.
	
	@param aError [In] The error code or KErrNone if successful.
	*/
	virtual void OnMessageSent(TInt aError) = 0;
	};


/** 
An utility class for sending UDP messages on the specified port and reporting about 
sent messages to the observer.
*/
class CTe_LbsSuplSmsTriggerSender : public CActive
	{	
public:
	static CTe_LbsSuplSmsTriggerSender* NewL(TInt aPort, MTe_LbsSuplSmsTriggerSenderObserver& aObserver);
	static CTe_LbsSuplSmsTriggerSender* NewLC(TInt aPort, MTe_LbsSuplSmsTriggerSenderObserver& aObserver);
	
	virtual ~CTe_LbsSuplSmsTriggerSender();
	
	void SendMessage(const TDesC8& aMessage);
	
private:
	CTe_LbsSuplSmsTriggerSender(MTe_LbsSuplSmsTriggerSenderObserver& aObserver);
	void ConstructL(TInt aPort);
	
	virtual void RunL();
	virtual void DoCancel();
	
private:
	MTe_LbsSuplSmsTriggerSenderObserver& iObserver;
	RSocket iSocket;
	RSocketServ iSocketServ;
	TInetAddr iAddr;
	}; 

#endif //__TE_LBSSUPLSMSTRIGGERSENDER_H__
	
	
