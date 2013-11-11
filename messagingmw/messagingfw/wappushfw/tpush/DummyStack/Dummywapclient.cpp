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

#include <e32std.h>
#include "Dummywapstd.h"
#include "DummyWapClient.h"
#include "dummywapstack.h"



/** Bound Connectionless Push
*/
EXPORT_C CWapBoundCLPushService* CWapBoundCLPushService::NewL()
	{
	CWapBoundCLPushService* me = new (ELeave) CWapBoundCLPushService();
	return me;
	}

EXPORT_C CWapBoundCLPushService::~CWapBoundCLPushService()
	{
	if(iSessHandle&&iClConnHandle)
		{
		iSessHandle->Close();
		}
	STATIC_CAST(CDummyWapStack* , Dll::Tls())->CloseSession(iSessHandle);
	delete iClConnHandle;
 	}


TInt CWapBoundCLPushService::Connect(Wap::TBearer /*aBearer*/, Wap::TPort aPort, TBool /*aSecure*/)
	{
	TInt err = KErrNone;
	if (!iSessHandle)
		{
		TRAP(err, iSessHandle=  STATIC_CAST(CDummyWapStack* , Dll::Tls())->NewSessionL());
		}
	if (!iSessHandle)
		return KErrServerTerminated;

	TRAP(err,iClConnHandle =iSessHandle->NewWSPCLConnL(aPort));
	if(err)
		return err;
	return KErrNone; 
	}


/**
 * Return the "dummy" wap session associated with this client session
 */
CDummyWapSession* CWapBoundCLPushService::DummyWapSession()
	{
	return iSessHandle;
	}

TInt CWapBoundCLPushService::AwaitPush(TDes8& aPushHeaders, TDes8& aPushBody, TPckgBuf<TUint8>& aPushIdPckg, TRequestStatus& aReqStatus)
	{
	TPushID pushID;
	pushID() = aPushIdPckg();
	TRAPD(err,iClConnHandle->UnitWaitPushL(aPushBody,aPushHeaders,pushID, aReqStatus));
	return err;
	}

void CWapBoundCLPushService::CancelAwaitPush()
	{
	iClConnHandle->CancelUnitWaitPush();
	}

TInt CWapBoundCLPushService::GetServerAddressL(HBufC8*& aRemoteHost) 
	{
	__ASSERT_ALWAYS(aRemoteHost==NULL, User::Invariant());

	if((iClConnHandle->GetServerAddr()).Compare(_L8("")) == 0)
		{
		_LIT8(KRemoteAddress, "www.zappow.com");
		aRemoteHost = KRemoteAddress().AllocL();
		}
	else
		{
		aRemoteHost = iClConnHandle->GetServerAddr().AllocL();	
		}	
	return KErrNone;
	}

EXPORT_C CWapBoundCLPushService::CWapBoundCLPushService() 
	{
	}



/**
 * Constructor
 */
EXPORT_C RWAPServ::RWAPServ()
	{
	}

/**
 * Creates a session with the dummy wap stack
 * Get the pointer to the stack from TLS
 * @param TInt aDefaultMessageSlots Ignored in the dummy stack
 */
EXPORT_C TInt RWAPServ::Connect(TInt /*aDefaultMessageSlots*/)
	{
	TRAPD(err, iSessHandle=  STATIC_CAST(CDummyWapStack* , Dll::Tls())->NewSessionL());
	if (err)
		return err;

	if (!iSessHandle)
		return KErrServerTerminated;

	return KErrNone; 
	}
	
/**
 * Closes a previously created WAP session
 */
EXPORT_C void RWAPServ::Close()
	{
	STATIC_CAST(CDummyWapStack* , Dll::Tls())->CloseSession(iSessHandle);
	}

/**
 * Return the "dummy" wap session associated with this client session
 */
CDummyWapSession* RWAPServ::DummyWapSession()
	{
	return iSessHandle;
	}







/**
 * Closes an open connection
 */
EXPORT_C TInt RWAPConn::Close()
	{
	if (iDummyConn)
		iDummyConn->Close();
	return KErrNone;
	}

/**
 * Return the RWTLS object on which the WTLS functions are available
 */
EXPORT_C RWTLS RWAPConn::Wtls()
	{
	RWTLS wtls; 
	wtls.iDummyWTLS = iDummyConn->WtlsL();
	return wtls;
	}







/**
 * Set suggested security parameters before (re)negotiating with the server. 
 * These return ETrue on success otherwise EFalse.
 */
