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


#ifndef __DUMMYWAPSTACK_H__
#define __DUMMYWAPSTACK_H__

#include <e32base.h>
#include <e32std.h>
#include "Dummywapstd.h"
#include "DummyWapClient.h"

/**
 *forward declaration
 */
class CDummyWapSession;
class CDummyWSPCLConn;
class CDummyWSPCOConn;
class CDummyWapStack;
class CDummyWSPCOTrans;
class CDummyWTLS;
typedef TUint16 TPort;



/**
 * Mixin class to allow communication between dummy stack and test harness
 */
class MDummyWapStackObserver
	{
public:
	virtual void DWSOUnitWaitPushL(CDummyWSPCLConn& aDummyCLConn)=0;
	virtual void DWSOCancelUnitWaitPush(CDummyWSPCLConn& aDummyCLConn)=0;
	virtual void DWSOServerShutDown()=0;
	virtual void DWSOError(const TDesC& aDes, TInt aLineNum)=0;
	virtual void DWSOConnect(CDummyWSPCOConn& aDummyCOConn)=0;
	virtual void DWSOGetEventL(CDummyWSPCOConn& aDummyCOConn)=0;
	virtual void DWSOCancelGetEvent(CDummyWSPCOConn& aDummyCOConn)=0;
	virtual void DWSOAckTransaction(CDummyWSPCOTrans& aDummyCOTrans)=0;
	};


/**
 * This is the 'dummy' equivalent of the WAP stack server
 * 
 * It uses the singleton design pattern to ensure there is only one
 * instance of this object. It is responsible for creating and managing
 * client sessions with the server.
 */
class CDummyWapStack : public CBase
	{
public:
	IMPORT_C static CDummyWapStack* NewL(MDummyWapStackObserver& aObserver); 
	~CDummyWapStack();

	IMPORT_C CDummyWapSession* NewSessionL();
	IMPORT_C void CloseSession(CDummyWapSession* aDummyWapSession);
	//
	IMPORT_C MDummyWapStackObserver& Observer();
private:	
	CDummyWapStack(MDummyWapStackObserver& aObserver);
	void ConstructL();
	CDummyWapStack* StackInstance(){return STATIC_CAST(CDummyWapStack* , Dll::Tls());};//return unique instance
	void FreeTls();
private:
	MDummyWapStackObserver& iObserver;
	RPointerArray<CDummyWapSession> iSessionsArray;
	CDummyWapStack* iInstance;
	};
 

/**
 * This is the 'dummy' equivalent of a single client session with the server 
 *
 * It represents the CWAPSession class, which is reponsible for creating and
 * managing subsessions, ie. individual connections (CWAPConn objects)
 */
class CDummyWapSession : public CBase
	{
public:
	static CDummyWapSession* NewL();
	~CDummyWapSession();

	CDummyWSPCLConn* NewWSPCLConnL(TPort aLocalPort);
	CDummyWSPCOConn* NewWSPCOConnL();
	
	void Close();
private:
	void ConstructL();
private:
	TInt iReferenceCount;
	};


/**
 * Base class for all 'dummy' WSP connections. 
 *
 * This is required so that the RWAPConn base class can correctly close the 
 * CDummy... classes, because the base class doesn't implement a virtual 
 * Close method
 */
class CDummyWSPConn : public CBase
	{
public:
	CDummyWSPConn(CDummyWapSession& aSession);
	virtual void Close();
	CDummyWTLS* WtlsL();
private:
	CDummyWapSession& iSession;
	CDummyWTLS* iWtls;
	};

#ifndef __WAPMESSAGE_H__
#ifndef __DUMMYWAPCLIENT_H__
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
 
	}
#endif
#endif

/** 
 *  Dummy connectionless connection
 */
class CDummyWSPCLConn : public CDummyWSPConn
	{
public:
	CDummyWSPCLConn(CDummyWapSession& aSession, TPort aLocalPort);
	~CDummyWSPCLConn();
	virtual TInt Connect(Wap::TBearer aBearer, Wap::TPort aPort, TBool aSecure);
	TInt AwaitPush(TDes8& aPushHeaders, TDes8& aPushBody, TPckgBuf<TUint8>& aPushIdPckg, TRequestStatus& aReqStatus);
	void UnitWaitPushL(TDes8& aBody,TDes8& aHeaders,TPushID& aID,TRequestStatus& aStatus);
	TInt CancelUnitWaitPush();
	TInt CancelAwaitPush(); 
	IMPORT_C void CompleteUnitWaitPushL(TDes8& aBody,TDes8& aHeaders);
	TInt UnitWaitPushL(TDes8& aBody,TDes8& aHeaders,TPushID& aID);//Sync Version
	IMPORT_C TBool HasMsgBeenCompleted();
	inline TPort LocalPort(){return iLocalPort;}
	IMPORT_C void SetServerAddressL(const TDesC8& aAddr);
	const TDesC8& GetServerAddr();
private:
	TDes8*	 iClientBody;
	TDes8*	 iClientHeaders;
	TPushID* iID;
	TRequestStatus* iStatus;
	//
	HBufC8* iPushHeadersData;
	TInt iPushHeadersDataOffset;
	HBufC8* iPushBodyData;
	TInt iPushBodyDataOffset;
	TPort iLocalPort; //using port we can identify this CL as either secure or unsecure 
	HBufC8* iServerAddr;
	};



