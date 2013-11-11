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

#include "pushtests.h"

#include "dummywapstack.h"
#include "pushcpoint.h"


//----------------------------------------------------------------------------------------
// CTestTimer
//----------------------------------------------------------------------------------------


_LIT(KTextFinished,"All finished");

void CWapPushR3Test::RunL()
	{
	switch (iState)
		{
	case EInitialisation:
		{
		iSiaMsgPushed=EFalse;
		//Create Hard coded Sia Push Msg Body and Header
		CreateClSecureSiaPushMsgL();

		// Create WAP Stack passing MDummyWapStackObserverderived instance
		// Create Connection Manager, once WAP stack setup
		iWapStack = CDummyWapStack::NewL(*this);
		iConnMan =  CConnectionManager::NewL(*this);
		iTimer = CTestTimer::NewL(this);
		MoveToNextState();
		return;
		}

	case EDWSOGetEventInitialisation:
		{
		iResponseState=ESessionConfirm;
		iAck = EFalse;
		MoveToNextState();
		return;
		}

	case EWaitForAcknowledge:
		{
		if (iAck)
			{
			iTimer->Cancel();
			}
		else
			{
			TInt KTimeOut = 500000;
			// If nothing happens after KTimeOut... get out of here.
			iTimer->After(KTimeOut);
			}
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

void CWapPushR3Test::Timeout()
	{
	iState = EFinished;
	CompleteOwnRequest();
	}


CWapPushR3Test::~CWapPushR3Test()
	{
	delete iConnMan;
	delete iWapStack;
	delete iTimer;
	}	

void CWapPushR3Test::DoCancel()
	{
	delete iConnMan;
	delete iWapStack;
	iEngine->TestCompleted(KErrCancel);
	}

void CWapPushR3Test::MoveToNextState()
	{
	iState = (TState)(iState+1);
	CompleteOwnRequest();
	}

void CWapPushR3Test::CompleteOwnRequest()
	{
	if(!IsActive())
		{
		TRequestStatus* stat = &iStatus;
		User::RequestComplete(stat,KErrNone);	
		SetActive();
		}
	}

const TDesC& CWapPushR3Test::TestName()
	{
	_LIT(KTextSLMessageTest,"Release Three");
	return KTextSLMessageTest;
	}


/** 
	Called by DummyWapStack and returns the push message body
	and header.
	-Push msg is pushed to the DummyWapStack only for the 
    case CL Secure, and it is pushed only once.
	-For the case of CL Unsecure no push msg is returned.

	@param aDummyCLConn
	@return iBodyBuf
	@return  iHeadersBuf
*/
void CWapPushR3Test::DWSOUnitWaitPushL(CDummyWSPCLConn& aDummyCLConn)	
	{
	//if unsecure CL then No Push Message is returned
	if(aDummyCLConn.LocalPort()== KPushPortSecure)
		if(!iSiaMsgPushed)
			{
			aDummyCLConn.CompleteUnitWaitPushL(iBodyBuf,iHeadersBuf);
			iSiaMsgPushed=ETrue;
			}
	}

/**
	display error message
	@param	string
	@param	line number
*/
void CWapPushR3Test::DWSOError(const TDesC& aDes, TInt /*aLineNum*/)
	{
	iEngine->Printf(TestName(),aDes);
	}

/**
	Wap Stack Server closing...
*/
void CWapPushR3Test::DWSOServerShutDown()
	{
	// TO DO: 
	iEngine->Printf(TestName(), _L(" ServerShutDown called"));
	}

/**
	Cancel pushwait operation
*/
void CWapPushR3Test::DWSOCancelUnitWaitPush(CDummyWSPCLConn& /*aDummyCLConn */)
	{
	iEngine->Printf(TestName(), _L(" DWSOCancelUnitWaitPush called"));
	}	

void CWapPushR3Test::DWSOConnect(CDummyWSPCOConn& /*aDummyCOConn*/)
	{
	}

void CWapPushR3Test::DWSOGetEventL(CDummyWSPCOConn& aDummyCOConn)
	{
	// only go through once
	if (!iAck)
		{
		iTimer->Cancel();
		RWSPCOConn::TEvent event;
		CDummyWSPCOTrans* dummyTransaction=NULL; 
		switch (iResponseState)
			{
			case CWapPushR3Test::ESessionConfirm:
				dummyTransaction = new (ELeave) CDummyWSPCOTrans(aDummyCOConn);
				event=RWSPCOConn::EConnect_cnf_s;
				iResponseState=CWapPushR3Test::ETransaction;
				aDummyCOConn.CompleteGetEventL(event,*dummyTransaction);
				break;
			case CWapPushR3Test::ETransaction:
				CreateMultiPartRelatedPushMsg();//Creates a New Push Msg
				//CreateSLPushMsg(); //Test 
				dummyTransaction = new (ELeave) CDummyWSPCOTrans(aDummyCOConn);
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

void CWapPushR3Test::DWSOCancelGetEvent(CDummyWSPCOConn& /*aDummyCOConn*/)
	{
	}

void CWapPushR3Test::DWSOAckTransaction(CDummyWSPCOTrans& /*aDummyCOTrans*/)
	{
	// only go through once
	if (!iAck)
		{
		iAck = ETrue;
		CompleteOwnRequest();
		}
	}

/**
 * Hardcoded Creation of an SL  Test Push Message
 */
void CWapPushR3Test::CreateSLPushMsg()	
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
   
	iBodyBuf.Delete(0,KBufferSize);
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

/* Create a hard coded Mulipart Related Push message.
 * The Multipart Push Message Contains:
 *		- 1. Si
 *		- 2. vCard
 *		- 3. JPEG
 */
void CWapPushR3Test::CreateMultiPartRelatedPushMsg()	
	{
	// The Multipart Header
	const TUint8 KMessageHeaderMultipart[]=
		{
		0xB3,  //Content type "application/vnd.wap.multipart.related"
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
	const TUint8 KNumParts[] = {0x03};
	iBodyBuf.Copy(KNumParts, sizeof(KNumParts));
	
	
	//----- Push Message Part No.1 ---------
	// Si Message with Multipart Code
	const TUint8 KSIMultiHeader1[]=
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
  	const TUint8 KSIMultiBody1[] = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"	\
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


	iBodyBuf.Append(KSIMultiHeader1, sizeof(KSIMultiHeader1));
	iBodyBuf.Append(KSIMultiBody1, sizeof(KSIMultiBody1)-1);

	//----- Push Message Part No.2 ---------
	// vCard Message 
	const TUint8 KvCardMultiHeader2[]=
		{
		0x0A,	// part 3 header size
		0x83,	// part 3 body size (496) UintVar 10000011
		0x70,	// 0111000 
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
  	const TUint8 KvCardMultiBody2[] =	"BEGIN:VCARD\r\n" \
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


	iBodyBuf.Append(KvCardMultiHeader2, sizeof(KvCardMultiHeader2));
	iBodyBuf.Append(KvCardMultiBody2, sizeof(KvCardMultiBody2)-1);



//----- Push Message Part No.3 ---------
	// JPEG Message Part 
	const TUint8 KJpegCMultipart3[]=
		{
		0x0A,	// part 2 header size
		0x64,	// part 2 body size 
		0x9E,	// content type "image/jpeg" 
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

	iBodyBuf.Append(KJpegCMultipart3, sizeof(KJpegCMultipart3));

	//-- end of Mulitipart Related Push Message ---
	}


/**
 * Creates a ConnectionLess Secure SIA Push Message.
 */
void CWapPushR3Test::CreateClSecureSiaPushMsgL()
	{
//	SetupAddressL();

	const TUint8 KSIAMessageHeader[] = 
		{
		0xB4,	//Content type "application/vnd.wap.sia" 
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
	iHeadersBuf.Append(KSIAMessageHeader, sizeof(KSIAMessageHeader));
	
	iBodyBuf.Delete(0,KBufferSize);

	const TUint8 SIA[]=
		{
		0x00,		//	version
		0x02,		//	AppIdListLen
		0x44,		//	Application Id List 
		0x44,			
		0x1B,		// ContactPointsLen
		// Contact Points
		0xD7,		//	bearer flag and port flag and address len 
		0x0A,		//	bearer Type (CSD)
		0x0B,		//	port number
		0x84,		//		= 2948
		0x68,		// address - "http://www.symbian.com/" (as text)
		0x74,		
		0x74,
		0x70,
		0x3A,
		0x2F,
		0x2F,
		0x77,
		0x77,		
		0x77,
		0x2E,
		0x73,
		0x79,
		0x6D,
		0x62,
		0x69,
		0x61,
		0x6E,
		0x2E,
		0x63,
		0x6F,
		0x6D,
		0x2F
		};
	iBodyBuf.Append(SIA, sizeof(SIA));
	}


/**
 *	Prime the commdb with address and security port settings.
 *	
 */
void CWapPushR3Test::SetupAddressL()
	{
	}