EXPORT_C TInt RWTLS::SetKeyExchangeSuite(TKeyExchangeSuite aSuite,TIdType aIdType,const TDesC8& aKeyId)
	{
	iDummyWTLS->SetKeyExchangeSuite(aSuite,aIdType,aKeyId);
	return ETrue;
	}

/**
 * Set suggested security parameters before (re)negotiating with the server. 
 * These return ETrue on success otherwise EFalse.
 */
EXPORT_C TInt RWTLS::SetCipherSuites(const CipherSuiteArray& aCipherSuites)
	{
	iDummyWTLS->SetCipherSuites(aCipherSuites);
	return ETrue;
	}

/**
 * Set suggested security parameters before (re)negotiating with the server. 
 * These return ETrue on success otherwise EFalse.
 */
EXPORT_C TInt RWTLS::SetSequenceNumberMode(TSequenceNumberMode aMode)
	{
	iDummyWTLS->SetSequenceNumberMode(aMode);
	return ETrue;
	}

/**
 * Set suggested security parameters before (re)negotiating with the server. 
 * These return ETrue on success otherwise EFalse.
 */
EXPORT_C TInt RWTLS::SetKeyRefreshRate(TUint8 aRate)
	{
	iDummyWTLS->SetKeyRefreshRate(aRate);
	return ETrue;
	}

/**
 * Set suggested security parameters before (re)negotiating with the server. 
 * These return ETrue on success otherwise EFalse.
 */
EXPORT_C TInt RWTLS::SetSharedSecret(const TDesC8& aSharedSecret)
	{
	iDummyWTLS->SetSharedSecret(aSharedSecret);
	return ETrue;
	}

/**
 * Set suggested security parameters before (re)negotiating with the server. 
 * These return ETrue on success otherwise EFalse.
 */
EXPORT_C TInt RWTLS::SetRecordLengthUsage(TBool aUsage)
	{
	iDummyWTLS->SetRecordLengthUsage(aUsage);
	return ETrue;
	}

/** 
 * Make the handshake with the server to agree on the security parameters.
 * The connection must be Opened (in secure mode) before this. 
 * Optionally the parameter setters may precede this call. 
 * If called more than once a renegotiation is done with possibly 
 * new parameters.
 * aTwoPhaseHandshake may be set to ETrue to return half way through to check
 * the server certificate. CancelConnect() or ConnectPhaseTwo() should succeed
 * this call if two phase connect is used.
 */
EXPORT_C void RWTLS::Connect(TRequestStatus& aStatus, TBool aTwoPhaseHandshake)
	{
	// assert that the connection has been opened
	iDummyWTLS->Connect(aStatus,aTwoPhaseHandshake);
	}

/**
 * Cancels the secure handshake. May be called after the first phase.
 */
EXPORT_C void RWTLS::CancelConnect()
	{
	iDummyWTLS->CancelConnect();
	}

/**
 * Continue the secure handshake after the first phase has completed.
 */
EXPORT_C TInt RWTLS::ConnectPhaseTwo(TRequestStatus& aStatus)
	{
	iDummyWTLS->ConnectPhaseTwo(aStatus);
	return KErrNone;
	}

/** 
 * Cancel second phase connection
 */
EXPORT_C void RWTLS::CancelConnectPhaseTwo()
	{
	iDummyWTLS->CancelConnectPhaseTwo();
	}

/**
 * Use the getters to see which values where agreed upon.
 */
EXPORT_C TInt RWTLS::GetCipherSuite(TCipherSuite& aCipherSuite) const
	{
	iDummyWTLS->GetCipherSuite(aCipherSuite);
	return KErrNone;
	}

/**
 * Use the getters to see which values where agreed upon.
 */
EXPORT_C TInt RWTLS::GetSequenceNumberMode(TSequenceNumberMode& aMode) const
	{
	iDummyWTLS->GetSequenceNumberMode(aMode);
	return KErrNone;
	}

/**
 * Use the getters to see which values where agreed upon.
 */
EXPORT_C TInt RWTLS::GetKeyRefreshRate(TUint8& aRate) const
	{
	iDummyWTLS->GetKeyRefreshRate(aRate);
	return KErrNone;
	}

/**
 * Use the getters to see which values where agreed upon.
 */
EXPORT_C RCertificate RWTLS::ServerCert()
	{
	RCertificate certificate = RCertificate();
	return certificate;
	}






/** 
 * Load a new certificate from persistent storage. Identified by subject.
 * NB: Not currently supported - returns KErrNotSupported
 */
