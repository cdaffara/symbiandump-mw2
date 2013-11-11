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


#ifndef __DUMMYWAPCLIENT_H__
#define __DUMMYWAPCLIENT_H__

#include <e32base.h>
#include <in_sock.h>

enum {KDefaultMessageSlots = 10};
/**
 * forward declaration
 */
class CDummyWapSession;
class CDummyWSPCLConn;
class CDummyWSPCOConn;
class CDummyWSPConn;
class CDummyWSPCOTrans;
class CDummyWTLS;

namespace Wap
	{
	// Bearer type definition
	typedef enum
		{
		EAll,
		EIP,
		ESMS7,
		ESMS,
		EWAPSMS7,
		EWAPSMS
		} TBearer;

	// Port number definition
	typedef TUint16 TPort;
 
	class TAddressInfo
		{
	public:
		TName iName;			// Interface name
		TInetAddr iAddress;		// Interface IP Address (*)
		};
	}



/** Bound Connectionless Push
*/
class CWapBoundCLPushService : public CBase
{

public: // creation/deletion
	IMPORT_C static CWapBoundCLPushService* NewL();
	IMPORT_C virtual ~CWapBoundCLPushService();

public: // API methods

	virtual TInt Connect(Wap::TBearer , Wap::TPort , TBool , TInetAddr ){return 0;}
	virtual TInt Connect(Wap::TBearer aBearer, Wap::TPort aPort, TBool aSecure);
	virtual TInt AwaitPush(TDes8& aPushHeaders, TDes8& aPushBody, TPckgBuf<TUint8>& aPushId, TRequestStatus& aReqStatus);
	virtual void CancelAwaitPush();
	virtual TInt GetLocalPort(Wap::TPort& /*aPort*/) { return KErrNotSupported;}	
	virtual TInt GetLocalAddress(HBufC8*& /*aLocalHost*/) { return KErrNotSupported;}
	virtual TInt GetBearer(Wap::TBearer& /*aBearer*/) { return KErrNotSupported;}
	virtual TInt GetServerAddressL(HBufC8*& aRemoteHost);

protected: // methods
	IMPORT_C CWapBoundCLPushService();

	CDummyWapSession* DummyWapSession();
private:
	CDummyWapSession* iSessHandle;
	CDummyWSPCLConn* iClConnHandle;
};

/**
 * This represents a client session with the server
 */
class RWAPServ
	{
public:
	IMPORT_C RWAPServ();
	IMPORT_C TInt Connect(TInt aDefaultMessageSlots = KDefaultMessageSlots);
	IMPORT_C void Close();
	CDummyWapSession* DummyWapSession();
private:
	CDummyWapSession* iSessHandle;
	CDummyWSPCLConn* iClConnHandle;
	};




/**
 *  Base class used by all type of connections
 */
class RWAPConn
	{
public:
	typedef TUint16 TPort;
	IMPORT_C virtual TInt Close();
	IMPORT_C class RWTLS Wtls();

	enum 
		{
		ERetCodeBase      = -5500,
		EWSPCORetCodeBase = ERetCodeBase + 25,
		EWSPCLRetCodeBase = ERetCodeBase + 50,
		EWTPRetCodeBase   = ERetCodeBase + 75,
		EWDPRetCodeBase   = ERetCodeBase + 100,
		EWTLSRetCodeBase  = ERetCodeBase + 125,
		ERetCodeTop       = ERetCodeBase + 200
		};
	enum TReturnCodes
		{  
		EMoreData           = ERetCodeBase + 0,
		EBearerError        = ERetCodeBase + 1,
		EPortAlreadyBound   = ERetCodeBase + 2,
		ECannotOpenPort     = ERetCodeBase + 3,
		ETooManyConnections = ERetCodeBase + 4,
		EBadConnection      = ERetCodeBase + 5,
		EBufferTooSmall     = ERetCodeBase + 6
		};
	enum TMethod 
		{
		EGet = 0x40,
		EOptions = 0x41,
		EHead = 0x42,
		EDelete = 0x43,
		ETrace = 0x44,
		EPost = 0x60,
		EPut = 0x61
		};
protected:
	CDummyWSPConn* iDummyConn;
	};



