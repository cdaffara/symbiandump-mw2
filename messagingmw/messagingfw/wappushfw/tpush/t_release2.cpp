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
// T_SLMESSAGE.CPP
// 
//

#include "pushtests.h"

#include "dummywapstack.h"
#include "pushcpoint.h"

_LIT(KTextDoingStuff,"Doing some stuff");
_LIT(KTextFinished,"All finished");

void CWapPushR2Test::RunL()
	{
	switch (iState)
		{
	case EInitialisation:
		{
		// Create WAP Stack passing MDummyWapStackObserver derived instance
		// Create Connection Manager, once WAP stack setup
		iWapStack = CDummyWapStack::NewL(*this);
		iConnMan =  CConnectionManager::NewL(*this);
		MoveToNextState();
		return;
		}
	case ESendSIA:
		{
		// call the connection manager export which creates a
		// connection-orientated connection.
 		_LIT8(KPushR2RemoteHost,"129.0.0.1");
 		TPushConnPoint cp;
 		cp.iHost=KPushR2RemoteHost;
		cp.iPort = KPushPortSecure;  //use secure port directly
 		iConnMan->CMOpenConnectionL(cp);		
		iResponseState=ESessionConfirm;
		MoveToNextState();
		iAck = EFalse;
		return;
		}
	case EWaitForAcknowledge:
		{	
		if (iAck)
			MoveToNextState();
		return;
		}
	case EFinished:
		{
		Printf(KTextFinished);
		SetPriority(EPriorityIdle);
		MoveToNextState();
		return;
		}
	default:
		// Finished
		iEngine->TestCompleted(iStatus.Int());
		return;
		}
	};


CWapPushR2Test::~CWapPushR2Test()
	{
	delete iConnMan;
	delete iWapStack;
	}	

void CWapPushR2Test::DoCancel()
	{
	delete iConnMan;
	delete iWapStack;
	iEngine->TestCompleted(KErrCancel);
	}

void CWapPushR2Test::MoveToNextState()
	{ 
	iState = (TState)(iState+1); 
	CompleteOwnRequest();
	}

void CWapPushR2Test::CompleteOwnRequest()
	{
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);	
	SetActive();
	}

const TDesC& CWapPushR2Test::TestName()
	{
	_LIT(KTextSLMessageTest,"Release Two");
	return KTextSLMessageTest;
	}


/** 
	called by DummyWapStack and returns the push message
	@param CDummyWSPCLConn
	@return CDummyWSPCLConn with WAP Push message
*/
void CWapPushR2Test::DWSOUnitWaitPushL(CDummyWSPCLConn& /*aDummyCLConn*/)	
	{
	}

/**
	display error message
	@param	string 
	@param	line number
*/
void CWapPushR2Test::DWSOError(const TDesC& aDes, TInt /*aLineNum*/)
	{
	iEngine->Printf(TestName(),aDes);
	}

/**
	Wap Stack Server closing...
*/
void CWapPushR2Test::DWSOServerShutDown()
	{
	// TO DO: 
	iEngine->Printf(TestName(), _L(" ServerShutDown called"));
	}

/**
	Cancel pushwait operation
*/
void CWapPushR2Test::DWSOCancelUnitWaitPush(CDummyWSPCLConn& /*aDummyCLConn */)
	{
	}	

void CWapPushR2Test::DWSOConnect(CDummyWSPCOConn& /*aDummyCOConn*/)
	{
	}