EXPORT_C TInt RCertificate::Load(const TDesC8& /*aSubject*/)
	{
	return KErrNotSupported;
	}

/**
 * Load a new certificate from persistent storage. Identified by id.
 * NB: Not currently supported - returns KErrNotSupported
 */
EXPORT_C TInt RCertificate::LoadById(const TCertId& /*aId*/)
	{
	return KErrNotSupported;
	}

/** Delete certificate identified by subject from the certificate store */
EXPORT_C TInt RCertificate::Delete(const TDesC8& /*aSubject*/)
	{
	return KErrNotSupported;
	}

/** Save this cert to persistent storage */
EXPORT_C TInt RCertificate::Save(const TCertificate& /*aCertificate*/, TDes8& /*aSubject*/)
	{
	return KErrNotSupported;
	}

/**
 * Find subject of first cert in persistent storage. Use aId to find next.
 * NB: Not currently supported - returns KErrNotSupported
 */
EXPORT_C TInt RCertificate::FindFirst(TDes8& /*aSubject*/, TCertId& /*aId*/) const
	{
	return KErrNotSupported;
	}

/**
 * Find subject of next cert
 * NB: Not currently supported - returns KErrNotSupported
 */
EXPORT_C TInt RCertificate::FindNext(TDes8& /*aSubject*/, TCertId& /*aId*/) const
	{
	return KErrNotSupported;
	}

/**
 * Get the details of the server certificate of this secure connection OR the 
 * certificate explicitly loaded from persistent storage.
 */
EXPORT_C TInt RCertificate::Info(TInfo& /*aInfo*/) const
	{
	return KErrNotSupported;
	}







/**
 *  Creates a Push Connectionless type connection
 *  @param aServ Reference to RWAPServ object that Created the WAP Sesssion
 */
EXPORT_C TInt RWSPCLConn::Open(RWAPServ& aServer, TPort aLocalPort)
	{	
	TRAPD(err,iClConnHandle =aServer.DummyWapSession()->NewWSPCLConnL(aLocalPort));
	if(err)
		return err;
	iDummyConn=iClConnHandle;
	return KErrNone; 
	}

/**
 *  Creates a Push Connectionless type connection
 *  @param aServ Reference to RWAPServ object that Created the WAP Sesssion
 *  @param aRemotePort Port Number the Server is going to send on
 *  @param aLocalPort Local Port, if set to 0 it's allocated automatically
 *  @param aBearer enum, can be EIP, ESMS, EMSM7, EALL
 *  @param aSecureConn ETrue WTLS will be used, EFalse, it wont
 */
EXPORT_C TInt RWSPCLConn::Open(RWAPServ& aServer,
						const TDesC8& /*aRemoteHost*/,
						TPort /*aRemotePort*/,
						TPort aLocalPort,
						Wap::TBearer /*aBearer*/,
						TBool /*aSecureConn*/)
	{	
	TRAPD(err,iClConnHandle =aServer.DummyWapSession()->NewWSPCLConnL(aLocalPort));
	if(err)
		return err;
	iDummyConn=iClConnHandle;
	return KErrNone; 
	}


/**
 *  Wait for push from the server. Assumes a bound RWSPCLConn object.
 */
EXPORT_C TInt RWSPCLConn::AwaitPush(TDes8& aPushHeaders, TDes8& aPushBody, TUint& aPushId, TRequestStatus& aReqStatus)
	{
	TRAPD(err,iClConnHandle->UnitWaitPushL(aPushBody,aPushHeaders,(TPushID&)aPushId, aReqStatus));
	return err;
	}

/**
 *  Wait for push from the server. Assumes a bound RWSPCLConn object.
 */
EXPORT_C void RWSPCLConn::UnitWaitPush(TDes8& aBody,TDes8& aHeaders,TPushID& aID,TRequestStatus& aStatus)
	{
	TRAPD(err,iClConnHandle->UnitWaitPushL(aBody,aHeaders,aID, aStatus));
	if(err)
		User::Panic(_L("RWSPCLConn::UnitWaitPush() --NewWSPCLConnL --"),err);
	}

/**
 *  Cancel the asynchronous UnitWaitPush operation.
 */
EXPORT_C void RWSPCLConn::CancelAwaitPush()
	{
	iClConnHandle->CancelUnitWaitPush();
	}


	/**
 *  Cancel the asynchronous UnitWaitPush operation.
 */
EXPORT_C void RWSPCLConn::CancelUnitWaitPush()
	{
	iClConnHandle->CancelUnitWaitPush();
	}

/**
 *  The synchronous version
 */