class RWTLS 
	{
public:
	enum TReturnCodes
		{
		EOutOfMemory            = RWAPConn::EWTLSRetCodeBase + 0,
		ESocketError            = RWAPConn::EWTLSRetCodeBase + 1,
		EBadParameters          = RWAPConn::EWTLSRetCodeBase + 2,
		ESocketError2           = RWAPConn::EWTLSRetCodeBase + 3,
		EResumeIDNotFound       = RWAPConn::EWTLSRetCodeBase + 4,
		EResumeError            = RWAPConn::EWTLSRetCodeBase + 5,
		ERenegotiationRequest   = RWAPConn::EWTLSRetCodeBase + 6,
		EIllegalCall            = RWAPConn::EWTLSRetCodeBase + 7,
		EConnectionCloseNotify  = RWAPConn::EWTLSRetCodeBase + 8,
		ESessionCloseNotify     = RWAPConn::EWTLSRetCodeBase + 9,
		ENoConnection           = RWAPConn::EWTLSRetCodeBase + 10,
		EUnexpectedMessage      = RWAPConn::EWTLSRetCodeBase + 11,
		EBadRecordMac           = RWAPConn::EWTLSRetCodeBase + 12,
		EDecryptionFailed       = RWAPConn::EWTLSRetCodeBase + 13,
		ERecordOverflow         = RWAPConn::EWTLSRetCodeBase + 14,
		EHandshakeFailure       = RWAPConn::EWTLSRetCodeBase + 15,
		EDecompressionFailure   = RWAPConn::EWTLSRetCodeBase + 16,
		EBadCertificate         = RWAPConn::EWTLSRetCodeBase + 17,
		EUnsupportedCertificate = RWAPConn::EWTLSRetCodeBase + 18,
		ECertificateRevoked     = RWAPConn::EWTLSRetCodeBase + 19,
		ECertificateExpired     = RWAPConn::EWTLSRetCodeBase + 20,
		ECertificateUnknown     = RWAPConn::EWTLSRetCodeBase + 21,
		EIllegalParameter       = RWAPConn::EWTLSRetCodeBase + 22,
		EUnknownCA              = RWAPConn::EWTLSRetCodeBase + 23,
		EAccessDenied           = RWAPConn::EWTLSRetCodeBase + 24,
		EDecodeError            = RWAPConn::EWTLSRetCodeBase + 25,
		EDecryptError           = RWAPConn::EWTLSRetCodeBase + 26,
		EUnknownKeyId           = RWAPConn::EWTLSRetCodeBase + 27,
		EDisableKeyId           = RWAPConn::EWTLSRetCodeBase + 28,
		EKeyExchangeDisabled    = RWAPConn::EWTLSRetCodeBase + 29,
		ESessionNotReady        = RWAPConn::EWTLSRetCodeBase + 30,
		EUnknownParameterIndex  = RWAPConn::EWTLSRetCodeBase + 31,
		EDuplicateFinishedRecv  = RWAPConn::EWTLSRetCodeBase + 32,
		EExportRestriction      = RWAPConn::EWTLSRetCodeBase + 33,
		EProtocolVersion        = RWAPConn::EWTLSRetCodeBase + 34,
		EInsufficientSecurity   = RWAPConn::EWTLSRetCodeBase + 35,
		EInternalError          = RWAPConn::EWTLSRetCodeBase + 36,
		EUserCanceled           = RWAPConn::EWTLSRetCodeBase + 37,
		ENoRenegotiation        = RWAPConn::EWTLSRetCodeBase + 38,

		EErrGeneral             = RWAPConn::EWTLSRetCodeBase + 39,
		EErrBulk                = RWAPConn::EWTLSRetCodeBase + 40,
		EErrMac                 = RWAPConn::EWTLSRetCodeBase + 41,
		EErrSequenceNumberMode  = RWAPConn::EWTLSRetCodeBase + 42,
		EErrKeyRefreshRate      = RWAPConn::EWTLSRetCodeBase + 43,
		};

	enum TIdType 
		{
		EIdNull,
		EText, 
		EBinary, 
		EKeyHashSha = 254, 
		EX509Name = 255
		};

	enum TKeyExchangeSuite 
		{    
		ESharedSecret = 1,
		EDHAnon,
		EDHAnon512,
		EDHAnon768,
		ERsaAnon,
		ERsaAnon512,
		ERsaAnon768,
		ERsa,
		ERsa512,
		ERsa768,
		EEcdhAnon,
		EEcdhAnon113,
		EEcdhAnon131,
		EEcdhEcdsa
		};

	IMPORT_C TInt SetKeyExchangeSuite(TKeyExchangeSuite aSuite,TIdType IdType,
			const TDesC8& aKeyId);

	enum TBulkEncryptionAlgorithm
		{
		EBulkNull     = 0x00,
		ERc5_cbc_40   = 0x01,
		ERc5_cbc_56   = 0x02,
		ERc5_cbc      = 0x03,
		EDes_cbc_40   = 0x04,
		EDes_cbc      = 0x05,
		E3Des_cbc_ede = 0x06,
		EIdea_cbc_40  = 0x07,
		EIdea_cbc_56  = 0x08,
		EIdea_cbc     = 0x09
		};
	
	enum TMacAlgorithm
		{
		ESha_0         = 0x00,
		ESha_40        = 0x01,
		ESha_80        = 0x02,
		ESha           = 0x03,
		ESha_xor_40    = 0x04,
		EMd5_40        = 0x05,
		EMd5_80        = 0x06,
		EMd5           = 0x07
		};
		
	struct TCipherSuite
		{
		TBulkEncryptionAlgorithm iBulkEncryptionAlgorithm;
		TMacAlgorithm            iMacAlgorithm;
		};
	
	typedef CArrayFixFlat<TCipherSuite> CipherSuiteArray;

	IMPORT_C TInt SetCipherSuites(const CipherSuiteArray& aCipherSuites);
			
	enum TSequenceNumberMode 
		{
		ENotUsed = 0, 
		EImplicit, 
		EExplicit
		};
		
	IMPORT_C TInt SetSequenceNumberMode(TSequenceNumberMode aMode);
	IMPORT_C TInt SetKeyRefreshRate(TUint8 aRate);
	IMPORT_C TInt SetSharedSecret(const TDesC8& aSharedSecret);
	IMPORT_C TInt SetRecordLengthUsage(TBool aUsage);


	// This is returned in the TRequestStatus on completion in 
	// Connect and ConnectPhaseTwo.
	enum HandshakeResult
		{
		EServerCertificateValid,
		EServerCertificateNotValid,
		ECertificateRequested,
		EHandshakeOk,
		EHandshakeError,
		EHandshakeUserAbort
		};

	IMPORT_C void Connect(TRequestStatus& aStatus, TBool aTwoPhaseHandshake);


	IMPORT_C void CancelConnect();
	
	IMPORT_C TInt ConnectPhaseTwo(TRequestStatus& aStatus);
	IMPORT_C void CancelConnectPhaseTwo();
	
	IMPORT_C TInt GetCipherSuite(TCipherSuite& aCipherSuite) const;
	IMPORT_C TInt GetSequenceNumberMode(TSequenceNumberMode& aMode) const;
	IMPORT_C TInt GetKeyRefreshRate(TUint8& aRate) const;
	IMPORT_C class RCertificate ServerCert();
private:
	friend class RWAPConn;
	CDummyWTLS* iDummyWTLS;
	};


