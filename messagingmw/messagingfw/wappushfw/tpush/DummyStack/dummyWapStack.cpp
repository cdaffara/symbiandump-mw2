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

//
#include "Dummywapstd.h"
#include "dummywapstack.h"
#include "dummycapcodec.h"
 
const TInt KWapStkArrayGranualarity=1;


/**
 * Factory construction method
 * Use this method to allocate and construct a new CDummyWapStack object
 */
 EXPORT_C CDummyWapStack* CDummyWapStack::NewL(MDummyWapStackObserver& aObserver)
	{
	CDummyWapStack* self=new (ELeave) CDummyWapStack(aObserver);
	CleanupStack::PushL(self);	
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	};

/**
 * Constructor
 */
CDummyWapStack::CDummyWapStack(MDummyWapStackObserver& aObserver) :iObserver(aObserver),iSessionsArray(KWapStkArrayGranualarity)
	{
	}

/**
 * 2nd phase constructor
 * If the server instance hasn't already been constructed, create one
 * and store the pointer in thread local storage
 */
void CDummyWapStack::ConstructL()
	{
	if(!iInstance)
		{
		User::LeaveIfError(Dll::SetTls(this));
		iInstance = StackInstance();
		}
	}

/**
 * Destructor
 */
CDummyWapStack::~CDummyWapStack()
	{
	FreeTls();
	iInstance =NULL;
	iObserver.DWSOServerShutDown();

	__ASSERT_DEBUG(iSessionsArray.Count()==0, User::Panic(_L("Session Not deleted  != 0"), 0));
	iSessionsArray.Close();
	}

/**
 * Removes the pointer stored in TLS and zero it
 */
void  CDummyWapStack::FreeTls()
	{
	Dll::FreeTls();
	Dll::SetTls(NULL);
	}

/**
 * Return the newly created session
 */
EXPORT_C CDummyWapSession* CDummyWapStack::NewSessionL()
	{
	CDummyWapSession* Session = CDummyWapSession::NewL();
	CleanupStack::PushL(Session);
	User::LeaveIfError(iSessionsArray.Append(Session));
	CleanupStack::Pop(); 	  
	return iSessionsArray[iSessionsArray.Count()-1];
	}

/**
 * Close the session with the server by removing it from the array 
 * of open sessions 
 * @param aDummyWapSession The session to be closed
 */
EXPORT_C void CDummyWapStack::CloseSession(CDummyWapSession* aDummyWapSession)
	{
	for(TInt loop=iSessionsArray.Count();--loop>=0;)
		{
		if (iSessionsArray[loop]==aDummyWapSession)
			{
			iSessionsArray.Remove(loop);
			delete aDummyWapSession;
			break;
			}
		}
	}

/**
 * Return a reference to the MDummyWapStackObserver 
 */
EXPORT_C MDummyWapStackObserver& CDummyWapStack::Observer()
	{
	return iObserver;
	}









/**
 *  Factory method to create new CDummyWapSession object
 */
CDummyWapSession* CDummyWapSession::NewL()
	{
	CDummyWapSession* self = new(ELeave) CDummyWapSession();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
	}

/**
 * 2nd phase constructor
 */
void CDummyWapSession::ConstructL()
	{
	}

/**
 * Destructor
 */
CDummyWapSession::~CDummyWapSession()
	{
	// check that all sessions have been closed
	__ASSERT_ALWAYS(iReferenceCount==0,User::Invariant());
	}

/**
 * Creates a new CL connection
 */
CDummyWSPCLConn*  CDummyWapSession::NewWSPCLConnL(TPort aPort)
	{
	CDummyWSPCLConn* dummyCL = new (ELeave) CDummyWSPCLConn(*this, aPort);
	iReferenceCount++;
	return dummyCL;
	}

/**
 * Creates a new CO connection
 */
CDummyWSPCOConn* CDummyWapSession::NewWSPCOConnL()
	{
	CDummyWSPCOConn* dummyCO = new (ELeave) CDummyWSPCOConn(*this);
	iReferenceCount++;
	return dummyCO;
	}

/**
 * Close a connection (subsession)
 */
void CDummyWapSession::Close()
	{
	// check that there is a session open to close
	__ASSERT_ALWAYS(iReferenceCount>0, User::Invariant());

	iReferenceCount--;
	}