EXPORT_C TInt RWSPCLConn::UnitWaitPush(TDes8& aBody,TDes8& aHeaders,TPushID& aID)
	{

	TRAPD(err,iClConnHandle->UnitWaitPushL(aBody, aHeaders, aID));
	if(err)
		return err;

	return KErrNone;
	}


//
// -- connection-orientated transaction --
//

/**
 * Constructor for connection-orientated transaction
 */
EXPORT_C RWSPCOTrans::RWSPCOTrans() : iCoTransHandle(0)
	{
	}

/**
 * Destructor for connection-orientated transaction
 */
EXPORT_C RWSPCOTrans::~RWSPCOTrans()
	{
	}

/**
 * Acknowledges a transaction. 
 * This must be used to accept a confirmed push from the server. 
 *
 */
EXPORT_C TInt RWSPCOTrans::Acknowledge(const TDesC8& aAckHeaders)
	{
	// if the handle to the server's transaction is NULL then
	// return RWSPCOConn::EInvalidTransaction;
	if (!iCoTransHandle)
		return RWSPCOConn::EInvalidTransaction;
	
	return iCoTransHandle->Acknowledge(aAckHeaders); 
	}

/**
 * Get the data for this transaction
 * If the allocated buffer is too small the function will return EMoreData and
 * the remaining data can be read with successive calls
 * 
 * @param aBuffer The user allocated buffer to be filled with the requested data (output)
 * @param aDataType The type of data to be returned (input)
 */
EXPORT_C TInt RWSPCOTrans::GetData(TDes8& aBuffer, TDataType aDataType, TInt* aSizeLeft) const
	{
	// if the handle to the server's transaction is NULL then
	// return RWSPCOConn::EInvalidTransaction;
	if (!iCoTransHandle)
		return RWSPCOConn::EInvalidTransaction;

	return iCoTransHandle->GetData(aBuffer,aDataType,aSizeLeft);
	};

/**
 * Retrieve the state the transaction is in
 * Can be one of EInit, ERequesting, EReplying, EDone, EAborted, EPushing
 */
EXPORT_C TInt RWSPCOTrans::GetState(TState& aState) const
	{
	// if the handle to the server's transaction is NULL then
	// return RWSPCOConn::EInvalidTransaction;
	if (!iCoTransHandle)
		return RWSPCOConn::EInvalidTransaction;

	return iCoTransHandle->GetState(aState);
	}

/**
 * Aborts a method invoke or push transaction 
 * State of the transaction can be EInit or ERequesting
 */
EXPORT_C TInt RWSPCOTrans::Abort(TAbortReason aReason)
	{
	// if the handle to the server's transaction is NULL then
	// return RWSPCOConn::EInvalidTransaction;
	if (!iCoTransHandle)
		return RWSPCOConn::EInvalidTransaction;
	//
	// assert that the state of the transaction is EInit or ERequesting
	return iCoTransHandle->Abort(aReason);
	}

/** 
 * Releases a transaction
 * Must be called after use. Causes the resources reserved by the transaction
 * to be released. Function succeeds only if the state of the transaction is 
 * EInit, EAborted or EDone. If the transaction is in any other state the 
 * transaction should be aborted first.
 */
EXPORT_C TInt RWSPCOTrans::Release()
	{
	// if the handle to the server's transaction is NULL then
	// return RWSPCOConn::EInvalidTransaction;
	if (!iCoTransHandle)
		return RWSPCOConn::EInvalidTransaction;
	//
	// assert that the transaction state is EInit, EAborted or EDone

	return iCoTransHandle->Release();
	}

EXPORT_C TInt RWSPCOTrans::Id(TTransID& aId)
	{
	// if the handle to the server's transaction is NULL then
	// return RWSPCOConn::EInvalidTransaction;
	if (!iCoTransHandle)
		return RWSPCOConn::EInvalidTransaction;
	// else return handle
	return iCoTransHandle->Id(aId);
	}




//
// -- connection-orientated connection --
//


/**
 * Fully defined open
 *
 */
EXPORT_C TInt RWSPCOConn::Open(RWAPServ& aServer, const TDesC8& /*aRemoteHost*/, TPort /*aRemotePort*/, TPort /*aLocalPort*/, 
							   TBearer /*aBearer*/, TBool /*aSecureConn*/)
	{
	TRAPD(err,iCoConnHandle=aServer.DummyWapSession()->NewWSPCOConnL());
	if(err)
		return err;
	iDummyConn=iCoConnHandle;
	return KErrNone;
	}

