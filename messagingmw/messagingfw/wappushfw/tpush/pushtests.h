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

#ifndef __PUSHTESTS_H__
#define __PUSHTESTS_H__

#include "t_wappush.h"
#include <msvapi.h>
#include <mtclbase.h>
#include "dummywapstack.h"		// MDummyWapStackObserver

#include <push/pushlog.h>			// for test engine Rfs/Cwatcherlog
#include "ConnMan.h"			// for connection manager
#include <caf/manager.h>
#include <caf/rightsmanager.h>

//const TInt KBufferSize = 256;		// imitates KReceiveBufferSize from CLWatcher
//const TInt KBufferSize = 1024;
const TInt KBufferSize = 2048;

class MWapPushLog;
class CWapPushMsgGen;

class MTimeoutCallback
	{
public:
	virtual void Timeout() = 0;
	};

class CTestTimer : public CTimer
	{
public:
	static CTestTimer* NewL(MTimeoutCallback* callback)
		{
		CTestTimer* self = new(ELeave) CTestTimer(callback);
		CleanupStack::PushL(self);
		self->ConstructL(); // CTimer
		CActiveScheduler::Add(self);
		CleanupStack::Pop();
		return self;
		}

protected:
	virtual void RunL() 
		{ 
		iCallback->Timeout(); 
		}
private:
	CTestTimer(MTimeoutCallback* callback): CTimer(EPriorityLow), iCallback(callback)
	{};
	
	MTimeoutCallback* iCallback;
	};


/**
 * 	Derived test case class which implementation the stack observer
 * 	to allow communication with the stack
 *
 *	This class has been made abstract to allow different derived classes
 *	to implement creating multiple test messages via PrepareTestMessageL()
 *	and the support of NumberTestCasesToRun()
 *	BOTH must be implimented in any derived class.
 *	ConfirmMessagesSavedL provides confirmation of message storage and 
 *  reports on number of messages found in store.
 */
class CWapPushBaseMessageTest : public CWapPushTest, public MDummyWapStackObserver
	{
public:
	virtual ~CWapPushBaseMessageTest();
	virtual void RunL();
	virtual TInt RunError(TInt aError);
	virtual void DoCancel();
	virtual const TDesC& TestName();

	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase) = 0;	// abstract class now!!!
	virtual TBool ConfirmMessagesSavedL() = 0;

	// from MDummyWapStackObserver	
	virtual void DWSOUnitWaitPushL(CDummyWSPCLConn& aDummyCLConn);
	virtual void DWSOCancelUnitWaitPush(CDummyWSPCLConn& aDummyCLConn);
	virtual void DWSOServerShutDown();
	virtual void DWSOError(const TDesC& aDes, TInt aLineNum);
	virtual void DWSOConnect(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOGetEventL(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOCancelGetEvent(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOAckTransaction(CDummyWSPCOTrans& aDummyCOTrans);
protected:
	void MoveToNextState();
	void CompleteOwnRequest();

	void InitialiseMsgService();
	
protected:
	enum TState
		{
		EInitialisation,
		EWaitForPush,
		EFinished
		};
protected:
	TState						iState;
	
	TBuf8<KBufferSize>			iBodyBuf;
	TBuf8<KBufferSize>			iHeadersBuf;
	HBufC8*						iBody;
	HBufC8*						iHeaders;

	CDummyWapStack* iWapStack;
	CConnectionManager* iConnMan;
	TBool iHasMsgBeenCompleted;
	TBool iHasTestBeenCompleted;

	TInt iNumberOfTestCases;			
	TInt iCurrentTestCase;

	};

/** SL derived test case */
class CWapPushSLMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
	
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();
	};

/** SLC derived test case */
class CWapPushSLCMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName ();
	virtual void NumberTestCasesToRun ();
	virtual void PrepareTestMessageL ( TInt aCurrentTestCase );
	virtual TBool ConfirmMessagesSavedL();
	};