/**
 * This class represents a server certificate 
 */
class RCertificate 
	{
public:
	typedef TUint16 TCertId;
	typedef TDesC8 TCertificate; // binary format

	IMPORT_C TInt Load(const TDesC8& aSubject);
	IMPORT_C TInt LoadById(const TCertId& aId);
	IMPORT_C TInt Delete(const TDesC8& aSubject);
	IMPORT_C TInt Save(const TCertificate& aCertificate, TDes8& aSubject);
	IMPORT_C TInt FindFirst(TDes8& aSubject, TCertId& aId) const;
	IMPORT_C TInt FindNext(TDes8& aSubject, TCertId& aId) const;

	enum TStatus
		{
		EOk, 
		ENotYetValid, 
		EExpired, 
		EUnknownCA, 
		EPermanentStorageReadError
		};

	struct TInfo
		{
		TUint32 iValidNotBefore;     //  is first valid date of the cert (unix GMT). 
		TUint32 iValidNotAfter;      //  is last valid date of the cert (unix GMT)
		TUint16 iSubjectCharacterSet;//  is an IANA defined number.
		TUint16 iIssuerCharacterSet; //  is an IANA defined number.
		TDes8*  iSubject;             
		TDes8*  iIssuer;             
		TStatus iServerCertificateStatus;
		TDes8*  iFingerPrint;
		};

	IMPORT_C TInt Info(TInfo& aInfo) const;
private:
	friend class RWTLS;
	RCertificate(){}
	};



