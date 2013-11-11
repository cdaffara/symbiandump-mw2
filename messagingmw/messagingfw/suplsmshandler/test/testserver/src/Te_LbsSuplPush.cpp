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
// A test proxy class for the CTe_LbsSuplPush class
// 
//

/**
 @file
 @internalComponent
 @prototype
*/

#include "Te_LbsSuplPush.h"

MLbsSuplPushObserver* CTe_LbsSuplPush::iTestObserver;

/**
Static factory method for creating an instance of the CTe_LbsSuplPush class.

@param aChannel  [In] The channel to be used for message delivering.
@param aObserver [In] The observer to receive message delivery notifications.

@return An instance of the class. The calling application becomes the
owner of the returned instance and is responsible its disposal.

@leave If a error happens, it leaves with one of the system error codes.
*/
CTe_LbsSuplPush* CTe_LbsSuplPush::NewL(TLbsSuplPushChannel aChannel, MLbsSuplPushObserver& aObserver)
	{
	__ASSERT_ALWAYS(iTestObserver!=0, User::Invariant());
	
	CTe_LbsSuplPush* newObj = new (ELeave) CTe_LbsSuplPush(aObserver);
	CleanupStack::PushL(newObj);
	newObj->ConstructL(aChannel);
	CleanupStack::Pop(newObj);
	return newObj;
	}


/**
Constructor.

@param aObserver [In] The observer to receive message delivery notifications.
*/
CTe_LbsSuplPush::CTe_LbsSuplPush(MLbsSuplPushObserver& aObserver) : iObserver(aObserver)
	{
	//Intentionally left blank
	}

/**
Destructor. Deletes the original class object.
*/
CTe_LbsSuplPush::~CTe_LbsSuplPush()
	{
	delete iImpl;
	}

/**
2nd phase constructor. Creates the original class object that implements the main functionality.

@param aChannel  [In] The channel to be used for message delivering.

@leave If a error happens, it leaves with one of the system error codes.

@see CTe_LbsSuplPush::NewL
*/
void CTe_LbsSuplPush::ConstructL(TLbsSuplPushChannel aChannel)
	{
	//Please note that the object itself is passed here as the notification observer
	iImpl = CLbsSuplPush::NewL(aChannel, *this);
	}

/**
Wrapper over the CLbsSuplPush::SuplInit method.

@param aReqId [Out] Channel unique request Id.
@param aMsg   [In]  A buffer containing a SUPL INIT message.
@param aReserverd   [In]  Rserved for future use.
@return An error code related to the synchronous part of the request.

@see CLbsSuplPush::SuplInit
*/
TInt CTe_LbsSuplPush::SuplInit(TLbsSuplPushRequestId& aReqId, const TDesC8& aMsg,TInt /*aReserved*/)
	{
	return iImpl->SuplInit(aReqId, aMsg, 0);
	}

/**
Overrides pure virtual MLbsSuplPushObserver::OnSuplInitComplete. Notifies both the external static
observer (usually a test step) and the internal observer (usually the being tested class using the 
CLbsSuplPush class).
	
@param aChannel  [In] The channel the call-back is related to.
@param aReqId    [In] An Id of the request the call-back is related to.
@param aError    [In] KErrNone if successful, KErrTimeout if it was not possible to deliver
                      the request before the timeout period, KErrArgument if the structure 
                      or content of the SUPL INIT message was incorrect. 
                      Any system wide error code otherwise.
@param aReserved [In] Reserved for future use.
	
@see MLbsSuplPushObserver::OnSuplInitComplete
*/
void CTe_LbsSuplPush::OnSuplInitComplete(TLbsSuplPushChannel aChannel, TLbsSuplPushRequestId aReqId, 
		TInt aError, TInt aReserved)
	{
	iObserver.OnSuplInitComplete(aChannel, aReqId, aError, aReserved);
	iTestObserver->OnSuplInitComplete(aChannel, aReqId, aError, aReserved);
	}