/** SI derived test case */
class CWapPushSIMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
		
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();

private:
	void PrepareApostropheTestMessageL();
	};

/** SIC derived test case */
class CWapPushSICMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();		
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();
	private:
	TBool TestMessageL ( const TDesC& aUri, const TDesC8& aText );
	};

/** Invalid DTD test case */
class CInvalidWAPDTDMessages : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName ();
	virtual void NumberTestCasesToRun ();
	virtual void PrepareTestMessageL (TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL ();
	virtual TInt RunError(TInt aError);
	};

/** Test case for INC081489,INC081568, INC082189, INC082190, INC082191  */
class CINC081489 : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
		
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();
	};


/** Multipart-Mixed derived test case */
class CWapPushMMMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
		
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();
	};

/** Multipart-Related derived test case */
class CWapPushMRMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
		
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();
	};

/** Multipart-Alternative derived test case */
class CWapPushMAMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
		
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();
	};

/** Unknown Handler derived test case */
class CWapPushUnkMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
		
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();
	void TestAndPrint(const TDesC& aTestCase, const TDesC& aFilename, const TDesC& aLocalFilename);
	};

/** 
 *	SIA Handler derived test case 
 */
class CWapPushSIAMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
		
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	
	virtual TBool ConfirmMessagesSavedL();
	void SetupAddressL();
	};