void CWapPushR2Test::DWSOGetEventL(CDummyWSPCOConn& aDummyCOConn)
	{
	// only go through once
	if (!iAck)
		{
		RWSPCOConn::TEvent event;
		CDummyWSPCOTrans* dummyTransaction=NULL; 
		switch (iResponseState)
			{
			case CWapPushR2Test::ESessionConfirm:
				dummyTransaction = new (ELeave) CDummyWSPCOTrans(aDummyCOConn);
				event=RWSPCOConn::EConnect_cnf_s;
				iResponseState=CWapPushR2Test::ETransaction;
				aDummyCOConn.CompleteGetEventL(event,*dummyTransaction);
				break;
			case CWapPushR2Test::ETransaction:
				dummyTransaction = new (ELeave) CDummyWSPCOTrans(aDummyCOConn);
				CreateMultiMixedPushMsg(); //Create Hard coded test Push Msg Body and Header
				dummyTransaction->SetData(iHeadersBuf, RWSPCOTrans::EPushHeaders);
				dummyTransaction->SetData(iBodyBuf, RWSPCOTrans::EPushBody);

				event=RWSPCOConn::EConfirmedPush_ind_t;
				aDummyCOConn.CompleteGetEventL(event,*dummyTransaction);
				break;
			default:
				dummyTransaction = new (ELeave) CDummyWSPCOTrans(aDummyCOConn);
				event=RWSPCOConn::EConfirmedPush_ind_t;
				aDummyCOConn.CompleteGetEventL(event,*dummyTransaction);
				break;
			}
		}
	}

void CWapPushR2Test::DWSOCancelGetEvent(CDummyWSPCOConn& aDummyCOConn)
	{
	// only go through once
	if (!iAck)
		aDummyCOConn.CancelGetEvent();
	}

void CWapPushR2Test::DWSOAckTransaction(CDummyWSPCOTrans& /*aDummyCOTrans*/)
	{
	// only go through once
	if (!iAck)
		{
		iAck = ETrue;
		CompleteOwnRequest();
		}
	}

/**
 * Hardcoded Creation of a Test Push Message
 *  -Todo Need to be Modified to use different
 *    Push messages.
 */
void CWapPushR2Test::CreatePushMsg()	
	{

	// create hardcoded SL test message (Julia's code)
	const TUint8 KMessageHeader2[] = 
		{
		0xAF,  //Content type "application/vnd.wap.sl" 
		0x92,  // Date
		0x04,  // length of field
		0x39,  // Monday 15 August 2000 10:41:37 GMT
		0x90,	
		0x6a,
		0xd0,
		0x94,  // Expires 
		0x04,  // length of field
		0x39,  // Monday 15 August 2000 10:41:37 GMT
		0x9a,	
		0x6a,
		0xd0,
		0xAF,  //X-Wap-Application-ID
		0x82  // Short int - 0x02 represent x-wap-application:push.ua
		};	


	TBuf8<40> tempBuf;
	TUint8 value;
	
	for (TInt i = 0; i < 15; i++)
		{
		value = KMessageHeader2[i];
		tempBuf.Append(value);
		}

	iHeadersBuf.Copy(tempBuf);
	tempBuf.Copy(_L8(""));


  	_LIT(LF, "\n");
  	//Hard coded SL Text Msg
  	_LIT(KSL1stLine,"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>");
  	_LIT(KSL2ndLine,"<!DOCTYPE sl PUBLIC \"-//WAPFORUM//DTD SL 1.0//EN\"");
  	_LIT(KSL3rdLine,"\"http://www.wapforum.org/DTD/sl.dtd\">");
  	_LIT(KSL4thLine,"<sl");
  	_LIT(KSL5thLine,"href=\"http://www.xyz.com/ppaid/123/abc.wml\"");
  	_LIT(KSL6thLine,"action=\"execute-high\">");
  	_LIT(KSL7thLine,"</sl>");
   
  	iBodyBuf.Append(KSL1stLine);
  	iBodyBuf.Append(LF);
  	iBodyBuf.Append(KSL2ndLine);
  	iBodyBuf.Append(LF);
  	iBodyBuf.Append(KSL3rdLine);
  	iBodyBuf.Append(LF);
  	iBodyBuf.Append(KSL4thLine);
  	iBodyBuf.Append(LF);
  	iBodyBuf.Append(KSL5thLine);
  	iBodyBuf.Append(LF);
  	iBodyBuf.Append(KSL6thLine);
  	iBodyBuf.Append(LF);
  	iBodyBuf.Append(KSL7thLine);
	}