/**
 * Constructor 
 * @param aSession Wap stack client session that created the connection
 */
CDummyWSPConn::CDummyWSPConn(CDummyWapSession& aSession) : iSession(aSession)
	{
	}


/**
 * Virtual close method for all "dummy" wap connections
 */
void CDummyWSPConn::Close()
	{
	iSession.Close();
	delete iWtls;
	delete this;
	}

/** 
 * Get a pointer to the WTLS implementation from the testcode
 */
CDummyWTLS* CDummyWSPConn::WtlsL()
	{
	if (!iWtls)
		{
		iWtls = new (ELeave) CDummyWTLS();
		}
	return iWtls;
	}










/**
 * simple constructor - call the base class
 * @param aSession Wap stack client session that created the connection
 */
CDummyWSPCLConn::CDummyWSPCLConn(CDummyWapSession& aSession,TPort aLocalPort) :
	CDummyWSPConn(aSession),iLocalPort(aLocalPort)
	{
	}

/**
 * Destructor
 */
CDummyWSPCLConn::~CDummyWSPCLConn()
	{
	delete iPushHeadersData;
	delete iPushBodyData;
	delete iServerAddr;
	}

TInt CDummyWSPCLConn::Connect(Wap::TBearer /*aBearer*/, Wap::TPort /*aPort*/, TBool /*aSecure*/)
	{
	TInt err = KErrNone;
	return err;
	}


TInt CDummyWSPCLConn::AwaitPush(TDes8& aPushHeaders, TDes8& aPushBody, TPckgBuf<TUint8>& aPushId, TRequestStatus& aReqStatus)
	{
	TPushID pushID;
	pushID() = aPushId();
	TRAPD(err, UnitWaitPushL(aPushBody, aPushHeaders, pushID, aReqStatus));
	return err;
	}


/**
 * 
 */
void CDummyWSPCLConn::UnitWaitPushL(TDes8& aBody,TDes8& aHeaders,TPushID& aID,TRequestStatus& aStatus)
	{
	__ASSERT_DEBUG(aBody.MaxSize()>0, User::Panic(_L("Msg Body Buffer Not Allocated"),0));
	__ASSERT_DEBUG(aHeaders.MaxSize()>0, User::Panic(_L("Msg Header Buffer Not Allocated"),0));
	
	aStatus=KRequestPending;
	//point to the Msg Buffers
	iClientBody=&aBody;
	iClientHeaders=&aHeaders;
	iID = &aID;
	iStatus=(&aStatus);

	//Initialise Buffers to zero Before Appending 
	iClientBody->Zero();
	iClientHeaders->Zero();
	*iID =0;

	//Invoke the dummy Observer UnitWaitPush
	STATIC_CAST(CDummyWapStack* , Dll::Tls())->Observer().DWSOUnitWaitPushL(*this);
	}

TInt CDummyWSPCLConn::CancelAwaitPush()
	{
	return CancelUnitWaitPush();
	}

/**
 * Cancel a unit wait push
 */
TInt CDummyWSPCLConn::CancelUnitWaitPush()
	{
 	if (iClientBody)
		iClientBody->Zero();
	if (iClientHeaders)
		iClientHeaders->Zero();
//	if (iID)
//		*iID =0;

	STATIC_CAST(CDummyWapStack* , Dll::Tls())->Observer().DWSOCancelUnitWaitPush(*this);
	
	User::RequestComplete(iStatus,KErrCancel);

	return KErrNone;
	}

/**
 * -Reads in the whole Msg. Header & Body From the WSS stack 
 * (i.e the test Harness) and Creates a Local Copy.
 * -Copy the Msg. Header & Body to Watcher Buffers:
 *		if the Watcher Buffers are big enough to hold the message,
 *		then the whole Mgs is copied and we Complete with KErrNone.
 *		if the Watcher Buffers are smaller then the Msg is copied 
 *      in blocks of size equal to the size of the Watcher Buffers
 *      completing with EMoreData every time until the watcher gets
 *		the whole Msg. At that point there is no more data to be 
 *		copied then we complete with KErrNone.
 */