/** SL derived test case */
class CWapPushCorruptMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
	
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();
private:	
	void SetHeaderBuffer(TUint aTestCase);

	enum TWapPushCorruptMsgType {
		//-- Test SI Corrupt Msgs -- 
		ECorruptSiMsg1=0,	// 'href' - Optinal data missing
		ECorruptSiMsg2,		// 'si-id' - Optianl data missing
		ECorruptSiMsg3,		// 'created' - Mandatory data missing
		ECorruptSiMsg4,		// 'si-expires' - Mandatory data missing	
		ECorruptSiMsg5,		// 'action' - Optional data missing
		ECorruptSiMsg6,		// Mandatory data missing - The INFO Element-
		ECorruptSiMsg7,		// Missing Body
		ECorruptSicMsg1,	// Missing Body
		//-- Test SL Corrupt Msgs --
		ECorruptSlMsg1,		// 
		ECorruptSlMsg2,		// Header - Content type SLC instead of SL
		ECorruptSlMsg3,		// Missing Body
		ECorruptSlMsg4,		// Unsupported XML version - 2.0
		ECorruptSlMsg5,		// Corrupt XML body - incorrect DTD
		ECorruptSlMsg6,		// Corrupt XML body - invalid tag in XML header 
		ECorruptSlMsg7,		// Corrupt XML body - unknown tags in XML content
		ECorruptSlMsg8,		// Corrupt XML body - "hreg" not "href"
		ECorruptSlMsg9,		// Corrupt XML body - "adtion" not "action"
		ECorruptSlMsg10,	// Missing Href	
		ECorruptSlMsg11,	// Missing Action

		//-- Test SLC Corrupt Msgs --
		ECorruptSlcMsg1,	// Header - wrong App ID 
		ECorruptSlcMsg2,	// Header - Content type SL instead of SLC
		ECorruptSlcMsg3,	// WMLC - body has corrupt bit 
		ECorruptSlcMsg4,	// WMLC - body has been abruptly ended - invalid termination
		ECorruptSlcMsg5,	// WMLC - missing Body
		ECorruptSlcMsg6,	// WMLC - body contains only a single Null character
		ECorruptSlcMsg7,	// WMLC - missing Href
		ECorruptSlcMsg8,	// WMLC - missing Action
		
		//-- Test SIA Corrupt Msgs --
		ECorruptSiaMsg1,	// Corrupt Body
		ECorruptSiaMsg2,	// Corrupt CO/Ack
		ECorruptSiaMsg3,	// Corrupt CO/no Ack
		ECorruptSiaMsg4,	// SIA doesn't accept a connection, the test harness should time out

		//-- Test Multi-Part/Mixed Corrupt Msgs --
		ECorruptMultiMixedMsg1,	//Corrupt first part
		ECorruptMultiMixedMsg2,	//Corrupt 2nd multipart
		ECorruptMultiMixedMsg3,	// Corrupt Header
		ECorruptMultiMixedMsg4,    // 1st Part Hdr - header length value too small
		ECorruptMultiMixedMsg5,	// 2nd Part Hdr - header length value too small
		ECorruptMultiMixedMsg6,	// 1st Part Hdr - header length value too large
		ECorruptMultiMixedMsg7,	// 2nd Part Hdr - header length value too large
		ECorruptMultiMixedMsg8,	// 1st Part Hdr - part body length value too small
		ECorruptMultiMixedMsg9,	// 2nd Part Hdr - part body length value too small
		ECorruptMultiMixedMsg10,	// 1st Part Hdr - part body length value too large
		ECorruptMultiMixedMsg11,	// 2nd Part Hdr - part body length value too large
		ECorruptMultiMixedMsg12,	// 1st Part Body - part body shorter than header length value 
		ECorruptMultiMixedMsg13,	// 2nd Part Body - part body shorter than header length value 
		ECorruptMultiMixedMsg14,	// 1st Part Body - part body longer than header length value 
		ECorruptMultiMixedMsg15,	// 2nd Part Body - part body longer than header length value 
		ECorruptMultiMixedMsg16,   // Multipart header incorrectly specifies 3 parts when there's 2
		ECorruptMultiMixedMsg17,	// Multipart header incorrectly specifies 2 parts when there's 3
		ECorruptMultiMixedMsg18,	// Corrupt Wrong App Id - UnknownAppHandler
		ECorruptMultiMixedMsg19,   // No binary data - WAP Push message body is empty
		ECorruptMultiMixedMsg20,	// WAP Push message body contains only a single null value
		ECorruptMultiMixedMsg21,	// Message body is full of junk - no proper WAP Multipart headers

		//-- Test Multi-Part/Related Corrupt Msgs --
		ECorruptMultiRelatedMsg1,	//Corrupt first part
		ECorruptMultiRelatedMsg2,	//Corrupt 2nd multipart
		ECorruptMultiRelatedMsg3,	// Corrupt Header
		ECorruptMultiRelatedMsg4,    // 1st Part Hdr - header length value too small
		ECorruptMultiRelatedMsg5,	// 2nd Part Hdr - header length value too small
		ECorruptMultiRelatedMsg6,	// 1st Part Hdr - header length value too large
		ECorruptMultiRelatedMsg7,	// 2nd Part Hdr - header length value too large
		ECorruptMultiRelatedMsg8,	// 1st Part Hdr - part body length value too small
		ECorruptMultiRelatedMsg9,	// 2nd Part Hdr - part body length value too small
		ECorruptMultiRelatedMsg10,	// 1st Part Hdr - part body length value too large
		ECorruptMultiRelatedMsg11,	// 2nd Part Hdr - part body length value too large
		ECorruptMultiRelatedMsg12,	// 1st Part Body - part body shorter than header length value 
		ECorruptMultiRelatedMsg13,	// 2nd Part Body - part body shorter than header length value 
		ECorruptMultiRelatedMsg14,	// 1st Part Body - part body longer than header length value 
		ECorruptMultiRelatedMsg15,	// 2nd Part Body - part body longer than header length value 
		ECorruptMultiRelatedMsg16,   // Multipart header incorrectly specifies 3 parts when there's 2
		ECorruptMultiRelatedMsg17,	// Multipart header incorrectly specifies 2 parts when there's 3
		ECorruptMultiRelatedMsg18,	// Corrupt Wrong App Id - UnknownAppHandler
		ECorruptMultiRelatedMsg19,   // No binary data - WAP Push message body is empty
		ECorruptMultiRelatedMsg20,	// WAP Push message body contains only a single null value
		ECorruptMultiRelatedMsg21,	// Message body is full of junk - no proper WAP Multipart headers
	
		//-- Test Multi-Part/Alternative Corrupt Msgs --
		ECorruptMultiAlternativeMsg1,	//Corrupt 1st multipart
		ECorruptMultiAlternativeMsg2,	//Corrupt 2nd multipart
		ECorruptMultiAlternativeMsg3,	// Corrupt Header
		ECorruptMultiAlternativeMsg4,    // 1st Part Hdr - header length value too small
		ECorruptMultiAlternativeMsg5,	// 2nd Part Hdr - header length value too small
		ECorruptMultiAlternativeMsg6,	// 1st Part Hdr - header length value too large
		ECorruptMultiAlternativeMsg7,	// 2nd Part Hdr - header length value too large
		ECorruptMultiAlternativeMsg8,	// 1st Part Hdr - part body length value too small
		ECorruptMultiAlternativeMsg9,	// 2nd Part Hdr - part body length value too small
		ECorruptMultiAlternativeMsg10,	// 1st Part Hdr - part body length value too large
		ECorruptMultiAlternativeMsg11,	// 2nd Part Hdr - part body length value too large
		ECorruptMultiAlternativeMsg12,	// 1st Part Body - part body shorter than header length value 
		ECorruptMultiAlternativeMsg13,	// 2nd Part Body - part body shorter than header length value 
		ECorruptMultiAlternativeMsg14,	// 1st Part Body - part body longer than header length value 
		ECorruptMultiAlternativeMsg15,	// 2nd Part Body - part body longer than header length value 
		ECorruptMultiAlternativeMsg16,   // Multipart header incorrectly specifies 3 parts when there's 2
		ECorruptMultiAlternativeMsg17,	// Multipart header incorrectly specifies 2 parts when there's 3
		ECorruptMultiAlternativeMsg18,	// Corrupt Wrong App Id - UnknownAppHandler
		ECorruptMultiAlternativeMsg19,   // No binary data - WAP Push message body is empty
		ECorruptMultiAlternativeMsg20,	// WAP Push message body contains only a single null value
		ECorruptMultiAlternativeMsg21	// Message body is full of junk - no proper WAP Multipart headers
		};
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS	
    /** 
	WAP Push Message status value. 
	*/
    enum TPushMsgStatus	
		{
		/** Message is corrupt. */
		EPushMsgStatusCorrupt = 3
		};
