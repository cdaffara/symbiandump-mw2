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
// A test proxy class for the CWapBoundDatagramService class
// 
//

/**
 @file
 @internalComponent
 @prototype
*/

#include <wapmessage.h>

#include "Te_WapBoundDatagramService.h"
#include "Te_LbsSuplSmsTriggerCommon.h"
#include "Te_ErrorPattern.h"

CTe_WapBoundDatagramService* CTe_WapBoundDatagramService::iLastWdpConn = 0;

/**
 *  Returns a reference on the last created CTe_WapBoundDatagramService instance.
 * 
 *	@return A reference on the last created CTe_WapBoundDatagramService instance.
 */
CTe_WapBoundDatagramService& CTe_WapBoundDatagramService::WdpConn()
	{
	__ASSERT_ALWAYS(iLastWdpConn, User::Invariant());
	return *iLastWdpConn;
	}

/**
 * Static factory method for creating an instance of the CLbsSuplWdpWatcher class.
 * 
 * @return An instance of the class. The calling application becomes the owner of the returned instance
 *  and is responsible its disposal. 
 * 
 * @leave If a error happens, it leaves with one of the system error codes.
 */
CTe_WapBoundDatagramService* CTe_WapBoundDatagramService::NewL()
	{
	CTe_WapBoundDatagramService* self = new(ELeave) CTe_WapBoundDatagramService();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	iLastWdpConn = self;
	return self;
	}
/**
 * Destructor. Deletes the original CWapBoundDatagramService object.
 */
CTe_WapBoundDatagramService::~CTe_WapBoundDatagramService()
	{
	delete iWdpConn;
	}

/**
 * Private default constructor. 
 */
CTe_WapBoundDatagramService::CTe_WapBoundDatagramService()
	{
	//Intentionally left blank
	}

/**
 * 2nd phase constructor. Creates the instance of the CWapBoundDatagramService, which provides the main
 * functionality.
 * 
 * @leave If a error happens, it leaves with one of the system error codes.
 */
void CTe_WapBoundDatagramService::ConstructL()
	{
	iWdpConn = CWapBoundDatagramService::NewL();
	}

/**
 * Connects to the WAP stack, opening an endpoint that can be used to listen for 
 * subsequent incoming datagrams.
 * 
 * @param aBearer [In] The bearer to listen on. Use EAll to specify all bearers. 
 * @param aPort [In] The port to listen on. If set to 0, a local port will be chosen for the client's
 * first SendTo(). 
 * @param aInetAddr [In] The IP address of the network interface that should be used in a multihomed system.
 * 
 * @return KErrNone on successful completion, or one of the system error codes on failure.
 * 
 * @see CWapBoundDatagramService::Connect
 */
TInt CTe_WapBoundDatagramService::Connect(Wap::TBearer aBearer, Wap::TPort aPort)
	{
	return iWdpConn->Connect(aBearer, aPort);
	}


/**
 * Waits for a datagram to be received, and discovers the size of buffer required to retrieve it. 
 * Checks with the error pattern if a error must occur and if it so, simulates the error.
 * 
 * @param aDataSizePckg [In] On completion, the size of data received, in bytes.
 * @param aReqStatus	 [In] Asynchonrous status word, used to signal when a data size is known.
 * 
 * @return KErrNone on successful completion, or one of the system error codes on failure.
 * 
 * @see CWapBoundDatagramService::AwaitRecvDataSize
 */
TInt CTe_WapBoundDatagramService::AwaitRecvDataSize(TPckg< TUint16 > &aDataSizePckg, TRequestStatus &aReqStatus)
	{
	TBool async;
	TInt err = RTe_ErrorPattern::CheckAsyncError(KModuleUid, ECWapBoundDatagramService_AwaitRecvDataSize,
			async);
	
	if(err!=KErrNone)
		{
		if(async)
			{
			TRequestStatus* requestStatus = &aReqStatus;
			User::RequestComplete(requestStatus, err);
			return KErrNone;
			}
		else
			{
			return err;
			}
		}
	return iWdpConn->AwaitRecvDataSize(aDataSizePckg, aReqStatus);
	}


/**
 * Receives data on a bound port.Checks with the error pattern if a error must occur and if it so,
 * simulates the error.
 * 
 * @param aRemoteHost 	[Out] On completion, the bearer-dependent address of the remote host from which the data
 * was received. 
 * @param aRemotePort 	[Out] On completion, the port on the remote host from which the data was received.
 * @param aBuffer 		[Out] A client-allocated data buffer that on completion is filled with data received.
 * Data that overflows the buffer is discarded. 
 * @param aTruncated 	[Out] On completion, indicates whether the received datagram was truncated to fit 
 * in the client's supplied buffer.
 * @param aReqStatus 	[Out] Asynchronous status word, used to notify the client that a datagram was received. 
 * @param aTimeout 	[In]  An optional millisecond time-out which allows a timed read to be made. If no data
 * is received within the timeout period, the request completes with KErrTimedOut. If a value of 0 is
 * supplied, the timeout is infinite. 
 * 
 * @return KErrNone on successful completion, or one of the system error codes on failure.
 * 
 * @see CWapBoundDatagramService::RecvFrom
 */
TInt CTe_WapBoundDatagramService::RecvFrom(TDes8 &aRemoteHost, Wap::TPort &aRemotePort, TDes8 &aBuffer,
		TBool &aTruncated, TRequestStatus &aReqStatus, TUint32 aTimeout)
	{
	TBool async;
	TInt err = RTe_ErrorPattern::CheckAsyncError(KModuleUid, ECWapBoundDatagramService_RecvFrom,
			async);
	
	if(err!=KErrNone)
		{
		if(async)
			{
			TRequestStatus* requestStatus = &aReqStatus;
			User::RequestComplete(requestStatus, err);
			return KErrNone;
			}
		else
			{
			return err;
			}
		}
	return iWdpConn->RecvFrom(aRemoteHost, aRemotePort, aBuffer, aTruncated, aReqStatus, aTimeout);
	}


/**
 * Cancels a previously asynchronous RecvFrom() or AwaitRecvDataSize() request.
 * 
 * @see CWapBoundDatagramService::CancelRecv
 */
void CTe_WapBoundDatagramService::CancelRecv()
	{
	iWdpConn->CancelRecv();
	}

/**
 * Gets the bearer on which a received datagram arrived.
 * 
 * @param aBearer [Out] On return, the bearer.
 * 
 * @return KErrNone on successful completion, or one of the system error codes on failure.
 */
TInt CTe_WapBoundDatagramService::GetBearer(Wap::TBearer &aBearer)
	{
	return iWdpConn->GetBearer(aBearer);
	}