EXPORT_C void CDummyWSPCLConn::CompleteUnitWaitPushL(TDes8& aBody,TDes8& aHeaders)
	{
	TPushID id;
	id=99;
	// Only read data from the WSS stack the first call
	if (iPushHeadersDataOffset == 0 && iPushBodyDataOffset == 0) 
		{
		__ASSERT_DEBUG(iPushHeadersData == 0, User::Panic(_L("headers != 0"), 0));
		__ASSERT_DEBUG(iPushBodyData == 0, User::Panic(_L("body != 0"), 0));
		
		TUint bodySize = aBody.Size();
		TUint headersSize =aHeaders.Size();		
		
		if (bodySize==0 && headersSize==0) 
			User::Leave(RWSPCLConn::EDataNotAvailable);
		
		iPushHeadersData= aHeaders.AllocL();
		iPushBodyData= aBody.AllocL();
		*iID=id;//**** Generate an Id per Msg, Not Supported in R1
		
		iPushHeadersData->Des().SetLength(headersSize);
		iPushBodyData->Des().SetLength(bodySize);
		}

	TInt clientHeadersSize = iClientHeaders->MaxSize();
	TInt clientBodySize = iClientBody->MaxSize();
	
	TInt headersRetSize = Min(clientHeadersSize, (iPushHeadersData->Size() - iPushHeadersDataOffset));
	TInt bodyRetSize = Min(clientBodySize, (iPushBodyData->Size() - iPushBodyDataOffset));

	iClientHeaders->Append(iPushHeadersData->Mid(iPushHeadersDataOffset, headersRetSize));
	iClientBody->Append(iPushBodyData->Mid(iPushBodyDataOffset, bodyRetSize));

	iPushHeadersDataOffset += headersRetSize;
	iPushBodyDataOffset += bodyRetSize;
	
	__ASSERT_DEBUG(iPushBodyDataOffset <= iPushBodyData->Size(), User::Panic(_L("Body offset too big"), 0));
	__ASSERT_DEBUG(iPushHeadersDataOffset <= iPushHeadersData->Size(), User::Panic(_L("Headers offset too big"), 0));
	
	// Are we done yet?
	if (iPushHeadersDataOffset == iPushHeadersData->Size() &&
		iPushBodyDataOffset == iPushBodyData->Size() )
		{
		delete iPushHeadersData;
		delete iPushBodyData;
		iPushHeadersData = 0;
		iPushBodyData = 0;
		iPushHeadersDataOffset = 0;
		iPushBodyDataOffset = 0;
		User::RequestComplete(iStatus,KErrNone);
		}
	else
		User::RequestComplete(iStatus,RWAPConn::EMoreData);
	}

EXPORT_C TBool CDummyWSPCLConn::HasMsgBeenCompleted()
	{
	return (iPushHeadersDataOffset == 0 && iPushBodyDataOffset == 0);
	}

/**
 * Sync Version
 */
TInt CDummyWSPCLConn::UnitWaitPushL(TDes8& /*aBody*/,TDes8& /*aHeaders*/,TPushID& /*aID*/)
	{
	return KErrNotSupported;
	}





/**
 * simple constructor - call the base class
 * @param aSession Wap stack client session that created the connection
 */
CDummyWSPCOConn::CDummyWSPCOConn(CDummyWapSession& aSession) : CDummyWSPConn(aSession), iSessionState(RWSPCOConn::EInit), iTransArray(1)
	{
	}

/**
 * Destructor
 */
CDummyWSPCOConn::~CDummyWSPCOConn()
	{
	// assert that the connection has been disconnected if it was connected?
	__ASSERT_ALWAYS(iCapCodec==NULL,User::Invariant());
	__ASSERT_ALWAYS(iTransArray.Count()==0, User::Panic(_L("Transaction not closed"), 0));
	iTransArray.Close();
	}

/**
 * Establish a connection-orientated WSP session with the server
 *
 * Takes ownership of the aCap parameter (the client's capabilities)
 * Informs the testcode (via the mixin) of the connect request so that
 * it can decide whether to confirm the connection. 
 */
TInt CDummyWSPCOConn::Connect(const TDesC8& aClientHeaders, class CCapCodec* /*aCap*/)
	{
	iClientHeaders=&aClientHeaders;
	iSessionState=RWSPCOConn::EConnecting;
	
	static_cast<CDummyWapStack*>(Dll::Tls())->Observer().DWSOConnect(*this);
	return KErrNone;
	}