/**
 * this class represents a connection of type connectionless Push
 */
class RWSPCLConn : public RWAPConn
	{
public:
	IMPORT_C TInt Open(RWAPServ& aServer, TPort aLocalPort);
	IMPORT_C TInt Open(RWAPServ& aServer,
						const TDesC8& aRemoteHost,
						TPort aRemotePort,
						TPort aLocalPort,
						Wap::TBearer aBearer,
						TBool aSecureConn);
	IMPORT_C TInt AwaitPush(TDes8& aPushHeaders, TDes8& aPushBody, TUint& aPushId, TRequestStatus& aReqStatus);
	IMPORT_C void UnitWaitPush(TDes8& aBody,TDes8& aHeaders,TPushID& aID,TRequestStatus& aStatus);
	IMPORT_C void CancelUnitWaitPush();
	IMPORT_C void CancelAwaitPush();
	IMPORT_C TInt UnitWaitPush(TDes8& aBody,TDes8& aHeaders,TPushID& aID);

	enum TReturnCodes
		{			
		EBufferTooSmall      = EWSPCLRetCodeBase + 0,
		EInvalidSession      = EWSPCLRetCodeBase + 1,
		EParameterError      = EWSPCLRetCodeBase + 2,
		ESessionNotConnected = EWSPCLRetCodeBase + 3,
		ENotSupported        = EWSPCLRetCodeBase + 4,
		EInvalidState        = EWSPCLRetCodeBase + 5,
		ESessionClosed       = EWSPCLRetCodeBase + 6,
		EMRUExceeded		 = EWSPCLRetCodeBase + 7,	
		EInvalidBufferSize   = EWSPCLRetCodeBase + 8,
		EDataNotAvailable    = EWSPCLRetCodeBase + 9,
		EErrorTimeout        = EWSPCLRetCodeBase + 10
		};
private:
	CDummyWSPCLConn* iClConnHandle;
	};


/**
 * Represents a connection-orientated transaction
 *
 */
class RWSPCOTrans 
	{
public:
	IMPORT_C RWSPCOTrans();
	IMPORT_C ~RWSPCOTrans();

	enum TDataType
		{
		EPushBody = 0,
		EPushHeaders,
		EResultStatus,
		EResultBody,
		EResultHeaders,
		EInvokeUri,
		EInvokeMethod,
		EInvokeHeaders,
		EInvokeBody,
		EAckHeaders,
		EAbortReason
		};	

	enum TState 
		{
		EInit = 0,
		ERequesting,
		EReplying,
		EDone,
		EAborted,
		EPushing
		};
	typedef TPckgBuf<TState> TStatePckg;
	typedef TPckgBuf<TInt> THandlePckg;
	typedef TUint8 TAbortReason;
	typedef TInt TTransID;

	/** acknowledge a transaction - must be used for confirmed push */
	IMPORT_C TInt Acknowledge(const TDesC8& aAckHeaders);

	/** Retrieves the data associated to a transaction */
	IMPORT_C TInt GetData(TDes8& aBuffer, TDataType aDataType, TInt* aSizeLeft = 0) const;
	
	/** Retrieve the state the transaction is in */
	IMPORT_C TInt GetState(TState& aState) const;
	IMPORT_C TInt Abort(TAbortReason aReason);
	IMPORT_C TInt Release();
	IMPORT_C TInt Id(TTransID& aId);
private:
	friend class CDummyWSPCOConn;
	CDummyWSPCOTrans* iCoTransHandle;	
	};