#endif//SYMBIAN_ENABLE_SPLIT_HEADERS
	
	};






#ifdef XXXX


/**
 * Tests the following use case
 * 'secure confirmed multipart/mixed containing SI and SL using WAP plug-in
 * interface'
 */
class CWapPushR2Test : public CWapPushTest, public MDummyWapStackObserver
	{
public:
	~CWapPushR2Test();
	virtual void RunL();
	virtual void DoCancel();
	virtual const TDesC& TestName();

	// from MDummyWapStackObserver	
	virtual void DWSOUnitWaitPushL(CDummyWSPCLConn& aDummyCLConn);
	virtual void DWSOCancelUnitWaitPush(CDummyWSPCLConn& aDummyCLConn);
	virtual void DWSOServerShutDown();
	virtual void DWSOError(const TDesC& aDes, TInt aLineNum);	
	virtual void DWSOConnect(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOGetEventL(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOCancelGetEvent(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOAckTransaction(CDummyWSPCOTrans& aDummyCOTrans);
private:
	void MoveToNextState();
	void CompleteOwnRequest();
	void CreatePushMsg();
	void CreateMultiMixedPushMsg();
private:
	enum TState
		{
		EInitialisation,
		ESendSIA,
		EWaitForAcknowledge,
		EFinished
		};
	enum TResponseState
		{
		ESessionConfirm,
		ETransaction
		};
private:
	TState iState;
	TResponseState iResponseState;

	TBuf8<KBufferSize> iBodyBuf;
	TBuf8<KBufferSize> iHeadersBuf;
	HBufC8*	iBody;
	HBufC8*	iHeaders;

	CDummyWapStack* iWapStack;
	CConnectionManager* iConnMan;
	TBool iHasMsgBeenCompleted;
	TBool iAck;

	};

#endif

/**
 * Tests the following use case:
 *  - Connectionless Secure SIA Push Message
 *  - Connection Oriented Unsecure
 *  - Multipart/related Push Message Containing Si, vCard, JPEG
 */
class CWapPushR3Test : public CWapPushTest, public MDummyWapStackObserver, public MTimeoutCallback
	{
public:
	~CWapPushR3Test();
	virtual void RunL();
	virtual void DoCancel();
	virtual const TDesC& TestName();

	// from MDummyWapStackObserver	
	virtual void DWSOUnitWaitPushL(CDummyWSPCLConn& aDummyCLConn);
	virtual void DWSOCancelUnitWaitPush(CDummyWSPCLConn& aDummyCLConn);
	virtual void DWSOServerShutDown();
	virtual void DWSOError(const TDesC& aDes, TInt aLineNum);	
	virtual void DWSOConnect(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOGetEventL(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOCancelGetEvent(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOAckTransaction(CDummyWSPCOTrans& aDummyCOTrans);

	// MTimeoutCallback
	virtual void Timeout();
private:
	void MoveToNextState();
	void CompleteOwnRequest();
	void CreateMultiPartRelatedPushMsg();
	void CreateSLPushMsg();
	void CreateClSecureSiaPushMsgL();
	void SetupAddressL();
private:
	enum TState
		{
		EInitialisation,
		EDWSOGetEventInitialisation,
		EWaitForAcknowledge,
		EFinished
		};
	enum TResponseState
		{
		ESessionConfirm,
		ETransaction
		};
private:
	TState iState;
	TResponseState iResponseState;

	TBuf8<KBufferSize> iBodyBuf;
	TBuf8<KBufferSize> iHeadersBuf;
	HBufC8*	iBody;
	HBufC8*	iHeaders;

	CDummyWapStack* iWapStack;
	CConnectionManager* iConnMan;
	TBool iHasMsgBeenCompleted;
	TBool iAck;
	TBool iSiaMsgPushed;
	CTestTimer	*iTimer;

	};

//------------------------------------------------------------------------------
class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};

//------------------------------------------------------------------------------
class CWapClientMtmFindTest : public CWapPushTest
	{
public:
	CWapClientMtmFindTest() : 
		iSession(NULL), iDummyObserver(NULL){};
	~CWapClientMtmFindTest();
private:
	// From CWapPushTest
	const TDesC& TestName();

	// From CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

	// Private methods
	void SetupFindDataL(CMsvEntry* entry);
	void PerformTests(CBaseMtm* aWapPushClientMtm);
	void TestAndPrint(
		CBaseMtm* aWapPushClientMtm, 
		const TDesC& aTestText, 
		TMsvPartList aResultList);

private:
	CMsvSession* iSession;
	CDummyObserver* iDummyObserver;
	TMsvId iMessageId;
	};
//------------------------------------------------------------------------------

/** DRM derived test case */
class CWapPushDRMMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
	
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();
	
	void RegRequesttriggerMessageL();
	void ROAquisationTriggerMessageL();
	void JoinDomaintriggerMessageL();
	void LeaveDomainTriggerMessageL();
	void WbXmlROMessageL();
	void InvalidWbXmlROMessageL();
	void XmlROMessageL();
	void InvalidXmlROMessageL();
	void LargeXmlROMessageL();
	void LargeWbXmlROMessageL();
	void InvalidRegRequesttriggerMessageL();
	void InvalidROAquisationTriggerMessageL();
	void InvalidJoinDomaintriggerMessageL();
	void InvalidLeaveDomainTriggerMessageL();
	private :
		HBufC8* iMsg;
	};


#endif
