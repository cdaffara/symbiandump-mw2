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
// The main SUPL WAP Push Plug-in class. It implements the ECOM interface, listens for the incoming messages,
// and notifies the SPM via the SUPL Push API.
// 
//

/**
 @file
 @internalComponent
 @prototype
*/

#include <ecom/implementationproxy.h>

#include "LbsSuplWapPush.h"
#include "lbsdevloggermacros.h"


/** ECOM implementation table */
const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(0x10283758, CLbsSuplWapPush::NewL)
	};

/** The only exported ECOM entry point function */
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}

/**
Static factory method for creating an instance of the CLbsSuplWapPush class.

@return An instance of the class. The calling application becomes the
owner of the returned instance and is responsible its disposal.

@leave If a error happens, it leaves with one of the system error codes.
*/
CLbsSuplWapPush* CLbsSuplWapPush::NewL()
	{
	CLbsSuplWapPush* self= new (ELeave) CLbsSuplWapPush();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

/**
Constructor.

@see CLbsSuplWapPush::NewL
*/
CLbsSuplWapPush::CLbsSuplWapPush()
	{
	}


/**
Destructor. Deletes the SUPL Push API object.
*/
CLbsSuplWapPush::~CLbsSuplWapPush()
	{
	LBSLOG(ELogP2, "SUPL WAP Push : CLbsSuplWapPush::~CLbsSuplWapPush");
	delete iSuplPush;
	}

/**
2nd phase constructor. Creates the SUPL Push API object.

@leave If a error happens, it leaves with one of the system error codes.

@see CLbsSuplWapPush::NewL
*/
void CLbsSuplWapPush::ConstructL()
	{
	LBSLOG(ELogP2, "SUPL WAP Push : CLbsSuplWapPush::ConstructL");
	iSuplPush = CLbsSuplPush::NewL(ELbsSuplPushChannelWAP, *this);
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
void CLbsSuplWapPush::OnSuplInitComplete(TLbsSuplPushChannel /*aChannel*/, TLbsSuplPushRequestId aReqId, 
			TInt aError, TInt /*aReserved*/)
	{
	if(aError==KErrNone)
		{
		LBSLOG2(ELogP3,"SUPL WAP Push : Message delivered successfully, reqId=%d", aReqId);
		}
	else
		{
		LBSLOG3(ELogP3,"SUPL WAP Push : Message delivery failed, reqId=%d, err=%d", aReqId, aError);
		}
#ifndef TE_UNITTESTMODE_ON
	__ASSERT_DEBUG(iPluginKiller, User::Invariant());
	iPluginKiller->KillPushPlugin();
#endif //TE_UNITTESTMODE_ON
	}


/**
Handles a push message asynchronously. It is not used since Symbian OS 8.0 when the 
implementation of the WAP Stack was cut down and support for the connection-oriented 
WAP Push was stopped.

@param aPushMsg [In]  Push message. Ownership of the message is passed to the object.
@param aStatus  [Out] Asynchronous status word.

@leave If a error happens, it leaves with one of the system error codes.
*/
void CLbsSuplWapPush::HandleMessageL(CPushMessage* /*aPushMsg*/, TRequestStatus& /*aStatus*/)
	{
	//Intentionally left blank
	}

/**
Handles a push message synchronously. 

@param aPushMsg [In]  Push message. Ownership of the message is passed to the object.

@leave If a error happens, it leaves with one of the system error codes.
*/
void CLbsSuplWapPush::HandleMessageL(CPushMessage* aPushMsg)
	{
	LBSLOG(ELogP3, "SUPL WAP Push : CLbsSuplWapPush::HandleMessageL");
	if(aPushMsg)
		{
		TPtrC8 body;
		TBool hasBody=aPushMsg->GetMessageBody(body);
		if(hasBody)
			{
			TLbsSuplPushRequestId reqId;
			TInt err=iSuplPush->SuplInit(reqId, body, 0);
			if(KErrNone==err)
				{
				LBSLOG2(ELogP3,"SUPL WAP Push : Started to deliver the message, reqId=%d", reqId);
				delete aPushMsg;
				return;
				}
			else
				{
				LBSLOG2(ELogP3,"SUPL WAP Push : CLbsSuplPush::SuplInit failed, err=%d", err);
				}
			}
		else
			{
			LBSLOG(ELogP3, "SUPL WAP Push : Empty message body, the message is skipped");
			}
		delete aPushMsg;
		}
	else
		{
		LBSLOG(ELogP3, "SUPL WAP Push : Null message pointer, the message is skipped");
		}
#ifndef TE_UNITTESTMODE_ON	
	__ASSERT_DEBUG(iPluginKiller, User::Invariant());
	iPluginKiller->KillPushPlugin();
#endif //TE_UNITTESTMODE_ON	
	}


/**
Cancels an outstanding HandleMessageL call. It is not used since Symbian OS 8.0 when the 
implementation of the WAP Stack was cut down and support for the connection-oriented WAP Push 
was stopped.
*/
void CLbsSuplWapPush::CancelHandleMessage()
	{
	//Intentionally left blank
	}

/**
Reserved for future expansion.
*/
void CLbsSuplWapPush::CPushHandlerBase_Reserved1()
	{
	//Intentionally left blank
	}

/**
Reserved for future expansion.
*/
void CLbsSuplWapPush::CPushHandlerBase_Reserved2()
	{
	//Intentionally left blank
	}

/**
Called when CActive::Cancel method is called. It is not used in this implementation.

@see CActive::DoCancel
*/
void CLbsSuplWapPush::DoCancel()
	{
	//Intentionally left blank
	}

/**
Overrides pure virtual CActive::RunL. It is not used in this implementation.

@see CActive::RunL
*/
void CLbsSuplWapPush::RunL()
	{
	//Intentionally left blank
	}