enum TBearer
{
	EAll,
	EIP,
	ESMS7,
	ESMS,
  	EWAPSMS7,
  	EWAPSMS
  	};

/**
 * Connection-orientated connection
 *
 */
class RWSPCOConn : public RWAPConn
	{
public:
	IMPORT_C TInt Open(RWAPServ& aServer, const TDesC8& aRemoteHost, TPort aRemotePort, TPort aLocalPort,
		TBearer aBearer, TBool aSecureConn);
	// Open fully defined
	IMPORT_C virtual TInt Close();

	IMPORT_C TInt Connect(const TDesC8& aClientHeaders, class CCapCodec* aCap = 0);
	IMPORT_C TInt Disconnect();
	
	IMPORT_C TInt Suspend();
	IMPORT_C TInt Resume(RWSPCOConn& aBlankConn, const TDesC8& aHeaders);

	enum TSessionState
		{
		EInit = 0,
		EConnecting,
		EConnected,
		ESuspended,
		EResuming,
		EReplying,
		EDisconnected
		};
	enum TSessionDataType 
		{
		EServerHeaders = 12,
		EClientHeaders,
		ECapabilities,
		ERedirectOptions,
		ERedirectAddresses,
		EQueueLen,
		EErrorHeaders,
		EErrorBody,
		EDisconReason,
		ESuspendReason
		};

	IMPORT_C TInt GetSessionState(TSessionState& aState) const;
	IMPORT_C TInt GetSessionData(TDes8& aData, TSessionDataType aType) const;

	IMPORT_C TInt CreateTransaction(TMethod aMethod, const TDesC& aURI, const TDesC8& aHeaders, 
		const TDesC8& aBody, RWSPCOTrans& aTrans);
	
	typedef TPckgBuf<TInt> TEvent;

	/** Gets next session or transaction event on this session */
	IMPORT_C void GetEvent(TEvent& aEvent, RWSPCOTrans& aTrans, TRequestStatus& aStatus) const;
	/** Cancel the asynchronous GetEvent operation */		
	IMPORT_C void CancelGetEvent();

	/** Gets next session or transaction event on this session (synchronous version */
	IMPORT_C TInt GetEvent(TEvent& aEvent, RWSPCOTrans& aTrans) const; 
		
	/** Return the number of events pending for the session */
	IMPORT_C TInt GetNrOfEvents(TUint& aNrOfEvents) const;

	enum TReturnCodes
		{
		EBufferTooSmall      = EWSPCORetCodeBase + 0,
		EInvalidSession      = EWSPCORetCodeBase + 1,
		EInvalidTransaction  = EWSPCORetCodeBase + 2,
		EParameterError      = EWSPCORetCodeBase + 3,
		ESessionNotConnected = EWSPCORetCodeBase + 4,
		ENotSupported        = EWSPCORetCodeBase + 5,
		EInvalidState        = EWSPCORetCodeBase + 6,
		ESessionClosed       = EWSPCORetCodeBase + 7,
		EMRUExceeded         = EWSPCORetCodeBase + 8,
		EInvalidBufferSize   = EWSPCORetCodeBase + 9,
		EDataNotAvailable    = EWSPCORetCodeBase + 10,
		EErrorTimeout        = EWSPCORetCodeBase + 11
		};	

	enum TEventType
		{    
		// session events
		EDisconnect_ind_s = 0,
		ESuspend_ind_s,
		EResume_cnf_s,
		EConnect_cnf_s,
		ERedirect_ind_s,
			
		// transaction events
		EMethodInvoke_cnf_t,
		EMethodResult_ind_t,
		EAbort_ind_t,
		EPush_ind_t,
		EConfirmedPush_ind_t,
			
		// exception events
		EException_ind_e,
		};
private:
	CDummyWSPCOConn* iCoConnHandle;
	};


#endif