/** 
 * Dummy connection-orientated transaction
 */
class CDummyWSPCOTrans : public CBase
	{
public:
	IMPORT_C CDummyWSPCOTrans(CDummyWSPCOConn& aConnection);
	IMPORT_C ~CDummyWSPCOTrans();
	TInt Acknowledge(const TDesC8& aAckHeaders);
	TInt GetData(TDes8& aBuffer, RWSPCOTrans::TDataType aDataType, TInt* aSizeLeft = 0) const;
	TInt GetState(RWSPCOTrans::TState& aState) const;
	TInt Abort(RWSPCOTrans::TAbortReason aReason);
	TInt Release();
	TInt Id(RWSPCOTrans::TTransID& aId);
	//for testcode
	IMPORT_C void SetData(const TDesC8& aBuffer,RWSPCOTrans::TDataType aDataType);
	IMPORT_C void SetId(TInt aId);
private:
	CDummyWSPCOConn& iConnection;
	RWSPCOTrans::TState iTransState;
	TInt iId;

	//TFixedArray<const TDesC8**,11> iDataArray;
	TFixedArray<const TDesC8*,11> iDataArray;
	TFixedArray<TInt*,11> iOffsetArray;
	
	const TDesC8* iClientAckHeaders;
	};




/**
 *  Dummy connection-orientated connection
 */
class CDummyWSPCOConn : public CDummyWSPConn
	{
public:
	CDummyWSPCOConn(CDummyWapSession& aSession);
	~CDummyWSPCOConn();

	TInt Connect(const TDesC8& aClientHeaders, class CCapCodec* aCap);
	TInt Disconnect();
	TInt GetSessionState(RWSPCOConn::TSessionState& aState);
	TInt GetEvent(RWSPCOConn::TEvent& aEvent, RWSPCOTrans& aTrans, TRequestStatus& aStatus);
	IMPORT_C void CancelGetEvent();
	TInt GetNrOfEvents(TUint& aNrOfEvents);
	//for testcode
	IMPORT_C void CompleteGetEventL(RWSPCOConn::TEvent& aEvent,CDummyWSPCOTrans& aDummyTrans);
	void RemoveTransaction(CDummyWSPCOTrans& aTrans);
private:
	const TDesC8* iClientHeaders;
	CCapCodec* iCapCodec;
	RWSPCOConn::TSessionState iSessionState;
	RPointerArray<CDummyWSPCOTrans> iTransArray;

	RWSPCOConn::TEvent* iEvent;
	RWSPCOTrans* iTrans;
	TRequestStatus* iStatus;
	};





/** 
 * Dummy WTLS 
 */
class CDummyWTLS : public CBase
	{
public:
	CDummyWTLS();
	~CDummyWTLS();

	TInt SetKeyExchangeSuite(RWTLS::TKeyExchangeSuite aSuite,RWTLS::TIdType IdType,const TDesC8& aKeyId);
	TInt SetCipherSuites(const RWTLS::CipherSuiteArray& aCipherSuites);
	TInt SetSequenceNumberMode(RWTLS::TSequenceNumberMode aMode);
	TInt SetKeyRefreshRate(TUint8 aRate);
	TInt SetSharedSecret(const TDesC8& aSharedSecret);
	TInt SetRecordLengthUsage(TBool aUsage);
	void Connect(TRequestStatus& aStatus, TBool aTwoPhaseHandshake);
	void CancelConnect();
	TInt ConnectPhaseTwo(TRequestStatus& aStatus);
	void CancelConnectPhaseTwo();
	TInt GetCipherSuite(RWTLS::TCipherSuite& aCipherSuite) const;
	TInt GetSequenceNumberMode(RWTLS::TSequenceNumberMode& aMode) const;
	TInt GetKeyRefreshRate(TUint8& aRate) const;
private:
	TRequestStatus* iConnect;
	TRequestStatus* iConnectPhaseTwo;
	TBool iTwoPhaseHandshake;
	RWTLS::TSequenceNumberMode iSequenceNumMode;
	TUint8 iKeyRefreshRate;
	TBool iRecordLenUsage;
	};

#endif