/**
 * Close CO connection
 */
EXPORT_C TInt RWSPCOConn::Close()
	{
	iDummyConn->Close();
	return KErrNone;
	}

/**
 * Establishes a WSP session with the server
 * Session creation status is retrieved by calls to GetEvent
 * @param aClientHeaders Session headers communicated to the server during 
 *                       session establishment
 * @param aCap           Proposed capabilities
 */
EXPORT_C TInt RWSPCOConn::Connect(const TDesC8& aClientHeaders, class CCapCodec* aCap)
	{
	return iCoConnHandle->Connect(aClientHeaders,aCap);
	}

/**
 * Disconnects the session and aborts all pending transactions
 */
EXPORT_C TInt RWSPCOConn::Disconnect()
	{
	return iCoConnHandle->Disconnect();
	}

/** 
 * Suspends a session. No incoming or outgoing activity is allowed within the 
 * session. All pending transactions of the session are aborted, events 
 * corresponding to aborted transactions are NOT delivered via WSP_GetEvent()
 */
EXPORT_C TInt RWSPCOConn::Suspend()
	{
	return KErrNotSupported;
	}

/**
 * Resumes a suspended session
 */
EXPORT_C TInt RWSPCOConn::Resume(RWSPCOConn& /*aBlankConn*/, const TDesC8& /*aHeaders*/)
	{
	return KErrNotSupported;
	}

/** 
 * Returns the state of the session
 */
EXPORT_C TInt RWSPCOConn::GetSessionState(TSessionState& aState) const
	{
	return iCoConnHandle->GetSessionState(aState);
	}

/**
 * Retrieves data associated with a session
 * If the allocated buffer is too small,the function will return EMoreData and
 * the remaining data can be read with successive calls.
 * @param aData The user allocated buffer to be filled with the requested data
 * @param aType The type of the requested data. 
 */
EXPORT_C TInt RWSPCOConn::GetSessionData(TDes8& /*aData*/, TSessionDataType /*aType*/) const
	{
	return KErrNone;
	}

/** 
 * Create a transaction
 * Creates and invokes a methodInvoke in the context of the session
 * @param aMethod  HTTP method. May be either of EGet, EOptions, EHead, EDelete, 
 *                 ETrace, EPost or EPut
 * @param aURI     Uniform Resource Identifier. Must not be empty
 * @param aHeaders Request headers. Content type must be the first header field 
 *                 if aBody is given. Can be empty
 * @param aBody    Data associated with the invoke. Semantically equivalent to 
 *                 HTTP entity body. If aMethod doesn't require a body. then
 *                 this must be empty
 * @param aTrans   The created transaction
 */
EXPORT_C TInt RWSPCOConn::CreateTransaction(TMethod /*aMethod*/, const TDesC& /*aURI*/, const TDesC8& /*aHeaders*/, const TDesC8& /*aBody*/, RWSPCOTrans& /*aTrans*/)
	{
	// assert that aURI isn't empty
	// assert if aMethod doesn't require a body - no body is provided

	return KErrNotSupported;
	}

/** 
 * Gets next session or transaction event on this session 
 * @param aEvent The event. Use the function operator to get the value of the event
 * @param aTrans The transaction, if aEvent is a transaction event
 * @param aStatus Indicates completion of this asynchronous request
 */
EXPORT_C void RWSPCOConn::GetEvent(TEvent& aEvent, RWSPCOTrans& aTrans, TRequestStatus& aStatus) const
	{
	iCoConnHandle->GetEvent(aEvent,aTrans,aStatus);
	}

/** 
 * Cancel the asynchronous GetEvent operation 
 */		
EXPORT_C void RWSPCOConn::CancelGetEvent()
	{
	iCoConnHandle->CancelGetEvent();
	}

/** 
 * Gets next session or transaction event on this session (synchronous version) 
 * This isn't implemented in the dummy stack
 * @param aEvent The event. Use the function operator to get the value of the event
 * @param aTrans The transaction, if aEvent is a transaction event
 */
EXPORT_C TInt RWSPCOConn::GetEvent(TEvent& /*aEvent*/, RWSPCOTrans& /*aTrans*/) const 
	{
	return KErrNotSupported;
	}
	
/** 
 * Return the number of events pending for the session 
 * @param aNrOfEvents The number of events pending 
 */
EXPORT_C TInt RWSPCOConn::GetNrOfEvents(TUint& aNrOfEvents) const
	{
	return iCoConnHandle->GetNrOfEvents(aNrOfEvents);
	}