/* Create a hard code Mulipart Push message.
 * The Multipart Push Message Contains:
 *		- 1. Sl
 *		- 2. Slc
 *		- 3. Si
 *		- 4. Sic
 *		- 5. vCard
 */
void CWapPushR2Test::CreateMultiMixedPushMsg()	
	{
	// The Multipart Header
	const TUint8 KMessageHeaderMultipart[]=
		{
		0xA3,  //Content type "application/vnd.wap.multipart.mixed"
		0x94,  // Date
		0x04,  // length of field
		0x35,  // Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0xAF,  //X-Wap-Application-ID
		0x82,  // Short int - 0x02 represent x-wap-application:push.ua
		};
	
	iHeadersBuf.Copy(KMessageHeaderMultipart, sizeof(KMessageHeaderMultipart));
	
	// Number of parts
	const TUint8 KNumParts[] = {0x05};
	iBodyBuf.Copy(KNumParts, sizeof(KNumParts));
	
	
	//----- Push Message Part No.1 ---------
	// SL Header with multipart code
	const TUint8 KSLMultiHeader1[]=
		{
		0x0A,	// part 1 header size
		0x81,	// part 1 body size (208) UintVar 10000001
		0x50,	// 01010000
		0xAF,	// content type application/vnd.wap.sl ... the headers:
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11
	};

	// SL Body
  	const TUint8 KSLMultiBody1[] = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"	\
  					"<!DOCTYPE sl PUBLIC \"-//WAPFORUM//DTD SL 1.0//EN\"\n"	\
					"\"http://www.wapforum.org/DTD/sl.dtd\">\n"	\
					"<sl\n"	\
					"href=\"http://www.xyz.com/ppaid/123/abc.wml\"\n"	\
					"action=\"execute-high\">\n"	\
					"</sl>";

	
	iBodyBuf.Append(KSLMultiHeader1, sizeof(KSLMultiHeader1));
	iBodyBuf.Append(KSLMultiBody1, sizeof(KSLMultiBody1)-1);


	//----- Push Message Part No.2 ---------
	// Slc Message with Multipart Code
	const TUint8 KSLCMultipart2[]=
		{
		0x0A,	// part 2 header size
		0x1D,	// part 2 body size
		0xB0,	// content type application/vnd.wap.slc ... the headers:
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11,
		0x01,	// Body - slc (www.schmookie.com)
		0x06,
		0x6A,
		0x00,
		0x85,
		0x09,
		0x03,
		0x6D,
		0x6D,
		0x6D,
		0x2E,
		0x73,
		0x63,
		0x68,
		0x6D,
		0x6F,
		0x6F,
		0x6B,
		0x69,
		0x65,
		0x2E,
		0x63,
		0x6F,
		0x64,
		0x65,
		0x2F,
		0x00,
		0x05, // [0x05, //xecute-low]  [0x06, //execute-high] [//0x07, //cache]
		0x01
	};

	iBodyBuf.Append(KSLCMultipart2, sizeof(KSLCMultipart2));


//----- Push Message Part No.3 ---------
	// Si Message with Multipart Code
	const TUint8 KSIMultiHeader3[]=
		{
		0x0A,	// part 3 header size
		0x82 ,	// part 3 body size (334) UintVar 10000010
		0x4E,	// 01001110
		0xAD,	// content type application/vnd.wap.si ... the headers:
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11
	};
	
	// Si Body
  	const TUint8 KSIMultiBody3[] = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"	\
										"<!DOCTYPE si PUBLIC \"-//WAPFORUM//DTD SI 1.0//EN\"\n"	\
										"\"http://www.wapforum.org/DTD/si.dtd\">\n"	\
										" <si>\n" \
										" <indication\n" \
										"	action=\"signal-none\"\n"	\
										"	href=\"http://mmm.ppp.qqq/\"\n"	\
										"	si-id=\"123123\"\n"	\
										"	created=\"2000-08-29T16:46:03Z\"\n"	\
										"	si-expires=\"2000-08-30T16:46:03Z\"\n"	\
										"	>\n"	\
										"	This is displayed text.\n"	\
										" </indication>\n"	\
										"</si>	\n";

	iBodyBuf.Append(KSIMultiHeader3, sizeof(KSIMultiHeader3));
	iBodyBuf.Append(KSIMultiBody3, sizeof(KSIMultiBody3)-1);


//----- Push Message Part No.4 ---------
	// Sic Message with Multipart Code
	const TUint8 KSiCMultipart4[]=
		{
		0x0A,	// part 2 header size
		0x64,	// part 2 body size 
		0xAE,	// content type application/vnd.wap.sic ... the headers:
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11, 
		0x01, 0x05, 0x6A, 0x12, 0x65, 0x6D, 0x61, 0x69,  0x6C, 0x2F, 0x31, 0x32, 0x33, 0x2F, 0x61, 0x62,
		0x63, 0x2E, 0x77, 0x6D, 0x6C, 0x00, 0x45, 0xC6,  0x05, 0x0D, 0x03, 0x78, 0x79, 0x7A, 0x00, 0x85,
		0x83, 0x00, 0x11, 0x03, 0x68, 0x74, 0x74, 0x70,  0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E, 0x78,
		0x79, 0x7A, 0x00, 0x85, 0x83, 0x00, 0x0A, 0xC3,  0x07, 0x19, 0x99, 0x06, 0x25, 0x15, 0x23, 0x15,
		0x10, 0xC3, 0x04, 0x19, 0x99, 0x06, 0x30, 0x01,  0x03, 0x20, 0x59, 0x6F, 0x75, 0x20, 0x68, 0x61,
		0x76, 0x65, 0x20, 0x34, 0x20, 0x6E, 0x65, 0x77,  0x20, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x73, 0x2E,
		0x20, 0x00, 0x01, 0x01};

	iBodyBuf.Append(KSiCMultipart4, sizeof(KSiCMultipart4));

	//----- Push Message Part No.5 ---------
	// vCard Message 

	// vCard Message with Multipart Code
	const TUint8 KvCardMultiHeader5[]=
		{
		0x0A,	// part 3 header size
		0x83,	// part 3 body size (496) UintVar 10000011
		0x70,	// 01110000 
		0x87,	// content type "text/x-vCard" 
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11
	};

	// vCard Body
  	const TUint8 KvCardMultiBody5[] =	"BEGIN:VCARD\r\n" \
										"VERSION:2.1\r\n" \
										"N:Wibble;Barney\r\n" \
										"FN:Barney Wibble\r\n" \
										"ORG:WibCo\r\n" \
										"TITLE:Wib Monitor\r\n" \
										"TEL;WORK;VOICE:+44 012345345\r\n" \
										"TEL;HOME;VOICE:+44 12143234\r\n" \
										"ADR;WORK;ENCODING=QUOTED-PRINTABLE:;;23 The Wibble=0D=0AWibble Street;WibbleHampton;Wibbleshire;;United Kingdom\r\n" \
										"LABEL;WORK;ENCODING=QUOTED-PRINTABLE:23 The Wibble=0D=0AWibble Street=0D=0AWibbleHampton, Wibbleshire=0D=0AUnited=Kingdom\r\n" \
										"URL:http://www.wibbleweb.wib\r\n" \
										"EMAIL;PREF;INTERNET:barney.wibble@wibco.co.uk\r\n" \
										"REV:20000322T104543Z\r\n" \
										"END:VCARD\r\n";


	iBodyBuf.Append(KvCardMultiHeader5, sizeof(KvCardMultiHeader5));
	iBodyBuf.Append(KvCardMultiBody5, sizeof(KvCardMultiBody5)-1);

	//-- end of Mulitipart message ---
	}