/**
 * Disconnects the session and abort all pending transactions
 */
TInt CDummyWSPCOConn::Disconnect()
	{
	iSessionState=RWSPCOConn::EDisconnected;

	TInt numTrans=iTransArray.Count();
	while(numTrans>0)
		{
		CDummyWSPCOTrans* trans = iTransArray[0];
		trans->Abort(99); //TODO
		iTransArray.Remove(0);
		numTrans--;
		}

	return KErrNone;
	}

/**
 * Return the state of a session
 */
TInt CDummyWSPCOConn::GetSessionState(RWSPCOConn::TSessionState& aState)
	{
	aState=iSessionState;
	return KErrNone;
	}

/**
 * Gets the next session or transaction event on this session
 * 
 * Stores the parameters supplied by the client and awaits callback from 
 * the testcode
 */
TInt CDummyWSPCOConn::GetEvent(RWSPCOConn::TEvent& aEvent, RWSPCOTrans& aTrans, TRequestStatus& aStatus)
	{
	aStatus=KRequestPending;
	iStatus=&aStatus;
	iEvent=&aEvent;
	iTrans=&aTrans;

	//Invoke the dummy Observer GetEvent
	static_cast<CDummyWapStack*>(Dll::Tls())->Observer().DWSOGetEventL(*this);

	return KErrNone;
	}

/**
 * Cancel a get event request
 */
EXPORT_C void CDummyWSPCOConn::CancelGetEvent()
	{
	static_cast<CDummyWapStack*>(Dll::Tls())->Observer().DWSOCancelGetEvent(*this);
	if (iStatus)
		User::RequestComplete(iStatus,KErrCancel);
	}

/**
 * Return the number of events pending for the session
 */
TInt CDummyWSPCOConn::GetNrOfEvents(TUint& aNrOfEvents)
	{
	//ask the testcode?
	aNrOfEvents=0;
	return KErrNone;
	}

/**
 * Complete the GetEvent call
 * Called by the test harness to indicate that the transaction handle 
 * has been set. Adds the dummy transaction to the array of transactions
 * stored in the connection, then completes the asynchronous request.
 * 
 */
EXPORT_C void CDummyWSPCOConn::CompleteGetEventL(RWSPCOConn::TEvent& aEvent, CDummyWSPCOTrans& aDummyTrans)
	{
	*iEvent=aEvent;
	iTrans->iCoTransHandle=&aDummyTrans;
	User::LeaveIfError(iTransArray.Append(&aDummyTrans));
	//TInt count = iTransArray.Count(); //REMOVE THIS LINE!
	User::RequestComplete(iStatus,KErrNone); 
	}


/**
 * Remove a transaction pending on this connection. 
 * This should be called by the transaction if the transaction is aborted
 * or released. It will remove the transaction from the list of transactions
 */
void CDummyWSPCOConn::RemoveTransaction(CDummyWSPCOTrans& aTrans)
	{
	RWSPCOTrans::TTransID id;
	aTrans.Id(id);
	RWSPCOTrans::TTransID transId;
	TInt count = iTransArray.Count();
	

	TInt i=0;
	while(i<count)
		{
		CDummyWSPCOTrans* trans = iTransArray[i];
		trans->Id(transId);
		if (transId==id)
			{
			iTransArray.Remove(i);
			delete trans; 
			count--; 
			}
		else
			i++;
		}


	}





/**
 * Trivial constructor for dummy transaction
 *
 * @param aConnection reference to the CO connection that created this transaction
 */
EXPORT_C CDummyWSPCOTrans::CDummyWSPCOTrans(CDummyWSPCOConn& aConnection) : iConnection(aConnection), iTransState(RWSPCOTrans::EInit)
	{
	iDataArray.Reset();
	iOffsetArray.Reset();
	for (TInt i = 0; i < 11; i++)
		iOffsetArray[i] = new TInt(0);
	}

/**
 * destructor
 */
EXPORT_C CDummyWSPCOTrans::~CDummyWSPCOTrans()
	{
	// DeleteAll can only be used for CBase derived classes in array
	//iDataArray.DeleteAll();
	iOffsetArray.DeleteAll();
	}

/**
 * Acknowledge this transaction with the clients headers
 * Stores a pointer to the headers used by the client to acknowledge
 * this transaction, then informs the mixin so the testcode can 
 * check the headers
 * @param aAckHeaders the headers to be ack'd
 */
TInt CDummyWSPCOTrans::Acknowledge(const TDesC8& aAckHeaders)
	{
	iClientAckHeaders=&aAckHeaders;
	static_cast<CDummyWapStack*>(Dll::Tls())->Observer().DWSOAckTransaction(*this);
	return KErrNone;
	}

/** 
 * Get the data from the transaction
 * 
 * Return the minimum of client buffer size and amount left to read
 */
TInt CDummyWSPCOTrans::GetData(TDes8& aBuffer, RWSPCOTrans::TDataType aDataType, TInt* aSizeLeft) const
	{	
	__ASSERT_DEBUG(aBuffer.MaxSize()>0, User::Panic(_L("Client buffer not allocated"),0));

	//const TDesC8* requestedData=*iDataArray[aDataType];
	const TDesC8* requestedData= iDataArray[aDataType];
	TInt bufferSize = aBuffer.MaxSize();
	TInt reqSize = requestedData->Size();
	TInt* offset = iOffsetArray.At(aDataType);

	TInt retSize = Min(bufferSize, (reqSize - *offset));	

	aBuffer.Zero();
	aBuffer.Append(requestedData->Mid(*offset, retSize));
	*offset += retSize;

	if (*offset==reqSize)
		{
		*aSizeLeft = 0;
		*offset = 0;
		return KErrNone;
		}
	else
		{
		*aSizeLeft = reqSize-*offset;
		return RWAPConn::EMoreData;
		}
	}


/** 
 * Return the state of the transaction
 */
TInt CDummyWSPCOTrans::GetState(RWSPCOTrans::TState& aState) const
	{	
	aState=iTransState;
	return KErrNone;
	}

/** 
 * Abort the transaction
 */
TInt CDummyWSPCOTrans::Abort(RWSPCOTrans::TAbortReason /*aReason*/)
	{
	iTransState=RWSPCOTrans::EAborted;
	iConnection.RemoveTransaction(*this);
	// tell the testcode also?
	return KErrNone;
	}

/**
 * Release the transaction
 * Inform the connection that the transaction has been released so 
 * that it won't abort the transaction if the connection is 
 * disconnected - or complain that the transaction hasn't been 
 * released
 */
TInt CDummyWSPCOTrans::Release()
	{
	iConnection.RemoveTransaction(*this);
	// tell the testcode also?
	return KErrNone;
	}

/** 
 * return the ID of this transaction
 */
TInt CDummyWSPCOTrans::Id(RWSPCOTrans::TTransID& aId)
	{
	aId=iId;
	return KErrNone;
	}

/**
 * Set the data to be returned by the transaction when RWSPCOTrans::GetData() is called
 *
 * Raises a USER 133 panic if aDataType isn't inside the enumeration range
 *
 * @param aBuffer a reference to the 8-bit descriptor containing the data
 * @param aDataType the type of data being set
 *
 */
EXPORT_C void CDummyWSPCOTrans::SetData(const TDesC8& aBuffer,RWSPCOTrans::TDataType aDataType)
	{
	iDataArray[aDataType]=&aBuffer;
	}


/** 
 * Set the ID of this transaction
 * This should be called by the testcode 
 * @param aId The transaction ID
 */
EXPORT_C void CDummyWSPCOTrans::SetId(TInt aId)
	{
	iId=aId;
	}







/**
 * Constructor for dummy WTLS
 * This should be called by the testcode using the stack
 * CDummyWSPConn will ask the testcode for the dummy WTLS class to be used
 * and set the pointer in RWTLS to point to it.
 */
CDummyWTLS::CDummyWTLS() : iSequenceNumMode(RWTLS::EImplicit), iKeyRefreshRate(4)
	{
	}

CDummyWTLS::~CDummyWTLS()
	{
	}

/**
 * Adds a new key exchange suite to the list of allowed key exchange suites 
 * for the secure connection. Each time this function is called a new key
 * exchange suite is added to the list. The first key exchange suite is the
 * most preferred key exchange suite. 
 * 
 * The default list is {ERsa, ERsa768, ERsa512}
 */
TInt CDummyWTLS::SetKeyExchangeSuite(RWTLS::TKeyExchangeSuite /*aSuite*/,RWTLS::TIdType /*IdType*/,const TDesC8& /*aKeyId*/)
	{
	return KErrNone;
	}

/**
 * Sets the supported cipher suites for the connection
 */
TInt CDummyWTLS::SetCipherSuites(const RWTLS::CipherSuiteArray& /*aCipherSuites*/)
	{
	return KErrNone;
	}

/**
 * Set the proposed sequence number mode
 */
TInt CDummyWTLS::SetSequenceNumberMode(RWTLS::TSequenceNumberMode aMode)
	{
	iSequenceNumMode=aMode; 
	return KErrNone;
	}

/**
 * Set the proposed key refresh rate
 */
TInt CDummyWTLS::SetKeyRefreshRate(TUint8 aRate)
	{
	iKeyRefreshRate=aRate;
	return KErrNone;
	}

/** 
 * Set the shared secret
 */
TInt CDummyWTLS::SetSharedSecret(const TDesC8& /*aSharedSecret*/)
	{
	return KErrNone;
	}

/** 
 * Enables/disables record length usage
 * If the record length usage is set to ETrue, each msg sent to the server
 * will contain the msg length field. Record length usage should be turned
 * on if the bearer is not able to provide the size of the transport SDU
 */
TInt CDummyWTLS::SetRecordLengthUsage(TBool aUsage)
	{
	iRecordLenUsage=aUsage;
	return KErrNone;
	}

/** 
 * Make the handshake with the server to agree security parameters
 * If called more than once, a renegotiation is done
 * pre-condition: The connection must be opened in secure mode before this.
 */
void CDummyWTLS::Connect(TRequestStatus& aStatus, TBool aTwoPhaseHandshake)
	{
	aStatus=KRequestPending;
	iConnect=&aStatus;
	iTwoPhaseHandshake=aTwoPhaseHandshake;
	//TODO: We need to use the Dummy Observer instead 
	User::RequestComplete(iConnect, RWTLS::EHandshakeOk);
	}

/** 
 * Stop processing a secure handshake
 * This method can be used to abort a secure handshake if application 
 * validation of the certificate fails
 */
void CDummyWTLS::CancelConnect()
	{
	User::RequestComplete(iConnect,KErrCancel);
	}

/**
 * Continues the processing of a secure handshake
 */
TInt CDummyWTLS::ConnectPhaseTwo(TRequestStatus& aStatus)
	{
	aStatus=KRequestPending;
	iConnectPhaseTwo=&aStatus;
	return KErrNone;
	}

/** 
 * Cancel second phase of connection
 */
void CDummyWTLS::CancelConnectPhaseTwo()
	{
	User::RequestComplete(iConnectPhaseTwo,KErrCancel);
	}

/**
 * Get the current cipher suite
 *
 * The default is RC5+SHA_80,RC5+SHA_40,RC5+SHA,RC5_56+SHA,RC5_40+SHA_80,RC5_40+SHA
 */
TInt CDummyWTLS::GetCipherSuite(RWTLS::TCipherSuite& /*aCipherSuite*/) const
	{
	//
	// need to set-up array with the default and return it
	//
	return KErrNone;
	}

/** 
 * Get the currently used sequence number mode
 * Either of ENotUsed, EImplicit, EExplicit
 * 
 * If the sequence number mode isn't set the default is EImplicit
 */
TInt CDummyWTLS::GetSequenceNumberMode(RWTLS::TSequenceNumberMode& aMode) const
	{
	aMode=iSequenceNumMode;
	return KErrNone;
	}

/** 
 * Get the current key refresh rate
 * Rate (0-15). The default is 4.
 */
TInt CDummyWTLS::GetKeyRefreshRate(TUint8& aRate) const
	{
	aRate = iKeyRefreshRate;
	return KErrNone;
	}
/**
Set the server address to the one read from the ini file.
@param aAddr Descriptor that hold the address read in from the ini file
*/
EXPORT_C void CDummyWSPCLConn::SetServerAddressL(const TDesC8& aAddr)
	{
	delete iServerAddr;
	iServerAddr = aAddr.AllocL();
	}
	
const TDesC8& CDummyWSPCLConn::GetServerAddr() 
	{
	if(!(iServerAddr))
		{
		return KNullDesC8;
		}
	else
		{
		return (*(iServerAddr));	
		}
	}

